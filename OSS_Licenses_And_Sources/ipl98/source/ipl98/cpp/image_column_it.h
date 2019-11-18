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

#ifndef _IPL98_IMAGECOLUMNIT_H
#define _IPL98_IMAGECOLUMNIT_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "ipl98_types.h"
#include "../kernel_c/image/kernel_functions.h"
#include "image_const_column_it.h"

// Forward declarations of 'template friend' functions.
namespace ipl 
{
    template<class T>
    class CImageColumnIterator;

    template<class T>
    bool operator==(
        const CImageColumnIterator<T>& p,
        const CImageColumnIterator<T>& q);

    template<class T>
    bool operator!=(
        const CImageColumnIterator<T>& p,
        const CImageColumnIterator<T>& q);

    template<class T>
    bool operator<(
        const CImageColumnIterator<T>& p,
        const CImageColumnIterator<T>& q);

    template<class T>
    bool operator<= (
        const CImageColumnIterator<T>& p,
        const CImageColumnIterator<T>& q);

    template<class T>
    bool operator> (
        const CImageColumnIterator<T>& p,
        const CImageColumnIterator<T>& q);

    template<class T>
    bool operator>= (
        const CImageColumnIterator<T>& p,
        const CImageColumnIterator<T>& q);
}

namespace ipl{ // use namespace if C++

/** @ingroup iteratorclasses
@{ */

/** Iterator class to access data from the class CImage, last updated 11/11/2003.
	This class is not complete, only a few operators are overloaded. But it can
	be used as a basic iterator where the iterator is incremented with the ++
	operator.
	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CImageColumnIterator ipl98/cpp/image_column_it.h
	@see CImageConstColumnIterator
	@version 0.60,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
template<class T>
class CImageColumnIterator
{
protected: // attributes
	/// Pointer to current element in actual row
    T* m_pCurr;
	/// Pointer to first element in actual column
	T* m_pBegin;
	/// Pointer to last element+1 in actual row
	T* m_pEnd;
	/// Width of a row, used to increment iterator
	unsigned int m_RowWidth;
public:
	/** Constructor
		@param ColumnIndex Index of actual column, without origo and ROI.
		@param pStartRowIndex Pointer to row from where iteration begins for actual column, without origo and ROI.
		@param pMaxRowIndex Pointer to row +1 where iteration ends, used to set the return value 
			for End(), without origo and ROI.
		@param RowWidth Width of a complete row of type T. */
	inline CImageColumnIterator(UINT32 ColumnIndex, T* pStartRowIndex, T* pMaxRowIndex, unsigned int RowWidth);
	/// Cast to corresponding const iterator in mixed expressions
	inline operator CImageConstColumnIterator<T>(){return CImageConstColumnIterator<T>(m_pCurr, m_pBegin, m_pEnd,m_RowWidth);}
	/// overloading = operator
	inline CImageColumnIterator& operator=(const CImageColumnIterator& It);
	/** Returns an iterator to the first element in current row or column.
		@return First element. */
	inline CImageColumnIterator<T> Begin() const;
	/** Returns an iterator to the last element plus one in current row or column.
		@return Last element plus one. */
	inline CImageColumnIterator<T> End() const;
	/// overloading + operator
	inline CImageColumnIterator operator+(int Value) const {CImageColumnIterator t=*this; t+=Value; return t;}
	/// overloading - operator
	inline CImageColumnIterator operator-(int Value) const {CImageColumnIterator t=*this; t+=Value; return t;}
	/** overloading ++ operator. Increments the pointer to data. */
	inline CImageColumnIterator<T>& operator++(){ m_pCurr-=m_RowWidth; return *this; }
	/** overloading ++ operator. Increments the pointer to data. Note: This post fix operator
		is slower than using the prefix operator! */
	inline CImageColumnIterator<T> operator++(int){CImageColumnIterator<T> t=*this;m_pCurr-=m_RowWidth;return t;}
	/** overloading -- operator. Decrements the pointer to data. */
	inline CImageColumnIterator<T>& operator--() {m_pCurr+=m_RowWidth; return *this; }
	/** overloading -- operator. Decrements the pointer to data. Note: This post fix operator
		is slower than using the prefix operator! */
	inline CImageColumnIterator<T> operator--(int){CImageColumnIterator<T> t=*this;m_pCurr+=m_RowWidth;return t;}
	/** overloading += operator. Increments pointer to data by Value. */
	inline CImageColumnIterator operator +=(int Value){ m_pCurr-=m_RowWidth*Value; return *this;}
	/** overloading -= operator. Decrements pointer to data by Value. */
	inline CImageColumnIterator operator -=(int Value){ m_pCurr+=m_RowWidth*Value; return *this;}
	/** overloading the dereference operator, typically used when iterating. */
	inline T& operator*(){return *m_pCurr;}
	/** overloading [] operator. */
	inline T& operator[](UINT32 i) {return *(m_pCurr-i*m_RowWidth);}
	/** overloading == operator.
		@return true, if iterator p and q points to same element in same column. */
	friend bool operator== __IPL_TF(const CImageColumnIterator& p, const CImageColumnIterator& q);
	/** overloading != operator.
		@return true, if iterator p and q points to different elements. */
	friend bool operator!= __IPL_TF(const CImageColumnIterator& p, const CImageColumnIterator& q);
	/** overloading < operator. Return value when comparing operators from different columns is undefined.
		@return true, if iterator p and q points to same column and p has column index < q's column index. */
	friend bool operator< __IPL_TF(const CImageColumnIterator& p, const CImageColumnIterator& q);
	/** overloading <= operator. Return value when comparing operators from different columns is undefined.
		@return true, if iterator p and q points to same column and p has column index <= q's column index. */
	friend bool operator<= __IPL_TF(const CImageColumnIterator& p, const CImageColumnIterator& q);
	/** overloading > operator. Return value when comparing operators from different columns is undefined.
		@return true, if iterator p and q points to same column and p has column index > q's column index. */
	friend bool operator> __IPL_TF(const CImageColumnIterator& p, const CImageColumnIterator& q);
	/** overloading >= operator. Return value when comparing operators from different columns is undefined.
		@return true, if iterator p and q points to same column and p has column index >= q's column index. */
	friend bool operator>= __IPL_TF(const CImageColumnIterator& p, const CImageColumnIterator& q);
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

template<class T>
inline CImageColumnIterator<T>::CImageColumnIterator(UINT32 ColumnIndex, T* pStartRowIndex,
													 T* pMaxRowIndex, unsigned int RowWidth):
		m_pCurr(pStartRowIndex+ColumnIndex), m_pBegin(m_pCurr), m_pEnd(pMaxRowIndex+ColumnIndex), 
		m_RowWidth(RowWidth)
{ 
} 

template<class T>
inline CImageColumnIterator<T>& 
		CImageColumnIterator<T>::operator=(const CImageColumnIterator<T>& It)
{
	m_pCurr=It.m_pCurr;
	m_pBegin=It.m_pBegin;
	m_pEnd=It.m_pEnd;
	m_RowWidth=It.m_RowWidth;
	return *this;
}

template<class T>
inline CImageColumnIterator<T> CImageColumnIterator<T>::Begin() const
{
	CImageColumnIterator t = *this;
	t.m_pCurr = m_pBegin;
	return t;
}

template<class T>
inline CImageColumnIterator<T> CImageColumnIterator<T>::End() const
{
	CImageColumnIterator t = *this;
	t.m_pCurr = m_pEnd;
	return t;
}

template<class T>
inline bool operator==(const CImageColumnIterator<T>& p, const CImageColumnIterator<T>& q)
{
	return (p.m_pCurr==q.m_pCurr);
}

template<class T>
inline bool operator!=(const CImageColumnIterator<T>& p, const CImageColumnIterator<T>& q)
{
	return !(p==q);
}

template<class T>
inline bool operator<(const CImageColumnIterator<T>& p, const CImageColumnIterator<T>& q)
{
	// comparison opposite for columns since scan lines are stored upside down!
	return (p.m_pCurr>q.m_pCurr);
}

template<class T>
inline bool operator<= (const CImageColumnIterator<T>& p, const CImageColumnIterator<T>& q)
{
	// comparison opposite for columns since scan lines are stored upside down!
	return (p.m_pCurr>=q.m_pCurr);
}

template<class T>
inline bool operator> (const CImageColumnIterator<T>& p, const CImageColumnIterator<T>& q)
{
	// comparison opposite for columns since scan lines are stored upside down!
	return (p.m_pCurr<q.m_pCurr);
}

template<class T>
inline bool operator>= (const CImageColumnIterator<T>& p, const CImageColumnIterator<T>& q)
{
	// comparison opposite for columns since scan lines are stored upside down!
	return (p.m_pCurr<=q.m_pCurr);
}

/*@}*/ /* end group "iteratorclasses" */

} // end namespace ipl

#endif //_IPL98_IMAGECOLUMNIT_H
