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

#ifndef _IPL98_HOUGHRHTLINE_H
#define _IPL98_HOUGHRHTLINE_H

#include "../../../ipl98_setup.h" /* always include the setup file */
#include "hough_base.h"
#include "hough_space2d.h"
#include "sparse_hough_space2d.h"

namespace ipl {

/** @ingroup algorithms
@{ */

/** CHoughRHTLine class.
	Example for using this class:
	Files used: <a href="../example_images/mellem-hough-test.zip">mellem-hough-test.zip</a>
	\verbatim

	//
	// Test of Random hough transform
	//

	// An image containing circles is loaded
	z1.Load("mellem-hough-test.bmp");
	CImage output;

	// A CHoughRHTLine object is created and initialized.
	CHoughRHTLine RHTHough(cout, true);
	RHTHough.Initialize(z1, HIGHCOLOR,130);

	// The Hough Circle transform is performed on the picture.
	RHTHough.PerformTransform(10,30);

	// An image of the peaks in the houghspace are put into 
	// a CImage object, and written to disc
	RHTHough.ToImage(output);
	output.Save("houghoutput.bmp");
	\endverbatim
    Output from above code is:
	\verbatim

	Quantizations are: (556,355)
	Step values are  : (0.00565035,1.99467)
	Theta value range: (-0.0113007,3.13029)
	Rho value range  : (-353.553,354.553)
	Computing sine and cosine value vectors...
	Randomized Hough transform started.
	Distance criterium (min): 2
	Distance criterium (max): 354.553
	Detection threshold:      10
	Parameter 1: (1.6273,187.499)
	DIST: 500
	Parameter 2: (2.18103,-91.7547)
	DIST: 545
	Parameter 3: (0.796699,55.8507)
	DIST: 594
	Parameter 4: (2.48615,59.84)
	DIST: 599
	Parameter 5: (0.745846,-121.675)
	DIST: 464
	Parameter 6: (0.48593,297.205)
	DIST: 98
	\endverbatim
	The example writes the file 'rht__RC_RHT_1.bmp'...'rht__RC_RHTC_6.bmp' containing the found
	lines after each internal iteration. Files 'rht__ORIGINAL_RHT_1.bmp'...'rht__ORIGINAL_RHT_7.bmp'
	show the remaining data after each iteration. File 'houghoutput.bmp' show the detected peaks
	in hough space.
	The example writes file 'result.bmp' in current directory, it contains an image with
    Calculates Hough transform of an image with standard randomized technique.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CHoughRHTLine ipl98/cpp/algorithms/hough_transform/hough_rht_line.h
    @author Qinyin Zhou <qinyin@mip.sdu.dk>
    @version 0.90
*/
class CHoughRHTLine {
public:
	/** Default constructor. */
   CHoughRHTLine();

   /** Constructor for the class. 
       @param os Output stream.
       @param WriteOutputFiles Option to set if write output files.
       @param FilePrefix Prefix of the output files.
    */
   CHoughRHTLine(ostream& os, bool WriteOutputFiles, const string& FilePrefix="rht_");

   /** Destructor for the class.
   */
	~CHoughRHTLine();

   /** Initialization function for the class.
       @param BinarySource Input image as a binary image object.
       @param Thickness Thickness of the lines of the source image. 1 by default.
       @param SpaceType Type of parameter space to be used. Full or sparse. Sparse as default.
       @param ParamType Type of parameterization. Normal by default.
   */
   bool Initialize(CHoughBase::CBinaryImage& BinarySource, int Thickness=1,
                   CHoughBase::PARAMETERSPACETYPE SpaceType=CHoughBase::SPARSE_SPACE, 
				       CHoughBase::PARAMETERTYPE ParamType=CHoughBase::NORMAL_PARAM);

