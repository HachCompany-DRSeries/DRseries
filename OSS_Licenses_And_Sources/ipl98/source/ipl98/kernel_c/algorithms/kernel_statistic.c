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

#include "kernel_statistic.h"
#include "../kernel_error.h"
#include <math.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

void k_InitStatistic(TStatistic* pStatistic)
{
	pStatistic->Min=0.0;
	pStatistic->Max=0.0;
	pStatistic->Mean=0.0;
	pStatistic->StdDev=0.0;
	pStatistic->SizeOfHistogram=0;
	pStatistic->pHistogram=NULL;
}

void k_EmptyStatistic(TStatistic* pStatistic)
{
	pStatistic->Min=0.0;
	pStatistic->Max=0.0;
	pStatistic->Mean=0.0;
	pStatistic->StdDev=0.0;
	if (pStatistic->pHistogram!=NULL)
	{
		if (pStatistic->SizeOfHistogram==65536)
		{
			/* if it is a IntImage set the pointer back to beginning of array! */
			pStatistic->pHistogram-=32768;
		}
		free(pStatistic->pHistogram);
		pStatistic->pHistogram=NULL;
	}
	pStatistic->SizeOfHistogram=0;
}

bool k_DeriveFloatImageStats(TStatistic* pStatistic,const TFloatImage* pImage)
{
	if (pImage->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_DeriveFloatImageStats() Image is empty");
		return false;
	}
	else
	{
		unsigned int x,y;
		FLOAT32 Value;
		double Sum=0.0;
		double Sum2=0.0;
		pStatistic->SizeOfHistogram=0; /* no histogram */
		pStatistic->Min=pStatistic->Max=kF_GetPixelMacro(0,0,*pImage); /* init min and max */
		for(x=0;x<pImage->Width;x++)
		{
			for(y=0;y<pImage->Height;y++)
			{
				Value=kF_GetPixelMacro(x,y,*pImage);
				if (Value<pStatistic->Min) pStatistic->Min=Value;
				else if (Value>pStatistic->Max) pStatistic->Max=Value;
				Sum+=Value;
				Sum2+=Value*Value;
			}
		}
		pStatistic->Mean=Sum/(pImage->Width*pImage->Height);
		pStatistic->StdDev=sqrt(Sum2/(pImage->Width*pImage->Height)-pStatistic->Mean*pStatistic->Mean);
		return true;
	}
}

bool k_DeriveIntImageStats(TStatistic* pStatistic,const TIntImage* pImage)
{
	if (pImage->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_DeriveIntImageStats() Image is empty");
		return false;
	}
	else
	{
		unsigned int x,y;
		INT16 Value;
		double Sum=0.0;
		double Sum2=0.0;
		pStatistic->pHistogram=(UINT32*)calloc(65536,sizeof(UINT32));
		pStatistic->pHistogram+=32768; /* set the histogram pointer to the middle of array! */
		pStatistic->SizeOfHistogram=65536;
		pStatistic->Min=pStatistic->Max=kI_GetPixelMacro(0,0,*pImage); /* init min and max */
		for(x=0;x<pImage->Width;x++)
		{
			for(y=0;y<pImage->Height;y++)
			{
				Value=kI_GetPixelMacro(x,y,*pImage);
				if (Value<pStatistic->Min) pStatistic->Min=Value;
				else if (Value>pStatistic->Max) pStatistic->Max=Value;
				Sum+=Value;
				Sum2+=Value*Value;
				pStatistic->pHistogram[Value]++;
			}
		}
		pStatistic->Mean=Sum/(pImage->Width*pImage->Height);
		pStatistic->StdDev=sqrt(Sum2/(pImage->Width*pImage->Height)-pStatistic->Mean*pStatistic->Mean);
		return true;
	}
}

