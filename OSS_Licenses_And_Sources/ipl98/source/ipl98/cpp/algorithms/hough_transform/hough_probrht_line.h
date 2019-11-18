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

#ifndef _IPL98_PROBRHT_H
#define _IPL98_PROBRHT_H

#include "../../../ipl98_setup.h" /* always include the setup file */
#include "hough_base.h"
#include "hough_rht_line.h"

namespace ipl {

/** @ingroup algorithms
@{ */

/** Probabilistic Randomized Hough Transform. 
    Randomly selects a subset of the source image and performs RHT on it.
    As a rule of thumb, 30-50% of total pixels are enough to recreate the shapes in an image.
    The reduction in the number of pixels in CBinaryImage object will speedup the detection process significantly.
	Example for using this class:
	Files used: <a href="../example_images/mellem-hough-test.zip">mellem-hough-test.zip</a>
	\verbatim

	//
	// Test of Probabilistic Random hough transform
	//

	// An image containing circles is loaded
	z1.Load("mellem-hough-test.bmp");

	// A CHoughProbRHTLine object is created and initialized.
	float ratio = 0.1;
	CHoughProbRHTLine ProbRHTHough(cout, true);
	ProbRHTHough.Initialize(ratio, z1, HIGHCOLOR,130);

	// The Hough Circle transform is performed on the picture.
	vector<CParameter2D<float> > Points;
	Points=ProbRHTHough.PerformTransform(10,20);

	cout << endl << "Found points: " << endl;
	for(count=0;count < Points.size(); count++)
	{
		cout << Points[count] << " ";
	}
	cout << endl;
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
	######Parameter 1: (2.48615,59.84)
	DIST: 599
	#Parameter 2: (2.18103,-91.7547)
	DIST: 545
	#Parameter 3: (1.6273,187.499)
	DIST: 500
	Parameter 4: (0.745846,-121.675)
	DIST: 464

	Found points:
	(2.48615,59.84) (2.18103,-91.7547) (1.6273,187.499) (0.745846,-121.675)
	\endverbatim
	The example writes the file '_RC_RHTC_1.bmp'...'_RC_RHTC_7.bmp' containing the found
	lines after each internal iteration. Files '_ORIGINAL_RHTC_1.bmp'...'_ORIGINAL_RHTC_7.bmp'
	show the remaining data after each iteration.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CHoughProbRHTLine ipl98/cpp/algorithms/hough_transform/hough_probrht_line.h
    @author Qinyin Zhou <qinyin@mip.sdu.dk>
    @version 0.90
*/
class CHoughProbRHTLine {
public:
	/** Default constructor. 
	*/
	CHoughProbRHTLine();

	/** Constructor of the clss.
		@param os Program information output stream.
		@param WriteOutputFiles If to make disk output. Set to false (no output) by default.
		@param FilePrefix Prefix of files for output.
	*/
	CHoughProbRHTLine(ostream& os, bool WriteOutputFiles=false, const string& FilePrefix=string(""));

	/** Destructor.  */
	~CHoughProbRHTLine();

   /** Initializing members of the object.
       @param Ratio Point selection rate. 0.0f-1.0f. A ration between 0.3f and 0.5f is preciated.
       @param Source A constructed image source of type CBinaryImage. Selection will take place
                     among the information provided by it. When finished, the object is contains a
                     fraction of the original pixels.
       @param Thickness Thickness of the shapes in the original image. Set to 1 by default.
       @param SpaceType Type of the parameter space. Set to SPARSE_SPACE by default.
       @param ParamType Type of the parameter to describe the shape. By deault, normal parameterization.
   */
   bool Initialize(float Ratio, CHoughBase::CBinaryImage& Source, int Thickness=1,
					CHoughBase::PARAMETERSPACETYPE SpaceType=CHoughBase::SPARSE_SPACE, 
					CHoughBase::PARAMETERTYPE ParamType=CHoughBase::NORMAL_PARAM);

   /** Initializing members of the object.
       @param Ratio Point selection rate. 0.0f-1.0f. A ration between 0.3f and 0.5f is preciated.
       @param Source Original image source of type CStdImage. A binary image will be created and 
                     the selection is performed for the binary image object.
       @param Color Color type of the source image.
       @param Threshold Image threshold.
       @param OriginType Type of origin of the source image. By default set to center with y-axis upwards.
       @param Thickness Thickness of the shapes in the original image. Set to 1 by default.
       @param SpaceType Type of the parameter space. Set to SPARSE_SPACE by default.
       @param ParamType Type of the parameter to describe the shape. By deault, normal parameterization.
   */
   bool Initialize(float Ratio, const CStdImage& Source, COLORTYPE Color, UINT8 Threshold, 
					CHoughBase::ORIGINTYPE OriginType=CHoughBase::CENTER_UP, int Thickness=1, 
					CHoughBase::PARAMETERSPACETYPE SpaceType=CHoughBase::SPARSE_SPACE, 
					CHoughBase::PARAMETERTYPE ParamType=CHoughBase::NORMAL_PARAM);
   
   /** Performs transformation after all initialized.
       @param Threshold Cell count threshold.
       @param MaxNumShapes A estimate of number of shapes expected to be found.
	   @return Found peaks stored in a vector. Empty returned is error.
   */
   vector<CParameter2D<float> > PerformTransform(int Threshold, int MaxNumShapes);

private:

   /** Set by constructor, if true hough image files are written. */
   bool m_writeOutputFiles;

   /** Set to true when Initialize() has been successfully called. */
   bool m_initialized;

   /** Set by constructor, prefix for image files written by Hough algorithm. */
   string m_filePrefix;

   /** Set by constructor, all information from algorithms are written to this stream. */
   ostream& m_os;

   /** A object used when transformation is performed. Used RHT. */
   CHoughRHTLine m_rht_line;

   /** Performs selected of random pixels of the source image.
       @param Ratio Selecteion ratio.
       @param Source Binary image source.
   */    
   void PerformSelection(float Ratio, CHoughBase::CBinaryImage& Source);

};

/*@}*/ /* end group "algorithms" */

} // end namespace ipl

#endif //_IPL98_PROBRHT_H
