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
CArray2D<T>::CArray2D()
{
	m_Width = 0;
	m_Height = 0;
	m_pData = NULL;
	m_Origin=EMPTY;
}

template <class T>
CArray2D<T>::CArray2D(UINT32 x, UINT32 y)
{
	m_pData=NULL;
	Alloc(x,y);
}

template <class T>
CArray2D<T>::~CArray2D()
{
	Empty();
}

template <class T>
CArray2D<T>::CArray2D(const CArray2D<T>& M)
{
	// copy constructor
	m_pData=NULL;
	*this=M;
}

template <class T>
void CArray2D<T>::Empty()
{
	if (m_pData!=NULL)
		delete m_pData;
	m_pData=NULL;
	m_Width = 0;
	m_Height = 0;
	m_Origin=EMPTY;
}

template <class T>
bool CArray2D<T>::Alloc(UINT32 x, UINT32 y)
{
	if ((x!=m_Width) || (y!=m_Height))
	{
		Empty();
		m_Width = x;
		m_Height = y;
		if ((m_pData = new valarray<T>(x*y)) == NULL)
		{
			ostringstream ost;
			ost << "CArray2D<T>::Alloc() Failed allocating data" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			return false;
		}
	}
	// set origin to RAM in all cases
	m_Origin=RAM;
	return true;
}

template <class T>
UINT32 CArray2D<T>::Size() const
{
	return m_Width*m_Height;
}

