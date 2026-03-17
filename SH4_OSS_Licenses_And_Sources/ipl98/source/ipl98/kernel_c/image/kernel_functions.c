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

#include "kernel_functions.h"
#include "../kernel_string.h"
#include "../kernel_big_endian.h"
#include "../algorithms/kernel_graphics.h"
#include "../kernel_error.h"
#include "../general_functions/ipl_general_functions_c_only.h"
#include "../kernel_history.h"
#include <memory.h>
#include <string.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/************************************************/
/********        Private functions       ********/
/************************************************/

UINT32 k_GetSizeScanLine(UINT32 Width, UINT16 Bits);
void k_SetImageMatrix(TImage* pInfo);

#ifdef IPL_BIG_ENDIAN
void k_WriteBigEndianPalette(TPalette* pPal,FILE** fp);
#endif /* IPL_BIG_ENDIAN */

bool k_LoadBMP(FILE* fp,TImage* pInfo);
bool k_SaveBMP(FILE* fp,TImage* pInfo);
void k_ReadComments(FILE *fp,char** ppComments);
bool k_LoadPGM(FILE* fp,TImage* pInfo);
bool k_SavePGM(FILE* fp,TImage* pInfo);
bool k_LoadPCD(FILE* fp,TImage* pInfo);

/************************************************/
/********        Public functions        ********/
/************************************************/

void k_EmptyImage(TImage* pInfo)
{
	k_EmptyFileInfo(&(pInfo->FileInfo));
	k_EmptyText(&(pInfo->History));
	k_EmptyPalette(&(pInfo->Pal));
	if (pInfo->pPixelStream!=NULL)
	{
		free(pInfo->pPixelStream);
		pInfo->pPixelStream=NULL;
	}
	if (pInfo->ppAllScanLines!=NULL)
	{
		free(pInfo->ppAllScanLines);
		pInfo->ppAllScanLines=NULL;
	}
	pInfo->Width=0;
	pInfo->Height=0;
	pInfo->Bits=0;
	pInfo->BorderSize=0;
	pInfo->ByteWidth=0;
	pInfo->TotalSize=0;
	pInfo->Origo.x=0;
	pInfo->Origo.y=0;
	pInfo->Origin=EMPTY;
	pInfo->Pal.PalEntries=0;
}

void k_InitImage(TImage* pInfo)
{
	k_InitFileInfo(&(pInfo->FileInfo));
	k_InitText(&(pInfo->History));
	k_InitPalette(&(pInfo->Pal));
	pInfo->pPixelStream=NULL;
	pInfo->ppMatrix=NULL;
	pInfo->ppAllScanLines=NULL;
	pInfo->Origin=EMPTY;
	pInfo->Origo.x=0; /* default origo (0,0) */
	pInfo->Origo.y=0;
}

bool k_AllocImage(UINT32 Width, UINT32 Height, UINT16 Bits,TImage* pInfo)
{
	if((Bits==1) || (Bits==4) || (Bits==8) || (Bits==24))
	{
		/* Allocate memory if necessary */
		if ((pInfo->Width!=Width) || (pInfo->Height!=Height) || 
			(pInfo->BorderSize!=0) || (pInfo->Bits!=Bits))
		{
			/* start with emptying the old image data (if any!) */
			k_EmptyImage(pInfo);
			/* set values in the image info structure */
			pInfo->Width=Width;
			pInfo->Height=Height;
			pInfo->Bits=Bits;
			pInfo->BorderSize=0; /* always 0 for a new image */
			/* calculate size and width (incl zero-padding) of image */
			pInfo->ByteWidth=(UINT16) k_GetSizeScanLine(Width,Bits);
			pInfo->TotalSize = pInfo->ByteWidth * Height; /* no compression */
			/* Allocate memory for image */
			if (Bits==24)
			{
				/** Due to how pixels are read when working with 24 b/p we need
					to allocate an extra byte at the end of the last scan line.
					The problem arise when accessing the last pixel where we
					read a type UINT32 which addresses an extra byte (the byte
					is never used, it is removed from the UINT32 value). */
				pInfo->pPixelStream=(UINT8*) calloc(sizeof(UINT8),pInfo->TotalSize+1);
			}
			else
				pInfo->pPixelStream=(UINT8*) calloc(sizeof(UINT8),pInfo->TotalSize);
			k_SetImageMatrix(pInfo);
			/* Create gray palette */
			if (k_CreatePal(Bits,&pInfo->Pal)==false)
			{
				k_EmptyImage(pInfo);
				return false;
			}
		}
		else
		{
			/* if new allocation not needed, we only reset a few things. */
			k_EmptyFileInfo(&(pInfo->FileInfo));
			k_EmptyText(&(pInfo->History));
			k_FlushImage(0,pInfo); /* reset existing memory to 0! */
		}
		/* Always set origo to (0,0) */
		pInfo->Origo.x=0;
		pInfo->Origo.y=0;
		/* Always set origin to RAM */
		pInfo->Origin=RAM;
		return true;
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_AllocImage() Bits=%d not supported",
			Bits);
		return false;
	}
}

bool k_AllocImageFast(UINT32 Width, UINT32 Height, UINT16 Bits,TImage* pInfo)
{
	if((Bits==1) || (Bits==4) || (Bits==8) || (Bits==24))
	{
		/* Allocate memory if necessary */
		if ((pInfo->Width!=Width) || (pInfo->Height!=Height) || 
			(pInfo->BorderSize!=0) || (pInfo->Bits!=Bits))
		{
			/* start with emptying the old image data (if any!) */
			k_EmptyImage(pInfo);
			/* set values in the image info structure */
			pInfo->Width=Width;
			pInfo->Height=Height;
			pInfo->Bits=Bits;
			pInfo->BorderSize=0; /* always 0 for a new image */
			/* calculate size and width (incl zero-padding) of image */
			pInfo->ByteWidth=(UINT16) k_GetSizeScanLine(Width,Bits);
			pInfo->TotalSize = pInfo->ByteWidth * Height; /* no compression */
			/* Allocate memory for image */
			pInfo->pPixelStream=(UINT8*) malloc(sizeof(UINT8)*pInfo->TotalSize);
			k_SetImageMatrix(pInfo);
			/* Create gray palette */
			if (k_CreatePal(Bits,&pInfo->Pal)==false)
			{
				k_EmptyImage(pInfo);
				return false;
			}
		}
		else
		{
			/* if new allocation not needed, we only reset a few things. */
			k_EmptyFileInfo(&(pInfo->FileInfo));
			k_EmptyText(&(pInfo->History));
		}
		/* Always set origo to (0,0) */
		pInfo->Origo.x=0;
		pInfo->Origo.y=0;
		/* Always set origin to RAM */
		pInfo->Origin=RAM;
		return true;
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_AllocImageFast() Bits=%d not supported",
			Bits);
		return false;
	}
}

bool k_FlushImage(UINT32 Color,TImage* pDest)
{
	if (pDest==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_FlushImage() pDest is a NULL pointer");
		return false;
	}
	else if (pDest->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_FlushImage() Image is empty - doing nothing");
		return false;
	}
	else if ((pDest->Bits!=1) && (pDest->Bits!=8) && (pDest->Bits!=24))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_WARNING,
			"k_FlushImage() %d b/p not supported - doing nothing",
			pDest->Bits);
		return false;
	}
	else
	{
		unsigned int y,x;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%sk_FlushImage(Color=%d)",
				ipl_HistoryIndent, Color);
			k_IplStartHistoryMacro();
		}
#endif
		if (pDest->Bits==1)
		{
			if (Color>1)
			{
				k_ShowMessageStd(
					IPL_POSITION, IPL_ERROR,
					"k_FlushImage() Color=%d not valid for an image with 1 b/p",
					Color);
				return false;
			}
			if (Color==0)
				memset(pDest->pPixelStream,0,pDest->TotalSize);
			else
				memset(pDest->pPixelStream,255,pDest->TotalSize);

		}
		else if (pDest->Bits==8)
		{
			if (Color>255)
			{
				k_ShowMessageStd(
					IPL_POSITION, IPL_ERROR,
					"k_FlushImage() Color=%d not valid for an image with 8 b/p",
					Color);
				return false;
			}
			memset(pDest->pPixelStream,Color,pDest->TotalSize);
		}
		else if (pDest->Bits==24)
		{
			UINT8* pPixels;
			UINT8 r=k_PalGetRVal(Color);
			UINT8 g=k_PalGetGVal(Color);
			UINT8 b=k_PalGetBVal(Color);
			unsigned int max=(int)((double)pDest->ByteWidth/3.0);
			for(y=0;y<pDest->Height;y++)
			{
				pPixels=pDest->ppAllScanLines[y];
				for(x=0;x<max;x++)
				{
					*pPixels=b;
					pPixels++;
					*pPixels=g;
					pPixels++;
					*pPixels=r;
					pPixels++;
				}
			}

		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool k_InvertImage(TImage* pInfo)
{
	int x,y;
	if (pInfo==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_InvertImage() pDest is a NULL pointer");
		return false;
	}
	else if (pInfo->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_InvertImage() Image is empty - doing nothing");
		return false;
	}
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(pInfo->History),
			"%sk_InvertImage()",
			ipl_HistoryIndent);
		k_IplStartHistoryMacro();
	}
#endif
	if (pInfo->Bits==1)
	{
		for(y=0; y<(int)pInfo->Height; y++)
			for(x=0; x<(int)pInfo->Width; x++)
				k_GetPixel1bp(x,y,(*pInfo))==0 ? k_SetPixel1bp(x,y,1,(*pInfo)) : k_SetPixel1bp(x,y,0,(*pInfo));
	}
	else if (pInfo->Bits==8)
	{
		for(y=0; y<(int)pInfo->Height; y++)
			for(x=0; x<(int)pInfo->Width; x++)
				k_SetPixel8bp(x,y,255-k_GetPixel8bp(x,y,(*pInfo)),(*pInfo));
	}
	else /* must be 24 b/p */
	{
		UINT32 PixVal;
		for(y=0; y<(int)pInfo->Height; y++)
			for(x=0; x<(int)pInfo->Width; x++)
			{
				PixVal=k_GetPixel24bp(x,y,(*pInfo));
				k_SetPixel24bp(x,y,k_PalCreateRGB(255-k_PalGetRVal(PixVal),
					255-k_PalGetGVal(PixVal),255-k_PalGetBVal(PixVal)),(*pInfo));
			}
	}
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
	return true;
}

bool k_Load(const char* PathAndFileName, TImage* pInfo)
{
	bool returnValue;
	FILE* fp;
	char* Path=NULL,*FileNameExt=NULL,*Ext=NULL;
	if (!k_SplitFileName(PathAndFileName, &Path,&FileNameExt,&Ext))
	{
		k_ShowMessageSimple(IPL_ERROR, "k_Load() Filename not valid");
		return false;
	}
	else if (strcmp(Ext,"")==0)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_Load() No extension found");
		returnValue=false;
	}
	else
	{
		returnValue=true;
		k_StringToUpper(Ext);
		fp=fopen(PathAndFileName,"rb"); /* open file */
		if (fp==NULL)
		{
			k_ShowMessageStd(
				IPL_POSITION, IPL_ERROR,
				"k_Load() Failed opening file %s",
				PathAndFileName);
			returnValue=false;
		}
		else if (!strcmp(Ext,"BMP"))
		{
			k_EmptyImage(pInfo);
			if (!k_LoadBMP(fp,pInfo))
			{
				returnValue=false;
			}
			k_SetImageMatrix(pInfo);
		}
		else if (!strcmp(Ext,"PGM"))
		{
			k_EmptyImage(pInfo);
			if (!k_LoadPGM(fp,pInfo))
			{
				returnValue=false;
			}
			k_SetImageMatrix(pInfo);
		}
		else if (!strcmp(Ext,"PCD"))
		{
			k_EmptyImage(pInfo);
			if (!k_LoadPCD(fp,pInfo))
			{
				returnValue=false;
			}
			else /* If Load failed don't set the Image Matrix */
			{
				k_SetImageMatrix(pInfo);
			}
		}
		else
		{
			k_ShowMessageSimple(IPL_ERROR, "k_Load() No valid extension found");
			returnValue=false;
		}
		if (returnValue==true)
		{
			pInfo->Origin=DISC;
			/* set file and path name */
			k_SetFileAndPath(PathAndFileName,&pInfo->FileInfo);
			/* update file information in the history */
			k_SetFileInfoFromPathFileText(&(pInfo->History),pInfo->FileInfo.PathAndFileName);
#ifdef IPL_ADD_HISTORY
			/* add comment to history */
			{
				k_PrintfAppendTextIPL(
					&(pInfo->History),
					"%sk_Load(%s)",
					ipl_HistoryIndent, PathAndFileName);
			}
#endif
		}
		if (fp!=NULL)
		{
			fclose(fp);
		}
	}
	free(Path);
	free(FileNameExt);
	free(Ext);
	return returnValue;
}

