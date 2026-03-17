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

#ifndef _IPL98_CPOSITIONGROUP2DCONSTIT_H
#define _IPL98_CPOSITIONGROUP2DCONSTIT_H

#include "../ipl98_setup.h" /* always include the setup file */
#include <vector>

namespace ipl{ // use namespace if C++

using std::vector;

/** @ingroup iteratorclasses
@{ */

/** Iterator class to access data from the class CPositionGroup2D, last updated 2/24/2003.
	This class is not complete, only a few operators are overloaded. But it can
	be used as a basic iterator where the iterator is incremented with the ++
	operator or decremented with the -- operator.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@see CPositionGroup2DIterator
	@see CPositionGroup2D
	@version 0.30,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
template<class T>
class CPositionGroup2DConstIterator
{
protected: // attributes
	/// Index of current element
	typename std::vector<T>::const_iterator m_It;
public:
	/// Default constructor
	CPositionGroup2DConstIterator(){ }
	/// Constructor
	CPositionGroup2DConstIterator(typename std::vector<T>::const_iterator It) :m_It(It) {}
	/// overloading = operator
	inline CPositionGroup2DConstIterator<T>& operator=(const CPositionGroup2DConstIterator<T>& It);
	/** overloading ++ operator (prefix). Increments the pointer to data. */
	inline CPositionGroup2DConstIterator<T>& operator++() {++m_It; return *this;}
	/** overloading ++ operator (postfix). Increments the pointer to data. */
	inline CPositionGroup2DConstIterator<T> operator++(int) {CPositionGroup2DConstIterator<T> t = *this; m_It++; return t;}
	/** overloading -- operator (prefix). Decrements the pointer to data. */
	inline CPositionGroup2DConstIterator<T>& operator--() {--m_It; return *this;}
	/** overloading -- operator (postfix). Decrements the pointer to data. */
	inline CPositionGroup2DConstIterator<T> operator--(int) {CPositionGroup2DConstIterator<T> t = *this; m_It--; return t;}
	/** overloading the dereference operator, typically used when iterating. */
	inline const T& operator*(){return *m_It;}
	/** overloading the dereference operator, typically used when iterating. */
	inline const T* operator->(){return &(*m_It);}
	/** overloading == operator.
		@return true, if iterator p and q points to same element. */
	friend bool operator==(const CPositionGroup2DConstIterator<T>& p, const CPositionGroup2DConstIterator<T>& q){return p.m_It==q.m_It;}
	/** overloading != operator.
		@return true, if iterator p and q points to different elements. */
	friend bool operator!=(const CPositionGroup2DConstIterator<T>& p, const CPositionGroup2DConstIterator<T>& q){return !(p==q);}
	/** overloading < operator.
		@return true, if iterator p points to element earlier than q. */
	friend bool operator<(const CPositionGroup2DConstIterator<T>& p, const CPositionGroup2DConstIterator<T>& q){return p.m_It<q.m_It;}
	/** overloading <= operator.
		@return true, if iterator p points to element earlier than q. */
	friend bool operator<=(const CPositionGroup2DConstIterator<T>& p, const CPositionGroup2DConstIterator<T>& q){return p.m_It<=q.m_It;}
	/** overloading > operator.
		@return true, if iterator p points to element earlier than q. */
	friend bool operator>(const CPositionGroup2DConstIterator<T>& p, const CPositionGroup2DConstIterator<T>& q){return p.m_It>q.m_It;}
	/** overloading >= operator.
		@return true, if iterator p points to element earlier than q. */
	friend bool operator>=(const CPositionGroup2DConstIterator<T>& p, const CPositionGroup2DConstIterator<T>& q){return p.m_It>=q.m_It;}
};


template<class T>
inline CPositionGroup2DConstIterator<T>& CPositionGroup2DConstIterator<T>::operator=(const CPositionGroup2DConstIterator<T>& It)
{
	m_It=It.m_It;
	return *this;
}

/*@}*/ /* end group "iteratorclasses" */

} // end namespace ipl

#endif //_IPL98_CPOSITIONGROUP2DCONSTIT_H
