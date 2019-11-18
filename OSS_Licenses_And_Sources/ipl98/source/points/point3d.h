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

#ifndef _POINT3D_H
#define _POINT3D_H

#include <iostream>
#include <limits>
#include <ostream>

/** @ingroup containerclasses
@{ */

/** CPoint3D template class with 3-dimensional points,
	last updated 8/13/2005.
	Use this class to manipulate 3-dimensional points.
	Most operators are overloaded and each member in a
	point (x,y,z) can be reached and manipulated through
	public methods. The position (x,y,z) can be streamed with
	the "<<"-operator and read by the ">>"-operator. A
	corresponding 2-dimensional point class is also available.
	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CPoint3D points/point3d.h
	@see CPoint2D
	@version 1.49,  by edr@mip.sdu.dk 
	@author René Dencker Eriksen */
template<class T>
class CPoint3D
{
	protected: // attributes
		/// The x-value of this 3D point
		T m_x;
		/// The y-value of this 3D point
		T m_y;
		/// The z-value of this 3D point
		T m_z;
	public: // methods
		 /// default constructor, init to zero
		CPoint3D();
		/// constr. with init. of point
		CPoint3D(T x,T y,T z);
		/// copy constructor with template conversion
		template<class POLY> inline CPoint3D(const CPoint3D<POLY>& P3D){m_x=(T)(P3D.GetX());m_y=(T)(P3D.GetY());m_z=(T)(P3D.GetZ());}
		 /// copy constructor
		CPoint3D(const CPoint3D<T>& P3D);
		/// default destructor
		~CPoint3D();

		/// cast to POLY in mixed expressions
		template<class POLY> operator CPoint3D<POLY>(){return CPoint3D<POLY>(m_x,m_y,m_z);}

