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

#include "ipl98_general_functions.h"
#include "../kernel_c/kernel_io.h"
#include "error.h"
#include <sstream>
#include <stdarg.h>
#include <cstring>
#include <vector>

namespace ipl{ // namespace ipl ~ General Functions

using std::ostringstream;
using std::vector;

istream& operator<<(bool b, istream& s)
{
	s >> b;
	return s;
}

bool SplitFileName(const char* pFilePathName, string& Path, string& FileNameExt, string& Ext)
{
	bool ReturnValue=true;
	char *pPath=NULL, *pFileNameExt=NULL, *pExt=NULL;
	if (k_SplitFileName(pFilePathName,&pPath,&pFileNameExt,&pExt)==true)
	{
		Path.assign(pPath);
		FileNameExt.assign(pFileNameExt);
		Ext.assign(pExt);
	}
	else
	{
		ostringstream ost;
		ost << "SplitFileName() Failed splitting filename" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		ReturnValue=false;
	}
	if (pPath!=NULL)
		free(pPath);
	if (pFileNameExt!=NULL)
		free(pFileNameExt);
	if (pExt!=NULL)
		free(pExt);
	return ReturnValue;
}

bool StripFileExtension(string& PathFileName)
{
	string::size_type index=PathFileName.find_last_of('.');
	if (index==string::npos)
		return false;
	PathFileName=PathFileName.substr(0,index);
	return true;
}

bool ConvertBackslashToSlash(char* Path)
{
	string buffer = Path;
	bool ok = ConvertBackslashToSlash(buffer);
	strcpy(Path, buffer.c_str());
	return ok;
}

bool ConvertBackslashToSlash(string& Path)
{
	for (string::iterator p = Path.begin(); p != Path.end(); ++p)
		if (*p == '\\')
			*p = '/';
	return true;
}

bool AddTrailingSlashToPath(string& Path)
{
	if (Path.size()==0)
		return false;
	if (Path.find_last_of('/')!=(Path.size()-1))
		Path += "/";
	return true;
}

string StreamIndentStr;
vector<unsigned short> StreamIndentList;

void StreamIndentAdd(unsigned short Spaces)
{
	StreamIndentStr.append(Spaces,' ');
	StreamIndentList.push_back(Spaces);
}

void StreamIndentRemoveLast() throw(string)
{
	if ((StreamIndentList.size()==0) ||
		((StreamIndentStr.size()-StreamIndentList.back())<0))
	{
		ostringstream ost;
		ost << "ipl::StreamIndentRemoveLast() Cannot remove last indent" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		throw(ost.str());
	}
	StreamIndentStr.resize(StreamIndentStr.size()-StreamIndentList.back());
	StreamIndentList.pop_back();
}

void StreamIndentRemoveAll()
{
	StreamIndentStr.clear();
	StreamIndentList.resize(0);
}

ostream& StreamIndent(ostream& os)
{
	return os << StreamIndentStr;
}

int CompareNoCase(const string& s, const string& s2)
{
	string::const_iterator p=s.begin();
	string::const_iterator p2=s2.begin();
	while (p!=s.end() && p2!=s2.end())
	{
		if (toupper(*p)!=toupper(*p2)) return (toupper(*p)<toupper(*p2))? -1 : 1;
		++p;
		++p2;
	}
	return (s2.size()==s.size()) ? 0 : (s.size()<s2.size()) ? -1 : 1; // size is unsigned
}

string ConvertLinesToComments(string& Str)
{
	ostringstream ost;
	int Pos;
	while((Pos=Str.find_first_of("\n"))!=-1)
	{
		ost << "# " << Str.substr(0,Pos) << endl;
		Str=Str.substr(Pos+1,Str.size()-Pos-1);
	}
	return ost.str();
}

} // end namespace ipl
