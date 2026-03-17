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

#ifndef _IPL98_KERNEL_PALETTE_H
#define _IPL98_KERNEL_PALETTE_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "ipl98_types.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup palette ANSI-C Palette functions
	Palette manipulation functions, last updated 10/9/2001.
	Including macros to create palette entries and to extract 
	R,G or B components in an entry.

	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 1.21
	@author René Dencker Eriksen edr@mip.sdu.dk
    @{ */

/** Palette information for an image.
	Includes values for each palette entry and number of entries.
	The members in the struct can be seen under "parameters". */
typedef struct {
	/** @param PalEntries Number of palette entries in pPalette. */
	UINT16 PalEntries;
	/** @param pPalette Pointer to palette. */
	UINT32* pPalette;
} TPalette;

#ifndef k_PalCreateRGB
/** Create a UINT32 RGB-component from the three 
	components r, g and b.
	The r, g and b components must be UINT8 types, i.e. 8 bits. */
#define k_PalCreateRGB(r, g ,b)  ((UINT32)(((UINT8)(b) | ((UINT16)(g)<<8)) | (((UINT32)(UINT8)(r))<<16)))
#endif /* k_PalCreateRGB */

#ifndef k_PalGetRVal
/** Extract the R-value from a UINT32 RGB-component.
	Return value is of type UINT8 */
#define k_PalGetRVal(rgb)   ((UINT8) ((rgb) >> 16))
#endif

#ifndef k_PalGetGVal
/** Extract the G-value from a UINT32 RGB-component.
	Return value is of type UINT8 */
#define k_PalGetGVal(rgb)   ((UINT8) (((UINT16) (rgb)) >> 8))
#endif

#ifndef k_PalGetBVal
/** Extract the B-value from a UINT32 RGB-component.
	Return value is of type UINT8 */
#define k_PalGetBVal(rgb)   ((UINT8) (rgb))
#endif

/**	Sets all the pointers in pPal to NULL and
	PalEntries to 0. Allways call this function 
	when creating a new TPalette variable. */
void k_InitPalette(TPalette* pPal);

/** Deletes all allocated memory for the palette, PalEntries is
	set to 0 and the pPalette pointer to NULL. */
void k_EmptyPalette(TPalette* pPal);

/** Allocates memory for palette and initialises to gray tones.
	If 24 b/p no palette is created and palette entries is
	set to 0 (pPal->PalEntries). 
	@return False if Bits is not 1,4,8 or 24. */
bool k_CreatePal(unsigned short Bits,TPalette* pPal);

/**	Returns the palette value at "Index" in "pPalValue".
	@return false if "Index" is outside palette. */
bool k_GetPalValue(int Index, UINT32* pPalValue, const TPalette* pPal);

/**	Returns the palette value at "Index". This function is the same
	as k_GetPalValue but the result is returned directly from the 
	function instead of in a parameter. Produces an error message
	if Index is out of range.
	@return UINT32 palette value at given index */
UINT32 k_GetPalColor(int Index, const TPalette* pPal);

/** Sets the palette value to "PalValue" at "Index".
	@return false if "Index" is outside palette. */
bool k_SetPalValue(int Index, UINT32 PalValue, TPalette* pPal);

/** Previous data in "pDest" are destroyed unless the function returns
	false, in that case pDest is unchanged.
	@return False, if source and destination palette is the same. */
bool k_CopyPalette(TPalette* pDest,const TPalette* pSource);

/** Returns true if the palette is a gray tone palette. In a gray tone
	palette each index value must have the same value in the R,G and B 
	components. For an image with 8 b/p at index M in the palette, the
	color channels must have R=M, G=M, and B=M. For an image with 1 b/p,
	index 0 must have R=0, G=0, and B=0 and index 1 must have R=255,
	G=255, and B=255.
	@return True, if palette is a gray tone palette. If palette contains
		a number of entries different from 2 or 256, false is returned. */
bool k_IsPaletteGrayTone(const TPalette* pPal);

/** Prints RGB value to stdout */
void k_PrintPalValue(UINT32 RGB);

/** Prinst all palette information to stdout. */
void k_PrintPaletteInfo(const TPalette* pPal,unsigned int EntriesPrLine);

/*@}*/ /* end group "palette" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_PALETTE_H */
