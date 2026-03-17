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

#include "i_kernel_functions.h"
#include "../kernel_error.h"
#include "../general_functions/ipl_general_functions_c_only.h"
#include "../kernel_pixelgroup.h"
#include "../kernel_history.h"
#include "../algorithms/kernel_graphics.h"
#include <string.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/************************************************/
/********        Private functions       ********/
/************************************************/

void kI_SetImageMatrix(TIntImage* pInfo);
/* returns the size in bytes of a scanline, no. zero padding in a float image */
UINT32 kI_GetSizeScanLine(UINT32 Width);
bool kI_LoadInt(FILE* fp,TIntImage* pInfo);
bool kI_SaveInt(FILE* fp,TIntImage* pInfo);

/************************************************/
/********        Public functions        ********/
/************************************************/

void kI_EmptyImage(TIntImage* pInfo)
{
	k_EmptyFileInfo(&(pInfo->FileInfo));
	k_EmptyText(&(pInfo->History));
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
	pInfo->BorderSize=0;
	pInfo->ByteWidth=0;
	pInfo->TotalSize=0;
	pInfo->Origo.x=0;
	pInfo->Origo.y=0;
	pInfo->Origin=EMPTY;
}

void kI_InitImage(TIntImage* pInfo)
{
	k_InitFileInfo(&(pInfo->FileInfo));
	k_InitText(&(pInfo->History));
	pInfo->pPixelStream=NULL;
	pInfo->ppMatrix=NULL;
	pInfo->ppAllScanLines=NULL;
	pInfo->Origin=EMPTY;
	pInfo->Origo.x=0; /* default origo (0,0) */
	pInfo->Origo.y=0;
}

