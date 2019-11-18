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

#ifndef _IPL98_HOUGHSHTLINE_H
#define _IPL98_HOUGHSHTLINE_H

#include "../../../ipl98_setup.h" /* always include the setup file */
#include "hough_base.h"
#include "hough_space2d.h"

namespace ipl {

/** @ingroup algorithms
@{ */

/** This class implements the standard Hough transformation method for line detection.
	Example for using this class:
	Files used: <a href="../example_images/mellem-hough-test.zip">mellem-hough-test.zip</a>
	\verbatim

	z1.Load("mellem-hough-test.bmp");

	// A new CHoughSHTLine object is created and initialized
	CHoughSHTLine Hough(cout,false);
	Hough.Initialize(HIGHCOLOR,pictureThreshold);

	// Hough transformation is performed on the image 
	Hough.PerformTransform(z1,lineThickness);

	// The peaks are extracted from the hough space
	vector<CParameter2D<float> > Peaks;
	Peaks = Hough.FindPeaks(CHoughBase::BUTTERFLY,houghSpaceThreshold);

	// The original image is reconstructed by drawing the found linies
	z2.Alloc(z1.GetWidth(),z1.GetHeight(),8,255);
	for(int x=0;x<Peaks.size();x++)
	{
	CHoughBase::DrawLine(z2, Peaks[x]);
	}

	// The reconstruction is saved to disk
	z2.Save("result.bmp");

	// The a image of the hough space is saved to disk
	Hough.ToImage(z3);
	z3.Save("hspace.bmp");
	\endverbatim
    Output from above code is:
	\verbatim

	Quantizations are: (556,355)
	Step values are  : (0.00565035,1.99467)
	Theta value range: (-0.0113007,3.13029)
	Rho value range  : (-353.553,354.553)
	Computing sine and cosine value vectors...
	Performing standard Hough transform...
	Finished procentage:
	################################################################################
	###################
	Standard Hough transform for lines finished.
	\endverbatim
	The example writes file 'result.bmp' in current directory, it contains an image with
	the detected lines drawn in it. File 'hspace.bmp' contains the internal Hough-space.
	File 'butterfly.bmp' shows the detected peaks in the Hough-space.
    A standard Hough space is implemented, and different peak detection methods can be chosen.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CHoughSHTLine ipl98/cpp/algorithms/hough_transform/hough_sht_line.h
    @author Qinyin Zhou <qinyin@mip.sdu.dk>
    @version 0.90
*/
class CHoughSHTLine {
public:

   /** Constructor for CHoughSHTLine.
       @param os Output stream.
       @param WriteOutputFiles Option to whether or not write output files.
       @param Output Prefix of the output files, defaults to "stdhough".
    */
   CHoughSHTLine(ostream& os, bool WriteOutputFiles, const string& Output=string("stdhough"));

   /** Destructor.
   */
   ~CHoughSHTLine();

   /** Initializes CHoughSHTLine.
       @param Color Color type, low (black background) or high color (white background).
       @param Threshold Threshold for pixel input value. 
                        The value must be set after the image type (8-bit or 1-bit).
       @param OriginType Type of the Origin location. CENTER_UP or LEFT_UPPER, defaults to CENTER_UP.
       @param ParamType Type of parametrization, defaults to NORMAL_PARAM.
   */
	bool Initialize(COLORTYPE Color, UINT8 Threshold, CHoughBase::ORIGINTYPE OriginType=CHoughBase::CENTER_UP, 
						CHoughBase::PARAMETERTYPE ParamType=CHoughBase::NORMAL_PARAM);

   /** StandardHoughTransform Performs the standard Hough transform for the entire image.
       @param Source Source image, must be 1 or 8 b/p gray tone palette.
       @param Thickness Thickness of the lines appeared in the source image. The default thickness is 1.
   */
   bool PerformTransform(const CStdImage& Source, int Thickness=1);

