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

#ifndef _IPL98_VECTOR2D_H
#define _IPL98_VECTOR2D_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../../../points/point2d.h"
#include <iostream>

namespace ipl{ // use namespace if C++

/** @ingroup containerclasses
@{ */

/** CVector2D template class with 2-dimensional vectors,
	last updated 11/4/2003.
	This class is inherited from the CPoint2D class, this
	version adds some new vector methods such as calculations
	of angle, projections, dot product etc.
	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CVector2D ipl98/cpp/vectors/vector2d.h
	@version 0.4,  by edr@mip.sdu.dk
	@author René Dencker Eriksen
	@see CPoint2D */
template<class T>
class CVector2D : public CPoint2D<T>
{
	public: // attributes
	private: // attributes
	public: // methods
		 /// default constructor, init to zero
		CVector2D();
		/// constr. with init. of point
		CVector2D(T x,T y);
		/** Constructs a unit vector from given direction.
			@param Dir Direction in radians. */
		CVector2D(T Dir);
		/// copy constructor with template conversion
		template<class POLY> inline CVector2D(const CVector2D<POLY>& V2D){this->m_x=V2D.GetX();this->m_y=V2D.GetY();}
		/// copy constructor of CPoint2D with template conversion
		template<class POLY> inline CVector2D(const CPoint2D<POLY>& P2D){this->m_x=P2D.GetX();this->m_y=P2D.GetY();}

		/// cast to POLY in mixed expressions
		template<class POLY> operator CVector2D<POLY>(){return CVector2D<POLY>(this->m_x,this->m_y);}

		/**@name overloading assignment operators */
		//@{
		/// overloading = operator with template conversion
		template<class POLY> inline CVector2D<T>& operator=(const CVector2D<POLY>& V2D){this->m_x=V2D.GetX();this->m_y=V2D.GetY();return *this;};
		/// overloading += operator with template conversion
		template<class POLY> inline CVector2D<T>& operator+=(const CVector2D<POLY>& V2D){this->m_x+=V2D.GetX();this->m_y+=V2D.GetY();return *this;};
		/// overloading -= operator with template conversion
		template<class POLY> inline CVector2D<T>& operator-=(const CVector2D<POLY>& V2D){this->m_x-=V2D.GetX();this->m_y-=V2D.GetY();return *this;};
		/** overloading *= operator with template conversion
			 @param Factor Multiply each dimension with this value */
		template<class POLY> inline CVector2D<T>& operator*=(const POLY& Factor){this->m_x*=Factor;this->m_y*=Factor;return *this;}
		/** overloading /= operator with template conversion
			 @param Factor Divide each dimension with this value */
		template<class POLY> inline CVector2D<T>& operator/=(const POLY& Factor){this->m_x/=Factor;this->m_y/=Factor;return *this;}
		//@}
		/**@name overloading comparison operators */
		//@{
		/// overloading == operator with template conversion
		template<class POLY> inline bool operator==(const CVector2D<POLY>& V2D) const{return ((this->m_x==V2D.GetX()) && (this->m_y==V2D.GetY()));}
		/// overloading != operator with template conversion
		template<class POLY> inline bool operator!=(const CVector2D<T>& V2D) const{return ((this->m_x!=V2D.GetX()) || (this->m_y!=V2D.GetY()));}
		//@}
		/**@name overloading arithmetic operators */
		//@{
		/// overloading + operator with template conversion
		template<class POLY> inline CVector2D<T> operator+(const CVector2D<POLY>& V2D) const{return CVector2D<T>(this->m_x+V2D.GetX(),this->m_y+V2D.GetY());}
		/// overloading - operator with template conversion
		template<class POLY> inline CVector2D<T> operator-(const CVector2D<POLY>& V2D) const{return CVector2D<T>(this->m_x-V2D.GetX(),this->m_y-V2D.GetY());}

		/** overloading * operator used for dot product. If you want the dot product
			of two vectors of different types with an implicit type cast, use the method
			GetDot. Trying to implement this dot product with template conversion gives
			an ambiguity with the * operator when used for scaling the vector.
			Note: The operator *(POLY factor) is also overloaded with implicit template
			conversion from POLY to T is also overloaded, but due to a bug in Visual C++ 6.0,
			this friend function cannot be declared inside this class, hence it is only
			declared outside. It makes it possible to write the following:
			\verbatim

			CVector2D<float> V0,V1(1,1);
			V0 = V1*3.0;
			\endverbatim
			@param Vector Calculate dot product with this parameter.
			@see GetDot */
		inline T operator*(const CVector2D<T>& Vector) const {return (this->m_x*Vector.GetX()+this->m_y*Vector.GetY());}

