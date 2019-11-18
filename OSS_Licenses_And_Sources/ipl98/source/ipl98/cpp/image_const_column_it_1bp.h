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

#ifndef _IPL98_IMAGECONSTCOLUMNIT1BP_H
#define _IPL98_IMAGECONSTCOLUMNIT1BP_H

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
	\class CImageConstColumnIterator1bp ipl98/cpp/image_const_column_it_1bp.h
	@see CImageConstRowIterator1bp
	@see CImage
	@version 0.60,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
class CImageConstColumnIterator1bp
{
protected: // attributes
	/// Pointer to TImage structure of the image to be iterated
	const TImage* m_pTImg;
	/** Pixel index in actual byte containing the column for iteration, i.e. the column 
		index mod 8 since one byte contains 8 pixels. When iterating a column, this value 
		stays constant. Set by constructor. */
    unsigned int m_PixelIndexInByte;
	/** Pointer to byte containing the pixel value for actual index, using this in conjunction
		wiht m_PixelIndexInByte makes it possible to extract the actual pixel. Updated when operator ++ is used. */
	UINT8* m_pCurrByte;
	/// Pointer to first element in actual column
	UINT8* m_pBegin;
	/// Counter for last element+1 in actual column
	UINT8* m_pEnd;
public:
	/** Constructor
		@param ColumnIndex Index of actual row, without origo and ROI.
		@param StartIndex Index to start iteration from in actual row, without origo and ROI.
		@param MaxIndex Max index + 1, where iteration should end, used to set the return value 
			for End(), without origo and ROI.
		@param pTImg Pointer to a TImage structure, a member of class CImage. */
	inline CImageConstColumnIterator1bp(UINT32 ColumnIndex, UINT32 StartIndex, UINT32 MaxIndex, const TImage* pTImg);
	/** Constructor used when the corresponding non const iterator casts itself to a const iterator. */
	inline CImageConstColumnIterator1bp(unsigned int PixelIndexInByte, UINT8* pCurrByte, UINT8* pBegin, UINT8* pEnd, const TImage* pTImg);
	/// overloading = operator
	inline CImageConstColumnIterator1bp& operator=(const CImageConstColumnIterator1bp& It);
	/** Returns an iterator to the first element in current row or column.
		@return First element. */
	inline CImageConstColumnIterator1bp Begin() const;
	/** Returns an iterator to the last element plus one in current row or column.
		@return Last element plus one. */
	inline CImageConstColumnIterator1bp End() const;
	/// overloading + operator
	inline CImageConstColumnIterator1bp operator+(int Value) const {CImageConstColumnIterator1bp t=*this; t+=Value; return t;}
	/// overloading - operator
	inline CImageConstColumnIterator1bp operator-(int Value) const {CImageConstColumnIterator1bp t=*this; t+=Value; return t;}
	/** overloading ++ operator. Increments the pointer to data. 
		Note: the postfix operator++ is not implemented in this class since the resulting
		code cannot be optimized. Using that operator would remove the point of using this 
		iterator class. */
	inline CImageConstColumnIterator1bp& operator++() {m_pCurrByte-=m_pTImg->ByteWidth; return *this; }
	/** overloading ++ operator (postfix). Increments the pointer to data. Note: This post fix operator
		is slower than using the prefix operator! */
	inline CImageConstColumnIterator1bp operator++(int){CImageConstColumnIterator1bp t=*this; m_pCurrByte-=m_pTImg->ByteWidth; return t;}
	/** overloading -- operator. Decrements the pointer to data. */
	inline CImageConstColumnIterator1bp& operator--() {m_pCurrByte+=m_pTImg->ByteWidth; return *this; }
	/** overloading -- operator (postfix). Decrements the pointer to data. Note: This post fix operator
		is slower than using the prefix operator! */
	inline CImageConstColumnIterator1bp operator--(int){CImageConstColumnIterator1bp t=*this; m_pCurrByte+=m_pTImg->ByteWidth; return t;}
	/** overloading += operator. Increments pointer to data by Value. */
	inline CImageConstColumnIterator1bp operator +=(int Value){ m_pCurrByte-=m_pTImg->ByteWidth*Value; return *this;}
	/** overloading -= operator. Decrements pointer to data by Value. */
	inline CImageConstColumnIterator1bp operator -=(int Value){ m_pCurrByte+=m_pTImg->ByteWidth*Value; return *this;}
	/** overloading the dereference operator, typically used when iterating. */
	inline UINT32 operator*(){return ((*(m_pCurrByte+(m_PixelIndexInByte>>3))) & (0x80>>(m_PixelIndexInByte%8)) )!=0;}
	/** overloading [] operator. */
	inline const UINT32 operator[](UINT32 i) {return ((*(m_pCurrByte+((m_PixelIndexInByte+i)>>3))) & (0x80>>((m_PixelIndexInByte+i)%8)) )!=0;}
	/** overloading == operator.
		@return true, if iterator p and q points to same element in same column. */
	inline friend bool operator==(const CImageConstColumnIterator1bp& p, const CImageConstColumnIterator1bp& q);
	/** overloading != operator.
		@return true, if iterator p and q points to different elements. */
	inline friend bool operator!=(const CImageConstColumnIterator1bp& p, const CImageConstColumnIterator1bp& q);
	/** overloading < operator. Return value when comparing operators from different columns is undefined.
		@return true, if iterator p and q points to same column and p has column index < q's column index. */
	inline friend bool operator< (const CImageConstColumnIterator1bp& p, const CImageConstColumnIterator1bp& q);
	/** overloading <= operator. Return value when comparing operators from different columns is undefined.
		@return true, if iterator p and q points to same column and p has column index <= q's column index. */
	inline friend bool operator<= (const CImageConstColumnIterator1bp& p, const CImageConstColumnIterator1bp& q);
	/** overloading > operator. Return value when comparing operators from different columns is undefined.
		@return true, if iterator p and q points to same column and p has column index > q's column index. */
	inline friend bool operator> (const CImageConstColumnIterator1bp& p, const CImageConstColumnIterator1bp& q);
	/** overloading >= operator. Return value when comparing operators from different columns is undefined.
		@return true, if iterator p and q points to same column and p has column index >= q's column index. */
	inline friend bool operator>= (const CImageConstColumnIterator1bp& p, const CImageConstColumnIterator1bp& q);
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

inline CImageConstColumnIterator1bp::CImageConstColumnIterator1bp(UINT32 ColumnIndex, UINT32 StartIndex,
																  UINT32 MaxIndex,	
																  const TImage* pTImg):
		m_pTImg(pTImg), m_PixelIndexInByte(ColumnIndex%8),
		m_pCurrByte(pTImg->ppAllScanLines[StartIndex]+(ColumnIndex>>3)),
		m_pBegin(m_pCurrByte), 
		m_pEnd(m_pCurrByte-pTImg->ByteWidth*(MaxIndex-StartIndex))
{
}

inline CImageConstColumnIterator1bp::CImageConstColumnIterator1bp(unsigned int PixelIndexInByte, 
																  UINT8* pCurrByte, UINT8* pBegin, 
																  UINT8* pEnd, const TImage* pTImg) : 
		m_pTImg(pTImg), m_PixelIndexInByte(PixelIndexInByte), 
		m_pCurrByte(pCurrByte), m_pBegin(pBegin), m_pEnd(pEnd)
{
};


inline CImageConstColumnIterator1bp& CImageConstColumnIterator1bp::operator=(const CImageConstColumnIterator1bp& It)
{
	m_pTImg            = It.m_pTImg;
	m_PixelIndexInByte = It.m_PixelIndexInByte;
	m_pCurrByte        = It.m_pCurrByte;
	m_pBegin           = It.m_pBegin;
	m_pEnd             = It.m_pEnd;
	return *this;
}

inline CImageConstColumnIterator1bp CImageConstColumnIterator1bp::Begin() const
{
	CImageConstColumnIterator1bp t = *this;
	t.m_pCurrByte = m_pBegin;
	return t;
}

inline CImageConstColumnIterator1bp CImageConstColumnIterator1bp::End() const
{
	CImageConstColumnIterator1bp t = *this;
	t.m_pCurrByte = m_pEnd;
	return t;
}

inline bool operator==(const CImageConstColumnIterator1bp& p, const CImageConstColumnIterator1bp& q)
{
	return (p.m_pCurrByte==q.m_pCurrByte);
}

inline bool operator!=(const CImageConstColumnIterator1bp& p, const CImageConstColumnIterator1bp& q)
{
	return !(p==q);
}

inline bool operator<(const CImageConstColumnIterator1bp& p, const CImageConstColumnIterator1bp& q)
{
	// comparison opposite for columns since scan lines are stored upside down!
	return (p.m_pCurrByte>q.m_pCurrByte);
}

inline bool operator<= (const CImageConstColumnIterator1bp& p, const CImageConstColumnIterator1bp& q)
{
	// comparison opposite for columns since scan lines are stored upside down!
	return (p.m_pCurrByte>=q.m_pCurrByte);
}

inline bool operator> (const CImageConstColumnIterator1bp& p, const CImageConstColumnIterator1bp& q)
{
	// comparison opposite for columns since scan lines are stored upside down!
	return (p.m_pCurrByte<q.m_pCurrByte);
}

inline bool operator>= (const CImageConstColumnIterator1bp& p, const CImageConstColumnIterator1bp& q)
{
	// comparison opposite for columns since scan lines are stored upside down!
	return (p.m_pCurrByte<=q.m_pCurrByte);
}

/*@}*/ /* end group "iteratorclasses" */

} // end namespace ipl

#endif //_IPL98_IMAGECONSTCOLUMNIT1BP_H
