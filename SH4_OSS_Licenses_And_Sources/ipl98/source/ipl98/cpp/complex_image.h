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

#ifndef _IPL98_COMPLEX_IMAGE_H
#define _IPL98_COMPLEX_IMAGE_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "image_row_it.h"
#include "image_const_row_it.h"
#include "image_column_it.h"
#include "image_const_column_it.h"
#include "generic_image.h"
#include "text.h"
#include "error.h"
#include "../kernel_c/complex_image/c_kernel_functions.h"
#include "../kernel_c/kernel_string.h"
#include "../kernel_c/kernel_error.h"
#include <sstream>
#include <string>

namespace ipl{ // use namespace in C++

using std::ostringstream;
using std::string;
// external definitions
class CStdImage;
class CIntImage;
class CFloatImage;

/** @ingroup containerclasses
@{ */

/** CComplexImage is a class derived from CGenericImage consisting
	of pixels with type FLOAT32 (both a real an imaginar part), 
	last updated 12/29/2005. Saving and loading
	images of this type has not yet been fully tested and documentation
	for the file format still need to be written. Though it should be
	possible to save and load and all bug reports are welcome!

    This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CComplexImage ipl98/cpp/complex_image.h
	@see CGenericImage
	@version 0.97,  by ivb@mip.sdu.dk and edr@mip.sdu.dk
	@author Ivar Balslev and René Dencker Eriksen */
class CComplexImage : CGenericImage{
	public: // attributes
		/// History, keeps track of operations performed on image
		CText m_History;
	protected: // attributes
		/// The structure holding all information about the image
		TComplexImage m_Image;

