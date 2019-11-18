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

#ifndef _IPL98_GRAPHICS_H
#define _IPL98_GRAPHICS_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../std_image.h"
#include "../pixelgroup.h"
#include <vector>

namespace ipl{ // use namespace if C++

using std::vector;

/** @ingroup utilities
@{ */

/** CGraphics contains two kind of functionalities: 1) A collection of algorithms 
	generating points for different curves such as lines, circles etc 2) A collection
	methods for writing bitmap text in images and corresponding text manipulation methods,
	last updated 2/20/2003.
	All point generating methods stores the generated discrete pixel positions in a 
	CPixelGroup container class. This makes it possible to iterate through all discrete
	values on a curve in an image. By using the method AddToImage(...) in the CPixelGroup
	class a generated set of points can be drawn in an image. If nothing else is mentioned
	a curve consists of eight-connected positions.
	Three fonts are available at the moment: CourierBold13x18 and Terminal12x16 and 
	Courier8x12. The fonts are only allocated when used, i.e. constructing a CGraphics 
	object and not using the fonts does not spend time on allocating the fonts! See the 
	font method section for available methods. Note: Font methods are only available in 
	C++.

	Examples on using this class:
	\verbatim

	CImage Img(200,200,8,255);
	CGraphics Graphics;
	// Writing and aligning text in an image
	Img.SetPixel(100,100,0);
	Graphics.SetFontActive(CGraphics::Terminal12x16);
	Graphics.SetFontColor(128);
	Graphics.SetFontTransparent(false);
	Graphics.SetFontBackgroundColor(200);
	Graphics.SetFontAlignment(CGraphics::TOPRIGHT);
	Graphics.PlotString("jg_RIGHT",CPoint2D<int>(100,100),Img);
	// Following a line in an image
	CPixelGroup Grp;
	CGraphics::Line(CPoint2D<INT16>(10,10),CPoint2D<INT16>(20,14),Grp);
	CPixelGroup::ConstIterator it=Grp.Begin();
	while(it!=Grp.End())
	{
		Img.SetPixelFast((*it).x,(*it).y,0); // do something with each position
		++it;
	}
	// Following a circle in an image
	Grp.Empty();
	CGraphics::Circle(CPoint2D<INT16>(40,40),10,Grp);
	it=Grp.Begin();
	while(it!=Grp.End())
	{
		Img.SetPixelFast((*it).x,(*it).y,0);
		++it;
	}
	// The CGraphics class also have CircleW (width), Ellipse, Cross, 
	// Plus, and Rectangle
	// To simply draw a line or circle, one can use the following
	Img.DrawLine(CPoint2D<int>(50,20),CPoint2D<int>(120,11),50);
	Img.DrawCircle(CPoint2D<INT16>(150,70),14,150);
	\endverbatim
	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CGraphics ipl98/cpp/algorithms/graphics.h
	@version 1.10
	@author René Dencker Eriksen (edr@mip.sdu.dk) */
class CGraphics{
	public: // attributes
		/** @name enumerations used for font related methods */
		//@{ 
		/** Enumeration defining names for available fonts, values are CourierBold13x18, 
			Courier8x12 and Terminal12x16. */
		enum BITMAPFONT {CourierBold13x18, Terminal12x16, Courier8x12};
		/** enumeration defining alignment names for text alignment, values are
			TOPLEFT, BOTTOMLEFT, TOPMID, BOTTOMMID, TOPRIGHT and BOTTOMRIGHT */
		enum FONTALIGNMENT {TOPLEFT, BOTTOMLEFT, TOPMID, BOTTOMMID, TOPRIGHT, BOTTOMRIGHT};
		//@}
	private: // attributes
		/** @name attributes for font related methods, to be manipulated through relevant methods */
		//@{ 
		/// set in constructor, must equal number of fonts in enum BITMAPFONT
		const int m_TotalFonts;
		/// set to one of the values from BITMAPFONT, default is CourierBold13x18.
		BITMAPFONT m_FontActive;
		/** vector of font data, initialize to size m_TotalFonts and all values set to NULL pointers, 
			methods keeps track of whether or not the font has been allocated. */
		vector<UINT8*> m_FontData;

