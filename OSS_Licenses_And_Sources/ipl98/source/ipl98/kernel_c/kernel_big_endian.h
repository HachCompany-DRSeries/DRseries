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

#ifndef _IPL98_KERNEL_BIG_ENDIAN_H
#define _IPL98_KERNEL_BIG_ENDIAN_H

#include "../ipl98_setup.h" /* always include the setup file */

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/*	Some macros for swapping bytes in case of a BIG ENDIAN machine
	To use these macros define the IPL_BIG_ENDIAN value in the beginning
	of ipl98/ipl98.h.

	@version 0.1,  last updated 2/11/1999
	@author René Dencker Eriksen edr@mip.sdu.dk
*/

#ifdef IPL_BIG_ENDIAN /* these macros only used on IPL_BIG_ENDIAN machines */
#define k_SWAP2BYTES(x) ((x << 8) | ((x & 0xff00) >> 8))
#define k_SWAP4BYTES(x) ((x << 24) | ((x & 0xff00) << 8) | ((x & 0xff0000) >> 8) | (x >> 24))
#endif /* IPL_BIG_ENDIAN */


#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_BIG_ENDIAN_H */
