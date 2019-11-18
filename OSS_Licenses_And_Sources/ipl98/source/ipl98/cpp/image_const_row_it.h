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

#ifndef _IPL98_IMAGECONSTROWIT_H
#define _IPL98_IMAGECONSTROWIT_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "ipl98_types.h"
#include "../kernel_c/image/kernel_functions.h"

// Forward declarations of 'template friend' functions.
namespace ipl 
{
    template <class T>
    class CImageConstRowIterator;

    template <class T>
    bool operator==(
        const CImageConstRowIterator<T>& p,
        const CImageConstRowIterator<T>& q);

    template<class T>
    bool operator!=(
        const CImageConstRowIterator<T>& p,
        const CImageConstRowIterator<T>& q);

    template<class T>
    bool operator<(
        const CImageConstRowIterator<T>& p,
        const CImageConstRowIterator<T>& q);

    template<class T>
    bool operator<= (
        const CImageConstRowIterator<T>& p,
        const CImageConstRowIterator<T>& q);

    template<class T>
    bool operator> (
        const CImageConstRowIterator<T>& p,
        const CImageConstRowIterator<T>& q);

    template<class T>
    bool operator>= (
        const CImageConstRowIterator<T>& p,
        const CImageConstRowIterator<T>& q);
}

namespace ipl{ // use namespace if C++

/** @ingroup iteratorclasses
@{ */

/** Iterator class to access data from the class CImage2D, last updated 11/11/2003.
	This class is not complete, only a few operators are overloaded. But it can
	be used as a basic iterator where the iterator is incremented with the ++
	operator.
	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CImageConstRowIterator ipl98/cpp/image_const_row_it.h
	@see CImageRowIterator8bp
	@see CImage
	@version 0.60,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
template<class T>
class CImageConstRowIterator
{
protected: // attributes
	/// Pointer to current element in actual row
    T* m_pCurr;
	/// Pointer to first element in actual row
	T* m_pBegin;
	/// Pointer to last element+1 in actual row
	T* m_pEnd;
public:
	/** Constructor
		@param pActualRow Absolute pointer to actual row.
		@param StartIndex Index to start iteration from in actual row, without origo and ROI.
		@param MaxIndex Max index + 1, where iteration should end, used to set the return value 
			for End(), without origo and ROI. */
	inline CImageConstRowIterator(T* pActualRow, UINT32 StartIndex, UINT32 MaxIndex);
	/** Constructor used when the corresponding non const iterator casts itself to a const iterator. */
	inline CImageConstRowIterator(T* pCurr, T* pBegin, T* pEnd);
	/// overloading = operator
	inline CImageConstRowIterator& operator=(const CImageConstRowIterator& It);
	/** Returns an iterator to the first element in current row or column.
		@return First element. */
	inline CImageConstRowIterator Begin() const;
	/** Returns an iterator to the last element plus one in current row or column.
		@return Last element plus one. */
	inline CImageConstRowIterator End() const;
	/// overloading + operator
	inline CImageConstRowIterator operator+(int Value) const {CImageConstRowIterator t=*this; t+=Value; return t;}
	/// overloading - operator
	inline CImageConstRowIterator operator-(int Value) const {CImageConstRowIterator t=*this; t+=Value; return t;}
	/** overloading ++ operator. Increments the pointer to data. */
	inline CImageConstRowIterator& operator++() { m_pCurr++; return *this; }
	/** overloading ++ operator (postfix). Increments the pointer to data. */
	inline CImageConstRowIterator operator++(int) { CImageConstRowIterator t=*this; m_pCurr++; return t;}
	/** overloading -- operator. Decrements the pointer to data. */
	inline CImageConstRowIterator& operator--() { m_pCurr--; return *this; }
	/** overloading -- operator (postfix). Decrements the pointer to data. Note: This post fix operator
		is slower than using the prefix operator! */
	inline CImageConstRowIterator operator--(int) { CImageConstRowIterator t=*this; m_pCurr--; return t;}
	/** overloading += operator. Increments pointer to data by Value. */
	inline CImageConstRowIterator operator +=(int Value){ m_pCurr+=Value; return *this;}
	/** overloading -= operator. Decrements pointer to data by Value. */
	inline CImageConstRowIterator operator -=(int Value){ m_pCurr-=Value; return *this;}
	/** overloading the dereference operator, typically used when iterating. */
	inline const T& operator*(){return *m_pCurr;}
	/** overloading [] operator. */
	inline const T& operator[](UINT32 i) {return *(m_pCurr+i);}
	/** overloading == operator.
		@return true, if iterator p and q points to same element in same column. */
	friend bool operator== __IPL_TF(const CImageConstRowIterator& p, const CImageConstRowIterator& q);
	/** overloading != operator.
		@return true, if iterator p and q points to different elements. */
	friend bool operator!= __IPL_TF(const CImageConstRowIterator& p, const CImageConstRowIterator& q);
	/** overloading < operator. Return value when comparing operators from different rows is undefined.
		@return true, if iterator p and q points to same column and p has row index < q's row index. */
	friend bool operator< __IPL_TF(const CImageConstRowIterator& p, const CImageConstRowIterator& q);
	/** overloading <= operator. Return value when comparing operators from different rows is undefined.
		@return true, if iterator p and q points to same column and p has row index <= q's row index. */
	friend bool operator<= __IPL_TF(const CImageConstRowIterator& p, const CImageConstRowIterator& q);
	/** overloading > operator. Return value when comparing operators from different rows is undefined.
		@return true, if iterator p and q points to same column and p has row index > q's row index. */
	friend bool operator> __IPL_TF(const CImageConstRowIterator& p, const CImageConstRowIterator& q);
	/** overloading >= operator. Return value when comparing operators from different rows is undefined.
		@return true, if iterator p and q points to same column and p has row index >= q's row index. */
	friend bool operator>= __IPL_TF(const CImageConstRowIterator& p, const CImageConstRowIterator& q);
};

