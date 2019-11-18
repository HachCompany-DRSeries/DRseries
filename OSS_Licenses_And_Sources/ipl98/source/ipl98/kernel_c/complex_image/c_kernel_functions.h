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

#ifndef _IPL98_C_KERNEL_FUNCTIONS_H
#define _IPL98_C_KERNEL_FUNCTIONS_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../ipl98_types.h"
#include "../kernel_io.h"
#include "../ipl98_types_c_only.h"
#include "../kernel_text.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
@{ */

/**	@defgroup compleximage ANSI-C ComplexImage functions
	Kernel ANSI C ComplexImage routines, last updated 8/21/2005.
	Basic image functions for pixels of type float (both a real an imaginar part), to be used as
	stand alone c functions, also used by the IPL98 C++ library.
	The functions works on the TComplexImage structure.
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 0.94
    @author Ivar Balslev, ivb@mip.sdu.dk
    @{ */

/** An image containing real and imaginary pixels of type FLOAT32.
	Always remember to call kC_InitComplexImage() after creating
	a variable of this type.
	
	@see TFileInfo */
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
	/** @param Pointer pointer to each scan line with complex real values inside the original image */
	FLOAT32** ppMatrixRe;
	/** @param Pointer pointer to each scan line with complex imaginar values inside the original image */
	FLOAT32** ppMatrixIm;
	/** @param Pointer to image and border data for complex real values. The pointer allways points 
		to allocated memory starting at border (internal use only) */
	FLOAT32* pPixelStreamRe;
	/** @param Pointer to image and border data for imaginar values. The pointer allways points to 
		allocated memory starting at border (internal use only) */
	FLOAT32* pPixelStreamIm;
	/** @param Pointer pointer to all scan lines with complex real values.	This includes border 
		in y-direction,	excl. border in x-dir (internal use only) */
	FLOAT32** ppAllScanLinesRe;
	/** @param Pointer pointer to all scan lines with complex imaginar values.
		This includes border in y-direction, excl. border in x-dir (internal use only) */
	FLOAT32** ppAllScanLinesIm; 
} TComplexImage;

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#include "../image/kernel_functions.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** Deletes all allocated memory for image, origo to (0,0), sets
	colors in pInfo to zero and all pointers to NULL.
	Origin is set to "EMPTY" */
void kC_EmptyImage(TComplexImage* pInfo);

/**	Sets all the pointers in pInfo to NULL, Origin
	to EMPTY and origo to (0,0), allways call this function
	when creating a new TComplexImage variable. */
void kC_InitImage(TComplexImage* pInfo);

/** Initialises a new image with given dimensions.
	All pixel values are initialised to 0. Sets
	Origin to "RAM", origo to (0,0) and size of border to 0.
	@return false, if pInfo is a NULL pointer.
	@see kC_AllocImageFast */
bool kC_AllocImage(UINT32 Width, UINT32 Height,TComplexImage* pInfo);

/** Initialises a new image with given dimensions.
	Pixel colors are not initialised. Sets
	Origin to "RAM", origo to (0,0) and size of border to 0.
	@return false, if pInfo is a NULL pointer.
	@see kC_AllocImage */
bool kC_AllocImageFast(UINT32 Width, UINT32 Height,TComplexImage* pInfo);

/**	Sets all pixels in the image to given color. Old pixel colors are destroyed.
	@param Color New pixel colors in image.
	@return False if pDest is empty or a NULL pointer */
bool kC_FlushImage(FLOAT32 Re, FLOAT32 Im,TComplexImage* pDest);

/** Load given file into "pInfo". The file name must include a relative
	or absolut path and the name of the file with extension, both characters
	'/' and '\' can be used to seperate folder names (remember to "escape" the
	character '\', ex. "c:\\test.bmp"). The type of the file is determined
	by the given extension. Origin is set to "DISC". */
bool kC_Load(const char* FileName, TComplexImage* pInfo);

/** Save given file to disc, the file name must comply with the same rules as
	in kC_Load.
	@see kC_Load */
bool kC_Save(const char* FileName, TComplexImage* pInfo);

/** Sets a border around image of size "Size", initialised to "Color"
	@param Size Size of border in pixels to be added in each direction.
	@param Color The color of the new border
	@return true if pInfo is not empty. */
bool kC_SetBorder(UINT16 Size,FLOAT32 Re,FLOAT32 Im,TComplexImage* pInfo);

/** Sets a border around image of size "Size", not initialised.
	@param Size Size of border in pixel to be added in each direction.
	@return true if pInfo is not empty. */