		/** Color of text when plotted into image, default is 0 for images with 1 and 8 b/p, 
			and (R,G,B)=(0,0,0) for 24 b/p, i.e. all text defaults to black with normal palettes. */
		UINT32 m_FontColor;
		/** If true, the background inbetween characters are left unchanged, defaults to true. */
		bool m_FontTransparent;
		/** Background color of characters, only used if m_FontTransparent is false. No default value. */
		UINT32 m_FontBackgroundColor;
		/** Defines how text should be aligned. Default value is TOPLEFT.
			@see SetFontAlignment */
		FONTALIGNMENT m_FontAlignment;
		//@}
	public:  // methods
		/** Default constructor. Default values used: Font color: 0 (black), Font transparant: true, Font
			alignment: TOPLEFT, Font active: CourierBold13x18. */
		CGraphics();
		/** Constructor with initial font settings.
			@param FontColor See m_FontColor.
			@param FontTransparent See m_FontTransparent.
			@param FontAlignment See m_FontAlignment.
			@param FontActive See m_FontActive, defaults to CourierBold13x18. */
		CGraphics(UINT32 FontColor, bool FontTransparent, FONTALIGNMENT FontAlignment,
					BITMAPFONT FontActive=CourierBold13x18);
		/** Default destructor. */
		~CGraphics();
		/** Generates a discrete set of points on a line described by the given parameters,
			positions are appended to the supplied CPixelGroup container.
			@param Start Beginning of line, positions in the pixelgroup will start from this point.
			@param End End of line, positions in the pixelgroup will end with this point.
			@param PixelGroup Generated discrete pixel values are stored in this group.
			@return True, in all cases when using the C++ version.
			@version 0.50
			@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk) */
		static bool Line(const CPoint2D<INT16>& Start, const CPoint2D<INT16>& End, CPixelGroup& PixelGroup);

		/** Generates a discrete set of points on a line with given width. This line is not perfect
			since the ends are either vertical or horisontal based on the angle of the line to be
			drawn, i.e. if the line slope is less than or equal to 45 degrees it is closest to 
			horisontal and a vertical line end is used and visa versa for a line with slope 
			greater than 45 degrees. Results are appended to the supplied CPixelGroup container.
			For a line widt width one use Line(Start,End,PixelGroup) instead.
			@param Start Beginning of line.
			@param End End of line.
			@param Width Width of line. If value is equal, the line is "shiftet" down (for lines closest
				to horisontal) or "shiftet" to the right (for lines closest to vertical).
			@param PixelGroup Generated discrete pixel values are stored in this group.
			@return False, if Width is zero.
			@version 0.50
			@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk) */
		static bool Line(const CPoint2D<INT16>& Start, const CPoint2D<INT16>& End, unsigned int Width,
							CPixelGroup& PixelGroup);

		/** Generates a discrete set of points on a circle described by the given parameters,
			positions are appended to the supplied CPixelGroup container. The positions are not
			ordered and there are duplicates of some positions (at every 45 degree a point is duplicated, 
			that is a total of 8 lines). The generation is very fast, it
			uses the Bresenham algorithm using integer calculations only. If you want an ordered
			set of points call the method Ellipse() instead.
			@param Center Position for center of circle.
			@param Radius Radius of circle.
			@param PixelGroup Generated discrete pixel values are stored in this group.
			@return True, in all cases when using the C++ version.
			@version 0.50
			@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
			@see Ellipse */
		static bool Circle(const CPoint2D<INT16>& Center, unsigned int Radius, CPixelGroup& PixelGroup);

