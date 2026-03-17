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

#ifndef _IPL98_ARRAY2DIT_H
#define _IPL98_ARRAY2DIT_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../ipl98_types.h"
#include <iostream>
#include <valarray>
#include <algorithm>
#include "array2d_const_it.h"

// Forward declarations of 'template friend' functions.
namespace ipl 
{
    template<class T>
    class CArray2DIterator;

    template<class T>
    bool operator==(
        const CArray2DIterator<T>& p,
        const CArray2DIterator<T>& q);

    template<class T>
    bool operator!=(
        const CArray2DIterator<T>& p,
        const CArray2DIterator<T>& q);

    template<class T>
    bool operator<(
        const CArray2DIterator<T>& p,
        const CArray2DIterator<T>& q);

    template<class T>
    bool operator<=(
        const CArray2DIterator<T>& p,
        const CArray2DIterator<T>& q);

    template<class T>
    bool operator>(
        const CArray2DIterator<T>& p,
        const CArray2DIterator<T>& q);

    template<class T>
    bool operator>= (
        const CArray2DIterator<T>& p,
        const CArray2DIterator<T>& q);
}

namespace ipl{ // use namespace if C++

using std::slice;
using std::valarray;

/** @ingroup iteratorclasses
@{ */

/** Iterator class to access data from the class CArray2D, last updated 3/4/2003.
	This class is not complete, only a few operators are overloaded. But it can
	be used as a basic iterator where the iterator is incremented with the ++
	operator. See example in CArray2D class.
	This class is build	from the basic example in C++ Programming Language Third 
	Edition p. 672 by Bjarne Stroustrup. Some of the code has been moved inside the
	declaration of the class, since some compilers have problems inlining the code
	if it is not shown at that early point.
	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CArray2DIterator ipl98/cpp/arrays/array2d_it.h
	@see CArray2DConstIterator
	@see CArray2D
	@version 0.51,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
template<class T>
class CArray2DIterator
{
protected: // attributes
	/** Direct pointer to data. */
    valarray<T>* m_pData;
	/** A slice of m_pData. */
    slice m_s;
	/** Index of current element. */
    UINT32 m_Curr;
public:
	/// Constructor
	CArray2DIterator(valarray<T>* vv, slice ss, UINT32 Curr=0) :m_pData(vv), m_s(ss), m_Curr(Curr) { }

	/// Virtual destructor
	virtual ~CArray2DIterator() {}

