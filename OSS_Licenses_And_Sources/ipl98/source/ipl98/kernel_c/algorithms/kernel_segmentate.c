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

#include "kernel_segmentate.h"
#include "../kernel_error.h"
#include <memory.h>
#include <assert.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/************************************************/
/********       Private structures       ********/
/************************************************/

typedef struct {
	/*	A group of pixels found by the blob segmentation algorithm */
	T2DInt First, Last; /* First/Last pixel-position in group */
	/*T2DInt Top, Bottom, Left, Right;*/ /* Most outlying pixels in each direction in blob */
	unsigned int NumberOfPixels; /* Total number of pixels in the group */
} TPixelGroupBlob;

typedef struct {
	/*	This is the type to keep track of more than one pixel group.
		Most images consists of more than one pixel group.*/
	TPixelGroupBlob* pPixelGroup; /* An array of TPixelGroupBlob types */
	unsigned long int NumberOfGroups; /* Actual number of pixelgroups, use
			this value minus 1 to access the last group in the pPixelGroup array */
	UINT32 MaxGroups; /* Used internally to keep track of allocated area for pixelgroups.*/
	unsigned int Width,Height; /* used internally for initial allocation sizes,
			Width of ppNext matrix and Height of ppNext matrix */
	T2DInt** ppNext; /* A matrix which is used as a container for the pixel groups */
} TPixelGroupsBlob;

/************************************************/
/********        Private functions       ********/
/************************************************/

/* See description of functions in bottom of this file */
void k_InitGroupsBlob(TPixelGroupsBlob* pPixelGroups);

void k_FreeGroupsBlob(TPixelGroupsBlob *pPixelGroups);

void k_AllocGroupsBlob(TPixelGroupsBlob* pPixelGroups,unsigned int Width,
						unsigned int Height,UINT32 MaxGroups);

void k_NewBlob(TPixelGroupsBlob *pPixelGroups,int* ThisRow,T2DInt r);

void k_JoinBlobs(TPixelGroupsBlob *pPixelGroups,int* pThisRow,int* pPreviousRow,int s,int t);

#define k_AddPoint(Groups,Row,s,r) do {                                                 \
    Groups->ppNext[Groups->pPixelGroup[s].Last.x][Groups->pPixelGroup[s].Last.y]=r;     \
    Groups->pPixelGroup[s].NumberOfPixels++;                                            \
    Groups->pPixelGroup[s].Last=r;                                                      \
    Row[r.x]=s;                                                                         \
} while (0)

void k_DeriveBlobsInternally(const TImage* pSource, TPixelGroupsBlob *pPixelGroups,
           COLORTYPE Color,UINT8 Threshold, bool EightConnected);

/************************************************/
/********        Public functions        ********/
/************************************************/

bool k_DeriveBlobs(TImage* pSource, TPixelGroups* pPixelGroups,
                    COLORTYPE Color, UINT8 Threshold, CONNECTIVITY Connected, unsigned int MinSize)
{
	T2DInt p;
	TPixelGroupsBlob PixelGroupsBlob; /* internal representation of blobs */
	TPixelGroup PixelGroup; /* used as temporary variable when
								copying to TPixelGroups structure */
	unsigned int i,j;
	T2DInt r,OriginalOrigo;
	bool EightConnected=false;
	if (Connected==EIGHTCONNECTED)
		EightConnected=true;

	k_InitGroup(&PixelGroup);
	k_InitGroupsBlob(&PixelGroupsBlob);
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_DeriveBlobs() Source image is empty");
		return false;
	}
	else if ((pSource->Bits!=1) && (pSource->Bits!=8))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_DeriveBlobs() Source image is %d b/p (must be 1 or 8 b/p)",
			pSource->Bits);
		return false;
	}

	/* Remember original origo */
	OriginalOrigo=pSource->Origo;
	pSource->Origo.x=0;
	pSource->Origo.y=0;

	k_AllocGroupsBlob(&PixelGroupsBlob,pSource->Width,pSource->Height,1000);
	k_DeriveBlobsInternally(pSource,&PixelGroupsBlob,Color,Threshold,EightConnected);
	/* Copy blobs to the TPixelGroups structure */
	k_EmptyGroups(pPixelGroups);
	k_AllocGroups(pPixelGroups,PixelGroupsBlob.NumberOfGroups);
	/* Copy one blob to a TPixelGroup structure */
	for(i=0; i<PixelGroupsBlob.NumberOfGroups; i++)
	{
		if (PixelGroupsBlob.pPixelGroup[i].NumberOfPixels>=MinSize)
		{
			r=PixelGroupsBlob.pPixelGroup[i].First;
			k_AllocGroup(&PixelGroup,PixelGroupsBlob.pPixelGroup[i].NumberOfPixels);
			for(j=0; j<PixelGroupsBlob.pPixelGroup[i].NumberOfPixels; j++)
			{
				/* the code below is the same as k_AddPosToGroup(), but we save
				   time by not calling a function and we save an "if"-statement */
				p.x=(INT16)(r.x-OriginalOrigo.x);
				p.y=(INT16)(r.y-OriginalOrigo.y);
				if (p.x>(PixelGroup.Right).x)
					PixelGroup.Right=p;
				if (p.x<(PixelGroup.Left).x)
					PixelGroup.Left=p;
				if (p.y>(PixelGroup.Bottom).y)
					PixelGroup.Bottom=p;
				if (p.y<(PixelGroup.Top).y)
					PixelGroup.Top=p;
				PixelGroup.pPos[PixelGroup.NumberOfPixels++] = p;
				/* get next pixel */
				r=PixelGroupsBlob.ppNext[r.x][r.y];
			}
			PixelGroup.Color=Color;
			k_AddGroupToGroups(&PixelGroup,pPixelGroups);
		}
	}
	k_EmptyGroup(&PixelGroup);
	k_FreeGroupsBlob(&PixelGroupsBlob); /* always free your TPixelGroupsBlob variable */

	/* reset origo */
	pSource->Origo=OriginalOrigo;
	return true;
}

