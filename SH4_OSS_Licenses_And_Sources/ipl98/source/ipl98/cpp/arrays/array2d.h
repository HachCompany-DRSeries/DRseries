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

#ifndef _IPL98_ARRAY2D_H
#define _IPL98_ARRAY2D_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "array2d_it.h"
#include "array2d_const_it.h"
#include "../error.h"
#include "../ipl98_general_functions.h"
#include "../../../points/point2d.h"
#include<iostream>
#include<ostream>
#include<fstream> 
#include<valarray>
#include<algorithm>
#include<limits>

namespace ipl{ // use namespace if C++

using std::valarray;
using std::ifstream;
using std::ofstream;
using std::ostream;
using std::numeric_limits;
//using ipl::CError;

/** @defgroup containerclasses Container classes (C++)
	Container classes are generic classes in IPL98 containing data such as
	pixels in an image etc. They contain some methods directly on the
	container classes, but more advanced algorithms working on the 
	data are found in the \link algorithms algorithms \endlink section. This section
	also includes more simple container classes such as CPoint2D (containing
	2D positions) and 3D geometry classes such as a CPlane and CVector3D. */

/** @ingroup containerclasses
@{ */

/** A 2D array to be used as a container, last updated 12/29/2005. The main
	difference from this container class compared to the standard image
	container classes is, that it is template based so you can put your 
	own types in it. A cell can be indexed as a double	array with [x][y] 
	and the performance is just as fast as indexing	into a conventional 
	ANSI C double array. All indices are zero-indexed. In addition an
	iterator class is available to iterate through the data in both columns
	and rows. An internal iterator has been defined (by a typedef), it
	means this class can be used with iterators the same way the standard
	library uses iterators, only different is the naming convention, where
	we here stick to the IPL98 naming convention. Example on using this
	class with it's iterators:

	\verbatim
	
	// initializes an array and prints a row and colomn from it
	int x=5,y=5;
	CArray2D<double> Arr(x,y);
	
	for(int C1=0; C1<x; C1++)
		for(int C2=0; C2<y; C2++)
			Arr[C1][C2]=x*C1+C2;

	cout << Arr << endl;
	// print a row
	cout << "Row 2: ";
	CArray2D<double>::Iterator it=Arr.Row(2);
	for(; it!=it.End(); it++)
		cout << *it << " ";
	// print a column
	it=Arr.Column(2);
	cout  << endl << "Column 2: ";
	for(; it!=it.End(); it++)
		cout << *it << " ";
	\endverbatim

	Supplied template types must have the following operators defined: <<, >> and =.
	Since not all operators makes sence for some types, only the
	above mentioned need to be implemented (expecting that the = operator 
	always makes sence). The operator << and >> is for streaming, see the friend
	functions for more information.
	
	Because only these operators are needed, not all methods from the image classes are
	available here, for instance CImage::Bias() will
	assume the + operator to be available, hence it is not available in this class.
	At the moment you can only use some of the container classes from IPL98, namely:
	CPoint2D<T>, CVector2D<T>, CPoint3D<T>, and CVector3D.
	
	It is also possible to expand this class 2*n dimensions, where integer n >= 1. For
	instance to create a 4 dimensional array, declare a variable like this:
	\verbatim

	CArray2D< CArray2D<double> > Arr4Dim(3,4);
	\endverbatim

	In this example the first two dimensions are 3 and 4, but you need to explicit allocate
	each element for dimension 3 and 4. 
	WARNING: Some compiler have bugs when dealing with recursive template use at the time of
	writing this. You cannot with Visual C++ 6.0 do the above to gain higher dimensions, although
	it seems to work with both Borland 4.0 and G++ 3.0.3.

	This class is build	from the basic example in C++ Programming Language Third 
	Edition p. 672 by Bjarne Stroustrup. Some of the code has been moved inside the
	declaration of the class, since some compilers have problems inlining the code
	if it is not shown at that early point.
	
	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CArray2D ipl98/cpp/arrays/array2d.h
	@version 0.38,  by edr@mip.sdu.dk
	@author René Dencker Eriksen
	@see CArray2DIterator
	@see CArray2DConstIterator
	@see CArrayOS2D */
template<class T>
class CArray2D
{
protected: // attributes
	/** stores elements by column, the method Array can be used
	    to get a handle to this data. */
    valarray<T>* m_pData;
	/// number of columns
    UINT32 m_Width;
	/// number of rows
	UINT32 m_Height;
	/** Originating point of array. */
	ORIGIN m_Origin;
public:
		/** Default constructor. */
        CArray2D();
		/** Constructor defining the array.
			@param x Width of array.
			@param y Height of array. */
        CArray2D(UINT32 x, UINT32 y);
		/// copy constructor
		CArray2D(const CArray2D<T>& M);
		/// overloading = operator
		inline CArray2D& operator=(const CArray2D& M);
		/** overloading = operator for setting each element to specified value.
			Works as the standard library = operator for a valarray. */
		inline CArray2D& operator=(const T& M);
		/// default destructor
        virtual ~CArray2D();
		/** Deletes all contents of this object, sets size to 0. */
		virtual void Empty();
		/** Checks if any data is present.
			@return True, if array contain no data. */
		inline bool IsEmpty() const;
		/** Sets the size of array. It is not efficient to
			reallocate the matrix, if size is known at creation
			time, use the constructor once for all.
			@return true, if allocation succeeded. */
		virtual bool Alloc(UINT32 x, UINT32 y);
        /** Returns the total number of elements in array.
			@return Total number of elements. */
        UINT32 Size() const;
		/** Returns the origin of array contained in attribute m_Origin.
			@return the origin of array */
		inline ORIGIN GetOrigin() const;
		/** Returns the width of the array.
			@return Width of array. */
        inline UINT32 GetWidth() const;
		/** Returns the height of the array.
				@return Height of array. */
        inline UINT32 GetHeight() const;
		/** Returns true if index (x,y) is valid.
			@return true if index (x,y) is valid. */
        inline bool InRange(UINT32 x, UINT32 y) const;
		/** Same as InRange(x,y), except the parameters are given as a CPoint2D. */
        inline bool InRange(const CPoint2D<UINT32>& P) const;
		/** Iterate from this value.
			@return min. X.
			@see GetMaxX() */
		virtual inline double GetMinX() const;
		/** Iterate from this value.
			@return min. Y.
			@see GetMaxY() */
		virtual inline double GetMinY() const;
		/** Iterate to this value minus one (the array is zero indexed).
			@return max. X.
			@see GetMinX() */
		virtual inline double GetMaxX() const;
		/** Iterate to this value minus one (the array is zero indexed).
			@return max. Y.
			@see GetMinY() */
		virtual inline double GetMaxY() const;
		/** Iterator for the CArray2D class. This typedef is included to make
			the class work like the std library. Instantiate an iterator object
			like this:
			\verbatim
			CArray2D<T>::Iterator it;
			\endverbatim */
		typedef CArray2DIterator<T> Iterator;
		/** Constant iterator for the CArray2D class. This typedef is included to make
			the class work like the std library. Instantiate a const iterator object
			like this:
			\verbatim
			CArray2D<T>::ConstIterator it;
			\endverbatim */
		typedef CArray2DConstIterator<T> ConstIterator;
		/** Returns an iterator to row number i.
			@param i Row number (zero indexed).
			@return iterator.
			@see CArray2DIt */
        virtual inline Iterator Row(UINT32 i);
		/** Returns a const iterator to row number i.
			@param i Row number (zero indexed).
			@return const iterator.
			@see CArray2DConstIterator */
        virtual inline ConstIterator ConstRow(UINT32 i) const;
		/** Returns an iterator to column number i.
			@param i Column number (zero indexed).
			@return iterator.
			@see CArray2DIt */
        virtual inline Iterator Column(UINT32 i);
		/** Returns a const iterator to column number i.
			@param i Column number (zero indexed).
			@return const iterator.
			@see CArray2DConstIterator */
        virtual inline const ConstIterator ConstColumn(UINT32 i) const;
		/** Read element at position P.
			@return Returns a const referense to value at position P. */
		virtual inline const T& operator()(const CPoint2D<UINT32>& P) const;
		/** Read or write element at position P.
			@return Returns a referense to value at position P. */
		virtual inline T& operator()(const CPoint2D<UINT32>& P);
		/** Returns an iterator for column i. Use another [] to index
			the row number. */
        virtual inline Iterator operator[](UINT32 i) { return Iterator(m_pData,slice(i*m_Height,m_Height,1)); }   // C-style subscript
		/** Returns a const iterator for column i. Use another [] to index
			the row number. */
        virtual inline ConstIterator operator[](UINT32 i) const { return ConstIterator(m_pData,slice(i*m_Height,m_Height,1)); }
		/** Returns a pointer to the data in the double array. The data is arranged in a
			one dimensianal array of type valarray from the std-library. Column i starts
			at index i*Height where Height is the height of the two dimensional array.
			@return A one dimensinal array where all data are stored */
        inline valarray<T>& Array() { return *m_pData; }
		/** Returns a constant pointer to the data in the double array. The data is arranged in a
			one dimensianal array of type valarray from the std-library. Column i starts
			at index i*Height where Height is the height of the two dimensional array.
			@return A one dimensinal array where all data are stored */
        inline const valarray<T>& Array() const { return *m_pData; }
		/** Load file from disk. File must be in correct format according
			to how the Save() method formats output. Note: There is no check at the
			moment whether the type T of this class is the same as the array being read,
			reading from a file with another template type leads to undefined results.
			Note: Comment lines (starting with a #) and whitespace can be put anywhere 
			within the stream.
			@param pPathAndFileName Name of file including extension and relative
			or absolute path, f.ex. absolut path "c:/temp/myarray2d.bmp",
			relative "myarray2d.txt". Both slash ("/") and back slash ("\") can be
			used in the path, but remember to escape a "\" character (i.e. write "\\"),
			otherwise it will be interpreted as a control character.
			@return false if load failed. */
		virtual bool Load(const char* pPathAndFileName);
		/** Save file to disk. First a comment line is generated, then a token
			"CArray2D", then a token "Dimensions:" followed by two integers (width and height).
			After that the 2D array is written. Each element is written using the streaming
			operator on that element.
			@param pPathAndFileName Name of file including extension and relative
			or absolute path, f.ex. absolut path "c:/temp/myarray2d.txt",
			relative "myarray2d.txt". Both slash ("/") and back slash ("\") can be
			used in the path, but remember to escape a "\" character (i.e. write "\\"),
			otherwise it will be interpreted as a control character.
			@return True, if saved. */
		virtual bool Save(const char* pPathAndFileName);
		/** Sets all values in the matrix to specified value.
			The same functionality as the = operator given a value of type T.
			@param M Value to be set in each element in this matrix. */
		inline void Flush(const T& M);
		/** Copies a CArray2D to this array.
			The positions (xmin,ymin) and  (xmax,ymax) specifies the rectangle to
			be copied including the specified values.
			If you want the resulting array placed in this array just use the same
			object for SourceArray. Note: In that case the method is a bit slower 
			(needs to copy a temporary array to the destination).
			@param Source Array to copy from.
			@param xmin defines left side of rectangle in source to copy from (including this value).
			@param ymin defines top side of rectangle in source to copy from (including this value).
			@param xmax defines right side of rectangle in source to copy from (including this value).
			@param ymax defines buttom side of rectangle in source to copy from	(including this value).
			@return false if outside array */
		virtual bool CopySubArray(const CArray2D<T>& Source,unsigned int xmin,unsigned int ymin, 
															unsigned int xmax,unsigned int ymax);
		/** Copies a CArray2D to this array.
			The positions UpperLeft and LowerRight specifies the rectangle to
			be copied including the specified values.
			If you want the resulting array placed in this array just use the same
			object for SourceArray. Note: In that case the method is a bit slower 
			(needs to copy a temporary array to the destination).
			@param Source Array to copy from.
			@param UpperLeft position in the upper left corner to start from (including value).
			@param LowerRight position in the lower right corner to stop at (including value).
			@return false if outside array */
		virtual bool CopySubArray(const CArray2D<T>& Source,const CPoint2D<unsigned int >& UpperLeft,
				  const CPoint2D<unsigned int >& LowerRight);
		/** Rotates array clockwise in step of 90 degrees.
			Note: Not speed optimized!
			@param Steps Rotate image 90 degrees * Steps */
		void Rotate90(int Steps);
		/** Flips array around a centered horisontal line.
			Note: Not speed optimised! */
		void FlipHorisontal();
		/** Flips array around a centered vertical line.
			Note: Not speed optimised! */
		void FlipVertical();
		/** Performs a displacement so that (dx,dy).
			in the old picture becomes (0,0). Periodic continuation
			is assumed.
			@param dx is the horizontal displacement
			@param dy is the vertical displacement
			@return False if the array is empty. */
		bool CopyDisplace(int dx,int dy);
		/** Writes all data to stream. Format is:
			\verbatim
			Dimensions: w h
			x0y0 x1y0 x2y0 ...
			x0y1 x1y1 x2y1 ...
			...
			\endverbatim

			Where w is width of array and h is height. Calls StreamIndent() for each streamed line. 
			Note: Due to problems with Visual C++ 6.0 and Borland 5.0 with template classes
			and friend functions, the definition needs to be in the header declaration, it helps
			a bit (but not in all cases) to bypass the bugs.
			@version 0.4 */
		friend ostream& operator<<(ostream& s, const CArray2D<T>& Array)
		{
			s << StreamIndent << "Dimensions: " << Array.GetWidth() << " " << Array.GetHeight() 
				<< endl << StreamIndent;
			for(UINT32 y=0; y<Array.GetHeight(); y++)
			{
				for(UINT32 x=0; x<Array.GetWidth(); x++)
					s << Array[x][y] << " ";
				s << "\n" << StreamIndent;
			}
			s << endl;
			return s;
		}
		/** Reads input from given stream into this class. Stream must be in correct format
			according to how the << ostream operator formats output.
			Note: Due to problems with Visual C++ 6.0 and Borland 5.0 with template classes
			and friend functions, the definition needs to be in the header declaration, it helps
			a bit (but not in all cases) to bypass the bugs.
			@version 0.3 */
		friend istream& operator>>(istream& is, CArray2D<T>& Array)
		{
			UINT32 w,h;
			SkipSpaceAndComments(is);
			is.ignore(std::numeric_limits<int>::max(),std::char_traits<char>::to_int_type(':'));
			SkipSpaceAndComments(is);
			is >> w;
			SkipSpaceAndComments(is);
			is >> h;
			Array.Alloc(w,h);
			for(UINT32 y=0; y<h; y++)
			{
				for(UINT32 x=0; x<w; x++)
				{
					SkipSpaceAndComments(is);
					//operator[](x)[y] << s;
					is >> Array.operator[](x)[y];
				}
			}
			return is;
		}
};


/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

template <class T>
inline CArray2D<T>& CArray2D<T>::operator=(const CArray2D<T>& M)
{
	if (m_pData!=NULL)
	{
		delete m_pData;
		m_pData=NULL;
	}
	m_Width = M.GetWidth();
	m_Height = M.GetHeight();
	if ((M.GetWidth()!=0) && (M.GetHeight()!=0))
	{
		m_pData = new valarray<T>(m_Width*m_Height);
		*m_pData=M.Array();
		m_Origin=RAM;
	}
	return *this;
}

template <class T>
inline CArray2D<T>& CArray2D<T>::operator=(const T& M)
{
	*m_pData=M;
	m_Origin=RAM;
	return *this;
}

template <class T>
inline bool CArray2D<T>::IsEmpty() const
{
	return (m_pData==NULL);
}

template <class T>
inline ORIGIN CArray2D<T>::GetOrigin() const
{
	return m_Origin;
}

template <class T>
inline UINT32 CArray2D<T>::GetWidth() const
{ 
	return m_Width;
}

template <class T>
inline UINT32 CArray2D<T>::GetHeight() const
{
	return m_Height;
}

template <class T>
inline bool CArray2D<T>::InRange(UINT32 x, UINT32 y) const 
{
	return ((x<m_Width) && (y<m_Height));
}

template <class T>
inline bool CArray2D<T>::InRange(const CPoint2D<UINT32>& P) const
{
	return ((P.GetX()<m_Width) && (P.GetY()<m_Height));
}

template <class T>
inline double CArray2D<T>::GetMinX() const
{
	return 0;
}

template <class T>
inline double CArray2D<T>::GetMinY() const
{
	return 0;
}

template <class T>
inline double CArray2D<T>::GetMaxX() const
{
	return m_Width;
}

template <class T>
inline double CArray2D<T>::GetMaxY() const
{
	return m_Height;
}

template <class T>
inline typename CArray2D<T>::Iterator CArray2D<T>::Row(UINT32 i)
{
	return Iterator(m_pData,slice(i,m_Width,m_Height));
	//return CArray2DIt<T>(m_pData,slice(i,m_Width,m_Height));
}

template <class T>
inline typename CArray2D<T>::ConstIterator CArray2D<T>::ConstRow(UINT32 i) const
{
	return ConstIterator(m_pData,slice(i,m_Width,m_Height));
}

template <class T>
inline typename CArray2D<T>::Iterator CArray2D<T>::Column(UINT32 i)
{
	return Iterator(m_pData,slice(i*m_Height,m_Height,1));
	//return CArray2DIt<T>(m_pData,slice(i*m_Height,m_Height,1));
}

template <class T>
inline const typename CArray2D<T>::ConstIterator CArray2D<T>::ConstColumn(UINT32 i) const
{
	return ConstIterator(m_pData,slice(i*m_Height,m_Height,1));
}

template <class T>
inline void CArray2D<T>::Flush(const T& M)
{
	operator=(M);
}

template <class T>
inline const T& CArray2D<T>::operator()(const CPoint2D<UINT32>& P) const
{
	return operator[](P.GetX())[P.GetY()];
}

template <class T>
inline T& CArray2D<T>::operator()(const CPoint2D<UINT32>& P)
{
	return operator[](P.GetX())[P.GetY()];
	//return 0;
}

#include "array2d.cpp"

} // end namespace ipl

/*@}*/ /* end group "containerclasses" */

#endif //_IPL98_ARRAY2D_H