bool k_Save(const char* FileName, TImage* pInfo)
{
	bool returnValue;
	FILE* fp;
	char* Path=NULL,*FileNameExt=NULL,*Ext=NULL;
	if (!k_SplitFileName(FileName, &Path,&FileNameExt,&Ext))
	{
		k_ShowMessageSimple(IPL_ERROR, "k_Save() Filename not valid");
		return false;
	}
	else if (strcmp(Ext,"")==0)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_Save() No extension found");
		returnValue=false;
	}
	else
	{
		returnValue=true;
		k_StringToUpper(Ext);
		fp=fopen(FileName,"wb"); /* open file */
		if (fp==NULL)
		{
			k_ShowMessageSimple(IPL_ERROR, "k_Save() Could not save file");
			returnValue=false;
		}
		else if (pInfo->Origin==EMPTY)
		{
			k_ShowMessageSimple(IPL_ERROR, "k_Save() No image - cannot save");
			returnValue=false;
		}

		else if (!strcmp(Ext,"BMP")) /* save as BMP */
		{
			if (!k_SaveBMP(fp,pInfo))
			{
				returnValue=false;
			}
		}
		else if (!strcmp(Ext,"PGM")) /* save as PGM */
		{
			if (!k_SavePGM(fp,pInfo))
			{
				returnValue=false;
			}
		}
		else
		{
			k_ShowMessageSimple(IPL_ERROR, "k_Save() Unknown file type");
			returnValue=false;
		}
		if (returnValue==true)
		{
			pInfo->Origin=DISC;
			/* empty file information before proceeding */
			k_EmptyFileInfo(&pInfo->FileInfo);
			/* set file and path name */
			pInfo->FileInfo.FileName = (char *) malloc(strlen(FileNameExt)+1);
			strcpy(pInfo->FileInfo.FileName,FileNameExt);
			pInfo->FileInfo.PathName = (char *) malloc(strlen(Path)+1);
			strcpy(pInfo->FileInfo.PathName,Path);
			pInfo->FileInfo.PathAndFileName = (char *) malloc(strlen(FileName)+1);
			strcpy(pInfo->FileInfo.PathAndFileName,FileName);
			/* update file information in the history */
			k_SetFileInfoFromPathFileText(&(pInfo->History),pInfo->FileInfo.PathAndFileName);
#ifdef IPL_ADD_HISTORY
			/* add comment to history */
			{
				k_PrintfAppendTextIPL(
					&(pInfo->History),
					"%sk_Save(%s)",
					ipl_HistoryIndent, FileName);
			}
#endif
		}
		if (fp!=NULL)
		{
			fclose(fp);
		}
	}
	free(Path);
	free(FileNameExt);
	free(Ext);
	return returnValue;
}

UINT8 k_GetRedComponent(int x, int y, const TImage* pInfo)
{
	UINT16 border=pInfo->BorderSize;
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if ((x>=-border) && (x<(int) pInfo->Width+border) &&
		(y>=-border) && (y<(int) pInfo->Height+border))
	{
		switch(pInfo->Bits){
		case 1:
			/* assumes a palette is present */
			return k_PalGetRVal(pInfo->Pal.pPalette[(((pInfo->ppMatrix[y][x>>3])&(0x80>>(x&0x7)))!=0)]);
		case 8:
			/* assumes a palette is present */
			return k_PalGetRVal(pInfo->Pal.pPalette[(pInfo->ppMatrix[y][x])&0xff]);
		case 24:
			return k_PalGetRVal((*(UINT32*)(&(pInfo->ppMatrix[y][3*x])))&0xffffff);
		default:
			{
				k_ShowMessageSimple(IPL_ERROR,"k_GetRedComponent() Only implemented for 1,8 and 24 bit per pixels");
				return 0;
			}
		}
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_GetRedComponent(%d,%d) Outside image and border",
			x, y);
		return 0;
	}
}

UINT8 k_GetRedComponentFast(int x, int y, const TImage* pInfo)
{
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	switch(pInfo->Bits){
	case 1:
		/* assumes a palette is present */
		return k_PalGetRVal(pInfo->Pal.pPalette[(((pInfo->ppMatrix[y][x>>3])&(0x80>>(x&0x7)))!=0)]);
	case 8:
		/* assumes a palette is present */
		return k_PalGetRVal(pInfo->Pal.pPalette[(pInfo->ppMatrix[y][x])&0xff]);
	case 24:
		return k_PalGetRVal((*(UINT32*)(&(pInfo->ppMatrix[y][3*x])))&0xffffff);
	default:
		{
			k_ShowMessageSimple(IPL_ERROR, "k_GetRedComponentFast() Only implemented for 1,8 and 24 bit per pixels");
			return 0;
		}
	}
}

UINT8 k_GetGreenComponent(int x, int y, const TImage* pInfo)
{
	UINT16 border=pInfo->BorderSize;
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if ((x>=-border) && (x<(int) pInfo->Width+border) &&
		(y>=-border) && (y<(int) pInfo->Height+border))
	{
		switch(pInfo->Bits){
		case 1:
			/* assumes a palette is present */
			return k_PalGetGVal(pInfo->Pal.pPalette[(((pInfo->ppMatrix[y][x>>3])&(0x80>>(x&0x7)))!=0)]);
		case 8:
			/* assumes a palette is present */
			return k_PalGetGVal(pInfo->Pal.pPalette[(pInfo->ppMatrix[y][x])&0xff]);
		case 24:
			return k_PalGetGVal((*(UINT32*)(&(pInfo->ppMatrix[y][3*x])))&0xffffff);
		default:
			{
				k_ShowMessageSimple(
					IPL_ERROR,
					"k_GetGreenComponent() Only implemented for 1,8 and 24 bit per pixels");
				return 0;
			}
		}
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_GetGreenComponent(%d,%d) Outside image and border",
			x, y);
		return 0;
	}
}

UINT8 k_GetGreenComponentFast(int x, int y, const TImage* pInfo)
{
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	switch(pInfo->Bits){
	case 1:
		/* assumes a palette is present */
		return k_PalGetGVal(pInfo->Pal.pPalette[(((pInfo->ppMatrix[y][x>>3])&(0x80>>(x&0x7)))!=0)]);
	case 8:
		/* assumes a palette is present */
		return k_PalGetGVal(pInfo->Pal.pPalette[(pInfo->ppMatrix[y][x])&0xff]);
	case 24:
		return k_PalGetGVal((*(UINT32*)(&(pInfo->ppMatrix[y][3*x])))&0xffffff);
	default:
		{
			k_ShowMessageSimple(
				IPL_ERROR,
				"k_GetGreenComponentFast() Only implemented for 1,8 and 24 bit per pixels");
			return 0;
		}
	}
}

UINT8 k_GetBlueComponent(int x, int y, const TImage* pInfo)
{
	UINT16 border=pInfo->BorderSize;
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if ((x>=-border) && (x<(int) pInfo->Width+border) &&
		(y>=-border) && (y<(int) pInfo->Height+border))
	{
		switch(pInfo->Bits){
		case 1:
			/* assumes a palette is present */
			return k_PalGetBVal(pInfo->Pal.pPalette[(((pInfo->ppMatrix[y][x>>3])&(0x80>>(x&0x7)))!=0)]);
		case 8:
			/* assumes a palette is present */
			return k_PalGetBVal(pInfo->Pal.pPalette[(pInfo->ppMatrix[y][x])&0xff]);
		case 24:
			return k_PalGetBVal((*(UINT32*)(&(pInfo->ppMatrix[y][3*x])))&0xffffff);
		default:
			{
				k_ShowMessageSimple(
					IPL_ERROR,
					"k_GetBlueComponent() Only implemented for 1,8 and 24 bit per pixels");
				return 0;
			}
		}
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_GetBlueComponent(%d,%d) Outside image and border",
			x, y);
		return 0;
	}
}

UINT8 k_GetBlueComponentFast(int x, int y, const TImage* pInfo)
{
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	switch(pInfo->Bits){
	case 1:
		/* assumes a palette is present */
		return k_PalGetBVal(pInfo->Pal.pPalette[(((pInfo->ppMatrix[y][x>>3])&(0x80>>(x&0x7)))!=0)]);
	case 8:
		/* assumes a palette is present */
		return k_PalGetBVal(pInfo->Pal.pPalette[(pInfo->ppMatrix[y][x])&0xff]);
	case 24:
		return k_PalGetBVal((*(UINT32*)(&(pInfo->ppMatrix[y][3*x])))&0xffffff);
	default:
		{
			k_ShowMessageSimple(
				IPL_ERROR,
				"k_GetBlueComponentFast() Only implemented for 1,8 and 24 bit per pixels");
			return 0;
		}
	}
}

bool k_SetRedComponent(int x, int y, UINT8 RedValue, const TImage* pInfo)
{
	UINT16 border=pInfo->BorderSize;
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if((x>=-border) && (x<(int) pInfo->Width+border) &&
		(y>=-border) && (y<(int) pInfo->Height+border))
	{
		switch(pInfo->Bits){
		case 24:
			*((UINT32*)&(pInfo->ppMatrix[y][3*x])) = ((*((UINT32*)&(pInfo->ppMatrix[y][3*x])))
				& 0xff00ffff) | (RedValue << 16);
			return true;
		default:
			{
				k_ShowMessageSimple(
					IPL_ERROR,
					"k_SetRedComponent() Only possible for 24 bis per pixel");
				return false;
			}
		}
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_SetRedComponent(%d,%d) Outside image and border",
			x, y);
		return false;
	}
}

bool k_SetRedComponentFast(int x, int y, UINT8 RedValue, const TImage* pInfo)
{
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	switch(pInfo->Bits){
	case 24:
		*((UINT32*)&(pInfo->ppMatrix[y][3*x])) = ((*((UINT32*)&(pInfo->ppMatrix[y][3*x])))
			& 0xff00ffff) | (RedValue << 16);
		return true;
	default:
		{
			k_ShowMessageSimple(
				IPL_ERROR,
				"k_SetRedComponentFast() Only possible for 24 bis per pixel");
			return false;
		}
	}
}

bool k_SetGreenComponent(int x, int y, UINT8 GreenValue, const TImage* pInfo)
{
	UINT16 border=pInfo->BorderSize;
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if((x>=-border) && (x<(int) pInfo->Width+border) &&
		(y>=-border) && (y<(int) pInfo->Height+border))
	{
		switch(pInfo->Bits){
		case 24:
			*((UINT32*)&(pInfo->ppMatrix[y][3*x])) = ((*((UINT32*)&(pInfo->ppMatrix[y][3*x])))
				& 0xffff00ff) | (GreenValue << 8);
			return true;
		default:
			{
				k_ShowMessageSimple(IPL_ERROR, "k_SetGreenComponent() Only possible for 24 bis per pixel");
				return false;
			}
		}
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_SetGreenComponent(%d,%d) Outside image and border",
			x, y);
		return false;
	}
}

bool k_SetGreenComponentFast(int x, int y, UINT8 GreenValue, const TImage* pInfo)
{
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	switch(pInfo->Bits){
	case 24:
		*((UINT32*)&(pInfo->ppMatrix[y][3*x])) = ((*((UINT32*)&(pInfo->ppMatrix[y][3*x])))
			& 0xffff00ff) | (GreenValue << 8);
		return true;
	default:
		{
			k_ShowMessageSimple(
				IPL_ERROR,
				"k_SetGreenComponentFast() Only possible for 24 bis per pixel");
			return false;
		}
	}
}

bool k_SetBlueComponent(int x, int y, UINT8 BlueValue, const TImage* pInfo)
{
	UINT16 border=pInfo->BorderSize;
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if((x>=-border) && (x<(int) pInfo->Width+border) &&
		(y>=-border) && (y<(int) pInfo->Height+border))
	{
		switch(pInfo->Bits){
		case 24:
			*((UINT32*)&(pInfo->ppMatrix[y][3*x])) = ((*((UINT32*)&(pInfo->ppMatrix[y][3*x])))
				& 0xffffff00) | (BlueValue);
			return true;
		default:
			{
				k_ShowMessageSimple(
					IPL_ERROR,
					"k_SetBlueComponent() Only possible for 24 bis per pixel");
				return false;
			}
		}
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_SetBlueComponent(%d,%d) Outside image and border",
			x, y);
		return false;
	}
}

bool k_SetBlueComponentFast(int x, int y, UINT8 BlueValue, const TImage* pInfo)
{
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	switch(pInfo->Bits){
	case 24:
		*((UINT32*)&(pInfo->ppMatrix[y][3*x])) = ((*((UINT32*)&(pInfo->ppMatrix[y][3*x])))
			& 0xffffff00) | (BlueValue);
		return true;
	default:
		{
			k_ShowMessageSimple(
				IPL_ERROR,
				"k_SetBlueComponentFast() Only possible for 24 bis per pixel");
			return false;
		}
	}
}

bool k_SetBorder(UINT16 Size, UINT32 Color, TImage* pInfo)
{
	UINT16 bits=pInfo->Bits;
	UINT32 realX,realY,offset;
	UINT32 UINT32Value; /* temporary variable */
	UINT8* pNewImage; /* variable pointing to new allocated image area */
	UINT32 i;
	UINT32 SmallestByteWidth;
	if ((bits==1) && (Size!=0))
	{
		/* round Size to nearest upward multiplum of 8 */
		/*printf("Size=%d, Size % 8=%d, new Size=%d\n",Size,Size%8,Size + (8-Size % 8));*/
		Size += 8-(Size % 8);
		/*printf("Size after: %d\n",Size);*/
	}
	realX=pInfo->Width+2*Size; /* width of image + border */
	realY=pInfo->Height+2*Size; /* height of image + border */
	/* calculate size and width (incl zero-padding) of image + border */
	UINT32Value=k_GetSizeScanLine(realX,bits);
	/* Only copy memory area for the shortest width of the destination and source image */
	SmallestByteWidth=UINT32Value<(UINT32)pInfo->ByteWidth?UINT32Value:(UINT32)pInfo->ByteWidth;
	pInfo->BorderSize=Size;
	/* Allocate memory for image */
	pNewImage=(UINT8*) calloc(sizeof(UINT8),UINT32Value*realY);
	/*printf("Allocating %d bytes for image and border starting at %d\n",UINT32Value*realY,pNewImage);
	printf("pOldImage=%d\n",pInfo->pPixelStream);
	printf("pNewImage=%d\n",pNewImage);
	printf("Size=%d SmallestByteWidth=%d\n",Size,SmallestByteWidth);*/
	offset=pInfo->Height-1+Size;
	switch(bits){
		case 1:
			for(i=Size;i<Size+pInfo->Height;i++)
			{
				/*printf("To: %d  From: %d\n",pNewImage+(Size/8)+i*UINT32Value,(pInfo->ppMatrix)[offset-i]);*/
				memcpy(pNewImage+(Size/8)+i*UINT32Value,(pInfo->ppMatrix)[offset-i],SmallestByteWidth);
			}
			break;
		case 8:
			for(i=Size;i<Size+pInfo->Height;i++)
			{
				/*printf("line %d:   To: %d  From: %d\n",i,pNewImage+Size+i*UINT32Value,(pInfo->ppMatrix)[offset-i]);*/
				memcpy(pNewImage+Size+i*UINT32Value,(pInfo->ppMatrix)[offset-i],SmallestByteWidth);
			}
			break;
		case 24:
			for(i=Size;i<Size+pInfo->Height;i++)
			{
				/*printf("To: %d  From: %d\n",pNewImage+Size*3+i*UINT32Value,(pInfo->ppMatrix)[i-Size]);*/
				memcpy(pNewImage+Size*3+i*UINT32Value,(pInfo->ppMatrix)[offset-i],SmallestByteWidth);
			}
			break;
		default:
			{
				k_ShowMessageSimple(
					IPL_ERROR,
					"k_SetBorder() Only possible for 1, 8 or 24 bit per pixel");
				return false;
			}
	}
	free(pInfo->pPixelStream); /* release old image area */
	pInfo->pPixelStream=pNewImage;
	k_SetImageMatrix(pInfo);
	pInfo->ByteWidth=(UINT16) UINT32Value;
	pInfo->TotalSize=(UINT16) UINT32Value*(realY);
	k_SetBorderColor(Color,pInfo);
	return true;
}

