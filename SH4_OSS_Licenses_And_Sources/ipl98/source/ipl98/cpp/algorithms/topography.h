#ifndef _IPL98_TOPOGRAPHY_H
#define _IPL98_TOPOGRAPHY_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../image.h"
#include "../byte_image.h"
#include "../pixelgroups.h"
#include "../vectors/vector2d.h"
#include "mask_operation.h"
#include "../arrays/array2d.h"

namespace ipl{ // use namespace ipl

/** Classification types for class CTopography. Possible values are: UNTREATED=0, PEAK=2, 
	PIT=4, SADDLE=8, RIDGE=16, RAVINE=32, FLAT=64, HILLSIDE */
typedef enum { UNTREATED=0, PEAK=2, PIT=4, SADDLE=8, RIDGE=16, RAVINE=32, FLAT=64, HILLSIDE=128} TC_FEATURETYPE;

/* This class should have been a private class inside CTopography,
   but because of a bug in the Visual C++ 6.0 compiler it is not possible. The problem
   is that the attribute "CArray2D<CPixelInformation> m_TCG" declared in 
   CTopography causes the compiler to fail when it looks for the
   streaming operators of CPixelInformation needed in CArray2D. I cannot find
   a better workaround than moving the class outside CTopography. */
class CPixelInformation {
private:
	TC_FEATURETYPE m_type;
	bool m_visited;
	int m_region;
public:
	CPixelInformation() {m_type = UNTREATED; m_visited = false;}
	CPixelInformation(TC_FEATURETYPE type) {m_type = type;m_visited = false;}
	inline TC_FEATURETYPE GetType() const {return m_type;}
	inline bool IsVisited() const {return m_visited;}
	inline void SetType(TC_FEATURETYPE type) {m_type = type;}
	inline void SetVisited() {m_visited = true;}
	int GetRegion() const {return m_region;}
	void SetRegion(int region)  {m_region = region;}
	// Copy operator
	inline CPixelInformation& operator=(const CPixelInformation& pI){
		m_type = pI.GetType(); 
		m_visited = pI.IsVisited();
		return *this;
	}
	/* Streaming operators */
	friend std::ostream& operator<<(std::ostream& s, CPixelInformation& pI) {
		return s << pI.GetType();
	}
	friend std::istream& operator>>(std::istream& is, CPixelInformation& pI) {
		int type;
		is >> type;
		pI.SetType( TC_FEATURETYPE(type) ); // edr: (const) removed in front of "type"
		return is;
	}	
};	

/** @ingroup algorithms
@{ */

/** Class that contains the declarations of the fields and methods, that is 
	used in the gray-scale feature extraction. It contains functionality to
	perform a topological classification of pixels in a grayscale image.
	The method that is used relies on direct gray-scale extraction
	of features for character recognition. The basic theory behind this method is
	presented in the paper "Direct Gray-Scale Extraction of Features for Character
	Recognition" by Li Wang and Theo Pavlidis, Fellow, IEEE.
	
	In addition there is functionality to retrieve and present the regions in
	various ways. Including a class representing a characteristic region.
	Furthermore there is public methods to extract relevant information for a
	specific pixel such as gradient, eigenvalues, eigenvectors and pixel labels.

	In this context the gray-scale value for the foreground is 255, that is white.
	If the preferred foreground color is black, and accordingly the preferred back-
	ground color is white, then simple invert the considered image. E.g. the image
	m_image is inverted by m_image.Invert();

	Examples on using this class:
	\verbatim

	Example 1: topological classification
	m_Img1.Load("d:/temp/testbig1.bmp");
	m_FeatureExtract.Initialize(m_Img1);

	// Run classification on image with N = 3
	m_FeatureExtract.DeriveTopographicClassification(3);

	// Paint the labels in a copy of the original image
  	m_FeatureExtract.PaintTopographicLabels();

	// Find all characteritic region of the classification
	m_FeatureExtract.FindAllRegions();
	
	// Save all the found regions to the image "RegionImage.bmp"
	m_FeatureExtract.PaintRegions();

	// Write information regarding all the characteristic region to a text-file 'Results.txt'
	m_FeatureExtract.WriteTofile();

	Example 2: information extraction for specific pixel
	xxx to be continued xxx
	\endverbatim


	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CTopography ipl98/cpp/algorithms/topography.h
	@version 0.1
	@author Søren Jørgensen (sorenj@mip.sdu.dk) and Mogens Christiansen (mog@mip.sdu.dk) */

class CTopography 
{
public:
	/** Class that contains the required fields of a characteristic regions, and 
	the methods to access and set the fields of a region in a correct manner. */

