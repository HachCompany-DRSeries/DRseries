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

#include "sparse_hough_space3d.h"
#include <algorithm>

namespace ipl {

using std::lower_bound;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSparseHoughSpace3D::CSparseHoughSpace3D()
{
}


CSparseHoughSpace3D::CSparseHoughSpace3D(int ImageWidth, int ImageHeight)
{
	m_circleQuante = this->ComputeQuantization(ImageWidth, ImageHeight);
	CArray2D<str_list<CPoint2D<int> > >::Alloc(m_circleQuante.GetX(), m_circleQuante.GetY());
}


CSparseHoughSpace3D::~CSparseHoughSpace3D()
{
}


//////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////


bool CSparseHoughSpace3D::Inc(const CPoint3D<int>& Position)
{
	int c1, c2, c3;
	c1 = Position.GetX();
	c2 = Position.GetY();
	c3 = Position.GetZ();
	
	c1+=m_offset.GetX();
	c2+=-m_offset.GetY();
	c2=-c2;
	
	CPoint2D<int> newCell(c3, 1);
	str_list<CPoint2D<int> >& c3_list = operator[](c1)[c2];
	str_list<CPoint2D<int> >::iterator p = lower_bound(c3_list.begin(), c3_list.end(), newCell);
	
	if (p->GetX() == c3)
	{
		p->AddY(1);
		if (p->GetY() > m_threshold)
		{
			m_peakCoordinates.SetX(c1);
			m_peakCoordinates.SetY(c2);
			m_peakCoordinates.SetZ(c3);
			operator[](c1)[c2] = c3_list;
			return true;
		}
	}
	else 
		c3_list.insert(p, newCell);
	
	// operator[](c1)[c2] = c3_list;
	return false;
}


bool CSparseHoughSpace3D::ParameterCheck(const CPoint3D<int>& Parameter) const
{
	int c1 = Parameter.GetX();
	int c2 = Parameter.GetY();
	int c3 = Parameter.GetZ();
	if (m_c1ValueRange.GetX() <= c1 && c1 < m_c1ValueRange.GetY())
	{
		if (m_c2ValueRange.GetX() <= c2 && c2 < m_c1ValueRange.GetY())
		{
			if (m_c3ValueRange.GetX() <= c3 && c3 < m_c1ValueRange.GetY())
			{
				return true;
			}
		}
	}
	return false;
}


void CSparseHoughSpace3D::Alloc()
{
	Flush(str_list<CPoint2D<int> >());
}

void CSparseHoughSpace3D::Alloc(int ImageWidth, int ImageHeight)
{
	m_circleQuante = this->ComputeQuantization(ImageWidth, ImageHeight);
	CArray2D<str_list<CPoint2D<int> > >::Alloc(m_circleQuante.GetX(), m_circleQuante.GetY());
	Alloc();
}


//CPoint2D<int> CSparseHoughSpace3D::GetOffset() const { return m_offset; }

// return value changed to CPoint3D<float> from int by edr
/*CPoint3D<float> CSparseHoughSpace3D::GetRealPeakCoordinates() const {
CPoint3D<float> realCoord;
realCoord.SetX(m_offset.GetX()+m_peakCoordinates.GetX());
realCoord.SetY(m_offset.GetY()-m_peakCoordinates.GetY());
realCoord.SetZ(m_peakCoordinates.GetZ());
return realCoord;
} */


void CSparseHoughSpace3D::SetThreshold(int Threshold)
{
	m_threshold = Threshold;
}


//////////////////////////////////////////////////////////////////////
// Private methods
//////////////////////////////////////////////////////////////////////


CPoint3D<int> CSparseHoughSpace3D::ComputeQuantization(int ImageWidth, int ImageHeight)
{
	int c1 = ImageWidth;
	int c2 = ImageHeight;
	int c3 = (int) sqrt((double)c1*c1+c2*c2);
	
	m_c1ValueRange.SetX(-c1/2);
	m_c1ValueRange.SetY(c1/2);
	m_c2ValueRange.SetX(-c2/2);
	m_c2ValueRange.SetY(c2/2);
	m_c3ValueRange.SetX(0);
	m_c3ValueRange.SetY(c3);
	
	m_offset.SetX(c1/2);
	m_offset.SetY(c2/2);
	
	return CPoint3D<int>(c1,c2,c3);
}

} // end namespace ipl
