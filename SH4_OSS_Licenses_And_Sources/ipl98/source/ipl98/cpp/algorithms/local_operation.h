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

#ifndef _IPL98_LOCAL_OPERATION_H
#define _IPL98_LOCAL_OPERATION_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../../kernel_c/algorithms/kernel_local_operation.h"
#include "../image.h"
#include "../int_image.h"
#include "../float_image.h"
#include "../complex_image.h"

namespace ipl{ // use namespace if C++

/** @ingroup algorithms
@{ */

/** CLocalOperation: Simple transformation involving two different image classes,
        last updated 4/8/2001.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CLocalOperation ipl98/cpp/algorithms/local_operation.h
	@version 0.91
	@author Ivar Balslev ivb@mip.sdu.dk **/
class CLocalOperation
{  private:
   public:
		/// default constructor
		CLocalOperation();
		/// default destructor
		~CLocalOperation();
		/** calculates the histogram of the image Source and draws it in
			Dest. The Dest image has dimensions (w,h,bits)=(277,300,1).
			If Source has 24 b/p the procedure calculates the
			histogram of the corresponding gray-tone image.
			The history of Source is copied to Dest, old history in Dest is deleted.
			@param Source is the source image
			@param Dest is the destination image
			@return False if the source image is empty, Source is the 
				same image as Dest or has less than 8 b/p
			@version 0.86
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool DrawHistogram(CStdImage &Source, CStdImage &Dest);
		/** transforms Source into Dest using a local pixel value
			transformation so that the histogram become equalized.
			If Source has 24 b/p the procedure calculates the
			histogram of the corresponding gray-tone image, and the
			LUT derived from this is applied so that all three
			rgb-intensities are scaled by the factor (LUT[p]/p)
			where p is the average of the three intensities.
			@param Source is a pointer to the source image
			@param Dest is the pointer to the destination image
			@return False if the source image is empty or has less than 8 b/p.
			@version 0.86
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk)*/
		static bool EqualizeHistogram(CStdImage &Source, CStdImage &Dest);
		/** copies an object of class CImage or CByteImage
			into a  CImage (8 b/p) using a logaritmic function or a power function
			in the gray tone transformation. Source Image must have 8 b/p.
			If (input<Min) output=0, If (input>Max) output=255. Otherwise:
			if (gamma==0) output=INT(255*log(input-Min+1)/log(Max-Min+1)), otherwise
			output = INT(255*pow((input-Min)/(Max-Min),gamma)).
			@param Source is the source image
			@param gamma is the power in the conversion function
				(logaritmic conversion if gamma ==0)
			@param Min is the value in Source giving gray tone 0 in Dest
			@param Max is the value in Source giving gray tone 255 in Dest
			@param Dest is the destimation image
			@return False, if the source image is empty or 
				does not have 8b/p or if Min>=Max
			@version 0.86
			@author Implementation by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool NonlinearCopyToByte(CStdImage &Source, float gamma,
										 int Min,int Max, CStdImage &Dest);
		/** copies a CFloatImage into a
			CImage (8 b/p) using a logaritmic function or a power function
			in the gray tone transformation: If (input<Min) output=0, If
			(input>Max) output=255. Otherwise: if (gamma==0) output=
			INT(255*log(input-Min+1)/log(Max-Min+1)), otherwise
			output = INT(255*pow((input-Min)/(Max-Min),gamma)).
			@param Source is the source image
			@param gamma is the power in the conversion function
				(logaritmic conversion if gamma ==0)
			@param Min is the value in Source giving gray tone 0 in Dest
			@param Max is the value in Source giving gray tone 255 in Dest
			@param Dest is the destimation image
			@return False, if the source image is empty or if Min>=Max
			@version 0.86
			@author Implementation by Ivar Balslev (ivb@mip.sdu.dk) */
		bool NonlinearCopyToByte(CFloatImage &Source,
						float gamma,float Min,float Max, CStdImage &Dest);
		/** copies a CIntImage into a
			CImage (8 b/p) using a logaritmic function or a power function
			in the gray tone transformation: If (input<Min) output=0, If
			(input>Max) output=255. Otherwise: if (gamma==0) output=
			INT(255*log(input-Min+1)/log(Max-Min+1)), otherwise
			output = INT(255*pow((input-Min)/(Max-Min),gamma)).
			@param Source is the source image
			@param gamma is the power in the conversion function
				(logaritmic conversion if gamma ==0)
			@param Min is the value in Source giving gray tone 0 in Dest
			@param Max is the value in Source giving gray tone 255 in Dest
			@param Dest is the destimation image
			@return False, if the source image is empty or if Min>=Max
			@version 0.86
			@author Implementation by Ivar Balslev (ivb@mip.sdu.dk) */
		bool NonlinearCopyToByte(CIntImage &Source,
				   float gamma, int Min,int Max, CStdImage &Dest);
		/** copies a CFloatImage into a
			CImage (8 b/p) using a logaritmic function or a power function
			in the gray tone transformation: If (input<Min) output=0, If
			(input>Max) output=255. Otherwise: if (gamma==0) output=
			INT(255*log(input-Min+1)/log(Max-Min+1)), otherwise
			output = INT(255*pow((input-Min)/(Max-Min),gamma)).
			@param Source is the source image
			@param mode Determines what part of the complex image is to be
				copied. The values are COMPLEX_REAL, COMPLEX_IMAGINARY, or COMPLEX_MODULUS.
			@param gamma is the power in the conversion function
				(logaritmic conversion if gamma ==0)
			@param Min is the value in Source giving gray tone 0 in Dest
			@param Max is the value in Source giving gray tone 255 in Dest
			@param Dest is the destimation image
			@return False, if the source image is empty or if Min>=Max
			@version 0.86
			@author Implementation by Ivar Balslev (ivb@mip.sdu.dk) */
		bool NonlinearCopyToByte(CComplexImage &Source,COMPLEXCHOISE mode,
				   float gamma,float Min, float Max, CStdImage &Dest);

