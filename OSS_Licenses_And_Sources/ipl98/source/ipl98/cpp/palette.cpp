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

#include "palette.h"
#include "../kernel_c/kernel_palette.h"
#include "ipl98_general_functions.h"
#include "error.h"
#include <sstream>

namespace ipl{

using std::ostringstream;

CPalette::CPalette()
{
	m_pPal=(TPalette*) malloc(sizeof(TPalette));
	k_InitPalette(m_pPal);
	m_InternalTPalette=true;
}

CPalette::CPalette(UINT16 Entries)
{
	if ((Entries!=2) && (Entries!=16) && (Entries!=256))
	{
		ostringstream ost;
		ost << "CPalette::CPalette(" << Entries << ") Number of entris must be 2, 16 or 256" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	m_pPal=(TPalette*) malloc(sizeof(TPalette));
	k_InitPalette(m_pPal);
	switch(Entries)
	{
	case 2:
		k_CreatePal(1,m_pPal);
		break;
	case 16:
		k_CreatePal(4,m_pPal);
		break;
	case 256:
		k_CreatePal(8,m_pPal);
		break;
	}
	m_InternalTPalette=true;
}

CPalette::CPalette(TPalette* pPal)
{
	if (pPal!=NULL)
	{
		m_pPal=pPal;
		m_InternalTPalette=false;
		return;
	}
	else
	{
		ostringstream ost;
		ost << "CPalette::CPalette(TPalette* pPal) pPal is a NULL-pointer" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
}

CPalette::~CPalette()
{
	if (m_InternalTPalette==true)
	{
		if (m_pPal!=NULL)
		{
			k_EmptyPalette(m_pPal);
			free(m_pPal);
		}
	}
}

CPalette::CPalette(const CPalette& Pal)
{
	// Copy constructor
	m_pPal=(TPalette*) malloc(sizeof(TPalette));
	k_InitPalette(m_pPal);
	m_InternalTPalette=true;
	*this=Pal;
}

CPalette& CPalette::operator=(const CPalette& SourcePal)
{
	k_CopyPalette(m_pPal,SourcePal.m_pPal);
	return *this;
}

bool CPalette::SetPalettePtr(TPalette* pPal)
{
	if (pPal==NULL)
	{
		return false;
	}
	m_pPal=pPal;
	return true;
}

TPalette* CPalette::GetPalettePtr() const
{
	// returns pointer to m_ptPal
	return m_pPal;
}

UINT16 CPalette::GetNumberOfEntries() const
{
	return m_pPal->PalEntries;
}

void CPalette::CopyPalette(const CPalette& Pal)
{
	k_CopyPalette(m_pPal,Pal.m_pPal);
}

bool CPalette::IsGrayTone()
{
	return k_IsPaletteGrayTone(m_pPal);
}

void CPalette::PrintInfo(unsigned int EntriesPrLine)
{
	k_PrintPaletteInfo(m_pPal,EntriesPrLine);
}

ostream& operator<<(ostream& s,const CPalette& Pal)
{
	unsigned int EntriesPrLine=4;
	unsigned int count1,count2;
	UINT32 PalValue;
	s << StreamIndent << "*************** Palette info *******************" << endl;
	s << StreamIndent << " Total entries: " << Pal.GetNumberOfEntries() << endl;
	s << StreamIndent << " Values: Index=\"R G B\"" << endl;
	for(count1=0; count1<Pal.GetNumberOfEntries()/EntriesPrLine; count1++)
	{
		for(count2=0; count2<EntriesPrLine; count2++)
		{
			Pal.GetColor(count1*EntriesPrLine+count2,PalValue);
			s << StreamIndent << count1*EntriesPrLine+count2 << "=\"" << (short)CPalette::GetRedVal(PalValue) <<
				" " << (short)CPalette::GetGreenVal(PalValue) << " " << 
				(short)CPalette::GetBlueVal(PalValue) << "\" ";
		}
		s << endl;
	}
	return s;
}

} // end ipl namespace
