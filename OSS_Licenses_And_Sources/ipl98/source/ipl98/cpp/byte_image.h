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

#ifndef _IPL98_BYTEIMAGE_H
#define _IPL98_BYTEIMAGE_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "image_row_it.h"
#include "image_column_it.h"
#include "image_const_row_it.h"
#include "image_const_column_it.h"
#include "std_image.h"
#include "palette.h"
#include "text.h"
#include "../kernel_c/kernel_string.h"
#include "../kernel_c/kernel_error.h"
#include <string>

namespace ipl{ // use namespace if C++

using std::string;

/** @ingroup containerclasses
@{ */

/** CByteImage for images with 8 bits per pixel,
	last updated 12/29/2005.
	Very fast pixel access. Origo always (0,0).

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CByteImage ipl98/cpp/byte_image.h
	@version 1.46,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
class CByteImage : public CStdImage{
	public: // attributes
		/** Pointer to pixels in image, indexing is by m_ppPixel[y][x]. This
			member can be used to both setting and getting pixels, but of course
			- no range check! */
		UINT8** &m_ppPixel;
	public: // methods
		/// Default constructor, no image memory allocated
		CByteImage();
		/** Constructor for empty image, allocates memory. ROI is inactivated.
			All pixel values are initialised to 0.
			@param Width The width of image in pixels
			@param Height The height of image in pixels
			@param Color Initial color value in all pixels. Defaults to 0. The
				function is faster when Color initializes to 0. */
		CByteImage(UINT32 Width,UINT32 Height, UINT32 Color=0);
		/** Copy constructor. ROI is inactivated.
			@param SourceImage Image to copy into this image */
		CByteImage(const CByteImage& SourceImage);
		/// Default destructor
		~CByteImage();
		/** Overloading of assignment operator (width a CByteImage). Accepts 8 b/p
			source images only. All information is copied
			to the destination image including border pixels, palette, ROI and history.
			Previous data in this object are destroyed. Origin is set to RAM. File
			information is left empty.  Origo is set to (0,0) indepently of source
			origo. The operator is fastest if this object has same dimensions
			as SourceImage! In that case no reallocation of destination image is needed. 
			Adds new line to history.
			An error is produced if both sides of the assignment operator is 
			the same object. */
		CByteImage& operator=(const CStdImage& SourceImage);
		/** Overloading of assignment operator (width a CStdImage). Accepts 8 b/p source 
			images only. All information is copied
			to the destination image including border pixels, palette, ROI and history.
			Previous data in this object are destroyed. Origin is set to RAM. File
			information is left empty. The operator is fastest if this object has same dimensions
			as SourceImage! In that case no reallocation of destination image is needed. 
			An error is produced if both sides of the 
			assignment operator is the same object. */
		CByteImage& operator=(const CByteImage& SourceImage);
		/** Allocates memory for image with given size.
			All pixel values are initialised to 0. ROI is inactivated.
			@param Width The width of image in pixels
			@param Height The height of image in pixels
			@param Bits Number of bits per pixel, must be 8.
			@param Color Initial color value in all pixels. Defaults to 0. The
				function is faster when Color initializes to 0.
			@return False, if allocation failed or if "Bits" is not 8 b/p.
			@see AllocFast */
		bool Alloc(UINT32 Width, UINT32 Height, UINT16 Bits, UINT32 Color=0);
		/** Allocates memory for image with given size.
			All pixel values are initialised to 0. ROI is inactivated.
			@param Width The width of image in pixels
			@param Height The height of image in pixels
			@return False, if allocation failed.
			@see AllocFast */
		bool Alloc(UINT32 Width, UINT32 Height);
		/** Allocates memory for image with given size.
			Pixel values are not initialised. ROI is inactivated.
			@param Width The width of image in pixels
			@param Height The height of image in pixels
			@param Bits Number of bits per pixel, must be 8.
			@return False, if allocation failed or if "Bits" is not 8 b/p.
			@see Alloc */
		bool AllocFast(UINT32 Width, UINT32 Height, UINT16 Bits);
		/** Allocates memory for image with given size.
			Pixel values are not initialised. ROI is inactivated.
			@param Width The width of image in pixels
			@param Height The height of image in pixels
			@return False, if allocation failed.
			@see Alloc */
		bool AllocFast(UINT32 Width, UINT32 Height);
		/** Copies a CStdImage to this image.
			The positions (xmin,ymin) and  (xmax,ymax) specifies the rectangle to
			be copied including the specified values. The method uses integers because
			of origo offset which may force the user to use values less than zero.
			If you want the resulting image placed in this image just use the same
			object for SourceImage. Note: In that case the method is a bit slower 
			(needs to copy a temporary image to the destination).
			Adds new line to history.
			@param SourceImage Image to copy from.
			@param xmin defines left side of rectangle in source image to copy from
				(including this value).
			@param ymin defines top side of rectangle in source image to copy from
				(including this value).
			@param xmax defines right side of rectangle in source image to copy from
				(excluding this value).
			@param ymax defines buttom side of rectangle in source image to copy from
				(excluding this value).
			@return false if outside image, or the source image does not have 8 b/p */
		bool CopySubImage(CStdImage& SourceImage,int xmin,int ymin,
				  int xmax,int ymax);
		/** Copies a CStdImage to this image.
			The positions (xmin,ymin) and  (xmax,ymax) specifies the rectangle to
			be copied including the specified values. The method uses integers because
			of origo offset which may force the user to use values less than zero.
			If you want the resulting image placed in this image just use the same
			object for SourceImage. Note: In that case the method is a bit slower 
			(needs to copy a temporary image to the destination).
			Adds new line to history.
			@param SourceImage Image to copy from.
			@param UpperLeft position in the upper left corner to start from (including this value).
			@param LowerRight position in the lower right corner to stop at (excluding this value).
			@return false if outside image, or the source image does not have 8 b/p */
		bool CopySubImage(CStdImage& SourceImage,const CPoint2D<int>& UpperLeft,
				  const CPoint2D<int>& LowerRight);
		/** Sets the size and color of image border
			@param Size The size of border in pixels
			@param Color The color of the new border
			@return false if failed */
		bool SetBorder(UINT16 Size, UINT32 Color);
		/** Sets the size of image border
			@param Size Size of border in pixel to be added in each direction.
			@return false if failed */
		bool SetBorderSize(UINT16 Size);
		/** Loads file of type given by extension from disk. ROI is inactivated.
			At the moment the following formats are supported:
			BMP: The windows BMP format in 1, 8 and 24 bits per pixel.
			PGM: Portable Gray Map, only in binary format (P5) and with 255 gray tones.
				 History present in comment lines is loaded into the TImage.History variable.
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
			CByteImage MyImg;
			// do something to fill image data into MyImg ...
			CByteImage::ConstRowIterator it=MyImg.ConstRow(RowNr);
			// this example writes all pixel values in rownr to standard out stream
			while(it!=it.End())
				cout << *it++ << " ";
			\endverbatim
		*/
		//@{ 
		/** Row iterator for the CImage class. */
		typedef CImageRowIterator<UINT8> RowIterator;
		/** Constant row iterator for the CImage class. */
		typedef CImageConstRowIterator<UINT8> ConstRowIterator;
		/** Column iterator for the CImage class. */
		typedef CImageColumnIterator<UINT8> ColumnIterator;
		/** Constant Column iterator for the CImage class. */
		typedef CImageConstColumnIterator<UINT8> ConstColumnIterator;
		/** Returns a row iterator to row number i.
			@param i Row number (zero indexed).
			@return Row iterator for this image.
			@see CImageRowIterator */
        inline RowIterator Row(UINT32 i);
		/** Returns a const row iterator to row number i.
			@param i Row number (zero indexed).
			@return Const row iterator.
			@see CImageConstRowIterator */
        inline ConstRowIterator ConstRow(UINT32 i) const;
		/** Returns a column iterator to column number i.
			@param i Column number (zero indexed).
			@return Column iterator for this image
			@see CImageColumnIterator */
        inline ColumnIterator Column(UINT32 i);
		/** Returns a const column iterator to column number i.
			@param i Column number (zero indexed).
			@return Const column iterator for this image.
			@see CImageConstColumnIterator */
        inline const ConstColumnIterator ConstColumn(UINT32 i) const;
		//@}
		/** @name Getting a pixel value at position (x,y).
			If a palette is used the return value is the index in the palette. In order to get the
			RGB-value look it up in the palette (m_Pal.GetPalValue(...)). */
		//@{ 
		/** Returns a pixel value at position (x,y),
			including range check, slow access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return pixel value */
		inline UINT32 GetPixel(int x, int y) const;
		/** Returns a pixel value at position Pos
			including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@return pixel value */
		inline UINT32 GetPixel(const CPoint2D<int>& Pos) const;
		/** Returns a pixel value at position Pos
			including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@return pixel value */
		inline UINT32 GetPixel(const CPoint2D<INT16>& Pos) const;
		/** Returns a pixel value at position (x,y), without range check.
			Fast access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return pixel value */
		inline UINT32 GetPixelFast(int x, int y) const;
		/** Returns a pixel value at position Pos without range check.
			Fast access.
			@param Pos 2-Dimensional position in image.
			@return pixel value */
		inline UINT32 GetPixelFast(const CPoint2D<int>& Pos) const;
		/** Returns a pixel value at position Pos without range check.
			Fast access.
			@param Pos 2-Dimensional position in image.
			@return pixel value */
		inline UINT32 GetPixelFast(const CPoint2D<INT16>& Pos) const;
		//@}
		/** @name Setting a pixel at position (x,y). */
		//@{ 
		/** Sets a pixel at position (x,y),
			including range check, slow access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@param Color The new pixel value, a palette index value.
			@return false if outside image */
		inline bool SetPixel(int x, int y, UINT32 Color);
		/** Sets a pixel at position Pos,
			including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@param Color The new pixel value, a palette index value.
			@return false if outside image */
		inline bool SetPixel(const CPoint2D<int>& Pos, UINT32 Color);
		/** Sets a pixel at position Pos,
			including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@param Color The new pixel value, a palette index value.
			@return false if outside image */
		inline bool SetPixel(const CPoint2D<INT16>& Pos, UINT32 Color);
		/** Sets a pixel at position (x,y), without range check, fast access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@param Color The new pixel value, for 24 b/p images an RGB value,
				otherwise a palette index value. */
		inline void SetPixelFast(int x, int y, UINT32 Color);
		/** Sets a pixel at position Pos, without range check, fast access.
			@param Pos 2-Dimensional position in image.
			@param Color The new pixel value, a palette index value. */
		inline void SetPixelFast(const CPoint2D<int>& Pos, UINT32 Color);
		/** Sets a pixel at position Pos, without range check, fast access.
			@param Pos 2-Dimensional position in image.
			@param Color The new pixel value, a palette index value. */
		inline void SetPixelFast(const CPoint2D<INT16>& Pos, UINT32 Color);
		//@}
		/** @return width of image */
		UINT32 GetWidth() const;		// get width of image
		/** @return height of image */
		UINT32 GetHeight() const;	// get height of image
		/** @name Get margin values relative to origo to iterate through
			image. Also takes ROI into account */
		//@{ 
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
		//@}
		/** @return bits per pixel: allways 8 for CByteImage */
		UINT16 GetBits() const; // get bits per pixel - allways 8 for CByteImage
		/** Returns origo for image 
			@return A 2D point - allways (0,0) for this class */
		CPoint2D<int> GetOrigo() const;
		/** Sets Region Of Interest (ROI) for this object. If outside image dimensions
			false is returned and an error is produced. In that case the state of ROI
			is left unchanged. Image dimensions are the values returned from the methods
			GetMinX(), GetMaxX(), GetMinY() and GetMaxY(), i.e. it includes origo offset.
			@param UL Upper left value of ROI, relative to origo.
			@param LR Lower right value of ROI, relative to origo.
			@return False, if values are outside image dimensions, relative to origo. */
		inline bool SetROI(const CPoint2D<INT32>& UL, const CPoint2D<INT32>& LR);
		/** Copies a whole image to "this" object and convert the pixel depth, if the new
			bits per pixel is the same as source image b/p the "="-operator
			is used. For a CByteImage the destination can only be 8 b/p. Convertion from
			1 to 8 b/p assumes black as 0-values and 1 as white values in the source image.
			For 24 to 8 b/p the mean value of the R, G and B component is
			used, i.e. the new image is a gray tone. If you want the
			resulting image placed in the source image just use the same object as
			parameter SourceImage. Note: In this case the function is a bit slower 
			(needs to copy a temporary image to the destination).
			To optimize speed let pSource and pDest be different variables, the function
			is even faster if pDest has same dimensions as pSource! In that case no
			reallocation of destination image is needed. 
			@param Bits Bits per pixel for the new image contained in "this" object.
			@param SourceImage Image to copy and convert from. The SourceImage is left unchanged.
			@return False, if source image is empty or if Bits is not 8. */
		bool CopyConvert(UINT16 Bits,CStdImage& SourceImage);
		/// prints every important data in image to stdout
		void PrintInfo() const;
		/// prints origin to stdout
		void PrintOrigin() const;
		/**@name streaming operators */
		//@{
		/** writes all the image information to stream */
		friend ostream& operator<<(ostream& s,const CByteImage& Img);
		//@}
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

inline CByteImage::RowIterator CByteImage::Row(UINT32 i)
{
	return RowIterator(m_Image.ppAllScanLines[i],GetMinX(), GetMaxX());
}

inline CByteImage::ConstRowIterator CByteImage::ConstRow(UINT32 i) const
{
	return ConstRowIterator(m_Image.ppAllScanLines[i],GetMinX(),GetMaxX());
}

inline CByteImage::ColumnIterator CByteImage::Column(UINT32 i)
{
	return ColumnIterator(i,m_Image.ppAllScanLines[GetMinY()],
							m_Image.ppAllScanLines[GetMaxY()-1]-m_Image.ByteWidth,m_Image.ByteWidth);
}

inline const CByteImage::ConstColumnIterator CByteImage::ConstColumn(UINT32 i) const
{
	return ConstColumnIterator(i,m_Image.ppAllScanLines[GetMinY()],
							m_Image.ppAllScanLines[GetMaxY()-1]-m_Image.ByteWidth,m_Image.ByteWidth);
}

inline UINT32 CByteImage::GetPixel(int x, int y) const
{
	// For 24 b/p: R+G*256+B*256*256
	UINT16 border=m_Image.BorderSize;

	if ((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		return m_ppPixel[y][x];
	}
	else
	{
		ostringstream ost;
		ost << "CByteImage::GetPixel("  << x << "," << y << ") Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline UINT32 CByteImage::GetPixel(const CPoint2D<int>& Pos) const
{
	// For 24 b/p: R+G*256+B*256*256
	UINT16 border=m_Image.BorderSize;
	int x=Pos.GetX();
	int y=Pos.GetY();

	if ((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		return m_ppPixel[y][x];
	}
	else
	{
		ostringstream ost;
		ost << "CByteImage::GetPixel" << Pos << " Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline UINT32 CByteImage::GetPixel(const CPoint2D<INT16>& Pos) const
{
	// For 24 b/p: R+G*256+B*256*256
	UINT16 border=m_Image.BorderSize;
	int x=Pos.GetX();
	int y=Pos.GetY();

	if ((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		return m_ppPixel[y][x];
	}
	else
	{
		ostringstream ost;
		ost << "CByteImage::GetPixel" << Pos << " Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline bool CByteImage::SetPixel(int x, int y, UINT32 Color)
{
	// returns false if outside image
	UINT16 border=m_Image.BorderSize;

	if ((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		m_ppPixel[y][x]=(UINT8) Color;
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CByteImage::SetPixel("  << x << "," << y << ") Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

inline bool CByteImage::SetPixel(const CPoint2D<int>& Pos, UINT32 Color)
{
	UINT16 border=m_Image.BorderSize;
	int x=Pos.GetX();
	int y=Pos.GetY();

	if ((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		m_ppPixel[y][x]=(UINT8) Color;
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CByteImage::SetPixel" << Pos << " Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

inline bool CByteImage::SetPixel(const CPoint2D<INT16>& Pos, UINT32 Color)
{
	UINT16 border=m_Image.BorderSize;
	int x=Pos.GetX();
	int y=Pos.GetY();

	if ((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		m_ppPixel[y][x]=(UINT8) Color;
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CByteImage::SetPixel" << Pos << " Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

inline UINT32 CByteImage::GetPixelFast(int x, int y) const
{
	// no range check
	return m_ppPixel[y][x];
}

inline UINT32 CByteImage::GetPixelFast(const CPoint2D<int>& Pos) const
{
	// no range check
	return m_ppPixel[Pos.GetY()][Pos.GetX()];
}

inline UINT32 CByteImage::GetPixelFast(const CPoint2D<INT16>& Pos) const
{
	// no range check
	return m_ppPixel[Pos.GetY()][Pos.GetX()];
}

inline void CByteImage::SetPixelFast(int x, int y, UINT32 Color)
{
	// no range check
	m_ppPixel[y][x]=(UINT8) Color;
}

inline void CByteImage::SetPixelFast(const CPoint2D<int>& Pos, UINT32 Color)
{
	// no range check
	m_ppPixel[Pos.GetY()][Pos.GetX()]=(UINT8) Color;
}

inline void CByteImage::SetPixelFast(const CPoint2D<INT16>& Pos, UINT32 Color)
{
	// no range check
	m_ppPixel[Pos.GetY()][Pos.GetX()]=(UINT8) Color;
}

inline UINT32 CByteImage::GetWidth() const
{
	// get width of image
	return m_Image.Width;
}

inline UINT32 CByteImage::GetHeight() const
{
	// get height of image
	return m_Image.Height;
}

inline int CByteImage::GetMinX() const
{
	// get min. X relative to origo
	if (ROIActive()==true)
		return m_ROIUpperLeft.GetX();
	else
		return 0;
}

inline int CByteImage::GetMinY() const
{
	// get min. Y relative to origo
	if (ROIActive()==true)
		return m_ROIUpperLeft.GetY();
	else
		return 0;
}

inline int CByteImage::GetMaxX() const
{
	// get max. X relative to origo
	if (ROIActive()==true)
		return m_ROILowerRight.GetX();
	else
		return m_Image.Width;
}

inline int CByteImage::GetMaxY() const
{
	// get max. Y relative to origo
	if (ROIActive()==true)
		return m_ROILowerRight.GetY();
	else
		return m_Image.Height;
}

inline UINT16 CByteImage::GetBits() const
{
	// get bits per pixel - allways 8 for CByteImage
	return m_Image.Bits;
}

inline CPoint2D<int> CByteImage::GetOrigo() const
{
	return CPoint2D<int>(0,0);
}

inline bool CByteImage::SetROI(const CPoint2D<INT32>& UL, const CPoint2D<INT32>& LR)
{
	if ((UL.GetX()>=LR.GetX()) || (UL.GetY()>=LR.GetY()))
	{
		ostringstream ost;
		ost << "CByteImage::SetROI() UL " << UL << " greater than LR " << LR << 
			" in at least one of the dimensions" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else if ((UL.GetX()<0) || (UL.GetY()<0) || 
		    (LR.GetX()>(INT32)m_Image.Width) || (LR.GetY()>(INT32)m_Image.Height))
	{
		ostringstream ost;
		ost << "CByteImage::SetROI() UL " << UL << " or LR " << LR <<
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

#endif //_IPL98_BYTEIMAGE_H
