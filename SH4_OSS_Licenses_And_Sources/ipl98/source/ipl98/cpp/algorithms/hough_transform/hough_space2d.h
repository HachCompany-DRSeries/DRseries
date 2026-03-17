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

#ifndef _IPL98_HOUGHSPACE2D_H
#define _IPL98_HOUGHSPACE2D_H

#include "../../../ipl98_setup.h" /* always include the setup file */
#include "../../arrays/array2d.h"
#include "hough_base.h"
#include "parameter2d.h"
#include <ostream>

namespace ipl {

using std::ostream;

/** @ingroup algorithms
@{ */

/** Full-size two dimensional Hough space for line detection of image.
    Stores accumulator cells.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CHoughSpace2D ipl98/cpp/algorithms/hough_transform/hough_space2d.h
	@author Qinyin Zhou <qinyin@mip.sdu.dk>
	@see CHoughSHTLine
    @version 0.99
*/
class CHoughSpace2D : public CArray2D<unsigned int> {
public:
   /**  Default constructor.
   */
   CHoughSpace2D();

   /** Constructor for a mini space.
       @param Width Width of the space.
       @param Height Height of the space.
   */
   CHoughSpace2D(int Width, int Height);

   /** Constructor for CHoughSpace2D specified for an image and type of parametrization.
       Creates a two dimensional Hough space object.
       @param os Output stream for algorithm information.
       @param ImageWidth Width of the source image object.
       @param ImageHeight Height of the source image object.
       @param Thickness Thickness of the lines of the source image.
       @param Threshold Detection threshold.
       @param ParamType Type of parametrization, slope intercept or normal parametrization. Normal as default.
   */
	CHoughSpace2D(ostream& os, int ImageWidth, int ImageHeight, int Thickness, 
                 int Threshold, CHoughBase::PARAMETERTYPE ParamType=CHoughBase::NORMAL_PARAM);

   /** Destructor of the class.
   */
	virtual ~CHoughSpace2D();

   /** Initializes CHoughSpace2D specified for an image and type of parametrization..
       Creates a two dimensional Hough space object.
       @param ImageWidth Width of the source image object.
       @param ImageHeight Height of the source image object.
       @param Thickness Thickness of the lines of the source image.
       @param Threshold Detection threshold.
       @param ParamType Type of parametrization, slope intercept or normal parametrization. Normal as default.
   */
	virtual bool Initialize(int ImageWidth, int ImageHeight, int Thickness, int Threshold, 
                           CHoughBase::PARAMETERTYPE ParamType=CHoughBase::NORMAL_PARAM);

   /** Makes the final allocation of the parameter space, when all parameters are set.
   */
   virtual bool Alloc();

   /** Convert the parameter space to a image. The space image will be scaled to 8-bit image.
       @param outImg Image object to be written into.
   */
   virtual void ToImage(CStdImage& outImg);

   /** Returns the values of space quantisized.
       @param i Coordinate of the parameter space, i=1 for the 1st parameter, i=2 for the second.
   */
   int GetQuantizationValue(int i) const;

   /** Returns the offset values of the parameters. 
       Normally when using normal parametrization offset of 2 in theta is deployed.
       @param i Coordinate of the parameter space.
   */
   int GetOffset(int i) const;

   /** Returns the size value of each cell.
       @param i Coordinate of the parameter space.
   */
   float GetStepValue(int i) const;

   /** Returns range value of a desized axis.
       @param i Coordinate of the parameter space.
       @param j Minimum or maximum range. i=1 for minimum, 1=2 for maximum.
   */
   inline float GetParameterRangeValue(int i, int j) const;

   /** Sets the detection threshold value.
       @param Threshold New detection threshold value.
   */
   void SetThreshold(unsigned int Threshold);
   
   /** Returns the detection threshold value.
   */
   unsigned int GetThreshold() const;

   /** Computes a real valued parameter to its corresponding disrectized cell coordinate value.
       @param i Coordinate of the space,
       @param v Value to the discretized.
   */
   inline int Discretize(int i, float v) const;

   /** Increment a accumulator cell at position pos with one.
       This function is called for accumulation.
       @param Position Position of the accumulator cell to be incremented.
       @return Return true if the score of position has exeeded threshold value. 
               This indicate that a peak may be found.
   */
   virtual inline bool Inc(const CPoint2D<int>& Position);

   /** Return the cell where it is marked as a peak.
   */
   CParameter2D<int> GetPeakCoordinates() const;

