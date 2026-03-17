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

#include "mask_operation.h"
#include "../../kernel_c/algorithms/kernel_mask_operation.h"
#include "../../kernel_c/kernel_history.h"

namespace ipl{

CMaskOperation::CMaskOperation()
{
}

CMaskOperation::~CMaskOperation()
{
}

bool CMaskOperation::Convolve(CStdImage &Img,const CFloatImage &Mask)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMaskOperation::Convolve() source/dest: CStdImage, with a %d*%d mask",
		ipl_HistoryIndent, Mask.GetWidth(), Mask.GetHeight());

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_ConvolveByteToByte(Img.GetTImagePtr(),Mask.GetConstTFloatImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMaskOperation::Convolve() failed" << IPLAddFileAndLine;
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

bool CMaskOperation::Convolve(CStdImage &Source,const CFloatImage &Mask,CIntImage &Dest)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Source.m_History.PrintfAppendIPL(
		"%sCMaskOperation::Convolve() source: CStdImage, dest:CIntImage, "
		"with a %d*%d mask",
		ipl_HistoryIndent, Mask.GetWidth(), Mask.GetHeight());

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_ConvolveByteToInt(Source.GetTImagePtr(),
				Mask.GetConstTFloatImagePtr(),Dest.GetTIntImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMaskOperation::Convolve() failed" << IPLAddFileAndLine;
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

bool CMaskOperation::Convolve(CIntImage &Img,const CFloatImage &Mask)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMaskOperation::Convolve() source/dest: CIntImage, with a %d*%d mask",
		ipl_HistoryIndent, Mask.GetWidth(), Mask.GetHeight());

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_ConvolveIntToInt(Img.GetTIntImagePtr(),Mask.GetConstTFloatImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMaskOperation::Convolve() failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added to source above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CMaskOperation::Convolve(CFloatImage &Img,const CFloatImage &Mask)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMaskOperation::Convolve() source/dest: CFloatImage, with a %d*%d mask",
		ipl_HistoryIndent, Mask.GetWidth(), Mask.GetHeight());

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_ConvolveFloatToFloat(Img.GetTFloatImagePtr(),Mask.GetConstTFloatImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMaskOperation::Convolve() failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added to source above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CMaskOperation::BlurFourConnected3x3(CStdImage &Img)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMaskOperation::BlurFourConnected3x3()", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_BlurFourConnected3x3(Img.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMaskOperation::BlurFourConnected3x3() failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added to source above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CMaskOperation::BlurEightConnected3x3(CStdImage &Img)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMaskOperation::BlurEightConnected3x3()", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_BlurEightConnected3x3(Img.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMaskOperation::BlurEightConnected3x3() failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added to source above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CMaskOperation::BlurEightConnected3x3(CIntImage &Img)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMaskOperation::BlurEightConnected3x3()", ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_BlurEightConnected3x3Int(Img.GetTIntImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMaskOperation::BlurEightConnected3x3() failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added to source above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CMaskOperation::ConvolveFast(CStdImage &Img,const CFloatImage &Mask,float Prefactor)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMaskOperation::ConvolveFast() source/dest: CStdImage, prefactor=%g",
		ipl_HistoryIndent, Prefactor);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_ConvolveFastByteToByte(Img.GetTImagePtr(),
		Mask.GetConstTFloatImagePtr(),Prefactor);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMaskOperation::ConvolveFast() failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added to source above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CMaskOperation::ConvolveFast(CStdImage &Source,const CFloatImage &Mask,
	float Prefactor, CIntImage &Dest)
{ 
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Source.m_History.PrintfAppendIPL(
		"%sCMaskOperation::ConvolveFast() source: CStdImage, "
		"dest:CIntImage, prefactor=%g",
		ipl_HistoryIndent, Prefactor);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_ConvolveFastByteToInt(Source.GetTImagePtr(),
		Mask.GetConstTFloatImagePtr(),Prefactor, Dest.GetTIntImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMaskOperation::ConvolveFast() failed" << IPLAddFileAndLine;
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

bool CMaskOperation::ConvolveFast(CIntImage &Img,const CFloatImage &Mask,
	float Prefactor)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMaskOperation::ConvolveFast() source/dest: CIntImage, prefactor=%g",
		ipl_HistoryIndent, Prefactor);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_ConvolveFastIntToInt(Img.GetTIntImagePtr(),
			Mask.GetConstTFloatImagePtr(),Prefactor);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMaskOperation::ConvolveFast() failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added to source above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CMaskOperation::MaxGradientFast(CStdImage &Img,float Prefactor)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMaskOperation::MaxGradientFast() source/dest: CStdImage prefactor=%g",
		ipl_HistoryIndent, Prefactor);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_MaxGradientFastByteToByte(Img.GetTImagePtr(),Prefactor);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMaskOperation::MaxGradientFast() failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added to source above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CMaskOperation::MaxGradientFast(CStdImage &Source,float Prefactor,CIntImage &Dest)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Source.m_History.PrintfAppendIPL(
		"%sCMaskOperation::MaxGradientFast() source: CStdImage, dest:CIntImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_MaxGradientFastByteToInt(Source.GetTImagePtr(),Prefactor,
		Dest.GetTIntImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMaskOperation::MaxGradientFast() failed" << IPLAddFileAndLine;
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

bool CMaskOperation::MaxGradientFast(CIntImage &Img,float Prefactor)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMaskOperation::MaxGradientFast() source/dest: CIntImage prefactor=%g",
		ipl_HistoryIndent, Prefactor);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_MaxGradientFastIntToInt(Img.GetTIntImagePtr(),Prefactor);
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMaskOperation::MaxGradientFast() failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added to source above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CMaskOperation::MedianFilterFourConnected3x3(CStdImage &Img)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMaskOperation::MedianFilterFourConnected3x3() source/dest: CStdImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_MedianFilterFourConnected3x3(Img.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMaskOperation::MedianFilterFourConnected3x3() failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added to source above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CMaskOperation::MedianFilterDiagonalConnected3x3(CStdImage &Img)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMaskOperation::MedianFilterDiagonalConnected3x3() "
		"source/dest: CStdImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_MedianFilterDiagonalConnected3x3(Img.GetTImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMaskOperation::MedianFilterDiagonalConnected3x3() failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added to source above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CMaskOperation::MedianFilterFourConnected3x3(CIntImage &Img)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMaskOperation::MedianFilterFourConnected3x3() "
		"source/dest: CIntImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_MedianFilterFourConnected3x3Int(Img.GetTIntImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMaskOperation::MedianFilterFourConnected3x3() failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added to source above */
		Img.m_History.RemoveLastLine();
#endif
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return ReturnValue;
}

bool CMaskOperation::MedianFilterDiagonalConnected3x3(CIntImage &Img)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	Img.m_History.PrintfAppendIPL(
		"%sCMaskOperation::MedianFilterDiagonalConnected3x3() "
		"source/dest: CIntImage",
		ipl_HistoryIndent);

	k_IplStartHistoryMacro();
#endif
	bool ReturnValue=k_MedianFilterDiagonalConnected3x3Int(Img.GetTIntImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CMaskOperation::MedianFilterDiagonalConnected3x3() failed" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
#ifdef IPL_ADD_HISTORY
		/* remove the history line added to source above */
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
