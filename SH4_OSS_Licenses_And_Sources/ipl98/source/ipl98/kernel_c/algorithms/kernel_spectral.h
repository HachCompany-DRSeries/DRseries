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

#ifndef _IPL98_KERNEL_SPECTRAL_H
#define _IPL98_KERNEL_SPECTRAL_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../ipl98_types.h"
#include "../complex_image/c_kernel_functions.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
@{ */

/** @defgroup spectral ANSI-C Spectral functions
	Kernel ANSI C Spectral operations, FFT performed on a whole image,
	last updated 11/22/99
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 0.80
	@author Ivar Balslev ivb@mip.sdu.dk
	@{ */

/** performs a Fast Fourier Transformation (FFT) (if isign =1)
    or an inverse FFT (if isign=-1).
	@param isign FFT if 1 and inverse FFT if -1
    @param pImg is a pointer to the source-target image
    @return False if the width or the height is not a power of 2
            or if *pImg is empty */
bool k_FFT(int isign, TComplexImage *pImg);

/*@}*/ /* end group "spectral" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_SPECTRAL_H */
