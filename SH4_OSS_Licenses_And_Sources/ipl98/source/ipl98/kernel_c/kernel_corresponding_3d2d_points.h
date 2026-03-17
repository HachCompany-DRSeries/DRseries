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

#ifndef _IPL98_KERNEL_CORRESPONDING_3D2D_POINTS_H
#define _IPL98_KERNEL_CORRESPONDING_3D2D_POINTS_H

#include <stdio.h>
#include "../ipl98_setup.h" /* always include the setup file */
#include "ipl98_types_c_only.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup corresponding3d2dpoints ANSI-C Corresponding 3D-2D points functions
	Kernel ANSI C functions for working with a list of corresponding
	3D and 2D points, last updated 12/5/2003. A structure named
	T3D2DPoints is used for holding data, that is a list ofTSinglePointSet.
	Optionally an ID can be stored for each set of corresponding 3D-2D
	points. It is possible only to insert a part of
	a set and later on let another algorithm find and insert the remaining
	part of a set. For this reason the struct TSinglePointSet includes
	a boolean for each value to check if the corresponding positions/id is
	in use.
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 0.92
	@author René Dencker Eriksen edr@mip.sdu.dk
    @{ */

/** Holds a single set of corresponding 3D-2D points and optionally an Id. */
typedef struct {
	/** @param Pnt3D (T3DFloat) 3D position. */
	T3DFloat Pnt3D;
	/** @param Pnt3DInUse (bool) If a Pnt3D has been inserted this is set to true, otherwise false. */
	bool Pnt3DInUse;
	/** @param Pnt2D (T2DFloat) 2D position corresponding to Pnt3D. */
	T2DFloat Pnt2D;
	/** @param Pnt2DInUse (bool) If a Pnt2D has been inserted this is set to true, otherwise false. */
	bool Pnt2DInUse;
	/** @param Id (int) A unique id attached to this point set, may not be in use. */
	int Id;
	/** @param IdInUse (bool) If an id has been inserted this is set to true, otherwise false. */
	bool IdInUse;
} TSinglePointSet;


/** Holds a list of corresponding 3D-2D points. */
typedef struct {
	/** @param pSet (TSinglePointSet*) Array containing sets of 2D-3D points. */
	TSinglePointSet* pSet;
	/** @param NumberOfSets (unsigned int) Total number of corresponding 3D-2D sets. */
	unsigned int NumberOfSets;
	/** @param AllocatedSets (unsigned int) Total number of allocated point sets
		for the pPointSet array, internal use. */
	unsigned int AllocatedSets; /* internal use */
} T3D2DPoints;

/**	Sets all the pointers in pInfo to NULL, and NumberOfSets to 0,
	allways call this function when creating a new T3D2DPoints
	variable. */
void k_Init3D2DPoints(T3D2DPoints* pInfo);

/** Deletes all allocated memory for T3D2DPoints structure
	and initializes structure by calling k_Init3D2DPoints(). */
void k_Empty3D2DPoints(T3D2DPoints* pInfo);

/**	Allocates memory for point sets in the T3D2DPoints structure.
	To make insertions fast, call this funcion with a value in InitialSize
	greater than the expected point sets in the group.
	@param InitialSize (unsigned int) Number of pooint sets
		to allocate memory for. Must be 1 or greater. */
void k_Alloc3D2DPoints(T3D2DPoints* pInfo,unsigned int InitialSize);

/** Reallocates memory for the T3D2DPoints structure. The new allocated memory
	is not initialised.
	@param NewSize (unsigned int) The new number of position sets to allocate memory for.
		Note: The NewSize may be less than the actual allocated memory! In that case
		the corresponding position sets stored in the end of the actual memory are deleted. */
void k_ReAlloc3D2DPoints(T3D2DPoints* pInfo,unsigned int NewSize);

/**	Removes unused memory allocated for position sets in the T3D2DPoints structure.
	Because memory allocation is a time consuming operation it is a good idea by each
	k_ReAlloc3D2DPoints() (or k_Alloc3D2DPoints()) call to allocate a big number of pixels and
	then at the end of the construction of a T3D2DPoints calling this
	k_AdjustSizeOf3D2DPoints function to deallocate unused memory. */
void k_AdjustSizeOf3D2DPoints(T3D2DPoints* pInfo);

/** Sets all 3D points to state "not used", i.e. as
	if they have never been set. If one or more points
	have no corresponding 2D value set, the function will 
	return false, but all points will still be iterated.
	@return False, if one or more points have not set a value for the 2D point
		or if pInfo is NULL. */
bool k_SetAll3DPointsToNotUsed(T3D2DPoints* pInfo);

/** Sets all 2D points to state "not used", i.e. as
	if they have never been set. If one or more points
	have no corresponding 3D value set, the function will 
	return false, but all points will still be iterated.
	@return False, if one or more points have not set a value for the 3D point
		or if pInfo is NULL. */
bool k_SetAll2DPointsToNotUsed(T3D2DPoints* pInfo);

/** Sets all Id's to state "not used", i.e. as if they have never been set.
	@return False, if pInfo is NULL.*/
bool k_SetAllIdsToNotUsed(T3D2DPoints* pInfo);

/** Previous data in "pDest" are destroyed unless the function returns
	false, in that case pDest is unchanged.
	@return False, if source and destination structure is the same. */
bool k_Copy3D2DPoints(T3D2DPoints* pDest,const T3D2DPoints* pSource);

/** Returns the total number of point sets where both the 3D and 2D point has
	been set.
	@return Total number of point sets where both 3D and 2D points are in use. */
unsigned int k_GetTotal3D2DPointSetsInUse(const T3D2DPoints* pInfo);

/** Returns the total number of point sets where only the 2D point has
	been set.
	@return Total number of point sets where only the 2D point is in use. */
unsigned int k_GetTotal2DPointsOnlyInUse(const T3D2DPoints* pInfo);

/** Returns the total number of point sets where only the 3D point has
	been set.
	@return Total number of point sets where only 3D point is in use. */
unsigned int k_GetTotal3DPointsOnlyInUse(const T3D2DPoints* pInfo);

/**	The pointset pPnt3D and pPnt2D is added to the point set structure. The function keeps
	track of allocation and updates the AllocatedSets (internal use) member in T3D2DPoints.
	The Id must be unique in the structure, otherwise an error is produced.
	@param pPnt3D (T3DFloat) 3D position to be added.
	@param pPnt2D (T2DFloat) 2D position corresponding to the pPnt3D value to be added.
	@param Id (unsigned int) The unique identification for this set of corresponding points.
		The user do not have to use this value, the function k_AddPointSetTo3D2DPoints()
		can be used instead.
	@return False, if Id is not unique or if pInfo is a NULL pointer. */
bool k_AddPointSetAndIdTo3D2DPoints(const T3DFloat* pPnt3D, const T2DFloat* pPnt2D, int Id, T3D2DPoints* pInfo);

/** Same as k_AddPointSetAndIdTo3D2DPoints() but only a 3D point is inserted, i.e. the corresponding
	2D point is not in use yet.
	@see k_AddPointSetAndIdTo3D2DPoints
	@return False, if Id is not unique or if pInfo is a NULL pointer. */
bool k_AddPoint3DAndIdTo3D2DPoints(const T3DFloat* pPnt3D, int Id, T3D2DPoints* pInfo);

/** Same as k_AddPointSetAndIdTo3D2DPoints() but only a 2D point is inserted, i.e. the corresponding
	3D point is not in use yet.
	@see k_AddPointSetAndIdTo3D2DPoints
	@return False, if Id is not unique or if pInfo is a NULL pointer. */
bool k_AddPoint2DAndIdTo3D2DPoints(const T2DFloat* pPnt2D, int Id, T3D2DPoints* pInfo);

/**	The pointset pPnt3D and pPnt2D is added to the point set structure. The function keeps
	track of allocation and updates the AllocatedSets (internal use) member in T3D2DPoints.
	The Id field is left unchanged (ie. undefined value).
	@param pPnt3D 3D position to be added.
	@param pPnt2D 2D position corresponding to the pPnt3D value to be added.
	@return False, if pInfo is a NULL pointer. */
bool k_AddPointSetTo3D2DPoints(const T3DFloat* pPnt3D, const T2DFloat* pPnt2D, T3D2DPoints* pInfo);

/** Removes a point set from the point set structure. The order of point sets stored is
	preserved.
	@param Id (unsigned int) Must be present in the point set structure, if not an
		error is produced and the function does nothing.
	@return False, if Id is not present in the point set structure. */
bool k_RemovePointSetFrom3D2DPoints(int Id, T3D2DPoints* pInfo);

/** Same as k_RemovePointSetFrom3D2DPoints(), except an index is supplied instead of an Id.
	@param Index Point set at this index will be removed.
	@return False, if Index is out of range.
	@see k_RemovePointSetFrom3D2DPoints
	@see k_RemoveAllSetsFrom3D2DPoints */
bool k_RemovePointSetIndexFrom3D2DPoints(unsigned int Index, T3D2DPoints* pInfo);


/** Removes all sets meeting the requirements given in supplied parameters.
	Note: Use Empty() to delete all point sets wihtout any exceptions.
	@param WithUnUsed3D If true, all entries with unused 3D information are removed
	@param WithUnused2D If true, all entries with unused 2D information are removed
	@param WithUnUsedId If true, all entries with unused Id information are removed
	@return Returns total sets removed
	@see k_RemovePointSetFrom3D2DPoints
	@see k_RemovePointSetIndexFrom3D2DPoints */
unsigned int k_RemoveAllSetsFrom3D2DPoints(bool WithUnUsed3D, bool WithUnused2D, 
										   bool WithUnUsedId, T3D2DPoints* pInfo);

/** Same as k_AddPointSetTo3D2DPoints() but only a 3D point is inserted, i.e. the corresponding
	2D point is not in use yet.
	@see k_AddPointSetAndIdTo3D2DPoints
	@return False, if Id is not unique or if pInfo is a NULL pointer. */
bool k_AddPoint3DTo3D2DPoints(const T3DFloat* pPnt3D, T3D2DPoints* pInfo);

/** Same as k_AddPointSetTo3D2DPoints() but only a 2D point is inserted, i.e. the corresponding
	3D point is not in use yet.
	@see k_AddPointSetAndIdTo3D2DPoints
	@return False, if Id is not unique or if pInfo is a NULL pointer. */
bool k_AddPoint2DTo3D2DPoints(const T2DFloat* pPnt2D, T3D2DPoints* pInfo);

/** Sets the 2D point value at given Index, if the 2D value already is in use it
	will be overwritten.
	@return False, if Index is out of range or if pInfo is a NULL pointer. */
bool k_SetPoint2DIn3D2DPoints(const T2DFloat* pPnt2D, unsigned int Index, T3D2DPoints* pInfo);

/** Sets the 3D point value at given Index, if the 3D value already is in use it
	will be overwritten.
	@return False, if Index is out of range or if pInfo is a NULL pointer. */
bool k_SetPoint3DIn3D2DPoints(const T3DFloat* pPnt3D, unsigned int Index, T3D2DPoints* pInfo);

/** Loads given file into "pInfo", all previous information in pInfo is destroyed. 
	The file name must include a relative
	or absolut path and the name of the file with extension, both characters
	'/' and '\' can be used to seperate folder names (remember to "escape" the
	character '\', ex. "c:\\test.txt").
	@param PathAndFileName Filename (including the path) to be loaded.
	@return False if the syntacs of PathAndFileName is wrong, the file is not found
		or the format of the file is wrong. */
bool k_Load3D2DPoints(const char* PathAndFileName, T3D2DPoints* pInfo);

/** Saves given file to disc, the file name must comply with the same rules as
	in k_Load3D2DPoints. Format of written file is:
	\verbatim
	
	NumberOfSets 2

	Id= 6 3D-pnt= (31.1,0,72.8) 2D-pnt= (569.373,450.096)  IdInUse=YES Pnt3DInUse=YES Pnt2DInUse=YES
	Id= 8 3D-pnt= (26.3,0,13.3) 2D-pnt= (475.865,343.986)  IdInUse=YES Pnt3DInUse=YES Pnt2DInUse=YES
	\endverbatim
	The tokens IdInUse, Pnt3DInUse, Pnt2DInUse tells if the corresponding values are available, if
	not, the corresponding Id or point will have undefined values.
	@param PathAndFileName Filename (including the path) to be loaded.
	@return False if the syntacs of PathAndFileName is wrong or the path is not found.
	@see k_Load3D2DPoints */
bool k_Save3D2DPoints(const char* PathAndFileName, const T3D2DPoints* pInfo);

/** Returns the index in T3D2DPoints structere with a matching id.
	@param Id Id to search for.
	@param pIndex The index containing given "Id" is returned in this parameter.
	@return False, if Id was not found. */
bool k_GetIndexIn3D2DPoints(unsigned int* pIndex, int Id, const T3D2DPoints* pInfo);

/** Returns true if given Id exists.
	@param Id The Id to search for.
	@return True, if given Id exists. */
bool k_IdExistsIn3D2DPoints(int Id,const T3D2DPoints* pInfo);

/** Prints Corresponding3D2DPoints structure information to stdout */
void k_Print3D2DPoints(const T3D2DPoints* pInfo);

/*@}*/ /* end group "corresponding3d2dpoints" */

/*@}*/ /* end group "ansic" */

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif

#endif /* _IPL98_KERNEL_CORRESPONDING_3D2D_POINTS_H */
