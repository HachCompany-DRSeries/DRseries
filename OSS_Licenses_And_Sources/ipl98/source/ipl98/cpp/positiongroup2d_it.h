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

#ifndef _IPL98_CPOSITIONGROUP2DIT_H
#define _IPL98_CPOSITIONGROUP2DIT_H

#include "../ipl98_setup.h" /* always include the setup file */
#include <vector>
#include "positiongroup2d_const_it.h"

namespace ipl{ // use namespace if C++

using std::vector;

/** @ingroup iteratorclasses
@{ */

/** Iterator class to access data from the class CPositionGroup2D, last updated 24/2/2003.
	This class is not complete, only a few operators are overloaded. But it can
	be used as a basic iterator where the iterator is incremented with the ++
	operator or decremented with the -- operator.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CPositionGroup2DIterator ipl98/cpp/positiongroup2d_it.h
	@see CPositionGroup2DConstIterator
	@see CPositionGroup2D
	@version 0.30,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
template<class T>
class CPositionGroup2DIterator
{
protected: // attributes
	/// Index of current element
	typename std::vector<T>::iterator m_It;
public:
	/// Default constructor
	CPositionGroup2DIterator(){ }
	/// Constructor
	CPositionGroup2DIterator(typename std::vector<T>::iterator It) :m_It(It){ }
	/// Cast to const iterator in mixed expressions
	operator CPositionGroup2DConstIterator<T>(){return CPositionGroup2DConstIterator<T>(m_It);}
	/// overloading = operator
	inline CPositionGroup2DIterator<T>& operator=(const CPositionGroup2DIterator<T>& It);
	/** overloading ++ operator (prefix). Increments the pointer to data. */
	inline CPositionGroup2DIterator<T>& operator++() {++m_It; return *this;}
	/** overloading ++ operator (postfix). Increments the pointer to data. */
	inline CPositionGroup2DIterator<T> operator++(int) {CPositionGroup2DIterator<T> t = *this; m_It++; return t;}
	/** overloading -- operator (prefix). Decrements the pointer to data. */
	inline CPositionGroup2DIterator<T>& operator--() {--m_It; return *this;}
	/** overloading -- operator (postfix). Decrements the pointer to data. */
	inline CPositionGroup2DIterator<T> operator--(int) {CPositionGroup2DIterator<T> t = *this; m_It--; return t;}
	/** overloading the dereference operator, typically used when iterating. */
	inline T& operator*(){return *m_It;}
	/** overloading the dereference operator as const, typically used when iterating. */
	inline const T& operator*() const{return *m_It;}
	/** overloading the dereference operator, typically used when iterating. */
	inline T* operator->(){return m_It;}
	/** overloading == operator.
	@return true, if iterator p and q points to same element. */
	friend bool operator==(const CPositionGroup2DIterator<T>& p, const CPositionGroup2DIterator<T>& q){return p.m_It==q.m_It;}
	/** overloading != operator.
	@return true, if iterator p and q points to different elements. */
	friend bool operator!=(const CPositionGroup2DIterator<T>& p, const CPositionGroup2DIterator<T>& q){return !(p==q);}
	/** overloading < operator.
	@return true, if iterator p points to element earlier than q. */
	friend bool operator<(const CPositionGroup2DIterator<T>& p, const CPositionGroup2DIterator<T>& q){return p.m_It<q.m_It;}
	/** overloading <= operator.
		@return true, if iterator p points to element earlier than q. */
	friend bool operator<=(const CPositionGroup2DIterator<T>& p, const CPositionGroup2DIterator<T>& q){return p.m_It<=q.m_It;}
	/** overloading > operator.
		@return true, if iterator p points to element earlier than q. */
	friend bool operator>(const CPositionGroup2DIterator<T>& p, const CPositionGroup2DIterator<T>& q){return p.m_It>q.m_It;}
	/** overloading >= operator.
		@return true, if iterator p points to element earlier than q. */
	friend bool operator>=(const CPositionGroup2DIterator<T>& p, const CPositionGroup2DIterator<T>& q){return p.m_It>=q.m_It;}
};


template<class T>
inline CPositionGroup2DIterator<T>& CPositionGroup2DIterator<T>::operator=(const CPositionGroup2DIterator<T>& It)
{
	m_It=It.m_It;
	return *this;
}

/*@}*/ /* end group "iteratorclasses" */

} // end namespace ipl

#endif //_IPL98_CPOSITIONGROUP2DIT_H
