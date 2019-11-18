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

template<class T>
CLine3D<T>::CLine3D()
{
	m_P.Set(0,0,0);
	m_Dir.Set(1,0,0);
}

template<class T>
CLine3D<T>::CLine3D(const CPoint3D<T>& P, const CVector3D<T>& Dir)
{
	m_P=P;
	m_Dir=Dir;
}

template <class T>
CLine3D<T>::CLine3D(const CLine3D<T>& Line3D)
{
	// copy constructor
	*this=Line3D;
}

template<class T>
CLine3D<T>& CLine3D<T>::operator=(const CLine3D<T>& Line3D)
{
	m_P=Line3D.m_P;
	m_Dir=Line3D.m_Dir;
	return *this;
}

template<class T>
bool CLine3D<T>::Set(const CPoint3D<T>& P, const CVector3D<T>& Dir)
{
	if (Dir.IsZero()==true)
	{
		ostringstream ost;
		ost << "CLine3D<T>::Set() Dir is a zero-vector"	<< IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	m_P=P;
	m_Dir=Dir;
	return true;
}

template<class T>
double CLine3D<T>::Distance(const CPoint3D<T>& P) const
{
	CPoint3D<float> Proj=GetProjection(P);
	return Proj.GetDist(P);
}

template<class T>
CPoint3D<T> CLine3D<T>::GetProjection(const CPoint3D<T>& P) const
{
	CVector3D<T> VecToP(P-GetPoint());
//	CVector3D<T> Dir(GetDir());
//	Dir.Normalize();
	CVector3D<float> ProjVec=VecToP.GetProjection(GetDir());
	return GetPoint()+ProjVec;
}

template<class T>
std::ostream& operator<<(std::ostream& s, const CLine3D<T>& Line)
{
	s << "CLine3D Point: " << Line.GetPoint() << " Dir: " << Line.GetDir();
	return s;
}

template<class T>
std::istream& operator>>(std::istream& is, CLine3D<T>& Line)
{
	string Token;
	ipl::SkipSpaceAndComments(is);
	// read 'CLine3D' token
	is >> Token;
	if (Token!=string("CLine3D"))
	{
		ostringstream ost;
		ost << "operator>>(istream, CLine3D) Token CLine3D not found"
			<< IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return is;
	}
	ipl::SkipSpaceAndComments(is);
	// skip 'Point:' token
	is.ignore(std::numeric_limits<int>::max(),std::char_traits<char>::to_int_type(':'));
	ipl::SkipSpaceAndComments(is);
	is >> Line.m_P;
	ipl::SkipSpaceAndComments(is);
	// skip 'Dir:' token
	is.ignore(std::numeric_limits<int>::max(),std::char_traits<char>::to_int_type(':'));
	ipl::SkipSpaceAndComments(is);
	is >> Line.m_Dir;
	return is;			
}
