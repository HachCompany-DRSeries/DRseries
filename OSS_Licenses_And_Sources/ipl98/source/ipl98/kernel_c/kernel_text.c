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

#include "kernel_text.h"
#include "kernel_error.h"
#include "../../time/kernel_c/kernel_time_date.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/************************************************/
/********        Private functions       ********/
/************************************************/

void k_ReAllocText(TIplText* pText,unsigned int NewSize);
/* returns true if given character if found in the string pStr */
bool k_ScanForCharacterText(const char* pStr,const char ch);
/* deletes all contents in a TIplText structure and initializes it so it can be used again */
void k_DeleteText(TIplText* pText);

/************************************************/
/********        Public functions        ********/
/************************************************/

void k_InitText(TIplText* pText)
{
	k_InitFileInfo(&(pText->FileInfo));
	pText->AppendTime=true;
	pText->AppendDate=true;
	pText->FindPosition=0;
	k_InitString(&(pText->SearchStr));
	pText->NumberOfLines=0;
	pText->AllocatedLines=0; /* internal use */
	pText->pStringList=NULL;
}

void k_EmptyText(TIplText* pText)
{
	unsigned int count;
	for(count=0; count<pText->AllocatedLines;count++)
	{
		k_EmptyString(&(pText->pStringList[count]));
	}
	if (pText->pStringList!=NULL)
	{
		free(pText->pStringList);
	}
	pText->FindPosition=0;
	pText->NumberOfLines=0;
	pText->AllocatedLines=0; /* internal use */
	pText->pStringList=NULL;
	k_EmptyFileInfo(&(pText->FileInfo));
	k_EmptyString(&(pText->SearchStr));
}

bool k_AppendText(TIplText* pText,const char* pStr)
{
	TString InsertStr; /* resulting string to be inserted */
	char* Temp=NULL;
	clock_t td; /* date functionality */
	if (k_ScanForCharacterText(pStr,'\n')==true)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"CText::k_AppendText() String argument=\"%s\" contains "
			"a newline character - no insertion",
			pStr);
		return false;
	}
	k_InitString(&InsertStr);
	k_AddCharArrayToString ("# ",&InsertStr);

	if(pText->AppendDate==true)
	{
		td=k_GetDate(&Temp, false);
		if (td==-1)
		{
			k_AddCharArrayToString("date not available ",&InsertStr);
		}
		else
		{
			k_AddCharArrayToString(Temp,&InsertStr);
			k_AddCharToString(' ',&InsertStr);
			free(Temp);
			Temp=NULL;
		}

	}
	if(pText->AppendTime==true)
	{
		td=k_GetTime(&Temp);
		if (td==-1)
		{
			k_AddCharArrayToString("time not available ",&InsertStr);
		}
		else
		{
			k_AddCharArrayToString(Temp,&InsertStr);
			k_AddCharToString(' ',&InsertStr);
			free(Temp);
			Temp=NULL;
		}
	}

	k_AddCharArrayToString(pStr,&InsertStr);
	k_AddCharToString('\n',&InsertStr);
	if (pText->NumberOfLines==pText->AllocatedLines)
	{
		k_ReAllocText(pText,2*pText->NumberOfLines);
	}
	k_CopyString(&(pText->pStringList[pText->NumberOfLines]),&InsertStr);
	pText->NumberOfLines++;
	k_EmptyString(&InsertStr);
	return true;
}

bool k_AppendTextIPL(TIplText* pText,const char* pStr)
{
	bool ReturnValue;
	TString StrTemp;
	char* Temp=(char*)malloc(50*sizeof(char));
	k_InitString(&StrTemp);
	sprintf(Temp,"IPL98 Version %.2f ",IPL98_VERSION);
	k_AddCharArrayToString(Temp,&StrTemp);
	k_AddCharArrayToString(pStr,&StrTemp);
	free(Temp);
	ReturnValue=k_AppendText(pText,StrTemp.pChars);
	k_EmptyString(&StrTemp);
	return ReturnValue;
}

bool k_PrintfAppendTextIPL(TIplText* pDest, const char* format, ...)
{
	int cnt;
	bool result;
	va_list args;
	const int MAX_STRING_SIZE = 4000;

	char* buffer = (char*)malloc(sizeof(char) * MAX_STRING_SIZE);
	assert(buffer);

	va_start(args, format);
	cnt = vsprintf(buffer, format, args);
	va_end(args);

	if (cnt <= -1 || MAX_STRING_SIZE <= cnt)
	{
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_PrintfAppendIplText() Buffer formatting error");
		free(buffer);
		return false;
	}

	result = k_AppendTextIPL(pDest, buffer);
	free(buffer);
	return result;
}