bool k_BlobExpansion(T2DInt Seed, TImage* pSource, TPixelGroup* pGrp,
					 UINT8 Threshold, CONNECTIVITY Connected)
{
	TImage Img2;
	T2DInt buffer;
	TPixelGroup missing;
	COLORTYPE ColorTp;
	bool ResetBorder=false;
	T2DInt current;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_BlobExpansion() Source image is empty");
		return false;
	}
	else if ((pSource->Bits!=1) && (pSource->Bits!=8))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_BlobExpansion() Source image is %d b/p (must be 1 or 8 b/p)",
			pSource->Bits);
		return false;
	}
	k_InitGroup(&missing);
	k_InitImage(&Img2);
	k_AllocImage(pSource->Width,pSource->Height,1,&Img2);
	k_FlushImage(0,&Img2);
	k_SetBorder(1,0,&Img2);
	if (pSource->Bits==1)
	{
		if (k_GetPixel(Seed.x,Seed.y,pSource)==0)
			ColorTp=LOWCOLOR;
		else
			ColorTp=HIGHCOLOR;
		if (pSource->BorderSize==0)
		{
			k_SetBorder(1,1-k_GetPixel(Seed.x,Seed.y,pSource),pSource);
			ResetBorder=true;
		}
		else
			k_SetBorderColor(1-k_GetPixel(Seed.x,Seed.y,pSource),pSource);		
	}
	else
	{
		/* must be 8 b/p */
		if (k_GetPixel(Seed.x,Seed.y,pSource)>(UINT32)Threshold)
			ColorTp=HIGHCOLOR;
		else
			ColorTp=LOWCOLOR;
		if (pSource->BorderSize==0)
		{
			ColorTp==HIGHCOLOR ? k_SetBorder(1,0,pSource) : k_SetBorder(1,255,pSource);
			ResetBorder=true;
		}
		else
			ColorTp==HIGHCOLOR ? k_SetBorderColor(0,pSource) : k_SetBorderColor(255,pSource);
	}
	/* add seed to missing, which is to be searched */
	k_Add2DPosToGroup(Seed,&missing);
	if (Connected == FOURCONNECTED)
	{
		if (pSource->Bits==1) /* source image 1 b/p */
		{
			while(missing.NumberOfPixels>0)
			{
				int i;
				buffer=missing.pPos[missing.NumberOfPixels-1];
				current=buffer;
				k_RemovePosFromGroupFast(missing.NumberOfPixels-1,&missing);
				for(i=0;i<4;i++)
				{
					switch(i){
					case 0:
						current.y--;
						break;
					case 1:
						current.y+=2;
						break;
					case 2:
						current.x--;
						current.y--;
						break;
					case 3:
						current.x+=2;
						break;
					}
					if (!k_GetPixel1bp(current.x,current.y,Img2))
					{
						k_SetPixel1bp(current.x,current.y,1,Img2);
						if (k_GetPixel1bp(current.x,current.y,*pSource)==(int)ColorTp)
						{
							k_Add2DPosToGroupFast(current,pGrp);
							k_Add2DPosToGroupFast(current,&missing);
						}
					}
				}
			}
		}
		else /* source image 8 b/p */
		{
			while(missing.NumberOfPixels>0)
			{
				int i;
				buffer=missing.pPos[missing.NumberOfPixels-1];
				current=buffer;
				k_RemovePosFromGroupFast(missing.NumberOfPixels-1,&missing);
				for(i=0;i<4;i++)
				{
					switch(i){
					case 0:
						current.y--;
						break;
					case 1:
						current.y+=2;
						break;
					case 2:
						current.x--;
						current.y--;
						break;
					case 3:
						current.x+=2;
						break;
					}
					if (!k_GetPixel1bp(current.x,current.y,Img2))
					{
						k_SetPixel1bp(current.x,current.y,1,Img2);
						if (ColorTp==LOWCOLOR ? k_GetPixel8bp(current.x,current.y,*pSource)<Threshold : 
												k_GetPixel8bp(current.x,current.y,*pSource)>Threshold)
						{
							k_Add2DPosToGroupFast(current,pGrp);
							k_Add2DPosToGroupFast(current,&missing);
						}
					}
				}
			}
		}
	}
	else 
	{
		if (pSource->Bits==1) /* source image 1 b/p */
		{
			while(missing.NumberOfPixels>0)
			{
				int i;
				buffer=missing.pPos[missing.NumberOfPixels-1];
				current=buffer;
				k_RemovePosFromGroupFast(missing.NumberOfPixels-1,&missing);
				for(i=0;i<8;i++)
				{
					switch(i){
					case 0:
						current.x--;
						current.y--;
						break;
					case 1:
						current.y++;
						break;
					case 2:
						current.y++;
						break;
					case 3:
						current.x++;
						break;
					case 4:
						current.y-=2;
						break;
					case 5:
						current.x++;
						break;
					case 6:
						current.y++;
						break;
					case 7:
						current.y++;
						break;
					}
					if (!k_GetPixel1bp(current.x,current.y,Img2))
					{
						k_SetPixel1bp(current.x,current.y,1,Img2);
						if (k_GetPixel1bp(current.x,current.y,*pSource)==(int)ColorTp)
						{
							k_Add2DPosToGroupFast(current,pGrp);
							k_Add2DPosToGroupFast(current,&missing);
						}
					}
				}
			}
		}
		else /* source image 8 b/p */
		{
			while(missing.NumberOfPixels>0)
			{
				int i;
				buffer=missing.pPos[missing.NumberOfPixels-1];
				current=buffer;
				k_RemovePosFromGroupFast(missing.NumberOfPixels-1,&missing);
				for(i=0;i<8;i++)
				{
					switch(i){
					case 0:
						current.x--;
						current.y--;
						break;
					case 1:
						current.y++;
						break;
					case 2:
						current.y++;
						break;
					case 3:
						current.x++;
						break;
					case 4:
						current.y-=2;
						break;
					case 5:
						current.x++;
						break;
					case 6:
						current.y++;
						break;
					case 7:
						current.y++;
						break;
					}
					if (!k_GetPixel1bp(current.x,current.y,Img2))
					{
						k_SetPixel1bp(current.x,current.y,1,Img2);
						if (ColorTp==LOWCOLOR ? k_GetPixel8bp(current.x,current.y,*pSource)<Threshold : 
												k_GetPixel8bp(current.x,current.y,*pSource)>Threshold)
						{
							k_Add2DPosToGroupFast(current,pGrp);
							k_Add2DPosToGroupFast(current,&missing);
						}
					}
				}
			}
		}
	}
	k_GroupUpdateBoundaries(pGrp);
	if (ResetBorder==true)
		k_SetBorderSize(0,pSource);
	k_EmptyGroup(&missing);
	k_EmptyImage(&Img2);
	return true;
}

/************************************************/
/********        Private functions       ********/
/************************************************/

void k_DeriveBlobsInternally(const TImage* pSource, TPixelGroupsBlob *pPixelGroups,
           COLORTYPE Color,UINT8 Threshold, bool EightConnected)
{
    UINT32 x,y,LocalThreshold;
	int n,w,nw,ne;
	bool NotBackground;
	T2DInt r;
	int* pThisRow,*pPreviousRow;
	if(pSource->Bits==1)
		LocalThreshold=1;
	else 
		LocalThreshold=Threshold;

	pPixelGroups->NumberOfGroups=0;
	r.x=0;
	r.y=0;
	pThisRow=(int*)malloc(2*(pSource->Width+1)*sizeof(int));
	pPreviousRow=pThisRow+pSource->Width+1;
	pThisRow[pSource->Width]=-1;
	pPreviousRow[pSource->Width]=-1;
	for (x=0;x<pSource->Width;x++)
	{
		pThisRow[x]=-1;
		pPreviousRow[x]=-1;
		for (y=0;y<pSource->Height;y++)
		{
			pPixelGroups->ppNext[x][y]=r;
		}
	}
	for (y=0;y<pSource->Height;y++)
	{
		w=-1;
		nw=-1;
		r.y=(INT16)y;
		for (x=0;x<pSource->Width;x++)
		{
			r.x=(INT16)x;
			if (pSource->Bits==1)
				NotBackground=((UINT32)k_GetPixel1bp(x,y,(*pSource))<LocalThreshold);
			else /* must be 8 b/p */
				NotBackground=((UINT32)k_GetPixel8bp(x,y,(*pSource))<LocalThreshold);

			if (Color==HIGHCOLOR){
				NotBackground=!NotBackground;
			}
			if (NotBackground)
			{
				if (x>0)
				{
					w=pThisRow[x-1];
					nw=pPreviousRow[x-1];
				};
				n=pPreviousRow[x];
				ne=pPreviousRow[x+1];;
				if (w==-1)
				{
					if (!EightConnected)
					{
						if (n!=-1){
							k_AddPoint(pPixelGroups,pThisRow,n,r);
						}
						else
						{
							k_NewBlob(pPixelGroups,pThisRow,r);
						}
					} 
					else
					{
						if (nw!=-1)
						{
							k_AddPoint(pPixelGroups,pThisRow,nw,r);
						}
						else
						{
							if (n!=-1)
							{
								k_AddPoint(pPixelGroups,pThisRow,n,r);
							}
							else
							{
								if (ne!=-1)
								{
									k_AddPoint(pPixelGroups,pThisRow,ne,r);
								}
								else
								{
									k_NewBlob(pPixelGroups,pThisRow,r);
									
								}
							}
						}
						if ((nw!=-1)&&(n==-1)&&(ne!=-1)&&(nw!=ne))
						{
							k_JoinBlobs(pPixelGroups,pThisRow,pPreviousRow,ne,nw);
						}
					}
				}
				else
				{
					k_AddPoint(pPixelGroups,pThisRow,w,r);
					if ((n!=-1)&&(n!=w)) k_JoinBlobs(pPixelGroups,pThisRow,pPreviousRow,n,w);
					if (EightConnected)
					{
						if ((ne!=-1)&&(ne!=w))
						{
							k_JoinBlobs(pPixelGroups,pThisRow,pPreviousRow,ne,w);
						}
						if ((nw!=-1)&&(nw!=w))
						{
							k_JoinBlobs(pPixelGroups,pThisRow,pPreviousRow,nw,w);
						}
					}
				}
			}
			else 
				pThisRow[x]=-1;
		}
		for (x=0;x<=pSource->Width;x++)
		{
			pPreviousRow[x]=pThisRow[x];
		}
	}
	free(pThisRow);
/*	free(pPreviousRow);*/
}

void k_JoinBlobs(TPixelGroupsBlob *pPixelGroups,int* pThisRow,int* pPreviousRow,int s,int t)
{
	INT32 s0;
	UINT32 i;
	T2DInt r;
	for (i=0;i<pPixelGroups->Width;i++)
	{
		if (pThisRow[i]==s)
			pThisRow[i]=t;
		if (pPreviousRow[i]==s)
			pPreviousRow[i]=t;
	}
	r=pPixelGroups->pPixelGroup[t].Last;
	pPixelGroups->ppNext[r.x][r.y]=pPixelGroups->pPixelGroup[s].First;
	pPixelGroups->pPixelGroup[t].Last=pPixelGroups->pPixelGroup[s].Last;
	pPixelGroups->pPixelGroup[t].NumberOfPixels+=pPixelGroups->pPixelGroup[s].NumberOfPixels;
	if (s<(int)pPixelGroups->NumberOfGroups-1)
	{
		s0=pPixelGroups->NumberOfGroups-1;
		pPixelGroups->pPixelGroup[s]=pPixelGroups->pPixelGroup[s0];
		for (i=0;i<pPixelGroups->Width;i++)
		{
			if (pThisRow[i]==s0)
				pThisRow[i]=s;
			if (pPreviousRow[i]==s0)
				pPreviousRow[i]=s;
		}
	}
	pPixelGroups->NumberOfGroups--;
}

