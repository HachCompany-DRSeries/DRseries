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

#ifndef _IPL98_CORRESPONDING_3D2D_POINTS_H
#define _IPL98_CORRESPONDING_3D2D_POINTS_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "../kernel_c/kernel_corresponding_3d2d_points.h"
#include "../../points/point2d.h"
#include "../../points/point3d.h"
#include <ostream>

namespace ipl{ // use namespace ipl

using std::ostream;

/** @ingroup containerclasses
@{ */

/** Container class for storing corresponding 3D and 2D points and
	optionally and id for each set of points, last updated 5/6/2003.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CCorresponding3D2DPoints ipl98/cpp/corresponding_3d2d_points.h
	@version 0.92,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */

class CCorresponding3D2DPoints{
	public: // attributes
	private: // attributes
		T3D2DPoints m_PointSets;
	public: // methods
		/// default constructor
		CCorresponding3D2DPoints();
		/**	Constructor allocating memory for point sets in the member attribute T3D2DPoints.
			To make insertions fast, use this constructor with a value in InitialSize
			greater than the expected point sets in the group.
			@param InitialSize Number of pooint sets
				to allocate memory for. Must be 1 or greater. */
		CCorresponding3D2DPoints(unsigned int InitialSize);
		/// default destructor
		~CCorresponding3D2DPoints();

		/// Copy constructor
		CCorresponding3D2DPoints(const CCorresponding3D2DPoints& PointSet);
		/** overloading of assignment operator, deletes old point sets in this object.
			Also deletes the color values if available. Calls the kernel function k_Copy3D2DPoints(). */
		CCorresponding3D2DPoints& operator=(const CCorresponding3D2DPoints& SourcePointSet);

		/** removes all entries in this object. */
		void Empty();

		/** Reallocates memory for point sets in the member attribute T3D2DPoints. 
			The new allocated memory is not initialised.
			@param NewSize The new number of position sets to allocate memory for.
				Note: The NewSize may be less than the actual allocated memory! In that case
				the corresponding position sets stored in the end of the actual memory are deleted. */
		void ReAlloc3D2DPoints(unsigned int NewSize);

		/**	Removes unused memory allocated for position sets in the member attribute T3D2DPoints.
			Because memory allocation is a time consuming operation it is a good idea by each
			ReAlloc3D2DPoints() call to allocate a big number of point sets and
			then at the end of the construction of a T3D2DPoints calling this
			AdjustSizeOf3D2DPoints method to deallocate unused memory. */
		void AdjustSizeOf3D2DPoints();

		/** Sets all 3D points to state "not used", i.e. as
			if they have never been set. If one or more points
			have no corresponding 2D value set, the function will 
			return false, but all points will still be iterated.
			@return False, if one or more points have not set a value for the 2D point. */
		bool SetAll3DPointsToNotUsed();

		/** Sets all 2D points to state "not used", i.e. as
			if they have never been set. If one or more points
			have no corresponding 3D value set, the function will 
			return false, but all points will still be iterated.
			@return False, if one or more points have not set a value for the 3D point. */
		bool SetAll2DPointsToNotUsed();

		/** Sets all Id's to state "not used", i.e. as if they have never been set.
			@return True, always. */
		bool SetAllIdsToNotUsed();

		/** Returns the total number of point sets where both the 3D and 2D point has
			been set.
			@return Total number of point sets where both 3D and 2D points are in use. */
		unsigned int GetTotalPointSetsInUse() const;

		/** Returns the total number of point sets where only the 2D point has
			been set.
			@return Total number of point sets where only 2D point is in use. */
		unsigned int GetTotal2DPointsOnlyInUse() const;

		/** Returns the total number of point sets where only the 3D point has
			been set.
			@return Total number of point sets where only 3D point is in use. */
		unsigned int GetTotal3DPointsOnlyInUse() const;

		/** returns the total number of corresponding 3D-2D sets of points inserted in this class.
			Note: the sets may not be complete, ie. one of the 2D or 3D points in a set may be unused.
			To get the total complete sets use the method GetTotalPointSetsInUse().
			@return Total point sets inserted.
			@see GetTotalPointSetsInUse */
		unsigned int GetTotalPointSets() const;

