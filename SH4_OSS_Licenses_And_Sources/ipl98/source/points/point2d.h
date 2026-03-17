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

#ifndef _POINT2D_H
#define _POINT2D_H

#include <iostream>
#include <limits>
#include <ostream>
#include <cmath>
#include "../ipl98/cpp/ipl98_general_functions.h"

/** @ingroup containerclasses
@{ */

/** CPoint2D template class with 2-dimensional points,
	last updated 8/13/2005.
	Use this class to manipulate 2-dimensional points.
	Most operators are overloaded and each member in a
	point (x,y) can be reached and manipulated through
	public methods.  The position (x,y) can be streamed with
	the "<<"-operator and read by the ">>"-operator. A
	corresponding 3-dimensional point class is also available.
	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CPoint2D points/point2d.h
	@see CPoint3D
	@version 1.49, by edr@mip.sdu.dk
	@author René Dencker Eriksen */
template<class T>
class CPoint2D
{
	protected: // attributes
		/// The x-value of this 2D point
		T m_x;
		/// The y-value of this 2D point
		T m_y;
	public: // methods
		 /// default constructor, init to zero
		CPoint2D();
		/// constr. with init. of point
		CPoint2D(T x,T y);
		/// copy constructor with template conversion
		template<class POLY> inline CPoint2D(const CPoint2D<POLY>& P2D){m_x=(T)P2D.GetX();m_y=(T)P2D.GetY();}
		/// copy constructor
		CPoint2D(const CPoint2D<T>& P2D);
		/// default destructor
		virtual ~CPoint2D();

		/// cast to POLY in mixed expressions
		template<class POLY> operator CPoint2D<POLY>(){return CPoint2D<POLY>(m_x,m_y);}

		/**@name Overloading assignment operators: */
		//@{
		/// overloading = operator with template conversion
		template<class POLY> inline CPoint2D<T>& operator=(const CPoint2D<POLY>& P2D){m_x=(T)P2D.GetX();m_y=(T)P2D.GetY();return *this;};
		/// overloading += operator with template conversion
		template<class POLY> inline CPoint2D<T>& operator+=(const CPoint2D<POLY>& P2D){m_x+=(T)P2D.GetX();m_y+=(T)P2D.GetY();return *this;};
		/// overloading -= operator with template conversion
		template<class POLY> inline CPoint2D<T>& operator-=(const CPoint2D<POLY>& P2D){m_x-=(T)P2D.GetX();m_y-=(T)P2D.GetY();return *this;};
		/** overloading *= operator with template conversion
			 @param Factor Multiply each dimension with this value */
		template<class POLY> inline CPoint2D<T>& operator*=(const POLY& Factor){m_x*=Factor;m_y*=Factor;return *this;}
		/** overloading /= operator with template conversion
			 @param Factor Divide each dimension with this value */
		template<class POLY> inline CPoint2D<T>& operator/=(const POLY& Factor){m_x/=Factor;m_y/=Factor;return *this;}
		//@}
		/**@name Overloading comparison operators: */
		//@{
		/// overloading == operator with template conversion
		template<class POLY> inline bool operator==(const CPoint2D<POLY>& P2D) const{return ((m_x==(T)P2D.GetX()) && (m_y==(T)P2D.GetY()));}
		/// overloading != operator with template conversion
		template<class POLY> inline bool operator!=(const CPoint2D<POLY>& P2D) const{return ((m_x!=(T)P2D.GetX()) || (m_y!=(T)P2D.GetY()));}
		/// overloading < operator with template conversion. First comparison on x, if equal comparison on y is performed.
		template<class POLY> inline bool operator<(const CPoint2D<POLY>& P2D) const{if (m_x==(T)P2D.GetX()){return (m_y<(T)P2D.GetY());}else{return (m_x<(T)P2D.GetX());}}
		/// overloading <= operator with template conversion. First comparison on x, if equal comparison on y is performed.
		template<class POLY> inline bool operator<=(const CPoint2D<POLY>& P2D) const{if (m_x==P2D.GetX()){return (m_y<=(T)P2D.GetY());}else{return (m_x<(T)P2D.GetX());}}
		/// overloading < operator with template conversion. First comparison on x, if equal comparison on y is performed.
		template<class POLY> inline bool operator>(const CPoint2D<POLY>& P2D) const{if (m_x==P2D.GetX()){return (m_y>(T)P2D.GetY());}else{return (m_x>(T)P2D.GetX());}}
		/// overloading >= operator with template conversion. First comparison on x, if equal comparison on y is performed.
		template<class POLY> inline bool operator>=(const CPoint2D<POLY>& P2D) const{if (m_x==(T)P2D.GetX()){return (m_y>=(T)P2D.GetY());}else{return (m_x>(T)P2D.GetX());}}
		//@}
		/**@name Overloading arithmetic operators: */
		//@{
		/// overloading + operator with template conversion
		template<class POLY> inline CPoint2D<T> operator+(const CPoint2D<POLY>& P2D) const {return CPoint2D<T>(m_x+(T)P2D.GetX(),m_y+(T)P2D.GetY());}
		/// overloading - operator with template conversion
		template<class POLY> inline CPoint2D<T> operator-(const CPoint2D<POLY>& P2D) const {return CPoint2D<T>(m_x-(T)P2D.GetX(),m_y-(T)P2D.GetY());}
		/** overloading * operator with template conversion
			 @param Factor Multiply each dimension with this value */
		template<class POLY> inline CPoint2D<T> operator*(const POLY& Factor) const {return CPoint2D<T>(m_x*Factor,m_y*Factor);}
		/** overloading / operator with template conversion
			 @param Factor Divide each dimension with this value */
		template<class POLY> inline CPoint2D<T> operator/(const POLY& Factor) const {return CPoint2D<T>((T)(m_x/Factor),(T)(m_y/Factor));}
		//@}

