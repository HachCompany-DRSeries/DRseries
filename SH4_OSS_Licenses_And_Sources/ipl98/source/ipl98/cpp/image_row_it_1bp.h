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

#ifndef _IPL98_IMAGEROWIT1BP_H
#define _IPL98_IMAGEROWIT1BP_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "ipl98_types.h"
#include "../kernel_c/image/kernel_functions.h"
#include "image_const_row_it_1bp.h"

namespace ipl{ // use namespace if C++

/** @ingroup iteratorclasses
@{ */

/** Iterator class to access data from the class CImage, last updated 11/11/2003.
	This class is not complete, only a few operators are overloaded. But it can
	be used as a basic iterator where the iterator is incremented with the ++
	operator.
	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CImageRowIterator1bp ipl98/cpp/image_row_it_1bp.h
	@see CImageRowIterator1bp
	@see CImage
	@version 0.60,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
class CImageRowIterator1bp
{
	/** Since a pixel for 1 b/p images is not a generic type nor a class, this intermediate
		type is needed in order to give the correct iterator interface to the programmer.
		Effort has been made to make it possible for compilers to optimize the code, at present
		it has been tested with the Visual C++ 6.0 compiler, and efficiency is indeed preserved. */
	class CIntermediateType
	{
		/// Pointer to the CImageRowIterator1bp::m_pBegin member, set by constructor
		UINT8** m_ppBegin;
		/// Pointer to the CImageRowIterator1bp::m_Curr member, set by constructor
		unsigned int* m_pCurr;
		public:
		/// Constructor
		inline CIntermediateType(unsigned int* pCurr, UINT8** ppBegin): m_ppBegin(ppBegin),m_pCurr(pCurr){}
		/// overloading = operator
		inline CIntermediateType& operator=(const CIntermediateType& Obj){*m_ppBegin=*Obj.m_ppBegin;*m_pCurr=*Obj.m_pCurr;return *this;}
		/** overloading = operator with UINT32 as r-value. This is necessary when the CImageRowIterator1bp
			iterator is used as l-value like this: 
			\verbatim
			*it = UINT32(Color);
			\endverbatim */
		inline CIntermediateType& operator=(UINT32 Color){(Color ? (*(*m_ppBegin+(*m_pCurr>>3)) |= (0x80>>(*m_pCurr%8))) : (*(*m_ppBegin+(*m_pCurr>>3)) &= (0xff7f>>((*m_pCurr%8)))));return *this;};
		/** overloading operator() (function operator). This is necessary when the CImageRowIterator1bp
			iterator is used as l-value like this: 
			\verbatim
			UINT32 Color = *it;
			\endverbatim
			*it is converted to the UINT32 Color value at the current position by this 
			operator overloading. */
		inline operator UINT32(){return ((*(*m_ppBegin+(*m_pCurr>>3))) & (0x80>>(*m_pCurr%8)) )!=0;}
		/** Called when CImageRowIterator1bp::operator[] is used. */
		inline const UINT32 operator[](UINT32 i){return ((*(*m_ppBegin+((*m_pCurr+i)>>3))) & (0x80>>((*m_pCurr+i)%8)) )!=0;}
	};
	/** Since a pixel for 1 b/p images is not a generic type nor a class, this intermediate
		type is needed in order to give the correct iterator interface to the programmer for
		the []-operator.
		Effort has been made to make it possible for compilers to optimize the code, at present
		it has been tested with the Visual C++ 6.0 compiler, and efficiency is indeed preserved.
		This extra intermediate type is needed, since CIntermediateType contains a reference to 
		the original CImageColumnIterator24bp::m_pCurr, but when indexing with []-operator,
		we need a temporary offset to the actual pixel indexed by the []-operator. */
	class CIntermediateType2
	{
		/// Pointer to the CImageRowIterator1bp::m_pBegin member, set by constructor
		UINT8** m_ppBegin;
		/// Pointer to the CImageRowIterator1bp::m_Curr member, set by constructor
		unsigned int m_Curr;
		public:
		/// Constructor
		inline CIntermediateType2(unsigned int Curr, UINT8** ppBegin):m_ppBegin(ppBegin),m_Curr(Curr){}
		/** overloading = operator with UINT32 as r-value. This is necessary when the CImageRowIterator1bp
			iterator is used as l-value like this: 
			\verbatim
			it[x] = UINT32(Color);
			\endverbatim */
		inline CIntermediateType2& operator=(UINT32 Color){(Color ? (*(*m_ppBegin+(m_Curr>>3)) |= (0x80>>(m_Curr%8))) : (*(*m_ppBegin+(m_Curr>>3)) &= (0xff7f>>((m_Curr%8)))));return *this;};
		/** overloading operator() (function operator). This is necessary when the CImageRowIterator1bp
			iterator is used as l-value like this: 
			\verbatim
			UINT32 Color = it[x];
			\endverbatim
			it[x] is converted to the UINT32 Color value at the current position by this 
			operator overloading. */
		inline operator UINT32(){return ((*(*m_ppBegin+(m_Curr>>3))) & (0x80>>(m_Curr%8)) )!=0;}
	};
protected: // attributes
	/// Counter for last element+1 in actual row
	unsigned int m_End;
	/// Counter for current element in actual row
    unsigned int m_Curr;
	/// Start index in m_pBegin byte
	UINT8 m_StartIndex;
	/// Pointer to start byte in actual row
	UINT8* m_pBegin;
	/** Since a pixel for 1 b/p images is not a generic type nor a class, this intermediate
		type is needed in order to give the correct iterator interface to the programmer */
	CIntermediateType m_IntermediateType;
public:
	/** Constructor
		@param pActualRow Absolute pointer to actual row.
		@param StartIndex Index to start iteration from in actual row, without origo and ROI.
		@param MaxIndex Max index + 1, where iteration should end, used to set the return value 
			for End(), without origo and ROI. */
	inline CImageRowIterator1bp(UINT8* pActualRow, UINT32 StartIndex, UINT32 MaxIndex);
	/// Cast to corresponding const iterator in mixed expressions
	inline operator CImageConstRowIterator1bp(){return CImageConstRowIterator1bp(m_Curr, m_pBegin, m_End, m_StartIndex);}
	/// overloading = operator
	inline CImageRowIterator1bp& operator=(const CImageRowIterator1bp& It);
	/** Returns an iterator to the first element in current row or column.
		@return First element. */
	inline CImageRowIterator1bp Begin() const;
	/** Returns an iterator to the last element plus one in current row or column.
		@return Last element plus one. */
	inline CImageRowIterator1bp End() const;
	/// overloading + operator
	inline CImageRowIterator1bp operator+(int Value) const {CImageRowIterator1bp t=*this; t+=Value; return t;}
	/// overloading - operator
	inline CImageRowIterator1bp operator-(int Value) const {CImageRowIterator1bp t=*this; t+=Value; return t;}
	/** overloading ++ operator. Increments the pointer to data. 
		Note: the postfix operator++ is not implemented in this class since the resulting
		code cannot be optimized. Using that operator would remove the point of using this 
		iterator class. */
	inline CImageRowIterator1bp& operator++() { m_Curr++; return *this; }
	/** overloading -- operator. Decrements the pointer to data. 
		Note: the postfix operator-- is not implemented in this class since the resulting
		code cannot be optimized. Using that operator would remove the point of using this 
		iterator class. */
	inline CImageRowIterator1bp& operator--() { m_Curr--; return *this; }
	/** overloading += operator. Increments pointer to data by Value. */
	inline CImageRowIterator1bp operator +=(int Value){ m_Curr+=Value; return *this;}
	/** overloading -= operator. Decrements pointer to data by Value. */
	inline CImageRowIterator1bp operator -=(int Value){ m_Curr-=Value; return *this;}
	/** overloading the dereference operator, typically used when iterating. */
	inline CIntermediateType& operator*(){return m_IntermediateType;}
	/** overloading [] operator. */
	inline CIntermediateType2 operator[](UINT32 i) {return CIntermediateType2(i,&m_pBegin);}
	/** overloading == operator.
		@return true, if iterator p and q points to same element in same column. */
	inline friend bool operator==(const CImageRowIterator1bp& p, const CImageRowIterator1bp& q);
	/** overloading != operator.
		@return true, if iterator p and q points to different elements. */
	inline friend bool operator!=(const CImageRowIterator1bp& p, const CImageRowIterator1bp& q);
	/** overloading < operator. Return value when comparing operators from different rows is undefined.
		@return true, if iterator p and q points to same column and p has row index < q's row index. */
	inline friend bool operator< (const CImageRowIterator1bp& p, const CImageRowIterator1bp& q);
	/** overloading <= operator. Return value when comparing operators from different rows is undefined.
		@return true, if iterator p and q points to same column and p has row index <= q's row index. */
	inline friend bool operator<= (const CImageRowIterator1bp& p, const CImageRowIterator1bp& q);
	/** overloading > operator. Return value when comparing operators from different rows is undefined.
		@return true, if iterator p and q points to same column and p has row index > q's row index. */
	inline friend bool operator> (const CImageRowIterator1bp& p, const CImageRowIterator1bp& q);
	/** overloading >= operator. Return value when comparing operators from different rows is undefined.
		@return true, if iterator p and q points to same column and p has row index >= q's row index. */
	inline friend bool operator>= (const CImageRowIterator1bp& p, const CImageRowIterator1bp& q);
};

