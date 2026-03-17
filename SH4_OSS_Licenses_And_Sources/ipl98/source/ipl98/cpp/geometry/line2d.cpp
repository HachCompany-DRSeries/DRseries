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
CLine2D<T>::CLine2D()
{
	m_P.Set(0,0);
	m_Dir.Set(1,0);
}

template<class T>
CLine2D<T>::CLine2D(const CPoint2D<T>& P, const CVector2D<T>& Dir)
{
	m_P=P;
	m_Dir=Dir;
}

template <class T>
CLine2D<T>::CLine2D(const CPoint2D<T>& P1, const CPoint2D<T>& P2)
{
	m_P=P1;
	m_Dir=CVector2D<T>(P2-P1);
}

template <class T>
CLine2D<T>::CLine2D(const CLine2D<T>& Line2D)
{
	// copy constructor
	*this=Line2D;
}

template<class T>
CLine2D<T>& CLine2D<T>::operator=(const CLine2D<T>& Line2D)
{
	m_P=Line2D.m_P;
	m_Dir=Line2D.m_Dir;
	return *this;
}

template<class T>
bool CLine2D<T>::Intersect(const CLine2D<T>& L,CPoint2D<T>& Q) const
{
	float N,a0,a;
	N=(float)(m_Dir.GetY()*L.m_Dir.GetX() - m_Dir.GetX()*L.m_Dir.GetY());
	if (N==0) 
		return false;
	a0=(float)(m_P.GetX()*m_Dir.GetY()-m_P.GetY()*m_Dir.GetX());
	a=(float)(L.m_P.GetX()*L.m_Dir.GetY()-L.m_P.GetY()*L.m_Dir.GetX());
	Q.Set((L.m_Dir.GetX()*a0-m_Dir.GetX()*a)/N,
		(L.m_Dir.GetY()*a0-m_Dir.GetY()*a)/N);
	return true;
}

template<class T>
bool CLine2D<T>::Intersect(const CStdImage& Img, CPoint2D<INT16>& P0, CPoint2D<INT16>& P1) const
{
	CLine2D<T> LineXMin(CPoint2D<T>((T)Img.GetMinX(),0),CVector2D<T>(0,1));
	CLine2D<T> LineXMax(CPoint2D<T>((T)(Img.GetMaxX()-1),0),CVector2D<T>(0,1));
	CLine2D<T> LineYMin(CPoint2D<T>(0,(T)Img.GetMinY()),CVector2D<T>(1,0));
	CLine2D<T> LineYMax(CPoint2D<T>(0,(T)(Img.GetMaxY()-1)),CVector2D<T>(1,0));
	CPoint2D<T> Q;
	// find intersection in left side of image
	if (Intersect(LineXMin,Q)==true)
	{
		if (Q.GetY()<Img.GetMinY())
			Intersect(LineYMin,Q);
		else if (Q.GetY()>Img.GetMaxY()-1)
			Intersect(LineYMax,Q);
	}
	else
		Intersect(LineYMin,Q);
	if ((Q.GetX()<Img.GetMinX()) || (Q.GetY()<Img.GetMinY()) || (Q.GetY()>Img.GetMaxY()-1))
		return false;
	P0=Q;
	// find intersection in rigth side of image
	if (Intersect(LineXMax,Q)==true)
	{
		if (Q.GetY()>Img.GetMaxY()-1)
			Intersect(LineYMax,Q);
		else if (Q.GetY()<Img.GetMinY())
			Intersect(LineYMin,Q);
	}
	else
		Intersect(LineYMax,Q);
	if ((Q.GetX()>Img.GetMaxX()-1) || (Q.GetY()<Img.GetMinY()) || (Q.GetY()>Img.GetMaxY()-1))
		return false;
	P1=Q;
	if ((GetDir().GetAngle()>(T)(ipl::PI/2.0)) || (GetDir().GetAngle()<-(T)(ipl::PI/2)))
		Swap(P0,P1);
	return true;
}

//template<class T>
//bool CLine2D<T>::Intersect(const CLine2D<T>& L, CPoint2D<T>& Q) const
//{
//	if (m_Dir==L.GetDir())
//	{
//		return false;
//	}
//	else
//	{
//		T x,y,f1,f2;
//		if ((m_Dir.GetX()==0) && (L.GetDir().GetY()==0))
//		{
//			Q.Set(m_P.GetX(),L.GetPoint().GetY());
//		}
//		else if ((m_Dir.GetY()==0) && (L.GetDir().GetX()==0))
//		{
//			Q.Set(L.GetPoint().GetX(),m_P.GetY());
//		}
//		else 
//		{
//			T b1,b2;
//			if (fabs(m_Dir.GetY())<fabs(m_Dir.GetX()))
//			{
//				f1=m_Dir.GetY()/m_Dir.GetX();
//				f2=L.GetDir().GetY()/L.GetDir().GetX();
//				b1=m_P.GetY()-f1*m_P.GetX();
//				b2=L.GetPoint().GetY()-f2*L.GetPoint().GetX();
//				x=(b2-b1)/(f1-f2);
//				y=f2*x+b2;
//			}
//			else
//			{
//				f1=m_Dir.GetX()/m_Dir.GetY();
//				f2=L.GetDir().GetX()/L.GetDir().GetY();
//				b1=m_P.GetX()-f1*m_P.GetY();
//				b2=L.GetPoint().GetX()-f2*L.GetPoint().GetY();
//				y=(b2-b1)/(f1-f2);
//				x=f2*y+b2;
//
//			}
//			Q.Set(x,y);
//		}
//		return true;
//	}
//}

template<class T>
bool CLine2D<T>::Set(const CPoint2D<T>& P1, const CPoint2D<T>& P2)
{
	if (P1==P2)
	{
		ostringstream ost;
		ost << "CLine2D<T>::Set() The two provided points contains same positions"
			<< IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	m_P=P1;
	m_Dir=CVector2D<T>(P2-P1);
	return true;
}

template<class T>
bool CLine2D<T>::Set(const CPoint2D<T>& P, const CVector2D<T>& Dir)
{
	if (Dir.IsZero()==true)
	{
		ostringstream ost;
		ost << "CLine2D<T>::Set() Dir is a zero-vector"	<< IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	m_P=P;
	m_Dir=Dir;
	return true;
}


template<class T>
std::ostream& operator<<(std::ostream& s, const CLine2D<T>& Line)
{
	s << "CLine2D Point: " << Line.GetPoint() << " Dir: " << Line.GetDir();
	return s;
}

template<class T>
std::istream& operator>>(std::istream& is, CLine2D<T>& Line)
{
	string Token;
	ipl::SkipSpaceAndComments(is);
	// read 'CLine2D' token
	is >> Token;
	if (Token!=string("CLine2D"))
	{
		ostringstream ost;
		ost << "operator>>(istream, CLine2D) Token CLine2D not found"
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
