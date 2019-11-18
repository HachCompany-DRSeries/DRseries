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

#ifndef _IPL98_IMAGEROWIT_H
#define _IPL98_IMAGEROWIT_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "ipl98_types.h"
#include "../kernel_c/image/kernel_functions.h"
#include "image_const_row_it.h"

// Forward declarations of 'template friend' functions.
namespace ipl 
{
    template<class T>
    class CImageRowIterator;

    template<class T>
    bool operator==(
        const CImageRowIterator<T>& p,
        const CImageRowIterator<T>& q);

    template<class T>
    bool operator!=(
        const CImageRowIterator<T>& p,
        const CImageRowIterator<T>& q);

    template<class T>
    bool operator<(
        const CImageRowIterator<T>& p,
        const CImageRowIterator<T>& q);

    template<class T>
    bool operator<= (
        const CImageRowIterator<T>& p,
        const CImageRowIterator<T>& q);

    template<class T>
    bool operator> (
        const CImageRowIterator<T>& p,
        const CImageRowIterator<T>& q);

    template<class T>
    bool operator>= (
        const CImageRowIterator<T>& p,
        const CImageRowIterator<T>& q);
}

namespace ipl{ // use namespace if C++

/** @ingroup iteratorclasses
@{ */

/** Iterator class to access data from the class CImage2D, last updated 11/11/2003.
	This class is not complete, only a few operators are overloaded. But it can
	be used as a basic iterator where the iterator is incremented with the ++
	operator.
	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CImageRowIterator ipl98/cpp/image_row_it.h
	@see CImageConstRowIterator8bp
	@see CImage
	@version 0.60,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
template<class T>
class CImageRowIterator
{
protected: // attributes
	/// Pointer to last element+1 in actual row
	T* m_pEnd;
	/// Pointer to current element in actual row
    T* m_pCurr;
	/// Pointer to first element in actual row
	T* m_pBegin;
public:
	/** Constructor
		@param pActualRow Absolute pointer to actual row.
		@param StartIndex Index to start iteration from in actual row, without origo and ROI.
		@param MaxIndex Max index + 1, where iteration should end, used to set the return value 
			for End(), without origo and ROI. */
	inline CImageRowIterator(T* pActualRow, UINT32 StartIndex, UINT32 MaxIndex);
	/// Cast to corresponding const iterator in mixed expressions
	inline operator CImageConstRowIterator<T>(){return CImageConstRowIterator<T>(m_pCurr, m_pBegin, m_pEnd);}
	/// overloading = operator
	inline CImageRowIterator& operator=(const CImageRowIterator& It);
	/** Returns an iterator to the first element in current row or column.
		@return First element. */
	inline CImageRowIterator Begin() const;
	/** Returns an iterator to the last element plus one in current row or column.
		@return Last element plus one. */
	inline CImageRowIterator End() const;
	/// overloading + operator
	inline CImageRowIterator operator+(int Value) const {CImageRowIterator t=*this; t+=Value; return t;}
	/// overloading - operator
	inline CImageRowIterator operator-(int Value) const {CImageRowIterator t=*this; t+=Value; return t;}
	/** overloading ++ operator. Increments the pointer to data. */
	inline CImageRowIterator& operator++() { m_pCurr++; return *this; }
	/** overloading ++ operator (postfix). Increments the pointer to data. */
	inline CImageRowIterator operator++(int) { CImageRowIterator t=*this; m_pCurr++; return t;}
	/** overloading -- operator. Decrements the pointer to data. */
	inline CImageRowIterator& operator--() { m_pCurr--; return *this; }
	/** overloading -- operator (postfix). Decrements the pointer to data. Note: This post fix operator
		is slower than using the prefix operator! */
	inline CImageRowIterator operator--(int) { CImageRowIterator t=*this; m_pCurr--; return t;}
	/** overloading += operator. Increments pointer to data by Value. */
	inline CImageRowIterator operator +=(int Value){ m_pCurr+=Value; return *this;}
	/** overloading -= operator. Decrements pointer to data by Value. */
	inline CImageRowIterator operator -=(int Value){ m_pCurr-=Value; return *this;}
	/** overloading the dereference operator, typically used when iterating. */
	inline T& operator*(){return *m_pCurr;}
	/** overloading [] operator. */
	inline T& operator[](UINT32 i) {return *(m_pCurr+i);}
	/** overloading == operator.
		@return true, if iterator p and q points to same element in same column. */
	friend bool operator== __IPL_TF(const CImageRowIterator& p, const CImageRowIterator& q);
	/** overloading != operator.
		@return true, if iterator p and q points to different elements. */
	friend bool operator!= __IPL_TF(const CImageRowIterator& p, const CImageRowIterator& q);
	/** overloading < operator. Return value when comparing operators from different rows is undefined.
		@return true, if iterator p and q points to same column and p has row index < q's row index. */
	friend bool operator< __IPL_TF(const CImageRowIterator& p, const CImageRowIterator& q);
	/** overloading <= operator. Return value when comparing operators from different rows is undefined.
		@return true, if iterator p and q points to same column and p has row index <= q's row index. */
	friend bool operator<= __IPL_TF(const CImageRowIterator& p, const CImageRowIterator& q);
	/** overloading > operator. Return value when comparing operators from different rows is undefined.
		@return true, if iterator p and q points to same column and p has row index > q's row index. */
	friend bool operator> __IPL_TF(const CImageRowIterator& p, const CImageRowIterator& q);
	/** overloading >= operator. Return value when comparing operators from different rows is undefined.
		@return true, if iterator p and q points to same column and p has row index >= q's row index. */
	friend bool operator>= __IPL_TF(const CImageRowIterator& p, const CImageRowIterator& q);
};

