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

#ifndef _IPL98_MASK_OPERATION_H
#define _IPL98_MASK_OPERATION_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../std_image.h"
#include "../float_image.h"
#include "../int_image.h"

namespace ipl{ // use namespace if C++

/** @ingroup algorithms
@{ */

/** CMaskOperation: Simple Mask operations, last updated 2/1/2003.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CMaskOperation ipl98/cpp/algorithms/mask_operation.h
	@version 0.87
	@author Ivar Balslev ivb@mip.sdu.dk **/
class CMaskOperation{
	private: // attributes
	public: // methods
		/// default constructor
		CMaskOperation();
		/// default destructor
		~CMaskOperation();

		/** calculates the convolution of Img and Mask. Mask is
			always a float image. No dimension of Mask should exceed that of Img.
			Boundary conditions periodic with width and height of Img are
			assumed. Reference cell is (0,0) of Mask. Apply CopyDisplace or
			SetOrigo to Img in case another reference cell is desired.
			@param Img is the source-target image of class CImage or CByteImage
			@param Mask is the mask image of class CFloatImage
			@return False if Img and/or Mask are empty or if the dimensions of 
				Mask exceed those of Img or if Img does not have 8 bits/pixel
			@version 0.85
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool Convolve(CStdImage &Img,const CFloatImage &Mask);
		/** calculates the convolution of Source and Mask. Mask is always
			a float image. No dimension of Mask should exceed that of Source.
			Boundary conditions periodic with width and height of Source are
			assumed. Reference cell is (0,0) of Mask. Apply CopyDisplace or
			SetOrigo to Img in case another reference cell is desired.
			@param Source is a pointer to the source image of class CImage or CByteImage
			@param Mask is the mask image of class CFloatImage
			@param Dest is  the destination image of class CIntImage
			@return False if Source and/or Mask are
				empty or if the dimensions of Mask exceed those of *pSource
			@version 0.85
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool Convolve(CStdImage &Source,const CFloatImage &Mask,CIntImage &Dest);
		/** calculates the convolution of Img and Mask. Mask is
			always a float image. No dimension of Mask should exceed that of Img.
			Boundary conditions periodic with width and height of Img are
			assumed. Reference cell is (0,0) of Mask. Apply CopyDisplace or
			SetOrigo to Img in case another reference cell is desired.
			@param Img is the source-target image of class CIntImage
			@param Mask is the mask image of class CFloatImage
			@return False if Img and/or Mask are empty
				or if the dimensions of Mask exceed those of *pImg
				or if Source does not have 8 bits/pixel
			@version 0.85
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool Convolve(CIntImage &Img,const CFloatImage &Mask);
		/** Convolve calculates the convolution of Img and Mask. Mask is
			always a float image. No dimension of Mask should exceed that of Img.
			Boundary conditions periodic with width and height of Img are
			assumed. Reference cell is (0,0) of Mask. Apply CopyDisplace or
			SetOrigo to Img in case another reference cell is desired.
			@param Img is  the source-target image of class CFloatImage
			@param Mask is  the mask image of class CFloatImage
			@return False if Img and/or Mask are empty or if the dimensions 
				of Mask exceed thoso of Img
			@version 0.85
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool Convolve(CFloatImage &Img,const CFloatImage &Mask);
		/** Blurs the image. It is a speed optimized blur algorithm
			replacing each graytone by the simple average of the gray
			tones of central cell and 4 nearest pixels. BlurFourConnected3x3 operates on
			opjects of class CImages and CByteImages.
			Edge pixels are calculated using a
			mirror image of the old edge pixels about the image edges.
			@param Img is the source-target image of type CImage or CByteImage
			@return False if Img is empty or have dimensions smaller than 4x4
			@version 0.85
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool BlurFourConnected3x3(CStdImage &Img);
		/** Blurs the image. It is a speed optimized blur algorithm
			replacing each graytone by the simple average of the
			gray tones of central cell and 8 nearest pixels. BlurEightConnected3x3
			operates on objects of class CImages or CByteImages.
			Edge pixels are calculated using a mirror image of the old
			edge pixels at the image edges.
			@param Img is the source-target image of type CImage or CByteImage
			@return False if Img is empty or have dimensions smaller than 4x4
				or if Img does not have 8 bits/pixel
			@version 0.85
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool BlurEightConnected3x3(CStdImage &Img);
		/** Same functionality as BlurEightConnected3x3(CStdImage) but works on
			a CIntImage container.
			@param Img is the source-target image.
			@return False if Img is empty or have dimensions smaller than 4x4.
			@version 0.80
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool BlurEightConnected3x3(CIntImage &Img);
		/** ConvolveFast performs a convolutions of Img using a 3x3
			CFloatImage Mask. The (result % 256) is saved in a CImage or CByte image.
			The mask must be a 3x3 image. The algorithm is very fast if the
			8 non-center elements are 1 or 0. The prefactor can be used for
			achieving this. The reference cell is the center cell. Edge pixels
			in Dest are set to the same gray tone
			as the nearest neighbour non-edge pixel.
			@param Img is the source-target image of type CImage or CByteImage
			@param Mask is the 3x3 mask image of class CFloatImage
			@param Prefactor is a common prefactor scaling the image Dest
			@return False if Img is empty or if the dimensions of pMask is not 3x3
				or if Img does not have 8 bits/pixel
			@version 0.85
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool ConvolveFast(CStdImage &Img,const CFloatImage& Mask,float Prefactor);
		/** ConvolveFast performs a convolutions of pSource using a 3x3
			CFloatImage Mask. The resultis saved in a CIntImage.
			The mask must be a 3x3 image. The algorithm is very fast if the
			8 non-center elements are 1 or 0. The prefactor can be used for
			achieving this. The reference cell is the center cell. Edge pixels
			in Dest are set to the same gray tone
			as the nearest neighbour non-edge pixel.
			The history of Source is copied to Dest, old history in Dest is deleted.
			@param Source is the source-target image of class CIntImage
			@param Mask is the 3x3 mask image of class CFloatImage
			@param Prefactor is a common prefactor scaling the image Dest
			@param Dest is the destination image of class CIntImage
			@return False if Source is empty
			or if the dimensions of Mask is not 3x3
			or if Source does not have 8 bits/pixel
			@version 0.85
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool ConvolveFast(CStdImage &Source, const CFloatImage &Mask,
					float Prefactor, CIntImage &Dest);

		/** ConvolveFast performs a convolutions of Source of class CIntImage
			using a 3x3 CFloatImage Mask. The result is saved in a CIntImage.
			The mask must be a 3x3 image. The algorithm is very fast if the
			8 non-center elements are 1 or 0. The prefactor can be used for
			achieving this. The reference cell is the  center cell. Edge pixels
			in Dest are set to the same gray tone
			as the nearest neighbour non-edge pixel.
			@param Img is the source-target image of type CImage or CByteImage
			@param Mask is the 3x3 mask image of type TFloatImage
			@param Prefactor is a common prefactor scaling the image Dest
			@return False if Source is empty or if the dimensions of Mask is not 3x3
			@version 0.85
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool ConvolveFast(CIntImage &Img, const CFloatImage &Mask,float Prefactor);
		/** MaxGradientFast is an edge detecting algoritm.
			It calculates the maximum absolute graytone difference
			of two opposite neighbour pixels over the 4 possible directions.
			Diagonal differences have weight 2/3. The result is stored. The gray tone of edge
			pixels are set to zero.
			@param Img is  the source-target image of class CImage or CByteImage
			@param Prefactor is a common prefactor scaling the image Dest
			@return False if Img is empty or if Img does not have 8 bits/pixel
			@version 0.85
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool MaxGradientFast(CStdImage &Img,float Prefactor);
		/** MaxGradientFast is an edge detecting algoritm.
			It calculates the maximum absolute graytone difference
			of two opposite neighbour pixels over the 4 possible directions.
			Diagonal differences have weight 2/3. The result is stored in an integer
			image. The gray tone of edge pixels are set to zero.
			@param Source the source image of class CImage or CByteImage
			@param Prefactor is a common prefactor scaling the image Dest
			@param Dest is  the destination image of class CIntImage
			@return False if *pSource is empty or if Source does not have 8 bits/pixel
			@version 0.85
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool MaxGradientFast(CStdImage &Source,float Prefactor,CIntImage &Dest);
		/** MaxGradientFast is an edge detecting algoritm.
			It calculates the maximum absolute graytone difference
			of two opposite neigbour pixels over the 4 possible directions.
			Diagonal differences have weight 2/3. The result is stored. The gray tone of edge
			pixels are set to zero.
			@param Img is the source-target image of class CIntImage
			@param Prefactor is a common prefactor scaling the image Dest
			@return False if Img is empty
			@version 0.85
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool MaxGradientFast(CIntImage &Img,float Prefactor);
		/** MedianFilterFourConnected3x3 takes the median of the graytone among the 5
			pixels. The pixels form a plus sign about each pixel. Edge pixels are unchanged.
			@param Img is the source-destination image of class CImage or CByteImage
			@return False if Source is empty or if Img does not have 8 bits/pixel
			@version 0.85
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool MedianFilterFourConnected3x3(CStdImage &Img);
		/** takes the median of the graytone among the 5 pixels.
			The pixels form a diagonal cross about each pixel. Edge pixels are
			unchanged.
			@param Img is the source-destination image of class CImage or CByteImage
			@return False if Source is empty or if Img does not have 8 bits/pixel
			@version 0.85
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool MedianFilterDiagonalConnected3x3(CStdImage &Img);
		/** takes the median of the graytone among the 5 pixels.
			The pixels form a plus sign about each pixel. Edge pixels are unchanged.
			@param Img is the source-destination image of class CIntImage
			@return False if Source is empty
			@version 0.85
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool MedianFilterFourConnected3x3(CIntImage &Img);
		/** takes the median of the graytone among the 5
			pixels. The pixels form a diagonal cross about each pixel.
			Edge pixels are unchanged.
			@param Img is the source-destination image of class CIntImage
			@return False if Source is empty
			@version 0.85
			@author Implemented by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool MedianFilterDiagonalConnected3x3(CIntImage &Img);
};

/*@}*/ /* end group "algorithms" */

} // end namespace ipl

#endif //_IPL98_MASK_OPERATION_H
