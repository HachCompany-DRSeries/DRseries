#ifndef _IPL98_ERROR_H
#define _IPL98_ERROR_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "../kernel_c/kernel_error.h"
#include <sstream>
#include <iostream>
#include <string>

//#ifdef IPL_USER_SUPPLY_ERROR_FUNCTION
extern void AddIPL98ErrorHistory(const char* str);
//#endif

namespace ipl{ // use namespace ipl

using std::ostringstream;
using std::cout;
using std::endl;
using std::string;

/** @ingroup utilities
@{ */

/** Macro adding file and line info to error messages. Example:
	\verbatim

	ostringstream ost;
	ost << " error message" << AddFileAndLine;
	\endverbatim

	This has to be a C type "#define" in order to force the line and file information
	to be where the program error was written. Placing this function in a class will
	result in line and file information related to this class instead of the place
	where the function was called. */
#define IPLAddFileAndLine " (" << __FILE__ << " line " << __LINE__ << ")"

/** Global error message functionality,	last updated 10/11/2001.
	Call 
	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CError ipl98/cpp/error.h
	@version 0.1,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
class CError{
	public: // methods
		/** Shows message depending on how user has set up the message system. 
			@param MsgType Users chooses the message type, it will be prepended to the message.
			@param str String containing error message. */
		static inline void ShowMessage(IPL_MESSAGETYPE MsgType, const char* str);
		/** Shows message depending on how user has set up the message system. 
			@param MsgType Users chooses the message type, it will be prepended to the message.
			@param str String containing error message. */
		static inline void ShowMessage(IPL_MESSAGETYPE MsgType, const string& str);
		/** Shows message depending on how user has set up the message system. 
			@param MsgType Users chooses the message type, it will be prepended to the message.
			@param ost String containing error message. */
		static inline void ShowMessage(IPL_MESSAGETYPE MsgType, const ostringstream& ost);
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

inline void CError::ShowMessage(IPL_MESSAGETYPE MsgType, const char* str)
{
/* return if DISABLE is enabled and equals the given MsgType */
#ifdef DISABLE_IPL_ERROR
	if (MsgType==IPL_ERROR)
		return;
#elif defined DISABLE_IPL_WARNING
	if (MsgType==IPL_WARNING)
		return;
#endif
	ostringstream Output;
	if (MsgType==IPL_ERROR)
	{
		Output << "IPL_ERROR: " << str;
	}
	else if (MsgType==IPL_WARNING)
	{
		Output << "IPL_WARNING: " << str;
	}
	else if (MsgType==IPL_NORMAL)
	{
		Output << "IPL_NOTE: " << str;
	}
	else
	{
		/* should not be possible to get here! */
		Output << "Error: ShowMessage() unknown message type";
	}
//	cout << Output.str().c_str() << endl;
//#ifdef IPL98_ERRORS_TO_WINDOW
	AddIPL98ErrorHistory(Output.str().c_str());
//#endif
}

inline void CError::ShowMessage(IPL_MESSAGETYPE MsgType, const string& str)
{
	ShowMessage(MsgType, str.c_str());
}

inline void CError::ShowMessage(IPL_MESSAGETYPE MsgType, const ostringstream& ost)
{
	ShowMessage(MsgType, ost.str().c_str());
}

/*@}*/ /* end group "utilities" */

} // end namespace ipl

#endif //_IPL98_ERROR_H
