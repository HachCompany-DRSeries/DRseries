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

#include "kernel_pixelgroup.h"
#include "kernel_error.h"
#include <stdlib.h>
#include <string.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/************************************************/
/********        Private functions       ********/
/************************************************/

/** Checks if the boundaries stored in the pixelgroup have to be updated when
	removing a position at given Index. Internal use only.
	@param Index The Index in the pixelgroup to be removed. Must be in range! */
void k_UpdateBoundaries(T2DInt RemovedPos,TPixelGroup* pPixelGroup);

/** Same as k_AddGroupToImage and k_AddGroupColorsToImage but without range check, called by k_AddGroupToImage 
	and k_AddGroupColorsToImage if range check is not needed.
	@param pColor If NULL the pPixelGroup color values are used - the pixelgroup MUST contain
		colors! If not null the value is used as color. */
bool k_AddGroupToImageFast(TImage* pDest, const TPixelGroup* pPixelGroup, const UINT32* pColor);
bool k_AddGroupToComplexImageFast(TComplexImage* pDest,const TPixelGroup* pPixelGroup,FLOAT32 Value);
bool k_AddGroupToFloatImageFast(TFloatImage* pDest, const TPixelGroup* pPixelGroup, FLOAT32 Value);
bool k_AddGroupToIntImageFast(TIntImage* pDest, const TPixelGroup* pPixelGroup, INT16 Value);


/************************************************/
/********        Public functions        ********/
/************************************************/

void k_InitGroup(TPixelGroup* pPixelGroup)
{
	pPixelGroup->Top.x=0;
	pPixelGroup->Top.y=32767; /* makes sure the border values are set at first k_AddPosition() call */
	pPixelGroup->Bottom.x=0;
	pPixelGroup->Bottom.y=-32768; /* makes sure the border values are set at first k_AddPosition() call */
	pPixelGroup->Left.x=32767; /* makes sure the border values are set at first k_AddPosition() call */
	pPixelGroup->Left.y=0;
	pPixelGroup->Right.x=-32768; /* makes sure the border values are set at first k_AddPosition() call */
	pPixelGroup->Right.y=0;
	pPixelGroup->Color=UNDEFINED;
	pPixelGroup->pPos=NULL;
	pPixelGroup->pColor=NULL;
	pPixelGroup->NumberOfPixels=0;
	pPixelGroup->AllocatedPixels=0; /* internal use */
}

/*	Deletes all allocated memory for PixelGroup */
void k_EmptyGroup(TPixelGroup* pPixelGroup)
{
	if (pPixelGroup->pPos!=NULL)
		free(pPixelGroup->pPos);
	if (pPixelGroup->pColor!=NULL)
		free(pPixelGroup->pColor);
	k_InitGroup(pPixelGroup);
}

void k_EmptyGroupNoDeallocation(TPixelGroup* pPixelGroup)
{
	pPixelGroup->Top.x=0;
	pPixelGroup->Top.y=32767; /* makes sure the border values are set at first k_AddPosition() call */
	pPixelGroup->Bottom.x=0;
	pPixelGroup->Bottom.y=-32768; /* makes sure the border values are set at first k_AddPosition() call */
	pPixelGroup->Left.x=32767; /* makes sure the border values are set at first k_AddPosition() call */
	pPixelGroup->Left.y=0;
	pPixelGroup->Right.x=-32768; /* makes sure the border values are set at first k_AddPosition() call */
	pPixelGroup->Right.y=0;
	pPixelGroup->Color=UNDEFINED;
	/* Set total pixels to 0 but no deallocation of positions and colors (if available) */
	pPixelGroup->NumberOfPixels=0;
}

void k_AllocGroup(TPixelGroup* pPixelGroup,unsigned int InitialSize)
{
	k_EmptyGroup(pPixelGroup);
	if (InitialSize==0)
		InitialSize=1; /* minimum one allocated position */
	pPixelGroup->pPos=(T2DInt*)malloc(InitialSize*sizeof(T2DInt));
	pPixelGroup->NumberOfPixels=0; /* just in case! */
	pPixelGroup->AllocatedPixels=InitialSize; /* internal use */
}

void k_AllocGroupAndColors(TPixelGroup* pPixelGroup,unsigned int InitialSize)
{
	k_EmptyGroup(pPixelGroup);
	if (InitialSize==0)
		InitialSize=1; /* minimum one allocated position */
	pPixelGroup->pPos=(T2DInt*)malloc(InitialSize*sizeof(T2DInt));
	pPixelGroup->pColor=(UINT32*)malloc(InitialSize*sizeof(UINT32));
	pPixelGroup->NumberOfPixels=0; /* just in case! */
	pPixelGroup->AllocatedPixels=InitialSize; /* internal use */
}

void k_ReAllocGroup(TPixelGroup* pPixelGroup,unsigned int NewSize)
{
	if ((pPixelGroup->pPos=(T2DInt*)realloc(pPixelGroup->pPos,NewSize*sizeof(T2DInt)))!=NULL)
	{
		pPixelGroup->AllocatedPixels=NewSize; /* internal use */
		/* check if colors are available and realloc if yes */
		if (pPixelGroup->pColor!=NULL)
		{
			if ((pPixelGroup->pColor=(UINT32*)realloc(pPixelGroup->pColor,NewSize*sizeof(UINT32)))==NULL)
			{
				k_ShowMessageSimple(IPL_ERROR, "k_ReAllocGroup() Realloc colors failed");
			}
		}
	}
	else
	{
		k_ShowMessageSimple(IPL_ERROR, "k_ReAllocGroup() Realloc of positions failed");
	}
}

void k_AdjustSizeOfGroup(TPixelGroup* pPixelGroup)
{
	pPixelGroup->pPos=(T2DInt*)realloc(pPixelGroup->pPos,pPixelGroup->NumberOfPixels*sizeof(T2DInt));
	pPixelGroup->AllocatedPixels=pPixelGroup->NumberOfPixels;
	if (pPixelGroup->pColor!=NULL)
	{
		pPixelGroup->pColor=(UINT32*)realloc(pPixelGroup->pColor,pPixelGroup->NumberOfPixels*sizeof(UINT32));
	}
}

bool k_AddPosToGroup(INT16 x, INT16 y, TPixelGroup* pPixelGroup)
{
	if (pPixelGroup->pColor!=NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_AddPosToGroup() Colors available, cannot insert position only");
		return false;
	}
	else
	{
		T2DInt p;
		p.x=x;
		p.y=y;
		if (pPixelGroup->NumberOfPixels==pPixelGroup->AllocatedPixels)
		{
			/* allocate 2 times previous allocated memory or 2 if memory never allocated before */
			pPixelGroup->AllocatedPixels = 
				(pPixelGroup->AllocatedPixels==0) ? 2 : 2*pPixelGroup->AllocatedPixels;
			k_ReAllocGroup(pPixelGroup,pPixelGroup->AllocatedPixels);
		}
		if (x>(pPixelGroup->Right).x)
			pPixelGroup->Right=p;
		if (x<(pPixelGroup->Left).x)
			pPixelGroup->Left=p;
		if (y>(pPixelGroup->Bottom).y)
			pPixelGroup->Bottom=p;
		if (y<(pPixelGroup->Top).y)
			pPixelGroup->Top=p;
		/* insert pixel and increment total pixels variable */
		pPixelGroup->pPos[pPixelGroup->NumberOfPixels++] = p;
		return true;
	}
}

bool k_Add2DPosToGroup(T2DInt Pos, TPixelGroup* pPixelGroup)
{
	if (pPixelGroup->pColor!=NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_Add2DPosToGroup() Colors available, cannot insert position only");
		return false;
	}
	else
	{
		if (pPixelGroup->NumberOfPixels==pPixelGroup->AllocatedPixels)
		{
			/* allocate 2 times previous allocated memory or 2 if memory never allocated before */
			pPixelGroup->AllocatedPixels = 
				(pPixelGroup->AllocatedPixels==0) ? 2 : 2*pPixelGroup->AllocatedPixels;

			k_ReAllocGroup(pPixelGroup,pPixelGroup->AllocatedPixels);
		}
		/* Update max. and min. values in pPixelGroup */
		if (Pos.x<(pPixelGroup->Left).x)
			pPixelGroup->Left=Pos;
		if (Pos.x>(pPixelGroup->Right).x)
			pPixelGroup->Right=Pos;
		if (Pos.y<(pPixelGroup->Top).y)
			pPixelGroup->Top=Pos;
		if (Pos.y>(pPixelGroup->Bottom).y)
			pPixelGroup->Bottom=Pos;
		/* insert pixel and increment total pixels variable */
		pPixelGroup->pPos[pPixelGroup->NumberOfPixels++] = Pos;
		return true;
	}
}

