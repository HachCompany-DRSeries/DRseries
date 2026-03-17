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

template <class T>
CVector3D<T>::CVector3D()
{
	// default constructor
	this->m_x=0;
	this->m_y=0;
	this->m_z=0;
}

template <class T>
CVector3D<T>::CVector3D(T x,T y,T z)
{
	// constr. with init.
	this->m_x=x;
	this->m_y=y;
	this->m_z=z;
}
