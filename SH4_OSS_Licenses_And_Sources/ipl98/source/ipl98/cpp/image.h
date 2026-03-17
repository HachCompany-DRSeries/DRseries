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

#ifndef _IPL98_IMAGE_H
#define _IPL98_IMAGE_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "image_row_it.h"
#include "image_const_row_it.h"
#include "image_column_it.h"
#include "image_const_column_it.h"
#include "image_row_it_1bp.h"
#include "image_column_it_1bp.h"
#include "image_const_row_it_1bp.h"
#include "image_const_column_it_1bp.h"
#include "image_row_it_24bp.h"
#include "image_column_it_24bp.h"
#include "image_const_row_it_24bp.h"
#include "image_const_column_it_24bp.h"
#include "std_image.h"
#include "byte_image.h"
#include "palette.h"
#include "text.h"
#include "error.h"
#include "../kernel_c/kernel_string.h"
#include "../../points/point2d.h"
#include <sstream>
#include <string>

namespace ipl{ // use namespace ipl

using std::ostringstream;
using std::string;

/** @ingroup containerclasses
@{ */

/** CImage is the most general image class in IPL98, last updated 12/129/2005.
	Fully support for 1,8 and 24 bits per pixel. Images with 4 b/p
	can be loaded and created but pixel manipulation is not yet 
	possible with this bit-depth. Different methods of setting/getting
	pixels are available. The slow methods (GetPixel and SetPixel) are 
	with range check, while the fast ones (GetPixelFast and SetPixelFast)
	are without any safety. For very fast pixel manipulation use the
	class CByteImage.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CImage ipl98/cpp/image.h
	@version 2.01,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */

class CImage : public CStdImage{
	public: // methods
		/// Default constructor, no image memory allocated
		CImage();
		/** Constructor for empty image, allocates memory. ROI is inactivated.
			All pixel values are initialised to 0.
			@param Width The width of image in pixels
			@param Height The height of image in pixels
			@param Bits Number of bits per pixel, 1, 4, 8 or 24
			@param Color Initial color value in all pixels. Defaults to 0. The
				function is faster when Color initializes to 0. */
		CImage(UINT32 Width, UINT32 Height, UINT16 Bits, UINT32 Color=0);
		/** Constructor for empty image, allocates memory. ROI is inactivated.
			All pixel values are initialised to 0.
			@param Dimensions The image dimensions in pixels.
			@param Bits Number of bits per pixel, 1, 4, 8 or 24
			@param Color Initial color value in all pixels. Defaults to 0. The
				function is faster when Color initializes to 0. */
		CImage(const CPoint2D<UINT32>& Dimensions, UINT16 Bits, UINT32 Color=0);
		/** Copy constructor. ROI is inactivated.
			@param SourceImage Image to copy into this image */
		CImage(const CImage& SourceImage);
		/** Overloading of assignment operator (width a CStdImage). All information is copied
			to the destination image including border pixels, palette, ROI, and history.
			Previous data in this object are destroyed. Origin is set to RAM. File
			information is left empty. The operator is fastest if this object has same dimensions
			as SourceImage! In that case no reallocation of destination image is needed. 
			Adds new line to history. An error is produced if both sides of the 
			assignment operator is the same object. */
		CImage& operator=(const CStdImage& SourceImage);
		/** Overloading of assignment operator (width a CImage). All information is copied
			to the destination image including border pixels, palette, ROI and history.
			Previous data in this object are destroyed. Origin is set to RAM. File
			information is left empty. The operator is fastest if this object has same dimensions
			as SourceImage! In that case no reallocation of destination image is needed. 
			Adds new line to history. An error is produced if both sides of the 
			assignment operator is the same object. */
		CImage& operator=(const CImage& SourceImage);
		/// Default destructor
		~CImage();