		/** Returns true if the 2D point at given index is in use, i.e. if it has been set and not just
			allocated. 
			@return False, if the 2D point hasn't been set or if Index is out of range. */
		bool Point2DInUse(unsigned int Index) const;

		/** Returns true if the 3D point at given index is in use, i.e. if it has been set and not just
			allocated. 
			@return False, if the 3D point hasn't been set or if Index is out of range. */
		bool Point3DInUse(unsigned int Index) const;

		/** Returns true if the Id at given index is in use, i.e. if it has been set and not just
			allocated. 
			@return False, if the Id hasn't been set or if Index is out of range. */
		bool IdInUse(unsigned int Index) const;

		/** returns the 3D point at Index.
			@return 3D point at Index, if Index is out of range the value (-1,-1,-1) is returned. */
		CPoint3D<float> GetPoint3D(unsigned int Index) const;

		/** returns the 2D point at Index.
			@return 2D point at Index, if Index is out of range the value (-1,-1) is returned. */
		CPoint2D<float> GetPoint2D(unsigned int Index) const;

		/** returns the Id at Index. Note: The user must be aware whether it is in use or not.
			@return Id point at Index, if Index is out of range the value 0 is returned. */
		int GetId(unsigned int Index) const;

		/** To use the kernel C functions it is sometimes necessary
			to pass the T3D2DPoints structure.
			In other cases do not use this function to manipulate the 
			point sets but use the methods in this class!
			@return pointer to point sets data */
		const T3D2DPoints* GetConstT3D2DPointsPtr() const;
		
		/** To use the kernel C functions it is sometimes necessary
			to pass the T3D2DPoints structure which contents will be changed.
			In other cases do not use this function to manipulate the 
			point sets but use the methods in this class!
			@return pointer to point sets data */
		T3D2DPoints* GetT3D2DPointsPtr();

		/**	The pointset Pnt3D and Pnt2D is added to the point set attribute. The class keeps
			track of allocation and updates the AllocatedPointSets (internal use) member in T3D2DPoints
			attribute.
			The Id must be unique, otherwise an error is produced.
			@param Pnt3D 3D position to be added.
			@param Pnt2D 2D position corresponding to the Pnt3D value to be added.
			@param Id The unique identification for this set of corresponding points.
				The user do not have to use this value, the method AddPointSet()
				can be used instead.
			@return False, if Id is not unique. */
		bool AddPointSetAndId(const CPoint3D<float>& Pnt3D, const CPoint2D<float>& Pnt2D, int Id);

		/** Same as AddPointSetAndId() but only a 3D point is inserted, i.e. the corresponding
			2D point is not in use yet.
			@see AddPointSetAndId
			@return False, if Id is not unique. */
		bool AddPoint3DAndId(const CPoint3D<float>& Pnt3D, int Id);

		/** Same as AddPointSetAndId() but only a 2D point is inserted, i.e. the corresponding
			3D point is not in use yet.
			@see AddPointSetAndId
			@return False, if Id is not unique. */
		bool AddPoint2DAndId(const CPoint2D<float>& Pnt2D, int Id);

		/**	The pointset Pnt3D and Pnt2D is added to the point set attribute. The class keeps
			track of allocation and updates the AllocatedPointSets (internal use) member in T3D2DPoints
			attribute.
			The Id field is left unchanged (ie. undefined value).
			@param Pnt3D 3D position to be added.
			@param Pnt2D 2D position corresponding to the Pnt3D value to be added.
			@return True always. */
		bool AddPointSet(const CPoint3D<float>& Pnt3D, const CPoint2D<float>& Pnt2D);

