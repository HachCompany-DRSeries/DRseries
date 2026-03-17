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

#include "coordinate_transform.h"
#include "../../kernel_c/algorithms/kernel_coordinate_transform.h"
#include "../../kernel_c/kernel_history.h"
#include "../byte_image.h"

namespace ipl{

CCoordinateTransform::CCoordinateTransform()
{
}


CCoordinateTransform::~CCoordinateTransform()
{
}

bool CCoordinateTransform::ScaleAndRotateAuto(CStdImage& Source,CStdImage& Dest,
											  double Sx, double Sy, double phi,UINT32 Background)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Source.m_History.PrintfAppendIPL(
		"%sCCoordinateTransform::ScaleAndRotateAuto"
		"(Sx=%g,Sy=%g,phi=%g,Background=%d)",
		ipl_HistoryIndent, Sx, Sy, phi, Background);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=k_ScaleAndRotateAuto(Source.GetTImagePtr(),Dest.GetTImagePtr(),Sx,Sy,phi,Background);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CCoordinateTransform::ScaleAndRotateAuto() Failed scaling/rotating" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
	/* remove the history line added to source above */
	if (&Source!=&Dest)
		Source.m_History.RemoveLastLine();
#endif
	return ReturnValue;
}

bool CCoordinateTransform::ScaleAndRotateFixed(CStdImage& Source,CStdImage& Dest,double x0,double y0,
						   int Width,int Height,double Sx, double Sy, double phi,UINT32 Background)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Source.m_History.PrintfAppendIPL(
		"%sCCoordinateTransform::ScaleAndRotateFixed"
		"(x0=%g,y0=%g,Width=%d,Height=%d,Sx=%g,Sy=%g,phi=%g,Background=%d)",
		ipl_HistoryIndent, x0, y0, Width, Height,
		Sx, Sy, phi, Background);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=k_ScaleAndRotateFixed(Source.GetTImagePtr(),Dest.GetTImagePtr(),x0,y0,
						   Width,Height,Sx,Sy,phi,Background);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CCoordinateTransform::ScaleAndRotateFixed() Failed scaling/rotating" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
	/* remove the history line added to source above */
	if (&Source!=&Dest)
		Source.m_History.RemoveLastLine();
#endif
	return ReturnValue;
}

bool CCoordinateTransform::ScaleAuto(CStdImage& Source,CStdImage& Dest,
									 double Sx, double Sy, UINT32 Background)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Source.m_History.PrintfAppendIPL(
		"%sCCoordinateTransform::ScaleAuto(Sx=%g,Sy=%g,Background=%d)",
		ipl_HistoryIndent, Sx, Sy, Background);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=k_ScaleAuto(Source.GetTImagePtr(),Dest.GetTImagePtr(),Sx,Sy,Background);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CCoordinateTransform::ScaleAuto() Failed scaling" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
	/* remove the history line added to source above */
	if (&Source!=&Dest)
		Source.m_History.RemoveLastLine();
#endif
	return ReturnValue;
}

bool CCoordinateTransform::ScaleFixed(CStdImage& Source,CStdImage& Dest,double x0,double y0,
				  int Width,int Height,double Sx, double Sy,UINT32 Background)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Source.m_History.PrintfAppendIPL(
		"%sCCoordinateTransform::ScaleFixed"
		"(x0=%g,y0=%g,Width=%d,Height=%d,Sx=%g,Sy=%g,Background=%d)",
		ipl_HistoryIndent, x0, y0, Width, Height, Sx, Sy, Background);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=k_ScaleFixed(Source.GetTImagePtr(),Dest.GetTImagePtr(),x0,y0,Width,Height,Sx,Sy,Background);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CCoordinateTransform::ScaleFixed() Failed scaling" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
	/* remove the history line added to source above */
	if (&Source!=&Dest)
		Source.m_History.RemoveLastLine();
#endif
	return ReturnValue;
}


} // end ipl namespace
