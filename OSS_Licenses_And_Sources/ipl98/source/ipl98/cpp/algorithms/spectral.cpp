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

#include "spectral.h"
#include "../../kernel_c/kernel_history.h"

namespace ipl{

CSpectral::CSpectral()
{
}

CSpectral::~CSpectral()
{
}

bool CSpectral::FFTForward(CComplexImage &Img)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	{
		Img.m_History.PrintfAppendIPL(
			"%sCSpectral::FFTForward()", ipl_HistoryIndent);

		k_IplStartHistoryMacro();
	}
#endif
	bool ReturnValue=k_FFT(1,Img.GetTComplexImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CSpectral::FFTForward() failed: Empty image or width/height not a power of 2" << IPLAddFileAndLine;
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

bool CSpectral::FFTInverse(CComplexImage &Img)
{
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	{
		Img.m_History.PrintfAppendIPL(
			"%CSpectral::FFTInverse()", ipl_HistoryIndent);

		k_IplStartHistoryMacro();
	}
#endif
	bool ReturnValue=k_FFT(-1,Img.GetTComplexImagePtr());
	if (ReturnValue==false)
	{
		ostringstream ost;
		ost << "CSpectral::FFTInverse() failed: Empty image or width/height not a power of 2" << IPLAddFileAndLine;
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

bool CSpectral::DeriveCorrelation(const CStdImage &Lg,const CStdImage &Sm, CFloatImage &Dest)
{
	if ((Lg.IsEmpty()==true) || (Sm.IsEmpty()==true))
	{
		ostringstream ost;
		ost << "CSpectral::DeriveCorrelation() failed: One of the input images is empty" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
	CComplexImage CLg, CSm, CBuf;
	int W=1;
	int H=1;
	int x,y;
	int wL=Lg.GetWidth();
	int hL=Lg.GetHeight();
	Dest.Alloc(wL,hL);
	while (W<=wL)
		W*=2;
	while (H<=hL)
		H*=2;
	int wS=Sm.GetWidth();
	int hS=Sm.GetHeight();
	CLg.Alloc(W,H);
	CSm.Alloc(W,H);
	float sum;
	sum=0;
	for (x=0;x<wL;x++)
		for (y=0;y<hL;y++)
			sum+=Lg.GetPixelFast(x,y);

	sum/=wL*hL;
	CLg.Flush(sum,0);
	for (x=0;x<wL;x++)
		for (y=0;y<hL;y++)
			CLg.SetPixelFastRe(x,y,(FLOAT32)Lg.GetPixelFast(x,y));
		
	sum=0;
	for (x=0;x<wS;x++)
		for (y=0;y<hS;y++)
			sum+=Sm.GetPixelFast(x,y);
	
	sum/=wS*hS;
	CSm.Flush(sum,0);
	
	for (x=0;x<wS;x++)
		for (y=0;y<hS;y++)		
			CSm.SetPixelFastRe(x,y,(FLOAT32)Sm.GetPixelFast(x,y));
				
	FFTForward(CLg);
	FFTForward(CSm);
	CLg.CCFactorAndMultiply(CSm);
	FFTInverse(CLg);
	for (x=0;x<wL;x++)
	{
		for (y=0;y<hL;y++)
		{
			Dest.SetPixelFast(x,y,CLg.GetPixelFastRe(x,y));
		}
	}
	return true;
}

} // end ipl namespace
