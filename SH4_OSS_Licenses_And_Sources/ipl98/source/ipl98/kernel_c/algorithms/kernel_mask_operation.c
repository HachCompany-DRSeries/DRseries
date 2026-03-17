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

#include "kernel_mask_operation.h"
#include "../general_functions/ipl_general_functions_c_only.h"
#include "../kernel_error.h"
#include "../kernel_history.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/************************************************/
/********        Public functions        ********/
/************************************************/

bool k_ConvolveByteToByte(TImage *pImg,const TFloatImage *pMask)
{
	int x,y,x1,y1,w,h,W,H;
	W=pImg->Width;
	H=pImg->Height;
	w=pMask->Width;
	h=pMask->Height;
	if ((W<=3) || (H<=3) || (w>W) || (h>H) || (pImg->Bits!=8))
	{
		if ((W<4) || (H<4))
		{
			k_ShowMessageStd(
				IPL_POSITION, IPL_ERROR,
				"k_ConvolveByteToByte() "
				"Image size %d x %d is too small. Must be at least 4x4",
				W, H);
		}
		if ((w>W) || (h>H))
		{
			k_ShowMessageSimple(IPL_ERROR, "k_ConvolveByteToByte() "
				"One or two mask dimensions exceeds the that of the image");
		}
		if (pImg->Bits!=8)
			k_ShowMessageSimple(IPL_ERROR, "k_ConvolveByteToByte() Image must have 8 b/p");

		return false;
	}
	else
	{
		float sum;
		TImage Temp;
		unsigned short BorderSize;
		if (pMask->Width > pMask->Height)
			BorderSize=(short)(pMask->Width+0.5);
		else
			BorderSize=(short)(pMask->Height+0.5);

		k_InitImage(&Temp);
		k_AllocImage(W,H,8,&Temp);
		k_SetPeriodicBorder(BorderSize,pImg);

#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(Temp.History),&(pImg->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(Temp.History),
				"%sk_ConvolveByteToByte() With a %d*%d mask",
				ipl_HistoryIndent, w, h);
			k_IplStartHistoryMacro();
		}
#endif
		for (y1=0;y1<H;y1++)
		{
			for (x1=0;x1<W;x1++)
			{  
				sum=0;
				for (y=0;y<h;y++)
				{
					for (x=0;x<w;x++)
					{
						sum+=pImg->ppMatrix[y1-y][x1-x]*pMask->ppMatrix[y][x];
					}
				}
				Temp.ppMatrix[y1][x1]=(UINT8)sum;
			}
		}
		k_CopyImage(pImg,&Temp);
		k_EmptyImage(&Temp);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	} 
}

bool k_ConvolveByteToInt(TImage *pSource,const TFloatImage *pMask,
						 TIntImage *pDest)
{
	int x,y,x1,y1,w,h,W,H;
	W=pSource->Width;
	H=pSource->Height;
	w=pMask->Width;
	h=pMask->Height;
	if ((W<=3) || (H<=3) || (w>W) || (h>H) || (pSource->Bits!=8))
	{
		if ((W<4) || (H<4)) {
			k_ShowMessageStd(
				IPL_POSITION, IPL_ERROR,
				"k_ConvolveByteToInt() Image size %d x %d is too small. Must be at least 4x4",
				W, H);
		}

        if ((w>W) || (h>H))
			k_ShowMessageSimple(
				IPL_ERROR,
				"k_ConvolveByteToInt() One or two mask dimensions exceeds the that of the image");

        if (pSource->Bits!=8)
			k_ShowMessageSimple(
				IPL_ERROR,
				"k_ConvolveByteToByte() Source image must have 8 b/p");

		return false;
	}
	else
	{
		float sum;
		unsigned short BorderSize;
		if (pMask->Width > pMask->Height)
			BorderSize=(short)(pMask->Width+0.5);
		else
			BorderSize=(short)(pMask->Height+0.5);

		kI_AllocImage(W,H,pDest);
		k_SetPeriodicBorder(BorderSize,pSource);
		
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%sk_ConvolveByteToInt() With a %d*%d mask",
				ipl_HistoryIndent, w, h);
			k_IplStartHistoryMacro();
		}
#endif
		for (y1=0;y1<H;y1++)
		{
			for (x1=0;x1<W;x1++)
			{ 
				sum=0;
				for (y=0;y<h;y++)
					for (x=0;x<w;x++)
					{
						sum+=pSource->ppMatrix[y1-y][x1-x]*pMask->ppMatrix[y][x];
					}
					pDest->ppMatrix[y1][x1]=(INT16)sum;
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	} 
}

bool k_ConvolveIntToInt(TIntImage *pImg,const TFloatImage *pMask)
{
	int x,y,x1,y1,w,h,W,H;
	W=pImg->Width;
	H=pImg->Height;
	w=pMask->Width;
	h=pMask->Height;
	if ((W<=3) || (H<=3) || (w>W) || (h>H))
	{
		if ((W<4) || (H<4)) {
			k_ShowMessageStd(
				IPL_POSITION, IPL_ERROR,
				"k_ConvolveIntToInt() Image size %d x %d is too small. Must be at least 4x4",
				W, H);
		}

        if ((w>W) || (h>H))
			k_ShowMessageSimple(
				IPL_ERROR,
				"k_ConvolveIntToInt() One or two mask dimensions exceeds the that of the image");

		return false;
	}
	else
	{
		float sum;
		TIntImage Temp;
		unsigned short BorderSize;
		if (pMask->Width > pMask->Height)
			BorderSize=(short)(pMask->Width+0.5);
		else
			BorderSize=(short)(pMask->Height+0.5);

		kI_InitImage(&Temp);
		kI_AllocImage(W,H,&Temp);
		kI_SetPeriodicBorder(BorderSize,pImg);

#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(Temp.History),&(pImg->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(Temp.History),
				"%sk_ConvolveIntToInt() With a %d*%d mask",
				ipl_HistoryIndent, w, h);
			k_IplStartHistoryMacro();
		}