		/**@name Overloading assignment operators: */
		//@{ 
		/// overloading = operator with template conversion
		template<class POLY> inline CPoint3D<T>& operator=(const CPoint3D<POLY>& P3D){m_x=P3D.GetX();m_y=P3D.GetY();m_z=P3D.GetZ();return *this;};
		/// overloading +=
		template<class POLY> inline CPoint3D<T>& operator+=(const CPoint3D<POLY>& P3D){m_x+=P3D.GetX();m_y+=P3D.GetY();m_z+=P3D.GetZ();return *this;};
		/// overloading -= operator with template conversion
		template<class POLY> inline CPoint3D<T>& operator-=(const CPoint3D<POLY>& P3D){m_x-=P3D.GetX();m_y-=P3D.GetY();m_z-=P3D.GetZ();return *this;};
		/** overloading *= operator with template conversion
			 @param Factor Multiply each dimension with this value */
		template<class POLY> inline CPoint3D<T>& operator*=(const POLY& Factor){m_x*=Factor;m_y*=Factor;m_z*=Factor;return *this;}
		/** overloading /= operator with template conversion
			 @param Factor Divide each dimension with this value */
		template<class POLY> inline CPoint3D<T>& operator/=(const POLY& Factor){m_x/=Factor;m_y/=Factor;m_z/=Factor;return *this;}
		//@}
		/**@name Overloading comparison operators: */
		//@{
		/// overloading == operator with template conversion
		template<class POLY> inline bool operator==(const CPoint3D<POLY>& P3D) const{return ((m_x==P3D.GetX()) && (m_y==P3D.GetY()) && (m_z==P3D.GetZ()));}
		/// overloading != operator with template conversion
		template<class POLY> inline bool operator!=(const CPoint3D<POLY>& P3D) const{return ((m_x!=P3D.GetX()) || (m_y!=P3D.GetY()) || (m_z!=P3D.GetZ()));}
		/// overloading < operator with template conversion. First comparison on x, if equal comparison on y is performed.
		template<class POLY> inline bool operator<(const CPoint3D<POLY>& P3D) const{if (m_x==P3D.GetX()){if (m_y==P3D.GetY()){return m_z<P3D.GetZ();}else{return (m_y<P3D.GetY());}}else{return (m_x<P3D.GetX());}}
		/// overloading <= operator with template conversion. First comparison on x, if equal comparison on y is performed.
		template<class POLY> inline bool operator<=(const CPoint3D<POLY>& P3D) const{if (m_x==P3D.GetX()){if (m_y==P3D.GetY()){return m_z<=P3D.GetZ();}else{return (m_y<P3D.GetY());}}else{return (m_x<P3D.GetX());}}
		/// overloading < operator with template conversion. First comparison on x, if equal comparison on y is performed.
		template<class POLY> inline bool operator>(const CPoint3D<POLY>& P3D) const{if (m_x==P3D.GetX()){if (m_y==P3D.GetY()){return m_z>P3D.GetZ();}else{return (m_y>P3D.GetY());}}else{return (m_x>P3D.GetX());}}
		/// overloading >= operator with template conversion. First comparison on x, if equal comparison on y is performed.
		template<class POLY> inline bool operator>=(const CPoint3D<POLY>& P3D) const{if (m_x==P3D.GetX()){if (m_y==P3D.GetY()){return m_z>=P3D.GetZ();}else{return (m_y>P3D.GetY());}}else{return (m_x>P3D.GetX());}}
		//@}
		/**@name Overloading arithmetic operators: */
		//@{
		/// overloading + operator with template conversion
		template<class POLY> inline CPoint3D<T> operator+(const CPoint3D<POLY>& P3D) const {return CPoint3D<T>(m_x+P3D.GetX(),m_y+P3D.GetY(),m_z+P3D.GetZ());}
		/// overloading - operator with template conversion
		template<class POLY> inline CPoint3D<T> operator-(const CPoint3D<POLY>& P3D) const {return CPoint3D<T>(m_x-P3D.GetX(),m_y-P3D.GetY(),m_z-P3D.GetZ());}
		/** overloading * operator with template conversion
			 @param Factor Multiply each dimension with this value */
		template<class POLY> inline CPoint3D<T> operator*(const POLY& Factor) const {return CPoint3D<T>(m_x*Factor,m_y*Factor,m_z*Factor);}
		/** overloading / operator with template conversion
			 @param Factor Divide each dimension with this value */
		template<class POLY> inline CPoint3D<T> operator/(const POLY& Factor) const {return CPoint3D<T>(m_x/Factor,m_y/Factor,m_z/Factor);}
		//@}
		/** a friend method overloading the * operator, makes it possible to write f*P3D,
			where f is a factor of type T and P3D is of type CPoint3D<POLY>. */
		friend CPoint3D<T> operator*(const T& Factor,const CPoint3D& P3D){return CPoint3D<T>(P3D.m_x*Factor, P3D.m_y*Factor, P3D.m_z*Factor);}

