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

#ifndef _IPL98_KERNEL_STATISTIC_H
#define _IPL98_KERNEL_STATISTIC_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../image/kernel_functions.h"
#include "../int_image/i_kernel_functions.h"
#include "../float_image/f_kernel_functions.h"
#include "../kernel_pixelgroup.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup statistic ANSI-C Statistic container functions
	Kernel ANSI C statistic functions, Last updated 12/7/1999.
	Works on a TStatistic structure. Calculates minimum, maximum, 
	mean and standard deviation for all pixel values in an image.
	Finally a histogram is created except for the CFloatImage type.

	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 0.70
	@author René Dencker Eriksen (edr@mip.sdu.dk)
	@{ */

/** A container for the statistical values.
	More statistical values may be added in later versions. */	
typedef struct {
	/** @param Min Minimum value in image or pixelgroup. */
	double Min;
	/** @param Max Maximum value in image or pixelgroup. */
	double Max;
	/** @param Mean Mean value for whole image or pixelgroup. */
	double Mean;
	/** @param StdDev Standard deviation for whole image or pixelgroup. */
	double StdDev;
	/** @param SizeOfHistogram Size of the histogram array found in pHistogram.
		Note: Not all functions derives a histogram due to the nature of some value 
		types, such as FLOAT32. */
	unsigned int SizeOfHistogram;
	/** @param pHistogram Array of long integers containing the histogram.
		Indexing is from pHistogram[Min] to pHistogram[Max]. */
	UINT32* pHistogram;
} TStatistic;

/** Sets the pointer pHistogram in TStatistic to NULL. Initialises all
	variables to 0. Always call this function when creating a variable
	of the TStatistic type */
void k_InitStatistic(TStatistic* pStatistic);

/**	Deletes all allocated memory for the TStatistic structure and sets variables to 0.
	Always call this function when leaving a scope where a TStatistic
	variable were created and used. */
void k_EmptyStatistic(TStatistic* pStatistic);

/**	Derives statistical information contained in the TStatistic structure
	for the given image. No histogram is created.
	@param pImage (TFloatImage*) The image to calculate statistics for.
	@return False, if the image is empty. */
bool k_DeriveFloatImageStats(TStatistic* pStatistic,const TFloatImage* pImage);

/**	Derives all statistical information contained in the TStatistic structure
	for the given integer image. A histogram with 65536 entries is created.
	Indexing is from -32768 to 32767 (min and max values for the INT16 type).
	@param pImage (TIntImage*) The image to calculate statistics for.
	@return False, if the image is empty. */
bool k_DeriveIntImageStats(TStatistic* pStatistic,const TIntImage* pImage);

/**	Derives all statistical information contained in the TStatistic structure
	for the given image. If the image is 24 b/p the statistics is derived from
	an average of each R,G and B channel, ie. like the corresponding graytone image.
	@param pImage (TImage*) The image to calculate statistics for.
	@return False, if the image is empty. */
bool k_DeriveImageStats(TStatistic* pStatistic,const TImage* pImage);

/**	Derives all statistical information contained in the TStatistic structure
	for the given pixelgroups color values. No histogram is created.
	@param pPixelGroup (TPixelGroup*) The pixelgroup to calculate statistics for.
	@return False, if the pixelgroup is empty or if no colors are available. */
bool k_DerivePixelGroupStats(TStatistic* pStatistic,
							 const TPixelGroup* pPixelGroup);

/** Prints the information contained in "pInfo" to stdout */
void k_PrintStatisticInfo(const TStatistic* pInfo);

/*@}*/ /* end group "statistic" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_STATISTIC_H */