void k_NewBlob(TPixelGroupsBlob *pPixelGroups,int* ThisRow,T2DInt r)
{
	int s0;
	if (pPixelGroups->NumberOfGroups==pPixelGroups->MaxGroups)
	{
		TPixelGroupBlob* Buf=pPixelGroups->pPixelGroup;
		UINT32 i;
		pPixelGroups->pPixelGroup=
			(TPixelGroupBlob*)malloc((pPixelGroups->MaxGroups+1000)*sizeof(TPixelGroupBlob));
		for (i=0;i<pPixelGroups->MaxGroups;i++)
						 pPixelGroups->pPixelGroup[i]=Buf[i];
		pPixelGroups->MaxGroups += 1000;
		free(Buf);
	}
    s0=pPixelGroups->NumberOfGroups;
    pPixelGroups->pPixelGroup[s0].First=r;
    pPixelGroups->pPixelGroup[s0].Last=r;
    pPixelGroups->pPixelGroup[s0].NumberOfPixels=1;
    ThisRow[r.x]=s0;
    pPixelGroups->NumberOfGroups++;
}

/*	Sets all the pointers in pInfo to NULL.allways call
	this function when creating a new TPixelGroupsBlob variable. */
void k_InitGroupsBlob(TPixelGroupsBlob* pPixelGroups)
{
	pPixelGroups->pPixelGroup=NULL;
	pPixelGroups->ppNext=NULL;
	pPixelGroups->Width=0;
	pPixelGroups->Height=0;
}

/*	Deletes all allocated memory for PixelGroups. Always call
	this function when leaving a scope where a TPixelGroupsBlob variable
	were created and used. */
void k_FreeGroupsBlob(TPixelGroupsBlob* pPixelGroups)
{
	if((pPixelGroups->Width>0) && (pPixelGroups->ppNext[0]!=NULL))
		free(pPixelGroups->ppNext[0]);
	if (pPixelGroups->ppNext != NULL)
		free(pPixelGroups->ppNext);
	if (pPixelGroups->pPixelGroup != NULL)
		free(pPixelGroups->pPixelGroup);
}

/*	Allocate memory for the matrix to hold information about the pixel groups.
	@param Width Width of matrix to hold the groups (usually the 
		width of a source image).
	@param Height Height of matrix to hold the groups (usually the
		width of a source image).
	@param MaxBlobs The number of expected blobs, if more are found the
		   function using this TPixelGroupsBlob dynamically expands this number.
		   The functions using TPixelGroupsBlob are fastest if MaxBlobs is greater
		   than the actual number of groups in image. */
void k_AllocGroupsBlob(TPixelGroupsBlob* pPixelGroups,unsigned int Width,
						unsigned int Height,UINT32 MaxGroups)
{
	unsigned int x;
	/* start with freeing the old data (if any!) */
	k_FreeGroupsBlob(pPixelGroups);
	pPixelGroups->Width=Width;
	pPixelGroups->Height=Height;
	pPixelGroups->MaxGroups=MaxGroups;
	pPixelGroups->NumberOfGroups=0;	
	pPixelGroups->pPixelGroup=(TPixelGroupBlob*)malloc(MaxGroups*sizeof(TPixelGroupBlob));
	pPixelGroups->ppNext=(T2DInt**)malloc(Width*sizeof(T2DInt*));
	pPixelGroups->ppNext[0]=(T2DInt*)malloc(Width*Height*sizeof(T2DInt));
	for (x=1;x<Width;x++)
		pPixelGroups->ppNext[x]=pPixelGroups->ppNext[0]+Height*x;
}

typedef struct {
	T2DInt *pPos;
	int NumberOfPixels;
	int MaxPixels;
	COLORTYPE PixelColor;
	ROTATION RotationDirection;
	COLORTYPE SurroundedColor;
	CONNECTIVITY Connected;
	T2DInt Top,Bottom,Left,Right;
}TContour;

void k_InitContour(TContour *Contour)
{
	Contour->pPos=NULL;
}

void k_AllocContour(TContour *Contour,int MaxPixels)
{
	Contour->pPos=(T2DInt*)calloc(MaxPixels,sizeof(T2DInt));
	Contour->MaxPixels=MaxPixels;
	Contour->RotationDirection=CLOCKWISE;
	Contour->SurroundedColor=LOWCOLOR;
}

void k_DeallocContour(TContour* Contour)
{
	free(Contour->pPos);
}

void k_CopyContourToPixelGroup(TPixelGroup* pPixelGroup,TContour* pContour)
{
	k_AllocGroup(pPixelGroup,pContour->NumberOfPixels);
	pPixelGroup->NumberOfPixels=pContour->NumberOfPixels;
	pPixelGroup->Top=pContour->Top;
	pPixelGroup->Bottom=pContour->Bottom;
	pPixelGroup->Left=pContour->Left;
	pPixelGroup->Right=pContour->Right;
	memcpy(pPixelGroup->pPos,pContour->pPos,pContour->NumberOfPixels*sizeof(T2DInt));
}

