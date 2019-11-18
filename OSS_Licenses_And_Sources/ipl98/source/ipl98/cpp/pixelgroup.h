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

#ifndef _IPL98_PIXELGROUP_H
#define _IPL98_PIXELGROUP_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "image.h"
#include "std_image.h"
#include "../kernel_c/kernel_pixelgroup.h"
#include "pixelgroup_it.h"
#include "pixelgroup_const_it.h"
#include <sstream>

namespace ipl{ // use namespace ipl

using std::ostringstream;

/** @ingroup containerclasses
@{ */

/** CPixelGroup, contains a group of pixels, it can optionally contain color
	information for each position, last updated 12/29/2005. One example of use
	is segmentation algorithms. The simple blob-algorithm
	found in the CSegmentate class uses several CPixelGroup objects to store
	the found segments in. The class CPixelGroups is used to keep track of many
	CPixelGroup objects. A template based group holding positions is also available,
	see CPositionGroup2D for more information.

	This class has two iterators: ConstIterator and Iterator. They work like the iterators
	in the C++ std-library, so if you are familiar with them, these are very easy to use.
	Here is an example:
	\verbatim
	CPixelGroup Grp;
	// Fill some pixels in the Grp object...

	// Search whole group for given positions
	// this could be done by calling PosInGroup(), but to demonstrate
	// the iterator class, we will do it this way
	CPoint2D<INT16> P(1500,388); // just a random position
	CPixelGroup::ConstIterator It;
	// iterating whole group with the ConstIterator class
	for(It=Grp.Begin();It<Grp.End();It++)
	{
		// The internal representation of positions in a CPixelGroup is
		// stored in a C struct T2DInt, but functions are available for
		// comparing CPoint2D<INT16> with these T2DInt, so no time consuming
		// conversion is needed. It means that the comparison 
		// '((P.GetX()==It->x) && (P.GetY()==It->y))' is not faster than 
		// 'P==*t', even though the last comparison is two different types.
		if (P==*It)
		{
			cout << "Found P" << endl;
			// If you explicitly need to convert the iterator
			// value to a CPoint2D<INT16>, the method ToCPoint2D() can do it
			cout << It.ToCPoint2D() << endl;
		}

	}
	// Doing the same without the iterator is considerably slower.
	// On a PENTIUM III with Visual C++ 6.0 it is about a factor 4 slower.
	for(int i=0; i<Grp.GetTotalPositions(); i++)
	{
		if (P==Grp.GetPosition(i))
			cout << "Found P" << endl;
	}

	\endverbatim

	For more details, see CPixelGroupIterator and CPixelGroupConstIterator
	classes.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CPixelGroup ipl98/cpp/pixelgroup.h
	@see CPixelGroups
	@see CPositionGroup2D
	@see CPositionGroupIterator
	@see CPositionGroupConstIterator
	@version 1.68,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
class CPixelGroup {
	public: // attributes
	private: // attributes
		TPixelGroup* m_pGroup;
		bool m_OwnGroup; // if true, the class contains its own TPixelGroup and must
						// remember to deallocate the memory when destructing
		// Setting for the streaming operator, number of positions per line
		unsigned int m_PosPrLine;
	public: // methods
		/// Default constructor
		CPixelGroup();
		/** Constructor allocating memory for pixel positions to be contained in this class.
			To make insertions fast, call this funcion with a value in InitialSize
			greater than the expected pixels in the group.
			@param InitialSize Number of positions to allocate memory for.
			@param AllocateColors If true, a color array is also allocated, defaults to false.
			Must be 1 or greater. */
		CPixelGroup(unsigned int InitialSize, bool AllocateColors=false);
		/** Constructor which takes an external TGroup as reference to data,
			in this case, the class does not deallocate memory for the TPixelGroup pointet
			to by m_pGroup.
			@param TGroup This class will be constructed with the contents of TGroup. */
		CPixelGroup(TPixelGroup& TGroup);
		/** Copy constructor 
			@param SourceGroup Group to copy into this group */
		CPixelGroup(const CPixelGroup& SourceGroup);
		/** overloading of assignment operator, deletes old pixel positions in destination.
			Also deletes the color values if available. Calls the kernel function k_CopyGroup(). */
		CPixelGroup& operator=(const CPixelGroup& SourceGroup);
		/** overloading of []-operator. Returns a reference to the position at Index, no range
			check, fast access. Note, this operator returns a T2DInt kernel C structure to avoid
			allocation of a CPoint2D<INT16> as in GetPosition() and hence make the access faster.
			The T2DInt structure contains attributes x and y of type INT16.
			A corresponding const version of this operator is also available.
			@see GetPosition */
		inline T2DInt& operator[](unsigned int Index);
		/** overloading of []-operator. Returns the position at Index, no range
			check, fast access. Note, this operator returns a T2DInt kernel C structure to avoid
			allocation of a CPoint2D<INT16> as in GetPosition() and hence make the access faster.
			The T2DInt structure contains attributes x and y of type INT16.
			@see GetPosition */
		inline const T2DInt& operator[](unsigned int Index) const;
		/// default destructor
		~CPixelGroup();
		/** Sets all members to 0 and deallocates memory */
		void Empty();
		/** Same as Empty(), except the allocated memory to store positions
			in, is not deallocated. Memory allocated for Colors (if available) is also
			left unchanged. This can optimize performance if the group
			is to be used again.
			@see Empty
			@version 0.7 */
		void EmptyNoDeallocation();
		/** @name Iterator functionality
			Iterators has been added to standardize the way to iterate through pixel
			values in the CPixelGroup class. To learn about iterators, look at the
			standard library iterators. Implemented iterators in the pixelgroup class are
			iterators and const iterators. Both iterators have ++ and -- (both postfix and prefix)
			operators and the comparison operators ==, != and <. 
			Typedefs are available in this class for each iterator type, just like 
			the standard library. Since this class builds on top of an ANSI C structore
			(T2DInt), the dereferencing (operator*) does not provide you with a
			CPoint2D<int> as one could expect. Instead you are returned a T2DInt structure,
			where the public members are x and y. If you need to work with CPoint2D<INT16>,
			use the method ToCPoint2D on the iterator classes.
			Here is a short example on using an iterator:
			\verbatim

			CPixelGroup Grp;
			Grp.AddPosition(1,2);
			Grp.AddPosition(0,0);
			CPixelGroup::ConstIterator it3;
			CPixelGroup::Iterator it=Grp.Begin();
			it3=it; // converting it to a const iterator
			it3++; // increment it3
			if (it<it3) // comparing two iterators
			{
				// streaming the CPoint2D<INT16> value of the iterator
				cout << it.ToCPoint2D() << endl;
			}
			\endverbatim
		*/
		//@{ 
		/** Iterator for the CPixelGroup class. This typedef is included to make
			the class work like the std library. Instantiate an iterator object
			like this:
			\verbatim
			CPixelGroup::Iterator it;
			\endverbatim */
		typedef CPixelGroupIterator Iterator;
		/** ConstIterator for the CPixelGroup class. This typedef is included to make
			the class work like the std library. Instantiate an iterator object
			like this:
			\verbatim
			CPixelGroup::ConstIterator it;
			\endverbatim */
		typedef CPixelGroupConstIterator ConstIterator;
		/** Returns an iterator to the last element plus one.
			@return Last element plus one. */
		inline Iterator End(){return Iterator(m_pGroup->pPos+m_pGroup->NumberOfPixels);}
		/** Returns a const iterator to the last element plus one.
			@return Last element plus one. */
		inline ConstIterator End() const{return ConstIterator(m_pGroup->pPos+m_pGroup->NumberOfPixels);}
		/** Returns an iterator to the first element.
			@return First element. */
		inline Iterator Begin(){return Iterator(m_pGroup->pPos);}
		/** Returns a const iterator to the first element.
			@return First element. */
		inline ConstIterator Begin() const{return ConstIterator(m_pGroup->pPos);}
		//@}
		/** Get position with lowest y-value in group.
			@return Top position in group. */
		CPoint2D<int> GetTop() const;
		/** Get position with highest y-value in group.
			@return Bottom position in group. */
		CPoint2D<int> GetBottom() const;
		/** Get position with lowest x-value in group.
			@return Leftmost position in group. */
		CPoint2D<int> GetLeft() const;
		/** Get position with highest x-value in group.
			@return Rightmost position in group. */
		CPoint2D<int> GetRight() const;
		/** Get number of positions stored in the group. To iterate through
			all positions in the group use indexes from 0 to GetTotalPostitions()-1.
			@return Total number of positions. */
		inline unsigned int GetTotalPositions() const;
		/** Returns the width of this group. Note: Uses the boundary information,
			be sure they are updated. They are only invalid if you have used the
			fast methods, such as AddPositionFast().
			@return Width of group. */
		inline unsigned int GetWidth() const;
		/** Returns the height of this group. Note: Uses the boundary information,
			be sure they are updated. They are only invalid if you have used the
			fast methods, such as AddPositionFast().
			@return Height of group. */
		inline unsigned int GetHeight() const;
		/** Returns the color type, HIGHCOLOR if the group is derived from high color pixels
			otherwise the value is LOWCOLOR. If not defined, the value should be UNDEFINED.
			Note, that this is an overall value for the whole pixel group, not to be confused
			with colors available for each pixel position.
			Be aware, that this value may not be used by all segmentation algorithms.
			@return The color type from which the positions were derived. */
		COLORTYPE GetColorType() const;
		/** Sets the color type of this group, in some cases it can be convenient to change
			LOWCOLOR/HIGHCOLOR value to the opposite to be used by algorithms working on a 
			CPixelGroup.
			@see GetColorType */
		void SetColorType(COLORTYPE Color);
		/** Returns the position stored at "Index" in the group. Maximum index is
			GetTotalPostitions()-1.
			@return Position at given index, returns (0,0) if Index is out of range. */
		inline CPoint2D<INT16> GetPosition(unsigned int Index) const;
		/** Returns the last element in the group, i.e. position with max index
			defined as GetTotalPostitions()-1.
			@return Position at last index. */
		inline CPoint2D<INT16> GetBack() const;
		/** Returns the position at the middle index in this group, that is the
			position with index GetTotalPositions()/2. If total positions is an
			even number, the highest index of the two middle indexes are chosen.
			@return Position at middle index, rounded up if total positions is an even number. */
		inline CPoint2D<INT16> GetMiddle() const;
		/** Returns the color corresponding to the position stored at "Index" in the group.
			Maximum index is GetTotalPostitions()-1. There may not be any colors stored
			in the group, use ColorsAvailable() to check this.
			@return Position at given index, returns 0 if index is out of range
				or if colors are unavailable.
			@see operator[] */
		inline UINT32 GetColor(unsigned int Index) const;
		/** Sets the color ColorValue corresponding to the position stored at "Index" in
			the group. Maximum index is GetTotalPostitions()-1. There may not be any
			colors stored in the group, use ColorsAvailable() to check this.
			In order to allocate colors (which must be done before using this method)
			call AllocColors(), AllocAndInitColors() or AddColors(Img).
			@return False, if index is out of range or if colors are unavailable.
			@see AllocColors
			@see AllocAndInitColors
			@see AddColors */
		inline bool SetColor(UINT32 ColorValue, unsigned int Index);
		/** Use this method to check if colors are stored in the pixel group.
			Colors can be allocated in a number of ways: By calling AllocColors() or 
			AllocAndInitColors() or by calling AddColors(Img).
			@return True, if there are colors stored in the group.
			@see AllocColors
			@see AllocAndInitColors
			@see AddColors */
		bool ColorsAvailable() const;
		/** Reallocates memory for the positions contained in this class. If colors 
			are available the color array will also be adjusted. The new allocated
			memory is not initialised. Calling AddPosition() and AddPositionFast()
			will be faster if memory has been preallocated by calling this method.
			@param NewSize The new number of positions to allocate memory for.
				Note: The NewSize may be less than the actual allocated memory! In that case
				the corresponding positions stored in the end of the actual memory are deleted. */
		void ReAllocPositions(unsigned int NewSize);
		/** Removes unused memory allocated for pixel positions in this class.
			If colors are available the color array will also be adjusted.
			Because memory allocation is a time consuming operation it is a good idea by each
			ReAllocPositions (or the CPixelGroup(unsigned int InitialSize) constructor) call to
			allocate a big number of pixels and then at the end of the construction of a
			CPixelGroup calling this AdjustSizeOfPositions() method to deallocate unused memory. */
		void AdjustSizeOfPositions();
		/** To use the kernel C functions it is sometimes necessary
			to pass the TPixelGroup structure.
			In other cases do not use this function to manipulate the 
			pixel group but use the methods in this class!
			@return pointer to pixel group data */
		const TPixelGroup* GetConstTPixelGroupPtr() const;
		/** To use the kernel C functions it is sometimes necessary
			to pass the TPixelGroup structure which contents will be changed.
			In other cases do not use this function to manipulate the 
			pixel group but use the methods in this class!
			@return pointer to pixel group data */
		TPixelGroup* GetTPixelGroupPtr();
		/* Only used by the class CPixelGroups */
		void MoveTPixelGroupPtr(TPixelGroup* p);
		/** The position (x,y) is added to the group of pixels. If pixel colors are 
			present this function cannot be used. A call to RemoveColors() is then
			needed. The method AddColorsToGroup() can be called later to insert the 
			colors again if a source image is available. The boundary is updated if
			the new position is outside the old boundary. To make this method faster,
			the method ReAllocPositions() can be called ahead.
			@param x Position in horisontal direction.
			@param y Position in vertical direction. 
			@return False, if colors are available, in that case the method does nothing.
			@see AddPositionFast
			@see ReAllocPositions */
		inline bool AddPosition(INT16 x, INT16 y);
		/**	Same as AddPosition(x,y), except the position is given in a CPoint2D object.
			@param Pos Position to add to PixelGroup.
			@see AddPositionFast
			@see ReAllocPositions */
		inline bool AddPosition(const CPoint2D<INT16>& Pos);
		/** Inserts the position (x,y) at given Index, which must be less than the
			inserted pixels (returned by GetTotalPositions()). The ordering of
			the positions is preserved. If pixel colors are present this function cannot
			be used. A call to RemoveColors() is then needed. The method AddColorsToGroup()
			can be called later to insert the colors again if a source image is available.
			The boundary is updated if the new position is outside the old boundary.
			@param x Position in horisontal direction.
			@param y Position in vertical direction.
			@param Index Index in the array of positions to be inserted.
			@return False, if pixel colors are present or if Index is out of range.
			@see InsertPositionFast */
		bool InsertPosition(INT16 x, INT16 y, unsigned int Index);
		/** Same as InsertPosition(x,y,Index), except the position is given in a
			CPoint2D<INT16> type.
			@param Pos Position to add to the PixelGroup.
			@param Index Index in the array of positions to be inserted.
			@return False, if pixel colors are present or if Index is out of range.
			@see InsertPositionFast */
		bool InsertPosition(CPoint2D<INT16> Pos, unsigned int Index);
		/** Same as AddPosition(x,y), except a color value is also inserted.
			@param x Position in horisontal direction.
			@param y Position in vertical direction. 
			@param Color Pixel value to be associated with position.
			@return False, if colors are not available, in that case the method does nothing.
			@see AddPositionColorFast */
		inline bool AddPositionColor(INT16 x, INT16 y, UINT32 Color);
		/**	Same as AddPositionColor(x,y), except the position is given in a CPoint2D object.
			@param Pos Position to add to PixelGroup.
			@param Color Pixel value to be associated with position.
			@see AddPositionColorFast */
		inline bool AddPositionColor(const CPoint2D<INT16>& Pos, UINT32 Color);
		/** Same as InsertPosition(x,y), except a color value is also inserted.
			@param x Position in horisontal direction.
			@param y Position in vertical direction. 
			@param Color Pixel value to be associated with position.
			@param Index Index in the array of positions to be inserted.
			@return False, if colors are not available, in that case the method does nothing.
			@see InsertPositionColorFast */
		bool InsertPositionColor(INT16 x, INT16 y, UINT32 Color, unsigned int Index);
		/**	Same as InsertPositionColor(x,y), except the position is given in a CPoint2D object.
			@param Pos Position to add to PixelGroup.
			@param Color Pixel value to be associated with position.
			@param Index Index in the array of positions to be inserted.
			@return False, if colors are not available, in that case the method does nothing.
			@see AddPositionColorFast */
		bool InsertPositionColor(const CPoint2D<INT16>& Pos, UINT32 Color, unsigned int Index);
		/** Removes a position stored in the group at given index. Done by moving the
			last position in the pPos array contained in the m_pGroup (type TPixelGroup)
			to the index which is to be removed. If pixel colors are available, the color
			array is updated. The boundary is also updated if the removed position causes
			changes in the boundary.
			@param Index Index in the array of position to be removed.
			@return False, if Index is out of range.
			@see RemovePositionSlow */
		bool RemovePosition(unsigned int Index);
		/** Same as RemovePosition, but the ordering of the pixels is preserved.
			This method is slower than RemovePosition.
			@param Index Index in the array of position to be removed.
			@return False, if Index is out of range.
			@see RemovePosition */
		bool RemovePositionSlow(unsigned int Index);
		/** Removes duplicates of same position in this pixelgroup. The algorithm could
			probably be more efficient but it is still nice to have this functionality.
			To make it a bit efficient though, the order of the positions is not preserved, i.e.
			the same code as RemovePosition is used to remove positions, but no update of
			boundary is neccesary.
			@return Total number of duplicate positions removed. */
		unsigned int RemoveDuplicates();
		/** Allocates cells for color values but does not initialize the values.
			Faster than using AllocAndInitColors(). Note: If you later on call AddPosition()
			or InsertPosition() without adding a color, memory for previous allocated colors
			are deleted since the number of allocated colors does not equal number of positions.
			The class takes care of it.
			@return False, if colors are already available.
			@see AllocAndInitColors */
		bool AllocColors();
		/** Same as AllocColors but here the initial value of the allocated
			cells can be specified. Note: If you later on call AddPosition()
			or InsertPosition() without adding a color, memory for previous allocated colors
			are deleted since the number of allocated colors does not equal number of positions.
			The class takes care of it.
			@param InitialValue Initial value of allocated colors.
			@return False, if colors are already available.
			@see AllocColors */
		bool AllocAndInitColors(UINT32 InitialValue);
		/**	Get the original pixel colors from the inserted pixels and given source image and
			insert them into the PixelGroup. Note: If you later on call AddPosition()
			or InsertPosition() without adding a color, memory for previous allocated colors
			are deleted since the number of allocated colors does not equal number of positions.
			The class takes care of it.
			@param Img Source image to get the original pixel values from. The PixelGroups positions
				must be derived from this image.
			@return False if the PixelGroup has no pixels in it, colors have already been added
				or one of the parameters is a NULL pointer. */
		bool AddColors(const CStdImage& Img);
		/** Removes colors stored in the m_pGroup member attribute. If no colors are present,
			the method just returns without doing anything. */
		void RemoveColors();
		/** Copies a pixel group to a new image. Origo of the new image is set to (-Left.x,-Top.y)
			which is the pixelgroups top left boundary. There are three cases which decides the
			bit depth of the output image: 1) No colors available (i.e. the method
			AddColors() has not been called), then a black and white image with
			1 b/p is produced and the positions are plotted in the image according to the
			"Color" member of the TPixelGroup-structure. If the Color-type is undefined
			the default is a white background with black pixels set at the given
			positions. 2) Colors available but no palette is given (i.e. Palette is a NULL
			pointer), then an image with a graytone palette is produced if the Bits pixel
			depth member of CPixelGroup is 1 or 8. If the pixel depts is 24 b/p
			an image with 24 b/p without a palette is produced. 3) Colors available and
			a palette is provided. An image with pixel depth given in CPixelGroup and the given
			palette is produced. The pallette must correspond with the pixel depth in the
			TPixelGroup, otherwise an error message will be produced.
			@param pPalette The pallette to be used in the new image. If no colors are available
				or the pixel depth is 24 b/p this parameter is ignored. If the parameter is NULL
				a default palette is used (see case 2 above).
			@param BackGround The background color of new image, the user must be sure that
				the value in case of 1 or 8 b/p is in the range of the palette index and in
				case of 24 b/p the value must be the RGB color.
			@param Dest Image to copy the group to. All previous information in Dest is
				destroyed.
			@return False, if the pixel group is empty. */
		bool CopyToImage(const CPalette* pPalette, UINT32 BackGround, CStdImage& Dest) const;