bool k_AppendCPArgumentsText(TIplText* pText,int argc, const char** ppArgv)
{
	int count;
	TString InsertStr; /* resulting string to be inserted */
	char* Temp=NULL;
	clock_t td; /* date functionality */
	if (ppArgv==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_AppendCPArgumentsText: ppArgv is a NULL pointer");
		return false;
	}
	k_InitString(&InsertStr);
	k_AddCharArrayToString ("# ",&InsertStr);

	if(pText->AppendDate==true)
	{
		td=k_GetDate(&Temp, false);
		if (td==-1)
		{
			k_AddCharArrayToString("date not available ",&InsertStr);
		}
		else
		{
			k_AddCharArrayToString(Temp,&InsertStr);
			k_AddCharToString(' ',&InsertStr);
			free(Temp);
			Temp=NULL;
		}

	}
	if(pText->AppendTime==true)
	{
		td=k_GetTime(&Temp);
		if (td==-1)
		{
			k_AddCharArrayToString("time not available ",&InsertStr);
		}
		else
		{
			k_AddCharArrayToString(Temp,&InsertStr);
			k_AddCharToString(' ',&InsertStr);
			free(Temp);
			Temp=NULL;
		}
	}

	k_AddCharArrayToString("Command prompt arg's:",&InsertStr);
	for(count=0; count<argc; count++)
	{
		k_AddCharArrayToString(ppArgv[count],&InsertStr);
		k_AddCharToString(' ',&InsertStr);
	}	
	k_AddCharToString('\n',&InsertStr);
	if (pText->NumberOfLines==pText->AllocatedLines)
	{
		k_ReAllocText(pText,2*pText->NumberOfLines);
	}
	k_CopyString(&(pText->pStringList[pText->NumberOfLines]),&InsertStr);
	pText->NumberOfLines++;
	k_EmptyString(&InsertStr);
	return true;
}

bool k_CopyText(TIplText* pDest,const TIplText* pSource)
{
	if (pDest==pSource)
	{
		/* cannot copy to itself! */
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_CopyText() Source and destination text the same - doing nothing");
		return false;
	}
	k_EmptyText(pDest);
	if (pSource->NumberOfLines!=0)
	{
		unsigned count;
		for(count=0;count<pSource->NumberOfLines;count++)
		{
			if (pDest->NumberOfLines==pDest->AllocatedLines)
			{
				k_ReAllocText(pDest,2*pDest->NumberOfLines);
			}
			k_CopyString(&(pDest->pStringList[pDest->NumberOfLines]),&(pSource->pStringList[count]));
			pDest->NumberOfLines++;
		}
	}
	return true;
}

bool k_FindText(TIplText* pText, const char* pStr, TString* pResult)
{
	bool Found=false;
	k_DeleteString(&(pText->SearchStr));
	k_AddCharArrayToString(pStr,&(pText->SearchStr));
	while((Found==false) && (pText->FindPosition<pText->NumberOfLines))
	{
		if (k_FindInString(pStr,&(pText->pStringList[pText->FindPosition]))!=NULL)
		{
			printf("Found item in line %ld\n",pText->FindPosition);
			k_CopyString(pResult,&(pText->pStringList[pText->FindPosition]));
			Found=true;
		}
		pText->FindPosition++;
	}
	return Found;
}

bool k_FindNextText(TIplText* pText, TString* pResult)
{
	bool Found=false;
	if (pText->SearchStr.NumberOfChars==0)
	{
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_FindNextText(): No search string present (call k_FindText() first)");
		return false;
	}
	while((Found==false) && (pText->FindPosition<pText->NumberOfLines))
	{
		if (k_FindInString(pText->SearchStr.pChars,&(pText->pStringList[pText->FindPosition]))!=NULL)
		{
			printf("Found item in line %ld\n",pText->FindPosition);
			k_CopyString(pResult,&(pText->pStringList[pText->FindPosition]));
			Found=true;
		}
		pText->FindPosition++;
	}
	return Found;
}


bool k_GetItemText(const TIplText* pText,unsigned long Index,TString* pResult)
{
	if(Index>=pText->NumberOfLines)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_GetItemText(): Index(%d) out of range",
			Index);
		return false;
	}
	else
	{
		k_CopyString(pResult,&(pText->pStringList[Index]));
		return true;
	}
}


