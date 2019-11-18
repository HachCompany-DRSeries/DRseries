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

#ifndef _IPL98_FEATURE_EXTRACTION_H
#define _IPL98_FEATURE_EXTRACTION_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../../kernel_c/algorithms/kernel_feature_extraction.h"
#include "../image.h"
#include "../pixelgroup.h"

namespace ipl{ // use namespace if C++

/** @ingroup algorithms
@{ */

/** CFeatures A collection of different feature extraction algorithms,
	last updated 8/25/2002.
	At present it includes simple graytone and binary moments, central
	moments, central normalized moments and Hu moments. Access to the results
	is directly through the member variables m_Moments, m_CentrMom and
	m_CentrNormMom. This will probably be changed in future versions, when a
	decision on the representation of features has been made. The members of these
	public attributes is mxy, where x and y is in the range 0 to 4 and the sum
	of x and y is less than or equal to 4, f.ex. m_Moments.m31.
	Works with the CPixelGroup class.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CFeatures ipl98/cpp/algorithms/feature_extraction.h
	@see CPixelGroup
	@version 1.02,  by ivb@mip.sdu.dk
	@author Ivar Balslev (ivb@mip.sdu.dk) */
class CFeatures{
	public: // attributes
		/// Set to true, when DeriveMomentsGrayTone() or DeriveMomentsBinary() has been successfully called.
		bool m_MomOK;
		/// Set to true, when DeriveCentralMoments has been successfully called.
		bool m_CentrMomOK;
		/// Set to true, when DeriveCentralNormMoments() has been successfully called.
		bool m_CentrNormMomOK;
		/// Set to true, when DeriveHuFromCentralMom() or DeriveHuFromCentralNormMom() has been successfully called.
		bool m_HuOK;
		/// Set when DeriveMomentsGrayTone() or DeriveMomentsBinary() has been successfully called.
		TMoment m_Moments;
		/// Set when DeriveCentralMoments() has been successfully called.
		TMoment m_CentrMom;
		/// Set when DeriveCentralNormMoments() has been successfully called.
		TMoment m_CentrNormMom;
		/** The Hu moments derived from DeriveHuFromCentralMom() or DeriveHuFromCentralNormMom().
			Indexing starts from one (that is how Hu moments are originally indexed)!. */
		double m_Hu[13];
	public:  // methods
		/// default constructor
		CFeatures();
		/// default destructor
		~CFeatures();
		/** Returns the center of mass for given pixelgroup. If you only need information
			about center of mass call this function which is speed optimized.
			@param PixelGroup The group of pixels to calculate center of mass on. Color values
				in pixelgroup are not used, i.e. all positions have same weight.
			@return Center of mass for given pixelgroup. If PixelGroup is NULL or it
				contains no positions, an error is produced an (0,0) is return. */
		static CPoint2D<FLOAT32> DeriveCenterOfMassBinary(const CPixelGroup& PixelGroup);
		/**	Moments features as explained in Gonzalez and Woods page 514-515. 
			The weight of each
			pixel is the original pixel value minus "Background" (see the "Background" parameter). The
			results are stored in the m_Moments attribute (look at the TMoment structure for more details).
			The calculation is very fast, and the parameter "MaxOrder" can be used to calculate the
			low order moments only, which makes the funtion even faster. Use the function 
			DeriveMomentsBinary() if you want binary moments.
			
			@param PixelGroup The group of pixels to calculate moments on. The PixelGroup must
				   include pixel colors.
			@param Background The value to subtract from the original pixelvalues.
			@param MaxOrder The maximum order of moments to calculate. If MaxOrder<=2, the moments up to
				   and including second order are calculated, the values of the higher order moments
				   in the paramter "pMom" is then undefined. If MaxOrder>=3 moments up to and including 4
				   are calculated.
			@return False if the group contains no pixel color information.
			@version 1.01,  by ivb@mip.sdu.dk
			@author Ivar Balslev (ivb@mip.sdu.dk)
			@see TMoment */
		bool DeriveMomentsGrayTone(const CPixelGroup& PixelGroup,double Background,
									 unsigned int MaxOrder);

		/** Calculate binary moments features as explained in Gonzalez and Woods page 514-515.
			Calculates the same moments as DeriveMomentsGrayTone(), except that
			each pixel has the value 1. Use the method DeriveMomentsGrayTone() if you want
			gray tone moments.
			@param PixelGroup The group of pixels to calculate moments on. Color information in the
				   PixelGroup is not needed.
			@param MaxOrder The maximum order of moments to calculate. If MaxOrder<=2, the moments up to
				   and including second order are calculated, the values of the higher order moments
				   in the paramter "pMom" is then undefined. If MaxOrder>=3 moments up to and including 4
				   are calculated.
			@return False if one of the parameters are a NULL pointer. 
			@version 1.01,  by ivb@mip.sdu.dk
			@author Ivar Balslev (ivb@mip.sdu.dk)
			@see TMoment */
		bool DeriveMomentsBinary(const CPixelGroup& PixelGroup,unsigned int MaxOrder);

		/** Central moments as described in Gonzalez and Woods page 514-515.
			The results are stored in the m_CentrMom attribute (look at the TMoment structure 
			for more details). Note that these moments are derived from the simple moments calculated
			by DeriveMomentsBinary() or DeriveMomentsGrayTone().
			@return False if the simple moments have not yet been calculated. 
			@version 1.01,  by ivb@mip.sdu.dk
			@author Ivar Balslev (ivb@mip.sdu.dk)
			@see TMoment */
		bool DeriveCentralMoments();

		/** Central normalized moments as described in Gonzalez and Woods page 515.
			The results are stored in the m_CentrNormMom attribute (look at the TMoment structure 
			for more details). Note that these moments are derived from the simple moments calculated
			by DeriveMomentsBinary() or DeriveMomentsGrayTone().
			@return False if the simple moments have not yet been calculated. 
			@version 1.01,  by ivb@mip.sdu.dk
			@author Ivar Balslev (ivb@mip.sdu.dk)
			@see TMoment */
		bool DeriveCentralNormMoments();

		/** Hu invariant moments as described in Gonzalez and Woods page 516.
			Read more about the Hu invariants in the article "Visual Pattern 
			Recognition by Moment Invariants", Ming-Kuei HU, IRE Transactions on information 
			theory 1962, p. 179-187. A total of 13 Hu
			moments are calculated (only the first seven are found in Gonzalez and Woods).
			This method calcates Hu moments from the central moments calculated by
			DeriveCentralMoments(). The method DeriveHuFromCentralNormMom() calculates the
			same moments but from the central normalized moments. The result is placed
			in the public member m_Hu which is an array of 13 doubles.
			Indexing is done according to the numbering of the invariants, that is
			starting from number 1, index 0 is not used.
			@return False if the central moments have not yet been calculated. 
			@version 1.01,  by ivb@mip.sdu.dk
			@author Ivar Balslev (ivb@mip.sdu.dk)
			@see TMoment */
		bool DeriveHuFromCentralMom();
		/** Same as DeriveHuFromCentrMom(), except that the source moments are now
			the central normalized moments. The result is placed in the public member
			m_Hu which is an array of 13 doubles.
			@return False if the central normalized moments have not yet been calculated. 
			@version 1.01,  by ivb@mip.sdu.dk
			@author Ivar Balslev (ivb@mip.sdu.dk)
			@see TMoment */
		bool DeriveHuFromCentralNormMom();
};

/*@}*/ /* end group "algorithms" */

} // end namespace ipl

#endif //_IPL98_FEATURE_EXTRACTION_H