bool k_SetBorderSize(UINT16 Size, TImage* pInfo)
{
	UINT16 bits=pInfo->Bits;
	UINT32 realX,realY,offset;
	UINT32 UINT32Value; /* temporary variable */
	UINT8* pNewImage; /* variable pointing to new allocated image area */
	UINT32 i;
	UINT32 SmallestByteWidth;
	if ((bits==1) && (Size!=0))
		Size += 8-(Size % 8);

	realX=pInfo->Width+2*Size; /* width of image + border */
	realY=pInfo->Height+2*Size; /* height of image + border */
	/* calculate size and width (incl zero-padding) of image + border */
	UINT32Value=k_GetSizeScanLine(realX,bits);
	/* Only copy memory area for the shortest width of the destination and source image */
	SmallestByteWidth=UINT32Value<(UINT32)pInfo->ByteWidth ? UINT32Value : pInfo->ByteWidth;
	pInfo->BorderSize=Size;
	/* Allocate memory for image */
	pNewImage=(UINT8*) calloc(sizeof(UINT8),UINT32Value*realY);
	offset=pInfo->Height-1+Size;
	switch(bits){
		case 1:
			for(i=Size;i<Size+pInfo->Height;i++)
				memcpy(pNewImage+(Size/8)+i*UINT32Value,(pInfo->ppMatrix)[offset-i],SmallestByteWidth);
			break;
		case 8:
			for(i=Size;i<Size+pInfo->Height;i++)
				memcpy(pNewImage+Size+i*UINT32Value,(pInfo->ppMatrix)[offset-i],SmallestByteWidth);
			break;
		case 24:
			for(i=Size;i<Size+pInfo->Height;i++)
				memcpy(pNewImage+Size*3+i*UINT32Value,(pInfo->ppMatrix)[offset-i],SmallestByteWidth);
			break;
		default:
			{
				k_ShowMessageSimple(
					IPL_ERROR,
					"k_SetBorderSize() Only possible for 1, 8 or 24 bit per pixel");
				return false;
			}
	}
	free(pInfo->pPixelStream); /* release old image area */
	pInfo->pPixelStream=pNewImage;
	k_SetImageMatrix(pInfo);
	pInfo->ByteWidth=(UINT16) UINT32Value;
	pInfo->TotalSize=(UINT16) UINT32Value*(realY);
	return true;
}


bool k_SetBorderColor(UINT32 Color, TImage* pInfo)
{
	UINT16 borderSize=pInfo->BorderSize;
	UINT16 byteWidth=pInfo->ByteWidth;
	int xmin,xmax,ymin,ymax;
	int i,j;
	int offset,col;
	T2DInt Origo=pInfo->Origo;

	pInfo->Origo.x=0;
	pInfo->Origo.y=0;

	switch(pInfo->Bits){
		case 1:
			xmax=k_GetMaxX(pInfo);
			ymin=k_GetMinY(pInfo);
			ymax=k_GetMaxY(pInfo);
			col= (Color==0) ? 0 : 255;
			/* Set upper border */
			memset(pInfo->pPixelStream,col,byteWidth*borderSize);
			offset=byteWidth*borderSize;
			for(i=ymin;i<ymax;i++)
			{
				/* Set left border */
				memset(pInfo->pPixelStream+offset,col,borderSize/8);
				/* Set right border */
				for(j=xmax; j<xmax+borderSize; j++)
				{
					k_SetPixel1bp(j,i,Color,*pInfo);
				}
				offset+=byteWidth;
			}
			/* Set lower border */
			offset=byteWidth*(borderSize+pInfo->Height);
			memset(pInfo->pPixelStream+offset,col,byteWidth*borderSize);
			break;
		case 8:
			/* Set upper border */
			memset(pInfo->pPixelStream,Color,byteWidth*borderSize);
			for(i=borderSize;i<(int)pInfo->Height+borderSize;i++){
				/* Set left border */
				memset(pInfo->ppAllScanLines[i]-borderSize,Color,borderSize);
				/* Set right border */
				memset(pInfo->ppAllScanLines[i]+pInfo->Width,Color,borderSize);
			}
			/* Set lower border */
			offset=byteWidth*(borderSize+pInfo->Height);
			memset(pInfo->pPixelStream+offset,Color,byteWidth*borderSize);
			break;
		case 24:
			xmin=k_GetMinX(pInfo);
			xmax=k_GetMaxX(pInfo);
			ymin=k_GetMinY(pInfo);
			ymax=k_GetMaxY(pInfo);
			/* Set upper border */
			for(i=ymin-borderSize;i<ymin;i++)
				for(j=xmin-borderSize; j<xmax+borderSize; j++)
					k_SetPixel24bp(j,i,Color,*pInfo);

			for(i=ymin;i<ymax;i++)
			{
				for(j=0;j<borderSize;j++)
				{
					/* Set left and right border */
					k_SetPixel24bp(j+xmin-borderSize,i,Color,*pInfo);
					k_SetPixel24bp(xmax+j,i,Color,*pInfo);
				}
			}
			/* Set lower border */
			for(i=ymax;i<ymax+borderSize;i++)
				for(j=xmin-borderSize; j<xmax+borderSize; j++)
					k_SetPixel24bp(j,i,Color,*pInfo);

			break;
		default:
			{
				k_ShowMessageSimple(
					IPL_ERROR,
					"k_SetBorderColor() Only possible for 1, 8 or 24 bit per pixel");
				pInfo->Origo=Origo; /* set origo to original value */
				return false;
			}
	}

	pInfo->Origo=Origo; /* set origo to original value */
	return true;
}

bool k_SetPeriodicBorder(unsigned short BorderSize,TImage* pInfo)
{
	if (pInfo->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_SetPeriodicBorder() Image is empty");
		return false;
	}
	else if (((unsigned int)BorderSize>pInfo->Width) || ((unsigned int)BorderSize>pInfo->Height))
	{
		k_ShowMessageSimple(IPL_ERROR, "k_SetPeriodicBorder() Border too large");
		return false;
	}
	else
	{
		int x,y;
		unsigned int w,h;
		w=pInfo->Width;
		h=pInfo->Height;
		if (pInfo->BorderSize!=BorderSize) /* only set border if necessary */
			k_SetBorderSize(BorderSize,pInfo);
		switch(pInfo->Bits)
		{
		case 1:
			/* setting upper and lover horisontal border values */
			for(y=1;y<=BorderSize;y++)
			{
				for(x=0;x<(int)pInfo->Width;x++)
				{
					k_SetPixel1bp(x,-y,k_GetPixel1bp(x,h-y,*pInfo),*pInfo); /* setting upper border */
					k_SetPixel1bp(x,h+y-1,k_GetPixel1bp(x,y-1,*pInfo),*pInfo); /* setting lower border */
				}
			}
			/* setting left and right vertical border values */
			for(x=1;x<=BorderSize;x++)
			{
				for(y=0;y<(int)pInfo->Height;y++)
				{
					k_SetPixel1bp(-x,y,k_GetPixel1bp(w-x,y,*pInfo),*pInfo); /* setting left border */
					k_SetPixel1bp(w+x-1,y,k_GetPixel1bp(x-1,y,*pInfo),*pInfo); /* setting right border */
				}
			}
			/* setting the four corners */
			for(y=1;y<=BorderSize;y++)
			{
				for(x=1;x<=BorderSize;x++)
				{
					/* setting top left corner */
					k_SetPixel1bp(-x,-y,k_GetPixel1bp(w-x,h-y,*pInfo),*pInfo);
					/* setting top right corner */
					k_SetPixel1bp(w+x-1,-y,k_GetPixel1bp(-x,h+y-1,*pInfo),*pInfo);
					/* setting bottom left corner */
					k_SetPixel1bp(-x,h+y-1,k_GetPixel1bp(w-x,y-1,*pInfo),*pInfo);
					/* setting bottom right corner */
					k_SetPixel1bp(w+x-1,h+y-1,k_GetPixel1bp(x-1,y-1,*pInfo),*pInfo);
				}
			}
			break;
		case 8:
			/* setting upper and lover horisontal border values */
			for(y=1;y<=BorderSize;y++)
			{
				for(x=0;x<(int)pInfo->Width;x++)
				{
					k_SetPixel8bp(x,-y,k_GetPixel8bp(x,h-y,*pInfo),*pInfo); /* setting upper border */
					k_SetPixel8bp(x,h+y-1,k_GetPixel8bp(x,y-1,*pInfo),*pInfo); /* setting lower border */
				}
			}
			/* setting left and right vertical border values */
			for(x=1;x<=BorderSize;x++)
			{
				for(y=0;y<(int)pInfo->Height;y++)
				{
					k_SetPixel8bp(-x,y,k_GetPixel8bp(w-x,y,*pInfo),*pInfo); /* setting left border */
					k_SetPixel8bp(w+x-1,y,k_GetPixel8bp(x-1,y,*pInfo),*pInfo); /* setting right border */
				}
			}
			/* setting the four corners */
			for(y=1;y<=BorderSize;y++)
			{
				for(x=1;x<=BorderSize;x++)
				{
					/* setting top left corner */
					k_SetPixel8bp(-x,-y,k_GetPixel8bp(w-x,h-y,*pInfo),*pInfo);
					/* setting top right corner */
					k_SetPixel8bp(w+x-1,-y,k_GetPixel8bp(-x,h+y-1,*pInfo),*pInfo);
					/* setting bottom left corner */
					k_SetPixel8bp(-x,h+y-1,k_GetPixel8bp(w-x,y-1,*pInfo),*pInfo);
					/* setting bottom right corner */
					k_SetPixel8bp(w+x-1,h+y-1,k_GetPixel8bp(x-1,y-1,*pInfo),*pInfo);
				}
			}
			break;
		case 24:
			/* setting upper and lover horisontal border values */
			for(y=1;y<=BorderSize;y++)
			{
				for(x=0;x<(int)pInfo->Width;x++)
				{
					k_SetPixel24bp(x,-y,k_GetPixel24bp(x,h-y,*pInfo),*pInfo); /* setting upper border */
					k_SetPixel24bp(x,h+y-1,k_GetPixel24bp(x,y-1,*pInfo),*pInfo); /* setting lower border */
				}
			}
			/* setting left and right vertical border values */
			for(x=1;x<=BorderSize;x++)
			{
				for(y=0;y<(int)pInfo->Height;y++)
				{
					k_SetPixel24bp(-x,y,k_GetPixel24bp(w-x,y,*pInfo),*pInfo); /* setting left border */
					k_SetPixel24bp(w+x-1,y,k_GetPixel24bp(x-1,y,*pInfo),*pInfo); /* setting right border */
				}
			}
			/* setting the four corners */
			for(y=1;y<=BorderSize;y++)
			{
				for(x=1;x<=BorderSize;x++)
				{
					/* setting top left corner */
					k_SetPixel24bp(-x,-y,k_GetPixel24bp(w-x,h-y,*pInfo),*pInfo);
					/* setting top right corner */
					k_SetPixel24bp(w+x-1,-y,k_GetPixel24bp(-x,h+y-1,*pInfo),*pInfo);
					/* setting bottom left corner */
					k_SetPixel24bp(-x,h+y-1,k_GetPixel24bp(w-x,y-1,*pInfo),*pInfo);
					/* setting bottom right corner */
					k_SetPixel24bp(w+x-1,h+y-1,k_GetPixel24bp(x-1,y-1,*pInfo),*pInfo);
				}
			}
			break;
		default:
			{
				k_ShowMessageSimple(
					IPL_ERROR,
					"k_SetPeriodicBorder() Only possible for 1, 8 or 24 bit per pixel");
				return false;
			}
		}
		return true;
	}
}

bool k_CopyImage(TImage* pDest,const TImage* pSource)
{
	if (pDest==pSource)
	{
		/* cannot copy to itself! */
		k_ShowMessageSimple(IPL_ERROR, "k_CopyImage() Source and destination image"
			" the same - doing nothing");
		return false;
	}
	/* Allocate memory if necessary */
	if ((pDest->TotalSize!=pSource->TotalSize) || (pDest->Bits!=pSource->Bits) || 
		(pDest->BorderSize!=pSource->BorderSize))
	{
		k_EmptyImage(pDest);
		pDest->TotalSize = pSource->TotalSize;
		pDest->Bits=pSource->Bits;
		pDest->BorderSize=pSource->BorderSize;
		pDest->pPixelStream=(UINT8*) malloc(pDest->TotalSize*sizeof(UINT8));
	}
	if ((pDest->Width!=pSource->Width) || (pDest->Height!=pSource->Height))
	{
		pDest->Width=pSource->Width; /* need to set width and height for k_SetImageMatrix! */
		pDest->Height=pSource->Height;
		k_SetImageMatrix(pDest);
	}
	pDest->Origin=RAM;
	pDest->Origo=pSource->Origo;
	pDest->ByteWidth=pSource->ByteWidth;
	if (pSource->Origin!=EMPTY)
	{
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%sk_CopyImage() Source image file info: %s",
				ipl_HistoryIndent, pSource->FileInfo.PathAndFileName);
			k_IplStartHistoryMacro();
		}
