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

#include <iostream>
#include <math.h>

template <class T>
CPoint2D<T>::CPoint2D()
{
	// default constructor
	m_x=0;
	m_y=0;
}

template <class T>
CPoint2D<T>::CPoint2D(T x,T y)
{
	// constr. with init.
	m_x=x;
	m_y=y;
}

//template <class T>
//CPoint2D<T>::CPoint2D(const T2DInt& P2D)
//{
//	// copy constructor
//	m_x=P2D.x;
//	m_y=P2D.y;
//}

template <class T>
CPoint2D<T>::CPoint2D(const CPoint2D<T>& P2D)
{
	// copy constructor
	*this=P2D;
}

template <class T>
CPoint2D<T>::~CPoint2D()
{
	//destructor
}

template <class T>
void CPoint2D<T>::Print() const
{
	// prints value of point to cout
	std::cout << "point=(" << m_x << "," << m_y << ")" << std::endl;
}