void k_EmptyLeaveOneLineText(TIplText* pText)
{
	if (pText->NumberOfLines>0)
	{
		TString SearchStr; /* the search string */
		TString Str; /* the new string to be inserted */
		char* Temp=NULL;
		unsigned int TotLinesRem=0; /* sum the total number of lines removed */
		k_InitString(&Str);
		k_InitString(&SearchStr);
		k_AddCharArrayToString("k_EmptyLeaveOneLineText()",&SearchStr); /* search string defined here */
		/* check for the precense of a previous call to k_EmptyLeaveOneLineText()
		   The text "k_EmptyLeaveOneLineText()" is always in the first line (if present!) */
		Temp=k_FindInString(SearchStr.pChars,&(pText->pStringList[0]));
		if (Temp!=NULL)
		{
			/* advance the Temp pointer to just after the search string */
			Temp+=SearchStr.NumberOfChars;
			/* TotLinesRem is updated with the number found at Temp's position in first line */
			sscanf(Temp,"%d",&TotLinesRem);
			TotLinesRem+=pText->NumberOfLines-1;
		}
		else
		{
			TotLinesRem+=pText->NumberOfLines;
		}
		k_AddCharArrayToString("k_EmptyLeaveOneLineText() ",&Str);
		Temp=(char*)malloc(50*sizeof(char));
		sprintf(Temp,"%d ",TotLinesRem);
		k_AddCharArrayToString(Temp,&Str);
		free(Temp);
		k_DeleteText(pText);	
		k_AddCharArrayToString("lines removed in total",&Str);
		k_AppendText(pText,Str.pChars);
		k_EmptyString(&Str);
		k_EmptyString(&SearchStr);
	}
}


bool k_LoadText(TIplText* pText, const char* pFileName)
{
	FILE* fp=fopen(pFileName,"rb"); /* open file */
	if (!fp)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_LoadText() Failed loading history file %s",
			pFileName);
		return false;
	}
	else
	{
		const int LineSize=400;
		char* pLine=(char*)malloc(LineSize*sizeof(char));
		k_DeleteText(pText);
		while(fgets(pLine,LineSize-1,fp)!=NULL)
		{
			if (pLine[0]!='#')
			{
				k_ShowMessageStd(
					IPL_POSITION, IPL_ERROR,
					"k_LoadText() Wrong format in history file (one "
					"of the lines contains more than %d characters or a '#' character is "
					"missing in the beginning of a line)",
					LineSize);
				fclose(fp);
				free(pLine);
				k_DeleteText(pText);
				return false;
			}
			if (pText->NumberOfLines==pText->AllocatedLines)
			{
				k_ReAllocText(pText,2*pText->NumberOfLines);
			}
			k_AddCharArrayToString(pLine,&(pText->pStringList[pText->NumberOfLines]));
			pText->NumberOfLines++;
		}
		free(pLine);
		return true;
	}
}


bool k_SaveText(TIplText* pText, const char* pFileName)
{
	FILE* fp;
	TString StrFileName;
	k_InitString(&StrFileName);
	if (pFileName==NULL)
	{
		if (pText->FileInfo.PathAndFileName==NULL)
		{
			k_ShowMessageSimple(
				IPL_ERROR,
				"k_SaveText() Both the pFileName and FileInfo "
				"(in the TIplText structure) are empty");
			return false;
		}
		else
		{
			k_AddCharArrayToString(pText->FileInfo.PathAndFileName,&StrFileName);
		}
	}
	else
	{
		k_AddCharArrayToString(pFileName,&StrFileName);
	}
	fp=fopen(StrFileName.pChars,"wb"); /* open file */
	if (!fp)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_SaveText() Failed saving history file %s",
			pFileName);
		return false;
	}
	else
	{
		unsigned int count;
		for(count=0;count<pText->NumberOfLines;count++)
		{
			fputs(pText->pStringList[count].pChars,fp);
		}
		fclose(fp);
		k_EmptyString(&StrFileName);
		return true;
	}
}


bool k_SetFileInfoFromPathFileText(TIplText* pText, const char* pPathAndFileName)
{
	char* Ext=NULL;
	char* Temp;
	TString PathAndFileName;
	if (pPathAndFileName==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_SetFileInfoFromPathFileText() pPathAndFileName is a NULL pointer");
		return false;
	}
	/* empty file information before proceeding */
	k_EmptyFileInfo(&(pText->FileInfo));
	/* change extension to txt */
	k_InitString(&PathAndFileName);
	k_AddCharArrayToString(pPathAndFileName,&PathAndFileName);
	Temp=PathAndFileName.pChars+PathAndFileName.NumberOfChars;
	while((*Temp!='.') && (Temp!=PathAndFileName.pChars))
	{
		Temp--;
	}
	if ((Temp==PathAndFileName.pChars) && (*Temp!='.'))
	{
		/* no extension found, just append a txt extension */
		k_AddCharArrayToString(".txt",&PathAndFileName);
	}
	else
	{
		/* replace old extension with txt */
		PathAndFileName.NumberOfChars-=((PathAndFileName.pChars+PathAndFileName.NumberOfChars)-Temp);
		k_AddCharArrayToString(".txt",&PathAndFileName);
	}

	if (k_SplitFileName(PathAndFileName.pChars,&(pText->FileInfo.PathName),&(pText->FileInfo.FileName),&Ext)==false)
	{
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_SetFileInfoFromPathFileText() Failed setting variables in TFileInfo structure");

		k_EmptyString(&PathAndFileName);
		if (Ext!=NULL)
		{
			free(Ext);
		}
		return false;
	}
	pText->FileInfo.PathAndFileName=(char *)malloc(PathAndFileName.NumberOfChars+1);
	strcpy(pText->FileInfo.PathAndFileName,PathAndFileName.pChars);
	k_EmptyString(&PathAndFileName);
	if (Ext!=NULL)
	{
		free(Ext);
	}
	return true;
}


