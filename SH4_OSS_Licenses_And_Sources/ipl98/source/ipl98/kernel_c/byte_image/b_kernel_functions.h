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

#ifndef _IPL98_B_KERNEL_FUNCTIONS_H
#define _IPL98_B_KERNEL_FUNCTIONS_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../image/kernel_functions.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup byteimage ANSI-C ByteImage functions
	Kernel ANSI C ByteImage routines, last updated 8/21/2005.
	Basic ByteImage functions to be used as stand alone c
	functions, most of the functions also used by the IPL98 C++ library.
	These functions all works on a TImage structure with 8 bits/pixel
	only! It means that all functions beginning with "kB_" is
	specialisations of the same functions with the prefix "k_". If a
	function is not found here just use the coresponding function
	beginning with "k_". Origo must always be (0,0), never change this
	value.

	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 1.43
	@author René Dencker Eriksen edr@mip.sdu.dk */
/*@{ */

/** Deletes all allocated memory for image, origo reset to (0,0), sets
	values in pInfo to zero and all pointers to NULL.
	Origin is set to "EMPTY", The history is cleared. */
void kB_EmptyImage(TImage* pInfo);

/**	Sets all the pointers in pInfo to NULL and
	Origin to EMPTY. All structures inside TImage
	is initialised with their corresponding init-function. 
	Allways call this function when creating a new TImage variable. */
void kB_InitImage(TImage* pInfo);

/** Initialises a new image with given dimensions and bits per pixel.
	All pixel values are initialised to 0. Sets
	Origin to "RAM", size of border to 0 and clears the history. 
	Allocates memory 
	for palette and initialises (in case of 1, 4 or 8 b/p) to gray tones.
	Black is zero and white is 1, 15 or 255.
	@param Width The width of image in pixels.
	@param Height The height of image in pixels.
	@return false, if creation of palette fails.
	@see kB_AllocImageFast */
bool kB_AllocImage(UINT32 Width, UINT32 Height,TImage* pInfo);

/** Initialises a new image with given dimensions and bits per pixel.
	Pixel values are not initialised. Sets
	Origin to "RAM", size of border to 0 and clears the history. 
	Allocates memory 
	for palette and initialises (in case of 1, 4 or 8 b/p) to gray tones.
	Black is zero and white is 1, 15 or 255.
	@param Width The width of image in pixels.
	@param Height The height of image in pixels.
	@return false, if creation of palette fails.
	@see kB_AllocImage */
bool kB_AllocImageFast(UINT32 Width, UINT32 Height,TImage* pInfo);

/** Loads given file into "pInfo", all previous information in pInfo is destroyed. 
	At the moment the following formats are supported:
	BMP: The windows BMP format in 1, 8 and 24 bits per pixel.
	PGM: Portable Gray Map, only in binary format (P5) and with 255 gray tones.
		 History present in comment lines is loaded into the TImage.History variable.
	The file name must include a relative
	or absolut path and the name of the file with extension, both characters
	'/' and '\' can be used to seperate folder names (remember to "escape" the
	character '\', ex. "c:\\test.bmp"). The type of the file is determined
	by the given extension. Origin is set to "DISC".
	Adds to the history.
	@param PathAndFileName Filename (including the path) to be loaded.
	@return False if the syntacs of PathAndFileName is wrong, the file is not found
		or the format of the file is wrong (including bitdepths different from 8 b/p). */
bool kB_Load(const char* FileName, TImage* pInfo);

/** Saves given file to disc, the file name must comply with the same rules as
	in kB_Load. 
	At the moment the following formats are supported:
	BMP: The windows BMP format in 1, 8 and 24 bits per pixel.
	PGM: Portable Gray Map, only in binary format (P5) and with 255 gray tones.
		 History present in comment lines is loaded into the TImage.History variable.
	Adds to the history.
	@param PathAndFileName Filename (including the path) to be loaded.
	@return False if the syntacs of PathAndFileName is wrong or the path is not found.
	@see kB_Load */
bool kB_Save(const char* FileName, TImage* pInfo);

/** Copies whole image from "pSource" to "pDest". Accepts 8 b/p source 
	images only. All information is copied
	to the destination image including border pixels, palette and history.
	Previous data in "pDest" are destroyed, including the old history. 
	Origin is set to RAM. File information is left empty. Origo is set to 
	(0,0) indepently of source origo.
	Adds to the history.
	@return False, if source and destination image is the same. */
