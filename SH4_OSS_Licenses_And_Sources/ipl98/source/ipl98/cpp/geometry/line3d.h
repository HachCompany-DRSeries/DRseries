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

#ifndef _IPL98_LINE3D_H
#define _IPL98_LINE3D_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../../../points/point3d.h"
#include "../vectors/vector3d.h"
#include <sstream>
#include "../error.h"

// Forward declarations of 'template friend' functions.
namespace ipl 
{
    template<class T>
    class CLine3D;

    template<class T>
    std::ostream& operator<<(
        std::ostream& s,
        const CLine3D<T>& Line);

    template<class T>
    std::istream& operator>>(
        std::istream& is,
        CLine3D<T>& Line);
}

namespace ipl{ // use namespace ipl

/** @ingroup containerclasses
@{ */

/** A class defining a 3 dimensional line by a 3D point and a
	direction vector, last updated 3/7/2005.
	The template type should be a floating point, i.e. type float or double.
	
	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CLine3D ipl98/cpp/geometry/line3d.h
	@version 0.63,  by ivb@mip.sdu.dk and edr@mip.sdu.dk
	@author Ivar Balslev and René Dencker Eriksen */
template<class T>
class CLine3D
{
	private: // attributes
		/// Defines a point on line
		CPoint3D<T> m_P;
		/// defines the 3D direction of line
		CVector3D<T> m_Dir;
	public: // methods
		/// default constructor, line point defaults to (0,0,0) and direction to (1,0,0)
		CLine3D();
		/** Constructor defining the plane.
			@param P Point on line.
			@param Dir Direction of line, must not be a zero vector. */			
		CLine3D(const CPoint3D<T>& P, const CVector3D<T>& Dir);
		/// copy constructor
		CLine3D(const CLine3D<T>& Line3D);
		/** Overloading of assignment operator. All attributes are copied
			to the destination. */
		CLine3D<T>& operator=(const CLine3D<T>& Line3D);
		/** Returns the direction of the line.
			@return Direction of line. */
		inline const CVector3D<T>& GetDir() const;
		/** Returns the point used to define the line.
			@return Point used to define the line. */
		inline const CPoint3D<T>& GetPoint() const;
		/** Defines a line by given point P and direction Dir.
			@param P Point on line.
			@param Dir Direction of line.
			@return False, if Dir is a zero vector. In that case nothing is changed
				in this object. */
		bool Set(const CPoint3D<T>& P, const CVector3D<T>& Dir);
		/** Calculates the intersectino point between this line and the provided line L.
			If they do not intersect the middle point for nearest intersection is 
			returned.
			@param L Line to calculate pseudo intersection with.
			@param PseudoIntersectionPoint Intersection point returned in this parameter.
			@param MinDistance The minimum distance between the two lines, if
				not 0, the returned intersection point is a pseudo intersection.
			@return False, if this line and L are parallel.
			@version 0.1 */
		inline bool PseudoIntersect(const CLine3D<T>& L, CPoint3D<T> &PseudoIntersectionPoint, 
								T& MinDistance) const;
		/** Calculates the distance between the specified point and this line
			@param P the specified Point.
			@return the distance between the line and the point */
		double Distance(const CPoint3D<T>& P) const;
		/** Returns the projection of the provided point onto this plane.
			@param P Point to be projected onto this plane.
			@return Point projected onto this plane. */
		CPoint3D<T> GetProjection(const CPoint3D<T>& P) const;
	/**@name streaming operators */
	//@{
	/** writes object to stream, format is:
		\verbatim
		
		CLine3D Point: position Dir: direction
		\endverbatim
		The token "Point" is followed by the stream operator used on the
		m_P attribute which is a CPoint3D<T> type. The token "Dir" is 
		followed by the stream operator used on the m_Dir attribute which 
		is a CVector3D<T> type.
		Since this is a one line output, StreamIndent() is not called.
		@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
		@version 0.6 */
	friend std::ostream& operator<< __IPL_TF(std::ostream& s, const CLine3D<T>& Line);

	/** Reads input from given stream into this class. Stream must be in 
		correct format according to how the << ostream operator formats output.
		@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
		@version 0.6 */
	friend std::istream& operator>> __IPL_TF(std::istream& is, CLine3D<T>& Line);
	//@}
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

template<class T>
inline const CVector3D<T>& CLine3D<T>::GetDir() const
{
	return m_Dir;
}

template<class T>
inline const CPoint3D<T>& CLine3D<T>::GetPoint() const
{
	return m_P;
}

template <class T>
inline bool CLine3D<T>::PseudoIntersect(const CLine3D<T>& L, CPoint3D<T>& PseudoIntersectionPoint, T& MinDistance) const
{
	T t1,t2,N,D1sq,D2sq,D1D2,D1Q21,D2Q21;
	CVector3D<T> Buf,P1,P2;
	CVector3D<T> Q21;
	D1sq=GetDir()*GetDir();
	D2sq=L.GetDir()*L.GetDir();
	D1D2=GetDir()*L.GetDir();
	Q21=CVector3D<T>(L.GetPoint()-GetPoint());
	D1Q21=GetDir()*Q21;
	D2Q21=L.GetDir()*Q21;
	N=D1sq*D2sq-D1D2*D1D2;
	if (N==0) 
		return false;//Lines are parallel
	
	t1=( D1Q21*D2sq-D1D2*D2Q21)/N;
	t2=(-D2Q21*D1sq+D1D2*D1Q21)/N;
	P1=CVector3D<T>(GetPoint())+GetDir()*t1;
	P2=CVector3D<T>(L.GetPoint())+L.GetDir()*t2;
	Buf=P2-P1;
	MinDistance=sqrt(Buf*Buf);
	Buf=P1+P2;
	PseudoIntersectionPoint=Buf*0.5;
	return true;
}

#include "line3d.cpp"

/*@}*/ /* end group "containerclasses" */

} // end namespace ipl

#endif //_IPL98_LINE3D_H