		/** Allocates memory for image with given size and pixel depth.
			All pixel values are initialised to 0. ROI is inactivated.
			@param Width The width of image in pixels
			@param Height The height of image in pixels
			@param Bits Number of bits per pixel, 1, 4, 8 or 24
			@param Color Initial color value in all pixels. Defaults to 0. The
				function is faster when Color initializes to 0.
			@return False, if allocation failed. */
		bool Alloc(UINT32 Width, UINT32 Height, UINT16 Bits,UINT32 Color=0);
		/** Allocates memory for image with given size and pixel depth.
			Pixel values are not initialised. ROI is inactivated.
			@param Width The width of image in pixels
			@param Height The height of image in pixels
			@param Bits Number of bits per pixel, 1, 4, 8 or 24
			@return False, if allocation failed. */
		bool AllocFast(UINT32 Width, UINT32 Height, UINT16 Bits);
		/** Load file of type given by extension from disk. ROI is inactivated.
			At the moment the following formats are supported:
			BMP: The windows BMP format in 1, 8 and 24 bits per pixel.
			PGM: Portable Gray Map, only in binary format (P5) and with 255 gray tones.
				 History present in comment lines is loaded into the TImage.History variable.
			PCD: Kodak PhotoCD format, loads in maximum possible non-huffmann encoded
				 resolution. This implies that the maximum possible resolution for this
				 loader is 768x512, even if the image is saved in higher resolutions.
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
		/** Save file of type given by extension to disk.
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
		/** @name Iterator functionality.
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
			CImage MyImg;
			// do something to fill image data into MyImg ...
			CImage::ConstRowIterator8bp it=MyImg.ConstRow8bp(RowNr);
			// this example writes all pixel values in rownr to standard out stream
			while(it!=it.End())
				cout << *it++ << " ";
			\endverbatim
		*/
		//@{ 
		/** Row iterator for the CImage class.
			Note: Image must have 1 b/p when using this iterator! */
		typedef CImageRowIterator1bp RowIterator1bp;
		/** Constant row iterator for the CImage class.
			Note: Image must have 1 b/p when using this iterator! */
		typedef CImageConstRowIterator1bp ConstRowIterator1bp;
		/** Column iterator for the CImage class.
			Note: Image must have 1 b/p when using this iterator! */
		typedef CImageColumnIterator1bp ColumnIterator1bp;
		/** Constant Column iterator for the CImage class.
			Note: Image must have 1 b/p when using this iterator! */
		typedef CImageConstColumnIterator1bp ConstColumnIterator1bp;
		/** Returns a row iterator to row number i.
			Note: Image must have 1 b/p when using this iterator!
			@param i Row number (zero indexed).
			@return Row iterator for image with 1 b/p.
			@see CImageRowIterator1bp */
        inline RowIterator1bp Row1bp(UINT32 i);
		/** Returns a const row iterator to row number i.
			@param i Row number (zero indexed).
			@return Const row iterator.
			@see CImageConstRowIterator1bp */
        inline ConstRowIterator1bp ConstRow1bp(UINT32 i) const;
		/** Returns a column iterator to column number i.
			Note: Image must have 1 b/p when using this iterator!
			@param i Column number (zero indexed).
			@return Column iterator for an image with 1 b/p.
			@see CImageColumnIterator1bp */
        inline ColumnIterator1bp Column1bp(UINT32 i);
		/** Returns a const column iterator to column number i.
			Note: Image must have 1 b/p when using this iterator!
			@param i Column number (zero indexed).
			@return Const column iterator for an image with 1 b/p.
			@see CImageConstColumnIterator1bp */
        inline const ConstColumnIterator1bp ConstColumn1bp(UINT32 i) const;
		/** Row iterator for the CImage class.
			Note: Image must have 8 b/p when using this iterator! */
		typedef CImageRowIterator<UINT8> RowIterator8bp;
		/** Constant row iterator for the CImage class.
			Note: Image must have 8 b/p when using this iterator! */
		typedef CImageConstRowIterator<UINT8> ConstRowIterator8bp;
		/** Column iterator for the CImage class.
			Note: Image must have 8 b/p when using this iterator! */
		typedef CImageColumnIterator<UINT8> ColumnIterator8bp;
		/** Constant Column iterator for the CImage class.
			Note: Image must have 8 b/p when using this iterator! */
		typedef CImageConstColumnIterator<UINT8> ConstColumnIterator8bp;
		/** Returns a row iterator to row number i.
			Note: Image must have 8 b/p when using this iterator!
			@param i Row number (zero indexed).
			@return Row iterator for image with 8 b/p.
			@see CImageRowIterator8bp */
        inline RowIterator8bp Row8bp(UINT32 i);
		/** Returns a const row iterator to row number i.
			@param i Row number (zero indexed).
			@return Const row iterator.
			@see CImageConstRowIterator8bp */
        inline ConstRowIterator8bp ConstRow8bp(UINT32 i) const;
		/** Returns a column iterator to column number i.
			Note: Image must have 8 b/p when using this iterator!
			@param i Column number (zero indexed).
			@return Column iterator for an image with 8 b/p.
			@see CImageColumnIterator8bp */
        inline ColumnIterator8bp Column8bp(UINT32 i);
		/** Returns a const column iterator to column number i.
			Note: Image must have 8 b/p when using this iterator!
			@param i Column number (zero indexed).
			@return Const column iterator for an image with 8 b/p.
			@see CImageConstColumnIterator8bp */
        inline const ConstColumnIterator8bp ConstColumn8bp(UINT32 i) const;
		/** Row iterator for the CImage class.
			Note: Image must have 24 b/p when using this iterator! */
		typedef CImageRowIterator24bp RowIterator24bp;
		/** Constant row iterator for the CImage class.
			Note: Image must have 24 b/p when using this iterator! */
		typedef CImageConstRowIterator24bp ConstRowIterator24bp;
		/** Column iterator for the CImage class.
			Note: Image must have 24 b/p when using this iterator! */
		typedef CImageColumnIterator24bp ColumnIterator24bp;
		/** Constant Column iterator for the CImage class.
			Note: Image must have 24 b/p when using this iterator! */
		typedef CImageConstColumnIterator24bp ConstColumnIterator24bp;
		/** Returns a row iterator to row number i.
			Note: Image must have 24 b/p when using this iterator!
			@param i Row number (zero indexed).
			@return Row iterator for image with 24 b/p.
			@see CImageRowIterator24bp */
        inline RowIterator24bp Row24bp(UINT32 i);
		/** Returns a const row iterator to row number i.
			@param i Row number (zero indexed).
			@return Const row iterator.
			@see CImageConstRowIterator24bp */
        inline ConstRowIterator24bp ConstRow24bp(UINT32 i) const;
		/** Returns a column iterator to column number i.
			Note: Image must have 24 b/p when using this iterator!
			@param i Column number (zero indexed).
			@return Column iterator for an image with 24 b/p.
			@see CImageColumnIterator24bp */
        inline ColumnIterator24bp Column24bp(UINT32 i);
		/** Returns a const column iterator to column number i.
			Note: Image must have 24 b/p when using this iterator!
			@param i Column number (zero indexed).
			@return Const column iterator for an image with 24 b/p.
			@see CImageConstColumnIterator24bp */
        inline const ConstColumnIterator24bp ConstColumn24bp(UINT32 i) const;
		//@}
		/** @name Getting a pixel value at position (x,y).
			If a palette is used the return value is the index in the palette. In order to get the
			RGB-value look it up in the palette (m_Pal.GetPalValue(...)). */
		//@{ 
		/** Returns a pixel value at position (x,y),
			including range check, slow access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return pixel value, for 24 b/p: R+G*256+B*256*256. */
		inline UINT32 GetPixel(int x, int y) const;
		/** Returns a pixel value at position Pos
			including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@return pixel value, for 24 b/p: R+G*256+B*256*256. */
		inline UINT32 GetPixel(const CPoint2D<int>& Pos) const;
		/** Returns a pixel value at position Pos
			including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@return pixel value, for 24 b/p: R+G*256+B*256*256. */
		inline UINT32 GetPixel(const CPoint2D<INT16>& Pos) const;
		/** Returns a pixel value at position (x,y), without range check.
			Fast access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return pixel value, for 24 b/p: R+G*256+B*256*256. */
		inline UINT32 GetPixelFast(int x, int y) const; // no range check
		/** Returns a pixel value at position Pos without range check.
			Fast access.
			@param Pos 2-Dimensional position in image.
			@return pixel value, for 24 b/p: R+G*256+B*256*256. */
		inline UINT32 GetPixelFast(const CPoint2D<int>& Pos) const;
		/** Returns a pixel value at position Pos without range check.
			Fast access.
			@param Pos 2-Dimensional position in image.
			@return pixel value, for 24 b/p: R+G*256+B*256*256. */
		inline UINT32 GetPixelFast(const CPoint2D<INT16>& Pos) const;
		//@}
		/** @name Setting a pixel at position (x,y). */
		//@{ 
		/** Sets a pixel at position (x,y),
			including range check, slow access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@param Color The new pixel value, for 24 b/p images an RGB value,
				otherwise a palette index value.
			@return false if outside image or if b/p is not supported. */
		inline bool SetPixel(int x, int y, UINT32 Color); 
		/** Sets a pixel at position Pos,
			including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@param Color The new pixel value, for 24 b/p images an RGB value,
				otherwise a palette index value.
			@return false if outside image or if b/p is not supported. */
		inline bool SetPixel(const CPoint2D<int>& Pos, UINT32 Color); 
		/** Sets a pixel at position Pos,
			including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@param Color The new pixel value, for 24 b/p images an RGB value,
				otherwise a palette index value.
			@return false if outside image or if b/p is not supported. */
		inline bool SetPixel(const CPoint2D<INT16>& Pos, UINT32 Color); 
		/** Sets a pixel at position (x,y), without range check, fast access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@param Color The new pixel value, for 24 b/p images an RGB value,
				otherwise a palette index value.
			@return false if b/p is not supported. */
		inline void SetPixelFast(int x, int y, UINT32 Color); 
		/** Sets a pixel at position Pos, without range check, fast access.
			@param Pos 2-Dimensional position in image.
			@param Color The new pixel value, for 24 b/p images an RGB value,
				otherwise a palette index value.
			@return false if b/p is not supported. */
		inline void SetPixelFast(const CPoint2D<int>& Pos, UINT32 Color); 
		/** Sets a pixel at position Pos, without range check, fast access.
			@param Pos 2-Dimensional position in image.
			@param Color The new pixel value, for 24 b/p images an RGB value,
				otherwise a palette index value.
			@return false if b/p is not supported. */
		inline void SetPixelFast(const CPoint2D<INT16>& Pos, UINT32 Color); 
		//@}
		/** Sets red component at position (x,y), leave other components unchanged.
			Including range check, slow access.
			@param x The position in horisontal direction
			@param y The position in vertical direction
			@param RedValue New value for red pixel component.
			@return False if position is outside image or image is not 24 b/p. */
		inline bool SetRedComponent(int x, int y, UINT8 RedValue);
		/** Sets red component at position (x,y), leave other components unchanged.
			Without range check, fast access.
			@param x The position in horisontal direction
			@param y The position in vertical direction
			@param RedValue New value for red pixel component.
			@return False if image is not 24 b/p. */
		inline bool SetRedComponentFast(int x, int y, UINT8 RedValue);
		/** Sets red component at position Pos, leave other components unchanged.
			Including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@param RedValue New value for red pixel component.
			@return False if position is outside image or image is not 24 b/p. */
		inline bool SetRedComponent(const CPoint2D<int>& Pos, UINT8 RedValue);
		/** Sets red component at position Pos, leave other components unchanged.
			Including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@param RedValue New value for red pixel component.
			@return False if position is outside image or image is not 24 b/p. */
		inline bool SetRedComponent(const CPoint2D<INT16>& Pos, UINT8 RedValue);
		/** Sets red component at position Pos, leave other components unchanged.
			Without range check, fast access.
			@param Pos 2-Dimensional position in image.
			@param RedValue New value for red pixel component.
			@return False image is not 24 b/p. */
		inline bool SetRedComponentFast(const CPoint2D<int>& Pos, UINT8 RedValue);
		/** Sets red component at position Pos, leave other components unchanged.
			Without range check, fast access.
			@param Pos 2-Dimensional position in image.
			@param RedValue New value for red pixel component.
			@return False image is not 24 b/p. */
		inline bool SetRedComponentFast(const CPoint2D<INT16>& Pos, UINT8 RedValue);
		/** Sets green component at position (x,y), leave other components unchanged.
			Including range check, slow access.
			@param x The position in horisontal direction
			@param y The position in vertical direction
			@param GreenValue New value for green pixel component.
			@return False if position is outside image or image is not 24 b/p. */
		inline bool SetGreenComponent(int x, int y, UINT8 GreenValue);
		/** Sets green component at position (x,y), leave other components unchanged.
			Without range check, fast access.
			@param x The position in horisontal direction
			@param y The position in vertical direction
			@param GreenValue New value for green pixel component.
			@return False if image is not 24 b/p. */
		inline bool SetGreenComponentFast(int x, int y, UINT8 GreenValue);
		/** Sets green component at position Pos, leave other components unchanged.
			Including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@param GreenValue New value for green pixel component.
			@return False if position is outside image or image is not 24 b/p. */
		inline bool SetGreenComponent(const CPoint2D<int>& Pos, UINT8 GreenValue);
		/** Sets green component at position Pos, leave other components unchanged.
			Including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@param GreenValue New value for green pixel component.
			@return False if position is outside image or image is not 24 b/p. */
		inline bool SetGreenComponent(const CPoint2D<INT16>& Pos, UINT8 GreenValue);
		/** Sets green component at position Pos, leave other components unchanged.
			Without range check, fast access.
			@param Pos 2-Dimensional position in image.
			@param GreenValue New value for green pixel component.
			@return False image is not 24 b/p. */
		inline bool SetGreenComponentFast(const CPoint2D<int>& Pos, UINT8 GreenValue);
		/** Sets green component at position Pos, leave other components unchanged.
			Without range check, fast access.
			@param Pos 2-Dimensional position in image.
			@param GreenValue New value for green pixel component.
			@return False image is not 24 b/p. */
		inline bool SetGreenComponentFast(const CPoint2D<INT16>& Pos, UINT8 GreenValue);
		/** Sets blue component at position (x,y), leave other components unchanged.
			Including range check, slow access.
			@param x The position in horisontal direction
			@param y The position in vertical direction
			@param BlueValue New value for blue pixel component.
			@return False if position is outside image or image is not 24 b/p. */
		inline bool SetBlueComponent(int x, int y, UINT8 BlueValue);
		/** Sets blue component at position (x,y), leave other components unchanged.
			Without range check, fast access.
			@param x The position in horisontal direction
			@param y The position in vertical direction
			@param BlueValue New value for blue pixel component.
			@return False if image is not 24 b/p. */
		inline bool SetBlueComponentFast(int x, int y, UINT8 BlueValue);
		/** Sets blue component at position Pos, leave other components unchanged.
			Including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@param BlueValue New value for blue pixel component.
			@return False if position is outside image or image is not 24 b/p. */
		inline bool SetBlueComponent(const CPoint2D<int>& Pos, UINT8 BlueValue);
		/** Sets blue component at position Pos, leave other components unchanged.
			Including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@param BlueValue New value for blue pixel component.
			@return False if position is outside image or image is not 24 b/p. */
		inline bool SetBlueComponent(const CPoint2D<INT16>& Pos, UINT8 BlueValue);
		/** Sets blue component at position Pos, leave other components unchanged.
			Without range check, fast access.
			@param Pos 2-Dimensional position in image.
			@param BlueValue New value for blue pixel component.
			@return False image is not 24 b/p. */
		inline bool SetBlueComponentFast(const CPoint2D<int>& Pos, UINT8 BlueValue);
		/** Sets blue component at position Pos, leave other components unchanged.
			Without range check, fast access.
			@param Pos 2-Dimensional position in image.
			@param BlueValue New value for blue pixel component.
			@return False image is not 24 b/p. */
		inline bool SetBlueComponentFast(const CPoint2D<INT16>& Pos, UINT8 BlueValue);
		/** Sets origo for image. If ROI is active, its values are updated according
			to this new origo value.
			Values greater than the dimensions of the image is possible. 
			@param x The position in horisontal direction
			@param y The position in vertical direction */
		inline void SetOrigo(UINT16 x, UINT16 y);
		/** Sets origo for image. If ROI is active, its values are updated according
			to this new origo value.
			Values greater than the dimensions of the image is possible. 
			@param Pos 2-Dimensional position in image. */
		inline void SetOrigo(const CPoint2D<int>& Pos);
		/** Sets Region Of Interest (ROI) for this object. If outside image dimensions
			false is returned and an error is produced. In that case the state of ROI
			is left unchanged. Image dimensions are the values returned from the methods
			GetMinX(), GetMaxX(), GetMinY() and GetMaxY(), i.e. it includes origo offset.
			@param UL Upper left value of ROI, relative to origo.
			@param LR Lower right value of ROI, relative to origo.
			@return False, if values are outside image dimensions, relative to origo. */
		inline bool SetROI(const CPoint2D<INT32>& UL, const CPoint2D<INT32>& LR);
		/** Flips image around a centered horisontal line. */
		void FlipHorisontal();
		/** Splits a 24 b/p image into three components of type CByteImage.
			@param R Red component from this image.
			@param G Green component from this image.
			@param B Blue component from this image.
			@return false if this image is not 24 b/p. */
		bool Split(CByteImage& R, CByteImage& G, CByteImage& B);
		/** Merges three components of type CByteImage into one 24 b/p image.
			ROI is inactivated.
			@param R Placed in the red component of this image
			@param G Placed in the green component of this image
			@param B Placed in the blue component of this image
			@return false if R,G and B are not all same size */
		bool Merge(const CByteImage& R, const CByteImage& G, const CByteImage& B);
		/** Copies a whole image to "this" object and convert the pixel depth, if the new
			bits per pixel is the same as source image b/p the "="-operator
			is used. Convertion from 1 to 8 b/p assumes black as 0-values
			and 1 as white values in the source image. Convertion from 1 to 24 b/p and
			8 to 24 b/p looks up the palette value in the source image and sets it in
			the destination - hence it works for both gray tones and color images.
			For 8 to 1 b/p a threshold of 128 is used (should be used for gray tone
			images only). For 24 to 8 b/p the mean value of the R, G and B component is
			used, i.e. the new image is a gray tone. For 24 to 1 b/p the method for 24
			to 8 b/p is used followed by a thresholding with 128. If you want the
			resulting image placed in the source image just use the same object as
			parameter SourceImage. Note: In this case the function is a bit slower 
			(needs to copy a temporary image to the destination).
			To optimize speed let SourceImage be different from image, the method
			is even faster if SourceImage has same dimensions as this image! In that case 
			no reallocation of destination image is needed. If you need to convert	from
			8 b/p to 1 b/p with a given threshold see the method CopyConvertThreshold.
			ROI is inactivated.
			@param Bits Bits per pixel for the new image contained in "this" object.
			@param SourceImage Image to copy and convert from. The SourceImage is left unchanged.
			@return False, if source image is empty or if Bits is not 1, 8 or 24.
			@see CopyConvertThreshold */
		bool CopyConvert(UINT16 Bits,CStdImage& SourceImage);
		/** Converts an image from 8 to 1 b/p with a given threshold.
			Same functionality as CopyConvert with a fixed conversion from 8 b/p
			to 1 b/p. The source image must have 8 b/p with a graytone palette.
			ROI is inactivated.
			@param Threshold Pixel values below this value is converted to black, the other
				are converted to white.
			@param SourceImage Image to copy and convert from. The SourceImage is left unchanged.
			@return False, if source image is empty or not 8 b/p.
			@see CopyConvert */
		bool CopyConvertThreshold(UINT8 Threshold,CStdImage& SourceImage);