inline CImageRowIterator1bp::CImageRowIterator1bp(UINT8* pActualRow, UINT32 StartIndex, UINT32 MaxIndex):
		m_End(MaxIndex), m_Curr(StartIndex%8), m_StartIndex(m_Curr),
		m_pBegin(pActualRow+(StartIndex>>3)), m_IntermediateType(&m_Curr,&m_pBegin)
{
}

inline CImageRowIterator1bp& CImageRowIterator1bp::operator=(const CImageRowIterator1bp& It)
{
	m_Curr  = It.m_Curr;
	m_StartIndex = It.m_StartIndex;
	m_pBegin = It.m_pBegin;
	m_End   = It.m_End;
	m_IntermediateType = It.m_IntermediateType;
	return *this;
}

inline CImageRowIterator1bp CImageRowIterator1bp::Begin() const
{
	CImageRowIterator1bp t = *this;
	t.m_Curr = m_StartIndex;
	return t;
}

inline CImageRowIterator1bp CImageRowIterator1bp::End() const
{
	CImageRowIterator1bp t = *this;
	t.m_Curr = m_End;
	return t;
}

inline bool operator==(const CImageRowIterator1bp& p, const CImageRowIterator1bp& q)
{
	return (p.m_Curr==q.m_Curr);
}

inline bool operator!=(const CImageRowIterator1bp& p, const CImageRowIterator1bp& q)
{
	return !(p==q);
}

inline bool operator<(const CImageRowIterator1bp& p, const CImageRowIterator1bp& q)
{
	return (p.m_Curr<q.m_Curr);
}

inline bool operator<= (const CImageRowIterator1bp& p, const CImageRowIterator1bp& q)
{
	return (p.m_Curr<=q.m_Curr);
}

inline bool operator> (const CImageRowIterator1bp& p, const CImageRowIterator1bp& q)
{
	return (p.m_Curr>q.m_Curr);
}

inline bool operator>= (const CImageRowIterator1bp& p, const CImageRowIterator1bp& q)
{
	return (p.m_Curr>=q.m_Curr);
}

/*@}*/ /* end group "iteratorclasses" */

} // end namespace ipl

#endif //_IPL98_IMAGEROWIT1BP_H