bool k_AddPosColorToGroup(INT16 x, INT16 y, UINT32 Color, TPixelGroup* pPixelGroup)
{
	if (pPixelGroup->pColor==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_AddPosColorToGroup() Colors not available");
		return false;
	}
	else
	{
		T2DInt p;
		p.x=x;
		p.y=y;
		if (pPixelGroup->NumberOfPixels==pPixelGroup->AllocatedPixels)
		{
			/* allocate 2 times previous allocated memory or 2 if memory never allocated before */
			pPixelGroup->AllocatedPixels = 
				(pPixelGroup->AllocatedPixels==0) ? 2 : 2*pPixelGroup->AllocatedPixels;
			k_ReAllocGroup(pPixelGroup,pPixelGroup->AllocatedPixels);
		}
		if (x>(pPixelGroup->Right).x)
			pPixelGroup->Right=p;
		if (x<(pPixelGroup->Left).x)
			pPixelGroup->Left=p;
		if (y>(pPixelGroup->Bottom).y)
			pPixelGroup->Bottom=p;
		if (y<(pPixelGroup->Top).y)
			pPixelGroup->Top=p;
		/* insert pixel and increment total pixels variable */
		pPixelGroup->pColor[pPixelGroup->NumberOfPixels] = Color;
		pPixelGroup->pPos[pPixelGroup->NumberOfPixels++] = p;
		return true;
	}
}

bool k_Add2DPosColorToGroup(T2DInt Pos, UINT32 Color, TPixelGroup* pPixelGroup)
{
	if (pPixelGroup->pColor==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_Add2DPosColorToGroup() Colors not available");
		return false;
	}
	else
	{
		if (pPixelGroup->NumberOfPixels==pPixelGroup->AllocatedPixels)
		{
			/* allocate 2 times previous allocated memory or 2 if memory never allocated before */
			pPixelGroup->AllocatedPixels = 
				(pPixelGroup->AllocatedPixels==0) ? 2 : 2*pPixelGroup->AllocatedPixels;

			k_ReAllocGroup(pPixelGroup,pPixelGroup->AllocatedPixels);
		}
		/* Update max. and min. values in pPixelGroup */
		if (Pos.x<(pPixelGroup->Left).x)
			pPixelGroup->Left=Pos;
		if (Pos.x>(pPixelGroup->Right).x)
			pPixelGroup->Right=Pos;
		if (Pos.y<(pPixelGroup->Top).y)
			pPixelGroup->Top=Pos;
		if (Pos.y>(pPixelGroup->Bottom).y)
			pPixelGroup->Bottom=Pos;
		/* insert pixel and increment total pixels variable */
		pPixelGroup->pColor[pPixelGroup->NumberOfPixels] = Color;
		pPixelGroup->pPos[pPixelGroup->NumberOfPixels++] = Pos;
		return true;
	}
}

UINT32 k_GetColorGroup(unsigned int Index, const TPixelGroup* pPixelGroup)
{	/* C only version, C++ has its own implementation to increase speed */
	if ((pPixelGroup->pColor!=NULL) && (Index<pPixelGroup->NumberOfPixels))
		return pPixelGroup->pColor[Index];
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_GetColorGroup() Index(%d) out of range or colors not available",
			Index);
		return 0;
	}
}

bool k_SetColorGroup(UINT32 ColorValue, unsigned int Index, TPixelGroup* pPixelGroup)
{	/* C only version, C++ has its own implementation to increase speed */
	if ((pPixelGroup->pColor!=NULL) && (Index<pPixelGroup->NumberOfPixels))
	{
		pPixelGroup->pColor[Index]=ColorValue;
		return true;
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_SetColorGroup() Index(%d) out of range or colors not available",
			Index);
		return false;
	}
}

bool k_ColorsAvailableGroup(const TPixelGroup* pPixelGroup)
{
	if (pPixelGroup->pColor!=NULL)
		return true;
	else
		return false;
}

bool k_InsertPosInGroup(INT16 x, INT16 y, unsigned int Index, TPixelGroup* pPixelGroup)
{
	if (Index>=pPixelGroup->NumberOfPixels)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_InsertPosInGroup() Index (%d) is out of range (NumberOfPixels=%d)",
			Index, pPixelGroup->NumberOfPixels);
		return false;
	}
	if (pPixelGroup->pColor!=NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_InsertPosInGroup() Colors are present - failed inserting");
		return false;
	}
	/* Add a dummy position to the end of position array */
	k_Add2DPosToGroup(pPixelGroup->pPos[0],pPixelGroup);
	/* Move positions */
	memmove(&(pPixelGroup->pPos[Index+1]),&(pPixelGroup->pPos[Index]),
		sizeof(T2DInt)*(pPixelGroup->NumberOfPixels-Index-1));
	/* Store the new position at Index */
	pPixelGroup->pPos[Index].x=x;
	pPixelGroup->pPos[Index].y=y;
	return true;
}

bool k_Insert2DPosInGroup(T2DInt Pos, unsigned int Index, TPixelGroup* pPixelGroup)
{
	if (Index>=pPixelGroup->NumberOfPixels)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_Insert2DPosInGroup() Index (%d) is out of range (NumberOfPixels=%d)",
			Index, pPixelGroup->NumberOfPixels);
		return false;
	}
	if (pPixelGroup->pColor!=NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_InsertPosInGroup() Colors are present - failed inserting");
		return false;
	}
	/* Add a dummy position to the end of position array */
	k_Add2DPosToGroup(pPixelGroup->pPos[0],pPixelGroup);
	/* Move positions */
	memmove(&(pPixelGroup->pPos[Index+1]),&(pPixelGroup->pPos[Index]),
		sizeof(T2DInt)*(pPixelGroup->NumberOfPixels-Index-1));
	/* Store the new position at Index */
	pPixelGroup->pPos[Index]=Pos;
	return true;
}

bool k_InsertPosColorInGroup(INT16 x, INT16 y, UINT32 Color, unsigned int Index, TPixelGroup* pPixelGroup)
{
	if (Index>=pPixelGroup->NumberOfPixels)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_InsertPosColorInGroup() Index (%d) is out of range (NumberOfPixels=%d)",
			Index, pPixelGroup->NumberOfPixels);
		return false;
	}
	if (pPixelGroup->pColor==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_InsertPosInGroup() Colors not available - failed inserting");
		return false;
	}
	/* Add a dummy position to the end of position array */
	k_Add2DPosColorToGroup(pPixelGroup->pPos[0],0,pPixelGroup);
	/* Move positions */
	memmove(&(pPixelGroup->pPos[Index+1]),&(pPixelGroup->pPos[Index]),
		sizeof(T2DInt)*(pPixelGroup->NumberOfPixels-Index-1));
	/* Move colors */
	memmove(&(pPixelGroup->pColor[Index+1]),&(pPixelGroup->pColor[Index]),
		sizeof(UINT32)*(pPixelGroup->NumberOfPixels-Index-1));
	/* Store the new position at Index */
	pPixelGroup->pPos[Index].x=x;
	pPixelGroup->pPos[Index].y=y;
	/* Store new color */
	pPixelGroup->pColor[Index]=Color;
	return true;
}

bool k_Insert2DPosColorInGroup(T2DInt Pos, UINT32 Color, unsigned int Index, TPixelGroup* pPixelGroup)
{
	if (Index>=pPixelGroup->NumberOfPixels)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_Insert2DPosColorInGroup() Index (%d) is out of range (NumberOfPixels=%d)",
			Index, pPixelGroup->NumberOfPixels);
		return false;
	}
	if (pPixelGroup->pColor==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_Insert2DPosInGroup() Colors not available - failed inserting");
		return false;
	}
	/* Add a dummy position to the end of position array */
	k_Add2DPosColorToGroup(pPixelGroup->pPos[0],0,pPixelGroup);
	/* Move positions */
	memmove(&(pPixelGroup->pPos[Index+1]),&(pPixelGroup->pPos[Index]),
		sizeof(T2DInt)*(pPixelGroup->NumberOfPixels-Index-1));
	/* Move colors */
	memmove(&(pPixelGroup->pColor[Index+1]),&(pPixelGroup->pColor[Index]),
		sizeof(UINT32)*(pPixelGroup->NumberOfPixels-Index-1));
	/* Store the new position at Index */
	pPixelGroup->pPos[Index]=Pos;
	/* Store new color */
	pPixelGroup->pColor[Index]=Color;
	return true;
}

bool k_RemovePosFromGroup(unsigned int Index, TPixelGroup* pPixelGroup)
{
	T2DInt RemovedPos;
	if (Index>=pPixelGroup->NumberOfPixels)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_RemovePosFromGroup() Index (%d) is out of range (NumberOfPixels=%d)",
			Index, pPixelGroup->NumberOfPixels);
		return false;
	}
	RemovedPos=pPixelGroup->pPos[Index];
	/* Move last position to Index */
	pPixelGroup->pPos[Index]=pPixelGroup->pPos[pPixelGroup->NumberOfPixels-1];
	/* If colors are present move the corresponding color */
	if (pPixelGroup->pColor!=NULL)
	{
		pPixelGroup->pColor[Index]=pPixelGroup->pColor[pPixelGroup->NumberOfPixels-1];
	}
	pPixelGroup->NumberOfPixels--;
	/* Check if the position removed will affect the boundaries of the pixel group */
	k_UpdateBoundaries(RemovedPos,pPixelGroup);
	return true;
}

