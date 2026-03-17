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

#ifndef _IPL98_PIXELGROUPIT_H
#define _IPL98_PIXELGROUPIT_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "../kernel_c/ipl98_types_c_only.h"
#include "../../points/point2d.h"

#include "pixelgroup_const_it.h"

namespace ipl{ // use namespace if C++

/** Iterator class to access data from the class CPixelGroup, last updated 3/6/2003.
	This class is not complete, only a few operators are overloaded. But it can
	be used as a basic iterator where the iterator is incremented with the ++
	operator or decremented with the -- operator. See example in CPixelGroup class.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CPixelGroupIterator ipl98/cpp/pixelgroup_it.h
	@see CPixelGroup
	@version 0.50,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
class CPixelGroupIterator
{
protected: // attributes
	/// Index of current element
    T2DInt* m_pCurr;
public:
	/// Default constructor
	CPixelGroupIterator(){ }
	/// Constructor with initialization of iterator
	CPixelGroupIterator(T2DInt* pCurr) :m_pCurr(pCurr){ }
	/// Cast to const iterator in mixed expressions
	operator CPixelGroupConstIterator(){return CPixelGroupConstIterator(m_pCurr);}
	/// overloading = operator
	inline CPixelGroupIterator& operator=(const CPixelGroupIterator& It);
	/** overloading ++ operator (prefix). Increments the pointer to data. */
	inline CPixelGroupIterator& operator++() {++m_pCurr; return *this;}
	/** overloading ++ operator (postfix). Increments the pointer to data. */
	inline CPixelGroupIterator operator++(int) {CPixelGroupIterator t = *this; m_pCurr++; return t;}
	/** overloading -- operator (prefix). Decrements the pointer to data. */
	inline CPixelGroupIterator& operator--() {--m_pCurr; return *this;}
	/** overloading -- operator (postfix). Decrements the pointer to data. */
	inline CPixelGroupIterator operator--(int) {CPixelGroupIterator t = *this; m_pCurr--; return t;}
	/** overloading the dereference operator, typically used when iterating. */
	inline T2DInt& operator*(){return *m_pCurr;}
	/** overloading the dereference operator as const, typically used when iterating. */
	inline T2DInt& operator*() const{return *m_pCurr;}
	/** overloading the dereference operator, typically used when iterating. */
	inline T2DInt* operator->(){return m_pCurr;}
	/** Converts the contents of the iterator to a CPoint2D. Note, that this should
		only be done if speed is not a major concern, since it takes time to construct
		a CPoint2D type from the internal T2DInt structure. */
	inline CPoint2D<INT16> ToCPoint2D(){return CPoint2D<INT16>(m_pCurr->x,m_pCurr->y);}
	/** overloading == operator.
		@return true, if iterator p and q points to same element. */
	friend bool operator==(const CPixelGroupIterator& p, const CPixelGroupIterator& q){return p.m_pCurr==q.m_pCurr;}
	/** overloading != operator.
		@return true, if iterator p and q points to different elements. */
	friend bool operator!=(const CPixelGroupIterator& p, const CPixelGroupIterator& q){return !(p==q);}
	/** overloading < operator.
		@return true, if iterator p points to element earlier than q. */
	friend bool operator<(const CPixelGroupIterator& p, const CPixelGroupIterator& q){return p.m_pCurr<q.m_pCurr;}
	/** overloading <= operator.
		@return true, if iterator p points to element earlier than q. */
	friend bool operator<=(const CPixelGroupIterator& p, const CPixelGroupIterator& q){return p.m_pCurr<=q.m_pCurr;}
	/** overloading > operator.
		@return true, if iterator p points to element earlier than q. */
	friend bool operator>(const CPixelGroupIterator& p, const CPixelGroupIterator& q){return p.m_pCurr>q.m_pCurr;}
	/** overloading >= operator.
		@return true, if iterator p points to element earlier than q. */
	friend bool operator>=(const CPixelGroupIterator& p, const CPixelGroupIterator& q){return p.m_pCurr>=q.m_pCurr;}
};


inline CPixelGroupIterator& CPixelGroupIterator::operator=(const CPixelGroupIterator& It)
{
	m_pCurr=It.m_pCurr;
	return *this;
}

/** Overloading the == operator with T2DInt as left operand and CPoint2D<INT16> as right operand.
	Mainly used when working with CPixelGroup2D and CPoint2D at the same time. */
