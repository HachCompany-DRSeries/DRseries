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

#ifndef _IPL98_LINE_SEGMENT2D_H
#define _IPL98_LINE_SEGMENT2D_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "line2d.h"
#include "../ipl98_general_functions.h"

namespace ipl{ // use namespace ipl

/** @ingroup containerclasses
@{ */

/** A class defining a 2 dimensional line segment by two 2D points, last updated 10/31/2003.
	The template type should be a floating point, i.e. type float or double.
	This class is derived from CLine2D and uses the base class' attribute members to store
	the line start point and the direction. This class should be used if a finite
	line is used as opposed to the infinite line defined by the CLine2D class.
	To iterate a line in an image use CGraphics::Line() instead, to draw a line
	in an image see CStdImage::DrawLine().

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CLineSegment2D ipl98/cpp/geometry/line_segment2d.h
	@version 0.2,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
template<class T>
class CLineSegment2D : protected CLine2D<T>
{
private: // attributes
	/// Defines end point of line, start point is the base class' m_P attribute
	CPoint2D<T> m_EndPoint;
	/// Length of line segment, defined as modulus of m_Dir.
	T m_Length;
public: // methods
	/** Constructor defining a finite line by two points P1 and P2.
		Note: User must ensure that P1 != P2.
		@param P1 Start point of line
		@param P2 End point of line. */
	CLineSegment2D(const CPoint2D<T>& P1, const CPoint2D<T>& P2);
	/// Copy constructor
	CLineSegment2D(const CLineSegment2D<T>& Line2D){*this=Line2D;}
	/** Overloading of assignment operator. All attributes are copied
		to the destination. */
	inline CLineSegment2D<T>& operator=(const CLineSegment2D<T>& Line2D);
	/** Defines a line by two points P1 and P2.
		The GetPoint() method will from this construction return P1 and
		the GetDir() method will return P2-P1.
		@param P1 First point on line.
		@param P2 Second point on line.
		@return False, if P1==P2. */
	bool Set(const CPoint2D<T>& P1, const CPoint2D<T>& P2);
	/** Returns the direction of the line.
		@return Direction of line. */
	inline const CVector2D<T>& GetDir() const{return CLine2D<T>::GetDir();}
	/** Returns the start point used to define the line segment.
		@return Point used to define the beginning of the line segment. */
	inline const CPoint2D<T>& GetStartPoint() const{return this->m_P;};
	/** Returns the end point used to define the line segment.
		@return Point used to define the end of the line segment. */
	inline const CPoint2D<T>& GetEndPoint() const{return m_EndPoint;};
	/** Returns the length of this line segment. Since the length is
		calculated when this class is defined/set, there are no
		time consuming calculates when calling this method. */
	inline const T GetLength() const{return m_Length;}
	
	/** Returns true, if the point P projected onto the line is within
		the limits P1 and P2 of this line.
		@param P Intersection with the projection of this point is checked.
		@version 0.2,  by edr@mip.sdu.dk */
	inline bool ProjectionOnSegment(const CPoint2D<T>& P) const;
	/** Returns true, if the point P, which is assumed to be on or very close to the
		infinite line defined by the direction of this line,
		is located on the segment of the line defined by this class.
		This method is faster than ProjectionOnSegment() and can typically
		be used if the user knows that the point P is close to the direction
		of this class' line direction and hence projection is not needed.
		@param P Point for which intersection is to be checked.
		@version 0.2,  by edr@mip.sdu.dk */
	inline bool PointOnSegment(const CPoint2D<T>& P) const;

	/** Calculates the intersection between this line segment and the provided line segment L.
		Intersection occurs only if it is within the line segments defined by
		this class and the provided line L.
		@param L Line to calculate intersection with.
		@param Q Intersection point returned in this parameter.
		@return False, if the lines do not intersect, i.e. they are parallel,
			or if intersection is outside line segments.
		@version 0.2,  by edr@mip.sdu.dk */
	inline bool Intersect(const CLineSegment2D<T>& L, CPoint2D<T>& Q) const;

	/** Calculates the intersection between this line segment and the provided infinite line L.
		Intersection occurs only if it is within the line segments defined by
		this class.
		@param L Inifinite line to calculate intersection with.
		@param Q Intersection point returned in this parameter.
		@return False, if the lines do not intersect, i.e. they are parallel,
			or if intersection is outside the line segment defined by this class.
		@version 0.2,  by edr@mip.sdu.dk */
	inline bool Intersect(const CLine2D<T>& L, CPoint2D<T>& Q) const;
	/** Calculates the distance between the specified point and the line.
		If the projection of point P is outside the line segment, then
		the distance is calculated as the distance between P and the closest
		endpoint. Note: This method is not speed optimized, it can probably
		be done more efficiently.
		@param P the specified Point.
		@return the distance between the line and the point */
	inline double Distance(const CPoint2D<T>& P) const;

	/**@name streaming operators */
	//@{
	/** writes object to stream, format is:
		\verbatim
		
		CLineSegment2D StartPoint: P1 EndPoint: P2 Dir: direction
		\endverbatim
		The token "Point" is followed by the stream operator used on the
		m_P attribute which is a CPoint2D<T> type. The token "Dir" is 
		followed by the stream operator used on the m_Dir attribute which 
		is a CVector2D<T> type. 
		Since this is a one line output, StreamIndent() is not called.
		@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
		@version 0.2 */
	friend std::ostream& operator<< __IPL_TF(std::ostream& s, const CLineSegment2D<T>& Line);