		/// Prints every important data in image to stdout
		void PrintInfo() const;
		/// Prints origin to stdout
		void PrintOrigin() const;
		/**@name streaming operators */
		//@{
		/** Writes all the image information to stream.
			Calls StreamIndent() for each streamed line. */
		friend ostream& operator<<(ostream& s,const CImage& Img);
		//@}
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

inline CImage::RowIterator1bp CImage::Row1bp(UINT32 i)
{
	return RowIterator1bp(m_Image.ppAllScanLines[i+m_Image.Origo.y],GetMinX()+m_Image.Origo.x, GetMaxX()+m_Image.Origo.x);
}

inline CImage::ConstRowIterator1bp CImage::ConstRow1bp(UINT32 i) const
{
	return ConstRowIterator1bp(m_Image.ppAllScanLines[i+m_Image.Origo.y],GetMinX()+m_Image.Origo.x, GetMaxX()+m_Image.Origo.x);
}

inline CImage::ColumnIterator1bp CImage::Column1bp(UINT32 i)
{
	return ColumnIterator1bp(i+m_Image.Origo.x,GetMinY()+m_Image.Origo.y,GetMaxY()+m_Image.Origo.y,&m_Image);
}

inline const CImage::ConstColumnIterator1bp CImage::ConstColumn1bp(UINT32 i) const
{
	return ConstColumnIterator1bp(i+m_Image.Origo.x,GetMinY()+m_Image.Origo.y,GetMaxY()+m_Image.Origo.y,&m_Image);
}

inline CImage::RowIterator8bp CImage::Row8bp(UINT32 i)
{
	return RowIterator8bp(m_Image.ppAllScanLines[i+m_Image.Origo.y],GetMinX()+m_Image.Origo.x, GetMaxX()+m_Image.Origo.x);
}

inline CImage::ConstRowIterator8bp CImage::ConstRow8bp(UINT32 i) const
{
	return ConstRowIterator8bp(m_Image.ppAllScanLines[i+m_Image.Origo.y],GetMinX()+m_Image.Origo.x, GetMaxX()+m_Image.Origo.x);
}

inline CImage::ColumnIterator8bp CImage::Column8bp(UINT32 i)
{
	return ColumnIterator8bp(i+m_Image.Origo.x,m_Image.ppAllScanLines[GetMinY()+m_Image.Origo.y],
								m_Image.ppAllScanLines[GetMaxY()+m_Image.Origo.y-1]-m_Image.ByteWidth,m_Image.ByteWidth);
}

inline const CImage::ConstColumnIterator8bp CImage::ConstColumn8bp(UINT32 i) const
{
	return ConstColumnIterator8bp(i+m_Image.Origo.x,m_Image.ppAllScanLines[GetMinY()+m_Image.Origo.y],
								m_Image.ppAllScanLines[GetMaxY()+m_Image.Origo.y-1]-m_Image.ByteWidth,m_Image.ByteWidth);
}

inline CImage::RowIterator24bp CImage::Row24bp(UINT32 i)
{
	return RowIterator24bp(m_Image.ppAllScanLines[i+m_Image.Origo.y],GetMinX()+m_Image.Origo.x, GetMaxX()+m_Image.Origo.x);
}

inline CImage::ConstRowIterator24bp CImage::ConstRow24bp(UINT32 i) const
{
	return ConstRowIterator24bp(m_Image.ppAllScanLines[i+m_Image.Origo.y],GetMinX()+m_Image.Origo.x, GetMaxX()+m_Image.Origo.x);
}

inline CImage::ColumnIterator24bp CImage::Column24bp(UINT32 i)
{
	return ColumnIterator24bp(i+m_Image.Origo.x,GetMinY()+m_Image.Origo.y,GetMaxY()+m_Image.Origo.y,&m_Image);
}

inline const CImage::ConstColumnIterator24bp CImage::ConstColumn24bp(UINT32 i) const
{
	return ConstColumnIterator24bp(i+m_Image.Origo.x,GetMinY()+m_Image.Origo.y,GetMaxY()+m_Image.Origo.y,&m_Image);
}

inline UINT32 CImage::GetPixel(int x, int y) const
{
	// For 24 b/p: R+G*256+B*256*256
	// adjust origo offset
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;
	UINT16 border=m_Image.BorderSize;

	if ((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		switch(m_Image.Bits){
		case 1:
			return (((m_Image.ppMatrix[y][x>>3])&(0x80>>(x&0x7)))!=0);
		case 8:
			return (m_Image.ppMatrix[y][x])&0xff;
		case 24:
#ifdef IPL_BIG_ENDIAN
			return ((((m_Image).ppMatrix[(y)][3*(x)+2]) << 16) | (((m_Image).ppMatrix[(y)][3*(x)+1]) << 8) | ((m_Image).ppMatrix[(y)][3*(x)]));
#else
			return ((*(UINT32*)(&(m_Image.ppMatrix[y][3*x])))&0xffffff);
#endif
		default:
			ostringstream ost;
			ost << "CImage::GetPixel() Only implemented for 1,8 and 24 bit per pixels" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			return 0;
		}
	}
	else
	{
		ostringstream ost;
		ost << "CImage::GetPixel(" << x << "," << y << ") Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline UINT32 CImage::GetPixel(const CPoint2D<int>& Pos) const
{
	// For 24 b/p: R+G*256+B*256*256
	// adjust origo offset
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;
	UINT16 border=m_Image.BorderSize;

	if ((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		switch(m_Image.Bits){
		case 1:
			return (((m_Image.ppMatrix[y][x>>3])&(0x80>>(x&0x7)))!=0);
		case 8:
			return (m_Image.ppMatrix[y][x])&0xff;
		case 24:
#ifdef IPL_BIG_ENDIAN
			return ((((m_Image).ppMatrix[(y)][3*(x)+2]) << 16) | (((m_Image).ppMatrix[(y)][3*(x)+1]) << 8) | ((m_Image).ppMatrix[(y)][3*(x)]));
#else
			return ((*(UINT32*)(&(m_Image.ppMatrix[y][3*x])))&0xffffff);
#endif
		default:
			ostringstream ost;
			ost << "CImage::GetPixel() Only implemented for 1,8 and 24 bit per pixels" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			return 0;
		}
	}
	else
	{
		ostringstream ost;
		ost << "CImage::GetPixel" << Pos << " Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline UINT32 CImage::GetPixel(const CPoint2D<INT16>& Pos) const
{
	// For 24 b/p: R+G*256+B*256*256
	// adjust origo offset
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;
	UINT16 border=m_Image.BorderSize;

	if ((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		switch(m_Image.Bits){
		case 1:
			return (((m_Image.ppMatrix[y][x>>3])&(0x80>>(x&0x7)))!=0);
		case 8:
			return (m_Image.ppMatrix[y][x])&0xff;
		case 24:
#ifdef IPL_BIG_ENDIAN
			return ((((m_Image).ppMatrix[(y)][3*(x)+2]) << 16) | (((m_Image).ppMatrix[(y)][3*(x)+1]) << 8) | ((m_Image).ppMatrix[(y)][3*(x)]));
#else
			return ((*(UINT32*)(&(m_Image.ppMatrix[y][3*x])))&0xffffff);
#endif
		default:
			ostringstream ost;
			ost << "CImage::GetPixel() Only implemented for 1,8 and 24 bit per pixels" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			return 0;
		}
	}
	else
	{
		ostringstream ost;
		ost << "CImage::GetPixel" << Pos << " Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline bool CImage::SetPixel(int x, int y, UINT32 Color)
{
	// returns false if outside image or if b/p is not supported
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;
	UINT16 border=m_Image.BorderSize;

	if((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		switch(m_Image.Bits){
		case 1:
			Color ? (m_Image.ppMatrix[y][x>>3] |= (0x80>>(x & 0x7))) : 
					(m_Image.ppMatrix[y][x>>3] &= (0xff7f>>(x & 0x7)));
			return true;
		case 8:
			m_Image.ppMatrix[y][x]=(UINT8)Color;
			return true;
		case 24:
#ifdef IPL_BIG_ENDIAN
			(m_Image).ppMatrix[(y)][3*(x)]=(UINT8)Color;
			(m_Image).ppMatrix[(y)][3*(x)+1]=(UINT8)(Color>>8);
			(m_Image).ppMatrix[(y)][3*(x)+2]=(UINT8)(Color>>16);
#else
			*((UINT32*)&(m_Image.ppMatrix[y][3*x])) = ((*((UINT32*)&(m_Image.ppMatrix[y][3*x])))
				& 0xff000000) | (Color & 0x00ffffff);
#endif
			return true;
		default:
			ostringstream ost;
			ost << "CImage::SetPixel() Only implemented for 1,8 and 24 bit per pixels" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			return false;
		}
	}
	else
	{
		ostringstream ost;
		ost << "CImage::SetPixel(" << x << "," << y << ") Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

inline bool CImage::SetPixel(const CPoint2D<int>& Pos, UINT32 Color)
{
	// returns false if outside image or if b/p is not supported
	// adjust origo offset
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;
	UINT16 border=m_Image.BorderSize;

	if((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		switch(m_Image.Bits){
		case 1:
			Color ? (m_Image.ppMatrix[y][x>>3] |= (0x80>>(x & 0x7))) : 
					(m_Image.ppMatrix[y][x>>3] &= (0xff7f>>(x & 0x7)));
			return true;
		case 8:
			m_Image.ppMatrix[y][x]=(UINT8)Color;
			return true;
		case 24:
#ifdef IPL_BIG_ENDIAN
			(m_Image).ppMatrix[(y)][3*(x)]=(UINT8)Color;
			(m_Image).ppMatrix[(y)][3*(x)+1]=(UINT8)(Color>>8);
			(m_Image).ppMatrix[(y)][3*(x)+2]=(UINT8)(Color>>16);
#else
			*((UINT32*)&(m_Image.ppMatrix[y][3*x])) = ((*((UINT32*)&(m_Image.ppMatrix[y][3*x])))
				& 0xff000000) | (Color & 0x00ffffff);
#endif
			return true;
		default:
			ostringstream ost;
			ost << "CImage::SetPixel() Only implemented for 1,8 and 24 bit per pixels" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			return false;
		}
	}
	else
	{
		ostringstream ost;
		ost << "CImage::SetPixel" << Pos << " Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

inline bool CImage::SetPixel(const CPoint2D<INT16>& Pos, UINT32 Color)
{
	// returns false if outside image or if b/p is not supported
	// adjust origo offset
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;
	UINT16 border=m_Image.BorderSize;

	if((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		switch(m_Image.Bits){
		case 1:
			Color ? (m_Image.ppMatrix[y][x>>3] |= (0x80>>(x & 0x7))) : 
					(m_Image.ppMatrix[y][x>>3] &= (0xff7f>>(x & 0x7)));
			return true;
		case 8:
			m_Image.ppMatrix[y][x]=(UINT8)Color;
			return true;
		case 24:
#ifdef IPL_BIG_ENDIAN
			(m_Image).ppMatrix[(y)][3*(x)]=(UINT8)Color;
			(m_Image).ppMatrix[(y)][3*(x)+1]=(UINT8)(Color>>8);
			(m_Image).ppMatrix[(y)][3*(x)+2]=(UINT8)(Color>>16);
#else
			*((UINT32*)&(m_Image.ppMatrix[y][3*x])) = ((*((UINT32*)&(m_Image.ppMatrix[y][3*x])))
				& 0xff000000) | (Color & 0x00ffffff);
#endif
			return true;
		default:
			ostringstream ost;
			ost << "CImage::SetPixel() Only implemented for 1,8 and 24 bit per pixels" << IPLAddFileAndLine;
			CError::ShowMessage(IPL_ERROR,ost);
			return false;
		}
	}
	else
	{
		ostringstream ost;
		ost << "CImage::SetPixel" << Pos << " Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

inline UINT32 CImage::GetPixelFast(int x, int y) const
{
	// no range check
	// returns for 24 b/p: R+G*256+B*256*256
	// adjust origo offset
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;
	switch(m_Image.Bits){
	case 1:
		return (((m_Image.ppMatrix[y][x>>3])&(0x80>>(x&0x7)))!=0);
	case 8:
		return (m_Image.ppMatrix[y][x])&0xff;
	case 24:
#ifdef IPL_BIG_ENDIAN
		return ((((m_Image).ppMatrix[(y)][3*(x)+2]) << 16) | (((m_Image).ppMatrix[(y)][3*(x)+1]) << 8) | ((m_Image).ppMatrix[(y)][3*(x)]));
#else
		return ((*(UINT32*)(&(m_Image.ppMatrix[y][3*x])))&0xffffff);
#endif
	default:
		ostringstream ost;
		ost << "CImage::GetPixelFast() Only implemented for 1,8 and 24 bit per pixels" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline UINT32 CImage::GetPixelFast(const CPoint2D<int>& Pos) const
{ 
	// no range check
	// returns for 24 b/p: R+G*256+B*256*256
	// adjust origo offset
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;
	switch(m_Image.Bits) {
	case 1:
		return (((m_Image.ppMatrix[y][x>>3])&(0x80>>(x&0x7)))!=0);
	case 8:
		return (m_Image.ppMatrix[y][x])&0xff;
	case 24:
#ifdef IPL_BIG_ENDIAN
		return ((((m_Image).ppMatrix[(y)][3*(x)+2]) << 16) | (((m_Image).ppMatrix[(y)][3*(x)+1]) << 8) | ((m_Image).ppMatrix[(y)][3*(x)]));
#else
		return ((*(UINT32*)(&(m_Image.ppMatrix[y][3*x])))&0xffffff);
#endif
	default:
		ostringstream ost;
		ost << "CImage::GetPixelFast() Only implemented for 1,8 and 24 bit per pixels" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline UINT32 CImage::GetPixelFast(const CPoint2D<INT16>& Pos) const
{ 
	// no range check
	// returns for 24 b/p: R+G*256+B*256*256
	// adjust origo offset
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;
	switch(m_Image.Bits) {
	case 1:
		return (((m_Image.ppMatrix[y][x>>3])&(0x80>>(x&0x7)))!=0);
	case 8:
		return (m_Image.ppMatrix[y][x])&0xff;
	case 24:
#ifdef IPL_BIG_ENDIAN
		return ((((m_Image).ppMatrix[(y)][3*(x)+2]) << 16) | (((m_Image).ppMatrix[(y)][3*(x)+1]) << 8) | ((m_Image).ppMatrix[(y)][3*(x)]));
#else
		return ((*(UINT32*)(&(m_Image.ppMatrix[y][3*x])))&0xffffff);
#endif
	default:
		ostringstream ost;
		ost << "CImage::GetPixelFast() Only implemented for 1,8 and 24 bit per pixels" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline void CImage::SetPixelFast(int x, int y, UINT32 Color)
{
	// no range check
	// returns false if outside image
	// adjust origo offset
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;
	switch(m_Image.Bits){
	case 1:
		Color ? (m_Image.ppMatrix[y][x>>3] |= (0x80>>(x & 0x7))) : 
				(m_Image.ppMatrix[y][x>>3] &= (0xff7f>>(x & 0x7)));
		return;
	case 8:
		m_Image.ppMatrix[y][x]=(UINT8)Color;
		return;
	case 24:
#ifdef IPL_BIG_ENDIAN
		(m_Image).ppMatrix[(y)][3*(x)]=(UINT8)Color;
		(m_Image).ppMatrix[(y)][3*(x)+1]=(UINT8)(Color>>8);
		(m_Image).ppMatrix[(y)][3*(x)+2]=(UINT8)(Color>>16);
#else
		*((UINT32*)&(m_Image.ppMatrix[y][3*x])) = ((*((UINT32*)&(m_Image.ppMatrix[y][3*x])))
			& 0xff000000) | (Color & 0x00ffffff);
#endif
		return;
	default:
		ostringstream ost;
		ost << "CImage::SetPixelFast() Only implemented for 1,8 and 24 bit per pixels" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return;
	}
}

inline void CImage::SetPixelFast(const CPoint2D<int>& Pos, UINT32 Color)
{
	// no range check
	// returns false if b/p is not supported
	// adjust origo offset
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;

	switch(m_Image.Bits){
	case 1:
		Color ? (m_Image.ppMatrix[y][x>>3] |= (0x80>>(x & 0x7))) : 
				(m_Image.ppMatrix[y][x>>3] &= (0xff7f>>(x & 0x7)));
		return;
	case 8:
		m_Image.ppMatrix[y][x]=(UINT8)Color;
		return;
	case 24:
#ifdef IPL_BIG_ENDIAN
		(m_Image).ppMatrix[(y)][3*(x)]=(UINT8)Color;
		(m_Image).ppMatrix[(y)][3*(x)+1]=(UINT8)(Color>>8);
		(m_Image).ppMatrix[(y)][3*(x)+2]=(UINT8)(Color>>16);
#else
		*((UINT32*)&(m_Image.ppMatrix[y][3*x])) = ((*((UINT32*)&(m_Image.ppMatrix[y][3*x])))
			& 0xff000000) | (Color & 0x00ffffff);
#endif
		return;
	default:
		ostringstream ost;
		ost << "CImage::SetPixelFast() Only implemented for 1,8 and 24 bit per pixels" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return;
	}
}

inline void CImage::SetPixelFast(const CPoint2D<INT16>& Pos, UINT32 Color)
{
	// no range check
	// returns false if b/p is not supported
	// adjust origo offset
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;

	switch(m_Image.Bits){
	case 1:
		Color ? (m_Image.ppMatrix[y][x>>3] |= (0x80>>(x & 0x7))) : 
				(m_Image.ppMatrix[y][x>>3] &= (0xff7f>>(x & 0x7)));
		return;
	case 8:
		m_Image.ppMatrix[y][x]=(UINT8)Color;
		return;
	case 24:
#ifdef IPL_BIG_ENDIAN
		(m_Image).ppMatrix[(y)][3*(x)]=(UINT8)Color;
		(m_Image).ppMatrix[(y)][3*(x)+1]=(UINT8)(Color>>8);
		(m_Image).ppMatrix[(y)][3*(x)+2]=(UINT8)(Color>>16);
#else
		*((UINT32*)&(m_Image.ppMatrix[y][3*x])) = ((*((UINT32*)&(m_Image.ppMatrix[y][3*x])))
			& 0xff000000) | (Color & 0x00ffffff);
#endif
		return;
	default:
		ostringstream ost;
		ost << "CImage::SetPixelFast() Only implemented for 1,8 and 24 bit per pixels" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return;
	}
}

inline bool CImage::SetRedComponent(int x, int y, UINT8 RedValue)
{
	return k_SetRedComponent(x,y,RedValue,&m_Image);
}

inline bool CImage::SetRedComponentFast(int x, int y, UINT8 RedValue)
{
	return k_SetRedComponentFast(x,y,RedValue,&m_Image);
}

inline bool CImage::SetRedComponent(const CPoint2D<int>& Pos, UINT8 RedValue)
{
	return k_SetRedComponent(Pos.GetX(),Pos.GetY(),RedValue,&m_Image);
}

inline bool CImage::SetRedComponent(const CPoint2D<INT16>& Pos, UINT8 RedValue)
{
	return k_SetRedComponent(Pos.GetX(),Pos.GetY(),RedValue,&m_Image);
}

inline bool CImage::SetRedComponentFast(const CPoint2D<int>& Pos, UINT8 RedValue)
{
	return k_SetRedComponentFast(Pos.GetX(),Pos.GetY(),RedValue,&m_Image);
}

inline bool CImage::SetRedComponentFast(const CPoint2D<INT16>& Pos, UINT8 RedValue)
{
	return k_SetRedComponentFast(Pos.GetX(),Pos.GetY(),RedValue,&m_Image);
}

inline bool CImage::SetGreenComponent(int x, int y, UINT8 GreenValue)
{
	return k_SetGreenComponent(x,y,GreenValue,&m_Image);
}

inline bool CImage::SetGreenComponentFast(int x, int y, UINT8 GreenValue)
{
	return k_SetGreenComponentFast(x,y,GreenValue,&m_Image);
}

inline bool CImage::SetGreenComponent(const CPoint2D<int>& Pos, UINT8 GreenValue)
{
	return k_SetGreenComponent(Pos.GetX(),Pos.GetY(),GreenValue,&m_Image);
}

inline bool CImage::SetGreenComponent(const CPoint2D<INT16>& Pos, UINT8 GreenValue)
{
	return k_SetGreenComponent(Pos.GetX(),Pos.GetY(),GreenValue,&m_Image);
}

inline bool CImage::SetGreenComponentFast(const CPoint2D<int>& Pos, UINT8 GreenValue)
{
	return k_SetGreenComponentFast(Pos.GetX(),Pos.GetY(),GreenValue,&m_Image);
}

inline bool CImage::SetGreenComponentFast(const CPoint2D<INT16>& Pos, UINT8 GreenValue)
{
	return k_SetGreenComponentFast(Pos.GetX(),Pos.GetY(),GreenValue,&m_Image);
}

inline bool CImage::SetBlueComponent(int x, int y, UINT8 BlueValue)
{
	return k_SetBlueComponent(x,y,BlueValue,&m_Image);
}

inline bool CImage::SetBlueComponentFast(int x, int y, UINT8 BlueValue)
{
	return k_SetBlueComponentFast(x,y,BlueValue,&m_Image);
}

inline bool CImage::SetBlueComponent(const CPoint2D<int>& Pos, UINT8 BlueValue)
{
	return k_SetBlueComponent(Pos.GetX(),Pos.GetY(),BlueValue,&m_Image);
}

inline bool CImage::SetBlueComponent(const CPoint2D<INT16>& Pos, UINT8 BlueValue)
{
	return k_SetBlueComponent(Pos.GetX(),Pos.GetY(),BlueValue,&m_Image);
}

inline bool CImage::SetBlueComponentFast(const CPoint2D<int>& Pos, UINT8 BlueValue)
{
	return k_SetBlueComponentFast(Pos.GetX(),Pos.GetY(),BlueValue,&m_Image);
}

inline bool CImage::SetBlueComponentFast(const CPoint2D<INT16>& Pos, UINT8 BlueValue)
{
	return k_SetBlueComponentFast(Pos.GetX(),Pos.GetY(),BlueValue,&m_Image);
}

inline void CImage::SetOrigo(UINT16 x, UINT16 y)
{
	SetOrigo(CPoint2D<int>(x,y));
}

inline void CImage::SetOrigo(const CPoint2D<int>& Pos)
{
	m_Image.Origo.x=Pos.GetX();
	m_Image.Origo.y=Pos.GetY();
	if (m_ROIActive==true)
	{
		m_ROIUpperLeft  -= Pos;
		m_ROILowerRight -= Pos;
	}
}

inline bool CImage::SetROI(const CPoint2D<INT32>& UL, const CPoint2D<INT32>& LR)
{
	if ((UL.GetX()>=LR.GetX()) || (UL.GetY()>=LR.GetY()))
	{
		ostringstream ost;
		ost << "CImage::SetROI() UL " << UL << " greater than LR " << LR << 
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
		ost << "CImage::SetROI() UL " << UL << " or LR " << LR << 
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

#endif //_IPL98_IMAGE_H
