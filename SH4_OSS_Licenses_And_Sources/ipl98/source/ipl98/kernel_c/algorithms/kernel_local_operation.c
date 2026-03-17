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

#include "kernel_local_operation.h"
#include "kernel_statistic.h"
#include "../kernel_error.h"
#include "../kernel_history.h"
#include <math.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

bool k_DrawHistogram(TImage *pSource, TImage *pDest)
{
	if ((pSource->Origin==EMPTY) || (pSource==pDest) || (pSource->Bits==1))
	{
		char* msg;

		if (pSource->Origin==EMPTY)
			msg = "k_DrawHistogram() Source image is empty";
		else if (pSource==pDest)
			msg = "k_DrawHistogram() Source and destination image cannot be the same";
		else
			msg = "k_DrawHistogram() Source has 1 b/p";

		k_ShowMessageSimple(IPL_ERROR, msg);
		return false;
	}
	else
	{
		float buf;
		unsigned int x,y;
		TStatistic Stat;
		k_InitStatistic(&Stat);
		k_EmptyImage(pDest);
		k_AllocImage(277,320,1,pDest);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%sk_DrawHistogram() Drawn from image with file info: %s",
				ipl_HistoryIndent,
				pSource->FileInfo.PathAndFileName);
			k_IplStartHistoryMacro();
		}
#endif
		k_FlushImage(1,pDest);
		for (x=10;x<267;x++)
		{
			k_SetPixel1bp(x,10,0,*pDest);
			k_SetPixel1bp(x,310,0,*pDest);
		}
		for (y=10;y<310;y++)
		{
			k_SetPixel1bp(10,y,0,*pDest);
			k_SetPixel1bp(267,y,0,*pDest);
		}
		k_DeriveImageStats(&Stat,pSource);
		buf=0;
		for (x=0;x<Stat.SizeOfHistogram;x++)
		{
			if ((float)Stat.pHistogram[x]>buf)
				buf=(float)Stat.pHistogram[x];
		}
		for (x=0;x<Stat.SizeOfHistogram;x++)
		{
			for (y=0;y<(unsigned int)(300.0*((float)Stat.pHistogram[x])/buf);y++)
			{
				k_SetPixel1bp(x+11,310-y,0,*pDest);
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		k_EmptyStatistic(&Stat);
		return true;
	}
}