bool kC_SetBorderSize(UINT16 Size, TComplexImage* pInfo);

/** Sets the color of the border.
	@param Color The new border value.
	@return true if pInfo is not empty. */
bool kC_SetBorderColor(FLOAT32 Re, FLOAT32 Im, TComplexImage* pInfo);

/** Expands the image periodically in the border of size BorderSize.
	If a border of size BorderSize is present in pInfo it is used, otherwise
	a new border of BorderSize is created.
	@param BorderSize Size of border to copy image periodically to.
	@return false if BorderSize is bigger than either width or height of pInfo. */
bool kC_SetPeriodicBorder(unsigned short BorderSize, TComplexImage* pInfo);

/** Copies whole image from "pSource" to "pDest". All information is copied
	to the destination image including border pixels, palette and history.
	Previous data in "pDest" are destroyed, including the old history. 
	Origin is set to RAM. File information is left empty.
	@return False, if source image is empty. */
bool kC_CopyImage(TComplexImage* pDest,const TComplexImage* pSource);

/** Copy rectangel from "pSource" to "pDest". Starting from (xmin,ymin)
	(including min values) and ending at (xmax,ymax) (excluding max values).
	Previous data in "pDest" are destroyed. Origin is set to RAM.
	@return false if rectangel is outside dimensions of source image or
	if source image is empty. */
bool kC_CopySubImage(int xmin,int ymin,int xmax,int ymax,
	      		  TComplexImage* pDest,const TComplexImage* pSource);

/** Rotate image clockwise in step of 90 degrees. Note: Not speed optimised!
	@param Steps Rotate image 90 degrees * Steps */
void kC_Rotate90(int Steps,TComplexImage* pSource);

/** Flip image around a centered horisontal line. Note: Not speed optimised! */
void kC_FlipHorisontal(TComplexImage* pSource);

/** Flip image around a centered vertical line. Note: Not speed optimised! */
void kC_FlipVertical(TComplexImage* pSource);

/** Iterate from this value
	@return min. X relative to origo */
int kC_GetMinX(const TComplexImage* Img);

/** Iterate from this value
	@return min. Y relative to origo */
int kC_GetMinY(const TComplexImage* Img);

/** Iterate to this value minus one (the image is zero indexed).
	@return max. X relative to origo */
int kC_GetMaxX(const TComplexImage* Img);

/** Iterate to this value minus one (the image is zero indexed).
	@return max. Y relative to origo */
int kC_GetMaxY(const TComplexImage* Img);

/** Checks if the given position is within range of the image by using
	the range methods kC_GetMinX(), kC_GetMaxX(), kC_GetMinY(), and kC_GetMaxY(). For
	the min values the comparison is x>=kC_GetMinX()-Delta and for the max.
	comparison it is x<kC_GetMaxX()+Delta. Similar with the y-value.
	@param x Position in x-direction.
	@param y Position in y-direction.
	@param Delta The delta value is subtracted from the min. ranges and
		added to the max. ranges.
	@return True, if the provided position is within range (including Delta)
		of the provided image. */
int kC_InRange(int x, int y, int Delta, const TComplexImage* Img);

