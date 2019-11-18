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

#ifndef _IPL98_POSITIONGROUP2D_H
#define _IPL98_POSITIONGROUP2D_H

#include "../ipl98_setup.h" /* always include the setup file */
#include "positiongroup2d_const_it.h"
#include "positiongroup2d_it.h"
#include "ipl98_general_functions.h"
#include "error.h"
#include "../kernel_c/ipl98_types.h"
#include <ios>
#include <iomanip>
#include <vector>
#include <sstream>
#include <fstream>

namespace ipl{ // use namespace ipl

using std::ifstream;
using std::ofstream;
using std::ostream;
using std::vector;
using std::ostringstream;
// external definitions
class CImage;

/** @ingroup containerclasses
@{ */

/** CPositionGroup2D, contains a group of positions, last updated 12/29/2005.
	The template type must be a CPoint2D type or a CPoint2D derived class type.
	It can be convenient to store more information with each point, for that
	purpose simply derive a class from CPoint2D and add the extra data and
	functionality. Note: If you add extra data (member attributes) which needs
	to be saved and loaded, you must overwrite the two functions:
	\verbatim
	  
		friend ostream& operator<<(ostream& s,const T& P2D);
		inline istream& operator<<(istream& s);
	\endverbatim
	Easiest way to do it, is to copy the code from the base class and add
	some code for the new attributes. Note: A similar class name CPixelGroup
	is available, it is specialized for CPoint2D types and contains additional
	methods for working with images, it is build on top of a C implementation.

	This class has two iterators: ConstIterator and Iterator. They work like the iterators
	in the C++ std-library, so if you are familiar with them, these are very easy to use.
	Here is an example:
	\verbatim
	CPositionGroup2D< CPoint2D<double> > Grp;
	Grp.AddPosition( CPoint2D<double>(1.1,1.1));
	Grp.AddPosition( CPoint2D<double>(2.2,2.2));
	Grp.AddPosition( CPoint2D<double>(3.3,3.3));

	CPoint2D<double> P(3.3,3.3);
	CPositionGroup2D< CPoint2D<double> >::ConstIterator It;
	for(It=Grp.Begin();It<Grp.End();It++)
	{
		if (P==*It)
		{
			cout << *It << endl;
			It--; // using postfix -- operator on iterator
			cout << *It << endl;
			--It; // using prefix -- operator on iterator
			cout << *It << endl;
			It++; // using postfix ++ operator on iterator
			++It; // using prefix ++ operator on iterator
		}
	}

	\endverbatim

	For more details, see CPositionGroup2DIterator and CPositionGroup2DConstIterator
	classes.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@see CPoint2D
	@see CPixelGroup
	@see CPositionGroup2DConstIterator
	@see CPositionGroup2DIterator
	\class CPositionGroup2D ipl98/cpp/positiongroup2d.h
	@version 0.61,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */
template<class T>
class CPositionGroup2D {
	protected: // attributes
		/** The data array of all positions. */
		vector< T > m_Positions;
		/** Position with lowest y-value in group. */
		T m_Top;
		/** Position with highest y-value in group. */
		T m_Bottom;
		/** Position with lowest x-value in group. */
		T m_Left;
		/** Position with highest x-value in group. */
		T m_Right;
	public: // methods
		/** constructor, initializes the boundary values to given value, defaults
			to (0,0). Unlike the CPixelGroup, we cannot set the boundary values to
			maximum opposite values, since we do not know the range of type T. */
		CPositionGroup2D(const T& InitBoundaryVal=T());
		/** Constructor allocating memory for positions to be contained in this class.
			To make insertions fast, call this funcion with a value in InitialSize
			greater than the expected pixels in the group.
			@param InitialSize Number of positions to allocate memory for. */
		CPositionGroup2D(unsigned int InitialSize);
		/** copy constructor 
			@param SourceGroup Group to copy into this group */
		CPositionGroup2D(const CPositionGroup2D& SourceGroup);
		/** overloading of assignment operator, deletes old pixel positions in destination.
			Also deletes the color values if available. Calls the kernel function k_CopyGroup(). */
		CPositionGroup2D& operator=(const CPositionGroup2D& SourceGroup);
		/** overloading of []-operator. Returns a reference to the position at Index, no range
			check, fast access.
			@see GetPosition */
		inline T& operator[](unsigned int Index);
		/** overloading of []-operator. Returns a constant reference to the position at Index,
			no range check, fast access.
			@see GetPosition */
		inline const T& operator[](unsigned int Index) const;
		/// default destructor
		~CPositionGroup2D();
		/** Sets all members to 0 and deallocates memory
			@see EmptyNoDeallocation */
		void Empty();
		/** Same as Empty(), except the allocated memory to store positions
			in, is not deallocated. This can optimize performance if the group
			is to be used again.
			@see Empty */
		void EmptyNoDeallocation();
		/** Checks if any data is present.
			@return True, if group contains no data. */
		inline bool IsEmpty() const;
		/** @name Iterator functionality
			Iterators has been added to standardize the way to iterate through positions
			in the CPositionsgroup2D class. To learn about iterators, look at the
			standard library iterators. Implemented iterators in the pixelgroup class are
			iterators and const iterators. Both iterators have ++ and -- (both postfix and prefix)
			operators and the comparison operators ==, != and <. 
			Typedefs are available in this class for each iterator type, just like 
			the standard library.
			Here is a short example on using an iterator:
			\verbatim

			CPositionGroup2D<CPoint2D<float> > Grp;
			Grp.AddPosition(CPoint2D<float>(1,2));
			Grp.AddPosition(CPoint2D<float>(0,0));
			CPositionGroup2D<CPoint2D<float> >::ConstIterator it3;
			CPositionGroup2D<CPoint2D<float> >::Iterator it=Grp.Begin();
			it3=it; // converting it to a const iterator
			it3++; // increment it3
			if (it<it3) // comparing two iterators
			{
				cout << *it << endl;
			}
			\endverbatim
		*/
		//@{ 
		/** Iterator for the CPixelGroup class. This typedef is included to make
			the class work like the std library. Instantiate an iterator object
			like this:
			\verbatim
			CPositionGroup2D<T>::Iterator it;
			\endverbatim */
		typedef CPositionGroup2DIterator<T> Iterator;
		/** ConstIterator for the CPixelGroup class. This typedef is included to make
			the class work like the std library. Instantiate an iterator object
			like this:
			\verbatim
			CPositionGroup2D<T>::ConstIterator it;
			\endverbatim */
		typedef CPositionGroup2DConstIterator<T> ConstIterator;
		/** Returns an iterator to the last element plus one.
			@return Last element plus one. */
		inline Iterator End(){return Iterator(m_Positions.end());}
		/** Returns a const iterator to the last element plus one.
			@return Last element plus one. */
		inline ConstIterator End() const{return ConstIterator(m_Positions.end());}
		/** Returns an iterator to the first element.
			@return First element. */
		inline Iterator Begin(){return Iterator(m_Positions.begin());}
		/** Returns a const iterator to the first element.
			@return First element. */
		inline ConstIterator Begin() const{return ConstIterator(m_Positions.begin());}
		//@}
		/** Get position with lowest y-value in group.
			@return Top position in group. */
		inline const T& GetTop() const;
		/** Get position with highest y-value in group.
			@return Bottom position in group. */
		inline const T& GetBottom() const;
		/** Get position with lowest x-value in group.
			@return Leftmost position in group. */
		inline const T& GetLeft() const;
		/** Get position with highest x-value in group.
			@return Rightmost position in group. */
		inline const T& GetRight() const;
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
		/** Get number of positions allocated, this number is greater than or equal to 
			result returned from GetTotalPositions(). Call ReAllocPositions() to reallocate
			more positions in memory.
			@return Total number of positions.
			@see ReAllocPositions */
		inline unsigned int GetAllocatedPositions() const;
		/** Returns the position stored at "Index" in the group. Maximum index is
			GetTotalPostitions()-1.
			@return Position at given index, returns (0,0) if Index is out of range. */
		inline T GetPosition(unsigned int Index) const;
		/** Returns the last element in the group, i.e. position with max index
			defined as GetTotalPostitions()-1.
			@return Position at last index. */
		inline T GetBack() const;
		/** Returns the position at the middle index in this group, that is the
			position with index GetTotalPositions()/2. If total positions is an
			even number, the highest index of the two middle indexes are chosen.
			@return Position at middle index, rounded up if total positions is an even number. */
		inline T GetMiddle() const;
		/** Returns a constant reference to the positions data contained in m_Positions,
			which is a standard vector class. Can be used for fast access to data.
			@return Constant reference to data contained in a vector class. */
		const vector<T>& GetDataReference() const{return m_Positions;}
		/** Returns a reference to the positions data contained in m_Positions,
			which is a standard vector class. Can be used for fast access and
			manipulation of data. Note: Changing data this way means that the boundary
			information may need to be updated by a call to UpdateBoundaries().
			@return Reference to data contained in a vector class. */
		vector<T>& GetDataReference() {return m_Positions;}
		/** Reallocates memory for the positions contained in this class. The new allocated
			memory is not initialised.
			@param NewSize The new number of positions to allocate memory for.
				Note: The NewSize may be less than the actual allocated memory! In that case
				the corresponding positions stored in the end of the actual memory are deleted. */
		void ReAllocPositions(unsigned int NewSize);
		/** Removes unused memory allocated for positions in this class.
			Because memory allocation is a time consuming operation it is a good idea by each
			ReAllocPositions (or the CPositionGroup2D(unsigned int InitialSize) constructor) call to
			allocate a big number of pixels and then at the end of the construction of a
			CPositionGroup2D calling this AdjustSizeOfPositions() method to deallocate unused memory. */
		void AdjustSizeOfPositions();
		/** The position Pos is added to the group of pixels. The boundary is updated if
			the new position is outside the old boundary.
			@param Pos Position to add to PixelGroup.
			@see AddPositionFast */
		inline void AddPosition(const T& Pos);
		/** Inserts the position Pos at given Index, which must be less than the
			inserted pixels (returned by GetTotalPositions()). The ordering of
			the positions is preserved. The boundary is updated if the new position 
			is outside the old boundary.
			@param Pos Position to add to the PixelGroup.
			@param Index Index in the array of positions to be removed.
			@return False, if Index is out of range.
			@see InsertPositionFast */
		bool InsertPosition(T Pos, unsigned int Index);