   /** GetHoughSpace Return the corresponding Hough space,
   */
   CHoughSpace2D GetHoughSpace() const;

   /** Convert the Hough space into an image. The Hough space image will be scaled to 8-bit image.
       @param outImg The image will be located in this object.
   */
   void ToImage(CStdImage& outImg);

   /** Peak detection part of the algorithm.
       @param DetectionMethod User specified detection strategy. Type of peak detection method.
       @param HSThreshold Peak detection threshold value.
       @return All peak parameters are stored in a vector.
   */
   vector<CParameter2D<float> > FindPeaks(CHoughBase::PEAKDETECTIONTYPE DetectionMethod, unsigned int HSThreshold);

private:
	/** Size of image, set when calling StandardHoughTransform(). */
	CPoint2D<unsigned int> m_imageDimensions;

   /** Type of the source image, high color or low color. */
   COLORTYPE m_color;

   /** Threshold value for peak detection, depends on peak dection type. */
   UINT8 m_hsThreshold;

   /** Threshold value. Pixel values are dependent on color type. */
   UINT8 m_imgThreshold;

   /** Origin of the image, can be set to corner or center. */
   CPoint2D<int> m_imgOriginPosition;

   /** Origin orientation of the image, used to turn the direction of axis if negated. */
   CPoint2D<int> m_imgOriginOrientation;

   /** Lookup table for sin(theta) with theta ranging [0,pi]. */
   vector<float> m_sinTheta;

   /** Lookup table for cos(theta) with theta ranging [0,pi]. */
   vector<float> m_cosTheta;

   /** Lookup table for [a]_i, slope values. */
   vector<float> m_slopeVector;

   /** Two-dimensional Hough space for storing accumulator cells. */
   CHoughSpace2D m_houghSpace2D;

   /** Set by constructor, if true hough image files are written. */
   bool m_writeOutputFiles;

   /** Set to true when Initialize() has been successfully called. */
   bool m_initialized;

   /** Set by constructor, prefix for image files written by Hough algorithm. */
   string m_filePrefix;

   /** Set by constructor, all information from algorithms are written to this stream. */
   ostream& m_os;

   /** Set by Initialize(). */
   CHoughBase::ORIGINTYPE m_originType;

   /** Set by Initialize(). */
   CHoughBase::PARAMETERTYPE m_paramType;

   /** A help function which computes two vectors as lookup tabels, one for sine(theta) and 
       another one for cosine(theta) for descritized values of theta ranging [0,pi]. 
       With these two vectors, the main program avoids multiple trigonomitric computations of 
       the same theta value.
       @param n Number of diskretization.
   */
   void ComputeSinCosTheta(int n);

   /** Speedup function to compute slope values.
   */
   void ComputeSlope(int n);

   /** This function computes Hough transform for the (x,y)'th pixel of an image and
       updates its corresponding cell in the parameterspace. Slope intercept parametrization method is used.
       @param x x-coordinate of the pixel for transform.
       @param y y-coordinate of the pixel for transform.
   */
   void ComputeHoughSlope(int x, int y);

   /** This function computes Hough transform for the (x,y)'th pixel of an image and
       updates its corresponding cell in the parameterspace. Normal parametrization method is used.
       @param x x-coordinate of the pixel for transform.
       @param y y-coordinate of the pixel for transform.
   */
   void ComputeHoughNormal(int x, int y);

   /** @name SetOrigin Set the origin of the image object.
       @param OriginType Type of the Origin location and orientation. LEFT_UPPER or CENTER_UP.
   */
   void SetOrigin(CHoughBase::ORIGINTYPE OriginType);

   /** Updates m_imageDimensions and calls methods which need to be updated according to image dimensions. 
       @param Source Image source.
       @param Thickness Thickness of the lines in the image.
   */
   void UpdateHoughSpace(const CStdImage& Source, int Thickness);
};

/*@}*/ /* end group "algorithms" */

} // end namespace ipl

#endif //_IPL98_HOUGHSHTLINE_H