bool k_FindAndFollowHighContour(TImage* pSource, T2DInt Start,
								DIRECTION SearchDirection, CONNECTIVITY Connected,
								TPixelGroup* pPixelGroup,ROTATION* pRotationDirection,
								COLORTYPE* pSurroundedColor)
{
	TContour Contour;
	T2DInt p,First,Top,Left,Bottom,Right;
	int i,k,Rot;
	bool found;
	DIRECTION Dir,StartDir;
	T2DInt* Buf;
	bool ResetBorder=false;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_FindAndFollowHighContour() Source image is empty - doing nothing");
		return false;
	}
	if (pSource->Bits==24)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContour() Source image is 24 b/p - not supported");
		return false;
	}
	if ((SearchDirection!=NORTH) && (SearchDirection!=EAST) && (SearchDirection!=SOUTH) && 
		(SearchDirection!=WEST))
	{
		k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContour() Wrong search direction");
		return false;
	}
	/* add black border if no border is present */
	if (pSource->BorderSize==0)
	{
		k_SetBorder(1,0,pSource);
		ResetBorder=true;
	}
	k_AllocContour(&Contour,1000); /* initialise the TContour structure */
	p=Start;
	found=false;
	switch (SearchDirection)
	{
	case NORTHEAST: case SOUTHEAST: case SOUTHWEST: case NORTHWEST: {
		k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContour() Internal error");
		return false;
	}
	case NORTH:
	do {
	     found=((k_GetPixel(p.x,p.y,pSource)==0));/* looks for low */
		 if (!found)p.y--;
	   }
    while ((p.y>=1)&&(!found));
	if (found)
	do {
		 found=false;
         if (k_GetPixel(p.x,p.y-1,pSource))
		 {
		 	found=true;
		 	Dir=WEST;
         }
         p.y--;
	  	}
    while ((!found)&&(p.y>=0));
	break;
	case EAST:
	do {
	     found=((k_GetPixel(p.x,p.y,pSource)==0));
		 if (!found)p.x++;
	   }
	while ((p.x<=(INT16)(pSource->Width-2))&&(!found));
	if (found)
	do {
	 	  found=false;
          if (k_GetPixel(p.x+1,p.y,pSource))
		  {
		  	found=true;
		  	Dir=NORTH;
		  }
          p.x++;
	 	}
    while (!found&&(p.x<=(INT16)(pSource->Width-1)));
			break;
	case SOUTH:
	do {
		found=((k_GetPixel(p.x,p.y,pSource)==0));
		if (!found)
			p.y++;
	   }
    while ((p.y<=(INT16)(pSource->Height-2))&&(!found));
	if (found)
	do {
	      found=false;
          if (k_GetPixel(p.x,p.y+1,pSource))
		  {
		  	found=true;
		  	Dir=EAST;
		  }
          p.y++;
       }
    while ((!found)&&(p.y<=(INT16)(pSource->Height-1)));
			break;
	case WEST:
	do {
		found=((k_GetPixel(p.x,p.y,pSource)==0));
		if (!found)
			p.x--;
	   }
    while ((p.x>=1)&&(!found));
	if (found)
	do {
		found=false;
        if (k_GetPixel(p.x-1,p.y,pSource))
		{
			found=true;
			Dir=SOUTH;
		}
        p.x--;
		}
    while ((!found)&&(p.y>=0));
			break;
    }
	if (!found)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContour() No contour found");
		if (ResetBorder==true)
			k_SetBorderSize(0,pSource);
		return false;
	}
	else
	{
	  StartDir=Dir;
	  First=p;
	  i=0;
	  Rot=0;
	  Contour.pPos[0]=p;
	  Top=p;
	  Bottom=p;
	  Left=p;
	  Right=p;
      if (Connected==FOURCONNECTED)
      {
	   do
        {
		switch (Dir){
		case NORTHEAST: case SOUTHEAST: case SOUTHWEST: case NORTHWEST: {
			k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContour() Internal error");
			return false;
		}
		case NORTH:
			if (!(k_GetPixel(p.x,p.y-1,pSource)))
			{
				Dir=EAST;
				Rot++;
			}
			else
			{
				if(k_GetPixel(p.x-1,p.y-1,pSource))
				{
					Dir=WEST;
					p.y--;
					Rot--;
					i++;
                    Contour.pPos[i]=p;
					p.x--;
					i++;
                    Contour.pPos[i]=p;
					if (p.x<Left.x)
						Left=p;
					if (p.y<Top.y)
						Top=p;
				}
				else
				{
					p.y--;
					i++;
					Contour.pPos[i]=p;
					if (p.y<Top.y)
						Top=p;
				}
			}
			break;
		case EAST:
			if (!(k_GetPixel(p.x+1,p.y,pSource)))
			{
				Dir=SOUTH;
				Rot++;
			}
			else
			{
				if (k_GetPixel(p.x+1,p.y-1,pSource))
				{
					Dir=NORTH;
					p.x++;
					Rot--;
					i++;
					Contour.pPos[i]=p;
					p.y--;
					i++;
					Contour.pPos[i]=p;
					if (p.x>Right.x)
						Right=p;
					if (p.y<Top.y)
						Top=p;
				}
				else
				{
					p.x++;i++;
					Contour.pPos[i]=p;
					if (p.x>Right.y)
						Right=p;
				}
			}
			break;
		case SOUTH:
			if (!(k_GetPixel(p.x,p.y+1,pSource)))
			{
				Dir=WEST;
				Rot++;
			}
			else
			{
				if (k_GetPixel(p.x+1,p.y+1,pSource))
				{
					Dir=EAST;
					p.y++;
					Rot--;
					i++;
					Contour.pPos[i]=p;
					p.x++;
					i++;
					Contour.pPos[i]=p;
					if (p.x>Right.x)
						Right=p;
					if (p.y>Bottom.y)
						Bottom=p;
				}
				else
				{
					p.y++;
					i++;
					Contour.pPos[i]=p;
					if (p.y>Bottom.y)
						Bottom=p;
				}
			}
			break;
		case WEST:
			if (!(k_GetPixel(p.x-1,p.y,pSource)))
			{
				Dir=NORTH;
				Rot++;
			}
			else
			{
				if (k_GetPixel(p.x-1,p.y+1,pSource))
				{
					Dir=SOUTH;
					p.x--;
					Rot--;
					i++;
					Contour.pPos[i]=p;
					p.y++;
					i++;
					Contour.pPos[i]=p;
					if (p.x<Left.x)
						Left=p;
					if (p.y>Bottom.y)
						Bottom=p;
				}
				else
				{
					p.x--;
					i++;
					Contour.pPos[i]=p;
					if (p.x<Left.x)
						Left=p;
				}
			}
         }
       	 if (i>Contour.MaxPixels-3)
         {
		 Buf=(T2DInt*)calloc(Contour.MaxPixels,sizeof(T2DInt));
		 for (k=0;k<=i;k++) /* int k changed to k by edr */
			Buf[k]=Contour.pPos[k];
	    	Contour.MaxPixels+=1000;
	    	Contour.pPos=(T2DInt*)calloc(Contour.MaxPixels,sizeof(T2DInt));
	    	for (k=0;k<=i;k++) /* int k changed to k by edr */
	    	Contour.pPos[k]=Buf[k];
	    	free(Buf);
	      }
        } while(!((p.x==First.x)&&(p.y==First.y)&&(Dir==StartDir)));/* end do */
      } /*end FOURCONNECTED*/
  	  else
      {
      do
 	    {
			   switch (Dir){
			   case NORTHEAST: case SOUTHEAST: case SOUTHWEST: case NORTHWEST: {
				   k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContour() Internal error");
				   return false;
			   }
			   case NORTH:
				   if (k_GetPixel(p.x-1,p.y-1,pSource))
				   {
					   Dir=WEST;
					   p.x--;
					   p.y--;
					   i++;
					   Contour.pPos[i]=p;
					   Rot--;
					   if (p.x<Left.x)
						   Left=p;
					   if (p.y<Top.y)
						   Top=p;
				   }
				   else if (k_GetPixel(p.x,p.y-1,pSource))
				   {
					   p.y--;
					   i++;
					   Contour.pPos[i]=p;
					   if (p.y<Top.y)
						   Top=p;
				   }
				   else
				   {
					   Dir=EAST;
					   Rot++;
				   }
				   break;
			   case EAST:
				   if (k_GetPixel(p.x+1,p.y-1,pSource))
				   {
				   	   Dir=NORTH;
                       p.x++;
				   	   p.y--;
				   	   i++;
				   	   Contour.pPos[i]=p;
				   	   Rot--;
				   	   if (p.x>Right.x)
				   		   Right=p;
				   	   if (p.y<Top.y)
				   		   Top=p;
				    }
				    else if (k_GetPixel(p.x+1,p.y,pSource))
				    {
				   	   p.x++;i++;
				   	   Contour.pPos[i]=p;
				   	   if (p.x>Right.x)
				   		   Right=p;
				    }
				    else
				    {
				   	   Dir=SOUTH;
				   	   Rot++;
				    }
				    break;
			   case SOUTH:
			   	    if (k_GetPixel(p.x+1,p.y+1,pSource))
					{
					   Dir=EAST;
					   p.x++;
					   p.y++;
					   i++;
					   Contour.pPos[i]=p;
					   Rot--;
					   if (p.x>Right.x)
						   Right=p;
					   if (p.y>Bottom.y)
					   Bottom=p;
					   }
					    else if (k_GetPixel(p.x,p.y+1,pSource))
					   {
					   p.y++;
					   i++;
					   Contour.pPos[i]=p;
					   if (p.y>Bottom.y)
					   Bottom=p;
					   }
					   else
                       {
					   Dir=WEST;
					   Rot++;
					   }
					   break;
			   case WEST:
			    	   if (k_GetPixel(p.x-1,p.y+1,pSource))
					   {
				    	   Dir=SOUTH;
					   	   p.x--;
					   	   p.y++;
					   	   i++;
					   	   Contour.pPos[i]=p;
					   	   Rot--;
					   	   if (p.x<Left.x)
					   		   Left=p;
					   	   if (p.y>Bottom.y)
					   		   Bottom=p;
					   }
					   else if (k_GetPixel(p.x-1,p.y,pSource))
					   {
					       p.x--;
					   	   i++;
					   	   Contour.pPos[i]=p;
					   	   if (p.x<Left.x)
					   		   Left=p;
					   }
					   else
					   {
					   	   Dir=NORTH;
					   	   Rot++;
					   }
               }
       	    if (i>Contour.MaxPixels-3)
            {
		    Buf=(T2DInt*)calloc(Contour.MaxPixels,sizeof(T2DInt));
		    for (k=0;k<=i;k++) /* int k changed to k by edr */
			Buf[k]=Contour.pPos[k];
	    	Contour.MaxPixels+=1000;
	    	Contour.pPos=(T2DInt*)calloc(Contour.MaxPixels,sizeof(T2DInt));
	    	for (k=0;k<=i;k++) /* int k changed to k by edr */
	    	Contour.pPos[k]=Buf[k];
	    	free(Buf);
	        }
        }
       while(!((p.x==First.x)&&(p.y==First.y)&&(Dir==StartDir)));/* end do */
    } /*end EIGHTCONNECTED*/
  if (Rot==4)
  {
	  Contour.RotationDirection=CLOCKWISE;
	  Contour.SurroundedColor=HIGHCOLOR;
  }
  if (Rot==-4)
  {
	  Contour.RotationDirection=ANTICLOCKWISE;
	  Contour.SurroundedColor=LOWCOLOR;
  }
  Contour.Connected=Connected;
  Contour.Top=Top;
  Contour.Bottom=Bottom;
  Contour.Right=Right;
  Contour.Left=Left;
  if (i==0)
	  Contour.NumberOfPixels=1;
  else
	  Contour.NumberOfPixels=i;
  Contour.PixelColor=HIGHCOLOR;
  /* Copy all results to the given parameters */
  *pRotationDirection=Contour.RotationDirection;
  *pSurroundedColor=Contour.SurroundedColor;
  pPixelGroup->Top=Contour.Top;
  pPixelGroup->Bottom=Contour.Bottom;
  pPixelGroup->Right=Contour.Right;
  pPixelGroup->Left=Contour.Left;
  k_CopyContourToPixelGroup(pPixelGroup,&Contour);
  k_DeallocContour(&Contour);
  if (ResetBorder==true)
	k_SetBorderSize(0,pSource);
  return true;

 } /* end else */
}