		/** Removes a point set from the point set structure. The order of point sets stored is
			preserved. When a point set is removed, an iteration over all point sets should
			have it's counter decreased by one in order to iterate all values. Here is a short
			example that removes all entries with even Id values:
			\verbatim

  			CCorresponding3D2DPoints PointSets;
			PointSets.AddPointSetAndId(CPoint3D<float>(0,0,0),CPoint2D<float>(0,0),0);
			PointSets.AddPointSetAndId(CPoint3D<float>(1,1,1),CPoint2D<float>(1,1),1);
			PointSets.AddPointSetAndId(CPoint3D<float>(2,2,2),CPoint2D<float>(2,2),2);
			PointSets.AddPointSetAndId(CPoint3D<float>(3,3,3),CPoint2D<float>(3,3),3);
			PointSets.AddPointSetAndId(CPoint3D<float>(4,4,4),CPoint2D<float>(4,4),4);

			cout << PointSets << endl;

			for(int i=0; i<PointSets.GetTotalPointSets(); i++)
			{
				int Id=PointSets.GetId(i);
				if (Id % 2 == 0)
				{
					PointSets.RemovePointSet(Id);
					i--;
				}
			}

			cout << PointSets << endl;
			\endverbatim

			@param Id (unsigned int) Must be present in the point set structure, if not an
				error is produced and the function does nothing.
			@return False, if Id is not present in the point set structure.
			@see RemovePointSetIndex
			@see RemoveAllSets */
		bool RemovePointSet(int Id);

		/** Same as RemovePointSet(), except an index is supllied instead of an Id.
			@param Index Point set at this index will be removed.
			@return False, if Index is out of range.
			@see RemovePointSet
			@see RemoveAllSets */
		bool RemovePointSetIndex(unsigned int Index);

		/** Removes all sets meeting the requirements given in supplied parameters.
			Note: Use Empty() to delete all point sets wihtout any exceptions.
			@param WithUnUsed3D If true, all entries with unused 3D information are removed
			@param WithUnused2D If true, all entries with unused 2D information are removed
			@param WithUnUsedId If true, all entries with unused Id information are removed
			@return Returns total sets removed
			@see RemovePointSet
			@see RemovePointSetIndex */
		unsigned int RemoveAllSets(bool WithUnUsed3D, bool WithUnused2D, bool WithUnUsedId);

		/** Same as AddPointSet() but only a 3D point is inserted, i.e. the corresponding
			2D point is not in use yet.
			@see AddPointSet
			@return False, if Id is not unique. */
		bool AddPoint3D(const CPoint3D<float>& Pnt3D);

		/** Same as AddPointSet() but only a 2D point is inserted, i.e. the corresponding
			3D point is not in use yet.
			@see AddPointSet
			@return False, if Id is not unique. */
		bool AddPoint2D(const CPoint2D<float>& Pnt2D);

		/** Sets the 2D point value at given Index, if the 2D value already is in use it
			will be overwritten.
			@return False, if Index is out of range. */
		bool SetPoint2D(const CPoint2D<float>& Pnt2D, unsigned int Index);

		/** Sets the 3D point value at given Index, if the 3D value already is in use it
			will be overwritten.
			@return False, if Index is out of range. */
		bool SetPoint3D(const CPoint3D<float>& Pnt3D, unsigned int Index);

		/** Returns the index with a matching id.
			@param Id (int) Id to search for.
			@param Index (unsigned int) The index containing given "Id" is returned in this parameter.
			@return False, if Id was not found. */
		bool GetIndex(unsigned int& Index, int Id) const;
		
		/** Returns true if given Id exists.
			@param Id The Id to search for.
			@return True, if given Id exists. */
		bool IdExists(int Id) const;

		/** Loads given file, all previous information in this class is destroyed. 
			The file name must include a relative
			or absolut path and the name of the file with extension, both characters
			'/' and '\' can be used to seperate folder names (remember to "escape" the
			character '\', ex. "c:\\test.txt").
			@param PathAndFileName Filename (including the path) to be loaded.
			@return False if the syntacs of PathAndFileName is wrong, the file is not found
				or the format of the file is wrong. */
		bool Load(const char* PathAndFileName);

		/** Saves given file to disc, the file name must comply with the same rules as
			in Load. Format of written file is:
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
		bool Save(const char* PathAndFileName) const;

		/** Writes all the pixel group information to stream.
			Calls StreamIndent() for each streamed line. */
		friend ostream& operator<<(ostream& s,const CCorresponding3D2DPoints& PointSets);
};

/*@}*/ /* end group "containerclasses" */

} // end namespace ipl

#endif //_IPL98_CORRESPONDING_3D2D_POINTS_H