#endif
		/* copy file information */
		k_CopyFileInfo(&(pDest->FileInfo),&(pSource->FileInfo));
		/* copy palette */
		k_CopyPalette(&(pDest->Pal),&(pSource->Pal));
		/* copy image data */
		memcpy(pDest->pPixelStream,pSource->pPixelStream,pDest->TotalSize);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
	}
	return true;
}

bool k_CopySubImage(int xmin,int ymin,int xmax,int ymax,
				  TImage* pDest,const TImage* pSource)
{
	TImage* pTarget=pDest;
	bool CopyToSource=false;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_CopySubImage() Source image is empty - doing nothing");
		return false;
	}
	else if ((pSource->Bits!=1) && (pSource->Bits!=8) && (pSource->Bits!=24))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_WARNING,
			"k_CopySubImage() %d b/p not supported - doing nothing",
			pSource->Bits);
		return false;
	}
	/* Check if the source and destination is the same image */
	if (pDest==pSource)
	{
		pTarget=(TImage*) malloc(sizeof(TImage));
		k_InitImage(pTarget);
		CopyToSource=true;
	}
	/* Not optimised regarding speed! */
	if ((xmin<xmax) && (ymin<ymax) && 
		(xmin>=k_GetMinX(pSource)) && (ymin>=k_GetMinY(pSource)) &&
		(xmax<=k_GetMaxX(pSource)) && (ymax<=k_GetMaxY(pSource)))
	{
		UINT16 xdim,ydim;
		int i;
		int xminNoOrigo=xmin+pSource->Origo.x;
		//int xmaxNoOrigo=xmax+pSource->Origo.x;
		int yminNoOrigo=ymin+pSource->Origo.y;
		int ymaxNoOrigo=ymax+pSource->Origo.y;
		xdim=(UINT16)(xmax-xmin);
		ydim=(UINT16)(ymax-ymin);
		if ((pTarget->Bits!=pSource->Bits) || (pTarget->TotalSize!=(k_GetSizeScanLine(xdim,pSource->Bits)*ydim)))
		{
			k_EmptyImage(pTarget);
			/* prepare destination image */
			k_AllocImageFast(xdim,ydim,pSource->Bits,pTarget);
		}
		/* copy palette */
		k_CopyPalette(&(pTarget->Pal),&(pSource->Pal));
		k_CopyFileInfo(&(pTarget->FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pTarget->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pTarget->History),
				"%sk_CopySubImage(xmin=%d,ymin=%d,xmax=%d,ymax=%d)"
				" Source image file info: %s",
				ipl_HistoryIndent, xmin, ymin, xmax, ymax,
				pSource->FileInfo.PathAndFileName);
			k_IplStartHistoryMacro();
		}
#endif
		/* copy image pixels */
		if (pSource->Bits==1)
		{
			int Ext;
			if ((xdim % 8) == 0)
				Ext=0;
			else
				Ext=1;
			for(i=yminNoOrigo; i<ymaxNoOrigo; i++)
				memcpy(pTarget->ppAllScanLines[i-yminNoOrigo],pSource->ppAllScanLines[i]+(xminNoOrigo/8),(xdim/8)+Ext);
		}
		else if (pSource->Bits==8)
		{
			for(i=yminNoOrigo; i<ymaxNoOrigo; i++)
				memcpy(pTarget->ppAllScanLines[(i-yminNoOrigo)],&(pSource->ppMatrix[i][xminNoOrigo]),xdim);
		}
		else /* must be 24 b/p */
		{
			for(i=yminNoOrigo; i<ymaxNoOrigo; i++)
				memcpy(pTarget->ppAllScanLines[i-yminNoOrigo],pSource->ppAllScanLines[i]+3*xminNoOrigo,xdim*3);
		}
		pTarget->Origin=RAM;
		/* Set origo */
		pTarget->Origo=pSource->Origo;
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
	else
	{
		k_ShowMessageSimple(IPL_ERROR, "k_CopySubImage() Illegal clipping rectangel");
		return false;
	}
}

bool k_CopyConvert(UINT16 DestBits, TImage* pDest, const TImage* pSource)
{
	int x,y;
	UINT32 PalValue;
	TImage* pTarget=pDest;
	bool CopyToSource=false;
	bool ReturnValue=true;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_CopyConvert() Source image is empty - doing nothing");
		return false;
	}
	if ((DestBits!=1) && (DestBits!=8) && (DestBits!=24))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_CopyConvert() Destination with %d bits per pixel not supported",
			DestBits);
		return false;
	}
	/* Do the copying */
	/* Check if the source and destination is the same image */
	if (pDest==pSource)
	{
		pTarget=(TImage*) malloc(sizeof(TImage));
		k_InitImage(pTarget);
		CopyToSource=true;
	}
	if (DestBits==pSource->Bits)
	{	
		/* Same pixel depth in destination image - just use k_CopyImage() */
		ReturnValue=k_CopyImage(pTarget,pSource);
	}
	else
	{
		/* prepare destination image, following lines faster than calling k_CopyImage() */
		k_AllocImageFast(pSource->Width,pSource->Height,DestBits,pTarget);
		pTarget->Origo=pSource->Origo;
		/*k_CopyPalette(&(pTarget->Pal),&(pSource->Pal));*/
		k_CopyFileInfo(&(pTarget->FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pTarget->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pTarget->History),
				"%sk_CopyConvert() From %d b/p to %d b/p, "
				"source image file info: %s",
				ipl_HistoryIndent, pSource->Bits, DestBits,
				pSource->FileInfo.PathAndFileName);
			k_IplStartHistoryMacro();
		}
#endif
		if ((pSource->Bits==1) && (DestBits==24))
		{
			/* Look up the palette value for each pixel */
			for(y=0; y<(int)pSource->Height; y++)
			{
				for(x=0; x<(int)pSource->Width; x++)
				{
					k_SetPixel24bp(x,y,(pSource->Pal.pPalette[k_GetPixel1bp(x,y,*pSource)]),*pTarget);
				}
			}
		}
		else if ((pSource->Bits==8) && (DestBits==24))
		{
			/* Look up the palette value for each pixel */
			for(y=0; y<(int)pSource->Height; y++)
			{
				for(x=0; x<(int)pSource->Width; x++)
				{
					k_SetPixel24bp(x,y,(pSource->Pal.pPalette[k_GetPixel8bp(x,y,*pSource)]),*pTarget);
				}
			}
		}
		else if ((pSource->Bits==1) && (DestBits==8))
		{
			/* Copy pixel value 1 in source image to pixel value 255 in destination */
			for(y=0; y<(int)pSource->Height; y++)
			{
				for(x=0; x<(int)pSource->Width; x++)
				{
					k_SetPixel8bp(x,y,(k_GetPixel1bp(x,y,*pSource)==0 ? 0 : 255),*pTarget);
				}
			}
		}
		else if ((pSource->Bits==8) && (DestBits==1))
		{
			/* Use 128 as threshold */
			for(y=0; y<(int)pSource->Height; y++)
			{
				for(x=0; x<(int)pSource->Width; x++)
				{
					k_SetPixel1bp(x,y,(k_GetPixel8bp(x,y,*pSource)>128),*pTarget);
				}
			}
		}
		else if ((pSource->Bits==24) && (DestBits==8))
		{
			/* Use mean value of RGB */
			for(y=0; y<(int)pSource->Height; y++)
			{
				for(x=0; x<(int)pSource->Width; x++)
				{
					PalValue=k_GetPixel24bp(x,y,*pSource);
					k_SetPixel8bp(x,y,(int)((k_PalGetRVal(PalValue)+
						k_PalGetGVal(PalValue)+k_PalGetBVal(PalValue))/3+0.5),*pTarget);
				}
			}
		}
		else if ((pSource->Bits==24) && (DestBits==1))
		{
			/* Use mean value of RGB and threshold with 128 */
			for(y=0; y<(int)pSource->Height; y++)
			{
				for(x=0; x<(int)pSource->Width; x++)
				{
					PalValue=k_GetPixel24bp(x,y,*pSource);
					k_SetPixel1bp(x,y,((int)((k_PalGetRVal(PalValue)+
						k_PalGetGVal(PalValue)+k_PalGetBVal(PalValue))/3+0.5))<128 ? 0 : 1,*pTarget);
				}
			}
		}
		else
		{
			/* should never end here - just in case ! */
			k_ShowMessageStd(
				IPL_POSITION, IPL_ERROR,
				"k_CopyConvert() The combination: %d b/p to %d b/p not implemented!",
				pSource->Bits, DestBits);
			ReturnValue=false;;
		}
	}
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
	return ReturnValue;
}

bool k_CopyConvertThreshold(TImage* pDest, UINT8 Threshold, const TImage* pSource)
{
	int x,y;
	TImage* pTarget=pDest;
	bool CopyToSource=false;
	bool ReturnValue=true;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_CopyConvertThreshold() Source image is empty - doing nothing");
		return false;
	}
	/* Do the copying */
	/* Check if the source and destination is the same image */
	if (pDest==pSource)
	{
		pTarget=(TImage*) malloc(sizeof(TImage));
		k_InitImage(pTarget);
		CopyToSource=true;
	}
	if (pSource->Bits!=8)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_WARNING,
			"k_CopyConvertThreshold() Source image must have 1 b/p (current has %d b/p)",
			pSource->Bits);
		return false;
	}
	else
	{
		/* prepare destination image, following lines faster than calling k_CopyImage() */
		if ((pTarget->TotalSize!=(k_GetSizeScanLine(pSource->Width,1)*pSource->Height)) || (pTarget->Bits!=1))
			k_AllocImageFast(pSource->Width,pSource->Height,1,pTarget);
		pTarget->Origo=pSource->Origo;
		/*k_CopyPalette(&(pTarget->Pal),&(pSource->Pal));*/
		k_CopyFileInfo(&(pTarget->FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pTarget->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pTarget->History),
				"%sk_CopyConvertThreshold() From 8 b/p to 1 b/p"
				" (always), source image file info: %s",
				ipl_HistoryIndent, pSource->FileInfo.PathAndFileName);
			k_IplStartHistoryMacro();
		}
#endif
		for(y=0; y<(int)pSource->Height; y++)
		{
			for(x=0; x<(int)pSource->Width; x++)
			{
				k_SetPixel1bp(x,y,(k_GetPixel8bp(x,y,*pSource)>Threshold),*pTarget);
			}
		}
	}
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
	return ReturnValue;
}

void k_Rotate90(int Steps,TImage* pSource)
{
	TImage NewImage;
	int x,y;
	int MaxYIndex=pSource->Height-1;
	int MaxXIndex=pSource->Width-1;
	int ActSteps=(Steps % 4);
	if (ActSteps<0)
		ActSteps=4+ActSteps;

	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_Rotate90() Image is empty - doing nothing");
		return;
	}
	if ((pSource->Bits!=1) && (pSource->Bits!=8) && (pSource->Bits!=24))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_Rotate90() %d bits per pixel not supported",
			pSource->Bits);
		return;
	}

	if (ActSteps==0)
		return;

	k_InitImage(&NewImage);
	/* prepare destination image, following lines faster than calling k_CopyImage() */
	if (ActSteps==2)
		k_AllocImage(pSource->Width,pSource->Height,pSource->Bits,&NewImage);
	else
		k_AllocImage(pSource->Height,pSource->Width,pSource->Bits,&NewImage);
	k_CopyPalette(&(NewImage.Pal),&(pSource->Pal));
	k_CopyFileInfo(&(NewImage.FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
	/* copy history before adding comment */
	k_CopyText(&(NewImage.History),&(pSource->History));
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(NewImage.History),
			"%sk_Rotate90(Steps=%d)",
			ipl_HistoryIndent, Steps);
		k_IplStartHistoryMacro();
	}
#endif

	/* rotate image */
	if (ActSteps==1)
	{
		if (pSource->Bits==1)
		{
			for(y=0; y<(int)pSource->Height; y++)
				for(x=0; x<(int)pSource->Width; x++)
					k_SetPixel1bp(-y+MaxYIndex,x,k_GetPixel1bp(x,y,*pSource),NewImage);
		}
		else if (pSource->Bits==8)
		{
			for(y=0; y<(int)pSource->Height; y++)
				for(x=0; x<(int)pSource->Width; x++)
					k_SetPixel8bp(-y+MaxYIndex,x,k_GetPixel8bp(x,y,*pSource),NewImage);
		}
		else /* must be 24 b/p */
		{
			for(y=0; y<(int)pSource->Height; y++)
				for(x=0; x<(int)pSource->Width; x++)
					k_SetPixel24bp(-y+MaxYIndex,x,k_GetPixel24bp(x,y,*pSource),NewImage);
		}
	}
	else if (ActSteps==2)
	{
		if (pSource->Bits==1)
		{
			for(y=0; y<(int)pSource->Height; y++)
				for(x=0; x<(int)pSource->Width; x++)
					k_SetPixel1bp(x,MaxYIndex-y,k_GetPixel1bp(MaxXIndex-x,y,*pSource),NewImage);
		}
		else if (pSource->Bits==8)
		{
			for(y=0; y<(int)pSource->Height; y++)
				for(x=0; x<(int)pSource->Width; x++)
					k_SetPixel8bp(x,MaxYIndex-y,k_GetPixel8bp(MaxXIndex-x,y,*pSource),NewImage);
		}
		else /* must be 24 b/p */
		{
			for(y=0; y<(int)pSource->Height; y++)
				for(x=0; x<(int)pSource->Width; x++)
					k_SetPixel24bp(x,MaxYIndex-y,k_GetPixel24bp(MaxXIndex-x,y,*pSource),NewImage);
		}
	}
	else if (ActSteps==3)
	{
		if (pSource->Bits==1)
		{
			for(y=0; y<(int)pSource->Height; y++)
				for(x=0; x<(int)pSource->Width; x++)
					k_SetPixel1bp(y,MaxXIndex-x,k_GetPixel1bp(x,y,*pSource),NewImage);
		}
		else if (pSource->Bits==8)
		{
			for(y=0; y<(int)pSource->Height; y++)
				for(x=0; x<(int)pSource->Width; x++)
					k_SetPixel8bp(y,MaxXIndex-x,k_GetPixel8bp(x,y,*pSource),NewImage);
		}
		else /* must be 24 b/p */
		{
			for(y=0; y<(int)pSource->Height; y++)
				for(x=0; x<(int)pSource->Width; x++)
					k_SetPixel24bp(y,MaxXIndex-x,k_GetPixel24bp(x,y,*pSource),NewImage);
		}
	}

	k_CopyImage(pSource,&NewImage);
	k_EmptyImage(&NewImage);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

