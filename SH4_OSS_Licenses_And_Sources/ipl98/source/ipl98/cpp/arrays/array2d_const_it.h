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

#ifndef _IPL98_ARRAY2DCONSTIT_H
#define _IPL98_ARRAY2DCONSTIT_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../ipl98_types.h"
#include <iostream>
#include <valarray>
#include <algorithm>

// Forward declarations of 'template friend' functions.
namespace ipl 
{
    template<class T>
    class CArray2DConstIterator;
    
    template<class T>
    bool operator==(
        const CArray2DConstIterator<T>& p,
        const CArray2DConstIterator<T>& q);

    template<class T>
    bool operator!=(
        const CArray2DConstIterator<T>& p,
        const CArray2DConstIterator<T>& q);

    template<class T>
    bool operator<(
        const CArray2DConstIterator<T>& p,
        const CArray2DConstIterator<T>& q);

    template<class T>
    bool operator<=(
        const CArray2DConstIterator<T>& p,
        const CArray2DConstIterator<T>& q);

    template<class T>
    bool operator>(
        const CArray2DConstIterator<T>& p,
        const CArray2DConstIterator<T>& q);

    template<class T>
    bool operator>= (
        const CArray2DConstIterator<T>& p,
        const CArray2DConstIterator<T>& q);
}

namespace ipl{ // use namespace if C++

using std::slice;
using std::valarray;

/** @defgroup iteratorclasses Iterators (C++)
		The iterator classes are optimized iterators used with
		the corresponding container classes. If you are not familiar with 
		iterators, I suggest you read about iterators in the standard library. */

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
	\class CArray2DConstIterator ipl98/cpp/arrays/array2d_const_it.h
	@see CArray2DIterator
	@see CArray2D
	@version 0.51,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
template<class T>
class CArray2DConstIterator 
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
    CArray2DConstIterator(valarray<T>* vv, slice ss, UINT32 Curr=0): m_pData(vv), m_s(ss), m_Curr(Curr){}
	/// overloading = operator
	inline CArray2DConstIterator& operator=(const CArray2DConstIterator& It);
	/** Returns an iterator to the first element in current row or column.
		@return First element. */
    inline CArray2DConstIterator Begin() const;
	/** Returns an iterator to the last element plus one in current row or column.
		@return Last element plus one. */
    inline CArray2DConstIterator End() const;
	/** overloading ++ operator. Increments the pointer to data. */
    inline CArray2DConstIterator& operator++() { m_Curr++; return *this; }
	/** overloading ++ operator. Increments the pointer to data. Note: This post fix operator
		is slower than using the prefix operator! */
    CArray2DConstIterator operator++(int) { CArray2DConstIterator t = *this; m_Curr++; return t; }
	/** overloading -- operator. Decrements the pointer to data. */
    inline CArray2DConstIterator& operator--() { m_Curr--; return *this; }
	/** overloading -- operator. Decrements the pointer to data. Note: This post fix operator
		is slower than using the prefix operator! */
    CArray2DConstIterator operator--(int) { CArray2DConstIterator t = *this; m_Curr--; return t; }
	/** overloading += operator. Increments pointer to data by Value. */
	inline CArray2DConstIterator& operator +=(int Value){ m_Curr+=Value; return *this;}
	/** overloading -= operator. Decrements pointer to data by Value. */
	inline CArray2DConstIterator& operator -=(int Value){ m_Curr-=Value; return *this;}
	/** overloading the dereference operator, typically used when iterating. */
	inline const T& operator*(){return (*m_pData)[m_s.start()+m_Curr*m_s.stride()];}
    /** overloading [] operator. */
    inline const T& operator[](UINT32 i) const { return (*m_pData)[m_s.start()+i*m_s.stride()]; }
	/** overloading == operator.
		@return true, if iterator p and q points to same element in same column. */
    friend bool operator== __IPL_TF(const CArray2DConstIterator& p, const CArray2DConstIterator& q);
	/** overloading != operator.
		@return true, if iterator p and q points to different elements. */
    friend bool operator!= __IPL_TF(const CArray2DConstIterator& p, const CArray2DConstIterator& q);
	/** overloading < operator.
		@return true, if iterator p and q points to same column and p has row index < q's row index. */
    friend bool operator< __IPL_TF(const CArray2DConstIterator& p, const CArray2DConstIterator& q);
	/** overloading <= operator.
		@return true, if iterator p and q points to same element in same column. */
	friend bool operator<= __IPL_TF(const CArray2DConstIterator& p, const CArray2DConstIterator& q);
	/** overloading > operator.
		@return true, if iterator p and q points to different elements. */
	friend bool operator> __IPL_TF(const CArray2DConstIterator& p, const CArray2DConstIterator& q);
	/** overloading >= operator.
		@return true, if iterator p and q points to same column and p has row index < q's row index. */
	friend bool operator>= __IPL_TF(const CArray2DConstIterator& p, const CArray2DConstIterator& q);
};

template<class T>
inline CArray2DConstIterator<T>& CArray2DConstIterator<T>::operator=(const CArray2DConstIterator<T>& It)
{
	m_pData=It.m_pData;
	m_s=It.m_s;
	m_Curr=It.m_Curr;
	return *this;
}

template<class T>
inline CArray2DConstIterator<T> CArray2DConstIterator<T>::Begin() const
{
	CArray2DConstIterator t = *this;
	t.m_Curr = 0; // index of first element
	return t;
}

template<class T>
inline CArray2DConstIterator<T> CArray2DConstIterator<T>::End() const
{
	CArray2DConstIterator t = *this;
	t.m_Curr = m_s.size(); // index of last element plus one
	return t;
}

template<class T>
bool operator==(const CArray2DConstIterator<T>& p, const CArray2DConstIterator<T>& q)
{
	return p.m_Curr==q.m_Curr
		&& p.m_s.stride()==q.m_s.stride()
		&& p.m_s.start()==q.m_s.start();
}

template<class T>
bool operator!=(const CArray2DConstIterator<T>& p, const CArray2DConstIterator<T>& q)
{
	return !(p==q);
}

template<class T>
bool operator<(const CArray2DConstIterator<T>& p, const CArray2DConstIterator<T>& q)
{
	return p.m_Curr<q.m_Curr
		&& p.m_s.stride()==q.m_s.stride()
		&& p.m_s.start()==q.m_s.start();
}

template<class T>
bool operator<=(const CArray2DConstIterator<T>& p, const CArray2DConstIterator<T>& q)
{
	return p.m_Curr<=q.m_Curr
		&& p.m_s.stride()==q.m_s.stride()
		&& p.m_s.start()==q.m_s.start();
}

template<class T>
bool operator>(const CArray2DConstIterator<T>& p, const CArray2DConstIterator<T>& q)
{
	return p.m_Curr>q.m_Curr
		&& p.m_s.stride()==q.m_s.stride()
		&& p.m_s.start()==q.m_s.start();
}

template<class T>
bool operator>= (const CArray2DConstIterator<T>& p, const CArray2DConstIterator<T>& q)
{
	return p.m_Curr>=q.m_Curr
		&& p.m_s.stride()==q.m_s.stride()
		&& p.m_s.start()==q.m_s.start();
}

} // end namespace ipl

/*@}*/ /* end group "iteratorclasses" */

#endif //_IPL98_ARRAY2DCONSTIT_H
