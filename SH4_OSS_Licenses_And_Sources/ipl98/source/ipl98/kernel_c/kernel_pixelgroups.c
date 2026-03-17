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

#include "kernel_pixelgroups.h"
#include "kernel_error.h"
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/************************************************/
/********        Private functions       ********/
/************************************************/

/* This is a private function */
void k_ReAllocGroups(TPixelGroups* pPixelGroups,unsigned int NewSize);

/************************************************/
/********        Public functions        ********/
/************************************************/

void k_InitGroups(TPixelGroups* pPixelGroups)
{
	pPixelGroups->NumberOfGroups=0;
	pPixelGroups->AllocatedGroups=0; /* internal use */
	pPixelGroups->pGroups=NULL;
}

/*	Deletes all allocated memory for PixelGroup */
void k_EmptyGroups(TPixelGroups* pPixelGroups)
{
	unsigned int i;
	if (pPixelGroups->pGroups!=NULL)
	{
		for(i=0;i<pPixelGroups->NumberOfGroups; i++)
		{
			k_EmptyGroup(&(pPixelGroups->pGroups[i]));
		}
		free(pPixelGroups->pGroups);
	}
	k_InitGroups(pPixelGroups);
}

void k_AllocGroups(TPixelGroups* pPixelGroups,unsigned int InitialSize)
{
	unsigned int i;
	k_EmptyGroups(pPixelGroups);
	if (InitialSize==0)
		InitialSize=1; /* minimum one allocated TPixelGroup */
	pPixelGroups->pGroups=(TPixelGroup*)malloc(InitialSize*sizeof(TPixelGroup));
	for(i=0;i<InitialSize;i++)
		k_InitGroup(&(pPixelGroups->pGroups[i]));

	pPixelGroups->NumberOfGroups=0; /* just in case! */
	pPixelGroups->AllocatedGroups=InitialSize; /* internal use */
}

void k_AddGroupToGroups(const TPixelGroup* pPixelGroup, TPixelGroups* pPixelGroups)
{
	const TPixelGroup* pGrp=pPixelGroup; /* short hand notation */
	TPixelGroups* pGrps=pPixelGroups; /* short hand notation */
	if (pGrps->NumberOfGroups==pGrps->AllocatedGroups)
	{
		/* allocate 20 new TPixelGroup structures in array */
		k_ReAllocGroups(pGrps,20+pGrps->AllocatedGroups);
	}
	/* Insert TPixelGroup structure */
	pGrps->pGroups[pGrps->NumberOfGroups] = *pPixelGroup;
	if (pGrp->pPos!=NULL)
	{
		/* allocate new memory for the pPos in new TPixelGroup and copy the position values */
		(pGrps->pGroups[pGrps->NumberOfGroups]).pPos = (T2DInt*)malloc((pGrp->NumberOfPixels*sizeof(T2DInt)));
		memcpy((pGrps->pGroups[pGrps->NumberOfGroups]).pPos,pGrp->pPos,pGrp->NumberOfPixels*sizeof(T2DInt));
	}
	if (pGrp->pColor!=NULL)
	{
		/* allocate new memory for the pColor in new TPixelGroup and copy color values */
		(pGrps->pGroups[pGrps->NumberOfGroups]).pColor = (UINT32*)malloc((pGrp->NumberOfPixels*sizeof(UINT32)));
		memcpy((pGrps->pGroups[pGrps->NumberOfGroups]).pColor,pGrp->pColor,pGrp->NumberOfPixels*sizeof(UINT32));
	}
	/* Increment total groups variable */
	pPixelGroups->NumberOfGroups++;
}

bool k_InsertGroupInGroups(const TPixelGroup* pPixelGroup, unsigned int Index,
						   TPixelGroups* pPixelGroups)
{
	TPixelGroup DummyGroup;
	if (Index>=pPixelGroups->NumberOfGroups)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_InsertGroupInGroups() Index (%d) is out of range (NumberOfGroups=%d)",
			Index, pPixelGroups->NumberOfGroups);
		return false;
	}
	k_InitGroup(&DummyGroup);
	/* Add a dummy position to the end of position array */
	k_AddGroupToGroups(&DummyGroup,pPixelGroups);
	/* Move groups */
	memmove(&(pPixelGroups->pGroups[Index+1]),&(pPixelGroups->pGroups[Index]),
		sizeof(TPixelGroup)*(pPixelGroups->NumberOfGroups-Index-1));
	/* Insert TPixelGroup structure at Index */
	pPixelGroups->pGroups[Index] = *pPixelGroup;
	if (pPixelGroup->pPos!=NULL)
	{
		/* allocate new memory for the pPos in new TPixelGroup and copy the position values */
		(pPixelGroups->pGroups[Index]).pPos = (T2DInt*)malloc((pPixelGroup->NumberOfPixels*sizeof(T2DInt)));
		memcpy((pPixelGroups->pGroups[Index]).pPos,pPixelGroup->pPos,pPixelGroup->NumberOfPixels*sizeof(T2DInt));
	}
	if (pPixelGroup->pColor!=NULL)
	{
		/* allocate new memory for the pColor in new TPixelGroup and copy color values */
		(pPixelGroups->pGroups[Index]).pColor = (UINT32*)malloc((pPixelGroup->NumberOfPixels*sizeof(UINT32)));
		memcpy((pPixelGroups->pGroups[Index]).pColor,pPixelGroup->pColor,pPixelGroup->NumberOfPixels*sizeof(UINT32));
	}
	/* The total groups variable (pPixelGroups->NumberOfGroups)
		are already updated when inserting the dummygroup above */
	return true;
}