#endif
		for (y1=0;y1<H;y1++)
		{
			for (x1=0;x1<W;x1++)
			{ 
				sum=0;
				for (y=0;y<h;y++)
					for (x=0;x<w;x++)
					{
						sum+=pImg->ppMatrix[y1-y][x1-x]*pMask->ppMatrix[y][x];
					}
					Temp.ppMatrix[y1][x1]=(INT16)sum;
			}
		}
		kI_CopyImage(pImg,&Temp);
		kI_EmptyImage(&Temp);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	} 
}

bool k_ConvolveFloatToFloat(TFloatImage *pImg,const TFloatImage *pMask)
{
	int x,y,x1,y1,w,h,W,H;
	W=pImg->Width;
	H=pImg->Height;
	w=pMask->Width;
	h=pMask->Height;
	if ((W<=3) || (H<=3) || (w>W) || (h>H))
	{
		if ((W<4) || (H<4)) {
			k_ShowMessageStd(
				IPL_POSITION, IPL_ERROR,
				"k_ConvolveFloatToFloat() Image size %d x %d is too small. Must be at least 4x4",
				W, H);
		}

        if ((w>W) || (h>H))
			k_ShowMessageSimple(
				IPL_ERROR,
				"k_ConvolveFloatToFloat() One or two mask dimensions exceeds the that of the image");

		return false;
	}
	else
	{
		float sum;
		TFloatImage Temp;
		unsigned short BorderSize;
		if (pMask->Width > pMask->Height)
			BorderSize=(short)(pMask->Width+0.5);
		else
			BorderSize=(short)(pMask->Height+0.5);

		kF_InitImage(&Temp);
		kF_AllocImage(W,H,&Temp);
		kF_SetPeriodicBorder(BorderSize,pImg);

#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(Temp.History),&(pImg->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(Temp.History),
				"%sk_ConvolveFloatToFloat() With a %d*%d mask",
				ipl_HistoryIndent, w, h);
			k_IplStartHistoryMacro();
		}
#endif
		for (y1=0;y1<H;y1++)
		{
			for (x1=0;x1<W;x1++)
			{  
				sum=0;
				for (y=0;y<h;y++)
					for (x=0;x<w;x++)
					{
						sum+=pImg->ppMatrix[y1-y][x1-x]*pMask->ppMatrix[y][x];
					}
					Temp.ppMatrix[y1][x1]=(FLOAT32)sum;
			}
		}
		kF_CopyImage(pImg,&Temp);
		kF_EmptyImage(&Temp);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	} 
}