bool kI_AllocImage(UINT32 Width, UINT32 Height,TIntImage* pInfo)
{
	if(pInfo!=NULL)
	{
		/* Allocate memory if necessary */
		if ((pInfo->Width!=Width) || (pInfo->Height!=Height) || 
			(pInfo->BorderSize!=0))
		{
			/* start with emptying the old image data (if any!) */
			kI_EmptyImage(pInfo);
			/* set values in the image info structure */
			pInfo->Width=Width;
			pInfo->Height=Height;
			pInfo->BorderSize=0; /* always 0 for a new image */
			/* calculate size and width (incl zero-padding) of image */
			pInfo->ByteWidth=(UINT16) kI_GetSizeScanLine(Width);
			pInfo->TotalSize = pInfo->ByteWidth * Height; /* no compression */
			/* Allocate memory for image */
			/*pInfo->pPixelStream=(INT16*) calloc(sizeof(INT16),pInfo->TotalSize*sizeof(INT16));*/
			pInfo->pPixelStream=(INT16*) calloc(sizeof(INT16),pInfo->TotalSize/sizeof(INT16));
			kI_SetImageMatrix(pInfo);
		}
		else
		{
			/* if new allocation not needed, we only reset a few things. */
			k_EmptyFileInfo(&(pInfo->FileInfo));
			k_EmptyText(&(pInfo->History));
			kI_FlushImage(0,pInfo); /* reset existing memory to 0! */
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
		k_ShowMessageSimple(IPL_ERROR, "kI_AllocImage() pInfo is a NULL pointer");
		return false;
	}
}

bool kI_AllocImageFast(UINT32 Width, UINT32 Height,TIntImage* pInfo)
{
	if(pInfo!=NULL)
	{
		/* Allocate memory if necessary */
		if ((pInfo->Width!=Width) || (pInfo->Height!=Height) || 
			(pInfo->BorderSize!=0))
		{
			/* start with emptying the old image data (if any!) */
			kI_EmptyImage(pInfo);
			/* set values in the image info structure */
			pInfo->Width=Width;
			pInfo->Height=Height;
			pInfo->BorderSize=0; /* always 0 for a new image */
			/* calculate size and width (incl zero-padding) of image */
			pInfo->ByteWidth=(UINT16) kI_GetSizeScanLine(Width);
			pInfo->TotalSize = pInfo->ByteWidth * Height; /* no compression */
			/* Allocate memory for image */
			pInfo->pPixelStream=(INT16*) malloc(pInfo->TotalSize);
			kI_SetImageMatrix(pInfo);
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
		k_ShowMessageSimple(IPL_ERROR, "kI_AllocImageFast() pInfo is a NULL pointer");
		return false;
	}
}

bool kI_FlushImage(INT16 Color,TIntImage* pDest)
{
	int y,x;
	if (pDest==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "kI_FlushImage() pDest is a NULL pointer");
		return false;
	}
	else if (pDest->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "kI_FlushImage() Image is empty - doing nothing");
		return false;
	}
	else
	{
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skI_FlushImage(Color=%d)",
				ipl_HistoryIndent, Color);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<(int)pDest->Height;y++)
			for (x=0;x<(int)pDest->Width;x++)
				kI_SetPixelMacro(x,y,Color,*pDest);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool kI_Load(const char* FileName, TIntImage* pInfo)
{
	bool returnValue;
	FILE* fp;
	char* Path=NULL,*FileNameExt=NULL,*Ext=NULL;
	if (!k_SplitFileName(FileName, &Path,&FileNameExt,&Ext))
	{
		k_ShowMessageSimple(IPL_ERROR, "kI_Load() Filename not valid");
		return false;
	}
	else if (strcmp(Ext,"")==0)
	{
		k_ShowMessageSimple(IPL_ERROR, "kI_Load() No extension found");
		returnValue=false;
	}
	else
	{
		returnValue=true;
		k_StringToUpper(Ext);
		fp=fopen(FileName,"rb"); /* open file */
		if (fp==NULL)
		{
			k_ShowMessageSimple(IPL_ERROR, "kI_Load() Could not open file");
			returnValue=false;
		}
		else
		{
			kI_EmptyImage(pInfo);
			/* read from file */
			returnValue=kI_LoadInt(fp,pInfo);
		}

		if (returnValue==true)
		{
			pInfo->Origin=DISC;
			/* set file and path name */
			k_SetFileAndPath(FileName,&pInfo->FileInfo);
			/* update file information in the history */
			k_SetFileInfoFromPathFileText(&(pInfo->History),pInfo->FileInfo.PathAndFileName);
#ifdef IPL_ADD_HISTORY
			/* add comment to history */
			{
				k_PrintfAppendTextIPL(
					&(pInfo->History),
					"%skI_Load(%s)",
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

bool kI_Save(const char* FileName, TIntImage* pInfo)
{
	bool returnValue;
	FILE* fp;
	char* Path=NULL,*FileNameExt=NULL,*Ext=NULL;
	if (!k_SplitFileName(FileName, &Path,&FileNameExt,&Ext))
	{
		k_ShowMessageSimple(IPL_ERROR, "kI_Save() Filename not valid");
		return false;
	}
	else if (strcmp(Ext,"")==0)
	{
		k_ShowMessageSimple(IPL_ERROR, "kI_Save() No extension found");
		returnValue=false;
	}
	else
	{
		returnValue=true;
		k_StringToUpper(Ext);
		fp=fopen(FileName,"wb"); /* open file */
		if (fp==NULL)
		{
			k_ShowMessageSimple(IPL_ERROR, "kI_Save() Could not save file");
			returnValue=false;
		}
		else if (pInfo->Origin==EMPTY)
		{
			k_ShowMessageSimple(IPL_ERROR, "kI_Save() No image - cannot save");
			returnValue=false;
		}
		else
		{
			/* Save image */
#ifdef IPL_ADD_HISTORY
			/* add comment to history */
			{
				k_PrintfAppendTextIPL(
					&(pInfo->History),
					"%skI_Save(%s)",
					ipl_HistoryIndent, FileName);
			}
#endif
			returnValue=kI_SaveInt(fp,pInfo);
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
		}
		else
		{
#ifdef IPL_ADD_HISTORY
			/* remove last line added to history */
			k_RemoveLastLineText(&(pInfo->History));
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

bool kI_SetBorder(UINT16 Size, INT16 Color, TIntImage* pInfo)
{
	kI_SetBorderSize(Size,pInfo);
	kI_SetBorderColor(Color,pInfo);
	return true;
}

bool kI_SetBorderSize(UINT16 Size, TIntImage* pInfo)
{
	UINT32 realX,realY,offset;
	UINT32 UINT32Value; /* temporary variable */
	INT16* pNewImage; /* variable pointing to new allocated image area */
	UINT32 i;
	UINT32 SmallestByteWidth;

	realX=pInfo->Width+2*Size; /* width of image + border */
	realY=pInfo->Height+2*Size; /* height of image + border */
	/* calculate size and width (incl zero-padding) of image + border */
	UINT32Value=kI_GetSizeScanLine(realX);
	/* Only copy memory area for the shortest width of the destination and source image */
	SmallestByteWidth = UINT32Value < (UINT32)pInfo->ByteWidth ? UINT32Value:pInfo->ByteWidth;
	pInfo->BorderSize = Size;
	pInfo->ByteWidth=(UINT16) UINT32Value;
	pInfo->TotalSize=(UINT16) UINT32Value*(realY);
	/* Allocate memory for image */
	pNewImage=(INT16*) malloc(pInfo->TotalSize);
	offset=pInfo->Height-1+Size;
	UINT32Value /= sizeof(INT16);
	for(i=Size;i<Size+pInfo->Height;i++)
		memcpy(pNewImage+Size+i*UINT32Value,(pInfo->ppMatrix)[offset-i],SmallestByteWidth);

	free(pInfo->pPixelStream); /* release old image area */
	pInfo->pPixelStream=pNewImage;
	kI_SetImageMatrix(pInfo);
	return true;
}

bool kI_SetBorderColor(INT16 Color, TIntImage* pInfo)
{
	UINT16 borderSize=pInfo->BorderSize;
	int i,j;
	/* Set upper border */
	for(i=-borderSize;i<0;i++)
	{
		for(j=-borderSize; j<(int)pInfo->Width+borderSize; j++)
		{
			kI_SetPixelMacro(j,i,Color,(*pInfo));
		}
	}
	for(i=0;i<(int)pInfo->Height;i++)
	{
		for(j=0;j<borderSize;j++)
		{
			/* Set left and right border */
			kI_SetPixelMacro(j-borderSize,i,Color,(*pInfo));
			kI_SetPixelMacro(pInfo->Width+j,i,Color,(*pInfo));
		}
	}
	/* Set lower border */
	for(i=pInfo->Height;i<(int)pInfo->Height+borderSize;i++)
	{
		for(j=-borderSize; j<(int)pInfo->Width+borderSize; j++)
		{
			kI_SetPixelMacro(j,i,Color,(*pInfo));
		}
	}
	return true;
}

bool kI_SetPeriodicBorder(unsigned short BorderSize,TIntImage* pInfo)
{
	if (pInfo->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "kI_SetPeriodicBorder() Image is empty");
		return false;
	}
	else if (((unsigned int)BorderSize>pInfo->Width) || ((unsigned int)BorderSize>pInfo->Height))
	{
		k_ShowMessageSimple(IPL_ERROR, "kI_SetPeriodicBorder() Border too large");
		return false;
	}
	else
	{
		int x,y;
		unsigned int w,h;
		w=pInfo->Width;
		h=pInfo->Height;
		if (pInfo->BorderSize!=BorderSize) /* only set border if necessary */
			kI_SetBorderSize(BorderSize,pInfo);
		/* setting upper and lover horisontal border values */
		for(y=1;y<=BorderSize;y++)
		{
			for(x=0;x<(int)pInfo->Width;x++)
			{
				kI_SetPixelMacro(x,-y,kI_GetPixelMacro(x,h-y,*pInfo),*pInfo); /* setting upper border */
				kI_SetPixelMacro(x,h+y-1,kI_GetPixelMacro(x,y-1,*pInfo),*pInfo); /* setting lower border */
			}
		}
		/* setting left and right vertical border values */
		for(x=1;x<=BorderSize;x++)
		{
			for(y=0;y<(int)pInfo->Height;y++)
			{
				kI_SetPixelMacro(-x,y,kI_GetPixelMacro(w-x,y,*pInfo),*pInfo); /* setting left border */
				kI_SetPixelMacro(w+x-1,y,kI_GetPixelMacro(x-1,y,*pInfo),*pInfo); /* setting right border */
			}
		}
		/* setting the four corners */
		for(y=1;y<=BorderSize;y++)
		{
			for(x=1;x<=BorderSize;x++)
			{
				/* setting top left corner */
				kI_SetPixelMacro(-x,-y,kI_GetPixelMacro(w-x,h-y,*pInfo),*pInfo);
				/* setting top right corner */
				kI_SetPixelMacro(w+x-1,-y,kI_GetPixelMacro(-x,h+y-1,*pInfo),*pInfo);
				/* setting bottom left corner */
				kI_SetPixelMacro(-x,h+y-1,kI_GetPixelMacro(w-x,y-1,*pInfo),*pInfo);
				/* setting bottom right corner */
				kI_SetPixelMacro(w+x-1,h+y-1,kI_GetPixelMacro(x-1,y-1,*pInfo),*pInfo);
			}
		}
		return true;
	}
}

bool kI_CopyImage(TIntImage* pDest,const TIntImage* pSource)
{
	if (pDest==pSource)
	{
		/* cannot copy to itself! */
		k_ShowMessageSimple(IPL_ERROR, "kI_CopyImage() Source and destination image"
			"the same - doing nothing");
		return false;
	}
	/* Allocate memory if necessary */
	if ((pDest->TotalSize!=pSource->TotalSize) || (pDest->BorderSize!=pSource->BorderSize))
	{
		kI_EmptyImage(pDest);
		pDest->TotalSize = pSource->TotalSize;
		pDest->BorderSize=pSource->BorderSize;
		pDest->pPixelStream=(INT16*) malloc(pDest->TotalSize);
	}
	if ((pDest->Width!=pSource->Width) || (pDest->Height!=pSource->Height))
	{
		pDest->Width=pSource->Width; /* need to set width and height for k_SetImageMatrix! */
		pDest->Height=pSource->Height;
		kI_SetImageMatrix(pDest);
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
				"%skI_CopyImage() Source image file info: %s",
				ipl_HistoryIndent, pSource->FileInfo.PathAndFileName);
			k_IplStartHistoryMacro();
		}
#endif
		/* copy file information */
		k_CopyFileInfo(&(pDest->FileInfo),&(pSource->FileInfo));
		/* copy image data */
		memcpy(pDest->pPixelStream,pSource->pPixelStream,pDest->TotalSize);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
	}
	return true;
}

bool kI_CopySubImage(int xmin,int ymin,int xmax,int ymax,
				  TIntImage* pDest,const TIntImage* pSource)
{
	TIntImage* pTarget=pDest;
	bool CopyToSource=false;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "kI_CopySubImage() Source image is empty - doing nothing");
		return false;
	}
	/* Check if the source and destination is the same image */
	if (pDest==pSource)
	{
		pTarget=(TIntImage*) malloc(sizeof(TIntImage));
		kI_InitImage(pTarget);
		CopyToSource=true;
	}
	/* Not optimised regarding speed! */
	if ((xmin<xmax) && (ymin<ymax) && 
		(xmin>=kI_GetMinX(pSource)) && (ymin>=kI_GetMinY(pSource)) &&
		(xmax<=kI_GetMaxX(pSource)) && (ymax<=kI_GetMaxY(pSource)))
	{
		UINT16 xdim,ydim;
		int i;
		int xminNoOrigo=xmin+pSource->Origo.x;
		int yminNoOrigo=ymin+pSource->Origo.y;
		int ymaxNoOrigo=ymax+pSource->Origo.y;
		xdim=(UINT16)(xmax-xmin);
		ydim=(UINT16)(ymax-ymin);
		if (pTarget->TotalSize!=(kI_GetSizeScanLine(xdim)*ydim))
		{
			kI_EmptyImage(pTarget);
			/* prepare destination image */
			kI_AllocImage(xdim,ydim,pTarget);
		}
		k_CopyFileInfo(&(pTarget->FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pTarget->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pTarget->History),
				"%skI_CopySubImage(xmin=%d,ymin=%d,xmax=%d,ymax=%d) "
				"Source image file info: %s",
				ipl_HistoryIndent, xmin, ymin, xmax, ymax,
				pSource->FileInfo.PathAndFileName);
			k_IplStartHistoryMacro();
		}
#endif
		/* copy image pixels */
		for(i=yminNoOrigo; i<ymaxNoOrigo; i++)
			memcpy(pTarget->ppAllScanLines[i-yminNoOrigo],pSource->ppAllScanLines[i]+sizeof(INT16)*xminNoOrigo,xdim*sizeof(INT16));

		pTarget->Origin=RAM;
		/* Set origo */
		pTarget->Origo=pSource->Origo;
        if (CopyToSource==true)
        {
	        kI_CopyImage(pDest,pTarget);
			kI_EmptyImage(pTarget);
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
		k_ShowMessageSimple(IPL_ERROR, "kI_CopySubImage() Illegal clipping rectangel");
		return false;
	}
}

void kI_Rotate90(int Steps,TIntImage* pSource)
{
	TIntImage NewImage;
	int x,y;
	int MaxYIndex=pSource->Height-1;
	int MaxXIndex=pSource->Width-1;
	int ActSteps=(Steps % 4);
	if (ActSteps<0)
		ActSteps=4+ActSteps;

	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "kI_Rotate90() Image is empty - doing nothing");
		return;
	}
	if (ActSteps==0)
		return;

	kI_InitImage(&NewImage);
	/* prepare destination image, following lines faster than calling kI_CopyImage() */
	if (ActSteps==2)
		kI_AllocImage(pSource->Width,pSource->Height,&NewImage);
	else
		kI_AllocImage(pSource->Height,pSource->Width,&NewImage);
	k_CopyFileInfo(&(NewImage.FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
	/* copy history before adding comment */
	k_CopyText(&(NewImage.History),&(pSource->History));
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(NewImage.History),
			"%skI_Rotate90(Steps=%d)",
			ipl_HistoryIndent, Steps);
		k_IplStartHistoryMacro();
	}
#endif

	/* rotate image */
	if (ActSteps==1)
	{
		for(y=0; y<(int)pSource->Height; y++)
			for(x=0; x<(int)pSource->Width; x++)
				kI_SetPixelMacro(-y+MaxYIndex,x,kI_GetPixelMacro(x,y,*pSource),NewImage);
	}
	else if (ActSteps==2)
	{
		for(y=0; y<(int)pSource->Height; y++)
			for(x=0; x<(int)pSource->Width; x++)
				kI_SetPixelMacro(x,MaxYIndex-y,kI_GetPixelMacro(MaxXIndex-x,y,*pSource),NewImage);
	}
	else if (ActSteps==3)
	{
		for(y=0; y<(int)pSource->Height; y++)
			for(x=0; x<(int)pSource->Width; x++)
				kI_SetPixelMacro(y,MaxXIndex-x,kI_GetPixelMacro(x,y,*pSource),NewImage);
	}
	kI_CopyImage(pSource,&NewImage);
	kI_EmptyImage(&NewImage);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

void kI_FlipHorisontal(TIntImage* pSource)
{
	TIntImage Buf;
	int y;
	int Half=pSource->Height/2;
	int Ext=pSource->Height%2;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "kI_FlipHorisontal() Image is empty - doing nothing");
		return;
	}

	kI_InitImage(&Buf);
	/* flip image */
	/* prepare destination image */
	kI_CopySubImage(kI_GetMinX(pSource),kI_GetMinY(pSource),kI_GetMaxX(pSource),Half-pSource->Origo.y,&Buf,pSource);
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(pSource->History),
			"%skI_FlipHorisontal()",
			ipl_HistoryIndent);
		k_IplStartHistoryMacro();
	}
