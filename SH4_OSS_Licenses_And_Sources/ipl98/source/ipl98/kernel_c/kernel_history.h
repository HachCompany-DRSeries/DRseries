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

#ifndef _IPL98_KERNEL_HISTORY_H
#define _IPL98_KERNEL_HISTORY_H

#include "../ipl98_setup.h" /* always include the setup file */
#include <string.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/************************************************/
/********     Internal history indent    ********/
/********           manipulation         ********/
/************************************************/


/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup history ANSI-C Variables and functions for working with the history
	Kernel ANSI C history functions, Last updated 7/2/2002.
	The history is associated with each image. Every time a method is called
	on an image, a line describing the manipulation is added to the image. The
	history can be saved with the image if the format PGM is chosen. It can
	also be accessed any time during runtime. At the moment this is not fully
	implemented but we hope to get up to date with this later. The history can
	be dissabled by setting some definitions in the ipl98_setup.h file.

	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 0.70
	@author René Dencker Eriksen (edr@mip.sdu.dk)
	@see setup
	@{ */

/** History indent variable and two macros for manipulating it.
   Used internally for indenting the text added to the history
   according to the level of nested function calls */
extern char ipl_HistoryIndent[]; /* contains a number of tabulator characters ('\t') used for indenting the history text */
/** macro for adding a tabulator character to the ipl_HistoryIndent[] variable */
#define k_IplStartHistoryMacro() do {							\
	ipl_HistoryIndent[strlen(ipl_HistoryIndent)+1]='\0';		\
	memset(ipl_HistoryIndent,'\t',strlen(ipl_HistoryIndent)+1);	\
} while (0)
/** macro for removing a tabulator character from the ipl_HistoryIndent[] variable */
#define k_IplStopHistoryMacro() (ipl_HistoryIndent[strlen(ipl_HistoryIndent)-1]='\0')

/*@}*/ /* end group "history" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_HISTORY_H */
