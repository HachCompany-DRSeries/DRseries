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

#ifndef _IPL98_SEGMENTATE_H
#define _IPL98_SEGMENTATE_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../std_image.h"
#include "../pixelgroups.h"

namespace ipl{ // use namespace if C++

/** @ingroup algorithms
@{ */

/** CSegmentate A collection of different segmentation algorithms,
	last updated 6/21/2001.
	Works with the CStdImage class.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CSegmentate ipl98/cpp/algorithms/segmentate.h
	@version 1.41, by ivb@mip.sdu.dk
	@author Ivar Balslev and René Dencker Eriksen */
class CSegmentate{
	private: // attributes
	public:  // methods
		/// default constructor
		CSegmentate();
		/// default destructor
		~CSegmentate();
	/** Derive blobs from image and put results in CPixelGroups. Old data in
		"PixelGroups" are destroyed. If you want the
		original colors added to the found pixel groups call the method
		AddColors(...) found in the CPixelGroups class. 
		@param Source Source image to derive blobs from. Image is left unchanged.
		@param PixelGroups Resulting pixelgroups are stored in this object, old information
			is destroyed.
		@param Color The color to segmentate into blobs, values are HIGHCOLOR and LOWCOLOR.
		@param Threshold A simpel threshold value to seperate background from
				blobs in graytone images. not used if source image is 1 b/p.
		@param Connected Decide if the blobs are to be eight- or fourconnected,
				as described in Gonzalez and Woods page 41.
		@param MinSize Blobs containing less than MinSize pixels are discarded, defaults to 1, i.e.
			all blobs are derived. Discarding more blobs makes this algorithm faster.
		@return False if source image given to constructor is empty or
			not 1 b/p or 8 b/p.
		@version 1.40
		@author Implementation by Ivar Balslev (ivb@mip.sdu.dk) and René Dencker Eriksen (edr@mip.sdu.dk) */
	static bool DeriveBlobs(CStdImage& Source, CPixelGroups& PixelGroups,
						COLORTYPE Color, UINT8 Threshold, CONNECTIVITY Connected, unsigned int MinSize=1);

	/** Derives the blob belonging to the given start position (seed). If the image is
		1 b/p the pixel color at the seed position decides the color of the blob to be
		found. It the image is 8 b/p, the threshold parameter is used and the blob found
		will be either below or above threshold regarding the color of the seed position.
		If you want the original colors added to the found pixel groups call the method
		AddColors(...) found in the CPixelGroup class. 
		@param Seed A starting position in the group to be extracted.
		@param Source The source image to derive from, Image is left unchanged.
		@param PixelGroup Resulting group is stored in this object, old information is destroyed.
		@param Threshold A simpel threshold value to seperate the blob from background in
				graytone images. Not used if source image is 1 b/p.
		@param Connected Decide if the blobs are to be eight- or fourconnected,
				as described in Gonzalez and Woods page 41.
		@return False if source image given to constructor is empty or
				not 1 b/p or 8 b/p.
		@version 0.40
		@author Steen M. Madsen and Rolf Thunbo. IPL98 integration by 
				René Dencker Eriksen (edr@mip.sdu.dk) */
	static bool BlobExpansion(const CPoint2D<INT16>& Seed, CImage& Source, 
						CPixelGroup& PixelGroup,UINT8 Threshold, CONNECTIVITY Connected);

	/** Finds the low contour in the image from given start point and search direction.
		If no border is present a high color (white) border is used internally. If set by the
		algorithm the border is removed again before returning.
		@param Source Source image, if source image is 8 b/p the low color must be 0 and
			all other values are treated as high color. Source image is left unchanged.
		@param Start Position to begin search from.
		@param SearchDirection Direction to search, beginning at "Start". Must be either 
			NORTH, EAST, SOUTH or WEST.
		@param Connected Connectivity may be EIGHTCONNECTED or FOURCONNECTED.
		@param PixelGroup Result placed in this pixel group. Old values are always destroyed.
		@param RotationDirection Resulting direction in which the contour was found, the
			pixels in pPixelGroup is ordered according to this direction.
		@param SurroundedColor Returns the color surrounded by the found contour.
		@return False in these cases: If SearchDirection is not one of NORTH, EAST, SOUTH or WEST. If source
			image is empty, If source image is 24 b/p.
		@version 1.02
		@author Implementation by Ivar Balslev (ivb@mip.sdu.dk)
		@see FindAndFollowHighContour */
	static bool FindAndFollowLowContour(CStdImage& Source, const CPoint2D<int>& Start,
							   DIRECTION SearchDirection, CONNECTIVITY Connected,
							   CPixelGroup& PixelGroup,ROTATION& RotationDirection,
							   COLORTYPE& SurroundedColor);