#endif
	for(y=0;y<Half;y++)
		memcpy(pSource->ppAllScanLines[y],pSource->ppAllScanLines[pSource->Height-y-1],pSource->ByteWidth);
	for(y=Half+Ext; y<(int)pSource->Height; y++)
		memcpy(pSource->ppAllScanLines[y],Buf.ppAllScanLines[Buf.Height-(y-Half-Ext)-1],pSource->ByteWidth);
	kI_EmptyImage(&Buf);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

void kI_FlipVertical(TIntImage* pSource)
{
	TIntImage NewImage;
	int x,y;
	int MaxXIndex=pSource->Width-1;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "kI_FlipVertical() Image is empty - doing nothing");
		return;
	}

	kI_InitImage(&NewImage);
	/* flip image */
	/* prepare destination image, following lines faster than calling k_CopyImage() */
	kI_AllocImage(pSource->Width,pSource->Height,&NewImage);
	k_CopyFileInfo(&(NewImage.FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
	/* copy history before adding comment */
	k_CopyText(&(NewImage.History),&(pSource->History));
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(NewImage.History),
			"%skI_FlipVertical()",
			ipl_HistoryIndent);
		k_IplStartHistoryMacro();
	}
#endif
	for(y=0; y<(int)pSource->Height; y++)
		for(x=0; x<(int)pSource->Width; x++)
			kI_SetPixelMacro(MaxXIndex-x,y,kI_GetPixelMacro(x,y,*pSource),NewImage);

	kI_CopyImage(pSource,&NewImage);
	kI_EmptyImage(&NewImage);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