		/** Appends a whole group to this group. The order is preserved and boundaries are
			updated. But there may be duplicate positions afterwards! Call RemoveDuplicates()
			to ensure that no duplicates exists.
			@param Grp Positions group to be added to this group.
			@see RemoveDuplicates
			@see AddGroupFront */
		void AddGroup(const CPositionGroup2D<T>& Grp);
		/** Inserts a whole group in front of the positions in this group. The order is 
			preserved and boundaries are updated. But there may be duplicate positions 
			afterwards! Call RemoveDuplicates() to ensure that no duplicates exists.
			@param Grp Positions group to be inserted in front of positions in this group.
			@see RemoveDuplicates
			@see AddGroup */
		void AddGroupFront(const CPositionGroup2D<T>& Grp);

		/** Removes a position stored in the group at given index. This is the fast method
			for removing positions and it does not preserve the order of positions stored in
			this class. To remove positions and preserve the order, call RemovePositionSlow().
			The boundary is also updated if the removed position causes changes in the boundary.
			This methods works by moving the last position in the internal m_Positions array 
			to the index which is to be removed.
			Note: When running a loop to check for removal of positions, Index must
			be decremented when a position is removed as shown here:
			\verbatim

			CPositionGroup2D<double> Grp;
			for(int Index=0; Index<Grp.GetTotalPositions(); Index++)
			{
				if "test Grp[Index] with something" true
				{
					// do something
					Index--; // decrement Index to avoid not testing all positions
				}
			}
			\endverbatim
			
			@param Index Index in the array of position to be removed.
			@return False, if Index is out of range.
			@see RemovePositionSlow
			@see RemovePositions */
		bool RemovePosition(unsigned int Index);
		/** Same as RemovePosition, but the ordering of the positions is preserved.
			This method is slower than RemovePosition. For iterating in a loop see
			RemovePosition for details.
			@param Index Index in the array of position to be removed.
			@return False, if Index is out of range.
			@see RemovePosition */
		bool RemovePositionSlow(unsigned int Index);
		/** Same as RemovePosition, except a whole range of positions can be removed at once.
			More efficient than callint RemovePosition() on each element. The ordering of the 
			positions is preserved and boundaries are updated.
			@param StartIndex Index in array of first position to be removed.
			@param EndIndex Index in array of last position to be removed.
			@return False, if any of the indexes are out of range.
			@see RemovePosition */
		bool RemovePositions(unsigned int StartIndex, unsigned int EndIndex);
		/** Removes duplicates of same position in this group. The algorithm could
			probably be more efficient but it is still nice to have this functionality.
			To make it a bit efficient though, the order of the positions is not preserved, i.e.
			the same code as RemovePosition is used to remove positions, but no update of
			boundary is neccesary.
			@return Total number of duplicate positions removed. */
		unsigned int RemoveDuplicates();
		/**	Tests if given position is included in the group. This method should be faster
			if the group is sorted in advance, haven't tested it though. Let me know if
			somebody have some results on that.
			@param Pos Position to test for.
			@param Index If the position "Pos" is found, the Index of the position is returned
				in this parameter. If not found, the parameter is unchanged.
			@return True if the position was found, false if not found. */
		bool PosInGroup(const T& Pos,unsigned int& Index);
		/** Load positions contained in file given by pPathAndFileName. Format must
			comply with the format described in the Save() method.
			@param pPathAndFileName Name of file including extension and relative
			or absolute path, f.ex. absolut path "c:/temp/positions.txt",
			relative "positions.txt". Note: In order to use this method when this
			class holds types derived from CPoint2D, you may need to overwrite some
			streaming operators, since this method uses the >> operator for each position
			to be loaded. See general description for this class for more info.
			@return False if load failed
			@see Save */
		bool Load(const char* pPathAndFileName);
		/** Saves positions contained in this group. First a comment line is generated, 
			then a token "CPositionGroup2D", then a token "TotalPositions:" followed by 
			an integer which is the total positions contained in file. Format of positions
			is:
			\verbatim

			# Data from a CPositionGroup2D object in IPL98
			CPositionGroup2D
			TotalPositions Total

			pos=(x0,y0) dir= (i0,j0)
			\endverbatim
			If direction is not available 'dir= (i0,j0)' is replaced with dir=N/A.
			Note: no space between 'dir=' and 'N/A', it is treated as one read token.
			@param pPathAndFileName Name of file including extension and relative
			or absolute path, f.ex. absolut path "c:/temp/positions.txt",
			relative "positions.txt". Note: In order to use this method when this
			class holds types derived from CPoint2D, you may need to overwrite some
			streaming operators, since this method uses the << operator on each point
			contained. See general description for this class for more info.
			@return False if save failed
			@see Load */
		bool Save(const char* pPathAndFileName) const;
		/** Copies a group to a new image. Origo of the new image is set to (-Left.x,-Top.y)
			which is the groups top left boundary. Absolute positions can be plotted to a new
			image by setting the parameter Absolute to true, see Absolute parameter for more info.
			If positions are floating values, they are rounded to nearest integer. A black and 
			white image with 1 b/p is produced, positions are plotted with opposite color as chosen
			background given in parameter BackGround
			@param BackGround The background color of new image, values are HIGHCOLOR (white)
				or LOWCOLOR (black)
			@param Dest Image to copy the group to. All previous information in Dest is
				destroyed.
			@param Absolute If true, group positions are plottet with their absolute values
				in a new image with dimensions (Right.x+1, Bottom.y+1). Origo is set to (0,0).
				The user must be sure that the group does not contain any negative values.
			@return False, if the group is empty, the Dest image is in that case left unchanged. */
		bool CopyToImage(COLORTYPE BackGround, CImage& Dest, bool Absolute=false);
		/** Same as CopyToImage(), with the exception that the supplied image 
			must be pre-allocated and the user can supply a color for the 
			positions to be plotted. 
			
			@param Dest Image to copy the group to. Must be pre-allocated by user. 
				The size of the image should be x-dim: Round(GetRight().GetX()-
				GetLeft().GetX()+1)	and y-dim: Round(GetBottom().GetY()-
				GetTop().GetY()+1). And origo should be set	to 
				Round(-GetLeft().GetX()),Round(-GetTop().GetY()). Here the 
				Get... methods must be the values returned from the groups to be 
				inserted with the greatest/smallest values. If none of the groups 
				to be inserted contains negative values, it is not necessary to set 
				the origo.
			@param Color The group will be plotted with this color. The user
				must ensure that the color value corresponds to the bit depth 
				of the image.
			@return False, if the group or image is empty.
			@see CopyToImage */
		bool CopyToImageColor(CImage& Dest, UINT32 Color);
		/**	Prints the information contained in the group to stdout. Uses the operator
			friend ostream& operator<<(ostream& s,const T& P2D) for the type T of this class.
			That means you have to overload this method if your CPoint2D derived class contains
			extra data.
			@param PrintPositions If true, all pixel positions stored in the group are written
				to stdout.
			@param PosPrLine Number of positions to be written on each line
				(defaults to 5). Only used if PrintPositions is true.
			@param Precision Number of decimals to be shown, defaults to 4. */
		void PrintInfo(bool PrintPositions,unsigned int PosPrLine=5, unsigned int Precision=4);