/** adds Bias to all pixels. The border (if present) is not
	affected. Adds to the history.
    @param pImg is a pointer to the source-target image of type TComplexImage
    @param Bias is the bias value
    @return False if *pImg is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kC_ApplyBias(FLOAT32 Bias,TComplexImage *pImg);

/** scales the real and imaginary colors using a constant multiplier.
	The border (if present) is not affected. Adds to the history.
    @param pImg is a pointer to the source-target image of type TComplexImage
    @param Prefactor is the scaling factor
    @return False if the image is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kC_ScaleIntensity(FLOAT32 Prefactor,TComplexImage *pImg);

/** calulates the sum of two TComplexImages.
	The border (if present) is not affected. Adds to the history.
    @param pImg is a pointer to the source-destination image of type TComplexImage
    @param pAddend is a pointer to the image of type TComplexImage to be added
    @return False if one or both of the images involved are empty or have different size
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kC_Add(TComplexImage *pImg,TComplexImage *pAddend);

/** calulates the difference of two TIntImages.
	The border (if present) is not affected. Adds to the history.
    @param pImg is a pointer to the source-destination image of type TIntImage
    @param pSubtrahend is a pointer to the image of type TIntImage to be subtracted
    @return False if one or both of the images involved are empty or have different size
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kC_Subtract(TComplexImage *pImg,TComplexImage *pSubtrahend);

/** calculates product of two images
	The border (if present) is not affected. Adds to the history.
    @param pImg is a pointer to the source-destination image of type TComplexImage
    @param pFactor a pointer to the image of type TComplexImage to be multiplied
    @return False if one or both of the images involved are empty or have different size
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kC_Multiply(TComplexImage *pImg, TComplexImage *pFactor);

/** calculates product of two images. *pFactor is complex conjugated
    before multiplication
	The border (if present) is not affected. Adds to the history.
    @param pImg is a pointer to the source-destination image of type TComplexImage
    @param pFactor a pointer to the image of type TComplexImage to be complex conjugated and multiplied
    @return False if one or both of the images involved are empty or have different size
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kC_CCFactorAndMultiply(TComplexImage *pImg,TComplexImage *pFactor);

/** changes sign of the imaginary part
	The border (if present) is not affected. Adds to the history.
    @param pImg is a pointer to the source-target image
    @return False if *pImg is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kC_ComplexConjugate(TComplexImage *pImg);

/** swaps diagonal quarters
	The border (if present) is not affected. Adds to the history.
    @param pImg is a pointer to the source image
    @return False if *pImg is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kC_Rearrange(TComplexImage *pImg);

/** performs a displacement so that (dx,dy)
    in the old picture becomes (0,0). Periodic continuation
    is assumed.
	The border (if present) is not affected. Adds to the history.
    @param dx is the horizontal displacement
    @param dy is the vertical displacement
    @param pImg is a pointer to the source image
    @return False if *pImg is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kC_CopyDisplace(int dx,int dy,TComplexImage *pImg);

/** copies an image into another of
    different size without scaling. The origin is retained. For
    increasing width/height, zero padding is applied. Simple cuts
    are applied for reduced width/height.
	The border (if present) is not affected. Adds to the history.
    @param pDest must be allocated prior to calling
    @param pSource is a pointer to the source image
    @return False if *pSource and/or *pDest are empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kC_CutOrZeroPad(TComplexImage *pSource, TComplexImage *Dest);

/** copies two images of type TImage into a TComplexImage as
    real and imaginary parts. If the image *pIm is empty or
    has dimensions different from *pRe, then the imaginary part
    becomes zero. *pDest attains the same dimensions as the *pRe.
	History from *pRe and pIm are copied to *pDest, old history in *pDest is deleted.
    @param pDest a pointer to destination image
    @param pRe a pointer to the source image to be the real part
    @param pIm a pointer to the source image to be the imaginary part
    @return False if the image *pRe is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kC_CopyFromByte(TComplexImage *pDest, TImage *pRe, TImage *pIm);

/** copies two images of type TIntImage into a TComplexImage as
    real and imaginary parts. If the image *pIm is empty or
    has dimensions different from *pRe, then the imaginary part
    becomes zero. *pDest attains the same dimensions as the  *pRe.
	History from *pRe and pIm are copied to *pDest, old history in *pDest is deleted.
    @param pRe a pointer to the source image to be the real part
    @param pIm a pointer to the source image to be the imaginary part
    @param pDest a pointer to the destination image
    @return False if the image *pRe is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kC_CopyFromInt(TComplexImage *pDest, TIntImage *pRe, TIntImage *pIm);

/** copies two images of type TFloatImage into a TComplexImage as
    real and imaginary parts. If the image *pIm is empty or
    has dimensions different from *pRe, then the imaginary part
    becomes zero. *pDest attains the same
    dimensions as the  pRe.
	History from *pRe and pIm are copied to *pDest, old history in *pDest is deleted.
    @param pRe a pointer to the source image to be the real part
    @param pIm a pointer to the source image to be the imaginary part
    @param pDest a pointer to the destination image
    @return False if the image *pRe
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool kC_CopyFromFloat(TComplexImage *pDest, TFloatImage *pRe, TFloatImage *pIm);

/** Prints the information contained in "pInfo" to stdout */
void kC_PrintImageInfo(const TComplexImage* pInfo);

/** Draws a line in image using the graphics routine k_GraphicsGenerateLine().
	For drawing ellipses and other symbols see graphic functions.
	@param Start Beginning of line, positions in the pixelgroup will start from this point.
	@param End End of line, positions in the pixelgroup will end with this point.
	@return False, if image is a NULL pointer.
	@see k_GraphicsGenerateLine */
bool kC_DrawLine(T2DInt Start, T2DInt End, FLOAT32 Color, TComplexImage *pDest);

