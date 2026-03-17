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

#ifndef _IPL98_PLANE_H
#define _IPL98_PLANE_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../../../points/point3d.h"
#include "../vectors/vector3d.h"
#include "line3d.h"

// Forward declarations of 'template friend' functions.
//namespace ipl 
//{
//    template<class T>
//    class CPlane;
//
//    template<class T>
//    std::ostream& operator<<(
//        std::ostream& s,
//        const CPlane<T>& Plane);
//
//    template<class T>
//    std::istream& operator>>(
//        std::istream& is,
//        CPlane<T>& Plane);
//}

namespace ipl{ // use namespace ipl

/** @ingroup containerclasses
@{ */

/** Definines a plane by two vectors and a point. Contains several
	methods for intersection with lines and planes. The template type
	should be a floating point, i.e. type float or double.
	Streaming operators are available but are at the moment not declared friend
	functions due to problems with the Visual C++ 7.1 compiler. The format is:
	\verbatim
	CPlane Point: position DirA: direction DirB: direction
	\endverbatim
	Since streaming is a one line output, StreamIndent() is not called.
	
	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CPlane ipl98/cpp/geometry/plane.h
	@version 0.63,  by ivb@mip.sdu.dk and edr@mip.sdu.dk
	@author Ivar Balslev and René Dencker Eriksen */
template<class T>
class CPlane
{
	private: // attributes
		/// Defines a point in plane
		CPoint3D<T> m_P;
		/// first direction vector defining one dimension
		CVector3D<T> m_DirA;
		/// second direction vector defining second dimension
		CVector3D<T> m_DirB;
	public: // methods
		/** Default constructor, plane point defaults to (0,0,0), the two directions
			to (1,0,0) and (0,1,0) */
		CPlane();
		/** Constructor defining the plane.
			@param P Point on plane.
			@param DirA Defines first direction dimension of plane.
			@param DirB Defines second direction dimension. */
		CPlane(const CPoint3D<T>& P, const CVector3D<T>& DirA, const CVector3D<T>& DirB);
		/// Copy constructor
		CPlane(const CPlane<T>& Plane);
		/** Overloading of assignment operator. All attributes are copied
			to the destination. */
		CPlane<T>& operator=(const CPlane<T>& Plane);

		/** Defines a new plane in this object.
			@param P Point on plane.
			@param DirA Defines first direction dimension of plane.
			@param DirB Defines second direction dimension.
			@return False, if DirA and DirB are parallel. */
		bool Set(const CPoint3D<T>& P, const CVector3D<T>& DirA, const CVector3D<T>& DirB);
		/** Returns the first direction defining the plane.
			@return Direction defining first dimension of this plane. */
		inline const CVector3D<T>& GetDirA() const;
		/** Returns the second direction defining the plane.
			@return Direction defining second dimension of this plane. */
		inline const CVector3D<T>& GetDirB() const;
		/** Returns the point used to define the plane.
			@return Point used to define the plane. */
		inline const CPoint3D<T>& GetPoint() const;
		/** Returns a normal to the plane by calculating the cross product
			of the two directions defining the orientation of the plane.
			@return Normal to plane found as crossproduct 'm_DirA' x 'm_DirB'. */
		inline CVector3D<T> GetPlaneNormal() const;
		/** Calculates the line for intersection between this plane and plane P.
			@param P Plane to calculate intersection with.
			@param L Intersection line returned in this parameter.
			@return False, if the planes do not intersect, i.e. they are parallel */
		bool Intersect(const CPlane<T>& P, CLine3D<T> &L) const;
		/** Calculates the point for intersection between this plane and line L.
			@param L Line to calculate intersection with.
			@param Q Intersection point returned in this parameter.
			@return False, if the plane and line do not intersect, i.e. they are parallel */
		bool Intersect(const CLine3D<T>& L, CPoint3D<T>& Q) const;
		/** Returns the projection of the provided point onto this plane.
			@param P Point to be projected onto this plane.
			@return Point projected onto this plane. */
		CPoint3D<T> GetProjection(const CPoint3D<T>& P);
	/**@name streaming operators */
	//@{
	/** writes object to stream, format is:
		\verbatim
		
		CPlane Point: position DirA: direction DirB: direction
		\endverbatim
		The token "Point" is followed by the stream operator used on the
		m_P attribute which is a CPoint2D<T> type. The token "DirA" is 
		followed by the stream operator used on the m_DirA attribute which 
		is a CVector3D<T> type. The same for token "DirB".
		@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
		@version 0.6 */
//	friend std::ostream& operator<< __IPL_TF(std::ostream& s, const CPlane<T>& Plane);

	/** Reads input from given stream into this class. Stream must be in 
		correct format according to how the << ostream operator formats output.
		@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
		@version 0.6 */
//	friend std::istream& operator>> __IPL_TF(std::istream& is, CPlane<T>& Plane);
	//@}
};

template<class T>
inline const CVector3D<T>& CPlane<T>::GetDirA() const
{
	return m_DirA;
}

template<class T>
inline const CVector3D<T>& CPlane<T>::GetDirB() const
{
	return m_DirB;
}

template<class T>
inline const CPoint3D<T>& CPlane<T>::GetPoint() const
{
	return m_P;
}

template<class T>
inline CVector3D<T> CPlane<T>::GetPlaneNormal() const
{
	return m_DirA.GetCross(m_DirB);
}

template<class T>
std::ostream& operator<<(std::ostream& s, const CPlane<T>& Plane)
{
	s   << "CPlane Point: " << Plane.GetPoint() << " DirA: " << Plane.GetDirA() 
		<< " DirB: " << Plane.GetDirB();
	return s;
}

template<class T>
std::istream& operator>>(std::istream& is, CPlane<T>& Plane)
{
	string Token;
	ipl::SkipSpaceAndComments(is);
	// read 'CPlane' token
	is >> Token;
	if (Token!=string("CPlane"))
	{
		ostringstream ost;
		ost << "operator>>(istream, CPlane) Token CPlane not found"
			<< IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return is;
	}
	CPoint3D<T> P;
	CVector3D<T> DirA,DirB;
	ipl::ReadToken("Point:",is);
	is >> P;
	ipl::ReadToken("DirA:",is);
	is >> DirA;
	ipl::ReadToken("DirB:",is);
	is >> DirB;
	Plane.Set(P,DirA,DirB);
	return is;			
}

#include "plane.cpp"

/*@}*/ /* end group "containerclasses" */

} // end namespace ipl

#endif //_IPL98_PLANE_H