void k_FlipHorisontal(TImage* pSource)
{
	TImage Buf;
	int y;
	int Half=pSource->Height/2;
	int Ext=pSource->Height%2;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_FlipHorisontal() Image is empty - doing nothing");
		return;
	}
	if ((pSource->Bits!=1) && (pSource->Bits!=8) && (pSource->Bits!=24))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_FlipHorisontal() %d bits per pixel not supported",
			pSource->Bits);
		return;
	}

	k_InitImage(&Buf);
	/* flip image */
	/* prepare buf image */
	k_CopySubImage(k_GetMinX(pSource),k_GetMinY(pSource),k_GetMaxX(pSource),Half-pSource->Origo.y,&Buf,pSource);
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(pSource->History),
			"%sk_FlipHorisontal()",
			ipl_HistoryIndent);
		k_IplStartHistoryMacro();
	}
#endif
	for(y=0;y<Half;y++)
		memcpy(pSource->ppAllScanLines[y],pSource->ppAllScanLines[pSource->Height-y-1],pSource->ByteWidth);
	for(y=Half+Ext; y<(int)pSource->Height; y++)
		memcpy(pSource->ppAllScanLines[y],Buf.ppAllScanLines[Buf.Height-(y-Half-Ext)-1],pSource->ByteWidth);
	k_EmptyImage(&Buf);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

void k_FlipVertical(TImage* pSource)
{
	TImage NewImage;
	int x,y;
	int MaxXIndex=pSource->Width-1;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_FlipVertical() Image is empty - doing nothing");
		return;
	}
	if ((pSource->Bits!=1) && (pSource->Bits!=8) && (pSource->Bits!=24))
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_FlipVertical() %d bits per pixel not supported",
			pSource->Bits);
		return;
	}

	k_InitImage(&NewImage);
	/* flip image */
	/* prepare destination image, following lines faster than calling k_CopyImage() */
	k_AllocImage(pSource->Width,pSource->Height,pSource->Bits,&NewImage);
	k_CopyPalette(&(NewImage.Pal),&(pSource->Pal));
	k_CopyFileInfo(&(NewImage.FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
	/* copy history before adding comment */
	k_CopyText(&(NewImage.History),&(pSource->History));
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(NewImage.History),
			"%sk_FlipVertical()",
			ipl_HistoryIndent);
		k_IplStartHistoryMacro();
	}
#endif
	if (pSource->Bits==1)
	{
		for(y=0; y<(int)pSource->Height; y++)
			for(x=0; x<(int)pSource->Width; x++)
				k_SetPixel1bp(MaxXIndex-x,y,k_GetPixel1bp(x,y,*pSource),NewImage);
	}
	else if (pSource->Bits==8)
	{
		for(y=0; y<(int)pSource->Height; y++)
			for(x=0; x<(int)pSource->Width; x++)
				k_SetPixel8bp(MaxXIndex-x,y,k_GetPixel8bp(x,y,*pSource),NewImage);
	}
	else /* must be 24 b/p */
	{
		for(y=0; y<(int)pSource->Height; y++)
			for(x=0; x<(int)pSource->Width; x++)
				k_SetPixel24bp(MaxXIndex-x,y,k_GetPixel24bp(x,y,*pSource),NewImage);
	}

	k_CopyImage(pSource,&NewImage);
	k_EmptyImage(&NewImage);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

int k_GetMinX(const TImage* Img)
{
	return -Img->Origo.x;
}

int k_GetMinY(const TImage* Img)
{
	return -Img->Origo.y;
}

int k_GetMaxX(const TImage* Img)
{
	return Img->Width-Img->Origo.x;
}

int k_GetMaxY(const TImage* Img)
{
	return Img->Height-Img->Origo.y;
}

int k_InRange(int x, int y, int Delta, const TImage* Img)
{
	if ((x>=(k_GetMinX(Img)-Delta)) && (x<(k_GetMaxX(Img)+Delta)) &&
		(y>=(k_GetMinY(Img)-Delta)) && (y<(k_GetMaxY(Img)+Delta)))
		return true;
	else
		return false;
}

/****************************************/
/*        Arithmetic functions          */
/****************************************/

bool k_ApplyBias(INT16 Bias,TImage *pImg)
{ 
	if ((pImg->Origin!=EMPTY) && (pImg->Bits==8))
	{
		unsigned int x,y;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%sk_ApplyBias() Bias=%g",
				ipl_HistoryIndent, Bias);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<pImg->Height;y++)
		{
			for (x=0;x<pImg->Width;x++) 
			{
				pImg->ppMatrix[y][x]=(UINT8)(pImg->ppMatrix[y][x]+Bias);
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	} 
	else
	{
		k_ShowMessageSimple(IPL_ERROR, "k_ApplyBias(): Source image is empty or has not 8 b/p");
		return false;
	}
}

