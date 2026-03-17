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

#include "pixelgroup.h"
#include "ipl98_general_functions.h"

namespace ipl{

CPixelGroup::CPixelGroup()
{
	// default constructor
	m_PosPrLine=9999;
	m_pGroup=new TPixelGroup;
	m_OwnGroup=true;
	k_InitGroup(m_pGroup);
}

CPixelGroup::CPixelGroup(TPixelGroup& TGroup)
{
	// constructor which takes an external TGroup as reference to data
	m_PosPrLine=9999;
	m_pGroup=&TGroup;
	m_OwnGroup=false;
}

CPixelGroup::CPixelGroup(unsigned int InitialSize, bool AllocateColors)
{
	m_PosPrLine=9999;
	m_pGroup=new TPixelGroup;
	m_OwnGroup=true;
	k_InitGroup(m_pGroup);
	if (AllocateColors==true)
	{
		k_AllocGroupAndColors(m_pGroup,InitialSize);
	}
	else
	{
		k_AllocGroup(m_pGroup,InitialSize);
	}
}

CPixelGroup::CPixelGroup(const CPixelGroup& SourceGroup)
{ 
	// copy constructor
	m_pGroup=new TPixelGroup;
	m_OwnGroup=true;
	k_InitGroup(m_pGroup);
	*this=SourceGroup; // use assignment operator
}

CPixelGroup& CPixelGroup::operator=(const CPixelGroup& SourceGroup)
{
	// overloading of assignment operator
	if (this != &SourceGroup)
	{
		// ok to do assignment
		m_PosPrLine=SourceGroup.m_PosPrLine;
		k_CopyGroup(GetTPixelGroupPtr(),SourceGroup.GetConstTPixelGroupPtr());
	}
	else
	{
		ostringstream ost;
		ost << "CPixelGroup::operator=() Cannot assign CPixelGroup to itself" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return *this;
}

CPixelGroup::~CPixelGroup()
{
	//destructor
	if(m_OwnGroup==true)
	{
		k_EmptyGroup(m_pGroup);
		delete m_pGroup;
	}
}

void CPixelGroup::Empty()
{
	k_EmptyGroup(m_pGroup);
}

void CPixelGroup::EmptyNoDeallocation()
{
	k_EmptyGroupNoDeallocation(m_pGroup);
}

CPoint2D<int> CPixelGroup::GetTop() const
{
	return CPoint2D<int>(m_pGroup->Top.x,m_pGroup->Top.y);
}

CPoint2D<int> CPixelGroup::GetBottom() const
{
	return CPoint2D<int>(m_pGroup->Bottom.x,m_pGroup->Bottom.y);
}

CPoint2D<int> CPixelGroup::GetLeft() const
{
	return CPoint2D<int>(m_pGroup->Left.x,m_pGroup->Left.y);
}

CPoint2D<int> CPixelGroup::GetRight() const
{
	return CPoint2D<int>(m_pGroup->Right.x,m_pGroup->Right.y);
}

COLORTYPE CPixelGroup::GetColorType() const
{
	return m_pGroup->Color;
}

void CPixelGroup::SetColorType(COLORTYPE Color)
{
	m_pGroup->Color=Color;
}

bool CPixelGroup::ColorsAvailable() const
{
	if (m_pGroup->pColor!=NULL)
		return true;
	else
		return false;
}

const TPixelGroup* CPixelGroup::GetConstTPixelGroupPtr() const
{
	return m_pGroup;
}


TPixelGroup* CPixelGroup::GetTPixelGroupPtr()
{
	return m_pGroup;
}

void CPixelGroup::MoveTPixelGroupPtr(TPixelGroup* p)
{
	m_pGroup=p;
}

bool CPixelGroup::InsertPosition(INT16 x, INT16 y, unsigned int Index)
{
	bool returnValue=k_InsertPosInGroup(x,y,Index,m_pGroup);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CPixelGroup::InsertPosition() Failed inserting position" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return returnValue;
}

bool CPixelGroup::InsertPosition(CPoint2D<INT16> Pos, unsigned int Index)
{
	bool returnValue=k_InsertPosInGroup(Pos.GetX(),Pos.GetY(),Index,m_pGroup);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CPixelGroup::InsertPosition() Failed inserting position" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return returnValue;
}

bool CPixelGroup::InsertPositionColor(INT16 x, INT16 y, UINT32 Color, unsigned int Index)
{
	bool returnValue=k_InsertPosColorInGroup(x,y,Color,Index,m_pGroup);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CPixelGroup::InsertPositionColor() Failed inserting position" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return returnValue;
}

bool CPixelGroup::InsertPositionColor(const CPoint2D<INT16>& Pos, UINT32 Color, unsigned int Index)
{
	bool returnValue=k_InsertPosColorInGroup(Pos.GetX(),Pos.GetY(),Color,Index,m_pGroup);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CPixelGroup::InsertPositionColor() Failed inserting position" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return returnValue;
}

bool CPixelGroup::RemovePosition(unsigned int Index)
{
	bool returnValue=k_RemovePosFromGroup(Index,m_pGroup);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CPixelGroup::RemovePosition() Failed removing position" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return returnValue;
}

bool CPixelGroup::RemovePositionSlow(unsigned int Index)
{
	bool returnValue=k_RemovePosFromGroupSlow(Index,m_pGroup);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CPixelGroup::RemovePositionSlow() Failed removing position" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return returnValue;
}

unsigned int CPixelGroup::RemoveDuplicates()
{
	return k_RemoveDuplicatesFromGroup(m_pGroup);
}

void CPixelGroup::ReAllocPositions(unsigned int NewSize)
{
	k_ReAllocGroup(m_pGroup,NewSize);
}

void CPixelGroup::AdjustSizeOfPositions()
{
	k_AdjustSizeOfGroup(m_pGroup);
}

bool CPixelGroup::AllocColors()
{
	bool returnValue=k_AllocColorsGroup(m_pGroup);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CPixelGroup::AllocColors() Failed allocating memory for color values or its allready allocated" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return returnValue;
}

bool CPixelGroup::AllocAndInitColors(UINT32 InitialValue)
{
	bool returnValue=k_AllocAndInitColorsGroup(InitialValue,m_pGroup);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CPixelGroup::AllocAndInitColors() Failed allocating memory for color values or its allready allocated" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return returnValue;
}

bool CPixelGroup::AddColors(const CStdImage& Img)
{
	bool returnValue=k_AddColorsToGroup(Img.GetConstTImagePtr(),m_pGroup);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CPixelGroup::AddColors() Failed adding colors" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return returnValue;
}

void CPixelGroup::RemoveColors()
{
	k_RemoveColors(m_pGroup);
}

bool CPixelGroup::CopyToImage(const CPalette* pPalette, UINT32 BackGround, CStdImage& Dest) const
{
	bool returnValue;
	if (pPalette!=NULL)
		returnValue=k_CopyGroupToImage(pPalette->GetPalettePtr(), BackGround,Dest.GetTImagePtr(),m_pGroup);
	else
		returnValue=k_CopyGroupToImage(NULL, BackGround,Dest.GetTImagePtr(),m_pGroup);

	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CPixelGroup::CopyToImage() Failed creating image" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return returnValue;
}

bool CPixelGroup::CopyToImage(UINT32 BackGround, UINT32 ForeGround, UINT16 Bits, CStdImage& Dest) const
{
	bool returnValue;
	returnValue=k_CopyGroupToImageFixedColors(BackGround, ForeGround, Bits, Dest.GetTImagePtr(),m_pGroup);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CPixelGroup::CopyToImage() Failed creating image" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return returnValue;
}

bool CPixelGroup::AddToImage(CStdImage& Dest,UINT32 Color) const
{
	bool ReturnValue=k_AddGroupToImage(Dest.GetTImagePtr(), m_pGroup, Color);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CPixelGroup::AddToImage() Failed adding group to image" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return ReturnValue;
}

bool CPixelGroup::AddToImage(CStdImage& Dest) const
{
	bool ReturnValue=k_AddGroupColorsToImage(Dest.GetTImagePtr(), m_pGroup);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CPixelGroup::AddToImage() Failed adding group with colors to image" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	return ReturnValue;
}

bool CPixelGroup::PosInGroup(const CPoint2D<INT16>& Pos,unsigned int& Index) const
{	
	T2DInt p;
	p.x=Pos.GetX();
	p.y=Pos.GetY();	
	return k_PosInGroup(m_pGroup,p,&Index);
}

bool CPixelGroup::PosInGroup(INT16 x, INT16 y,unsigned int& Index) const
{	
	T2DInt p;
	p.x=x;
	p.y=y;	
	return k_PosInGroup(m_pGroup,p,&Index);
}

bool CPixelGroup::PosInGroup(const CPoint2D<INT16>& Pos) const
{	
	unsigned int Dummy;
	T2DInt p;
	p.x=Pos.GetX();
	p.y=Pos.GetY();	
	return k_PosInGroup(m_pGroup,p,&Dummy);
}

bool CPixelGroup::PosInGroup(INT16 x, INT16 y) const
{
	unsigned int Dummy;
	T2DInt p;
	p.x=x;
	p.y=y;	
	return k_PosInGroup(m_pGroup,p,&Dummy);
}

void CPixelGroup::PrintInfo(bool PrintPositions,unsigned int PosPrLine) const
{
	k_PrintGroupInfo(m_pGroup,PrintPositions,PosPrLine);
}

void CPixelGroup::GetInfo(ostream& ost, bool PrintPositions, unsigned int PosPrLine) const
{
	ost.setf(std::ios_base::boolalpha);
	ost << StreamIndent << "CPixelGroup  NumberOfPixels: " << m_pGroup->NumberOfPixels;
	if (GetTotalPositions()!=0)
	{
		ost << " Width: " << GetWidth() << " Height: " << GetHeight();
		ost << " ColorsAvailable: " << ColorsAvailable() << endl;
		if (PrintPositions==true)
		{
			CPixelGroup::ConstIterator It=Begin();
			unsigned int Count=1;
			ost << StreamIndent << " "; // begin line with a space
			if (ColorsAvailable()==false)
			{
				while(It!=End())
				{
					ost << "(" << It->x << "," << It->y << ") ";
					if ((Count++ % PosPrLine) == 0)
						ost << endl << StreamIndent << " ";
					++It;
				}
			}
			else // colors available
			{
				while(It!=End())
				{
					ost << "(" << It->x << "," << It->y << ") " << GetColor(Count-1) << " ";
					if ((Count++ % PosPrLine) == 0)
						ost << endl << StreamIndent << " ";
					++It;
				}
			}
		}
	}
}

void CPixelGroup::StreamSettings(unsigned int PosPrLine)
{
	m_PosPrLine=PosPrLine;
}

ostream& operator<<(ostream& s,const CPixelGroup& Group)
{
	Group.GetInfo(s, true, Group.m_PosPrLine);
	return s;
}

std::istream& operator>>(std::istream& is, CPixelGroup& Group)
{
	is.setf(std::ios_base::boolalpha);
	string Token;
	ipl::SkipSpaceAndComments(is);
	// read 'CPixelGroup' token
	is >> Token;
	if (Token!=string("CPixelGroup"))
	{
		ostringstream ost;
		ost << "operator>>(istream, CPixelGroup) Token CPixelGroup not found"
			<< IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return is;
	}
	unsigned int NumberOfPixels;
	bool ColorsAvailable;
	unsigned int Dummy;
	ipl::SkipSpaceAndComments(is);
	// skip 'NumberOfPixels:' token
	is.ignore(std::numeric_limits<int>::max(),std::char_traits<char>::to_int_type(':'));
	ipl::SkipSpaceAndComments(is);
	is >> NumberOfPixels;
	ipl::SkipSpaceAndComments(is);
	// skip 'Width:' token
	is.ignore(std::numeric_limits<int>::max(),std::char_traits<char>::to_int_type(':'));
	ipl::SkipSpaceAndComments(is);
	is >> Dummy;
	// skip 'Height:' token
	is.ignore(std::numeric_limits<int>::max(),std::char_traits<char>::to_int_type(':'));
	ipl::SkipSpaceAndComments(is);
	is >> Dummy;
	ipl::SkipSpaceAndComments(is);
	// skip 'ColorsAvailable:' token
	is.ignore(std::numeric_limits<int>::max(),std::char_traits<char>::to_int_type(':'));
	ipl::SkipSpaceAndComments(is);
	is >> ColorsAvailable;
	Group.Empty();
	Group.ReAllocPositions(NumberOfPixels);
	CPoint2D<INT16> P;
	if (ColorsAvailable==false)
	{
		for(int i=0; i<(int)NumberOfPixels; i++)
		{
			is >> P;
			Group.AddPositionFast(P);
		}
	}
	else // colors available
	{
		Group.AllocColors();
		UINT32 Color;
		for(int i=0; i<(int)NumberOfPixels; i++)
		{
			is >> P;
			Group.AddPositionFast(P);
			is >> Color;
			Group.SetColor(Color,i);
		}
	}
	Group.UpdateBoundaries();
	return is;			
}

void CPixelGroup::UpdateBoundaries()
{
	k_GroupUpdateBoundaries(m_pGroup);
}

bool CPixelGroup::AddOffset(const CPoint2D<INT16>& Offset)
{
	T2DInt p;
	p.x=Offset.GetX();
	p.y=Offset.GetY();	
	return k_GroupAddOffset(m_pGroup,p);
}

} // end ipl namespace
