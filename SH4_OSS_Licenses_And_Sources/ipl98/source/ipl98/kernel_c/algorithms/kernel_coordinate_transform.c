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

#include "kernel_coordinate_transform.h"
#include "../kernel_error.h"
#include "../kernel_history.h"
#include <math.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/************************************************/
/********        Public functions        ********/
/************************************************/

bool k_ScaleAndRotateAuto(TImage* pSource,TImage* pDest,double Sx, double Sy,
						  double phi,UINT32 Background)
{
	double minx,miny,maxx,maxy, PosSourceX, PosSourceY;
	TImage* pTarget=pDest;
	bool CopyToSource=false;
	bool ResetBorder=false;
	int x,y,w,h,w1,h1;
	double x1,y1;
	double cosphi=cos(phi);
	double sinphi=sin(phi);
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_ScaleAndRotateAuto() Source image is empty - doing nothing");
		return false;
	}
	if ((pSource->Bits!=1) && (pSource->Bits!=8) && (pSource->Bits!=24))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_ScaleAndRotateAuto() Number of bits per pixel=%d not supported",
			pSource->Bits);
		return false;
	}
	if (pDest==pSource)
	{
		pTarget=(TImage*) malloc(sizeof(TImage));
		k_InitImage(pTarget);
		CopyToSource=true;
	}
	w=pSource->Width;
	h=pSource->Height;
	if (cosphi>0)
	{ 
		if (sinphi>0)
		{
			minx=0;
			maxx=Sx*w*cosphi+Sy*h*sinphi;
			miny=-Sx*w*sinphi;
			maxy=Sy*h*cosphi;
		}
		else
		{
			minx=Sy*h*sinphi;
			maxx=Sx*w*cosphi;
			miny=0;
			maxy=-Sx*w*sinphi+Sy*h*cosphi;
		}
	}
	else
	{ 
		if (sinphi>0)
		{
			minx=Sx*w*cosphi;
			maxx=Sy*h*sinphi;
			miny=-Sx*w*sinphi+Sy*h*cosphi;
			maxy=0;
		}
		else
		{
			minx=Sx*w*cosphi+Sy*h*sinphi;
			maxx=0;
			miny=Sy*h*cosphi;
			maxy=-Sx*w*sinphi;
		}
	}
	w1=(int)(maxx-minx+0.5);
	h1=(int)(maxy-miny+0.5);
	/* prepare destination image, following lines faster than calling k_CopyImage() */
	k_AllocImage(w1,h1,pSource->Bits,pTarget);
	k_CopyPalette(&pTarget->Pal,&pSource->Pal);
	k_CopyFileInfo(&(pTarget->FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
	/* copy history before adding comment */
	k_CopyText(&(pTarget->History),&(pSource->History));
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(pTarget->History),
			"%sk_ScaleAndRotateAuto(Sx=%g,Sy=%g,phi=%g,Background=%d)",
			ipl_HistoryIndent, Sx, Sy, phi, Background);
		k_IplStartHistoryMacro();
	}