		/** Same as the other version of CopyToImage() (different parameters), 
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
				24 b/p. */
		bool CopyToImage(UINT32 BackGround, UINT32 ForeGround, UINT16 Bits, CStdImage& Dest) const;

		/** Adds the points in PixelGroup to the already allocated image, only the positions contained
			in the pixelgroup will affect the image. Positions outside image are ignored. If the
			pixelgroup contains color values they can be used as plotting values by calling
			the method AddColorsToImage(CStdImage& Dest) instead. The method automaically does the
			copying as fast as possible by doing some range checks before entering the inner loops.
			@param Dest Image to plot points in, only pixels at positions contained in pixelgroup
				will affect the image.
			@param Color A UINT32 value containing the color, in case of a palette it is the index
				in the palette. If the group contains colors, you can use them by calling
				AddToImage(CStdImage& Dest) instead of this function.
			@return True, always when using this C++ version of the algorithm. */
		bool AddToImage(CStdImage& Dest, UINT32 Color) const;
		/** Adds the points in PixelGroup to the already allocated image, only the positions
			contained in the pixelgroup will affect the image. Positions outside image are ignored.
			The color values in the pixelgroup are used when inserting postitions, if you want
			the positions plotted with a constant color call the method 
			AddToImage(CStdImage& Dest, UINT32 Color). Ensure that the bit depth (and palette if
			present) complies with the color values in the pixelgroup. The method will not do any
			tests for that. The function automaically does the copying as fast as possible by
			doing some range checks before entering the inner loops.
			@param Dest Image to plot points in, only pixels at positions contained in pixelgroup
				will affect the image.
			@return False, if pPixelgroup is NULL.
			@see True, always when using this C++ version of the algorithm. */
		bool AddToImage(CStdImage& Dest) const;
		/**	Tests if given position is included in the PixelGroup
			@param Pos Position to test for.
			@param Index If the position "Pos" is found, the Index of the position is returned
				in this parameter. If not found, the parameter is unchanged.
			@return true if the position was found, false if not found. */
		bool PosInGroup(const CPoint2D<INT16>& Pos,unsigned int& Index) const;
		/** Same as PosInGroup, except the position is given as two INT16
			values (x,y).
			@return true if the position was found, false if not found. */
		bool PosInGroup(INT16 x, INT16 y,unsigned int& Index) const;
		/**	Tests if given position is included in the PixelGroup. If you want
			the index with the position returned, see 
			PosInGroup(const CPoint2D<INT16>& Pos,unsigned int& Index).
			@param Pos Position to test for.
			@return true if the position was found, false if not found. */
		bool PosInGroup(const CPoint2D<INT16>& Pos) const;
		/** Same as PosInGroup(Pos), except the position is given as two INT16
			values (x,y).
			@return true if the position was found, false if not found. */
		bool PosInGroup(INT16 x, INT16 y) const;
		/**	Prints the information contained in the PixelGroup to stdout. This
			is an old method (only kept for backwards compatibility) for
			retrieving information about the group. It is recommended to use
			GetInfo() instead, or use the streaming operator. Internally, this
			method calls the kernel C function k_PrintGroupInfo(). Format is:
			\verbatim
			**************** PixelGroup info ********************
			Positions: Top=(-10,-10) Bottom=(11,17) Left=(-15,8) Right=(43,-5)
			Width=59 Height=28
			NumberOfPixels=118 AllocatedPixels=128
			pos 0=(-10,-10) pos 1=(-9,11) ...
			\endverbatim
			where the positions are split into lines with PosPrLine positions on each line.
			@param PrintPositions If true, all pixel positions stored in the PixelGroup are written
				to stdout.
			@param PosPrLine Number of positions to be written on each line
				(defaults to 5). Only used if PrintPositions is true.
			@see GetInfo(). */
		void PrintInfo(bool PrintPositions,unsigned int PosPrLine=5) const;
		/** Same as streaming operator, except that the parameter PrintPositions
			decides whether positions should be written to provided stream. This
			method is also very similar to the PrintInfo(), which is only kept for
			backwards compatibility.
			Calls StreamIndent() for each streamed line.
			@param ost Information is written to this stream.
			@param PrintPositions If true, all pixel positions stored in the PixelGroup are written
				to stdout.
			@param PosPrLine Number of positions to be written on each line
				(defaults to 5). Only used if PrintPositions is true.*/
		void GetInfo(ostream& ost, bool PrintPositions, unsigned int PosPrLine=5) const;
		/**@name streaming operators */
		//@{
		/** Controls the number of positions per line when using the streaming
			operator. The default value is 9999.
			@param PosPrLine Number of positions per line, must be greater than 0. */
		void StreamSettings(unsigned int PosPrLine);
		/** writes object to stream, format is:
		\verbatim
		
		CPixelGroup  NumberOfPixels: 7 Width: 4 Height: 9 Colors available: false
		  (1,2) (4,5) (4,6) (4,7) (4,8) (4,9) (4,10)
		\endverbatim
		or if colors are available:
		\verbatim
		
		CPixelGroup  NumberOfPixels: 7 Width: 4 Height: 9 Colors available: true
		  (1,2) 0 (4,5) 112 (4,6) 87 (4,7) 1 (4,8) 248 (4,9) 252 (4,10) 255
		\endverbatim
		Boundaries are automatically updated after loading all positions.
		The number of positions per line can be set by calling StreamSettings()
		príor to using the streaming operator.
		Calls StreamIndent() for each streamed line.
		@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
		@version 0.11 */
		friend std::ostream& operator<<(std::ostream& s,const CPixelGroup& Group);
		/** Reads input from given stream into this class. Stream must be in 
			correct format according to how the << ostream operator formats output.
			@author Implementation by René Dencker Eriksen (edr@mip.sdu.dk)
			@version 0.1 */
		friend std::istream& operator>>(std::istream& is, CPixelGroup& Group);
		//@}
		
