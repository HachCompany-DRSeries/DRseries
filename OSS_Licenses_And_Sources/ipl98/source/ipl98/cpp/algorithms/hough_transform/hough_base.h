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

#ifndef _IPL98_HOUGHBASE_H
#define _IPL98_HOUGHBASE_H

#include "parameter2d.h"
#include "../../std_image.h"
#include "../../image.h"
#include "../../../../points/point3d.h"
#include <vector>

namespace ipl {

using std::vector;

/** @ingroup algorithms
@{ */

/** CHoughBase class
    Base class for all hough transform classes. All hough transform classes are derived from it.
    This class provides types and methods utilized in the different transformation techniques.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CHoughBase ipl98/cpp/algorithms/hough_transform/hough_base.h
    @author Qinyin Zhou <qinyin@mip.sdu.dk>
    @version 0.99 */
class CHoughBase {
public:

	/** The PARAMETERTYPE defines which type of parameterization method should be used to describe a 
       line shape. Used by SHT, RHT, ProbHT, and ProbRHT. Normally set to NORMAL_PARAM.
       Slope intercept parametrization (y=ax+b, with singularity problem) or 
       normal parametrization (r=x cos(th)+y sin(th)) can be selected. */
	typedef enum {
		SLOPE_PARAM,         // slope intercept parametrization
		NORMAL_PARAM         // normal parametrization
	} PARAMETERTYPE;
	
	/** The ORIGINTYPE defines the location and orientation of the coordinate system of the source image.
       The best case is CENTER_UP where the origo is located at the center of the image with y-axis pointing upwards. 
       This feature compilicates the algorithm with axis conversion but makes the algorithm 
       mathematically more easy to understand. */
	typedef enum { 
		LEFT_UPPER, 
		CENTER_UP,
		CENTER_DOWN
	} ORIGINTYPE;
	
	/** The PEAKDETECTIONTYPE defines with strategy to use when finding a peak in a parameter space. 
       Need to choose when performing SHT or ProbSHT.
       The BUTTERFLY method is the most approved method to take. */
	typedef enum {
		BUTTERFLY,
		DIFFUSION,
		ITERATION,
		LOCALMAXIMA
	} PEAKDETECTIONTYPE;
	
	/** The PARAMETERSPACETYPE defines which of the two parameter space implementation should be utilized 
       when performing RHT and ProbRHT. QUANTIZATION_SPACE uses a full nxn space to collect the votes.
       SPARCE_SPACE used a 1D-array-list structure to store un-zero cells and saves space. */
	typedef enum {
		QUANTIZATION_SPACE,
		SPARSE_SPACE
	} PARAMETERSPACETYPE;


	/** CBinaryImage class.
		This class presents binary image objects. Taken a full-size image, it is converted into a new object
		where coordinates for pixels with values != 0 stored in a 2D vector, where the pixel coordinates are stored.
		A better data structure can be implemented to gain better storage saving.
		@author Qinyin Zhou <qinyin@mip.sdu.dk>
		@version 0.99
	*/
	class CBinaryImage {
	public:

	   /** Default constructor. 
	   */
	   CBinaryImage();
   
	   /** Constructor for CBinaryImage class.
		   @param w Width of the image.
		   @param h Height of the image.
		   @param OriginType Type of the image origin. 
							 Can be set to center or corner. Center with y-axis upwards by default.
	   */
	   CBinaryImage(int w, int h, ORIGINTYPE OriginType=CENTER_UP);

	   /** Constructor for CBinaryImage.
		   Constructs a binary image object from a normal image.
		   @param Source Input full-size image object.
		   @param Color Type of the color of the image.
		   @param Threshold Threshold value to determine pixel or not pixel.
		   @param OriginType Type of the origin of the image. Center with y-axis upwards by default.
	   */
	   CBinaryImage(const CStdImage& Source, COLORTYPE Color, UINT8 Threshold, 
						ORIGINTYPE OriginType=CENTER_UP);

	   /** Destructor of the class.
	   */
		~CBinaryImage();

	   /** ToImage Write a binary image into a full-size image of CStdImage type.
		   @param img Image object to be written into.
	   */
	   void ToImage(CStdImage& img);

	   /** Get the width of the binary image.
		   @return Width of the image object.
	   */
	   int GetWidth() const;

	   /** Get the height of the binary image.
		   @return Height of the image object.
	   */
	   int GetHeight() const;

	   /** Get the total number of pixels in the image.
		   @return Gets the size (size<=w*h) of the image.
	   */
	   int GetSize() const;

	   /** The pixels are stored in a vector. This method is appied when sampling a pixel.
		   @param i The index of one of the elements of the vector.
		   @return Gets a pixel object at index i.
	   */
	   CPoint2D<int> GetPixel(int i) const;

	   /** Removes a specific pixel from the image. 
		   This method is called when removing shapes from the binary image object. 
		   To spped to the process the pixel at i is shifted with the last alive pixel, and the total
		   number of pixels is decremented by 1.
		   @param i Location of the pixel to be removed.
	   */
	   void RemovePixel(int i);

