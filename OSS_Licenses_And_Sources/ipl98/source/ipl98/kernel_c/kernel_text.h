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

#ifndef _IPL98_KERNEL_TEXT_H
#define _IPL98_KERNEL_TEXT_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "kernel_string.h"
#include "kernel_io.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup ipltext ANSI-C IplText functions
	A simple text class to hold information on operations 
	performed on images, last updated 10/11/1999.
	The structures TImage, TIntImage, TComplexImage and TFloatImage all 
	have an attribute of this type to store
	information about which methods have been used on the image. It is possible
	to search this history list for previous calls by the methods Find() and
	FindNext(). Items cannot be removed, but a call to Empty() will remove all
	items leaving only one line containing information on how many lines (calls
	to methods) were deleted. All strings appended to the list will be appended
	with a date and time stamp. These stamps can be turned off by setting the members
	AppendTime and AppendDate in TIplText to false. All lines inserted will also be 
	appended a '\#' character at the front and a '\n' at the end. Methods can only 
	insert single lines, that is lines without the '\n' character.

	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 0.4,  by edr@mip.sdu.dk
	@author René Dencker Eriksen
	@{ */

/** Contains a list of strings used to insert lines with information about 
	operations performed.
	The list of string contained in this structure is used by the algorithms in IPL98,
	they add one line to this list each time they are called.
	@see TString
	@see TFileInfo */
typedef struct{
	/** @param AppendTime True to automatically append time. */
	bool AppendTime;
	/** @param AppendDate True to automatically append date. */
	bool AppendDate;
	/** @param FindPosition position following the position in StringList
								where last Find() succeeded. */
	unsigned long FindPosition;
	/** @param SearchStr The string to search for, set by Find(). */
	TString SearchStr;
	/** @param NumberOfLines Total number of lines in the stringlist. */
	unsigned int NumberOfLines;
	/** @param AllocatedLines Internal use. Remembers the maximum allocated
		lines available in the ppStringList array.. */
	unsigned int AllocatedLines; /* internal use */
	/** @param pStringList contains the text data. */
	TString* pStringList;
	/** @param FileInfo default path and filename. */
	TFileInfo FileInfo;
} TIplText;

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#ifdef _IPL98_USING_CPP
} /* end namespace ipl */
#endif

/* put #include directives here */

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
#endif

