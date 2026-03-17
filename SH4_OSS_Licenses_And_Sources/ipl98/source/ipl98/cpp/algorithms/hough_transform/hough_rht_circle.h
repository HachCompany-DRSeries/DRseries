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

#ifndef _IPL98_RHTCIRCLE_H
#define _IPL98_RHTCIRCLE_H

#include "../../../ipl98_setup.h" /* always include the setup file */
#include "hough_base.h"
#include "sparse_hough_space3d.h"

namespace ipl {

/** @ingroup algorithms
@{ */

/** Calculates Hough transform of an image with standard randomized technique.
	Example for using this class:
	Files used: <a href="../example_images/cirkelhoughtest.zip">cirkelhoughtest.zip</a>
	\verbatim

	//
	// Test of hough transform for circles
	//

	// An image containing circles is loaded
	z1.Load("cirkelhoughtest.bmp");

	// A CHoughRHTCircle object is created and initialized.
	CHoughRHTCircle HoughCircle(cout, true);
	HoughCircle.Initialize(z1,HIGHCOLOR,130);

	// A vector to contain the found parameters is constructed.
	vector<CPoint3D<int> > circleResult;
		
	// The Hough Circle transform is performed on the picture.
	circleResult = HoughCircle.PerformTransform(100);

	\endverbatim
    Output from above code is:
	\verbatim

	Circle 1: (-32,87,159)
	Circle 2: (61,-98,153)
	Circle 3: (-46,130,97)
	Circle 4: (-174,-66,108)
	Circle 5: (-98,-27,79)
	Circle 6: (179,145,79)
	Circle 7: (117,46,57)
	\endverbatim
	The example writes the file '_RC_RHTC_1.bmp'...'_RC_RHTC_7.bmp' containing the found
	circles after each internal iteration. Files '_ORIGINAL_RHTC_1.bmp'...'_ORIGINAL_RHTC_7.bmp'
	show the remaining data after each iteration.
	\class CHoughRHTCircle ipl98/cpp/algorithms/hough_transform/hough_rht_circle.h
    @author Qinyin Zhou <qinyin@mip.sdu.dk>
    @version 0.90
*/
class CHoughRHTCircle {
public:
   /** Default constructor. Performs RHT on circle image.
   */
	CHoughRHTCircle();

   /** Constructor.
       @param os Algorithm information output stream.
       @param WriteOutputFiles Option for disk output. No output by default.
       @param FilePrefix File name prefix if output.
   */
   CHoughRHTCircle(ostream& os, bool WriteOutputFiles=false, const string& FilePrefix="");

   /** Destructor.
   */
   ~CHoughRHTCircle();

   /** Initialize function when the image is of IPL image type. 
       A binary image object (CBinaryImage) will be constructed for transformation purpose.
       @param Source Image source object.
       @param Color Type of the source image.
       @param Threshold Image pixel threshold.
       @param OriginType Type of origin to be applied to the binary image. By default is set to center of 
                         the image with y-axis poitning upwards.
   */
   bool Initialize(const CStdImage& Source, COLORTYPE Color, UINT8 Threshold, 
					CHoughBase::ORIGINTYPE OriginType=CHoughBase::CENTER_UP);

   /** Initialise function when the source image is a binary image object.
       @param Source The source binary image object.
       @param OriginType Type of origin to be applied to the binary image. By default is set to center of 
                         the image with y-axis poitning upwards.
   */
   bool Initialize(const CHoughBase::CBinaryImage& Source, CHoughBase::ORIGINTYPE OriginType=CHoughBase::CENTER_UP);

   /** Master transformation and detection method. 
       This method performs CHoughRHTCircle::RH_Circle_Transform() iteratively and stops when no more shapes
       can be founded from the image left.
       Returns a vector of parameters of found shapes where counts has exceeded a value.
       @param Threshold Peak detection threshold. When a cell has exceeded this value, a peak has been found.
       @return A vector object containing 3D parameters.
   */
   vector<CPoint3D<int> > PerformTransform(int Threshold);

private:
   /** A 2D-array-list structure to store accumulator cells. */
   CSparseHoughSpace3D m_space;

   /** A binary image object for sampling. When taken a CImage object in, it is converted to CBinarImage. */
   CHoughBase::CBinaryImage m_bImage;

   /** 1st sampling point. */
   CPoint2D<int> m_p1;

   /** 2nd sampling point. */
   CPoint2D<int> m_p2;

   /** 3rd sampling point. */
   CPoint2D<int> m_p3;

   /** Storing results of peak detections. */
   vector<CPoint3D<int> > m_peaks;

   /** Algorithm information output stream. */
   ostream& m_os;

   /** File prefix of output files. */
   string m_filePrefix;

   /** Option set by constructor for output files or not. */
   bool m_writeOutputFiles;

   /** Performs random sampling of three points m_p1, m_p2, and m_p3 from the binary image.
   */
   void GetPixelTriple();

   /** Performs distance check to ensure distance criteria.
   */
   bool DistanceCheck();

   /** Performs transform of a selected point set. At each counting step, three pixels from 
       the image are sampled to compute a circle parameter set.
       @return The solution.
   */
   CPoint3D<float> RH_Circle_Transform();

   /** Checks if the solution is legal before accumulation to the space.
       @param Parameter Solution of RHT_Circle_Transform.
   */
   bool ParameterCheck(const CPoint3D<int>& Parameter);

   /** Routine to perform disk output.
       @param outImage1 Image object where the reconstruction images will be written into.
       @param Parameter A collection of parameters for detected shapes.
       @param NumOfShapes Number of shapes (or files) to be written.
   */
   void WriteFiles(CStdImage& outImage1, const CPoint3D<float>& Parameter, int NumOfShapes);

   /** Common initialization routine called by both Init functions. */
   bool InitializeCommon();

};

/*@}*/ /* end group "algorithms" */

} // end namespace ipl

#endif //_IPL98_RHTCIRCLE_H
