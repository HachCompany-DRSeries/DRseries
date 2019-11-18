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

#ifndef _IPL98_IMAGECOLUMNIT24BP_H
#define _IPL98_IMAGECOLUMNIT24BP_H

#include "ipl98_types.h"
#include "../kernel_c/image/kernel_functions.h"
#include "image_const_column_it_24bp.h"

namespace ipl{ // use namespace if C++

/** @ingroup iteratorclasses
@{ */

/** Iterator class to access data from the class CImage, last updated 11/11/2003.
	This class is not complete, only a few operators are overloaded. But it can
	be used as a basic iterator where the iterator is incremented with the ++
	operator.
	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CImageColumnIterator24bp ipl98/cpp/image_column_it_24bp.h
	@see CImageConstColumnIterator24bp
	@see CImage
	@version 0.60,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
class CImageColumnIterator24bp
{
	/** Since a pixel for 24 b/p images is not a generic type nor a class, this intermediate
		type is needed in order to give the correct iterator interface to the programmer.
		Effort has been made to make it possible for compilers to optimize the code, at present
		it has been tested with the Visual C++ 6.0 compiler, and efficiency is indeed preserved. */
	class CIntermediateType
	{
		/// Pointer to the CImageColumnIterator24bp::m_pTImg member, set by constructor
		UINT8** m_ppCurr;
		public:
		/// Constructor
		inline CIntermediateType(UINT8** ppCurr):m_ppCurr(ppCurr){}
		/// overloading = operator
		inline CIntermediateType& operator=(const CIntermediateType& Obj){*m_ppCurr=*Obj.m_ppCurr;return *this;}
		/** overloading = operator with UINT32 as r-value. This is necessary when the CImageColumnIterator24bp
			iterator is used as l-value like this: 
			\verbatim
			*it = UINT32(Color);
			\endverbatim */
		inline CIntermediateType& operator=(UINT32 Color){**m_ppCurr = (char)Color;*(*m_ppCurr+1)=(char)(Color>>8);*(*m_ppCurr+2)=(char)(Color>>16);return *this;};
		/** overloading operator() (function operator). This is necessary when the CImageColumnIterator24bp
			iterator is used as l-value like this: 
			\verbatim
			UINT32 Color = *it;
			\endverbatim
			*it is converted to the UINT32 Color value at the current position by this 
			operator overloading. */
		inline operator UINT32(){return (UINT32)(*(UINT32*)(*m_ppCurr) & 0xffffff);}
	};
	/** Since a pixel for 24 b/p images is not a generic type nor a class, this intermediate
		type is needed in order to give the correct iterator interface to the programmer for
		the []-operator.
		Effort has been made to make it possible for compilers to optimize the code, at present
		it has been tested with the Visual C++ 6.0 compiler, and efficiency is indeed preserved.
		This extra intermediate type is needed, since CIntermediateType contains a reference to 
		the original CImageColumnIterator24bp::m_pCurr, but when indexing with []-operator,
		we need a temporary offset to the actual pixel indexed by the []-operator. */
	class CIntermediateType2
	{
		/// Pointer to the actual pixel in CImageColumnIterator24bp::m_pTImg member, set by constructor
		UINT8* m_pCurr;
		public:
		/// Constructor
		inline CIntermediateType2(UINT8* pCurr):m_pCurr(pCurr){}
		/** overloading = operator with UINT32 as r-value. This is necessary when the CImageColumnIterator24bp
			iterator is used as l-value like this: 
			\verbatim
			it[x] = UINT32(Color);
			\endverbatim */
		inline CIntermediateType2& operator=(UINT32 Color){*m_pCurr = (char)Color;*(m_pCurr+1)=(char)(Color>>8);*(m_pCurr+2)=(char)(Color>>16);return *this;};
		/** overloading operator() (function operator). This is necessary when the CImageColumnIterator24bp
			iterator is used as l-value like this: 
			\verbatim
			UINT32 Color = it[x];
			\endverbatim
			it[x] is converted to the UINT32 Color value at the current position by this 
			operator overloading. */
		inline operator UINT32(){return (UINT32)(*(UINT32*)(m_pCurr) & 0xffffff);}
	};
protected: // attributes
	/// Pointer to TImage structure of the image to be iterated
	const TImage* m_pTImg;
	/// Pointer to current element in actual column
    UINT8* m_pCurr;
	/// Pointer to first element in actual column
	UINT8* m_pBegin;
	/// Pointer to last element+1 in actual column
	UINT8* m_pEnd;
	/** Since a pixel for 24 b/p images is not a generic type nor a class, this intermediate
		type is needed in order to give the correct iterator interface to the programmer */
	CIntermediateType m_IntermediateType;
public:
	/** Constructor
		@param ColumnIndex Index of actual row, without origo and ROI.
		@param StartIndex Index to start iteration from in actual column, without origo and ROI.
		@param MaxIndex Max index + 1, where iteration should end, used to set the return value 
			for End(), without origo and ROI.
		@param pTImg Pointer to a TImage structure, a member of class CImage. */
	inline CImageColumnIterator24bp(UINT32 ColumnIndex, UINT32 StartIndex, UINT32 MaxIndex, const TImage* pTImg);
	/// Cast to corresponding const iterator in mixed expressions
	inline operator CImageConstColumnIterator24bp(){return CImageConstColumnIterator24bp(m_pCurr, m_pBegin,m_pEnd, m_pTImg);}
	/// overloading = operator
	inline CImageColumnIterator24bp& operator=(const CImageColumnIterator24bp& It);
	/** Returns an iterator to the first element in current row or column.
		@return First element. */
	inline CImageColumnIterator24bp Begin() const;
	/** Returns an iterator to the last element plus one in current row or column.
		@return Last element plus one. */
	inline CImageColumnIterator24bp End() const;
	/// overloading + operator
	inline CImageColumnIterator24bp operator+(int Value) const {CImageColumnIterator24bp t=*this; t+=Value; return t;}
	/// overloading - operator
	inline CImageColumnIterator24bp operator-(int Value) const {CImageColumnIterator24bp t=*this; t+=Value; return t;}
	/** overloading ++ operator. Increments the pointer to data. 
		Note: the postfix operator++ is not implemented in this class since the resulting
		code cannot be optimized. Using that operator would remove the point of using this 
		iterator class. */
	inline CImageColumnIterator24bp& operator++() { m_pCurr-=m_pTImg->ByteWidth; return *this; }
	/** overloading -- operator. Decrements the pointer to data. 
		Note: the postfix operator-- is not implemented in this class since the resulting
		code cannot be optimized. Using that operator would remove the point of using this 
		iterator class. */
	inline CImageColumnIterator24bp& operator--() {m_pCurr+=m_pTImg->ByteWidth; return *this; }
	/** overloading += operator. Increments pointer to data by Value. */
	inline CImageColumnIterator24bp operator +=(int Value){ m_pCurr-=m_pTImg->ByteWidth*Value; return *this;}
	/** overloading -= operator. Decrements pointer to data by Value. */
	inline CImageColumnIterator24bp operator -=(int Value){ m_pCurr+=m_pTImg->ByteWidth*Value; return *this;}
	/** overloading the dereference operator, typically used when iterating. */
	inline CIntermediateType& operator*(){return m_IntermediateType;}
	/** overloading [] operator. */
	inline CIntermediateType2 operator[](UINT32 i) {return CIntermediateType2(m_pBegin-m_pTImg->ByteWidth*i);}
	/** overloading == operator.
		@return true, if iterator p and q points to same element in same column. */
	inline friend bool operator==(const CImageColumnIterator24bp& p, const CImageColumnIterator24bp& q);
	/** overloading != operator.
		@return true, if iterator p and q points to different elements. */
	inline friend bool operator!=(const CImageColumnIterator24bp& p, const CImageColumnIterator24bp& q);
	/** overloading < operator. Return value when comparing operators from different columns is undefined.
		@return true, if iterator p and q points to same column and p has column index < q's column index. */
	inline friend bool operator< (const CImageColumnIterator24bp& p, const CImageColumnIterator24bp& q);
	/** overloading <= operator. Return value when comparing operators from different columns is undefined.
		@return true, if iterator p and q points to same column and p has column index <= q's column index. */
	inline friend bool operator<= (const CImageColumnIterator24bp& p, const CImageColumnIterator24bp& q);
	/** overloading > operator. Return value when comparing operators from different columns is undefined.
		@return true, if iterator p and q points to same column and p has column index > q's column index. */
	inline friend bool operator> (const CImageColumnIterator24bp& p, const CImageColumnIterator24bp& q);
	/** overloading >= operator. Return value when comparing operators from different columns is undefined.
		@return true, if iterator p and q points to same column and p has column index >= q's column index. */
	inline friend bool operator>= (const CImageColumnIterator24bp& p, const CImageColumnIterator24bp& q);
};