template <class T>
bool CArray2D<T>::CopySubArray(const CArray2D<T>& Source,unsigned int xmin,unsigned int ymin,
							   unsigned int xmax,unsigned int ymax)
{
	CArray2D<T>* pTarget=this;
	bool Copy=false;
	/* Not optimised regarding speed! */
	if ((xmin<xmax) && (ymin<ymax) &&
                (xmax<=Source.GetWidth()) && (ymax<=Source.GetHeight()))
	{
		/* Check if the source and destination is the same array */
		if (&Source==this)
		{
			pTarget=new CArray2D<T>(xmax-xmin+1,ymax-ymin+1);
			Copy=true;
		}
		else if ((pTarget->GetWidth()!=Source.GetWidth()) || (pTarget->GetHeight()!=Source.GetHeight()))
		{
			pTarget->Alloc(xmax-xmin+1,ymax-ymin+1);
		}
		unsigned int i,j;
		/* copy cells */
		for(i=ymin; i<=ymax; i++)
			for(j=xmin; j<=xmax; j++)
				(*pTarget)[j-xmin][i-ymin]=Source[j][i];

		if (Copy==true)
        {
			*this=*pTarget;
			delete pTarget;
        }
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CArray2D<T>::CopySubArray() Illegal clipping rectangel" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

template <class T>
bool CArray2D<T>::Load(const char* pPathAndFileName)
{
	ifstream s(pPathAndFileName);
	if (s.is_open()==true)
	{
		Empty(); // delete all contents
		string str;
		SkipSpaceAndComments(s);
		s >> str;
		if (str!=string("CArray2D"))
		{
			ostringstream ost;
			ost << "CArray2D<T>::Load() Did not find the token 'CArray2D' in beginning of file" << IPLAddFileAndLine;
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
		s >> m_Width;
		SkipSpaceAndComments(s);
		s >> m_Height;
		Alloc(m_Width,m_Height);
		for(unsigned int y=0; y<GetHeight(); y++)
		{
			for(unsigned int x=0; x<GetWidth(); x++)
			{
				SkipSpaceAndComments(s);
				T& Element=operator[](x)[y];
				//operator[](x)[y] << s;
				s >> Element;
//				s >> operator[](x)[y];
			}
		}
		m_Origin=DISC;
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CArray2D<T>::Load() Failed opening file: " << pPathAndFileName << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

template <class T>
bool CArray2D<T>::Save(const char* pPathAndFileName)
{
	ofstream s(pPathAndFileName);
	if (s.is_open()==true)
	{
		s << "# Data from a CArray2D object in IPL98" << endl;
		s << "CArray2D" << endl;
		s << "Dimensions: " << GetWidth() << " " << GetHeight() << endl;
		for(unsigned int y=0; y<GetHeight(); y++)
		{
			for(unsigned int x=0; x<GetWidth(); x++)
			{
				s << operator[](x)[y];
				s << " ";
			}
			s << "\n";
		}
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CArray2D<T>::Save() Failed opening file: " << pPathAndFileName << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

template <class T>
bool CArray2D<T>::CopySubArray(const CArray2D<T>& Source,const CPoint2D<unsigned int >& UpperLeft,
							   const CPoint2D<unsigned int >& LowerRight)
{
	return CopySubArray(Source, UpperLeft.GetX(),UpperLeft.GetY(),LowerRight.GetX(),
					LowerRight.GetY());
}

template <class T>
void CArray2D<T>::Rotate90(int Steps)
{
	CArray2D<T> Source(*this);
	int x,y;
	int MaxYIndex=m_Height-1;
	int MaxXIndex=m_Width-1;
	int ActSteps=(Steps % 4);
	if (ActSteps<0)
		ActSteps=4+ActSteps;

	if (m_Origin==EMPTY)
	{
		ostringstream ost;
		ost << "CArray2D<T>::Rotate90() Array is empty - doing nothing" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_WARNING,ost);
		return;
	}
	if (ActSteps==0)
		return;

	/* prepare destination image, following lines faster than calling k_CopyImage() */
	if (ActSteps==2)
		Alloc(Source.GetWidth(),Source.GetHeight());
	else
		Alloc(Source.GetHeight(),Source.GetWidth());
	/* rotate array */
	if (ActSteps==1)
	{
		for(y=0; y<(int)Source.GetHeight(); y++)
			for(x=0; x<(int)Source.GetWidth(); x++)
			{
				operator[](-y+MaxYIndex)[x]=Source[x][y];
			}
	}
	else if (ActSteps==2)
	{
		for(y=0; y<(int)Source.GetHeight(); y++)
			for(x=0; x<(int)Source.GetWidth(); x++)
				operator[](x)[MaxYIndex-y]=Source[MaxXIndex-x][y];
	}
	else if (ActSteps==3)
	{
		for(y=0; y<(int)Source.GetHeight(); y++)
			for(x=0; x<(int)Source.GetWidth(); x++)
				operator[](y)[MaxXIndex-x]=Source[x][y];
	}
}

template <class T>
void CArray2D<T>::FlipHorisontal()
{
	CArray2D<T> Source(*this);
	unsigned int x,y;
	int MaxYIndex=m_Height-1;
	if (m_Origin==EMPTY)
	{
		ostringstream ost;
		ost << "CArray2D<T>::FlipHorisontal() Array is empty - doing nothing" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_WARNING,ost);
		return;
	}
	/* flip image */
	for(y=0; y<m_Height; y++)
		for(x=0; x<m_Width; x++)
			operator[](x)[MaxYIndex-y]=Source[x][y];
}

template <class T>
void CArray2D<T>::FlipVertical()
{
	CArray2D<T> Source(*this);
	unsigned int x,y;
	int MaxXIndex=m_Width-1;
	if (m_Origin==EMPTY)
	{
		ostringstream ost;
		ost << "CArray2D<T>::FlipVertical() Array is empty - doing nothing" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_WARNING,ost);
		return;
	}
	/* flip image */
	for(y=0; y<m_Height; y++)
		for(x=0; x<m_Width; x++)
			operator[](MaxXIndex-x)[y]=Source[x][y];
}

template <class T>
bool CArray2D<T>::CopyDisplace(int dx,int dy)
{
	if (m_Origin!=EMPTY)
	{
		int x,y,w,h,xd,yd;
		CArray2D<T> Source(*this);
		w=m_Width;
		h=m_Height;
		for (y=0;y<h;y++)
		{
			for (x=0;x<w;x++)
			{
				xd=x-dx;
				yd=y-dy;
				while (xd<0) xd+=w;
				while (xd>=w) xd-=w;
				while (yd<0) yd+=h;
				while (yd>=h) yd-=h;
				operator[](xd)[yd]=Source[x][y];
			}
		}
		return true;
	} 
	else
	{
		ostringstream ost;
		ost << "CArray2D<T>::CopyDisplace() Array is empty - doing nothing" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_WARNING,ost);
		return false;
	}
}

//template<class T> 
//istream& CArray2D<T>::operator<<(istream& s)
//{
//	Empty(); // delete all contents of this tensor field
//	string str;
//	SkipSpaceAndComments(s);
//	s >> str;
//	if (str!=string("CArray2D"))
//	{
//		cerr << "ERROR: Did not find the token CArray2D in beginning of file" << endl;
//	}
//	//s.ignore(numeric_limits<int>::max(),'\n');
//	SkipSpaceAndComments(s);
//	s >> str;
//	if (str!=string("Dimensions:"))
//	{
//		cerr << "ERROR: Did not find the token Dimensions:" << endl;
//	}
//	SkipSpaceAndComments(s);
//	s >> m_Width >> m_Height;
//	Alloc(m_Width,m_Height);
//	for(int y=0; y<GetHeight(); y++)
//	{
//		for(int x=0; x<GetWidth(); x++)
//		{
//			SkipSpaceAndComments(s);
//			operator[](x)[y] << s;
//		}
//	}
//	m_Origin=FILE;
//	return s;
//}