		/** Runs through all positions in the group and update the left/right/top/bottom
			boundaries, this is usefull if you have used the fast methods which do not
			update boundary values. */
		void UpdateBoundaries();

		/** @name Fast functions, no updating of boundaries! */
		//@{
		/** Same as RemovePosition, except that there is no range check for
			given index and the boundary is not	updated and hence the function is a
			lot faster.
			@return True, always.
			@see RemovePosition */
		inline bool RemovePositionFast(unsigned int Index);
		
		/**	Same as AddPosition, except the boundary is not updated. It is not
			checked if the colors are available, i.e. it is not legal later on
			to access the colors eventhough they seem to be available! You can
			manually later on call RemoveColors() and AllocColors() to update
			the array, but earlier values will be lost. To make this method
			faster, the method ReAllocPositions() can be called ahead.
			@param x Position in horisontal direction.
			@param y Position in vertical direction.
			@see AddPosition
			@see ReAllocPositions */
		inline void AddPositionFast(INT16 x, INT16 y);

		/**	Same as AddPositionFast(x,y), except the position is given in a
			CPoint2D<INT16> type.
			@param Pos Position to add to PixelGroup
			@see AddPosition
			@see ReAllocPositions */
		inline void AddPositionFast(const CPoint2D<INT16>& Pos);

