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

#ifndef _IPL98_GENERAL_FUNCTIONS_H
#define _IPL98_GENERAL_FUNCTIONS_H

#include "../kernel_c/ipl98_types.h"

#include <math.h>
#include <istream>
#include <ostream>
#include <limits>
#include <cctype>
#include <string>
#include <sstream>

namespace ipl{ // namespace ipl ~ General Functions

using std::istream;
using std::ostream;
using std::string;
using std::ostringstream;
using std::numeric_limits;

/** \ingroup globals
    @{ */

/** \defgroup cpp_globals IPL98 Global C++ only constants and template functions.
    @{ */
/*x* \defgroup constants IPL98 Constants
    @{ */
/** Define PI with machine hardware precision (global constant).
	The following include statement should be used in order to use this constant:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim */
#ifndef PI
const double PI = acos(-1.0);
#endif
/** Define 2*PI with machine hardware precision (global constant).
	The following include statement should be used in order to use this constant:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim */
#ifndef PI2
const double PI2 = 2*PI;
#endif
/*x*@}*/ /* end group "constants" */

/*x* \defgroup templates IPL98 Template functions
	Basic small template functions which may be usefull everywhere.
	The following include statement should be used in order to use the 
	functions defined in this group:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim
	@version 0.1,  last updated 1/10/1999
	@author René Dencker Eriksen edr@mip.sdu.dk
    @{ */


/** Conversion from Radians to Degrees (global constant). The template type
	must be a floating point type.
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim   */
template <class TYPE>
inline
TYPE RadToDegree(const TYPE &Rad)
{
	return (TYPE)((180.0/PI)*Rad);
}

/** Conversion from Degrees to Radians (global constant). The template type
	must be a floating point type.
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim   */
template <class TYPE>
inline
TYPE DegreeToRad(const TYPE &Degree)
{
	return (TYPE)((PI/180.0)*Degree);
}

/** Calculate the square of given parameter value.
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim   */
template <class TYPE>
inline
TYPE Sqr(const TYPE &x)
{
	return (x*x);
}

/** Swap the values - copy x value to y and copy y value to x.
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim   */
template <class TYPE>
inline
void Swap(TYPE &x,TYPE &y)
{
	TYPE t = x;
	x = y;
	y = t;
}

/** Get the maximum value of x and y.
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim   */
template <class TYPE>
inline
TYPE Max(const TYPE &x, const TYPE &y)
{
	return ((x>y) ? x : y);
}

/** Get the minimum value of x and y.
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim   */
template <class TYPE>
inline
TYPE Min(const TYPE	&x,	const TYPE	&y)
{
	return ((x<y) ? x : y);
}

/** Round x to nearest integer value.
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim   */
template <class TYPE>
inline
int Round(const TYPE &x)
{
	return ((x<0) ? (int)(x-0.5) : (int)(x+0.5));
}

/** Round x to nearest UINT8 value, or if x>255 then x=255, or if x<0 then x=0.
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim   */
template <class TYPE>
inline
unsigned char RoundUINT8(const TYPE &x)
{
	return ((x<0) ? 0 : (x>255) ? 255 : (UINT8)(x+0.5));
}

/*x*@}*/ /* end group "templates" */

/** Streaming operator for a bool on an istream. The
	way IPL98 classes are built, we need this functionality
	for types used in the template based container classes such
	as CArray2D and CArrayOS2D. This may be changed later!
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim
	@version 0.1
	@author Rene Dencker Eriksen (edr@mip.sdu.dk) */
istream& operator<<(bool b, istream& s);

/** Splits a file name including path given in "FilePathName" into
	a path (returned in "Path") and a filename including extension
	(returned in "FileNameExt") and an extension (returned in "Ext").
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim
	@return False in these cases:
		FilePathName==NULL or it is an empty string (produce an error message)
		FilePathName ends with the character '/' or '\' (produce an error message) */
bool SplitFileName(const char* pFilePathName, string& Path, string& FileNameExt, string& Ext);

/** Removes the file extension from provided file name (optionally including a path).
	Works by searching for the last '.' in the string.
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim
	@param PathFileName Path and file name from which the extension will be striped.
	@return False, if no extension is found.
	@version 0.1 */
bool StripFileExtension(string& PathFileName);

/** Converts all back slash characters '\' to slash '/'. Used to avoid problems
	with C++ reading a '\' path separation as an escape character.
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim
	@param Path Path to be converted, resultint string returned in this parameter.
	@version 0.8 */
bool ConvertBackslashToSlash(char* Path);

/** Same as ConvertBackslashToSlash(char* Path), except argument is type string.
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim */
bool ConvertBackslashToSlash(string& Path);

/** Adds a trailing '/' character to the path if not present.
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim
	@return False, if Path is an empty string.
	@version 0.8 */
bool AddTrailingSlashToPath(string& Path);

/** Used for in streams. Skips all whitespace, newline, tab characters and
	comment lines, i.e. rest of line from a '#' character.
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim
	@version 0.8 */
inline 
void SkipSpaceAndComments(istream& s)
{
	while(isspace(s.peek()))
		s.get();
	while (s.peek()=='#')
	{
		s.ignore(std::numeric_limits<int>::max(),'\n');
		while (isspace(s.peek()))
			s.get();
	}
}

/** Reads a token from stream is and compares to provided Token string.
	If not equal, this function throws an error message string in the following
	format: Token: 'token' not found, read 'read_token' instead.
	The function calls SkipSpaceAndComments(is) first.
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim
	@version 0.1 */
inline void ReadToken(const string& Token,istream& is) throw(string)
{
	string ReadToken;
	SkipSpaceAndComments(is);
	is >> ReadToken;
	if (ReadToken!=Token)
	{
		ostringstream ost;
		ost << "Token: '" << Token << "' not found, read: '" << ReadToken << "' instead";
		throw(ost.str());
	}
}

/** Reads the next token (all characters until next white-space is met)
	into Token, but puts it back into the stream, i.e. the streams current
	position is left unchanged.
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim
	@version 0.1 */
inline
void PeekToken(istream& ist,string& Token)
{
	unsigned int Pos=ist.tellg();
	ist >> Token;
	ist.seekg(Pos);
}

/** Used for in streams. Skips all whitespace except '\n' and returns true
	if a newline character '\n' or EOF (End Of File) is met. The
	read state position of the stream is set to the first none white
	character found searching from current input position or the first
	'\n' character in case it was met first..
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim
	@return True, if a '\n' character or EOF is met.
	@version 0.8 */
inline bool IsNextNewLineOrEOF(istream& is)
{
	char ch;
	do{
		if (!is.get(ch)) return true;
	}while(ch!='\n' && isspace(ch));
	if (ch=='\n')
		return true;
	else
	{
		is.putback(ch);
		return false;
	}
}

/** Used to add indentions to nested streaming. All classes in the
	IPL98 with streaming operators will call StreamIndent() for each
	line they stream, except if they only stream one line. An example
	of usage:
	\verbatim
    CImage Img(10,10,24);
    CPoint2D<int> P0(3,3),P1(4,4);
    cout << "Image data:" << endl;
    StreamIndentAdd(); // will add 2 spaces at the beginning of each line
    cout << Img << endl; // Indent is done by streaming operator for CImage
    // Must indent own message lines...
    cout << endl << StreamIndent << "Positions to be accessed:" << endl;
    StreamIndentAdd(5); // will add another 5 spaces to indent
    cout << StreamIndent << P0 << endl; // must add StreamIndent here since CPoint2D<T>
    cout << StreamIndent << P1 << endl; // is a one line streaming and does not include Indent
    StreamIndentRemoveLast();
    cout << StreamIndent << "This line is back to first indent (2 spaces)" << endl;
    StreamIndentRemoveLast();
	\endverbatim
	This example gives the following output:
	\verbatim
    Image data:
      **************** Image info ********************
       Width=10 Height=10 ByteWidth=32 SizeImage=320 Bits=24
       PalEntries=0 BorderSize=0 Origo=(0,0) Origin=RAM
       ROI inactive
       File name: Not available   File path: Not available

      Positions to be accessed:
           (3,3)
           (4,4)
      This line is back to first indent (2 spaces)
    \endverbatim
	@param Spaces Number of spaces to be added to StreamIndent().
	@see StreamIndentRemoveLast()
	@see StreamIndentRemoveAll()
	@see StreamIndent() */
void StreamIndentAdd(unsigned short Spaces=2);

/** Removes last indented spaces added by a previous call
	to StreamIndentAdd(). This function throws an error
	string, if no indents are available for removal. */
void StreamIndentRemoveLast() throw(string);

/** Removes all previous indention added by StreamIndentAdd(). */
void StreamIndentRemoveAll();

/** Indents given stream with the available spaces added by previous calls
	to StreamIndentAdd().
	@return The ostream parameter given as input parameter to this function. */
ostream& StreamIndent(ostream& os);


/** Converts all characters in str to upper case by calling std::toupper(char*)
	on each element in str.
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim */
inline void ToUpperCase(string& str)
{
	string::iterator it=str.begin();
	while(it!=str.end())
	{
		*it=toupper(*it);
		++it;
	}
}

/** Compares two strings case insensitive. Return values are like the ANSI cmp function:
	-1 if s<s2, 0 if s==s2 or 1 if s>s2.
	The following include statement should be used in order to use this function:
	\verbatim

	#include <ipl98/cpp/ipl98_general_functions.h>
	\endverbatim */
int CompareNoCase(const string& s, const string& s2);


/** Inserts ' #' in front of each line contained in Str. This is used
	for writing information to a file as comment lines.
	@param Str String containing the lines to be converted.
	@return New string with inserted characters in front of each line.
	@version 0.2
	@author Rene Dencker Eriksen (edr@mip.sdu.dk) */
string ConvertLinesToComments(string& Str);

/**@}*/ /* end group "cpp_globals" */

/*@}*/ /* end of group "globals" */

} // end namespace ipl


#endif /* _IPL98_GENERAL_FUNCTIONS_H */