bool k_FindAndFollowLowContour(TImage* pSource, T2DInt Start,
							   DIRECTION SearchDirection, CONNECTIVITY Connected,
							   TPixelGroup* pPixelGroup,ROTATION* pRotationDirection,
								COLORTYPE* pSurroundedColor)
{
	TContour Contour;
	T2DInt p,First,Top,Left,Bottom,Right;
	int i,k,Rot;
	bool found;
	char Dir,StartDir;
	T2DInt* Buf;
	bool ResetBorder=false;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_FindAndFollowHighContour() Source image is empty - doing nothing");
		return false;
	}
	if (pSource->Bits==24)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContour() Source image is 24 b/p - not supported");
		return false;
	}
	if ((SearchDirection!=NORTH) && (SearchDirection!=EAST) && (SearchDirection!=SOUTH) && 
		(SearchDirection!=WEST))
	{
		k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContour() Wrong search direction");
		return false;
	}
	/* add white border if no border is present */
	if (pSource->BorderSize==0)
	{
		ResetBorder=true;
		if (pSource->Bits==1)
			k_SetBorder (1,1,pSource); /* Set color to 1 */
		else
			k_SetBorder (1,255,pSource); /* Set color to 255 */
	}
	k_AllocContour(&Contour,1000); /* initialise the TContour structure */
	p=Start;
	found=false;
	switch (SearchDirection)
	{
	case NORTHEAST: case SOUTHEAST: case SOUTHWEST: case NORTHWEST: {
		k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContour() Internal error");
		return false;
	}
	case NORTH:
	do {
	     found=(!(k_GetPixel(p.x,p.y,pSource)==0));/* looks for low */
		 if (!found)p.y--;
	   }
    while ((p.y>=1)&&(!found));
	if (found)
	do {
		 found=false;
         if (!(k_GetPixel(p.x,p.y-1,pSource)))
		 {
		 	found=true;
		 	Dir=WEST;
         }
         p.y--;
	  	}
    while ((!found)&&(p.y>=0));
	break;
	case EAST:
	do {
	     found=(!(k_GetPixel(p.x,p.y,pSource)==0));
		 if (!found)p.x++;
	   }
	while ((p.x<=(INT16)(pSource->Width-2))&&(!found));
	if (found)
	do {
	 	  found=false;
          if (!(k_GetPixel(p.x+1,p.y,pSource)))
		  {
		  		found=true;
		  		Dir=NORTH;
		  }
          p.x++;
	 	}
    while (!found&&(p.x<=(INT16)(pSource->Width-1)));
			break;
	case SOUTH:
	do {
		found=(!(k_GetPixel(p.x,p.y,pSource)==0));
		if (!found)
			p.y++;
	   }
    while ((p.y<=(INT16)(pSource->Height-2))&&(!found));
	if (found)
	do {
	      found=false;
          if (!(k_GetPixel(p.x,p.y+1,pSource)))
		  {
		  		found=true;
		  		Dir=EAST;
		  }
          p.y++;
		}
    while ((!found)&&(p.y<=(INT16)(pSource->Height-1)));
			break;
	case WEST:
	do {
		found=(!(k_GetPixel(p.x,p.y,pSource)==0));
		if (!found)
			p.x--;
	   }
    while ((p.x>=1)&&(!found));
	if (found)
	do {
		found=false;
        if (!(k_GetPixel(p.x-1,p.y,pSource)))
		{
			found=true;
			Dir=SOUTH;
		}
        p.x--;
		}
    while ((!found)&&(p.x>=0)); // bug fixed in v. 2.12, before p.y>=0 (edr)
			break;
    }
	if (!found)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowLowContour() No contour found");
		if (ResetBorder==true)
			k_SetBorderSize(0,pSource);
		return false;
	}
	else
	{
	   StartDir=Dir;
	   First=p;
	   i=0;
	   Rot=0;
	   Contour.pPos[0]=p;
	   Top=p;
	   Bottom=p;
	   Left=p;
	   Right=p;
       if (Connected==FOURCONNECTED)
       {
         do
         {
		   switch (Dir)
           {
		   case NORTH:
		    	if ((k_GetPixel(p.x,p.y-1,pSource)))
			    {
			  	    Dir=EAST;
				    Rot++;
			    }
			    else
			    {
				  if (!(k_GetPixel(p.x-1,p.y-1,pSource)))
				  {
					Dir=WEST;
					p.y--;
					Rot--;
					i++;
                    Contour.pPos[i]=p;
					p.x--;
					i++;
                    Contour.pPos[i]=p;
					if (p.x<Left.x)
						Left=p;
					if (p.y<Top.y)
						Top=p;
				  }
				  else
				  {
					p.y--;
					i++;
					Contour.pPos[i]=p;
					if (p.y<Top.y)
						Top=p;
				  }
                }
		   break;
		   case EAST:
		        if ((k_GetPixel(p.x+1,p.y,pSource)))
			    {
				    Dir=SOUTH;
				    Rot++;
			    }
		    	else
		    	{
		    	  if (!(k_GetPixel(p.x+1,p.y-1,pSource)))
				  {
					Dir=NORTH;
					p.x++;
					Rot--;
					i++;
					Contour.pPos[i]=p;
					p.y--;
					i++;
					Contour.pPos[i]=p;
					if (p.x>Right.x)
						Right=p;
					if (p.y<Top.y)
						Top=p;
				  }
				  else
				  {
					p.x++;i++;
					Contour.pPos[i]=p;
					if (p.x>Right.y)
						Right=p;
				  }
                }
		   break;
		   case SOUTH:
		    	if ((k_GetPixel(p.x,p.y+1,pSource)))
			    {
				    Dir=WEST;
				    Rot++;
		    	}
		    	else
		    	{
				if (!(k_GetPixel(p.x+1,p.y+1,pSource)))
				  {
					Dir=EAST;
					p.y++;
					Rot--;
					i++;
					Contour.pPos[i]=p;
					p.x++;
					i++;
					Contour.pPos[i]=p;
					if (p.x>Right.x)
						Right=p;
					if (p.y>Bottom.y)
						Bottom=p;
				  }
				  else
			      {
					p.y++;
					i++;
					Contour.pPos[i]=p;
					if (p.y>Bottom.y)
						Bottom=p;
				  }
                }
		   break;
		   case WEST:
		    	if ((k_GetPixel(p.x-1,p.y,pSource)))
			    {
				    Dir=NORTH;
				    Rot++;
			    }
			    else
			    {
				if (!(k_GetPixel(p.x-1,p.y+1,pSource)))
			      {
					Dir=SOUTH;
					p.x--;
					Rot--;
					i++;
					Contour.pPos[i]=p;
					p.y++;
					i++;
					Contour.pPos[i]=p;
					if (p.x<Left.x)
						Left=p;
					if (p.y>Bottom.y)
						Bottom=p;
				  }
				  else
				  {
					p.x--;
					i++;
					Contour.pPos[i]=p;
					if (p.x<Left.x)
						Left=p;
				  }
                }
		   } /*end SWITCH*/
           if (i>Contour.MaxPixels-3)
             {
		     Buf=(T2DInt*)calloc(Contour.MaxPixels,sizeof(T2DInt));
		     for (k=0;k<=i;k++) /* int k changed to k by edr */
			 Buf[k]=Contour.pPos[k];
		     Contour.MaxPixels+=1000;
		     Contour.pPos=(T2DInt*)calloc(Contour.MaxPixels,sizeof(T2DInt));
		     for (k=0;k<=i;k++) /* int k changed to k by edr */
		     Contour.pPos[k]=Buf[k];
		     free(Buf);
	         }
         }
         while(!((p.x==First.x)&&(p.y==First.y)&&(Dir==StartDir)));/* end do */
     } /*end FOURCONNECTED*/
     else
     {
	   do
	   {
	     switch (Dir)
         {
		   case NORTH:
  			   if (!(k_GetPixel(p.x-1,p.y-1,pSource)))
  			     {
  				   Dir=WEST;
  				   p.x--;
  				   p.y--;
  				   i++;
  				   Contour.pPos[i]=p;
  				   Rot--;
  				   if (p.x<Left.x)
				   	   Left=p;
				   	   if (p.y<Top.y)
				   	   Top=p;
				 }
				 else if (!(k_GetPixel(p.x,p.y-1,pSource)))
				    {
				 	   p.y--;
				 	   i++;
				 	   Contour.pPos[i]=p;
				 	   if (p.y<Top.y)
				 		   Top=p;
				    }
				    else
				    {
				       Dir=EAST;
				 	   Rot++;
		            }
		   break;
		   case EAST:
				   if (!(k_GetPixel(p.x+1,p.y-1,pSource)))
				     {
				   	   Dir=NORTH;
				   	   p.x++;
				   	   p.y--;
				   	   i++;
				   	   Contour.pPos[i]=p;
				   	   Rot--;
				   	   if (p.x>Right.x)
				   		   Right=p;
				   	   if (p.y<Top.y)
				   		   Top=p;
				     } else if (!(k_GetPixel(p.x+1,p.y,pSource)))
				  	    {
				  		   p.x++;i++;
				  		   Contour.pPos[i]=p;
				  		   if (p.x>Right.x)
				  			   Right=p;
				  	    }
						else
						{
						   Dir=SOUTH;
						   Rot++;
		                }
		   break;
		   case SOUTH:
				   if (!(k_GetPixel(p.x+1,p.y+1,pSource)))
				       {
				    	   Dir=EAST;
						   p.x++;
						   p.y++;
						   i++;
						   Contour.pPos[i]=p;
						   Rot--;
						   if (p.x>Right.x)
							   Right=p;
						   if (p.y>Bottom.y)
							   Bottom=p;
					   }
					   else if (!(k_GetPixel(p.x,p.y+1,pSource)))
						   {
							   p.y++;
							   i++;
							   Contour.pPos[i]=p;
							   if (p.y>Bottom.y)
								   Bottom=p;
						   }
						   else
                           {
							   Dir=WEST;
							   Rot++;
		                   }
		   break;
		   case WEST:
		   		   if (!(k_GetPixel(p.x-1,p.y+1,pSource)))
		   			   {
		   				   Dir=SOUTH;
		   				   p.x--;
		   				   p.y++;
		   				   i++;
		   				   Contour.pPos[i]=p;
		   				   Rot--;
		   				   if (p.x<Left.x)
		   					   Left=p;
		   				   if (p.y>Bottom.y)
		   					   Bottom=p;
		   			   }
		   			   else if (!(k_GetPixel(p.x-1,p.y,pSource)))
						   {
							   p.x--;
							   i++;
							   Contour.pPos[i]=p;
							   if (p.x<Left.x)
								   Left=p;
						   }
						   else
						   {
							   Dir=NORTH;
							   Rot++;
						   }
           } /*end SWITCH*/
           if (i>Contour.MaxPixels-3)
             {
		     Buf=(T2DInt*)calloc(Contour.MaxPixels,sizeof(T2DInt));
		     for (k=0;k<=i;k++) /* int k changed to k by edr */
			 Buf[k]=Contour.pPos[k];
		     Contour.MaxPixels+=1000;
		     Contour.pPos=(T2DInt*)calloc(Contour.MaxPixels,sizeof(T2DInt));
		     for (k=0;k<=i;k++) /* int k changed to k by edr */
		    	Contour.pPos[k]=Buf[k];
		     free(Buf);
   	        }
         }
     while(!((p.x==First.x)&&(p.y==First.y)&&(Dir==StartDir)));/* end do */
  }/*end EIGHTCONNECTED*/
  if (Rot==4)
  {
	  Contour.RotationDirection=CLOCKWISE;
	  Contour.SurroundedColor=HIGHCOLOR;
  }
  if (Rot==-4)
  {
	  Contour.RotationDirection=ANTICLOCKWISE;
	  Contour.SurroundedColor=LOWCOLOR;
  }
  Contour.Connected=Connected;
  Contour.Top=Top;
  Contour.Bottom=Bottom;
  Contour.Right=Right;
  Contour.Left=Left;
  if (i==0)
	  Contour.NumberOfPixels=1;
  else
	  Contour.NumberOfPixels=i;

  Contour.PixelColor=LOWCOLOR;
  /* Copy all results to the given parameters */
  *pRotationDirection=Contour.RotationDirection;
  *pSurroundedColor=Contour.SurroundedColor;
  pPixelGroup->Top=Contour.Top;
  pPixelGroup->Bottom=Contour.Bottom;
  pPixelGroup->Right=Contour.Right;
  pPixelGroup->Left=Contour.Left;
  k_CopyContourToPixelGroup(pPixelGroup,&Contour);
  k_DeallocContour(&Contour);
  if (ResetBorder==true)
	k_SetBorderSize(0,pSource);
  return true;
 } /* end else */
}