		/** Same as InsertPosition, except that there is no range check for
			given index and if colors are available it is ignored, the function
			simply inserts the position without referencing the color.
			@param x Position in horisontal direction.
			@param y Position in vertical direction.
			@param Index Index in the array of positions to be inserted.
			@return True, always.
			@see InsertPosition */
		inline bool InsertPositionFast(INT16 x, INT16 y, unsigned int Index);

		/** Same as InsertPositionFast, except the position is given in a CPoint2D<INT16> type.
			@param Pos Position to add to PixelGroup.
			@param Index Index in the array of positions to be inserted.
			@return True, always.
			@see InsertPositionFast */
		inline bool InsertPositionFast(CPoint2D<INT16> Pos, unsigned int Index);

		/** Same as AddPositionFast, except a color value is also inserted.
			@param x Position in horisontal direction.
			@param y Position in vertical direction. 
			@param Color Pixel value to be associated with position.
			@see AddPositionFast */
		inline void AddPositionColorFast(INT16 x, INT16 y, UINT32 Color);
		/**	Same as AddPositionColorFast(x,y), except the position is given in a CPoint2D object.
			@param Pos Position to add to PixelGroup.
			@param Color Pixel value to be associated with position.
			@see AddPositionFast */
		inline void AddPositionColorFast(const CPoint2D<INT16>& Pos, UINT32 Color);
		/** Same as InsertPosition(x,y), except a color value is also inserted.
			@param x Position in horisontal direction.
			@param y Position in vertical direction. 
			@param Color Pixel value to be associated with position.
			@param Index Index in the array of positions to be inserted.
			@return True, always.
			@see InsertPositionColorFast */
		inline bool InsertPositionColorFast(INT16 x, INT16 y, UINT32 Color, unsigned int Index);
		/**	Same as InsertPositionColor(x,y), except the position is given in a CPoint2D object.
			@param Pos Position to add to PixelGroup.
			@param Color Pixel value to be associated with position.
			@param Index Index in the array of positions to be inserted.
			@return True, always.
			@see AddPositionColorFast */
		inline bool InsertPositionColorFast(const CPoint2D<INT16>& Pos, UINT32 Color, unsigned int Index);
		/** Adds an offset to all points in group. Boundaries are updated.
			@param Offset The offset value to be added to all positions.
			@return False, if the group is empty. */
		bool AddOffset(const CPoint2D<INT16>& Offset);
		//@}

};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

