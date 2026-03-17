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

#ifndef _IPL98_GRAPH_PLOT2D_H
#define _IPL98_GRAPH_PLOT2D_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include <fstream>
#include <cmath>
#include <sstream>
#include "../image.h"
#include "graphics.h"

namespace ipl{ // use namespace if C++

/** @ingroup utilities
@{ */

/** The class CGraphPlot2D is used for plotting 2D data in a window
	with user defined units, tic marks, and numerical labels.
	The class is GUI independent since the plotting is done in
	a 24 bpp instance of CImage of ipl98. This image can be
	shown during run time in suitable GUI environments. If
	userdefined coordinates of a graphical cursor is demanded
	written on the screen, then the method GetUserPos should
	be called when the cursor is moved.
	\class CGraphPlot2D ipl98/cpp/algorithms/graph_plot2d.h
	@version 0.61
	@author Implementation by Ivar Balslev (ivb@mip.sdu.dk) */
class CGraphPlot2D
{
	public: // attributes
		/** Enumeration defining the axis type for both abscissa and ordinate. The types
			are linear (LIN) and logarithmic (LOG). First name denotes abscissa and second
			the ordinate, i.e. LINLOG is a linear abscissa and a logarithmic ordinate. */
		enum AXISPLOT {LINLIN,LINLOG,LOGLIN,LOGLOG};
	/** Destination image for the plotting (24 bits/pixel).
		This reference is provided to constructor, can be reset by calling SetImage().
		@author Implementation by Ivar Balslevs (ivb@mip.sdu.dk) */
		CStdImage& m_Img;
   
	/** Constructor. Must call InitPlot() to initialize this class.
		All plotting is performed in the supplied image. Note: Colors given
		as parameters to methods must agree with the image type supplied here.
		If a CImage is provided, the resulting image will be with 24 b/p, and
		the user must use CPallette::CreateRGB(r,g,b) as color value. If a
		CByteImage is provided, the resulting image will be with 8 b/p, and
		the user must use a simple palette index in the range [0;255].
		@param Img Image to perform 2D plot in. */
     CGraphPlot2D(CStdImage& Img);
	/// Destructor
    ~CGraphPlot2D(){};

    /**  Defines the linear conversion between user coordinates and pixel coordinates
         and draws a plotting area with numerical labels along the axes.
         @param LeftX Is the horisontal user coordinate of the left plotting limit
         @param RightX Is the horisontal user coordinate of the right plotting limit
         @param BottomY Is the vertical user coordinate of the bottom plotting limit
         @param TopY Is the vertical user coordinate of the top plotting limit
         @param PixWidth Is the pixel width of the plotting area. The resulting image will be somewhat
           wider to make room for the numerical labels. PixWidth must be between 50 and 2000
         @param PixHeight Is the pixel height of the plotting area. The resulting image will be somewhat
           higher to make room for the numerical labels. PixHeight must be between 20 and 2000
         @param AxisTypes Types of axis for both the abscissa and the ordinate. Defaults to linear
			on both axis'.
         @return False if the parameters are out of relevant ranges.
         @author Implementation by Ivar Balslev (ivb@mip.sdu.dk) */
    bool InitPlot(float LeftX,float RightX,
       float BottomY,float TopY,int PixWidth,int PixHeight, AXISPLOT AxisTypes=LINLIN);

     /** Moves the internal cursor to a point in user units.
         @param x Is the horisontal coordinate (user units)
         @param y Is the vertical coordinate (user units)
         @return False if InitPlot is not called succesfully.
         @author Implementation by Ivar Balslev (ivb@mip.sdu.dk) */
    bool MoveTo(float x, float y);

     /** Draws a line of specified color from the internal cursor to a
         given point, and moves the internal cursor to the this point 
         @param x Is the horisontal coordinate (user units)
         @param y Is the vertical coordinate (user units)
         @param Color determines the color. If image given to constructor was a
			CImage, then set by CPalette::CreateRGB(r,g,b)), if image given to
			constructor was a CByteImage, then set by a palette index in the
			range [0;255].
         @return False if InitPlot is not called succesfully.
         @author Implementation by Ivar Balslev (ivb@mip.sdu.dk) */
    bool LineTo(float x, float y, ipl::UINT32 Color);

     /** Plots a point (one pixel) of specified color
         @param x Is the horisontal coordinate (user units)
         @param y Is the vertical coordinate (user units)
         @param Color determines the color. If image given to constructor was a
			CImage, then set by CPalette::CreateRGB(r,g,b)), if image given to
			constructor was a CByteImage, then set by a palette index in the
			range [0;255].
         @return False if InitPlot is not called succesfully.
         @author Implementation by Ivar Balslev (ivb@mip.sdu.dk) */
    bool PlotPoint(float x,float y,ipl::UINT32 Color);

    /**  Draws a circle with specified center, radius and color
         @param x Is the horisontal coordinate (user units) of the circle center
         @param y Is the vertical coordinate (user units) of the circle center
         @param PixelRadius is the radius in pixels of the.circle.
         @param Color determines the color. If image given to constructor was a
			CImage, then set by CPalette::CreateRGB(r,g,b)), if image given to
			constructor was a CByteImage, then set by a palette index in the
			range [0;255].
         @return False if InitPlot is not called succesfully.
         @author Implementation by Ivar Balslev (ivb@mip.sdu.dk)*/
    bool PlotCircle(float x,float y,int PixelRadius,ipl::UINT32 Color);

    /**  Returns the userdefined coordinates of a pixel position P.
         @param P Is point expressed in pixel units
         @return The userdefined coordinates of P. 
         @author Implementation by Ivar Balslev (ivb@mip.sdu.dk) */    
    CPoint2D<float> GetUserPos(const CPoint2D<ipl::INT16>& P) const;
  
    /**  Returns the pixel coordinates of the user defined point P. 
         @param P is point expressed in user units units
         @return The pixel coordinates of P.
         @author Implementation by Ivar Balslev (ivb@mip.sdu.dk) */    
    CPoint2D<ipl::INT16> GetUserPos(const CPoint2D<float>& P) const;
    
  private:
	  // attributes
	  int m_ixb,m_iyb,m_ixw,m_iyw;
	  CPoint2D<ipl::INT16> m_PlotPos;
	  bool m_Initiated;
	  CGraphics m_G;
	  CPixelGroup m_Pg;
	  float m_L10;
	  bool m_LogX,m_LogY;
	  float m_cx1,m_cy1,m_cx0,m_cy0;
	  // methods
	  inline bool InsideFrame(const CPoint2D<ipl::INT16>& P);
	  void Line(int x1, int x2, int y1,int y2,ipl::UINT32 color);
	  int convx(float x) const;
	  int convy(float y) const;
	  bool InitPlot0(float LeftX,float RightX,float BottomY,float TopY,
							int PixWidth,int PixHeight,bool LogX,bool LogY);
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

bool CGraphPlot2D::InsideFrame(const CPoint2D<ipl::INT16>& P)
{
	return (P.GetX()>=m_ixb&&
		P.GetX()<=m_ixb+m_ixw&&
		P.GetY()>=m_iyb&&
		P.GetY()<=m_iyb+m_iyw);
}

/*@}*/ /* end group "utilities" */

} // end namespace ipl

#endif //_IPL98_GRAPH_PLOT2D_H