bool k_RemovePosFromGroupSlow(unsigned int Index, TPixelGroup* pPixelGroup)
{
	T2DInt RemovedPos;
	if (Index>=pPixelGroup->NumberOfPixels)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_RemovePosFromGroupSlow() Index (%d) is out of range (NumberOfPixels=%d)",
			Index, pPixelGroup->NumberOfPixels);
		return false;
	}
	RemovedPos=pPixelGroup->pPos[Index];
	/* If the Index is the last position in group no movement */
	if (Index<(pPixelGroup->NumberOfPixels-1))
	{
		/* Move positions */
		memmove(&(pPixelGroup->pPos[Index]),&(pPixelGroup->pPos[Index+1]),
			sizeof(T2DInt)*(pPixelGroup->NumberOfPixels-Index-1));
		if (pPixelGroup->pColor!=NULL)
		{
			/* Move colors */
			memmove(&(pPixelGroup->pColor[Index]),&(pPixelGroup->pColor[Index+1]),
				sizeof(UINT32)*(pPixelGroup->NumberOfPixels-Index-1));
		}
	}
	pPixelGroup->NumberOfPixels--;
	/* Check if the position removed will affect the boundaries of the pixel group */
	k_UpdateBoundaries(RemovedPos,pPixelGroup);
	return true;
}

unsigned int k_RemoveDuplicatesFromGroup(TPixelGroup* pPixelGroup)
{
	unsigned int count1=0,count2;
	unsigned int TotalPositions=pPixelGroup->NumberOfPixels;
	unsigned PosRemoved=0;
	while(count1<TotalPositions)
	{
		count2=count1+1;
		while(count2<TotalPositions)
		{
			if ((pPixelGroup->pPos[count2].x==pPixelGroup->pPos[count1].x) &&
				(pPixelGroup->pPos[count2].y==pPixelGroup->pPos[count1].y))
			{
				/* Move last position to Index */
				pPixelGroup->pPos[count2]=pPixelGroup->pPos[pPixelGroup->NumberOfPixels-1];
				/* If colors are present move the corresponding color */
				if (pPixelGroup->pColor!=NULL)
				{
					pPixelGroup->pColor[count2]=pPixelGroup->pColor[pPixelGroup->NumberOfPixels-1];
				}
				pPixelGroup->NumberOfPixels--;
				PosRemoved++;
			}
			else
			{
				count2++;
			}
			TotalPositions=pPixelGroup->NumberOfPixels;
		}
		count1++;
	}
	return PosRemoved;
}

bool k_AllocColorsGroup(TPixelGroup* pPixelGroup)
{
	if (pPixelGroup->pColor==NULL)
	{
		pPixelGroup->pColor=(UINT32*)malloc(pPixelGroup->AllocatedPixels*sizeof(UINT32));
		return true;
	}
	else
	{
		k_ShowMessageSimple(IPL_ERROR, "k_AllocColorsGroup() PixelGroup already contains colors");
		return false;
	}
}

bool k_AllocAndInitColorsGroup(UINT32 InitialValue,TPixelGroup* pPixelGroup)
{
	if (k_AllocColorsGroup(pPixelGroup)==true)
	{
		memset(pPixelGroup->pColor,InitialValue,pPixelGroup->AllocatedPixels);
		return true;
	}
	else
	{
		k_ShowMessageSimple(IPL_ERROR, "k_AllocAndInitColorsGroup() PixelGroup already contains colors");
		return false;
	}
}

void k_CopyGroup(TPixelGroup* pDest, const TPixelGroup* pSource)
{
	k_EmptyGroup(pDest);
	pDest->Top=pSource->Top;
	pDest->Bottom=pSource->Bottom;
	pDest->Left=pSource->Left;
	pDest->Right=pSource->Right;
	pDest->Color=pSource->Color;
	pDest->Bits=pSource->Bits;
	pDest->NumberOfPixels=pSource->NumberOfPixels;
	pDest->AllocatedPixels=pSource->AllocatedPixels;
	/* allocate memory for positions and copy positions */
	pDest->pPos=(T2DInt*) malloc(pSource->AllocatedPixels*sizeof(T2DInt));
	memcpy(pDest->pPos,pSource->pPos,pSource->NumberOfPixels*sizeof(T2DInt));
	if (pSource->pColor!=NULL) /* only copy if colors are available */
	{
		/* allocate memory for colors and copy colors */
		pDest->pColor=(UINT32*) malloc(pSource->NumberOfPixels*sizeof(UINT32));
		memcpy(pDest->pColor,pSource->pColor,pSource->NumberOfPixels*sizeof(T2DInt));
	}
}

bool k_AddColorsToGroup(const TImage* pSource,TPixelGroup* pPixelGroup)
{
	unsigned int i;
	T2DInt p;
	if ((pPixelGroup==NULL) || (pSource==NULL))
	{
		k_ShowMessageSimple(IPL_ERROR, "k_AddColorsToGroup() At least one of the parameters are NULL");
		return false;
	}
	if(pPixelGroup->NumberOfPixels==0)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_AddColorsToGroup() PixelGroup contains no pixels - doing nothing");
		return false;
	}
	if (((UINT32)((pPixelGroup->Bottom.y-pPixelGroup->Top.y+1))>pSource->Height) ||
		((UINT32)(pPixelGroup->Right.x-pPixelGroup->Left.x+1)>pSource->Width))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_AddColorsToGroup() Dimensions in PixelGroup (%d*%d) bigger than source image (%d*%d)",
			(pPixelGroup->Bottom.y - pPixelGroup->Top.y + 1),
			(pPixelGroup->Right.x - pPixelGroup->Left.x + 1),
			pSource->Width,
			pSource->Height);
		return false;
	}
	if (pPixelGroup->pColor!=NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_AddColorsToGroup() PixelGroup already contains colors");
		return false;
	}
	k_AllocColorsGroup(pPixelGroup);
	for(i=0; i<pPixelGroup->NumberOfPixels; i++)
	{
		p=pPixelGroup->pPos[i];
		pPixelGroup->pColor[i]=k_GetPixelFast(p.x,p.y,pSource);
	}
	pPixelGroup->Bits=pSource->Bits;
	return true;
}

void k_RemoveColors(TPixelGroup* pPixelGroup)
{
	if (pPixelGroup->pColor!=NULL)
	{
		free(pPixelGroup->pColor);
		pPixelGroup->pColor=NULL;
	}
}

bool k_CopyGroupToImage(const TPalette* pPalette, UINT32 BackGround, TImage* pDest, const TPixelGroup* pPixelGroup)
{
	unsigned int i;
	T2DInt r;
	if ((pDest==NULL) || (pPixelGroup==NULL))
	{
		k_ShowMessageSimple(IPL_ERROR, "k_CopyGroupToImage() pPixelGroup or pDest parameter is NULL");
		return false;
	}

	if (pPixelGroup->NumberOfPixels==0)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_CopyGroupToImage() no points stored in pPixelGroup");
		return false;
	}
	/* Case 1) No colors available */
	if (pPixelGroup->pColor==NULL)
	{
		if (pPalette!=NULL)
		{
			k_ShowMessageSimple(
				IPL_WARNING,
				"k_CopyGroupToImage() No colors available, ignoring the palette provided");
		}
		/* Allocate a binary image with correct dimensions */
		k_AllocImage((pPixelGroup->Right).x-(pPixelGroup->Left).x+1,
				(pPixelGroup->Bottom).y-(pPixelGroup->Top).y+1,1,pDest);
		pDest->Origo.x=-pPixelGroup->Left.x;
		pDest->Origo.y=-pPixelGroup->Top.y;
		/* Initialise background color */
		if (pPixelGroup->Color!=UNDEFINED)
			k_FlushImage(BackGround,pDest);
		else
			k_FlushImage(1,pDest);
			

		/* copy pixels to image */
		if (pPixelGroup->Color==HIGHCOLOR)
		{
			for (i=0;i<pPixelGroup->NumberOfPixels;i++)
			{
				r=pPixelGroup->pPos[i];
				k_SetPixelFast(r.x,r.y,1,pDest);
			}
		}
		else
		{
			for (i=0;i<pPixelGroup->NumberOfPixels;i++)
			{
				r=pPixelGroup->pPos[i];
				k_SetPixelFast(r.x,r.y,0,pDest);
			}
		}
		return true;
	}
	/* Case 2) Colors available but no palette */
	else if (pPalette==NULL) /* colors are avialable - no check */
	{
		/* Allocate an image with bit depth pPixelGroup->Bits and correct dimensions */
		k_AllocImage((pPixelGroup->Right).x-(pPixelGroup->Left).x+1,
				(pPixelGroup->Bottom).y-(pPixelGroup->Top).y+1,pPixelGroup->Bits,pDest);
		pDest->Origo.x=-pPixelGroup->Left.x;
		pDest->Origo.y=-pPixelGroup->Top.y;
		/* Initialise background color */
		k_FlushImage(BackGround,pDest);
		/* copy pixels to image */
		for (i=0;i<pPixelGroup->NumberOfPixels;i++)
		{
			r=pPixelGroup->pPos[i];
			k_SetPixelFast(r.x,r.y,pPixelGroup->pColor[i],pDest);
		}
		return true;
	}
	/* Case 3) Both colors and palette available */
	else /* colors and palette are avialable - no check */
	{
		if ((pPixelGroup->Bits==8) && (pPalette->PalEntries!=256))
		{
			k_ShowMessageStd(
				IPL_POSITION, IPL_ERROR,
				"k_CopyGroupToImage() pPixelGroup pixel depth is"
				" %d but pPalette has %d entries, should be 256",
				pPixelGroup->Bits,
				pPalette->PalEntries);
			return false;
		}
		else if ((pPixelGroup->Bits==1) && (pPalette->PalEntries!=2))
		{
			k_ShowMessageStd(
				IPL_POSITION, IPL_ERROR,
				"k_CopyGroupToImage() pPixelGroup pixel depth is %d but"
				" pPalette has %d entries, should be 2",
				pPixelGroup->Bits, pPalette->PalEntries);
			return false;
		}
		else
		{
			/* Allocate an image with bit depth pPixelGroup->Bits and correct dimensions */
			k_AllocImage((pPixelGroup->Right).x-(pPixelGroup->Left).x+1,
					(pPixelGroup->Bottom).y-(pPixelGroup->Top).y+1,pPixelGroup->Bits,pDest);
			pDest->Origo.x=-pPixelGroup->Left.x;
			pDest->Origo.y=-pPixelGroup->Top.y;
			/* Initialise background color */
			k_FlushImage(BackGround,pDest);

			if (pPixelGroup->Bits==24)
			{
				k_ShowMessageSimple(
					IPL_WARNING,
					"k_CopyGroupToImage() pPixelGroup bit depth is 24 b/p, ignoring the palette provided");
			}
			else
			{ /* copy palette to new image */
				k_CopyPalette (&(pDest->Pal),pPalette);
			}
			/* copy pixels to image */
			for (i=0;i<pPixelGroup->NumberOfPixels;i++)
			{
				r=pPixelGroup->pPos[i];
				k_SetPixelFast(r.x,r.y,pPixelGroup->pColor[i],pDest);
			}
			return true;
		}
	}
}

