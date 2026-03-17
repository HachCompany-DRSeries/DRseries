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

#ifndef _IPL98_TYPES_CPP_H
#define _IPL98_TYPES_CPP_H

#include "../ipl98_setup.h" /* always include the setup file */

/* A simple file which includes types used in both the C kernel and
   C++ part of the library. This file simply includes the corresponding
   kernel C file defining the types. It is ment as a simple way for the
   programmer using the C++ part of the library to include the IPL98
   types.
   Here is a short summary of the defined types and enums. Look at the
   kernel file for more information.
   Types defined:
      INT8
	  UINT8
	  INT16
	  UINT16
	  INT32
	  UINT32
	  FLOAT32
   Enums defined:
      IPL98_VERSION
	  IMAGETYPE
	  ORIGIN
	  COLORTYPE
	  CONNECTIVITY
	  DIRECTION
	  ROTATION
	  COMPLEXCHOISE
*/
#include "../kernel_c/ipl98_types.h"

#endif /* _IPL98_TYPES_CPP_H */
