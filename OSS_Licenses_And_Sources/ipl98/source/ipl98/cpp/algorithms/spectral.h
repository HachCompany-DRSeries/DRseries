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

#ifndef _IPL98_SPECTRAL_H
#define _IPL98_SPECTRAL_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../../kernel_c/algorithms/kernel_spectral.h"
#include "../std_image.h"
#include "../float_image.h"
#include "../complex_image.h"

namespace ipl{ // use namespace if C++

/** @ingroup algorithms
@{ */

/** CSpectral: Fourier and related transforms, Last updated 10/9/2001.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CSpectral ipl98/cpp/algorithms/spectral.h
	@version 1.00
	@author Ivar Balslev ivb@mip.sdu.dk **/
class CSpectral{
private: // attributes
public: // methods
	/// default constructor
	CSpectral();
	/// default destructor
	~CSpectral();
	/** FFTForward performs a Fast Fourier Transformation (FFT)
		@param Img is the source-target image
		@return False if the width or the height is not a power of 2
			or if Img is empty
		@version 1.00 */
	static bool FFTForward(CComplexImage &Img);
	/** FFTInverse performs an inverse Fast Fourier Transformation (FFT)
		@param Img is the source-target image
		@return False if the width or the height is not a power of 2
			or if Img is empty
		@version 1.00 */
	static bool FFTInverse(CComplexImage &Img);
	/** Correlation using the fast fourier methods FFTForward() and FFTInverse()
		in this class. This method takes care of resizing the image dimensions to
		a power of 2, zeropadding is added as an average of the whole image.
		Example on using this method:
		\verbatim
		CFloatImage Dest;
		CImage View, z1, z2;
		z1.Load("d:/temp/fnta-abc.bmp");
		z2.Load("d:/temp/a.bmp");
		CSpectral::DeriveCorrelation(z1,z2,Dest);
		// scale the CFloatImage and copy to View image object
		CLocalOperation LO;
		CStatistic Stat;
		Stat.DeriveStats(Dest);
		LO.NonlinearCopyToByte(Dest,10,Stat.GetMin(),Stat.GetMax(),View);

		\endverbatim
		@param Lg The large input image to search in.
		@param Sm The small input image to look for in Lg.
		@param Dest Resulting correlation result placed in this parameter.
		@return False, if one of Lg or Sm is empty.
		@version 0.70 */ 
	static bool DeriveCorrelation(const CStdImage &Lg, const CStdImage &Sm, CFloatImage &Dest);
};

/*@}*/ /* end group "algorithms" */

} // end namespace ipl

#endif //_IPL98_SPECTRAL_H
