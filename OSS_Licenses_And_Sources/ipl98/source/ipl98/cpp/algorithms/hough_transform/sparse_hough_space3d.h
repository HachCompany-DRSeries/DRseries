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

#ifndef _IPL98_SPARCEHOUGHSPACE3D_H
#define _IPL98_SPARCEHOUGHSPACE3D_H

#include "../../../ipl98_setup.h" /* always include the setup file */
#include "str_list.h"
#include "../../arrays/array2d.h"
#include "../../../../points/point3d.h"

namespace ipl {

/* This class must be defined in order to use str_list. */
template class str_list<CPoint2D<int> >; 


/*  Parameter space for 3D parameter objects. Derived from a CArray2D-list structure.
	@see CHoughRHTCircle

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CSparseHoughSpace3D ipl98/cpp/algorithms/hough_transform/sparse_hough_space3d.h
    @author Qinyin Zhou <qinyin@mip.sdu.dk>
    @version 0.99
*/
class CSparseHoughSpace3D : public CArray2D<str_list<CPoint2D<int> > > {
public:
   /** Default constructor.
   */
	CSparseHoughSpace3D();

   /** Constructor.
       @param ImageWidth  Width of the input image object. Used for quantization
       @param ImageHeight Height of the input image object. Used for quatization.
   */
   CSparseHoughSpace3D(int ImageWidth, int ImageHeight);

   /** Destructor
   */
	~CSparseHoughSpace3D();

   /** Accumulation in the space.
       @param Position Position of the cell for accumulation.
   */
   bool Inc(const CPoint3D<int>& Position);

   /** Checks if the parameter solved is legal.
       @param Parameter Solved parameter.
   */
   bool ParameterCheck(const CPoint3D<int>& Parameter) const;

   /** Performs reallocation of the space, uses width and height parameters given to constructor.
   */
   void Alloc();

   /** Performs reallocation of the space.
   */
   void Alloc(int ImageWidth, int ImageHeight);

   /** Streaming operator.
   */
   inline friend ostream& operator<<(std::ostream& s, const CSparseHoughSpace3D& sl) {
      s << "CSparseHoughSpace3D " << endl << sl << endl;
      s << endl;
      return s;
   }

   /** Parameter space offsets.
       @return value changed to CPoint3D<float> from int by edr */
   CPoint2D<float> GetOffset() const;

   /** Returns the detected peak coordinate.
   */
   CPoint3D<int> GetRealPeakCoordinates() const;

   /** Set the treshold value.
       @param Threshold This threshold value is used for peak detection.
   */
   void SetThreshold(int Threshold);

private:
   /** Quantization of the parameter space. */
   CPoint3D<int> m_circleQuante;

   /** Allowed c1 value range. */
   CPoint2D<int> m_c1ValueRange;

   /** Allowed c2 value range. */
   CPoint2D<int> m_c2ValueRange;

   /** Allowed c3 value range. */
   CPoint2D<int> m_c3ValueRange;

   /** Detected peak coordinates. */
   CPoint3D<int> m_peakCoordinates;

   /** Parameter space offset. */
   CPoint2D<int> m_offset;

   /** Threshold value. A signal is returned when the accumulator value has exceeded it. */
   int m_threshold;

   /** Automatically computes the quantization values. */
   CPoint3D<int> ComputeQuantization(int ImageWidth, int ImageHeight);
};


} // end namespace ipl

#endif //_IPL98_SPARCEHOUGHSPACE3D_H
