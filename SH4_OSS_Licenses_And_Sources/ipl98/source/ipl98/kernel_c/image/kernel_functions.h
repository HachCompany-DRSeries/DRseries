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

#ifndef _IPL98_KERNEL_FUNCTIONS_H
#define _IPL98_KERNEL_FUNCTIONS_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../kernel_io.h"
#include "../kernel_palette.h"
#include "../ipl98_types.h"
#include "../ipl98_types_c_only.h"
#include "../kernel_text.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup image ANSI-C Image functions
	Kernel ANSI C image routines, last updated 8/21/2005.
	Basic image and palette functions to be used as stand alone c
	functions, also used by the IPL98 C++ library. The functions
	works on the TImage and TPalette structures.
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 1.45
	@author René Dencker Eriksen edr@mip.sdu.dk (except arithmetic and k_CopyDisplace)
			Ivar Balslev ivb@mip.sdu.dk (arithmetic and k_CopyDisplace)
    @{ */

/** All the information about a general image.
	Pixels consists of either 1, 8 or 24 bits per pixel.
	This type can be saved to disk as a graphic file-type (BMP or PGM).
	Always remember to call k_InitImage() after creating
	a variable of this type.
	The members in the struct can be seen under "parameters"
	
	@see TPalette
	@see TFileInfo
	@see TIplText
	@see TStatistic */
typedef struct {
	/** @param Palette data */
	TPalette Pal; /* Palette data */
	/** @param File name and path info */
	TFileInfo FileInfo;
	/** @param String list with information about algorithm performed on this image */
	TIplText History;
	/** @param Originating point of image */
	ORIGIN Origin;
	/** @param Width of bitmap in pixels */
	UINT32 Width;
	/** @param Height of bitmap in pixels  */
	UINT32 Height;
	/** @param Bits per pixel (bit planes): 1,4,8 or 24 */
	UINT16 Bits;
	/** @param Size of image in bytes (incl. zero padding and border) */
	UINT32 TotalSize;
	/** @param Width in bytes of each scanline incl. zero padding and border (SizeImage/Height) */
	UINT16 ByteWidth;
	/** @param Size of border */
	UINT16 BorderSize;
	/** @param Origo in image */
	T2DInt Origo;
	/** @param Pointer pointer to each scan line inside image */
	UINT8** ppMatrix;
	/** @param Pointer to image and border data. The pointer allways points to 
		allocated memory starting at border (internal use only) */
	UINT8* pPixelStream;
	/** @param Pointer pointer to all scan lines. This includes border in 
		y-direction, excl. border in x-dir (internal use only) */
	UINT8** ppAllScanLines;
} TImage;

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#include "../byte_image/b_kernel_functions.h"
#include "../int_image/i_kernel_functions.h"
#include "../float_image/f_kernel_functions.h"
#include "../complex_image/c_kernel_functions.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** Deletes all allocated memory for image, origo reset to (0,0), sets
	values in pInfo to zero and all pointers to NULL. All structures inside TImage
	is emptied with their corresponding empty-function.
	Origin is set to "EMPTY" and the history is cleared. */
void k_EmptyImage(TImage* pInfo);

/**	Sets all the pointers in pInfo to NULL, Origin
	to EMPTY and origo to (0,0). All structures inside TImage
	is initialised with their corresponding init-function. Allways call this 
	function when creating a new TImage variable. */
void k_InitImage(TImage* pInfo);

/** Initialises a new image with given dimensions and bits per pixel.
	All pixel values are initialised to 0. Sets
	Origin to "RAM", origo to (0,0), size of border to 0 and clears 
	the history. Allocates memory 
	for palette and initialises (in case of 1, 4 or 8 b/p) to gray tones.
	Black is zero and white is 1, 15 or 255.
	@param Width The width of image in pixels.
	@param Height The height of image in pixels.
	@return false, if Bits is not 1,8 or 24 or if creation of palette fails.
	@see k_AllocImageFast */
bool k_AllocImage(UINT32 Width, UINT32 Height, UINT16 Bits,TImage* pInfo);

/** Initialises a new image with given dimensions and bits per pixel.
	Pixel values are not initialised. Sets
	Origin to "RAM", origo to (0,0), size of border to 0 and clears 
	the history. Allocates memory 
	for palette and initialises (in case of 1, 4 or 8 b/p) to gray tones.
	Black is zero and white is 1, 15 or 255.
	@param Width The width of image in pixels.
	@param Height The height of image in pixels.
	@return false, if Bits is not 1,8 or 24 or if creation of palette fails.
	@see k_AllocImage */
bool k_AllocImageFast(UINT32 Width, UINT32 Height, UINT16 Bits,TImage* pInfo);

/**	Sets all pixels in the image to given color. Old pixel values are destroyed.
	Adds to the history.
	@param Color New pixel values in image.
	@return False if pDest is empty or a NULL pointer */
bool k_FlushImage(UINT32 Color,TImage* pDest);

/**	Inverts all pixel values in image. Works on all pixel depths (i.e. 1, 8 and 24 b/p).
	Adds to the history.
	@return False if pDest is empty or a NULL pointer */
bool k_InvertImage(TImage* pInfo);

/** Loads given file into "pInfo", all previous information in pInfo is destroyed.
	At the moment the following formats are supported:
	BMP: The windows BMP format in 1, 8 and 24 bits per pixel.
	PGM: Portable Gray Map, only in binary format (P5) and with 255 gray tones.
		 History present in comment lines is loaded into the TImage.History variable.
	PCD: Kodak PhotoCD format, loads in maximum possible non-huffmann encoded
		 resolution. This implies that the maximum possible resolution for this
		 loader is 768x512, even if the image is saved in higher resolutions.
	The file name must include a relative
	or absolut path and the name of the file with extension, both characters
	'/' and '\' can be used to seperate folder names (remember to "escape" the
	character '\', ex. "c:\\test.bmp"). The type of the file is determined
	by the given extension. Origin is set to "DISC".
	Adds to the history.
	@param PathAndFileName Filename (including the path) to be loaded.
	@return False if the syntacs of PathAndFileName is wrong, the file is not found
		or the format of the file is wrong. */
bool k_Load(const char* PathAndFileName, TImage* pInfo);

/** Saves given file to disc, the file name must comply with the same rules as
	in k_Load.
	At the moment the following formats are supported:
	BMP: The windows BMP format in 1, 8 and 24 bits per pixel.
	PGM: Portable Gray Map, only in binary format (P5) and with 255 gray tones.
		 History present in comment lines is loaded into the TImage.History variable.
	Adds to the history.
	@param PathAndFileName Filename (including the path) to be loaded.
	@return False if the syntacs of PathAndFileName is wrong or the path is not found.
	@see kB_Load */
bool k_Save(const char* PathAndFileName, TImage* pInfo);

/** Returns the red component value at position (x,y), including range check.
	Slow access. If the image contains a palette this function takes care 
	of finding the color value from the palette index and extracting the
	red	component. If no palette is present it extracts the red component
	directly from the pixel value.
	@param x Position in vertical direction.
	@param y Position in horisontal direction.
	@return Red component value. */
UINT8 k_GetRedComponent(int x, int y, const TImage* pInfo);

/** Returns the red component value at position (x,y), without range check.
	Fast access. If the image contains a palette this function takes care 
	of finding the color value from the palette index and extracting the
	red	component. If no palette is present it extracts the red component
	directly from the pixel value.
	@param x Position in vertical direction.
	@param y Position in horisontal direction.
	@return Red component value. */
UINT8 k_GetRedComponentFast(int x, int y, const TImage* pInfo);

/** Returns the green component value at position (x,y), including range check.
	Slow access. If the image contains a palette this function takes care 
	of finding the color value from the palette index and extracting the
	green component. If no palette is present it extracts the green component
	directly from the pixel value.
	@param x Position in vertical direction.
	@param y Position in horisontal direction.
	@return Green component value. */
UINT8 k_GetGreenComponent(int x, int y, const TImage* pInfo);

/** Returns the green component value at position (x,y), without range check.
	Fast access. If the image contains a palette this function takes care 
	of finding the color value from the palette index and extracting the
	green	component. If no palette is present it extracts the green component
	directly from the pixel value.
	@param x Position in vertical direction.
	@param y Position in horisontal direction.
	@return Green component value. */
UINT8 k_GetGreenComponentFast(int x, int y, const TImage* pInfo);

/** Returns the blue component value at position (x,y), including range check.
	Slow access. If the image contains a palette this function takes care 
	of finding the color value from the palette index and extracting the
	blue component. If no palette is present it extracts the blue component
	directly from the pixel value.
	@param x Position in vertical direction.
	@param y Position in horisontal direction.
	@return Green component value. */
UINT8 k_GetBlueComponent(int x, int y, const TImage* pInfo);

/** Returns the blue component value at position (x,y), without range check.
	Fast access. If the image contains a palette this function takes care 
	of finding the color value from the palette index and extracting the
	blue component. If no palette is present it extracts the blue component
	directly from the pixel value.
	@param x Position in vertical direction.
	@param y Position in horisontal direction.
	@return Green component value. */
UINT8 k_GetBlueComponentFast(int x, int y, const TImage* pInfo);

/** Sets red component for position (x,y), leave other components unchanged.
	Including range check, slow access.
	@param x Position in vertical direction.
	@param y Position in horisontal direction.
	@return False if position is outside image or image is not 24 b/p. */
bool k_SetRedComponent(int x, int y, UINT8 RedValue, const TImage* pInfo);

/** Sets red component for position (x,y), leave other components unchanged.
	Without range check, fast access.
	@param x Position in vertical direction.
	@param y Position in horisontal direction.
	@return False if image is not 24 b/p. */
bool k_SetRedComponentFast(int x, int y, UINT8 RedValue, const TImage* pInfo);

/** Sets green component for position (x,y), leave other components unchanged.
	Including range check, slow access.
	@param x Position in vertical direction.
	@param y Position in horisontal direction.
	@return False if position is outside image or image is not 24 b/p. */
bool k_SetGreenComponent(int x, int y, UINT8 GreenValue, const TImage* pInfo);

/** Sets green component for position (x,y), leave other components unchanged.
	Without range check, fast access.
	@param x Position in vertical direction.
	@param y Position in horisontal direction.
	@return False if image is not 24 b/p. */
bool k_SetGreenComponentFast(int x, int y, UINT8 GreenValue, const TImage* pInfo);

/** Sets blue component for position (x,y), leave other components unchanged.
	Including range check, slow access.
	@param x Position in vertical direction.
	@param y Position in horisontal direction.
	@return False if position is outside image or image is not 24 b/p. */
bool k_SetBlueComponent(int x, int y, UINT8 BlueValue, const TImage* pInfo);

/** Sets blue component for position (x,y), leave other components unchanged.
	Without range check, fast access.
	@param x Position in vertical direction.
	@param y Position in horisontal direction.
	@return False if image is not 24 b/p. */
bool k_SetBlueComponentFast(int x, int y, UINT8 BlueValue, const TImage* pInfo);

/** Sets a border around image of size "Size", initialised to "Color"
	If pInfo->Bits=1 then "Size" is rounded up to nearest muliplum of 8
	to make sure that image data begins on a byte boundary.
	@param Size Size of border in pixel to be added in each direction.
	@param Color The color of the new border
	@return true if bits is supported (i.e. 1, 8 and 24 b/p) */
bool k_SetBorder(UINT16 Size, UINT32 Color, TImage* pInfo);

/** Sets a border around image of size "Size", not initialised.
	If pInfo->Bits=1 then "Size" is rounded up to nearest muliplum of 8
	to make sure that image data begins on a byte boundary.
	@param Size Size of border in pixel to be added in each direction.
	@return true if bits is supported (i.e. 1, 8 and 24 b/p) */
bool k_SetBorderSize(UINT16 Size, TImage* pInfo);

/** Sets the color of the border.
	@param Color The new border color.
	@return true if bits is supported (i.e. 1, 8 and 24 b/p) */
bool k_SetBorderColor(UINT32 Color, TImage* pInfo);

/** Expands the image periodically in the border of size BorderSize.
	If a border of size BorderSize is present in pInfo it is used, otherwise
	a new border of BorderSize is created.
	@param BorderSize Size of border to copy image periodically to.
	@return false if BorderSize is bigger than either width or height of pInfo
		or if b/p is not 1,8 or 24. */
bool k_SetPeriodicBorder(unsigned short BorderSize, TImage* pInfo);

/** Copies whole image from "pSource" to "pDest". All information is copied
	to the destination image including border pixels, palette and history.
	Previous data in "pDest" are destroyed, including the old history. 
	Origin is set to RAM. File information is left empty. The function
	is fastest if pDest has same dimensions as pSource! In that case no
	reallocation of destination image is needed. 
	@return False, if source and destination image is the same. */
bool k_CopyImage(TImage* pDest,const TImage* pSource);

/** Copies rectangel from "pSource" to "pDest". Starting from (xmin,ymin) 
	(including min values) and ending at (xmax,ymax) (excluding max values).
	Previous data in "pDest" are destroyed. Origin is set to RAM. If you want the
	resulting image placed in the source image just use the same pointer for both
	parameters "pSource" and "pDest". Note: When "pSource" and "pDest" points to
	same image the function is a bit slower (needs to copy a temporary image to
	the destination). The function is fastest if pDest is already allocated
	with the dimensions ((xmax-xmin),(ymax-ymin))!
	In that case no reallocation of destination image is needed.
	Adds to the history.
	@param pDest Destination of new image, may be the same as pSource. Note:
		In case of same source and destination the algorithm is a bit slower.
	@param pSource Source image, may be the same as pDest. Note:
		In case of same source and destination the algorithm is a bit slower.
	@return false if rectangel is outside dimensions of source image or
		if source image is empty. */
bool k_CopySubImage(int xmin,int ymin,int xmax,int ymax,
				  TImage* pDest,const TImage* pSource);

/** Copies a whole image to a new one with a different pixel depth, if the new
	bits per pixel is the same as source image b/p the function
	k_CopyImage is used. Convertion from 1 to 8 b/p assumes black as 0-values
	and 1 as white values in the source image. Convertion from 1 to 24 b/p and
	8 to 24 b/p looks up the palette value in the source image and sets it in
	the destination - it works for both gray tones and color images. For 8 to 1 b/p
	a threshold of 128 is used (should be used for gray tone images only).
	For 24 to 8 b/p the mean value of the R, G and B component is used, i.e.
	the new image is a gray tone. For 24 to 1 b/p the method for 24 to 8 b/p
	is used followed by a thresholding with 128. If you want the resulting image
	placed in the source image just use the same pointer for both parameters
	"pSource" and "pDest". Note: When "pSource" and "pDest" points to same image
	the function is a bit slower (needs to copy a temporary image to the destination).
	To optimize speed let pSource and pDest be different variables, the function
	is even faster if pDest has same dimensions as pSource! In that case no
	reallocation of destination image is needed. 
	Adds to the history.
	@param pDest Destination of new image, may be the same as pSource. Note:
		In case of same source and destination the algorithm is a bit slower.
	@param pSource Source image, may be the same as pDest. Note:
		In case of same source and destination the algorithm is a bit slower.
	@param DestBits Bits per pixel in destination image.
	@return False, if source image is empty or if DestBits is not 1, 8 or 24. */
bool k_CopyConvert(UINT16 DestBits, TImage* pDest, const TImage* pSource);


/** Converts an image from 8 to 1 b/p with a given threshold.
	Same functionality as k_CopyConvert with a fixed conversion from 8 b/p
	to 1 b/p but here with a threshold as parameter. The source image must 
	have 8 b/p with a graytone palette.
	@param pDest Destination of new image, may be the same as pSource. Note:
		In case of same source and destination the algorithm is a bit slower.
	@param Threshold Pixel values below this value is converted to black, the other
		are converted to white.
	@param pSource Source image with 8 b/p, may be the same as pDest. Note:
		In case of same source and destination the algorithm is a bit slower.
	@return False, if source image is empty or not 8 b/p. */
bool k_CopyConvertThreshold(TImage* pDest, UINT8 Threshold, const TImage* pSource);

/** Rotates image clockwise in step of 90 degrees. Note: Not speed optimised!
	Adds to the history.
	@param Steps Rotate image 90 degrees * Steps */
void k_Rotate90(int Steps,TImage* pSource);

/** Flips image around a centered horisontal line. Note: Not speed optimised!
	Adds to the history. */
void k_FlipHorisontal(TImage* pSource);

/** Flips image around a centered vertical line. Note: Not speed optimised! 
	Adds to the history. */
void k_FlipVertical(TImage* pSource);

/** Iterate from this value, border is not included.
	@return min. X relative to origo */
int k_GetMinX(const TImage* Img);

/** Iterate from this value, border is not included.
	@return min. Y relative to origo */
int k_GetMinY(const TImage* Img);

/** Iterate to this value minus one (the image is zero indexed), 
	border is not included.
	@return max. X relative to origo */
int k_GetMaxX(const TImage* Img);

/** Iterate to this value minus one (the image is zero indexed), 
	border is not included.
	@return max. Y relative to origo */
int k_GetMaxY(const TImage* Img);

/** Checks if the given position is within range of the image by using
	the range methods k_GetMinX(), k_GetMaxX(), k_GetMinY(), and k_GetMaxY(). For
	the min values the comparison is x>=k_GetMinX()-Delta and for the max.
	comparison it is x<k_GetMaxX()+Delta. Similar with the y-value.
	@param x Position in x-direction.
	@param y Position in y-direction.
	@param Delta The delta value is subtracted from the min. ranges and
		added to the max. ranges.
	@return True, if the provided position is within range (including Delta)
		of the provided image. */
int k_InRange(int x, int y, int Delta, const TImage* Img);

/** adds Bias to all pixels. pSource may be equal to pDest.
	@param Bias is the bias value
	@param pImg is a pointer to the source-target image of type TImage
	@return False if *pImg is empty or does not have 8 b/p
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_ApplyBias(INT16 Bias,TImage *pImg);

/** scales the colors using a constant multiplier.
	@param Prefactor is the scaling factor
	@param pImg is a pointer to the source-target image of type TImage
	@return False if *pImg is empty or does not have 8 b/p
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_ScaleIntensity(float Prefactor,TImage *pImg);

/** calulates the sum of two TImages.
	@param pImg is a pointer to the source-destination image of type TImage
	@param pAddend is a pointer to the image of type TImage to be added
	@return False if one or both of the images involved are empty or have different size
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_Add(TImage *pImg,TImage *pAddend);

/** calulates the difference of two TImages.
	@param pImg is a pointer to the source-destination image of type TImage
	@param pSubtrahend is a pointer to the image of type TImage to be subtracted
	@return False if one or both of the images involved are empty or have different size
		or or does not have 8 b/p
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_Subtract(TImage *pImg,TImage *pSubtrahend);

/** calculates product of two images
	@param pImg is a pointer to the source-destination image of type TImage
	@param pFactor a pointer to the image of type TImage to be multiplied
	@return False if one or both of the images involved are empty or have different size
		or does not have 8 b/p
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_Multiply(TImage *pImg,TImage *pFactor);

/** performs a displacement so that (dx,dy)
	in the old picture becomes (0,0). Periodic continuation is assumed.
	@param dx is the horizontal displacement
	@param dy is the vertical displacement
	@param pImg is a pointer to the source image
	@return False if *pImg is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_CopyDisplace(int dx,int dy,TImage *pImg);

/** copies an image into another with
	different size without scaling. The origin is retained. For
	increasing width/height, zero padding is applied. Simple cuts
	are applied for reduced width/height.
	@param pDest must be allocated prior to calling
	@param pSource is a pointer to the source image
	@return False if *pSource and/or *pDest are empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_CutOrZeroPad(TImage *Dest,TImage *pSource);

/** copies a TImage into a TIntImage. *pDest attains the same dimensions
	as the *pSource. *pDest will have 8 b/p.
	Note that this is a straight forward convertion, values are truncated
	directly to the destination range. For more advanced convertions look
	at the functions available in local operation .
	History from *pSource is copied to *pDest, old history in *pDest is deleted.
	@param pSource a pointer to the source image
	@param pDest a pointer to the destination image
	@return False if source image is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_CopyFromInt(TImage *pDest,TIntImage *pSource);

/** copies a TFloatImage into a TImage. *pDest attains the same dimensions
	as the *pSource.  *pDest will have 8 b/p.
	History from *pSource is copied to *pDest, old history in *pDest is deleted.
	Note that this is a straight forward convertion, values are truncated
	directly to the destination range. For more advanced convertions look
	at the functions available in local operation .
	@param pSource a pointer to the source image
	@param pDest a pointer to the destination image
	@return False if source image is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_CopyFromFloat(TImage *pDest,TFloatImage *pSource);

/** copies partially a TComplexImage into a TStdImage.
	*pDest attains the same dimensions as *pSource, and mode determines
	which part of the source image is copied. *pDest will have 8 b/p.
	History from *pSource is copied to *pDest, old history in *pDest is deleted.
	Note that this is a straight forward convertion, values are truncated
	directly to the destination range. For more advanced convertions look
	at the functions available in local operation .
	@param mode determines which part is copied: if mode==1 the real part
	is copied, if mode==2 the imaginary part is copied. Otherwise the
	modulus is copied,
	@param mode Determines what part of the complex image is to be
		copied. The values are COMPLEX_REAL, COMPLEX_IMAGINARY, or COMPLEX_MODULUS.
	@param pDest a pointer to the destination image
	@param pSource a pointer to the source image
	@return False if source image is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_CopyFromComplex(COMPLEXCHOISE mode, TImage *pDest,TComplexImage *pSource);

/** Prints the information contained in "pInfo" to stdout */
void k_PrintImageInfo(const TImage* pInfo);

/** Draws a line in image using the graphics routine k_GraphicsGenerateLine().
	For drawing ellipses and other symbols see graphic functions.
	@param Start Beginning of line, positions in the pixelgroup will start from this point.
	@param End End of line, positions in the pixelgroup will end with this point.
	@return False, if image is a NULL pointer.
	@see k_GraphicsGenerateLine */
bool k_DrawLine(T2DInt Start, T2DInt End, UINT32 Color, TImage *pDest);

/** Draws a circle in image using the graphics routine k_GraphicsGenerateCircle().
	For drawing ellipses and other symbols see graphic functions.
	@param Center Position for center of circle.
	@param Radius Radius of circle.
	@return False, if image is a NULL pointer.
	@see k_GraphicsGenerateCircle */
bool k_DrawCircle(T2DInt Center, unsigned int Radius, UINT32 Color, TImage *pDest);

/** @defgroup image_c_only Image C only
	Kernel ANSI C only routines, last updated 12/5/1999.
	Basic image functions to be used as stand alone c
	functions only. The C++ part does not use these functions.
	@version 1.00
	@author René Dencker Eriksen edr@mip.sdu.dk
    @{ */

/** Returns a pixel value at position (x,y),
	including range check, slow access.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@return pixel value, for 24 b/p: R+G*256+B*256*256. */
UINT32 k_GetPixel(int x, int y,const TImage* pInfo);

/** Returns a pixel value at position (x,y),
	without range check, fast access.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@return pixel value, for 24 b/p: R+G*256+B*256*256. */
UINT32 k_GetPixelFast(int x, int y,const TImage* pInfo);

/** Macro function for accessing pixels in an image with 1 b/p only. The macro is very
	fast, but be carefull when using the macro, there are no type and range checking.
	Orego always (0,0).
	@param x (int) x-position
	@param y (int) y-position
	@param Image (TImage) The image to access pixel in. */
#define k_GetPixel1bp(x,y,Image) ((((Image).ppMatrix[(y)][(x)>>3])&(0x80>>((x)&0x7)))!=0)

/** Macro function for accessing pixels in an image with 8 b/p only. The macro is very
	fast, but be carefull when using the macro, there are no type and range checking.
	Orego always (0,0).
	@param x (int) x-position
	@param y (int) y-position
	@param Image (TImage) The image to access pixel in. */
#define k_GetPixel8bp(x,y,Image) (((Image).ppMatrix[(y)][(x)])&0xff)

/** Macro function for accessing pixels in an image with 24 b/p only. The macro is very
	fast, but be carefull when using the macro, there are no type and range checking.
	Orego always (0,0).
	@param x (int) x-position
	@param y (int) y-position
	@param Image (TImage) The image to access pixel in. */
#ifdef IPL_BIG_ENDIAN
#define k_GetPixel24bp(x,y,Image) ((((Image).ppMatrix[(y)][3*(x)+2]) << 16) | (((Image).ppMatrix[(y)][3*(x)+1]) << 8) | ((Image).ppMatrix[(y)][3*(x)]))
#else
#define k_GetPixel24bp(x,y,Image) ((*(UINT32*)(&((Image).ppMatrix[(y)][3*(x)])))&0xffffff)
#endif /* IPL_BIG_ENDIAN */

/** Sets a pixel at position (x,y),
	including range check, slow access.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@param Color The new pixel value, for 24 b/p images an RGB value,
		otherwise a palette index value.
	@return false if outside image or if b/p is not supported. */
bool k_SetPixel(int x, int y, UINT32 Color,TImage* pInfo);

/** Sets a pixel at position (x,y),
	without range check, fast access.
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@param Color The new pixel value, for 24 b/p images an RGB value,
		otherwise a palette index value.
	@return false if b/p is not supported. */
bool k_SetPixelFast(int x, int y, UINT32 Color,TImage* pInfo);

/** Macro function for setting pixels in an image with 1 b/p only. The macro is very
	fast, but be carefull when using the macro, there are no type and range checking.
	Orego always (0,0).
	@param x (int) x-position
	@param y (int) y-position
	@param Image (TImage) The image to set pixel in. */
#define k_SetPixel1bp(x,y,Color,Image) (Color ? ((Image).ppMatrix[(y)][(x)>>3] |= (0x80>>((x) & 0x7))) : ((Image).ppMatrix[(y)][(x)>>3] &= (0xff7f>>((x) & 0x7))))


/** Macro function for setting pixels in an image with 8 b/p only. The macro is very
	fast, but be carefull when using the macro, there are no type and range checking.
	Orego always (0,0).
	@param x (int) x-position
	@param y (int) y-position
	@param Image (TImage) The image to set pixel in. */
#define k_SetPixel8bp(x,y,Color,Image) ((Image).ppMatrix[(y)][(x)]=(UINT8)Color)

/** Macro function for setting pixels in an image with 24 b/p only. The macro is very
	fast, but be carefull when using the macro, there are no type and range checking.
	Orego always (0,0).
	@param x (int) x-position
	@param y (int) y-position
	@param Image (TImage) The image to set pixel in. */
#ifdef IPL_BIG_ENDIAN
#define k_SetPixel24bp(x,y,Color,Image) do {			\
	(Image).ppMatrix[(y)][3*(x)]=(UINT8)Color;			\
	(Image).ppMatrix[(y)][3*(x)+1]=(UINT8)(Color>>8);	\
	(Image).ppMatrix[(y)][3*(x)+2]=(UINT8)(Color>>16);	\
} while (0)
#else
#define k_SetPixel24bp(x,y,Color,Image) (*((UINT32*)&((Image).ppMatrix[(y)][3*(x)])) = ((*((UINT32*)&((Image).ppMatrix[(y)][3*(x)]))) & 0xff000000) | (Color & 0x00ffffff))
#endif /* IPL_BIG_ENDIAN */

