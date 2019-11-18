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

#ifndef _IPL98_STATISTIC_H
#define _IPL98_STATISTIC_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include <vector>
#include "../float_image.h"
#include "../pixelgroup.h"
#include "../../kernel_c/algorithms/kernel_statistic.h"

namespace ipl{ // use namespace if C++

using std::vector;

/** @ingroup utilities
@{ */

/** CStatistic calculates minimum, maximum, mean, standard deviation
	and a histogram for a given image, Last updated 12/29/2005.
	Note that a histogram is not created for the CFloatImage type.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CStatistic ipl98/cpp/algorithms/statistic.h
	@version 0.81
	@author René Dencker Eriksen edr@mip.sdu.dk */
class CStatistic{
	private: // attributes
		TStatistic m_Statistic;
		bool m_StatisticsDerived;
	public: // methods
		/// default constructor
		CStatistic();
		/// default destructor
		~CStatistic();
		/** Gets the found mean value in image or pixelgroup.
			If statistics is not yet calculated an error message is produced.
			@return Mean value. */
		double GetMean() const;
		/** Gets the found minimum value in image or pixelgroup.
			If statistics is not yet calculated an error message is produced.
			@return Min value. */
		double GetMin() const;
		/** Gets the found maximum value in image or pixelgroup.
			If statistics is not yet calculated an error message is produced.
			@return Max value. */
		double GetMax() const;
		/** Gets the found standard deviation in image or pixelgroup.
			If statistics is not yet calculated an error message is produced.
			@return Standard deviation. */
		double GetStdDev() const;
		/** Gets the size of the histogram created from image, that is the number
			of different color indexes.
			If statistics is not yet calculated or if no histogram is available
			(see the DeriveStats methods) an error message is produced.
			@return Size of histogram. */
		unsigned int GetSizeOfHistogram() const;
		/** A pointer to the histogram is returned. Note that the indexing from
			the returned pointer is different depending on the original image.
			For an CIntImage the indexing is from -32768 to 32767 (min and max 
			values for the INT16 type), for a CStdImage with 1 b/p it is from 0 to 1,
			and for a CStdImage with 8 b/p it is from 0 to 255.
			If statistics is not yet calculated or if no histogram is available
			(see the DeriveStats methods) an error message is produced.
			@return Pointer to the histogram array or NULL if not available. */
		UINT32* GetHistogramPtr();
		/** Derives statistical information for the given image. No histogram is created.
			@param Image The image to calculate statistics for.
			@return False, if the image is empty. */
		bool DeriveStats(const CFloatImage& Image);
		/** Derives statistical information for the given image. No histogram is created.
			@param Image The image to calculate statistics for.
			@return False, if the image is empty. */
		bool DeriveStats(const CIntImage& Image);
		/** Derives statistical information for the given integer image. A histogram 
			with 65536 entries is created. Indexing is from -32768 to 32767 (min and 
			max values for the INT16 type).
			@param Image The image to calculate statistics for.
			@return False, if the image is empty. */
		bool DeriveStats(const CStdImage& Image);
		/**	Derives statistical information for the given pixelgroups color values.
			No histogram is created.
			@param PixelGroup The pixelgroup to calculate statistics for.
			@return False, if the pixelgroup is empty or if no colors are available. */
		bool DeriveStats(const CPixelGroup& PixelGroup);
		/**	Derives statistical information for the values contained in the given vector.
			The vector type can be any of the primitive types such as int, float, and double.
			No histogram is created.
			@param Values The array of values to calculate statistics for.
			@return False, if the provided array Values is empty. */
		template<class POLY> bool DeriveStats(const vector<POLY>& Values)
		{
			unsigned int TotalValues=Values.size();
			if (TotalValues==0)
			{
				ostringstream ost;
				ost << "CStatistic::DeriveStats(vector<T>) No values in given vector" << IPLAddFileAndLine;
				CError::ShowMessage(IPL_ERROR,ost);
				return false;
			}
			else
			{
				POLY Value;
				double Sum=0.0;
				double Sum2=0.0;
				typename std::vector<POLY>::const_iterator it=Values.begin();
				m_Statistic.SizeOfHistogram=0; /* no histogram */
				m_Statistic.Min=m_Statistic.Max=*it; /* init min and max */
				while(it!=Values.end())
				{
					Value=*it++;
					if (Value<m_Statistic.Min) m_Statistic.Min=Value;
					else if (Value>m_Statistic.Max) m_Statistic.Max=Value;
					Sum+=Value;
					Sum2+=Value*Value;
				}
				m_Statistic.Mean=Sum/TotalValues;
				m_Statistic.StdDev=sqrt(Sum2/TotalValues-m_Statistic.Mean*m_Statistic.Mean);
				m_StatisticsDerived=true;
				return true;
			}
		}

		/** @name streaming operators */
		//@{
		/** Writes the statistical information to stream, the user must
			ensure that it is available by a previous successful call to
			a DeriveStats method.
			Calls StreamIndent() for each streamed line. */
		friend ostream& operator<<(ostream& s,const CStatistic& Img);
		//@}
};

/////////////////////////////////////////////////
//// Inline methods
/////////////////////////////////////////////////

/*@}*/ /* end group "utilities" */

} // end namespace ipl

#endif //_IPL98_STATISTIC_H