template<class T>
inline CImageRowIterator<T>::CImageRowIterator(T* pActualRow, UINT32 StartIndex, UINT32 MaxIndex):
		m_pEnd(pActualRow + MaxIndex), m_pCurr(pActualRow+StartIndex), m_pBegin(m_pCurr)
{
}

template<class T>
inline CImageRowIterator<T>& CImageRowIterator<T>::operator=(const CImageRowIterator<T>& It)
{
	m_pCurr   = It.m_pCurr;
	m_pBegin  = It.m_pBegin;
	m_pEnd    = It.m_pEnd;
	return *this;
}

template<class T>
inline CImageRowIterator<T> CImageRowIterator<T>::Begin() const
{
	CImageRowIterator t = *this;
	t.m_pCurr = m_pBegin;
	return t;
}

template<class T>
inline CImageRowIterator<T> CImageRowIterator<T>::End() const
{
	CImageRowIterator t = *this;
	t.m_pCurr = m_pEnd;
	return t;
}

template<class T>
inline bool operator==(const CImageRowIterator<T>& p, const CImageRowIterator<T>& q)
{
	return (p.m_pCurr==q.m_pCurr);
}

template<class T>
inline bool operator!=(const CImageRowIterator<T>& p, const CImageRowIterator<T>& q)
{
	return !(p==q);
}

template<class T>
inline bool operator<(const CImageRowIterator<T>& p, const CImageRowIterator<T>& q)
{
	return (p.m_pCurr<q.m_pCurr);
}

template<class T>
inline bool operator<= (const CImageRowIterator<T>& p, const CImageRowIterator<T>& q)
{
	return (p.m_pCurr<=q.m_pCurr);
}

template<class T>
inline bool operator> (const CImageRowIterator<T>& p, const CImageRowIterator<T>& q)
{
	return (p.m_pCurr>q.m_pCurr);
}

template<class T>
inline bool operator>= (const CImageRowIterator<T>& p, const CImageRowIterator<T>& q)
{
	return (p.m_pCurr>=q.m_pCurr);
}

/*@}*/ /* end group "iteratorclasses" */

} // end namespace ipl

#endif //_IPL98_IMAGEROWIT_H
