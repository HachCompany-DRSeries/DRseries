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

#include "morphology.h"
#include "../../kernel_c/algorithms/kernel_morphology.h"
#include "../../kernel_c/kernel_history.h"

namespace ipl{

CMorphology::CMorphology()
{
}


CMorphology::~CMorphology()
{
}

bool CMorphology::DilateFast(CStdImage& Img,unsigned short q10,unsigned short q20,unsigned short q30)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMorphology::DilateFast(q10=%d,q20=%d,q30=%d)",
		ipl_HistoryIndent, q10, q20, q30);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=k_DilateFast(Img.GetTImagePtr(),q10,q20,q30);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMorphology::DilateFast() Fast dilation failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CMorphology::ErodeFast(CStdImage& Img,unsigned short q10,unsigned short q20,unsigned short q30)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMorphology::ErodeFast(q10=%d,q20=%d,q30=%d)",
		ipl_HistoryIndent, q10, q20, q30);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=k_ErodeFast(Img.GetTImagePtr(),q10,q20,q30);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMorphology::ErodeFast() Fast erotion failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CMorphology::Erode(const CStdImage& ImgMask, CStdImage& Img)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMorphology::Erode() with dimensions (%d*%d)",
		ipl_HistoryIndent, ImgMask.GetWidth(), ImgMask.GetHeight());

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=k_Erode(ImgMask.GetConstTImagePtr(),Img.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMorphology::Erode() Erotion failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CMorphology::Dilate(const CStdImage& ImgMask, CStdImage& Img)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMorphology::Dilate() with dimensions (%d*%d)",
		ipl_HistoryIndent, ImgMask.GetWidth(), ImgMask.GetHeight());

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	bool ReturnValue=k_Dilate(ImgMask.GetConstTImagePtr(),Img.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMorphology::Dilate() Dilation failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

int CMorphology::Thinning(CStdImage& Img)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMorphology::Thinning())", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	int ReturnValue=k_Thinning(Img.GetTImagePtr());
	if (ReturnValue==-1)
	{
		ostringstream ost;
		ost << "CMorphology::Thinning() Thinning failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

int CMorphology::Skeletonizing(CStdImage& Img)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMorphology::Skeletonizing())",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	int ReturnValue=k_Skeletonizing(Img.GetTImagePtr());
	if (ReturnValue==-1)
	{
		ostringstream ost;
		ost << "CMorphology::Skeletonizing() Skeletonizing failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

int CMorphology::SkeletonZhou(CStdImage& Img, bool PreserveCorners)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMorphology::SkeletonZhou())",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	/* call kernel function */
	int ReturnValue=k_SkeletonZhou(Img.GetTImagePtr(),PreserveCorners);
	if (ReturnValue==-1)
	{
		ostringstream ost;
		ost << "CMorphology::SkeletonZhou() Skeletonizing with Zhou algotithm failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}


} // end ipl namespace
