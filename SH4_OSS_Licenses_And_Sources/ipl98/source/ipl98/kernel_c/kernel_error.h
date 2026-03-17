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

#ifndef _IPL98_KERNEL_ERROR_H
#define _IPL98_KERNEL_ERROR_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "kernel_string.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup errorhandling ANSI-C Error and warning handling by IPL98.
	Kernel ANSI C error/warning functions, Last updated 3/23/1999.
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 0.85
	@author René Dencker Eriksen (edr@mip.sdu.dk)
    @{ */

/** The three values IPL_ERROR, IPL_WARNING and IPL_NORMAL denotes what type of message should
	be produced.
	@see k_ShowMessageStd */
typedef enum {IPL_ERROR, IPL_WARNING, IPL_NORMAL} IPL_MESSAGETYPE;

/** defines the maximum size in bytes of the error messages produced by k_ShowMessageStd() */
#define MAX_MESSAGE_SIZE 4000

/** A short-cut for the __FILE__, __LINE__ arguments needed by k_ShowMessageStd(). */
#define IPL_POSITION __FILE__, __LINE__

/** This function should be used by all messages produced by the ANSI C part of
	IPL98.

	In the header file "ipl98.h" you can decide which error messages should be
	produced by the three definitions "DISABLE_IPL_ERROR", "DISABLE_IPL_WARNING"
	and "DISABLE_IPL_NORMAL".

	By default error messages are printed to stdout. To change or extend this
	behaviour, manually edit the definition of k_ShowMessageStd() found in the
	file kernel_error.c.

	Example of use:

		k_ShowMessageStd(IPL_POSITION, IPL_NORMAL, "The value of x is %d", x);

	@param File (char*) The file name to insert in the message (e.g. __FILE__).

	@param Line (int) The line number to insert in the message (e.g. __LINE__).

	@param MsgType (IPL_MESSAGETYPE) Depending on the message type (IPL_ERROR,
		IPL_WARNING or IPL_NORMAL) a header is prepended to the message. You can
		manually customize k_ShowMessageStd() to for example abort the program
		if the message is of type IPL_ERROR.

	@param Format (char*) A format string in the style of the printf family of
		functions. As in printf, the format string is followed by the arguments
		needed by the format.

	@see IPL_POSITION
	@see k_ShowMessageSimple
*/
void k_ShowMessageStd(char* File, int Line, IPL_MESSAGETYPE MsgType, char* Format, ...);

/** This macro takes care of inserting the __FILE__ and __LINE__ arguments of
	k_ShowMessageStd() for the case where no arguments for the format string are
	needed.

	@param MsgType (IPL_MESSAGETYPE) The type of message (IPL_ERROR, IPL_WARNING
		or IPL_NORMAL).

	@param Msg (char *) The message to display.

	@see k_ShowMessageStd
*/
#define k_ShowMessageSimple(MsgType, Msg) k_ShowMessageStd(__FILE__, __LINE__, MsgType, "%s", Msg)

/** k_ShowMessage() is DEPRECATED. Use k_ShowMessageStd() instead.

    @see k_ShowMessageStd

	This function should be used by all messages produced by the ANSI C part of
	IPL98. In the header file "ipl98.h" you can decide which error messages
	should be produced by the three definitions "DISABLE_IPL_ERROR",
	"DISABLE_IPL_WARNING" and "DISABLE_IPL_NORMAL". By default these constants
	are not defined. To remove path/file information and line information look
	at the explanation to k_AddFileAndLine(). To change the output (default is
	to stdout) to another stream edit the bottom source code in this function
	(in the file kernel_error.c).

	@param MsgType (IPL_MESSAGETYPE) Three types of messages can be handled:
		IPL_ERROR, IPL_WARNING and IPL_NORMAL. In case of IPL_ERROR or
		IPL_WARNING, the given string "pLineAndFile" is appended to the
		resulting message and either "IPL_ERROR" or "IPL_WARNING" is inserted in
		front of the message. In case of "IPL_NORMAL" no appending the parameter
		"fmt" is done.

	@param pLineAndFile (TString*) This parameter should be initialised with a
		call to the function k_AddFileAndLine() found in the string section.

	@param fmt (char*) Control format string exactly as the one given to
		"printf" in C. Must be followed by the corresponding arguments as in a
		"printf"-specification.

	@see k_AddFileAndLine */
void k_ShowMessage(IPL_MESSAGETYPE MsgType, const TString* pLineAndFile,char* fmt, ...);

/*@}*/ /* end group "errorhandling" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_ERROR_H */
