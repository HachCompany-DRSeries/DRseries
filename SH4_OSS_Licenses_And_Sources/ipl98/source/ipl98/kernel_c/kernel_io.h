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

#ifndef _IPL98_KERNEL_IO_H
#define _IPL98_KERNEL_IO_H

#include <stdio.h>
#include "../ipl98_setup.h" /* always include the setup file */
#include "ipl98_types.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup filefunctions ANSI-C File related functions
	Kernel ANSI C file related routines, last updated 9/18/2003.
	Basic read/write functions for reading/writing UINT8s, UINT16s and UINT32s
	and functions for handling file name and path strings. These functions
	are helper functions and only used by the kernel C functions.
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 1.12
	@author René Dencker Eriksen edr@mip.sdu.dk
    @{ */

/** File information for an image.
	The file/path names are set by k_Load() and k_Save().
	The members in the struct can be seen under "parameters".
	@see k_Load
	@see k_Save */
typedef struct {
	/** @param FileName file name with extension, NULL if no file loaded. */
	char* FileName;
	/** @param PathName path for loaded file, NULL if no file loaded . */
	char* PathName;
	/** @param PathAndFileName path including file name for loaded file,
		NULL if no file loaded. */
	char* PathAndFileName;
} TFileInfo;

/**	Sets all the pointers in pInfo to NULL, allways call this
	function when creating a new TFileInfo variable. */
void k_InitFileInfo(TFileInfo* pInfo);

/** Deletes all allocated memory for TFileInfo structure and
	sets all pointers to NULL. */
void k_EmptyFileInfo(TFileInfo* pInfo);

/** Previous data in "pDest" are destroyed unless the function returns
	false, in that case pDest is unchanged.
	@return False, if source and destination file info is the same. */
bool k_CopyFileInfo(TFileInfo* pDest,const TFileInfo* pSource);

/** Prints file information to stdout */
void k_PrintFileInfo(const TFileInfo* pFileInfo);

/** Reads UINT8 type from file */
UINT8 k_ReadUINT8(FILE* fp);

/** Reads UINT16 type from file */
UINT16 k_ReadUINT16(FILE* fp);

/** Reads UINT32 type from file */
UINT32 k_ReadUINT32(FILE* fp);

/** Saves UINT8 type to file */
void k_WriteUINT8(FILE* fp, UINT8 c);

/** Saves UINT8 type to file */
void k_WriteUINT16(FILE* fp, UINT16 c);

/** Saves UINT8 type to file */
void k_WriteUINT32(FILE* fp, UINT32 c);

/** Skips all whitespaces including '\\n', '\t', ' ' and '\r'
	@return number of whitespace characters read or -1 if end of file was reached */
int k_SkipWhiteSpace(FILE *fp);

/** Skips a whole line, ie. reads characters until a '\\n' character
	is met (the '\\n' is also skipped).
	@return Number of characters read or -1 if end of file was reached. */
int k_SkipLine(FILE *fp);

/** Reads the next sequence of non whitespace characters (a token). Allocates memory
	for the token and returns a pointer to that memory. If the file pointer is at
	a whitespace character, the function automatically increases the file pointer
	position until a non whitespace character is met.
	@return A pointer to the token read or NULL if end of file was reached without
	reading any non whitespace characters. */
bool k_ReadToken(FILE *fp,char** ppToken);

/** Sets all variables in pInfo according to the given FileAndPathName parameter.
	@param FileAndPathName Must include both path and filename (including extension).
		Note: The path may be empty, the information is the relative to working
		directory.
	Returns False if FileAndPathName is NULL or the string is invalid. */
bool k_SetFileAndPath(const char* FileAndPathName,TFileInfo* pInfo);

/** Splits a file name including path given in "FilePathName" into
	a path (returned in "Path") and a filename including extension
	(returned in "FileNameExt") and an extension (returned in "Ext").
	Returns False in these cases:
		FilePathName==NULL or it is an empty string (produce an error message)
		FilePathName ends with the character '/' or '\' (produce an error message) */
bool k_SplitFileName(const char* FilePathName, char** Path, char** FileNameExt, char** Ext);

/*@}*/ /* end group "filefunctions" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_IO_H */