bool k_EqualizeHistogram(TImage *pSource,TImage *pDest)
{
	if ((pSource->Origin==EMPTY) || (pSource->Bits<8))
	{
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_EqualizeHistogram() Source image is empty "
			"or image is not 8 or 24 b/p");
		return false;
	}
	else
	{
		TImage* pTarget=pDest;
		bool CopyToSource=false;
		unsigned long int Histogram[256];
		unsigned short LUT[256];
		unsigned long int sum;
		int i,x,y,w,h;
		UINT32 PalValue;
		float buf;
		/* Check if the source and destination is the same image */
		if (pDest==pSource)
		{
			pTarget=(TImage*) malloc(sizeof(TImage));
			k_InitImage(pTarget);
			CopyToSource=true;
		}
		for (i=0;i<256;i++)
			Histogram[i]=0;
		w=pSource->Width;
		h=pSource->Height;
		if (pSource->Bits==8)
		{
			for (y=0;y<h;y++)
			{
				for (x=0;x<w;x++)
				{
					Histogram[k_GetPixel8bp(x,y,*pSource)]++;
				}
			}
		}
		else
		{
			for (y=0;y<h;y++)
			{
				for (x=0;x<w;x++)
				{
					PalValue=k_GetPixel24bp(x,y,*pSource);
					Histogram[(UINT8)((int)((k_PalGetRVal(PalValue)+
						k_PalGetGVal(PalValue)+k_PalGetBVal(PalValue))/3+0.5))]++;
				}
			}
		}
		k_EmptyImage(pTarget);
		sum=Histogram[0];
		LUT[0]=0;
		for (i=1;i<255;i++)
		{
			sum+=Histogram[i];
			LUT[i]=(int)(255.0*sum/w/h+0.5);
		}
		LUT[255]=255;
		if (pSource->Bits==8)
		{
			k_AllocImage(w,h,8,pTarget);
			for (y=0;y<h;y++)
				for (x=0;x<w;x++)
					k_SetPixel8bp(x,y,LUT[k_GetPixel8bp(x,y,*pSource)],*pTarget);
		}
		else
		{
			int r,g,b;
			k_AllocImage(w,h,24,pTarget);
			for (y=0;y<h;y++)
			{
				for (x=0;x<w;x++)
				{
					PalValue=k_GetPixel24bp(x,y,*pSource);
					i=((int)((k_PalGetRVal(PalValue)+
						k_PalGetGVal(PalValue)+k_PalGetBVal(PalValue))/3+0.5));
					if (i==0)
						buf=0;
					else
						buf=(float)(LUT[i])/(float)(i);

					r=(int)(buf*k_PalGetRVal(PalValue));
					if (r>255)
						r=255;
					g=(int) (buf*k_PalGetGVal(PalValue));
					if (g>255)
						g=255;
					b=(int)(buf*k_PalGetBVal(PalValue));
					if (b>255)
						b=255;
					k_SetPixel24bp(x,y,k_PalCreateRGB(r,g,b),*pTarget);
				}
			}
		}
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pTarget->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pTarget->History),
				"%sk_EqualizeHistogram()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		if (CopyToSource==true)
		{
			k_CopyImage(pDest,pTarget);
			k_EmptyImage(pTarget);
			free(pTarget);
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool k_NonlinearCopyByteToByte(TImage *pSource,float gamma,
							   int Min,int Max, TImage *pDest)
{
	if (pSource->Bits!=8)
	{
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_NonLinearByteToByte(). Source image does not have 8 b/p");
		return false;
	}
	else if ((pSource->Origin==EMPTY) || (Max<=Min) || (gamma<0))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_NonLinearCopyByteToByte(). Source image is empty or wrong"
			" input data: gamma= %f, Min = %d, Max = %d",
			gamma, Min, Max);
		return false;
	}
	else
	{
		unsigned int x,y,w,h;
		double p,q,den,logden;
		den=Max-Min;
		logden=log(den+1);
		h=pSource->Height;
		w=pSource->Width;
		if ((pDest->Width!=w) || (pDest->Height!=h) || (pDest->Bits!=8))
		{
			k_AllocImage(w,h,8,pDest);
		}
#ifdef IPL_ADD_HISTORY
		if (pDest!=pSource)
		{
			/* make sure to delete history in pDest! */
			k_EmptyText(&(pDest->History));
			/* copy history before adding comment */
			k_CopyText(&(pDest->History),&(pSource->History));
		}
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%sk_NonLinearByteToByte() gamma=%f, Min=%d, Max=%d",
				ipl_HistoryIndent, gamma, Min, Max);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<h;y++)
		{
			for (x=0;x<w;x++)
			{
				q=(float)pSource->ppMatrix[y][x];
				if (q<=Min)
					p=0;
				else
				{
					if (gamma==(float)1)
						p=(q-Min)/den;
					else if (gamma==(float)0)
						p=log(q-Min+1)/logden;
					else
						p=pow((double)(q-Min)/den,(double)gamma);
					if (p>1)
						p=1;
				}
				pDest->ppMatrix[y][x]=(UINT8)(p*255.0);
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}


bool k_NonlinearCopyIntToByte(TIntImage *pSource,float gamma,
							  int Min,int Max, TImage *pDest)
{
	if ((pSource->Origin==EMPTY) || (Max<=Min) || (gamma<0))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_NonLinearCopyIntToByte(). Source image is empty or wrong"
			" input data: gamma= %f, Min = %d, Max = %d",
			gamma, Min, Max);
		return false;
	}
	else
	{
		unsigned int x,y,w,h;
		double p,q,den,logden;
		den=Max-Min;
		logden=log(den+1);
		h=pSource->Height;
		w=pSource->Width;
		if ((pDest->Width!=w) || (pDest->Height!=h) || (pDest->Bits!=8))
		{
			k_AllocImage(w,h,8,pDest);
		}
#ifdef IPL_ADD_HISTORY
		/* empty old history from destination in all cases */
		k_EmptyText(&(pDest->History));
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%sk_NonLinearIntToByte() gamma=%f, Min=%d, Max=%d",
				ipl_HistoryIndent, gamma, Min, Max);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<h;y++)
		{
			for (x=0;x<w;x++)
			{
				q=(float)pSource->ppMatrix[y][x];
				if (q<=Min)
					p=0;
				else
				{
					if (gamma==(float)1)
						p=(q-Min)/den;
					else if (gamma==(float)0)
						p=log(q-Min+1)/logden;
					else
						p=pow((double)(q-Min)/den,(double)gamma);
					if (p>1)
						p=1;
				}
				pDest->ppMatrix[y][x]=(UINT8)(p*255.0);
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool k_NonlinearCopyFloatToByte(TFloatImage* pSource ,float gamma,
								float Min, float Max, TImage *pDest)
{
	if ((pSource->Origin==EMPTY) && (Max<=Min) && (gamma<0))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_NonLinearCopyFloatToByte(). Source image is empty or wrong"
			" input data: gamma= %f, Min = %f, Max = %f",
			gamma, Min, Max);
		return false;
	}
	else
	{
		unsigned int w,h,x,y;
		double p,q,den,logden;
		den=Max-Min;
		logden=log(den+1);
		h=pSource->Height;
		w=pSource->Width;
		if ((pDest->Width!=w) || (pDest->Height!=h) || (pDest->Bits!=8))
		{
			k_AllocImage(w,h,8,pDest);
		}
#ifdef IPL_ADD_HISTORY
		/* empty old history from destination in all cases */
		k_EmptyText(&(pDest->History));
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%sk_NonLinearCopyFloatToByte() gamma=%f, Min=%f, Max=%f",
				ipl_HistoryIndent, gamma, Min, Max);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<h;y++)
		{
			for (x=0;x<w;x++)
			{
				q=pSource->ppMatrix[y][x];
				if (q<=Min)
					p=0;
				else
				{
					if (gamma==(float)1)
						p=(q-Min)/den;
					else if (gamma==(float)0)
						p=log(q-Min+1)/logden;
					else
						p=pow((double)(q-Min)/den,(double)gamma);
					if (p>1)
						p=1;
				}
				pDest->ppMatrix[y][x]=(UINT8)(p*255.0);
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool k_NonlinearCopyComplexToByte(TComplexImage *pSource,COMPLEXCHOISE mode,FLOAT32 gamma,
								  float Min, float Max, TImage *pDest)
{
	if ((pSource->Origin==EMPTY) || (Max<=Min) || (gamma<0))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_NonLinearComplexToByte(). Source image is empty or wrong"
			" input data: gamma= %f, Min = %f, Max = %f",
			gamma, Min, Max);
		return false;
	}
	else
	{
		int x,y;
		unsigned int w,h;
		double p,q,den,logden,re,im;
		den=Max-Min;
		logden=log(den+1);
		h=pSource->Height;
		w=pSource->Width;
		if ((pDest->Width!=w) || (pDest->Height!=h) || (pDest->Bits!=8))
		{
			k_AllocImage(w,h,8,pDest);
		}
#ifdef IPL_ADD_HISTORY
		/* empty old history from destination in all cases */
		k_EmptyText(&(pDest->History));
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%sk_NonLinearCopyComplexToByte() gamma=%f, Min=%f, Max=%f",
				ipl_HistoryIndent, gamma, Min, Max);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=kC_GetMinY(pSource);y<kC_GetMaxY(pSource);y++)
		{
			for (x=kC_GetMinX(pSource);x<kC_GetMaxX(pSource);x++)
			{
				re=pSource->ppMatrixRe[y][x];
				im=pSource->ppMatrixIm[y][x];
				if (mode==COMPLEX_REAL)
					q=re;
				else
					if (mode==COMPLEX_IMAGINARY)
						q=im;
					else
						q=sqrt(re*re+im*im);
					if (q<=Min)
						p=0;
					else /* assumes COMPLEX_MODULUS */
					{
						if (gamma==(float)1)
							p=(q-Min)/den;
						else if (gamma==(float)0)
							p=log(q-Min+1)/logden;
						else
							p=pow((double)(q-Min)/den,(double)gamma);
						if (p>1)
							p=1;
					}
					pDest->ppMatrix[y][x]=(UINT8)(p*255.0);
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
