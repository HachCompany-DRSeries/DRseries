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

#ifndef _IPL98_KERNEL_STRING_H
#define _IPL98_KERNEL_STRING_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "ipl98_types.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup string ANSI-C String container functions
	Functions used by the ANSI C kernel functions, Last updated 1/27/2000.
	Since string handling is very limited in ANSI C it was necessary
	to write these helper functions. Works on a TString structure.
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 1.10
	@author René Dencker Eriksen (edr@mip.sdu.dk)
    @{ */

/** A container for a string, includes corresponding functions to
	manipulate the string. Note that no comparison function is given,
	the standard C functions strcmp can be used. */
typedef struct {
	/** @param NumberOfChars Total number of characters in the string. */
	unsigned int NumberOfChars;
	/** @param AllocatedChars Internal use. Remembers the maximum allocated
		chars available in the pChars array. */
	unsigned int AllocatedChars; /* internal use */
	/** @param pChars Array of characters. */
	char* pChars;
} TString;

/** Sets the pointer pChars in TString to NULL. Initialises all
	integer to 0. Always call this function when creating a variable
	of the TString type */
void k_InitString(TString* pString);

/**	Deletes all allocated memory for the string and sets variables to 0.
	Always call this function when leaving a scope where a TString
	variable were created and used. */
void k_EmptyString(TString* pString);

/**	Deletes all text containe in the string and and leaves a '\0' charcter only.
	That means one character is left in the string so that printing the string will
	not fail. */
void k_DeleteString(TString* pString);

/**	Allocates memory for the array of characters in the TString structure.
	To make appendings fast, call this funcion with a value in InitialSize
	greater than the expected characters in the string.
	@param InitialSize (unsigned int) Number of characters to allocate memory
		for. Must be 1 or greater. */
void k_AllocString(TString* pString,unsigned int InitialSize);

/** Reallocates memory for the TString structure. The new allocated memory
	is not initialised.
	@param NewSize (unsigned int) The new number of characters to allocate memory for.
		Note: The NewSize may be less than the actual allocated memory! In that case
		the corresponding characters stored in the end of the actual memory are deleted. */
void k_ReAllocString(TString* pString,unsigned int NewSize);

/**	Removes unused memory allocated for characters in the TString structure.
	Because memory allocation is a time consuming operation it is a good idea by each
	k_ReAllocString() (or k_AllocString()) call to allocate a big number of characters and
	then at the end of the construction of a TString calling this
	k_AdjustSizeOfString function to deallocate unused memory. */
void k_AdjustSizeOfString(TString* pString);

/**	The given character is added to the string. The function keeps track of
	allocation and updates the AllocatedChars member in TString.
	@param Character (char) Character to append to string. */
void k_AddCharToString(char Character,TString* pString);

/**	The given array of characters is added to the string. The function keeps
	track of allocation and updates the AllocatedChars member in TString.
	@param CharArray (char*) Array of characters to append to string. */
void k_AddCharArrayToString(const char* pCharArray,TString* pString);

/**	Works like the normal ANSI C funktion sprintf except the result is store
	in a TString structure.
	@param fmt (char*) Control format string exactly as the one given to "sprintf" in C.
		Must be followed by the corresponding arguments as in a "sprintf"-specification. */
void k_SprintfString(TString* pString,char* fmt, ...);

/** Insert the given character at given Index, which must be less than the
	NumberOfChars (stored in the TString structure). The ordering of
	the character is preserved.
	@param Character (char) Character to append to string.
	@return False, if Index is out of range. */
bool k_InsertCharInString(char Character, unsigned int Index, TString* pString);

/** Removes a character stored in the string at given index, the order of the
	characters is preserved.
	@param Index Index of character in the string to be removed.
	@return False, if Index is out of range. */
bool k_RemoveCharFromString(unsigned int Index, TString* pString);

/** Copies whole group from "pSource" to "pDest".
	Previous data in "pDest" are destroyed. */
void k_CopyString(TString* pDest, const TString* pSource);

/** Search for given string pStr in the string list.
	@param pStr String to search for.
	@return If found a pointer to the beginning of the found position 
	in pSource is returned, else returns NULL. */
char* k_FindInString(const char* pStr, const TString* pSource);

/** Converts the entire character array contained in str to upper case characters. */
void k_StringToUpper(char* str);

/** Searches the given string "str" for the character in "SearchChar" and
	replaces them with the character in "ReplaceChar"
	@param str Pointer to an array of characters.
	@param SearchChar Character to be replaced.
	@param ReplaceChar Replace with this character. */
void k_ConvertCharacters(char* str,char SearchChar, char ReplaceChar);

/** This macro (#define-macro) appends complete path/file information and line number
	in source code from where this function is called. Used to produce error/warning
	messages. To remove path/file information comment out the call
	"k_AddCharArrayToString(__FILE__,&str)" and to remove line information comment out
	the call "k_AddCharArrayToString(i,&str)". This functions is found in the file
	kernel_string.h".
	@param str (TString) String to append the path/file and line number to. */
#define k_AddFileAndLine(str) do {								\
	char* AddFileAndLine_pStr=(char*)malloc(sizeof(char)*25);	\
	k_AddCharToString('(',&str);								\
	k_AddCharArrayToString(__FILE__,&str);						\
	sprintf(AddFileAndLine_pStr," line %d",__LINE__);			\
	k_AddCharArrayToString(AddFileAndLine_pStr,&str);			\
	k_AddCharToString(')',&str);								\
	free(AddFileAndLine_pStr);									\
} while (0)

/*@}*/ /* end group "string" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_STRING_H */