bool k_CopyGroupToImageFixedColors(UINT32 BackGround, UINT32 ForeGround, UINT16 Bits, TImage* pDest,
								   const TPixelGroup* pPixelGroup)
{
	unsigned int i;
	T2DInt r;
	if ((pDest==NULL) || (pPixelGroup==NULL))
	{
		k_ShowMessageSimple(IPL_ERROR, "k_CopyGroupToImageFixedColors() pPixelGroup or pDest parameter is NULL");
		return false;
	}

	if (pPixelGroup->NumberOfPixels==0)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_CopyGroupToImageFixedColors() no points stored in pPixelGroup");
		return false;
	}
	if ((Bits!=1) && (Bits!=8) && (Bits!=24))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_CopyGroupToImageFixedColors() Bits=%d is illegal (must be 1, 8 or 24)",
			Bits);
		return false;
	}
	/* Allocate image with correct dimensions */
	k_AllocImage((pPixelGroup->Right).x-(pPixelGroup->Left).x+1,
			(pPixelGroup->Bottom).y-(pPixelGroup->Top).y+1,Bits,pDest);
	pDest->Origo.x=-pPixelGroup->Left.x;
	pDest->Origo.y=-pPixelGroup->Top.y;
	/* Initialise background color */
	k_FlushImage(BackGround,pDest);

	for (i=0;i<pPixelGroup->NumberOfPixels;i++)
	{
		r=pPixelGroup->pPos[i];
		k_SetPixelFast(r.x,r.y,ForeGround,pDest);
	}
	return true;
}

bool k_AddGroupToImage(TImage* pDest, const TPixelGroup* pPixelGroup,UINT32 Color)
{
	unsigned int i;
	T2DInt r;
	int MinX = k_GetMinX(pDest) - pDest->BorderSize;
	int MinY = k_GetMinY(pDest) - pDest->BorderSize;
	int MaxX = k_GetMaxX(pDest) + pDest->BorderSize;
	int MaxY = k_GetMaxY(pDest) + pDest->BorderSize;
	if (pPixelGroup==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_AddGroupToImage() pPixelGroup is NULL");
		return false;
	}
	/* add positions */
	/* use special function if all pixels inside image */
	if ((pPixelGroup->Left.x>=MinX) && (pPixelGroup->Top.y>=MinY) && 
		(pPixelGroup->Right.x<MaxX) && (pPixelGroup->Bottom.y<MaxY))
	{
		return k_AddGroupToImageFast(pDest,pPixelGroup,&Color);
	}
	else if ((pDest->Origo.x==0) && (pDest->Origo.y==0))
	{
		/* special case: Origo=(0,0) */
		/* slow insertion - need to range check all positions */
		/* use the supplied pColor value */
		switch(pDest->Bits){
		case 1:
			{
				for (i=0;i<pPixelGroup->NumberOfPixels;i++)
				{
					r=pPixelGroup->pPos[i];
					if((r.x >= MinX) && (r.x<(int) MaxX) &&
					   (r.y >= MinY)  && (r.y<(int) MaxY))
					{
						k_SetPixel1bp(r.x,r.y,Color,*pDest);
					}
				}
				break;
			}
		case 8:
			{
				for (i=0;i<pPixelGroup->NumberOfPixels;i++)
				{
					r=pPixelGroup->pPos[i];
					if((r.x >= MinX) && (r.x<(int) MaxX) &&
					   (r.y >= MinY)  && (r.y<(int) MaxY))
					{
						k_SetPixel8bp(r.x,r.y,Color,*pDest);
					}
				}
				break;
			}
		case 24:
			{
				for (i=0;i<pPixelGroup->NumberOfPixels;i++)
				{
					r=pPixelGroup->pPos[i];
					if((r.x >= MinX) && (r.x<(int) MaxX) &&
					   (r.y >= MinY)  && (r.y<(int) MaxY))
					{
						k_SetPixel24bp(r.x,r.y,Color,*pDest);
					}
				}
				break;
			}
		default:
			{
				k_ShowMessageSimple(IPL_ERROR, "k_AddGroupToImage() Only implemented for 1,8 and 24 bit per pixels");
				return false;
			}
		}
	}
	else
	{
		/* Origo is NOT (0,0) */
		/* slow insertion - need to range check all positions and add origo */
		INT16 OrigoX=pDest->Origo.x;
		INT16 OrigoY=pDest->Origo.y;
		/* use the supplied Color value */
		switch(pDest->Bits){
		case 1:
			{
				for (i=0;i<pPixelGroup->NumberOfPixels;i++)
				{
					r=pPixelGroup->pPos[i];
					if((r.x >= MinX) && (r.x<(int) MaxX) &&
					   (r.y >= MinY)  && (r.y<(int) MaxY))
					{
						k_SetPixel1bp(r.x+OrigoX,r.y+OrigoY,Color,*pDest);
					}
				}
				break;
			}
		case 8:
			{
				for (i=0;i<pPixelGroup->NumberOfPixels;i++)
				{
					r=pPixelGroup->pPos[i];
					if((r.x >= MinX) && (r.x<(int) MaxX) &&
					   (r.y >= MinY)  && (r.y<(int) MaxY))
					{
						k_SetPixel8bp(r.x+OrigoX,r.y+OrigoY,Color,*pDest);
					}
				}
				break;
			}
		case 24:
			{
				for (i=0;i<pPixelGroup->NumberOfPixels;i++)
				{
					r=pPixelGroup->pPos[i];
					if((r.x >= MinX) && (r.x<(int) MaxX) &&
					   (r.y >= MinY)  && (r.y<(int) MaxY))
					{
						k_SetPixel24bp(r.x+OrigoX,r.y+OrigoY,Color,*pDest);
					}
				}
				break;
			}
		default:
			{
				k_ShowMessageSimple(IPL_ERROR, "k_AddGroupToImage() Only implemented for 1,8 and 24 bit per pixels");
				return false;
			}
		}
	}
	return true;
}

bool k_AddGroupToComplexImage(TComplexImage* pDest, const TPixelGroup* pPixelGroup, FLOAT32 Value)
{
	unsigned int i;
	T2DInt r;
	int MinX = kC_GetMinX(pDest) - pDest->BorderSize;
	int MinY = kC_GetMinY(pDest) - pDest->BorderSize;
	int MaxX = kC_GetMaxX(pDest) + pDest->BorderSize;
	int MaxY = kC_GetMaxY(pDest) + pDest->BorderSize;
	if (pPixelGroup==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_AddGroupToComplexImage() pPixelGroup is NULL");
		return false;
	}
	/* add positions */
	/* use special function if all pixels inside image */
	if ((pPixelGroup->Left.x>=MinX) && (pPixelGroup->Top.y>=MinY) && 
		(pPixelGroup->Right.x<MaxX) && (pPixelGroup->Bottom.y<MaxY))
	{
		return k_AddGroupToComplexImageFast(pDest,pPixelGroup,Value);
	}
	else if ((pDest->Origo.x==0) && (pDest->Origo.y==0))
	{
		/* special case: Origo=(0,0) */
		/* slow insertion - need to range check all positions */
		/* use the supplied Value */
		for (i=0;i<pPixelGroup->NumberOfPixels;i++)
		{
			r=pPixelGroup->pPos[i];
			if((r.x >= -MinX) && (r.x<(int) MaxX) &&
			   (r.y >= MinY)  && (r.y<(int) MaxY))
			{
				kC_SetPixelFast(r.x,r.y,Value,Value,pDest);
			}
		}
	}
	else
	{
		/* Origo is NOT (0,0) */
		/* slow insertion - need to range check all positions and add origo */
		INT16 OrigoX=pDest->Origo.x;
		INT16 OrigoY=pDest->Origo.y;
		/* use the supplied Color value */
		for (i=0;i<pPixelGroup->NumberOfPixels;i++)
		{
			r=pPixelGroup->pPos[i];
			if((r.x >= -MinX) && (r.x<(int) MaxX) &&
			   (r.y >= MinY)  && (r.y<(int) MaxY))
			{
				kC_SetPixelFast(r.x+OrigoX,r.y+OrigoY,Value,Value,pDest);
			}
		}
	}
	return true;
}