/** For graytone images only, needs 1,4 and 8 b/p, produces an error
	in other cases. Uses bilinar interpolation.
	@param x The position in horisontal direction
	@param y The position in vertical direction
	@return interpolated pixel value */
float k_GetPixelInterpolated(float x,float y, const TImage* pInfo);

/** A faster version of k_GetPixelInterpolated(), no range checking
	and origo must be (0,0).
	@param x The position in horisontal direction
	@param y The position in vertical direction
	@return interpolated pixel value */
float k_GetPixelInterpolatedFast(float x,float y, const TImage* pInfo);

/** For images with 24 b/p only (error if not 24 b/p), uses bilinar 
	interpolation for each color channel (R, G and B) and rounds of
	to integer value
	@param x The position in horisontal direction
	@param y The position in vertical direction
	@return interpolated pixel value */
UINT32 k_GetColorInterpolated(float x, float y, const TImage* pInfo);

/** A faster version of GetColorInterpolated(), no checking of 24 b/p, no range checking,
	and origo must be (0,0).
	@param x The position in horisontal direction
	@param y The position in vertical direction
	@return interpolated pixel value */
UINT32 k_GetColorInterpolatedFast(float x, float y, const TImage* pInfo);

/*@}*/ /* end group "image_c_only" */

/*@}*/ /* end group "image" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_FUNCTIONS_H */
