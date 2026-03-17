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

#include "hough_probrht_line.h"
#include <time.h>

namespace ipl {

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CHoughProbRHTLine::CHoughProbRHTLine() : m_os(cout)
{
}


CHoughProbRHTLine::CHoughProbRHTLine(ostream& os, bool WriteOutputFiles, const string& FilePrefix) : m_os(os)
{
	m_writeOutputFiles = WriteOutputFiles;
	m_filePrefix = FilePrefix;
}


CHoughProbRHTLine::~CHoughProbRHTLine()
{
}


//////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////


bool CHoughProbRHTLine::Initialize(float Ratio, CHoughBase::CBinaryImage& Source, int Thickness, 
                                   CHoughBase::PARAMETERSPACETYPE SpaceType, CHoughBase::PARAMETERTYPE ParamType)
{
	this->PerformSelection(Ratio, Source);
	return m_rht_line.Initialize(Source, Thickness, SpaceType, ParamType);
}


bool CHoughProbRHTLine::Initialize(float Ratio, const CStdImage& Source, COLORTYPE Color, UINT8 Threshold, 
                                   CHoughBase::ORIGINTYPE OriginType, int Thickness, 
                                   CHoughBase::PARAMETERSPACETYPE SpaceType, CHoughBase::PARAMETERTYPE ParamType)
{
	CHoughBase::CBinaryImage InImage(Source, Color, Threshold, OriginType);
	this->PerformSelection(Ratio, InImage);
	return m_rht_line.Initialize(InImage, Thickness, SpaceType, ParamType);
}


vector<CParameter2D<float> > CHoughProbRHTLine::PerformTransform(int Threshold, int MaxNumShapes)
{
	return m_rht_line.PerformTransform(Threshold, MaxNumShapes);
}


void CHoughProbRHTLine::PerformSelection(float Ratio, CHoughBase::CBinaryImage& Source)
{
	if (0.0f<Ratio && Ratio<=1.0f)
	{
		int k = (int)((1.0-Ratio)*Source.GetSize());
		srand( (unsigned) time( NULL ) );
		while (k!=0)
		{
			int pos = Round(1.0*rand()/RAND_MAX*Source.GetSize());
			Source.RemovePixel(pos);
			k--;
		}
	}
}

} // end namespace ipl