#endif
	if (pSource->BorderSize==0)
	{
		k_SetBorder(1,Background,pSource);
		ResetBorder=true;
	}

	if (pSource->Bits==24)
	{
		for (x=0;x<w1;x++)
		{
			for (y=0;y<h1;y++)
			{
				x1=(x+minx);
				y1=(y+miny);
				PosSourceX=(x1*cosphi-y1*sinphi)/Sx;
				PosSourceY=(x1*sinphi+y1*cosphi)/Sy;
				/*if((PosSourceX<=-1) || (PosSourceX>=w) || (PosSourceY<=-1) || (PosSourceY>=h))*/ /* changed in v. 1.3 */
				if((PosSourceX<0) || (PosSourceX>w-1) || (PosSourceY<0) || (PosSourceY>h-1))
				{
					/*k_SetPixelFast(x,y,Background,pTarget);*/
					k_SetPixel24bp(x,y,Background,(*pTarget));
				}
				else
				{
					/*k_SetPixelFast(x,y,k_GetColorInterpolated(PosSourceX,PosSourceY,pSource),pTarget);*/
					k_SetPixel24bp(x,y,k_GetColorInterpolatedFast((FLOAT32)PosSourceX,(FLOAT32)PosSourceY,pSource),(*pTarget));
				}
			}
		}
	}
	else if (pSource->Bits==8)
	{
		for (x=0;x<w1;x++)
		{
			for (y=0;y<h1;y++)
			{
				x1=(x+minx);
				y1=(y+miny);
				PosSourceX=(x1*cosphi-y1*sinphi)/Sx;
				PosSourceY=(x1*sinphi+y1*cosphi)/Sy;
				/*if((PosSourceX<=-1) || (PosSourceX>=w) || (PosSourceY<=-1) || (PosSourceY>=h))*/ /* changed in v. 1.3 */
				if((PosSourceX<0) || (PosSourceX>w-1) || (PosSourceY<0) || (PosSourceY>h-1))
				{
					/*k_SetPixelFast(x,y,Background,pTarget);*/
					k_SetPixel8bp(x,y,Background,(*pTarget));
				}
				else
				{
					/*k_SetPixelFast(x,y,(UINT32)(k_GetPixelInterpolatedFast(PosSourceX,PosSourceY,pSource)+0.5),pTarget);*/
					k_SetPixel8bp(x,y,(UINT32)(k_GetPixelInterpolatedFast((FLOAT32)PosSourceX,(FLOAT32)PosSourceY,pSource)+0.5),(*pTarget));
				}
			}
		}
	}
	else if (pSource->Bits==1)
	{
		for (x=0;x<w1;x++)
		{
			for (y=0;y<h1;y++)
			{
				x1=(x+minx);
				y1=(y+miny);
				PosSourceX=(x1*cosphi-y1*sinphi)/Sx;
				PosSourceY=(x1*sinphi+y1*cosphi)/Sy;
				/*if((PosSourceX<=-1) || (PosSourceX>=w) || (PosSourceY<=-1) || (PosSourceY>=h))*/ /* changed in v. 1.3 */
				if((PosSourceX<0) || (PosSourceX>w-1) || (PosSourceY<0) || (PosSourceY>h-1))
				{
					/*k_SetPixelFast(x,y,Background,pTarget);*/
					k_SetPixel1bp(x,y,Background,(*pTarget));
				}
				else
				{
					/*k_SetPixelFast(x,y,(UINT32)(k_GetPixelInterpolatedFast(PosSourceX,PosSourceY,pSource)+0.5),pTarget);*/
					k_SetPixel1bp(x,y,(UINT32)(k_GetPixelInterpolatedFast((FLOAT32)PosSourceX,(FLOAT32)PosSourceY,pSource)+0.5),(*pTarget));
				}
			}
		}
	}
	if (CopyToSource==true)
	{
		k_CopyImage(pDest,pTarget);
		k_EmptyImage(pTarget);
		free(pTarget);
	}
	else if (ResetBorder==true)
		k_SetBorderSize(0,pSource);

#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
	return true;
}

bool k_ScaleAndRotateFixed(TImage* pSource,TImage* pDest,double x0,double y0,
						   int Width,int Height,double Sx, double Sy, double phi, UINT32 Background)
{
	int x,y,x1,y1,w,h;
	TImage* pTarget=pDest;
	bool CopyToSource=false;
	bool ResetBorder=false;
	double cosphi=cos(phi);
	double sinphi=sin(phi);
	double PosSourceX, PosSourceY;
	int wHalf=Width/2, hHalf=Height/2;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_ScaleAndRotateFixed() Source image is empty - doing nothing");
		return false;
	}
	if (pDest==pSource)
	{
		pTarget=(TImage*) malloc(sizeof(TImage));
		k_InitImage(pTarget);
		CopyToSource=true;
	}
	if ((pSource->Bits!=1) && (pSource->Bits!=8) && (pSource->Bits!=24))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_ScaleAndRotateFixed() Number of bits per pixel=%d not supported",
			pSource->Bits);
		return false;
	}
	w=pSource->Width;
	h=pSource->Height;
	if (pSource->BorderSize==0)
	{
		k_SetBorder(1,Background,pSource);
		ResetBorder=true;
	}
	/* prepare destination image, following lines faster than calling k_CopyImage() */
	k_AllocImage(Width,Height,pSource->Bits,pTarget);
	k_CopyPalette(&pTarget->Pal,&pSource->Pal);
	k_CopyFileInfo(&(pTarget->FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
	/* copy history before adding comment */
	k_CopyText(&(pTarget->History),&(pSource->History));
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(pTarget->History),
			"%sk_ScaleAndRotateFixed(x0=%g,y0=%g,Width=%d,"
			"Height=%d,Sx=%g,Sy=%g,phi=%g,Background=%d)",
			ipl_HistoryIndent, x0, y0, Width, Height,
			Sx, Sy, phi, Background);
		k_IplStartHistoryMacro();
	}