int kI_GetMinX(const TIntImage* Img)
{
	return -Img->Origo.x;
}

int kI_GetMinY(const TIntImage* Img)
{
	return -Img->Origo.y;
}

int kI_GetMaxX(const TIntImage* Img)
{
	return Img->Width-Img->Origo.x;
}

int kI_GetMaxY(const TIntImage* Img)
{
	return Img->Height-Img->Origo.y;
}

int kI_InRange(int x, int y, int Delta, const TIntImage* Img)
{
	if ((x>=(kI_GetMinX(Img)-Delta)) && (x<(kI_GetMaxX(Img)+Delta)) &&
		(y>=(kI_GetMinY(Img)-Delta)) && (y<(kI_GetMaxY(Img)+Delta)))
		return true;
	else
		return false;
}

/****************************************/
/*        Arithmetic functions          */
/****************************************/

bool kI_ApplyBias(INT16 Bias, TIntImage *pImg)
{ 
	if (pImg->Origin!=EMPTY)
	{
		unsigned int x,y;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%skI_ApplyBias() Bias=%g",
				ipl_HistoryIndent, Bias);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<pImg->Height;y++)
		{
			for (x=0;x<pImg->Width;x++) 
			{
				pImg->ppMatrix[y][x]+=Bias;
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
		k_ShowMessageSimple(IPL_ERROR, "kI_ApplyBias: Source image empty");
		return false;
	}
}