		/** Runs through all positions in the group and update the left/right/top/bottom
			boundaries, this is usefull if you have used the fast methods which do not
			update boundary values. */
		void UpdateBoundaries();

		/** Same as RemovePosition, except that there is no range check for
			given index and the boundary is not	updated and hence the function is a
			lot faster. For iterating in a loop see RemovePosition for details.
			@return True, always.
			@see RemovePosition */
		inline bool RemovePositionFast(unsigned int Index);
		
		/**	Same as AddPosition, except the boundary is not updated.
			@param Pos Position to add to PixelGroup
			@see AddPosition */
		inline void AddPositionFast(const T& Pos);

		/** Same as InsertPosition, except that there is no range check for
			given index and boundaries are not updated.
			@param Pos Position to add to group.
			@param Index Index in the array of positions to be removed.
			@return True, always.
			@see InsertPosition */
		inline bool InsertPositionFast(T Pos, unsigned int Index);

		/** Adds an offset to all points in group. Boundaries are updated.
			@param Offset The offset value to be added to all positions.
			@return False, if the group is empty. */
		bool AddOffset(const T& Offset);

		/** Sorts all positions contained in this class, sorting on first x and if
			equal on y value, position index 0 contains the "lowest" point. At the
			moment there is no way to ask if the points are sorted, the user must
			keep track of that. The functionality of this method may change a bit
			in future versions, it would be nice to specify if y should be the 
			first sorting criteria.
			@version 0.1
			@author René Dencker Eriksen */
		void Sort();