inline bool operator==(const T2DInt& p, const CPoint2D<INT16>& q){return ((p.x==q.GetX()) && (p.y==q.GetY()));}
/** Overloading the != operator with T2DInt as left operand and CPoint2D<INT16> as right operand.
	Mainly used when working with CPixelGroup2D and CPoint2D at the same time. */
inline bool operator!=(const T2DInt& p, const CPoint2D<INT16>& q){return ((p.x!=q.GetX()) || (p.y!=q.GetY()));}
/** Overloading the < operator with T2DInt as left operand and CPoint2D<INT16> as right operand.
	First comparison on x, if equal comparison on y is performed.
	Mainly used when working with CPixelGroup2D and CPoint2D at the same time. */
inline bool operator<(const T2DInt& p, const CPoint2D<INT16>& q){if (p.x==q.GetX()){return (p.y<q.GetY());}else{return (p.x<q.GetX());}}
/** Overloading the <= operator with T2DInt as left operand and CPoint2D<INT16> as right operand.
	First comparison on x, if equal comparison on y is performed.
	Mainly used when working with CPixelGroup2D and CPoint2D at the same time. */
inline bool operator<=(const T2DInt& p, const CPoint2D<INT16>& q){if (p.x==q.GetX()){return (p.y<=q.GetY());}else{return (p.x<q.GetX());}}
/** Overloading the < operator with T2DInt as left operand and CPoint2D<INT16> as right operand.
	First comparison on x, if equal comparison on y is performed.
	Mainly used when working with CPixelGroup2D and CPoint2D at the same time. */
inline bool operator>(const T2DInt& p, const CPoint2D<INT16>& q){if (p.x==q.GetX()){return (p.y>q.GetY());}else{return (p.x>q.GetX());}}
/** Overloading the >= operator with T2DInt as left operand and CPoint2D<INT16> as right operand.
	First comparison on x, if equal comparison on y is performed.
	Mainly used when working with CPixelGroup2D and CPoint2D at the same time. */
inline bool operator>=(const T2DInt& p, const CPoint2D<INT16>& q){if (p.x==q.GetX()){return (p.y>=q.GetY());}else{return (p.x>q.GetX());}}

/** Overloading the == operator with CPoint2D as left operand and T2DInt as right operand.
	Mainly used when working with CPixelGroup2D and CPoint2D at the same time. */
inline bool operator==(const CPoint2D<INT16>& q, const T2DInt& p){return ((p.x==q.GetX()) && (p.y==q.GetY()));}
/** Overloading the != operator with CPoint2D as left operand and T2DInt as right operand.
	Mainly used when working with CPixelGroup2D and CPoint2D at the same time. */
inline bool operator!=(const CPoint2D<INT16>& q, const T2DInt& p){return ((p.x!=q.GetX()) || (p.y!=q.GetY()));}
/** Overloading the < operator with CPoint2D as left operand and T2DInt as right operand.
	First comparison on x, if equal comparison on y is performed.
	Mainly used when working with CPixelGroup2D and CPoint2D at the same time. */
inline bool operator<(const CPoint2D<INT16>& q, const T2DInt& p){if (p.x==q.GetX()){return (p.y<q.GetY());}else{return (p.x<q.GetX());}}
/** Overloading the <= operator with CPoint2D as left operand and T2DInt as right operand.
	First comparison on x, if equal comparison on y is performed.
	Mainly used when working with CPixelGroup2D and CPoint2D at the same time. */
inline bool operator<=(const CPoint2D<INT16>& q, const T2DInt& p){if (p.x==q.GetX()){return (p.y<=q.GetY());}else{return (p.x<q.GetX());}}
/** Overloading the < operator with CPoint2D as left operand and T2DInt as right operand.
	First comparison on x, if equal comparison on y is performed.
	Mainly used when working with CPixelGroup2D and CPoint2D at the same time. */
inline bool operator>(const CPoint2D<INT16>& q, const T2DInt& p){if (p.x==q.GetX()){return (p.y>q.GetY());}else{return (p.x>q.GetX());}}
/** Overloading the >= operator with CPoint2D as left operand and T2DInt as right operand.
	First comparison on x, if equal comparison on y is performed.
	Mainly used when working with CPixelGroup2D and CPoint2D at the same time. */
inline bool operator>=(const CPoint2D<INT16>& q, const T2DInt& p){if (p.x==q.GetX()){return (p.y>=q.GetY());}else{return (p.x>q.GetX());}}

} // end namespace ipl

#endif //_IPL98_PIXELGROUPIT_H