	class CCharacteristicRegion
	{
	private:
		int m_regionID;
		TC_FEATURETYPE m_type;
		CPixelGroup m_pixelGroup;
		vector<int> m_neighbourRegion;
		
	public:
		/** Default constructor. */
		CCharacteristicRegion(){};
	
		/** Returns the ID of a specific region.
		@return m_regionID ID of region */
		inline int GetRegionID() const {return m_regionID;}
		
		/** Sets the ID of a specific region.
		@param num The ID for the region */
		inline void SetRegionID(int num) {m_regionID = num;}
		
		/** Returns the type of a specific region.
		@return m_type Type of region */
		inline TC_FEATURETYPE GetType() const {return m_type;}

		/** Sets the type of a specific region.
		@param m_type The ID for the region */		
		inline void SetType(TC_FEATURETYPE type) {m_type = type;}

		/** Adds a PixelGroup to pixel container of the region.
		@param x X-coordinate for the pixel
		@param y Y-coordinate for the pixel */	
		inline void AddToPixelGroup(int x, int y) {m_pixelGroup.AddPosition(x,y);}

		/** Returns a PixelGroup from the pixel container of the region.
		@return m_pixelGroup Pixel group of the region */
		inline CPixelGroup GetPixelGroup() const {return m_pixelGroup;}

		/** Returns the number of pixels in the container of the region.
		@return number of pixels in the region */
		int NumberOfPixels() {
			return m_pixelGroup.GetTotalPositions();
		}

		/** Returns all pixels from the pixel group of the region.
		@return pixels, a vector of CPoint2D<int> */
		vector< CPoint2D<int> > GetPixels() {
			vector< CPoint2D<int> > pixels;
			CPoint2D<int> pixel;
			for(unsigned int i=0; i<m_pixelGroup.GetTotalPositions(); i++) {
				pixel = m_pixelGroup.GetPosition(i);
				pixels.push_back(pixel);
			}
			return pixels;
		}

		/** Adds an index to the vectors of ints containing the indecies
		of neighbour regions.
		@param index The index of the neighbour region */
		inline void AddNeighbourRegion(int index)  {
			m_neighbourRegion.push_back(index);
		}

		/** Returns the vector of ints containing the indecies
		of neighbour regions.
		@return m_neighbourRegion, a vector of int's */
		inline vector<int> GetAllNeighbourRegions() const {
			return m_neighbourRegion;
		}
		
		/** Returns the number of pixels in the container of the region.
		@return number of neighbour regions */
		inline int NumberOfNeighbourRegion() {
			return m_neighbourRegion.size();
		}
		
		/** Decides whether neighbour region already is treated.
		@return true, if index of neighbour region already is represented in
		indecies container. */
		bool IsInNeighbourRegion(int index)  {
			int size = m_neighbourRegion.size();
			for (int i = 0; i < size; i++) {
				if (index == m_neighbourRegion[i])
					return true;
			}
			return false;
		}

		/** Overloading of assignment operator. */
		inline CCharacteristicRegion& operator=(const CCharacteristicRegion& cR)
		{
			m_regionID = cR.GetRegionID();
			m_type = cR.GetType(); 
			m_pixelGroup = cR.GetPixelGroup();
			m_neighbourRegion = cR.GetAllNeighbourRegions();
			return *this;
		}
	};

private:
	class CPixelEncaps
	{
	private:
		CPixelInformation m_pixelInfo;
		int m_x; int m_y;

	public:
		CPixelEncaps(){}
		CPixelEncaps(int x, int y, const CPixelInformation& pixelInfo, bool value) {
			m_x = x; m_y = y; m_pixelInfo = pixelInfo;m_pixelInfo.SetVisited();}
		
		inline int GetX() const {return m_x;}
		inline int GetY() const {return m_y;}
		inline CPixelInformation GetPixelInfo() const {return m_pixelInfo;}
	};

	CByteImage m_image;
	
	bool m_initialized;
	bool m_deriveTopography;
	bool m_findCharRegions;
	bool m_dynamicN;