		/** Reverses the order index order of positions stored in this group. */
		void ReverseOrder();
	private: // methods
		// if used positions is equal to capacity, it reserves double of the
		// current capacity in m_Positions,
		// if capacity is 0, a new capacity of 2 is allocated.
		inline void DoubleCapacity();
		// updates the boundaries according to given Pos, used 
		// by the add and insert methods
		inline void UpdateBoundaries(const T& Pos);
	protected: // methods
		/** used for loading. Skips all whitespace, newline, tab characters and
			comment lines, i.e. rest of line from a '#' character. */
		inline void SkipSpaceAndComments(ifstream& s);
		/** Called by Load(const char* pPathAndFileName). This method is added so that
			derived classes can write their information to a given file and decide where
			the information from this base class should be added to that file.
			Note: The actual Load(pPathAndFileName) called by user must empty this
			class if necessary - it cannot be done by this method, it may delete already loaded
			information in derived class!
			@param s File stream to read from. Must be opened prior to calling this method.
			@return True, if stream fulfills the output described in Save(). */
		bool LoadInternal(ifstream& s);
		/** Called by Save(const char* pPathAndFileName). This method is added so that
			derived classes can write their information to a given file and decide where
			the information from this base class should be added to that file.
			@param s File stream to write to. Must be opened prior to calling this method.
			@param Spaces Number of white spaces to put in front of each line. This makes it
				possible for a derived class to indent the output from this class. Defaults to 0.
			@return True, always. */
		bool SaveInternal(ofstream& s, unsigned int Spaces=0) const;
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

template<class T> 
inline bool CPositionGroup2D<T>::IsEmpty() const
{
	return m_Positions.empty();
}

template<class T> 
inline T& CPositionGroup2D<T>::operator[](unsigned int Index)
{
	return m_Positions[Index];
}

template<class T> 
inline const T& CPositionGroup2D<T>::operator[](unsigned int Index) const
{
	return m_Positions[Index];
}


template<class T> 
inline const T& CPositionGroup2D<T>::GetTop() const
{
	return m_Top;
}

template<class T> 
inline const T& CPositionGroup2D<T>::GetBottom() const
{
	return m_Bottom;
}

template<class T> 
inline const T& CPositionGroup2D<T>::GetLeft() const
{
	return m_Left;
}

template<class T> 
inline const T& CPositionGroup2D<T>::GetRight() const
{
	return m_Right;
}

template<class T> 
inline unsigned int CPositionGroup2D<T>::GetTotalPositions() const
{
	return m_Positions.size();
}

template<class T> 
inline unsigned int CPositionGroup2D<T>::GetWidth() const
{
	return (unsigned int)(m_Right.GetX()-m_Left.GetX()+1);
}

template<class T> 
inline unsigned int CPositionGroup2D<T>::GetHeight() const
{
	return (unsigned int)(m_Bottom.GetY()-m_Top.GetY()+1);
}

template<class T> 
inline unsigned int CPositionGroup2D<T>::GetAllocatedPositions() const
{
	return m_Positions.capacity();
}

template<class T> 
inline T CPositionGroup2D<T>::GetPosition(unsigned int Index) const
{
	if (Index<m_Positions.size())
		return m_Positions[Index];
	else
	{
		ostringstream ost;
		ost << "CPositionGroup2D::GetPosition() Index(" << Index << ") out of range" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return T(0,0);
	}
}

template<class T> 
inline T CPositionGroup2D<T>::GetBack() const
{
	return m_Positions.back();
}

template<class T> 
inline T CPositionGroup2D<T>::GetMiddle() const
{
	return m_Positions[m_Positions.size()/2];
}

template<class T> 
inline void CPositionGroup2D<T>::AddPosition(const T& Pos)
{
	DoubleCapacity(); // doubles the capacity if necessary
	m_Positions.push_back(Pos);
	UpdateBoundaries(Pos);
}

template<class T> 
inline bool CPositionGroup2D<T>::RemovePositionFast(unsigned int Index)
{
	m_Positions[Index]=m_Positions[GetTotalPositions()-1];
	m_Positions.resize(GetTotalPositions()-1);
	return true;
}

template<class T> 
inline void CPositionGroup2D<T>::AddPositionFast(const T& Pos)
{
	DoubleCapacity(); // doubles the capacity if necessary
	m_Positions.push_back(Pos);
}

template<class T> 
inline bool CPositionGroup2D<T>::InsertPositionFast(T Pos, unsigned int Index)
{
	vector< T > v;
	v.assign(m_Positions.begin()+Index,m_Positions.end());
	m_Positions[Index]=Pos;
	m_Positions.resize(Index+1);
	typename std::vector< T >::const_iterator it;
	for(it=v.begin(); it!=v.end(); ++it)
	{
		m_Positions.push_back(*it);
	}
	return true;
}

template<class T> 
inline void CPositionGroup2D<T>::DoubleCapacity()
{
	unsigned int c=GetAllocatedPositions();
	if (c==GetTotalPositions())
		c==0 ? m_Positions.reserve(2) : m_Positions.reserve(2*c);
}

template<class T> 
inline void CPositionGroup2D<T>::UpdateBoundaries(const T& Pos)
{
	if (m_Positions.size()==1)
	{
		// first time, set all boundaries to first inserted position
		m_Right=Pos;
		m_Left=Pos;
		m_Bottom=Pos;
		m_Top=Pos;
	}
	else
	{
		if (Pos.GetX()>m_Right.GetX())
			m_Right=Pos;
		if (Pos.GetX()<m_Left.GetX())
			m_Left=Pos;
		if (Pos.GetY()>m_Bottom.GetY())
			m_Bottom=Pos;
		if (Pos.GetY()<m_Top.GetY())
			m_Top=Pos;
	}
}

template <class T>
inline void CPositionGroup2D<T>::SkipSpaceAndComments(ifstream& s)
{
	while(isspace(s.peek()))
		s.get();
	while (s.peek()=='#')
	{
		s.ignore(std::numeric_limits<int>::max(),'\n');
		while (isspace(s.peek()))
			s.get();
	}
}

/** Writes only part of the group information to stream. Format is:

    \verbatim
    **************** PositionGroup2D info *****************
    Positions: Top=(0,0) Bottom=(0,0) Left=(0,0) Right=(0,0)
    Width=1 Height=1
    Total positions=0 Allocated positions=0
    \endverbatim

    The above is simply an example of a group with only one position. To output
    positions use the PrintInfo() method. In order to save
    all information in this class, use method Save() instead.
	Calls StreamIndent() for each streamed line. */
template<class T> 
ostream& operator<<(ostream& s, const CPositionGroup2D<T>& Group);

#include "positiongroup2d.cpp"

/*@}*/ /* end group "containerclasses" */

} // end namespace ipl

#endif //_IPL98_POSITIONGROUP2D_H