bool k_FindAndFollowHighContourWithThreshold(TImage* pSource, T2DInt Start, unsigned int Threshold,
											 DIRECTION SearchDirection, CONNECTIVITY Connected,
											 TPixelGroup* pPixelGroup,ROTATION* pRotationDirection,
											 COLORTYPE* pSurroundedColor)
{
	TContour Contour;
	T2DInt p,First,Top,Left,Bottom,Right;
	int i,k,Rot;
	bool found;
	DIRECTION Dir,StartDir;
	T2DInt* Buf;
	bool ResetBorder=false;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_FindAndFollowHighContourWithThreshold() Source image is empty - doing nothing");
		return false;
	}
	if (pSource->Bits==24)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContourWithThreshold() Source image is 24 b/p - not supported");
		return false;
	}
	if ((SearchDirection!=NORTH) && (SearchDirection!=EAST) && (SearchDirection!=SOUTH) &&
		(SearchDirection!=WEST))
	{
		k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContour() Wrong search direction");
		return false;
	}
	/* add black border if no border is present */
	if (pSource->BorderSize==0)
	{
		k_SetBorder (1,0,pSource);
		ResetBorder=true;
	}
	k_AllocContour(&Contour,1000); /* initialise the TContour structure */
	p=Start;
	found=false;
	switch (SearchDirection)
	{
	case NORTHEAST: case SOUTHEAST: case SOUTHWEST: case NORTHWEST: {
		k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContour() Internal error");
		return false;
	}
	case NORTH:
		do {
			found=((k_GetPixel(p.x,p.y,pSource)<=Threshold));/* looks for low */
			if (!found)p.y--;
		}
		while ((p.y>=1)&&(!found));
		if (found)
			do {
				found=false;
				if (k_GetPixel(p.x,p.y-1,pSource)>Threshold)
				{
					found=true;
					Dir=WEST;
				}
				p.y--;
			}
			while ((!found)&&(p.y>=0));
			break;
	case EAST:
		do {
			found=((k_GetPixel(p.x,p.y,pSource)<=Threshold));
			if (!found)p.x++;
		}
		while ((p.x<=(INT16)(pSource->Width-2))&&(!found));
		if (found)
			do {
				found=false;
				if (k_GetPixel(p.x+1,p.y,pSource)>Threshold)
				{
					found=true;
					Dir=NORTH;
				}
				p.x++;
			}
			while (!found&&(p.x<=(INT16)(pSource->Width-1)));
			break;
	case SOUTH:
		do {
			found=((k_GetPixel(p.x,p.y,pSource)<=Threshold));
			if (!found)
				p.y++;
		}
		while ((p.y<=(INT16)(pSource->Height-2))&&(!found));
		if (found)
			do {
				found=false;
				if (k_GetPixel(p.x,p.y+1,pSource)>Threshold)
				{
					found=true;
					Dir=EAST;
				}
				p.y++;
			}
			while ((!found)&&(p.y<=(INT16)(pSource->Height-1)));
			break;
	case WEST:
		do {
			found=((k_GetPixel(p.x,p.y,pSource)<=Threshold));
			if (!found)
				p.x--;
		}
		while ((p.x>=1)&&(!found));
		if (found)
			do {
				found=false;
				if (k_GetPixel(p.x-1,p.y,pSource)>Threshold)
				{
					found=true;
					Dir=SOUTH;
				}
				p.x--;
			}
			while ((!found)&&(p.y>=0));
			break;
    }
	if (!found)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContour() No contour found");
		if (ResetBorder==true)
			k_SetBorderSize(0,pSource);
		return false;
	}
	else
	{
		StartDir=Dir;
		First=p;
		i=0;
		Rot=0;
		Contour.pPos[0]=p;
		Top=p;
		Bottom=p;
		Left=p;
		Right=p;
		if (Connected==FOURCONNECTED)
		{
			do
			{
				switch (Dir) {
				case NORTHEAST: case SOUTHEAST: case SOUTHWEST: case NORTHWEST: {
					k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContour() Internal error");
					return false;
				}
				case NORTH:
					if (k_GetPixel(p.x,p.y-1,pSource)<=Threshold)
					{
						Dir=EAST;
						Rot++;
					}
					else
					{
						if(k_GetPixel(p.x-1,p.y-1,pSource)>Threshold)
						{
							Dir=WEST;
							p.y--;
							Rot--;
							i++;
							Contour.pPos[i]=p;
							p.x--;
							i++;
							Contour.pPos[i]=p;
							if (p.x<Left.x)
								Left=p;
							if (p.y<Top.y)
								Top=p;
						}
						else
						{
							p.y--;
							i++;
							Contour.pPos[i]=p;
							if (p.y<Top.y)
								Top=p;
						}
					}
					break;
				case EAST:
					if (k_GetPixel(p.x+1,p.y,pSource)<=Threshold)
					{
						Dir=SOUTH;
						Rot++;
					}
					else
					{
						if (k_GetPixel(p.x+1,p.y-1,pSource)>Threshold)
						{
							Dir=NORTH;
							p.x++;
							Rot--;
							i++;
							Contour.pPos[i]=p;
							p.y--;
							i++;
							Contour.pPos[i]=p;
							if (p.x>Right.x)
								Right=p;
							if (p.y<Top.y)
								Top=p;
						}
						else
						{
							p.x++;i++;
							Contour.pPos[i]=p;
							if (p.x>Right.y)
								Right=p;
						}
					}
					break;
				case SOUTH:
					if (k_GetPixel(p.x,p.y+1,pSource)<=Threshold)
					{
						Dir=WEST;
						Rot++;
					}
					else
					{
						if (k_GetPixel(p.x+1,p.y+1,pSource)>Threshold)
						{
							Dir=EAST;
							p.y++;
							Rot--;
							i++;
							Contour.pPos[i]=p;
							p.x++;
							i++;
							Contour.pPos[i]=p;
							if (p.x>Right.x)
								Right=p;
							if (p.y>Bottom.y)
								Bottom=p;
						}
						else
						{
							p.y++;
							i++;
							Contour.pPos[i]=p;
							if (p.y>Bottom.y)
								Bottom=p;
						}
					}
					break;
				case WEST:
					if (k_GetPixel(p.x-1,p.y,pSource)<=Threshold)
					{
						Dir=NORTH;
						Rot++;
					}
					else
					{
						if (k_GetPixel(p.x-1,p.y+1,pSource)>Threshold)
						{
							Dir=SOUTH;
							p.x--;
							Rot--;
							i++;
							Contour.pPos[i]=p;
							p.y++;
							i++;
							Contour.pPos[i]=p;
							if (p.x<Left.x)
								Left=p;
							if (p.y>Bottom.y)
								Bottom=p;
						}
						else
						{
							p.x--;
							i++;
							Contour.pPos[i]=p;
							if (p.x<Left.x)
								Left=p;
						}
					}
         }
       	 if (i>Contour.MaxPixels-3)
         {
			 Buf=(T2DInt*)calloc(Contour.MaxPixels,sizeof(T2DInt));
			 for (k=0;k<=i;k++) /* int k changed to k by edr */
				 Buf[k]=Contour.pPos[k];
			 Contour.MaxPixels+=1000;
			 Contour.pPos=(T2DInt*)calloc(Contour.MaxPixels,sizeof(T2DInt));
			 for (k=0;k<=i;k++) /* int k changed to k by edr */
				 Contour.pPos[k]=Buf[k];
			 free(Buf);
		 }
        } while(!((p.x==First.x)&&(p.y==First.y)&&(Dir==StartDir)));/* end do */
      } /*end FOURCONNECTED*/
	  else
      {
		  do
		  {
			  switch (Dir) {
			  case NORTHEAST: case SOUTHEAST: case SOUTHWEST: case NORTHWEST: {
				  k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContour() Internal error");
				  return false;
			  }
			  case NORTH:
				  if (k_GetPixel(p.x-1,p.y-1,pSource)>Threshold)
				  {
					  Dir=WEST;
					  p.x--;
					  p.y--;
					  i++;
					  Contour.pPos[i]=p;
					  Rot--;
					  if (p.x<Left.x)
						  Left=p;
					  if (p.y<Top.y)
						  Top=p;
				  }
				  else if (k_GetPixel(p.x,p.y-1,pSource)>Threshold)
				  {
					  p.y--;
					  i++;
					  Contour.pPos[i]=p;
					  if (p.y<Top.y)
						  Top=p;
				  }
				  else
				  {
					  Dir=EAST;
					  Rot++;
				  }
				  break;
			  case EAST:
				  if (k_GetPixel(p.x+1,p.y-1,pSource)>Threshold)
				  {
					  Dir=NORTH;
					  p.x++;
					  p.y--;
					  i++;
					  Contour.pPos[i]=p;
					  Rot--;
					  if (p.x>Right.x)
						  Right=p;
					  if (p.y<Top.y)
						  Top=p;
				  }
				  else if (k_GetPixel(p.x+1,p.y,pSource)>Threshold)
				  {
					  p.x++;i++;
					  Contour.pPos[i]=p;
					  if (p.x>Right.x)
						  Right=p;
				  }
				  else
				  {
					  Dir=SOUTH;
					  Rot++;
				  }
				  break;
			  case SOUTH:
				  if (k_GetPixel(p.x+1,p.y+1,pSource)>Threshold)
				  {
					  Dir=EAST;
					  p.x++;
					  p.y++;
					  i++;
					  Contour.pPos[i]=p;
					  Rot--;
					  if (p.x>Right.x)
						  Right=p;
					  if (p.y>Bottom.y)
						  Bottom=p;
				  }
				  else if (k_GetPixel(p.x,p.y+1,pSource)>Threshold)
				  {
					  p.y++;
					  i++;
					  Contour.pPos[i]=p;
					  if (p.y>Bottom.y)
						  Bottom=p;
				  }
				  else
				  {
					  Dir=WEST;
					  Rot++;
				  }
				  break;
			  case WEST:
				  if (k_GetPixel(p.x-1,p.y+1,pSource)>Threshold)
				  {
					  Dir=SOUTH;
					  p.x--;
					  p.y++;
					  i++;
					  Contour.pPos[i]=p;
					  Rot--;
					  if (p.x<Left.x)
						  Left=p;
					  if (p.y>Bottom.y)
						  Bottom=p;
				  }
				  else if (k_GetPixel(p.x-1,p.y,pSource)>Threshold)
				  {
					  p.x--;
					  i++;
					  Contour.pPos[i]=p;
					  if (p.x<Left.x)
						  Left=p;
				  }
				  else
				  {
					  Dir=NORTH;
					  Rot++;
				  }
               }
			   if (i>Contour.MaxPixels-3)
			   {
				   Buf=(T2DInt*)calloc(Contour.MaxPixels,sizeof(T2DInt));
				   for (k=0;k<=i;k++) /* int k changed to k by edr */
					   Buf[k]=Contour.pPos[k];
				   Contour.MaxPixels+=1000;
				   Contour.pPos=(T2DInt*)calloc(Contour.MaxPixels,sizeof(T2DInt));
				   for (k=0;k<=i;k++) /* int k changed to k by edr */
					   Contour.pPos[k]=Buf[k];
				   free(Buf);
			   }
        }
		while(!((p.x==First.x)&&(p.y==First.y)&&(Dir==StartDir)));/* end do */
    } /*end EIGHTCONNECTED*/
	if (Rot==4)
	{
		Contour.RotationDirection=CLOCKWISE;
		Contour.SurroundedColor=HIGHCOLOR;
	}
	if (Rot==-4)
	{
		Contour.RotationDirection=ANTICLOCKWISE;
		Contour.SurroundedColor=LOWCOLOR;
	}
	Contour.Connected=Connected;
	Contour.Top=Top;
	Contour.Bottom=Bottom;
	Contour.Right=Right;
	Contour.Left=Left;
	if (i==0)
		Contour.NumberOfPixels=1;
	else
		Contour.NumberOfPixels=i;
	Contour.PixelColor=HIGHCOLOR;
	/* Copy all results to the given parameters */
	*pRotationDirection=Contour.RotationDirection;
	*pSurroundedColor=Contour.SurroundedColor;
	pPixelGroup->Top=Contour.Top;
	pPixelGroup->Bottom=Contour.Bottom;
	pPixelGroup->Right=Contour.Right;
	pPixelGroup->Left=Contour.Left;
	k_CopyContourToPixelGroup(pPixelGroup,&Contour);
	k_DeallocContour(&Contour);
	if (ResetBorder==true)
		k_SetBorderSize(0,pSource);
	return true;

 } /* end else */
}

