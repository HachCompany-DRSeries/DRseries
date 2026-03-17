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

#ifndef _IPL98_KERNEL_PIXELGROUP_H
#define _IPL98_KERNEL_PIXELGROUP_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "image/kernel_functions.h"
#include "ipl98_types.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup pixelgroup ANSI-C Pixel group container functions
	Kernel ANSI C pixel group functions, last updated 9/3/2005.
	Works on TPixelGroup structure.
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 1.44
	@author René Dencker Eriksen (edr@mip.sdu.dk)
    @{ */

/** A group of pixels found by a segmentation algorithm.
	The segmentation algorithm decides which pixels are to
	be considered as one group. The Blob-algorithm is one
	example of a segmentation algorithm.
	
	@see TStatistic */
typedef struct {
	/** @param Top (T2DInt) Position with lowest y-value in group. */
	T2DInt Top;
	/** @param Bottom (T2DInt) Position with highest y-value in group. */
	T2DInt Bottom;
	/** @param Left (T2DInt) Position with lowest x-value in group. */
	T2DInt Left;
	/** @param Right (T2DInt) Position with highest x-value in group. */
	T2DInt Right;
	/** @param Color (COLORTYPE) Value HIGHCOLOR if the group is derived from high color pixels
		otherwise the value is LOWCOLOR. This value may not be used by all algorithms. */
	COLORTYPE Color;
	/** @param Bits per pixel (bit planes): 1,8 or 24. Is set when the function k_AddColors
		is called. */
	UINT16 Bits;
	/** @param NumberOfPixels (unsigned int) Total number of pixels in the group. */
	unsigned int NumberOfPixels;
	/** @param AllocatedPixels (unsigned int) Internal use. Remembers the maximum allocated
		pixel positions available in the pPos array. */
	unsigned int AllocatedPixels; /* internal use */
	/** @param pPos (T2DInt*) Array of pixel positions. */
	T2DInt* pPos;
	/** @param pColor (UINT32) Colors corresponding to the positions stored in the pPos array.
		Index i corresponds to position stored in pPos[i]. This array is not allways used,
		so remember to check if it is a NULL pointer. */
	UINT32* pColor;
} TPixelGroup;

/** Sets all the pointers in pPixelGroup to NULL. Initialises all
	integer to 0 and T2DInt members to (0,0). Always call
	this function when creating a variable of the TPixelGroup type */
void k_InitGroup(TPixelGroup* pPixelGroup);

/**	Deletes all allocated memory for PixelGroup and sets variables to 0.
	Always call this function when leaving a scope where a TPixelGroup
	variable were created and used. */
void k_EmptyGroup(TPixelGroup* pPixelGroup);

/** Same as k_EmptyGroup(), except the allocated memory to store positions
	in, is not deallocated. Memory allocated for Colors (if available) is also
	left unchanged. This can optimize performance if the group
	is to be used again.
	@see k_EmptyGroup
	@version 0.7 */
void k_EmptyGroupNoDeallocation(TPixelGroup* pPixelGroup);

/**	Allocates memory for pixel positions in the TPixelGroup structure.
	To make insertions fast, call this funcion with a value in InitialSize
	greater than the expected pixels in the group.
	@param InitialSize (unsigned int) Number of positions (T2DInt structures)
		to allocate memory for. Must be 1 or greater. */
void k_AllocGroup(TPixelGroup* pPixelGroup,unsigned int InitialSize);

/**	Same as k_AllocGroup, except that the array containing color information
	in the TPixelGroup structure is also allocate.
	@param InitialSize (unsigned int) Number of positions (T2DInt structures)
		to allocate memory for. Must be 1 or greater. */
void k_AllocGroupAndColors(TPixelGroup* pPixelGroup,unsigned int InitialSize);

/** Reallocates memory for the TPixelGroup structure. The new allocated memory
	is not initialised. If colors are available the color array will also be reallocated.
	@param NewSize (unsigned int) The new number of positions to allocate memory for.
		Note: The NewSize may be less than the actual allocated memory! In that case
		the corresponding positions stored in the end of the actual memory are deleted. */
void k_ReAllocGroup(TPixelGroup* pPixelGroup,unsigned int NewSize);

/**	Removes unused memory allocated for pixel positions in the TPixelGroup structure.
	If colors are available the color array will also be adjusted.
	Because memory allocation is a time consuming operation it is a good idea by each
	k_ReAllocGroup() (or k_AllocGroup()) call to allocate a big number of pixels and
	then at the end of the construction of a TPixelGroup calling this
	k_AdjustSizeOfGroup function to deallocate unused memory. */
void k_AdjustSizeOfGroup(TPixelGroup* pPixelGroup);

/**	The position (x,y) is added to the group of pixels. The function keeps track of
	allocation and updates the AllocatedPixels member in TPixelGroup. The boundary is
	also updated if the new position is outside the old boundary.
	@param x (INT16) Position in horisontal direction.
	@param y (INT16) Position in vertical direction.
	@return False, if colors are available, in that case the method does nothing. */
bool k_AddPosToGroup(INT16 x, INT16 y, TPixelGroup* pPixelGroup);

/**	Same as k_AddPosToGroup(), except the position is given in a T2DInt structure.
	@param Pos (T2DInt) Position to add to PixelGroup
	@see k_AddPosToGroup 
	@return False, if colors are available, in that case the method does nothing. */
bool k_Add2DPosToGroup(T2DInt Pos, TPixelGroup* pPixelGroup);

/** Same as k_AddPosition(x,y), except a color value is also inserted.
	@param x Position in horisontal direction.
	@param y Position in vertical direction. 
	@return False, if colors are not available, in that case the function does nothing.
	@see k_AddPosColorToGroupFast */
bool k_AddPosColorToGroup(INT16 x, INT16 y, UINT32 Color, TPixelGroup* pPixelGroup);

/**	Same as k_AddPosColorToGroup(x,y), except the position is given in a T2DInt structure.
	@param Pos Position to add to PixelGroup.
	@see k_Add2DPosColorToGroupFast */
bool k_Add2DPosColorToGroup(T2DInt Pos, UINT32 Color, TPixelGroup* pPixelGroup);

/** Get the color corresponding to the position stored at "Index" in the group.
	Maximum index is TPixelGroup.TotalPositions-1. There may not be any colors stored
	in the group, use k_ColorsAvailable() to check this.
	@return Position at given index, returns 0 if index is out of range
		or if colors are unavailable. */
UINT32 k_GetColorGroup(unsigned int Index, const TPixelGroup* pPixelGroup);

/** Set the color ColorValue corresponding to the position stored at "Index" in
	the group. Maximum index is TPixelGroup.TotalPositions-1. There may not be any
	colors stored in the group, use k_ColorsAvailable() to check this.
	@return False, if index is out of range or if colors are unavailable. */
bool k_SetColorGroup(UINT32 ColorValue, unsigned int Index, TPixelGroup* pPixelGroup);

/** Use this method to check if colors are stored in the pixel group.
	@return True, if there are colors stored in the group. */
bool k_ColorsAvailableGroup(const TPixelGroup* pPixelGroup);

/** Insert the position (x,y) at given Index, which must be less than the
	NumberOfPixels (stored in the TPixelGroup structure). The ordering of
	the positions is preserved. If pixel colors are present in the
	TPixelGroup structure this function cannot be used. A call to k_RemoveColors()
	is then needed. The function k_AddColorsToGroup() can be called
	later to insert the colors again. The boundary is updated if the new position
	is outside the old boundary.
	@param x (INT16) Position in horisontal direction.
	@param y (INT16) Position in vertical direction.
	@param Index Index in the array of positions to be removed.
	@return False, if pixel colors are present (in the pColor array ot the
		TPixelGroup structure) or if Index is out of range. */
bool k_InsertPosInGroup(INT16 x, INT16 y, unsigned int Index, TPixelGroup* pPixelGroup);

/** Same as k_InsertPosInGroup, except the position is given in a T2DInt structure.
	@param Pos (T2DInt) Position to add to PixelGroup.
	@return False, if pixel colors are present (in the pColor array ot the
		TPixelGroup structure) or if Index is out of range. */
bool k_Insert2DPosInGroup(T2DInt Pos, unsigned int Index, TPixelGroup* pPixelGroup);

/** Same as k_InsertPosInGroup(x,y), except a color value is also inserted.
	@param x Position in horisontal direction.
	@param y Position in vertical direction. 
	@return False, if colors are not available, in that case the function does nothing.
	@see k_InsertPosColorInGroupFast */
bool k_InsertPosColorInGroup(INT16 x, INT16 y, UINT32 Color, unsigned int Index, TPixelGroup* pPixelGroup);

/**	Same as InsertPositionColor(x,y), except the position is given in a T2DInt structure.
	@param Pos Position to add to PixelGroup.
	@return False, if colors are not available, in that case the function does nothing. */
bool k_Insert2DPosColorInGroup(T2DInt Pos, UINT32 Color, unsigned int Index, TPixelGroup* pPixelGroup);

/** Remove a position stored in the group at given index. Done by moving the
	last position in the pPos array contained in TPixelGroups to the index
	which is to be removed. If pixel colors are available in the TPixelGroup
	structure (in the pColor array), this array is updated. The boundary is also
	updated if the removed position causes changes in the boundary.
	@param Index Index in the array of position to be removed.
	@return False, if Index is out of range.
	@see k_RemovePosFromGroupSlow */
bool k_RemovePosFromGroup(unsigned int Index, TPixelGroup* pPixelGroup);

/** Same as k_RemovePosFromGroup, but the ordering of the pixels is preserved.
	This function is slower than k_RemovePosFromGroup.
	@param Index Index in the array of position to be removed.
	@return False, if Index is out of range.
	@see k_RemovePosFromGroup */
bool k_RemovePosFromGroupSlow(unsigned int Index, TPixelGroup* pPixelGroup);

/** Removes duplicates of same position in given pixelgroup. The algorithm could
	probably be more efficient but it is still nice to have this functionality.
	To make it a bit efficient though, the order of the positions is not preserved, i.e.
	the same code as k_RemovePosFromGroup is used to remove positions, but no update of
	boundary is neccesary.
	@return Total number of duplicate positions removed. */
unsigned int k_RemoveDuplicatesFromGroup(TPixelGroup* pPixelGroup);

/** Allocates cells for color values but does not initialize the values.
	Faster than using k_AllocAndInitColorsGroup(). Note: If you later on call k_AddPosToGroup()
	or k_InsertPosInGroup() without adding a color, memory for previous allocated colors
	are deleted since the number of allocated colors does not equal number of positions.
	The functions takes care of it.
	@return False, if colors are allready available.
	@see AllocAndInitColors */
bool k_AllocColorsGroup(TPixelGroup* pPixelGroup);

/** Same as k_AllocColorsGroup but here the initial value of the allocated
	cells can be specified. Note: If you later on call k_AddPosToGroup()
	or k_InsertPosInGroup() without adding a color, memory for previous allocated colors
	are deleted since the number of allocated colors does not equal number of positions.
	The functions takes care of it.
	@param InitialValue Initial value of allocated colors.
	@return False, if colors are allready available.
	@see AllocColors */
bool k_AllocAndInitColorsGroup(UINT32 InitialValue,TPixelGroup* pPixelGroup);

/** Copy whole group from "pSource" to "pDest". The "pDest" parameter must point
	to an allocated and initialized TPixelGroup structure.
	Previous data in "pDest" are destroyed. */
void k_CopyGroup(TPixelGroup* pDest, const TPixelGroup* pSource);

/**	After the creation of a PixelGroup this function will get the original pixel
	colors from given source image and insert them into the PixelGroup. The pixel
	depth (b/p) from the source image is also stored in the pixel group. A call
	to the function k_CopyGroupToImage() will then produce an image with same
	pixel depth.
	@param pSource Image to get the original pixel values from. The PixelGroups positions
		must be derived from this image.
	@return False if the PixelGroup has no pixels in it, colors have already been added
		or one of the parameters is a NULL pointer. */
bool k_AddColorsToGroup(const TImage* pSource, TPixelGroup* pPixelGroup);

/** Remove colors stored in the TPixelGroup. Sets the pPixelGroup member pColor to NULL.
	If colors are present, the pColor memory is deallocated. */
void k_RemoveColors(TPixelGroup* pPixelGroup);

/** Copies a pixel group to a new image. Origo of the new image is set to (-Left.x,-Top.y)
	which is the pixelgroups top left boundary. There are three cases which decides the
	bit depth of the output image: 1) No colors available (i.e. the function
	k_AddColorsToGroup() has not been called), then a black and white image with
	1 b/p is produced and the positions are plotted in the image according to the
	"Color" member of the TPixelGroup-structure. If the Color-type is undefined
	the default is a white background with black pixels set at the given
	positions. 2) Colors available but no palette is given (i.e. pPalette is a NULL
	pointer), then an image with a graytone palette is produced if the Bits pixel
	depth member of TPixelGroup is 1 or 8. If the pixel depts is 24 b/p
	an image with 24 b/p without a palette is produced. 3) Colors available and
	a palette is provided. An image with pixel depth given in TPixelGroup and the given
	palette is produced. The pallette must correspond with the pixel depth in the
	TPixelGroup, otherwise an error message will be produced.
	@param pPalette The pallette to be used in the new image. If no colors are available
		or the pixel depth is 24 b/p this parameter is ignored. If the parameter is NULL
		a default palette is used (see case 2 above).
	@param BackGround The background color of new image, the user must be sure that
		the value in case of 1 or 8 b/p is in the range of the palette index and in
		case of 24 b/p the value must be the RGB color.
	@param pDest Image to copy the group to. All previous information in pDest is
		destroyed.
	@return False, if the pixel group is empty or "pDest" is a NULL pointer.
	@see k_CopyGroupToImageFixedColors */
bool k_CopyGroupToImage(const TPalette* pPalette, UINT32 BackGround, TImage* pDest, const TPixelGroup* pPixelGroup);


/** Same as k_CopyGroupToImage() (different parameters), 
	except that the pixel positions are plotted with the given color 
	value ForeGround. Color values available in this pixel group are
	ignored.
	@param BackGround The background color of new image, the user 
		must be sure that the value in case of 1 or 8 b/p is in the 
		range of the palette index and in case of 24 b/p the value must 
		be the RGB color.
	@param ForeGround The foreground color of new image, the user must 
		be sure that the value in case of 1 or 8 b/p is in the range of 
		the palette index and in case of 24 b/p the value must be the 
		RGB color.
	@param Bits Bits per pixel for destination image. The user must be 
		sure that the bit depth corresponds to the two color value 
		parameters. In case of 8 b/p, a gray tone palette is created, 
		in case of 1 b/p a black and white palette is created.
	@param Dest Image to copy the group to. All previous information in 
		Dest is destroyed.
	@return False, if the pixel group is empty or Bits is not 1, 8 or 
		24 b/p.
	@see k_CopyGroupToImage */
bool k_CopyGroupToImageFixedColors(UINT32 BackGround, UINT32 ForeGround, UINT16 Bits, TImage* pDest,
								   const TPixelGroup* pPixelGroup);

/** Adds the points in pPixelGroup to the already allocated image, only the positions contained
	in the pixelgroup will affect the image. Positions outside image are ignored. If the
	pixelgroup contains color values they can be used as plotting values by calling
	the function k_AddGroupColorsToImage() instead. The function
	automaically does the copying as fast as possible by doing some range checks before
	entering the inner loops.
	@param pDest Image to plot points in, only pixels at positions contained in pixelgroup
		will affect the image.
	@param pPixelGroup The group of positions to be plotted in image.
	@param Color A UINT32 value containing the color, in case of a palette it is the index
		in the palette. If the group contains colors, you can use them by calling
		k_AddGroupColorsToImage() instead of this function.
	@return False, if pPixelgroup is NULL.
	@see k_AddGroupColorsToImage */
bool k_AddGroupToImage(TImage* pDest, const TPixelGroup* pPixelGroup, UINT32 Color);

/** Adds the points in pPixelGroup to both the complex and the real part of the already
	allocated image, only the positions contained
	in the pixelgroup will affect the image. Positions outside image are ignored. The function
	automaically does the copying as fast as possible by doing some range checks before
	entering the inner loops.
	@param pDest Image to plot points in, only pixels at positions contained in pixelgroup
		will affect the image.
	@param pPixelGroup The group of positions to be plotted in image.
	@param Value A FLOAT32 value containing the value.
	@return False, if pPixelgroup is NULL.*/
bool k_AddGroupToComplexImage(TComplexImage* pDest, const TPixelGroup* pPixelGroup, FLOAT32 Value);

/** Adds the points in pPixelGroup of the already allocated image, only the positions contained
	in the pixelgroup will affect the image. Positions outside image are ignored. The function
	automaically does the copying as fast as possible by doing some range checks before
	entering the inner loops.
	@param pDest Image to plot points in, only pixels at positions contained in pixelgroup
		will affect the image.
	@param pPixelGroup The group of positions to be plotted in image.
	@param Value A FLOAT32 value containing the value.
	@return False, if pPixelgroup is NULL.*/
bool k_AddGroupToFloatImage(TFloatImage* pDest, const TPixelGroup* pPixelGroup, FLOAT32 Value);

/** Adds the points in pPixelGroup of the already allocated image, only the positions contained
	in the pixelgroup will affect the image. Positions outside image are ignored. The function
	automaically does the copying as fast as possible by doing some range checks before
	entering the inner loops.
	@param pDest Image to plot points in, only pixels at positions contained in pixelgroup
		will affect the image.
	@param pPixelGroup The group of positions to be plotted in image.
	@param Value A INT16 value containing the value.
	@return False, if pPixelgroup is NULL.*/
bool k_AddGroupToIntImage(TIntImage* pDest, const TPixelGroup* pPixelGroup, INT16 Value);

/** Adds the points in pPixelGroup to the already allocated image, only the positions
	contained in the pixelgroup will affect the image. Positions outside image are ignored.
	The color values in the pixelgroup are used when inserting postitions, if you want
	the positions plotted with a constant color call the function k_AddGroupToImage().
	Ensure that the bit depth (and palette if present) complies with the color
	values in the pixelgroup. The function will not do any tests for that. The function
	automaically does the copying as fast as possible by doing some range checks before
	entering the inner loops.
	@param pDest Image to plot points in, only pixels at positions contained in pixelgroup
		will affect the image.
	@param pPixelGroup The group of positions to be plotted in image.
	@return False, if pPixelgroup is NULL.
	@see k_AddGroupToImage */
bool k_AddGroupColorsToImage(TImage* pDest, const TPixelGroup* pPixelGroup);

/*  IMPORTANT: Replaced by k_CopyGroupToImage(), only here for IPL98 to be backward compatible
	Copy a PixelGroup to a binary image.
	@param Color If HIGHCOLOR the pixels in the group are set to white (1). If LOWCOLOR
	pixels in the group are set to black (0). Background is initialised to the inverse
	value.
	@param pDest Image to copy the group to. All previous information in pDest is
	destroyed. */
bool k_GroupToBinaryImage(const TPixelGroup* pPixelGroup,COLORTYPE Color,TImage* pDest);

/*  IMPORTANT: Replaced by k_CopyGroupToImage(), only here for IPL98 to be backward compatible
	Copy a PixelGroup to a gray tone image. Needs the source image to get the original
	pixel values. If the pixel values is included in the PixelGroup (in the pColor
	member) use the function k_GroupColorsToGrayImage().
	@param Background Pixel colors in new image background.
	@param pSource Image to get the original pixel values from. The PixelGroups positions
		must be derived from this image.
	@param pDest Image to copy the group to. All previous information in pDest is
	destroyed. */
bool k_GroupToGrayImage(const TPixelGroup* pPixelGroup,UINT8 Background,
						   const TImage* pSource, TImage* pDest);

/*	IMPORTANT: Replaced by k_CopyGroupToImage(), only here for IPL98 to be backward compatible
	Copy a PixelGroup with pixel values to a new image. 
	@param Background Pixel colors in new image background.
	@param pDest Image to copy the group to. All previous information in pDest is
	destroyed. */
bool k_GroupColorsToGrayImage(const TPixelGroup* pPixelGroup,UINT8 Background,
							  TImage* pDest);

/** Test if given position is included in the PixelGroup
	@param Pos Position to test for.
	@param pIndex If the position "pos" is found, the pIndex of the position is returned
		in this parameter. If not found, the parameter is unchanged.
	@return true if the position was found, false if not found or pPixelGroup is NULL. */
bool k_PosInGroup(const TPixelGroup* pPixelGroup, T2DInt Pos, unsigned int* pIndex);

/** Prints the information contained in "pPixelGroup" to stdout, format is:
	\verbatim
	**************** PixelGroup info ********************
	 Positions: Top=(-10,-10) Bottom=(11,17) Left=(-15,8) Right=(43,-5)
	 Width=59 Height=28
	 NumberOfPixels=118 AllocatedPixels=128
	 pos 0=(-10,-10) pos 1=(-9,11) ...
	\endverbatim
	where the positions are split into lines with PosPrLine positions on each line.
	@param PrintPositions If true, all pixel positions stored in TPixelGroup are written
		to stdout.
	@param PosPrLine Number of positions to be written on each line.
		Only used in PrintPositions is true. */
void k_PrintGroupInfo(const TPixelGroup* pPixelGroup,bool PrintPositions,unsigned int PosPrLine);

/** Runs through all positions in the group and update the left/right/top/bottom
	boundaries, this is usefull if you have used the fast functions which do not
	update boundary values. */
void k_GroupUpdateBoundaries(TPixelGroup* pPixelGroup);

/** @defgroup pixelgroupfast Fast functions, no updating of boundaries!
	Optimized functions, the programmer must be sure
	that the code is without bugs, no checks in these functions.
	@version 1.00
	@author René Dencker Eriksen edr@mip.sdu.dk
    @{ */

/** Same as k_RemovePosFromGroup, except that there is no range check for
	given index and the boundary is not	updated and hence the function is a
	lot faster.
	@return True, always.
	@see k_RemovePosFromGroup */
bool k_RemovePosFromGroupFast(unsigned int Index, TPixelGroup* pPixelGroup);

/**	Same as k_RemovePosFromGroup, except the boundary is not updated.
	Call k_GroupUpdateBoundaries() anytime to update boundaries.
	@param x (INT16) Position in horisontal direction.
	@param y (INT16) Position in vertical direction.
	@see k_AddPosToGroup
	@see k_GroupUpdateBoundaries */
void k_AddPosToGroupFast(INT16 x, INT16 y, TPixelGroup* pPixelGroup);

/**	Same as k_AddPosToGroupFast(), except the position is given in a T2DInt structure.
	@param Pos (T2DInt) Position to add to PixelGroup
	@see k_AddPosToGroupFast
	@see k_GroupUpdateBoundaries */
void k_Add2DPosToGroupFast(T2DInt Pos, TPixelGroup* pPixelGroup);

/** Same as k_InsertPosInGroup, except that there is no range check for
	given index and if colors are available it is ignored, the function
	simply inserts the position without referencing the color.
	Call k_GroupUpdateBoundaries() anytime to update boundaries.
	@param x (INT16) Position in horisontal direction.
	@param y (INT16) Position in vertical direction.
	@param Index Index in the array of positions to be removed.
	@return True, always.
	@see k_InsertPosInGroup
	@see k_GroupUpdateBoundaries */
bool k_InsertPosInGroupFast(INT16 x, INT16 y, unsigned int Index, TPixelGroup* pPixelGroup);

/** Same as k_InsertPosInGroupFast, except the position is given in a T2DInt structure.
	@param Pos (T2DInt) Position to add to PixelGroup.
	@return True, always.
	@see k_InsertPosInGroupFast
	@see k_GroupUpdateBoundaries */
bool k_Insert2DPosInGroupFast(T2DInt Pos, unsigned int Index, TPixelGroup* pPixelGroup);

/** Same as k_AddPosToGroupFast, except a color value is also inserted.
	@param x Position in horisontal direction.
	@param y Position in vertical direction. 
	@return False, if colors are not available, in that case the function does nothing.
	@see k_AddPosToGroupFast
	@see k_GroupUpdateBoundaries */
void k_AddPosColorToGroupFast(INT16 x, INT16 y, UINT32 Color, TPixelGroup* pPixelGroup);

/**	Same as k_AddPosColorToGroupFast(x,y), except the position is given in a T2DInt structure.
	@param Pos Position to add to PixelGroup.
	@return False, if colors are not available, in that case the function does nothing.
	@see k_GroupUpdateBoundaries */
void k_Add2DPosColorToGroupFast(T2DInt Pos, UINT32 Color, TPixelGroup* pPixelGroup);

/** Same as k_InsertPosInGroupFast(x,y), except a color value is also inserted.
	@param x Position in horisontal direction.
	@param y Position in vertical direction. 
	@return True, always.
	@see k_InsertPosInGroupFast
	@see k_GroupUpdateBoundaries */
bool k_InsertPosColorInGroupFast(INT16 x, INT16 y, UINT32 Color, unsigned int Index, TPixelGroup* pPixelGroup);

/**	Same as k_InsertPosColorInGroupFast(x,y), except the position is given in a T2DInt structure.
	@param Pos Position to add to PixelGroup.
	@return True, always.
	@see k_GroupUpdateBoundaries */
bool k_Insert2DPosColorInGroupFast(T2DInt Pos, UINT32 Color, unsigned int Index, TPixelGroup* pPixelGroup);

/** Adds an offset to all points in group. Boundaries are updated.
	@param Offset The offset value to be added to all positions.
	@return False, if the group is empty. */
bool k_GroupAddOffset(TPixelGroup* pPixelGroup,T2DInt Offset);

/*@}*/ /* end group "pixelgroupfast" */

/*@}*/ /* end group "pixelgroup" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_PIXELGROUP_H */