/** Draws a circle in image using the graphics routine k_GraphicsGenerateCircle().
	For drawing ellipses and other symbols see graphic functions.
	@param Center Position for center of circle.
	@param Radius Radius of circle.
	@return False, if image is a NULL pointer.
	@see k_GraphicsGenerateCircle */
bool kC_DrawCircle(T2DInt Center, unsigned int Radius, FLOAT32 Color, TComplexImage *pDest);

/** @defgroup compleximage_c_only ComplexImage C only
	Kernel ANSI C only routines, last updated 11/24/1999.
	Basic ComplexImage functions to be used as stand alone c
	functions only. The C++ part does not use these functions.
	@version 0.90
	@author René Dencker Eriksen edr@mip.sdu.dk
    @{ */

/** returns the real pixel value at position (x,y),
	including range check.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@return the pixel value */
FLOAT32 kC_GetPixelRe(int x, int y,const TComplexImage* pInfo);

/** returns the imaginary pixel value at position (x,y),
	including range check.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@return the pixel value */
FLOAT32 kC_GetPixelIm(int x, int y,const TComplexImage* pInfo);

/** returns the real pixel value at position (x,y),
	without range check.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@return pixel value. */
FLOAT32 kC_GetPixelFastRe(int x, int y,const TComplexImage* pInfo);

/** returns the imaginary pixel value at position (x,y),
	without range check.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@return pixel value. */
FLOAT32 kC_GetPixelFastIm(int x, int y,const TComplexImage* pInfo);

/** Macro function for accessing real values in a complex image. The macro is very
	fast, but be careful when using the macro, there are no type and range checking.
	Origo always (0,0).
	@param x (int) x-position
	@param y (int) y-position
	@param Image (TComplexImage) The image to access pixel in. */
#define kC_GetPixelMacroRe(x,y,Image) ((Image).ppMatrixRe[y][x])

/** Macro function for accessing imaginar values in a complex image. The macro is very
	fast, but be careful when using the macro, there are no type and range checking.
	Origo always (0,0).
	@param x (int) x-position
	@param y (int) y-position
	@param Image (TComplexImage) The image to access pixel in. */
#define kC_GetPixelMacroIm(x,y,Image) ((Image).ppMatrixIm[y][x])

/** sets both the real (Re) and imaginary (Im) complex pixel value 
	at position (x,y), including range check, slow acces.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@return false if outside image. */
bool kC_SetPixel(int x, int y,FLOAT32 Re,FLOAT32 Im,TComplexImage* pInfo);

/** sets both the real (Re) and imaginary (Im) complex pixel value 
	at position (x,y), without range check, fast access.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@return false if outside image. */
bool kC_SetPixelFast(int x, int y, FLOAT32 Re, FLOAT32 Im,TComplexImage* pInfo);

/** sets only the real pixel value at position (x,y),
	including range check, slow access.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@return false if outside image. */
bool kC_SetPixelRe(int x, int y, FLOAT32 Re,TComplexImage *pInfo);

/** sets only the imaginary pixel value at position (x,y),
	including range check, slow access.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@return false if outside image. */
bool kC_SetPixelIm(int x, int y, FLOAT32 Im,TComplexImage *pInfo);

/** sets only the real pixel value at position (x,y),
	without range check, fast access.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@return true (always) */
bool kC_SetPixelFastRe(int x, int y, FLOAT32 Re,TComplexImage *pInfo);

/** sets only the imaginary pixel value at position (x,y),
	without range check, fast access.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@return true (always) */
bool kC_SetPixelFastIm(int x, int y, FLOAT32 Im,TComplexImage *pInfo);

/** Macro function for setting real values in a complex image. The macro is very
	fast, but be carefull when using the macro, there are no type and range checking.
	Origo always (0,0).
	@param x (int) x-position
	@param y (int) y-position
	@param Image (TComplexImage) The image to set pixel in. */
#define kC_SetPixelMacroRe(x,y,Color,Image) ((Image).ppMatrixRe[y][x]=Color)

/** Macro function for setting imaginar values in a complex image. The macro is very
	fast, but be carefull when using the macro, there are no type and range checking.
	Origo always (0,0).
	@param x (int) x-position
	@param y (int) y-position
	@param Image (TComplexImage) The image to set pixel in. */
#define kC_SetPixelMacroIm(x,y,Color,Image) ((Image).ppMatrixIm[y][x]=Color)
/*@}*/ /* end group "compleximage_c_only" */

/*@}*/ /* end group "compleximage" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_C_KERNEL_FUNCTIONS_H */