	/** Reads input from given stream into this class. Stream must be in 
		correct format according to how the << ostream operator formats output.
		@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
		@version 0.2 */
	friend std::istream& operator>> __IPL_TF(std::istream& is, CLineSegment2D<T>& Line);
	//@}

};

template<class T>
CLineSegment2D<T>::CLineSegment2D(const CPoint2D<T>& P1, const CPoint2D<T>& P2) : CLine2D<T>(P1,P2)
{
	m_EndPoint=P2;
	m_Length=(T)this->m_Dir.GetModulus();
}

template<class T>
inline CLineSegment2D<T>& CLineSegment2D<T>::operator=(const CLineSegment2D<T>& Line2D)
{
	CLine2D<T>::operator =(Line2D);
	m_EndPoint=Line2D.m_EndPoint;
	m_Length=Line2D.m_Length;
	return *this;
}

template<class T>
bool CLineSegment2D<T>::Set(const CPoint2D<T>& P1, const CPoint2D<T>& P2)
{
	if (CLine2D<T>::Set(P1,P2)==false) // Call base class' set method
		return false;
	m_EndPoint=P2;
	m_Length=(T)this->m_Dir.GetModulus();
	return true;
}

template<class T>
inline bool CLineSegment2D<T>::ProjectionOnSegment(const CPoint2D<T>& P) const
{
	const float ANGLE_ZERO_ADJUST=0.001f; // if angle < ANGLE_ZERO_ADJUST it is considered 0
	CVector2D<T> V(P-this->m_P);
	CVector2D<T> VProj;
	VProj = V.GetProjection(this->m_Dir);
	//float Angle=this->m_Dir.GetAngle(VProj);
	if ((fabs(this->m_Dir.GetAngle(VProj))<ANGLE_ZERO_ADJUST) && (m_Length>=V.GetModulus()))
		return true;
	else
		return false;
}

template<class T>
inline bool CLineSegment2D<T>::PointOnSegment(const CPoint2D<T>& P) const
{
	CVector2D<T> V(P-this->m_P);
	if ((fabs(this->m_Dir.GetAngle(V))>(PI/(T)2)) && (m_Length>=V.GetModulus()))
		return true;
	else
		return false;
}

template<class T>
inline bool CLineSegment2D<T>::Intersect(const CLineSegment2D<T>& L, CPoint2D<T>& Q) const
{
	if (CLine2D<T>::Intersect(L,Q)==true)
		return (PointOnSegment(Q) && L.PointOnSegment(Q));
	else
		return false;
}

template<class T>
inline bool CLineSegment2D<T>::Intersect(const CLine2D<T>& L, CPoint2D<T>& Q) const
{
	if (CLine2D<T>::Intersect(L,Q)==true)
		return (PointOnSegment(Q));
	else
		return false;
}

template<class T>
inline double CLineSegment2D<T>::Distance(const CPoint2D<T>& P) const
{
	if (ProjectionOnSegment(P)==true)
		return CLine2D<T>::Distance(P);
	else
	{
		// simply use the closes endpoint
		return ipl::Max(P.GetDist(this->m_P),P.GetDist(m_EndPoint));
	}
}

template<class T>
std::ostream& operator<<(std::ostream& s, const CLineSegment2D<T>& Line)
{
	s << "CLineSegment2D StartPoint: " << Line.GetStartPoint() << " EndPoint: " 
	  << Line.GetEndPoint() << " Dir: " << Line.GetDir();
	return s;
}

template<class T>
std::istream& operator>>(std::istream& is, CLineSegment2D<T>& Line)
{
	string Token;
	ipl::SkipSpaceAndComments(is);
	// read 'CLineSegment2D' token
	is >> Token;
	if (Token!=string("CLineSegment2D"))
	{
		ostringstream ost;
		ost << "operator>>(istream, CLineSegment2D) Token CLineSegment2D not found"
			<< IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return is;
	}
	ipl::SkipSpaceAndComments(is);
	// skip 'StartPoint:' token
	is.ignore(std::numeric_limits<int>::max(),std::char_traits<char>::to_int_type(':'));
	ipl::SkipSpaceAndComments(is);
	is >> Line.m_P;
	ipl::SkipSpaceAndComments(is);
	// skip 'EndPoint:' token
	is.ignore(std::numeric_limits<int>::max(),std::char_traits<char>::to_int_type(':'));
	ipl::SkipSpaceAndComments(is);
	is >> Line.m_EndPoint;
	ipl::SkipSpaceAndComments(is);
	// skip 'Dir:' token
	is.ignore(std::numeric_limits<int>::max(),std::char_traits<char>::to_int_type(':'));
	ipl::SkipSpaceAndComments(is);
	is >> Line.m_Dir;
	// Finally calculate the length to be stored in Line
	Line.m_Length = Line.m_Dir.GetModulus();
	return is;			
}


/*@}*/ /* end group "containerclasses" */

} // end namespace ipl

#endif //_IPL98_LINE_SEGMENT2D_H
