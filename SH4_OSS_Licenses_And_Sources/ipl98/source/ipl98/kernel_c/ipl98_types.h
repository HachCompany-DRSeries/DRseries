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

#ifndef _IPL98_TYPES_H
#define _IPL98_TYPES_H

#include "../ipl98_setup.h" /* always include the setup file */

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** \defgroup globals IPL98 Global C and C++ enums, definitions, structs etc.
    @{ */

/*x* \defgroup macrodefines IPL98 Macro definitions with the #define directive
	@{ */
#ifndef IPL98_VERSION
/** This is the overall version of the IPL98 library. You can always print this number
	in your programs if you want to know the version number of IPL98 that were compiled
	with your program. Note that functions, methods, classes etc. can have different
	version numbers but will always be less or equal to this number. */
#define IPL98_VERSION 2.20
#endif /* IPL98_VERSION */
/*x@}*/ /* end group "macrodefines" */

/*x* \defgroup types IPL98 Type enumerations and definitions
	Basic definitions used in all parts of IPL98
	These are generic types and in case of a given bit-size must
	comply with the given size of each type.

	@version 1.00,  last updated 12/7/1999
	@author René Dencker Eriksen edr@mip.sdu.dk 
	@{ */

/*x* \defgroup abstract IPL98 Abstract types

	@version 0.90,  last updated 12/7/1999
	@author René Dencker Eriksen edr@mip.sdu.dk
    @{ */
/** Image types that the library can load: "BMP" and "PGM" */
typedef enum {BMP, PGM} IMAGETYPE;

/** Originating points for image: "CAMERA", "DISC", "RAM" and "EMPTY" */
typedef enum {CAMERA, DISC, RAM, EMPTY} ORIGIN;

/** Returns a string containing the name of origin. Allocates memory
	for the string, so remember to delete this memory! */
void k_GetOriginString(ORIGIN Origin,char** Name);

/** Prints Origin to stdout */
void k_PrintOrigin(ORIGIN Origin);

/** Three values are avaiable: LOWCOLOR, HIGHCOLOR and UNDEFINED. The two values
	HIGHCOLOR and LOWCOLOR denotes, if a pixel group was derived from high color or low
	color pixel values. If not used the value UNDEFINED should be used. */
typedef enum {
	LOWCOLOR=0, HIGHCOLOR=1, UNDEFINED=2
} COLORTYPE;

/** The two values FOURCONNECTED and EIGHTCONNECTED denotes the type of connectivity
	to be used as described in Gonzalez and Woods page 41 */
typedef enum {
	FOURCONNECTED=4, EIGHTCONNECTED=8
} CONNECTIVITY;

/** The eight values NORTH, EAST, SOUTH, WEST, NORTHEAST, NORTHWEST, SOUTHEAST and SOUTHWEST
	denotes directions in the image, east is to the right in an image (increasing x-values). */
typedef enum {
	NORTH=0, NORTHEAST=1, EAST=2, SOUTHEAST=3, SOUTH=4, SOUTHWEST=5, WEST=6, NORTHWEST=7
} DIRECTION;

/** The two values CLOCKWISE and ANTICLOCKWISE denotes a rotation direction. */
typedef enum {
	CLOCKWISE, ANTICLOCKWISE
} ROTATION;

/** The three values COMPLEX_REAL means the real part of a complex value, 
	COMPLEX_IMAGINARY mean the imaginary part of a complex value, and 
	COMPLEX_MODULUS means the modulus of the real an imaginary part. */
typedef enum {
	COMPLEX_REAL, COMPLEX_IMAGINARY, COMPLEX_MODULUS
} COMPLEXCHOISE;

/*x@}*/ /* end group "abstracts" */

/*x* \defgroup valuetypes IPL98 Value types
	Must comply with the given size of each type.

	@version 1.30,  last updated 7/12/2000
	@author René Dencker Eriksen edr@mip.sdu.dk
    @{ */

/**	Must be 8 bit signed, range for INT8: [-128;127]. */
typedef char INT8;
/**	Must be 8 bit unsigned, range for UINT8: [0;255]. */
typedef unsigned char UINT8;

/**	Must be 16 bit signed, range for INT16: [-32768;32767]. */
typedef short int INT16;
/**	Must be 16 bit unsigned, range for UINT16: [0;65535]. */
typedef unsigned short int UINT16;

/**	Must be 32 bit signed, range for INT32: [0;4294967295]. */
typedef long int INT32;
/**	Must be 32 bit unsigned, range for UINT32: [-2147483648;2147483647]. */
typedef unsigned long int UINT32;

/** Must be 32 bit floating point (type float or double) */
typedef float FLOAT32;

/*x@}*/ /* end group "valuetypes" */

/*x* \defgroup cppcompliance IPL98 Bool, true and false for C only
	The type bool (integer) is only defined when IPL98 is used as an
	ANSI C only library. The value "false" is defined as 0 and "true" as 1.
	In C++ the boolean type is already defined.
	Defined to comply with the C++ counterpart
	@{ */
#ifndef _IPL98_USING_CPP
/** defines bool as an integer */
typedef int bool;
/** defines false to 0 (zero) */
#define false 0
/** defines true to 1 (one) */
#define true 1
#endif /* _IPL98_USING_CPP */

/*x@}*/ /* end group "cppcompliance" */

/*x@}*/ /* end group "types" */

/*@}*/ /* end of group "globals" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_TYPES_H */