#endif
	if (pSource->Bits==24)
	{
		for (x=0;x<Width;x++)
		{
			for (y=0;y<Height;y++)
			{
				x1=x-wHalf;
				y1=y-hHalf;
				PosSourceX=(x1*cosphi-y1*sinphi)/Sx+x0;
				PosSourceY=(x1*sinphi+y1*cosphi)/Sy+y0;
				/*if((PosSourceX<=-1) || (PosSourceX>=w) || (PosSourceY<=-1) || (PosSourceY>=h))*/ /* changed in v. 1.3 */
				if((PosSourceX<0) || (PosSourceX>w-1) || (PosSourceY<0) || (PosSourceY>h-1))
				{
					/*k_SetPixelFast(x,y,Background,pTarget);*/
					k_SetPixel24bp(x,y,Background,(*pTarget)); /* just use background as color */
				}
				else 
				{
					/*k_SetPixelFast(x,y,k_GetColorInterpolated(PosSourceX,PosSourceY,pSource),pTarget);*/
					/*k_SetPixel24bp(x,y,k_GetColorInterpolatedFast(PosSourceX,PosSourceY,pSource),(*pTarget));*/
					k_SetPixel24bp(x,y,k_GetColorInterpolatedFast((FLOAT32)((x1*cosphi-y1*sinphi)/Sx+x0),(FLOAT32)((x1*sinphi+y1*cosphi)/Sy+y0),pSource),(*pTarget));
				}
			}
		}
	}
	else if (pSource->Bits==8)
	{
		for (x=0;x<Width;x++)
		{
			for (y=0;y<Height;y++)
			{
				x1=x-wHalf;
				y1=y-hHalf;
				PosSourceX=(x1*cosphi-y1*sinphi)/Sx+x0;
				PosSourceY=(x1*sinphi+y1*cosphi)/Sy+y0;
				/*if((PosSourceX<=-1) || (PosSourceX>=w) || (PosSourceY<=-1) || (PosSourceY>=h))*/ /* changed in v. 1.3 */
				if((PosSourceX<0) || (PosSourceX>w-1) || (PosSourceY<0) || (PosSourceY>h-1))
				{
					/*k_SetPixelFast(x,y,Background,pTarget);*/
					k_SetPixel8bp(x,y,Background,(*pTarget)); /* just use background as color */
				}
				else
				{
					/*k_SetPixelFast(x,y,(int)k_GetPixelInterpolated(PosSourceX,PosSourceY,pSource),pTarget);*/
					/*k_SetPixel8bp(x,y,(int)k_GetPixelInterpolatedFast(PosSourceX,PosSourceY,pSource),(*pTarget));*/
					k_SetPixel8bp(x,y,(int)k_GetPixelInterpolatedFast((FLOAT32)((x1*cosphi-y1*sinphi)/Sx+x0),(FLOAT32)((x1*sinphi+y1*cosphi)/Sy+y0),pSource),(*pTarget));
				}
			}
		}
	}
	else if (pSource->Bits==1)
	{
		for (x=0;x<Width;x++)
		{
			for (y=0;y<Height;y++)
			{
				x1=x-wHalf;
				y1=y-hHalf;
				PosSourceX=(x1*cosphi-y1*sinphi)/Sx+x0;
				PosSourceY=(x1*sinphi+y1*cosphi)/Sy+y0;
				/*if((PosSourceX<=-1) || (PosSourceX>=w) || (PosSourceY<=-1) || (PosSourceY>=h))*/ /* changed in v. 1.3 */
				if((PosSourceX<0) || (PosSourceX>w-1) || (PosSourceY<0) || (PosSourceY>h-1))
				{
					k_SetPixel1bp(x,y,Background,(*pTarget)); /* just use background as color */
				}
				else
				{
					/*k_SetPixel1bp(x,y,(int)k_GetPixelInterpolatedFast(PosSourceX,PosSourceY,pSource),(*pTarget));*/
					k_SetPixel1bp(x,y,(int)k_GetPixelInterpolatedFast((FLOAT32)((x1*cosphi-y1*sinphi)/Sx+x0),(FLOAT32)((x1*sinphi+y1*cosphi)/Sy+y0),pSource),(*pTarget));
				}
			}
		}
	}

	if (CopyToSource==true)
	{
		k_CopyImage(pDest,pTarget);
		k_EmptyImage(pTarget);
		free(pTarget);
	}
	else if (ResetBorder==true)
		k_SetBorderSize(0,pSource);

