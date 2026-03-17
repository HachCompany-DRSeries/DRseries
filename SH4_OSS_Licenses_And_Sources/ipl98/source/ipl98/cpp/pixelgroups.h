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

#ifndef _IPL98_PIXELGROUPS_H
#define _IPL98_PIXELGROUPS_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "../kernel_c/kernel_pixelgroups.h"
#include "pixelgroup.h"
#include <vector>

namespace ipl{ // use namespace ipl

using std::vector;

/** @ingroup containerclasses
@{ */

/** CPixelGroups contains several CPixelGroup classes, last updated 12/29/2005.
	Keeps track of more than one pixel group (CPixelGroup). Most images consists of more
	than one pixel group so this structure is indeed useful.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CPixelGroups ipl98/cpp/pixelgroups.h
	@see CPixelGroup
	@version 1.35,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */

class CPixelGroups {
	private: // attributes
		/// The kernel C structure containing information about all groups
		TPixelGroups m_Groups;
		/// Contains a list of CPixelGroups corresponding to m_Groups, internal use
		vector<CPixelGroup*> m_pCGroups;
	protected: // attributes
	public: // methods
		/// Default constructor
		CPixelGroups();
		/** Allocates memory for "InitialSize" CPixelGroup classes.
			@param InitialSize Number of CPixelGroup classes
				to allocate memory for. Must be 1 or greater. */
		CPixelGroups(unsigned int InitialSize);
		/** copy constructor 
			@param SourceGroups The CPixelGroups object to be copied into this object */
		CPixelGroups(const CPixelGroups& SourceGroups);
		/** overloading of assignment operator, deletes old groups contained in destination. */
		CPixelGroups& operator=(const CPixelGroups& SourceGroups);
		/** overloading of []-operator. Returns a reference to the group at Index, no range
			check, fast access. 
			@see GetGroup */
		inline CPixelGroup& operator[](unsigned int Index);
		/** overloading of []-operator. Returns a const reference to the group at Index, no range
			check, fast access. 
			@see GetGroup */
		inline const CPixelGroup& operator[](unsigned int Index) const;
		/// Default destructor
		~CPixelGroups();
		/** Sets all members to 0 and deallocates memory for all contained
			CPixelGroup objects. */
		void Empty();
		/** Returns the total number of groups contained in this class. To iterate through
			all groups use indexes from 0 to GetTotalGroups()-1.
			@return Total groups. */
		unsigned int GetTotalGroups() const;
		/** Returns a pointer to the group at given index. All changes made to the
			contents of this pointer affects the group stored in this class.
			@param Index Group to access.
			@return A pointer to the group at "Index", NULL if out of range.
			@see operator[] */
		CPixelGroup* GetGroup(unsigned int Index) const;
		/** To use the kernel C functions it is sometimes necessary
			to pass the TPixelGroups structure.
			In other cases do not use this function to manipulate the 
			pixel groups but use the methods in this class!
			@return pointer to pixel groups data */
		const TPixelGroups* GetConstTPixelGroupsPtr() const;
		/** To use the kernel C functions it is sometimes necessary
			to pass the TPixelGroups structure which contents will be changed.
			In other cases do not use this function to manipulate the 
			pixel groups but use the methods in this class!
			@return pointer to pixel groups data */
		TPixelGroups* GetTPixelGroupsPtr();
		/** When an ANSI C kernel function which inserts TPixelGroup structures into
			the TPixelGroups structure on its own, call this function to update
			the m_pCGroups member in this class. */
		void Update();
		/** The PixelGroup parameter is added to this class. Note: All
			memory allocated in the PixelGroup class parameter is copied to a new structure,
			you must remember to free the memory in PixelGroup.
			@param PixelGroup PixelGroup to be added. */
		void AddGroup(const CPixelGroup& PixelGroup);
		/** Inserts PixelGroup at given Index. The ordering of the groups
			is preserved.
			@param PixelGroup PixelGroup to be inserted.
			@param Index Index in the array of groups to insert at.
			@return False, if Index is out of range. */
		bool InsertGroupInGroups(const CPixelGroup& PixelGroup, unsigned int Index);
		/** After the creation of groups contained in CPixelGroups this method will get the original
			pixel colors from given source image and insert them into all the groups contained
			in CPixelGroups.
			@param Source Image to get the original pixel values from. The PixelGroups positions
				must be derived from this image.
			@return False if the CPixelGroups structure contains no groups or one of the
				parameters is a NULL pointer. */
		bool AddColors(const CStdImage& Source);
		/** Removes a CPixelGroup from this CPixelGroups class. Done by moving the last group
			in the pGroups array contained in the m_Groups member to the index which is to
			be removed. All memory allocated for the group to be removed are freed. Note: When
			removing an element at index GroupIndex in a loop which runs through all indexes,
			remember to iterate at GroupIndex again. Otherwise you will miss a group!
			@param GroupIndex Index of group in the m_Groups member of type TPixelGroups.
			@return False if GroupIndex is out of range */
		bool RemoveGroup(unsigned int GroupIndex);
		/** Same as RemoveGroup, but the ordering of the groups is preserved.
			This function is slower than RemoveGroup.
			@param GroupIndex Index in the array of groups to be removed.
			@return False, if Index is out of range. */
		bool RemoveGroupSlow(unsigned int GroupIndex);
		/** Returns the index in which the given position were found, position index also 
			returned. See also other method where position index is returned.
			@param GroupIndex The first group index in which the position Pos were found.
				Unchanged if position was not found.
			@param Pos Position to test for.
			@param PosIndex If the position "Pos" is found, the PosIndex of the position is returned
				in this parameter. If not found, the parameter is unchanged.
			@return true if the position was found, false if not found. */
		bool GetGroupWithPos(unsigned int& GroupIndex,const CPoint2D<INT16>& Pos,
						unsigned int& PosIndex);
		/** Returns the index in which the given position were found, no position index is
			returned. See also other method where position index is returned.
			@param GroupIndex The first group index in which the position Pos were found.
				Unchanged if position was not found.
			@param Pos Position to test for.
			@return true if the position was found, false if not found. */
		bool GetGroupWithPos(unsigned int& GroupIndex,const CPoint2D<INT16>& Pos);
		/** Prints the information contained in "m_Groups" to stdout. */
		void PrintInfo();
		/** Writes all the pixel groups information to stream.
			Calls StreamIndent() for each streamed line. */
		friend ostream& operator<<(ostream& s,const CPixelGroups& Groups);
	private:
		// Used when inserting or removing a group somewhere in the array
		void UpdateAll();
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

inline CPixelGroup& CPixelGroups::operator[](unsigned int Index)
{
	return *(m_pCGroups[Index]);
}

inline const CPixelGroup& CPixelGroups::operator[](unsigned int Index) const
{
	return *(m_pCGroups[Index]);
}

/*@}*/ /* end group "containerclasses" */

} // end namespace ipl

#endif //_IPL98_PIXELGROUPS_H