	int m_N;
	int m_maxN;
	int m_minN;
	int m_maxX;
	int m_maxY;
	int m_regionCounter;
	int m_numberOfCharRegions;
	int m_min0;int m_min1;int m_min2;int m_min3;int m_min4;
	int m_min5;int m_min6;int m_min7;int m_min8;int m_min9;

	float m_zeroLim_1deriv;
	float m_zeroLim_eig;
	float m_zeroLim_dirDeriv;

	UINT32 m_background_threshold; // edr: changed from int to UINT32

	CPixelGroup m_missingRelations;

	float m_h[10][3][3][20][20];
	
	CArray2D<CVector2D<float> > m_gradient;

	// For iterators
	vector<CByteImage::ConstRowIterator> m_imageScanLines; 
	std::vector<CByteImage::ConstRowIterator>::iterator m_imageScanLines_p; 

	// Contains the topographic classification of all image pixels
	CArray2D<CPixelInformation> m_TCG;

	CArray2D<int> m_distanceToBackground;

	vector<CCharacteristicRegion> m_TFG;

	/* Functions for computing derivative, gradients, eigenvalues and eigenvectors */
	inline float ComputeDeriv(int p,int q, int x, int y);
	inline void ComputeGrad(int x, int y, CVector2D<float>& grad);
	inline void ComputeHessian(int x, int y, float H[2][2]);
	inline void ComputeEigenVal(const float H[2][2], float& eigVal0, float& eigVal1);
	inline void ComputeEigenVec(const float H[2][2], float eigVal0, float eigVal1, 
						        CVector2D<float>& eigVec0,CVector2D<float>& eigVec2);
	
	// Functions for classifying a pixel 
	// (giving it a label in [peak, pit,hillside,ridge,ravine,saddle])
	inline void ClassifyPixelD(int x, int y, float eigVal0, float eigVal1);
	inline void ClassifyPixelE(int x, int y, float eigVal0, float eigVal1,
							   const CVector2D<float>& eigVec0, const CVector2D<float>& eigVec1);

	inline bool ZeroThresholdEig(float& eigVal0, float& eigVal1);
	
	void MakeDistanceToBackgroundTable();

	void FindRegions(int x, int y, int parentIndex);
	
	CPixelGroup FindRegion(int x, int y, CCharacteristicRegion* newRegion);
	
	void MakeRelations();
	
public:
	/** Used when calling DeriveTopographicClassification */
	static const short DYNAMIC_N;

	/** Default constructer takes no parameters */
	CTopography();


	/**	Initializes various settings. Including setting up and saving the considered image,
		iterators and various memory allocations. Must be called before all other public methods.
		@param Img The considered image
		@return True, if allocation of memory was performed correctly */
	bool Initialize(CImage& Img);


	/** Finds the topographical label for each pixel in the considered image. For more details about
		the implemented method see the paper "Direct Gray-Scale Extraction of Features for Character
		Recognition" by Li Wang and Theo Pavlidis, Fellow, IEEE. 

		@param N Size of the (2N+1) x (2N+1) neighborhood used to calculate derivates. Must be bewteen 2 and 9. 
		         If supplied value is CTopography::DYNAMIC_N, the neighbourhood size is determined dynamically. 
				 Default value is 3.
		@return false, if Initialize has not been called first or the N parameter is wrong */
	bool DeriveTopographicClassification(int N = 3);


	/** Is used to change the value of the threshold for the background color in
		the dynamic calculation of N. The value has to be between 0 and 256.
		@param BackgroundValue New value for background threshold */
	bool ChangeBackgroundValue(int BackgroundValue);

	/** Is used to change the values of the thredsholds, which is used to check if
		a value is eaual to zero, when computational errors are taken into account.
		The value should be be between 0 and 256.
		@param ZeroLim_1deriv limit for first derivatives
		@param ZeroLim_eig limit for eigenvalues
		@param ZeroLim_dirDeriv limit for directive derivatives 
		@return false, if one of the parameters are negative */
	bool ChangeLimitValues(float ZeroLim_1deriv, float ZeroLim_eig, float ZeroLim_dirDeriv);

	/** Is used to change values in refinement of the dynamic neighborhood size N used when calculating
		derivatives. 
		@param minX if distance to background is X, then N=minX for N is used. The supplied values
		has to be bewteen 2 and 9
		(X is contained in {0,1,2,3,4,5,6,7,8,9}.)
		@return false, if one of the parameters are negative */
	bool ChangeDynamicValues(int min0, int min1,int min2,int min3,int min4,int min5,int min6,int min7,int min8,int min9);

