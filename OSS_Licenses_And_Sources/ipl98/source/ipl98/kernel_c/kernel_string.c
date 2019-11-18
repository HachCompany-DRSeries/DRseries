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

#include "kernel_string.h"
#include "kernel_error.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

void k_InitString(TString* pString)
{
	pString->pChars=(char*)malloc(sizeof(char));
	pString->pChars[0]='\0';
	pString->NumberOfChars=0;
	pString->AllocatedChars=1; /* internal use */
}

/*	Deletes all allocated memory for the string */
void k_EmptyString(TString* pString)
{
	if (pString->pChars!=NULL)
	{
		free(pString->pChars);
		pString->pChars=NULL;
	}
	pString->NumberOfChars=0;
	pString->AllocatedChars=0; /* internal use */
}

void k_DeleteString(TString* pString)
{
	if (pString->pChars!=NULL)
		free(pString->pChars);
	k_InitString(pString);
}

void k_AllocString(TString* pString,unsigned int InitialSize)
{
	k_EmptyString(pString);
	if (InitialSize==0)
		InitialSize=1; /* minimum one allocated char */
	InitialSize++; /* allocate space for the ending '\0' character */
	pString->pChars=(char*)malloc(InitialSize*sizeof(char));
	pString->pChars[0]='\0'; /* start with an empty string */
	pString->NumberOfChars=0; /* just in case! */
	pString->AllocatedChars=InitialSize; /* internal use */
}

void k_ReAllocString(TString* pString,unsigned int NewSize)
{
	if (pString->pChars==NULL)
		k_InitString(pString);

	if ((pString->pChars=(char*)realloc(pString->pChars,(++NewSize)*sizeof(char)))!=NULL)
	{
		pString->AllocatedChars=NewSize; /* internal use */
	}
	else
	{
		k_ShowMessageSimple(IPL_ERROR, "k_ReAllocString() Realloc failed");
	}
}

void k_AdjustSizeOfString(TString* pString)
{
	pString->pChars=(char*)realloc(pString->pChars,pString->NumberOfChars*sizeof(char));
	pString->AllocatedChars=pString->NumberOfChars;
}

void k_AddCharToString(char Character,TString* pString)
{
	if (pString->pChars==NULL)
		k_InitString(pString);

	if (pString->NumberOfChars==(pString->AllocatedChars-1))
		/* the -1 to take into account the allocated '\0' character */
	{
		/* allocate 2 times previous allocated memory or 2 if memory never allocated before */
		pString->AllocatedChars = 
			(pString->AllocatedChars==1) ? 2 : 2*(pString->AllocatedChars-1);

		k_ReAllocString(pString,pString->AllocatedChars);
	}
	/* insert character and increment total characters variable */
	pString->pChars[pString->NumberOfChars] = Character;
	pString->pChars[pString->NumberOfChars+1] = '\0';
	pString->NumberOfChars++;
}

void k_AddCharArrayToString(const char* pCharArray,TString* pString)
{
	unsigned int length=strlen(pCharArray);
	if (pString->pChars==NULL)
		k_InitString(pString);

	if ((pString->NumberOfChars+length)>=(pString->AllocatedChars-1))
		/* the -1 to take into account the allocated '\0' character */
	{
		/* allocate 2 times previous allocated memory or 2*"length" if allocated memory 
			is less than "length" */
		pString->AllocatedChars = 
			(pString->AllocatedChars<=length) ? (2*length) : 2*(pString->AllocatedChars-1);

		k_ReAllocString(pString,pString->AllocatedChars);
	}
	/* copy given string (pCharArray) to pString and increment total characters variable */
	memcpy(&pString->pChars[pString->NumberOfChars],pCharArray,length);
	pString->NumberOfChars+=length;
	pString->pChars[pString->NumberOfChars] = '\0';
}

void k_SprintfString(TString* pString,char* fmt, ...)
{
	const unsigned int MAX_STRING_SIZE=200;
	char* p;
	va_list args; /* argument pointer */
	p=(char*)malloc(sizeof(char)*MAX_STRING_SIZE);

	va_start(args,fmt);
	if ((unsigned int)vsprintf(p,fmt,args)>=MAX_STRING_SIZE)
	{
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_SprintfString() array contained in the parameter fmt too long");
		return;
	}
	else
	{
		k_AddCharArrayToString(p,pString);
	}
	va_end(args);
	free(p);
}

bool k_InsertCharInString(char Character, unsigned int Index, TString* pString)
{
	if (Index>=pString->NumberOfChars)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_InsertCharInString() Index (%d) is out of range (NumberOfChars=%d)",
			Index, pString->NumberOfChars);
		return false;
	}
	if (pString->pChars==NULL)
		k_InitString(pString);

	/* Add a dummy position to the end of position array */
	k_AddCharToString(pString->pChars[0],pString);
	/* Move positions */
	memmove(&(pString->pChars[Index+1]),&(pString->pChars[Index]),
		sizeof(char)*(pString->NumberOfChars-Index-1));
	/* Store the new position at Index */
	pString->pChars[Index]=Character;
	return true;
}

bool k_RemoveCharFromString(unsigned int Index, TString* pString)
{
	if (Index>=pString->NumberOfChars)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_RemoveCharFromString() Index (%d) is out of range (NumberOfChars=%d)",
			Index, pString->NumberOfChars);
		return false;
	}
	if (pString->pChars==NULL)
		k_InitString(pString);

	/* If the Index is the last position in character array no movement */
	if (Index<(pString->NumberOfChars-1))
	{
		/* Move positions */
		memmove(&(pString->pChars[Index]),&(pString->pChars[Index+1]),
			sizeof(char)*(pString->NumberOfChars-Index));
	}
	else
	{
		pString->pChars[pString->NumberOfChars-1]='\0';
	}
	pString->NumberOfChars--;
	return true;
}

void k_CopyString(TString* pDest, const TString* pSource)
{
	k_EmptyString(pDest);
	pDest->NumberOfChars=pSource->NumberOfChars;
	pDest->AllocatedChars=pSource->AllocatedChars;
	/* allocate memory for characters and copy characters */
	pDest->pChars=(char*) malloc(pSource->AllocatedChars*sizeof(char));
	memcpy(pDest->pChars,pSource->pChars,(pSource->NumberOfChars+1)*sizeof(char));
}

char* k_FindInString(const char* pStr, const TString* pSource)
{
	return strstr(pSource->pChars,pStr);
}

/* Convert string to uppercase */
void k_StringToUpper(char* str)
{
	char *c=str;
	while(*c!='\0')
	{
		*c=(char)toupper(*c);
		c++;
	}
}

void k_ConvertCharacters(char* str,char SearchChar, char ReplaceChar)
{
	char *c=str;
	while(*c!='\0')
	{
		if (*c==SearchChar)
		{
			*c=ReplaceChar;
		}
		c++;
	}
}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
