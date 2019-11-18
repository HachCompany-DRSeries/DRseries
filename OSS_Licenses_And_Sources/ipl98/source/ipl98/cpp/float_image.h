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

#ifndef _IPL98_FLOATIMAGE_H
#define _IPL98_FLOATIMAGE_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "image_row_it.h"
#include "image_const_row_it.h"
#include "image_column_it.h"
#include "image_const_column_it.h"
#include "generic_image.h"
#include "text.h"
#include "error.h"
#include "../kernel_c/float_image/f_kernel_functions.h"
#include "../kernel_c/kernel_string.h"
#include "../kernel_c/kernel_error.h"
#include <sstream>
#include <string>

namespace ipl{ // use namespace in C++

using std::ostringstream;
using std::string;
// external definitions
class CIntImage;
class CComplexImage;
class CStdImage;

/** @ingroup containerclasses
@{ */

/** CFloatImage is a class derived from CGenericImage consisting
	of pixels with type FLOAT32, last updated 12/29/2005. Saving and loading
	images of this type has not yet been fully tested and documentation
	for the file format still need to be written. Though it should be
    possible to save and load and all bug reports are welcome!

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CFloatImage ipl98/cpp/float_image.h
	@see CGenericImage
	@version 1.54,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */

class CFloatImage : CGenericImage{
	public: // attributes
		/// History, keeps track of operations performed on image
		CText m_History;
	protected: // attributes
		/// The structure holding all information about the image
		TFloatImage m_Image;