bool k_ScaleIntensity(float Prefactor,TImage *pImg)
{ 
	if ((pImg->Origin!=EMPTY) && (pImg->Bits==8))
	{
		unsigned int x,y;
		double NewValue;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%sk_ScaleIntensity() Prefactor=%g",
				ipl_HistoryIndent, Prefactor);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<pImg->Height;y++)
		{
			for (x=0;x<pImg->Width;x++)
			{
				NewValue=pImg->ppMatrix[y][x]*Prefactor;
				pImg->ppMatrix[y][x]=k_RoundUINT8(NewValue);
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
	else
	{
		k_ShowMessageSimple(IPL_ERROR, "k_ScaleIntensity: Source image is empty or has not 8 b/p");
		return false;
	}
}

bool k_Add(TImage *pImg,TImage *pAddend)
{
	if ((pImg->Origin==EMPTY) || (pAddend->Origin==EMPTY)
		|| (pImg->Width!=pAddend->Width) || (pImg->Height!=pAddend->Height)
		|| (pImg->Bits!=8)||(pAddend->Bits!=8))
	{
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_Add(): Images are empty, have different size, or does not have 8 b/p");
		return false;
	}
	else
	{
		unsigned int x,y;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%sk_Add() Adding a TImage",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
        for (y=0;y<pImg->Height;y++)
		{
			for (x=0;x<pImg->Width;x++)
			{
				pImg->ppMatrix[y][x]+=pAddend->ppMatrix[y][x];
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
        return true;
	}
}

bool k_Subtract(TImage *pImg,TImage *pSubtrahend)
{
	if ((pImg->Origin==EMPTY) || (pSubtrahend->Origin==EMPTY)
		|| (pImg->Width!=pSubtrahend->Width) || (pImg->Height!=pSubtrahend->Height)
		|| (pImg->Bits!=8) || (pSubtrahend->Bits!=8))
	{
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_Subtract(): Images are empty, have different size, or does not have 8 b/p");
		return false;
	}
	else
	{
		unsigned int x,y;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%sk_Subtract() subtracting a TImage",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
        for (y=0;y<pImg->Height;y++)
		{
			for (x=0;x<pImg->Width;x++)
			{
                pImg->ppMatrix[y][x]-=pSubtrahend->ppMatrix[y][x];
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
        return true;
	}
}

bool k_Multiply(TImage *pImg,TImage *pFactor)
{
	if ((pImg->Origin==EMPTY) || (pFactor->Origin==EMPTY)
		|| (pImg->Width!=pFactor->Width) || (pImg->Height!=pFactor->Height)
		|| (pImg->Bits!=8) || (pFactor->Bits!=8))
	{
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_Multiply(): Images are empty, have different size, or does not have 8 b/p");
		return false;
	}
	else
	{
		unsigned int x,y;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%sk_Multiply() multiplying a TImage",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
        for (y=0;y<pImg->Height;y++)
		{
			for (x=0;x<pImg->Width;x++)
			{
				pImg->ppMatrix[y][x]*=pFactor->ppMatrix[y][x];
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool k_CopyDisplace(int dx,int dy,TImage *pImg)
{
	if (pImg->Origin!=EMPTY)
	{
		int x,y,w,h,xd,yd;
		TImage Temp;
		w=pImg->Width;
		h=pImg->Height;
		k_InitImage(&Temp);
		k_AllocImage(w,h,pImg->Bits,&Temp);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(Temp.History),&(pImg->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(Temp.History),
				"%sk_CopyDisplace() dx=%d dy=%d",
				ipl_HistoryIndent, dx, dy);
			k_IplStartHistoryMacro();
		}
#endif
		switch(pImg->Bits){
			case 1:
				for (y=0;y<h;y++)
				{
					for (x=0;x<w;x++)
					{
						xd=x-dx;
						yd=y-dy;
						while (xd<0) xd+=w;
						while (xd>=w) xd-=w;
						while (yd<0) yd+=h;
						while (yd>=h) yd-=h;
						k_SetPixel1bp(xd,yd,k_GetPixel1bp(x,y,*pImg),Temp);
					}
				}
				break;
			case 8:
				for (y=0;y<h;y++)
				{
					for (x=0;x<w;x++)
					{
						xd=x-dx;
						yd=y-dy;
						while (xd<0) xd+=w;
						while (xd>=w) xd-=w;
						while (yd<0) yd+=h;
						while (yd>=h) yd-=h;
						Temp.ppMatrix[yd][xd]=pImg->ppMatrix[y][x];
					}
				}
				break;
			case 24:
				for (y=0;y<h;y++)
				{
					for (x=0;x<w;x++)
					{
						xd=x-dx;
						yd=y-dy;
						while (xd<0) xd+=w;
						while (xd>=w) xd-=w;
						while (yd<0) yd+=h;
						while (yd>=h) yd-=h;
						k_SetPixel24bp(xd,yd,k_GetPixel24bp(x,y,*pImg),Temp);
					}
				}
				break;
			default:
				{
					k_ShowMessageSimple(IPL_ERROR, "k_CopyDisplace(): Only possible for 1, 8 or 24 bit per pixel");
					return false;
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
	else
	{
		k_ShowMessageSimple(IPL_ERROR, "k_CopyDisplace(): Image is empty");
		return false;
	}
}

bool k_CutOrZeroPad(TImage* pDest,TImage *pSource)
{
	int x,y,ws,hs,wd,hd;
	ws=pSource->Width;
	hs=pSource->Height;
	wd=pDest->Width;
	hd=pDest->Height;
	if ((pSource->Origin!=EMPTY) && (pDest->Origin!=EMPTY) && 
		(ws>0) && (hs>0) && (wd>0) && (hd>0) && (pSource!=pDest))
	{
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%sk_CutOrZeroPad() Source image file info: %s",
				ipl_HistoryIndent, pSource->FileInfo.PathAndFileName);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<hd;y++)
		{
			for (x=0;x<wd;x++)
			{ 
				if ((x<ws)&&(y<hs))
				{
					pDest->ppMatrix[y][x]=pSource->ppMatrix[y][x]; 
				}
				else
				{
					pDest->ppMatrix[y][x]=0;
				}
			}
		} 
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
	else
	{
		if (pSource==pDest)
		{
			k_ShowMessageSimple(IPL_ERROR, "k_CutOrZeroPad(): Cannot use same image as dest/source"); 
		}
		else if ((ws<=0)||(hs<=0)||(wd<=0)||(hd<=0))
		{
			k_ShowMessageSimple(IPL_ERROR, "k_CutOrZeroPad(): One or both images have no pixels");
		}
		else
		{
			k_ShowMessageSimple(IPL_ERROR, "k_CutOrZeroPad(): One or both images are empty");
		}
		return false;
	}
}

bool k_CopyFromInt(TImage *pDest,TIntImage *pSource)
{
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_CopyFromInt(): Image is empty");
		return false;
	}
	else
	{
		int x,y,w,h;
		w=pSource->Width;
		h=pSource->Height;
		k_AllocImage(w,h,8,pDest);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%sk_CopyFromInt()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<h;y++)
		{
			for (x=0;x<w;x++)
			{
				pDest->ppMatrix[y][x]=(UINT8)(pSource->ppMatrix[y][x]);
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool k_CopyFromFloat(TImage *pDest,TFloatImage *pSource)
{
	int x,y,w,h;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_CopyFromFloat(): Image is empty");
		return false;
	}
	else
	{
		w=pSource->Width;
		h=pSource->Height;
		k_AllocImage(w,h,8,pDest);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%sk_CopyFromFloat()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<h;y++)
		{
			for (x=0;x<w;x++)
			{
				pDest->ppMatrix[y][x]=(UINT8)(pSource->ppMatrix[y][x]);
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool k_CopyFromComplex(COMPLEXCHOISE mode, TImage *pDest,TComplexImage *pSource)
{
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_CopyFromComplex(): Image is empty");
		return false;
	}
	else
	{
		double buf;
		int x,y,w,h;
        w=pSource->Width;
        h=pSource->Height;
        k_AllocImage(w,h,8,pDest);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%sk_CopyFromComplex()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
        if (mode==COMPLEX_REAL)
        {
			for (y=0;y<h;y++)
			{
				for (x=0;x<w;x++)
				{
					pDest->ppMatrix[y][x]=(UINT8)(pSource->ppMatrixRe[y][x]);
				}
			}
        }
        if (mode==COMPLEX_IMAGINARY)
        {
			for (y=0;y<h;y++)
			{
				for (x=0;x<w;x++)
				{
					pDest->ppMatrix[y][x]=(UINT8)(pSource->ppMatrixIm[y][x]);
				}
			}
        } 
		else /* assumes COMPLEX_MODULUS */
        {
			for (y=0;y<h;y++)
			{
				for (x=0;x<w;x++)
				{
					buf=sqrt(pSource->ppMatrixRe[y][x]*pSource->ppMatrixRe[y][x]+
						pSource->ppMatrixIm[y][x]*pSource->ppMatrixIm[y][x]);
					pDest->ppMatrix[y][x]=(UINT8)buf;
				}
			}
        }
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
        return true;
	}
}

void k_PrintImageInfo(const TImage* pInfo)
{
	char* Name;
	k_GetOriginString(pInfo->Origin,&Name);
	printf("**************** Image info ********************\n");
	printf(" Width=%ld Height=%ld ByteWidth=%d SizeImage=%ld Bits=%d\n",
		pInfo->Width, pInfo->Height, pInfo->ByteWidth, pInfo->TotalSize, pInfo->Bits);
	printf(" PalEntries=%d BorderSize=%d Origo=(%d,%d) Origin=%s\n",
		pInfo->Pal.PalEntries,pInfo->BorderSize,pInfo->Origo.x,
		pInfo->Origo.y,Name);
	k_PrintFileInfo(&pInfo->FileInfo);
	free(Name);
}

bool k_DrawLine(T2DInt Start, T2DInt End, UINT32 Color, TImage *pDest)
{
	TPixelGroup grp;
	if (pDest==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_DrawLine(): Image is empty");
		return false;
	}
	k_InitGroup(&grp);
	k_GraphicsGenerateLine(Start,End,&grp);
	k_AddGroupToImage(pDest,&grp,Color);
	k_EmptyGroup(&grp);
	return true;
}

bool k_DrawCircle(T2DInt Center, unsigned int Radius, UINT32 Color, TImage *pDest)
{
	TPixelGroup grp;
	if (pDest==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_DrawCircle(): Image is empty");
		return false;
	}
	k_InitGroup(&grp);
	k_GraphicsGenerateCircle(Center,Radius,&grp);
	k_AddGroupToImage(pDest,&grp,Color);
	k_EmptyGroup(&grp);
	return true;
}

/****************************************/
/*           C only functions           */
/****************************************/

UINT32 k_GetPixel(int x, int y,const TImage* pInfo)
{
	UINT16 border=pInfo->BorderSize;
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if ((x>=-border) && (x<(int) pInfo->Width+border) &&
		(y>=-border) && (y<(int) pInfo->Height+border))
	{
		switch(pInfo->Bits) {
		case 1:
			return k_GetPixel1bp(x,y,(*pInfo));
		case 8:
			return k_GetPixel8bp(x,y,(*pInfo));
		case 24:
			return k_GetPixel24bp(x,y,(*pInfo));
		default:
			{
				k_ShowMessageSimple(
					IPL_ERROR,
					"k_GetPixel() Only implemented for 1,8 and 24 bit per pixels");
				return 0;
			}
		}
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_GetPixel(%d,%d) Outside image and border",
			x, y);
		return 0;
	}
}

bool k_SetPixel(int x, int y, UINT32 Color,TImage* pInfo)
{
	UINT16 border=pInfo->BorderSize;
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if((x>=-border) && (x<(int) pInfo->Width+border) &&
		(y>=-border) && (y<(int) pInfo->Height+border))
	{
		switch(pInfo->Bits){
		case 1:
			k_SetPixel1bp(x,y,Color,(*pInfo));
			return true;
		case 8:
			k_SetPixel8bp(x,y,Color,(*pInfo));
			return true;
		case 24:
			k_SetPixel24bp(x,y,Color,(*pInfo));
			return true;
		default:
			{
				k_ShowMessageSimple(
					IPL_ERROR,
					"k_SetPixel() Only implemented for 1,8 and 24 bit per pixels");
				return false;
			}
		}
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_SetPixel(%d,%d) Outside image and border",
			x, y);
		return false;
	}
}

UINT32 k_GetPixelFast(int x, int y,const TImage* pInfo)
{
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	switch(pInfo->Bits) {
	case 1:
		return k_GetPixel1bp(x,y,(*pInfo));
	case 8:
		return k_GetPixel8bp(x,y,(*pInfo));
	case 24:
		return k_GetPixel24bp(x,y,(*pInfo));
	default:
		{
			k_ShowMessageSimple(IPL_ERROR, "k_GetPixelFast() Only implemented for 1,8 and 24 bit per pixels");
			return 0;
		}
	}
}

bool k_SetPixelFast(int x, int y, UINT32 Color,TImage* pInfo)
{
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	switch(pInfo->Bits){
	case 1:
		k_SetPixel1bp(x,y,Color,(*pInfo));
		return true;
	case 8:
		k_SetPixel8bp(x,y,Color,(*pInfo));
		return true;
	case 24:
		k_SetPixel24bp(x,y,Color,(*pInfo));
		return true;
	default:
		{
			k_ShowMessageSimple(IPL_ERROR, "k_SetPixelFast() Only implemented for 1,8 and 24 bit per pixels");
			return false;
		}
	}
}

float k_GetPixelInterpolated(float x, float y, const TImage* pInfo)
{
	/* for graytone images only
	   for 1,4 and 8 b/p: uses bilinar interpolation */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if ((pInfo->Bits!=1) && (pInfo->Bits!=4) && (pInfo->Bits!=8))
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GetPixelInterpolated() Image must be 1, 4 or 8 b/p");
		return 0;
	}

	if ((x>-pInfo->BorderSize) && (x<(int) pInfo->Width+pInfo->BorderSize-1) &&
		(y>-pInfo->BorderSize) && (y<(int) pInfo->Height+pInfo->BorderSize-1))
	{
		/* upper left pixel (l ~ low) */
		int xl = (x >=0 ? (int)x : (int)(x-1)); /* always round down */
		int yl = (y >=0 ? (int)y : (int)(y-1)); /* always round down */
		/* lower right pixel (h ~ high) */
		int xh = xl + 1;
		int yh = yl + 1;
		/* do bilinear interpolation */
		switch(pInfo->Bits)
		{
		case 1:
			return ((k_GetPixel1bp(xl,yl,(*pInfo))*((float)xh-x) + k_GetPixel1bp(xh,yl,(*pInfo))*(x-(float)xl))*((float)yh-y) +
			 (k_GetPixel1bp(xl,yh,(*pInfo))*((float)xh-x) + k_GetPixel1bp(xh,yh,(*pInfo))*(x-(float)xl))*(y-(float)yl));
		case 4:
			{
				k_ShowMessageSimple(IPL_ERROR, "k_GetPixelInterpolated() Image with 4 b/p not implemented yet");
				return 0;
			}
		case 8:
			return ((k_GetPixel8bp(xl,yl,(*pInfo))*((float)xh-x) + k_GetPixel8bp(xh,yl,(*pInfo))*(x-(float)xl))*((float)yh-y) +
			 (k_GetPixel8bp(xl,yh,(*pInfo))*((float)xh-x) + k_GetPixel8bp(xh,yh,(*pInfo))*(x-(float)xl))*(y-(float)yl));
		default:
			{
				k_ShowMessageStd(
					IPL_POSITION, IPL_ERROR,
					"k_GetPixelInterpolated() Pixel depth (b/p) must be 1 or 8"
					" (current is %d b/p)\n",
					pInfo->Bits);
				return -1;
			}
		}
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_GetPixelInterpolated(%.2f,%.2f) Needs "
			"to access pixels outside image and border\n",
			x, y);
		return -1;
	}
}

float k_GetPixelInterpolatedFast(float x, float y, const TImage* pInfo)
{
	/* for graytone images only
	   for 1,4 and 8 b/p: uses bilinar interpolation
	   upper left pixel (l ~ low) */
	int xl = (x >=0 ? (int)x : (int)(x-1)); /* always round down */
	int yl = (y >=0 ? (int)y : (int)(y-1)); /* always round down */
	/* lower right pixel (h ~ high) */
	int xh = xl + 1;
	int yh = yl + 1;
	/* do bilinear interpolation */
	switch(pInfo->Bits)
	{
	case 1:
		return ((k_GetPixel1bp(xl,yl,(*pInfo))*((float)xh-x) + k_GetPixel1bp(xh,yl,(*pInfo))*(x-(float)xl))*((float)yh-y) +
		 (k_GetPixel1bp(xl,yh,(*pInfo))*((float)xh-x) + k_GetPixel1bp(xh,yh,(*pInfo))*(x-(float)xl))*(y-(float)yl));
	case 4:
		{
			k_ShowMessageSimple(IPL_ERROR, "k_GetPixelInterpolatedFast() Image with 4 b/p not implemented yet");
			return 0;
		}
	case 8:
		return ((k_GetPixel8bp(xl,yl,(*pInfo))*((float)xh-x) + k_GetPixel8bp(xh,yl,(*pInfo))*(x-(float)xl))*((float)yh-y) +
		 (k_GetPixel8bp(xl,yh,(*pInfo))*((float)xh-x) + k_GetPixel8bp(xh,yh,(*pInfo))*(x-(float)xl))*(y-(float)yl));
	default:
		{
			k_ShowMessageStd(
				IPL_POSITION, IPL_ERROR,
				"k_GetPixelInterpolatedFast() Pixel depth (b/p) must be 1 or 8"
				" (current is %d b/p)\n",
				pInfo->Bits);
			return -1;
		}
	}
}

UINT32 k_GetColorInterpolated(float x, float y, const TImage* pInfo)
{
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if (pInfo->Bits!=24)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GetColorInterpolated() Image must be 24 b/p");
		return 0;
	}

	if ((x>-pInfo->BorderSize) && (x<(int) pInfo->Width+pInfo->BorderSize-1) &&
		(y>-pInfo->BorderSize) && (y<(int) pInfo->Height+pInfo->BorderSize-1))
	{
		/* upper left pixel (l ~ low) */
		int xl = (x >=0 ? (int)x : (int)(x-1)); /* always round down */
		int yl = (y >=0 ? (int)y : (int)(y-1)); /* always round down */
		/* lower right pixel (h ~ high) */
		int xh = xl + 1;
		int yh = yl + 1;
		/* do bilinear interpolation */
		UINT32 UL=k_GetPixel24bp(xl,yl,(*pInfo)); /* Upper left pixel */
		UINT32 UR=k_GetPixel24bp(xh,yl,(*pInfo)); /* Upper right pixel */
		UINT32 LL=k_GetPixel24bp(xl,yh,(*pInfo)); /* Lower left pixel */
		UINT32 LR=k_GetPixel24bp(xh,yh,(*pInfo)); /* Lower right pixel */
		UINT32 RVal=(int)(((k_PalGetRVal(UL)*(xh-x) + k_PalGetRVal(UR)*(x-xl))*(yh-y) +
						(k_PalGetRVal(LL)*(xh-x) + k_PalGetRVal(LR)*(x-xl))*(y-yl))+0.5);
		UINT32 GVal=(int)(((k_PalGetGVal(UL)*(xh-x) + k_PalGetGVal(UR)*(x-xl))*(yh-y) +
						(k_PalGetGVal(LL)*(xh-x) + k_PalGetGVal(LR)*(x-xl))*(y-yl))+0.5);
		UINT32 BVal=(int)(((k_PalGetBVal(UL)*(xh-x) + k_PalGetBVal(UR)*(x-xl))*(yh-y) +
						(k_PalGetBVal(LL)*(xh-x) + k_PalGetBVal(LR)*(x-xl))*(y-yl))+0.5);
		return k_PalCreateRGB(RVal,GVal,BVal);
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_GetColorInterpolated(%.2f,%.2f) Needs "
			"to access pixels outside image and border\n",
			x, y);
		return 0;
	}
}

UINT32 k_GetColorInterpolatedFast(float x, float y, const TImage* pInfo)
{
	/* upper left pixel (l ~ low) */
	int xl = (x >=0 ? (int)x : (int)(x-1)); /* always round down */
	int yl = (y >=0 ? (int)y : (int)(y-1)); /* always round down */
	/* lower right pixel (h ~ high) */
	int xh = xl + 1;
	int yh = yl + 1;
	/* do bilinear interpolation */
	UINT32 UL=k_GetPixel24bp(xl,yl,(*pInfo)); /* Upper left pixel */
	UINT32 UR=k_GetPixel24bp(xh,yl,(*pInfo)); /* Upper right pixel */
	UINT32 LL=k_GetPixel24bp(xl,yh,(*pInfo)); /* Lower left pixel */
	UINT32 LR=k_GetPixel24bp(xh,yh,(*pInfo)); /* Lower right pixel */
	UINT32 RVal=(int)(((k_PalGetRVal(UL)*(xh-x) + k_PalGetRVal(UR)*(x-xl))*(yh-y) +
					(k_PalGetRVal(LL)*(xh-x) + k_PalGetRVal(LR)*(x-xl))*(y-yl))+0.5);
	UINT32 GVal=(int)(((k_PalGetGVal(UL)*(xh-x) + k_PalGetGVal(UR)*(x-xl))*(yh-y) +
					(k_PalGetGVal(LL)*(xh-x) + k_PalGetGVal(LR)*(x-xl))*(y-yl))+0.5);
	UINT32 BVal=(int)(((k_PalGetBVal(UL)*(xh-x) + k_PalGetBVal(UR)*(x-xl))*(yh-y) +
					(k_PalGetBVal(LL)*(xh-x) + k_PalGetBVal(LR)*(x-xl))*(y-yl))+0.5);
	return k_PalCreateRGB(RVal,GVal,BVal);
}



/************************************************/
/********        Private functions       ********/
/************************************************/

UINT32 k_GetSizeScanLine(UINT32 Width, UINT16 Bits)
{
	/* returns the size of a scanline incl. zero padding in bytes */
	UINT32 UINT32Value = (Width * Bits) / 32;
	if((Width * Bits) % 32)
	{
		UINT32Value++;
	}
	UINT32Value *= 4;
	return UINT32Value;
}

void k_SetImageMatrix(TImage* pInfo)
{
	/* Sets "pInfo->ppMatrix[y]" to point to start of every scan line
	   inside image and set the pInfo->ppMatrix[y][x]
	   call this function after pInfo->BorderSize is set */
	UINT32 y;
	UINT16 border=pInfo->BorderSize;
	UINT32 totalHeight=pInfo->Height+2*border;
	UINT16 bits=pInfo->Bits;
	UINT32 realX=pInfo->Width+2*border; /* width of image + border */
	UINT32 UINT32Value; /* temporary variable */
	/* calculate size and width (incl zero-padding) of image + border */
	UINT32Value=k_GetSizeScanLine(realX,bits);
	/*printf("totalHeight=%d\n",totalHeight);*/
	if (pInfo->ppAllScanLines!=NULL)
	{
		free(pInfo->ppAllScanLines);
	}

	(pInfo->ppAllScanLines) = (UINT8**) malloc(sizeof(UINT8*)*(totalHeight));
	pInfo->ppMatrix = &((pInfo->ppAllScanLines)[border]);

	switch(pInfo->Bits){
		case 1:
			for(y=0;y<totalHeight;y++)
			{
				(pInfo->ppAllScanLines)[totalHeight-1-y]=(pInfo->pPixelStream+y*UINT32Value+border/8);
			}
			break;
		case 8:
			for(y=0;y<totalHeight;y++)
			{
				(pInfo->ppAllScanLines)[totalHeight-1-y]=(pInfo->pPixelStream+y*UINT32Value+border);
			}
			break;
		case 24:
			for(y=0;y<totalHeight;y++)
			{
				(pInfo->ppAllScanLines)[totalHeight-1-y]=(pInfo->pPixelStream+y*UINT32Value+border*3);
			}
			break;
		default:
			{
				k_ShowMessageSimple(IPL_ERROR, "k_SetImageMatrix() Only possible for 1, 8 or 24 bit per pixel");
			}
	}
}

/* BITMAPINFOHEADER and BITMAPFILEHEADER structs need to be moved out of k_LoadBMP 
	when compiling with Borland 4.0, it's a bug in Borlands compiler */

bool k_LoadBMP(FILE* fp,TImage* pInfo)
{
	typedef struct{ /* info header structure */
	   UINT32  biSize;
	   INT32   biWidth;
	   INT32   biHeight;
	   UINT16   biPlanes;
	   UINT16   biBitCount;
	   UINT32  biCompression;
	   UINT32  biSizeImage;
	   INT32   biXPelsPerMeter;
	   INT32   biYPelsPerMeter;
	   UINT32  biClrUsed;
	   UINT32  biClrImportant;
	}BITMAPINFOHEADER;
	typedef struct{ /* file header structure */
		UINT16    bfType;
		UINT32   bfSize;
		UINT16    bfReserved1;
		UINT16    bfReserved2;
		UINT32   bfOffBits;
	}BITMAPFILEHEADER;
	BITMAPINFOHEADER tInfoHeader;
	BITMAPFILEHEADER tFileHeader;
 	UINT32 UINT32Value; /* temporary variable */
	const UINT8 sizeInfoHeader=54; /* offset for beginning of palette */

	/* Read file header */
	tFileHeader.bfType=k_ReadUINT16(fp);
	if(tFileHeader.bfType != 0x4d42) /* 'BM' */
	{
		k_ShowMessageSimple(IPL_ERROR, "k_LoadBMP() Not a BMP file");
		fclose(fp);
		return false;
	}
	tFileHeader.bfSize=k_ReadUINT32(fp); /* size of bitmap file (in bytes) */
	tFileHeader.bfReserved1=k_ReadUINT16(fp); /* reserved */
	tFileHeader.bfReserved1=k_ReadUINT16(fp); /* reserved */
	tFileHeader.bfOffBits=k_ReadUINT32(fp); /* offset, in bytes, from the file header to the bitmap bits */

	/* Read info header */
	tInfoHeader.biSize=k_ReadUINT32(fp); /* read UINT32 */
	tInfoHeader.biWidth=k_ReadUINT32(fp); /* read INT32 */
	tInfoHeader.biHeight=k_ReadUINT32(fp); /* read INT32 */
	tInfoHeader.biPlanes=k_ReadUINT16(fp); /* read UINT16 */
	tInfoHeader.biBitCount=k_ReadUINT16(fp); /* read UINT16 */
	tInfoHeader.biCompression=k_ReadUINT32(fp); /* read UINT32 */
	tInfoHeader.biSizeImage=k_ReadUINT32(fp); /* read UINT32 */
	tInfoHeader.biXPelsPerMeter=k_ReadUINT32(fp); /* read INT32 */
	tInfoHeader.biYPelsPerMeter=k_ReadUINT32(fp); /* read INT32 */
	tInfoHeader.biClrUsed=k_ReadUINT32(fp); /* read UINT32 */
	tInfoHeader.biClrImportant=k_ReadUINT32(fp); /* read UINT32 */

	/* set values in the image info structure */
	pInfo->Width=tInfoHeader.biWidth;
	pInfo->Height=tInfoHeader.biHeight;
	UINT32Value=k_GetSizeScanLine((UINT32) tInfoHeader.biWidth,tInfoHeader.biBitCount);
	pInfo->ByteWidth = (UINT16) UINT32Value;
	pInfo->TotalSize = UINT32Value * tInfoHeader.biHeight; /* no compr. */
	pInfo->Bits=tInfoHeader.biBitCount;
	pInfo->BorderSize=0; /* always 0 when loading an image */

	/* check if Windows 3.x or Windows NT BMP */
	if (tInfoHeader.biSize!=40)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_LoadBMP() Not a windows BMP file");
		fclose(fp);
		return false;
	}
	/* check if compressed */
	if (tInfoHeader.biCompression!=0)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_LoadBMP() Cannot read compressed BMP files");
		fclose(fp);
		return false;
	}

	/* Read palette */
	pInfo->Pal.PalEntries=(UINT16) ((tFileHeader.bfOffBits-sizeInfoHeader)/sizeof(UINT32)); /* Set Palette entries */
	if (sizeInfoHeader<tFileHeader.bfOffBits)
	{
#ifdef IPL_BIG_ENDIAN
		unsigned int count;
		pInfo->Pal.pPalette=(UINT32*) malloc(tFileHeader.bfOffBits-sizeInfoHeader);
		for(count=0;count<tInfoHeader.biClrUsed;count++)
			*(pInfo->Pal.pPalette+count)=k_ReadUINT32(fp);
#else
		pInfo->Pal.pPalette=(UINT32*) malloc(tFileHeader.bfOffBits-sizeInfoHeader);
		fread(pInfo->Pal.pPalette,sizeof(UINT8),tFileHeader.bfOffBits-sizeInfoHeader,fp);
#endif /* IPL_BIG_ENDIAN */		
	}

	/* Read bitmap data */
	pInfo->pPixelStream=(UINT8*) malloc(pInfo->TotalSize);
	if(fread(pInfo->pPixelStream,sizeof(UINT8),pInfo->TotalSize,fp) != pInfo->TotalSize)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_LoadBMP() Failed reading image data");
		fclose(fp);
		return false;
	}
	return true;
}

bool k_SaveBMP(FILE* fp,TImage* pInfo)
{
	if (pInfo->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_SaveBMP() No image to save");
		return false;
	}
	else
	{
		int y;
		const UINT8 sizeInfoHeader=54; /* offset for beginning of palette */
		/* calculate size and width (incl zero-padding) of image */
		UINT32 UINT32Value=k_GetSizeScanLine(pInfo->Width,pInfo->Bits);

		/* Write file header */
		k_WriteUINT16(fp,0x4d42); /* 'BM' */
		k_WriteUINT32(fp,sizeInfoHeader+pInfo->Pal.PalEntries*sizeof(UINT32)+
			pInfo->TotalSize); /* size of bitmap file (in bytes) */
		k_WriteUINT16(fp,0); /* reserved */
		k_WriteUINT16(fp,0); /* reserved */
		/* offset, in bytes, from the file header to the bitmap bits */
		k_WriteUINT32(fp,sizeInfoHeader+pInfo->Pal.PalEntries*sizeof(UINT32));

		/* Write info header (elements from BITMAPINFOHEADER) */
		k_WriteUINT32(fp,40);					/* write biSize (UINT32) */
		k_WriteUINT32(fp,pInfo->Width);		/* write biWidth (INT32) */
		k_WriteUINT32(fp,pInfo->Height);	/* write biHeight (INT32) */
		k_WriteUINT16(fp,1);						/* write biPlanes (UINT16) */
		k_WriteUINT16(fp,pInfo->Bits);		/* write biBitCount (UINT16) */
		k_WriteUINT32(fp,0);					/* write biCompression (UINT32) */
		k_WriteUINT32(fp,pInfo->TotalSize);	/* write biSizeImage (UINT32) */
		k_WriteUINT32(fp,0);					/* write biXPelsPerMeter (INT32) */
		k_WriteUINT32(fp,0);					/* write biYPelsPerMeter (INT32) */
		k_WriteUINT32(fp,pInfo->Pal.PalEntries);/* write biClrUsed (UINT32) */
		k_WriteUINT32(fp,pInfo->Pal.PalEntries);/* write biClrImportant (UINT32) */

		/* Write palette */
#ifdef IPL_BIG_ENDIAN
		k_WriteBigEndianPalette(&pInfo->Pal,&fp);
#else
		fwrite(pInfo->Pal.pPalette,sizeof(UINT32),pInfo->Pal.PalEntries,fp);
#endif /* IPL_BIG_ENDIAN */

		/* Write bitmap data */
		for(y=pInfo->Height-1;y>=0;y--)
		{
			fwrite((pInfo->ppMatrix)[y],sizeof(UINT8),UINT32Value,fp);
		}

	}
	return true;
}

#ifdef IPL_BIG_ENDIAN
void k_WriteBigEndianPalette(TPalette* pPal,FILE** fp)
{
	int x;
	UINT32 SwappedPalEntry;
	for(x=0; x<pPal->PalEntries; x++)
	{
		SwappedPalEntry=k_SWAP4BYTES(pPal->pPalette[x]);
		fwrite(&SwappedPalEntry,sizeof(UINT32),1,*fp);
	}
}
#endif /* IPL_BIG_ENDIAN */

void k_ReadComments(FILE *fp,char** ppComments)
{
	char* tempStr=(char*)malloc(sizeof(char));
	char ch;
	int count,TotalCharRead=0;
	bool Continue=true;
	while(Continue==true)
	{
		fscanf(fp, "%c", &ch);
		count=1;
		TotalCharRead++;
		if (ch == '#') 
		{
			do 
			{
				fscanf(fp, "%c", &ch);
				count++;
				TotalCharRead++;
			} while (ch != '\n');
			tempStr=(char*)realloc(tempStr,TotalCharRead);
			fseek(fp,-count,SEEK_CUR);
			fread(tempStr+TotalCharRead-count,sizeof(char),count,fp);
		}
		else
		{
			ungetc(ch,fp);
			Continue=false;
		}
	}
	if (ppComments!=NULL)
	{
		/* copy the tempStr to ppComments */
		*ppComments=(char*)malloc(TotalCharRead+1);
		memcpy(*ppComments,tempStr,TotalCharRead);
		(*ppComments)[TotalCharRead-1]='\0';
	}
	free(tempStr);
}

bool k_LoadPGM(FILE* fp,TImage* pInfo)
{
	char* pComments=NULL;
	UINT16 Type=k_ReadUINT16(fp);
	int MaxVal; /* holds the number of gray tones in PGM */
	UINT32 y; /* used to count scanlines for loading image data */
	UINT8* pImageTemp; /* used for loading image data */
	if(Type == 0x3250) /* 'P2' */
	{
		k_ShowMessageSimple(IPL_ERROR, "k_LoadPGM() PGM in ascii format not supported");
		fclose(fp);
		return false;
	}
	else if (Type != 0x3550) /* 'P5' */
	{
		k_ShowMessageSimple(IPL_ERROR, "k_LoadPGM() Not a PGM file");
		fclose(fp);
		return false;
	}
	/* Read file header */
	if (k_SkipWhiteSpace(fp)<1)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_LoadPGM() Invalid header (1)");
		fclose(fp);
		return false;
	}
	/* Read comments into the history member of pInfo */
	k_ReadComments(fp,&pComments);
	k_CharArrayToListText(&(pInfo->History),pComments);
	if (pComments!=NULL)
		free(pComments);

	fscanf(fp,"%i", (int *)&pInfo->Width); /* width of image */
	if (k_SkipWhiteSpace(fp)<1)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_LoadPGM() Invalid header (2)");
		fclose(fp);
		return false;
	}
	fscanf(fp,"%i", (int *)&pInfo->Height); /* height of image */
	fscanf(fp,"%i",&MaxVal); /* highest gray tone value */
	if (MaxVal!=255)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_LoadPGM() Number of gray tones must be 255 (actual is %d)",
			MaxVal);
		fclose(fp);
		return false;
	}
	pInfo->Bits=8; /* allways 8 bits per pixel! */
	pInfo->ByteWidth = (UINT16) k_GetSizeScanLine(pInfo->Width,pInfo->Bits);
	pInfo->TotalSize = pInfo->ByteWidth*pInfo->Height; /* no compr. */
	pInfo->BorderSize = 0; /* always 0 when loading an image */
	/* Create gray palette */
	k_CreatePal(pInfo->Bits,&pInfo->Pal); /* alloc palette and init to gray tones */

	/* Read bitmap data */
	pInfo->pPixelStream=(UINT8*) malloc(pInfo->TotalSize);
	pImageTemp=pInfo->pPixelStream+pInfo->TotalSize-pInfo->ByteWidth;
	fread(pImageTemp,sizeof(UINT8),1,fp); /* read LF from file */
	for(y=0; y<pInfo->Height; y++)
	{
		if (fread(pImageTemp,sizeof(UINT8),pInfo->Width,fp) != pInfo->Width)
		{
			k_ShowMessageSimple(IPL_ERROR, "k_LoadPGM() Failed reading image data");
			fclose(fp);
			return false;
		}
		pImageTemp-=pInfo->ByteWidth;
	}
	return true;
}