		/** Generates a discrete set of points on a circle described by the given parameters,
			positions are appended to the supplied CPixelGroup container. The positions are not
			ordered and there are duplicates of some positions (at every 45 degree the line from 
			inner perimeter to outer perimeter is duplicated, that is a total of 8 lines). The 
			generation is very fast, it uses the Bresenham algorithm using integer calculations 
			only. If you want an ordered set of points call the method Ellipse() instead.
			@param Center Position for center of circle.
			@param Radius1 Radius for inner perimeter of circle ring.
			@param Radius2 Radius for outer perimeter of circle ring.
			@param PixelGroup Generated discrete pixel values are stored in this group.
			@return True, in all cases when using the C++ version.
			@version 0.50
			@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
			@see Ellipse */
		static bool Circle(const CPoint2D<INT16>& Center, unsigned int Radius1, unsigned int Radius2, 
							CPixelGroup& PixelGroup);

		/** Same as Circle(Center,Radius1,Radius2,PixelGroup), except the width of the ring 
			is provided instead	of two different radii.
			@param Center Position for center of circle.
			@param Radius Distance from circle center to center of ring.
			@param Width Width of circle to be drawn. If Width is even the outer half of the circle
				ring will be one greater than the inner.
			@param PixelGroup Generated discrete pixel values are stored in this group.
			@return False, if Width is 0.
			@version 0.50
			@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
			@see Circle */
		static bool CircleW(const CPoint2D<INT16>& Center, unsigned int Radius, unsigned int Width,
							CPixelGroup& PixelGroup);

		/** Generates a discrete set of points on an ellipse described by the given parameters,
			results are appended to the supplied CPixelGroup container. The positions are ordered
			starting at the bottom (highest y-value) and runnig clockwise.
			The algorithm is not optimized.
			@param Center Position for center of circle.
			@param RadiusX Main axis in X-direction.
			@param RadiusY Main axis in Y-direction.
			@param PixelGroup Generated discrete pixel values are stored in this group.
			@return False, if one of the main axises is 0.
			@version 0.50
			@author Implementation by Ivar Balslev (ivb@mip.sdu.dk) */
		static bool Ellipse(const CPoint2D<INT16>& Center, unsigned int RadiusX, 
										unsigned int RadiusY, CPixelGroup& PixelGroup);

		/** Generates a discrete set of points forming a cross, always same size in each dimension.
			Positions are appended to the supplied CPixelGroup container.
			@param Center Center of cross.
			@param Size Total size of cross.
			@param PixelGroup Generated discrete pixel values are stored in this group.
			@return True, in all cases when using the C++ version.
			@version 0.50
			@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk) */
		static bool Cross(const CPoint2D<INT16>& Center, unsigned int Size, CPixelGroup& PixelGroup);

		/** Generates a discrete set of points forming a plus, always same size in each dimension.
			Positions are appended to the supplied CPixelGroup container.
			@param Center Center of cross.
			@param Size Total size of cross.
			@param PixelGroup Generated discrete pixel values are stored in this group.
			@return True, in all cases when using the C++ version.
			@version 0.50
			@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk) */
		static bool Plus(const CPoint2D<INT16>& Center, unsigned int Size, CPixelGroup& PixelGroup);

		/** Generates a discrete set of points forming a rectangle. To avoid misunderstandings, 
			the first supplied point defining the rectangle must be upper left corner and the
			second point must be the lower right corner. The order of the generated positions are
			then: First the upper horisontal line startaing at left end, next the rightmost
			vertical line starting from top, third the bottom horisontal line beginning at right
			side and finally the leftmost vertical line starting from bottom.
			Positions are appended to the supplied CPixelGroup container.
			@param TopLeft Upper left corner of rectangle.
			@param BottomRight Bottom right corner of rectangle.
			@param PixelGroup Generated discrete pixel values are stored in this group.
			@return False, if TopLeft and BottomRight is not positioned correct.
			@version 0.50
			@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk) */
		static bool Rectangle(const CPoint2D<INT16>& TopLeft, const CPoint2D<INT16>& BottomRight, 
								CPixelGroup& PixelGroup);

