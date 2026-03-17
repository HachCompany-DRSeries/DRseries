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

#include "text.h"
#include "error.h"
#include "../kernel_c/ipl98_types.h"
#include "../../time/time_date.h"
#include "ipl98_general_functions.h"
#include <iostream>
#include <sstream>
#include <assert.h>
#include <stdarg.h>

namespace ipl{

using std::cout;
using std::endl;
using std::ostringstream;

CText::CText()
{
	// default constructor
	m_pHistory=(TIplText*) malloc(sizeof(TIplText));
	k_InitText(m_pHistory);
	m_InternalTIplText=true;
}

CText::CText(TIplText* pHistory)
{
	if (pHistory!=NULL)
	{
		m_pHistory=pHistory;
		m_InternalTIplText=false;
		return;
	}
	else
	{
		ostringstream ost;
		ost << "CText::CText(TIplText* pHistory) pHistory is a NULL-pointer" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR, ost.str());
	}
}

CText::~CText()
{
	// default destructor
	if (m_InternalTIplText==true)
	{
		if (m_pHistory!=NULL)
		{
			k_EmptyText(m_pHistory);
			free(m_pHistory);
		}
	}
}

CText& CText::operator=(const CText& SourceHistory)
{
	k_CopyText(m_pHistory,SourceHistory.m_pHistory);
	return *this;
}

bool CText::SetTextPtr(TIplText* pHistory)
{
	if (pHistory==NULL)
	{
		return false;
	}
	m_pHistory=pHistory;
	return true;
}

TIplText* CText::GetTextPtr() const
{
	return m_pHistory;
}

bool CText::Append(const string& str)
{
	bool returnValue=k_AppendText(m_pHistory,str.c_str());
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CText::Append() Append failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR, ost.str());
	}
	return returnValue;
}

bool CText::AppendIPL(const string& str)
{
	return AppendIPL(str.c_str());
}

bool CText::AppendIPL(const char* pCharArray)
{
	if (pCharArray==NULL)
	{
		ostringstream ost;
		ost << "CText::AppendIPL(const char* pCharArray) Append failed, pCharArray is a NULL pointer" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR, ost.str());
		return false;
	}
	bool returnValue=k_AppendTextIPL(m_pHistory,pCharArray);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CText::AppendIPL() Append failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR, ost.str());
	}
	return returnValue;
}

bool CText::PrintfAppendIPL(const char* format, ...)
{
	va_list args;

	const int MAX_STRING_SIZE = 4000;

	char* buffer = (char*)malloc(sizeof(char) * MAX_STRING_SIZE);
	assert(buffer);

	va_start(args, format);
	int cnt = vsprintf(buffer, format, args);
	va_end(args);

	if (cnt <= -1 || MAX_STRING_SIZE <= cnt)
	{
		ostringstream ost;
		ost << "CText::AppendIPL() Buffer formatting error" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR, ost);
		free(buffer);
		return false;
	}

	bool result = AppendIPL(buffer);
	free(buffer);
	return result;
}

bool CText::AppendCPArguments(int argc, const char** argv)
{
	bool returnValue=k_AppendCPArgumentsText(m_pHistory,argc,argv);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CText::AppendCPArguments() Append failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR, ost.str());
	}
	return returnValue;
}

bool CText::Find(const string& str, string& Result)
{
	TString InternalResult;
	k_InitString(&InternalResult);
	if (k_FindText(m_pHistory,str.c_str(),&InternalResult)==true)
	{
		/* copy internal TString representation to a C++ std string */
		Result.assign(InternalResult.pChars);
		k_EmptyString(&InternalResult);
		return true;
	}
	else
	{
		k_EmptyString(&InternalResult);
		return false;
	}
}


bool CText::FindNext(string& Result)
{
	TString InternalResult;
	k_InitString(&InternalResult);
	bool returnValue=k_FindNextText(m_pHistory,&InternalResult);
	if (m_pHistory->SearchStr.NumberOfChars==0)
	{
		ostringstream ost;
		ost << "CText::FindNext() No search string present - call Find() first!" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR, ost.str());
	    k_EmptyString(&InternalResult);
		return false;
	}
	else
	{
		/* copy internal TString representation to a C++ std string */
		Result.assign(InternalResult.pChars);
		k_EmptyString(&InternalResult);
		return returnValue;
	}
}


bool CText::GetItem(unsigned long Index,string& Result) const
{
	TString InternalResult;
	k_InitString(&InternalResult);
	bool returnValue=k_GetItemText(m_pHistory,Index,&InternalResult);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CText::GetItem() Failed!" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR, ost.str());
	}
	else
	{
		/* copy internal TString representation to a C++ std string */
		Result.assign(InternalResult.pChars);
		k_EmptyString(&InternalResult);
	}
	return returnValue;
}


unsigned long CText::TotalItems() const
{
	return m_pHistory->NumberOfLines;
}


bool CText::AddTime(bool Value)
{
	bool ReturnValue=m_pHistory->AppendTime;
	m_pHistory->AppendTime=Value;
	return ReturnValue;
}


bool CText::AddDate(bool Value)
{
	bool ReturnValue=m_pHistory->AppendTime;
	m_pHistory->AppendDate=Value;
	return ReturnValue;
}


void CText::Empty()
{
	k_EmptyLeaveOneLineText(m_pHistory);
}


bool CText::Load(const char* pFileName)
{
	bool returnValue=k_LoadText(m_pHistory,pFileName);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CText::Load() Load failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR, ost.str());
	}
	return returnValue;
}


bool CText::Save(const char* pFileName)
{
	bool returnValue=k_SaveText(m_pHistory,pFileName);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CText::Save() Save failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR, ost.str());
	}
	return returnValue;
}

bool CText::SetFileInfoFromPathFile(const char* pPathAndFileName)
{
	bool returnValue=k_SetFileInfoFromPathFileText(m_pHistory,pPathAndFileName);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CText::SetFileInfoFromPathFile() Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR, ost.str());
	}
	return returnValue;
}



void CText::PrintData()
{
	k_PrintDataText(m_pHistory);
}

ostream& operator<<(ostream& s,CText& Txt)
{
	TIplText* pText=Txt.m_pHistory;
	unsigned int count;
	cout << StreamIndent << "**************** CText info ********************" << endl;
	for(count=0;count<pText->NumberOfLines;count++)
	{
		cout << "line " << count << ": " << pText->pStringList[count].pChars;
	}
	cout << endl;

	return s;
}

void CText::ListToCharArray(char** History) const
{
	k_ListToCharArrayText(m_pHistory,History);
}

void CText::CharArrayToList(const char* pHistory)
{
	k_CharArrayToListText(m_pHistory,pHistory);
}

bool CText::RemoveLastLine()
{
	return k_RemoveLastLineText(m_pHistory);
}

void CText::AppendCText(const CText& Source)
{
	k_AppendIplText(m_pHistory,Source.GetTextPtr());
}

} // end ipl namespace
