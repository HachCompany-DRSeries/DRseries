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

#ifndef _IPL98_SPARSEHOUGHSPACE2D_H
#define _IPL98_SPARSEHOUGHSPACE2D_H

#include "../../../ipl98_setup.h" /* always include the setup file */
#include "str_list.h"
#include "hough_base.h"
#include "hough_space2d.h" 

namespace ipl {

/*  This class is used for randomized hough transform to store accumulator cells in a sparse space.
    This class use a 1D-vector-list structure to store cells. Used internally by CHoughRHTLine.
	@see CHoughRHTLine

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CSparseHoughSpace2D ipl98/cpp/algorithms/hough_transform/sparse_hough_space2d.h
	@author Qinyin Zhou <qinyin@mip.sdu.dk>
    @version 0.99
*/
class CSparseHoughSpace2D : public CHoughSpace2D {
public:
   /** Default constructor. 
   */
   CSparseHoughSpace2D();

   /** Constructor. Makes a space of desired size.
       @param os Output stream.
       @param ImageWidth Width of the source image.
       @param ImageHeight Height of the source image.
       @param Thickness Thickness of lines appeared in image.
	   @param Threshold Peak detection threshold.
       @param ParamType Type of parameter. Normal parameterization as default.
   */
	CSparseHoughSpace2D(ostream& os, int ImageWidth, int ImageHeight, int Thickness, int Threshold, 
						CHoughBase::PARAMETERTYPE ParamType=CHoughBase::NORMAL_PARAM);

   /** Destructor.
   */
	~CSparseHoughSpace2D();

   /** Initializes CSparseHoughSpace2D specified for an image and type of parametrization..
       Creates a two dimensional Hough space object.
       @param ImageWidth Width of the source image object.
       @param ImageHeight Height of the source image object.
       @param Thickness Thickness of the lines of the source image.
       @param Threshold Peak detection threshold.
       @param ParamType Type of parametrization, slope intercept or normal parametrization. 
                        Normal parameterization method is set as default.
   */
   bool Initialize(int ImageWidth, int ImageHeight, int Thickness, int Threshold, 
				   CHoughBase::PARAMETERTYPE ParamType=CHoughBase::NORMAL_PARAM);

   /** Insert a cell into the space. The order of the cells will always remain sorted.
       Overload parent class method.
       @param Position Position of the cell for accumulation.
   */
   bool Inc(const CPoint2D<int>& Position);

   /** Reset the parameter space. The space will be set to empty. 
       Overload parent class method.
   */
   bool Alloc();

   /** UpdateSpace Update the space by 
       (1) insert a new cell or
       (2) increment score of one of the existing cell.
       The order of the cells will always remain sorted.
       @param x Cell object describing where to update.
       @return
   */
   // bool InsertNewCell(int x, int y);

   /** Convert the Hough space into an image object. Only 2D Hough space can result in 
       a successfull writing. Overload parent class method.
   */
   void ToImage(CStdImage& OutputImg);

private:
   /** Stored sparse Hough space. The structure is a 1D-vector-list structure. 
       The str_list class is inherited from list with streaming ability.
       Overload parent object. */
   vector<str_list<CPoint2D<int> > > m_space;

};

} // end namespace ipl

#endif //_IPL98_SPARSEHOUGHSPACE2D_H