	public: // methods
		/// Default constructor
		CFloatImage();
		/** Constructor for empty image, allocates memory.
			All pixel values are initialised to 0. ROI is inactivated.
			@param Width The width of image in pixels.
			@param Height The height of image in pixels.
			@param Color Initial color value in all pixels. Defaults to 0. The
				function is faster when Color initializes to 0. */
		CFloatImage(UINT32 Width, UINT32 Height, FLOAT32 Color=0);
		/** Copy constructor. ROI is inactivated.
			@param SourceImage Image to copy into this image */
		CFloatImage(const CFloatImage& SourceImage); 
		/** Overloading of assignment operator. All information is copied
			to the destination image including border pixels, ROI and history.
			Previous data in this object are destroyed. Origin is set to RAM. File
			information is left empty. Adds new line to history.
			An error is produced if both sides of the 
			assignment operator is the same object. */
		CFloatImage& operator=(const CFloatImage& SourceImage);
		/// Default destructor
		~CFloatImage();
		/** Allocates memory for image with given size.
			All pixel values are initialised to 0. ROI is inactivated.
			@param Width The width of image in pixels
			@param Height The height of image in pixels
			@param Color Initial color value in all pixels. Defaults to 0. The
				function is faster when Color initializes to 0.
			@return False, if allocation failed.
			@see AllocFast */
		bool Alloc(UINT32 Width, UINT32 Height, FLOAT32 Color=0);
		/** Allocates memory for image with given size.
			Pixel values are not initialised. ROI is inactivated.
			@param Width The width of image in pixels
			@param Height The height of image in pixels
			@return False, if allocation failed.
			@see Alloc */
		bool AllocFast(UINT32 Width, UINT32 Height);
		/** Loads file of type given by extension from disk.
			Adds new line to history. ROI is inactivated.
			@param pPathAndFileName Name of file including extension and relative
			or absolute path, f.ex. absolut path "c:/temp/doggie.bmp",
			relative "doggie.bmp". Both slash ("/") and back slash ("\") can be
			used in the path. There is no differense between upper
			and lower case characters, they are treated equally.
			@return false if load failed */
		bool Load(const char* pPathAndFileName);
		/** Same as Load(const char* pPathAndFileName) except the file name and path
			is given in a string object. */
		bool Load(const string& PathAndFileName);
		/** Saves file of type given by extension to disk.
			Adds new line to history.
			@param pPathAndFileName Name of file including extension and relative
			or absolute path, f.ex. absolut path "c:/temp/doggie.bmp",
			relative "doggie.bmp". Both slash ("/") and back slash ("\") can be
			used in the path. There is no differense between upper
			and lower case characters, they are treated equally.
			@return true if saved */
		bool Save(const char* pPathAndFileName);
		/** Same as Save(const char* pPathAndFileName) except the file name and path
			is given in a string object. */
		bool Save(const string& PathAndFileName);
		/** Deletes all allocated memory for image and resets variables. ROI is inactivated,
			history is cleared. */
		void Empty();
		/** Checks if any data is present.
			@return True, if image contain no data. */
		inline bool IsEmpty() const;
		/** Get the file name for image, if available.
			@param Str The file name is passed in this parameter, empty if not available.
			@return False, if file name not available. */
		bool GetFileName(string& Str);
		/** Same as GetFileName(string& Str) except a character pointer to the file
			name is returned.
			@return NULL if file name is unavailable. */
		const char* GetFileName();
		/** Get the path for image, if available.
			@param Str The file name is passed in this parameter, empty if not available.
			@return False, if path is unavailable. */
		bool GetPathName(string& Str);
		/** Same as GetPathName(string& Str) except a character pointer to the path
			is returned.
			@return NULL if path is unavailable. */
		const char* GetPathName();
		/** Get the complete path and file name for image, if available.
			@param Str The complete path and file name is passed in this parameter,
			empty if not available.
			@return False, if path and file name is unavailable. */
		bool GetPathAndFileName(string& Str);
		/** Same as GetPathAndFileName(string& Str) except a character pointer to the path
			and file name is returned.
			@return NULL if path and file name is unavailable. */
		const char* GetPathAndFileName();
		/**	Sets all pixels in the image to given color. Old pixel values are destroyed.
			Adds new line to history.
			@param Color New pixel values in image.
			@return False if image is empty */
		bool Flush(FLOAT32 Color);
		/** @name Iterator functionality
			Iterators has been added to standardize the way to iterate through pixel
			values in the image classes of IPL98. To learn about iterators, look at the
			standard library iterators. Implemented iterators in the image classes are
			row iterators, column iterators, const row iterators and const column iterators.
			All are forward iterators only, i.e. only the ++ operator is available. They
			do include the [] operator for random access though. Typedefs are available in 
			this class for each iterator type, just like the standard library. Here is a
			short example on using an iterator:
			\verbatim

			int RowNr=1;
			CFloatImage MyImg;
			// do something to fill image data into MyImg ...
			CFloatImage::ConstRowIterator it=MyImg.ConstRow(RowNr);
			// this example writes all pixel values in rownr to standard out stream
			while(it!=it.End())
				cout << *it++ << " ";
			\endverbatim
		*/
		//@{ 
		/** Row iterator for the CFloatImage class. */
		typedef CImageRowIterator<FLOAT32> RowIterator;
		/** Constant row iterator for the CFloatImage class. */
		typedef CImageConstRowIterator<FLOAT32> ConstRowIterator;
		/** Column iterator for the CFloatImage class. */
		typedef CImageColumnIterator<FLOAT32> ColumnIterator;
		/** Constant Column iterator for the CFloatImage class. */
		typedef CImageConstColumnIterator<FLOAT32> ConstColumnIterator;
		/** Returns a row iterator to row number i.
			@param i Row number (zero indexed).
			@return Row iterator for this image.
			@see CFloatImageRowIterator */
        inline RowIterator Row(UINT32 i);
		/** Returns a const row iterator to row number i.
			@param i Row number (zero indexed).
			@return Const row iterator.
			@see CFloatImageConstRowIterator */
        inline ConstRowIterator ConstRow(UINT32 i) const;
		/** Returns a column iterator to column number i.
			@param i Column number (zero indexed).
			@return Column iterator for this image
			@see CFloatImageColumnIterator */
        inline ColumnIterator Column(UINT32 i);
		/** Returns a const column iterator to column number i.
			@param i Column number (zero indexed).
			@return Const column iterator for this image.
			@see CFloatImageConstColumnIterator */
        inline ConstColumnIterator ConstColumn(UINT32 i) const;
		//@}
		/** returns the pixel value at position (x,y),
			including range check, slow access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return pixel value */
		inline FLOAT32 GetPixel(int x, int y) const;
		/** returns the pixel value at position Pos,
			including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@return pixel value */
		inline FLOAT32 GetPixel(const CPoint2D<int>& Pos) const;
		/** returns the pixel value at position (x,y),
			without range check, fast access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return pixel value */
		inline FLOAT32 GetPixelFast(int x, int y) const;
		/** returns the pixel value at position Pos,
			without range check, fast access.
			@param Pos 2-Dimensional position in image.
			@return pixel value */
		inline FLOAT32 GetPixelFast(const CPoint2D<int>& Pos) const;
		/** returns the interpolated value at position (x,y), including
			range check. Uses bilinar interpolation.
			@param x The position in horisontal direction
			@param y The position in vertical direction
			@return interpolated pixel value
			@version 0.90 */
		inline FLOAT32 GetPixelInterpolated(float x,float y) const;
		/** returns the interpolated value at position (x,y), including
			range check. Uses bilinar interpolation.
			@param Pos 2-Dimensional position in image.
			@return interpolated pixel value
			@version 0.90 */
		inline FLOAT32 GetPixelInterpolated(const CPoint2D<float>& Pos) const;
		/** A faster version of GetPixelInterpolated(), no range checking
			and origo must be (0,0).
			@param x The position in horisontal direction
			@param y The position in vertical direction
			@return interpolated pixel value
			@version 0.90 
			@see GetPixelInterpolated */
		inline FLOAT32 GetPixelInterpolatedFast(float x,float y) const;
		/** A faster version of GetPixelInterpolated(), no range checking
			and origo must be (0,0).
			@param Pos 2-Dimensional position in image.
			@return interpolated pixel value
			@version 0.90 
			@see GetPixelInterpolated */
		inline FLOAT32 GetPixelInterpolatedFast(const CPoint2D<float>& Pos) const;
		/** Sets a pixel at position (x,y),
			including range check, slow access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@param Color The new pixel value.
			@return false if outside image */
		inline bool SetPixel(int x, int y, FLOAT32 Color);
		/** Sets a pixel at position Pos,
			including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@param Color The new pixel value.
			@return false if outside image */
		inline bool SetPixel(const CPoint2D<int>& Pos, FLOAT32 Color);
		/** Sets a pixel at position (x,y),
			without range check, fast access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@param Color The new pixel value. */
		inline void SetPixelFast(int x, int y, FLOAT32 Color);
		/** Sets a pixel at position (x,y),
			without range check, fast access.
			@param Pos 2-Dimensional position in image.
			@param Color The new pixel value. */
		inline void SetPixelFast(const CPoint2D<int>& Pos, FLOAT32 Color);
		/** @return width of image. */
		inline UINT32 GetWidth() const;
		/** @return height of image. */
		inline UINT32 GetHeight() const;
		/** @return size of border */
		inline UINT16 GetBorderSize() const;
		/** Sets the size and color of image border
			@param Size The size of border in pixels
			@param Color The color of the new border
			@return false if failed */
		bool SetBorder(UINT16 Size, FLOAT32 Color);
		/** Sets the size of image border
			@param Size Size of border in pixel to be added in each direction.
			@return false if failed */
		bool SetBorderSize(UINT16 Size);
		/** Sets the color of image border
			@param Color The new color of border
			@return false if failed */
		bool SetBorderColor(FLOAT32 Color);
		/** Copies sourceImage of type CFloatImage to this image.
			The values xmin, ymin, xmax and ymax) specifies the rectangle to
			be copied, uses integers because of origo offset. ROI is inactivated.
			If you want the resulting image placed in this image just use the same
			object for SourceImage. Note: In that case the method is a bit slower 
			(needs to copy a temporary image to the destination).
			Adds new line to history.
			@param SourceImage Image to copy from.
			@param xmin defines left side of rectangle in source image to copy from (including this value).
			@param ymin defines top side of rectangle in source image to copy from (including this value).
			@param xmax defines right side of rectangle in source image to copy from (excluding this value).
			@param ymax defines buttom side of rectangle in source image to copy from (excluding this value).
			@return false if outside image */
		bool CopySubImage(CFloatImage& SourceImage,int xmin,int ymin,
				  int xmax,int ymax);
		/** Same as CopySubImage(CFloatImage& SourceImage,int xmin,int ymin,
				  int xmax,int ymax) except the points is given in CPoint2D<int> types.
			@param SourceImage Image to copy from.
			@param UpperLeft position in the upper left corner to start from (including this value).
			@param LowerRight position in the lower right corner to stop at (excluding this value).
			@return false if outside image */
		bool CopySubImage(CFloatImage& SourceImage,const CPoint2D<int>& UpperLeft,
				  const CPoint2D<int>& LowerRight);
		/** @return byte width of a scanline */
		UINT16 GetLineByteWidth() const;
		/** Returns a pointer to the beginning of image data.
			@return UINT8* to image data */
		FLOAT32* GetImageDataPtr();
		/** To use the kernel C functions it is sometimes necessary
			to pass the TFloatImage structure.
			In other cases do not use this function to manipulate the 
			image but use the methods in this class!
			@return pointer to a TFloatImage structure. */
		const TFloatImage* GetConstTFloatImagePtr() const;
		/** To use the kernel C functions it is sometimes necessary
			to pass the TFloatImage structure which contents will be changed.
			In other cases do not use this function to manipulate the 
			image but use the methods in this class!
			@return pointer to a TFloatImage structure. */
		TFloatImage* GetTFloatImagePtr();
		/** Iterate from this value. Takes ROI into account.
			@return min. X relative to origo
			@see SetROI() */
		inline int GetMinX() const;
		/** Iterate from this value. Takes ROI into account.
			@return min. Y relative to origo
			@see SetROI() */
		inline int GetMinY() const;
		/** Iterate to this value minus one (the image is zero indexed).
			Takes ROI into account.
			@return max. X relative to origo
			@see SetROI() */
		inline int GetMaxX() const;
		/** Iterate to this value minus one (the image is zero indexed).
			Takes ROI into account.
			@return max. Y relative to origo
			@see SetROI() */
		inline int GetMaxY() const;
		/** Returns origo for image 
			@return A 2D point - position may be negative or greater than image size */
		inline CPoint2D<int> GetOrigo() const;
		/** @return the origin of image */
		inline ORIGIN GetOrigin() const;
		/** Rotates image clockwise in step of 90 degrees.
			Note: Not speed optimised! Adds new line to history.
			@param Steps Rotate image 90 degrees * Steps */
		void Rotate90(int Steps);
		/** Flips image around a centered horisontal line.
			Note: Not speed optimised! Adds new line to history. */
		void FlipHorisontal();
		/** Flips image around a centered vertical line.
			Note: Not speed optimised! Adds new line to history. */
		void FlipVertical();
		/** Sets origo for image. If ROI is active, its values are updated according
			to this new origo value.
			Values greater than the dimensions of the image is possible. 
			@param x The position in horisontal direction
			@param y The position in vertical direction */
		void inline SetOrigo(UINT16 x, UINT16 y);
		/** Sets origo for image. If ROI is active, its values are updated according
			to this new origo value.
			Values greater than the dimensions of the image is possible. 
			@param Pos 2-Dimensional position in image. */
		void inline SetOrigo(const CPoint2D<int>& Pos);
		/** Sets Region Of Interest (ROI) for this object. If outside image dimensions
			false is returned and an error is produced. In that case the state of ROI
			is left unchanged. Image dimensions are the values returned from the methods
			GetMinX(), GetMaxX(), GetMinY() and GetMaxY(), i.e. it includes origo offset.
			@param UL Upper left value of ROI, relative to origo.
			@param LR Lower right value of ROI, relative to origo.
			@return False, if values are outside image dimensions, relative to origo. */
		inline bool SetROI(const CPoint2D<INT32>& UL, const CPoint2D<INT32>& LR);
		/// Prints every important data in image to stdout
		void PrintInfo() const;
		/** @name streaming operators */
		//@{
		/** Writes all the image information to stream.
			Calls StreamIndent() for each streamed line. */
		friend ostream& operator<<(ostream& s,const CFloatImage& Img);
		//@}
		/** adds Bias to all pixels. The border (if present) is not
			affected. Adds to the history.
			@param Bias is the bias value
			@return False if this image is empty */
		bool ApplyBias(float Bias);
		/** scales all pixel values using a constant multiplier. The border (if present) is not
			affected. Adds to the history.
			@param Prefactor is the scaling factor
			@return False if this image is empty */
		bool ScaleIntensity(float Prefactor);
		/** Calculates the sum of two CFloatImages. The border (if present) is not
			affected. Adds to the history.
			@param Addend is the image of class CFloatImage to be added
			@return False if one or both of the images involved are empty */
		bool Add(CFloatImage &Addend);
		/** Calculates the difference of two CFloatImages. The border (if present) is not
			affected. Adds to the history.
			@param Subtrahend is the image of class CFloatImage to be subtracted
			@return False if one or both of the images involved are empty */
		bool Subtract(CFloatImage &Subtrahend);
		/** Calculates product of two images. The border (if present) is not
			affected. Adds to the history.
			@param Factor is the image of class CFloatImage to be multiplied
			@return False if one or both of the images involved are empty */
		bool Multiply(CFloatImage &Factor);
		/** Performs a displacement so that (dx,dy)
			in the old picture becomes (0,0). Periodic continuation is assumed.
			Adds to the history.
			@param dx is the horizontal displacement
			@param dy is the vertical displacement
			@return False if this the image is empty */
		bool CopyDisplace(int dx,int dy);
		/** Copies a source image into 'this' object without
			scaling. 'This' object must be allocated. If the width/height
			of the source is larger than that of 'this' object, zero padding
			is applied. Simple cuts are applied for reduced width/height.
			The border (if present) is not affected. Adds to the history.
			@param Source is the spource image
			@return False if this image and/or the source image are empty */
		bool CutOrZeroPad(CFloatImage &Source);
		/** Copies an image of class CImage or CByteImage into this object.
			ROI is inactivated.	History from Source is copied to this image, 
			old history in this image is deleted.
			@param Source is the source image.
			@return False if the source image is empty. */
		bool Copy(CStdImage &Source);
		/** Copies an image of class CIntImage into this object. ROI is inactivated.
			History from Source is copied to this image, old history in this image is deleted.
			@param Source is the source image.
			@return False if the source image is empty. */
		bool Copy(CIntImage &Source);
		/** Copies partially an image of class CComplex into this object. ROI is inactivated.
			History from Source is copied to this image, old history in this image is deleted.
			@param mode Determines what part of the complex image is to be
				copied. The values are COMPLEX_REAL, COMPLEX_IMAGINARY, or COMPLEX_MODULUS.
			@param Source is the source image.
			@return False if the source image is empty. */
		bool Copy(COMPLEXCHOISE mode, CComplexImage &Source);
		/** Draws a line in image using the graphics routine CGraphics::Line().
			For drawing ellipses and other symbols see graphic functions.
			@param Start Beginning of line, positions in the pixelgroup will start from this point.
			@param End End of line, positions in the pixelgroup will end with this point.
			@param Value New value in the generated points from line.
			@return True, always.
			@see CGraphics */
		bool DrawLine(const CPoint2D<int>& Start, const CPoint2D<int>& End, FLOAT32 Value);
		/** Draws a circle in image using the graphics routine CGraphics::Circle().
			For drawing ellipses and other symbols see graphic functions.
			@param Center Position for center of circle.
			@param Radius Radius of circle.
			@param Value New value in the generated points from line.
			@return True, always.
			@see CGraphics */
		bool DrawCircle(const CPoint2D<int>& Center, unsigned int Radius, FLOAT32 Value);
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

inline CFloatImage::RowIterator CFloatImage::Row(UINT32 i)
{
	return RowIterator(m_Image.ppAllScanLines[i+m_Image.Origo.y],GetMinX()+m_Image.Origo.x, GetMaxX()+m_Image.Origo.x);
}

inline CFloatImage::ConstRowIterator CFloatImage::ConstRow(UINT32 i) const
{
	return ConstRowIterator(m_Image.ppAllScanLines[i+m_Image.Origo.y],GetMinX()+m_Image.Origo.x, GetMaxX()+m_Image.Origo.x);
}

inline CFloatImage::ColumnIterator CFloatImage::Column(UINT32 i)
{
	return ColumnIterator(i+m_Image.Origo.x,m_Image.ppAllScanLines[GetMinY()+m_Image.Origo.y],
						m_Image.ppAllScanLines[GetMaxY()+m_Image.Origo.y-1]-m_Image.ByteWidth/sizeof(FLOAT32),
						m_Image.ByteWidth/sizeof(FLOAT32));
}

inline CFloatImage::ConstColumnIterator CFloatImage::ConstColumn(UINT32 i) const
{
	return ConstColumnIterator(i+m_Image.Origo.x,m_Image.ppAllScanLines[GetMinY()+m_Image.Origo.y],
							m_Image.ppAllScanLines[GetMaxY()+m_Image.Origo.y-1]-m_Image.ByteWidth/sizeof(FLOAT32),
							m_Image.ByteWidth/sizeof(FLOAT32));
}

inline bool CFloatImage::IsEmpty() const
{
	return (m_Image.pPixelStream==NULL);
}

inline FLOAT32 CFloatImage::GetPixel(int x, int y) const
{
	UINT16 border=m_Image.BorderSize;
	/* adjust origo offset */
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;

	if ((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		return m_Image.ppMatrix[y][x];
	}
	else
	{
		ostringstream ost;
		ost << "CFloatImage::GetPixel(" << x << "," << y << ") Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline FLOAT32 CFloatImage::GetPixel(const CPoint2D<int>& Pos) const
{
	UINT16 border=m_Image.BorderSize;
	/* adjust origo offset */
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;

	if ((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		return m_Image.ppMatrix[y][x];
	}
	else
	{
		ostringstream ost;
		ost << "CFloatImage::GetPixel" << Pos << " Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline FLOAT32 CFloatImage::GetPixelFast(int x, int y) const
{
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;

	return m_Image.ppMatrix[y][x];
}

inline FLOAT32 CFloatImage::GetPixelFast(const CPoint2D<int>& Pos) const
{
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;

	return m_Image.ppMatrix[y][x];
}

inline FLOAT32 CFloatImage::GetPixelInterpolated(float x,float y) const
{
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;

	if((x>-m_Image.BorderSize) && (x<(int) m_Image.Width+m_Image.BorderSize-1) &&
		(y>-m_Image.BorderSize) && (y<(int) m_Image.Height+m_Image.BorderSize-1))
	{
		// upper left pixel (l ~ low)
		int xl = (x >=0 ? (int)x : (int)(x-1)); /* always round down */
		int yl = (y >=0 ? (int)y : (int)(y-1)); /* always round down */
		// lower right pixel (h ~ high)
		int xh = xl + 1;
		int yh = yl + 1;
		// do bilinear interpolation
		return ((GetPixelFast(xl,yl)*((FLOAT32)xh-x) + GetPixelFast(xh,yl)*(x-(FLOAT32)xl))*((FLOAT32)yh-y) +
		 (GetPixelFast(xl,yh)*((FLOAT32)xh-x) + GetPixelFast(xh,yh)*(x-(FLOAT32)xl))*(y-(FLOAT32)yl));
	}
	else
	{
		ostringstream ost;
		ost << "CFloatImage::GetPixelInterpolated(" << x << "," << y << 
			") Needs to access pixels outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return -1;
	}
}

inline FLOAT32 CFloatImage::GetPixelInterpolated(const CPoint2D<float>& Pos) const
{
	float x=Pos.GetX()+m_Image.Origo.x;
	float y=Pos.GetY()+m_Image.Origo.y;

	if((x>-m_Image.BorderSize) && (x<(int) m_Image.Width+m_Image.BorderSize-1) &&
		(y>-m_Image.BorderSize) && (y<(int) m_Image.Height+m_Image.BorderSize-1))
	{
		// upper left pixel (l ~ low)
		int xl = (x >=0 ? (int)x : (int)(x-1)); /* always round down */
		int yl = (y >=0 ? (int)y : (int)(y-1)); /* always round down */
		// lower right pixel (h ~ high)
		int xh = xl + 1;
		int yh = yl + 1;
		// do bilinear interpolation
		return ((GetPixelFast(xl,yl)*((FLOAT32)xh-x) + GetPixelFast(xh,yl)*(x-(FLOAT32)xl))*((FLOAT32)yh-y) +
		 (GetPixelFast(xl,yh)*((FLOAT32)xh-x) + GetPixelFast(xh,yh)*(x-(FLOAT32)xl))*(y-(FLOAT32)yl));
	}
	else
	{
		ostringstream ost;
		ost << "CFloatImage::GetPixelInterpolated" << Pos << 
			" Needs to access pixels outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return -1;
	}
}

inline FLOAT32 CFloatImage::GetPixelInterpolatedFast(float x,float y) const
{
	// upper left pixel (l ~ low)
	int xl = (x >=0 ? (int)x : (int)(x-1)); /* always round down */
	int yl = (y >=0 ? (int)y : (int)(y-1)); /* always round down */
	// lower right pixel (h ~ high)
	int xh = xl + 1;
	int yh = yl + 1;
	// do bilinear interpolation
	return ((GetPixelFast(xl,yl)*((FLOAT32)xh-x) + GetPixelFast(xh,yl)*(x-(FLOAT32)xl))*((FLOAT32)yh-y) +
	 (GetPixelFast(xl,yh)*((FLOAT32)xh-x) + GetPixelFast(xh,yh)*(x-(FLOAT32)xl))*(y-(FLOAT32)yl));
}

inline FLOAT32 CFloatImage::GetPixelInterpolatedFast(const CPoint2D<float>& Pos) const
{
	float x=Pos.GetX();
	float y=Pos.GetY();
	int xl = (x >=0 ? (int)x : (int)(x-1)); /* always round down */
	int yl = (y >=0 ? (int)y : (int)(y-1)); /* always round down */
	// lower right pixel (h ~ high)
	int xh = xl + 1;
	int yh = yl + 1;
	// do bilinear interpolation
	return ((GetPixelFast(xl,yl)*((FLOAT32)xh-x) + GetPixelFast(xh,yl)*(x-(FLOAT32)xl))*((FLOAT32)yh-y) +
	 (GetPixelFast(xl,yh)*((FLOAT32)xh-x) + GetPixelFast(xh,yh)*(x-(FLOAT32)xl))*(y-(FLOAT32)yl));
}


inline bool CFloatImage::SetPixel(int x, int y, FLOAT32 Color)
{
	UINT16 border=m_Image.BorderSize;
	/* adjust origo offset */
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;

	if((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		m_Image.ppMatrix[y][x]=Color;
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CFloatImage::SetPixel(" << x << "," << y << ") Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

inline bool CFloatImage::SetPixel(const CPoint2D<int>& Pos, FLOAT32 Color)
{
	UINT16 border=m_Image.BorderSize;
	/* adjust origo offset */
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;

	if((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		m_Image.ppMatrix[y][x]=Color;
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CFloatImage::SetPixel" << Pos << " Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

inline void CFloatImage::SetPixelFast(int x, int y, FLOAT32 Color)
{
	/* adjust origo offset */
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;

	m_Image.ppMatrix[y][x]=Color;
}

inline void CFloatImage::SetPixelFast(const CPoint2D<int>& Pos, FLOAT32 Color)
{
	/* adjust origo offset */
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;

	m_Image.ppMatrix[y][x]=Color;
}

inline UINT32 CFloatImage::GetWidth() const
{
	// get width of image
	return m_Image.Width;
}

inline UINT32 CFloatImage::GetHeight() const
{
	// get height of image
	return m_Image.Height;
}

inline UINT16 CFloatImage::GetBorderSize() const
{
	// get size of border
	return m_Image.BorderSize;
}

inline int CFloatImage::GetMinX() const
{
	if (ROIActive()==true)
		return m_ROIUpperLeft.GetX();
	else
		return -m_Image.Origo.x;
}

inline int CFloatImage::GetMinY() const
{
	if (ROIActive()==true)
		return m_ROIUpperLeft.GetY();
	else
		return -m_Image.Origo.y;
}

inline int CFloatImage::GetMaxX() const
{
	if (ROIActive()==true)
		return m_ROILowerRight.GetX();
	else
		return m_Image.Width-m_Image.Origo.x;
}

inline int CFloatImage::GetMaxY() const
{
	if (ROIActive()==true)
		return m_ROILowerRight.GetY();
	else
		return m_Image.Height-m_Image.Origo.y;
}

inline CPoint2D<int> CFloatImage::GetOrigo() const
{
	return CPoint2D<int>(m_Image.Origo.x,m_Image.Origo.x);
}

inline ORIGIN CFloatImage::GetOrigin() const
{
	// get the origin of image
	return m_Image.Origin;
}

inline void CFloatImage::SetOrigo(UINT16 x, UINT16 y)
{
	SetOrigo(CPoint2D<int>(x,y));
}

inline void CFloatImage::SetOrigo(const CPoint2D<int>& Pos)
{
	m_Image.Origo.x=Pos.GetX();
	m_Image.Origo.y=Pos.GetY();
	if (m_ROIActive==true)
	{
		m_ROIUpperLeft  -= Pos;
		m_ROILowerRight -= Pos;
	}
}

inline bool CFloatImage::SetROI(const CPoint2D<INT32>& UL, const CPoint2D<INT32>& LR)
{
	if ((UL.GetX()>=LR.GetX()) || (UL.GetY()>=LR.GetY()))
	{
		ostringstream ost;
		ost << "CFloatImage::SetROI() UL " << UL << " greater than LR " << LR << 
			" in at least one of the dimensions" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	// we check with the MinX, MaxX, MinY and MaxY without ROI
	INT32 MinXNoROI=-m_Image.Origo.x;
	INT32 MinYNoROI=-m_Image.Origo.y;
	INT32 MaxXNoROI=m_Image.Width-m_Image.Origo.x;
	INT32 MaxYNoROI=m_Image.Height-m_Image.Origo.y;
	if ((UL.GetX()<MinXNoROI) || (UL.GetY()<MinYNoROI) || 
		    (LR.GetX()>MaxXNoROI) || (LR.GetY()>MaxYNoROI))
	{
		ostringstream ost;
		ost << "CFloatImage::SetROI() UL " << UL << " or LR " << LR << 
			" outside image dimensions: " << CPoint2D<int>(GetMinX(),GetMinY()) <<
			" - " << CPoint2D<int>(GetMaxX(),GetMaxY()) << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	m_ROIActive=true;
	m_ROIUpperLeft=UL;
	m_ROILowerRight=LR;
	return true;
}

/*@}*/ /* end group "containerclasses" */

} // end namespace ipl

#endif //_IPL98_FLOATIMAGE_H
