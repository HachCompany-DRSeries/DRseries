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

#include "sparse_hough_space2d.h"
#include <algorithm>

namespace ipl {
	
using std::lower_bound;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSparseHoughSpace2D::CSparseHoughSpace2D() 
{ 
}


CSparseHoughSpace2D::CSparseHoughSpace2D(ostream& os, int ImageWidth, int ImageHeight, int Thickness,
	int Threshold, CHoughBase::PARAMETERTYPE ParamType)
{
	this->Initialize(ImageWidth, ImageHeight, Thickness, Threshold, ParamType);
}


CSparseHoughSpace2D::~CSparseHoughSpace2D()
{
}


//////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////


bool CSparseHoughSpace2D::Initialize(int ImageWidth, int ImageHeight, int Thickness, int Threshold, 
	CHoughBase::PARAMETERTYPE ParamType)
{
	ComputeQuantization(ImageWidth, ImageHeight, Thickness, ParamType);
	m_space.resize(GetQuantizationValue(1));
	SetThreshold(Threshold);
	return true;
}


bool CSparseHoughSpace2D::Inc(const CPoint2D<int>& Position)
{
	CPoint2D<int> newCell(Position.GetY(), 1);
	str_list<CPoint2D<int> > y_list = (str_list<CPoint2D<int> >) m_space[Position.GetX()];
	str_list<CPoint2D<int> >::iterator p = lower_bound(y_list.begin(), y_list.end(), newCell);
	
	if (p->GetX() == Position.GetY())
	{
		p->AddY(1);
		
		if (p->GetY() > (int)GetThreshold())
		{
			SetPeakCoordinates(Position);
			return true;
		}
	} 
	else 
		y_list.insert(p, newCell); 
	
	m_space[Position.GetX()] = y_list;
	return false;
}


bool CSparseHoughSpace2D::Alloc()
{ 
	for(unsigned int i=0; i<m_space.size(); i++)
		((list<CPoint2D<int> >) m_space[i]).clear();
	return true; 
}


void CSparseHoughSpace2D::ToImage(CStdImage& OutputImg)
{
	str_list<CPoint2D<int> > y_list;
	CPoint2D<int> cell;
	str_list<CPoint2D<int> >::iterator itY;
	unsigned int x_coord, y_coord;
	OutputImg.AllocFast(GetQuantizationValue(1), GetQuantizationValue(2), 8);
	OutputImg.Flush(0);
	
	for (x_coord=0; x_coord<m_space.size(); x_coord++)
	{
		
		y_list = (str_list<CPoint2D<int> >) m_space[x_coord];
		for (itY=y_list.begin(); itY!=y_list.end(); itY++)
		{
			cell = (CPoint2D<int>) *itY;
			y_coord = cell.GetX();
			//cout << x_coord << " " << y_coord << " " << cell.GetY() << endl;
			OutputImg.SetPixel(x_coord, y_coord, cell.GetY());
		}
	}
	
	CHoughBase::ScaleHoughImage(OutputImg);
}


//////////////////////////////////////////////////////////////////////
// Private methods
//////////////////////////////////////////////////////////////////////

} // end namespace ipl