bool kI_ScaleIntensity(float Prefactor, TIntImage *pImg)
{ 
	if (pImg->Origin!=EMPTY)
	{
		unsigned int x,y;
		double NewValue;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%skI_ScaleIntensity() Prefactor=%g",
				ipl_HistoryIndent, Prefactor);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<pImg->Height;y++)
		{
			for (x=0;x<pImg->Width;x++) 
			{
				NewValue=pImg->ppMatrix[y][x]*Prefactor;
				pImg->ppMatrix[y][x]=k_Round(NewValue);
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
		k_ShowMessageSimple(IPL_ERROR, "kI_ScaleIntensity: Source image empty");
		return false;
	}
}

bool kI_Add(TIntImage *pImg,TIntImage *pAddend)
{
	if ((pImg->Origin==EMPTY) || (pAddend->Origin==EMPTY)
		|| (pImg->Width!=pAddend->Width) || (pImg->Height!=pAddend->Height))
	{
		k_ShowMessageSimple(IPL_ERROR, "kI_Add(): Images are empty or of different size");
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
				"%skI_Add() Adding a TIntImage",
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

bool kI_Subtract(TIntImage *pImg,TIntImage *pSubtrahend)
{
	if ((pImg->Origin==EMPTY) || (pSubtrahend->Origin==EMPTY)
		|| (pImg->Width!=pSubtrahend->Width) || (pImg->Height!=pSubtrahend->Height))
	{
		k_ShowMessageSimple(IPL_ERROR, "kI_Subtract(): Images are empty or of different size");
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
				"%skI_Subtract() subtracting a TIntImage",
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

bool kI_Multiply(TIntImage *pImg,TIntImage *pFactor)
{
	if ((pImg->Origin==EMPTY) || (pFactor->Origin==EMPTY)
		|| (pImg->Width!=pFactor->Width) || (pImg->Height!=pFactor->Height))
	{
		k_ShowMessageSimple(IPL_ERROR, "kI_Multiply(): Images are empty or of different size");
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
				"%skI_Multiply() multiplying a TIntImage",
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

bool kI_CopyDisplace(int dx,int dy, TIntImage *pImg)
{
	if (pImg->Origin!=EMPTY)
	{
		int x,y,w,h,xd,yd;
		TIntImage Temp;
		w=pImg->Width;
		h=pImg->Height;
		kI_InitImage(&Temp);
		kI_AllocImage(w,h,&Temp);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(Temp.History),&(pImg->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(Temp.History),
				"%skI_CopyDisplace() dx=%d dy=%d",
				ipl_HistoryIndent, dx, dy);
			k_IplStartHistoryMacro();
		}
#endif
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
		kI_CopyImage(pImg,&Temp);
		kI_EmptyImage(&Temp);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	} 
	else
	{
		k_ShowMessageSimple(IPL_ERROR, "kI_CopyDisplace(): Image is empty");
		return false;
	}
}

bool kI_CutOrZeroPad(TIntImage*pDest,TIntImage *pSource)
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
				"%skI_CutOrZeroPad()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<hd;y++)
		{
			for (x=0;x<wd;x++)
			{ 
				if ((x<ws) && (y<hs))
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
		char* msg;
		
		if (pSource==pDest)
			msg = "kI_CutOrZeroPad(): Cannot use same image as dest/source";
 		else if ((ws<=0) || (hs<=0) || (wd<=0) || (hd<=0))
			msg = "kI_CutOrZeroPad(): One or both images have no pixels";
		else
			msg = "kI_CutOrZeroPad(): One or both images are empty";
		k_ShowMessageSimple(IPL_ERROR, msg);
		return false;
	}
}

bool kI_CopyFromByte(TIntImage *pDest,TImage *pSource)
{
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "kI_CopyFromByte(): Image is empty");
		return false;
	}
	else
	{
	    int x,y,w,h;
        w=pSource->Width;
        h=pSource->Height;
        kI_AllocImage(w,h,pDest);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skI_CopyFromByte()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
        for (y=0;y<h;y++)
		{
			for (x=0;x<w;x++)
			{
				pDest->ppMatrix[y][x]=pSource->ppMatrix[y][x];
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
        return true;
	}
}

bool kI_CopyFromFloat(TIntImage *pDest,TFloatImage *pSource)
{
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "kI_CopyFromFloat(): Image is empty");
		return false;
	}
	else
	{
		int x,y,w,h;
        w=pSource->Width;
        h=pSource->Height;
        kI_AllocImage(w,h,pDest);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skI_CopyFromFloat()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
        for (y=0;y<h;y++)
		{
			for (x=0;x<w;x++)
			{
				pDest->ppMatrix[y][x]=(INT16)(pSource->ppMatrix[y][x]);
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
        return true;
	}
}

bool kI_CopyFromComplex(COMPLEXCHOISE mode, TIntImage *pDest,TComplexImage *pSource)
{
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "kI_CopyFromComplex(): Image is empty");
		return false;
	}
	else
	{
		int x,y,w,h;
		double buf;
        w=pSource->Width;
        h=pSource->Height;
        kI_AllocImage(w,h,pDest);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skI_CopyFromComplex()",
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
					pDest->ppMatrix[y][x]=(INT16)(pSource->ppMatrixRe[y][x]);
				}
			}
        }
        if (mode==COMPLEX_IMAGINARY)
        {
			for (y=0;y<h;y++)
			{
				for (x=0;x<w;x++)
				{
					pDest->ppMatrix[y][x]=(INT16)(pSource->ppMatrixIm[y][x]);
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
					pDest->ppMatrix[y][x]=(INT16)buf;
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

void kI_PrintImageInfo(const TIntImage* pInfo)
{
	char* Name;
	k_GetOriginString(pInfo->Origin,&Name);
	printf("**************** IntImage info ********************\n");
	printf(" Width=%ld Height=%ld ByteWidth=%d SizeImage=%ld\n",
		pInfo->Width, pInfo->Height, pInfo->ByteWidth, pInfo->TotalSize);
	printf(" BorderSize=%d Origo=(%d,%d) Origin=%s\n",
		pInfo->BorderSize, pInfo->Origo.x, pInfo->Origo.y,Name);
	k_PrintFileInfo(&pInfo->FileInfo);
	free(Name);
}

bool kI_DrawLine(T2DInt Start, T2DInt End, INT16 Value, TIntImage *pDest)
{
	TPixelGroup grp;
	if (pDest==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "kI_DrawLine(): Image is empty");
		return false;
	}
	k_InitGroup(&grp);
	k_GraphicsGenerateLine(Start,End,&grp);
	k_AddGroupToIntImage(pDest,&grp,Value);
	return true;
}

bool kI_DrawCircle(T2DInt Center, unsigned int Radius, INT16 Value, TIntImage *pDest)
{
	TPixelGroup grp;
	if (pDest==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "kI_DrawCircle(): Image is empty");
		return false;
	}
	k_InitGroup(&grp);
	k_GraphicsGenerateCircle(Center,Radius,&grp);
	k_AddGroupToIntImage(pDest,&grp,Value);
	return true;
}

/************************************************/
/********        C only functions        ********/
/************************************************/

INT16 kI_GetPixel(int x, int y,const TIntImage* pInfo)
{
	UINT16 border=pInfo->BorderSize;
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if ((x>=-border) && (x<(int) pInfo->Width+border) &&
		(y>=-border) && (y<(int) pInfo->Height+border))
	{
		return kI_GetPixelMacro(x,y,(*pInfo));
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"kI_GetPixel(%d,%d) Outside image and border",
			x, y);
		return 0;
	}
}

INT16 kI_GetPixelFast(int x, int y,const TIntImage* pInfo)
{
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	return kI_GetPixelMacro(x,y,(*pInfo));
}

bool kI_SetPixel(int x, int y, INT16 Color,TIntImage* pInfo)
{
	UINT16 border=pInfo->BorderSize;
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if((x>=-border) && (x<(int) pInfo->Width+border) &&
		(y>=-border) && (y<(int) pInfo->Height+border))
	{
		kI_SetPixelMacro(x,y,Color,(*pInfo));
		return true;
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"kI_SetPixel(%d,%d) Outside image and border",
			x, y);
		return false;
	}
}

bool kI_SetPixelFast(int x, int y, INT16 Color,TIntImage* pInfo)
{
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	kI_SetPixelMacro(x,y,Color,(*pInfo));
	return true;
}

float kI_GetPixelInterpolated(float x, float y, const TIntImage* pInfo)
{
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

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
		return ((kI_GetPixelMacro(xl,yl,(*pInfo))*((float)xh-x) + kI_GetPixelMacro(xh,yl,(*pInfo))*(x-(float)xl))*((float)yh-y) +
		 (kI_GetPixelMacro(xl,yh,(*pInfo))*((float)xh-x) + kI_GetPixelMacro(xh,yh,(*pInfo))*(x-(float)xl))*(y-(float)yl));
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"kI_GetPixelInterpolated(%.2f,%.2f) Needs "
			"to access pixels outside image and border\n",
			x, y);
		return -1;
	}
}