bool kB_CopyImage(TImage* pDest, const TImage* pSource);

/** Sets a border around image of size "Size", initialised to "Color"
	@return Always true */
bool kB_SetBorder(UINT16 Size, UINT32 Color, TImage* pInfo);

/** Sets the color of the border
	@return Always true */
bool kB_SetBorderColor(UINT32 Color, TImage* pInfo);

/** Copies rectangel from "pSource" to "pDest". Starting from (xmin,ymin) 
	(including min values) and ending at (xmax,ymax) (excluding max values).
	Previous data in "pDest" are destroyed. Origin is set to RAM.
	Adds to the history.
	@return false if rectangel is outside dimensions of source image */
bool kB_CopySubImage(int xmin,int ymin,int xmax,int ymax,
					 TImage* pDest,const TImage* pSource);

/** Iterate from this value
	@return min. X which is always 0 for TImage */
int kB_GetMinX(const TImage* Img);

/** Iterate from this value
	@return min. Y which is always 0 for TImage */
int kB_GetMinY(const TImage* Img);

/** Iterate to this value minus one (the image is zero indexed).
	@return max. X which always equals the width of image for TImage */
int kB_GetMaxX(const TImage* Img);

/** Iterate to this value minus one (the image is zero indexed).
	@return max. Y which always equals the width of image for TImage */
int kB_GetMaxY(const TImage* Img);

/** Checks if the given position is within range of the image by using
	the range methods kB_GetMinX(), kB_GetMaxX(), kB_GetMinY(), and kB_GetMaxY(). For
	the min values the comparison is x>=kB_GetMinX()-Delta and for the max.
	comparison it is x<kB_GetMaxX()+Delta. Similar with the y-value.
	@param x Position in x-direction.
	@param y Position in y-direction.
	@param Delta The delta value is subtracted from the min. ranges and
		added to the max. ranges.
	@return True, if the provided position is within range (including Delta)
		of the provided image. */
int kB_InRange(int x, int y, int Delta, const TImage* Img);

/** Prints the information contained in "pInfo" to stdout */
void kB_PrintImageInfo(const TImage* pInfo);

/** Sets the image matrix pointer in TImage structure, for internal use only.
	Note: only in this header file because the C++ CByteImage class needs to
	use this function once. */
void kB_SetImageMatrix(TImage* pInfo);

/** @defgroup byteimage_c_only ByteImage C only
	Kernel ANSI C only routines, last updated 12/5/1999.
	Basic ByteImage functions to be used as stand alone c
	functions only. The C++ part does not use these functions.
	@version 1.00
	@author René Dencker Eriksen edr@mip.sdu.dk
    @{ */

/** Returns a pixel value at position (x,y),
	including range check, slow access.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@return pixel value */
UINT32 kB_GetPixel(int x, int y,const TImage* pInfo);

/** Returns a pixel value at position (x,y),
	without range check, fast access.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@return pixel value */
UINT32 kB_GetPixelFast(int x, int y,const TImage* pInfo);

/** Sets a pixel at position (x,y),
	including range check, slow access.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@param Color The new pixel value, a palette index value.
	@return false if outside image or if b/p is not supported. */
bool kB_SetPixel(int x, int y, UINT32 Color,TImage* pInfo);

/** Sets a pixel at position (x,y),
	without range check, fast access.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@param Color The new pixel value, a palette index value.
	@return false if b/p is not supported. */
bool kB_SetPixelFast(int x, int y, UINT32 Color,TImage* pInfo);

/** For graytone images only, needs 1,4 and 8 b/p, produces an error
	in other cases. Uses bilinar interpolation.
	@param x The position in horisontal direction
	@param y The position in vertical direction
	@return interpolated pixel value */
float kB_GetPixelInterpolated(float x,float y, const TImage* pInfo);

/** A faster version of k_GetPixelInterpolated(), no checking of b/p
	and origo must be (0,0).
	@param x The position in horisontal direction
	@param y The position in vertical direction
	@return interpolated pixel value */
float kB_GetPixelInterpolatedFast(float x,float y, const TImage* pInfo);

/*@}*/ /* end group "byteimage_c_only" */

/*@}*/ /* end group "byteimage" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_B_KERNEL_FUNCTIONS_H */