	/** Draws the topographic label of every pixel in m_image. The colors in the image
		represents the topological labels as follows:
		Cyan:	Peak
		Purple:	Pit
		Red:	Saddle
		Green:	Ridge
		Blue:	Ravine
		Gray:	Flat
		The method DeriveTopographicClassification() must be run prior.
		@param All If true, then every type is drawn. If false, then only most
		important are considered 
		@return false, if the topographic classification with DeriveTopographicClassification()
        has not been run*/
	bool PaintTopographicLabels(CStdImage& Img,int DrawPixelLabel = RAVINE | PIT | HILLSIDE) const;

	/** Runs through all pixels in m_image. When unvisted PEAK or RIDGE is seen,
	findRegions is called. Ensures that pixels are visited and thereby finds
	all characteristic regions of importants. Thereby only allowing regions of
	Saddles and Flats to be included if these somehow is connected to a Pit or
	Ridge region through other valid characteristic regions */
	bool FindCharacteristicRegions();

	/** Return the TFG.
		@param TFG, that is the ADT containing all the characteristic regions. */
	const vector<CCharacteristicRegion>& GetTFG() const;

	/** Paints the image of all pixels belonging to an characteristic
		region of interest. The colors in the image represents the topological
		labels as follows:
		Cyan:	Peak
		Purple:	Pit
		Red:	Saddle
		Green:	Ridge
		Blue:	Ravine
		Gray:	Flat */
	bool PaintCharacteristicRegions(CStdImage& Img) const;

	/** Writes all interesting information for each characte-
		ristic region out to given ost parameter.
		@param ost Information written to this stream. */
	bool WriteTofile(ostringstream& ost) const;

	/** Slims down the areas consisting of ridges, resulting in areas with a smaller width.*/
	bool ThinRidgeLines();


	/** @name Local topographic information methods.
	*/
	//@{ 
	/** Calculates the gradient at position (x,y).
		@param x The position in horisontal direction
		@param y The position in vertical direction
		@param Grad The calulated gradient is placed in this 2D vector
		@param N Size of the (2N+1) x (2N+1) neighborhood used to calculate derivates. Must be bewteen 2 and 9. Default value is 3
		@return False If N is not between 2 and 9 or the position is outside the image */
	bool ComputeGradient(int x, int y,CVector2D<float>& Grad,int N = 3);

	/** Calculates the eigenvalues at position (x,y).
		@param x The position in horisontal direction
		@param y The position in vertical direction
		@param EigVal0 The absolute biggest calulated eigenvalue
		@param EigVal1 The absolute smallest calulated eigenvalue
		@param N Size of the (2N+1) x (2N+1) neighborhood used to calculate derivates. Must be bewteen 2 and 9. Default value is 3.
		@return False If N is not between 2 and 9 or the position is outside the image */
	bool ComputeEigenValues(int x,int y,float& EigVal0, float& EigVal1,int N = 3); 

	/** Calculates the eigenvalues at position (x,y).
		@param x The position in horisontal direction
		@param y The position in vertical direction
		@param EigVec0 The calulated eigenvector for absolute the biggest eigenvalue is placed in this 2D vector
		@param EigVec1 The calulated eigenvector for absolute the smallest eigenvalue is placed in this 2D vector
		@param N Size of the (2N+1) x (2N+1) neighborhood used to calculate derivates. Must be bewteen 2 and 9. Default value is 3.
		@return False If N is not between 2 and 9 or the position is outside the image */
	bool ComputeEigenVectors(int x,int y,CVector2D<float>& EigVec0,CVector2D<float>& EigVec1,int N = 3);