bool k_SavePGM(FILE* fp,TImage* pInfo)
{
	if (pInfo->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_SavePGM() No image to save");
		fclose(fp);
		return false;
	}
	/* check bits per pixel */
	else if (pInfo->Bits != 8)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_SavePGM() Image must be 8 b/p (actual is %d b/p)",
			pInfo->Bits);
		fclose(fp);
		return false;
	}
	else
	{
		char* pHistory=NULL;
		UINT32 count=0; /* used to check palette and count scanlines */
		UINT8* pImageTemp; /* used for loading image data */
		UINT32 palVal;
		/* check palette for gray tones */
		while((UINT16)count<pInfo->Pal.PalEntries)
		{
			k_GetPalValue(count,&palVal,&pInfo->Pal);
			if (((UINT32)k_PalGetRVal(palVal)!=count) || ((UINT32)k_PalGetGVal(palVal)!=count) || ((UINT32)k_PalGetBVal(palVal)!=count))
				break;
			count++;
		}
		if (count!=(UINT32)pInfo->Pal.PalEntries)
		{
			k_ShowMessageSimple(IPL_WARNING, "k_SavePGM()  Not a gray tone palette (saving anyway!)");
		}

		/* save header */
		k_ListToCharArrayText(&(pInfo->History),&pHistory);
		if (pHistory!=NULL)
		{
			fprintf(fp, "P5\n%s%i %i\n255\n", pHistory, (int)pInfo->Width, (int)pInfo->Height);
			free(pHistory);
		}
		else
		{
			fprintf(fp, "P5\n%i %i\n255\n", (int)pInfo->Width, (int)pInfo->Height);
		}
		/* Save bitmap data */
		pImageTemp=pInfo->pPixelStream+pInfo->TotalSize-pInfo->ByteWidth;
		/* remember border offset */
		pImageTemp-=(pInfo->BorderSize*pInfo->ByteWidth-pInfo->BorderSize);
		for(count=0; count<pInfo->Height; count++)
		{
			fwrite(pImageTemp,sizeof(UINT8),pInfo->Width,fp);
			pImageTemp-=pInfo->ByteWidth;
		}
	}
	return true;
}