		/** a friend method overloading the * operator, makes it possible to write f*P2D,
			where f is a factor of type T and P2D is of type CPoint2D<POLY>. */
		friend CPoint2D<T> operator*(const T& Factor,const CPoint2D& P2D){return CPoint2D<T>(P2D.m_x*Factor, P2D.m_y*Factor);}

		// other methods
		/// add "x" and "y" to current position
		inline void Add(const T x, const T y);
		/// add "x" to current x-value
		inline void AddX(const T x);
		/// add "y" to current y-value
		inline void AddY(const T y);
		/// sub "x" and "y" from current position
		inline void Sub(const T x, const T y);
		/// sub "x" from current x-value
		inline void SubX(const T x);
		/// sub "y" from current y-value
		inline void SubY(const T y);
		/// returns the x and y values in given parameters
		inline void Get(T& x, T& y) const;
		/// return x-value
		inline T GetX() const;
		/// return y-value
		inline T GetY() const;
		/// set x- and y-value
		inline void Set(const T x, const T y);
		/// set x-value
		inline void SetX(const T x);
		/// set y-value
		inline void SetY(const T y);
		/** Returns true, if point is zero, that is both x and y values are zero.
			@return True, if point is zero.
			@see ZeroAdjust */
		inline bool IsZero() const;
		/** Returns true, if point is smaller than supplied value, that is both x and 
			y values are less than supplied value. A method without parameters checking
			if point is exactly zero is available.
			@param Zero Value to compare both x and y components with, must be greater
				than or equal to zero.
			@return True, if point is smaller than Zero parameter.
			@see ZeroAdjust */
		inline bool IsZero(T Zero) const;
		/** Sets values in point to zero if they are smaller than supplied value.
			@return A reference to this object with updated values.
			@see IsZero */
		inline CPoint2D<T>& ZeroAdjust(const T& Zero);
		/** Returns a rounded value of this point. The floating type should be float
			or double when calling this method. */
		inline CPoint2D<T> GetRounded() const;
		/** return Euclidian distance (length) between two points.
			@param P2D The point to calculate distance to, defaults to (0,0).
			@return Euclidian distance between this object and the given 2D point. */
		inline double GetDist(const CPoint2D<T>& P2D=CPoint2D<T>(0,0)) const;
		/** Rotates this point Rad radians counter clock wise about origo in an image
			coordinate system, that is with x axis increasing to the right and
			y axis increasing downwards. If Rad=n*2PI+r with integer n>=1, a rotation
			of r radians is performed. If a rotation of PI is needed, the faster
			method RotatePi can be called.
			@param Rad Radians to rotate point.
			@version 0.3
			@see RotatePi RotatePiHalf */ 
		inline void Rotate(double Rad);
		/** Rotates this point PI radians counter clock wise about origo. It is a fast version of
			Rotate since it only requires a sign change of the (x,y) value.
			@version 0.3
			@see Rotate RotatePiHalf */
		inline void RotatePi();
		/** Rotates this point PI/2 radians counter clock wise about origo. It is a fast version of
			Rotate since it only requires (x,y) -> (-y,x).
			@version 0.1
			@see Rotate RotatePi */
		inline void RotatePiHalf();
		// info methods
		/// prints value of point to stdout
		void Print() const;
		/**@name streaming operators */
		//@{
		/** Writes point to stream as '(x,y)'.
			Since this is a one line output, StreamIndent() is not called. */
		friend std::ostream& operator<<(std::ostream& s,const CPoint2D<T>& P2D){return s << "(" << P2D.m_x << "," << P2D.m_y << ")";}

