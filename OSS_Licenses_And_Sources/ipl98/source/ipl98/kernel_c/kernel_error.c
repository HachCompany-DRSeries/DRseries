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

#include "kernel_error.h"
#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

extern void AddIPL98ErrorHistory(const char* str);

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/* Error checking for values returned by the sprintf and vsprintf. To avoid
 * overflowing of buffers the snprintf and vsnprintf functions really should
 * have been used, but these functions might not be present on all platforms we
 * wish to support.
 */
void k_AbortIfOverflow(int cnt, char* File, int Line)
{
	if (cnt <= -1 || MAX_MESSAGE_SIZE <= cnt) {
		fprintf(
			stderr,
			"IPL_ERROR: k_ShowMessageStd() Error message too long at %s line %d"
			" - increase the MAX_MESSAGE_SIZE definition (%s line %d)",
			File,
			Line,
			__FILE__,
			__LINE__);
		assert(0);
	}
}

char* k_MessageHeader(IPL_MESSAGETYPE MsgType)
{
	switch (MsgType) {
	case IPL_ERROR: return "IPL_ERROR: ";
	case IPL_WARNING: return "IPL_WARNING: ";
	case IPL_NORMAL: return "";
	}
	return NULL;
}

char* k_MessageTrailer(IPL_MESSAGETYPE MsgType, char* File, int Line)
{
	int cnt;
	char* trailer = (char *)malloc(MAX_MESSAGE_SIZE * sizeof(char));

	if (MsgType == IPL_ERROR || MsgType == IPL_WARNING) {
		cnt = sprintf(trailer, " (%s line %d)", File, Line);
		k_AbortIfOverflow(cnt, File, Line);
	} else {
		trailer[0] = '\0';
	}
	return trailer;
}

void k_ShowMessageStd(char* File, int Line, IPL_MESSAGETYPE MsgType, char* Format, ...)
{
	char* body;
	char* header;
	char* trailer;
	char* message;
	int cnt;
	va_list args;

    /* return if DISABLE is enabled and equals the given MsgType */
#ifdef DISABLE_IPL_ERROR
	if (MsgType==IPL_ERROR) return;
#endif
#ifdef DISABLE_IPL_WARNING
	if (MsgType==IPL_WARNING) return;
#endif
#ifdef DISABLE_IPL_NORMAL
	if (MsgType==IPL_NORMAL) return;
#endif

	/* Print the user provided message to a buffer. */
	va_start(args, Format);
	body = (char*)malloc(MAX_MESSAGE_SIZE * sizeof(char));
	cnt = vsprintf(body, Format, args);
	k_AbortIfOverflow(cnt, File, Line);
	va_end(args);

	/* Select the header of the message */
	header = k_MessageHeader(MsgType);
	if (header == NULL) {
		k_ShowMessageStd(
			__FILE__, __LINE__, IPL_ERROR,
			"k_ShowMessageStd() unknown message type at %s line %d",
			File, Line);
		return;
	}

	/* Select the trailer of the message */
	trailer = k_MessageTrailer(MsgType, File, Line);

	/* Add header and trailer to the user message */
	message = (char*)malloc(MAX_MESSAGE_SIZE * sizeof(char));
	cnt = sprintf(message, "%s%s%s\n", header, body, trailer);
	k_AbortIfOverflow(cnt, File, Line);

	/* 'message' contains the full message and is now ready for being displayed.
	 * You can customize how messages are being displayed by editing the code
	 * below.
	 */
	AddIPL98ErrorHistory(message);

	/* Freeing of allocated buffers. 'header' is a static string and is
	 * therefore not freed.
	 */
	free(body);
	free(trailer);
	free(message);
}

/************************************************************************
 * DEPRECATED
 ************************************************************************/

void k_ShowMessage(IPL_MESSAGETYPE MsgType, const TString* pLineAndFile,char* fmt, ...)
{
	TString StrOut;
	char* p;
	va_list args; /* argument pointer */
/* return if DISABLE is enabled and equals the given MsgType */
#ifdef DISABLE_IPL_ERROR
	if (MsgType==IPL_ERROR)
		return;
#elif defined DISABLE_IPL_WARNING
	if (MsgType==IPL_WARNING)
		return;
#endif
	k_InitString(&StrOut);
	p=(char*)malloc(sizeof(char)*MAX_MESSAGE_SIZE);

	va_start(args,fmt);
	if (vsprintf(p,fmt,args)>=MAX_MESSAGE_SIZE)
	{
		k_AddCharArrayToString("IPL_ERROR: k_ShowMessage() Error message to long "
			"- increase the MAX_MESSAGE_SIZE definition",&StrOut);
		k_AddFileAndLine(StrOut);
		fprintf(stderr,"%s\n",StrOut.pChars);
		assert(0);
	}
	else if (MsgType==IPL_ERROR)
	{
		k_AddCharArrayToString("IPL_ERROR: ",&StrOut);
		k_AddCharArrayToString(p,&StrOut);
		k_AddCharToString(' ',&StrOut);
		k_AddCharArrayToString(pLineAndFile->pChars,&StrOut);
	}
	else if (MsgType==IPL_WARNING)
	{
		k_AddCharArrayToString("IPL_WARNING: ",&StrOut);
		k_AddCharArrayToString(p,&StrOut);
		k_AddCharArrayToString(pLineAndFile->pChars,&StrOut);
	}
	else if (MsgType==IPL_NORMAL)
	{
		k_AddCharArrayToString(p,&StrOut);
	}
	else
	{
		/* should not be possible to get here! */
		k_AddCharArrayToString("Error: k_ShowMessage() unknown message type",&StrOut);
	}
	k_AddCharToString('\n',&StrOut);
	va_end(args);
	free(p);

	/***********************************************************
	 * To change the destination of error messages made by     *
	 * IPL98 replace the code below with your own              *
	 * The resulting string is accessed by "StrOut.pChars"     *
	 ***********************************************************/
//	printf("%s",StrOut.pChars);
//#ifdef IPL98_ERRORS_TO_WINDOW
	AddIPL98ErrorHistory(StrOut.pChars);
//#endif

	/* Don't delete this line */
	k_EmptyString(&StrOut);

}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
