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

#ifndef _IPL98_ARRAYOS2D_H
#define _IPL98_ARRAYOS2D_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "array2d.h"
#include "arrayos2d_it.h"
#include "arrayos2d_const_it.h"
#include "../ipl98_general_functions.h"

namespace ipl{ // use namespace if C++

/** @ingroup containerclasses
@{ */

/** Derived from CArray2D this class offers subindexing and origo, last updated 12/29/2005.
	Read more about the functionality in the base class CArray2D. The feature subindexing
	means, you specify how many subindexes you want between two subsequent integer
	values when you construct the object or call Alloc(). If subindexing is set to 1, indexes
	with stepsize 0.5 are valid, see example below. Origo works like the IPL98 image
	classes, simply offsets the (0,0) index.
	An iterator class is available, read more in the base class and the iterator classes
	CArray2DConstIterator and CArray2DIterator.

	\verbatim
	// initializes an array and prints a row and colomn from it
	int x1=5,y1=3;
	CArrayOS2D<double> Arr1(x1,y1,1); // subindexing is 1, hence step size is 0.5

	for(double C1=0; C1<x1; C1+=Arr1.GetSubStep())
		for(double C2=0; C2<y1; C2+=Arr1.GetSubStep())
			Arr1[C1][C2]=x1*C1+C2;

	cout << Arr1 << endl;
	cout << "Index [0.5][1.5]=" << Arr1[0.5][1.5] << endl;
	// print a row
	cout << "Row 2: ";
	CArrayOS2D<double>::ConstIterator it1=Arr1.ConstRow(2);
	for(; it1!=it1.End(); it1++)
		cout << *it1 << " ";
	// print a column
	it1=Arr1.ConstColumn(2);
	cout  << endl << "Column 2: ";
	for(; it1!=it1.End(); it1++)
		cout << *it1 << " ";
	\endverbatim

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CArrayOS2D ipl98/cpp/arrays/arrayos2d.h
	@version 0.37,  by edr@mip.sdu.dk
	@author René Dencker Eriksen
	@see CArray2D
	@see CArrayOS2DIterator
	@see CArrayOS2DConstIterator */
template<class T> 
class CArrayOS2D : public CArray2D<T>
{
protected: // attributes
	/** Contains the offset counted in cells in array. Note, it is the cell
		offset, not the unit length offset, this is to avoid a multiplication
		when used internally. */
	CPoint2D<int> m_OrigoCells;
	/** Contains the offset counted in units. */
	CPoint2D<double> m_Origo;
	/** The granularity of the array. Holds the number of sub indices minus 1 present
		between	two neighbouring integer values. If value is 4, it is possible to get 
		a value at each position (0.25*i, 0.25*j), where i and j are integers.
		Note: This value has been added one from the parameter value given when creating
		this object or calling Alloc! This has been done in order to optimize speed
		internally. */
	unsigned int m_SubIndexing;
	/** Sub step size for this array. It is always set to (1/m_SubIndexing), but
		is is convenient to have the value precalculated. */
	double m_SubStep;

public:
		/** Default constructor. */
        CArrayOS2D();
		/** Constructor defining the array.
			@param x Width of array.
			@param y Height of array.
			@param SubIndexing the number of sub indices present
				between	two neighbouring integer values */
        CArrayOS2D(UINT32 x, UINT32 y, unsigned int SubIndexing=0);
		/// copy constructor
		CArrayOS2D(const CArrayOS2D<T>& M);
		/// overloading = operator
		inline CArrayOS2D& operator=(const CArrayOS2D& M);
		/** overloading = operator for setting each element to specified value.
			Works as the standard library = operator for a valarray. */
		inline CArrayOS2D& operator=(const T& M);
		/** Deletes all contents of this object, sets size to 0,
			origo to (0,0) and subsampling to 1 (none). */
		void Empty();
		/** Sets the size of array, origo is not changed. It is not efficient
			to reallocate the matrix, if size is known at creation time, use
			the constructor once for all.
			@param x Integer width of array, note actual array will contain
				(SubIndexing+1)*x elements.
			@param y Integer height of array, note actual array will contain
				(SubIndexing+1)*y elements.
			@param SubIndexing the number of sub indices present
				between	two neighbouring integer values
			@return false, if allocation failed or if SubIndexing is 0. */
		virtual bool Alloc(UINT32 x, UINT32 y, unsigned int SubIndexing=0);
		/** Returns the integer width of the array, if you want
			the number of cells use GetWidthCells().
			@return Integer width of array.
			@see GetWidthCells */
        inline UINT32 GetWidth() const;
		/** Returns the integer height of the array, if you want
			the number of cells use GetHeightCells().
			@return Integer width of array.
			@see GetHeightCells */
        inline UINT32 GetHeight() const;
		/** Returns the total cells in horizontal direction, including
			subindexed cells. If you want
			the width of the array call GetWidth().
			@return Total cells in horizontal direction.
			@see GetWidth */
        inline UINT32 GetWidthCells() const;
		/** Returns the total cells in vertical direction, including
			subindexed cells. If you want
			the height of the array call GetHeight().
			@return Total cells in vertical direction.
			@see GetHeight */
        inline UINT32 GetHeightCells() const;
		/** Returns origo for this array.
			@return Origo.
			@see SetOrigo */
		inline const CPoint2D<double> GetOrigo() const;
		/** Sets origo for this array, will be rounded to
			nearest sub index. Can be outside array dimensions. */
		inline void SetOrigo(const CPoint2D<double>& Origo);
		/** Sets origo for this array, will be rounded to
			nearest sub index. Can be outside array dimensions. */
		inline void SetOrigo(double x, double y);
		/** Returns true if given position is within the range of the array, 
			the range is calculated according to origo. If
			result is true, it is safe to call GetFast().
			@return True, if given position is within the range of array.
			@see GetFast */
		inline bool InRange(double x, double y) const;
		/** Same as InRange(x,y), except the parameters are given as a CPoint2D. */
		inline bool InRange(const CPoint2D<double>& P) const;
		/** Iterate from this value.
			@return min. X relative to origo
			@see GetMaxX() */
		inline double GetMinX() const;
		/** Iterate from this value.
			@return min. Y relative to origo
			@see GetMaxY() */
		inline double GetMinY() const;
		/** Iterate to this value minus one (the array is zero indexed).
			@return max. X relative to origo
			@see GetMinX() */
		inline double GetMaxX() const;
		/** Iterate to this value minus one (the array is zero indexed).
			@return max. Y relative to origo
			@see GetMinY() */
		inline double GetMaxY() const;
		/** Iterator for the CArrayOS2D class. This typedef is included to make
			the class work like the std library. Instantiate an iterator object
			like this:
			\verbatim
			CArrayOS2D<T>::Iterator it;
			\endverbatim */
		typedef CArrayOS2DIterator<T> Iterator;
		/** Constant iterator for the CArrayOS2D class. This typedef is included to make
			the class work like the std library. Instantiate a const iterator object
			like this:
			\verbatim
			CArrayOS2D<T>::ConstIterator it;
			\endverbatim */
		typedef CArrayOS2DConstIterator<T> ConstIterator;
		/** Returns an iterator to row number i.
			@param i Row number (zero indexed).
			@return iterator.
			@see CArrayOS2DIt */
        inline Iterator Row(double i);
		/** Returns a const iterator to row number i.
			@param i Row number (zero indexed).
			@return const iterator.
			@see CArrayOS2DConstIt */
        inline ConstIterator ConstRow(double i) const;
		/** Returns an iterator to column number i.
			@param i Column number (zero indexed).
			@return iterator.
			@see CArrayOS2DIt */
        inline Iterator Column(double i);
		/** Returns a const iterator to column number i.
			@param i Column number (zero indexed).
			@return const iterator.
			@see CArrayOS2DConstIt */
        inline ConstIterator ConstColumn(double i) const;
		/** Read element at position P.
			@return Returns a const referense to value at position P. */
		inline const T& operator()(const CPoint2D<double>& P) const;
		/** Read or write element at position P.
			@return Returns a referense to value at position P. */
		inline T& operator()(const CPoint2D<double>& P);
		/** Returns an iterator for column i. Use another [] to index
			the row number. To make the indexing as fast as possible,
			the value of i is asumed to be a valid subindex, if	an
			indexing at nearest valid subindex value is wanted use the
			Get, Set, GetFast or SetFast medthods instead.
			@see Get
			@see Set */
        inline Iterator operator[](double i) { return Iterator(this->m_pData,slice((size_t)((i+m_Origo.GetX())*m_SubIndexing*this->m_Height),this->m_Height,1),m_SubIndexing,&m_Origo); }   // C-style subscript
		/** Returns a const iterator for column i. Use another [] to index
			the row number. To make the indexing as fast as possible,
			the value of i is asumed to be a valid subindex, if	an
			indexing at nearest valid subindex value is wanted use the
			Get or GetFast medthods instead.
			@see Get
			@see GetFast */
        inline ConstIterator operator[](double i) const { return ConstIterator(this->m_pData,slice((size_t)((i+m_Origo.GetX())*m_SubIndexing*this->m_Height),this->m_Height,1),m_SubIndexing,&m_Origo); }
		/** Returns the value at (x,y) rounded to nearest sub index in array.
			Includes range check, if out of range, the value at
			origo is returned.
			@param x Column relative to origo.
			@param y Row relative to origo.
			@return Value at (x,y) relative to origo.
			@see GetFast */
		inline const T& Get(double x, double y) const;
		/** Same as Get(x,y), except the parameters are given as a CPoint2D. */
		inline const T& Get(const CPoint2D<double>& P) const;
		/** Fast version of Get(x,y), no range check. The user
			must ensure that the given position is valid.
			@param x Column relative to origo.
			@param y Row relative to origo.
			@return Value at (x,y) relative to origo.
			@see Get */
		inline const T& GetFast(double x, double y) const;
		/** Same as GetFast(x,y), except the parameters are given as a CPoint2D. */
		inline const T& GetFast(const CPoint2D<double>& P) const;
		/** Sets the given value at (x,y).
			Includes range check, if out of range nothing is done.
			@param x Column relative to origo.
			@param y Row relative to origo.
			@param Val Value to be set at (x,y) relative to origo.
			@return False, if (x,y) is out of range.
			@see SetFast */
		inline bool Set(double x, double y, const T& Val);
		/** Same as Set(x,y), except the parameters are given as a CPoint2D. */
		inline bool Set(const CPoint2D<double>& P, const T& Val);
		/** Fast version of Set(x,y,Val), no range check. The user
			must ensure that the given position is valid (relative to origo).
			@param x Column relative to origo.
			@param y Row relative to origo.
			@param Val Value to be set at (x,y) relative to origo.
			@see Set */
		inline void SetFast(double x, double y, const T& Val);
		/** Same as Set(x,y), except the parameters are given as a CPoint2D. */
		inline void SetFast(const CPoint2D<double>& P, const T& Val);
		/** Returns the sub indexing, that is the number of sub indices present
			between	two neighbouring integer values. See attribute m_SubIndexing
			for more info.
			@return Sub indexing value.
			@see m_SubIndexing. */
		inline unsigned int GetSubIndexing() const;
		/** Returns the step size valid for sub indexing, use this value when in-
			or decrementing	a counter in a loop.
			See attribute m_SubIndexing for more info.
			@return Sub indexing value.
			@see m_SubIndexing. */
		double GetSubStep() const;
		/** Rounds the given value to nearest valid sub indexing value,
			that is the nearest value that will be a legal indexing value.
			@param Val Value to be rounded.
			@return Val rounded to nearest valid sub indexing value.
			@see m_SubIndexing
			@see GetNearestSubIndex */
		inline double RoundToSubValue(const double& Val) const;
		/** Returns the index to be used for given value rounded to nearest
			index. Same information can be extracted from RoundToSubValue but a 
			multiplication is saved here.
			@see RoundToSubValue */
		inline int GetNearestSubIndex(const double& Val) const;
		/** Load file from disk. File must be in correct format according
			to how the Save() method formats output. Note: There is no check at the
			moment whether the type T of this class is the same as the array being read,
			reading from a file with another template type leads to undefined results.
			Note: Comment lines (starting with a #) and whitespace can be put anywhere 
			within the stream.
			@param pPathAndFileName Name of file including extension and relative
			or absolute path, f.ex. absolut path "c:/temp/myarrayos2d.bmp",
			relative "myarrayos2d.txt". Both slash ("/") and back slash ("\") can be
			used in the path, but remember to escape a "\" character (i.e. write "\\"),
			otherwise it will be interpreted as a control character.
			@return false if load failed. */
		bool Load(const char* pPathAndFileName);
		/** Save file to disk. First a comment line is generated, then a token
			"CArrayOS2D", then a token "Dimensions:" followed by two integers (width and height),
			then a token "Origo:" followed by (x,y), then a 
			token "SubIndexing:" followed by the sub indexing number (see m_SubIndexing).
			After that the 2D array is written. Each element is written using the streaming
			operator on that element.
			@param pPathAndFileName Name of file including extension and relative
			or absolute path, f.ex. absolut path "c:/temp/myarray2d.txt",
			relative "myarray2d.txt". Both slash ("/") and back slash ("\") can be
			used in the path, but remember to escape a "\" character (i.e. write "\\"),
			otherwise it will be interpreted as a control character.
			@return True, if saved. */
		bool Save(const char* pPathAndFileName);
		/** Copies a CArrayOS2D to this array.
			The positions (xmin,ymin) and  (xmax,ymax) specifies the rectangle to
			be copied including the specified values. The method uses integers because
			of origo offset which may force the user to use values less than zero.
			If you want the resulting array placed in this array just use the same
			object for SourceArray. Note: In that case the method is a bit slower 
			(needs to copy a temporary array to the destination).
			@param Source Array to copy from.
			@param xmin defines left side of rectangle in source to copy from (including this value).
			@param ymin defines top side of rectangle in source to copy from (including this value).
			@param xmax defines right side of rectangle in source to copy from (including this value).
			@param ymax defines buttom side of rectangle in source to copy from	(including this value).
			@return false if outside array */
		bool CopySubArray(const CArrayOS2D<T>& Source,double xmin,double ymin,double xmax,double ymax);
		/** Copies a CArrayOS2D to this array.
			The positions UpperLeft and LowerRight specifies the rectangle to
			be copied including the specified values, both relative to origo. The method 
			uses integers because of origo offset which may force the user to use values
			less than zero.
			If you want the resulting array placed in this array just use the same
			object for SourceArray. Note: In that case the method is a bit slower 
			(needs to copy a temporary array to the destination).
			@param Source Array to copy from.
			@param UpperLeft position in the upper left corner to start from (including value).
			@param LowerRight position in the lower right corner to stop at (including value).
			@return false if outside array */
		bool CopySubArray(const CArrayOS2D<T>& Source,const CPoint2D<double>& UpperLeft,
				  const CPoint2D<double>& LowerRight);
		/** Writes all data to stream. */
		//friend ostream& operator<< __IPL_TF(ostream& s,const CArrayOS2D<T>& TS);
		/** Writes all data to stream. Format is:
			\verbatim
			Dimensions: w h
			Origo: (xh,yh)
			SubIndexing: s
			x0y0 x1y0 x2y0 ...
			x0y1 x1y1 x2y1 ...
			...
			\endverbatim

			Where w, h is width and height respectively, s is subindexing, and (xh,yh) is origo.
			Calls StreamIndent() for each streamed line. 
			Note: Due to problems with Visual C++ 6.0 and Borland 5.0 with template classes
			and friend functions, the definition needs to be in the header declaration, it helps
			a bit (but not in all cases) to bypass the bugs.
			@version 0.31 */
		friend ostream& operator<< __IPL_TF(ostream& s, const CArrayOS2D<T>& Array);
		/** Reads input from given stream into this class. Stream must be in correct format
			according to how the << ostream operator formats output.
			Note: Due to problems with Visual C++ 6.0 and Borland 5.0 with template classes
			and friend functions, the definition needs to be in the header declaration, it helps
			a bit (but not in all cases) to bypass the bugs. 
			@version 0.3 */
		friend istream& operator>> (istream& is, ipl::CArrayOS2D<T>& Array)
		{
			UINT32 w,h;
			unsigned int SubIndexing;
			is.ignore(std::numeric_limits<int>::max(),std::char_traits<char>::to_int_type(':')); // ignore 'Dimensions:'
			SkipSpaceAndComments(is);
			is >> w;
			SkipSpaceAndComments(is);
			is >> h;
			SkipSpaceAndComments(is);
			is.ignore(std::numeric_limits<int>::max(),std::char_traits<char>::to_int_type(':')); // ignore 'Origo:'
			SkipSpaceAndComments(is);

			CPoint2D<double> Origo;
			is >> Origo;
			SkipSpaceAndComments(is);
			is.ignore(std::numeric_limits<int>::max(),std::char_traits<char>::to_int_type(':')); // ignore 'SubIndexing:'
			SkipSpaceAndComments(is);
			is >> SubIndexing;
			Array.Alloc(w,h,SubIndexing);
			Array.SetOrigo(Origo);
			for(double y=Array.GetMinY(); y<Array.GetMaxY(); y+=Array.GetSubStep())
			{
				for(double x=Array.GetMinX(); x<Array.GetMaxX(); x+=Array.GetSubStep())
				{
					SkipSpaceAndComments(is);
					Array.operator[](x)[y] << is;
				}
			}
			return is;
		}
protected:
		/** Sets the attribute m_SubIndexing, and updates the corresponding
			attribute m_SubStep.
			@return False, if SubIndexing is 0. */
		inline bool SetSubIndexing(unsigned int SubIndexing);
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

template <class T>
inline CArrayOS2D<T>& CArrayOS2D<T>::operator=(const CArrayOS2D<T>& M)
{
	if (this->m_pData!=NULL)
	{
		delete this->m_pData;
		this->m_pData=NULL;
	}
	this->m_Width = M.GetWidth();
	this->m_Height = M.GetHeight();
	if ((M.GetWidth()!=0) && (M.GetHeight()!=0))
	{
		SetSubIndexing(M.GetSubIndexing());
		SetOrigo(M.GetOrigo());
		this->m_pData = new valarray<T>(m_SubIndexing*this->m_Width*m_SubIndexing*this->m_Height);
		*this->m_pData=M.Array();
	}
	return *this;
}

template <class T>
inline CArrayOS2D<T>& CArrayOS2D<T>::operator=(const T& M)
{
	CArray2D<T>::operator=(M);
	return *this;
}

template <class T>
inline UINT32 CArrayOS2D<T>::GetWidth() const
{
	return this->m_Width/m_SubIndexing;
}

template <class T>
inline UINT32 CArrayOS2D<T>::GetHeight() const
{
	return this->m_Height/m_SubIndexing;
}

template<class T> 
inline UINT32 CArrayOS2D<T>::GetWidthCells() const
{
	return CArray2D<T>::GetWidth();
}

template<class T> 
inline UINT32 CArrayOS2D<T>::GetHeightCells() const
{
	return CArray2D<T>::GetHeight();
}

template<class T> 
inline const CPoint2D<double> CArrayOS2D<T>::GetOrigo() const
{
	return m_Origo;
}

template<class T> 
inline void CArrayOS2D<T>::SetOrigo(const CPoint2D<double>& Origo)
{
	double x=RoundToSubValue(Origo.GetX());
	double y=RoundToSubValue(Origo.GetY());
	m_OrigoCells.SetX((int)(x*m_SubIndexing));
	m_OrigoCells.SetY((int)(y*m_SubIndexing));
	m_Origo.Set(x,y);
}

template<class T> 
inline void CArrayOS2D<T>::SetOrigo(double x, double y)
{
	SetOrigo(CPoint2D<double>(x,y));
}

template<class T> 
inline double CArrayOS2D<T>::GetMinX() const
{
	return -m_Origo.GetX();
}

template<class T> 
inline double CArrayOS2D<T>::GetMinY() const
{
	return -m_Origo.GetY();
}

template<class T> 
inline double CArrayOS2D<T>::GetMaxX() const
{
	return ((double)this->m_Width-m_OrigoCells.GetX())/(double)m_SubIndexing;
}

template<class T> 
inline double CArrayOS2D<T>::GetMaxY() const
{
	return ((double)this->m_Height-m_OrigoCells.GetY())/(double)m_SubIndexing;
}

template<class T> 
inline bool CArrayOS2D<T>::InRange(double x, double y) const
{
	int ix=GetNearestSubIndex(x)+m_OrigoCells.GetX();
	int iy=GetNearestSubIndex(y)+m_OrigoCells.GetY();
	return ((ix>=0) && (ix<=(int)(this->m_Width-1)) && (iy>=0) && (iy<=(int)(this->m_Height-1)));
}

template <class T>
inline bool CArrayOS2D<T>::InRange(const CPoint2D<double>& P) const
{
	int ix=GetNearestSubIndex(P.GetX())+m_OrigoCells.GetX();
	int iy=GetNearestSubIndex(P.GetY())+m_OrigoCells.GetY();
	return ((ix>=0) && (ix<=(int)(this->m_Width-1)) && (iy>=0) && (iy<=(int)(this->m_Height-1)));
}

template <class T>
inline typename CArrayOS2D<T>::Iterator CArrayOS2D<T>::Row(double i)
{
	return Iterator(this->m_pData,slice((size_t)((i+m_Origo.GetX())*m_SubIndexing),this->m_Width,this->m_Height),m_SubIndexing,&m_Origo);
}

template <class T>
inline typename CArrayOS2D<T>::ConstIterator CArrayOS2D<T>::ConstRow(double i) const
{
	return ConstIterator(this->m_pData,slice((size_t)((i+m_Origo.GetX())*m_SubIndexing),this->m_Width,this->m_Height),m_SubIndexing,&m_Origo);
}

template <class T>
inline typename CArrayOS2D<T>::Iterator CArrayOS2D<T>::Column(double i)
{
	return Iterator(this->m_pData,slice((size_t)((i+m_Origo.GetX())*m_SubIndexing*this->m_Height),this->m_Height,1),m_SubIndexing,&m_Origo);
}

template <class T>
inline typename CArrayOS2D<T>::ConstIterator CArrayOS2D<T>::ConstColumn(double i) const
{
	return ConstIterator(this->m_pData,slice((size_t)((i+m_Origo.GetX())*m_SubIndexing*this->m_Height),this->m_Height,1),m_SubIndexing,&m_Origo);
}

template <class T>
inline const T& CArrayOS2D<T>::Get(double x, double y) const
{
	int ix=GetNearestSubIndex(x)+m_OrigoCells.GetX();
	int iy=GetNearestSubIndex(y)+m_OrigoCells.GetY();
	if ((ix>=0) && (ix<=(int)(this->m_Width-1)) && (iy>=0) && (iy<=(int)(this->m_Height-1)))
		return CArray2D<T>::operator[](ix)[iy];
	else
		return CArray2D<T>::operator[](m_OrigoCells.GetX())[m_OrigoCells.GetY()];;
}

template <class T>
inline const T& CArrayOS2D<T>::Get(const CPoint2D<double>& P) const
{
	int ix=GetNearestSubIndex(P.GetX())+m_OrigoCells.GetX();
	int iy=GetNearestSubIndex(P.GetY())+m_OrigoCells.GetY();
	if ((ix>=0) && (ix<=(int)(this->m_Width-1)) && (iy>=0) && (iy<=(int)(this->m_Height-1)))
		return CArray2D<T>::operator[](ix)[iy];
	else
		return CArray2D<T>::operator[](m_OrigoCells.GetX())[m_OrigoCells.GetY()];;
}

template <class T>
inline const T& CArrayOS2D<T>::GetFast(double x, double y) const
{
	int ix=GetNearestSubIndex(x)+m_OrigoCells.GetX();
	int iy=GetNearestSubIndex(y)+m_OrigoCells.GetY();
	return CArray2D<T>::operator[](ix)[iy];
}

template <class T>
inline const T& CArrayOS2D<T>::GetFast(const CPoint2D<double>& P) const
{
	int ix=GetNearestSubIndex(P.GetX())+m_OrigoCells.GetX();
	int iy=GetNearestSubIndex(P.GetY())+m_OrigoCells.GetY();
	return CArray2D<T>::operator[](ix)[iy];
}

template <class T>
inline bool CArrayOS2D<T>::Set(double x, double y, const T& Val)
{
	int ix=GetNearestSubIndex(x)+m_OrigoCells.GetX();
	int iy=GetNearestSubIndex(y)+m_OrigoCells.GetY();
	if ((ix>=0) && (ix<=(int)(this->m_Width-1)) && (iy>=0) && (iy<=(int)(this->m_Height-1)))
	{
		CArray2D<T>::operator[](ix)[iy]=Val;
		return true;
	}
	else
		return false;
}

template <class T>
inline bool CArrayOS2D<T>::Set(const CPoint2D<double>& P, const T& Val)
{
	int ix=GetNearestSubIndex(P.GetX())+m_OrigoCells.GetX();
	int iy=GetNearestSubIndex(P.GetY())+m_OrigoCells.GetY();
	if ((ix>=0) && (ix<=(int)(this->m_Width-1)) && (iy>=0) && (iy<=(int)(this->m_Height-1)))
	{
		CArray2D<T>::operator[](ix)[iy]=Val;
		return true;
	}
	else
		return false;
}

template <class T>
inline void CArrayOS2D<T>::SetFast(double x, double y, const T& Val)
{
	int ix=GetNearestSubIndex(x)+m_OrigoCells.GetX();
	int iy=GetNearestSubIndex(y)+m_OrigoCells.GetY();
	CArray2D<T>::operator[](ix)[iy]=Val;
}

template <class T>
inline void CArrayOS2D<T>::SetFast(const CPoint2D<double>& P, const T& Val)
{
	int ix=GetNearestSubIndex(P.GetX())+m_OrigoCells.GetX();
	int iy=GetNearestSubIndex(P.GetY())+m_OrigoCells.GetY();
	CArray2D<T>::operator[](ix)[iy]=Val;
}

template <class T>
inline double CArrayOS2D<T>::RoundToSubValue(const double& Val) const
{
	double n = Val*m_SubIndexing;
	n > 0 ? n+=0.5 : n-=0.5;
	return ((int)n)*m_SubStep;
}

template <class T>
inline unsigned int CArrayOS2D<T>::GetSubIndexing() const
{
	return m_SubIndexing-1;
}

template <class T>
inline int CArrayOS2D<T>::GetNearestSubIndex(const double& Val) const
{
	double n = Val*m_SubIndexing;
	n > 0 ? n+=0.5 : n-=0.5;
	return (int)n;
}

template <class T>
inline bool CArrayOS2D<T>::SetSubIndexing(unsigned int SubIndexing)
{
	SubIndexing++; // internal stored value is added one to avoid some calculation later on
	m_SubIndexing=SubIndexing;
	m_SubStep=1.0/(double)m_SubIndexing;
	return true;
}

template <class T>
inline const T& CArrayOS2D<T>::operator()(const CPoint2D<double>& P) const
{
	return operator[](P.GetX())[P.GetY()];
}

template <class T>
inline T& CArrayOS2D<T>::operator()(const CPoint2D<double>& P)
{
	return operator[](P.GetX())[P.GetY()];
}

template <class T>
std::ostream& operator<<(std::ostream& s, const CArrayOS2D<T>& Array)
{
	s << StreamIndent << "Dimensions: " << Array.GetWidth() << " " << Array.GetHeight() << endl;
	s << StreamIndent << "Origo: " << Array.GetOrigo() << endl;
	s << StreamIndent << "SubIndexing: " << Array.GetSubIndexing() << endl;
	for(double y=0; y<Array.GetHeight(); y+=Array.GetSubStep())
	{
		for(double x=0; x<Array.GetWidth(); x+=Array.GetSubStep())
		{
			s << Array[x][y] << " ";
		}
		s << "\n" << StreamIndent;
	}
	s << StreamIndent;
	return s;
}

#include "arrayos2d.cpp"

/*@}*/ /* end group "containerclasses" */

} // end namespace ipl

#endif //_IPL98_ARRAYOS2D_H
