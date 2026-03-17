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

#ifndef _IPL98_KERNEL_COORDINATE_TRANSFORM_H
#define _IPL98_KERNEL_COORDINATE_TRANSFORM_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../ipl98_types.h"
#include "../image/kernel_functions.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup coordinatetransform ANSI-C Transformation functions
	Kernel ANSI C collection of different pixel coordinate
	transformation algorithms,
	last updated 10/18/1999 by edr@mip.sdu.dk.
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 1.10
	@author Ivar Balslev ivb@mip.sdu.dk
    @{ */

/** Performes a scaling given by (Sx,Sy) and a rotation through the angle phi.
	Automatic sizing is used, i.e. adjustment to the minimum size allowing the
	full image to be included. Works on 24 b/p images and graytone images with 1,4 or
	8 b/p. If you want the resulting image placed in the source
	image just use the same pointer for both parameters "pSource" and "pDest". Note:
	When "pSource" and "pDest" points to same image the function is a bit slower
	(needs to copy a temporary image to the destination). If the provided source
	image has no border, the function uses the "Background" parameter to add a temporary
	background with the given value to do interpolation at edge-pixels fast.
	Adds a line to the history of pDest.
	@param pSource Source image to scale.
	@param pDest Destination image, may be same as source image.
	@param Sx Scaling in x-direction.
	@param Sy Scaling in y-direction.
	@param phi Rotation in radians.
	@param Background If source image has no border, a temporary border with this
		value is added to the source image. Note: If the source image has a border
		the function uses the pixel values in this border to interpolate edge pixels.
	@return False if source image is empty. */
bool k_ScaleAndRotateAuto(TImage* pSource,TImage* pDest,double Sx, double Sy, double phi,UINT32 Background);

/** Makes an image with dimensions (Width * Height) and unchanged bits per pixel.
	Performs a scaling (Sx,Sy) and a rotation through the angle phi.
	The position (x0,y0) of the old image is placed at (Width/2,Height/2) in the new.
	Works on 24 b/p images and graytone images with 1,4 or
	8 b/p. If you want the resulting image placed in the source
	image just use the same pointer for both parameters "pSource" and "pDest". Note:
	When "pSource" and "pDest" points to same image the function is a bit slower
	(needs to copy a temporary image to the destination). If the provided source
	image has no border, the function uses the "Background" parameter to add a temporary
	background with the given value to do interpolation at edge-pixels fast.
	Adds a line to the history of pDest.
	@param pSource Source image to scale.
	@param pDest Destination image, may be same as source image.
	@param x0 Horisontal position from old image placed at Width/2 in new image.
	@param y0 Vertical position from old image placed at Height/2 in new image.
	@param Width Size in horisontal direction of new image.
	@param Height Size in vertical direction of new image.
	@param Sx Scaling in x-direction.
	@param Sy Scaling in y-direction.
	@param phi Rotation in radians.
	@param Background If source image has no border, a temporary border with this
		value is added to the source image. Note: If the source image has a border
		the function uses the pixel values in this border to interpolate edge pixels.
	@return False if source image is empty. */
bool k_ScaleAndRotateFixed(TImage* pSource,TImage* pDest,double x0,double y0,
						   int Width,int Height,double Sx, double Sy, double phi,UINT32 Background);

/** Performs a scaling using the scale factors (Sx,Sy) and autosizing of the new
	image placed in "pDest". Works on 24 b/p images and graytone images with 1,4 or
	8 b/p. If you want the resulting image placed in the source
	image just use the same pointer for both parameters "pSource" and "pDest". Note:
	When "pSource" and "pDest" points to same image the function is a bit slower
	(needs to copy a temporary image to the destination). If the provided source
	image has no border, the function uses the "Background" parameter to add a temporary
	background with the given value to do interpolation at edge-pixels fast.
	Adds a line to the history of pDest.
	@param pSource Source image to scale.
	@param pDest Destination image, may be same as source image.
	@param Sx Scaling in x-direction.
	@param Sy Scaling in y-direction.
	@param Background If source image has no border, a temporary border with this
		value is added to the source image. Note: If the source image has a border
		the function uses the pixel values in this border to interpolate edge pixels.
	@return False if source image is empty. */
bool k_ScaleAuto(TImage* pSource,TImage* pDest,double Sx, double Sy, UINT32 Background);

/** Makes an image with dimensions (Width * Height).and unchanged bits per pixel.
	Performs a scaling (Sx,Sy).
	The position (x0,y0) of the old image is placed at (Width/2,Height/2) in the new.
	Works on 24 b/p images and graytone images with 1,4 or
	8 b/p. If you want the resulting image placed in the source
	image just use the same pointer for both parameters "pSource" and "pDest". Note:
	When "pSource" and "pDest" points to same image the function is a bit slower
	(needs to copy a temporary image to the destination). If the provided source
	image has no border, the function uses the "Background" parameter to add a temporary
	background with the given value to do interpolation at edge-pixels fast.
	Adds a line to the history of pDest.
	@param pSource Source image to scale.
	@param pDest Destination image, may be same as source image.
	@param x0 Horisontal position from old image placed at Width/2 in new image.
	@param y0 Vertical position from old image placed at Height/2 in new image.
	@param Width Size in horisontal direction of new image.
	@param Height Size in vertical direction of new image.
	@param Sx Scaling in x-direction.
	@param Sy Scaling in y-direction.
	@param Background If source image has no border, a temporary border with this
		value is added to the source image. Note: If the source image has a border
		the function uses the pixel values in this border to interpolate edge pixels.
	@return False if source image is empty. */
bool k_ScaleFixed(TImage* pSource,TImage* pDest,double x0,double y0,
				  int Width,int Height,double Sx, double Sy,UINT32 Background);

/*@}*/ /* end group "coordinatetransform" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_COORDINATE_TRANSFORM_H */