bool k_BlurFourConnected3x3(TImage *pImg)
{
	UINT8 *L0,*L1,*L2,*Stop,*buf,*buf1;
	UINT8 temp;
	int x,y,w,h;
	w=pImg->Width;
	h=pImg->Height;
	if ((w<=3) || (h<=3) || (pImg->Bits!=8))
	{
        if (pImg->Bits!=8)
			k_ShowMessageSimple(IPL_ERROR, "k_BlurFourConnected3x3() Image does not have 8 b/p");
        else {
			k_ShowMessageStd(
				IPL_POSITION, IPL_ERROR,
				"k_BlurFourConnected3x3() Image size %d x %d is too small. Must be at least 4x4",
				w, h);
		}
		return false;
	}
	else
	{
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%sk_BlurFourConnected3x3()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		buf=(UINT8*) calloc(sizeof(UINT8),w+1);
		buf1=buf;
		L1=pImg->ppMatrix[0];
		L2=pImg->ppMatrix[1];
		Stop=L1+w-1;
		L1++;
        L2++;
        *buf1=(UINT8)((((UINT32)(*(L1-1))+(UINT32)(*L1)+(UINT32)(*(L1-1))
			+(UINT32)(*(L1-1))+(UINT32)(*(L2-1)))*13107)>>16);
		buf1++;
		while (L1<Stop)
		{
			L1++;
			L2++;
			*buf1=(UINT8)((((UINT32)(*(L1-2))+(UINT32)(*L1)+(UINT32)(*(L1-1))
				+(UINT32)(*(L1-1))+(UINT32)(*(L2-1)))*13107)>>16);
			buf1++;
		}
		*buf1=(UINT8)((((UINT32)(*(L1-1))+(UINT32)(*L1)+(UINT32)(*L1)
			+(UINT32)(*L1)+(UINT32)(*L2))*13107)>>16);
		for (y=1;y<h;y++)
		{
			buf1=buf;
			L0=pImg->ppMatrix[y-1];
			L1=pImg->ppMatrix[y];
			if (y==h-1)
				L2=L1;
			else
				L2=pImg->ppMatrix[y+1];
			Stop=L1+w-1;
			temp=*L0;
			*L0=*buf1;
			L0++;L1++;L2++;
			*buf1=(UINT8)((((UINT32)(*(L1-1))+(UINT32)(*L1)+(UINT32)(*(L1-1))
				+temp+(UINT32)(*(L2-1)))*13107)>>16);
			buf1++;
			while (L1<Stop)
			{
				temp=*L0;
				*L0=*buf1;
				L0++;
				L1++;
				L2++;
				*buf1=(UINT8)((((UINT32)(*(L1-2))+(UINT32)(*L1)+(UINT32)(*(L1-1))
					+temp+(UINT32)(*(L2-1)))*13107)>>16);
				buf1++;
			}
			*L0=*buf1;
			*buf1=(UINT8)((((UINT32)(*(L1-1))+(UINT32)(*L1)+(UINT32)(*L1)
				+(UINT32)(*L0)+(UINT32)(*L2))*13107)>>16);
		}
		for (x=0;x<w;x++)
			pImg->ppMatrix[h-1][x]=*(buf+x);
		free(buf);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool k_BlurEightConnected3x3(TImage *pImg)
{
	UINT8 *L0,*L1,*L2,*Stop,*buf,*buf1;
	UINT32 q0,q1,q2;
	int x,y,w,h;
	w=pImg->Width;
	h=pImg->Height;
	if ((w<=3) || (h<=3) || (pImg->Bits!=8))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_BlurEightConnected3x3() Image size %d x %d is too small. Must be at least 4x4",
			w, h);
		return false;
	}
	else
	{
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%sk_BlurEightConnected3x3()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		buf=(UINT8*) calloc(sizeof(UINT8),w+1);
		buf1=buf;
		L1=pImg->ppMatrix[0];
		L2=pImg->ppMatrix[1];
		Stop=L1+w-1;
		q1=(UINT32)(*L1)+(UINT32)(*L1)+(UINT32)(*L2);
		q2=q1;
		while (L1<Stop)
		{ 
			L1++;
			L2++;
			q0=q1;
			q1=q2;
			q2=(UINT32)(*L1)+(UINT32)(*L1)+(UINT32)(*L2);
			*buf1=(UINT8)(((q0+q1+q2)*7282)>>16);
			buf1++;
		}
		*buf1=(UINT8)(((q1+q2+q2)*7282)>>16);
		for (y=1;y<h;y++)
		{
			buf1=buf;
			L0=pImg->ppMatrix[y-1];
			L1=pImg->ppMatrix[y];
			if (y==h-1)
				L2=L1; 
			else 
				L2=pImg->ppMatrix[y+1];
			Stop=L1+w-1;
			q1=(UINT32)(*L0)+(UINT32)(*L1)+(UINT32)(*L2);
			q2=q1;
			while (L1<Stop)
			{
				*L0=*buf1;
				L0++;L1++;L2++;
				q0=q1;q1=q2;
				q2=(UINT32)(*L0)+(UINT32)(*L1)+(UINT32)(*L2);
				*buf1=(UINT8)(((q0+q1+q2)*7282)>>16);
				buf1++;
			}
			*L0=*buf1;
			*buf1=(UINT8)(((q1+q2+q2)*7282)>>16);
		}
		for (x=0;x<w;x++)
			pImg->ppMatrix[h-1][x]=*(buf+x);
		free(buf);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool k_BlurEightConnected3x3Int(TIntImage *pImg)
{
	INT16 *L0,*L1,*L2,*Stop,*buf,*buf1;
	UINT32 q0,q1,q2;
	int x,y,w,h;
	w=pImg->Width;
	h=pImg->Height;
	if ((w<=3) || (h<=3) )
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_BlurEightConnected3x3Int() Image size %d x %d is too small. Must be at least 4x4",
			w, h);
		return false;
	}
	else
	{
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%sk_BlurEightConnected3x3Int()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		buf=(INT16*) calloc(sizeof(INT16),w+1);
		buf1=buf;
		L1=pImg->ppMatrix[0];
		L2=pImg->ppMatrix[1];
		Stop=L1+w-1;
		q1=(UINT32)(*L1)+(UINT32)(*L1)+(UINT32)(*L2);
		q2=q1;
		while (L1<Stop)
		{
			L1++;
			L2++;
			q0=q1;
			q1=q2;
			q2=(UINT32)(*L1)+(UINT32)(*L1)+(UINT32)(*L2);
			*buf1=(INT16)(((q0+q1+q2)*7282)>>16);
			buf1++;
		}
		*buf1=(INT16)(((q1+q2+q2)*7282)>>16);
		for (y=1;y<h;y++)
		{
			buf1=buf;
			L0=pImg->ppMatrix[y-1];
			L1=pImg->ppMatrix[y];
			if (y==h-1)
				L2=L1;
			else
				L2=pImg->ppMatrix[y+1];
			Stop=L1+w-1;
			q1=(UINT32)(*L0)+(UINT32)(*L1)+(UINT32)(*L2);
			q2=q1;
			while (L1<Stop)
			{
				*L0=*buf1;
				L0++;L1++;L2++;
				q0=q1;q1=q2;
				q2=(UINT32)(*L0)+(UINT32)(*L1)+(UINT32)(*L2);
				*buf1=(INT16)(((q0+q1+q2)*7282)>>16);
				buf1++;
			}
			*L0=*buf1;
			*buf1=(INT16)(((q1+q2+q2)*7282)>>16);
		}
		for (x=0;x<w;x++)
			pImg->ppMatrix[h-1][x]=*(buf+x);
		free(buf);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool k_ConvolveFastByteToByte(TImage *pImg, const TFloatImage *pMask,float Prefactor)
{
	int w,h,W,H,x,y,intsum;
	w=pMask->Width;
	h=pMask->Height;
	W=pImg->Width;
	H=pImg->Height;
	if ((w!=3) || (h!=3) || (W<=3) || (H<=3) || (pImg->Bits!=8))
	{
		 if ((W<4) || (H<4))
		 {
			 k_ShowMessageStd(
				 IPL_POSITION, IPL_ERROR,
				 "k_ConvolveFastByteToByte() "
				 "Image size %d x %d is too small. Must be at least 4x4",
				 W, H);
		 }
		 else if ((w!=3) || (h!=3))
		 {
			 k_ShowMessageStd(
				 IPL_POSITION, IPL_ERROR,
				 "k_ConvolveFastByteToByte() Mask size %d x %d is wrong. Mask must be 3x3",
				 w,h);
		 }
		 else
			 k_ShowMessageSimple(IPL_ERROR, "k_ConvolveFastByteToByte() Image does not have 8 b/p");
		 
		 return false;
	}
	else
	{
		float m00,m01,m02,m10,m11,m12,m20,m21,m22;
		UINT8 *Row0,*Row1,*Row2;
		TImage Temp;
		k_InitImage(&Temp);
		k_AllocImage(W,H,pImg->Bits,&Temp);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(Temp.History),&(pImg->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(Temp.History),
				"%sk_ConvolveFastByteToByte() prefactor=%g",
				ipl_HistoryIndent, Prefactor);
			k_IplStartHistoryMacro();
		}
#endif
		m00=pMask->ppMatrix[0][0];
		m01=pMask->ppMatrix[0][1];
		m02=pMask->ppMatrix[0][2];
		m10=pMask->ppMatrix[1][0];
		m11=pMask->ppMatrix[1][1];
		m12=pMask->ppMatrix[1][2];
		m20=pMask->ppMatrix[2][0];
		m21=pMask->ppMatrix[2][1];
		m22=pMask->ppMatrix[2][2];
		Row1=pImg->ppMatrix[0];
		Row2=pImg->ppMatrix[1];
		if ((m00==(float)(0)) && (m20==(float)(0)) &&
			(m02==(float)(0)) && (m22==(float)(0)))
		{
			if ((m10==(float)(1)) && (m01==(float)(1)) &&
				(m12==(float)(1)) && (m21==(float)(1)))
			{
				for (y=1;y<H-1;y++)
				{
					Row0=Row1;
					Row1=Row2;
					Row2=pImg->ppMatrix[y+1];
					for (x=1;x<W-1;x++)
					{
						float TempVal;
						intsum=Row1[x-1]+Row0[x]+Row1[x+1]+Row2[x];
						TempVal=Prefactor*(intsum+Row1[x]*m11);
						Temp.ppMatrix[y][x]=(UINT8)((TempVal>0)?TempVal:0);
					}
				} /* end y=H-2 */
			} /* end if */
			else
			{
				for (y=1;y<H-1;y++)
				{
					Row0=Row1;
					Row1=Row2;
					Row2=pImg->ppMatrix[y+1];
					for (x=1;x<W-1;x++)
					{
						int TempVal=(int)((Row1[x-1]*m01+Row0[x]*m10
							+Row1[x+1]*m21+Row2[x]*m12
							+Row1[x]*m11)*Prefactor);
						Temp.ppMatrix[y][x]=(UINT8)((TempVal>0)?TempVal:0);
					} /* end x */
				} /*end y */
			} /* end else */
		} /* end if (5 element mask) */
		else
		{
			if ((m00==(float)(1)) && (m01==(float)(1)) &&
				(m02==(float)(1)) && (m10==(float)(1)) &&
				(m12==(float)(1)) && (m20==(float)(1)) &&
				(m21==(float)(1)) && (m22==(float)(1)))
			{
				for (y=1;y<H-1;y++)
				{
					Row0=Row1;
					Row1=Row2;
					Row2=pImg->ppMatrix[y+1];
					for (x=1;x<W-1;x++)
					{
						float TempVal;
						intsum=Row1[x-1]+Row0[x]+Row1[x+1]+Row2[x]+
							Row0[x-1]+Row0[x+1]+Row2[x-1]+Row2[x+1];
						TempVal=Prefactor*(intsum+Row1[x]*m11);
						Temp.ppMatrix[y][x]=(UINT8)((TempVal>0)?TempVal:0);
					}
				} /* end y */
			} /* end if */
			else
			{
				for (y=1;y<H-1;y++)
				{
					Row0=Row1;
					Row1=Row2;
					Row2=pImg->ppMatrix[y+1];
					for (x=1;x<W-1;x++)
					{
						int TempVal=(int)((Row1[x-1]*m01+Row0[x]*m10+
							Row1[x+1]*m21+Row2[x]*m12+
							Row0[x-1]*m00+Row0[x+1]*m20+
							Row2[x-1]*m02+Row2[x+1]*m22+
							Row1[x]*m11)*Prefactor);
						Temp.ppMatrix[y][x]=(UINT8)((TempVal>0)?TempVal:0);
					}
				} /* end y */
			} /* end else */
		} /* end else (all 9 elements) */
		for (x=0;x<W-1;x++)
		{
			Temp.ppMatrix[0][x]=Temp.ppMatrix[1][x];
			Temp.ppMatrix[H-1][x]=Temp.ppMatrix[H-2][x];
		}
		for (y=0;y<H-1;y++)
		{
			Temp.ppMatrix[y][0]=Temp.ppMatrix[0][1];
			Temp.ppMatrix[y][W-1]=Temp.ppMatrix[y][W-1];
		}
		k_CopyImage(pImg,&Temp);
		k_EmptyImage(&Temp);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
     }
}

bool k_ConvolveFastByteToInt(TImage *pSource, const TFloatImage *pMask,
							 float Prefactor, TIntImage *pDest)
{
	int w,h,W,H,x,y,intsum;
	w=pMask->Width;
	h=pMask->Height;
	W=pSource->Width;
	H=pSource->Height;
	if ((w!=3) || (h!=3) || (W<=3) || (H<=3) || (pSource->Bits!=8))
	{
		if ((W<4) || (H<4))
		{
			k_ShowMessageStd(
				IPL_POSITION, IPL_ERROR,
				"k_ConvolveFastByteToInt() Image size %d x %d is too small. Must be at least 4x4",
				W,H);
		}
		else
		{
			if ((w!=3) || (h!=3)) 
				k_ShowMessageStd(
					IPL_POSITION, IPL_ERROR,
					"k_ConvolveFastByteToInt() Mask size %d x %d is wrong. Mask must be 3x3",
					w, h);
			else
				k_ShowMessageSimple(IPL_ERROR, "k_ConvolveFastByteToInt() Source image does not have 8 b/p"); 
		}
		return false;
	}
	else
	{
		float m00,m01,m02,m10,m11,m12,m20,m21,m22;
		UINT8 *Row0,*Row1,*Row2;
		if (((int)pDest->Width!=W) || ((int)pDest->Height!=H))
		{
			kI_EmptyImage(pDest);
			kI_AllocImage(W,H,pDest);
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
				"%sk_ConvolveFastByteToInt() prefactor=%g",
				ipl_HistoryIndent, Prefactor);
			k_IplStartHistoryMacro();
		}
#endif
		m00=pMask->ppMatrix[0][0];
		m01=pMask->ppMatrix[0][1];
		m02=pMask->ppMatrix[0][2];
		m10=pMask->ppMatrix[1][0];
		m11=pMask->ppMatrix[1][1];
		m12=pMask->ppMatrix[1][2];
		m20=pMask->ppMatrix[2][0];
		m21=pMask->ppMatrix[2][1];
		m22=pMask->ppMatrix[2][2];
		Row1=pSource->ppMatrix[0];
		Row2=pSource->ppMatrix[1];
		if ((m00==(float)(0)) && (m20==(float)(0)) &&
			(m02==(float)(0)) && (m22==(float)(0)))
		{
			if ((m10==(float)(1)) && (m01==(float)(1)) &&
				(m12==(float)(1)) && (m21==(float)(1)))
			{
				for (y=1;y<H-1;y++)
				{
					Row0=Row1;
					Row1=Row2;
					Row2=pSource->ppMatrix[y+1];
					for (x=1;x<W-1;x++)
					{
						intsum=Row1[x-1]+Row0[x]+Row1[x+1]+Row2[x];
						pDest->ppMatrix[y][x]=(INT16)
							(Prefactor*(intsum+Row1[x]*m11));
					}
				} /* end y=H-2 */
			} /* end if */
			else
			{
				for (y=1;y<H-1;y++)
				{
					Row0=Row1;Row1=Row2;
					Row2=pSource->ppMatrix[y+1];
					for (x=1;x<W-1;x++)
					{
						pDest->ppMatrix[y][x]=
							(INT16)((Row1[x-1]*m01+Row0[x]*m10
							+Row1[x+1]*m21+Row2[x]*m12
							+Row1[x]*m11)*Prefactor);
					} /* end x */
				} /* end y */
			} /* end else */
		} /*end if (5 element mask) */
		else
		{
			if ((m00==(float)(1)) && (m01==(float)(1)) &&
				(m02==(float)(1)) && (m10==(float)(1)) &&
				(m12==(float)(1)) && (m20==(float)(1)) &&
				(m21==(float)(1)) && (m22==(float)(1)))
			{
				for (y=1;y<H-1;y++)
				{
					Row0=Row1;
					Row1=Row2;
					Row2=pSource->ppMatrix[y+1];
					for (x=1;x<W-1;x++)
					{
						intsum=Row1[x-1]+Row0[x]+Row1[x+1]+Row2[x]+
							Row0[x-1]+Row0[x+1]+Row2[x-1]+Row2[x+1];
						pDest->ppMatrix[y][x]=(INT16)
							(Prefactor*(intsum+Row1[x]*m11));
					}
				} /* end y */
			} /* end if */
			else
			{
				for (y=1;y<H-1;y++)
				{
					Row0=Row1;
					Row1=Row2;
					Row2=pSource->ppMatrix[y+1];
					for (x=1;x<W-1;x++)
					{
						pDest->ppMatrix[y][x]=(INT16)
							((Row1[x-1]*m01+Row0[x]*m10+
							Row1[x+1]*m21+Row2[x]*m12+
							Row0[x-1]*m00+Row0[x+1]*m20+
							Row2[x-1]*m02+Row2[x+1]*m22+
							Row1[x]*m11)*Prefactor);
					}
				} /* end y */
			} /* end else */
		} /* end else (all 9 elements) */
		for (x=0;x<W-1;x++)
		{
			pDest->ppMatrix[0][x]=pDest->ppMatrix[1][x];
			pDest->ppMatrix[H-1][x]=pDest->ppMatrix[H-2][x];
		}
		for (y=0;y<H-1;y++)
		{
			pDest->ppMatrix[y][0]=pDest->ppMatrix[y][1];
			pDest->ppMatrix[y][W-1]=pDest->ppMatrix[y][W-1];
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
   } /* end if 3x3 */
}

bool k_ConvolveFastIntToInt(TIntImage *pImg, const TFloatImage *pMask,float Prefactor)
{
	int w,h,W,H,x,y,intsum;
	w=pMask->Width;
	h=pMask->Height;
	W=pImg->Width;
	H=pImg->Height;
	if ((w!=3) || (h!=3) || (W<=3) || (H<=3))
	{
		if ((W<4) || (H<4))
		{
			k_ShowMessageStd(
				IPL_POSITION, IPL_ERROR,
				"k_ConvolveFastIntToInt() Image size %d x %d is too small. Must be at least 4x4",
				W, H);
		}
		if ((w!=3) || (h!=3)) 
		{
			k_ShowMessageStd(
				IPL_POSITION, IPL_ERROR,
				"k_ConvolveFastIntToInt() Mask size %d x %d is wrong. Mask must be 3x3",
				w, h);
		}
		return false;
	}
	else
	{
		float m00,m01,m02,m10,m11,m12,m20,m21,m22;
		INT16 *Row0,*Row1,*Row2;
		TIntImage Temp;
		kI_InitImage(&Temp);
		kI_AllocImage(W,H,&Temp);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(Temp.History),&(pImg->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(Temp.History),
				"%sk_ConvolveFastIntToInt() prefactor=%g",
				ipl_HistoryIndent, Prefactor);
			k_IplStartHistoryMacro();
		}
#endif
		m00=pMask->ppMatrix[0][0];
		m01=pMask->ppMatrix[0][1];
		m02=pMask->ppMatrix[0][2];
		m10=pMask->ppMatrix[1][0];
		m11=pMask->ppMatrix[1][1];
		m12=pMask->ppMatrix[1][2];
		m20=pMask->ppMatrix[2][0];
		m21=pMask->ppMatrix[2][1];
		m22=pMask->ppMatrix[2][2];
		Row1=pImg->ppMatrix[0];
		Row2=pImg->ppMatrix[1];
		if ((m00==(float)(0)) && (m20==(float)(0)) &&
			(m02==(float)(0)) && (m22==(float)(0)))
		{
			if ((m10==(float)(1)) && (m01==(float)(1)) &&
				(m12==(float)(1)) && (m21==(float)(1)))
			{
				for (y=1;y<H-1;y++)
				{
					Row0=Row1;
					Row1=Row2;
					Row2=pImg->ppMatrix[y+1];
					for (x=1;x<W-1;x++)
					{
						intsum=Row1[x-1]+Row0[x]+Row1[x+1]+Row2[x];
						Temp.ppMatrix[y][x]=(INT16)
							(Prefactor*(intsum+Row1[x]*m11));
					}
				} /* end y=H-2 */
			} /* end if */
			else
			{
				for (y=1;y<H-1;y++)
				{
					Row0=Row1;
					Row1=Row2;
					Row2=pImg->ppMatrix[y+1];
					for (x=1;x<W-1;x++)
					{
						Temp.ppMatrix[y][x]=
							(INT16)((Row1[x-1]*m01+Row0[x]*m10
							+Row1[x+1]*m21+Row2[x]*m12
							+Row1[x]*m11)*Prefactor);
					} /* end x */
				} /* end y */
			} /* end else */
		} /* end if (5 element mask) */
		else
		{
			if ((m00==(float)(1)) && (m01==(float)(1)) &&
				(m02==(float)(1)) && (m10==(float)(1)) &&
				(m12==(float)(1)) && (m20==(float)(1)) &&
				(m21==(float)(1)) && (m22==(float)(1)))
			{
				for (y=1;y<H-1;y++)
				{
					Row0=Row1;
					Row1=Row2;
					Row2=pImg->ppMatrix[y+1];
					for (x=1;x<W-1;x++)
					{
						intsum=Row1[x-1]+Row0[x]+Row1[x+1]+Row2[x]+
							Row0[x-1]+Row0[x+1]+Row2[x-1]+Row2[x+1];
						Temp.ppMatrix[y][x]=(INT16)
							(Prefactor*(intsum+Row1[x]*m11));
					}
				} /* end y */
			} /* end if */
			else
			{
				for (y=1;y<H-1;y++)
				{
					Row0=Row1;
					Row1=Row2;
					Row2=pImg->ppMatrix[y+1];
					for (x=1;x<W-1;x++)
					{
						Temp.ppMatrix[y][x]=(INT16)
							((Row1[x-1]*m01+Row0[x]*m10+
							Row1[x+1]*m21+Row2[x]*m12+
							Row0[x-1]*m00+Row0[x+1]*m20+
							Row2[x-1]*m02+Row2[x+1]*m22+
							Row1[x]*m11)*Prefactor);
					}
				} /* end y */
			} /* end else */
		} /* end else (all 9 elements) */
		for (x=0;x<W-1;x++)
		{
			Temp.ppMatrix[0][x]=Temp.ppMatrix[1][x];
			Temp.ppMatrix[H-1][x]=Temp.ppMatrix[H-2][x];
		}
		for (y=0;y<H-1;y++)
		{
			Temp.ppMatrix[y][0]=Temp.ppMatrix[y][1];
			Temp.ppMatrix[y][W-1]=Temp.ppMatrix[y][W-1];
		}
		kI_CopyImage(pImg,&Temp);
		kI_EmptyImage(&Temp);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
   } /* end if 3x3 */
}