		/** overloading / operator with template conversion
			 @param Factor Divide each dimension with this value */
		template<class POLY> inline CVector2D<T> operator/(const POLY& Factor) const {return CVector2D<T>(this->m_x/Factor,this->m_y/Factor);}
		//@}
		
		/** Returns the dot product between this vector and the parameter Vector.
			@param Vector Calculate dot product with this parameter.
			@return Dot product between the two vectors. */
		template<class POLY> inline T GetDot(const POLY& Vector) const {return (this->m_x*Vector.GetX()+this->m_y*Vector.GetY());}
		/** Returns modulus, that is sqrt(x*x+y*y). Calls the base class GetDist().
			@return Modulus. */
		inline double GetModulus() const;
		/** Returns the angle in radians compared to the vector (1;0). Range is ]-PI,PI].
			Another version of this method with angle between supplied vector and this
			vector is available.
			@return Angle in radians, range ]-PI,PI] */
		inline double GetAngle() const;
		/** Returns the angle in radians between this vector and the supplied vector.
			This vector defines the reference coordinate system with the x-axis aligned
			with this vector. Range is ]-PI,PI]. Another version of this method returns
			angle between this vector and the fixed vector (1;0). This method is a bit slow
			and can probably be optimized.
			@return Angle in radians, range ]-PI,PI] */
		template<class POLY> inline double GetAngle(const CVector2D<POLY>& Vec) const {double p=GetDot(Vec)/(GetModulus()*Vec.GetModulus());if (p>1)p=1;if (GetDeterminant(Vec)>=0) {return atan2(sqrt(1-p*p),p);}else{return -atan2(sqrt(1-p*p),p);}}
		/** Returns the projection of this vector on the supplied vector.
			@param Vec This vector is projected onto Vec.
			@return Projected vector. */
		template<class POLY> inline CVector2D<T> GetProjection(const CVector2D<POLY>& Vec) const {return Vec*GetDot(Vec)/(Vec.GetX()*Vec.GetX()+Vec.GetY()*Vec.GetY());}
		/** Returns the determinant of this vector and the supplied vector.
			@param Vec Vector to calculate determinant with.
			@return Determinant of this vector and supplied vector. */
		template<class POLY> inline double GetDeterminant(const CVector2D<POLY>& Vec) const {return this->m_x*Vec.GetY() - this->m_y*Vec.GetX();}
		/** Returns true, if vectors are parallel with each other.
			@param Vec Vector to compare with.
			@return True, if vectors are parallel. */
		template<class POLY> inline bool IsParallel(const CVector2D<POLY>& Vec) const{return GetDeterminant(Vec)==0;}
		/** Returns true, if vectors are orthogonal to each other.
			@param Vec Vector to compare with.
			@return True, if vectors are orthogonal to each other. */
		template<class POLY> inline bool IsOrthogonal(const CVector2D<POLY>& Vec) const{return GetDot(Vec)==0;}
		/** Normalizes the vector, i.e. length is one after this operation.
			If this vector is a null vector, nothing is done.
			@return False, if this vector is a null vector. */
		inline bool Normalize(){T L=(T)GetModulus();if (L!=0){this->m_x/=L;this->m_y/=L;return true;}else{return false;}}
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

template <class T>
inline double CVector2D<T>::GetModulus() const
{
	return sqrt((double)(this->m_x*this->m_x+this->m_y*this->m_y));
}

template <class T>
inline double CVector2D<T>::GetAngle() const
{
	return atan2((double)this->m_y,(double)this->m_x);
}

/** a utility function overloading the * operator, makes it possible to
	write f*V2D, where f is a factor of type T and V2D is of type
	CVector2D<V>. For some reason this cannot be converted as other
	operators, i.e. f and V2D must contain same types. */
template <class T>
CVector2D<T> operator*(const T& Factor,const CVector2D<T>& V2D)
{
	return CVector2D<T>(V2D.GetX()*Factor, V2D.GetY()*Factor);
}

/** overloading * operator with template conversion
	 @param Factor Multiply each dimension with this value */
template<class T, class POLY> inline
CVector2D<T> operator*(const CVector2D<T>& V2D, const POLY& Factor) 
{
	return CVector2D<T>(V2D.GetX()*Factor, V2D.GetY()*Factor);
}

#include "vector2d.cpp"

/*@}*/ /* end group "containerclasses" */

} // end namespace ipl

#endif //_IPL98_VECTOR2D_H