template<class T>
inline CImageConstRowIterator<T>::CImageConstRowIterator(T* pActualRow, UINT32 StartIndex, UINT32 MaxIndex):
		m_pCurr(pActualRow+StartIndex), m_pBegin(m_pCurr), m_pEnd(pActualRow + MaxIndex)
{
}

template<class T>
inline CImageConstRowIterator<T>::CImageConstRowIterator(T* pCurr, T* pBegin, T* pEnd) :
		m_pEnd(pEnd), m_pCurr(pCurr), m_pBegin(pBegin)
{
}

template<class T>
inline CImageConstRowIterator<T>& CImageConstRowIterator<T>::operator=(const CImageConstRowIterator& It)
{
	m_pCurr   = It.m_pCurr;
	m_pBegin  = It.m_pBegin;
	m_pEnd    = It.m_pEnd;
	return *this;
}

template<class T>
inline CImageConstRowIterator<T> CImageConstRowIterator<T>::Begin() const
{
	CImageConstRowIterator t = *this;
	t.m_pCurr = m_pBegin;
	return t;
}

template<class T>
inline CImageConstRowIterator<T> CImageConstRowIterator<T>::End() const
{
	CImageConstRowIterator t = *this;
	t.m_pCurr = m_pEnd;
	return t;
}

template<class T>
inline bool operator==(const CImageConstRowIterator<T>& p, const CImageConstRowIterator<T>& q)
{
	return (p.m_pCurr==q.m_pCurr);
}

template<class T>
inline bool operator!=(const CImageConstRowIterator<T>& p, const CImageConstRowIterator<T>& q)
{
	return !(p==q);
}

template<class T>
inline bool operator<(const CImageConstRowIterator<T>& p, const CImageConstRowIterator<T>& q)
{
	return (p.m_pCurr<q.m_pCurr);
}

template<class T>
inline bool operator<= (const CImageConstRowIterator<T>& p, const CImageConstRowIterator<T>& q)
{
	return (p.m_pCurr<=q.m_pCurr);
}

template<class T>
inline bool operator> (const CImageConstRowIterator<T>& p, const CImageConstRowIterator<T>& q)
{
	return (p.m_pCurr>q.m_pCurr);
}

template<class T>
inline bool operator>= (const CImageConstRowIterator<T>& p, const CImageConstRowIterator<T>& q)
{
	return (p.m_pCurr>=q.m_pCurr);
}

/*@}*/ /* end group "iteratorclasses" */

} // end namespace ipl

#endif //_IPL98_IMAGECONSTROWIT_H
