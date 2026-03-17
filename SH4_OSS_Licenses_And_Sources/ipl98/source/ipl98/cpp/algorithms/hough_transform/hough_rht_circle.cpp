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

#include "hough_rht_circle.h"
#include <algorithm>
#include <time.h>

namespace ipl { 

using std::find;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CHoughRHTCircle::CHoughRHTCircle() : m_os(cout)
{
}


CHoughRHTCircle::CHoughRHTCircle(ostream& os, bool WriteOutputFiles, const string& FilePrefix) : m_os(os)
{
	m_filePrefix = FilePrefix;
	m_writeOutputFiles = WriteOutputFiles;
}


CHoughRHTCircle::~CHoughRHTCircle()
{
}


//////////////////////////////////////////////////////////////////////
// Public Methods
//////////////////////////////////////////////////////////////////////


bool CHoughRHTCircle::Initialize(const CStdImage& Source, COLORTYPE Color, UINT8 Threshold, 
                                 CHoughBase::ORIGINTYPE OriginType)
{
	m_bImage = CHoughBase::CBinaryImage(Source, Color, Threshold, OriginType);
	return this->InitializeCommon();
}


bool CHoughRHTCircle::Initialize(const CHoughBase::CBinaryImage& Source, CHoughBase::ORIGINTYPE OriginType)
{
	m_bImage = Source;
	return this->InitializeCommon();
}


vector<CPoint3D<int> > CHoughRHTCircle::PerformTransform(int Threshold)
{
	int numofShapes=0, k=m_bImage.GetSize()*Threshold;
	bool detected;
	
	CImage outImage1, outImage2;
	outImage1.Alloc(m_bImage.GetWidth(), m_bImage.GetHeight(), 8);
	outImage1.Flush(255);
	m_space.SetThreshold(Threshold);
	
	srand( (unsigned) time( NULL ) );
	
	while(k>0 && numofShapes<15 && m_bImage.GetSize()>Threshold*3)
	{
		this->GetPixelTriple();
		CPoint3D<float> P3d(this->RH_Circle_Transform()); // has to use a tempary variable to get rid of warning from gcc
		CPoint3D<int> parameter((int)P3d.GetX(),(int)P3d.GetY(),(int)P3d.GetZ()); // changed to CPoint3D<int> from float by edr
		//parameter.Set((int)P3d.GetX(),(int)P3d.GetY(),(int)P3d.GetZ());
		CPoint3D<float> parameter2(parameter);  // inserted parameter2 copy as CPoint3D<float> by edr
		
		if (m_space.ParameterCheck(parameter))
		{
			detected = m_space.Inc(parameter);
			
			if (detected) {
				std::vector<CPoint3D<int> >::iterator p;
				p = find(m_peaks.begin(), m_peaks.end(), parameter);
				if (p == m_peaks.end())
				{
					m_peaks.push_back(parameter);
					numofShapes++;
					cout << "Circle " << numofShapes << ": " << parameter << endl;
					// parameter = m_space.GetRealPeakCoordinates();
					m_bImage.RemoveCircleRange(parameter2,3); // changed to use parameter2 copy by edr
					
					if (m_writeOutputFiles) WriteFiles(outImage1, parameter2, numofShapes); // changed to use parameter2 copy by edr
				}
				else
				{
					m_bImage.RemoveCircleRange(parameter2,5); // changed to use parameter2 copy by edr
				}
				m_space.Alloc();
			}
		}
		k--;
	}
	return m_peaks;
}


//////////////////////////////////////////////////////////////////////
// Private Methods
//////////////////////////////////////////////////////////////////////


bool CHoughRHTCircle::InitializeCommon()
{
	m_space.Alloc(m_bImage.GetWidth(), m_bImage.GetHeight());
	//   m_space = CSparseHoughSpace3D(m_bImage.GetWidth(), m_bImage.GetHeight());
	return true;
}


void CHoughRHTCircle::GetPixelTriple()
{
	
	int index1, index2, index3;
	index1 = Round(1.0f*rand()/RAND_MAX*m_bImage.GetSize());   // get 1st index
	index2 = Round(1.0f*rand()/RAND_MAX*m_bImage.GetSize());   // get 2nd index
	index3 = Round(1.0f*rand()/RAND_MAX*m_bImage.GetSize());   // get 3rd index
	m_p1 = m_bImage.GetPixel(index1);  
	m_p2 = m_bImage.GetPixel(index2);  
	m_p3 = m_bImage.GetPixel(index3);  
	
	if (this->DistanceCheck())
	{
	}
	else
	{
		this->GetPixelTriple();
	}
}


CPoint3D<float> CHoughRHTCircle::RH_Circle_Transform()
{
	int x1, y1, x2, y2, x3, y3, c1, c2, c3, c31, c32, c33, d;
	x1 = m_p1.GetX();
	y1 = m_p1.GetY();
	x2 = m_p2.GetX();
	y2 = m_p2.GetY();
	x3 = m_p3.GetX();
	y3 = m_p3.GetY();
	
	// cout << m_p1 << " " << m_p2 << " " << m_p3 << endl;
	
	d = x1*(y2-y3)+x2*(y3-y1)+x3*(y1-y2);
	if (d != 0)
	{
		c1 = x3*x3*(y1-y2)+(x1*x1+(y1-y2)*(y1-y3))*(y2-y3)+x2*x2*(y3-y1);
		c2 = x1*x1*(x3-x2)-x2*x2*x3+x3*(y1*y1-y2*y2)+x1*(x2*x2-x3*x3+y2*y2-y3*y3)+x2*(x3*x3-y1*y1+y3*y3);
		c31 = ((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
		c32 = ((x1-x3)*(x1-x3)+(y1-y3)*(y1-y3));
		c33 = ((x2-x3)*(x2-x3)+(y2-y3)*(y2-y3));
		
		c1 = c1/2/d;
		c2 = c2/2/d;
		c3 = (int)fabs(sqrt(1.0*c31)*sqrt(1.0*c32)*sqrt(1.0*c33)/2/d);
	}
	
	return CPoint3D<float>((float) c1, (float) c2, (float) c3);
}


bool CHoughRHTCircle::DistanceCheck()
{
	int k = 3;
	return (abs(m_p1.GetX()-m_p2.GetX())>k && abs(m_p1.GetX()-m_p3.GetX())>k && 
		abs(m_p2.GetX()-m_p3.GetX())>k && abs(m_p1.GetY()-m_p2.GetY())>k && 
		abs(m_p1.GetY()-m_p3.GetY())>k && abs(m_p2.GetY()-m_p3.GetY())>k);
}


void CHoughRHTCircle::WriteFiles(CStdImage& outImage1, const CPoint3D<float>& Parameter, int NumOfShapes)
{
	CImage outImage2;
	string filename;
	char no[2];
	sprintf(no, "%i", NumOfShapes);
	
	// Reconstruction files
	filename = "";
	filename += m_filePrefix+"_RC_RHTC_"+no+".bmp";
	// rc_bImg.AddCircle(peak_coordinate_real);
	// rc_bImg.ToImage(outImage);
	CPoint2D<int> center((int) Parameter.GetX(), (int) Parameter.GetY());
	center = m_bImage.OriginConvert(center);
	outImage1.DrawCircle(center, (int) Parameter.GetZ(), 0);
	// outImage.DrawCircle(CPoint2D<int>(100,100), 100, 255);
	// outImage1.SetPixel(100,100,255);
	outImage1.Save(filename);
	
	// Original image files
	filename = "";
	filename += m_filePrefix+"_ORIGINAL_RHTC_"+no+".bmp";
	m_bImage.ToImage(outImage2);
	outImage2.Save(filename);
	
	// Parameter space files
	/*
	filename = "";
	filename += m_filePrefix+"_HS_RHT_"+no+".bmp";
	m_houghSpace2D.ToImage(outImage);
	outImage.Save(filename);
	*/
}

} // end namespace ipl