bool k_AddGroupToFloatImage(TFloatImage* pDest, const TPixelGroup* pPixelGroup, FLOAT32 Value)
{
	unsigned int i;
	T2DInt r;
	int MinX = kF_GetMinX(pDest) - pDest->BorderSize;
	int MinY = kF_GetMinY(pDest) - pDest->BorderSize;
	int MaxX = kF_GetMaxX(pDest) + pDest->BorderSize;
	int MaxY = kF_GetMaxY(pDest) + pDest->BorderSize;
	if (pPixelGroup==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_AddGroupToFloatImage() pPixelGroup is NULL");
		return false;
	}
	/* add positions */
	/* use special function if all pixels inside image */
	if ((pPixelGroup->Left.x>=MinX) && (pPixelGroup->Top.y>=MinY) && 
		(pPixelGroup->Right.x<MaxX) && (pPixelGroup->Bottom.y<MaxY))
	{
		return k_AddGroupToFloatImageFast(pDest,pPixelGroup,Value);
	}
	else if ((pDest->Origo.x==0) && (pDest->Origo.y==0))
	{
		/* special case: Origo=(0,0) */
		/* slow insertion - need to range check all positions */
		/* use the supplied Value */
		for (i=0;i<pPixelGroup->NumberOfPixels;i++)
		{
			r=pPixelGroup->pPos[i];
			if((r.x >= -MinX) && (r.x<(int) MaxX) &&
			   (r.y >= MinY)  && (r.y<(int) MaxY))
			{
				kF_SetPixelFast(r.x,r.y,Value,pDest);
			}
		}
	}
	else
	{
		/* Origo is NOT (0,0) */
		/* slow insertion - need to range check all positions and add origo */
		INT16 OrigoX=pDest->Origo.x;
		INT16 OrigoY=pDest->Origo.y;
		/* use the supplied Color value */
		for (i=0;i<pPixelGroup->NumberOfPixels;i++)
		{
			r=pPixelGroup->pPos[i];
			if((r.x >= -MinX) && (r.x<(int) MaxX) &&
			   (r.y >= MinY)  && (r.y<(int) MaxY))
			{
				kF_SetPixelFast(r.x+OrigoX,r.y+OrigoY,Value,pDest);
			}
		}
	}
	return true;
}

bool k_AddGroupToIntImage(TIntImage* pDest, const TPixelGroup* pPixelGroup, INT16 Value)
{
	unsigned int i;
	T2DInt r;
	int MinX = kI_GetMinX(pDest) - pDest->BorderSize;
	int MinY = kI_GetMinY(pDest) - pDest->BorderSize;
	int MaxX = kI_GetMaxX(pDest) + pDest->BorderSize;
	int MaxY = kI_GetMaxY(pDest) + pDest->BorderSize;
	if (pPixelGroup==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_AddGroupToIntImage() pPixelGroup is NULL");
		return false;
	}
	/* add positions */
	/* use special function if all pixels inside image */
	if ((pPixelGroup->Left.x>=MinX) && (pPixelGroup->Top.y>=MinY) && 
		(pPixelGroup->Right.x<MaxX) && (pPixelGroup->Bottom.y<MaxY))
	{
		return k_AddGroupToIntImageFast(pDest,pPixelGroup,Value);
	}
	else if ((pDest->Origo.x==0) && (pDest->Origo.y==0))
	{
		/* special case: Origo=(0,0) */
		/* slow insertion - need to range check all positions */
		/* use the supplied Value */
		for (i=0;i<pPixelGroup->NumberOfPixels;i++)
		{
			r=pPixelGroup->pPos[i];
			if((r.x >= -MinX) && (r.x<(int) MaxX) &&
			   (r.y >= MinY)  && (r.y<(int) MaxY))
			{
				kI_SetPixelFast(r.x,r.y,Value,pDest);
			}
		}
	}
	else
	{
		/* Origo is NOT (0,0) */
		/* slow insertion - need to range check all positions and add origo */
		INT16 OrigoX=pDest->Origo.x;
		INT16 OrigoY=pDest->Origo.y;
		/* use the supplied Color value */
		for (i=0;i<pPixelGroup->NumberOfPixels;i++)
		{
			r=pPixelGroup->pPos[i];
			if((r.x >= -MinX) && (r.x<(int) MaxX) &&
			   (r.y >= MinY)  && (r.y<(int) MaxY))
			{
				kI_SetPixelFast(r.x+OrigoX,r.y+OrigoY,Value,pDest);
			}
		}
	}
	return true;
}

bool k_AddGroupColorsToImage(TImage* pDest, const TPixelGroup* pPixelGroup)
{
	unsigned int i;
	T2DInt r;
	int MinX = k_GetMinX(pDest) - pDest->BorderSize;
	int MinY = k_GetMinY(pDest) - pDest->BorderSize;
	int MaxX = k_GetMaxX(pDest) + pDest->BorderSize;
	int MaxY = k_GetMaxY(pDest) + pDest->BorderSize;
	if (pPixelGroup==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_AddGroupColorsToImage() pPixelGroup is NULL");
		return false;
	}
	else if (pPixelGroup->pColor==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_AddGroupColorsToImage() No colors in PixelGroup");
		return false;
	}
	/* add positions */
	/* use special function if all pixels inside image */
	if ((pPixelGroup->Left.x>=MinX) && (pPixelGroup->Top.y>=MinY) && 
		(pPixelGroup->Right.x<MaxX) && (pPixelGroup->Bottom.y<MaxY))
	{
		return k_AddGroupToImageFast(pDest,pPixelGroup,NULL);
	}
	else if ((pDest->Origo.x==0) && (pDest->Origo.y==0))
	{
		/* special case: Origo=(0,0) */
		/* slow insertion - need to range check all positions */
		switch(pDest->Bits){
		case 1:
			{
				for (i=0;i<pPixelGroup->NumberOfPixels;i++)
				{
					r=pPixelGroup->pPos[i];
					if((r.x >= -MinX) && (r.x<(int) MaxX) &&
					   (r.y >= MinY)  && (r.y<(int) MaxY))
					{
						k_SetPixel1bp(r.x,r.y,pPixelGroup->pColor[i],*pDest);
					}
				}
				break;
			}
		case 8:
			{
				for (i=0;i<pPixelGroup->NumberOfPixels;i++)
				{
					r=pPixelGroup->pPos[i];
					if((r.x >= -MinX) && (r.x<(int) MaxX) &&
					   (r.y >= MinY)  && (r.y<(int) MaxY))
					{
						k_SetPixel8bp(r.x,r.y,pPixelGroup->pColor[i],*pDest);
					}
				}
				break;
			}
		case 24:
			{
				for (i=0;i<pPixelGroup->NumberOfPixels;i++)
				{
					r=pPixelGroup->pPos[i];
					if((r.x >= -MinX) && (r.x<(int) MaxX) &&
					   (r.y >= MinY)  && (r.y<(int) MaxY))
					{
						k_SetPixel24bp(r.x,r.y,pPixelGroup->pColor[i],*pDest);
					}
				}
				break;
			}
		default:
			{
				k_ShowMessageSimple(
					IPL_ERROR,
					"k_AddGroupColorsToImage() Only implemented for 1,8 and 24 bit per pixels");
				return false;
			}
		}
	}
	else
	{
		/* Origo is NOT (0,0) */
		/* slow insertion - need to range check all positions and add origo */
		INT16 OrigoX=pDest->Origo.x;
		INT16 OrigoY=pDest->Origo.y;
		switch(pDest->Bits){
		case 1:
			{
				for (i=0;i<pPixelGroup->NumberOfPixels;i++)
				{
					r=pPixelGroup->pPos[i];
					if((r.x >= -MinX) && (r.x<(int) MaxX) &&
					   (r.y >= MinY)  && (r.y<(int) MaxY))
					{
						k_SetPixel1bp(r.x+OrigoX,r.y+OrigoY,pPixelGroup->pColor[i],*pDest);
					}
				}
				break;
			}
		case 8:
			{
				for (i=0;i<pPixelGroup->NumberOfPixels;i++)
				{
					r=pPixelGroup->pPos[i];
					if((r.x >= -MinX) && (r.x<(int) MaxX) &&
					   (r.y >= MinY)  && (r.y<(int) MaxY))
					{
						k_SetPixel8bp(r.x+OrigoX,r.y+OrigoY,pPixelGroup->pColor[i],*pDest);
					}
				}
				break;
			}
		case 24:
			{
				for (i=0;i<pPixelGroup->NumberOfPixels;i++)
				{
					r=pPixelGroup->pPos[i];
					if((r.x >= -MinX) && (r.x<(int) MaxX) &&
					   (r.y >= MinY)  && (r.y<(int) MaxY))
					{
						k_SetPixel24bp(r.x+OrigoX,r.y+OrigoY,pPixelGroup->pColor[i],*pDest);
					}
				}
				break;
			}
		default:
			{
				k_ShowMessageSimple(
					IPL_ERROR,
					"k_AddGroupColorsToImage() Only implemented for 1,8 and 24 bit per pixels");
				return false;
			}
		}
	}
	return true;
}