		// other methods
		/// add "x", "y" and "z" to current position
		inline void Add(const T x, const T y, const T z);
		/// add "x" to current x-value
		inline void AddX(const T x);
		/// add "y" to current y-value
		inline void AddY(const T y);
		/// add "z" to current z-value
		inline void AddZ(const T z);
		/// sub "x", "y" and "z" from current position
		inline void Sub(const T x, const T y, const T z);
		/// sub "x" from current x-value
		inline void SubX(const T x);
		/// sub "y" from current y-value
		inline void SubY(const T y);
		/// sub "z" from current z-value
		inline void SubZ(const T z);
		/// returns the x, y and z values in given parameters
		inline void Get(T& x, T& y, T& z) const;
		/// return x-value
		inline T GetX() const;
		/// return y-value
		inline T GetY() const;
		/// return z-value
		inline T GetZ() const;
		/// set x-, y- and z-value
		inline void Set(const T x, const T y, const T z);
		/// set x-value
		inline void SetX(const T x);
		/// set y-value
		inline void SetY(const T y);
		/// set z-value
		inline void SetZ(const T z);
		/** Returns true, if point is zero, that is x, y, and z values are all zero.
			@return True, if point is zero.
			@see ZeroAdjust */
		inline bool IsZero() const;
		/** Returns true, if point is smaller than supplied value, that is both x, y, and 
			z values are less than supplied value. A method without parameters checking
			if point is exactly zero is available.
			@param Zero Value to compare both x, y, and z components with, must be greater
				than or equal to zero.
			@return True, if point is smaller than Zero parameter.
			@see ZeroAdjust */
		inline bool IsZero(T Zero) const;
		/** Sets values in vector to zero if they are smaller than supplied value.
			@return A reference to this object with updated values.
			@see IsZero */
		inline CPoint3D<T>& ZeroAdjust(const T& Zero);
		/** Returns a rounded value of this point. The floating type should be float
			or double when calling this method. */
		inline CPoint3D<T> GetRounded() const;
		/** return Euclidian distance (length) between two points.
			@param P3D The point to calculate distance to, defaults to (0,0,0).
			@return Euclidian distance between this object and the given 3D point. */
		inline double GetDist(const CPoint3D<T>& P3D=CPoint3D<T>(0,0,0)) const;
		/** Rotates this point RadA radians anti clock wise (looking at the y-z plane from a point 
			on the x-axis with x>0) about x-axis, in a right hand coordinate system, then rotates 
			RadB radians anti clock wise (looking at the new z-x plane from a point on the new y-axis
			with y>0) about the new y-axis. If RadA or RadB  is n*2PI+r with integer n>=1, a rotation
			of r radians is performed in the respective rotation.
			@param RadA Radians to rotate point anti clock wise (looking at the y-z plane from a point 
					on the x-axis with x>0) about x-axis, in a right hand coordinate system.
			@param RadB Radians to rotate point anti clock wise (looking at the new z-x plane from a point 
					on the new y-axis with y>0) about new y-axis, in a right hand coordinate system.
			@version 0.3 */ 
		inline void Rotate(double RadA, double RadB);

		// info methods
		/// prints value of point to stdout
		void Print() const;
		/**@name streaming operators */
		//@{
		/** Writes point to stream as '(x,y,z)'.
			Since this is a one line output, StreamIndent() is not called. */
		friend std::ostream& operator<<(std::ostream& s,const CPoint3D<T>& P3D){return s << "(" << P3D.m_x << "," << P3D.m_y << "," << P3D.m_z << ")";}