		/** Transforms pixels based on their RGB value from an input image with
			24 b/p. The	resulting pixel value is 255 if they are inside a
			truncated cone in the RGB space. The cone itself is defined by a point
			in the RGB-space (Color) and an angle (Angle) defining the cone width.
			In addition, the cone can be truncated by setting lower and upper 
			limits. The maximum upper limit can be found by calling 
			GetRGBConeUpperLimit. Colors outside this cone are considered 
			background and will be set to 0.
			@param Source Image to derive blobs from, must have 24 b/p.
			@param Dest Destination image will have 8 b/p, where white pixels
				(255) are original color values in Source which are inside the 
				RGB cone.
			@param Color The direction of the cone axis is defined by this color.
			@param LowerLimit Truncates the tip of the cone at this value (towards 
				origo of the RGB space). This is the distance from origo to the
				truncated tip of the cone. Value must be greater than 0.
			@param UpperLimit Truncates the base of the cone at this value. This
				is the distance from origo to the truncated base of the cone.
				The maximum value for this parameter can be found by calling
				GetRGBConeUpperLimit(UINT32 Color). If UpperLimit is greater than
				the value returned by GetRGBConeUpperLimit() it will simply be 
				truncated to this value (a warning is produced).
			@param Angle Angle in radians defining the cone width.
			@return False, if source image is not 24 b/p.
			@version 0.2
			@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
			@see GetRGBConeUpperLimit */
		static bool RGBConeTransformation(const CImage& Source, CImage& Dest, 
						UINT32 Color, float LowerLimit, float UpperLimit, float Angle);

		/** Returns the upper limit for the cone defined when calling SegmentateFromRGBCone().
			@param Color The direction of the cone axis is defined by this color.
			@return Upper limit for base in the RGB space or 0 if Color=(r,g,b)=(0,0,0).
			@version 0.86
			@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk) */
		static float GetRGBConeUpperLimit(UINT32 Color);
};

/*@}*/ /* end group "algorithms" */

} // end namespace ipl

#endif //_IPL98_LOCAL_OPERATION_H
