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

#ifndef _IPL98_KERNEL_FEATURE_EXTRACTION_H
#define _IPL98_KERNEL_FEATURE_EXTRACTION_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../ipl98_types.h"
#include "../kernel_pixelgroup.h"
#include "../general_functions/ipl_general_functions_c_only.h"
#include <math.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @defgroup ansic ANSI-C Kernel code
	This is all the functions and structures contained in the kernel ANSI C part
	of IPL98. You will only in very rare occations need to look here, if you are
	using the C++ part of the library.
@{ */

/** @defgroup featureextraction ANSI-C Feature extraction functions
	Kernel ANSI C feature extraction functions, Last updated 11/2/1999.
	Works on TImage structures with both 1 b/p and 8 b/p (graytones).
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 1.01
	@author Ivar Balslev ivb@mip.sdu.dk
    @{ */

/** Structure for moment values.
	Contains moment values for simple and central moments up to order 4. */	
typedef struct
{
	/** @param m00 Moment with order 0. */
	double m00;
	/** @param m10 Moment with order 1. */
	double m10;
	/** @param m01 Moment with order 1. */
	double m01;
	/** @param m20 Moment with order 2. */
	double m20;
	/** @param m11 Moment with order 2. */
	double m11;
	/** @param m02 Moment with order 2. */
	double m02;
	/** @param m30 Moment with order 3. */
	double m30;
	/** @param m21 Moment with order 3. */
	double m21;
	/** @param m12 Moment with order 3. */
	double m12;
	/** @param m03 Moment with order 3. */
	double m03;
	/** @param m40 Moment with order 4. */
	double m40;
	/** @param m31 Moment with order 4. */
	double m31;
	/** @param m22 Moment with order 4. */
	double m22;
	/** @param m13 Moment with order 4. */
	double m13;
	/** @param m04 Moment with order 4. */
	double m04;
} TMoment;

/** Returns the center of mass for given pixelgroup. If you only need information
	about center of mass call this function which is speed optimized.
	@param pPixelGroup The group of pixels to calculate center of mass on. Color values
		in pixelgroup are not used, i.e. all positions have same weight.
	@return Center of mass for given pixelgroup. If PixelGroup is NULL or it
		contains no positions, an error is produced an (0,0) is return. */
T2DFloat k_DeriveCenterOfMassBinary(const TPixelGroup* pPixelGroup);

/**	Moments features as explained in Gonzalez and Woods page 514-515.
	The weight of each
	pixel is the original pixel value minus "Background" (see the "Background" parameter). The
	results are stored in the pMom parameter (see this parameter for more details). The 
	calculation is very fast, and the parameter "MaxOrder" can be used to calculate the
	low order moments only, which makes the funtion even faster. Use the function 
	k_DeriveMomentsBinary() if you want binary moments.
	@param pPixelGroup The group of pixels to calculate moments on. The PixelGroup must
		   include pixel colors.
	@param BackGround The value to subtract from the original pixelvalues.
	@param MaxOrder The maximum order of moments to calculate. If MaxOrder<=2, the moments up to
		   and including second order are calculated, the values of the higher order moments
		   in the paramter "pMom" is then undefined. If MaxOrder>=3 moments up to and including 4
		   are calculated.
	@param pMom The calculated moment values are put into this structure, see the structure for
		   more information.
	@return False if the group contains no pixel color information or one of the parameters
		   are a NULL pointer.
	@version 1.01,  by ivb@mip.sdu.dk
	@author Ivar Balslev (ivb@mip.sdu.dk) */
bool k_DeriveMomentsGrayTone(const TPixelGroup* pPixelGroup,double Background,
							 unsigned int MaxOrder,TMoment* pMom);

/** Calculate binary moments features as explained in Gonzalez and Woods page 514-515.
	This function calculates the same moments as k_DeriveMomentsGrayTone(), except that
	each pixel has the value 1. Use the function k_DeriveMomentsGrayTone() if you want
	gray tone moments.
	@param pPixelGroup The group of pixels to calculate moments on. Color information in the
		   PixelGroup is not needed.
	@param MaxOrder The maximum order of moments to calculate. If MaxOrder<=2, the moments up to
		   and including second order are calculated, the values of the higher order moments
		   in the paramter "pMom" is then undefined. If MaxOrder>=3 moments up to and including 4
		   are calculated.
	@param pMom The calculated moment values are put into this structure, see the structure for
		   more information.
	@return False if one of the parameters are a NULL pointer.
	@version 1.01,  by ivb@mip.sdu.dk
	@author Ivar Balslev (ivb@mip.sdu.dk) */
bool k_DeriveMomentsBinary(const TPixelGroup *pPixelGroup,unsigned int MaxOrder,TMoment* pMom);

/** Central moments as described in Gonzalez and Woods page 514-515.
	Note that these moments are derived from the simple moments calculated
	by k_DeriveMomentsBinary() or k_DeriveMomentsGrayTone().
	@param pMom (TMoment*) This structure must contain the moment values calculated by a
		call to k_DeriveMomentsBinary() or k_DeriveMomentsGrayTone().
	@param pCentrMom (TMoment*) Results will be placed in this structure.
	@return False if one of the parameteres are a NULL pointer.
	@version 1.01,  by ivb@mip.sdu.dk
	@author Ivar Balslev (ivb@mip.sdu.dk) */
bool k_DeriveCentralMoments(const TMoment* pMom, TMoment* pCentrMom);

/** Central normalized moments as described in Gonzalez and Woods page 515.
	Note that these moments are derived from the simple moments calculated
	by k_DeriveMomentsBinary() or k_DeriveMomentsGrayTone().
	@param pMom (TMoment*) This structure must contain the moment values calculated by a
		call to k_DeriveMomentsBinary() or k_DeriveMomentsGrayTone().
	@param pCentrNormMom (TMoment*) Results will be placed in this structure.
	@return False if one of the parameteres are a NULL pointer.
	@version 1.01,  by ivb@mip.sdu.dk
	@author Ivar Balslev (ivb@mip.sdu.dk) */
bool k_DeriveCentralNormMoments(const TMoment* pMom, TMoment* pCentrNormMom);

/** Hu invariant moments as described in Gonzalez and Woods page 516.
	The Hu invariants can be calculated from the central moments or the normalized
	central moments. Read more about the Hu invariants in the article "Visual Pattern 
	Recognition by Moment Invariants", Ming-Kuei HU, IRE Transactions on information 
	theory 1962, p. 179-187. A total of 13 Hu
	moments are calculated (only the first seven are found in Gonzalez and Woods).
	Indexing is done according to the numbering of the invariants, that is
	starting from number 1, index 0 is not used.
	@param pCentrMom (TMoment*) This structure may contain the central moment values
	calculated by k_DeriveCentralMoments() or it may contain the central normalized
	moment values calculated by k_DeriveCentralNormMoments().
	@param pHu (TMoment*) Results will be placed in this structure.
	@return False if one of the parameteres are a NULL pointer.
	@version 1.01,  by ivb@mip.sdu.dk
	@author Ivar Balslev (ivb@mip.sdu.dk) */
bool k_DeriveHuInvariants(const TMoment* pCentrMom,double *pHu);

/*@}*/ /* end group "featureeactraction" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_FEATURE_EXTRACTION_H */
