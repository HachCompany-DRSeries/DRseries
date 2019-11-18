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

#ifndef _IPL98_I_KERNEL_FUNCTIONS_H
#define _IPL98_I_KERNEL_FUNCTIONS_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../ipl98_types.h"
#include "../ipl98_types_c_only.h"
#include "../kernel_io.h"
#include "../kernel_text.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/**	@defgroup intimage ANSI-C IntImage functions
	Kernel ANSI C IntImage routines, last updated 8/21/2005.
	Basic image functions for pixels of type float, to be used as
	stand alone c functions, also used by the IPL98 C++ library.
	The functions works on the TIntImage structure.
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 1.44
	@author René Dencker Eriksen edr@mip.sdu.dk (except for arithmetic and kI_CopyDisplace methods)
			Ivar Balslev ivb@mip.sdu.dk (arithmetic and kI_CopyDisplace)
    @{ */

/** An image containing pixels of type INT16.
	Always remember to call kI_InitIntImage() after creating
	a variable of this type.
	@see TFileInfo
	@see TStatistic */
typedef struct {
	/** @param File name and path info */
	TFileInfo FileInfo;
	/** @param String list with information about algorithm performed on this image */
	TIplText History;
	/** @param Originating point of image */
	ORIGIN Origin;
	/** @param Width of bitmap in pixels */
	UINT32 Width;
	/** @param Height of bitmap in pixels */
	UINT32 Height;
	/** @param Size of image in bytes (incl. zero padding and border) */
	UINT32 TotalSize;
	/** @param Width in bytes of each scanline incl. zero padding and border (SizeImage/Height) */
	UINT16 ByteWidth;
	/** @param Size of border */
	UINT16 BorderSize;
	/** @param Origo in image */
	T2DInt Origo;
	/** @param Pointer pointer to each scan line inside image */
	INT16** ppMatrix;
	/** @param Pointer to image and border data. The pointer allways points to 
		allocated memory starting at border (internal use only) */
	INT16* pPixelStream;
	/** @param Pointer pointer to all scan lines. This includes border in y-direction, 
		excl. border in x-dir (internal use only) */
	INT16** ppAllScanLines;
} TIntImage;

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#include "../image/kernel_functions.h"
#include "../float_image/f_kernel_functions.h"
#include "../complex_image/c_kernel_functions.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** Deletes all allocated memory for image, origo reset to (0,0), sets
	values in pInfo to zero and all pointers to NULL. All structures inside TIntImage
	is emptied with their corresponding empty-function.
	Origin is set to "EMPTY", The history is cleared. */
void kI_EmptyImage(TIntImage* pInfo);

/**	Sets all the pointers in pInfo to NULL, Origin
	to EMPTY and origo to (0,0). All structures inside TImage
	is initialised with their corresponding init-function. 
	Allways call this function 
	when creating a new TIntImage variable. */
void kI_InitImage(TIntImage* pInfo);

/** Initialises a new image with given dimensions.
	All pixel colors are initialised to 0. Sets
	Origin to "RAM", origo to (0,0), size of border to 0 
	and clears the history.
	@param Width The width of image in pixels.
	@param Height The height of image in pixels.
	@return false, if pInfo is a NULL pointer.
	@see kI_AllocImageFast */
bool kI_AllocImage(UINT32 Width, UINT32 Height,TIntImage* pInfo);

/** Initialises a new image with given dimensions.
	Pixel colors are not initialised. Sets
	Origin to "RAM", origo to (0,0), size of border to 0 
	and clears the history.
	@param Width The width of image in pixels.
	@param Height The height of image in pixels.
	@return false, if pInfo is a NULL pointer
	@see kI_AllocImage */
bool kI_AllocImageFast(UINT32 Width, UINT32 Height,TIntImage* pInfo);

/**	Sets all pixels in the image to given color. Old pixel colors are destroyed.
	Adds to the history.
	@param Color New pixel colors in image.
	@return False if pDest is empty or a NULL pointer */
bool kI_FlushImage(INT16 Color,TIntImage* pDest);

/** Loads given file into "pInfo", all previous information in pInfo is destroyed. 
	The file name must include a relative
	or absolut path and the name of the file with extension, both characters
	'/' and '\' can be used to seperate folder names (remember to "escape" the
	character '\', ex. "c:\\test.bmp"). The type of the file is determined
	by the given extension. Origin is set to "DISC".
	Adds to the history.
	@param PathAndFileName Filename (including the path) to be loaded.
	@return False if the syntacs of PathAndFileName is wrong, the file is not found
		or the format of the file is wrong. */
bool kI_Load(const char* PathAndFileName, TIntImage* pInfo);

/** Saves given file to disc, the file name must comply with the same rules as
	in kI_Load.
	Adds to the history.
	@param PathAndFileName Filename (including the path) to be loaded.
	@return False if the syntacs of PathAndFileName is wrong or the path is not found.
	@see kI_Load */
bool kI_Save(const char* PathAndFileName, TIntImage* pInfo);

/** Sets a border around image of size "Size", initialised to "Color"
	@param Size Size of border in pixels to be added in each direction.
	@param Color The color of the new border
	@return true if pInfo is not empty. */
bool kI_SetBorder(UINT16 Size, INT16 Color, TIntImage* pInfo);

/** Sets a border around image of size "Size", not initialised.
	@param Size Size of border in pixel to be added in each direction.
	@return true if pInfo is not empty. */
bool kI_SetBorderSize(UINT16 Size, TIntImage* pInfo);

/** Sets the color of the border.
	@param Color The new border value.
	@return true if pInfo is not empty. */
bool kI_SetBorderColor(INT16 Color, TIntImage* pInfo);

/** Expands the image periodically in the border of size BorderSize.
	If a border of size BorderSize is present in pInfo it is used, otherwise
	a new border of BorderSize is created.
	@param BorderSize Size of border to copy image periodically to.
	@return false if BorderSize is bigger than either width or height of pInfo. */
bool kI_SetPeriodicBorder(unsigned short BorderSize, TIntImage* pInfo);

/** Copies whole image from "pSource" to "pDest". All information is copied
	to the destination image including border pixels, palette and history.
	Previous data in "pDest" are destroyed, including the old history. 
	Origin is set to RAM. File information is left empty.
	@return False, if source and destination image is the same. */
bool kI_CopyImage(TIntImage* pDest,const TIntImage* pSource);

/** Copies rectangel from "pSource" to "pDest". Starting from (xmin,ymin) 
	(including min values) and ending at (xmax,ymax) (excluding max values).
	Previous data in "pDest" are destroyed. Origin is set to RAM.
	Adds to the history.
	@return false if rectangel is outside dimensions of source image or
	if source image is empty. */
bool kI_CopySubImage(int xmin,int ymin,int xmax,int ymax,
				  TIntImage* pDest,const TIntImage* pSource);

/** Rotates image clockwise in step of 90 degrees. Note: Not speed optimised!
	Adds to the history.
	@param Steps Rotate image 90 degrees * Steps */
void kI_Rotate90(int Steps,TIntImage* pSource);

/** Adds to the history.
	Flips image around a centered horisontal line. Note: Not speed optimised! */
void kI_FlipHorisontal(TIntImage* pSource);

/** Flips image around a centered vertical line. Note: Not speed optimised! */
void kI_FlipVertical(TIntImage* pSource);

/** Iterate from this value
	@return min. X relative to origo */
int kI_GetMinX(const TIntImage* Img);

/** Iterate from this value
	@return min. Y relative to origo */
int kI_GetMinY(const TIntImage* Img);

/** Iterate to this value minus one (the image is zero indexed).
	@return max. X relative to origo */
int kI_GetMaxX(const TIntImage* Img);

/** Iterate to this value minus one (the image is zero indexed).
	@return max. Y relative to origo */
int kI_GetMaxY(const TIntImage* Img);

/** Checks if the given position is within range of the image by using
	the range methods kI_GetMinX(), kI_GetMaxX(), kI_GetMinY(), and kI_GetMaxY(). For
	the min values the comparison is x>=kI_GetMinX()-Delta and for the max.
	comparison it is x<kI_GetMaxX()+Delta. Similar with the y-value.
	@param x Position in x-direction.
	@param y Position in y-direction.
	@param Delta The delta value is subtracted from the min. ranges and
		added to the max. ranges.
	@return True, if the provided position is within range (including Delta)
		of the provided image. */
int kI_InRange(int x, int y, int Delta, const TIntImage* Img);

/** adds Bias to all pixels. pSource may be equal to pDest.
	The border (if present) is not affected. Adds to the history.
	@param pImg is a pointer to the source-target image of type TIntImage
	@param Bias is the bias value
	@return False if *pImg is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kI_ApplyBias(INT16 Bias,TIntImage *pImg);

/** scales the integer colors using a constant multiplier.
	The border (if present) is not affected. Adds to the history.
	@param pImg is a pointer to the source-target image of type TIntImage
	@param Prefactor is the scaling factor
	@return False if *pImg is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kI_ScaleIntensity(float Prefactor,TIntImage *pImg);

/** calulates the sum of two TIntImages.
	The border (if present) is not affected. Adds to the history.
	@param pAddend is a pointer to the image of type TIntImage to be added
	@param pImg is a pointer to the source-destination image of type TIntImage
	@return False if one or both of the images involved are empty or have different size
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kI_Add(TIntImage *pImg,TIntImage *pAddend);

/** calulates the difference of two TIntImages.
	The border (if present) is not affected. Adds to the history.
	@param pSubtrahend is a pointer to the image of type TIntImage to be subtracted
	@param pImg is a pointer to the source-destination image of type TIntImage
	@return False if one or both of the images involved are empty or have different size 
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kI_Subtract(TIntImage *pImg,TIntImage *pSubtrahend);

/** calculates product of two images
	The border (if present) is not affected. Adds to the history.
	@param pFactor a pointer to the image of type TIntImage to be multiplied
	@param pImg is a pointer to the source-destination image of type TIntImage
	@return False if one or both of the images involved are empty or have different size
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kI_Multiply(TIntImage *pImg,TIntImage *pFactor);

/** performs a displacement so that (dx,dy)
	in the old picture becomes (0,0). Periodic continuation
	is assumed.
	The border (if present) is not affected. Adds to the history.
	@param pImg is a pointer to the source image
	@param dx is the horizontal displacement
	@param dy is the vertical displacement
	@return False if *pImg is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kI_CopyDisplace(int dx,int dy,TIntImage *pImg);

/** copies an image into another of
	different size without scaling. The origin is retained. For
	increasing width/height, zero padding is applied. Simple cuts
	are applied for reduced width/height.
	The border (if present) is not affected. Adds to the history.
	@param pSource is a pointer to the source image
	@param pDest must be allocated prior to calling
	@return False if *pSource and/or *pDest are empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kI_CutOrZeroPad( TIntImage *Dest,TIntImage *pSource);

/** copies a TImage into an TIntImage. *pDest attains the same dimensions
	as the *pSource.
	History from *pSource is copied to *pDest, old history in *pDest is deleted.
	@param pSource a pointer to the source image
	@param pDest a pointer to the destination image
	@return False if source image is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kI_CopyFromByte(TIntImage *pDest,TImage *pSource);

/** copies a TFloatImage into a TIntImage. *pDest attains the same dimensions
	as the *pSource.
	Note that this is a straight forward convertion, values are truncated
	directly to the destination range. For more advanced convertions look
	at the functions available in local operation .
	History from *pSource is copied to *pDest, old history in *pDest is deleted.
	@param pSource a pointer to the source image
	@param pDest a pointer to the destination image
	@return False if source image is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kI_CopyFromFloat(TIntImage *pDest,TFloatImage *pSource);

/** copies partially a TComplexImage into a TIntImage.
	*pDest attains the same dimensions as *PSource, and mode determines
	which part of the source image is copied.
	Note that this is a straight forward convertion, values are truncated
	directly to the destination range. For more advanced convertions look
	at the functions available in local operation .
	History from *pSource is copied to *pDest, old history in *pDest is deleted.
	@param mode Determines what part of the complex image is to be
		copied. The values are COMPLEX_REAL, COMPLEX_IMAGINARY, or COMPLEX_MODULUS.
	@param pDest a pointer to the destination image
	@param pSource a pointer to the source image
	@return False if source image is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kI_CopyFromComplex(COMPLEXCHOISE mode, TIntImage *pDest,TComplexImage *pSource);

/** Prints the information contained in "pInfo" to stdout */
void kI_PrintImageInfo(const TIntImage* pInfo);

/** Draws a line in image using the graphics routine k_GraphicsGenerateLine().
	For drawing ellipses and other symbols see graphic functions.
	@param Start Beginning of line, positions in the pixelgroup will start from this point.
	@param End End of line, positions in the pixelgroup will end with this point.
	@return False, if image is a NULL pointer.
	@see k_GraphicsGenerateLine */
bool kI_DrawLine(T2DInt Start, T2DInt End, INT16 Value, TIntImage *pDest);

/** Draws a circle in image using the graphics routine k_GraphicsGenerateCircle().
	For drawing ellipses and other symbols see graphic functions.
	@param Center Position for center of circle.
	@param Radius Radius of circle.
	@return False, if image is a NULL pointer.
	@see k_GraphicsGenerateCircle */
bool kI_DrawCircle(T2DInt Center, unsigned int Radius, INT16 Value, TIntImage *pDest);

/** @defgroup intimage_c_only IntImage C only
	Kernel ANSI C only routines, last updated 12/5/1999.
	Basic IntImage functions to be used as stand alone c
	functions only. The C++ part does not use these functions.
	@version 1.00
	@author René Dencker Eriksen edr@mip.sdu.dk
    @{ */

/** including range check, slow access.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@return pixel value */
INT16 kI_GetPixel(int x, int y,const TIntImage* pInfo);

/** returns the pixel value at position (x,y),
	without range check, fast access.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@return pixel value */
INT16 kI_GetPixelFast(int x, int y,const TIntImage* pInfo);

/** Macro function for accessing pixels in a float image. The macro is very
	fast, but be carefull when using the macro, there are no type and range checking.
	Origo always (0,0).
	@param x (int) x-position
	@param y (int) y-position
	@param Image (TIntImage) The image to access pixel in. */
#define kI_GetPixelMacro(x,y,Image) ((Image).ppMatrix[y][x])

/** Sets a pixel at position (x,y),
	including range check, slow access.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@param Color The new pixel value.
	@return false if outside image */
bool kI_SetPixel(int x, int y, INT16 Color,TIntImage* pInfo);

/** sets a pixel at position (x,y),
	without range check, fast access.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@param Color The new pixel value. */
bool kI_SetPixelFast(int x, int y, INT16 Color,TIntImage* pInfo);

/** Macro function for setting pixels in a float image. The macro is very
	fast, but be carefull when using the macro, there are no type and range checking.
	Origo always (0,0).
	@param x (int) x-position
	@param y (int) y-position
	@param Image (TIntImage) The image to set pixel in. */
#define kI_SetPixelMacro(x,y,Color,Image) ((Image).ppMatrix[y][x]=Color)

/** returns the interpolated value at position (x,y). Uses bilinar interpolation.
	@param x The position in horisontal direction
	@param y The position in vertical direction
	@return interpolated pixel value 
	@version 0.90 */
float kI_GetPixelInterpolated(float x,float y, const TIntImage* pInfo);

/** A faster version of kI_GetPixelInterpolated(), no range checking
	and origo must be (0,0).
	@param x The position in horisontal direction
	@param y The position in vertical direction
	@return interpolated pixel value 
	@version 0.90 */
float kI_GetPixelInterpolatedFast(float x,float y, const TIntImage* pInfo);

/*@}*/ /* end group "intimage_c_only" */

/*@}*/ /* end group "intimage" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_I_KERNEL_FUNCTIONS_H */
