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

#ifndef _IPL98_KERNEL_PIXELGROUPS_H
#define _IPL98_KERNEL_PIXELGROUPS_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "ipl98_types.h"
#include "kernel_pixelgroup.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup pixelgroups ANSI-C Pixel groups container functions
	Kernel ANSI C TPixelGroups functions, last updated 11/15/2004.
	Works on TPixelGroups structure and keeps track of an array of
	TPixelGroup structures.
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 1.02
	@author René Dencker Eriksen (edr@mip.sdu.dk)
    @{ */

/** A container for TPixelGroup structures.
	Keeps track of more than one pixel group. Most images consists of more
	than one PixelGroup so this structure is indeed useful. */	
typedef struct {
	/** @param NumberOfGroups (unsigned int) Total number of TPixelGroup structures. */
	unsigned int NumberOfGroups;
	/** @param AllocatedGroups (unsigned int) Internal use. Remembers the maximum allocated
		TPixelGroup structures available in the pGroups array. */
	unsigned int AllocatedGroups; /* internal use */
	/** @param pGroups (TPixelGroup*) Array of TPixelGroup structures. */
	TPixelGroup* pGroups;
} TPixelGroups;

/** Sets all the pointers in pPixelGroups to NULL. Always call
	this function when creating a variable of the TPixelGroups type */
void k_InitGroups(TPixelGroups* pPixelGroups);

/**	Deletes all allocated memory in TPixelGroups including all allocated memory
	for each TPixelGroup structure in the pGroups array. Sets the other variables
	to 0. Always call this function when leaving a scope where a TPixelGroups
	variable were created and used. */
void k_EmptyGroups(TPixelGroups* pPixelGroups);

/**	Allocates memory for TPixelGroup structures. All new TPixelGroup structures
	are initialised by calling k_InitPixelGroup(),
	@param InitialSize (unsigned int) Number of TPixelGroup structures
		to allocate memory for. Must be 1 or greater. */
void k_AllocGroups(TPixelGroups* pPixelGroups,unsigned int InitialSize);

/**	The pPixelGroup structure is added to pPixelGroups. The function keeps track of
	allocation and updates the AllocatedPixelGroups member in TPixelGroups. Note: All
	memory allocated in the TPixelGroup structure is copied to a new structure, you
	must remember to free the memory in pPixelGroup.
	@param pPixelGroup PixelGroup to be added. */
void k_AddGroupToGroups(const TPixelGroup* pPixelGroup, TPixelGroups* pPixelGroups);

/** Insert "pPixelGroup" at given Index. The ordering of the groups
	is preserved.
	@param pPixelGroup PixelGroup to be inserted.
	@param Index Index in the array of groups to insert at.
	@return False, if Index is out of range. */
bool k_InsertGroupInGroups(const TPixelGroup* pPixelGroup, unsigned int Index,
						   TPixelGroups* pPixelGroups);

/**	After the creation of a TPixelGroups structure this function will get the original
	pixel colors from given source image and insert them into all the groups contained
	in TPixelGroups.
	@param pSource Image to get the original pixel values from. The PixelGroups positions
		must be derived from this image.
	@return False if the TPixelGroups structure contains no groups or one of the
		parameters is a NULL pointer. */
bool k_AddColorsToGroups(const TImage* pSource,TPixelGroups* pPixelGroups);

/** Remove a TPixelGroup from the TPixelGroups structure. Done by moving the last group
	in the pGroups array contained in TPixelGroups to the index which is to
	be removed. All memory allocated for the group to be removed are freed. Note: When
	removing an element at index GroupIndex in a loop which runs through all indexes,
	remember to iterate at GroupIndex again. Otherwise you will miss a group!
	@param GroupIndex Index of group in TPixelGroups.
	@return False if GroupIndex is out of range or pPixelGroups is a null pointer */
bool k_RemoveGroup(TPixelGroups *pPixelGroups,unsigned int GroupIndex);

/** Same as k_RemoveGroup, but the ordering of the groups is preserved.
	This function is slower than k_RemoveGroup.
	@param Index Index in the array of groups to be removed.
	@return False, if Index is out of range. */
bool k_RemoveGroupSlow(TPixelGroups *pPixelGroups,unsigned int GroupIndex);

/** Returns the index in which the given position were found.
	@param pGroupIndex The first group index in which the position Pos were found.
		Unchanged if position was not found.
	@param Pos Position to test for.
	@param pPosIndex If the position "pos" is found, the pPosIndex of the position is returned
		in this parameter, if NULL this parameter is not used. If not found, the parameter is
		unchanged.
	@return true if the position was found, false if not found or pPixelGroup is NULL. */
bool k_GetGroupWithPos(const TPixelGroups* pPixelGroups, unsigned int* pGroupIndex,
					   T2DInt Pos,unsigned int* pPosIndex);

/** Prints the information contained in "pPixelGroups" to stdout. */
void k_PrintGroupsInfo(const TPixelGroups* pPixelGroups);

/*@}*/ /* end group "pixelgroups" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_PIXELGROUPS_H */