	public: // methods
		/// Default constructor
		CComplexImage();
		/** Constructor for empty image, allocates memory. ROI is inactivated.
			All pixel values are initialised to 0.
			@param Width The width of image in pixels.
			@param Height The height of image in pixels.
			@param ReAndIm Initial value at all positions in both real and imaginary
				part. Defaults to 0. The function is faster when ReAndIm initializes to 0.*/
		CComplexImage(UINT32 Width, UINT32 Height, FLOAT32 ReAndIm=0);
		/** Copy constructor. ROI is inactivated.
			@param SourceImage Image to copy into this image */
		CComplexImage(const CComplexImage& SourceImage);
		/** Overloading of assignment operator. All information is copied
			to the destination image including border pixels, palette, ROI and history.
			Previous data in this object are destroyed. Origin is set to RAM. File
			information is left empty. The operator is fastest if this object has same dimensions
			as SourceImage! In that case no reallocation of destination image is needed. 
			Adds new line to history. An error is produced if both sides of the 
			assignment operator is the same object. */
		CComplexImage& operator=(const CComplexImage& SourceImage);
		/// Default destructor
		~CComplexImage();
		/** Allocates memory for image with given size. All pixel values are 
			initialised to 0. Sets Origin to "RAM", origo to (0,0) and size 
			of border to 0. ROI is inactivated.
			@param Width The width of image in pixels
			@param Height The height of image in pixels
			@param ReAndIm Initial value at all positions in both real and imaginary
				part. Defaults to 0. The function is faster when ReAndIm initializes to 0.
			@return False, if allocation failed.
			@see AllocFast */
		bool Alloc(UINT32 Width, UINT32 Height, FLOAT32 ReAndIm=0);
		/** Allocates memory for image with given size.	Pixel values are not 
			initialised. Sets Origin to "RAM", origo to (0,0) and size of 
			border to 0. ROI is inactivated.
			@param Width The width of image in pixels
			@param Height The height of image in pixels
			@return False, if allocation failed.
			@see Alloc */
		bool AllocFast(UINT32 Width, UINT32 Height);
		/** Loads a file of type given by extension from disk. ROI is inactivated.
			@param pPathAndFileName Name of file including extension and relative
				or absolute path, f.ex. absolut path "c:/temp/doggie.bmp",
				relative "doggie.bmp". Both slash ("/") and back slash ("\") can be
				used in the path. There is no differense between upper
				and lower case characters, they are treated equally.
			@return False if load failed */
		bool Load(const char* pPathAndFileName);
		/** Does the same as Load(const char* pPathAndFileName) except
            the file name and path is given in a string object. */
		bool Load(const string& PathAndFileName);
		/** Saves the image on a file of type given by extension to disk.
			@param pPathAndFileName Name of file including extension and relative
			or absolute path, f.ex. absolut path "c:/temp/doggie.bmp",
			relative "doggie.bmp". Both slash ("/") and back slash ("\") can be
			used in the path. There is no differense between upper
			and lower case characters, they are treated equally.
			@return true if saved */
		bool Save(const char* pPathAndFileName);
		/** does the same as Save(const char* pPathAndFileName) except
                        the file name and path is given in a string object. */
		bool Save(const string& PathAndFileName);
		/** Deletes all allocated memory for image and resets variables. ROI is inactivated,
			history is cleared. */
		void Empty();
		/** Checks if any data is present.
			@return True, if image contains no data. */
		inline bool IsEmpty() const;
		/** Gets the file name for image, if available.
			@param Str The file name is passed in this parameter,
                        empty if not available.
			@return False, if file name not available. */
		bool GetFileName(string& Str);
		/** Does the same as GetFileName(string& Str) except a
                        character pointer to the file name is returned.
			@return NULL if file name is unavailable. */
		const char* GetFileName();
		/** Gets the path for image, if available.
			@param Str The file name is passed in this parameter,
                        empty if not available.
			@return False, if path is unavailable. */
		bool GetPathName(string& Str);
		/** Does the same as GetPathName(string& Str) except a character
            pointer to the path is returned.
			@return NULL if path is unavailable. */
		const char* GetPathName();
		/** Gets the complete path and file name for image, if
            available.
			@param Str The complete path and file name is passed
                        in this parameter,
			empty if not available.
			@return False, if path and file name is unavailable. */
		bool GetPathAndFileName(string& Str);
		/** Does the same as GetPathAndFileName(string& Str)
            except a character pointer to the path
			and file name is returned.
			@return NULL if path and file name is unavailable. */
		const char* GetPathAndFileName();
		/** Sets all pixels in the image to given color.
            Old pixel values are destroyed.
			@param Re New real part pixel values in image.
			@param Im New real part pixel values in image.
			@return False if image is empty */
		bool Flush(FLOAT32 Re, FLOAT32 Im);
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
			CComplexImage MyImg;
			// do something to fill image data into MyImg ...
			CComplexImage::ReConstRowIterator it=MyImg.ReConstRow(RowNr);
			// this example writes all pixel values in rownr to standard out stream
			while(it!=it.End())
				cout << *it++ << " ";
			\endverbatim
		*/
		//@{ 
		/** Row iterator (real part of complex value) for the CComplexImage class. */
		typedef CImageRowIterator<FLOAT32> ReRowIterator;
		/** Constant row iterator (real part of complex value) for the CComplexImage class. */
		typedef CImageConstRowIterator<FLOAT32> ReConstRowIterator;
		/** Column iterator (real part of complex value) for the CComplexImage class. */
		typedef CImageColumnIterator<FLOAT32> ReColumnIterator;
		/** Constant Column iterator (real part of complex value) for the CComplexImage class. */
		typedef CImageConstColumnIterator<FLOAT32> ReConstColumnIterator;
		/** Returns a row iterator for the real part to row number i.
			@param i Row number (zero indexed).
			@return Row iterator for this image.
			@see CComplexImageReRowIterator */
        inline ReRowIterator ReRow(UINT32 i);
		/** Returns a const row iterator for the real part to row number i.
			@param i Row number (zero indexed).
			@return Const row iterator.
			@see CComplexImageReConstRowIterator */
        inline ReConstRowIterator ReConstRow(UINT32 i) const;
		/** Returns a column iterator for the real part to column number i.
			@param i Column number (zero indexed).
			@return Column iterator for this image
			@see CComplexImageReColumnIterator */
        inline ReColumnIterator ReColumn(UINT32 i);
		/** Returns a const column iterator for the real part to column number i.
			@param i Column number (zero indexed).
			@return Const column iterator for this image.
			@see CComplexImageReConstColumnIterator */
        inline ReConstColumnIterator ReConstColumn(UINT32 i) const;
		/** Row iterator (imaginar part of complex value) for the CComplexImage class. */
		typedef CImageRowIterator<FLOAT32> ImRowIterator;
		/** Constant row iterator (imaginar part of complex value) for the CComplexImage class. */
		typedef CImageConstRowIterator<FLOAT32> ImConstRowIterator;
		/** Column iterator (imaginar part of complex value) for the CComplexImage class. */
		typedef CImageColumnIterator<FLOAT32> ImColumnIterator;
		/** Constant Column iterator (imaginar part of complex value) for the CComplexImage class. */
		typedef CImageConstColumnIterator<FLOAT32> ImConstColumnIterator;
		/** Returns a row iterator for the imaginar part to row number i.
			@param i Row number (zero indexed).
			@return Row iterator for this image.
			@see CComplexImageImRowIterator */
        inline ImRowIterator ImRow(UINT32 i);
		/** Returns a const row iterator for the imaginar part to row number i.
			@param i Row number (zero indexed).
			@return Const row iterator.
			@see CComplexImageImConstRowIterator */
        inline ImConstRowIterator ImConstRow(UINT32 i) const;
		/** Returns a column iterator for the imaginar part to column number i.
			@param i Column number (zero indexed).
			@return Column iterator for this image
			@see CComplexImageImColumnIterator */
        inline ImColumnIterator ImColumn(UINT32 i);
		/** Returns a const column iterator for the imaginar part to column number i.
			@param i Column number (zero indexed).
			@return Const column iterator for this image.
			@see CComplexImageImConstColumnIterator */
        inline ImConstColumnIterator ImConstColumn(UINT32 i) const;
		//@}
		/** Returns the real pixel value at position (x,y),
			including range check, slow access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return pixel value */
		inline FLOAT32 GetPixelRe(int x, int y) const;
		/** Returns the imaginary pixel value at position (x,y),
			including range check, slow access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return pixel value */
		inline FLOAT32 GetPixelIm(int x, int y) const;
		/** Returns the real pixel value at position Pos,
			including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@return pixel value. */
		inline FLOAT32 GetPixelRe(const CPoint2D<int>& Pos) const;
		/** Returns the imaginary pixel value at position Pos,
			including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@return pixel value. */
		inline FLOAT32 GetPixelIm(const CPoint2D<int>& Pos) const;
		/** Returns the real pixel value at position (x,y),
			without range check, fast access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return pixel value. */
		inline FLOAT32 GetPixelFastRe(int x, int y) const;
		/** Returns the imaginary pixel value at position (x,y),
			without range check, fast access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return pixel value. */
		inline FLOAT32 GetPixelFastIm(int x, int y) const;
		/** Returns the real pixel value at position Pos,
			without range check, fast access.
			@param Pos 2-Dimensional position in image.
			@return pixel value. */
		inline FLOAT32 GetPixelFastRe(const CPoint2D<int>& Pos) const;
		/** Returns the imaginary pixel value at position Pos,
			without range check, fast access.
			@param Pos 2-Dimensional position in image.
			@return pixel value. */
		inline FLOAT32 GetPixelFastIm(const CPoint2D<int>& Pos) const;
		/** Sets both the real (Re) and imaginary (Im) complex pixel value 
			at position (x,y), including range check, slow acces.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@param Re New real part value of pixel.
			@param Im New imaginar part value of pixel.
			@return false if outside image. */
		inline bool SetPixel(int x, int y, FLOAT32 Re,FLOAT32 Im);
		/** Sets both the real (Re) and imaginary (Im) complex pixel value 
			at position Pos, including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@param Re New real part value of pixel.
			@param Im New imaginar part value of pixel. */
		inline bool SetPixel(const CPoint2D<int>& Pos, FLOAT32 Re, FLOAT32 Im);
		/** Sets both the real (Re) and imaginary (Im) complex pixel value 
			at position (x,y), without range check, fast access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@param Re New real part value of pixel.
			@param Im New imaginar part value of pixel.
			@return false if outside image. */
		inline void SetPixelFast(int x, int y, FLOAT32 Re, FLOAT32 Im);
		/** Sets both the real (Re) and imaginary (Im) complex pixel value 
			at position Pos, without range check, fast access.
			@param Pos 2-Dimensional position in image.
			@param Re New real part value of pixel.
			@param Im New imaginar part value of pixel. */
		inline void SetPixelFast(const CPoint2D<int>& Pos, FLOAT32 Re, FLOAT32 Im);