/* assumens all pixels are in valid range */
bool k_AddGroupToImageFast(TImage* pDest, const TPixelGroup* pPixelGroup, const UINT32* pColor)
{
	unsigned int i;
	T2DInt r;
	if ((pDest->Origo.x==0) && (pDest->Origo.y==0))
	{
		/* special case: Origo=(0,0) */
		/* fast insertion - no range check */
		if (pColor==NULL)
		{
			switch(pDest->Bits){
			case 1:
				{
					for (i=0;i<pPixelGroup->NumberOfPixels;i++)
					{
						r=pPixelGroup->pPos[i];
						k_SetPixel1bp(r.x,r.y,pPixelGroup->pColor[i],*pDest);
					}
					break;
				}
			case 8:
				{
					for (i=0;i<pPixelGroup->NumberOfPixels;i++)
					{
						r=pPixelGroup->pPos[i];
						k_SetPixel8bp(r.x,r.y,pPixelGroup->pColor[i],*pDest);
					}
					break;
				}
			case 24:
				{
					for (i=0;i<pPixelGroup->NumberOfPixels;i++)
					{
						r=pPixelGroup->pPos[i];
						k_SetPixel24bp(r.x,r.y,pPixelGroup->pColor[i],*pDest);
					}
					break;
				}
			default:
				{
					k_ShowMessageSimple(
						IPL_ERROR,
						"k_AddGroupToImageFast() Only implemented for 1,8 and 24 bit per pixels");
					return false;
				}
			}
		}
		else
		{
			/* use the supplied pColor value */
			switch(pDest->Bits){
			case 1:
				{
					for (i=0;i<pPixelGroup->NumberOfPixels;i++)
					{
						r=pPixelGroup->pPos[i];
						k_SetPixel1bp(r.x,r.y,*pColor,*pDest);
					}
					break;
				}
			case 8:
				{
					for (i=0;i<pPixelGroup->NumberOfPixels;i++)
					{
						r=pPixelGroup->pPos[i];
						k_SetPixel8bp(r.x,r.y,*pColor,*pDest);
					}
					break;
				}
			case 24:
				{
					for (i=0;i<pPixelGroup->NumberOfPixels;i++)
					{
						r=pPixelGroup->pPos[i];
						k_SetPixel24bp(r.x,r.y,*pColor,*pDest);
					}
					break;
				}
			default:
				{
					k_ShowMessageSimple(
						IPL_ERROR,
						"k_AddGroupToImageFast() Only implemented for 1,8 and 24 bit per pixels");
					return false;
				}
			}
		}
	}
	else
	{
		/* Origo is NOT (0,0) */
		/* fast insertion - no range check but need to add origo */
		INT16 OrigoX=pDest->Origo.x;
		INT16 OrigoY=pDest->Origo.y;
		if (pColor==NULL)
		{
			switch(pDest->Bits){
			case 1:
				{
					for (i=0;i<pPixelGroup->NumberOfPixels;i++)
					{
						r=pPixelGroup->pPos[i];
						k_SetPixel1bp(r.x+OrigoX,r.y+OrigoY,pPixelGroup->pColor[i],*pDest);
					}
					break;
				}
			case 8:
				{
					for (i=0;i<pPixelGroup->NumberOfPixels;i++)
					{
						r=pPixelGroup->pPos[i];
						k_SetPixel8bp(r.x+OrigoX,r.y+OrigoY,pPixelGroup->pColor[i],*pDest);
					}
				}
			case 24:
				{
					for (i=0;i<pPixelGroup->NumberOfPixels;i++)
					{
						r=pPixelGroup->pPos[i];
						k_SetPixel24bp(r.x+OrigoX,r.y+OrigoY,pPixelGroup->pColor[i],*pDest);
					}
					break;
				}
			default:
				{
					k_ShowMessageSimple(
						IPL_ERROR,
						"k_AddGroupToImageFast() Only implemented for 1,8 and 24 bit per pixels");
					return false;
				}
			}
		}
		else
		{
			/* use the supplied pColor value */
			switch(pDest->Bits){
			case 1:
				{
					for (i=0;i<pPixelGroup->NumberOfPixels;i++)
					{
						r=pPixelGroup->pPos[i];
						k_SetPixel1bp(r.x+OrigoX,r.y+OrigoY,*pColor,*pDest);
					}
					break;
				}
			case 8:
				{
					for (i=0;i<pPixelGroup->NumberOfPixels;i++)
					{
						r=pPixelGroup->pPos[i];
						k_SetPixel8bp(r.x+OrigoX,r.y+OrigoY,*pColor,*pDest);
					}
					break;
				}
			case 24:
				{
					for (i=0;i<pPixelGroup->NumberOfPixels;i++)
					{
						r=pPixelGroup->pPos[i];
						k_SetPixel24bp(r.x+OrigoX,r.y+OrigoY,*pColor,*pDest);
					}
					break;
				}
			default:
				{
					k_ShowMessageSimple(
						IPL_ERROR,
						"k_AddGroupToImageFast() Only implemented for 1,8 and 24 bit per pixels");
					return false;
				}
			}
		}
	}
	return true;
}

/* assumens all pixels are in valid range */
bool k_AddGroupToComplexImageFast(TComplexImage* pDest,const TPixelGroup* pPixelGroup,FLOAT32 Value)
{
	unsigned int i;
	T2DInt r;
	if ((pDest->Origo.x==0) && (pDest->Origo.y==0))
	{
		/* special case: Origo=(0,0) */
		/* fast insertion - no range check */
		/* use the supplied Value */
		for (i=0;i<pPixelGroup->NumberOfPixels;i++)
		{
			r=pPixelGroup->pPos[i];
			kC_SetPixelFast(r.x,r.y,Value,Value,pDest);
		}
	}
	else
	{
		/* Origo is NOT (0,0) */
		/* fast insertion - no range check but need to add origo */
		INT16 OrigoX=pDest->Origo.x;
		INT16 OrigoY=pDest->Origo.y;
		/* use the supplied Value */
		for (i=0;i<pPixelGroup->NumberOfPixels;i++)
		{
			r=pPixelGroup->pPos[i];
			kC_SetPixelFast(r.x+OrigoX,r.y+OrigoY,Value,Value,pDest);
		}
	}
	return true;
}

bool k_AddGroupToFloatImageFast(TFloatImage* pDest, const TPixelGroup* pPixelGroup, FLOAT32 Value)
{
	unsigned int i;
	T2DInt r;
	if ((pDest->Origo.x==0) && (pDest->Origo.y==0))
	{
		/* special case: Origo=(0,0) */
		/* fast insertion - no range check */
		/* use the supplied Value */
		for (i=0;i<pPixelGroup->NumberOfPixels;i++)
		{
			r=pPixelGroup->pPos[i];
			kF_SetPixelFast(r.x,r.y,Value,pDest);
		}
	}
	else
	{
		/* Origo is NOT (0,0) */
		/* fast insertion - no range check but need to add origo */
		INT16 OrigoX=pDest->Origo.x;
		INT16 OrigoY=pDest->Origo.y;
		/* use the supplied Value */
		for (i=0;i<pPixelGroup->NumberOfPixels;i++)
		{
			r=pPixelGroup->pPos[i];
			kF_SetPixelFast(r.x+OrigoX,r.y+OrigoY,Value,pDest);
		}
	}
	return true;
}

bool k_AddGroupToIntImageFast(TIntImage* pDest, const TPixelGroup* pPixelGroup, INT16 Value)
{
	unsigned int i;
	T2DInt r;
	if ((pDest->Origo.x==0) && (pDest->Origo.y==0))
	{
		/* special case: Origo=(0,0) */
		/* fast insertion - no range check */
		/* use the supplied Value */
		for (i=0;i<pPixelGroup->NumberOfPixels;i++)
		{
			r=pPixelGroup->pPos[i];
			kI_SetPixelFast(r.x,r.y,Value,pDest);
		}
	}
	else
	{
		/* Origo is NOT (0,0) */
		/* fast insertion - no range check but need to add origo */
		INT16 OrigoX=pDest->Origo.x;
		INT16 OrigoY=pDest->Origo.y;
		/* use the supplied Value */
		for (i=0;i<pPixelGroup->NumberOfPixels;i++)
		{
			r=pPixelGroup->pPos[i];
			kI_SetPixelFast(r.x+OrigoX,r.y+OrigoY,Value,pDest);
		}
	}
	return true;
}

