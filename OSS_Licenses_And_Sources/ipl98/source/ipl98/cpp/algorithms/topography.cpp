#include "topography.h"
#include "morphology.h"
#include "statistic.h"
#include <cmath>
#include <fstream>

namespace ipl{

const short CTopography::DYNAMIC_N = -1;

/*** CTopography **************************
 * The constructor. Sets up different values, including
 * default values for thresholds, and precalculates all
 * possible value for the h's.
 *********************************************************/
CTopography::CTopography() 
{
	// Default values for thresholds (parameters) for the classification proces
	m_zeroLim_1deriv = 5.5;
	m_zeroLim_eig = 2.5;
	m_zeroLim_dirDeriv = 5.5;

	m_maxN = 9;
	m_minN = 2;
	m_background_threshold = 65;

	m_min0 = 3; m_min1 = 3; m_min2 = 3; m_min3 = 3;
	m_min4 = 4; m_min5 = 4; m_min6 = 4;
	m_min7 = 5; m_min8 = 5; m_min9 = 5;

	// Calculation of h's
	float h[10][3][20];
	int n; // edr: declare variable outside loop
	for(n=2; n < 10; n++) 
	{
		for(int i=-n; i <= n; i++)
		{
			// reduced expressions for h
			h[n][0][i+n] =  (float)(( 3+15*pow(i,2)-9*n*(n+1) ) / 
							( 3-2*n*(4*pow(n,2)+6*n-1) ));
	
			h[n][1][i+n] = (float)(( 5*i*( 7*pow(i,2)*(3*n*(n+1)-1) -15*pow(n,3)*(n+2)+15*n-5 ) ) / 
						   ( (n-1)*n*(n+1)*(n+2)*(2*n-1)*(2*n+1)*(2*n+3) ));	
	
			h[n][2][i+n] = (float)(( 30*(3*pow(i,2)-n-pow(n,2)) ) / 
						   ( n*(n+1)*(2*n-1)*(2*n+1)*(2*n+3)));
		}
	}

	// Pre-compute products h(i,p)*h(j,q) (i,j = [-9,.., 9] and p,q = [0,1,2])
	for(n=2; n < 10; n++) {
		for(int p = 0; p < 3; p++) {
			for(int q = 0; q < 3; q++) {
				for(int i=-n; i <= n; i++)  {
					for(int j=-n; j <= n; j++)
						m_h[n][p][q][i+n][j+n] = h[n][p][i+n]*h[n][q][j+n];	
				}
			}
		}
	}
	m_initialized = false;
	m_deriveTopography = false;
	m_findCharRegions = false;
}


/*** Initialize *******************************************
 * Initializes various settings. Including setting up
 * the considered image various memory allocation.
 *********************************************************/
bool CTopography::Initialize(CImage& img) 
{
	if(img.GetBits() > 8) 
		return false; // Must be a grayscale image
	
	m_image = img;

	m_TFG.clear();
	m_missingRelations.Empty();
	m_regionCounter = 0;


	m_imageScanLines.clear();	// ('reserve' gives an exception when a new image is loaded)

	// Get row iterators of the image
	for(UINT32 rowNr = 0; rowNr < m_image.GetHeight(); rowNr++) // edr: rowNr changed from int to UINT32
		m_imageScanLines.push_back(m_image.ConstRow(rowNr));

	// Get iterator to the vector with image row iterators
	m_imageScanLines_p = m_imageScanLines.begin();
	
	// Allocate mem for gradients and classification labels
	bool success = true;
		
	success &= m_gradient.Alloc(m_image.GetWidth(), m_image.GetHeight());
	success &= m_TCG.Alloc(m_image.GetWidth(), m_image.GetHeight());
	
	if(!success) 
	{
		ostringstream ost;
		ost << "CTopography: Could not allocate mem" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}	
	m_initialized = true;
	m_deriveTopography = false;
	m_findCharRegions = false;
	return true;
}


/*** DeriveTopographicClassification *********************
 * Sets up the topographical classification according to
 * the selected value of N, and precalculates gradients.
 * Finally it finds the topographical label for each
 * pixel in the considered image.
 *********************************************************/
bool CTopography::DeriveTopographicClassification(int N) 
{
	if( (N < -1) || (N == 0) || (N == 1) || (N > 9))
	{
		ostringstream ost;
		ost << "CTopography::ComputePixelLabel() N=" << N << " out of range [2;9]" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	m_N = N;

	if (!m_initialized)	
	{
		ostringstream ost;
		ost << "CTopography: Not initialized" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	
	float H[2][2];
	float eigVal0, eigVal1;
	CVector2D<float> eigVec0, eigVec1;
	int w, h;
	
	
	// Using dynamic determination of N
	if(m_N == DYNAMIC_N)
	{
		m_dynamicN = true;
		MakeDistanceToBackgroundTable();	// Calc. min. distance to background in each pixel
	}
	else
		m_dynamicN = false;
	

	// Make sure that value of m_N and m_maxN is initialized correctly
	if(m_dynamicN)
		m_N = m_maxN;
	else
		m_maxN = m_N;

	w = m_image.GetWidth();
	h = m_image.GetHeight();
	m_maxX = w - m_N-1;
	m_maxY = h - m_N-1;

	
	// Calculation of gradients
	int x; // edr: declare variable outside loop
	for(x = m_maxN; x < m_maxX; x++) {
		for(int y = m_maxN; y < m_maxY; y++) {
			if(m_dynamicN) m_N = m_distanceToBackground[x][y];	

			ComputeGrad(x,y,m_gradient[x][y]);
		}
	}

	
	// Classification of pixels
	for(x = 0; x < w; x++) {
		for(int y = 0; y < h; y++) {
			// Don't classify pixels on the border
			if( (x < m_maxN || x >= m_maxX) || (y < m_maxN || y >= m_maxY) )
				m_TCG[x][y] = UNTREATED;	
			
			else 
			{
				// Get computed optimal value for N in the pixel
				if(m_dynamicN) m_N = m_distanceToBackground[x][y];
				
				ComputeHessian(x,y,H);
				ComputeEigenVal(H,eigVal0,eigVal1);
				
				if( m_gradient[x][y].IsZero() ) 
				{
					ZeroThresholdEig(eigVal0, eigVal1);	
					ClassifyPixelD(x,y,eigVal0,eigVal1);
				}
				
				else 
				{
					ComputeEigenVec(H, eigVal0, eigVal1, eigVec0, eigVec1);	
					if( ZeroThresholdEig(eigVal0, eigVal1) )
					{
						::ipl::Swap(eigVec0, eigVec1);
					}

					ClassifyPixelE(x,y,eigVal0, eigVal1, eigVec0, eigVec1);
				}
			}
		}
	}
	m_deriveTopography= true;
	return true;
}



/*** PaintTopographicLabels *******************************
 * Draws the topographic label of every pixel in m_image
 * If the bool 'all' is 'true', then every type is drawn
 * If 'all' is 'false', then only most important are
 * considered.
 *********************************************************/

bool CTopography::PaintTopographicLabels(CStdImage& Img,int DrawPixelLabel) const
{

	if (!m_findCharRegions)	
	{
		ostringstream ost;
		ost << "CTopography: The characteristic regions are yet to be extracted" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}

	int w = m_image.GetWidth();
	int h = m_image.GetHeight();
	int m_maxX = w - m_N-1;
	int m_maxY = h - m_N-1;
	
	Img=m_image;
	Img.CopyConvert(24,Img);

	for(int x = m_N; x < m_maxX; x++) {
		for(int y = m_N; y < m_maxY; y++) {
			if((m_TCG[x][y].GetType() == PEAK)  && (DrawPixelLabel & PEAK))
				Img.SetPixel(x,y,CPalette::CreateRGB(0,255,255)); // turkIs
			else if((m_TCG[x][y].GetType() == SADDLE) && (DrawPixelLabel & SADDLE))
				Img.SetPixel(x,y,CPalette::CreateRGB(255,0,0));
			else if((m_TCG[x][y].GetType() == RIDGE) && (DrawPixelLabel & RIDGE))
				Img.SetPixel(x,y,CPalette::CreateRGB(0,255,0));
			else if((m_TCG[x][y].GetType() == FLAT) && (DrawPixelLabel & FLAT))
				Img.SetPixel(x,y,CPalette::CreateRGB(192,192,192));
			else if(m_TCG[x][y].GetType() == RAVINE && (DrawPixelLabel & RAVINE))	
				Img.SetPixel(x,y,CPalette::CreateRGB(0,0,192));
			else if(m_TCG[x][y].GetType() == PIT && (DrawPixelLabel & PIT)) 
				Img.SetPixel(x,y,CPalette::CreateRGB(192,0,192));
			else if(m_TCG[x][y].GetType() == HILLSIDE && (DrawPixelLabel & HILLSIDE))  
				Img.SetPixel(x,y,CPalette::CreateRGB(192,192,0)); //HillSide
			
		}
	}
	return true;
}


/*** ChangeLimitValues ************************************
 * Is used to change the values of the thredsholds, which
 * is used to check if a value is eaual to zero, when
 * computational errors are taken into account.
 *********************************************************/
bool CTopography::ChangeLimitValues(float zeroLim_1deriv, 
	float zeroLim_eig, float zeroLim_dirDeriv)
{
	if( (zeroLim_1deriv < 0) || (zeroLim_eig < 0) || (zeroLim_dirDeriv < 0))
	{
		ostringstream ost;
		ost << "CTopography: The limits should be non-negative" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	m_zeroLim_1deriv = zeroLim_1deriv;
	m_zeroLim_eig = zeroLim_eig;
	m_zeroLim_dirDeriv = zeroLim_dirDeriv;
	return true;
}

/*** ChangeBackgroundValue ********************************
 * Is used to change the value of the threshold for the background color
 * (Used for dynamic neighborhood size N)
 *********************************************************/
bool CTopography::ChangeBackgroundValue(int backThreshold) 
{
	if( (backThreshold < 0) || (backThreshold > 255) )
	{
		ostringstream ost;
		ost << "CTopography::ChangeBackgroundValue() backThreshold=" << backThreshold << " out of range [0;255]" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	m_background_threshold = backThreshold;
	return true;
}


/*** ThinRidgeLines ***************************************
 * Slims down the areas consisting of ridges, resulting
 * in areas with a smaller width.
 *********************************************************/
bool CTopography::ThinRidgeLines() 
{
	if (!m_deriveTopography)	
	{
		ostringstream ost;
 		ost << "CTopography: Topographic derivatives has not been computed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}

	CImage ridges_image(m_TCG.GetWidth(),m_TCG.GetHeight(),1,1);
	vector<T2DInt> ridge_coords;

	for(int x = 0; x < (int)m_TCG.GetWidth();x++) { // edr: cast GetWidth() to int
		for(int y = 0; y < (int)m_TCG.GetHeight();y++) { // edr: cast GetHeight() to int
			if(m_TCG[x][y].GetType() == RIDGE) 
			{
				T2DInt ridge_coord;
				ridge_coord.x = x;	
				ridge_coord.y = y;
				ridge_coords.push_back(ridge_coord);
				ridges_image.SetPixelFast(x,y,0);
			}
		}
	}

	// Thin ridge-lines
	CMorphology::SkeletonZhou(ridges_image,true);
	
	// Assign removed ridge-pixels a new label
	TC_FEATURETYPE type;
	for(unsigned int i = 0; i < ridge_coords.size(); i++) { // edr: changed i from int to unsigned int
		// Ridge-pixel har been removed
		if(ridges_image.GetPixelFast(ridge_coords[i].x, ridge_coords[i].y) == 1) 
		{ 
			// Check 8 neighbour pixels
			for(int dx = -1; dx < 2; dx++) { 
				for(int dy = -1; dy < 2; dy++) {
					type = m_TCG[ridge_coords[i].x+dx][ridge_coords[i].y+dy].GetType(); 
					// (Ridge is situated near flats og hillsides)
					if( type == HILLSIDE || type == FLAT) 
					{
						// Change topografic label
						m_TCG[ridge_coords[i].x][ ridge_coords[i].y].SetType(type); 
						break;
					}
				}
			}
		}
	}
	ridges_image.Save("Thinned_image.bmp");
	return true;
}




/*** ChangeDynamicValues ********************************
 * Is used to change values in refinement of dynamic N
 *********************************************************/
bool CTopography::ChangeDynamicValues(int min0, int min1,
			int min2, int min3,int min4,int min5,int min6,int min7,int 
			min8,int min9)
{
	if( (m_min0 < 2) || (m_min0 > 9) || (m_min1 < 2) || (m_min1 > 9)|| (m_min2 < 2) || (m_min2 > 9)
		|| (m_min3 < 2) || (m_min3 > 9)|| (m_min4 < 2) || (m_min4 > 9)|| (m_min5 < 2) || (m_min5 > 9)
		|| (m_min6 < 2) || (m_min6 > 9)|| (m_min7 < 2) || (m_min7 > 9)|| (m_min8 < 2) || (m_min8 > 9)
		|| (m_min9 < 2) || (m_min9 > 9)) {
		ostringstream ost;
		ost << "CTopography: The values should all be between 2 and 9" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}

	m_min0 = min0;m_min1 = min1;m_min2 = min2;m_min3 = min3;m_min4 = min4;
	m_min5 = min5;m_min6 = min6;m_min7 = min7;m_min8 = min8;m_min9 = min9;
	return true;
}

/*** MakeDistanceToBackgroundTable ************************
 * Calculates the minimum distance for the each pixel to
 * the background and uses them to give an value for N
 * to each pixel. There are saved in an 2d-array
 *********************************************************/
void CTopography::MakeDistanceToBackgroundTable() 
{
	//'-2' because derivatives of neighbour pixels may e
	// calc. under classification
	int xRun,yRun;
	m_maxX = m_image.GetWidth()- m_maxN-1;
	m_maxY = m_image.GetHeight()- m_maxN-1;
	xRun = 9;
	yRun = 9;
	
	m_distanceToBackground.Alloc(m_maxX, m_maxY);

	// Calculates min. distance to background (not optimized)
	int minFoundDist,absI,absJ;
	for(int x = 0; x < m_maxX; x++) {
		for(int y = 0; y < m_maxY; y++) {
			if((x>9 /*&& x<m_maxX-9*/) && (y>9 /*&& x<m_maxY-9*/)) {
				minFoundDist = 9;
				for(int i = -xRun; i <= xRun; i++) {
					for(int j = -yRun; j <= yRun; j++) {
						if(m_image.GetPixelFast(x+i,y+j) < m_background_threshold) {
							absI = abs(i); 
							absJ = abs(j);
							if(minFoundDist > Min(absI,absJ)) // edr: Changed __min (Visual specific!) to ipl::Min
								minFoundDist = Max(absI,absJ);
						}
					}
				}
				// Finds and saves a value for each pixel
				if (minFoundDist == 0) m_distanceToBackground[x][y] = m_min0;
				else if (minFoundDist == 1) m_distanceToBackground[x][y] = m_min1;
				else if (minFoundDist == 2) m_distanceToBackground[x][y] = m_min2;
				else if (minFoundDist == 3) m_distanceToBackground[x][y] = m_min3;
				else if (minFoundDist == 4) m_distanceToBackground[x][y] = m_min4;
				else if (minFoundDist == 5) m_distanceToBackground[x][y] = m_min5;
				else if (minFoundDist == 6) m_distanceToBackground[x][y] = m_min6;
				else if (minFoundDist == 7) m_distanceToBackground[x][y] = m_min7;
				else if (minFoundDist == 8) m_distanceToBackground[x][y] = m_min8;
				else m_distanceToBackground[x][y] = m_min9;
			}
			else 
				m_distanceToBackground[x][y] = m_minN;
		}
	}
	
	// The array is streamed out to the file 'dist.txt'.
	ofstream out_file("dist.txt");
	out_file << m_distanceToBackground;
}

/*** FindAllRegions ***************************************
 * Runs through all pixels in m_image. When unvisted PEAK
 * or RIDGE is seen, findRegions is called. Ensures that
 * pixels are visited and thereby finds all characteristic
 * regions of importants.
 *********************************************************/
bool CTopography::FindCharacteristicRegions() 
{
	if (!m_deriveTopography)	
	{
		ostringstream ost;
 		ost << "CTopography: Topographic derivatives has not been computed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}

	TC_FEATURETYPE type;
	for(int x = m_N; x < m_maxX; x++) {
		for(int y = m_N; y < m_maxY; y++) {
			type = m_TCG[x][y].GetType();
			if((m_TCG[x][y].IsVisited() == false) && (type == PEAK || type == RIDGE))
				FindRegions(x, y, -1);
			else if (type != SADDLE && type != FLAT)
				m_TCG[x][y].SetVisited();
		}
	}
	MakeRelations();
	m_findCharRegions = true;
	return true;
}

/*** FindRegions *****************************************
 * Returns the characteristic region, that coordinat (x,y)
 * belongs to. Recursively calls itself on all neighbours
 * of importants. Also finds relations between found
 * regions.
 *********************************************************/
void CTopography::FindRegions(int x, int y, int parentIndex)
{
	CPixelGroup stack;
	CCharacteristicRegion newRegion;
	CPoint2D<int> currentPixel;
	int xPos; int yPos;
	//TC_FEATURETYPE type; // edr: not used (reported by Borland C++)

	CCharacteristicRegion tempRegion;
	CCharacteristicRegion* thisRegion;
	CCharacteristicRegion* parent;

	if (m_TCG[x][y].IsVisited() == false)
		stack = FindRegion(x, y, &newRegion);
	m_TFG.push_back(newRegion);

	int index = m_TFG.size()-1;

	while(stack.GetTotalPositions() > 0)
	{
		currentPixel = stack.GetPosition(0);
		stack.RemovePositionFast(0);
		xPos = currentPixel.GetX();
		yPos = currentPixel.GetY();
		//type = m_TCG[xPos][yPos].GetType(); // edr: not used (reported by Borland C++)
		if(m_TCG[xPos][yPos].IsVisited() == false)
			FindRegions(xPos, yPos, index);
		else 
		{
			int temp = m_TCG[xPos][yPos].GetRegion()-1;
			m_missingRelations.AddPosition(index,temp);
		}
	}

	thisRegion = &m_TFG[index];
	parent = &m_TFG[parentIndex];

	if(parentIndex != -1) 
	{
		thisRegion->AddNeighbourRegion(parentIndex);
		parent->AddNeighbourRegion(index);
	}
}


/*** FindRegion ******************************************
 * Finds the characteristic region, that coordinat (x,y)
 * belongs to. Returns a stack of important neighbours.
 *********************************************************/
CPixelGroup CTopography::FindRegion(int x, int y, CCharacteristicRegion* newRegion) 
{	
	CPixelGroup stack;
	newRegion->SetType(m_TCG[x][y].GetType());
	vector<CPixelEncaps> untreated;
	CPixelEncaps currentPixelEncaps;
	CPixelEncaps neighbourPixelEncaps;
	CPixelInformation currentPixelInfo;
	TC_FEATURETYPE currentType;
	TC_FEATURETYPE neighbourType;
	int posX; int posY;
//	CPixelInformation neighbourPixelInfo; // edr: not used
//	CPixelInformation lastPixelInfo; // edr: not used
	int iMin; int iMax; int jMin; int jMax;

	currentPixelEncaps = CPixelEncaps(x,y,m_TCG[x][y],false);
	untreated.push_back(currentPixelEncaps);
	++m_regionCounter;
	
	while(untreated.size() != 0) 
	{
		currentPixelEncaps = untreated.back();
		currentPixelInfo = currentPixelEncaps.GetPixelInfo();
		untreated.pop_back();
		posX = currentPixelEncaps.GetX(); posY = currentPixelEncaps.GetY();
		currentType = currentPixelInfo.GetType();

		// check if pixel inside valid images boundaries
		if ((posX >= m_N && posX <= m_maxX-1) && (posY >= m_N && posY <= m_maxY-1)) 
		{

			iMin = -1;
			iMax = 1;
			jMin = -1;
			jMax = 1;
			if(posX == m_N)
				iMin = 0;
			else if(posX == m_maxX-1)
				iMax = 0;
			if(posY == m_N)
				jMin = 0;
			else if(posX == m_maxY-1)
				jMax = 0;

			// check neighbours
			for(int i = iMin; i <= iMax; i++) {
				for(int j = jMin; j <= jMax; j++) {
					if(i == 0 && j == 0) j++;
					if(m_TCG[posX+i][posY+j].IsVisited() == false) {
						neighbourType = m_TCG[posX+i][posY+j].GetType();
						// if part of region...
						if(neighbourType == currentType) {
							m_TCG[posX+i][posY+j].SetVisited();
							neighbourPixelEncaps = CPixelEncaps(posX+i,posY+j,m_TCG[posX+i][posY+j],true);
							untreated.push_back(neighbourPixelEncaps);
						}
						// if an interesting neighboour type
						else if(neighbourType == PEAK || neighbourType == RIDGE || neighbourType == SADDLE || neighbourType == FLAT)
							stack.AddPositionFast(posX+i,posY+j);
						// else just mark as 'visited'.
						else m_TCG[posX+i][posY+j].SetVisited();
					}
				}
			}
		}

		m_TCG[posX][posY].SetVisited();
		m_TCG[posX][posY].SetRegion(m_regionCounter);
		newRegion->AddToPixelGroup(posX,posY);
	}
	newRegion->SetRegionID(m_regionCounter);
	return stack;
}


/*** MakeRelations ****************************************
 * This method make the relations that couldn't be made
 * in findRegions. This is called as the last thing in
 * findAllRegions.
 *********************************************************/
void CTopography::MakeRelations() 
{
	CCharacteristicRegion* firstRegion;
	CCharacteristicRegion* secondRegion;
	CPoint2D<int> pixel;
	int firstIndex; int secondIndex;
	for(unsigned int i=0; i<m_missingRelations.GetTotalPositions(); i++) // edr: changed i from int to unsigned int
	{
		pixel = m_missingRelations.GetPosition(i);
		firstIndex = pixel.GetX();
		secondIndex = pixel.GetY();
		firstRegion = &m_TFG[firstIndex];
		secondRegion = &m_TFG[secondIndex];
		if(firstRegion->IsInNeighbourRegion(secondIndex) == false) 
		{
			firstRegion->AddNeighbourRegion(secondIndex);
			secondRegion->AddNeighbourRegion(firstIndex);
		}
	}
}

/*** GetTFG ***********************************************
 * Return the TFG
 *********************************************************/
const vector<CTopography::CCharacteristicRegion>& CTopography::GetTFG() const
{
	return m_TFG;
}


/*** PaintRegions *****************************************
 * Paints and saves image of all pixels belonging to an
 * characteristic region of interest.
 *********************************************************/
bool CTopography::PaintCharacteristicRegions(CStdImage& Img) const
{

	if (!m_findCharRegions)	
	{
		ostringstream ost;
		ost << "CTopography: The characteristic regions are yet to be extracted" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}

	int numberOfRegions;
	int numberOfPixels;
	CCharacteristicRegion currentRegion;
	CPixelGroup currentPixelGroup;
	CPoint2D<int> currentPixel;
	TC_FEATURETYPE currentType;
	int xPos, yPos;
	numberOfRegions = m_TFG.size();
	Img=m_image;
	Img.CopyConvert(24,Img);
	
	for(int i = 0; i < numberOfRegions; i++) {
		currentRegion = m_TFG[i];
		currentPixelGroup = currentRegion.GetPixelGroup();
		numberOfPixels = currentPixelGroup.GetTotalPositions();
		for(int j = 0; j < numberOfPixels; j++) {
			currentPixel = currentPixelGroup.GetPosition(j);
			xPos = currentPixel.GetX(); 
			yPos = currentPixel.GetY();
			currentType = currentRegion.GetType();
			if(currentType == PEAK)			Img.SetPixel(xPos,yPos,CPalette::CreateRGB(0,255,255)); // turkIs
			else if(currentType == PIT)		Img.SetPixel(xPos,yPos,CPalette::CreateRGB(192,0,192));
			else if(currentType == SADDLE)  Img.SetPixel(xPos,yPos,CPalette::CreateRGB(255,0,0));
			else if(currentType == RIDGE)	Img.SetPixel(xPos,yPos,CPalette::CreateRGB(0,255,0));
			else if(currentType == RAVINE)	Img.SetPixel(xPos,yPos,CPalette::CreateRGB(0,0,192));
			else if(currentType == FLAT)	Img.SetPixel(xPos,yPos,CPalette::CreateRGB(192,192,192));
			else  Img.SetPixel(xPos,yPos,CPalette::CreateRGB(192,192,0)); //HillSide
		}
	}
	return true;
}


/*** WriteTofile *****************************************
 * Writes all interesting information for each characte-
 * ristic region out to a file.
 *********************************************************/
bool CTopography::WriteTofile(ostringstream& ost) const
{

	if (!m_findCharRegions)	
	{
		ostringstream ost;
		ost << "CTopography: The characteristic regions are yet to be extracted" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}

	int noOfRegions; int idNo;
	int noOfPixels;
	int noOfNeighbourRegions;
	vector<int> neighbourRegions;
	TC_FEATURETYPE type;
	vector< CPoint2D<int> > pixels;

	CCharacteristicRegion currentRegion;
	CCharacteristicRegion neighbourRegion;
	string str;

	noOfRegions = m_TFG.size();
	ost << "Number of characterIstic regions found: ";
	ost << noOfRegions << endl;
	ost << endl;

	for(int regionNo = 0; regionNo < noOfRegions; regionNo++) 
	{
		currentRegion = m_TFG[regionNo];
		idNo = currentRegion.GetRegionID();
		// write ID number of region
		ost << "Region number: " << idNo << endl;
		// write type of region
		type = currentRegion.GetType();
		if(type == PEAK) str = "PEAK";
		else if(type == PIT) str = "Pit";
		else if(type == SADDLE) str = "Saddle";
		else if(type == RIDGE) str = "Ridge";
		else if(type == RAVINE) str = "Ravine";
		else if(type == FLAT) str = "Flat";
		else if(type == HILLSIDE) str = "HillSide";
		ost << "Region type: " << str	<< endl;
		// write number of pixels
		noOfPixels = currentRegion.NumberOfPixels();
		ost << "Number of pixels in region: " << noOfPixels << endl;
		// write all pixels
		ost << "Pixels: ";
		pixels = currentRegion.GetPixels();
		int i; // edr: declare variable outside loop
		for(i=0; i<noOfPixels;i++) 
		{
			if(i>0) 
				ost << ", ";
			ost << pixels[i];
		}
		ost << endl;

		// write number of neighbour regions
		ost << "Number of neighbour regions: ";
		noOfNeighbourRegions = currentRegion.NumberOfNeighbourRegion();
		neighbourRegions = currentRegion.GetAllNeighbourRegions();
		ost << noOfNeighbourRegions << endl;
		// write neighbour regions
		ost << "Neighbour regions: ";
		for(i = 0; i < noOfNeighbourRegions; i++) 
		{
			if(i>0) 
				ost << ", ";
			int neighbourID = neighbourRegions[i];
			ost << neighbourID+1;
		}
		ost << endl << endl;
	}
	return true;
}

bool CTopography::ComputeGradient(int x, int y,CVector2D<float>& Grad,int N) 
{
	
	// Check if the class is initialized!!
	if( (N < 2) || (N > 9))
	{
		ostringstream ost;
		ost << "CTopography::ComputeGradient() N=" << N << " out of range [2;9]" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	
	if( (x < N || x >= (int)m_image.GetWidth()- N-1) || // edr: cast GetWidth() to int
		(y < N || y >= (int)m_image.GetHeight()- N-1) ) // edr: cast GetHeight() to int
	{
		return false;
	}

	int tmp = m_N;
	m_N = N;
	
	ComputeGrad(x,y,Grad);
	
	m_N = tmp;
	return true;
}

bool CTopography::ComputeEigenValues(int x,int y,float& eigVal0, float& eigVal1,int N)
{
	// Check if the class is initialized!!
	
	if( (N < 2) || (N > 9))
	{
		ostringstream ost;
		ost << "CTopography::ComputeEigenValues() N=" << N << " out of range [2;9]" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	
	if( (x < N || x >= (int)m_image.GetWidth()- N-1) || // edr: cast GetWidth() to int
		(y < N || y >= (int)m_image.GetHeight()- N-1) ) // edr: cast GetHeight() to int
	{
		return false;
	}

	float H[2][2];
	int tmp = m_N;
	m_N = N;
	
	ComputeHessian(x,y,H);
	ComputeEigenVal(H,eigVal0,eigVal1);
	ZeroThresholdEig(eigVal0, eigVal1);	

	m_N = tmp;
	return true;
}

bool CTopography::ComputeEigenVectors(int x,int y,CVector2D<float>& eigVec0,CVector2D<float>& eigVec1,int N)
{
	// Check if the class is initialized!!
	
	if( (N < 2) || (N > 9))
	{
		ostringstream ost;
		ost << "CTopography::ComputeEigenVectors() N=" << N << " out of range [2;9]" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	
	if( (x < N || x >= (int)m_image.GetWidth()- N-1) || // edr: cast GetWidth() to int
		(y < N || y >= (int)m_image.GetHeight()- N-1) ) // edr: cast GetHeight() to int
	{
		return false;
	}
	
	float H[2][2];
	float eigVal0,eigVal1;
	int tmp = m_N;
	m_N = N;

	ComputeHessian(x,y,H);
	ComputeEigenVal(H,eigVal0,eigVal1);
	ComputeEigenVec(H,eigVal0,eigVal1,eigVec0,eigVec1);
	if( ZeroThresholdEig(eigVal0, eigVal1) )
	{
		Swap(eigVec0, eigVec1);
	}
	
	m_N = tmp;
	return true;
}

bool CTopography::ComputePixelLabel(int x,int y,TC_FEATURETYPE& PixelLabel,int N)
{
	// Check if the class is initialized!!
	if( (N < 2) || (N > 9))
	{
		ostringstream ost;
		ost << "CTopography::ComputePixelLabel() N=" << N << " out of range [2;9]" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	
	if( (x < N || x >= (int)m_image.GetWidth()- N-1) || // edr: cast GetWidth() to int
		(y < N || y >= (int)m_image.GetHeight()- N-1) ) // edr: cast GetHeight() to int
	{
		ostringstream ost;
		ost << "CTopography::ComputePixelLabel() Position (x,y)=(" << x << "," 
			<< y << ") out of image range" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	
	float H[2][2];
	float eigVal0,eigVal1;
	CVector2D<float> grad;
	int tmp = m_N;
	m_N = N;

	ComputeGrad(x,y,grad);
	ComputeHessian(x,y,H);
	ComputeEigenVal(H,eigVal0,eigVal1);

	if( grad.IsZero() ) 
	{
		ZeroThresholdEig(eigVal0, eigVal1);	
		
		if( eigVal0 == 0) 
		{
			if(eigVal1 == 0)
				PixelLabel = FLAT;
			else
				PixelLabel = HILLSIDE;
		}

		else if(eigVal0 < 0) 
		{
			// A Ridge
			if( eigVal1 == 0 ) 
				PixelLabel = RIDGE;
			// A Peak 
			else if(eigVal1 < 0) 
				PixelLabel = PEAK;
			// A Saddle
			else
				PixelLabel = SADDLE;
		}

		else 
		{
			// A Ravine
			if( eigVal1  == 0 ) 
				PixelLabel = RAVINE;
			// A Pit 
			else if(eigVal1 > 0)
				PixelLabel = PIT;
					
			// A Saddle
			else 
				PixelLabel = SADDLE;
		}
	}
				
	else 
	{
		float dD_P11,dD_P12,dD_P21,dD_P22;
		int d11,d12,d21,d22;
		UINT32 p_av; // edr: changed from int to UINT32
		CVector2D<float> grad0,grad1,eigVec0,eigVec1;
		
		ComputeEigenVec(H, eigVal0, eigVal1, eigVec0, eigVec1);	
		if( ZeroThresholdEig(eigVal0, eigVal1) )
		{
			Swap(eigVec0, eigVec1);
		}		

		/* Calc. directional deriv. in direction of eigenvectors */
		// Find direction (approximate eigenvectors)
		d11 = Round(eigVec0.GetX()); 
		d12 = Round(eigVec0.GetY());

		ComputeGrad(x+d11,y-d12,grad0);
		ComputeGrad(x-d11,y+d12,grad1);

		dD_P11 = grad0.GetDot(eigVec0);  
		dD_P12 = grad1.GetDot(eigVec0);

		// Ortogonal direction
		d21 = d12;
		d22 = -d11;

		ComputeGrad(x+d21,y-d22,grad0);
		ComputeGrad(x-d21,y+d22,grad1);
	
		dD_P21 = grad0.GetDot(eigVec1);   
		dD_P22 = grad1.GetDot(eigVec1);  
	
		// Threshold directional derivatives
		if( fabs(dD_P11) < m_zeroLim_dirDeriv )	dD_P11 = 0;
		if( fabs(dD_P12) < m_zeroLim_dirDeriv )	dD_P12 = 0;
		if( fabs(dD_P21) < m_zeroLim_dirDeriv ) dD_P21 = 0;
		if( fabs(dD_P22) < m_zeroLim_dirDeriv ) dD_P22 = 0;
	
		/* Classify pixel */
		if(dD_P11*dD_P12 < 0) 
		{								// Zero-crossing in one direction, d1
			if(dD_P21*dD_P22 < 0) {		// Zero-crossing in both direction
				if( eigVal0 == 0) 
				{
					if(eigVal1 == 0)
						PixelLabel = FLAT;
					else
						PixelLabel = HILLSIDE;
				}

				else if(eigVal0 < 0) 
				{
				// A Ridge
				if( eigVal1 == 0 ) 
					PixelLabel = RIDGE;
				// A Peak 
				else if(eigVal1 < 0) 
					PixelLabel = PEAK;
				// A Saddle
				else
					PixelLabel = SADDLE;
				}

				else 
				{
					// A Ravine
					if( eigVal1  == 0 ) 
						PixelLabel = RAVINE;
					// A Pit 
					else if(eigVal1 > 0)
						PixelLabel = PIT;
					
					// A Saddle
					else 
						PixelLabel = SADDLE;
				}
			}

			else 
			{						
				if(eigVal0 > 0)		
					PixelLabel = RAVINE;
				else if(eigVal0 < 0)	
					PixelLabel = RIDGE;
				else 
				{					
					p_av = (m_image.GetPixelFast(x+d11,y-d12) +
							  m_image.GetPixelFast(x-d11,y+d12))/ 2;
										
					if(m_image.GetPixelFast(x,y) > p_av)  
						PixelLabel = RIDGE;
					else								  
						PixelLabel = RAVINE;
			}
		}
	}

	else if(dD_P21*dD_P22 < 0) 
	{	// Zero-crossing in one direction, d2
		if(eigVal1 > 0)			
			PixelLabel = RAVINE;
		else if(eigVal1 < 0)	
			PixelLabel = RIDGE;
		else 
		{					
			p_av = ( m_image.GetPixelFast(x+d21,y-d22) +
					      m_image.GetPixelFast(x-d21,y+d22) )/ 2;
							
			if(m_image.GetPixelFast(x,y) > p_av)
				PixelLabel = RIDGE;
			else	 
				PixelLabel = RAVINE;
		}
	}

	else	
		PixelLabel = HILLSIDE;
	}

	
	m_N = tmp;
	return true;
}

} // end ipl namespace
