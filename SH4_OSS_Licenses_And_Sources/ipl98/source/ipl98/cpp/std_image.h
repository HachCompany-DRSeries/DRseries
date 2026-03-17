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

#ifndef _IPL98_STDIMAGE_H
#define _IPL98_STDIMAGE_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "generic_image.h"
#include "palette.h"
#include "text.h"
#include "error.h"
#include "../kernel_c/ipl98_types.h"
#include "../kernel_c/image/kernel_functions.h"
#include "../kernel_c/kernel_string.h"
#include "../kernel_c/kernel_error.h"
#include <sstream>
#include <string>

namespace ipl{ // use namespace if C++
	
using std::string;
using std::ostringstream;
// external definitions
class CIntImage;
class CFloatImage;
class CComplexImage;

/** @ingroup containerclasses
@{ */

/** CStdImage is an abstract base class derived from CGenericImage from which all
	photo related image classes are derived, last updated 8/21/2005.
	At the moment two derived classes exists: CImage and CByteImage.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CStdImage ipl98/cpp/std_image.h
	@see CGenericImage
	@version 1.54,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */

class CStdImage : public CGenericImage{
	public: // attributes
		/// image palette, see the CPalette class for more information
		CPalette m_Pal;
		/// history, keeps track of operations performed on image
		CText m_History;
	protected: // attributes
		/// The structure from the C kernel, holding all information about the image
		TImage m_Image;