inline T2DInt& CPixelGroup::operator[](unsigned int Index)
{
	return m_pGroup->pPos[Index];
}

inline const T2DInt& CPixelGroup::operator[](unsigned int Index) const
{
	return m_pGroup->pPos[Index];
}

inline unsigned int CPixelGroup::GetTotalPositions() const
{
	return m_pGroup->NumberOfPixels;
}

inline unsigned int CPixelGroup::GetWidth() const
{
	return m_pGroup->Right.x-m_pGroup->Left.x+1;
}

inline unsigned int CPixelGroup::GetHeight() const
{
	return m_pGroup->Bottom.y-m_pGroup->Top.y+1;
}

inline CPoint2D<INT16> CPixelGroup::GetPosition(unsigned int Index) const
{
	if (Index<m_pGroup->NumberOfPixels)
		return CPoint2D<INT16>(m_pGroup->pPos[Index].x,m_pGroup->pPos[Index].y);
	else
	{
		ostringstream ost;
		ost << "CPixelGroup::GetPosition() Index(" << Index << ") out of range" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return CPoint2D<INT16>(0,0);
	}
}

inline CPoint2D<INT16> CPixelGroup::GetBack() const
{
	return CPoint2D<INT16>(m_pGroup->pPos[m_pGroup->NumberOfPixels-1].x,m_pGroup->pPos[m_pGroup->NumberOfPixels-1].y);
}