	   /** Find all pixels with the same value as p in the binary image. 
		   If found, the pixels are removed from the image.
		   @param p Pixel (coordinate of the pixel) to be removed.
	   */
	   bool RemovePixel(const CParameter2D<int>& p);

	   /** Find the first pixel with the same value as p in the binary image. 
		   If found, the pixel is removed from the image.
		   @param p Pixel (coordinate of the pixel) to be removed.
	   */
	   bool RemovePixelFast(const CParameter2D<int>& p);

	   /** By iteratively calling RemovePixel to remove a group of pixels. 
		   The surrounding group is always 4-connected with the center.
		   @param p Coordinate of the center pixel.
		   @param tol Radius of the group in pixels.
	   */
	   bool RemovePixelRange(CParameter2D<int>& p, int tol);

	   /** Remove all the pixels lying on the line described by p in the image.
		   1st attempt, remove connected component of each pixel within a range.
		   @param p Parameter describing the line.
		   @param Thickness Region to be removed.
	   */
	   void RemoveLine1(const CParameter2D<float>& p, int Thickness);

	   /** Remove all the pixels lying on the line described by p in the image.
		   2nd attempt, remove by vary line equation.
		   @param p Parameter describing the line.
		   @param Thickness Region to be removed.
	   */
	   void RemoveLine2(const CParameter2D<float>& p, int Thickness);

	   /** RemoveCircle remove all points spanning a circle within a range of 1 in thickness from the image. 
		   @param p Parameter of a circle.
	   */
	   void RemoveCircle(const CPoint3D<float>& p);

	   /** Remove a circle from the image.
		   @param p Parameter of the circle. p=(x,y,r).
		   @param i Width of the circle to be removed.
	   */
	   void RemoveCircleRange(const CPoint3D<float>& p, int i);

	   /** Adds a pixel to the image. The image is checked for redundancy.
		   The new pixel is appended at the end of the pixel vector.
		   @param pixel New pixel for addition. pixel=(x,y).
	   */
	   void AddPixel(const CPoint2D<int>& pixel);

	   /** Adds a pixel to the image. No redundancy check.    
	   */
	   void AddPixelFast(const CPoint2D<int>& pixel);

	   /** Adds a line of 1 in thickness to the image.
		   @param p Parameter of the line. Normal parameter (p=(rho,theta)) only.
	   */
	   void AddLine(const CParameter2D<float>& p);

	   /** Adds a circle of 1 in thickness to the image.
		   @param p Parameter of the circle. p=(x,y,r).
	   */
	   void AddCircle(const CPoint3D<float>& p);

	   /** Computes intersection points of a line with the frame of the image.
		   @param p Line parameter.
		   @return Intersection points. vector[0]=x1, vector[1]=y1, vector[2]=x2, vector[3]=y2.
	   */
	   vector<int> ComputeIntersectionPoints(const CParameter2D<float>& p);

	   /** Conversion of a pixel value to real image pixel value.
		   @param pixel Pixel in the actual coordinate system.
		   @return Pixel in the real image coordinate system.
	   */
	   CPoint2D<int> OriginConvert(const CPoint2D<int>& pixel);

	   /** Gets the type of image origin.
	   */
	   ORIGINTYPE GetOrigin() const;

	private:
	   /** Width of the image object. */
	   int m_width;

	   /** Height of the image object. */
	   int m_height;

	   /** Type of the image origin. */
	   ORIGINTYPE m_origin;

	   /** Storing x coordinates. img[m_xCoord[i]][m_yCoord[i]] = 1 forall i. */
	   vector<CPoint2D<int> > m_coord;

	   /** Translational vector of the coordinate system. */
	   CPoint2D<int> m_originPosition;

	   /** Orientation vector of the coordinate system. */
	   CPoint2D<int> m_originOrientation;

	   /** Sets the image coordinate system as required.
	   */
	   void SetOrigin(ORIGINTYPE OriginType=CENTER_UP);
	};	
	
	
	
	
	/** Scale a Hough space converted image to a grayscale image. 
       The pixels in the image are scaled by pi/max(pi) ratio.
		 @param Source Input and output image.
	*/
	static void ScaleHoughImage(CStdImage& Source);
	
	/** Draws a line described by normal parameterization in an image.
		@param img Image object for addition of a new shape.
		@param Parameter of the line object.
	*/
	static void DrawLine(CImage& img, const CParameter2D<float>& Parameter);
	
	/** Computer intersection points of a line with the perimeter (normal) of the image.
		@param img Image object.
		@param p Parameter of the line.
	*/
	static vector<int> ComputeIntersectionPoints(const CImage& img, const CParameter2D<float>& p);
	
};

/*@}*/ /* end group "algorithms" */

} // end namespace ipl

#endif //_IPL98_HOUGHBASE_H