#ifdef __cplusplus
extern "C" { /* prevent name mangling for C functions */
#endif

/** Initializes the TIplText structure. AppendTime and AppendDate are set to true. FindPosition
	is set to 0. SearchStr is set to NULL. StringList is initialized to a NULL pointer. FileInfo
	is initialized with k_InitFileInfo.
	@param pText Structure to initialize */
void k_InitText(TIplText* pText);

/** Deletes all allocated memory in the TIplText structure and calls k_InitText.
	@param pText Structure to initialize. */
void k_EmptyText(TIplText* pText);

/** Appends a new string to the text list. Date is added in the front if the AppendDate is set to
	true in the TIplText structure. Time is added after date is AppendTime is true in TIplText.
	@param Text The list of text to append the new string to.
	@param pStr The string to be appended.
	@return false if line contains the character '\n'. */
bool k_AppendText(TIplText* pText, const char* pStr);

/** Same as k_AppendText() except a text containing "IPL98 version xxx performed", where
	xxx is the current version of IPL98, is put in front of the text conatined in
	the parameter pStr.
	@pText The list of text to append the new string to.
	@param pStr The string to be appended.
	@return false if line contains the character '\n' */
bool k_AppendTextIPL(TIplText* pText, const char* pStr);

/** Creates a text string created from a printf format specification,
	and appends the string by a call to k_AppendTextIPL().

	@param pDest (TIplText*) The text to append to.

	@param format (const char*) A format string in the style of the printf
	  family of functions. The format string is followed by the parameters of
	  the format.

	@return false if the formatting fails or k_AppendTextIPL() fails.
*/
bool k_PrintfAppendTextIPL(TIplText* pDest, const char* format, ...);

/** Appends a command promt argument list to the text list.
	@pText The list of text to append the command prompt string to.
	@param argc Number of arguments (including the program name).
	@param ppArgv List of arguments from command prompt.
	@return false if argv is a NULL pointer. */
bool k_AppendCPArgumentsText(TIplText* pText, int argc, const char** ppArgv);

/** Copies whole TIplText structure from "pSource" to "pDest". 
	Previous data in "pDest" are destroyed unless the function returns
	false, in that case pDest is unchanged.
	@return False, if source and destination text is the same. */
bool k_CopyText(TIplText* pDest, const TIplText* pSource);

/** Finds first entry in the list containing the text given in pStr, the whole
	line is returned in pResult. The search string is copied to the SearchStr 
	member of pText to be used by the function k_FindNextText().
	@param pStr The text to search for.
	@param pResult The found string, unchanged if not found.
	@return False if not found. */
bool k_FindText(TIplText* pText, const char* pStr, TString* pResult);

/** Finds the next entry in the list containing the text given by
	a previous call to Find(). Only one occurence in each line is found, i.e.
	a line can only be returned in pResult one time.
	@param pResult The found string, unchanged if not found.
	@return False if not found. */
bool k_FindNextText(TIplText* pText, TString* pResult);

/** Gets a string item in the text list at given index.
	@param Index The item number in the list of strings.
	@param pResult The string at the given index, unchanged if out of range.
	@return False if Index is out of range. */
bool k_GetItemText(const TIplText* pText, unsigned long Index, TString* pResult);

/** Removes all items leaving only one line containing information on how many
	lines (calls to methods) were deleted. Sums up previous list deletions. */
void k_EmptyLeaveOneLineText(TIplText* pText);

/** Loads history file. Must be the same format as normal representation,
	old history is deleted before loading.
	@param pFileName Name of file including extension and relative
	or absolute path, f.ex. absolut path "c:/temp/history.txt",
	relative "history.txt". Both slash ("/") and back slash ("\") can be
	used in the path. There is no differense between upper
	and lower case characters, they are treated equally.
	@return false if load failed, in this case the pText will be empty. */
bool k_LoadText(TIplText* pText, const char* pFileName);

/** Saves the history list to a file. The history list is always saved inside a
	PGM file.
	@param pFileName Name of file including extension and relative
	or absolute path, f.ex. absolut path "c:/temp/history.txt",
	relative "history.txt". If no string is given, the file will be saved
	with the filename set in the member variable m_FileInfo. If this class is
	an object in the CStdImage class, the m_FileInfo will be set to the same
	value as the CStdImage fileinfo (contained in the m_Image member), except
	that the file extension is "txt". Both slash ("/") and back slash ("\")
	can be used in the path. There is no differense between upper
	and lower case characters, they are treated equally.
	@return false, if both pFileName and the m_FileInfo are NULL. */
bool k_SaveText(TIplText* pText, const char* pFileName);

/** Sets the file information for the TIplText structure from a given path and file name. 
	The extension from the original path and filename is replaced with "txt". */
bool k_SetFileInfoFromPathFileText(TIplText* pText, const char* pPathAndFileName);

/** Prints the strings in the list to standard out */
void k_PrintDataText(TIplText* pText);

/** Converts entire list of strings to one "char*" array.
	Needed when calling the k_Save method on an image which cannot handle
	the structure from TIplText!
	@param ppHistory The resulting "char*" array. */
void k_ListToCharArrayText(const TIplText* pText, char** ppHistory);

/** Converts a "char*" array to one list of strings.
	Needed when calling the k_Save method on an image which cannot handle
	the structure from TIplText!
	@param pHistory The "char*" array to be inserted into this class. */
void k_CharArrayToListText(TIplText* pText, const char* pHistory);

/** Removes last line from the history, for internal use only.
	@return False, if the history is empty (ie. could not remove line). */
bool k_RemoveLastLineText(TIplText* pText);

/** Appends one text to an existing text, for internal use only.
	@param pDest Destination text to append source to.
	@param pSource Source text to be appended to pDest. */
void k_AppendIplText(TIplText* pDest, const TIplText* pSource);

/*@}*/ /* end group "ipltext" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_TEXT_H */