inline CImageColumnIterator24bp::CImageColumnIterator24bp(UINT32 ColumnIndex, UINT32 StartIndex, UINT32 MaxIndex,
															const TImage* pTImg):
		m_pTImg(pTImg), m_pCurr(pTImg->ppAllScanLines[StartIndex]+ColumnIndex*3), m_pBegin(m_pCurr),
			m_pEnd(m_pCurr-pTImg->ByteWidth*(MaxIndex-StartIndex)), m_IntermediateType(&m_pCurr)
{
}


inline CImageColumnIterator24bp& CImageColumnIterator24bp::operator=(const CImageColumnIterator24bp& It)
{
	m_pTImg  = It.m_pTImg;
	m_pCurr  = It.m_pCurr;
	m_pBegin = It.m_pBegin;
	m_pEnd   = It.m_pEnd;
	m_IntermediateType = It.m_IntermediateType;
	return *this;
}

inline CImageColumnIterator24bp CImageColumnIterator24bp::Begin() const
{
	CImageColumnIterator24bp t = *this;
	t.m_pCurr = m_pBegin;
	return t;
}

inline CImageColumnIterator24bp CImageColumnIterator24bp::End() const
{
	CImageColumnIterator24bp t = *this;
	t.m_pCurr = m_pEnd;
	return t;
}