/* --- YUV/YCbCr To RGB --- */
#define IPL_PCD_RANGE       320

#define IPL_PCD_CR_NUL      137
#define IPL_PCD_CB_NUL      156

#define IPL_PCD_LUN_MUL     360
#define IPL_PCD_CR_MUL      512
#define IPL_PCD_CB_MUL      512
#define IPL_PCD_GREEN1_MUL  (-IPL_PCD_CR_MUL/2)      /* -256  */
#define IPL_PCD_GREEN2_MUL  (-IPL_PCD_CB_MUL/6)      /* -85   */

#define IPL_PCD_CR_SUB      (IPL_PCD_CR_NUL*IPL_PCD_CR_MUL) /* -137*512 */
#define IPL_PCD_CB_SUB      (IPL_PCD_CB_NUL*IPL_PCD_CB_MUL) /* -156*512 */
#define IPL_PCD_GREEN1_ADD  (IPL_PCD_CR_SUB/2)      /* -137*256 */
#define IPL_PCD_GREEN2_ADD  (IPL_PCD_CB_SUB/6)      /*  -52*256 */

static unsigned int  IPL_PCD_LUT_Gray[256];
static unsigned int  IPL_PCD_LUT_Cr[256];
static unsigned int  IPL_PCD_LUT_Cb[256];
static unsigned int  IPL_PCD_LUT_Green1[256];
static unsigned int  IPL_PCD_LUT_Green2[256];
static unsigned int  IPL_PCD_LUT_Range[256 + 2 * IPL_PCD_RANGE];

#define IPL_PCD_GET_RED(cr,y)      (IPL_PCD_LUT_Range[IPL_PCD_RANGE+IPL_PCD_LUT_Gray[y]+IPL_PCD_LUT_Cr[cr]])
#define IPL_PCD_GET_GREEN(cr,cb,y) (IPL_PCD_LUT_Range[IPL_PCD_RANGE+IPL_PCD_LUT_Gray[y]+IPL_PCD_LUT_Green1[cr]+IPL_PCD_LUT_Green2[cb]])
#define IPL_PCD_GET_BLUE(cb,y)     (IPL_PCD_LUT_Range[IPL_PCD_RANGE+IPL_PCD_LUT_Gray[y]+IPL_PCD_LUT_Cb[cb]])

void k_PCD_LUT_TableInit()
{
  int    i;
  /* Initialize Lookup table - since it is faster than calculation */
  for (i = 0; i < 256; i++)
    {
      IPL_PCD_LUT_Gray[i]   =  i * IPL_PCD_LUN_MUL >> 8;                  /*    0 -  358 */
      IPL_PCD_LUT_Cr[i]     = (i * IPL_PCD_CR_MUL     - IPL_PCD_CR_SUB  ) >> 8;   /* -274 -  236 */
      IPL_PCD_LUT_Cb[i]     = (i * IPL_PCD_CB_MUL     - IPL_PCD_CB_SUB  ) >> 8;   /* -312 -  198 */
      IPL_PCD_LUT_Green1[i] = (i * IPL_PCD_GREEN1_MUL + IPL_PCD_GREEN1_ADD) >> 8; /*  137 - -118 */
      IPL_PCD_LUT_Green2[i] = (i * IPL_PCD_GREEN2_MUL + IPL_PCD_GREEN2_ADD) >> 8; /*   52 -  -33 */
    }
  for (i = 0; i < IPL_PCD_RANGE; i++)
    IPL_PCD_LUT_Range[i] = 0;
  for (; i < IPL_PCD_RANGE + 256; i++)
    IPL_PCD_LUT_Range[i] = i - IPL_PCD_RANGE;
  for (; i < 2 * IPL_PCD_RANGE + 256; i++)
    IPL_PCD_LUT_Range[i] = 255;
}



bool k_LoadPCD(FILE* fp,TImage* pInfo)
{
  /*bool VerticalOrientation; */
int pcd_img_start[]  = {0 /* Dummy */ , 8192, 47104, 196608};
int pcd_def_width[]  = {0 /* Dummy */ , 192,    384,    768, 1536, 3072, 6144};
int pcd_def_height[] = {0 /* Dummy */ , 128,    256,    512, 1024, 2048, 4096};

  INT32 fileSize;
  int x,y,yy,p,step,maxy;
  /*UINT8 *data;*/    /* for use in resolution 4 & 5 */ 
  UINT8 *buffer;    /* Contains the entire PCD file */
  UINT8 *Y0buf;     /* Pointer to first Luminans in line */
  UINT8 *Y1buf;     /* Pointer to second Luminans in line */
  UINT8 *Crbuf;     /* Pointer to Red Chrominans in line */
  UINT8 *Cbbuf;     /* Pointer to Blue Chrominans in line */
  UINT8 resolution; /* PCD Resolution */

  /* Get the size of the file */
  fseek(fp, 0L, SEEK_END);
  fileSize = ftell(fp);
  /* Initialize file buffer */
  buffer = (UINT8 *)malloc(fileSize*sizeof(UINT8));
  if (buffer==NULL)
    {
      k_ShowMessageSimple(IPL_ERROR, "k_LoadPCD() malloc error");
      fclose(fp);
      return false;
    }

  /* Buffer OK! Go to start of file */
  fseek(fp, 0L, SEEK_SET);

  /* Read only what we need for the moment - change when doing resolution 4 & 5 */
  if(fileSize>786432)
    fileSize = 1024*1024;

  /* Read the whole file into the buffer */
  fread(buffer,sizeof(UINT8),fileSize,fp);

  /* Sanity check for file Size */
  if (fileSize < 49152)
    {
      k_ShowMessageSimple(IPL_ERROR, "k_LoadPCD() probably not a PhotoCD image (too small)");
      fclose(fp);
      free(buffer);
      return false;
    }
  else if(fileSize == 49152) /* 192*128*2 */
    {
      resolution = 1;
    }
  else if(fileSize == 196608) /* 384*256*2 */
    {
      resolution = 2;
    }
  else /* if(fileSize == 786432)*/ /* 768*512*2 */
    {
      resolution = 3; /* Choose 3 for now! */
    }
#if 0
  else
    {
      resolution = 5; /* 4 & 5 isn't implemented yet! */
    }
#endif

  pInfo->Width  = pcd_def_width[resolution];
  pInfo->Height = pcd_def_height[resolution];

  /* VerticalOrientation = (buffer[72]  & 3); */
  pInfo->Bits = 24;
  pInfo->ByteWidth = (UINT16) k_GetSizeScanLine(pInfo->Width,pInfo->Bits);
  pInfo->TotalSize = pInfo->ByteWidth*pInfo->Height; /* nr. of bytes */
  pInfo->BorderSize = 0;           /* always 0 when loading an image */
  pInfo->pPixelStream=(UINT8 *)malloc(pInfo->TotalSize);

  /* Initialize the LUT table */
  k_PCD_LUT_TableInit();

 
  if(resolution<4)
    {
      /* Set pointers to data in the first line */
      Y0buf = buffer + pcd_img_start[resolution];      /*  0  */
      Y1buf = Y0buf  + pcd_def_width[resolution];      /* 1/3 */
      Cbbuf = Y1buf  + pcd_def_width[resolution];;     /* 2/3 */
      Crbuf = Cbbuf  + (pcd_def_width[resolution]>>1); /* 5/6 */

      step = pInfo->ByteWidth; /* pcd_def_width[resolution]*3; */
      maxy = pcd_def_width[resolution]>>1;
      p=pInfo->TotalSize-step;

      /* Do 2 lines at a time */
      for (x = 0; x <  (pcd_def_height[resolution]>>1); x++)
	{
	  for (y = 0 , yy=0; y < maxy; y++)
	    {
	      pInfo->pPixelStream[p  ]      = IPL_PCD_GET_BLUE(Cbbuf[y],Y0buf[yy]);
	      pInfo->pPixelStream[p+1]      = IPL_PCD_GET_GREEN(Crbuf[y],Cbbuf[y],Y0buf[yy]);
	      pInfo->pPixelStream[p+2]      = IPL_PCD_GET_RED(Crbuf[y],Y0buf[yy]);
	      
	      pInfo->pPixelStream[p  -step] = IPL_PCD_GET_BLUE(Cbbuf[y],Y1buf[yy]);
	      pInfo->pPixelStream[p+1-step] = IPL_PCD_GET_GREEN(Crbuf[y],Cbbuf[y],Y1buf[yy]);
	      pInfo->pPixelStream[p+2-step] = IPL_PCD_GET_RED(Crbuf[y],Y1buf[yy]);
	      
	      p+=3;
	      yy++;
	      
	      /* Notice! Exactly the same as above - Loop unrolling for big caches
		 and frees a register on CISC because no extra for() loop is needed */ 
	      pInfo->pPixelStream[p  ]      = IPL_PCD_GET_BLUE(Cbbuf[y],Y0buf[yy]);
	      pInfo->pPixelStream[p+1]      = IPL_PCD_GET_GREEN(Crbuf[y],Cbbuf[y],Y0buf[yy]);
	      pInfo->pPixelStream[p+2]      = IPL_PCD_GET_RED(Crbuf[y],Y0buf[yy]);

	      pInfo->pPixelStream[p  -step] = IPL_PCD_GET_BLUE(Cbbuf[y],Y1buf[yy]);
	      pInfo->pPixelStream[p+1-step] = IPL_PCD_GET_GREEN(Crbuf[y],Cbbuf[y],Y1buf[yy]);
	      pInfo->pPixelStream[p+2-step] = IPL_PCD_GET_RED(Crbuf[y],Y1buf[yy]);
	      
	      p+=3;
	      yy++;
	    } /* for Width */
	  
	  /* Incremet all pointers to point to next line */
	  p     -= 3*step; /* p is incremented by step in the loop. */
	  Y0buf += step;
	  Y1buf += step;
	  Crbuf += step;
	  Cbbuf += step;
	  
	} /* for Height */
    }
#if 0 /* resolution 4 and 5 not implemented! */
	 /* This is done by upsampling from resolution 3 by a factor 4
	     and then adding the huffmann encoded data ?:/ */
  else
    {
      /* high resolution, have to malloc memory */
      data = (UINT8 *)malloc(pInfo->Height*pInfo->Width * 3 / 2);
      if (data == NULL)
	{
	  k_ShowMessageSimple(IPL_ERROR, "k_LoadPCD() malloc error");
          free(buffer);
	  free(pInfo->pPixelStream);
	  return false;
	}
      else
	{
	  free(data);
	}
    }
#endif

        free(buffer);
      return true;
    }


#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