bool k_MaxGradientFastByteToByte(TImage *pImg,float Prefactor)
{
	int W,H,x,y,Mx,d1,d2,d3,d4;
	W=pImg->Width;
	H=pImg->Height;
	if ((W<=3) || (H<=3) || (pImg->Bits!=8))
	{
        if (pImg->Bits!=8)
		{
			k_ShowMessageSimple(IPL_ERROR, "k_MaxGratientFastByteToByte() Image does not have 8 Bits/pixel");
		}
		else
		{
			k_ShowMessageStd(
				IPL_POSITION, IPL_ERROR,
				"k_MaxGratientFastByteToByte() Image size %d x %d is too small. Must be at least 4x4",
				W, H);
		}
		return false;
	}
	else
	{
		TImage Temp;
		UINT8 *Row0,*Row1,*Row2;
		k_InitImage(&Temp);
		k_AllocImage(W,H,8,&Temp);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(Temp.History),&(pImg->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(Temp.History),
				"%sk_MaxGradientFastByteToByte() prefactor=%g",
				ipl_HistoryIndent, Prefactor);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<H;y++){
			Temp.ppMatrix[y][0]=0;
			Temp.ppMatrix[y][W-1]=0;
		}
		for (x=0;x<W;x++){
			Temp.ppMatrix[0][x]=0; 
			Temp.ppMatrix[H-1][x]=0;
		}
		Row1=pImg->ppMatrix[0];
		Row2=pImg->ppMatrix[1];
		for (y=1;y<H-1;y++)
		{
			Row0=Row1;
			Row1=Row2;
			Row2=pImg->ppMatrix[y+1];
			for (x=1;x<W-1;x++)
			{
				d1=abs(Row1[x-1]-Row1[x+1]);
				d2=abs(Row0[x]-Row2[x]);
				d3=abs((2*(Row0[x-1]-Row2[x+1]))/3);
				d4=abs((2*(Row0[x+1]-Row2[x-1]))/3);
				Mx=k_Max(d1,d2);
				Mx=k_Max(d3,Mx);
				Mx=k_Max(d4,Mx);
				Mx=(int)(Mx*Prefactor);
				if (Mx>255)
					Mx=255;
				Temp.ppMatrix[y][x]=Mx;
			}
		}
		k_CopyImage(pImg,&Temp);
		k_EmptyImage(&Temp);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool k_MaxGradientFastByteToInt(TImage *pSource,float Prefactor,TIntImage *pDest)
{
	int W,H,x,y;
	W=pSource->Width;
	H=pSource->Height;
	if ((W<=3) || (H<=3) || (pSource->Bits!=8))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_MaxGratientFastByteToInt() Image size %d x %d is too small. Must be at least 4x4",
			W, H);
		return false;
	}
	else
	{
		INT16 Mx,d1,d2,d3,d4;
		UINT8 *Row0,*Row1,*Row2;
		if (((int)pDest->Width!=W) || ((int)pDest->Height!=H))
		{
			kI_EmptyImage(pDest);
			kI_AllocImage(W,H,pDest);
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
				"%sk_MaxGradientFastByteToInt() prefactor=%g",
				ipl_HistoryIndent, Prefactor);
			k_IplStartHistoryMacro();
		}
