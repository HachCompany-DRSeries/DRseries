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

#ifndef _IPL98_KERNEL_LOCAL_OPERATIONS_H
#define _IPL98_KERNEL_LOCAL_OPERATIONS_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../ipl98_types.h"
#include "../image/kernel_functions.h"
#include "../complex_image/c_kernel_functions.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup localoperations ANSI-C Local operations
	Kernel ANSI C local operations, last updated 11/30/1999.
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 0.80
	@author Ivar Balslev ivb@mip.sdu.dk
    @{ */

/** calculates the histogram of the image *pSource and draws it in *pDest.
    The *pDest image has dimensions (w,h,bits)=(277,300,1).
    If *pSource has 24 b/p the procedure calculates the
    histogram of the corresponding gray-tone image.
	The history of *pSource is copied to *pDest, old history in *pDest is deleted.
    @param pSource is a pointer to the source image
    @param pDest is the pointer to the destination image
    @return False if the source image is empty, *pSource is the 
		same image as *pDest or has less than 8 b/p.
    @version 0.80
    @author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_DrawHistogram(TImage *pSource, TImage *pDest);

/** transforms *pSource into *pDest using a local pixel value
    transformation so that the histogram become equalized, the destination
	image will have same b/p as source image.
    If *pSource has 24 b/p the procedure calculates the
    histogram of the corresponding gray-tone image, and the
    LUT derived from this is applied so that all rgb-intensities
    are scaled by the factor (LUT[p]/p) where p is the average
    of the three intensities.
    @param pSource is a pointer to the source image
    @param pDest is the pointer to the destination image
    @return False if the source image is empty or has less than 8 b/p.
    @version 0.80
    @author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_EqualizeHistogram(TImage *pSource,TImage *pDest);

/** copies a TImage (8 b/p) into a
    TImage (8 b/p) using a logaritmic function or a power function
    in the gray tone transformation: If (input<Min) output=0, If
    (input>Max) output=255. Otherwise: if (gamma==0) output=
    INT(255*log(input-Min+1)/log(Max-Min+1)), otherwise
    output = (UINT8)(255*pow((input-Min)/(Max-Min),gamma)).
	History from pSource is copied to pDest, old history in pDest is deleted.
    @param pSource is a pointer to the source image
    @param gamma is the power in the nonlinear converion (logaritmic if gamma==0)
    @param Min is the value giving zero in *pDest
    @param Max is the value giving 255 in *pDest
    @param pDest is the pointer to the destination image
    @return False if the *pSource is empty
		or does not have 8 b/p or if Min>=Max or if
    @version 0.80
    @author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_NonlinearCopyByteToByte(TImage *pSource,float gamma,
                           int Min,int Max,TImage *pDest);

/** copies an integer image into a
    TImage (8 b/p) using a logaritmic function or a power function
    in the gray tone transformation: If (input<Min) output=0, If
    (input>Max) output=255. Otherwise: if (gamma==0) output=
    INT(255*log(input-Min+1)/log(Max-Min+1)), otherwise
    output = INT(255*pow((input-Min)/(Max-Min),gamma))
    @param pSource is a pointer to the source image
    @param gamma is the power in the nonlinear converion (logaritmic if gamma==0)
    @param Min is the value giving zero in *pDest
    @param Max is the value giving 255 in *pDest
    @param pDest is the pointer to the destination image
    @return False if the *pSource is empty or if Min>=Max
    @version 0.80
    @author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_NonlinearCopyIntToByte(TIntImage *pSource,float gamma,
                           int Min,int Max,TImage *pDest);

/** copies a float image into a
    TImage (8 b/p) using a logaritmic function or a power function
    in the gray tone transformation: If (input<Min) output=0, If
    (input>Max) output=255. Otherwise: if (gamma==0) output=
    INT(255*log(input-Min+1)/log(Max-Min+1)), otherwise
    output = INT(255*pow((input-Min)/(Max-Min),gamma))
    @param pSource is a pointer to the source image
    @param gamma is the power in the nonlinear converion
    (logaritmic conversion if gamma==0)
    @param Min is the value giving zero in *pDest
    @param Max is the value giving 255 in *pDest
    @param pDest is the pointer to the destination image
    @return False if the *pSource is empty or if Min>=Max
    @version 0.80
    @author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_NonlinearCopyFloatToByte(TFloatImage *pSource,float gamma,
                           float Min, float Max, TImage *pDest);

/** copies a complex image into a
    TImage (8 b/p) using a logaritmic function or a power function
    in the gray tone transformation: If (input<Min) output=0, If
    (input>Max) output=255. Otherwise: if (gamma==0) output=
    INT(255*log(input-Min+1)/log(Max-Min+1)), otherwise
    output = INT(255*pow((input-Min)/(Max-Min),gamma))
    @param pSource is a pointer to the source image
	@param mode Determines what part of the complex image is to be
		copied. The values are COMPLEX_REAL, COMPLEX_IMAGINARY, or COMPLEX_MODULUS.
    @param gamma is the power in the nonlinear converion
    (logaritmic conversion if gamma==0)
    @param Min is the value giving zero in *pDest
    @param Max is the value giving 255 in *pDest
    @param pDest is the pointer to the destination image
    @return False if the *pSource is empty or if Min>=Max
    @version 0.80
    @author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_NonlinearCopyComplexToByte(TComplexImage *pSource,COMPLEXCHOISE mode,float gamma,
                           float Min,float Max, TImage *pDest);

/*@}*/ /* end group "localoperations" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_LOCAL_OPERATIONS_H */