#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
	return true;
}

bool k_ScaleAuto(TImage* pSource,TImage* pDest,double Sx, double Sy,UINT32 Background)
{
	int x,y,w,h;
	TImage* pTarget=pDest;
	bool CopyToSource=false;
	bool ResetBorder=false;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_ScaleAuto() Source image is empty - doing nothing");
		return false;
	}
	if (pDest==pSource)
	{
		pTarget=(TImage*) malloc(sizeof(TImage));
		k_InitImage(pTarget);
		CopyToSource=true;
	}
	if ((pSource->Bits!=1) && (pSource->Bits!=8) && (pSource->Bits!=24))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_ScaleAuto() Number of bits per pixel=%d not supported",
			pSource->Bits);
		return false;
	}
	w=(int)(Sx*pSource->Width);
	h=(int)(Sy*pSource->Height);
	/* prepare destination image, following lines faster than calling k_CopyImage() */
	k_AllocImage(w,h,pSource->Bits,pTarget);
	k_CopyPalette(&pTarget->Pal,&pSource->Pal);
	k_CopyFileInfo(&(pTarget->FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
	/* copy history before adding comment */
	k_CopyText(&(pTarget->History),&(pSource->History));
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&pTarget->History,
			"%sk_ScaleAuto(Sx=%g,Sy=%g,Background=%d)",
			ipl_HistoryIndent, Sx, Sy, Background);
		k_IplStartHistoryMacro();
	}
#endif
	if (pSource->BorderSize==0)
	{
		k_SetBorder(1,Background,pSource);
		ResetBorder=true;
	}

	if (pSource->Bits==24)
	{
		for (x=0;x<w;x++)
			for (y=0;y<h;y++)
				k_SetPixel24bp(x,y,k_GetColorInterpolatedFast((FLOAT32)(x/Sx),(FLOAT32)(y/Sy),pSource),(*pTarget));
				/*k_SetPixelFast(x,y,k_GetColorInterpolatedFast(x/Sx,y/Sy,pSource),pTarget);*/
	}
	else if (pSource->Bits==8)
	{
		for (x=0;x<w;x++)
			for (y=0;y<h;y++)
				k_SetPixel8bp(x,y,(int)(k_GetPixelInterpolatedFast((FLOAT32)(x/Sx),(FLOAT32)(y/Sy),pSource)+0.5),(*pTarget));
				/*k_SetPixelFast(x,y,(int)(k_GetPixelInterpolatedFast(x/Sx,y/Sy,pSource)+0.5),pTarget);*/
	}
	else if (pSource->Bits==1)
	{
		for (x=0;x<w;x++)
			for (y=0;y<h;y++)
				k_SetPixel1bp(x,y,(int)(k_GetPixelInterpolatedFast((FLOAT32)(x/Sx),(FLOAT32)(y/Sy),pSource)+0.5),(*pTarget));
	}

	if (CopyToSource==true)
	{
		k_CopyImage(pDest,pTarget);
		k_EmptyImage(pTarget);
		free(pTarget);
	}
	else if (ResetBorder==true)
		k_SetBorderSize(0,pSource);