float kI_GetPixelInterpolatedFast(float x, float y, const TIntImage* pInfo)
{
	/* upper left pixel (l ~ low) */
	int xl = (x >=0 ? (int)x : (int)(x-1)); /* always round down */
	int yl = (y >=0 ? (int)y : (int)(y-1)); /* always round down */
	/* lower right pixel (h ~ high) */
	int xh = xl + 1;
	int yh = yl + 1;
	/* do bilinear interpolation */
	return ((kI_GetPixelMacro(xl,yl,(*pInfo))*((float)xh-x) + kI_GetPixelMacro(xh,yl,(*pInfo))*(x-(float)xl))*((float)yh-y) +
	 (kI_GetPixelMacro(xl,yh,(*pInfo))*((float)xh-x) + kI_GetPixelMacro(xh,yh,(*pInfo))*(x-(float)xl))*(y-(float)yl));
}

/************************************************/
/********        Private functions       ********/
/************************************************/

void kI_SetImageMatrix(TIntImage* pInfo)
{
	/* Sets "pInfo->ppMatrix[y]" to point to start of every scan line
	   inside image and set the pInfo->ppMatrix[y][x]
	   call this function after pInfo->BorderSize is set */
	UINT32 y;
	UINT16 border=pInfo->BorderSize;
	UINT32 totalHeight=pInfo->Height+2*border;
	UINT32 realX=pInfo->Width+2*border; /* width of image + border */
	UINT32 UINT32Value; /* temporary variable */
	/* calculate size and width (incl zero-padding) of image + border */
	/*UINT32Value=kI_GetSizeScanLine(realX)*/;
	UINT32Value=(kI_GetSizeScanLine(realX)/sizeof(INT16));
	/*printf("totalHeight=%d\n",totalHeight);*/
	if (pInfo->ppAllScanLines!=NULL)
	{
		free(pInfo->ppAllScanLines);
	}

	(pInfo->ppAllScanLines) = (INT16**) malloc(sizeof(INT16*)*(totalHeight));
	pInfo->ppMatrix = &((pInfo->ppAllScanLines)[border]);

	for(y=0;y<totalHeight;y++)
	{
		(pInfo->ppAllScanLines)[totalHeight-1-y]=(pInfo->pPixelStream+y*UINT32Value+border);
	}
}