		/** Sets only the real pixel value at position (x,y),
			including range check, slow access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@param Re New real part value of pixel.
			@return false if outside image. */
		inline bool SetPixelRe(int x, int y, FLOAT32 Re);
		/** Sets only the real pixel value at position Pos,
			including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@param Re New real part value of pixel.
			@return false if outside image. */
		inline bool SetPixelRe(const CPoint2D<int>& Pos, FLOAT32 Re);
		
		/** Sets only the imaginary pixel value at position (x,y),
			including range check, slow access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@param Im New imaginar part value of pixel.
			@return false if outside image. */
		inline bool SetPixelIm(int x, int y, FLOAT32 Im);
		/** Sets only the imaginary pixel value at position Pos,
			including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@param Im New imaginar part value of pixel.
			@return false if outside image. */
		inline bool SetPixelIm(const CPoint2D<int>& Pos, FLOAT32 Im);
		
		/** Sets only the real pixel value at position (x,y),
			without range check, fast access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@param Re New real part value of pixel. */
		inline void SetPixelFastRe(int x, int y, FLOAT32 Re);
		/** Sets only the real pixel value at position Pos,
			without range check, fast access.
			@param Pos 2-Dimensional position in image.
			@param Re New real part value of pixel. */
		inline void SetPixelFastRe(const CPoint2D<int>& Pos, FLOAT32 Re);
		