#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
	return true;
}

bool k_ScaleFixed(TImage* pSource,TImage* pDest,double x0,double y0,
				  int Width,int Height,double Sx, double Sy,UINT32 Background)
{
	int x,y;
	int wHalf=Width/2, hHalf=Height/2;
	int PosSourceX;
	int PosSourceY;
	TImage* pTarget=pDest;
	bool CopyToSource=false;
	bool ResetBorder=false;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_ScaleFixed() Source image is empty - doing nothing");
		return false;
	}
	if (pDest==pSource)
	{
		pTarget=(TImage*) malloc(sizeof(TImage));
		k_InitImage(pTarget);
		CopyToSource=true;
	}
	if ((pSource->Bits!=8) && (pSource->Bits!=24))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_ScaleFixed() Number of bits per pixel=%d not supported",
			pSource->Bits);
		return false;
	}
	/* prepare destination image, following lines faster than calling k_CopyImage() */
	k_AllocImage(Width,Height,pSource->Bits,pTarget);
	k_CopyPalette(&pTarget->Pal,&pSource->Pal);
	k_CopyFileInfo(&(pTarget->FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
	/* copy history before adding comment */
	k_CopyText(&(pTarget->History),&(pSource->History));
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(pTarget->History),
			"%sk_ScaleFixed(x0=%g,y0=%g,Width=%d,Height=%d,"
			"Sx=%g,Sy=%g,Background=%d)",
			ipl_HistoryIndent, x0, y0, Width,
			Height, Sx, Sy, Background);
		k_IplStartHistoryMacro();
	}
#endif
	if (pSource->BorderSize==0)
	{
		k_SetBorder(1,Background,pSource);
		ResetBorder=true;
	}

	if (pSource->Bits==24)
	{
		for (x=0;x<Width;x++)
		{
			for (y=0;y<Height;y++)
			{
				PosSourceX=(int)((x-wHalf)/Sx+x0);
				PosSourceY=(int)((y-hHalf)/Sy+y0);
				if((PosSourceX<=-1) || (PosSourceX>=(int)pSource->Width) || (PosSourceY<=-1) || (PosSourceY>=(int)pSource->Height))
				{
					k_SetPixel24bp(x,y,Background,(*pTarget)); /* just use background as color */
				}
				else
				{
					k_SetPixel24bp(x,y,k_GetColorInterpolatedFast((FLOAT32)((x-wHalf)/Sx+x0),(FLOAT32)((y-hHalf)/Sy+y0),pSource),(*pTarget));
				}
			}
		}
	}
	else if (pSource->Bits==8)
	{
		for (x=0;x<Width;x++)
		{
			for (y=0;y<Height;y++)
			{
				PosSourceX=(int)((x-wHalf)/Sx+x0);
				PosSourceY=(int)((y-hHalf)/Sy+y0);
				if((PosSourceX<=-1) || (PosSourceX>=(int)pSource->Width) || (PosSourceY<=-1) || (PosSourceY>=(int)pSource->Height))
				{
					k_SetPixel8bp(x,y,Background,(*pTarget)); /* just use background as color */
				}
				else
				{
					/*k_SetPixel8bp(x,y,(int)k_GetPixelInterpolatedFast(PosSourceX,PosSourceY,pSource),(*pTarget));*/
					k_SetPixel8bp(x,y,(int)k_GetPixelInterpolatedFast((FLOAT32)((x-wHalf)/Sx+x0),(FLOAT32)((y-hHalf)/Sy+y0),pSource),(*pTarget));
				}
			}
		}
	}

	if (CopyToSource==true)
	{
		k_CopyImage(pDest,pTarget);
		k_EmptyImage(pTarget);
		free(pTarget);
	}
	else if (ResetBorder==true)
		k_SetBorderSize(0,pSource);

#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
	return true;
}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