inline bool operator==(const CImageColumnIterator24bp& p, const CImageColumnIterator24bp& q)
{
	return (p.m_pCurr==q.m_pCurr);
}

inline bool operator!=(const CImageColumnIterator24bp& p, const CImageColumnIterator24bp& q)
{
	return !(p==q);
}

inline bool operator<(const CImageColumnIterator24bp& p, const CImageColumnIterator24bp& q)
{
	// comparison opposite for columns since scan lines are stored upside down!
	return (p.m_pCurr>q.m_pCurr);
}

inline bool operator<= (const CImageColumnIterator24bp& p, const CImageColumnIterator24bp& q)
{
	// comparison opposite for columns since scan lines are stored upside down!
	return (p.m_pCurr>=q.m_pCurr);
}

inline bool operator> (const CImageColumnIterator24bp& p, const CImageColumnIterator24bp& q)
{
	// comparison opposite for columns since scan lines are stored upside down!
	return (p.m_pCurr<q.m_pCurr);
}

inline bool operator>= (const CImageColumnIterator24bp& p, const CImageColumnIterator24bp& q)
{
	// comparison opposite for columns since scan lines are stored upside down!
	return (p.m_pCurr<=q.m_pCurr);
}

/*@}*/ /* end group "iteratorclasses" */

} // end namespace ipl

#endif //_IPL98_IMAGECOLUMNIT24BP_H