UINT32 kI_GetSizeScanLine(UINT32 Width)
{
	/* returns the size of a scanline, no zero padding in a float image */
	return Width*sizeof(INT16);
}

bool kI_LoadInt(FILE* fp,TIntImage* pInfo)
{
	unsigned int Width,Height;
	unsigned int Rows,Cols;
	UINT32 x,y;
	INT16 Value;
	bool WidthHeightRead=false;
	/* when set to true, the first token has been read and following comment lines
	   (starting with '#') will not be added to the history */
	bool TokenRead=false;
	TIplText TempHistory; /* keep history in a tempory variable and copy in the end to pInfo */
	k_InitText(&TempHistory);
	k_SkipWhiteSpace(fp);
	while (fgetc(fp)=='#')
	{
		if ((fgetc(fp)!='#') && (TokenRead==false))
		{
			/* no tokens read yet, add comment line to history */
			int CharsInLine=k_SkipLine(fp);
			if (CharsInLine==-1)
			{
				k_ShowMessageSimple(IPL_ERROR, "kI_LoadFloat() wrong header - "
					"only found a history - no data");
				return false;
			}
			else
			{
				char* pStr=(char*)malloc((CharsInLine+2)*sizeof(char));
				fseek(fp,-(CharsInLine+2),SEEK_CUR); /* rewind file pointer to beginning of the current line */
				fread(pStr,sizeof(char),CharsInLine+1,fp);
				pStr[CharsInLine+1]='\0';
				k_AppendText(&TempHistory,pStr);
				free(pStr);
			}
		}
		else if ((fgetc(fp)!='#') && (TokenRead==true))
		{
			/* at least one token already read, then just skip the rest ot the commentlines */
			k_SkipLine(fp);
		}
		else
		{
			/* read a special comment line */
			char* Token=NULL;
			k_ReadToken(fp,&Token);
			if (Token==NULL)
			{
				k_ShowMessageSimple(IPL_ERROR, "kI_LoadInt() wrong file format, end of file reached");
				return false;
			}
			else if (strcmp(Token,"WidthHeight")==0)
			{
				/* The following two values must be integers */
				if (fscanf(fp,"%d %d",&Width,&Height)!=2)
				{
					k_ShowMessageSimple(IPL_ERROR, "kI_LoadInt() wrong header - "
						"found \"## WidthHeight\" but no width and height values found");
					return false;
				}
				if (WidthHeightRead==true)
				{
					/* test if rows and cols is the same as width and height */
					if ((Width!=Cols) || (Height!=Rows))
					{
						k_ShowMessageStd(
							IPL_POSITION, IPL_ERROR,
							"kI_LoadInt() Width=Cols (%d=%d) and height=Rows (%d,%d) "
							"does not match",
							Width, Cols, Height, Cols, Rows);
						return false;
					}
				}
				else
					WidthHeightRead=true;
			}
			else if (strcmp(Token,"RowsCols")==0)
			{
				if (fscanf(fp,"%d %d",&Rows,&Cols)!=2)
				{
					k_ShowMessageSimple(IPL_ERROR, "kI_LoadInt() wrong header - "
						"found \"## WidthHeight\" but no width and height values found");
					return false;
				}
				if (WidthHeightRead==true)
				{
					/* test if rows and cols is the same as width and height */
					if ((Width!=Cols) || (Height!=Rows))
					{
						k_ShowMessageStd(
							IPL_POSITION, IPL_ERROR,
							"kI_LoadInt() Width=Cols (%d=%d) and height=Rows (%d,%d) "
							"does not match",
							Width, Cols, Height, Cols, Rows);
						return false;
					}
				}
				else
				{
					/* no width and height found - use rows and cols to define size */
					Width=Cols;
					Height=Rows;
					WidthHeightRead=true;
				}
			}
			if (Token!=NULL)
				free(Token);
		}
		k_SkipWhiteSpace(fp);
	}
	fseek(fp,-1,SEEK_CUR); /* reverse fp by 1 */
	/* Now we must be at the beginning of the image data */
	if (WidthHeightRead==false)
	{
		k_ShowMessageSimple(IPL_ERROR, "kI_LoadInt() wrong header - width and height not found");
		return false;
	}
	/* allocate image */
	kI_AllocImage(Width,Height,pInfo);
	/* Read data */
	for(y=0;y<Height;y++)
	{
		for(x=0;x<Width;x++)
		{
			if (k_SkipWhiteSpace(fp)!=-1)
			{
				if (fscanf(fp,"%hd", (short int *)&Value)==1)
				{
					kI_SetPixel(x,y,Value,pInfo);
				}
				else
				{
					k_ShowMessageSimple(IPL_ERROR, "kI_LoadInt() Failed reading image data");
					kI_EmptyImage(pInfo);
					return false;
				}
			}
			else
			{
				k_ShowMessageSimple(IPL_ERROR, "kI_LoadInt() Failed reading image data");
				kI_EmptyImage(pInfo);
				return false;
			}
		}
	}
	/* copy history to pInfo */
	k_CopyText(&(pInfo->History),&TempHistory);
	k_EmptyText(&TempHistory);
	return true;
}

bool kI_SaveInt(FILE* fp,TIntImage* pInfo)
{
	unsigned int x,y;
	char* pHistory=NULL;
	if (pInfo->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "kI_SaveInt() No image to save");
		fclose(fp);
		return false;
	}
	k_ListToCharArrayText(&(pInfo->History),&pHistory);
	fprintf(fp,"%s\n",pHistory);
	if (pHistory!=NULL)
	{
		free(pHistory);
	}
	fprintf(fp,"## WidthHeight %d %d\n", (int)pInfo->Width, (int)pInfo->Height);
	fprintf(fp,"## RowsCols %d %d\n\n", (int)pInfo->Height, (int)pInfo->Width);
	for(y=0;y<pInfo->Height;y++)
	{
		for(x=0;x<pInfo->Width;x++)
		{
			fprintf(fp,"%d\t",kI_GetPixelMacro(x,y,(*pInfo)));
		}
		fprintf(fp,"\n");
	}
	return true;
}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
