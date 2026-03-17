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

#include "kernel_palette.h"
#include "kernel_string.h"
#include "kernel_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

void k_InitPalette(TPalette* pPal)
{
	pPal->pPalette=NULL;
	pPal->PalEntries=0;
}

void k_EmptyPalette(TPalette* pPal)
{
	if (pPal->pPalette!=NULL)
	{
		free(pPal->pPalette);
		pPal->pPalette=NULL;
	}
	pPal->PalEntries=0;
}

bool k_CreatePal(unsigned short Bits,TPalette* pPal)
{
	/* allocates memory for palette and initialises to gray tones */
	UINT16 count;
	switch(Bits){
		case 1:
			pPal->PalEntries=2;
			pPal->pPalette=(UINT32*) malloc(sizeof(UINT32)*2);
			/* Create black and white palette */
			k_SetPalValue(0,k_PalCreateRGB(0,0,0),pPal);
			k_SetPalValue(1,k_PalCreateRGB(255,255,255),pPal);
			return true;
		case 4:
			pPal->PalEntries=16;
			pPal->pPalette=(UINT32*) malloc(sizeof(UINT32)*16);
			/* Create gray tone palette */
			for(count=0; count<16; count++){
				k_SetPalValue(count,k_PalCreateRGB(count*17,count*17,count*17),
					pPal);
			}
			return true;
		case 8:
			pPal->PalEntries=256;
			pPal->pPalette=(UINT32*) malloc(sizeof(UINT32)*256);
			/* Create gray tone palette */
			for(count=0; count<256; count++){
				k_SetPalValue(count,k_PalCreateRGB(count,count,count),pPal);
			}
			return true;
		case 24:
			pPal->PalEntries=0; /* no palette for 24 b/p images */
			return true;
		default:
			{
				k_ShowMessageSimple(IPL_ERROR, "k_CreatePal() Only supports 1,8 and 24 bit per pixels");
				return false;
			}
	}
}

bool k_GetPalValue(int Index, UINT32* pPalValue, const TPalette* pPal)
{
	if (Index<pPal->PalEntries)
	{
		*pPalValue=*(pPal->pPalette+Index);
		return true;
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_GetPalValue() Index=%d outside range of palette",
			Index);
		return false;
	}
}

UINT32 k_GetPalColor(int Index, const TPalette* pPal)
{
	if (Index<pPal->PalEntries)
	{
		return *(pPal->pPalette+Index);
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_GetPalColor() Index=%d outside range of palette",
			Index);
		return false;
	}
}

bool k_SetPalValue(int Index, UINT32 PalValue, TPalette* pPal)
{
	if (Index<pPal->PalEntries)
	{
		*(pPal->pPalette+Index)=PalValue;
		return true;
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_SetPalValue() Index=%d outside range of palette",
			Index);
		return false;
	}
}

bool k_CopyPalette(TPalette* pDest,const TPalette* pSource)
{
	if (pDest==pSource)
	{
		/* cannot copy to itself! */
		k_ShowMessageSimple(IPL_ERROR, "k_CopyPalette() Source and destination palette"
			"the same - doing nothing");
		return false;
	}
	if (pDest->PalEntries!=pSource->PalEntries)
	{
		k_EmptyPalette(pDest);
		pDest->PalEntries=pSource->PalEntries;
		pDest->pPalette=(UINT32*) malloc(sizeof(UINT32)*pDest->PalEntries);
	}
	if(pSource->PalEntries>0)
	{
		memcpy(pDest->pPalette,pSource->pPalette,sizeof(UINT32)*pSource->PalEntries);
	}
	return true;
}

void k_PrintPaletteInfo(const TPalette* pPal,unsigned int EntriesPrLine)
{
	unsigned int count1,count2;
	UINT32 PalValue;
	printf("*************** Palette info *******************\n");
	printf(" Total entries: %d\n",pPal->PalEntries);
	printf(" Values: Index=\"R G B\",\n");
	for(count1=0; count1<pPal->PalEntries/EntriesPrLine; count1++)
	{
		for(count2=0; count2<EntriesPrLine; count2++)
		{
			PalValue=pPal->pPalette[count1*EntriesPrLine+count2];
			printf("%d=\"%d %d %d\"  ",count1*EntriesPrLine+count2,k_PalGetRVal(PalValue),
				k_PalGetGVal(PalValue),k_PalGetBVal(PalValue));
		}
		printf("\n");
	}
}

bool k_IsPaletteGrayTone(const TPalette* pPal)
{
	if (pPal->PalEntries==0)
		return false;
	if (pPal->PalEntries==2)
	{
		UINT32* PalValue=pPal->pPalette;
		if ((k_PalGetRVal(*PalValue)!=0)  ||
			(k_PalGetGVal(*PalValue)!=0)  ||
			(k_PalGetBVal(*PalValue)!=0))
		{
			return false;
		}
		PalValue=pPal->pPalette+1;
		if ((k_PalGetRVal(*PalValue)!=255)  ||
			(k_PalGetGVal(*PalValue)!=255)  ||
			(k_PalGetBVal(*PalValue)!=255))
		{
			return false;
		}
		return true;
	}
	else if (pPal->PalEntries==256)
	{
		UINT16  Count;
		for(Count=0; Count<pPal->PalEntries; Count++)
		{
			UINT32* PalValue=pPal->pPalette+Count;
			if ((k_PalGetRVal(*PalValue)!=Count)  ||
				(k_PalGetGVal(*PalValue)!=Count)  ||
				(k_PalGetBVal(*PalValue)!=Count))
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_IsPaletteGrayTone() Number of palette intries is %d (must be 2 or 256)",
			pPal->PalEntries);
		return false;
	}
}

void k_PrintPalValue(UINT32 RGB)
{
	printf("Red=%d Green=%d Blue=%d\n",
		k_PalGetRVal(RGB),k_PalGetGVal(RGB),k_PalGetBVal(RGB));
}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