   /** Set the peak coordinate (score>t) to a position.
       @param Position New position to be set.
   */
   void SetPeakCoordinates(const CPoint2D<int>& Position);

   /** Returns the real parameter value whose corresponding cell is marked as a peak. */
   CParameter2D<float> GetRealPeakCoordinates();

   /** Finds all peaks with a specified method.
       @param DetectionMethod The user must specify a detection strategy, The butterfly method is set by default.
       @return All peaks are returned in a vector object.
   */
   vector<CParameter2D<float> > FindPeaks(CHoughBase::PEAKDETECTIONTYPE DetectionMethod=CHoughBase::BUTTERFLY);

   /** ComputeQuantization Computes the size of quantization of the parameter rho for line detection.
       @param ImageWidth Width of the input image.
       @param ImageHeight Height of the input image.
       @param Thickness Thickness of the shapes of the image.
       @param ParamType Type of parameterization. Normal by default.
       @return Quantization of parameters (nRho,nTheta).
   */
   CPoint2D<int> ComputeQuantization(int ImageWidth, int ImageHeight, int Thickness, 
                                     CHoughBase::PARAMETERTYPE ParamType=CHoughBase::NORMAL_PARAM);

private:
   /** Threshold value for peak detection. 
       Cells with scores above this value can be considered as a parameter set. */
   unsigned int m_threshold;
   
   /** Number of discretization of parameter (1st) theta (2nd) rho. */
   CPoint2D<int> m_lineQuante;

   /** Value range of the 1st parameter. x for minimum value, y for maximum value. */
   CPoint2D<float> m_param1ValueRange;

   /** Value range of the 2nd parameter. */
   CPoint2D<float> m_param2ValueRange;

   /** Step size of a cell in each of the axes. Equal to (dt,dr) for normal parametrization. */
   CPoint2D<float> m_stepValue;

   /** Parameter space offset values in each axis. */
   CPoint2D<int> m_offset;

   /** A reference to the coordinate of the found peak cell. */
   CParameter2D<int> m_peakCoord;

   /** Set to true when Initialize() and Alloc() has been successfully called. */
   bool m_initialized;

   /** Set by constructor, all information from algorithms are written to this stream. */
   ostream& m_os;

   /** CellResetTest Check if this cell is still going downwards.
       @param x Coordinate of the cell in x-axis.
       @param y Coordinate of the cell in y-axis.
       @param from_value Value of the previous cell. 
       @param direction "Coming from" direction.
   */
   bool CellResetTest(int x, int y, unsigned int from_value, DIRECTION direction);

   /** Find a peak coordinate in the space by searching the entire space.
       @return Gets the peak cell cooridnate.
   */
   vector<CParameter2D<float> > FindPeaks_Diffusion();

   /** Use butterfly filter to find peaks. Not yet completed.
   */
   vector<CParameter2D<float> > FindPeaks_Butterfly();

   /** Use numerical differentation to obtain local maxima infomation. Not yet completed.
   */
   vector<CParameter2D<float> > FindPeaks_LocalMaxima();

   /** Use iterative method to find local maxima. Not yet completed.
   */
   vector<CParameter2D<float> > FindPeaks_Iteration();

   /** Convolve Performs convolution of parameter space with a filter.
       @param Filter A square filter.
   */
   void Convolve(CHoughSpace2D& newspace, const CArray2D<float>& Filter);

};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

inline float CHoughSpace2D::GetParameterRangeValue(int i, int j) const
{
   CPoint2D<float> p;
   if (i==1) p = m_param1ValueRange;
   else p = m_param2ValueRange;

   if (j==1) return p.GetX();
   else return p.GetY();
}


inline int CHoughSpace2D::Discretize(int i, float v) const
{
   if (i==1) return Round(m_offset.GetX()+v/m_stepValue.GetX()); 
   else      return Round(m_offset.GetY()-v/m_stepValue.GetY());
}


inline bool CHoughSpace2D::Inc(const CPoint2D<int>& Position)
{ 
   int x = Position.GetX();
   int y = Position.GetY();
   this->operator[](x)[y]++; 

   if (this->operator[](x)[y] == m_threshold) {
      m_peakCoord.SetX(x);
      m_peakCoord.SetY(y);
      return true;
   }

   return false;
}

/*@}*/ /* end group "algorithms" */

} // end namespace ipl

#endif //_IPL98_HOUGHSPACE2D_H
