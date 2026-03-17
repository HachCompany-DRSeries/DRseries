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

#ifndef _IPL98_PIXELGROUPCONSTIT_H
#define _IPL98_PIXELGROUPCONSTIT_H

#include "../ipl98_setup.h" /* always include the setup file */

namespace ipl{ // use namespace if C++

/** Iterator class to access data from the class CPixelGroup, last updated 24/2/2003.
	This class is not complete, only a few operators are overloaded. But it can
	be used as a basic iterator where the iterator is incremented with the ++
	operator or decremented with the -- operator. See example in CPixelGroup class.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CPixelGroupConstIterator ipl98/cpp/pixelgroup_const_it.h
	@see CPixelGroupIterator
	@see CPixelGroup
	@version 0.30,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
class CPixelGroupConstIterator
{
protected: // attributes
	/// Index of current element
    T2DInt* m_pCurr;
public:
	/// Default constructor
	CPixelGroupConstIterator(){ }
	/// Constructor
	CPixelGroupConstIterator(T2DInt* pCurr) :m_pCurr(pCurr){ }
	/// overloading = operator
	inline CPixelGroupConstIterator& operator=(const CPixelGroupConstIterator& It);
	/** overloading ++ operator (prefix). Increments the pointer to data. */
	inline CPixelGroupConstIterator& operator++() {++m_pCurr; return *this;}
	/** overloading ++ operator (postfix). Increments the pointer to data. */
	inline CPixelGroupConstIterator operator++(int) {CPixelGroupConstIterator t = *this; m_pCurr++; return t;}
	/** overloading -- operator (prefix). Decrements the pointer to data. */
	inline CPixelGroupConstIterator& operator--() {--m_pCurr; return *this;}
	/** overloading -- operator (postfix). Decrements the pointer to data. */
	inline CPixelGroupConstIterator operator--(int) {CPixelGroupConstIterator t = *this; m_pCurr--; return t;}
	/** overloading the dereference operator, typically used when iterating. */
	inline const T2DInt& operator*(){return *m_pCurr;}
	/** overloading the dereference operator, typically used when iterating. */
	inline const T2DInt* operator->(){return m_pCurr;}
	/** Converts the contents of the iterator to a CPoint2D. Note, that this should
		only be done if speed is not a major concern, since it takes time to construct
		a CPoint2D type from the internal T2DInt structure. */
	inline CPoint2D<INT16> ToCPoint2D(){return CPoint2D<INT16>(m_pCurr->x,m_pCurr->y);}
	/** overloading == operator.
		@return true, if iterator p and q points to same element. */
	friend bool operator==(const CPixelGroupConstIterator& p, const CPixelGroupConstIterator& q){return p.m_pCurr==q.m_pCurr;}
	/** overloading != operator.
		@return true, if iterator p and q points to different elements. */
	friend bool operator!=(const CPixelGroupConstIterator& p, const CPixelGroupConstIterator& q){return !(p==q);}
	/** overloading < operator.
		@return true, if iterator p points to element earlier than q. */
	friend bool operator<(const CPixelGroupConstIterator& p, const CPixelGroupConstIterator& q){return p.m_pCurr<q.m_pCurr;}
	/** overloading <= operator.
		@return true, if iterator p points to element earlier than q. */
	friend bool operator<=(const CPixelGroupConstIterator& p, const CPixelGroupConstIterator& q){return p.m_pCurr<=q.m_pCurr;}
	/** overloading > operator.
		@return true, if iterator p points to element earlier than q. */
	friend bool operator>(const CPixelGroupConstIterator& p, const CPixelGroupConstIterator& q){return p.m_pCurr>q.m_pCurr;}
	/** overloading >= operator.
		@return true, if iterator p points to element earlier than q. */
	friend bool operator>=(const CPixelGroupConstIterator& p, const CPixelGroupConstIterator& q){return p.m_pCurr>=q.m_pCurr;}
};


inline CPixelGroupConstIterator& CPixelGroupConstIterator::operator=(const CPixelGroupConstIterator& It)
{
	m_pCurr=It.m_pCurr;
	return *this;
}

} // end namespace ipl

#endif //_IPL98_PIXELGROUPCONSTIT_H
