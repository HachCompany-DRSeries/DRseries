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

#ifndef _IPL98_GENERICIMAGE_H
#define _IPL98_GENERICIMAGE_H

#include "../ipl98_setup.h" /* always include the setup file */
#include <string>
#include "../kernel_c/ipl98_types.h"
#include "../../points/point2d.h"

/** Namespace for the Image Processing Library (ipl) system. This prevents 
	nameclash with classes from other libraries. */
namespace ipl{ // use namespace if C++

using std::string;

/** @ingroup containerclasses
@{ */

/** CGenericImage is an abstract base class from which all
	image classes are derived, last updated 8/21/2005.
	At the moment several derived classes exists: CStdImage, CFloatImage, CIntImage
	and CComplexImage. The purpose of this class is to define some basic operations
	all image classes must have. Note that access to image data cannot be defined
	here since the data types are different. All derived classes should have a
	GetPixel and SetPixel with its own type as a minimum.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CGenericImage ipl98/cpp/generic_image.h
	@see CStdImage
	@see CFloatImage
	@see CIntImage
	@see CComplexImage
	@version 0.91,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
class CGenericImage
{
	protected: // attributes
		/// Set to true if SetROI()
		bool m_ROIActive;
		/// Upper left position of ROI, relative to Origo. Only valid if m_ROIActive is active
		CPoint2D<INT32> m_ROIUpperLeft;
		/// Lower right position of ROI, relative to Origo. Only valid if m_ROIActive is active
		CPoint2D<INT32> m_ROILowerRight;
	public: // methods
		/// default destructor
		virtual ~CGenericImage() {};
		/// deletes all allocated memory for image and resets variables
		virtual void Empty()=0;
		/** Checks if any data is present.
			@return True, if image contain no data. */
		virtual bool IsEmpty() const=0;
		/** load file of type given by extension from disk.
			@param pPathAndFileName Name of file including extension and relative
			or absolute path, f.ex. absolut path "c:/temp/doggie.bmp",
			relative "doggie.bmp". Both slash ("/") and back slash ("\") can be
			used in the path. There is no differense between upper
			and lower case characters, they are treated equally.
			@return false if load failed. */
		virtual bool Load(const char* pPathAndFileName)=0;
		/** load file of type given by extension from disk.
			@param PathAndFileName Name of file including extension and relative
			or absolute path, f.ex. absolut path "c:/temp/doggie.bmp",
			relative "doggie.bmp". Both slash ("/") and back slash ("\") can be
			used in the path. There is no differense between upper
			and lower case characters, they are treated equally.
			@return false if load failed. */
		virtual bool Load(const string& PathAndFileName)=0;
		/** save file of type given by extension to disk.
			@param pPathAndFileName Name of file including extension and relative
			or absolute path, f.ex. absolut path "c:/temp/doggie.bmp",
			relative "doggie.bmp". Both slash ("/") and back slash ("\") can be
			used in the path. There is no differense between upper
			and lower case characters, they are treated equally.
			@return true if saved. */
		virtual bool Save(const char* pPathAndFileName)=0;
		/** save file of type given by extension to disk.
			@param PathAndFileName Name of file including extension and relative
			or absolute path, f.ex. absolut path "c:/temp/doggie.bmp",
			relative "doggie.bmp". Both slash ("/") and back slash ("\") can be
			used in the path. There is no differense between upper
			and lower case characters, they are treated equally.
			@return true if saved. */
		virtual bool Save(const string& PathAndFileName)=0;
		/** Get the file name for image, if available.
			@param Str The file name is passed in this parameter, empty if not available.
			@return False, if file name not available. */
		virtual bool GetFileName(string& Str)=0;
		/** Get the path for image, if available.
			@param Str The file name is passed in this parameter, empty if not available.
			@return False, if path not available. */
		virtual bool GetPathName(string& Str)=0;
		/** Get the complete path and file name for image, if available.
			@param Str The complete path and file name is passed in this parameter,
			empty if not available.
			@return False, if path and file name not available. */
		virtual bool GetPathAndFileName(string& Str)=0;
		/** @return width of image. */
		virtual UINT32 GetWidth() const=0;
		/** @return height of image. */
		virtual UINT32 GetHeight() const=0;
		/** Set the size of border. */
		virtual bool SetBorderSize(UINT16 Size)=0;
		/** @return size of border */
		virtual UINT16 GetBorderSize() const=0;
		/** @return byte width of a scanline */
		virtual UINT16 GetLineByteWidth() const=0;
		/** Iterate from this value.
			@return min. X relative to origo */
		virtual int GetMinX() const=0;
		/** Iterate from this value.
			@return min. Y relative to origo */
		virtual int GetMinY() const=0;
		/** Iterate to this value minus one (the image is zero indexed).
			@return max. X relative to origo */
		virtual int GetMaxX() const=0;
		/** Iterate to this value minus one (the image is zero indexed).
			@return max. Y relative to origo */
		virtual int GetMaxY() const=0;
		/** Checks if the given position is within range of the image by using
			the range methods GetMinX(), GetMaxX(), GetMinY(), and GetMaxY(). For
			the min values the comparison is x>=GetMinX()-Delta and for the max.
			comparison it is x<GetMaxX()+Delta. Similar with the y-value.
			@param x Position in x-direction.
			@param y Position in y-direction.
			@param Delta The delta value is subtracted from the min. ranges and
				added to the max. ranges. The parameter defaults to 0.
			@return True, if the provided position is within range (including Delta)
				of the image. */
		inline bool InRange(int x, int y, int Delta=0) const;
		/** Same as InRange(int,int,Delta), except the position is given
			as a CPoint2D type. */
		inline bool InRange(CPoint2D<INT16> P, int Delta=0) const;
		/** @return the origin of image */
		virtual ORIGIN GetOrigin() const=0;
		/** Get origo for image 
			@return A 2D point - position may be negative or greater than image size */
		virtual CPoint2D<int> GetOrigo() const=0;
		/** @name Region Of Interest (ROI). A specific area of an image can be set as
				the region of interest. This will cause certain methods to operate on
				that area only. This is faster than copying the area to a new image and
				work on it from there. The following methods will return values relative
				to an active ROI (and origo):
			\verbatim
				GetMinX(), GetMinY()
				GetMaxX(), GetMaxY()
				All iterator related methods.
			\endverbatim
				To make sure your algorithm will take advantage of a ROI, use row
				(or column) iterators to access pixels. Learn more about iterators by
				reading documentation in the derived image classes. */
		//@{ 
		/** Returns true, if Region Of Interest (ROI) is activated by a previous call
			to SetROI().
			@return True, if ROI is active. */
		inline bool ROIActive() const;
		/** Sets Region Of Interest (ROI) for this object. If outside image dimensions
			false is returned and an error is produced. In that case the state of ROI
			is left unchanged. Image dimensions are the values returned from the methods
			GetMinX(), GetMaxX(), GetMinY() and GetMaxY(), i.e. it includes origo offset.
			@param UL Upper left value of ROI, relative to origo.
			@param LR Lower right value of ROI, relative to origo.
			@return False, if values are outside image dimensions, relative to origo. */
		virtual inline bool SetROI(const CPoint2D<INT32>& UL, const CPoint2D<INT32>& LR)=0;
		/** Disables the ROI functionality. */
		inline void DisableROI();
		/** Returns upper left value of Region Of Interest (ROI). User must assure
			that ROI is active.
			@return Upper left value of ROI. */
		inline CPoint2D<INT32> GetROIUpperLeft() const;
		/** Returns lower right value of Region Of Interest (ROI). User must assure
			that ROI is active.
			@return Upper left value of ROI. */
		inline CPoint2D<INT32> GetROILowerRight() const;
		//@}
		/** Rotate image clockwise in step of 90 degrees.
			@param Steps Rotate image 90 degrees * Steps */
		virtual void Rotate90(int Steps)=0;
		/** Flip image around a centered horisontal line. */
		virtual void FlipHorisontal()=0;
		/** Flip image around a centered vertical line. */
		virtual void FlipVertical()=0;
		/// prints every important data in image to stdout
		virtual void PrintInfo() const=0;
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

inline bool CGenericImage::InRange(int x, int y, int Delta) const
{
	if ((x>=(GetMinX()-Delta)) && (x<(GetMaxX()+Delta)) &&
		(y>=(GetMinY()-Delta)) && (y<(GetMaxY()+Delta)))
		return true;
	else
		return false;
}

inline bool CGenericImage::InRange(CPoint2D<INT16> P, int Delta) const
{
	return InRange(P.GetX(),P.GetY(),Delta);
}

inline bool CGenericImage::ROIActive() const
{
	return m_ROIActive;
}

inline void CGenericImage::DisableROI()
{
	m_ROIActive=false;
}

inline CPoint2D<INT32> CGenericImage::GetROIUpperLeft() const
{
	return m_ROIUpperLeft;
}

inline CPoint2D<INT32> CGenericImage::GetROILowerRight() const
{
	return m_ROILowerRight;
}

/*@}*/ /* end group "containerclasses" */

} // end namespace ipl

#endif //_IPL98_GENERICIMAGE_H
