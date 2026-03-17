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

#ifndef _IPL98_LINE2D_H
#define _IPL98_LINE2D_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../../cpp/std_image.h"
#include "../../../points/point2d.h"
#include "../vectors/vector2d.h"
#include <sstream>
#include "../error.h"

// Forward declarations of 'template friend' functions.
namespace ipl 
{
    template<class T>
    class CLine2D;

    template<class T>
    std::ostream& operator<< (std::ostream& s, const CLine2D<T>& Line);

    template<class T>
    std::istream& operator>> (std::istream& is, CLine2D<T>& Line);
}

namespace ipl{ // use namespace ipl

/** @ingroup containerclasses
@{ */

/** A class defining a 2 dimensional line by a 2D point and a 
	direction vector, last updated 2/26/2004.
	The template type should be a floating point, i.e. type float or double.
	To iterate a line in an image use CGraphics::Line() instead, to draw a line
	in an image see CStdImage::DrawLine().

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CLine2D ipl98/cpp/geometry/line2d.h
	@version 0.65,  by ivb@mip.sdu.dk and edr@mip.sdu.dk
	@author Ivar Balslev and René Dencker Eriksen */
template<class T>
class CLine2D
{
	public: // attributes
		/// Defines a point on line
		CPoint2D<T> m_P;
		/// defines the 2D direction of line
		CVector2D<T> m_Dir;
	public: // methods
		/// Default constructor, line point defaults to (0,0) and direction to (1,0)
		CLine2D();
		/** Constructor defining a line by given point P and direction Dir.
			Note: User must ensure the values are valid, i.e. Dir is not a zero vector.
			@param P Point on line.
			@param Dir Direction of line. */
		CLine2D(const CPoint2D<T> &P, const CVector2D<T>& Dir);
		/** Constructor defining a line by two points P1 and P2.
			The GetPoint() method will from this construction return P1 and
			the GetDir() method will return P2-P1.
			Note: User must ensure that P1 != P2.
			@param P1 First point on line.
			@param P2 Second point on line. */
		CLine2D(const CPoint2D<T>& P1, const CPoint2D<T>& P2);
		/// Copy constructor
		CLine2D(const CLine2D<T>& Line2D);
		/** Overloading of assignment operator. All attributes are copied
			to the destination. */
		CLine2D<T>& operator=(const CLine2D<T>& Line2D);
		/** Returns the direction of the line.
			@return Direction of line. */
		inline const CVector2D<T>& GetDir() const;
		/** Returns the point used to define the line.
			@return Point used to define the line. */
		inline const CPoint2D<T>& GetPoint() const;
		/** Calculates the intersection between this line and the provided line L.
			Intersection point is returned in Q.
			@return False, if the lines do not intersect, i.e. they are parallel. */
		bool Intersect(const CLine2D<T>& L, CPoint2D<T>& Q) const;
		/** Returns the two points on this line that intersect the edges of provided image.
			This is useful for drawing or iterating a CLine2D object in an image by using
			the methods CStdImage::DrawLine() and CGraphics::Line() respectively.
			@param Img Image to calculate intersection with. Takes origo and RIO into account.
			@param P0 First intersection point returned in this parameter. Defined as the 
				point in opposite direction of Dir for this line object.
			@param P1 Opposite intersection point returned in this parameter.
			@return False, if this line does not intersect with the provided image. In this
				case P0 and P1 are left unchanges.
			@version 0.1 by edr@mip.sdu.dk */
		bool Intersect(const CStdImage& Img, CPoint2D<INT16>& P0, CPoint2D<INT16>& P1) const;
		/** Defines a line by two points P1 and P2.
			The GetPoint() method will from this construction return P1 and
			the GetDir() method will return P2-P1.
			@param P1 First point on line.
			@param P2 Second point on line.
			@return False, if P1==P2. */
		bool Set(const CPoint2D<T>& P1, const CPoint2D<T>& P2);
		/** Defines a line by given point P and direction Dir.
			@param P Point on line.
			@param Dir Direction of line.
			@return False, if Dir is a zero vector. In that case nothing is changed
				in this object. */
		bool Set(const CPoint2D<T>& P, const CVector2D<T>& Dir);
		/** Calculates the distance between the specified point and this line
			@param P the specified Point.
			@return the distance between the line and the point */
		inline double Distance(const CPoint2D<T>& P) const;
	/**@name streaming operators */
	//@{
	/** Writes object to stream, format is:
		\verbatim
		
		CLine2D Point: position Dir: direction
		\endverbatim
		The token "Point" is followed by the stream operator used on the
		m_P attribute which is a CPoint2D<T> type. The token "Dir" is 
		followed by the stream operator used on the m_Dir attribute which 
		is a CVector2D<T> type. 
		Since this is a one line output, StreamIndent() is not called.
		@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
		@version 0.6 */
	friend std::ostream& operator<< __IPL_TF(std::ostream& s, const CLine2D<T>& Line);

	/** Reads input from given stream into this class. Stream must be in 
		correct format according to how the << ostream operator formats output.
		@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
		@version 0.6 */
	friend std::istream& operator>> __IPL_TF(std::istream& is, CLine2D<T>& Line);
	//@}
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

template<class T>
inline const CVector2D<T>& CLine2D<T>::GetDir() const
{
	return m_Dir;
}

template<class T>
inline double CLine2D<T>::Distance(const CPoint2D<T>& P) const
{    
	 //create the 3 lines composed by the 2 points on the line and the specified point
	 CPoint2D<T> P2=m_P+(CPoint2D<T>)m_Dir;
	 CLine2D<T> line(m_P,P2);
	 CLine2D<T> l1(P,m_P);
	 CLine2D<T> l2(P,P2);
	 //gets the distance by dividing triangle's area to the bottom length
	 //area is calculated by using 1/2*a*b*sinC
	 CVector2D<T> dir1=l1.GetDir();
	 CVector2D<T> dir2=l2.GetDir();
	 return fabs(m_P.GetDist(P)*P2.GetDist(P)*sin(dir1.GetAngle()-
				 dir2.GetAngle()))/m_P.GetDist(P2);
}

template<class T>
inline const CPoint2D<T>& CLine2D<T>::GetPoint() const
{
	return m_P;
}

#include "line2d.cpp"

/*@}*/ /* end group "containerclasses" */

} // end namespace ipl

#endif //_IPL98_LINE2D_H