bool k_FindAndFollowLowContourWithThreshold(TImage* pSource, T2DInt Start,unsigned int Threshold,
											DIRECTION SearchDirection, CONNECTIVITY Connected,
											TPixelGroup* pPixelGroup,ROTATION* pRotationDirection,
											COLORTYPE* pSurroundedColor)
{
	TContour Contour;
	T2DInt p,First,Top,Left,Bottom,Right;
	int i,k,Rot;
	bool found;
	char Dir,StartDir;
	T2DInt* Buf;
	bool ResetBorder=false;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_FindAndFollowHighContour() Source image is empty - doing nothing");
		return false;
	}
	if (pSource->Bits==24)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContour() Source image is 24 b/p - not supported");
		return false;
	}
	if ((SearchDirection!=NORTH) && (SearchDirection!=EAST) && (SearchDirection!=SOUTH) &&
		(SearchDirection!=WEST))
	{
		k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContour() Wrong search direction");
		return false;
	}
	/* add white border if no border is present */
	if (pSource->BorderSize==0)
	{
		ResetBorder=true;
		if (pSource->Bits==1)
			k_SetBorder (1,1,pSource); /* Set color to 1 */
		else
			k_SetBorder (1,255,pSource); /* Set color to 255 */
	}
	k_AllocContour(&Contour,1000); /* initialise the TContour structure */
	p=Start;
	found=false;
	switch (SearchDirection)
	{
	case NORTHEAST:	case SOUTHEAST:	case SOUTHWEST:	case NORTHWEST: {
		k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowHighContour() Internal error");
		return false;
	}
	case NORTH:
		do {
			found=(k_GetPixel(p.x,p.y,pSource)>Threshold);/* looks for low */
			if (!found)p.y--;
		}
		while ((p.y>=1)&&(!found));
		if (found)
			do {
				found=false;
				if (k_GetPixel(p.x,p.y-1,pSource)<=Threshold)
				{
					found=true;
					Dir=WEST;
				}
				p.y--;
			}
			while ((!found)&&(p.y>=0));
			break;
	case EAST:
		do {
			found=(k_GetPixel(p.x,p.y,pSource)>Threshold);
			if (!found)p.x++;
		}
		while ((p.x<=(INT16)(pSource->Width-2))&&(!found));
		if (found)
			do {
				found=false;
				if (k_GetPixel(p.x+1,p.y,pSource)<=Threshold)
				{
					found=true;
					Dir=NORTH;
				}
				p.x++;
			}
			while (!found&&(p.x<=(INT16)(pSource->Width-1)));
			break;
	case SOUTH:
		do {
			found=(k_GetPixel(p.x,p.y,pSource)>Threshold);
			if (!found)
				p.y++;
		}
		while ((p.y<=(INT16)(pSource->Height-2))&&(!found));
		if (found)
			do {
				found=false;
				if (k_GetPixel(p.x,p.y+1,pSource)<=Threshold)
				{
					found=true;
					Dir=EAST;
				}
				p.y++;
			}
			while ((!found)&&(p.y<=(INT16)(pSource->Height-1)));
			break;
	case WEST:
		do {
			found=(k_GetPixel(p.x,p.y,pSource)>Threshold);
			if (!found)
				p.x--;
		}
		while ((p.x>=1)&&(!found));
		if (found)
			do {
				found=false;
				if (k_GetPixel(p.x-1,p.y,pSource)<=Threshold)
				{
					found=true;
					Dir=SOUTH;
				}
				p.x--;
			}
			while ((!found)&&(p.y>=0));
			break;
    }
	if (!found)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_FindAndFollowLowContour() No contour found");
		if (ResetBorder==true)
			k_SetBorderSize(0,pSource);
		return false;
	}
	else
	{
		StartDir=Dir;
		First=p;
		i=0;
		Rot=0;
		Contour.pPos[0]=p;
		Top=p;
		Bottom=p;
		Left=p;
		Right=p;
		if (Connected==FOURCONNECTED)
		{
			do
			{
				switch (Dir)
				{
				case NORTH:
					if (k_GetPixel(p.x,p.y-1,pSource)>Threshold)
					{
						Dir=EAST;
						Rot++;
					}
					else
					{
						if (k_GetPixel(p.x-1,p.y-1,pSource)<=Threshold)
						{
							Dir=WEST;
							p.y--;
							Rot--;
							i++;
							Contour.pPos[i]=p;
							p.x--;
							i++;
							Contour.pPos[i]=p;
							if (p.x<Left.x)
								Left=p;
							if (p.y<Top.y)
								Top=p;
						}
						else
						{
							p.y--;
							i++;
							Contour.pPos[i]=p;
							if (p.y<Top.y)
								Top=p;
						}
					}
					break;
				case EAST:
					if (k_GetPixel(p.x+1,p.y,pSource)>Threshold)
					{
						Dir=SOUTH;
						Rot++;
					}
					else
					{
						if (k_GetPixel(p.x+1,p.y-1,pSource)<=Threshold)
						{
							Dir=NORTH;
							p.x++;
							Rot--;
							i++;
							Contour.pPos[i]=p;
							p.y--;
							i++;
							Contour.pPos[i]=p;
							if (p.x>Right.x)
								Right=p;
							if (p.y<Top.y)
								Top=p;
						}
						else
						{
							p.x++;i++;
							Contour.pPos[i]=p;
							if (p.x>Right.y)
								Right=p;
						}
					}
					break;
				case SOUTH:
					if (k_GetPixel(p.x,p.y+1,pSource)>Threshold)
					{
						Dir=WEST;
						Rot++;
					}
					else
					{
						if (k_GetPixel(p.x+1,p.y+1,pSource)<=Threshold)
						{
							Dir=EAST;
							p.y++;
							Rot--;
							i++;
							Contour.pPos[i]=p;
							p.x++;
							i++;
							Contour.pPos[i]=p;
							if (p.x>Right.x)
								Right=p;
							if (p.y>Bottom.y)
								Bottom=p;
						}
						else
						{
							p.y++;
							i++;
							Contour.pPos[i]=p;
							if (p.y>Bottom.y)
								Bottom=p;
						}
					}
					break;
				case WEST:
					if (k_GetPixel(p.x-1,p.y,pSource)>Threshold)
					{
						Dir=NORTH;
						Rot++;
					}
					else
					{
						if (k_GetPixel(p.x-1,p.y+1,pSource)<=Threshold)
						{
							Dir=SOUTH;
							p.x--;
							Rot--;
							i++;
							Contour.pPos[i]=p;
							p.y++;
							i++;
							Contour.pPos[i]=p;
							if (p.x<Left.x)
								Left=p;
							if (p.y>Bottom.y)
								Bottom=p;
						}
						else
						{
							p.x--;
							i++;
							Contour.pPos[i]=p;
							if (p.x<Left.x)
								Left=p;
						}
					}
		   } /*end SWITCH*/
           if (i>Contour.MaxPixels-3)
		   {
			   Buf=(T2DInt*)calloc(Contour.MaxPixels,sizeof(T2DInt));
			   for (k=0;k<=i;k++) /* int k changed to k by edr */
				   Buf[k]=Contour.pPos[k];
			   Contour.MaxPixels+=1000;
			   Contour.pPos=(T2DInt*)calloc(Contour.MaxPixels,sizeof(T2DInt));
			   for (k=0;k<=i;k++) /* int k changed to k by edr */
				   Contour.pPos[k]=Buf[k];
			   free(Buf);
		   }
         }
         while(!((p.x==First.x)&&(p.y==First.y)&&(Dir==StartDir)));/* end do */
     } /*end FOURCONNECTED*/
     else
     {
		 do
		 {
			 switch (Dir)
			 {
			 case NORTH:
				 if (k_GetPixel(p.x-1,p.y-1,pSource)<=Threshold)
				 {
					 Dir=WEST;
					 p.x--;
					 p.y--;
					 i++;
					 Contour.pPos[i]=p;
					 Rot--;
					 if (p.x<Left.x)
						 Left=p;
					 if (p.y<Top.y)
						 Top=p;
				 }
				 else if (k_GetPixel(p.x,p.y-1,pSource)<=Threshold)
				 {
					 p.y--;
					 i++;
					 Contour.pPos[i]=p;
					 if (p.y<Top.y)
						 Top=p;
				 }
				 else
				 {
					 Dir=EAST;
					 Rot++;
				 }
				 break;
			 case EAST:
				 if (k_GetPixel(p.x+1,p.y-1,pSource)<=Threshold)
				 {
					 Dir=NORTH;
					 p.x++;
					 p.y--;
					 i++;
					 Contour.pPos[i]=p;
					 Rot--;
					 if (p.x>Right.x)
						 Right=p;
					 if (p.y<Top.y)
						 Top=p;
				 } else if (k_GetPixel(p.x+1,p.y,pSource)<=Threshold)
				 {
					 p.x++;i++;
					 Contour.pPos[i]=p;
					 if (p.x>Right.x)
						 Right=p;
				 }
				 else
				 {
					 Dir=SOUTH;
					 Rot++;
				 }
				 break;
			 case SOUTH:
				 if (k_GetPixel(p.x+1,p.y+1,pSource)<=Threshold)
				 {
					 Dir=EAST;
					 p.x++;
					 p.y++;
					 i++;
					 Contour.pPos[i]=p;
					 Rot--;
					 if (p.x>Right.x)
						 Right=p;
					 if (p.y>Bottom.y)
						 Bottom=p;
				 }
				 else if (k_GetPixel(p.x,p.y+1,pSource)<=Threshold)
				 {
					 p.y++;
					 i++;
					 Contour.pPos[i]=p;
					 if (p.y>Bottom.y)
						 Bottom=p;
				 }
				 else
				 {
					 Dir=WEST;
					 Rot++;
				 }
				 break;
			 case WEST:
				 if (k_GetPixel(p.x-1,p.y+1,pSource)<=Threshold)
				 {
					 Dir=SOUTH;
					 p.x--;
					 p.y++;
					 i++;
					 Contour.pPos[i]=p;
					 Rot--;
					 if (p.x<Left.x)
						 Left=p;
					 if (p.y>Bottom.y)
						 Bottom=p;
				 }
				 else if (k_GetPixel(p.x-1,p.y,pSource)<=Threshold)
				 {
					 p.x--;
					 i++;
					 Contour.pPos[i]=p;
					 if (p.x<Left.x)
						 Left=p;
				 }
				 else
				 {
					 Dir=NORTH;
					 Rot++;
				 }
           } /*end SWITCH*/
           if (i>Contour.MaxPixels-3)
		   {
			   Buf=(T2DInt*)calloc(Contour.MaxPixels,sizeof(T2DInt));
			   for (k=0;k<=i;k++) /* int k changed to k by edr */
				   Buf[k]=Contour.pPos[k];
			   Contour.MaxPixels+=1000;
			   Contour.pPos=(T2DInt*)calloc(Contour.MaxPixels,sizeof(T2DInt));
			   for (k=0;k<=i;k++) /* int k changed to k by edr */
				   Contour.pPos[k]=Buf[k];
			   free(Buf);
		   }
         }
		 while(!((p.x==First.x)&&(p.y==First.y)&&(Dir==StartDir)));/* end do */
  }/*end EIGHTCONNECTED*/
  if (Rot==4)
  {
	  Contour.RotationDirection=CLOCKWISE;
	  Contour.SurroundedColor=HIGHCOLOR;
  }
  if (Rot==-4)
  {
	  Contour.RotationDirection=ANTICLOCKWISE;
	  Contour.SurroundedColor=LOWCOLOR;
  }
  Contour.Connected=Connected;
  Contour.Top=Top;
  Contour.Bottom=Bottom;
  Contour.Right=Right;
  Contour.Left=Left;
  if (i==0)
	  Contour.NumberOfPixels=1;
  else
	  Contour.NumberOfPixels=i;
  
  Contour.PixelColor=LOWCOLOR;
  /* Copy all results to the given parameters */
  *pRotationDirection=Contour.RotationDirection;
  *pSurroundedColor=Contour.SurroundedColor;
  pPixelGroup->Top=Contour.Top;
  pPixelGroup->Bottom=Contour.Bottom;
  pPixelGroup->Right=Contour.Right;
  pPixelGroup->Left=Contour.Left;
  k_CopyContourToPixelGroup(pPixelGroup,&Contour);
  k_DeallocContour(&Contour);
  if (ResetBorder==true)
	  k_SetBorderSize(0,pSource);
  return true;
 } /* end else */
 }

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