		/** Sets only the imaginary pixel value at position (x,y),
			without range check, fast access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@param Im New imaginar part value of pixel. */
		inline void SetPixelFastIm(int x, int y, FLOAT32 Im);
		/** Sets only the imaginary pixel value at position Pos,
			without range check, fast access.
			@param Pos 2-Dimensional position in image.
			@param Im New imaginar part value of pixel. */
		inline void SetPixelFastIm(const CPoint2D<int>& Pos, FLOAT32 Im);

		/** @return the width of image. */
		inline UINT32 GetWidth() const;
		/** @return the height of image. */
		inline UINT32 GetHeight() const;
		/** @return size of border */
		inline UINT16 GetBorderSize() const;
		/** Sets the size and color of image border
			@param Size The size of border in pixels
			@param Re The new color of border in the real part.
			@param Im The new color of border in the imaginary part.
			@return false if failed */
		bool SetBorder(UINT16 Size, FLOAT32 Re,FLOAT32 Im);
		/** Sets the size of image border
			@param Size Size of border in pixel to be added in each direction.
			@return false if failed */
		bool SetBorderSize(UINT16 Size);
		/** Sets the color of image border
			@param Re The new color of border in the real part.
			@param Im The new color of border in the imaginary part.
			@return false if failed */
		bool SetBorderColor(FLOAT32 Re,FLOAT32 Im);
		/** Copies SourceImage of type CComplexImage to this image.
			The values xmin, ymin, xmax and ymax) specifies the rectangle to
			be copied, uses integers because of origo offset. ROI is inactivated.
			@param SourceImage Image to copy from.
			@param xmin defines left side of rectangle in source image to copy from (including this value).
			@param ymin defines top side of rectangle in source image to copy from (including this value).
			@param xmax defines right side of rectangle in source image to copy from (excluding this value).
			@param ymax defines buttom side of rectangle in source image to copy from (excluding this value).
			@return false if outside image */
		bool CopySubImage(CComplexImage& SourceImage,int xmin,int ymin,
				  int xmax,int ymax);
		/** Same as CopySubImage(CComplexImage& SourceImage,int xmin,int ymin,
				  int xmax,int ymax) except the points is given in CPoint2D<int> types.
			@param SourceImage Image to copy from.
			@param UpperLeft position in the upper left corner to start from (including this value).
			@param LowerRight position in the lower right corner to stop at (excluding this value).
			@return false if outside image */
		bool CopySubImage(CComplexImage& SourceImage,const CPoint2D<int>& UpperLeft,
				  const CPoint2D<int>& LowerRight);
		/** @returns the byte width of a scanline */
		UINT16 GetLineByteWidth() const;
		/** Returns the pointer to the begining of the real value image data.
			@return FLOAT32* to real value image data */
		FLOAT32* GetImageDataPtrRe();
		/** Returns the pointer to the begining of the imaginary value image data.
			@return FLOAT32* to imaginary value image data */
       	FLOAT32* GetImageDataPtrIm();
		/** Returns the pointer to the TComplexImage structure.
			To use the kernel C functions it is sometimes necessary 
			to pass the TComplexImage structure.
			In other cases do not use this function to manipulate
            the image but use the methods in this class!
			@return pointer to a TComplexImage structure. */
		const TComplexImage* GetConstTComplexImagePtr() const;
		/** Returns the pointer to the TComplexImage structure.
			To use the kernel C functions it is sometimes necessary
			to pass the TComplexImage structure which contents will 
			be changed.
			In other cases do not use this function to manipulate the
			image but use the methods in this class!
			@return pointer to a TComplexImage structure. */
		TComplexImage* GetTComplexImagePtr();
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
		/** Get origo for image
			@return A 2D point - position may be negative or
								greater than image size */
		inline CPoint2D<int> GetOrigo() const;
		/** @returns the origin of image */
		inline ORIGIN GetOrigin() const;
		/** Rotates the image clockwise in step of 90 degrees.
			Note: Not speed optimised!
			@param Steps Rotate image 90 degrees * Steps */
		void Rotate90(int Steps);
		/** flips the image around a centered horisontal line.
			Note: Not speed optimised! */
		void FlipHorisontal();
		/** flips the image around a centered vertical line.
			Note: Not speed optimised! */
		void FlipVertical();
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
		/// Prints all important data in image to stdout
		void PrintInfo() const;
		/** @name streaming operators */
		//@{
		/** Writes all the image information to stream.
			Calls StreamIndent() for each streamed line. */
		friend ostream& operator<<(ostream& s,const CComplexImage& Img);
		//@}
		/** Adds Bias to all pixels. The border (if present) is not
			affected. Adds to the history.
			@param Bias is the bias value
			@return False if the this image is empty */
		bool ApplyBias(float Bias);
		/** Scales the real and imaginary float colors
			using a constant multiplier. The border (if present) is not
			affected. Adds to the history.
			@param Prefactor is the scaling factor
			@return False if the this image is empty */
		bool ScaleIntensity(float Prefactor);
		/** Calculates the sum of two CComplexImages. The border (if present) is not
			affected. Adds to the history.
			@param Addend is the image of class CComplexImage to be added
			@return False if one or both of the images involved are empty */
		bool Add(CComplexImage &Addend);
		/** Subtract calculates the difference of two CComplexImages. The border (if present) is not
			affected. Adds to the history.
			@param Subtrahend is the image of class CComplexImage to
				be subtracted
			@return False if one or both of the images involved are empty */
		bool Subtract(CComplexImage &Subtrahend);
		/** Calculates product of two images. The border (if present) is not
			affected. Adds to the history.
			@param Factor is the image of class CComplexImage
				to be multiplied
			@return False if one or both of the images involved are empty */
		bool Multiply(CComplexImage &Factor);
		/** Calculates product of two images.
			The image Factor is complex conjugated before multiplication.
			The border (if present) is not affected. Adds to the history.
			@param Factor is the image of class CComplexImage
				to be complex conjugated and multiplied
			@return False if one or both of the images involved are empty
				or if they have different dimensions */
		bool CCFactorAndMultiply(CComplexImage &Factor);
		/** Changes sign of the imaginary part. 
			The border (if present) is not affected. Adds to the history.
			@return False if the image is empty */
		bool ComplexConjugate();
		/** swaps diagonal quarters. The border (if present) is not
			affected. Adds to the history.
			@return False if the image is empty  or if they
				have different dimensions */
		bool Rearrange();
		/** Performs a displacement so that (dx,dy)
			in the old picture becomes (0,0). Periodic continuation is assumed.
			The border (if present) is not affected. Adds to the history.
			@param dx is the horizontal displacement
			@param dy is the vertical displacement
			@return False if the image is empty */
		bool CopyDisplace(int dx,int dy);
		/** Copies a source image into 'this' object without
			scaling. If the width/height
			of the source is larger than that of 'this' object, zero padding
			is applied. Simple cuts are applied for reduced width/height.
			The border (if present) is not affected. Adds to the history.
			@param Source is the source image
			@return False if 'this' image and/or the source image
				are empty */
		bool CutOrZeroPad(CComplexImage &Source);
		/** Copies two images of class CImage or CByteImage into this object
			History from both Re and Im are copied to this image, old 
			history in this image is deleted. ROI is inactivated.
			@param Re is the image to be copied into the real part of this object
			@param Im is the image to be copied into the imaginary part
			@return False if the two images have different dimensions or
				are empty */
		bool Copy(CStdImage &Re, CStdImage &Im);
		/** Copies two images of class CIntImage into this object
			History from both Re and Im are copied to this image, old 
			history in this image is deleted. ROI is inactivated.
			@param Re is the image to be copied into the real part
				of this object
			@param Im is the image to be copied into the imaginary part
			@return False if the two images have different dimensions or
				are empty */
		bool Copy(CIntImage &Re, CIntImage &Im);
		/** Copies two images of class CFloatImage into this object
			History from both Re and Im are copied to this image, old 
			history in this image is deleted. ROI is inactivated.
			@param Re is the image to be copied into the real part of this object
			@param Im is the image to be copied into the imaginary part
			@return False if the two images have different dimensions or are empty */
		bool Copy(CFloatImage &Re, CFloatImage &Im);
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

inline CComplexImage::ReRowIterator CComplexImage::ReRow(UINT32 i)
{
	return ReRowIterator(m_Image.ppAllScanLinesRe[i+m_Image.Origo.y],GetMinX()+m_Image.Origo.x, GetMaxX()+m_Image.Origo.x);
}

inline CComplexImage::ReConstRowIterator CComplexImage::ReConstRow(UINT32 i) const
{
	return ReConstRowIterator(m_Image.ppAllScanLinesRe[i+m_Image.Origo.y],GetMinX()+m_Image.Origo.x, GetMaxX()+m_Image.Origo.x);
}

inline CComplexImage::ReColumnIterator CComplexImage::ReColumn(UINT32 i)
{
	return ReColumnIterator(i+m_Image.Origo.x,m_Image.ppAllScanLinesRe[GetMinY()+m_Image.Origo.y],
						m_Image.ppAllScanLinesRe[GetMaxY()+m_Image.Origo.y-1]-m_Image.ByteWidth/sizeof(FLOAT32),
						m_Image.ByteWidth/sizeof(FLOAT32));
}

inline CComplexImage::ReConstColumnIterator CComplexImage::ReConstColumn(UINT32 i) const
{
	return ReConstColumnIterator(i+m_Image.Origo.x,m_Image.ppAllScanLinesRe[GetMinY()+m_Image.Origo.y],
						m_Image.ppAllScanLinesRe[GetMaxY()+m_Image.Origo.y-1]-m_Image.ByteWidth/sizeof(FLOAT32),
						m_Image.ByteWidth/sizeof(FLOAT32));
}

inline CComplexImage::ImRowIterator CComplexImage::ImRow(UINT32 i)
{
	return ImRowIterator(m_Image.ppAllScanLinesIm[i+m_Image.Origo.y],GetMinX()+m_Image.Origo.x, GetMaxX()+m_Image.Origo.x);
}

inline CComplexImage::ImConstRowIterator CComplexImage::ImConstRow(UINT32 i) const
{
	return ImConstRowIterator(m_Image.ppAllScanLinesIm[i+m_Image.Origo.y],GetMinX()+m_Image.Origo.x, GetMaxX()+m_Image.Origo.x);
}

inline CComplexImage::ImColumnIterator CComplexImage::ImColumn(UINT32 i)
{
	return ReColumnIterator(i+m_Image.Origo.x,m_Image.ppAllScanLinesIm[GetMinY()+m_Image.Origo.y],
						m_Image.ppAllScanLinesIm[GetMaxY()+m_Image.Origo.y-1]-m_Image.ByteWidth/sizeof(FLOAT32),
						m_Image.ByteWidth/sizeof(FLOAT32));
}

inline CComplexImage::ImConstColumnIterator CComplexImage::ImConstColumn(UINT32 i) const
{
	return ReConstColumnIterator(i+m_Image.Origo.x,m_Image.ppAllScanLinesIm[GetMinY()+m_Image.Origo.y],
						m_Image.ppAllScanLinesIm[GetMaxY()+m_Image.Origo.y-1]-m_Image.ByteWidth/sizeof(FLOAT32),
						m_Image.ByteWidth/sizeof(FLOAT32));
}

inline bool CComplexImage::IsEmpty() const
{
	return (m_Image.pPixelStreamRe==NULL);
}

inline FLOAT32 CComplexImage::GetPixelRe(int x, int y) const
{
	UINT16 border=m_Image.BorderSize;
	/* adjust origo offset */
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;
	
	if ((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		return m_Image.ppMatrixRe[y][x];
	}
	else
	{
		ostringstream ost;
		ost << "CComplexImage::GetPixelRe(" << x << "," << y << ") Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline FLOAT32 CComplexImage::GetPixelIm(int x, int y) const
{
	UINT16 border=m_Image.BorderSize;
	/* adjust origo offset */
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;
	
	if ((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		return m_Image.ppMatrixIm[y][x];
	}
	else
	{
		ostringstream ost;
		ost << "CComplexImage::GetPixelIm(" << x << "," << y << ") Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline FLOAT32 CComplexImage::GetPixelRe(const CPoint2D<int>& Pos) const
{
	UINT16 border=m_Image.BorderSize;
	//* adjust origo offset
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;
	
	if ((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		return m_Image.ppMatrixRe[y][x];
	}
	else
	{
		ostringstream ost;
		ost << "CComplexImage::GetPixelRe" << Pos << " Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline FLOAT32 CComplexImage::GetPixelIm(const CPoint2D<int>& Pos) const
{
	UINT16 border=m_Image.BorderSize;
	//* adjust origo offset
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;
	
	if ((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		return m_Image.ppMatrixIm[y][x];
	}
	else
	{
		ostringstream ost;
		ost << "CComplexImage::GetPixelIm" << Pos << " Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline FLOAT32 CComplexImage::GetPixelFastRe(int x, int y) const
{
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;
	
	return m_Image.ppMatrixRe[y][x];
}

inline FLOAT32 CComplexImage::GetPixelFastIm(int x, int y) const
{
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;
	
	return m_Image.ppMatrixIm[y][x];
}

inline FLOAT32 CComplexImage::GetPixelFastRe(const CPoint2D<int>& Pos) const
{
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;
	
	return m_Image.ppMatrixRe[y][x];
}

inline FLOAT32 CComplexImage::GetPixelFastIm(const CPoint2D<int>& Pos) const
{
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;
	return m_Image.ppMatrixIm[y][x];
}

inline bool CComplexImage::SetPixel(int x, int y, FLOAT32 Re,FLOAT32 Im)
{
	UINT16 border=m_Image.BorderSize;
	/* adjust origo offset */
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;
	
	if((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		m_Image.ppMatrixRe[y][x]=Re; 
		m_Image.ppMatrixIm[y][x]=Im;
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CComplexImage::SetPixel(" << x << "," << y << ") Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

inline bool CComplexImage::SetPixel(const CPoint2D<int>& Pos, FLOAT32 Re, FLOAT32 Im)
{
	UINT16 border=m_Image.BorderSize;
	//* adjust origo offset
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;
	
	if((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		m_Image.ppMatrixRe[y][x]=Re;
		m_Image.ppMatrixIm[y][x]=Im;
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CComplexImage::SetPixel" << Pos << " Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

inline void CComplexImage::SetPixelFast(int x, int y, FLOAT32 Re, FLOAT32 Im)
{
	/* adjust origo offset */
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;
	m_Image.ppMatrixRe[y][x]=Re;
	m_Image.ppMatrixIm[y][x]=Im;
}

inline void CComplexImage::SetPixelFast(const CPoint2D<int>& Pos, FLOAT32 Re, FLOAT32 Im)
{
	//* adjust origo offset
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;
	m_Image.ppMatrixRe[y][x]=Re;
	m_Image.ppMatrixIm[y][x]=Im;
}

inline bool CComplexImage::SetPixelRe(int x, int y, FLOAT32 Re)
{
	UINT16 border=m_Image.BorderSize;
	/* adjust origo offset */
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;

	if((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		m_Image.ppMatrixRe[y][x]=Re; 
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CComplexImage::SetPixelRe(" << x << "," << y << ") Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

inline bool CComplexImage::SetPixelRe(const CPoint2D<int>& Pos, FLOAT32 Re)
{
	UINT16 border=m_Image.BorderSize;
	/* adjust origo offset */
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;

	if((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		m_Image.ppMatrixRe[y][x]=Re; 
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CComplexImage::SetPixelRe" << Pos << " Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

inline bool CComplexImage::SetPixelIm(int x, int y, FLOAT32 Im)
{
	UINT16 border=m_Image.BorderSize;
	/* adjust origo offset */
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;

	if((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		m_Image.ppMatrixIm[y][x]=Im;
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CComplexImage::SetPixelIm(" << x << "," << y << ") Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

inline bool CComplexImage::SetPixelIm(const CPoint2D<int>& Pos, FLOAT32 Im)
{
	UINT16 border=m_Image.BorderSize;
	/* adjust origo offset */
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;

	if((x>=-border) && (x<(int) m_Image.Width+border) &&
		(y>=-border) && (y<(int) m_Image.Height+border))
	{
		m_Image.ppMatrixIm[y][x]=Im; 
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CComplexImage::SetPixelIm" << Pos << " Outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

inline void CComplexImage::SetPixelFastRe(int x, int y, FLOAT32 Re)
{
	/* adjust origo offset */
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;
	
	m_Image.ppMatrixRe[y][x]=Re;
}

inline void CComplexImage::SetPixelFastRe(const CPoint2D<int>& Pos, FLOAT32 Re)
{
	/* adjust origo offset */
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;
	
	m_Image.ppMatrixRe[y][x]=Re;
}

inline void CComplexImage::SetPixelFastIm(int x, int y, FLOAT32 Im)
{
	/* adjust origo offset */
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;
	
	m_Image.ppMatrixIm[y][x]=Im;
}

inline void CComplexImage::SetPixelFastIm(const CPoint2D<int>& Pos, FLOAT32 Im)
{
	/* adjust origo offset */
	int x = Pos.GetX()+m_Image.Origo.x;
	int y = Pos.GetY()+m_Image.Origo.y;
	
	m_Image.ppMatrixIm[y][x]=Im;
}

inline UINT32 CComplexImage::GetWidth() const
{
	// get width of image
	return m_Image.Width;
}

inline UINT32 CComplexImage::GetHeight() const
{
	// get height of image
	return m_Image.Height;
}

inline UINT16 CComplexImage::GetBorderSize() const
{
	// get size of border
	return m_Image.BorderSize;
}

inline int CComplexImage::GetMinX() const
{
	if (ROIActive()==true)
		return m_ROIUpperLeft.GetX();
	else
		return -m_Image.Origo.x;
}

inline int CComplexImage::GetMinY() const
{
	if (ROIActive()==true)
		return m_ROIUpperLeft.GetY();
	else
		return -m_Image.Origo.y;
}

inline int CComplexImage::GetMaxX() const
{
	if (ROIActive()==true)
		return m_ROILowerRight.GetX();
	else
		return m_Image.Width-m_Image.Origo.x;
}

inline int CComplexImage::GetMaxY() const
{
	if (ROIActive()==true)
		return m_ROILowerRight.GetY();
	else
		return m_Image.Height-m_Image.Origo.y;
}

inline CPoint2D<int> CComplexImage::GetOrigo() const
{
	return CPoint2D<int>(m_Image.Origo.x,m_Image.Origo.x);
}

inline ORIGIN CComplexImage::GetOrigin() const
{
	// get the origin of image
	return m_Image.Origin;
}

inline void CComplexImage::SetOrigo(UINT16 x, UINT16 y)
{
	SetOrigo(CPoint2D<int>(x,y));
}

inline void CComplexImage::SetOrigo(const CPoint2D<int>& Pos)
{
	m_Image.Origo.x=Pos.GetX();
	m_Image.Origo.y=Pos.GetY();
	if (m_ROIActive==true)
	{
		m_ROIUpperLeft  -= Pos;
		m_ROILowerRight -= Pos;
	}
}

inline bool CComplexImage::SetROI(const CPoint2D<INT32>& UL, const CPoint2D<INT32>& LR)
{
	if ((UL.GetX()>=LR.GetX()) || (UL.GetY()>=LR.GetY()))
	{
		ostringstream ost;
		ost << "CComplexImage::SetROI() UL " << UL << " greater than LR " << LR << 
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
		ost << "CComplexImage::SetROI() UL " << UL << " or LR " << LR << 
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

#endif //_IPL98_COMPLEX_IMAGE_H

