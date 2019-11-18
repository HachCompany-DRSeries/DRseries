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

#ifndef _IPL98_IMAGECONSTCOLUMNIT24BP_H
#define _IPL98_IMAGECONSTCOLUMNIT24BP_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "ipl98_types.h"
#include "../kernel_c/image/kernel_functions.h"

namespace ipl{ // use namespace if C++

/** @ingroup iteratorclasses
@{ */

/** Iterator class to access data from the class CImage, last updated 11/11/2003.
	This class is not complete, only a few operators are overloaded. But it can
	be used as a basic iterator where the iterator is incremented with the ++
	operator.
	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CImageConstColumnIterator24bp ipl98/cpp/image_const_row_it_24bp.h
	@see CImageRowIterator24bp
	@see CImage
	@version 0.60,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
class CImageConstColumnIterator24bp
{
protected: // attributes
	/// Pointer to TImage structure of the image to be iterated
	const TImage* m_pTImg;
	/// Pointer to current element in actual row
    UINT8* m_pCurr;
	/// Pointer to first element in actual column
	UINT8* m_pBegin;
	/// Pointer to last element+1 in actual row
	UINT8* m_pEnd;
public:
	/** Constructor
		@param ColumnIndex Index of actual column, without origo and ROI.
		@param StartIndex Index to start iteration from in actual column, without origo and ROI.
		@param MaxIndex Max index + 1, where iteration should end, used to set the return value 
			for End(), without origo and ROI.
		@param pTImg Pointer to a TImage structure, a member of class CImage. */
	inline CImageConstColumnIterator24bp(UINT32 ColumnIndex, UINT32 StartIndex, UINT32 MaxIndex, const TImage* pTImg);
	/** Constructor used when the corresponding non const iterator casts itself to a const iterator. */
	inline CImageConstColumnIterator24bp(UINT8* m_pCurr, UINT8* pBegin, UINT8* pEnd, const TImage* pTImg);
	/// overloading = operator
	inline CImageConstColumnIterator24bp& operator=(const CImageConstColumnIterator24bp& It);
	/** Returns an iterator to the first element in current row or column.
		@return First element. */
	inline CImageConstColumnIterator24bp Begin() const;
	/** Returns an iterator to the last element plus one in current row or column.
		@return Last element plus one. */
	inline CImageConstColumnIterator24bp End() const;
	/// overloading + operator
	inline CImageConstColumnIterator24bp operator+(int Value) const {CImageConstColumnIterator24bp t=*this; t+=Value; return t;}
	/// overloading - operator
	inline CImageConstColumnIterator24bp operator-(int Value) const {CImageConstColumnIterator24bp t=*this; t+=Value; return t;}
	/** overloading ++ operator. Increments the pointer to data. */
	inline CImageConstColumnIterator24bp& operator++(){m_pCurr-=m_pTImg->ByteWidth; return *this;}
	/** overloading ++ operator (postfix). Increments the pointer to data. */
	inline CImageConstColumnIterator24bp operator++(int){CImageConstColumnIterator24bp t=*this; m_pCurr-=m_pTImg->ByteWidth; return t;}
	/** overloading -- operator. Decrements the pointer to data. */
	inline CImageConstColumnIterator24bp& operator--(){m_pCurr+=m_pTImg->ByteWidth; return *this;}
	/** overloading -- operator (postfix). Decrements the pointer to data. Note: This post fix operator
		is slower than using the prefix operator! */
	inline CImageConstColumnIterator24bp operator--(int){CImageConstColumnIterator24bp t=*this; m_pCurr+=m_pTImg->ByteWidth; return t;}
	/** overloading += operator. Increments pointer to data by Value. */
	inline CImageConstColumnIterator24bp operator +=(int Value){ m_pCurr-=m_pTImg->ByteWidth*Value; return *this;}
	/** overloading -= operator. Decrements pointer to data by Value. */
	inline CImageConstColumnIterator24bp operator -=(int Value){ m_pCurr+=m_pTImg->ByteWidth*Value; return *this;}
	/** overloading the dereference operator, typically used when iterating. */
	inline UINT32 operator*(){return ((*((UINT32*)m_pCurr)) & 0xffffff);}
	/** overloading [] operator. */
	inline const UINT32 operator[](UINT32 i) const{return (*(m_pCurr-i*m_pTImg->ByteWidth) & 0xffffff);}
	/** overloading == operator.
		@return true, if iterator p and q points to same element in same column. */
	inline friend bool operator==(const CImageConstColumnIterator24bp& p, const CImageConstColumnIterator24bp& q);
	/** overloading != operator.
		@return true, if iterator p and q points to different elements. */
	inline friend bool operator!=(const CImageConstColumnIterator24bp& p, const CImageConstColumnIterator24bp& q);
	/** overloading < operator. Return value when comparing operators from different columns is undefined.
		@return true, if iterator p and q points to same column and p has column index < q's column index. */
	inline friend bool operator< (const CImageConstColumnIterator24bp& p, const CImageConstColumnIterator24bp& q);
	/** overloading <= operator. Return value when comparing operators from different columns is undefined.
		@return true, if iterator p and q points to same column and p has column index <= q's column index. */
	inline friend bool operator<= (const CImageConstColumnIterator24bp& p, const CImageConstColumnIterator24bp& q);
	/** overloading > operator. Return value when comparing operators from different columns is undefined.
		@return true, if iterator p and q points to same column and p has column index > q's column index. */
	inline friend bool operator> (const CImageConstColumnIterator24bp& p, const CImageConstColumnIterator24bp& q);
	/** overloading >= operator. Return value when comparing operators from different columns is undefined.
		@return true, if iterator p and q points to same column and p has column index >= q's column index. */
	inline friend bool operator>= (const CImageConstColumnIterator24bp& p, const CImageConstColumnIterator24bp& q);
};

