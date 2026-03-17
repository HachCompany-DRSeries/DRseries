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

#ifndef _IPL98_PALETTE_H
#define _IPL98_PALETTE_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "../kernel_c/kernel_palette.h"
#include <ostream>

namespace ipl{ // use namespace if C++

using std::ostream;

/** @ingroup containerclasses
@{ */

/** CPalette handles all palette manipulation for images,
	last updated 12/29/2005.
	This class does not contain the palette data itself. The data is stored in
	the m_Image (TImage structure) member variable of the CStdImage class. The
	class is only intended to give some methods to manipulate a palette contained
	in an image (CImage or CByteImage), if you want to use it as a standalone class
	you must provide a pointer to its constructor.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CPalette ipl98/cpp/palette.h
	@version 1.23
	@author René Dencker Eriksen edr@mip.sdu.dk */
class CPalette{
	private: // attributes
		/// points to palette in image given to constructor
		TPalette* m_pPal;
		bool m_InternalTPalette; // used to check if the destructor must
								 // delete the TPalette structure
	public: // methods
		/** Default constructor, sets m_pPal to NULL and 
			m_InternalTPalette to true */
		CPalette();
		/** Creates a TPalette structure and sets m_pPal to point to this
			structure. The palette is initialised to gray tones. Sets
			m_InternalTPalette to true. The TPalette structure is deleted when
			the destructor is called.
			@param Entries Number of entries in palette, must be 2, 16 or 256. */
		CPalette(UINT16 Entries);
		/** Constructor, initializes the member m_pPal pointer to
			point to the "Pal"-variable in the TImage structure. Sets
			m_InternalTPalette to false;
			The destructor does not delete the structure pointed to by
			m_pPal, this should be done when the TImage structure which
			contains the TPalette structure is destroyed. */
		CPalette(TPalette* pPal);
		/// Copy constructor
		CPalette(const CPalette& Pal);
		/** The destructor only deletes the contents of m_pPal if it is an internal
			created variable (done by checking the m_InternalTPalette attribute),
			ie. the constructor CPalette(UINT16 Entries) was used. */
		~CPalette();
		/** overloading of assignment operator, deletes old palette in destination */
		CPalette& operator=(const CPalette& SourcePal);
		/** Sets the m_pPal attribute to point to the 
			given pointer pPal.
			@return False, if pPal is a NULL pointer. */
		bool SetPalettePtr(TPalette* pPal);
		/** Returns the pointer to palette data.
			@return pointer to m_pPal */
		TPalette* GetPalettePtr() const;
		/** Returns number of palette entries for the image
			@return number of palette entries */
		UINT16 GetNumberOfEntries() const;
		/**	Create a UINT32 RGB-component from the three 
			components r, g and b.
			@return A palette value containing all three r,b and b components. */
		inline static UINT32 CreateRGB(UINT8 r, UINT8  g, UINT8 b);
		/** Swaps the R and B value in a RGB, i.e. RGB -> BGR. */
		inline static void SwapRB(UINT32& Color);
		/** Get palette value (at Index) in the Color parameter.
			@return false if outside palette range */
		inline bool GetColor(int Index, UINT32& Color) const;
		/**	Returns the palette value at "Index". This method is the same
			as GetColor(Index,&Color) but the result is returned directly from the 
			method instead of in a parameter.
			@return UINT32 palette value at given index */
		inline UINT32 GetColor(int Index) const;
		/** Extracts the R-value from a UINT32 RGB-component (returned by 
			the GetColor() method in this class).
			@return UINT8 value of the red component. */
		inline static UINT8 GetRedVal(UINT32 rgb);
		/** Extracts the G-value from a UINT32 RGB-component (returned by 
			the GetColor() method in this class).
			@return UINT8 value of the green component. */
		inline static UINT8 GetGreenVal(UINT32 rgb);
		/** Extracts the B-value from a UINT32 RGB-component (returned by 
			the GetColor() method in this class).
			@return UINT8 value of the blue component. */
		inline static UINT8 GetBlueVal(UINT32 rgb);
		/** Sets palette value (at Index) to PalValue
			@return false if outside palette range */
		inline bool SetColor(int Index, UINT32 Color);
		/** Copies palette from another image. The palette data is copied
			from the pointer m_pPal in the "Pal" object to the address
			contained in m_pPal in this class.
			"Pal" is the member attribute of source image. */
		void CopyPalette(const CPalette& Pal);
	/** Returns true if the palette is a gray tone palette. In a gray tone
		palette each index value must have the same value in the R,G and B 
		components. For an image with 8 b/p at index M in the palette, the
		color channels must have R=M, G=M, and B=M. For an image with 1 b/p,
		index 0 must have R=0, G=0, and B=0 and index 1 must have R=255,
		G=255, and B=255.
		@return True, if palette is a gray tone palette. If palette contains
			a number of entries different from 2 or 256, false is returned. */
		bool IsGrayTone();
		/** Prints the information contained in the TPalette to stdout.
		@param EntriesPrLine Number of RGB entries to be written on each line
				(defaults to 4). */
		void PrintInfo(unsigned int EntriesPrLine=4);
		/** @name streaming operators */
		//@{
		/** Writes all the palette information to stream.
			Calls StreamIndent() for each streamed line. */
		friend ostream& operator<<(ostream& s,const CPalette& Pal);
		//@}
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

inline UINT32 CPalette::CreateRGB(UINT8 r, UINT8  g, UINT8 b)
{
	return k_PalCreateRGB(r, g ,b);
}

inline void CPalette::SwapRB(UINT32& Color)
{
//	UINT32 Val=Color;
	Color = ((Color << 16) & 0x00ff0000) | ((Color >> 16) & 0x000000ff) | (Color & 0x0000ff00);
//	Color = Color & 0x00ffffff;
//	Color & (Val & 0x0000ff00);
}

inline UINT8 CPalette::GetRedVal(UINT32 rgb)
{
	return k_PalGetRVal(rgb);
}

inline UINT8 CPalette::GetGreenVal(UINT32 rgb)
{
	return k_PalGetGVal(rgb);
}

inline UINT8 CPalette::GetBlueVal(UINT32 rgb)
{
	return k_PalGetBVal(rgb);
}

inline bool CPalette::GetColor(int Index, UINT32& Color) const
{
	return k_GetPalValue(Index,&Color,m_pPal);
}

inline UINT32 CPalette::GetColor(int Index) const
{
	return k_GetPalColor(Index,m_pPal);
}

inline bool CPalette::SetColor(int Index, UINT32 Color)
{
	return k_SetPalValue(Index,Color,m_pPal);
}

/*@}*/ /* end group "containerclasses" */

} // end namespace ipl

#endif //_IPL98_PALETTE_H
