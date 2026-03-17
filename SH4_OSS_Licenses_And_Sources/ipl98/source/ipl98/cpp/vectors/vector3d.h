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

#ifndef _IPL98_VECTOR3D_H
#define _IPL98_VECTOR3D_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../../../points/point3d.h"
#include <iostream>

namespace ipl{ // use namespace if C++

/** @ingroup containerclasses
@{ */

/** CVector3D template class with 3-dimensional vectors,
	last updated 1/31/2005.
	This class is inherited from the CPoint3D class, this
	version adds some new vector methods such as calculations
	of angle, projections, dot product etc.
	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CVector3D ipl98/cpp/vectors/vector3d.h
	@version 0.41,  by edr@mip.sdu.dk
	@author René Dencker Eriksen
	@see CPoint3D */
template<class T>
class CVector3D : public CPoint3D<T>
{
	public: // attributes
	private: // attributes
	public: // methods
		 /// default constructor, init to zero
		CVector3D();
		/// constr. with init. of point
		CVector3D(T x,T y,T z);
		/// copy constructor with template conversion
		template<class POLY> inline CVector3D(const CVector3D<POLY>& V3D){this->m_x=(T)V3D.GetX();this->m_y=(T)V3D.GetY();this->m_z=(T)V3D.GetZ();}
		/// copy constructor of CPoint3D with template conversion
		template<class POLY> inline CVector3D(const CPoint3D<POLY>& P3D){this->m_x=(T)P3D.GetX();this->m_y=(T)P3D.GetY();this->m_z=(T)P3D.GetZ();}

		/// cast to POLY in mixed expressions
		template<class POLY> operator CVector3D<POLY>(){return CVector3D<POLY>(this->m_x,this->m_y,this->m_z);}

		/**@name overloading assignment operators */
		//@{
		/// overloading = operator with template conversion
		template<class POLY> inline CVector3D<T>& operator=(const CVector3D<POLY>& V3D){this->m_x=V3D.GetX();this->m_y=V3D.GetY();this->m_z=V3D.GetZ();return *this;};
		/// overloading += operator with template conversion
		template<class POLY> inline CVector3D<T>& operator+=(const CVector3D<POLY>& V3D){this->m_x+=V3D.GetX();this->m_y+=V3D.GetY();this->m_z+=V3D.GetZ();return *this;};
		/// overloading -= operator with template conversion
		template<class POLY> inline CVector3D<T>& operator-=(const CVector3D<POLY>& V3D){this->m_x-=V3D.GetX();this->m_y-=V3D.GetY();this->m_z-=V3D.GetZ();return *this;};
		/** overloading *= operator with template conversion
			 @param Factor Multiply each dimension with this value */
		template<class POLY> inline CVector3D<T>& operator*=(const POLY& Factor){this->m_x*=Factor;this->m_y*=Factor;this->m_z*=Factor;return *this;}
		/** overloading /= operator with template conversion
			 @param Factor Divide each dimension with this value */
		template<class POLY> inline CVector3D<T>& operator/=(const POLY& Factor){this->m_x/=Factor;this->m_y/=Factor;this->m_z/=Factor;return *this;}
		//@}
		/**@name overloading comparison operators */
		//@{
		/// overloading == operator with template conversion
		template<class POLY> inline bool operator==(const CVector3D<POLY>& V3D) const{return ((this->m_x==V3D.GetX()) && (this->m_y==V3D.GetY()) && (this->m_z==V3D.GetZ()));}
		/// overloading != operator with template conversion
		template<class POLY> inline bool operator!=(const CVector3D<T>& V3D) const{return ((this->m_x!=V3D.GetX()) || (this->m_y!=V3D.GetY()) || (this->m_z!=V3D.GetZ()));}
		//@}
		/**@name overloading arithmetic operators */
		//@{
		/// overloading + operator with template conversion
		template<class POLY> inline CVector3D<T> operator+(const CVector3D<POLY>& V3D) const{return CVector3D<T>(this->m_x+V3D.GetX(),this->m_y+V3D.GetY(),this->m_z+V3D.GetZ());}
		/// overloading - operator with template conversion
		template<class POLY> inline CVector3D<T> operator-(const CVector3D<POLY>& V3D) const{return CVector3D<T>(this->m_x-V3D.GetX(),this->m_y-V3D.GetY(),this->m_z-V3D.GetZ());}