inline CPoint2D<INT16> CPixelGroup::GetMiddle() const
{
	unsigned int Index=m_pGroup->NumberOfPixels/2;
	return CPoint2D<INT16>(m_pGroup->pPos[Index].x,m_pGroup->pPos[Index].y);
}

inline UINT32 CPixelGroup::GetColor(unsigned int Index) const
{
	if ((m_pGroup->pColor!=NULL) && (Index<m_pGroup->NumberOfPixels))
		return m_pGroup->pColor[Index];
	else
	{
		ostringstream ost;
		ost << "CPixelGroup::GetColor() Index(" << Index << ") out of range or colors not available" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
}

inline bool CPixelGroup::SetColor(UINT32 ColorValue, unsigned int Index)
{
	if ((m_pGroup->pColor!=NULL) && (Index<m_pGroup->NumberOfPixels))
	{
		m_pGroup->pColor[Index]=ColorValue;
		return true;
	}
	else
	{
		ostringstream ost;
		ost << "CPixelGroup::SetColor() Index(" << Index << ") out of range or colors not available" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
}

inline bool CPixelGroup::AddPosition(INT16 x, INT16 y)
{
	if (k_AddPosToGroup(x,y,m_pGroup)!=true)
	{
		ostringstream ost;
		ost << "CPixelGroup::AddPosition() Colors available, cannot insert position only" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
		return true;
}

inline bool CPixelGroup::AddPosition(const CPoint2D<INT16>& Pos)
{
	if (k_AddPosToGroup(Pos.GetX(),Pos.GetY(),m_pGroup)!=true)
	{
		ostringstream ost;
		ost << "CPixelGroup::AddPosition() Colors available, cannot insert position only" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
		return true;	
}

inline bool CPixelGroup::AddPositionColor(INT16 x, INT16 y, UINT32 Color)
{
	if (k_AddPosColorToGroup(x,y,Color,m_pGroup)!=true)
	{
		ostringstream ost;
		ost << "CPixelGroup::AddPositionColor() Colors not available" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
		return true;
}

inline bool CPixelGroup::AddPositionColor(const CPoint2D<INT16>& Pos,UINT32 Color)
{
	if (k_AddPosColorToGroup(Pos.GetX(),Pos.GetY(),Color,m_pGroup)!=true)
	{
		ostringstream ost;
		ost << "CPixelGroup::AddPositionColor() Colors not available" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	else
		return true;
}

inline bool CPixelGroup::RemovePositionFast(unsigned int Index)
{
	return k_RemovePosFromGroupFast(Index,m_pGroup);
}

inline void CPixelGroup::AddPositionFast(INT16 x, INT16 y)
{
	k_AddPosToGroupFast(x,y,m_pGroup);
}

inline void CPixelGroup::AddPositionFast(const CPoint2D<INT16>& Pos)
{
	k_AddPosToGroupFast(Pos.GetX(),Pos.GetY(),m_pGroup);
}

inline void CPixelGroup::AddPositionColorFast(INT16 x, INT16 y, UINT32 Color)
{
	k_AddPosColorToGroupFast(x,y,Color,m_pGroup);
}

inline void CPixelGroup::AddPositionColorFast(const CPoint2D<INT16>& Pos, UINT32 Color)
{
	k_AddPosColorToGroupFast(Pos.GetX(),Pos.GetY(),Color,m_pGroup);
}

inline bool CPixelGroup::InsertPositionFast(INT16 x, INT16 y, unsigned int Index)
{
	return k_InsertPosInGroupFast(x,y,Index,m_pGroup);
}

inline bool CPixelGroup::InsertPositionFast(CPoint2D<INT16> Pos, unsigned int Index)
{
	return k_InsertPosInGroupFast(Pos.GetX(),Pos.GetY(),Index,m_pGroup);
}

bool CPixelGroup::InsertPositionColorFast(INT16 x, INT16 y, UINT32 Color, unsigned int Index)
{
	return k_InsertPosColorInGroupFast(x,y,Color,Index,m_pGroup);
}

bool CPixelGroup::InsertPositionColorFast(const CPoint2D<INT16>& Pos, UINT32 Color, unsigned int Index)
{
	return k_InsertPosColorInGroupFast(Pos.GetX(),Pos.GetY(),Color,Index,m_pGroup);
}

/*@}*/ /* end group "containerclasses" */

} // end namespace ipl

#endif //_IPL98_PIXELGROUP_H