		/** @name Font related methods, note: only available in C++. */
		//@{
		/** Plots a string of characters to image at given position, alignment is done according 
			to the value in m_FontAlignment, manipulated by the method SetFontAlignment().
			@param str String to be plotted in image.
			@param x Position of string in x-direction.
			@param y Position of string in y-direction.
			@param Img Image to plot character into.
			@return False, if the active font or alignment value are not valid.
			@see SetFontAlignment
			@version 0.50
			@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk) */
		bool PlotString(const char* str, int x, int y, CStdImage& Img);
		/** Plots a string of characters to image at given position, alignment is done according 
			to the value in m_FontAlignment, manipulated by the method SetFontAlignment().
			@param str String to be plotted in image.
			@param Pos Position of string.
			@param Img Image to plot character into.
			@return False, if the active font or alignment value are not valid.
			@see SetFontAlignment
			@version 0.50
			@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk) */
		bool PlotString(const char* str, const CPoint2D<int>& Pos, CStdImage& Img);
		/** Set active font.
			@param Font Font to be active, values can be seen in BITMAPFONT, get name
				by writing CGraphics::FONTNAME.
			@see BITMAPFONT */
		void SetFontActive(BITMAPFONT Font);
		/** Returns active font. */
		BITMAPFONT GetFontActive() const;
		/** Define whether text is written with a background or transparent. Note:
			If you set it to non transparent, you must call SetFontBackgroundColor(),
			otherwise the background color is undefined.
			@param Transparent If false, text is written with a background defined
				by m_FontBackgroundColor.
			@see SetFontBackgroundColor */
		void SetFontTransparent(bool Transparent);
		/** Returns true, if font is transparent. */
		bool GetFontTransparent() const;
		/** Define the color for text.
			@param Color Text color, the user must ensure this value corresponds with
				image type. To create an RGB value use CreateRGB in CPalette.
			@see CPalette */
		void SetFontColor(UINT32 Color);
		/** Returns text color. */
		UINT32 GetFontColor() const;
		/** Define the color for text background, only in use if there is no transparancy.
			@param Color Background color, the user must ensure this value corresponds with
				image type. To create an RGB value use CreateRGB in CPalette.
			@see SetFontTransparent
			@see CPalette */
		void SetFontBackgroundColor(UINT32 Color);
		/** Returns background color. */
		UINT32 GetFontBackgroundColor() const;
		/** Defines where text should be written according to the supplied position.
			@param Align Text can be aligned to TOPLEFT corner, BOTTOMLEFT corner, TOPMID top middle
				of string, BOTTOMMID bottom middle of string, TOPRIGHT corner and BOTTOMRIGHT corner.
				Get alignment name by writing CGraphics::ALIGNMENTNAME.
			@see PlotString
			@see FONTALIGNMENT */
		void SetFontAlignment(FONTALIGNMENT Align);
		/** Return font alignment. */
		FONTALIGNMENT GetFontAlignment() const;
		//@}
private: // methods
		bool InitFont();
		void InitCourierBold13x18();
		void InitCourier8x12();
		void InitTerminal12x16();
		bool GetFontInfo(BITMAPFONT Font, UINT8** ppFontData, UINT8& ASCIIStart, int& TotalChars,int& CharWidth,int& CharHeight);
		bool AdjustAlignment(const char* str, CPoint2D<int>& Pos);
		int GetSizeString(const char* str);
		/* Plots a single character to image at given position, alignment is done according 
			to the value in m_FontAlignment, manipulated by the method SetFontAlignment().
			Use PlotString() to plot a string of characters.
			@param ch Character to be plotted in image.
			@param Pos Position of character.
			@param Img Image to plot character into.
			@return False, if the active font is not valid.
			@see PlotString
			@see SetFontAlignment */
		bool PlotChar(char ch, const CPoint2D<int>& Pos, CStdImage& Img);
};

/*@}*/ /* end group "utilities" */

} // end namespace ipl

#endif //_IPL98_GRAPHICS_H
