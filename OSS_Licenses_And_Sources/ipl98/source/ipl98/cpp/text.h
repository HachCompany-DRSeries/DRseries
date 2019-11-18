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

#ifndef _IPL98_TEXT_H
#define _IPL98_TEXT_H

#include <vector>
#include <string>

#include "../kernel_c/kernel_io.h"
#include "../kernel_c/kernel_text.h"

namespace ipl{ // use namespace ipl

using std::ostream;
using std::string;

/** @ingroup utilities
@{ */

/** CText is a simple text class to hold information on operations performed on images,
	last updated 11/2/1999.
	The CImage and CByteImage classes have an attribute of this type to store
	information about which methods have been used on the image. It is possible
	to search this history list for previous calls by the methods Find() and
	FindNext(). Items cannot be removed, but a call to Empty() will remove all
	items leaving only one line containing information on how many lines (calls
	to methods) were deleted. All strings appended to the list will be appended
	with a date and time stamp. These stamps can be turned off by calling NoTime()
	and NoDate(). All lines inserted will also be appended a '\#' character at the front
	and a '\n' at the end. Methods can only insert single lines, that is lines without
	the '\n' character.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CText ipl98/cpp/text.h
	@version 1.10,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
class CText
{
	// attributes
	private:
		TIplText* m_pHistory; // contains all the history information
		bool m_InternalTIplText; // used to check if the destructor must
								 // delete the TIplText structure

	// methods
	public:
		/** The default constructor creates an internal version of m_pHistory
			and sets m_InternalTIplText to true */
		CText();
		/** Constructor, initializes the member m_pHistory pointer to
			point to the "History"-variable in the TImage structure. Sets
			m_InternalTIplText to false;
			The destructor does not delete the structure pointed to by
			m_pHistory, this should be done when the TImage structure which
			contains the TIplText structure is destroyed. */
		CText(TIplText* pHistory);
		/** The destructor only deletes the contents of m_pHistory if it is an internal
			created variable (done by checking the m_InternalTHistory attribute),
			ie. the constructor CText() was used. */
		~CText();
		/** overloading of assignment operator, deletes old history in destination */
		CText& operator=(const CText& SourceHistory);
		/** Sets the m_pHistory attribute to point to the 
			given pointer pHistory.
			@return False, if pHistory is a NULL pointer. */
		bool SetTextPtr(TIplText* pHistory);
		/** Returns the pointer to the history data.
			@return pointer to m_pHistory */
		TIplText* GetTextPtr() const;
		/** Appends a new string to the text list
			@return false if line contains the character '\n' */
		bool Append(const string& str);
		/** Same as Append() except a text containing "IPL98 version xxx performed", where
			xxx is the current version of IPL98, is put in front of the text conatined in
			the parameter str.
			@return false if line contains the character '\n' */
		bool AppendIPL(const string& str);
		/** Same as AppendIPL(const string& str) except the given string parameter is now a char*.
			@return false if line contains the character '\n' */
		bool AppendIPL(const char* pCharArray);
		/** Appends a command promt argument list to the text list.
			@param argc Number of arguments (including the program name).
			@param argv List of arguments.
			@return false if argv is a NULL pointer. */

		/** Creates a text string created from a printf format specification,
			and appends the string by a call to AppendIPL().

			@param format (const char*) A format string in the style of the
				printf family of functions. The format string is followed by the
				parameters of the format.

			@return false if the formatting fails or AppendIPL() fails.
		*/
		bool PrintfAppendIPL(const char* format, ...);

		bool AppendCPArguments(int argc, const char** argv);
		/** Finds first entry in the list containing the text given in str, the whole
			line is returned in Result. The search string is copied to an internal
			attribute to be used by the method FindNext().
			@param str The text to search for
			@param Result The found string, unchanged if not found
			@return False if not found */
		bool Find(const string& str, string& Result);
		/** Finds the next entry in the list containing the text given by
			a previous call to Find(). Only one occurence in each line is found, i.e.
			a line can only be returned in Result one time.
			@param Result The found string, unchanged if not found
			@return False if not found */
		bool FindNext(string& Result);
		/** Gets a string item in the vector at given index 
			@param Index The item number in the list of strings
			@param Result The string at the given index, unchanged if out of range
			@return False if Index is out of range */
		bool GetItem(unsigned long Index,string& Result) const;
		/** Gets the number of string items in the list
			@return Number of strings in list */
		unsigned long TotalItems() const;
		/** Disables or enables the automatic adding of time stamps
			@param Value True to enable time stamps.
			@return Value before setting it with this method. */
		bool AddTime(bool Value);
		/** Disables or enables the automatic adding of date stamps
			@param Value True to enable date stamps.
			@return Value before setting it with this method. */
		bool AddDate(bool Value);
		/** Removes all items leaving only one line containing information on how many 
			lines (calls to methods) were deleted. Sums up previous list deletions. */
		void Empty();
		/** Loads history file. Must be the same format as normal representation.
			old history is deleted before loading.
			@param pFileName Name of file including extension and relative
			or absolute path, f.ex. absolut path "c:/temp/history.txt",
			relative "history.txt". Both slash ("/") and back slash ("\") can be
			used in the path. There is no differense between upper
			and lower case characters, they are treated equally.
			@return false if load failed */
		bool Load(const char* pFileName);
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
		bool Save(const char* pFileName=NULL);
		/** Sets the file information for the CText object from a given path and file name. */
		bool SetFileInfoFromPathFile(const char* pPathAndFileName);
		/** Prints the strings in the list to standard out */
		void PrintData();
		/** @name streaming operators */
		//@{
		/** Writes all the text information to stream.
			Calls StreamIndent() for each streamed line. */
		friend ostream& operator<<(ostream& s,CText& Txt);
		//@}
		/** Converts entire list of strings to one "char*" array.
			Needed when calling the Save method on an image. The Save
			method calls the ANSI C function k_Save(), which cannot handle
			the C++ strings and lists!
			@param ppHistory The resulting "char*" array */
		void ListToCharArray(char** ppHistory) const;
		/** Converts a "char*" array to one list of strings.
			Needed when calling the Load method on an image. The Load
			method calls the ANSI C function k_Load(), which cannot handle
			the C++ strings and lists!
			@param pHistory The "char*" array to be inserted into this class */
		void CharArrayToList(const char* pHistory);
		/** Removes last line from the history, for internal use only.
			@return False, if the history is empty (ie. could not remove line). */
		bool RemoveLastLine();
		/** Appends one text to this object, for internal use only.
			@param Source Source text to be appended to this object. */
		void AppendCText(const CText& Source);
};

/*@}*/ /* end group "utilities" */

} // end namespace ipl

#endif //_IPL98_TEXT_H