	/** Finds the topographic label of the pixel at position (x,y). The label types are: Ridge, ravine, peak, pit, saddle, flat and hillside.
		@param x The position in horisontal direction
		@param y The position in vertical direction
		@param PixelLabel The pixel's label
		@param N Size of the (2N+1) x (2N+1) neighborhood used to calculate derivates. Must be bewteen 2 and 9. Default value is 3.
		@return False If N is not between 2 and 9 or the position is outside the image */
	bool ComputePixelLabel(int x,int y,TC_FEATURETYPE& PixelLabel,int N = 3);
	//@}
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

/* Computes the (p+q)'th derivative using a N x N (m_N) neighborhood */
inline float CTopography::ComputeDeriv(int p,int q,int x, int y) {
	float df = 0.0;
	float* h_ptr;

	// Use of iterators for accessing pixels -> not used because 
	// of a possible bug not found yet
	
	// Get vector iterator
	m_imageScanLines_p += y+m_N;
	
	for(int i = -m_N; i <= m_N; i++) 
	{
		// Get row iterator
		*m_imageScanLines_p += x+m_N;
		// Get pointer to h's
		h_ptr = m_h[m_N][q][p][i+m_N]-1;
		
		for(int j = -m_N; j <= m_N; j++) 
		{
			df += ( *(*m_imageScanLines_p) ) * (*++h_ptr); 
			--(*m_imageScanLines_p);
		}			
		// Restore iterator to the first elem.
		(*m_imageScanLines_p) += m_N-x+1;
		// Change row;
		--m_imageScanLines_p;
	}
	
	// Restore vector iterator
	m_imageScanLines_p += m_N-y+1;

//
//	// Calculate derivative
//	for(int i = -m_N; i <= m_N; i++) {
//		// Get pointer to h's
//		h_ptr = m_h[m_N][p][q][i+m_N]-1;
//		
//		for(int j = -m_N; j <= m_N; j++)
//			df += m_image.GetPixelFast(x-i,y-j) * (*++h_ptr); 
//	}

	return df;
}


inline void CTopography::ComputeGrad(int x, int y, CVector2D<float>& grad) 
{
	grad.Set(ComputeDeriv(1,0,x,y),ComputeDeriv(0,1,x,y));

	// Threshold the size of the gradient
	if( fabs(grad.GetX()) < m_zeroLim_1deriv )
	{
		grad.SetX(0);
	}
	
	if( fabs(grad.GetY()) < m_zeroLim_1deriv )
	{
		grad.SetY(0);
	}
}

inline void CTopography::ComputeHessian(int x, int y, float H[2][2]) 
{
	H[0][0] = ComputeDeriv(2,0,x,y);
	H[0][1] = ComputeDeriv(1,1,x,y);
	H[1][0] = H[0][1];
	H[1][1] = ComputeDeriv(0,2,x,y);
}

inline bool CTopography::ZeroThresholdEig(float& eigVal0, float& eigVal1)
{
	/* Check if the eigenvalues are zero and ensure lamda0 is absolute larger
	   than lambda1 */
	if( fabs(eigVal0) < m_zeroLim_eig ) 
	{	
		if( fabs(eigVal1) < m_zeroLim_eig ) 
		{
			eigVal0 = 0; 
			eigVal1 = 0;
			return false;
		}

		else 
		{
			eigVal0 = eigVal1;	
			eigVal1 = 0;
			return true;
		}
		
	}

	else if( fabs(eigVal1) < m_zeroLim_eig ) 
	{
		eigVal1 = 0;
		return false;
	}

	if( fabs(eigVal0) < fabs(eigVal1) ) 
	{
		::ipl::Swap(eigVal0,eigVal1);
		return true;
	}
	return false;
}

inline void CTopography::ComputeEigenVal(const float H[2][2], float& eigVal0, float& eigVal1) 
{
	/* Special case -> avoid division by zero */
	if( H[0][1] == 0) 
	{
		if(fabs(H[0][0]) > fabs(H[1][1])) 
		{
			eigVal0 = H[0][0]; 
			eigVal1 = H[1][1];
		}

		else {
			eigVal0 = H[1][1]; 
			eigVal1 = H[0][0];
		}
	}
	/* Eigenvalues */
	else {
		float sqr = sqrt(4*H[0][1]*H[1][0] + (H[0][0]-H[1][1])*(H[0][0]-H[1][1])); 
		eigVal0 = (float)0.5*( H[0][0] - sqr + H[1][1] ); 
		eigVal1 = (float)0.5*( H[0][0] + sqr + H[1][1] );
	}
}

inline void CTopography::ComputeEigenVec(const float H[2][2], float eigVal0, float eigVal1, 
												  CVector2D<float>& eigVec0, CVector2D<float>& eigVec1) 
{
	float norm, eigVec_x;

	/* Special case (avoid dividing with zero) */
	if( H[0][1] == 0) 
	{
		eigVec0.Set(1,0);
		eigVec1.Set(0,1);
		return;
	}
	
	/* calc. eigenvectors */
	eigVec_x = (eigVal0-H[1][1])/ H[1][0];
	norm = (float)1.0 / sqrt(1 + eigVec_x*eigVec_x);
	eigVec0.Set(eigVec_x*norm, norm);

	eigVec1.Set(eigVec0.GetY(),-eigVec0.GetX());
}

// Classify a pixel directly using the eigenvalues
inline void CTopography::ClassifyPixelD(int x, int y, float eigVal0, float eigVal1) 
{		
	if( eigVal0 == 0) 
	{
		if(eigVal1 == 0)
			m_TCG[x][y] = FLAT;
		else
			m_TCG[x][y] = HILLSIDE;
	}
	else if(eigVal0 < 0) 
	{
		// A Ridge
		if( eigVal1 == 0 ) 
			m_TCG[x][y] = RIDGE;
		// A Peak 
		else if(eigVal1 < 0) 
			m_TCG[x][y] = PEAK;
		// A Saddle
		else
			m_TCG[x][y] = SADDLE;
	}
	else 
	{
		// A Ravine
		if( eigVal1  == 0 ) 
			m_TCG[x][y] = RAVINE;
		// A Pit 
		else if(eigVal1 > 0)
			m_TCG[x][y] = PIT;
		// A Saddle
		else 
			m_TCG[x][y] = SADDLE;
	}
}


// Classify a pixel looking at zero-crossing of directional derivatives
inline void CTopography::ClassifyPixelE(int x, int y, float eigVal0, 
			float eigVal1, const CVector2D<float>& eigVec0, 
			const CVector2D<float>& eigVec1) 
{
	float dD_P11,dD_P12,dD_P21,dD_P22;
	int d11,d12,d21,d22;
	UINT32 p_av; // edr: changed from type int to UING32

	/* Calc. directional deriv. in direction of eigenvectors */
	// Find direction (approximate eigenvectors)
	d11 = Round(eigVec0.GetX()); 
	d12 = Round(eigVec0.GetY());

	dD_P11 = m_gradient[x+d11][y-d12].GetDot(eigVec0);  
	dD_P12 = m_gradient[x-d11][y+d12].GetDot(eigVec0);

	// Ortogonal direction
	d21 = d12;
	d22 = -d11;
	
	dD_P21 = m_gradient[x+d21][y-d22].GetDot(eigVec1);   
	dD_P22 = m_gradient[x-d21][y+d22].GetDot(eigVec1);  
	
	// Threshold directional derivatives
	if( fabs(dD_P11) < m_zeroLim_dirDeriv )	dD_P11 = 0;
	if( fabs(dD_P12) < m_zeroLim_dirDeriv )	dD_P12 = 0;
	if( fabs(dD_P21) < m_zeroLim_dirDeriv ) dD_P21 = 0;
	if( fabs(dD_P22) < m_zeroLim_dirDeriv ) dD_P22 = 0;
	
	/* Classify pixel */
	if(dD_P11*dD_P12 < 0) 
	{			// Zero-crossing in one direction, d1
		if(dD_P21*dD_P22 < 0) 		// Zero-crossing in both direction
			ClassifyPixelD(x,y,eigVal0, eigVal1);
		else 
		{						
			if(eigVal0 > 0)		
				m_TCG[x][y] = RAVINE;
			else if(eigVal0 < 0)	
				m_TCG[x][y] = RIDGE;
			else 
			{					
				p_av = (m_image.GetPixelFast(x+d11,y-d12) +
							  m_image.GetPixelFast(x-d11,y+d12))/ 2;
										
				if(m_image.GetPixelFast(x,y) > p_av)  
					m_TCG[x][y] = RIDGE;
				else								  
					m_TCG[x][y] = RAVINE;
			}
		}
	}

	else if(dD_P21*dD_P22 < 0) 
	{	// Zero-crossing in one direction, d2
		if(eigVal1 > 0)			
			m_TCG[x][y] = RAVINE;
		else if(eigVal1 < 0)	
			m_TCG[x][y] = RIDGE;
		else 
		{					
			p_av = ( m_image.GetPixelFast(x+d21,y-d22) +
					      m_image.GetPixelFast(x-d21,y+d22) )/ 2;
							
			if(m_image.GetPixelFast(x,y) > p_av)
				m_TCG[x][y] = RIDGE;
			else	 
				m_TCG[x][y] = RAVINE;
		}
	}
	else	
		m_TCG[x][y] = HILLSIDE;	
}

/*@}*/ /* end group "algorithms" */

} // end namespace ipl

#endif	// _IPL98_TOPOGRAPHY_H
