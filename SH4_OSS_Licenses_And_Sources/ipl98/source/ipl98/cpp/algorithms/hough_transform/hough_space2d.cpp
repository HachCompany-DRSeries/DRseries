/********************************************************************
   The Image Processing Library 98 - IPL98
   Copyright (C) by René Dencker Eriksen - edr@mip.sdu.dk

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation. Only
   addition is, that if the library is used in proprietary software
   it must be stated that IPL98 was used.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

   More details can be found in the file licence.txt distributed
   with this library.
*********************************************************************/

#include "hough_space2d.h"

namespace ipl {

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CHoughSpace2D::CHoughSpace2D() : m_os(cout)
{
	m_initialized = false; 
}


CHoughSpace2D::CHoughSpace2D(ostream& os, int ImageWidth, int ImageHeight, int Thickness, int Threshold, 
	CHoughBase::PARAMETERTYPE ParamType) : m_os(os)
{
	m_initialized = false;
	Initialize(ImageWidth, ImageHeight, Thickness, Threshold, ParamType);
}


CHoughSpace2D::CHoughSpace2D(int Width, int Height) : m_os(cout)
{
	m_lineQuante.Set(Width, Height);
	this->Alloc();
}


CHoughSpace2D::~CHoughSpace2D()
{
}


//////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////


bool CHoughSpace2D::Initialize(int ImageWidth, int ImageHeight, int Thickness, int Threshold, 
	CHoughBase::PARAMETERTYPE ParamType)
{
	m_lineQuante = this->ComputeQuantization(ImageWidth, ImageHeight, Thickness, ParamType);
	m_threshold = Threshold;   // set threshold value. might need recomputation.
	return this->Alloc();
}


bool CHoughSpace2D::Alloc()
{
	if (this->CArray2D<unsigned int>::Alloc(m_lineQuante.GetX(), m_lineQuante.GetY())==true)
	{
		this->Flush(0);
		m_initialized = true;
		return true;
	}
	else
	{ 
		ostringstream ost;
		ost << "CHoughSpace2D::Alloc() Not initialized" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false; 
	}
}


void CHoughSpace2D::ToImage(CStdImage& outImg)
{
	int value, x, y, maxColorValue = 0;
	outImg.AllocFast(m_lineQuante.GetX(), m_lineQuante.GetY(), 8);
	
	// color scaling to greyscale
	for (y=0; y<m_lineQuante.GetY(); y++)
	{
		for (x=0; x<m_lineQuante.GetX(); x++)
		{
			value = operator[](x)[y];
			if (value > maxColorValue) 
				maxColorValue = value;
		}
	}
	
	for (y=0; y<m_lineQuante.GetY(); y++)
	{
		for (x=0; x<m_lineQuante.GetX(); x++)
		{
			value = (int) (1.0*operator[](x)[y]/maxColorValue*255);
			outImg.SetPixelFast(x,y,255-value);
		}
	}
}


int CHoughSpace2D::GetQuantizationValue(int i) const
{ 
	if (i==1) 
		return m_lineQuante.GetX();
	else 
		return m_lineQuante.GetY();
}


int CHoughSpace2D::GetOffset(int i) const
{
	if (i==1) 
		return m_offset.GetX();
	else 
		return m_offset.GetY();
}


float CHoughSpace2D::GetStepValue(int i) const
{ 
	if (i==1) 
		return m_stepValue.GetX();
	else 
		return m_stepValue.GetY();
}


void CHoughSpace2D::SetThreshold(unsigned int Threshold)
{
	m_threshold = Threshold;
}


unsigned int CHoughSpace2D::GetThreshold() const
{
	return m_threshold;
}


CParameter2D<int> CHoughSpace2D::GetPeakCoordinates() const
{
	return m_peakCoord;
}


void CHoughSpace2D::SetPeakCoordinates(const CPoint2D<int>& Position)
{
	m_peakCoord.Set(Position.GetX(), Position.GetY());
}


CParameter2D<float> CHoughSpace2D::GetRealPeakCoordinates()
{
	m_peakCoord.SetX(m_peakCoord.GetX()-m_offset.GetX());
	CParameter2D<float> realCoord;
	realCoord.SetX((float) m_stepValue.GetX()*m_peakCoord.GetX());
	realCoord.SetY((float) (m_offset.GetY()-m_peakCoord.GetY())*m_stepValue.GetY());
	
	return realCoord;
}


vector<CParameter2D<float> > CHoughSpace2D::FindPeaks(CHoughBase::PEAKDETECTIONTYPE DetectionMethod)
{
	switch (DetectionMethod)
	{
	case CHoughBase::BUTTERFLY:
		return this->FindPeaks_Butterfly();
	case CHoughBase::DIFFUSION:
		return this->FindPeaks_Diffusion();
	case CHoughBase::ITERATION:
		return this->FindPeaks_Iteration();
	case CHoughBase::LOCALMAXIMA:
		return this->FindPeaks_LocalMaxima();
	default:
		return this->FindPeaks_Butterfly();
	}
}


//////////////////////////////////////////////////////////////////////
// Private methods
//////////////////////////////////////////////////////////////////////


CPoint2D<int> CHoughSpace2D::ComputeQuantization(int ImageWidth, int ImageHeight, 
	int Thickness, CHoughBase::PARAMETERTYPE ParamType)
{
	int error = 0;
	float D, deltaRho;
	int nTheta, nRho, HRho;
	D = (float) sqrt(pow((double)ImageWidth,2.0)+pow((double)ImageHeight,2.0));
	
	switch (ParamType)
	{
	case CHoughBase::NORMAL_PARAM:
		deltaRho = (float)(2*error + Thickness + 1);
		HRho = (int)ceil(D/2.0f/deltaRho);
		nTheta = (int)ceil(PI/2.0f*D/deltaRho);
		nRho = 2*HRho+1;
		
		m_param1ValueRange.SetX((float) (-2.0*PI/nTheta));
		m_param1ValueRange.SetY((float) (PI-2.0*PI/nTheta));
		m_param2ValueRange.SetX(-D/2);
		m_param2ValueRange.SetY(D/2+1);
		m_lineQuante.SetX(nTheta);
		m_lineQuante.SetY(nRho);
		m_offset.SetX(2);
		m_offset.SetY(HRho);
		
		break;
	case CHoughBase::SLOPE_PARAM:
		// All values are user defined
		m_param1ValueRange.SetX(-1.0f);
		m_param1ValueRange.SetY( 1.0f);
		m_param2ValueRange.SetX(-D/2.0f);
		m_param2ValueRange.SetY( D/2.0f);
		m_lineQuante.SetX(400);
		m_lineQuante.SetY((int)(D/2));
		m_offset.SetX(200);
		m_offset.SetY((int)(D/4));
		
		// m_rhoStepConst = (float) 2*D/nRho;
		// m_rhoValue = D;
		break;
	default:
		break;
	}
	m_stepValue.SetX((m_param1ValueRange.GetY()-m_param1ValueRange.GetX())/m_lineQuante.GetX());
	m_stepValue.SetY((m_param2ValueRange.GetY()-m_param2ValueRange.GetX())/m_lineQuante.GetY());
	
	m_os << "Quantizations are: " << m_lineQuante << endl;
	m_os << "Step values are  : " << m_stepValue << endl;
	m_os << "Theta value range: " << m_param1ValueRange << endl;
	m_os << "Rho value range  : " << m_param2ValueRange << endl;
	
	return m_lineQuante;
}


vector<CParameter2D<float> > CHoughSpace2D::FindPeaks_Diffusion()
{
	unsigned int max, score;
	int k = 0, x, y, xi, yi;
	int width = this->GetWidth(), height = this->GetHeight();
	CParameter2D<float> real_peak_coord;
	vector<CParameter2D<float> > peaks;
	
	int p = 1;
	
	while (p > 10)
	{
		p = 0;
		
		max = m_threshold;
		// find peak coordinate
		for (y=0; y<height; y++)
		{
			for (x=0; x<width; x++)
			{
				score = this->operator[](x)[y];
				
				if (score>m_threshold) p++;
				
				if (score > max)
				{
					max = score;
					xi = x;
					yi = y;
				}
			}
		}
		
		if (max > m_threshold)
		{
			// makes interpolation !!!
			real_peak_coord.SetX((float) xi);
			real_peak_coord.SetY((float) yi);
			
			// reset accumulator cell and surrounding neighbours
			this->operator[](xi)[yi] = 0;
			this->CellResetTest(xi,  yi-1,max,NORTH);
			this->CellResetTest(xi,  yi+1,max,SOUTH);
			this->CellResetTest(xi-1,yi,  max,WEST);
			this->CellResetTest(xi+1,yi,  max,EAST);
			this->CellResetTest(xi-1,yi-1,max,NORTHWEST);
			this->CellResetTest(xi+1,yi-1,max,NORTHEAST);
			this->CellResetTest(xi-1,yi+1,max,SOUTHWEST);
			this->CellResetTest(xi+1,yi+1,max,SOUTHEAST);
			
			if (0 < xi && xi < (int)GetWidth() - 1 &&
				0 < yi && yi < (int)GetHeight() - 1)
			{
				this->operator[](xi+1)[yi] = 0;
				this->operator[](xi-1)[yi] = 0;
				this->operator[](xi)[yi+1] = 0;
				this->operator[](xi)[yi-1] = 0;
				this->operator[](xi-1)[yi-1] = 0;
				this->operator[](xi-1)[yi+1] = 0;
				this->operator[](xi+1)[yi-1] = 0;
				this->operator[](xi+1)[yi+1] = 0;
			}
			
			m_peakCoord.SetX(xi);
			m_peakCoord.SetY(yi);
			
			peaks.push_back(this->GetRealPeakCoordinates());
			cout << peaks[k] << endl;
		}
		
		k++;
	}
	return peaks;
}


bool CHoughSpace2D::CellResetTest(int x, int y, unsigned int from_value, DIRECTION direction)
{
	int noise_tol = 10;
	if (0 <= x && x < (int)this->GetWidth() &&
		0 <= y && y < (int)this->GetHeight())
	{        // range test
		int this_value = this->operator[](x)[y];
		if ((int)m_threshold < this_value &&
			(int)(from_value - this_value) >= - noise_tol)
		{														// gradient test
			this->operator[](x)[y] = 0;						// cell reset
			switch(direction)									// recursive call to neighbours
			{
			case NORTH:
				CellResetTest(x, y-1, this_value, NORTH);
				return true;
			case SOUTH:
				CellResetTest(x, y+1, this_value, SOUTH);
				return true;
			case WEST:
				CellResetTest(x-1, y, this_value, WEST);
				return true;
			case EAST:
				CellResetTest(x+1, y, this_value, EAST);
				return true;
			case NORTHWEST:
				CellResetTest(x,   y-1, this_value, NORTH);
				CellResetTest(x-1, y,   this_value, WEST);
				CellResetTest(x-1, y-1, this_value, NORTHWEST);
				return true;
			case NORTHEAST:
				CellResetTest(x,   y-1, this_value, NORTH);
				CellResetTest(x+1, y,   this_value, EAST);
				CellResetTest(x+1, y-1, this_value, NORTHEAST);
				return true;
			case SOUTHWEST:
				CellResetTest(x,   y+1, this_value, SOUTH);
				CellResetTest(x-1, y,   this_value, WEST);
				CellResetTest(x-1, y+1, this_value, SOUTHWEST);
				return true;
			case SOUTHEAST:
				CellResetTest(x,   y+1, this_value, SOUTH);
				CellResetTest(x+1, y,   this_value, EAST);
				CellResetTest(x+1, y+1, this_value, SOUTHEAST);
				return true;
			default:
				return false;
			}
		} else return false;
		
	} else return false;
}


vector<CParameter2D<float> > CHoughSpace2D::FindPeaks_Butterfly()
{
	vector<CParameter2D<float> > peaks;
	CArray2D<float> btf(3,3);
	
	float l = m_stepValue.GetY()/sin(m_stepValue.GetX());
	float L = (float)m_threshold;
	float v = -(2.0f*l+L)/2.0f;
	float denum = L;
	
	l /= denum;
	L /= denum;
	v /= denum;
	
	btf.Flush(0.0f);
	
	btf[1][0] = v;
	btf[0][1] = l;
	btf[1][1] = L;
	btf[2][1] = l;
	btf[1][2] = v;
	
	CHoughSpace2D newspace(this->GetWidth(), this->GetHeight());
	this->Convolve(newspace, btf);
	
	for (int y = 0; y < (int)newspace.GetHeight(); y++)
	{
		for (int x = 0; x < (int)newspace.GetWidth(); x++)
		{
			if (newspace[x][y] == 1)
			{
				m_peakCoord.Set(x, y);
				peaks.push_back(this->GetRealPeakCoordinates());
			}
		}
	}
	
	return peaks;
}


vector<CParameter2D<float> > CHoughSpace2D::FindPeaks_LocalMaxima()
{
	vector<CParameter2D<float> > peaks;
	int x, y, dx, dy, v/*, ddx, ddy*/;
	for (y=1; y<m_lineQuante.GetY()-1; y++)
	{
		for (x=1; x<m_lineQuante.GetX()-1; x++)
		{
			if (this->operator[](x)[y] > m_threshold)
			{
				dx = this->operator[](x+1)[y]-this->operator[](x-1)[y];
				dy = this->operator[](x)[y+1]-this->operator[](x)[y-1];
				//ddx = this->operator[](x+1)[y] - 2*this->operator[](x)[y] + this->operator[](x-1)[y];
				//ddy = this->operator[](x)[y+1] - 2*this->operator[](x)[y] + this->operator[](x)[y-1];
				v = abs(dx)-abs(dy);
				if (abs(v)<2 && this->operator[](x+1)[y]<=this->operator[](x)[y] && this->operator[](x-1)[y]<=this->operator[](x)[y])
				{
					m_peakCoord.Set(x,y);
					//cout << this->GetRealPeakCoordinates() << endl;
				}
			}
		}
	}
	return peaks;
}


void CHoughSpace2D::Convolve(CHoughSpace2D& Newspace, const CArray2D<float>& Filter)
{
	int x, y, i, j;
	int n = Filter.GetWidth();
	int nn = n/2;
	int ymax = this->GetHeight() - nn;
	int xmax = this->GetWidth() - nn;
	float sum;
	
	for (y=nn; y<ymax; y++)
	{
		for (x=nn; x<xmax; x++)
		{
			sum = 0.0f;
			for (j=-nn; j<=nn; j++)
			{
				for (i=-nn; i<=nn; i++)
				{
					sum += this->operator[](x+i)[y+j]*Filter[i+nn][j+nn];
				}
			}
			if (sum <= (float)m_threshold)
			{ 
				sum = 0;
			}
			else {
				sum = 1;
			}
			Newspace[x][y] = (unsigned int) sum;
		}
	}
	
	CImage img;
	Newspace.ToImage(img);
	img.Save("butterfly.bmp");
}


vector<CParameter2D<float> > CHoughSpace2D::FindPeaks_Iteration()
{
	vector<CParameter2D<float> > peaks;
	CParameter2D<int> p;
	CParameter2D<float> g;
	for (unsigned int y=0; y<this->GetHeight(); y++)
	{
		for (unsigned int x=0; x<this->GetWidth(); x++)
		{
			p.SetX(x);
			p.SetY(y);
			// g = Gradient(p.GetX(), p.GetY(), 1);
			// if(g.GetX() != 0.0f && g.GetY() != 0.0f)
			// printf("pos=(%i,%i) g=(%6.3f,%6.3f)\n", x, y, g.GetX(), g.GetY());
			;
		}
	}
	return peaks;
}

} // end namespace ipl

