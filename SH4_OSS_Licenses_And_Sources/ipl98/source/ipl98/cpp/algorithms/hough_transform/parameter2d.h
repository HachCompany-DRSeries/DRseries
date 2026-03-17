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

#ifndef _IPL98_PARAMETER2D_H
#define _IPL98_PARAMETER2D_H


#include "../../../ipl98_setup.h" /* always include the setup file */
#include "../../../../points/point2d.h"
#include <iostream>

using std::cout;
using std::endl;

/** Template class for two dimensional parameter. Derived from CPoint2D.
    This class can be used to describe normal parameterization method (rho=x cos(th)+y sin(th))
    and slope interception method (y=ax+b). Used by the Hough classes.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CParameter2D ipl98/cpp/algorithms/hough_transform/parameter2d.h
    @author Qinyin Zhou <qinyin@mip.sdu.dk>
    @version 0.99
*/
template<class T>class CParameter2D : public CPoint2D<T>
{
public:
	/** Default constructor. */
	inline CParameter2D();

   /** @name CParameter2D Constructs a parameter object with specified values.
       @param x Value for the 1st parameter.
       @param y Value for the 2nd parameter. */
   inline CParameter2D(const T x, const T y);

   /** Destructor. */
   ~CParameter2D();

	/** Computes intersection points of 2 lines described using normal parametrixaton.
		@param line Normal parameter set for the line, (theta,rho).
		@return Intersection points between this line and the input line. */
	inline CPoint2D<float> NormalLineIntersection(const CParameter2D<T>& line) const;
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


template<class T> inline CParameter2D<T>::CParameter2D() : CPoint2D<T>()
{
}


template<class T> inline CParameter2D<T>::CParameter2D(const T x, const T y) : CPoint2D<T>(x, y)
{
}


template<class T> inline CParameter2D<T>::~CParameter2D()
{
}


//////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////


template<class T> 
inline CPoint2D<float> CParameter2D<T>::NormalLineIntersection(const CParameter2D<T>& line) const
{
	float x, y, denum;
	CPoint2D<float> point;
	T t1, t2, r1, r2;
	t1 = this->GetX();
	r1 = this->GetY();
	t2 = line.GetX();
	r2 = line.GetY();
	
	// cout << "params: " << *this << " " << line << endl;
	// Parallel case   
	// cout << endl << "t1 og t2 og abs: " << t1 << " " << t2 << " " << fabs(t1-t2) << endl;
	
	if (fabs(t1-t2) <= 1.0e-06) 
	{
		point.Set(0.0f, 0.0f);
	} 
	else 
	{
		denum = -cos(t2)*sin(t1)+cos(t1)*sin(t2);
		x = -(r2*sin(t1)-r1*sin(t2))/denum;
		y = -(r1*cos(t2)-r2*cos(t1))/denum;
		point.Set(x,y);
	}
	return point;
}


#endif // _IPL98_PARAMETER2D_H
