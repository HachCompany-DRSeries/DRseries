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

#ifndef _IPL98_KERNEL_MASK_OPERATION_H
#define _IPL98_KERNEL_MASK_OPERATION_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../ipl98_types.h"
#include "../image/kernel_functions.h"
#include "../float_image/f_kernel_functions.h"
#include "../int_image/i_kernel_functions.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup maskoperations ANSI-C Mask operations
	Kernel ANSI C Mask operations performed on a whole image,
	last updated 8/21/2005. Note: all methods must be gray tone images with 8 b/p.
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 0.91
	@author Ivar Balslev ivb@mip.sdu.dk
    @{ */

/** calculates the convolution of *pImg and *pMask. *pMask is always
	a float image. No dimension of *pMask should exceed that of *pImg.
	*pImg is considered to be periodically repeated on all directions.
	Reference cell is (0,0) of *pMask. Apply k_CopyDisplace
	or k_SetOrigo to pImg in case another reference cell is desired.
	@param pImg is a pointer to the source-target image of type TImage or TByteImage
	@param pMask is a pointer to the mask image of type TFloatImage
	@return False if *pImg and/or *pMask are empty
		or if the dimensions of *pMask exceed those of *pImg
		or if *pImg does not have 8 bits per pixel
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_ConvolveByteToByte(TImage *pImg,const TFloatImage *pMask);

/** calculates the convolution of *pSource and *pMask. *pMask is always
	a float image. No dimension of *pMask should exceed that of *pSource.
	*pImg is considered to be periodically repeated on all directions.
	Reference cell is (0,0) of *pMask. Apply k_CopyDisplace
	in case another reference cell is desired.
	The history of *pSource is copied to *pDest, old history in *pDest is deleted.
	@param pSource is a pointer to the source image of type TImage
	@param pMask is a pointer to the mask image of type TFloatImage
	@param pDest is a pointer to the destination image of type TIntImage
	@return False if *pSource and/or *pMask are empty
		or if the dimensions of *pMask exceed those of *pSource
		or if *pSource does not have 8 bits per pixel
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_ConvolveByteToInt(TImage *pSource,const TFloatImage *pMask, TIntImage *pDest);

/** calculates the convolution of *pImg and *pMask. *pMask is always
	a float image. No dimension of *pMask should exceed that of *pImg.
	*pImg is considered to be periodically repeated on all directions.
	Reference cell is (0,0) of *pMask. Apply kI_CopyDisplace
	to pImg in case another reference cell is desired.
	@param pImg is a pointer to the source-target image of type TIntImage
	@param pMask is a pointer to the mask image of type TFloatImage
	@return False if *pImg and/or *pMask are empty
		or if the dimensions of *pMask exceed those of *pImg
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_ConvolveIntToInt(TIntImage *pImg,const TFloatImage *pMask);

/** calculates the convolution of *pImg and *pMask. *pMask is always
	a float image. No dimension of *pMask should exceed that of *pImg.
	Boundary conditions periodic with width and height of *pImg are
	assumed. Reference cell is (0,0) of *pMask. Apply kF_CopyDisplace
	to pImg in case another reference cell is desired.
	@param pImg is a pointer to the source-target image of type TFloatImage
	@param pMask is a pointer to the mask image of type TFloatImage
	@return False if *pImg and/or *pMask are empty
		or if the dimensions of *pMask exceed those of *pImg
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_ConvolveFloatToFloat(TFloatImage *pImg,const TFloatImage *pMask);

/** a speed optimized blur algorithm replacing each
	graytone by the simple average of the gray tones of central cell and
	4 nearest pixels. k_BlurFourConnected3x3 operates on TImages with 8 b/p.
	Edge pixels are  calculated using a mirror image of the old
	edge pixels at the image edges.
	@param pImg is a pointer to the source-target image.
	@return False if *pImg is empty or have dimensions smaller than 4x4
		or if *pImg does not have 8 bits per pixel
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_BlurFourConnected3x3(TImage *pImg);

/** is a speed optimized blur algorithm replacing each
	graytone by the simple average of the gray tones of central cell and
	8 nearest pixels. k_BlurEightConnected3x3 operates on TImages with 8 b/p .
	Edge pixels are  calculated using a mirror image of the old
	edge pixels at the image edges.
	@param pImg is a pointer to the source-target image.
	@return False if *pImg is empty or have dimensions smaller than 4x4
		or if *pImg does not have 8 bits per pixel
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_BlurEightConnected3x3(TImage *pImg);

/** Same functionality as k_BlurEightConnected3x3 but works on
	a TIntImage structure.
	@param pImg is a pointer to the source-target image.
	@return False if *pImg is empty or have dimensions smaller than 4x4.
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk)
	@see k_BlurEightConnected3x3 */
bool k_BlurEightConnected3x3Int(TIntImage *pImg);

/** performs a convolutions of *pImg using the TFloatImage
	*pMask. The (result mod 256) is saved in a byte image.
	The mask must be a 3x3 image. The prefactor is used for speeding up
	convolution: If the 8 non-center elemets of the mask are scaled to
	0 or 1, then the algorithm is very fast. The reference cell is the
	center cell. Edge pixels in *pDest are set to the same gray tone as
	the nearest neighbour non-edge pixel.
	@param pImg is a pointer to the source-target image of type TImage
	@param pMask is a pointer to the 3x3 mask image of type TFloatImage
	@return False if *pImg is empty or if the dimensions of *pMask is not 3x3
		or if *pImg does not have 8 bits per pixel
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_ConvolveFastByteToByte(TImage *pImg, const TFloatImage *pMask,
           float Prefactor);

/** performs a convolutions of *pSource using the TFloatImage
	*pMask. The (result mod 256) is saved in an integer image.
	The mask must be a 3x3 image. The algorithm is very fast if the 8
	non-center elements are 1 or 0. The prefactor can be used for
	achieving this. The reference cell is the
	center cell. Edge pixels in *pDest are set to the same gray tone as
	the nearest neighbour non-edge pixel.
	The history of *pSource is copied to *pDest, old history in *pDest is deleted.
	@param pSource is a pointer to source image of type TImage with 8b/p
	@parameter Prefactor is a common prefactor scaling the image *pDest
	@param pMask is a pointer to the 3x3 mask image of type TFloatImage
	@param pDest is a pointer to destination image of type TIntImage
	@return False if *pSource is empty or if the dimensions of *pMask is not 3x3
		or if *pSource does not have 8 bits per pixel
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_ConvolveFastByteToInt(TImage *pSource, const TFloatImage *pMask,
           float Prefactor ,TIntImage *pDest);

/** performs a convolutions of *pImg using the TFloatImage
	*pMask. The (result mod 256) is saved in an integer image.
	The mask must be a 3x3 image. The algorithm is very fast if the
	8 non-center elements are 1 or 0. The prefactor can be used for
	achieving this. The reference cell is the center cell. Edge pixels
	in *pDest are set to the same gray tone as the nearest
	neighbour non-edge pixel.
	@param pImg is a pointer to the source-target image of type TIntImage
	@param pMask is a pointer to the 3x3 mask image of type TFloatImage
	@return False if *pImg is empty or if the dimensions of *pMask is not 3x3
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_ConvolveFastIntToInt(TIntImage *pImg, const TFloatImage *pMask,
           float Prefactor) ;

/** calculates the maximum absolute graytone difference  of two opposite
	neighbour pixels over the 4 possible directions. Diagonal differences
	have weight 2/3. The (result % 256) is stored. The gray tone of edge
	pixels are set to zero.
	@param pImg is a pointer to the source-target image of type TImage with 8 b/p
	@parameter Prefactor is a common prefactor scaling the image *pDest
	@param pDest is a pointer to the deYstination image of type TImage
	@return False if *pImg is empty or if *pImg does not have 8 bits per pixel
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_MaxGradientFastByteToByte(TImage *pImg,float Prefactor);

/** calculates the maximum absolute graytone difference of two opposite
	neighbour pixels over the 4 possible directions. Diagonal differences
	have weight 2/3. The result is stored in an integer
	image. The gray tone of edge pixels are set to zero.
	The history of *pSource is copied to *pDest, old history in *pDest is deleted.
	@param pImg is a pointer to the source image of type TImage with 8 b/p
	@parameter Prefactor is a common prefactor scaling the image *pDest
	@param pDest is a pointer to the destination image of type TIntImage
	@return False if *pSource is empty or if *pSource does not have 8 bits per pixel
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_MaxGradientFastByteToInt(TImage *pSource,float Prefactor,TIntImage *pDest);

/** calculates the maximum absolute graytone difference  of two opposite
	neighbour pixels over the 4 possible directions. Diagonal differences
	have weight 2/3. The (result  % 256) is stored. The gray tone of edge
	pixels are set to zero.
	@param pImg is a pointer to the source-target image of type TIntImage
	@parameter Prefactor is a common prefactor scaling the image *pDest
	@return False if *pImg is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_MaxGradientFastIntToInt(TIntImage *pImg,float Prefactor);

/** takes the median of the graytone among the 5 pixels forming a plus
	sign about each pixel. Edge pixels are unchanged.
	@param pImg is a pointer to the source-destination image of type TImage
	@return False if *pImg is empty or if *pImg does not have 8 bits per pixel
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_MedianFilterFourConnected3x3(TImage *pImg);

/** takes the median of the graytone among the 5 pixels forming a cross
	about each pixel. Edge pixels are unchanged.
	@param pImg is a pointer to the source-destination image of type TImage
	@return False if *pImg is empty or if *pImg does not have 8 bits per pixel
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_MedianFilterDiagonalConnected3x3(TImage *pImg);

/** takes the median of the graytone among the 5 pixels forming a plus
	sign about each pixel. Edge pixels are unchanged.
	@param pImg is a pointer to the source-destination image of type TIntImage
	@return False if *pImg is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_MedianFilterFourConnected3x3Int(TIntImage *pImg);

/** takes the median of the graytone among the 5 pixels forming a cross
	about each pixel. Edge pixels are unchanged.
	@param pImg is a pointer to the source-destination image of type TIntImage
	@return False if *pImg is empty
	@version 0.80
	@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_MedianFilterDiagonalConnected3x3Int(TIntImage *pImg);

/*@}*/ /* end group "maskoperations" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_MASK_OPERATION_H */
