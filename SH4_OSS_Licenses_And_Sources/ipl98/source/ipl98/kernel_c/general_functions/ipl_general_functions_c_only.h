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

#ifndef _IPL98_GF_C_ONLY_H
#define _IPL98_GF_C_ONLY_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include <math.h>

#ifdef _IPL98_USING_CPP
// if we are using C++, we include the global constants from this file.
// Done to prevent a simple #define PI definition which may result in name clash.
#include "../../cpp/ipl98_general_functions.h"
#endif /* _IPL98_USING_CPP */

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

#ifndef _IPL98_USING_CPP // define PI only when using the C kernel alone

/** @ingroup c_globals
	Define PI with machine hardware precision (global constant) */
#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

/** @ingroup c_globals
	Define 2*PI with machine hardware precision (global constant) */
#ifndef PI2
#define PI2 6.283185307179586476925286766559
#endif

#endif /*_IPL98_USING_CPP*/

/** @ingroup c_globals
	Calculate the square of given parameter value, last updated 11/24/1999
	@version 1.0
	@author René Dencker Eriksen edr@mip.sdu.dk */
#define k_Sqr(x) ((x)*(x))

/** @ingroup c_globals
	Get the maximum value of x and y.
	@version 1.0
	@author René Dencker Eriksen edr@mip.sdu.dk */
#define k_Max(x,y) ((x)>(y) ? (x) : (y))

/** @ingroup c_globals
	Get the minimum value of x and y.
	@version 1.0
	@author René Dencker Eriksen edr@mip.sdu.dk */
#define k_Min(x,y) ((x)<(y) ? (x) : (y))

/** @ingroup c_globals
	Round x to nearest integer value.
	@version 1.0
	@author René Dencker Eriksen edr@mip.sdu.dk */
#define k_Round(x) ((x)<0 ? (int)(x-0.5) : (int)(x+0.5))

/** @ingroup c_globals
	Round x to nearest UINT8 value or if x>255 then x=255.
	@version 1.0
	@author René Dencker Eriksen edr@mip.sdu.dk */
#define k_RoundUINT8(x) ((x)<0 ? 0 : ((x>255) ? 255 : (UINT8)(x+0.5)))

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_GF_C_ONLY_H */