bool k_GroupToBinaryImage(const TPixelGroup* pPixelGroup,COLORTYPE Color,TImage* pDest)
{
	unsigned int i;
	T2DInt r;
	{
		k_ShowMessageSimple(
			IPL_WARNING,
			"k_GroupToBinaryImage() This function will be removed in next version - use k_CopyGroupToImage()");
	}
	if (pPixelGroup==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GroupToBinaryImage() pPixelGroup parameter is NULL");
		return false;
	}

	/* Allocate a binary image with correct dimensions */
	k_AllocImage((pPixelGroup->Right).x-(pPixelGroup->Left).x+1,
			(pPixelGroup->Bottom).y-(pPixelGroup->Top).y+1,1,pDest);
	/* Initialise background color */
	if (Color==HIGHCOLOR) 
		k_FlushImage(0,pDest);
	else
		k_FlushImage(1,pDest);
	/* copy pixels to image */
	for (i=0;i<pPixelGroup->NumberOfPixels;i++)
	{
		r=pPixelGroup->pPos[i];
		if (Color==HIGHCOLOR)
			k_SetPixel(r.x-(pPixelGroup->Left).x,r.y-(pPixelGroup->Top).y,1,pDest);
		else
			k_SetPixel(r.x-(pPixelGroup->Left).x,r.y-(pPixelGroup->Top).y,0,pDest);
	}
	return true;
}

bool k_GroupToGrayImage(const TPixelGroup* pPixelGroup,UINT8 Background,
						   const TImage* pSource, TImage* pDest)
{
	unsigned i;
	T2DInt r;
	{
		k_ShowMessageSimple(
			IPL_WARNING,
			"k_GroupToGrayImage() This function will be removed in next version - use k_CopyGroupToImage()");
	}
	if ((pPixelGroup==NULL) || (pSource==NULL) || (pDest==NULL))
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GroupToGrayImage() One of the parameters is NULL");
		return false;
	}
	/* Allocate a gray tone image with correct dimensions */
	k_AllocImage((pPixelGroup->Right).x-(pPixelGroup->Left).x+1,
		(pPixelGroup->Bottom).y-(pPixelGroup->Top).y+1,8,pDest);
	k_FlushImage(Background,pDest); /* Initialise background color */
	/* copy pixels to image */
	for (i=0;i<pPixelGroup->NumberOfPixels;i++)
	{
		r=pPixelGroup->pPos[i];
		k_SetPixel(r.x-(pPixelGroup->Left).x,r.y-(pPixelGroup->Top).y,k_GetPixel(r.x,r.y,pSource),pDest);
	}
	return true;
}

bool k_GroupColorsToGrayImage(const TPixelGroup* pPixelGroup,UINT8 Background,
							  TImage* pDest)
{
	unsigned i;
	T2DInt r;
	{
		k_ShowMessageSimple(
			IPL_WARNING,
			"k_GroupColorsToGrayImage() This function will be removed in next version - use k_CopyGroupToImage()");
	}
	if ((pPixelGroup==NULL) || (pDest==NULL))
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GroupToGrayImage() One of the parameters is NULL");
		return false;
	}
	/* Allocate a gray tone image with correct dimensions */
	k_AllocImage((pPixelGroup->Right).x-(pPixelGroup->Left).x+1,
		(pPixelGroup->Bottom).y-(pPixelGroup->Top).y+1,8,pDest);
	k_FlushImage(Background,pDest); /* Initialise background color */
	/* copy pixels to image */
	for (i=0;i<pPixelGroup->NumberOfPixels;i++)
	{
		r=pPixelGroup->pPos[i];
		k_SetPixel(r.x-(pPixelGroup->Left).x,r.y-(pPixelGroup->Top).y,pPixelGroup->pColor[i],pDest);
	}
	return true;
}

bool k_PosInGroup(const TPixelGroup* pPixelGroup, T2DInt Pos,unsigned int* pIndex)
{
	if (pPixelGroup==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_PosInGroup() pPixelGroup parameter is NULL");
		return false;
	}
	else
	{
		T2DInt* p=pPixelGroup->pPos;
		T2DInt* end=&(pPixelGroup->pPos[pPixelGroup->NumberOfPixels]);
		while(p!=end)
		{
			if ((p->x==Pos.x) && (p->y==Pos.y))
			{
				*pIndex=p-pPixelGroup->pPos;
				return true;
			}
			p++;
		}
		/* if the loop didn't return, the given position is not in the group */
		return false;
	}
}

void k_PrintGroupInfo(const TPixelGroup* pPixelGroup,bool PrintPositions,unsigned int PosPrLine)
{
	unsigned int x,y;
	if (PosPrLine==0)
		PosPrLine=1; /* minimum one position on each line! */
	printf("**************** PixelGroup info ********************\n");
	printf(" Positions: Top=(%d,%d) Bottom=(%d,%d) Left=(%d,%d) Right=(%d,%d)\n",
		pPixelGroup->Top.x,pPixelGroup->Top.y,pPixelGroup->Bottom.x,pPixelGroup->Bottom.y,
		pPixelGroup->Left.x,pPixelGroup->Left.y,pPixelGroup->Right.x,pPixelGroup->Right.y);
	printf(" Width=%d Height=%d\n",pPixelGroup->Right.x-pPixelGroup->Left.x+1,
			pPixelGroup->Bottom.y-pPixelGroup->Top.y+1);
	printf(" NumberOfPixels=%d AllocatedPixels=%d\n",pPixelGroup->NumberOfPixels,
		pPixelGroup->AllocatedPixels);
	if (PrintPositions==true)
	{
		printf("\n");
		if (pPixelGroup->pPos!=NULL)
		{
			for(x=0; x<(pPixelGroup->NumberOfPixels/PosPrLine); x++)
			{
				for(y=0; y<PosPrLine; y++)
				{
					printf(" pos %d=(%d,%d) ",x*PosPrLine+y,pPixelGroup->pPos[x*PosPrLine+y].x,pPixelGroup->pPos[x*PosPrLine+y].y);
				}
				printf("\n");
			}
			for(y=pPixelGroup->NumberOfPixels-(pPixelGroup->NumberOfPixels %  PosPrLine);
			y<pPixelGroup->NumberOfPixels; y++)
			{
				printf(" pos %d=(%d,%d) ",y,pPixelGroup->pPos[y].x,pPixelGroup->pPos[y].y);
			}
			printf("\n");
			
		}
		else
		{
			printf("No positions in PixelGroup\n");
		}
	}
}

void k_GroupUpdateBoundaries(TPixelGroup* pPixelGroup)
{
	T2DInt Pos;
	unsigned int i;
	/* set the boundary values so that a update is guaranteed */
	pPixelGroup->Top.x=0;
	pPixelGroup->Top.y=32767;
	pPixelGroup->Bottom.x=0;
	pPixelGroup->Bottom.y=-32768;
	pPixelGroup->Left.x=32767;
	pPixelGroup->Left.y=0;
	pPixelGroup->Right.x=-32768;
	pPixelGroup->Right.y=0;
	for (i=0;i<pPixelGroup->NumberOfPixels;i++)
	{
		Pos=pPixelGroup->pPos[i];
		/* Update max. and min. values in pPixelGroup */
		if (Pos.x<(pPixelGroup->Left).x)
			pPixelGroup->Left=Pos;
		if (Pos.x>(pPixelGroup->Right).x)
			pPixelGroup->Right=Pos;
		if (Pos.y<(pPixelGroup->Top).y)
			pPixelGroup->Top=Pos;
		if (Pos.y>(pPixelGroup->Bottom).y)
			pPixelGroup->Bottom=Pos;
	}
}

bool k_RemovePosFromGroupFast(unsigned int Index, TPixelGroup* pPixelGroup)
{
	/* Move last position to Index */
	pPixelGroup->pPos[Index]=pPixelGroup->pPos[pPixelGroup->NumberOfPixels-1];
	/* If colors are present move the corresponding color */
	if (pPixelGroup->pColor!=NULL)
	{
		pPixelGroup->pColor[Index]=pPixelGroup->pColor[pPixelGroup->NumberOfPixels-1];
	}
	pPixelGroup->NumberOfPixels--;
	return true;
}

void k_AddPosToGroupFast(INT16 x, INT16 y, TPixelGroup* pPixelGroup)
{
	T2DInt p;
	p.x=x;
	p.y=y;
	if (pPixelGroup->NumberOfPixels==pPixelGroup->AllocatedPixels)
	{
		/* allocate 2 times previous allocated memory or 2 if memory never allocated before */
		pPixelGroup->AllocatedPixels = 
			(pPixelGroup->AllocatedPixels==0) ? 2 : 2*pPixelGroup->AllocatedPixels;
		k_ReAllocGroup(pPixelGroup,pPixelGroup->AllocatedPixels);
	}
	/* insert pixel and increment total pixels variable */
	pPixelGroup->pPos[pPixelGroup->NumberOfPixels++] = p;
}

