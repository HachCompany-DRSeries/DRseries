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

#ifndef _IPL98_STR_LIST_H
#define _IPL98_STR_LIST_H

#include "../../../ipl98_setup.h" /* always include the setup file */
#include <list>
#include "../../error.h"

namespace ipl{

using std::list;

/* str_list class.
    Is inherited from the standard template class list in order to enable streaming ooperators.    
    @author Qinyin Zhou <qinyin@mip.sdu.dk>, René Dencker Eriksen <edr@mip.sdu.dk>
    @version 0.99
*/
template<class T> class str_list : public list<T> {
public:
   /** Default constructor, not used.
   */
	str_list();


   /** Writes the contents of the object into streams.
	   Calls StreamIndent() for each streamed line. */
   inline friend std::ostream& operator<<(std::ostream& s, const str_list<T>& sl) 
   {
	   s << StreamIndent << "str_list " << endl << StreamIndent << "TotalItems: " << sl.size() << endl;
		/* Needs to include the following precompiler dependence. If it is the GNU C++ compiler,
		   the keyword "typename" must be inserted in front of this line in order to avoid a
		   warning from that compiler. With Borland C++ Builder 6.0, this keyword is not accepted.
		   Visual C++ 6.0 does not care! */
#ifdef __GNUC__
	   typename str_list<T>::const_iterator it=sl.begin();
#else
	   str_list<T>::const_iterator it=sl.begin();
#endif
	   while (it!=sl.end()) {
		   s << *it << " ";
		   ++it;
	   }
	   s << endl;
	   return s;
   }

   /** Reads input from given stream into this class. Stream must be in correct format
       according to how the << ostream operator formats output.
       Note: Because of a bug in the Borland C++ 4.0 compiler, the definition of this
       method is placed in the header file, otherwise the Borland compiler
       can't link.
   */
   //template<class T> // inserted by edr - should not be necessary but for some reason Borland 4.0 needs it!
   inline friend std::istream& operator>>(std::istream& is, str_list<T>& sl)
   {
	   string Token;
	   ipl::SkipSpaceAndComments(is);
	   // read 'str_list' token
	   is >> Token;
	   if (Token!=string("str_list")) {
		   ostringstream ost;
		   ost << "operator>>(istream, str_list) Token str_list not found" << IPLAddFileAndLine;
		   CError::ShowMessage(IPL_ERROR,ost);
		   return is;
	   }
	   ipl::SkipSpaceAndComments(is);
	   
	   // skip 'TotalItems:' token
	   is.ignore(std::numeric_limits<int>::max(),std::char_traits<char>::to_int_type(':'));
	   
	   unsigned int TotalItems;
	   is >> TotalItems;
	   
	   sl.clear();

	   for (unsigned int i = 0; i < TotalItems; i++) {
		   T Var;
		   is >> Var;
		   sl.push_back(Var);
	   }
	   return is;
   }
};


template<class T> str_list<T>::str_list() : list<T>() { ; }

} // end namespace ipl

#endif //_IPL98_STR_LIST_H
