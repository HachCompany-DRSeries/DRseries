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

#ifndef _IPL98_TYPES_C_ONLY_H
#define _IPL98_TYPES_C_ONLY_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "ipl98_types.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** \ingroup globals
@{ */

/** \defgroup c_only_globals ANSI-C IPL98 Global C only enums, definitions, structs etc.
    @{ */

/*x* \defgroup structs_2d_3d IPL98 2D and 3D structures for C only
	Basic ANSI C typedefinitions. All basic
	2D and 3D typedefinitions are here. Some of the structures
	need correct initialization, functions for this
	is also provided, always use them. After using a
	structure that needs initialisation a "destructor"
	function is also available and should be used.

	@version 0.51,  last updated 2/8/1999
	@author René Dencker Eriksen edr@mip.sdu.dk
	@{ */

#ifndef _T2DINT
#define _T2DINT
/** 2-dimensional point with signed integer (INT16) values.
	This type is only used by the C part of the library, if
	you use the C++ part of the library use the CPoint2D
	class instead. */
typedef struct {
	/** The first dimension */
	INT16 x;
	/** The second dimension */
	INT16 y;
} T2DInt;
#endif /* _T2DINT */

/** 2-dimensional point with float (FLOAT32) values.
	This type is only used by the C part of the library, if
	you use the C++ part of the library use the CPoint2D
	class instead. */
typedef struct {
	/** The first dimension */
	FLOAT32 x;
	/** The second dimension */
	FLOAT32 y;
} T2DFloat;

/** 3-dimensional point with signed integer (INT16) values.
	This type is only used by the C part of the library, if
	you use the C++ part of the library use the CPoint3D
	class instead. */
typedef struct {
	/** The first dimension */
	INT16 x;
	/** The second dimension */
	INT16 y;
	/** The third dimension */
	INT16 z;
} T3DInt;

/** 3-dimensional point with float (FLOAT32) values.
	This type is only used by the C part of the library, if
	you use the C++ part of the library use the CPoint3D
	class instead. */
typedef struct {
	/** The first dimension */
	FLOAT32 x;
	/** The second dimension */
	FLOAT32 y;
	/** The third dimension */
	FLOAT32 z;
} T3DFloat;

/*x@}*/ /* end group "structs_2d_3d" */

/*@}*/ /* end of group "Globals_c_only" */

/*@}*/ /* end of group "globals" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_TYPES_C_ONLY_H */
