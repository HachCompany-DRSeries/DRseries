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

#ifndef _IPL98_KERNEL_GRAPHICS_H
#define _IPL98_KERNEL_GRAPHICS_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../ipl98_types.h"
#include "../ipl98_types_c_only.h"
#include "../kernel_pixelgroup.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup graphics ANSI-C Graphics functions
	Kernel ANSI C Graphics algorithms, last updated 4/5/2002 by edr@mip.sdu.dk. 
	A collection of algorithms generating points for different curves such as 
	lines, circles etc.
	All methods stores the generated discrete pixel positions in a TPixelGroup
	container class. This makes it possible to iterate through all discrete values
	on a curve in an image.	By using the method AddToImage(...) working with the
	TPixelGroup structure a generated set of points can be drawn in an image. If 
	nothing else is mentioned a curve consists of eight-connected positions.
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 1.00
	@author René Dencker Eriksen edr@mip.sdu.dk
    @{ */

/** Generates a discrete set of points on a line described by the given parameters,
	results are appended to the supplied TPixelGroup container. The positions are ordered
	starting at position "Start". The generation is very fast, it uses the Bresenham
	algorithm using integer calculations only.
	@param Start Beginning of line, positions in the pixelgroup will start from this point.
	@param End End of line, positions in the pixelgroup will end with this point.
	@param pPixelGroup Generated discrete pixel values are stored in this group.
	@return False, if pPixelGroup is NULL.
	@version 1.00
	@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk) */
bool k_GraphicsGenerateLine(T2DInt Start, T2DInt End, TPixelGroup* pPixelGroup);

/** Generates a discrete set of points on a line with given width. This line is not perfect
	since the ends are either vertical or horisontal based on the angle of the line to be
	drawn, i.e. if the line slope is less than or equal to 45 degrees it is closest to 
	horisontal and a vertical line end is used and visa versa for a line with slope 
	greater than 45 degrees. Results are appended to the supplied TPixelGroup container.
	For a line widt width one use k_GraphicsGenerateLine() instead.
	@param Start Beginning of line.
	@param End End of line.
	@param Width Width of line. If value is equal, the line is "shiftet" down (for lines closest
		to horisontal) or "shiftet" to the right (for lines closest to vertical).
	@param pPixelGroup Generated discrete pixel values are stored in this group.
	@return False, if pPixelGroup is NULL or if Width is zero.
	@version 0.50
	@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk) 
	@see k_GraphicsGenerateLine */
bool k_GraphicsGenerateLineThick(T2DInt Start, T2DInt End, unsigned int Width, TPixelGroup* pPixelGroup);

/** Generates a discrete set of points on a circle described by the given parameters,
	results are appended to the supplied TPixelGroup container. The positions are not ordered
	and there are duplicates of some positions (at every 45 degree a point is duplicated, that 
	is a total of 8 lines).
	The generation is very fast, it uses the Bresenham algorithm using integer calculations
	only. If you want an ordered set of points call the function k_GraphicsGenerateEllipse()
	instead.
	@param Center Position for center of circle.
	@param Radius Radius of circle.
	@param pPixelGroup Generated discrete pixel values are stored in this group.
	@return False, if pPixelGroup is NULL.
	@version 1.00
	@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
	@see k_GraphicsGenerateEllipse */
bool k_GraphicsGenerateCircle(T2DInt Center, unsigned int Radius, TPixelGroup* pPixelGroup);

/** Generates a discrete set of points on a circle ring described by the given parameters,
	results are appended to the supplied TPixelGroup container. The positions are not ordered
	and there are duplicates of some positions (at every 45 degree the line from inner perimeter
	to outer perimeter is duplicated, that is a total of 8 lines).
	The generation is very fast, it uses the Bresenham algorithm using integer calculations
	only.
	@param pPixelGroup The group to store the positions in.
	@param Center Position for center of circle.
	@param Radius1 Radius for inner perimeter of circle ring.
	@param Radius2 Radius for outer perimeter of circle ring.
	@param pPixelGroup Generated discrete pixel values are stored in this group.
	@return False, if pPixelGroup is NULL.
	@version 0.50
	@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
	@see k_GraphicsGenerateCircle */
bool k_GraphicsGenerateCircleRing(T2DInt Center, unsigned int Radius1, unsigned int Radius2, TPixelGroup* pPixelGroup);

/** Same as k_GraphicsGenerateCircleRing(), except the width of the ring is provided instead
	of two different radii.
	@param Center Position for center of circle.
	@param Radius Distance from circle center to center of ring.
	@param Width Width of circle to be drawn. If Width is even the outer half of the circle
		ring will be one greater than the inner.
	@param pPixelGroup Generated discrete pixel values are stored in this group.
	@return False, if Width is 0.
	@version 0.50
	@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
	@see k_GraphicsGenerateCircleRing */
bool k_GraphicsGenerateCircleRingW(T2DInt Center, unsigned int Radius, unsigned int Width, TPixelGroup* pPixelGroup);

/** Generates a discrete set of points on an ellipse described by the given parameters,
	results are appended to the supplied TPixelGroup container. The positions are ordered
	starting at the bottum (highest y-value) and runnig clockwise.
	The algorithm is not optimized.
	@param Center Position for center of circle.
	@param RadiusX Main axis in X-direction.
	@param RadiusY Main axis in Y-direction.
	@param pPixelGroup Generated discrete pixel values are stored in this group.
	@return False, if one of the main axises is 0 or if pPixelGroup is NULL.
	@version 0.50
	@author Implementation by Ivar Balslev (ivb@mip.sdu.dk) */
bool k_GraphicsGenerateEllipse(T2DInt Center, unsigned int RadiusX, unsigned int RadiusY, TPixelGroup* pPixelGroup);

/** Generates a discrete set of points forming a cross, always same size in each dimension.
	Positions are appended to the supplied TPixelGroup container.
	Calls k_GraphicsGenerateLine() twice.
	@param Center Center of cross.
	@param Size Total size of cross.
	@param pPixelGroup Generated discrete pixel values are stored in this group.
	@return False, if pPixelGroup is NULL.
	@version 0.50
	@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk) */
bool k_GraphicsGenerateCross(T2DInt Center, unsigned int Size, TPixelGroup* pPixelGroup);

/** Generates a discrete set of points forming a plus, always same size in each dimension.
	Positions are appended to the supplied TPixelGroup container.
	Calls k_GraphicsGenerateLine() twice.
	@param Center Center of cross.
	@param Size Total size of cross.
	@param pPixelGroup Generated discrete pixel values are stored in this group.
	@return False, if pPixelGroup is NULL.
	@version 0.50
	@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk) */
bool k_GraphicsGeneratePlus(T2DInt Center, unsigned int Size, TPixelGroup* pPixelGroup);

/** Generates a discrete set of points forming a rectangle. To avoid misunderstandings, the first
	supplied point defining the rectangle must be upper left corner and the second point must be
	the lower right corner. The order of the generated positions are then: First the upper horisontal
	line startaing at left end, next the rightmost vertical line starting from top, third the
	bottum horisontal line beginning at right side and finally the leftmost vertical line starting
	from bottum. Positions are appended to the supplied TPixelGroup container.
	Calls k_GraphicsGenerateLine() four times.
	@param TopLeft Upper left corner of rectangle.
	@param BottumRight Bottum right corner of rectangle.
	@param pPixelGroup Generated discrete pixel values are stored in this group.
	@return False, if TopLeft and BottumRight is not positioned correct or if pPixelGroup is NULL.
	@version 0.50
	@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk) */
bool k_GraphicsGenerateRectangle(T2DInt TopLeft, T2DInt BottumRight, TPixelGroup* pPixelGroup);

/*@}*/ /* end group "graphics" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_GRAPHICS_H */
