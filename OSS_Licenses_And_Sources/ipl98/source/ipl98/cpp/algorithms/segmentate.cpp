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

#include "segmentate.h"
#include "../../kernel_c/algorithms/kernel_segmentate.h"

namespace ipl{

CSegmentate::CSegmentate()
{
}

CSegmentate::~CSegmentate()
{
}

bool CSegmentate::DeriveBlobs(CStdImage& Source, CPixelGroups& PixelGroups,
					COLORTYPE Color, UINT8 Threshold, CONNECTIVITY Connected, unsigned int MinSize)
{
	PixelGroups.Empty();
	if (k_DeriveBlobs(Source.GetTImagePtr(),PixelGroups.GetTPixelGroupsPtr(),
                    Color,Threshold,Connected,MinSize)==false)
	{
		ostringstream ost;
		ost << "CSegmentate::DeriveBlobs() Failed deriving blobs" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	PixelGroups.Update();
/*	string str("CSegmentate::DeriveBlobs() with:");
	ostringstream ost;
	ost << " Color=" << Color << " Threshold=" << Threshold << " Connected=" << Connected;
	str.append(ost.str());
	Source.m_History.AppendIPL(str);*/
	return true;
}

bool CSegmentate::BlobExpansion(const CPoint2D<INT16>& Seed, CImage& Source, 
								CPixelGroup& PixelGroup,UINT8 Threshold, CONNECTIVITY Connected)
{
	T2DInt P;
	P.x=Seed.GetX();
	P.y=Seed.GetY();
	if (k_BlobExpansion(P,Source.GetTImagePtr(),PixelGroup.GetTPixelGroupPtr(),Threshold, Connected)==false)
	{
		ostringstream ost;
		ost << "CSegmentate::BlobExpansion() Failed expanding blob" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	return true;
}

bool CSegmentate::FindAndFollowLowContour(CStdImage& Source, const CPoint2D<int>& Start,
							   DIRECTION SearchDirection, CONNECTIVITY Connected,
							   CPixelGroup& PixelGroup,ROTATION& RotationDirection,
								COLORTYPE& SurroundedColor)
{
	T2DInt p;
	p.x=Start.GetX();
	p.y=Start.GetY();
	if (k_FindAndFollowLowContour(Source.GetTImagePtr(),p,SearchDirection,
			Connected,PixelGroup.GetTPixelGroupPtr(),&RotationDirection,&SurroundedColor)==false)
	{
		ostringstream ost;
		ost << "CSegmentate::FindAndFollowLowContour() Failed finding contour" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	return true;
}

bool CSegmentate::FindAndFollowHighContour(CStdImage& Source, const CPoint2D<int>& Start,
								DIRECTION SearchDirection, CONNECTIVITY Connected,
								CPixelGroup& PixelGroup,ROTATION& RotationDirection,
								COLORTYPE& SurroundedColor)
{
	T2DInt p;
	p.x=Start.GetX();
	p.y=Start.GetY();
	if (k_FindAndFollowHighContour(Source.GetTImagePtr(),p,SearchDirection,
			Connected,PixelGroup.GetTPixelGroupPtr(),&RotationDirection,&SurroundedColor)==false)
	{
		ostringstream ost;
		ost << "CSegmentate::FindAndFollowHighContour() Failed finding contour" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	return true;
}

bool CSegmentate::FindAndFollowLowContourWithThreshold(CStdImage& Source, const CPoint2D<int>& Start,int Threshold,
								DIRECTION SearchDirection, CONNECTIVITY Connected,
								CPixelGroup& PixelGroup,ROTATION& RotationDirection,
								COLORTYPE& SurroundedColor)
{
	T2DInt p;
	p.x=Start.GetX();
	p.y=Start.GetY();
	if (k_FindAndFollowLowContourWithThreshold(Source.GetTImagePtr(),p,Threshold,SearchDirection,
			Connected,PixelGroup.GetTPixelGroupPtr(),&RotationDirection,&SurroundedColor)==false)
	{
		ostringstream ost;
		ost << "CSegmentate::FindAndFollowLowContourWithThreshold() Failed finding contour" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	return true;
}

bool CSegmentate::FindAndFollowHighContourWithThreshold(CStdImage& Source, const CPoint2D<int>& Start,int Threshold,
								DIRECTION SearchDirection, CONNECTIVITY Connected,
								CPixelGroup& PixelGroup,ROTATION& RotationDirection,
								COLORTYPE& SurroundedColor)
{
	T2DInt p;
	p.x=Start.GetX();
	p.y=Start.GetY();
	if (k_FindAndFollowHighContourWithThreshold(Source.GetTImagePtr(),p,Threshold,SearchDirection,
			Connected,PixelGroup.GetTPixelGroupPtr(),&RotationDirection,&SurroundedColor)==false)
	{
		ostringstream ost;
		ost << "CSegmentate::FindAndFollowHighContourWithThreshold() Failed finding contour" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	return true;
}

} // end ipl namespace
