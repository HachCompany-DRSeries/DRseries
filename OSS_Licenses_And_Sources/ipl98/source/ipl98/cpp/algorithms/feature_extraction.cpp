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

#include "feature_extraction.h"

namespace ipl{

CFeatures::CFeatures()
{
	m_MomOK=false;
	m_CentrMomOK=false;
	m_CentrNormMomOK=false;
	m_HuOK=false;
}

CFeatures::~CFeatures()
{
}

CPoint2D<FLOAT32> CFeatures::DeriveCenterOfMassBinary(const CPixelGroup& PixelGroup)
{
	T2DFloat CM;
	CM=k_DeriveCenterOfMassBinary(PixelGroup.GetConstTPixelGroupPtr());
	return CPoint2D<FLOAT32>(CM.x,CM.y);
}

bool CFeatures::DeriveMomentsGrayTone(const CPixelGroup& PixelGroup,double Background,
									 unsigned int MaxOrder)
{
	bool returnValue=k_DeriveMomentsGrayTone(PixelGroup.GetConstTPixelGroupPtr(),Background,MaxOrder,&m_Moments);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CFeatures::DeriveMomentsGrayTone() Failed deriving moments" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
		m_MomOK=true;
	return returnValue;
}

bool CFeatures::DeriveMomentsBinary(const CPixelGroup& PixelGroup,unsigned int MaxOrder)
{
	//bool returnValue=k_DeriveMomentsBinary(&PixelGroup,MaxOrder,&m_Moments);
	bool returnValue=k_DeriveMomentsBinary(PixelGroup.GetConstTPixelGroupPtr(),MaxOrder,&m_Moments);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CFeatures::DeriveMomentsBinary() Failed deriving moments" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
		m_MomOK=true;
	return returnValue;
}

bool CFeatures::DeriveCentralMoments()
{
	if (m_MomOK==false)
	{
		ostringstream ost;
		ost << "CFeatures::DeriveCentralMoments() Simple moments not derived" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	bool returnValue=k_DeriveCentralMoments(&m_Moments,&m_CentrMom);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CFeatures::DeriveMomentsGrayTone() Failed deriving moments" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
		m_CentrMomOK=true;
	return returnValue;
}

bool CFeatures::DeriveCentralNormMoments()
{
	if (m_MomOK==false)
	{
		ostringstream ost;
		ost << "CFeatures::DeriveCentralNormMoments() Simple moments not derived" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	bool returnValue=k_DeriveCentralNormMoments(&m_Moments,&m_CentrNormMom);
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CFeatures::DeriveCentralNormMoments() Failed deriving moments" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
		m_CentrNormMomOK=true;
	return returnValue;
}

bool CFeatures::DeriveHuFromCentralMom()
{
	if (m_CentrMomOK==false)
	{
		ostringstream ost;
		ost << "CFeatures::DeriveHuFromCentralMom() Simple moments not derived" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	bool returnValue=k_DeriveHuInvariants(&m_CentrMom,&(m_Hu[0]));
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CFeatures::DeriveHuFromCentralMom() Failed deriving moments" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
		m_HuOK=true;
	return returnValue;
}

bool CFeatures::DeriveHuFromCentralNormMom()
{
	if (m_CentrNormMomOK==false)
	{
		ostringstream ost;
		ost << "CFeatures::DeriveHuFromCentralNormMom() Simple moments not derived" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	bool returnValue=k_DeriveHuInvariants(&m_CentrNormMom,&(m_Hu[0]));
	if (returnValue==false)
	{
		ostringstream ost;
		ost << "CFeatures::DeriveHuFromCentralNormMom() Failed deriving moments" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	else
		m_HuOK=true;
	return returnValue;
}

} // end ipl namespace
