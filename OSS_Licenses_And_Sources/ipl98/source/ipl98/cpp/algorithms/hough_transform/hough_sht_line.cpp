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

#include "hough_sht_line.h"

namespace ipl {

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CHoughSHTLine::CHoughSHTLine(ostream& os, bool WriteOutputFiles, const string& FilePrefix) : m_os(os)
{
	m_initialized = false;
	m_writeOutputFiles = WriteOutputFiles;
	m_filePrefix = FilePrefix;
}


CHoughSHTLine::~CHoughSHTLine()
{
}


//////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////


bool CHoughSHTLine::Initialize(COLORTYPE Color, UINT8 Threshold, CHoughBase::ORIGINTYPE OriginType, 
	CHoughBase::PARAMETERTYPE ParamType)
{
	if (Color == HIGHCOLOR) m_color = HIGHCOLOR;
	else m_color = LOWCOLOR;
	m_imgThreshold = Threshold;
	m_originType = OriginType;
	m_paramType = ParamType;
	m_initialized = true;
	
	return true;
}


bool CHoughSHTLine::PerformTransform(const CStdImage& Source, int Thickness)
{
	if (!m_initialized)
	{
		ostringstream ost;
		ost << "CHoughSHTLine::StandardHoughTransform() Not initialized" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	if ((Source.GetBits()!=1) && (Source.GetBits()!=8))
	{
		ostringstream ost;
		ost << "CHoughSHTLine::StandardHoughTransform() Source must be 1 or 8 b/p (current is " 
			<< Source.GetBits() << ")" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	this->UpdateHoughSpace(Source, Thickness);
	
	m_os << "Performing standard Hough transform..." << endl;
	m_os << "Finished procentage:" << endl;
	int p;
	int width = Source.GetWidth();
	int height = Source.GetHeight();
	int x, y, xi, yi;
	int procent = 0, procent_old = 0;
	bool color = false;
	if (m_color == HIGHCOLOR) color = true;
	
	for (y=0; y<height; y++)
	{
		// compute finished procentage
		procent = (int) (100.0*y/height);
		if (procent > procent_old)
		{
			m_os << "#";
			procent_old = procent;
		}
		
		yi = m_imgOriginOrientation.GetY()*(y-m_imgOriginPosition.GetY());
		for(x=0; x<width; x++)
		{
			p = Source.GetPixelFast(x,y);         
			if ((color)^(p>m_imgThreshold))
			{    // Used for both low/high color images
				xi = m_imgOriginOrientation.GetX()*(x-m_imgOriginPosition.GetX());
				if (m_paramType == CHoughBase::NORMAL_PARAM) 
					this->ComputeHoughNormal(xi, yi);
				else 
					this->ComputeHoughSlope(xi, yi);
			}
			// else if (m_color && p<m_threshold) this->ComputeHough(x,y);
		}
	}
	
	if (m_writeOutputFiles)
	{
		CImage img;
		this->ToImage(img);
		img.Save(m_filePrefix+"_HS_0.bmp");
	}
	
	m_os << "\nStandard Hough transform for lines finished." << endl;
	return true;
}


CHoughSpace2D CHoughSHTLine::GetHoughSpace() const
{ 
	return m_houghSpace2D;
}


void CHoughSHTLine::ToImage(CStdImage& outImg)
{
	m_houghSpace2D.ToImage(outImg);
}


vector<CParameter2D<float> > CHoughSHTLine::FindPeaks(CHoughBase::PEAKDETECTIONTYPE DetectionMethod, 
	unsigned int HSThreshold)
{
	unsigned int k = 0;
	m_houghSpace2D.SetThreshold(HSThreshold);
	vector<CParameter2D<float> > peaks = m_houghSpace2D.FindPeaks(DetectionMethod);
	if (m_writeOutputFiles)
	{
		CImage img;
		img.Alloc(m_imageDimensions.GetX(), m_imageDimensions.GetY(), 8);
		img.Flush(255);	
		while (k < peaks.size())
		{
			CParameter2D<float> param = peaks[k];
			m_os << "Parameter " << k << ": " << param << endl;      
			CHoughBase::DrawLine(img, param);
			
			string filename = "";
			char no[2];
			sprintf(no, "%i", k);
			filename = m_filePrefix + "_RC_SHT_" + no + ".bmp";
			img.Save(filename);
			k++;
		}
	}
	return peaks;
}


//////////////////////////////////////////////////////////////////////
// Private methods
//////////////////////////////////////////////////////////////////////


void CHoughSHTLine::ComputeSinCosTheta(int n)
{
	m_os << "Computing sine and cosine value vectors..." << endl;
	float T = m_houghSpace2D.GetParameterRangeValue(1,2) - m_houghSpace2D.GetParameterRangeValue(1,1);
	float theta = (float) (-m_houghSpace2D.GetOffset(1)*T/n);
	float step  = (float) (T/n);
	
	m_sinTheta.resize(n);
	m_cosTheta.resize(n);
	for(int i=0; i<n; i++)
	{
		m_sinTheta[i] = (float) sin(theta);
		m_cosTheta[i] = (float) cos(theta);
		theta += step;
	}
}


void CHoughSHTLine::ComputeSlope(int n)
{
	m_os << "Computing slope vector..." << endl;
	float A = m_houghSpace2D.GetParameterRangeValue(1,2) - m_houghSpace2D.GetParameterRangeValue(1,1);
	float step  = (float) (A/n);
	float v = 0.0f;
	int n2 = n/2;
	
	m_slopeVector.resize(n);
	m_slopeVector[n2] = 0.0f;
	for(int i=1; i<n2; i++)
	{
		v += step;
		m_slopeVector[n2+i] = v;
		m_slopeVector[n2-i] = -v;
	}
}


void CHoughSHTLine::ComputeHoughNormal(int x, int y)
{
	float pk;   
	CPoint2D<int> position;
	float rhoMin = m_houghSpace2D.GetParameterRangeValue(2,1);
	float rhoMax = m_houghSpace2D.GetParameterRangeValue(2,2);
	int param1Q = m_houghSpace2D.GetQuantizationValue(1);
	int param2Q = m_houghSpace2D.GetQuantizationValue(2);
	
	for (int k=0; k<param1Q; k++)
	{
		pk = (float) (x*m_cosTheta[k] + y*m_sinTheta[k]);     // kernel part
		if (!(rhoMin>=pk || pk>rhoMax))
		{
			int r = m_houghSpace2D.Discretize(2, pk);
			if (0<=r && r<param2Q)
			{
				position.Set(k,r);
				m_houghSpace2D.Inc(position);
			} 
		}
		else
		{
			ostringstream ost;
			ost << "CHoughSHTLine::ComputeHoughNormal() Out of range error" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
		}
	}
}


void CHoughSHTLine::ComputeHoughSlope(int x, int y)
{
	float bk;
	CPoint2D<int> position;
	float bMin = m_houghSpace2D.GetParameterRangeValue(2,1);
	float bMax = m_houghSpace2D.GetParameterRangeValue(2,2);
	int param1Q = m_houghSpace2D.GetQuantizationValue(1);
	int param2Q = m_houghSpace2D.GetQuantizationValue(2);
	
	for (int k=0; k<param1Q; k++)
	{
		bk = (float) (-m_slopeVector[k]*x + y);
		if (!(bMin>=bk || bk>bMax))
		{
			int b = m_houghSpace2D.Discretize(2, bk);
			if (0<=b && b<param2Q)
			{
				position.Set(k,b);
				m_houghSpace2D.Inc(position);
			} 
		}
	}
}


void CHoughSHTLine::SetOrigin(CHoughBase::ORIGINTYPE OriginType)
{
	CPoint2D<int> P,O;
	switch(OriginType)
	{
	case CHoughBase::LEFT_UPPER:
		P.Set(0,0);
		O.Set(1,1);
		break;
	case CHoughBase::CENTER_UP:
		P = m_imageDimensions/2;
		O.Set(1,-1);
		break;
	case CHoughBase::CENTER_DOWN:
		P = m_imageDimensions/2;
		O.Set(1,1);
		break;
	default:
		P = m_imageDimensions/2;
		O.Set(1,-1);
		break;
	}
	m_imgOriginPosition = P;
	m_imgOriginOrientation = O;
}


void CHoughSHTLine::UpdateHoughSpace(const CStdImage& Source, int Thickness)
{
	m_imageDimensions.Set(Source.GetWidth(),Source.GetHeight());
	SetOrigin(m_originType);
	// last parameter (100) below not used, always reset in FindPeaks()
	m_houghSpace2D.Initialize(Source.GetWidth(), Source.GetHeight(), Thickness, 100, m_paramType);
	
	switch (m_paramType)
	{
	case CHoughBase::SLOPE_PARAM:
		this->ComputeSlope(m_houghSpace2D.GetQuantizationValue(1));
		break;
	case CHoughBase::NORMAL_PARAM:
		this->ComputeSinCosTheta(m_houghSpace2D.GetQuantizationValue(1));
		break;
	}
}


} // end namespace ipl
