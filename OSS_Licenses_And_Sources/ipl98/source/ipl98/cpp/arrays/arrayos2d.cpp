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
CArrayOS2D<T>::CArrayOS2D() : CArray2D<T>()
{
	SetSubIndexing(0); // subindexing defaults to zero
	SetOrigo(CPoint2D<double>(0,0));
}

template <class T>
CArrayOS2D<T>::CArrayOS2D(UINT32 x, UINT32 y, unsigned int SubIndexing)
{
	this->m_pData=NULL;
	if (Alloc(x,y,SubIndexing)==false)
	{
		ostringstream ost;
		ost << "CArrayOS2D<T>::CArrayOS2D() Object construction failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
}

template <class T>
CArrayOS2D<T>::CArrayOS2D(const CArrayOS2D<T>& M)
{
	// copy constructor
	this->m_pData=NULL;
	*this=M;
}

template <class T>
void CArrayOS2D<T>::Empty()
{
	CArray2D<T>::Empty(); // call base class' Empty()
	SetSubIndexing(0); // subindexing defaults to zero
	SetOrigo(CPoint2D<double>(0,0));
}

template <class T>
bool CArrayOS2D<T>::Alloc(UINT32 x, UINT32 y, unsigned int SubIndexing)
{
	//SubIndexing++; // internal stored value is added one to avoid some calculation later on
	if (CArray2D<T>::Alloc(x*(SubIndexing+1),y*(SubIndexing+1))==false) // call base class' Alloc()
		return false;
	bool ReturnValue=SetSubIndexing(SubIndexing);
	SetOrigo(CPoint2D<double>(0,0));
	return ReturnValue;
}

template <class T>
double CArrayOS2D<T>::GetSubStep() const
{
	return m_SubStep;
}

template <class T>
bool CArrayOS2D<T>::Load(const char* pPathAndFileName)
{
	ifstream s(pPathAndFileName);
	if (s.is_open()==true)
	{
		Empty(); // delete all contents
		string str;
		SkipSpaceAndComments(s);
		s >> str;
		if (str!=string("CArrayOS2D"))
		{
			ostringstream ost;
			ost << "CArrayOS2D<T>::Load() Did not find the token 'CArray2D' in beginning of file" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			s.close();
			return false;
		}
		SkipSpaceAndComments(s);
		s >> str;
		if (str!=string("Dimensions:"))
		{
			ostringstream ost;
			ost << "CArray2D<T>::Load() Did not find the token 'Dimensions:'" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			s.close();
			return false;
		}
		SkipSpaceAndComments(s);
		s >> this->m_Width;
		SkipSpaceAndComments(s);
		s >> this->m_Height;
		SkipSpaceAndComments(s);
		s >> str;
		if (str!=string("Origo:"))
		{
			ostringstream ost;
			ost << "CArray2D<T>::Load() Did not find the token 'Origo:'" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			s.close();
			return false;
		}
		SkipSpaceAndComments(s);
		CPoint2D<double> Origo;
		Origo << s;
		SkipSpaceAndComments(s);
		s >> str;
		if (str!=string("SubIndexing:"))
		{
			ostringstream ost;
			ost << "CArray2D<T>::Load() Did not find the token 'SubIndexing:'" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			s.close();
			return false;
		}
		SkipSpaceAndComments(s);
		s >> m_SubIndexing;
		Alloc(this->m_Width,this->m_Height,m_SubIndexing);
		SetOrigo(Origo);
		for(double y=GetMinY(); y<GetMaxY(); y+=GetSubStep())
		{
			for(double x=GetMinX(); x<GetMaxX(); x+=GetSubStep())
			{
				SkipSpaceAndComments(s);
				operator[](x)[y] << s;
			}
		}
		this->m_Origin=DISC;
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CArrayOS2D<T>::Load() Failed opening file: " << pPathAndFileName << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

template <class T>
bool CArrayOS2D<T>::Save(const char* pPathAndFileName)
{
	ofstream s(pPathAndFileName);
	if (s.is_open()==true)
	{
		s << "# Data from a CArrayOS2D object in IPL98" << endl;
		//s << "CArrayOS2D" << endl;
		s << "Dimensions: " << GetWidth() << " " << GetHeight() << endl;
		s << "Origo: " << GetOrigo() << endl;
		s << "SubIndexing: " << GetSubIndexing() << endl;
		for(double y=GetMinY(); y<GetMaxY(); y+=GetSubStep())
		{
			for(double x=GetMinX(); x<GetMaxX(); x+=GetSubStep())
				s << operator[](x)[y] << " ";
			s << "\n";
		}
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CArrayOS2D<T>::Save() Failed opening file: " << pPathAndFileName << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

template <class T>
bool CArrayOS2D<T>::CopySubArray(const CArrayOS2D<T>& Source,double xmin,double ymin,double xmax,double ymax)
{
	double Step=m_SubIndexing;
//	double Step=GetSubIndexing();
	int ixmin=(int)(Source.GetNearestSubIndex(xmin)+Source.GetOrigo().GetX()*Step);
	int ixmax=(int)(Source.GetNearestSubIndex(xmax)+Source.GetOrigo().GetX()*Step);
	int iymin=(int)(Source.GetNearestSubIndex(ymin)+Source.GetOrigo().GetY()*Step);
	int iymax=(int)(Source.GetNearestSubIndex(ymax)+Source.GetOrigo().GetY()*Step);
	if ((xmin<xmax) && (ymin<ymax) && 
		(ixmin>=0) && (ixmax<=(int)Source.GetWidthCells()-1) &&
		(iymin>=0) && (iymax<=(int)Source.GetHeightCells()-1))
	{
		// call base class method
		bool ReturnValue=CArray2D<T>::CopySubArray(Source,ixmin,iymin,ixmax,iymax);
		// setting subindexing
		SetSubIndexing(Source.GetSubIndexing());
		return ReturnValue;
	}
	else
	{
		ostringstream ost;
		ost << "CArrayOS2D<T>::CopySubArray() Illegal clipping rectangel" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

template <class T>
bool CArrayOS2D<T>::CopySubArray(const CArrayOS2D<T>& Source,const CPoint2D<double>& UpperLeft,
		  const CPoint2D<double>& LowerRight)
{
	return CopySubArray(Source, UpperLeft.GetX(),UpperLeft.GetY(),LowerRight.GetX(),
					LowerRight.GetY());
}