		/** Reads input from given stream into this class. Stream must be in correct format
			according to how the << ostream operator formats output. Both paranteses are removed
			from stream.
			Note: Beacuause of a bug in the Borland C++ 4.0 compiler, the definition of this
			method is placed in the header file, otherwise the Borland compiler can't link.
			@version 0.2 */
		friend std::istream& operator>>(std::istream& is, CPoint3D<T>& P3D)
		{
			SkipSpaceAndComments(is);
			is.get(); // eat '('
			SkipSpaceAndComments(is);
			is >> P3D.m_x;
			SkipSpaceAndComments(is);
			is.get(); // eat ','
			SkipSpaceAndComments(is);
			is >> P3D.m_y;
			SkipSpaceAndComments(is);
			is.get(); // eat ','
			SkipSpaceAndComments(is);
			is >> P3D.m_z;
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
		std::istream& operator<<(std::istream& s);
		//@}
	protected: // methods
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

// other methods
template <class T>
inline void CPoint3D<T>::Add(const T x, const T y, const T z)
{
	m_x +=x;
	m_y +=y;
	m_z +=z;
}

template <class T>
inline void CPoint3D<T>::AddX(const T x)
{
	// add "x" to current x-value
	m_x += x;
}

template <class T>
inline void CPoint3D<T>::AddY(const T y)
{
	// add "y" to current y-value
	m_y += y;
}

template <class T>
inline void CPoint3D<T>::AddZ(const T z)
{
	// add "z" to current z-value
	m_z += z;
}

template <class T>
inline void CPoint3D<T>::Sub(const T x, const T y, const T z)
{
	m_x -= x;
	m_y -= y;
	m_z -= z;
}

template <class T>
inline void CPoint3D<T>::SubX(const T x)
{
	// sub "x" from current x-value
	m_x -= x;
}

template <class T>
inline void CPoint3D<T>::SubY(const T y)
{
	// sub "y" from current y-value
	m_y -= y;
}

template <class T>
inline void CPoint3D<T>::SubZ(const T z)
{
	// sub "z" from current z-value
	m_z -= z;
}

template <class T>
inline void CPoint3D<T>::Get(T& x, T& y, T& z) const
{
	x=m_x;
	y=m_y;
	z=m_z;
}

template <class T>
inline T CPoint3D<T>::GetX() const
{
	// return x-value
	return m_x;
}

template <class T>
inline T CPoint3D<T>::GetY() const
{
	// return y-value
	return m_y;
}

template <class T>
inline T CPoint3D<T>::GetZ() const
{
	// return z-value
	return m_z;
}

template <class T>
inline void CPoint3D<T>::Set(const T x, const T y, const T z)
{
	m_x=x;
	m_y=y;
	m_z=z;
}

template <class T>
inline void CPoint3D<T>::SetX(const T x)
{
	// set x-value
	m_x=x;
}

template <class T>
inline void CPoint3D<T>::SetY(const T y)
{
	// set y-value
	m_y=y;
}

template <class T>
inline void CPoint3D<T>::SetZ(const T z)
{
	// set z-value
	m_z=z;
}

template <class T>
inline bool CPoint3D<T>::IsZero() const
{
	return ((m_x==T(0)) && (m_y==T(0)) && (m_z==T(0)));
}

template <class T>
inline bool CPoint3D<T>::IsZero(T Zero) const
{
	return ((m_x<Zero) && (m_x>-Zero) && (m_y<Zero) && (m_y>-Zero) && (m_z<Zero) && (m_z>-Zero));
}

template <class T>
inline CPoint3D<T>& CPoint3D<T>::ZeroAdjust(const T& Zero)
{
	if (m_x<Zero)
		m_x=T(0);
	if (m_y<Zero)
		m_y=T(0);
	if (m_z<Zero)
		m_z=T(0);
	return *this;
}

template <class T>
inline CPoint3D<T> CPoint3D<T>::GetRounded() const
{
	return CPoint3D<T>((T)ipl::Round(m_x),(T)ipl::Round(m_y),(T)ipl::Round(m_z));
}

template <class T>
inline double CPoint3D<T>::GetDist(const CPoint3D<T>& P3D) const
{
	// return distance between two points
	T distX= m_x>P3D.GetX() ? m_x-P3D.GetX() : P3D.GetX()-m_x;
	T distY= m_y>P3D.GetY() ? m_y-P3D.GetY() : P3D.GetY()-m_y;
	T distZ= m_z>P3D.GetZ() ? m_z-P3D.GetZ() : P3D.GetZ()-m_z;
	return sqrt((double)(distX*distX+distY*distY+distZ*distZ));
}

template <class T>
inline void CPoint3D<T>::Rotate(double RadA, double RadB)
{
	double cosa=cos(RadA);
	double sina=sin(RadA);
	double cosb=cos(RadB);
	double sinb=sin(RadB);
	double x = m_x*cosb + m_z*sinb;
	double y = m_x*sinb*sina + m_y*cosa - m_z*cosb*sina;
	double z = -m_x*sinb*cosa + m_y*sina + m_z*cosb*cosa;
	m_x=(T)x;
	m_y=(T)y;
	m_z=(T)z;
}

#include "point3d.cpp"

/*@}*/ /* end group "containerclasses" */

#endif //_POINT3D_H