	/// Cast to corresponding const iterator in mixed expressions
	inline operator CArray2DConstIterator<T>(){return CArray2DConstIterator<T>(m_pData, m_s, m_Curr);}
	/// overloading = operator
	inline CArray2DIterator& operator=(const CArray2DIterator& It);
	/** Returns an iterator to the first element in current row or column.
		@return First element. */
	inline CArray2DIterator Begin() const;
	/** Returns an iterator to the last element plus one in current row or column.
		@return Last element plus one. */
	inline CArray2DIterator End() const;
	/** overloading ++ operator. Increments the pointer to data. */
	inline CArray2DIterator& operator++() { m_Curr++; return *this; }
	/** overloading ++ operator. Increments the pointer to data. Note: This post fix operator
		is slower than using the prefix operator! */
	inline CArray2DIterator operator++(int) { CArray2DIterator t = *this; m_Curr++; return t; }
	/** overloading -- operator. Decrements the pointer to data. */
	inline CArray2DIterator& operator--() { m_Curr--; return *this; }
	/** overloading -- operator. Decrements the pointer to data. Note: This post fix operator
		is slower than using the prefix operator! */
	inline CArray2DIterator operator--(int) { CArray2DIterator t = *this; m_Curr--; return t; }
	/** overloading += operator. Increments pointer to data by Value. */
	inline CArray2DIterator& operator +=(int Value){ m_Curr+=Value; return *this;}
	/** overloading -= operator. Decrements pointer to data by Value. */
	inline CArray2DIterator& operator -=(int Value){ m_Curr-=Value; return *this;}
	/** overloading the dereference operator, typically used when iterating. */
	inline T& operator*(){return (*m_pData)[m_s.start()+m_Curr*m_s.stride()];}
	/** overloading [] operator. */
	virtual inline T& operator[](UINT32 i) {return (*m_pData)[m_s.start()+i*m_s.stride()];}
	/** overloading == operator.
		@return true, if iterator p and q points to same element in same column. */	
	friend bool operator== __IPL_TF(const CArray2DIterator& p, const CArray2DIterator& q);
	/** overloading != operator.
		@return true, if iterator p and q points to different elements. */
	friend bool operator!= __IPL_TF(const CArray2DIterator& p, const CArray2DIterator& q);
	/** overloading < operator.
		@return true, if iterator p and q points to same column and p has row index < q's row index. */
	friend bool operator< __IPL_TF(const CArray2DIterator& p, const CArray2DIterator& q);
	/** overloading <= operator.
		@return true, if iterator p and q points to same element in same column. */
	friend bool operator<= __IPL_TF(const CArray2DIterator& p, const CArray2DIterator& q);
	/** overloading > operator.
		@return true, if iterator p and q points to different elements. */
	friend bool operator> __IPL_TF(const CArray2DIterator& p, const CArray2DIterator& q);
	/** overloading >= operator.
		@return true, if iterator p and q points to same column and p has row index < q's row index. */
	friend bool operator>= __IPL_TF(const CArray2DIterator& p, const CArray2DIterator& q);
};


template<class T>
inline CArray2DIterator<T>& CArray2DIterator<T>::operator=(const CArray2DIterator<T>& It)
{
	m_pData=It.m_pData;
	m_s=It.m_s;
	m_Curr=It.m_Curr;
	return *this;
}

template<class T>
inline CArray2DIterator<T> CArray2DIterator<T>::Begin() const
{
	CArray2DIterator t = *this;
	t.m_Curr = 0; // index of first element
	return t;
}

template<class T>
inline CArray2DIterator<T> CArray2DIterator<T>::End() const
{
	CArray2DIterator<T> t = *this;
	t.m_Curr = m_s.size();      // index of last-plus-one element
	return t;
}

template<class T>
bool operator==(const CArray2DIterator<T>& p, const CArray2DIterator<T>& q)
{
	return p.m_Curr==q.m_Curr
		&& p.m_s.stride()==q.m_s.stride()
		&& p.m_s.start()==q.m_s.start();
}

template<class T>
bool operator!=(const CArray2DIterator<T>& p, const CArray2DIterator<T>& q)
{
	return !(p==q);
}

template<class T>
bool operator<(const CArray2DIterator<T>& p, const CArray2DIterator<T>& q)
{
	return p.m_Curr<q.m_Curr
		&& p.m_s.stride()==q.m_s.stride()
		&& p.m_s.start()==q.m_s.start();
}

template<class T>
bool operator<=(const CArray2DIterator<T>& p, const CArray2DIterator<T>& q)
{
	return p.m_Curr<=q.m_Curr
		&& p.m_s.stride()==q.m_s.stride()
		&& p.m_s.start()==q.m_s.start();
}

template<class T>
bool operator>(const CArray2DIterator<T>& p, const CArray2DIterator<T>& q)
{
	return p.m_Curr>q.m_Curr
		&& p.m_s.stride()==q.m_s.stride()
		&& p.m_s.start()==q.m_s.start();
}

template<class T>
bool operator>= (const CArray2DIterator<T>& p, const CArray2DIterator<T>& q)
{
	return p.m_Curr>=q.m_Curr
		&& p.m_s.stride()==q.m_s.stride()
		&& p.m_s.start()==q.m_s.start();
}

/*@}*/ /* end group "iteratorclasses" */

} // end namespace ipl

#endif //_IPL98_ARRAY2DIT_H
