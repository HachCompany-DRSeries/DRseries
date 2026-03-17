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

#include <stdio.h>

#ifndef _IPL98_SETUP_H
#define _IPL98_SETUP_H	

/************************************************/
/************************************************/
/********      IPL98 Setup area          ********/
/************************************************/
/************************************************/

/* Visual C++ 6.0 has a bug regarding the standard library functions isspace() and toupper()
   which they forgot to put inside the std namespace, therefore we can only open these functions
   when we are sure that we are not using Visual C++. */
#ifdef __cplusplus
#ifndef _MSC_VER
#include <locale>
using std::isspace;
using std::toupper;
#endif
#endif

/** If __cplusplus is defined, we define _IPL98_USING_CPP. This causes
	all code in the kernel ANSI C part to be included into the ipl namespace.
	If your compiler for some reason doesn't (it should but not all do) define 
	the __cplusplus but you know it is a C++ compiler, just make sure that 
	_IPL98_USING_CPP will be defined here. */
#ifdef __cplusplus
#define _IPL98_USING_CPP
#endif

/** If the compiler is GCC define a macro to insert the template marker <> in
	template friend functions. */
#ifdef __GNUC__
#  define __IPL_TF <>
#elif __BCPLUSPLUS__	
#  define __IPL_TF <>
#else
#  define __IPL_TF
#endif

/** @defgroup setup Controls the compilation of the IPL98 library
	Definitions for controlling the compilation, last updated 7/2/2002.
	This section controls various parts of the compilation of IPL98. Pay
	attention to the big-endian definition if compiling on sparch etc.

	@version 0.70
	@author René Dencker Eriksen (edr@mip.sdu.dk)
	@{ */

/* Define IPL_BIG_ENDIAN on Sparc machines (f.ex. SGI and SUN) */
/*#define IPL_BIG_ENDIAN*/

/* Enable or disable messages produced by IPL98 */
/*#define DISABLE_IPL_ERROR*/		/* comment this line in to disable all errors of type IPL_ERROR */
/*#define DISABLE_IPL_WARNING*/		/* comment this line in to disable all errors of type IPL_WARNING */
/*#define DISABLE_IPL_NORMAL*/		/* comment this line in to disable all messages of type IPL_NORMAL */

/* Enable or disable adding to history from the ANSI C 
   and C++ part of the library. To disable adding history 
   out comment the line "#define IPL_ADD_HISTORY". */
#define IPL_ADD_HISTORY

/* If the value IPL_USER_SUPPLY_ERROR_FUNCTION is defined, the CPP file corresponding to this
   setup will not define the function void AddIPL98ErrorHistory(const char* str). In this case the
   user must define this function. The default behaviour defined by IPL98 is to output all messages
   from the library to std out. If a GUI frontend is used, the text directed to std out is
   normally lost, so in that case the user may want to implement a function specialized for redirecting
   the information to a window. */
/* #define IPL_USER_SUPPLY_ERROR_FUNCTION */

/*@}*/ /* end group "setup" */

#endif /* _IPL98_SETUP_H */