	/** Finds the high contour in the image from given start point and search direction.
		If no border is present a low color (black) border is used internally. If set by the 
		algorithm the border is removed again before returning.
		Same parameters as FindAndFollowLowContour().
		@version 1.02
		@author Implementation by Ivar Balslev (ivb@mip.sdu.dk)
		@see FindAndFollowLowContour */
	static bool FindAndFollowHighContour(CStdImage& Source, const CPoint2D<int>& Start,
								DIRECTION SearchDirection, CONNECTIVITY Connected,
								CPixelGroup& PixelGroup,ROTATION& RotationDirection,
								COLORTYPE& SurroundedColor);

	/** Finds the low contour in the image from given start point and search direction,
		works as FindAndFollowLowContour(...) except a threshold is supplied.
		If no border is present a low color (black) border is used internally. If set by the 
		algorithm the border is removed again before returning.
		@param Source Source image, must be 1 or 8 b/p.
		@param Start Position to begin search from.
		@param Threshold Pixel values above this value is considered as HIGHCOLOR
		@param SearchDirection Direction to search, beginning at "Start". Must be either 
			NORTH, EAST, SOUTH or WEST.
		@param Connected Connectivity may be EIGHTCONNECTED or FOURCONNECTED.
		@param PixelGroup Result placed in this pixel group. Old values are always destroyed.
		@param RotationDirection Resulting direction in which the contour was found, the
			pixels in PixelGroup is ordered according to this direction.
		@param SurroundedColor Returns the color surrounded by the found contour.
		@return False in these cases: If SearchDirection is not one of NORTH, EAST, SOUTH or WEST. If source
			image is empty, If source image is 24 b/p.
		@version 0.90
		@author Implementation by Ivar Balslev (ivb@mip.sdu.dk)
		@see FindAndFollowLowContour */
	bool FindAndFollowLowContourWithThreshold(CStdImage& Source, const CPoint2D<int>& Start,int Threshold,
								DIRECTION SearchDirection, CONNECTIVITY Connected,
								CPixelGroup& PixelGroup,ROTATION& RotationDirection,
								COLORTYPE& SurroundedColor);

	/** Finds the high contour in the image from given start point and search direction,
		works as FindAndFollowHighContour() except a threshold is supplied.
		If no border is present a low color (black) border is used internally. If set by the 
		algorithm the border is removed again before returning.
		@param Source Source image, must be 1 or 8 b/p.
		@param Start Position to begin search from.
		@param Threshold Pixel values above this value is considered as HIGHCOLOR
		@param SearchDirection Direction to search, beginning at "Start". Must be either 
			NORTH, EAST, SOUTH or WEST.
		@param Connected Connectivity may be EIGHTCONNECTED or FOURCONNECTED.
		@param PixelGroup Result placed in this pixel group. Old values are always destroyed.
		@param RotationDirection Resulting direction in which the contour was found, the
			pixels in PixelGroup is ordered according to this direction.
		@param SurroundedColor Returns the color surrounded by the found contour.
		@return False in these cases: If SearchDirection is not one of NORTH, EAST, SOUTH or WEST. If source
			image is empty, If source image is 24 b/p.
		@version 0.90
		@author Implementation by Ivar Balslev (ivb@mip.sdu.dk)
		@see FindAndFollowHighContour */
	bool FindAndFollowHighContourWithThreshold(CStdImage& Source, const CPoint2D<int>& Start,int Threshold,
								DIRECTION SearchDirection, CONNECTIVITY Connected,
								CPixelGroup& PixelGroup,ROTATION& RotationDirection,
								COLORTYPE& SurroundedColor);
};

/*@}*/ /* end group "algorithms" */

} // end namespace ipl

#endif //_IPL98_SEGMENTATE_H