bool k_DeriveImageStats(TStatistic* pStatistic,const TImage* pImage)
{
	unsigned int x,y;
	UINT32 Value;
	double Sum=0.0;
	double Sum2=0.0;
	if (pImage->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_DeriveImageStats() Image is empty");
		return false;
	}
	/* allocate histogram */
	if (pImage->Bits==1)
	{
		pStatistic->pHistogram=(UINT32*)calloc(2,sizeof(UINT32));
		pStatistic->SizeOfHistogram=2;
		pStatistic->Min=pStatistic->Max=k_GetPixel1bp(0,0,*pImage); /* init min and max */
		for(x=0;x<pImage->Width;x++)
		{
			for(y=0;y<pImage->Height;y++)
			{
				Value=k_GetPixel1bp(x,y,*pImage);
				if (Value<pStatistic->Min) pStatistic->Min=Value;
				else if (Value>pStatistic->Max) pStatistic->Max=Value;
				Sum+=Value;
				Sum2+=Value*Value;
				pStatistic->pHistogram[Value]++;
			}
		}
		pStatistic->Mean=Sum/(pImage->Width*pImage->Height);
		pStatistic->StdDev=sqrt(Sum2/(pImage->Width*pImage->Height)-pStatistic->Mean*pStatistic->Mean);
		return true;

	}
	else /* either 8 or 24 b/p */
	{
		pStatistic->pHistogram=(UINT32*)calloc(256,sizeof(UINT32));
		pStatistic->SizeOfHistogram=256;
		pStatistic->Min=pStatistic->Max=k_GetPixel8bp(0,0,*pImage); /* init min and max */
		if (pImage->Bits==8)
		{
			pStatistic->Min=pStatistic->Max=k_GetPixel8bp(0,0,*pImage); /* init min and max */
			for(x=0;x<pImage->Width;x++)
			{
				for(y=0;y<pImage->Height;y++)
				{
					Value=k_GetPixel8bp(x,y,*pImage);
					if (Value<pStatistic->Min) pStatistic->Min=Value;
					else if (Value>pStatistic->Max) pStatistic->Max=Value;
					Sum+=Value;
					Sum2+=Value*Value;
					pStatistic->pHistogram[Value]++;
				}
			}
			pStatistic->Mean=Sum/(pImage->Width*pImage->Height);
			pStatistic->StdDev=sqrt(Sum2/(pImage->Width*pImage->Height)-pStatistic->Mean*pStatistic->Mean);
			return true;
		}
		else if (pImage->Bits==24)
		{
			Value=k_GetPixel24bp(0,0,*pImage);
			pStatistic->Min=pStatistic->Max=(int)((k_PalGetRVal(Value)+k_PalGetGVal(Value)+k_PalGetBVal(Value))/3+0.5);
			for(x=0;x<pImage->Width;x++)
			{
				for(y=0;y<pImage->Height;y++)
				{
					Value=k_GetPixel24bp(x,y,*pImage);
					Value=(int)((k_PalGetRVal(Value)+k_PalGetGVal(Value)+k_PalGetBVal(Value))/3+0.5);
					if (Value<pStatistic->Min) pStatistic->Min=Value;
					else if (Value>pStatistic->Max) pStatistic->Max=Value;
					Sum+=Value;
					Sum2+=Value*Value;
					pStatistic->pHistogram[Value]++;
				}
			}
			pStatistic->Mean=Sum/(pImage->Width*pImage->Height);
			pStatistic->StdDev=sqrt(Sum2/(pImage->Width*pImage->Height)-pStatistic->Mean*pStatistic->Mean);
			return true;
		}
		else
		{
			k_ShowMessageStd(
				IPL_POSITION, IPL_ERROR,
				"k_DeriveImageStats() Image with %d b/p not supported",
				pImage->Bits);
			return false;
		}
	}
}

bool k_DerivePixelGroupStats(TStatistic* pStatistic,const TPixelGroup* pPixelGroup)
{
	if (pPixelGroup->NumberOfPixels==0)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_DerivePixelGroupStats() No positions available");
		return false;
	}
	else if (k_ColorsAvailableGroup(pPixelGroup)==false)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_DerivePixelGroupStats() No color values available");
		return false;
	}
	else
	{
		unsigned int x;
		UINT32 Value;
		double Sum=0.0;
		double Sum2=0.0;
		pStatistic->SizeOfHistogram=0; /* no histogram */
		pStatistic->Min=pStatistic->Max=pPixelGroup->pColor[0]; /* init min and max */
		for(x=0;x<pPixelGroup->NumberOfPixels;x++)
		{
			Value=pPixelGroup->pColor[x];
			if (Value<pStatistic->Min) pStatistic->Min=Value;
			else if (Value>pStatistic->Max) pStatistic->Max=Value;
			Sum+=Value;
			Sum2+=Value*Value;
		}
		pStatistic->Mean=Sum/pPixelGroup->NumberOfPixels;
		pStatistic->StdDev=sqrt(Sum2/pPixelGroup->NumberOfPixels-pStatistic->Mean*pStatistic->Mean);
		return true;
	}
}

void k_PrintStatisticInfo(const TStatistic* pInfo)
{
	printf("************** Statistic info ******************\n");
	printf(" Min=% .g Max=% .g Mean=% .g Standard deviation=% .g\n",
		pInfo->Min,pInfo->Max,pInfo->Mean,pInfo->StdDev);
	printf(" Size of histogram=%d\n",pInfo->SizeOfHistogram);
}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