bool k_AddColorsToGroups(const TImage* pSource,TPixelGroups* pPixelGroups)
{
	unsigned int i;
	if ((pSource==NULL) || (pPixelGroups==NULL))
	{
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_AddColorsToGroups() At least one parameter is a NULL pointer");
		return false;
	}
	for(i=0; i<pPixelGroups->NumberOfGroups; i++)
	{
		k_AddColorsToGroup(pSource,&(pPixelGroups->pGroups[i]));
	}
	return true;
}

bool k_RemoveGroup(TPixelGroups *pPixelGroups,unsigned int GroupIndex)
{
	if (pPixelGroups==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR,"k_RemoveGroup() pPixelGroups parameter is NULL");
		return false;
	}
	if (GroupIndex>=pPixelGroups->NumberOfGroups)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_RemoveGroup() Blob index out of range");
		return false;
	}
	if (GroupIndex<pPixelGroups->NumberOfGroups-1)
	{
		/* free the memory in the group to be removed */
		k_EmptyGroup(&(pPixelGroups->pGroups[GroupIndex]));
		/*s0=pPixelGroups->NumberOfGroups-1;*/
		pPixelGroups->pGroups[GroupIndex]=pPixelGroups->pGroups[pPixelGroups->NumberOfGroups-1];
	}
	else
	{
		/* only need to free the memory in the group to be removed */
		k_EmptyGroup(&(pPixelGroups->pGroups[GroupIndex]));
	}
	pPixelGroups->NumberOfGroups--;
	return true;
}

bool k_RemoveGroupSlow(TPixelGroups *pPixelGroups,unsigned int GroupIndex)
{
	if (GroupIndex>=pPixelGroups->NumberOfGroups)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_RemoveGroupSlow() Index (%d) is out of range (NumberOfGroups=%d)",
			GroupIndex, pPixelGroups->NumberOfGroups);
		return false;
	}
	k_EmptyGroup(&(pPixelGroups->pGroups[GroupIndex]));
	/* Move groups */
	memmove(&(pPixelGroups->pGroups[GroupIndex]),&(pPixelGroups->pGroups[GroupIndex+1]),
		sizeof(TPixelGroup)*(pPixelGroups->NumberOfGroups-GroupIndex-1));
	/* Decrement total groups variable */
	pPixelGroups->NumberOfGroups--;
	return true;
}

bool k_GetGroupWithPos(const TPixelGroups* pPixelGroups, unsigned int* pGroupIndex,
					   T2DInt Pos,unsigned int* pPosIndex)
{
	unsigned int i;
	unsigned TempIndex;
	if (pPixelGroups==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GetGroupWithPos() pPixelGroup parameter is NULL");
		return false;
	}
	for (i=0;i<pPixelGroups->NumberOfGroups;i++)
	{
		if (k_PosInGroup(&(pPixelGroups->pGroups[i]),Pos,&TempIndex))
		{
			*pGroupIndex = i;
			if (pPosIndex!=NULL)
				*pPosIndex=TempIndex;
			return true;
		}
	}
	/* if the loope didn't return, the given position is not in any of the groups */
	return false;
}

void k_PrintGroupsInfo(const TPixelGroups* pPixelGroups)
{
	printf("**************** PixelGroups info ********************\n");
	printf(" Number of groups inserted: %d  TPixelGroup structures allocated: %d\n",
		pPixelGroups->NumberOfGroups,pPixelGroups->AllocatedGroups);
}

/************************************************/
/********        Private functions       ********/
/************************************************/

void k_ReAllocGroups(TPixelGroups* pPixelGroups,unsigned int NewSize)
{
	unsigned int i;
	if ((pPixelGroups->pGroups=(TPixelGroup*)realloc(pPixelGroups->pGroups,NewSize*sizeof(TPixelGroup)))!=NULL)
	{
		for(i=pPixelGroups->AllocatedGroups; i<NewSize; i++)
			k_InitGroup(&(pPixelGroups->pGroups[i]));

		pPixelGroups->AllocatedGroups=NewSize; /* internal use */
	}
	else
	{
		k_ShowMessageSimple(IPL_ERROR, "k_ReAllocGroups() Realloc failed");
	}
}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
