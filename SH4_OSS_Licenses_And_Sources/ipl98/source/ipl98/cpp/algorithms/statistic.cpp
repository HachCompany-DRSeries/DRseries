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

#include "statistic.h"
#include "../int_image.h"

namespace ipl{

CStatistic::CStatistic()
{
	k_InitStatistic(&m_Statistic);
	m_StatisticsDerived=false;
}


CStatistic::~CStatistic()
{
	k_EmptyStatistic(&m_Statistic);
}

double CStatistic::GetMean() const
{
	if (m_StatisticsDerived==false)
	{
		ostringstream ost;
		ost << "CStatistic::GetMean() Statistics not yet derived - call DeriveStats() first" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0.0;
	}
	else
	{
		return m_Statistic.Mean;
	}
}

double CStatistic::GetMin() const
{
	if (m_StatisticsDerived==false)
	{
		ostringstream ost;
		ost << "CStatistic::GetMin() Statistics not yet derived - call DeriveStats() first" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0.0;
	}
	else
	{
		return m_Statistic.Min;
	}
}

double CStatistic::GetMax() const
{
	if (m_StatisticsDerived==false)
	{
		ostringstream ost;
		ost << "CStatistic::GetMax() Statistics not yet derived - call DeriveStats() first" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0.0;
	}
	else
	{
		return m_Statistic.Max;
	}
}

double CStatistic::GetStdDev() const
{
	if (m_StatisticsDerived==false)
	{
		ostringstream ost;
		ost << "CStatistic::GetStdDev() Statistics not yet derived - call DeriveStats() first" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0.0;
	}
	else
	{
		return m_Statistic.StdDev;
	}
}

unsigned int CStatistic::GetSizeOfHistogram() const
{
	if (m_StatisticsDerived==false)
	{
		ostringstream ost;
		ost << "CStatistic::GetSizeOfHistogram() Statistics not yet derived - call DeriveStats() first" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
	else
	{
		return m_Statistic.SizeOfHistogram;
	}
}

UINT32* CStatistic::GetHistogramPtr()
{
	if (m_StatisticsDerived==false)
	{
		ostringstream ost;
		ost << "CStatistic::GetHistogramPtr() Statistics not yet derived - call DeriveStats() first" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return NULL;
	}
	else
	{
		return m_Statistic.pHistogram;
	}
}

bool CStatistic::DeriveStats(const CFloatImage& Image)
{
	bool ReturnValue=k_DeriveFloatImageStats(&m_Statistic,Image.GetConstTFloatImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStatistic::DeriveStats(CFloatImage) Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
	{
		m_StatisticsDerived=true;
	}
	return ReturnValue;	
}

bool CStatistic::DeriveStats(const CIntImage& Image)
{
	bool ReturnValue=k_DeriveIntImageStats(&m_Statistic,Image.GetConstTIntImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStatistic::DeriveStats(CIntImage) Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
	{
		m_StatisticsDerived=true;
	}
	return ReturnValue;	
}

bool CStatistic::DeriveStats(const CStdImage& Image)
{
	bool ReturnValue=k_DeriveImageStats(&m_Statistic,Image.GetConstTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStatistic::DeriveStats(CStdImage) Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
	{
		m_StatisticsDerived=true;
	}
	return ReturnValue;	
}

bool CStatistic::DeriveStats(const CPixelGroup& PixelGroup)
{
	bool ReturnValue=k_DerivePixelGroupStats(&m_Statistic,PixelGroup.GetConstTPixelGroupPtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CStatistic::DeriveStats(CPixelGroup) Failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
	{
		m_StatisticsDerived=true;
	}
	return ReturnValue;	
}

ostream& operator<<(ostream& s,const CStatistic& Stat)
{
	s << StreamIndent << "**************** Statistic info ******************\n" << 
		StreamIndent << " Min=" << Stat.GetMin() << " Max=" << Stat.GetMax() <<
		" Mean=" << Stat.GetMean() << " StdDev=" <<
		Stat.GetStdDev() << "\n SizeOfHistogram=" << Stat.GetSizeOfHistogram();	
	return s;
}


} // end ipl namespace
