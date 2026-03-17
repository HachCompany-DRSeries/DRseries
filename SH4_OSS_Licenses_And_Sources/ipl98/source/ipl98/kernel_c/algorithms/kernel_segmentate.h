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

#ifndef _IPL98_KERNEL_SEGMENTATE_H
#define _IPL98_KERNEL_SEGMENTATE_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../ipl98_types.h"
#include "../kernel_pixelgroup.h"
#include "../kernel_pixelgroups.h"
#include "../image/kernel_functions.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup segmentate ANSI-C Segmentate functions
	Kernel ANSI C collection of different segmentation algorithms,
	last updated 13/5/2003 by edr@mip.sdu.dk.
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 1.42
	@author Ivar Balslev (ivb@mip.sdu.dk) and René Dencker Eriksen (edr@mip.sdu.dk)
    @{ */

/** Derive blobs from image and put results in TPixelGroups. If you want the
	original colors added to the found pixel groups call the function
	k_AddColorsToGroups(...) found in the TPixelGroups area. 
	@param pSource The source image to derive from, nothing is changed in the image.
	@param Color The color to segmentate into blobs, values are HIGHCOLOR and LOWCOLOR.
	@param Threshold A simpel threshold value to seperate background from
			blobs in graytone images. Not used if source image is 1 b/p.
	@param Connected Decide if the blobs are to be eight- or fourconnected,
			as described in Gonzalez and Woods page 41.
	@param MinSize Blobs containing less than MinSize pixels are discarded, a value of one means all 
		blobs are derived. Discarding more blobs makes this algorithm faster.
	@return False if source image given to constructor is empty or
		not 1 b/p or 8 b/p.
	@version 1.00
	@author Implementation by Ivar Balslev (ivb@mip.sdu.dk) and René Dencker Eriksen (edr@mip.sdu.dk) */
bool k_DeriveBlobs(TImage* pSource, TPixelGroups* pPixelGroups,
                    COLORTYPE Color, UINT8 Threshold, CONNECTIVITY Connected, unsigned int MinSize);

/** Derives the blob belonging to the given start position (seed). If the image is
	1 b/p the pixel color at the seed position decides the color of the blob to be
	found. It the image is 8 b/p, the threshold parameter is used and the blob found
	will be either below or above threshold regarding the color of the seed position.
	If you want the original colors added to the found pixel groups call the function
	k_AddColorsToGroup(...) found in the TPixelGroup area. 
	@param pSource The source image to derive from, nothing is changed in the image.
	@param Threshold A simpel threshold value to seperate the blob from background in
			graytone images. Not used if source image is 1 b/p.
	@param Connected Decide if the blobs are to be eight- or fourconnected,
			as described in Gonzalez and Woods page 41.
	@return False if source image given to constructor is empty or
			not 1 b/p or 8 b/p.
	@version 0.40
	@author Steen M. Madsen and Rolf Thunbo. IPL98 integration by 
			René Dencker Eriksen (edr@mip.sdu.dk) */
bool k_BlobExpansion(T2DInt Seed, TImage* pSource, TPixelGroup* pGrp,
					 UINT8 Threshold, CONNECTIVITY Connected);

/** Finds the low contour in the image from given start point and search direction.
	If no border is present a high color (white) border is used internally. If set by the
	algorithm the border is removed again before returning.
	@param pSource Source image, if source image is 8 b/p the low color must be 0 and
		all other values are treated as high color.
	@param Start Position to begin search from.
	@param SearchDirection Direction to search, beginning at "Start". Must be either 
		NORTH, EAST, SOUTH or WEST.
	@param Connected Connectivity may be EIGHTCONNECTED or FOURCONNECTED.
	@param pPixelGroup Result placed in this pixel group. Old values are always destroyed.
	@param pRotationDirection Resulting direction in which the contour was found, the
		pixels in pPixelGroup is ordered according to this direction.
	@param pSurroundedColor Returns the color surrounded by the found contour.
	@return False in these cases: If SearchDirection is not one of NORTH, EAST, SOUTH or WEST. If source
		image is empty, If source image is 24 b/p.
	@version 1.02
	@author Implementation by Ivar Balslev (ivb@mip.sdu.dk)
	@see k_FindAndFollowHighContour */
bool k_FindAndFollowLowContour(TImage* pSource, T2DInt Start,
							   DIRECTION SearchDirection, CONNECTIVITY Connected,
							   TPixelGroup* pPixelGroup,ROTATION* pRotationDirection,
								COLORTYPE* pSurroundedColor);

/** Finds the high contour in the image from given start point and search direction.
	If no border is present a low color (black) border is used internally. If set by the 
	algorithm the border is removed again before returning.
	Same parameters as k_FindAndFollowLowContour().
	@version 1.01
	@author Implementation by Ivar Balslev (ivb@mip.sdu.dk)
	@see k_FindAndFollowLowContour */
bool k_FindAndFollowHighContour(TImage* pSource, T2DInt Start,
								DIRECTION SearchDirection, CONNECTIVITY Connected,
								TPixelGroup* pPixelGroup,ROTATION* pRotationDirection,
								COLORTYPE* pSurroundedColor);

/** Finds the high contour in the image from given start point and search direction,
	works as k_FindAndFollowLowContour() except a threshold is supplied.
	If no border is present a low color (black) border is used internally. If set by the 
	algorithm the border is removed again before returning.
	@param pSource Source image, must be 1 or 8 b/p.
	@param Start Position to begin search from.
	@param Threshold Pixel values above this value is considered as HIGHCOLOR
	@param SearchDirection Direction to search, beginning at "Start". Must be either 
		NORTH, EAST, SOUTH or WEST.
	@param Connected Connectivity may be EIGHTCONNECTED or FOURCONNECTED.
	@param pPixelGroup Result placed in this pixel group. Old values are always destroyed.
	@param pRotationDirection Resulting direction in which the contour was found, the
		pixels in pPixelGroup is ordered according to this direction.
	@param pSurroundedColor Returns the color surrounded by the found contour.
	@return False in these cases: If SearchDirection is not one of NORTH, EAST, SOUTH or WEST. If source
		image is empty, If source image is 24 b/p.
	@version 0.90
	@author Implementation by Ivar Balslev (ivb@mip.sdu.dk)
	@see k_FindAndFollowLowContour */
bool k_FindAndFollowLowContourWithThreshold(TImage* pSource, T2DInt Start,unsigned int Threshold,
											DIRECTION SearchDirection, CONNECTIVITY Connected,
											TPixelGroup* pPixelGroup,ROTATION* pRotationDirection,
											COLORTYPE* pSurroundedColor);

/** Finds the high contour in the image from given start point and search direction,
	works as k_FindAndFollowHighContour() except a threshold is supplied.
	If no border is present a low color (black) border is used internally. If set by the
	algorithm the border is removed again before returning.
	Same parameters as k_FindAndFollowHighContour().
	@param pSource Source image, must be 1 or 8 b/p.
	@param Start Position to begin search from.
	@param Threshold Pixel values above this value is considered as HIGHCOLOR
	@param SearchDirection Direction to search, beginning at "Start". Must be either
		NORTH, EAST, SOUTH or WEST.
	@param Connected Connectivity may be EIGHTCONNECTED or FOURCONNECTED.
	@param pPixelGroup Result placed in this pixel group. Old values are always destroyed.
	@param pRotationDirection Resulting direction in which the contour was found, the
		pixels in pPixelGroup is ordered according to this direction.
	@param pSurroundedColor Returns the color surrounded by the found contour.
	@return False in these cases: If SearchDirection is not one of NORTH, EAST, SOUTH or WEST. If source
		image is empty, If source image is 24 b/p.
	@version 0.90
	@author Implementation by Ivar Balslev (ivb@mip.sdu.dk)
	@see k_FindAndFollowHighContour */
bool k_FindAndFollowHighContourWithThreshold(TImage* pSource, T2DInt Start, unsigned int Threshold,
											 DIRECTION SearchDirection, CONNECTIVITY Connected,
											 TPixelGroup* pPixelGroup,ROTATION* pRotationDirection,
											 COLORTYPE* pSurroundedColor);

/*@}*/ /* end group "segmentate" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_SEGMENTATE_H */