void k_Add2DPosToGroupFast(T2DInt Pos, TPixelGroup* pPixelGroup)
{
	if (pPixelGroup->NumberOfPixels==pPixelGroup->AllocatedPixels)
	{
		/* allocate 2 times previous allocated memory or 2 if memory never allocated before */
		pPixelGroup->AllocatedPixels = 
			(pPixelGroup->AllocatedPixels==0) ? 2 : 2*pPixelGroup->AllocatedPixels;

		k_ReAllocGroup(pPixelGroup,pPixelGroup->AllocatedPixels);
	}
	/* insert pixel and increment total pixels variable */
	pPixelGroup->pPos[pPixelGroup->NumberOfPixels++] = Pos;
}

bool k_InsertPosInGroupFast(INT16 x, INT16 y, unsigned int Index, TPixelGroup* pPixelGroup)
{
	/* Add a dummy position to the end of position array */
	k_Add2DPosToGroupFast(pPixelGroup->pPos[0],pPixelGroup);
	/* Move positions */
	memmove(&(pPixelGroup->pPos[Index+1]),&(pPixelGroup->pPos[Index]),
		sizeof(T2DInt)*(pPixelGroup->NumberOfPixels-Index-1));
	/* Store the new position at Index */
	pPixelGroup->pPos[Index].x=x;
	pPixelGroup->pPos[Index].y=y;
	return true;
}

bool k_Insert2DPosInGroupFast(T2DInt Pos, unsigned int Index, TPixelGroup* pPixelGroup)
{
	/* Add a dummy position to the end of position array */
	k_Add2DPosToGroupFast(pPixelGroup->pPos[0],pPixelGroup);
	/* Move positions */
	memmove(&(pPixelGroup->pPos[Index+1]),&(pPixelGroup->pPos[Index]),
		sizeof(T2DInt)*(pPixelGroup->NumberOfPixels-Index-1));
	/* Store the new position at Index */
	pPixelGroup->pPos[Index]=Pos;
	return true;
}

void k_AddPosColorToGroupFast(INT16 x, INT16 y, UINT32 Color, TPixelGroup* pPixelGroup)
{
	T2DInt p;
	p.x=x;
	p.y=y;
	if (pPixelGroup->NumberOfPixels==pPixelGroup->AllocatedPixels)
	{
		/* allocate 2 times previous allocated memory or 2 if memory never allocated before */
		pPixelGroup->AllocatedPixels = 
			(pPixelGroup->AllocatedPixels==0) ? 2 : 2*pPixelGroup->AllocatedPixels;
		k_ReAllocGroup(pPixelGroup,pPixelGroup->AllocatedPixels);
	}
	/* insert pixel and color and increment total pixels variable */
	pPixelGroup->pColor[pPixelGroup->NumberOfPixels] = Color;
	pPixelGroup->pPos[pPixelGroup->NumberOfPixels++] = p;
}

void k_Add2DPosColorToGroupFast(T2DInt Pos, UINT32 Color, TPixelGroup* pPixelGroup)
{
	if (pPixelGroup->NumberOfPixels==pPixelGroup->AllocatedPixels)
	{
		/* allocate 2 times previous allocated memory or 2 if memory never allocated before */
		pPixelGroup->AllocatedPixels = 
			(pPixelGroup->AllocatedPixels==0) ? 2 : 2*pPixelGroup->AllocatedPixels;

		k_ReAllocGroup(pPixelGroup,pPixelGroup->AllocatedPixels);
	}
	/* insert pixel and color and increment total pixels variable */
	pPixelGroup->pColor[pPixelGroup->NumberOfPixels] = Color;
	pPixelGroup->pPos[pPixelGroup->NumberOfPixels++] = Pos;
}

bool k_InsertPosColorInGroupFast(INT16 x, INT16 y, UINT32 Color, unsigned int Index, TPixelGroup* pPixelGroup)
{
	/* Add a dummy position to the end of position array */
	k_Add2DPosColorToGroupFast(pPixelGroup->pPos[0],0,pPixelGroup);
	/* Move positions */
	memmove(&(pPixelGroup->pPos[Index+1]),&(pPixelGroup->pPos[Index]),
		sizeof(T2DInt)*(pPixelGroup->NumberOfPixels-Index-1));
	/* Move colors */
	memmove(&(pPixelGroup->pColor[Index+1]),&(pPixelGroup->pColor[Index]),
		sizeof(UINT32)*(pPixelGroup->NumberOfPixels-Index-1));
	/* Store the new position at Index */
	pPixelGroup->pPos[Index].x=x;
	pPixelGroup->pPos[Index].y=y;
	/* Store the new color at Index */
	pPixelGroup->pColor[Index]=Color;
	return true;
}

bool k_Insert2DPosColorInGroupFast(T2DInt Pos, UINT32 Color, unsigned int Index, TPixelGroup* pPixelGroup)
{
	/* Add a dummy position to the end of position array */
	k_Add2DPosColorToGroupFast(pPixelGroup->pPos[0],0,pPixelGroup);
	/* Move positions */
	memmove(&(pPixelGroup->pPos[Index+1]),&(pPixelGroup->pPos[Index]),
		sizeof(T2DInt)*(pPixelGroup->NumberOfPixels-Index-1));
	/* Move colors */
	memmove(&(pPixelGroup->pColor[Index+1]),&(pPixelGroup->pColor[Index]),
		sizeof(UINT32)*(pPixelGroup->NumberOfPixels-Index-1));
	/* Store the new position at Index */
	pPixelGroup->pPos[Index]=Pos;
	/* Store the new color at Index */
	pPixelGroup->pColor[Index]=Color;
	return true;
}

/************************************************/
/********        Private functions       ********/
/************************************************/

/*  Checks if the boundaries stored in the pixelgroup have to be updated when
	removing a position at given Index. Internal use only.
	@param Index The Index in the pixelgroup to be removed. Must be in range! */
void k_UpdateBoundaries(T2DInt RemovedPos,TPixelGroup* pPixelGroup)
{
	/* if the pixelgroup is empty after removing the actual position
		then reset all boundaries to (0,0) */
	if (pPixelGroup->NumberOfPixels==0)
	{
		pPixelGroup->Left.x=0;
		pPixelGroup->Left.y=0;
		pPixelGroup->Right.x=0;
		pPixelGroup->Right.y=0;
		pPixelGroup->Top.x=0;
		pPixelGroup->Top.y=0;
		pPixelGroup->Bottom.x=0;
		pPixelGroup->Bottom.y=0;
	}
	else
	{
		if ((RemovedPos.x==(pPixelGroup->Left.x)) && (RemovedPos.y==(pPixelGroup->Left.y)))
		{
			unsigned int x;
			pPixelGroup->Left.x=pPixelGroup->Right.x; /* Set Left.x to the highest possible value */
			for(x=0;x<pPixelGroup->NumberOfPixels;x++)
			{
				if(pPixelGroup->pPos[x].x<=pPixelGroup->Left.x)
				{
					pPixelGroup->Left=pPixelGroup->pPos[x];
				}
			}
		}
		if ((RemovedPos.x==(pPixelGroup->Right.x)) && (RemovedPos.y==(pPixelGroup->Right.y)))
		{
			unsigned int x;
			pPixelGroup->Right.x=pPixelGroup->Left.x; /* Set Right.x to the lowest possible value */
			for(x=0;x<pPixelGroup->NumberOfPixels;x++)
			{
				if(pPixelGroup->pPos[x].x>=pPixelGroup->Right.x)
				{
					pPixelGroup->Right=pPixelGroup->pPos[x];
				}
			}
		}
		if ((RemovedPos.x==(pPixelGroup->Top.x)) && (RemovedPos.y==(pPixelGroup->Top.y)))
		{
			unsigned int x;
			pPixelGroup->Top.y=pPixelGroup->Bottom.y; /* Set Top.y to the highest possible value */
			for(x=0;x<pPixelGroup->NumberOfPixels;x++)
			{
				if(pPixelGroup->pPos[x].y<=pPixelGroup->Top.y)
				{
					pPixelGroup->Top=pPixelGroup->pPos[x];
				}
			}
		}
		if ((RemovedPos.x==(pPixelGroup->Bottom.x)) && (RemovedPos.y==(pPixelGroup->Bottom.y)))
		{
			unsigned int x;
			pPixelGroup->Bottom.y=pPixelGroup->Top.y; /* Set Bottom.y to the lowest possible value */
			for(x=0;x<pPixelGroup->NumberOfPixels;x++)
			{
				if(pPixelGroup->pPos[x].y>=pPixelGroup->Bottom.y)
				{
					pPixelGroup->Bottom=pPixelGroup->pPos[x];
				}
			}
		}
	}
}

bool k_GroupAddOffset(TPixelGroup* pPixelGroup,T2DInt Offset)
{
	unsigned int x;
	if (pPixelGroup->NumberOfPixels==0)
		return false;
	for(x=0;x<pPixelGroup->NumberOfPixels;x++)
	{
		pPixelGroup->pPos[x].x += Offset.x;
		pPixelGroup->pPos[x].y += Offset.y;
	}
	/* updates boundaries, have to run through all values in case
	   the boundaries were not up to date before this function call */
	k_GroupUpdateBoundaries(pPixelGroup);
	return true;
}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