		/** Reads input from given stream into this class. Stream must be in correct format
			according to how the << ostream operator formats output. Both paranteses are removed
			from stream.
			Note: Because of a bug in the Borland C++ 4.0 compiler, the definition of this
			method is placed in the header file, otherwise the Borland compiler can't link.
			@version 0.2 */
		friend std::istream& operator>>(std::istream& is, CPoint2D<T>& P2D)
		{
			SkipSpaceAndComments(is);
			is.get(); // eat '('
			SkipSpaceAndComments(is);
			is >> P2D.m_x;
			SkipSpaceAndComments(is);
			is.get(); // eat ','
			SkipSpaceAndComments(is);
			is >> P2D.m_y;
			SkipSpaceAndComments(is);
			is.get(); // eat ')'
			return is;
		}

		/** Note: This method will be removed in a future version, this is not the correct
			way of using an istream.
			Reads input from given stream, stream must be in correct format according
			to how the << ostream operator formats output. Both paranteses are removed
			from stream. Be careful when using this method, it has not been tested
			with all template types and may change in future versions.
			Note: Comment lines and whitespace can be put anywhere within the stream.
			@version 0.1 */
		virtual inline std::istream& operator<<(std::istream& s);
		//@}
	public: // methods
		/** used for loading. Skips all whitespace, newline, tab characters and
			comment lines, i.e. rest of line from a '#' character. */
		static inline void SkipSpaceAndComments(std::istream& s)
		{
			while(isspace(s.peek()))
				s.get();
			while (s.peek()=='#')
			{
				s.ignore(std::numeric_limits<int>::max(),'\n');
				while (isspace(s.peek()))
					s.get();
			}
		}
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

/*template <class T>
std::istream& operator>>(std::istream& is, CPoint2D<T>& P2D)
{
	P2D.SkipSpaceAndComments(is);
	is.get(); // eat '('
	P2D.SkipSpaceAndComments(is);
	is >> P2D.m_x;
	P2D.SkipSpaceAndComments(is);
	is.get(); // eat ','
	P2D.SkipSpaceAndComments(is);
	is >> P2D.m_y;
	P2D.SkipSpaceAndComments(is);
	is.get(); // eat ')'
	return is;
}*/

//template <class T> std::ostream& operator<<(std::ostream& s,const CPoint2D<T>& P2D){return s << "(" << P2D.m_x << "," << P2D.m_y << ")";}

// other methods

template <class T>
inline void CPoint2D<T>::Add(const T x, const T y)
{
	m_x +=x;
	m_y +=y;
}

template <class T>
inline void CPoint2D<T>::AddX(const T x)
{
	// add "x" to current x-value
	m_x += x;
}

template <class T>
inline void CPoint2D<T>::AddY(const T y)
{
	// add "y" to current y-value
	m_y += y;
}

template <class T>
inline void CPoint2D<T>::Sub(const T x, const T y)
{
	m_x -= x;
	m_y -= y;
}

template <class T>
inline void CPoint2D<T>::SubX(const T x)
{
	// sub "x" from current x-value
	m_x -= x;
}

template <class T>
inline void CPoint2D<T>::SubY(const T y)
{
	// sub "y" from current y-value
	m_y -= y;
}

template <class T>
inline void CPoint2D<T>::Get(T& x, T& y) const
{
	x=m_x;
	y=m_y;
}

template <class T>
inline T CPoint2D<T>::GetX() const
{
	// return x-value
	return m_x;
}

template <class T>
inline T CPoint2D<T>::GetY() const
{
	// return y-value
	return m_y;
}

template <class T>
inline void CPoint2D<T>::Set(const T x, const T y)
{
	m_x=x;
	m_y=y;
}

template <class T>
inline void CPoint2D<T>::SetX(const T x)
{
	// set x-value
	m_x=x;
}

template <class T>
inline void CPoint2D<T>::SetY(const T y)
{
	// set y-value
	m_y=y;
}

template <class T>
inline bool CPoint2D<T>::IsZero() const
{
	return ((m_x==T(0)) && (m_y==T(0)));
}

template <class T>
inline bool CPoint2D<T>::IsZero(T Zero) const
{
 	return ((m_x<Zero) && (m_x>-Zero) && (m_y<Zero) && (m_y>-Zero));
}

template <class T>
inline CPoint2D<T>& CPoint2D<T>::ZeroAdjust(const T& Zero)
{
	if (m_x<Zero)
		m_x=T(0);
	if (m_y<Zero)
		m_y=T(0);
	return *this;
}

template <class T>
inline CPoint2D<T> CPoint2D<T>::GetRounded() const
{
	return CPoint2D<T>((T)ipl::Round(m_x),(T)ipl::Round(m_y));
}

template <class T>
inline double CPoint2D<T>::GetDist(const CPoint2D<T>& P2D) const
{
	// return distance between two points
	T distX= m_x>P2D.GetX() ? m_x-P2D.GetX() : P2D.GetX()-m_x;
	T distY= m_y>P2D.GetY() ? m_y-P2D.GetY() : P2D.GetY()-m_y;
	return sqrt((double)(distX*distX+distY*distY));
}

template <class T>
inline void CPoint2D<T>::Rotate(double Rad)
{
	double cosa=cos(Rad);
	double sina=sin(Rad);
//	double x = m_x*cosa + m_y*sina;
//	m_y = m_x*sina - m_y*cosa;
//	m_x=x;
	double x = m_x*cosa + m_y*sina;
	double y = m_y*cosa - m_x*sina;
	m_x=(T)x;
	m_y=(T)y;
}

template <class T>
inline void CPoint2D<T>::RotatePi()
{
	m_x=-m_x;
	m_y=-m_y;
}

template <class T>
inline void CPoint2D<T>::RotatePiHalf()
{
	T x=m_x;
	m_x=-m_y;
	m_y=x;
}

template <class T>
inline std::istream& CPoint2D<T>::operator<<(std::istream& s)
{
	while(isspace(s.peek())){s.get();}while (s.peek()=='#'){s.ignore(500,'\n');while (isspace(s.peek())){s.get();}} 
	s.get(); // eat '('
	while(isspace(s.peek())){s.get();}while (s.peek()=='#'){s.ignore(500,'\n');while (isspace(s.peek())){s.get();}} 
	s >> m_x;
	while(isspace(s.peek())){s.get();}while (s.peek()=='#'){s.ignore(500,'\n');while (isspace(s.peek())){s.get();}} 
	s.get(); // eat ','
	while(isspace(s.peek())){s.get();}while (s.peek()=='#'){s.ignore(500,'\n');while (isspace(s.peek())){s.get();}} 
	s >> m_y;
	while(isspace(s.peek())){s.get();}while (s.peek()=='#'){s.ignore(500,'\n');while (isspace(s.peek())){s.get();}} 
	s.get(); // eat ')'
	return s;
}

#include "point2d.cpp"

/*@}*/ /* end group "containerclasses" */

#endif //_POINT2D_H


