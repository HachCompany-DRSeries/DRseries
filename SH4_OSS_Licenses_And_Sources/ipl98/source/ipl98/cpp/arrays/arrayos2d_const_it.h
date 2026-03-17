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

#ifndef _IPL98_ARRAYOS2DCONSTIT_H
#define _IPL98_ARRAYOS2DCONSTIT_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include<iostream>
#include<valarray>
#include<algorithm>
#include "array2d_const_it.h"

namespace ipl{ // use namespace if C++

/** @ingroup iteratorclasses
@{ */

/** Iterator class to access data from the class CArrayOS2D, last updated 3/4/2003.
	This class is not complete, only a few operators are overloaded. But it can
	be used as a basic iterator where the iterator is incremented with the ++
	operator. See example in CArrayOS2D class.
	This class is build	from the basic example in C++ Programming Language Third 
	Edition p. 672 by Bjarne Stroustrup. Some of the code has been moved inside the
	declaration of the class, since some compilers have problems inlining the code
	if it is not shown at that early point.
	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CArrayOS2DConstIterator ipl98/cpp/arrays/arrayos2d_const_it.h
	@see CArrayOS2DIterator
	@see CArrayOS2D
	@version 0.51,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
template<class T>
class CArrayOS2DConstIterator : public CArray2DConstIterator<T>
{
private: // attributes
	/** The granularity of the array, see attribute m_SubIndexing in class CArrayOS2D
		for more info.
		@see CArrayOS2D */
	unsigned int m_SubIndexing;
	/** Reference to origo in the array, see m_Origo in class CArrayOS2D
		for more info.
		@see CArrayOS2D */
	const CPoint2D<double>* m_Origo;
public:
	/// Constructor
	CArrayOS2DConstIterator(valarray<T>* vv, slice ss, unsigned int SubIndexing, const CPoint2D<double>* Origo, UINT32 Curr=0) :
									CArray2DConstIterator<T>(vv,ss,Curr), m_SubIndexing(SubIndexing), m_Origo(Origo){}
	/// overloading = operator
	inline CArrayOS2DConstIterator& operator=(const CArrayOS2DConstIterator& It);
	/** Returns an iterator to the first element in current row or column.
		@return First element. */
    inline CArrayOS2DConstIterator Begin() const;
	/** Returns an iterator to the last element plus one in current row or column.
		@return Last element plus one. */
	inline CArrayOS2DConstIterator End() const;
	/** overloading ++ operator. Increments the pointer to data. */
	inline CArrayOS2DConstIterator& operator++() { this->m_Curr++; return *this; }
	/** overloading ++ operator. Increments the pointer to data. Note: This post fix operator
		is slower than using the prefix operator! */
	CArrayOS2DConstIterator operator++(int) { CArrayOS2DConstIterator t = *this; this->m_Curr++; return t; }
	/** overloading -- operator. Decrements the pointer to data. */
	inline CArrayOS2DConstIterator& operator--() { this->m_Curr--; return *this; }
	/** overloading -- operator. Increments the pointer to data. Note: This post fix operator
		is slower than using the prefix operator! */
	CArrayOS2DConstIterator operator--(int) { CArrayOS2DConstIterator t = *this; this->m_Curr--; return t; }
	/** overloading += operator. Increments pointer to data by Value. */
	inline CArrayOS2DConstIterator& operator +=(int Value){ this->m_Curr+=Value; return *this;}
	/** overloading -= operator. Decrements pointer to data by Value. */
	inline CArrayOS2DConstIterator& operator -=(int Value){ this->m_Curr-=Value; return *this;}
	/** overloading [] operator. */
	inline const T& operator[](double i) const { return (*this->m_pData)[(size_t)(this->m_s.start()+(i+m_Origo->GetY())*m_SubIndexing*this->m_s.stride())];}
};

template<class T>
inline CArrayOS2DConstIterator<T>& CArrayOS2DConstIterator<T>::operator=(const CArrayOS2DConstIterator<T>& It)
{
	CArray2DConstIterator<T>::operator=(It);
	m_SubIndexing=It.m_SubIndexing;
	m_Origo=It.m_Origo;
	return *this;
}

template<class T>
inline CArrayOS2DConstIterator<T> CArrayOS2DConstIterator<T>::Begin() const
{
	CArrayOS2DConstIterator<T> t = *this;
	t.m_Curr = 0; // index of first element
	return t;
}

template<class T>
inline CArrayOS2DConstIterator<T> CArrayOS2DConstIterator<T>::End() const
{
	CArrayOS2DConstIterator<T> t = *this;
	t.m_Curr = this->m_s.size();
	return t;
}

/*@}*/ /* end group "iteratorclasses" */

} // end namespace ipl

#endif //_IPL98_ARRAYOS2DCONSTIT_H
