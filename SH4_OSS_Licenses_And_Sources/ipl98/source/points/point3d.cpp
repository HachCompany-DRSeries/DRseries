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

#include <iostream>
#include <math.h>

template <class T>
CPoint3D<T>::CPoint3D()
{
	// default constructor
	m_x=0;
	m_y=0;
	m_z=0;
}

template <class T>
CPoint3D<T>::CPoint3D(T x,T y,T z)
{
	// constr. with init.
	m_x=x;
	m_y=y;
	m_z=z;
}

template <class T>
CPoint3D<T>::CPoint3D(const CPoint3D<T>& P3D)
{
	// copy constructor
	*this=P3D;
}

template <class T>
CPoint3D<T>::~CPoint3D()
{
	//destructor
}

template <class T>
void CPoint3D<T>::Print() const
{
	// prints value of point to stdout
	std::cout << "point=(" << m_x << "," << m_y << "," << m_z << ")" << std::endl;
}

template <class T>
std::istream& CPoint3D<T>::operator<<(std::istream& s)
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
	s.get(); // eat ','
	while(isspace(s.peek())){s.get();}while (s.peek()=='#'){s.ignore(500,'\n');while (isspace(s.peek())){s.get();}} 
	s >> m_z;
	while(isspace(s.peek())){s.get();}while (s.peek()=='#'){s.ignore(500,'\n');while (isspace(s.peek())){s.get();}} 
	s.get(); // eat ')'
	return s;
}