   /** Initialization function for the class. A common method is called.
       @param Source Source image as 1/8 bit. Conversion into binary image object.
       @param Color Color type of the source image.
       @param Threshold Image threshold value.
       @param OriginType Image origin type. Origo at center with y-axis upwards by default.
       @param Thickness Thickness of the lines of the source image. 1 by default.
       @param SpaceType Type of parameter space to be used. Full or sparse. Sparse as default.
       @param ParamType Type of parameterization. Normal by default.
   */
   bool Initialize(const CStdImage& Source, COLORTYPE Color, UINT8 Threshold, 
                   CHoughBase::ORIGINTYPE OriginType=CHoughBase::CENTER_UP, int Thickness=1, 
                   CHoughBase::PARAMETERSPACETYPE SpaceType=CHoughBase::SPARSE_SPACE, 
                   CHoughBase::PARAMETERTYPE ParamType=CHoughBase::NORMAL_PARAM);

   /** ToImage Output the associated Hough space into an image object.
       @param outImg Output image written in this object.
   */
   void ToImage(CStdImage& outImg);

   /** Method to run an entire transformation and peak detection after initialization.
       @param HSThreshold Threshold value for peak detection.
       @param MaxNumShapes User predicted number of shapes appeared in the source image.
	   @return Found peaks in object m_peaks. Empty vector if error.
   */
   vector<CParameter2D<float> > PerformTransform(int HSThreshold, int MaxNumShapes);

private:
   /** m_bImg The associated binary image object to randomized Hough transform. */
   CHoughBase::CBinaryImage m_binaryImage;

   /** m_p1 1st sampled point. */
   CPoint2D<int> m_p1;

   /** 2nd sampled point. */
   CPoint2D<int> m_p2;

   /** All detected peaks are stored in the vector. */
   vector<CPoint2D<int> > m_peaks;

   /** A full size Hough space to store scores. */
   CHoughSpace2D* m_houghSpace2D;

   /** Minimum distance criterium for sampling. */
   float m_distanceCriteriumMin;

   /** Maximum distance criterium for sampling. */
   float m_distanceCriteriumMax;

   /** Threshold value to determine detection of a peak. */
   int m_threshold;

   /** Output stream set be constructor. */
   ostream& m_os;

   /** Output files prefix. */
   string m_filePrefix;

   /** Flag to set if write files to disk. */
   bool m_writeOutputFiles;

   /** Flag to check whether the object has been legally initialized. */
   bool m_initialized;

   /** Sin(theta) lookup table. */
   vector<float> m_sinTheta;

   /** Lookup table for cos(theta) with theta ranging [0,pi]. */
   vector<float> m_cosTheta;

   /** Gets two random pixels from the binary image. */
   void GetPixelPair();

   /** Calsulates distance between two random pixels. 
       The distance is used for dist criterium.
       @return Distance between two pixels in float.
   */
   float Distance();

   /** Check the distance between the random point pair satisfies the distance criteria.
       @return True if check OK. Else false.
   */
   bool DistanceCheck();

   /** Compute quantisized values of sine and cosine, maybe little speed optimization.
   */
   void ComputeSinCosTheta(int n);

   /** Solve the parameter space point (a,b) using the curve equation with the random points.
       The points are stored in m_p1 and m_p2.
       The corresponding cell (theta,rho) in the parameter space will be updated.
       @return Point (a,b) in the parameter space to be updated.
   */
   bool ComputeRandomizedHough();

   /** A routine to perform disk output.
       @param BImage Image image source for image output.
       @param Coordinate The parameter of the shape.
       @param NumOfShape The actual number of shapes (number of files),
   */
   void WriteFiles(CHoughBase::CBinaryImage& BImage, const CParameter2D<float>& Coordinate, int NumOfShapes);

   /** Common initialization routine for both initialization functions. 
       Called after the source image has been initialized.
       @param Thickness Thickness of the lines of the source image.
       @oaram SpaceType Type of parameter space to be used. Full or sparse. Sparse as default.
       @param ParamType Type of parameterization. Normal parameterization by default.
   */
   bool InitializeCommon(int Thickness=1, CHoughBase::PARAMETERSPACETYPE SpaceType=CHoughBase::SPARSE_SPACE, 
							CHoughBase::PARAMETERTYPE ParamType=CHoughBase::NORMAL_PARAM);

};

/*@}*/ /* end group "algorithms" */

} // end namespace ipl

#endif //_IPL98_HOUGHRHTLINE_H