#endif
		Row1=pSource->ppMatrix[0];
		Row2=pSource->ppMatrix[1];
		for (y=1;y<H-1;y++)
		{ 
			Row0=Row1;
			Row1=Row2;
			Row2=pSource->ppMatrix[y+1];
			for (x=1;x<W-1;x++)
			{
				d1=abs(Row1[x-1]-Row1[x+1]);
				d2=abs(Row0[x]-Row2[x]);
				d3=abs((2*(Row0[x-1]-Row2[x+1]))/3);
				d4=abs((2*(Row0[x+1]-Row2[x-1]))/3);
				Mx=k_Max(d1,d2);
				Mx=k_Max(d3,Mx);
				Mx=k_Max(d4,Mx);
				pDest->ppMatrix[y][x]=(INT16)(Mx*Prefactor);
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	} 
}

bool k_MaxGradientFastIntToInt(TIntImage *pImg,float Prefactor)
{
	int W,H,x,y;
	W=pImg->Width;
	H=pImg->Height;
	if ((W<=3) || (H<=3))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_MaxGratientFastIntToInt(). Image size %d x %d is too small. Must be at least 4x4",
			W, H);
		return false;
	}
	else
	{
		INT16 *Row0,*Row1,*Row2;
		INT16 Mx,d1,d2,d3,d4;
		TIntImage Temp;
		kI_InitImage(&Temp);
		kI_AllocImage(W,H,&Temp);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(Temp.History),&(pImg->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(Temp.History),
				"%sk_MaxGradientFastIntToInt() prefactor=%g",
				ipl_HistoryIndent, Prefactor);
			k_IplStartHistoryMacro();
		}
