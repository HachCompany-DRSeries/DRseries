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

#ifndef _IPL98_IMAGECONSTROWIT1BP_H
#define _IPL98_IMAGECONSTROWIT1BP_H

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
	\class CImageConstRowIterator1bp ipl98/cpp/image_const_row_it_1bp.h
	@see CImageRowIterator1bp
	@see CImage
	@version 0.61,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
class CImageConstRowIterator1bp
{
protected: // attributes
	/// Counter for last element+1 in actual row
	unsigned int m_End;
	/// Counter for current element in actual row
    unsigned int m_Curr;
	/// Start index in m_pBegin byte
	UINT8 m_StartIndex;
	/// Pointer to start byte in actual row
	UINT8* m_pBegin;
public:
	/** Constructor
		@param pActualRow Absolute pointer to actual row.
		@param StartIndex Index to start iteration from in actual row, without origo and ROI.
		@param MaxIndex Max index + 1, where iteration should end, used to set the return value 
			for End(), without origo and ROI. */
	inline CImageConstRowIterator1bp(UINT8* pActualRow, UINT32 StartIndex, UINT32 MaxIndex);
	/** Constructor used when the corresponding non const iterator casts itself to a const iterator. */
	inline CImageConstRowIterator1bp(unsigned int Curr, UINT8* pBegin, unsigned int End, UINT8 StartIndex);
	/// overloading = operator
	inline CImageConstRowIterator1bp& operator=(const CImageConstRowIterator1bp& It);
	/** Returns an iterator to the first element in current row or column.
		@return First element. */
	inline CImageConstRowIterator1bp Begin() const;
	/** Returns an iterator to the last element plus one in current row or column.
		@return Last element plus one. */
	inline CImageConstRowIterator1bp End() const;
	/// overloading + operator
	inline CImageConstRowIterator1bp operator+(int Value) const {CImageConstRowIterator1bp t=*this; t+=Value; return t;}
	/// overloading - operator
	inline CImageConstRowIterator1bp operator-(int Value) const {CImageConstRowIterator1bp t=*this; t+=Value; return t;}
	/** overloading ++ operator. Increments the pointer to data. */
	inline CImageConstRowIterator1bp& operator++() { m_Curr++; return *this; }
	/** overloading ++ operator (postfix). Increments the pointer to data. */
	inline CImageConstRowIterator1bp operator++(int) { CImageConstRowIterator1bp t=*this; m_Curr++; return t;}
	/** overloading -- operator. Decrements the pointer to data. */
	inline CImageConstRowIterator1bp& operator--() { m_Curr--; return *this; }
	/** overloading -- operator (postfix). Decrements the pointer to data. Note: This post fix operator
		is slower than using the prefix operator! */
	inline CImageConstRowIterator1bp operator--(int) { CImageConstRowIterator1bp t=*this; m_Curr--; return t;}
	/** overloading += operator. Increments pointer to data by Value. */
	inline CImageConstRowIterator1bp operator +=(int Value){ m_Curr+=Value; return *this;}
	/** overloading -= operator. Decrements pointer to data by Value. */
	inline CImageConstRowIterator1bp operator -=(int Value){ m_Curr-=Value; return *this;}
	/** overloading the dereference operator, typically used when iterating. */
	inline UINT32 operator*(){return ( (*(m_pBegin+(m_Curr>>3))) & (0x80>>(m_Curr%8)) )!=0;}
	/** overloading [] operator. */
	inline const UINT32 operator[](UINT32 i){return ( (*(m_pBegin+((m_Curr+i)>>3))) & (0x80>>((m_Curr+i)%8)) )!=0;}
	/** overloading == operator.
		@return true, if iterator p and q points to same element in same column. */
	inline friend bool operator==(const CImageConstRowIterator1bp& p, const CImageConstRowIterator1bp& q);
	/** overloading != operator.
		@return true, if iterator p and q points to different elements. */
	inline friend bool operator!=(const CImageConstRowIterator1bp& p, const CImageConstRowIterator1bp& q);
	/** overloading < operator. Return value when comparing operators from different rows is undefined.
		@return true, if iterator p and q points to same column and p has row index < q's row index. */
	inline friend bool operator< (const CImageConstRowIterator1bp& p, const CImageConstRowIterator1bp& q);
	/** overloading <= operator. Return value when comparing operators from different rows is undefined.
		@return true, if iterator p and q points to same column and p has row index <= q's row index. */
	inline friend bool operator<= (const CImageConstRowIterator1bp& p, const CImageConstRowIterator1bp& q);
	/** overloading > operator. Return value when comparing operators from different rows is undefined.
		@return true, if iterator p and q points to same column and p has row index > q's row index. */
	inline friend bool operator> (const CImageConstRowIterator1bp& p, const CImageConstRowIterator1bp& q);
	/** overloading >= operator. Return value when comparing operators from different rows is undefined.
		@return true, if iterator p and q points to same column and p has row index >= q's row index. */
	inline friend bool operator>= (const CImageConstRowIterator1bp& p, const CImageConstRowIterator1bp& q);
};

inline CImageConstRowIterator1bp::CImageConstRowIterator1bp(UINT8* pActualRow, UINT32 StartIndex, UINT32 MaxIndex):
		m_End(MaxIndex), m_Curr(StartIndex%8), m_StartIndex(m_Curr), m_pBegin(pActualRow+(StartIndex>>3))
{
}

inline CImageConstRowIterator1bp::CImageConstRowIterator1bp(unsigned int Curr, UINT8* pBegin, 
															unsigned int End, UINT8 StartIndex) :
		m_End(End), m_Curr(Curr), m_StartIndex(StartIndex), m_pBegin(pBegin)
{
}

inline CImageConstRowIterator1bp& CImageConstRowIterator1bp::operator=(const CImageConstRowIterator1bp& It)
{
	m_Curr  = It.m_Curr;
	m_StartIndex = It.m_StartIndex;
	m_pBegin = It.m_pBegin;
	m_End   = It.m_End;
	return *this;
}

inline CImageConstRowIterator1bp CImageConstRowIterator1bp::Begin() const
{
	CImageConstRowIterator1bp t = *this;
	t.m_Curr = m_StartIndex;
	return t;
}

inline CImageConstRowIterator1bp CImageConstRowIterator1bp::End() const
{
	CImageConstRowIterator1bp t = *this;
	t.m_Curr = m_End;
	return t;
}

inline bool operator==(const CImageConstRowIterator1bp& p, const CImageConstRowIterator1bp& q)
{
	return (p.m_Curr==q.m_Curr);
}

inline bool operator!=(const CImageConstRowIterator1bp& p, const CImageConstRowIterator1bp& q)
{
	return !(p==q);
}

inline bool operator<(const CImageConstRowIterator1bp& p, const CImageConstRowIterator1bp& q)
{
	return (p.m_Curr<q.m_Curr);
}

inline bool operator<= (const CImageConstRowIterator1bp& p, const CImageConstRowIterator1bp& q)
{
	return (p.m_Curr<=q.m_Curr);
}

inline bool operator> (const CImageConstRowIterator1bp& p, const CImageConstRowIterator1bp& q)
{
	return (p.m_Curr>q.m_Curr);
}

inline bool operator>= (const CImageConstRowIterator1bp& p, const CImageConstRowIterator1bp& q)
{
	return (p.m_Curr>=q.m_Curr);
}

/*@}*/ /* end group "iteratorclasses" */

} // end namespace ipl

#endif //_IPL98_IMAGECONSTROWIT1BP_H
