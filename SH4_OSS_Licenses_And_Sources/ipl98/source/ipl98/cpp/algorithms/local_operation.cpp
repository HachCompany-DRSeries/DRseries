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

#include "local_operation.h"
#include "../../kernel_c/algorithms/kernel_local_operation.h"
#include "../../kernel_c/kernel_history.h"
#include "../ipl98_general_functions.h"
#include "../vectors/vector3d.h"
#include "../geometry/plane.h"

namespace ipl{

CLocalOperation::CLocalOperation()
{
}

CLocalOperation::~CLocalOperation()
{
}

bool CLocalOperation::DrawHistogram(CStdImage &Source, CStdImage &Dest)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Source.m_History.PrintfAppendIPL(
		"%sCLocalOperation::DrawHistogram()", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_DrawHistogram(Source.GetTImagePtr(),Dest.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CLocalOperation::DrawHistogram() failed" << IPLAddFileAndLine;
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

bool CLocalOperation::EqualizeHistogram(CStdImage &Source, CStdImage &Dest)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Source.m_History.PrintfAppendIPL(
		"%sCLocalOperation::EqualizeHistogram()", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_EqualizeHistogram(Source.GetTImagePtr(), Dest.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CLocalOperation::EqualizeHistogram() failed" << IPLAddFileAndLine;
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

bool CLocalOperation::NonlinearCopyToByte(CStdImage &Source, float gamma,
	int Min,int Max,CStdImage &Dest)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Source.m_History.PrintfAppendIPL(
		"%sCLocalOperation::NonlinearCopyToByte() CStdImage as source "
		"gamma=%f, Min=%d, Max=%d",
		ipl_HistoryIndent, gamma, Min, Max);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_NonlinearCopyByteToByte(Source.GetTImagePtr(),gamma,Min,Max,Dest.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CLocalOperation::NonlinearCopyToByte() failed" << IPLAddFileAndLine;
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

bool CLocalOperation::NonlinearCopyToByte(CIntImage &Source, float gamma,
	int Min,int Max,CStdImage &Dest)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Source.m_History.PrintfAppendIPL(
		"%sCLocalOperation::NonlinearCopyToByte() CIntImage as source "
		"gamma=%f, Min=%d, Max=%d",
		ipl_HistoryIndent, gamma, Min, Max);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_NonlinearCopyIntToByte(Source.GetTIntImagePtr(),gamma,Min,Max,Dest.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CLocalOperation::NonlinearCopyToByte() failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
	/* remove the history line added to source above */
	Source.m_History.RemoveLastLine();
#endif
	return ReturnValue;
}

bool CLocalOperation::NonlinearCopyToByte(CFloatImage &Source,
	float gamma, float Min, float Max,CStdImage &Dest)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Source.m_History.PrintfAppendIPL(
		"%sCLocalOperation::NonlinearCopyToByte() CFloatImage as source "
		"gamma=%f, Min=%d, Max=%d",
		ipl_HistoryIndent, gamma, Min, Max);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_NonlinearCopyFloatToByte(Source.GetTFloatImagePtr(),gamma,Min,Max,Dest.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CLocalOperation::NonlinearCopyToByte() failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
	/* remove the history line added to source above */
	Source.m_History.RemoveLastLine();
#endif
	return ReturnValue;
}

bool CLocalOperation::NonlinearCopyToByte(CComplexImage &Source,COMPLEXCHOISE mode,
	float gamma, float Min,float Max,CStdImage &Dest)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Source.m_History.PrintfAppendIPL(
		"%sCLocalOperation::NonlinearCopyToByte() CComplexImage as source "
		"gamma=%f, Min=%d, Max=%d",
		ipl_HistoryIndent, gamma, Min, Max);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_NonlinearCopyComplexToByte(Source.GetTComplexImagePtr(),mode,gamma,Min,Max,Dest.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CLocalOperation::NonlinearCopyToByte() failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
	/* remove the history line added to source above */
	Source.m_History.RemoveLastLine();
#endif
	return ReturnValue;
}

bool CLocalOperation::RGBConeTransformation(const CImage& Source, CImage& Dest, 
				UINT32 Color, float LowerLimit, float UpperLimit, float Angle)
{
	float Max=GetRGBConeUpperLimit(Color);
	if (UpperLimit>Max)
	{
		ostringstream ost;
		ost << "CLocalOperation::RGBConeTransformation() UpperLimit outside RGB-cube, "
			"changing it to edge of cube" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_WARNING,ost);
		UpperLimit=Max;
	}
	if (LowerLimit<=0)
	{
		ostringstream ost;
		ost << "CLocalOperation::RGBConeTransformation LowerLimit<=0" 
			<< IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	if ((Angle<0) || (Angle>ipl::PI))
	{
		ostringstream ost;
		ost << "CLocalOperation::RGBConeTransformation() Angle out of range [0;PI] (current is " << 
			Angle << " rad)" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	if (Source.GetBits()!=24)
	{
		ostringstream ost;
		ost << "CLocalOperation::RGBConeTransformation() Input image must be 24 b/p (current is " << 
			Source.GetBits() << ")" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	Dest.Alloc(Source.GetWidth(),Source.GetHeight(),8,0);
	// to save a sqrt calculation, we compare the squared lengths
	LowerLimit=LowerLimit*LowerLimit; 
	UpperLimit=UpperLimit*UpperLimit;
	UINT32 Col;
	int y;
	float ModulusSq;
	CVector3D<float> Vs(CPalette::GetRedVal(Color),	CPalette::GetGreenVal(Color),
						CPalette::GetBlueVal(Color));
	for(y=Source.GetMinY(); y<Source.GetMaxY(); y++)
	{
		CImage::ConstRowIterator24bp It=Source.ConstRow24bp(y);
		CImage::RowIterator8bp It2=Dest.Row8bp(y);
		while(It!=It.End())
		{
			Col=*It;
			CVector3D<float> V(CPalette::GetRedVal(Col),CPalette::GetGreenVal(Col),CPalette::GetBlueVal(Col));
			ModulusSq=V.GetX()*V.GetX()+V.GetY()*V.GetY()+V.GetZ()*V.GetZ();
			if ((ModulusSq>=LowerLimit) && (ModulusSq<=UpperLimit) && (Vs.GetAngle(V)<Angle))
				*It2=255;
			++It;
			++It2;
		}
	}
	return true;
}

float CLocalOperation::GetRGBConeUpperLimit(UINT32 Color)
{
	UINT8 r=CPalette::GetRedVal(Color);
	UINT8 g=CPalette::GetGreenVal(Color);
	UINT8 b=CPalette::GetBlueVal(Color);
	if ((r==0) && (g==0) && (b==0))
	{
		ostringstream ost;
		ost << "CSegmentate::GetRGBConeUpperLimit() Input color is (0,0,0)" 
			<< IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return 0;
	}
	UINT8 Max = ipl::Max(r,ipl::Max(g,b));
	CPlane<float> Plane;
	CVector3D<float> Dir(r,g,b);
	CPoint3D<float> P;
	if (r==Max)
		Plane = CPlane<float>(CPoint3D<float>(255,0,0), CVector3D<float>(0,1,0),CVector3D<float>(0,0,1));
	else if (g==Max)
		Plane = CPlane<float>(CPoint3D<float>(0,255,0), CVector3D<float>(1,0,0),CVector3D<float>(0,0,1));
	else if (b==Max)
		Plane = CPlane<float>(CPoint3D<float>(0,0,255), CVector3D<float>(1,0,0),CVector3D<float>(0,1,0));
	else
	{
		ostringstream ost;
		ost << "CLocalOperation::GetRGBConeUpperLimit() No Max(r,g,b) found - check code" 
			<< IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
	}
	CLine3D<float> L(CPoint3D<float>(0,0,0),Dir);
	Plane.Intersect(L,P);
	return (float)P.GetDist();
}

} // end ipl namespace