		/** overloading * operator used for dot product. If you want the dot product
			of two vectors of different types with an implicit type cast, use the method
			GetDot. Trying to implement this dot product with template conversion gives
			an ambiguity with the * operator when used for scaling the vector.
			Note: The operator *(POLY factor) is also overloaded with implicit template
			conversion from POLY to T is also overloaded, but due to a bug in Visual C++ 6.0,
			this friend function cannot be declared inside this class, hence it is only
			declared outside. It makes it possible to write the following:
			\verbatim

			CVector3D<float> V0,V1(1,1,1);
			V0 = V1*3.0;
			\endverbatim
			@param Vector Calculate dot product with this parameter.
			@see GetDot */
		inline T operator*(const CVector3D<T>& Vector) const {return (this->m_x*Vector.GetX()+this->m_y*Vector.GetY()+this->m_z*Vector.GetZ());}

		/** overloading / operator with template conversion
			 @param Factor Divide each dimension with this value */
		template<class POLY> inline CVector3D<T> operator/(const POLY& Factor) const {return CVector3D<T>(this->m_x/Factor,this->m_y/Factor,this->m_z/Factor);}
		//@}
		
		/** Returns the dot product between this vector and the parameter Vector.
			@param Vector Calculate dot product with this parameter.
			@return Dot product between the two vectors. */
		template<class POLY> inline T GetDot(const POLY& Vector) const {return (T)(this->m_x*Vector.GetX()+this->m_y*Vector.GetY()+this->m_z*Vector.GetZ());}
		/** Returns modulus, that is sqrt(x*x+y*y+z*z). Calls the base class GetDist().
			@return Modulus. */
		inline double GetModulus() const;
		/** Returns the angle in radians in the X-Y plane compared to the vector (1;0). Range is [0,PI].
			Another version of this method with angle between supplied vector and this
			vector is available.
			@return Angle in radians, range [0,PI] */
		inline T GetXYAngle() const;
		/** Returns the angle in radians between this vector and the X-Y plane. Range is [0,PI].
			@return Angle in radians, range [0,PI] */
		inline T GetZAngle() const;
		/** Returns the angle in radians between this vector and the supplied vector.
			Range is [0,PI]. Other methods are available for angle in X-Y plane and angle
			between vector and .X-Y plane.
			@return Angle in radians, range [0,PI]
			@see GetXYAngle
			@see GetZAngle */
		template<class POLY> inline double GetAngle(const CVector3D<POLY>& Vec) const {double p=(double)GetDot(Vec)/(GetModulus()*Vec.GetModulus());if (p>1)p=1;return atan2(sqrt(1-p*p),p);}
		/** Returns the projection of this vector on the supplied vector.
			@param Vec This vector is projected onto Vec.
			@return Projected vector. */
		template<class POLY> inline CVector3D<T> GetProjection(const CVector3D<POLY>& Vec) const {return Vec*GetDot(Vec)/(Vec.GetX()*Vec.GetX()+Vec.GetY()*Vec.GetY()+Vec.GetZ()*Vec.GetZ());}
		/** Returns the cross product of this vector and the supplied vector, that is a cross
			between vector object A and B: A x B is obtained by A.GetCross(B).
			@param Vec Vector to calculate cross product with.
			@return Cross product of this vector and supplied vector. */
		template<class POLY> inline CVector3D<T> GetCross(const CVector3D<POLY>& Vec) const {return CVector3D<T>(this->m_y*Vec.m_z-this->m_z*Vec.m_y, this->m_z*Vec.m_x-this->m_x*Vec.m_z, this->m_x*Vec.m_y-this->m_y*Vec.m_x);}
		/** Returns true, if vector is a zero-vector.
			@return True, if vector is a zero-vector. */
		inline bool IsZero() const;
		/** Returns true, if vectors are parallel with each other.
			@param Vec Vector to compare with.
			@return True, if vectors are parallel. */
		template<class POLY> inline bool IsParallel(const CVector3D<POLY>& Vec) const{return GetCross(Vec).IsZero();}
		/** Returns true, if vectors are orthogonal to each other.
			@param Vec Vector to compare with.
			@return True, if vectors are orthogonal to each other. */
		template<class POLY> inline bool IsOrthogonal(const CVector3D<POLY>& Vec) const{return GetDot(Vec)==0;}
		/** Normalizes the vector, i.e. length is one after this operation.
			If this vector is a null vector, nothing is done.
			@return False, if this vector is a null vector. */
		inline bool Normalize(){T L=(T)GetModulus();if (L!=0){this->m_x/=L;this->m_y/=L;this->m_z/=L;return true;}else{return false;}}
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

template <class T>
inline T CVector3D<T>::GetXYAngle() const
{
	return (T)atan2((double)this->m_y, (double)this->m_x);
}

template <class T>
inline T CVector3D<T>::GetZAngle() const
{
	 return (T)atan2((double)this->m_z,(double)sqrt((double)(this->m_x*this->m_x + this->m_y*this->m_y)));
}

template <class T>
inline double CVector3D<T>::GetModulus() const
{
	return sqrt((double)(this->m_x*this->m_x+this->m_y*this->m_y+this->m_z*this->m_z));
}

template <class T>
inline bool CVector3D<T>::IsZero() const
{
	return ((this->m_x==T(0)) && (this->m_y==T(0)) && (this->m_z==T(0)));
}

/** a utility function overloading the * operator, makes it possible to write
	f*V3D, where f is a factor of type T and V3D is of type CVector3D<V>. For
	some reason this cannot be converted as other operators, i.e. f and V3D must
	contain same types. */
template <class T>
CVector3D<T> operator*(const T& Factor,const CVector3D<T>& V3D)
{
	return CVector3D<T>(
		V3D.GetX() * Factor,
		V3D.GetY() * Factor,
		V3D.GetZ() * Factor);
}

/** overloading * operator with template conversion
	 @param Factor Multiply each dimension with this value */
template<class T, class POLY> inline 
CVector3D<T> operator*(const CVector3D<T>& V3D, const POLY& Factor) 
{
	return CVector3D<T>(
		(T)(V3D.GetX() * Factor),
		(T)(V3D.GetY() * Factor),
		(T)(V3D.GetZ() * Factor));
}

/** overloading * operator used for dot product.

	If you want the dot product of two vectors of different types with an
	implicit type cast, use the method GetDot. Trying to implement this dot
	product with template conversion gives an ambiguity with the * operator when
	used for scaling the vector. Note: The operator *(POLY factor) is also
	overloaded with implicit template conversion from POLY to T is also
	overloaded, but due to a bug in Visual C++ 6.0, this friend function cannot
	be declared inside this class, hence it is only declared outside. It makes
	it possible to write the following:

	\verbatim
	CVector3D<float> V0,V1(1,1,1);
	V0 = V1*3.0;
	\endverbatim

	@param Vector Calculate dot product with this parameter.
	@see GetDot */
template<class T>
T operator* (const CVector3D<T>& a, const CVector3D<T>& b)
{
	return
		a.GetX() * b.GetX()+
		a.GetY() * b.GetY()+
		a.GetZ() * b.GetZ();
}

#include "vector3d.cpp"

/*@}*/ /* end group "containerclasses" */

} // end namespace ipl

#endif //_IPL98_VECTOR3D_H