#endif
		Row1=pImg->ppMatrix[0];
		Row2=pImg->ppMatrix[1];
		for (y=1;y<H-1;y++)
		{ 
			Row0=Row1;
			Row1=Row2;
			Row2=pImg->ppMatrix[y+1];
			for (x=1;x<W-1;x++)
			{
				d1=abs(Row1[x-1]-Row1[x+1]);
				d2=abs(Row0[x]-Row2[x]);
				d3=abs((2*(Row0[x-1]-Row2[x+1]))/3);
				d4=abs((2*(Row0[x+1]-Row2[x-1]))/3);
				Mx=k_Max(d1,d2);
				Mx=k_Max(d3,Mx);
				Mx=k_Max(d4,Mx);
				Temp.ppMatrix[y][x]=(INT16)(Mx*Prefactor);
			}
		}
		kI_CopyImage(pImg,&Temp);
		kI_EmptyImage(&Temp);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	} 
}

bool k_MedianFilterFourConnected3x3(TImage *pImg)
{
	int W,H,x,y;
	W=pImg->Width;
	H=pImg->Height;
	if ((W<=3) || (H<=3) || (pImg->Bits!=8))
	{
        if (pImg->Bits!=8)
			k_ShowMessageSimple(IPL_ERROR, "k_MedianFilterFourConnected3x3() Image does not have 8 Bits/pixel"); 
		else {
			k_ShowMessageStd(
				IPL_POSITION, IPL_ERROR,
				"k_MedianFilterFourConnected3x3() Image size %d x %d is too small. Must be at least 4x4",
				W, H);
		}
		return false;
	}
	else
	{
		TImage Temp;
		UINT8 q0,q1,q2,q3,q4,buf;
		k_InitImage(&Temp);
		k_AllocImage(W,H,8,&Temp);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(Temp.History),&(pImg->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(Temp.History),
				"%sk_MedianFilterFourConnected3x3()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<H-1;y++)
		{
			Temp.ppMatrix[y][0]=pImg->ppMatrix[0][1];
			Temp.ppMatrix[y][W-1]=pImg->ppMatrix[y][W-1];
		}
		for (y=1;y<H-1;y++)
		{
			for (x=1;x<W-1;x++)
			{ 
				q0=pImg->ppMatrix[y][x];
				q1=pImg->ppMatrix[y-1][x];
				q2=pImg->ppMatrix[y+1][x];
				q3=pImg->ppMatrix[y][x-1];
				q4=pImg->ppMatrix[y][x+1];
				if (q0<q1)
				{
					buf=q0;
					q0=q1;
					q1=buf;
				}
				if (q0<q2)
				{
					buf=q0;
					q0=q2;
					q2=buf;
				}
				if (q0<q3)
				{
					buf=q0;
					q0=q3;
					q3=buf;
				}
				if (q0<q4)
				{
					buf=q0;
					q0=q4;
					q4=buf;
				}
				if (q1<q2)
				{
					buf=q1;
					q1=q2;
					q2=buf;
				}
				if (q1<q3)
				{
					buf=q1;
					q1=q3;
					q3=buf;
				}
				if (q1<q4)
				{
					buf=q1;
					q1=q4;
					q4=buf;
				}
				if (q2<q3)
				{
					q2=q3;
				}
				if (q2<q4)
				{
					q2=q4;
				}
				Temp.ppMatrix[y][x]=q2;
			}
		}
		k_CopyImage(pImg,&Temp);
		k_EmptyImage(&Temp);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool k_MedianFilterDiagonalConnected3x3(TImage *pImg)
{
	int W,H,x,y;
	W=pImg->Width;
	H=pImg->Height;
	if ((W<=3) || (H<=3) || (pImg->Bits!=8))
	{
		if (pImg->Bits!=8)
			k_ShowMessageSimple(IPL_ERROR, "k_MedianFilterDiagonalConnected3x3() Image does not have 8 b/p"); 
		else {
			k_ShowMessageStd(
				IPL_POSITION, IPL_ERROR,
				"k_MedianFilterDiagonalConnected3x3() Image size %d x %d is too small. Must be at least 4x4",
				W, H);
		}
		return false;
	}
	else
	{
		TImage Temp;
		UINT8 q0,q1,q2,q3,q4,buf;
		k_InitImage(&Temp);
		k_AllocImage(W,H,8,&Temp);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(Temp.History),&(pImg->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(Temp.History),
				"%sk_MedianFilterDiagonalConnected3x3()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<H-1;y++)
		{
			Temp.ppMatrix[y][0]=pImg->ppMatrix[0][1];
			Temp.ppMatrix[y][W-1]=pImg->ppMatrix[y][W-1];
		}
		for (y=1;y<H-1;y++)
		{
			for (x=1;x<W-1;x++)
			{ 
				q0=pImg->ppMatrix[y][x];
				q1=pImg->ppMatrix[y-1][x-1];
				q2=pImg->ppMatrix[y+1][x-1];
				q3=pImg->ppMatrix[y-1][x+1];
				q4=pImg->ppMatrix[y+1][x+1];
				if (q0<q1)
				{
					buf=q0;
					q0=q1;
					q1=buf;
				}
				if (q0<q2)
				{
					buf=q0;
					q0=q2;
					q2=buf;
				}
				if (q0<q3)
				{
					buf=q0;
					q0=q3;
					q3=buf;
				}
				if (q0<q4)
				{
					buf=q0;
					q0=q4;
					q4=buf;
				}
				if (q1<q2)
				{
					buf=q1;
					q1=q2;
					q2=buf;
				}
				if (q1<q3)
				{
					buf=q1;
					q1=q3;
					q3=buf;
				}
				if (q1<q4)
				{
					buf=q1;
					q1=q4;
					q4=buf;
				}
				if (q2<q3)
				{
					q2=q3;
				}
				if (q2<q4)
				{
					q2=q4;
				}
				Temp.ppMatrix[y][x]=q2;
			}
		}
		k_CopyImage(pImg,&Temp);
		k_EmptyImage(&Temp);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool k_MedianFilterFourConnected3x3Int(TIntImage *pImg)
{
	int W,H,x,y;
	W=pImg->Width;
	H=pImg->Height;
	if ((W<=3) || (H<=3))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_MedianFilterFourConnected3x3Int() Image size %d x %d is too small. Must be at least 4x4",
			W, H);
		return false;
	}
	else
	{
		TIntImage Temp;
		INT16 q0,q1,q2,q3,q4,buf;
		kI_InitImage(&Temp);
		kI_AllocImage(W,H,&Temp);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(Temp.History),&(pImg->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(Temp.History),
				"%sk_MedianFilterFourConnected3x3Int()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<H-1;y++)
		{
			Temp.ppMatrix[y][0]=pImg->ppMatrix[0][1];
			Temp.ppMatrix[y][W-1]=pImg->ppMatrix[y][W-1];
		}
		for (y=1;y<H-1;y++)
		{
			for (x=1;x<W-1;x++)
			{ 
				q0=pImg->ppMatrix[y][x];
				q1=pImg->ppMatrix[y-1][x];
				q2=pImg->ppMatrix[y+1][x];
				q3=pImg->ppMatrix[y][x-1];
				q4=pImg->ppMatrix[y][x+1];
				if (q0<q1)
				{
					buf=q0;
					q0=q1;
					q1=buf;
				}
				if (q0<q2)
				{
					buf=q0;
					q0=q2;
					q2=buf;
				}
				if (q0<q3)
				{
					buf=q0;
					q0=q3;
					q3=buf;
				}
				if (q0<q4)
				{
					buf=q0;
					q0=q4;
					q4=buf;
				}
				if (q1<q2)
				{
					buf=q1;
					q1=q2;
					q2=buf;
				}
				if (q1<q3)
				{
					buf=q1;
					q1=q3;
					q3=buf;
				}
				if (q1<q4)
				{
					buf=q1;
					q1=q4;
					q4=buf;
				}
				if (q2<q3)
				{
					q2=q3;
				}
				if (q2<q4)
				{
					q2=q4;
				}
				Temp.ppMatrix[y][x]=q2;
			}
		}
		kI_CopyImage(pImg,&Temp);
		kI_EmptyImage(&Temp);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool k_MedianFilterDiagonalConnected3x3Int(TIntImage *pImg)
{
	TIntImage Temp;
	INT16 q0,q1,q2,q3,q4,buf;
	int W,H,x,y;
	W=pImg->Width;
	H=pImg->Height;
	if ((W<=3) || (H<=3))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_MedianFilterDiagonalConnected3x3Int(). Image size %d x %d is too small. Must be at least 4x4",
			W, H);
		return false;
	}
	else
	{
		kI_InitImage(&Temp);
		kI_AllocImage(W,H,&Temp);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(Temp.History),&(pImg->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(Temp.History),
				"%sk_MedianFilterDiagonalConnected3x3Int()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<H-1;y++)
		{
			Temp.ppMatrix[y][0]=pImg->ppMatrix[0][1];
			Temp.ppMatrix[y][W-1]=pImg->ppMatrix[y][W-1];
		}
		for (y=1;y<H-1;y++)
		{
			for (x=1;x<W-1;x++)
			{ 
				q0=pImg->ppMatrix[y][x];
				q1=pImg->ppMatrix[y-1][x-1];
				q2=pImg->ppMatrix[y+1][x-1];
				q3=pImg->ppMatrix[y-1][x+1];
				q4=pImg->ppMatrix[y+1][x+1];
				if (q0<q1)
				{
					buf=q0;
					q0=q1;
					q1=buf;
				}
				if (q0<q2)
				{
					buf=q0;
					q0=q2;
					q2=buf;
				}
				if (q0<q3)
				{
					buf=q0;
					q0=q3;
					q3=buf;
				}
				if (q0<q4)
				{
					buf=q0;
					q0=q4;
					q4=buf;
				}
				if (q1<q2)
				{
					buf=q1;
					q1=q2;
					q2=buf;
				}
				if (q1<q3)
				{
					buf=q1;
					q1=q3;
					q3=buf;
				}
				if (q1<q4)
				{
					buf=q1;
					q1=q4;
					q4=buf;
				}
				if (q2<q3)
				{
					q2=q3;
				}
				if (q2<q4)
				{
					q2=q4;
				}
				Temp.ppMatrix[y][x]=q2;
			}
		}
		kI_CopyImage(pImg,&Temp);
		kI_EmptyImage(&Temp);
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