	public: // methods
		/// default destructor
		virtual ~CStdImage() {};
		/// default constructor
		CStdImage();
		/** Overloading of assignment operator. All information is copied
			to the destination image including border pixels, palette, ROI and history.
			Previous data in this object are destroyed. Origin is set to RAM. File
			information is left empty. Source image is left unchanged. Adds new 
			line to history.
			An error is produced if both sides of the assignment operator is 
			the same object. */
		CStdImage& operator=(const CStdImage& SourceImage);
		/** Allocates memory for image with given size and pixel depth.
			All pixel values are initialised to 0.
			@param Width The width of image in pixels
			@param Height The height of image in pixels
			@param Bits Number of bits per pixel, 1, 4, 8 or 24. Note: If the instance
				of this object is a CByteImage an error will be generated if "Bits" is
				not 8 b/p.
			@param Color Initial color value in all pixels, for 24 b/p (see Bits parameter) 
				images an RGB value, otherwise a palette index value..
			@return False, if allocation failed or in case of CByteImage if "Bits" is not 8 b/p. */
		virtual bool Alloc(UINT32 Width, UINT32 Height, UINT16 Bits, UINT32 Color)=0;
		/** Allocates memory for image with given size and pixel depth.
			Pixel values are not initialised.
			@param Width The width of image in pixels
			@param Height The height of image in pixels
			@param Bits Number of bits per pixel, 1, 4, 8 or 24
			@return False, if allocation failed or in case of CByteImage if "Bits" is not 8 b/p. */
		virtual bool AllocFast(UINT32 Width, UINT32 Height, UINT16 Bits)=0;
		/** Deletes all allocated memory for image and resets variables. ROI is inactivated,
			history is cleared. */
		virtual void Empty();
		/** Checks if any data is present.
			@return True, if image contain no data. */
		inline bool IsEmpty() const;
		/** load file of type given by extension from disk. See inherited class for supported filetypes.
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
		/** Gets the file name for image, if available.
			@param Str The file name is passed in this parameter, empty if not available.
			@return False, if file name not available. */
		bool GetFileName(string& Str);
		/** Does the same as GetFileName(string& Str) except a character pointer to the file
			name is returned.
			@return NULL if file name is unavailable. */
		const char* GetFileName();
		/** Gets the path for image, if available.
			@param Str The file name is passed in this parameter, empty if not available.
			@return False, if path is unavailable. */
		bool GetPathName(string& Str);
		/** Does the same as GetPathName(string& Str) except a character pointer to the path
			is returned.
			@return NULL if path is unavailable. */
		const char* GetPathName();
		/** Gets the complete path and file name for image, if available.
			@param Str The complete path and file name is passed in this parameter,
			empty if not available.
			@return False, if path and file name is unavailable. */
		bool GetPathAndFileName(string& Str);
		/** Does the same as GetPathAndFileName(string& Str) except a character pointer to the path
			and file name is returned.
			@return NULL if path and file name is unavailable. */
		const char* GetPathAndFileName();
		/**	Sets all pixels in the image to given color. Old pixel values are destroyed.
			Adds new line to history.
			@param Color New pixel values in image.
			@return False if image is empty */
		virtual bool Flush(UINT32 Color);
		/**	Inverts all pixel values in image. Works on all pixel depths (i.e. 1, 8 and 24 b/p).
			Adds new line to history.
			@return False if image is empty */
		bool Invert();
		/** Returns a pixel value at position (x,y),
			including range check, slow access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return pixel value, for 24 b/p: R+G*256+B*256*256. */
		virtual inline UINT32 GetPixel(int x, int y) const=0;
		/** Returns a pixel value at position Pos
			including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@return pixel value, for 24 b/p: R+G*256+B*256*256. */
		virtual inline UINT32 GetPixel(const CPoint2D<int>& Pos) const=0;
		/** Returns a pixel value at position Pos
			including range check, slow access.
			@param Pos 2-Dimensional position in image.
			@return pixel value, for 24 b/p: R+G*256+B*256*256. */
		virtual inline UINT32 GetPixel(const CPoint2D<INT16>& Pos) const=0;
		/** Returns a pixel value at position (x,y), without range check.
			Fast access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return pixel value, for 24 b/p: R+G*256+B*256*256. */
		virtual UINT32 GetPixelFast(int x, int y) const=0;
		/** Returns a pixel value at position Pos without range check.
			Fast access.
			@param Pos 2-Dimensional position in image.
			@return pixel value, for 24 b/p: R+G*256+B*256*256. */
		virtual inline UINT32 GetPixelFast(const CPoint2D<int>& Pos) const=0;
		/** Returns a pixel value at position Pos without range check.
			Fast access.
			@param Pos 2-Dimensional position in image.
			@return pixel value, for 24 b/p: R+G*256+B*256*256. */
		virtual inline UINT32 GetPixelFast(const CPoint2D<INT16>& Pos) const=0;
		/** Returns the interpolated pixel value at position (x,y), including
			range check.
			To be used for graytone images only, needs 1 or 8 b/p, produces
			an error in other cases. Uses bilinear interpolation.
			@param x The position in horisontal direction
			@param y The position in vertical direction
			@return interpolated pixel value */
		virtual inline float GetPixelInterpolated(float x,float y) const;
		/** Returns the interpolated pixel value at position Pos, including
			range check.
			To be used for graytone images only, needs 1 or 8 b/p, produces
			an error in other cases. Uses bilinear interpolation.
			@param Pos 2-Dimensional position in image.
			@return interpolated pixel value */
		virtual inline float GetPixelInterpolated(const CPoint2D<float>& Pos) const;
		/** Returns the interpolated pixel value at position (x,y), without
			range check.
			To be used for graytone images only, needs 1 or 8 b/p, produces
			an error in other cases. Uses bilinear interpolation.
			@param x The position in horisontal direction
			@param y The position in vertical direction
			@return interpolated pixel value */
		virtual inline float GetPixelInterpolatedFast(float x,float y) const;
		/** Returns the interpolated pixel value at position Pos, without
			range check.
			To be used for graytone images only, needs 1 or 8 b/p, produces
			an error in other cases. Uses bilinear interpolation.
			@param Pos 2-Dimensional position in image.
			@return interpolated pixel value */
		virtual inline float GetPixelInterpolatedFast(const CPoint2D<float>& Pos) const;
		/** returns the interpolated color value at position (x,y), including
			range check and origo offset. To be used for 
			images with 24 b/p only, produces an error in other cases. Uses bilinar 
			interpolation for each color channel (R, G and B) and rounds of
			to nearest integer value.
			@param x The position in horisontal direction
			@param y The position in vertical direction
			@return interpolated pixel value */
		virtual inline UINT32 GetColorInterpolated(float x, float y) const;
		/** returns the interpolated color value at position Pos, including
			range check and origo offset. To be used for 
			images with 24 b/p only, produces an error in other cases. Uses bilinar 
			interpolation for each color channel (R, G and B) and rounds of
			to nearest integer value.
			@param Pos 2-Dimensional position in image.
			@return interpolated pixel value */
		virtual inline UINT32 GetColorInterpolated(const CPoint2D<float>& Pos) const;
		/** returns the interpolated color value at position (x,y), without
			range check and origo offset. To be used for images with 24 b/p only. Uses bilinar 
			interpolation for each color channel (R, G and B) and rounds of
			to nearest integer value.
			@param x The position in horisontal direction
			@param y The position in vertical direction
			@return interpolated pixel value */
		virtual inline UINT32 GetColorInterpolatedFast(float x, float y) const;
		/** returns the interpolated color value at position Pos, without
			range check and origo offset. To be used for images with 24 b/p only. Uses bilinar 
			interpolation for each color channel (R, G and B) and rounds of
			to nearest integer value.
			@param Pos 2-Dimensional position in image.
			@return interpolated pixel value */
		virtual inline UINT32 GetColorInterpolatedFast(const CPoint2D<float>& Pos) const;
		/** Sets a pixel at position (x,y),
			including range check and origo offset, slow access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@param Color The new pixel value, for 24 b/p images an RGB value,
				otherwise a palette index value.
			@return false if outside image or if b/p is not supported. */
		virtual bool SetPixel(int x, int y, UINT32 Color)=0; 
		/** Sets a pixel at position Pos,
			including range check and origo offset, slow access.
			@param Pos 2-Dimensional position in image.
			@param Color The new pixel value, for 24 b/p images an RGB value,
				otherwise a palette index value.
			@return false if outside image or if b/p is not supported. */
		virtual inline bool SetPixel(const CPoint2D<int>& Pos, UINT32 Color)=0; 
		/** Sets a pixel at position Pos,
			including range check and origo offset, slow access.
			@param Pos 2-Dimensional position in image.
			@param Color The new pixel value, for 24 b/p images an RGB value,
				otherwise a palette index value.
			@return false if outside image or if b/p is not supported. */
		virtual bool SetPixel(const CPoint2D<INT16>& Pos, UINT32 Color)=0; 
		/** Sets a pixel at position (x,y), without range check but with origo offset, fast access.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@param Color The new pixel value, for 24 b/p images an RGB value,
				otherwise a palette index value. */
		virtual void SetPixelFast(int x, int y, UINT32 Color)=0; 
		/** Sets a pixel at position Pos, without range check but with origo offset, fast access.
			@param Pos 2-Dimensional position in image.
			@param Color The new pixel value, for 24 b/p images an RGB value,
				otherwise a palette index value. */
		virtual inline void SetPixelFast(const CPoint2D<int>& Pos, UINT32 Color)=0;
		/** Sets a pixel at position Pos, without range check but with origo offset, fast access.
			@param Pos 2-Dimensional position in image.
			@param Color The new pixel value, for 24 b/p images an RGB value,
				otherwise a palette index value.
			@return false if b/p is not supported. */
		virtual inline void SetPixelFast(const CPoint2D<INT16>& Pos, UINT32 Color)=0; 
		/** Returns the red component value at position (x,y), including range check.
			Slow access. If the image contains a palette this method takes care 
			of finding the color value from the palette index and extracting the 
			red component. If no palette is present it extracts the red component
			directly from the pixel value.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return Red component value */
		virtual inline UINT8 GetRedComponent(int x, int y) const;
		/** Returns the red component value at position (x,y), without range check.
			Fast access. If the image contains a palette this method takes care 
			of finding the color value from the palette index and extracting the 
			red component. If no palette is present it extracts the red component
			directly from the pixel value.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return Red component value */
		virtual inline UINT8 GetRedComponentFast(int x, int y) const;
		/** Returns the red component value at position Pos, including range check.
			Slow access. If the image contains a palette this method takes care 
			of finding the color value from the palette index and extracting the 
			red component. If no palette is present it extracts the red component
			directly from the pixel value.
			@param Pos 2-Dimensional position in image.
			@return Red component value */
		virtual inline UINT8 GetRedComponent(const CPoint2D<int>& Pos) const;
		/** Returns the red component value at position Pos, without range check.
			Fast access. If the image contains a palette this method takes care 
			of finding the color value from the palette index and extracting the 
			red component. If no palette is present it extracts the red component
			directly from the pixel value.
			@param Pos 2-Dimensional position in image.
			@return Red component value */
		virtual inline UINT8 GetRedComponentFast(const CPoint2D<int>& Pos) const;
		/** Returns the green component value at position (x,y), including range check.
			Slow access. If the image contains a palette this method takes care 
			of finding the color value from the palette index and extracting the 
			green component. If no palette is present it extracts the green component
			directly from the pixel value.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return Green component value */
		virtual inline UINT8 GetGreenComponent(int x, int y) const;
		/** Returns the green component value at position (x,y), without range check.
			Fast access. If the image contains a palette this method takes care 
			of finding the color value from the palette index and extracting the 
			green component. If no palette is present it extracts the green component
			directly from the pixel value.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return Green component value */
		virtual inline UINT8 GetGreenComponentFast(int x, int y) const;
		/** Returns the green component value at position Pos, including range check.
			Slow access. If the image contains a palette this method takes care 
			of finding the color value from the palette index and extracting the 
			green component. If no palette is present it extracts the green component
			directly from the pixel value.
			@param Pos 2-Dimensional position in image.
			@return Green component value */
		virtual inline UINT8 GetGreenComponent(const CPoint2D<int>& Pos) const;
		/** Returns the green component value at position Pos, without range check.
			Fast access. If the image contains a palette this method takes care 
			of finding the color value from the palette index and extracting the 
			green component. If no palette is present it extracts the green component
			directly from the pixel value.
			@param Pos 2-Dimensional position in image.
			@return Green component value */
		virtual inline UINT8 GetGreenComponentFast(const CPoint2D<int>& Pos) const;
		/** Returns the blue component value at position (x,y), including range check.
			Slow access. If the image contains a palette this method takes care 
			of finding the color value from the palette index and extracting the 
			blue component. If no palette is present it extracts the blue component
			directly from the pixel value.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return Blue component value */
		virtual inline UINT8 GetBlueComponent(int x, int y) const;
		/** Returns the blue component value at position (x,y), without range check.
			Fast access. If the image contains a palette this method takes care 
			of finding the color value from the palette index and extracting the 
			blue component. If no palette is present it extracts the blue component
			directly from the pixel value.
			@param x The position in horisontal direction.
			@param y The position in vertical direction.
			@return Blue component value */
		virtual inline UINT8 GetBlueComponentFast(int x, int y) const;
		/** Returns the blue component value at position Pos, including range check.
			Slow access. If the image contains a palette this method takes care 
			of finding the color value from the palette index and extracting the 
			blue component. If no palette is present it extracts the blue component
			directly from the pixel value.
			@param Pos 2-Dimensional position in image.
			@return Blue component value */
		virtual inline UINT8 GetBlueComponent(const CPoint2D<int>& Pos) const;
		/** Returns the blue component value at position Pos, without range check.
			Fast access. If the image contains a palette this method takes care 
			of finding the color value from the palette index and extracting the 
			blue component. If no palette is present it extracts the blue component
			directly from the pixel value.
			@param Pos 2-Dimensional position in image.
			@return Blue component value */
		virtual inline UINT8 GetBlueComponentFast(const CPoint2D<int>& Pos) const;
		/** @return width of image. */
		virtual UINT32 GetWidth() const;
		/** @return height of image. */
		virtual UINT32 GetHeight() const;
		/** @return size of border */
		virtual UINT16 GetBorderSize() const;
		/** Sets the size and color of image border
			@param Size The size of border in pixels
			@param Color The color of the new border
			@return false if failed */
		bool SetBorder(UINT16 Size, UINT32 Color);
		/** Sets the size of image border
			@param Size Size of border in pixel to be added in each direction.
			@return false if failed */
		bool SetBorderSize(UINT16 Size);
		/** Sets the color of image border
			@param Color The new color of border
			@return false if failed */
		bool SetBorderColor(UINT32 Color);
		/** Copies a CStdImage to this image.
			The positions (xmin,ymin) and  (xmax,ymax) specifies the rectangle to
			be copied including the specified values. The method uses integers because
			of origo offset which may force the user to use values less than zero.
			If you want the resulting image placed in this image just use the same
			object for SourceImage. Note: In that case the method is a bit slower 
			(needs to copy a temporary image to the destination). ROI is inactivated.
			Adds new line to history.
			@param SourceImage Image to copy and convert from. The SourceImage is left unchanged.
			@param xmin defines left side of rectangle in source image to copy from
				(including this value).
			@param ymin defines top side of rectangle in source image to copy from
				(including this value).
			@param xmax defines right side of rectangle in source image to copy from
				(excluding this value).
			@param ymax defines buttom side of rectangle in source image to copy from
				(iexcluding this value).
			@return false if outside image */
		bool CopySubImage(CStdImage& SourceImage,int xmin,int ymin,
				  int xmax,int ymax);
		/** Copies a CStdImage to this image.
			The positions UpperLeft and LowerRight specifies the rectangle to
			be copied including the specified values. The method uses integers because
			of origo offset which may force the user to use values less than zero.
			If you want the resulting image placed in this image just use the same
			object for SourceImage. Note: In that case the method is a bit slower 
			(needs to copy a temporary image to the destination). ROI is inactivated.
			Adds new line to history.
			@param SourceImage Image to copy a sub area from.
			@param UpperLeft position in the upper left corner to start from (including this value).
			@param LowerRight position in the lower left corner to stop at (excluding this value).
			@return false if outside image */
		bool CopySubImage(CStdImage& SourceImage,const CPoint2D<int>& UpperLeft,
				  const CPoint2D<int>& LowerRight);
		/** Copies a whole image to "this" object and convert the pixel depth, if the new
			bits per pixel is the same as source image b/p the "="-operator
			is used. See inherited classes for details about possible conversions.
			ROI is inactivated. Adds new line to history.
			@param Bits Bits per pixel for the new image contained in "this" object.
			@param SourceImage Image to copy and convert from. The SourceImage is left unchanged.
			@return False, if source image is empty or if Bits is not 1, 8 or 24. */
		virtual bool CopyConvert(UINT16 Bits,CStdImage& SourceImage)=0;
		/** @return byte width of a scanline */
		virtual UINT16 GetLineByteWidth() const;
		/** Returns a pointer to the beginning of image data.
			@return UINT8* to image data */
		virtual UINT8* GetImageDataPtr();
		/** To use the kernel C functions it is sometimes necessary
			to pass the TImage structure.
			In other cases do not use this function to manipulate the 
			image but use the methods in this class!
			@return pointer to a TImage structure. */
		const TImage* GetConstTImagePtr() const;
		/** To use the kernel C functions it is sometimes necessary
			to pass the TImage structure which contents will be changed.
			In other cases do not use this function to manipulate the 
			image but use the methods in this class!
			@return pointer to a TImage structure. */
		TImage* GetTImagePtr();
		/** Iterate from this value. Takes ROI into account.
			@return min. X relative to origo 
			@see SetROI() */
		virtual inline int GetMinX() const;
		/** Iterate from this value. Takes ROI into account.
			@return min. Y relative to origo
			@see SetROI() */
		virtual inline int GetMinY() const;
		/** Iterate to this value minus one (the image is zero indexed).
			Takes ROI into account.
			@return max. X relative to origo
			@see GetMinX()
			@see SetROI() */
		virtual inline int GetMaxX() const;
		/** Iterate to this value minus one (the image is zero indexed).
			Takes ROI into account.
			@return max. Y relative to origo
			@see GetMinY()
			@see SetROI() */
		virtual inline int GetMaxY() const;
		/** Returns origo for image 
			@return A 2D point - position may be negative or greater than image size */
		virtual inline CPoint2D<int> GetOrigo() const;
		/** Number of bits used to represent one pixel.
			@return bits per pixel. */
		virtual inline UINT16 GetBits() const;
		/** @return the origin of image */
		virtual inline ORIGIN GetOrigin() const;
		/** Rotates image clockwise in step of 90 degrees.
			Note: Not speed optimised! Adds new line to history.
			@param Steps Rotate image 90 degrees * Steps */
		virtual void Rotate90(int Steps);
		/** Flips image around a centered horisontal line.
			Note: Not speed optimised! Adds new line to history. */
		virtual void FlipHorisontal();
		/** Flips image around a centered vertical line.
			Note: Not speed optimised! Adds new line to history. */
		void FlipVertical();
		/** adds Bias to all pixels.  The border (if present) is not
			affected. Adds to the history.
			@param Bias is the bias value
			@return False if *pSource is empty or image depth is not 8 b/p */
		bool ApplyBias(int Bias);
		/** scales the int colors using a constant multiplier. The border (if present) is not
			affected. Adds to the history.
			@param Prefactor is the scaling factor
			@return False if this image is empty or image depth is not 8 b/p */
		bool ScaleIntensity(float Prefactor);
		/** calculates the sum of two CStdImages. The border (if present) is not
			affected. Adds to the history.
			@param Addend is the image of class CStdImage to be added
			@return False if one or both of the images involved are empty or image depth is not 8 b/p */
		bool Add(CStdImage &Addend);
		/** calculates the difference of two CStdImages. The border (if present) is not
			affected. Adds to the history.
			@param Subtrahend is the image of class CStdImage to be subtracted
			@return False if one or both of the images involved are empty
				or image depth is not 8 b/p */
		bool Subtract(CStdImage &Subtrahend);
		/** calculates product of two images. The border (if present) is not
			affected. Adds to the history.
			@param Factor a pointer to the image of class CStdImage to be multiplied
			@return False if one or both of the images involved are empty or image depth is not 8 b/p */
		bool Multiply(CStdImage &Factor);
		/** performs a displacement so that (dx,dy).
			in the old picture becomes (0,0). Periodic continuation
			is assumed. The border (if present) is not
			affected. Adds to the history.
			@param dx is the horizontal displacement
			@param dy is the vertical displacement
			@return False if the image is empty or image depth is not 8 b/p */
		bool CopyDisplace(int dx,int dy);
		/** copies a source image into 'this' object without
			scaling. 'This' object must be allocated. If the width/height
			of the source is larger than that of 'this' object, zero padding
			is applied. Simple cuts are applied for reduced width/height.
			The border (if present) is not affected. Adds to the history.
			@param Source is the spource image
			@return False if 'this' image and/or the source image
				are empty */
		bool CutOrZeroPad(CStdImage &Source);
		/** copies an image of class CIntImage into this object. Adds new line to history.
			Note that this is a straight forward convertion, values are truncated
			directly to the destination range. For more advanced convertions look
			at the methods available in the class CLocalOperation.
			History from Source is copied to this image, old history in this image is deleted.
			@param Source is the source image.
			@return False if the source image is empty. */
		bool Copy(CIntImage &Source);
		/** copies an image of class CFloatImage into this object. Adds new line to history.
			Note that this is a straight forward convertion, values are truncated
			directly to the destination range. For more advanced convertions look
			at the methods available in the class CLocalOperation.
			History from Source is copied to this image, old history in this image is deleted.
			@param Source is the source image.
			@return False if the source image is empty. */
		bool Copy(CFloatImage &Source);
		/** copies partially an image of class CComplex into this object.
			Adds new line to history.
			Note that this is a straight forward convertion, values are truncated
			directly to the destination range. For more advanced convertions look
			at the methods available in the class CLocalOperation.
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
			@param Color The new pixel value, for 24 b/p images an RGB value,
				otherwise a palette index value.
			@return True, always.
			@see CGraphics */
		bool DrawLine(const CPoint2D<int>& Start, const CPoint2D<int>& End, UINT32 Color);
		/** Draws a circle in image using the graphics routine CGraphics::Circle().
			For drawing ellipses and other symbols see graphic functions.
			@param Center Position for center of circle.
			@param Radius Radius of circle.
			@param Color The new pixel value, for 24 b/p images an RGB value,
				otherwise a palette index value.
			@return True, always.
			@see CGraphics */
		bool DrawCircle(const CPoint2D<int>& Center, unsigned int Radius, UINT32 Color);
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

inline bool CStdImage::IsEmpty() const
{
	return (m_Image.pPixelStream==NULL);
}

inline float CStdImage::GetPixelInterpolated(float x, float y) const
{
	// for graytone images only
	// for 1 and 8 b/p: uses bilinear interpolation
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;

	if ((m_Image.Bits!=1) && (m_Image.Bits!=4) && (m_Image.Bits!=8))
	{
		ostringstream ost;
		ost << "CStdImage::GetPixelInterpolated() Image must be 1,8 or 24 b/p" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}

	if((x>-m_Image.BorderSize) && (x<(int) m_Image.Width+m_Image.BorderSize-1) &&
		(y>-m_Image.BorderSize) && (y<(int) m_Image.Height+m_Image.BorderSize-1))
	{
		// upper left pixel (l ~ low)
		int xl = (x >=0 ? (int)x : (int)(x-1)) - m_Image.Origo.x; /* always round down */
		int yl = (y >=0 ? (int)y : (int)(y-1)) - m_Image.Origo.y; /* always round down */
		// lower right pixel (h ~ high)
		int xh = xl + 1;
		int yh = yl + 1;
		// do bilinear interpolation
		return ((GetPixelFast(xl,yl)*((float)xh-x) + GetPixelFast(xh,yl)*(x-(float)xl))*((float)yh-y) +
		 (GetPixelFast(xl,yh)*((float)xh-x) + GetPixelFast(xh,yh)*(x-(float)xl))*(y-(float)yl));
	}
	else
	{
		ostringstream ost;
		ost << "CStdImage::GetPixelInterpolated(" << x << "," << y << 
			") Needs to access pixels outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return -1;
	}
}

inline float CStdImage::GetPixelInterpolated(const CPoint2D<float>& Pos) const
{
	// for graytone images only
	// for 1 and 8 b/p: uses bilinear interpolation
	float x=Pos.GetX()+m_Image.Origo.x;
	float y=Pos.GetY()+m_Image.Origo.y;

	if ((m_Image.Bits!=1) && (m_Image.Bits!=4) && (m_Image.Bits!=8))
	{
		ostringstream ost;
		ost << "CStdImage::GetPixelInterpolated() Image must be 1,8 or 24 b/p" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}

	if((x>-m_Image.BorderSize) && (x<(int) m_Image.Width+m_Image.BorderSize-1) &&
		(y>-m_Image.BorderSize) && (y<(int) m_Image.Height+m_Image.BorderSize-1))
	{
		// upper left pixel (l ~ low)
		int xl = (x >=0 ? (int)x : (int)(x-1)) - m_Image.Origo.x; /* always round down */
		int yl = (y >=0 ? (int)y : (int)(y-1)) - m_Image.Origo.y; /* always round down */
		// lower right pixel (h ~ high)
		int xh = xl + 1;
		int yh = yl + 1;
		// do bilinear interpolation
		return ((GetPixelFast(xl,yl)*((float)xh-x) + GetPixelFast(xh,yl)*(x-(float)xl))*((float)yh-y) +
		 (GetPixelFast(xl,yh)*((float)xh-x) + GetPixelFast(xh,yh)*(x-(float)xl))*(y-(float)yl));
	}
	else
	{
		ostringstream ost;
		ost << "CStdImage::GetPixelInterpolated" << Pos << 
			" Needs to access pixels outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return -1;
	}
}

inline float CStdImage::GetPixelInterpolatedFast(float x, float y) const
{
	// for graytone images only
	// for 1 and 8 b/p: uses bilinar interpolation
	// upper left pixel (l ~ low)
	int xl = (x >=0 ? (int)x : (int)(x-1)); /* always round down */
	int yl = (y >=0 ? (int)y : (int)(y-1)); /* always round down */
	// lower right pixel (h ~ high)
	int xh = xl + 1;
	int yh = yl + 1;
	// do bilinear interpolation
	return ((GetPixelFast(xl,yl)*((float)xh-x) + GetPixelFast(xh,yl)*(x-(float)xl))*((float)yh-y) +
	 (GetPixelFast(xl,yh)*((float)xh-x) + GetPixelFast(xh,yh)*(x-(float)xl))*(y-(float)yl));
}

inline float CStdImage::GetPixelInterpolatedFast(const CPoint2D<float>& Pos) const
{
	// for graytone images only
	// for 1 and 8 b/p: uses bilinar interpolation
	// upper left pixel (l ~ low)
	float x=Pos.GetX();
	float y=Pos.GetY();
	int xl = (x >=0 ? (int)x : (int)(x-1)); /* always round down */
	int yl = (y >=0 ? (int)y : (int)(y-1)); /* always round down */
	// lower right pixel (h ~ high)
	int xh = xl + 1;
	int yh = yl + 1;
	// do bilinear interpolation
	return ((GetPixelFast(xl,yl)*((float)xh-x) + GetPixelFast(xh,yl)*(x-(float)xl))*((float)yh-y) +
	 (GetPixelFast(xl,yh)*((float)xh-x) + GetPixelFast(xh,yh)*(x-(float)xl))*(y-(float)yl));
}

inline UINT32 CStdImage::GetColorInterpolated(float x, float y) const
{
	x += m_Image.Origo.x;
	y += m_Image.Origo.y;

	if (m_Image.Bits!=24)
	{
		ostringstream ost;
		ost << "CStdImage::GetColorInterpolated() Image must be 24 b/p" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}

	if((x>-m_Image.BorderSize) && (x<(int) m_Image.Width+m_Image.BorderSize-1) &&
		(y>-m_Image.BorderSize) && (y<(int) m_Image.Height+m_Image.BorderSize-1))
	{
		// upper left pixel (l ~ low)
		int xl = (x >=0 ? (int)x : (int)(x-1)) - m_Image.Origo.x; /* always round down */
		int yl = (y >=0 ? (int)y : (int)(y-1)) - m_Image.Origo.y; /* always round down */
		// lower right pixel (h ~ high)
		int xh = xl + 1;
		int yh = yl + 1;
		// do bilinear interpolation
		UINT32 UL=GetPixelFast(xl,yl); // Upper left pixel
		UINT32 UR=GetPixelFast(xh,yl); // Upper right pixel
		UINT32 LL=GetPixelFast(xl,yh); // Lower left pixel
		UINT32 LR=GetPixelFast(xh,yh); // Lower right pixel
		// Adding 0.5 before converting to UINT32 rounds the result to the right value!
		UINT32 RVal=(UINT32)((k_PalGetRVal(UL)*(xh-x) + k_PalGetRVal(UR)*(x-xl))*(yh-y) +
						(k_PalGetRVal(LL)*(xh-x) + k_PalGetRVal(LR)*(x-xl))*(y-yl)+0.5);
		UINT32 GVal=(UINT32)((k_PalGetGVal(UL)*(xh-x) + k_PalGetGVal(UR)*(x-xl))*(yh-y) +
						(k_PalGetGVal(LL)*(xh-x) + k_PalGetGVal(LR)*(x-xl))*(y-yl)+0.5);
		UINT32 BVal=(UINT32)((k_PalGetBVal(UL)*(xh-x) + k_PalGetBVal(UR)*(x-xl))*(yh-y) +
						(k_PalGetBVal(LL)*(xh-x) + k_PalGetBVal(LR)*(x-xl))*(y-yl)+0.5);
		return k_PalCreateRGB(RVal,GVal,BVal);
	}
	else
	{
		ostringstream ost;
		ost << "CStdImage::GetColorInterpolated(" << x << "," << y << 
			") Needs to access pixels outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline UINT32 CStdImage::GetColorInterpolated(const CPoint2D<float>& Pos) const
{
	float x=Pos.GetX()+m_Image.Origo.x;
	float y=Pos.GetY()+m_Image.Origo.y;

	if (m_Image.Bits!=24)
	{
		ostringstream ost;
		ost << "CStdImage::GetColorInterpolated() Image must be 24 b/p" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}

	if((x>-m_Image.BorderSize) && (x<(int) m_Image.Width+m_Image.BorderSize-1) &&
		(y>-m_Image.BorderSize) && (y<(int) m_Image.Height+m_Image.BorderSize-1))
	{
		// upper left pixel (l ~ low)
		int xl = (x >=0 ? (int)x : (int)(x-1)) - m_Image.Origo.x; /* always round down */
		int yl = (y >=0 ? (int)y : (int)(y-1)) - m_Image.Origo.y; /* always round down */
		// lower right pixel (h ~ high)
		int xh = xl + 1;
		int yh = yl + 1;
		// do bilinear interpolation
		UINT32 UL=GetPixelFast(xl,yl); // Upper left pixel
		UINT32 UR=GetPixelFast(xh,yl); // Upper right pixel
		UINT32 LL=GetPixelFast(xl,yh); // Lower left pixel
		UINT32 LR=GetPixelFast(xh,yh); // Lower right pixel
		// Adding 0.5 before converting to UINT32 rounds the result to the right value!
		UINT32 RVal=(UINT32)((k_PalGetRVal(UL)*(xh-x) + k_PalGetRVal(UR)*(x-xl))*(yh-y) +
						(k_PalGetRVal(LL)*(xh-x) + k_PalGetRVal(LR)*(x-xl))*(y-yl)+0.5);
		UINT32 GVal=(UINT32)((k_PalGetGVal(UL)*(xh-x) + k_PalGetGVal(UR)*(x-xl))*(yh-y) +
						(k_PalGetGVal(LL)*(xh-x) + k_PalGetGVal(LR)*(x-xl))*(y-yl)+0.5);
		UINT32 BVal=(UINT32)((k_PalGetBVal(UL)*(xh-x) + k_PalGetBVal(UR)*(x-xl))*(yh-y) +
						(k_PalGetBVal(LL)*(xh-x) + k_PalGetBVal(LR)*(x-xl))*(y-yl)+0.5);
		return k_PalCreateRGB(RVal,GVal,BVal);
	}
	else
	{
		ostringstream ost;
		ost << "CStdImage::GetColorInterpolated" << Pos << 
			" Needs to access pixels outside image and border" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline UINT32 CStdImage::GetColorInterpolatedFast(float x, float y) const
{
	// upper left pixel (l ~ low)
	int xl = (x >=0 ? (int)x : (int)(x-1)); /* always round down */
	int yl = (y >=0 ? (int)y : (int)(y-1)); /* always round down */
	// lower right pixel (h ~ high)
	int xh = xl + 1;
	int yh = yl + 1;
	// do bilinear interpolation
	UINT32 UL=GetPixelFast(xl,yl); // Upper left pixel
	UINT32 UR=GetPixelFast(xh,yl); // Upper right pixel
	UINT32 LL=GetPixelFast(xl,yh); // Lower left pixel
	UINT32 LR=GetPixelFast(xh,yh); // Lower right pixel
	// Adding 0.5 before converting to UINT32 rounds the result to the right value!
	UINT32 RVal=(UINT32)((k_PalGetRVal(UL)*(xh-x) + k_PalGetRVal(UR)*(x-xl))*(yh-y) +
					(k_PalGetRVal(LL)*(xh-x) + k_PalGetRVal(LR)*(x-xl))*(y-yl)+0.5);
	UINT32 GVal=(UINT32)((k_PalGetGVal(UL)*(xh-x) + k_PalGetGVal(UR)*(x-xl))*(yh-y) +
					(k_PalGetGVal(LL)*(xh-x) + k_PalGetGVal(LR)*(x-xl))*(y-yl)+0.5);
	UINT32 BVal=(UINT32)((k_PalGetBVal(UL)*(xh-x) + k_PalGetBVal(UR)*(x-xl))*(yh-y) +
					(k_PalGetBVal(LL)*(xh-x) + k_PalGetBVal(LR)*(x-xl))*(y-yl)+0.5);
	return k_PalCreateRGB(RVal,GVal,BVal);
}

inline UINT32 CStdImage::GetColorInterpolatedFast(const CPoint2D<float>& Pos) const
{
	float x=Pos.GetX();
	float y=Pos.GetY();
	// upper left pixel (l ~ low)
	int xl = (x >=0 ? (int)x : (int)(x-1)); /* always round down */
	int yl = (y >=0 ? (int)y : (int)(y-1)); /* always round down */
	// lower right pixel (h ~ high)
	int xh = xl + 1;
	int yh = yl + 1;
	// do bilinear interpolation
	UINT32 UL=GetPixelFast(xl,yl); // Upper left pixel
	UINT32 UR=GetPixelFast(xh,yl); // Upper right pixel
	UINT32 LL=GetPixelFast(xl,yh); // Lower left pixel
	UINT32 LR=GetPixelFast(xh,yh); // Lower right pixel
	// Adding 0.5 before converting to UINT32 rounds the result to the right value!
	UINT32 RVal=(UINT32)((k_PalGetRVal(UL)*(xh-x) + k_PalGetRVal(UR)*(x-xl))*(yh-y) +
					(k_PalGetRVal(LL)*(xh-x) + k_PalGetRVal(LR)*(x-xl))*(y-yl)+0.5);
	UINT32 GVal=(UINT32)((k_PalGetGVal(UL)*(xh-x) + k_PalGetGVal(UR)*(x-xl))*(yh-y) +
					(k_PalGetGVal(LL)*(xh-x) + k_PalGetGVal(LR)*(x-xl))*(y-yl)+0.5);
	UINT32 BVal=(UINT32)((k_PalGetBVal(UL)*(xh-x) + k_PalGetBVal(UR)*(x-xl))*(yh-y) +
					(k_PalGetBVal(LL)*(xh-x) + k_PalGetBVal(LR)*(x-xl))*(y-yl)+0.5);
	return k_PalCreateRGB(RVal,GVal,BVal);
}

inline UINT8 CStdImage::GetRedComponent(int x, int y) const
{
	return k_GetRedComponent(x,y,&m_Image);
}

inline UINT8 CStdImage::GetRedComponentFast(int x, int y) const
{
	return k_GetRedComponentFast(x,y,&m_Image);
}

inline UINT8 CStdImage::GetRedComponent(const CPoint2D<int>& Pos) const
{
	return k_GetRedComponent(Pos.GetX(),Pos.GetY(),&m_Image);
}

inline UINT8 CStdImage::GetRedComponentFast(const CPoint2D<int>& Pos) const
{
	return k_GetRedComponentFast(Pos.GetX(),Pos.GetY(),&m_Image);
}

inline UINT8 CStdImage::GetGreenComponent(int x, int y) const
{
	return k_GetGreenComponent(x,y,&m_Image);
}

inline UINT8 CStdImage::GetGreenComponentFast(int x, int y) const
{
	return k_GetGreenComponentFast(x,y,&m_Image);
}

inline UINT8 CStdImage::GetGreenComponent(const CPoint2D<int>& Pos) const
{
	return k_GetGreenComponent(Pos.GetX(),Pos.GetY(),&m_Image);
}

inline UINT8 CStdImage::GetGreenComponentFast(const CPoint2D<int>& Pos) const
{
	return k_GetGreenComponentFast(Pos.GetX(),Pos.GetY(),&m_Image);
}

inline UINT8 CStdImage::GetBlueComponent(int x, int y) const
{
	return k_GetBlueComponent(x,y,&m_Image);
}

inline UINT8 CStdImage::GetBlueComponentFast(int x, int y) const
{
	return k_GetBlueComponentFast(x,y,&m_Image);
}

inline UINT8 CStdImage::GetBlueComponent(const CPoint2D<int>& Pos) const
{
	return k_GetBlueComponent(Pos.GetX(),Pos.GetY(),&m_Image);
}

inline UINT8 CStdImage::GetBlueComponentFast(const CPoint2D<int>& Pos) const
{
	return k_GetBlueComponentFast(Pos.GetX(),Pos.GetY(),&m_Image);
}

inline UINT32 CStdImage::GetWidth() const
{
	// get width of image
	return m_Image.Width;
}

inline UINT32 CStdImage::GetHeight() const
{
	// get height of image
	return m_Image.Height;
}

inline int CStdImage::GetMinX() const
{
	if (ROIActive()==true)
		return m_ROIUpperLeft.GetX();
	else
		return -m_Image.Origo.x;
}

inline int CStdImage::GetMinY() const
{
	if (ROIActive()==true)
		return m_ROIUpperLeft.GetY();
	else
		return -m_Image.Origo.y;
}

inline int CStdImage::GetMaxX() const
{
	if (ROIActive()==true)
		return m_ROILowerRight.GetX();
	else
		return m_Image.Width-m_Image.Origo.x;
}

inline int CStdImage::GetMaxY() const
{
	if (ROIActive()==true)
		return m_ROILowerRight.GetY();
	else
		return m_Image.Height-m_Image.Origo.y;
}


inline UINT16 CStdImage::GetBorderSize() const
{
	// get size of border
	return m_Image.BorderSize;
}

inline CPoint2D<int> CStdImage::GetOrigo() const
{
	return CPoint2D<int>(m_Image.Origo.x,m_Image.Origo.y);
}

inline UINT16 CStdImage::GetBits() const
{
	// get bits per pixel
	return m_Image.Bits;
}

inline ORIGIN CStdImage::GetOrigin() const
{
	// get the origin of image
	return m_Image.Origin;
}

/*@}*/ /* end group "containerclasses" */

} // end namespace ipl

#endif //_IPL98_STDIMAGE_H