void k_PrintDataText(TIplText* pText)
{
	unsigned int count;
	printf("**************** TIplText info ********************\n");
	for(count=0;count<pText->NumberOfLines;count++)
	{
		printf("line %2d: %s",count,pText->pStringList[count].pChars);
	}
	printf("\n");
}


void k_ListToCharArrayText(const TIplText* pText,char** ppHistory)
{
	unsigned long count;
	TString Temp;
	k_InitString(&Temp);
	for(count=0; count < pText->NumberOfLines; count++)
	{
		k_AddCharArrayToString(pText->pStringList[count].pChars,&Temp);
	}
	*ppHistory = (char*)malloc((Temp.NumberOfChars+1)*sizeof(char));
	memcpy((*ppHistory),Temp.pChars,Temp.NumberOfChars+1);
	k_EmptyString(&Temp);
}


void k_CharArrayToListText(TIplText* pText,const char* pHistory)
{
	if (pHistory!=NULL)
	{
		TString Str;
		char* tempStr=(char*)malloc(strlen(pHistory)+1);
		const char* Temp=pHistory;
		unsigned int count,ScannedChars=0;
		k_InitString(&Str);
		while((size_t)(Temp-pHistory)<strlen(pHistory))
		{
			count=0;
			do 
			{
				tempStr[count]=*Temp;
				count++;
				ScannedChars++;
				Temp++;
			} while (*Temp != '\n');
			Temp++;
			tempStr[count]='\0';
			k_AddCharArrayToString(tempStr,&Str);
			k_AddCharToString('\n',&Str);
			if (pText->NumberOfLines==pText->AllocatedLines)
			{
				k_ReAllocText(pText,2*pText->NumberOfLines);
			}
			k_CopyString(&(pText->pStringList[pText->NumberOfLines]),&Str);
			pText->NumberOfLines++;
			k_DeleteString(&Str);
		}
		free(tempStr);
		k_EmptyString(&Str);
	}
}

bool k_RemoveLastLineText(TIplText* pText)
{
	if (pText->NumberOfLines!=0)
	{
		pText->NumberOfLines--;
		return true;
	}
	else
	{
		return false;
	}
}

void k_AppendIplText(TIplText* pDest,const TIplText* pSource)
{
	unsigned int count;
	for(count=0; count<pSource->NumberOfLines;count++)
	{
		if (pDest->NumberOfLines==pDest->AllocatedLines)
		{
			k_ReAllocText(pDest,2*pDest->NumberOfLines);
		}
		k_CopyString(&(pDest->pStringList[pDest->NumberOfLines]),&pSource->pStringList[count]);
		pDest->NumberOfLines++;
	}
}

/************************************************/
/********        Private functions       ********/
/************************************************/

void k_ReAllocText(TIplText* pText,unsigned int NewSize)
{
	if(pText->AllocatedLines==0)
	{
		/* first time - allocate 4 lines */
		const unsigned int InitialAllocation=4;
		unsigned int count;
		pText->pStringList=(TString*)malloc(InitialAllocation*sizeof(TString));
		for(count=0;count<InitialAllocation;count++)
		{
			k_InitString(&(pText->pStringList[count]));
		}
		pText->AllocatedLines=InitialAllocation; /* internal use */
		return;

	}
	else if ((pText->pStringList=(TString*)realloc(pText->pStringList,NewSize*sizeof(TString)))!=NULL)
	{
		unsigned int count;
		for(count=pText->AllocatedLines; count<NewSize; count++)
		{
			k_InitString(&(pText->pStringList[count]));
		}
		pText->AllocatedLines=NewSize; /* internal use */
		return;
	}
	else
	{
		k_ShowMessageSimple(IPL_ERROR, "k_ReAllocText() Realloc failed");
	}
}

/* returns true if given character if found in the string pStr */
bool k_ScanForCharacterText(const char* pStr,const char ch)
{
	const char* c=pStr;
	while(*c!='\0')
	{
		if (*c==ch)
			return true;
		c++;
	}
	return false;
}

/* deletes all contents in a TIplText structure and initializes it so it can be used again */
void k_DeleteText(TIplText* pText)
{
	k_EmptyText(pText);
	k_InitText(pText);
}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