inline CImageConstColumnIterator24bp::CImageConstColumnIterator24bp(UINT32 ColumnIndex, UINT32 StartIndex, UINT32 MaxIndex,
															const TImage* pTImg):
		m_pTImg(pTImg), m_pCurr(pTImg->ppAllScanLines[StartIndex]+ColumnIndex*3), m_pBegin(m_pCurr),
		m_pEnd(m_pCurr-pTImg->ByteWidth*(MaxIndex-StartIndex))
{
}

inline CImageConstColumnIterator24bp::CImageConstColumnIterator24bp(UINT8* pCurr, UINT8* pBegin, 
																   UINT8* pEnd, const TImage* pTImg) :
		m_pTImg(pTImg), m_pCurr(pCurr), m_pBegin(pBegin), m_pEnd(pEnd)
{
}

inline CImageConstColumnIterator24bp& CImageConstColumnIterator24bp::operator=(const CImageConstColumnIterator24bp& It)
{
	m_pTImg = It.m_pTImg;
	m_pCurr  = It.m_pCurr;
	m_pBegin = It.m_pBegin;
	m_pEnd   = It.m_pEnd;
	return *this;
}

inline CImageConstColumnIterator24bp CImageConstColumnIterator24bp::Begin() const
{
	CImageConstColumnIterator24bp t = *this;
	t.m_pCurr = m_pBegin;
	return t;
}

inline CImageConstColumnIterator24bp CImageConstColumnIterator24bp::End() const
{
	CImageConstColumnIterator24bp t = *this;
	t.m_pCurr = m_pEnd;
	return t;
}

inline bool operator==(const CImageConstColumnIterator24bp& p, const CImageConstColumnIterator24bp& q)
{
	return (p.m_pCurr==q.m_pCurr);
}

inline bool operator!=(const CImageConstColumnIterator24bp& p, const CImageConstColumnIterator24bp& q)
{
	return !(p==q);
}

inline bool operator<(const CImageConstColumnIterator24bp& p, const CImageConstColumnIterator24bp& q)
{
	// comparison opposite for columns since scan lines are stored upside down!
	return (p.m_pCurr>q.m_pCurr);
}

inline bool operator<= (const CImageConstColumnIterator24bp& p, const CImageConstColumnIterator24bp& q)
{
	// comparison opposite for columns since scan lines are stored upside down!
	return (p.m_pCurr>=q.m_pCurr);
}

inline bool operator> (const CImageConstColumnIterator24bp& p, const CImageConstColumnIterator24bp& q)
{
	// comparison opposite for columns since scan lines are stored upside down!
	return (p.m_pCurr<q.m_pCurr);
}

inline bool operator>= (const CImageConstColumnIterator24bp& p, const CImageConstColumnIterator24bp& q)
{
	// comparison opposite for columns since scan lines are stored upside down!
	return (p.m_pCurr<=q.m_pCurr);
}

/*@}*/ /* end group "iteratorclasses" */

} // end namespace ipl

#endif //_IPL98_IMAGECONSTCOLUMNIT24BP_H
