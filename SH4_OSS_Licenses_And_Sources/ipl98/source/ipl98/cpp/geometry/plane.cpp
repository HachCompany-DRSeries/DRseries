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
CPlane<T>::CPlane()
{
	m_P.Set(0,0,0);
	m_DirA.Set(1,0,0);
	m_DirB.Set(0,1,0);
}

template<class T>
CPlane<T>::CPlane(const CPoint3D<T>& P, const CVector3D<T>& DirA, const CVector3D<T>& DirB)
{
	m_P=P;
	m_DirA=DirA;
	m_DirB=DirB;
}

template <class T>
CPlane<T>::CPlane(const CPlane<T>& Plane)
{
	// copy constructor
	*this=Plane;
}

template<class T>
CPlane<T>& CPlane<T>::operator=(const CPlane<T>& Plane)
{
	m_P=Plane.m_P;
	m_DirA=Plane.m_DirA;
	m_DirB=Plane.m_DirB;
	return *this;
}

template<class T>
bool CPlane<T>::Set(const CPoint3D<T>& P, const CVector3D<T>& DirA, 
					const CVector3D<T>& DirB)
{
	if (DirA.IsZero()==true)
	{
	}
	if ((DirA.IsZero()) || (DirB.IsZero()) || (DirA.IsParallel(DirB)==true))
	{
		ostringstream ost;
		ost << "CPlane<T>::Set() Input direction are illegal"
			<< IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	m_P=P;
	m_DirA=DirA;
	m_DirB=DirB;
	return true;
}

template<class T>
bool CPlane<T>::Intersect(const CPlane<T>& P, CLine3D<T> &L) const
{
	CVector3D<T> N1,N2,N3;
	N1=GetPlaneNormal();
	N2=P.GetPlaneNormal();
	N3=N1.GetCross(N2);
	if (N3.IsZero()==true) 
		return false; // parallel planes
	else
	{
		CVector3D<T> N4(N3.GetCross(N1));
		CVector3D<T> Q;
		CLine3D<T> L1(GetPoint(),N4);
		if (P.Intersect(L1,Q)==true)
		{
			L.Set(Q,N3);
			return true;
		}
		else 
			return false;
	}
}
		
template<class T>
bool CPlane<T>::Intersect(const CLine3D<T>& L, CPoint3D<T>& Q) const
{
	const CVector3D<T>& D=L.GetDir();
	T F= m_DirA.GetX()*(-m_DirB.GetY()*D.GetZ()+m_DirB.GetZ()*D.GetY())
		 +m_DirA.GetY()*(-m_DirB.GetZ()*D.GetX()+m_DirB.GetX()*D.GetZ())
		 +m_DirA.GetZ()*(-m_DirB.GetX()*D.GetY()+m_DirB.GetY()*D.GetX());
	if (F!=0)
	{
		CVector3D<T> P(L.GetPoint()-GetPoint());
		T Tl=(m_DirA.GetX()*(m_DirB.GetY()*P.GetZ()-m_DirB.GetZ()*P.GetY())
			+m_DirA.GetY()*(m_DirB.GetZ()*P.GetX()-m_DirB.GetX()*P.GetZ())
			+m_DirA.GetZ()*(m_DirB.GetX()*P.GetY()-m_DirB.GetY()*P.GetX()))/F;
		Q=L.GetPoint()+D*Tl;
		//Q.Set(L.GetPoint()+D*Tl);
		return true;
	} 
	else 
		return false;//Line parallel with plane
}

template<class T>
CPoint3D<T> CPlane<T>::GetProjection(const CPoint3D<T>& P)
{
	if (P.GetDist(m_P)==0)
		return P;
	CVector3D<T> Normal=GetPlaneNormal();
	CVector3D<T> V1,V2,V3;
	V1=(CVector3D<T>)(P-m_P);
	V2=V1.GetCross(Normal);
	if (V2.GetModulus()==0)
		return P;
	V3=V2.GetCross(Normal);
	return m_P+(CPoint3D<T>)(V1.GetProjection(V3));
}

