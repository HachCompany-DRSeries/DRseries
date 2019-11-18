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

#include "f_kernel_functions.h"
#include "../kernel_string.h"
#include "../kernel_error.h"
#include "../kernel_pixelgroup.h"
#include "../kernel_history.h"
#include "../algorithms/kernel_graphics.h"
#include <string.h>
#include <math.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif
/************************************************/
/********        Private functions       ********/
/************************************************/

void kF_SetImageMatrix(TFloatImage* pInfo);
/* returns the size of a scanline, no. zero padding in a float image */
UINT32 kF_GetSizeScanLine(UINT32 Width);
bool kF_LoadFloat(FILE* fp,TFloatImage* pInfo);
bool kF_SaveFloat(FILE* fp,TFloatImage* pInfo);

/************************************************/
/********        Public functions        ********/
/************************************************/

void kF_EmptyImage(TFloatImage* pInfo)
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

void kF_InitImage(TFloatImage* pInfo)
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

bool kF_AllocImage(UINT32 Width, UINT32 Height,TFloatImage* pInfo)
{
	if(pInfo!=NULL)
	{
		/* Allocate memory if necessary */
		if ((pInfo->Width!=Width) || (pInfo->Height!=Height) || 
			(pInfo->BorderSize!=0))
		{
			/* start with emptying the old image data (if any!) */
			kF_EmptyImage(pInfo);
			/* set values in the image info structure */
			pInfo->Width=Width;
			pInfo->Height=Height;
			pInfo->BorderSize=0; /* always 0 for a new image */
			/* calculate size and width (incl zero-padding) of image */
			pInfo->ByteWidth=(UINT16) kF_GetSizeScanLine(Width);
			pInfo->TotalSize = pInfo->ByteWidth * Height; /* no compression */
			/* Allocate memory for image */
			pInfo->pPixelStream=(FLOAT32*) calloc(sizeof(FLOAT32),pInfo->TotalSize/sizeof(FLOAT32));
			kF_SetImageMatrix(pInfo);
		}
		else
		{
			/* if new allocation not needed, we only reset a few things. */
			k_EmptyFileInfo(&(pInfo->FileInfo));
			k_EmptyText(&(pInfo->History));
			kF_FlushImage(0,pInfo); /* reset existing memory to 0! */
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
		k_ShowMessageSimple(IPL_ERROR, "kF_AllocImage() pInfo is a NULL pointer");
		return false;
	}
}

bool kF_AllocImageFast(UINT32 Width, UINT32 Height,TFloatImage* pInfo)
{
	if(pInfo!=NULL)
	{
		/* Allocate memory if necessary */
		if ((pInfo->Width!=Width) || (pInfo->Height!=Height) || 
			(pInfo->BorderSize!=0))
		{
			/* start with emptying the old image data (if any!) */
			kF_EmptyImage(pInfo);
			/* set values in the image info structure */
			pInfo->Width=Width;
			pInfo->Height=Height;
			pInfo->BorderSize=0; /* always 0 for a new image */
			/* calculate size and width (incl zero-padding) of image */
			pInfo->ByteWidth=(UINT16) kF_GetSizeScanLine(Width);
			pInfo->TotalSize = pInfo->ByteWidth * Height; /* no compression */
			/* Allocate memory for image */
			pInfo->pPixelStream=(FLOAT32*) malloc(pInfo->TotalSize);
			kF_SetImageMatrix(pInfo);
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
		k_ShowMessageSimple(IPL_ERROR, "kF_AllocImageFast() pInfo is a NULL pointer");
		return false;
	}
}

bool kF_FlushImage(FLOAT32 Color,TFloatImage* pDest)
{
	int y,x;
	if (pDest==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "kF_FlushImage() pDest is a NULL pointer");
		return false;
	}
	else if (pDest->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "kF_FlushImage() Image is empty - doing nothing");
		return false;
	}
	else
	{
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skF_FlushImage(Color=%d)",
				ipl_HistoryIndent, Color);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<(int)pDest->Height;y++)
			for (x=0;x<(int)pDest->Width;x++)
				kF_SetPixelMacro(x,y,Color,*pDest);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool kF_Load(const char* FileName, TFloatImage* pInfo)
{
	bool returnValue;
	FILE* fp;
	char* Path=NULL,*FileNameExt=NULL,*Ext=NULL;
	if (!k_SplitFileName(FileName, &Path,&FileNameExt,&Ext))
	{
		k_ShowMessageSimple(IPL_ERROR, "kF_Load() Filename not valid");
		return false;
	}
	else if (strcmp(Ext,"")==0)
	{
		k_ShowMessageSimple(IPL_ERROR, "kF_Load() No extension found");
		returnValue=false;
	}
	else
	{
		returnValue=true;
		k_StringToUpper(Ext);
		fp=fopen(FileName,"rb"); /* open file */
		if (fp==NULL)
		{
			k_ShowMessageSimple(IPL_ERROR, "kF_Load() Could not open file");
			returnValue=false;
		}
		else
		{
			kF_EmptyImage(pInfo);
			/* read from file */
			returnValue=kF_LoadFloat(fp,pInfo);
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
					"%skF_Load(%s)",
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

bool kF_Save(const char* FileName, TFloatImage* pInfo)
{
	bool returnValue;
	FILE* fp;
	char* Path=NULL,*FileNameExt=NULL,*Ext=NULL;
	if (!k_SplitFileName(FileName, &Path,&FileNameExt,&Ext))
	{
		k_ShowMessageSimple(IPL_ERROR, "kF_Save() Filename not valid");
		return false;
	}
	else if (strcmp(Ext,"")==0)
	{
		k_ShowMessageSimple(IPL_ERROR, "kF_Save() No extension found");
		returnValue=false;
	}
	else
	{
		returnValue=true;
		k_StringToUpper(Ext);
		fp=fopen(FileName,"wb"); /* open file */
		if (fp==NULL)
		{
			k_ShowMessageSimple(IPL_ERROR, "kF_Save() Could not save file");
			returnValue=false;
		}
		else if (pInfo->Origin==EMPTY)
		{
			k_ShowMessageSimple(IPL_ERROR, "kF_Save() No image - cannot save");
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
					"%skF_Save(%s)",
					ipl_HistoryIndent, FileName);
			}
#endif
			returnValue=kF_SaveFloat(fp,pInfo);
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

bool kF_SetBorder(UINT16 Size, FLOAT32 Color, TFloatImage* pInfo)
{
	kF_SetBorderSize(Size,pInfo);
	kF_SetBorderColor(Color,pInfo);
	return true;
}

bool kF_SetBorderSize(UINT16 Size, TFloatImage* pInfo)
{
	UINT32 realX,realY,offset;
	UINT32 UINT32Value; /* temporary variable */
	FLOAT32* pNewImage; /* variable pointing to new allocated image area */
	UINT32 i;
	UINT32 SmallestByteWidth;

	realX=pInfo->Width+2*Size; /* width of image + border */
	realY=pInfo->Height+2*Size; /* height of image + border */
	/* calculate size and width (incl zero-padding) of image + border */
	UINT32Value=kF_GetSizeScanLine(realX);
	/* Only copy memory area for the shortest width of the destination and source image */
	SmallestByteWidth = UINT32Value < (UINT32)pInfo->ByteWidth ? UINT32Value:pInfo->ByteWidth;
	pInfo->BorderSize = Size;
	pInfo->ByteWidth=(UINT16) UINT32Value;
	pInfo->TotalSize=(UINT16) UINT32Value*(realY);
	/* Allocate memory for image */
	pNewImage=(FLOAT32*) malloc(pInfo->TotalSize);
	offset=pInfo->Height-1+Size;
	UINT32Value /= sizeof(FLOAT32);
	for(i=Size;i<Size+pInfo->Height;i++)
		memcpy(pNewImage+Size+i*UINT32Value,(pInfo->ppMatrix)[offset-i],SmallestByteWidth);

	free(pInfo->pPixelStream); /* release old image area */
	pInfo->pPixelStream=pNewImage;
	kF_SetImageMatrix(pInfo);
	return true;
}

bool kF_SetBorderColor(FLOAT32 Color, TFloatImage* pInfo)
{
	UINT16 borderSize=pInfo->BorderSize;
	int i,j;
	/* Set upper border */
	for(i=-borderSize;i<0;i++)
	{
		for(j=-borderSize; j<(int)pInfo->Width+borderSize; j++)
		{
			/*kF_SetPixelFast(j,i,Color,pInfo);*/
			kF_SetPixelMacro(j,i,Color,(*pInfo));
		}
	}
	for(i=0;i<(int)pInfo->Height;i++)
	{
		for(j=0;j<borderSize;j++)
		{
			/* Set left and right border */
			/*kF_SetPixelFast(j-borderSize,i,Color,pInfo);*/
			kF_SetPixelMacro(j-borderSize,i,Color,(*pInfo));
			/*kF_SetPixelFast(pInfo->Width+j,i,Color,pInfo);*/
			kF_SetPixelMacro(pInfo->Width+j,i,Color,(*pInfo));
		}
	}
	/* Set lower border */
	for(i=pInfo->Height;i<(int)pInfo->Height+borderSize;i++)
	{
		for(j=-borderSize; j<(int)pInfo->Width+borderSize; j++)
		{
			/*kF_SetPixelFast(j,i,Color,pInfo);*/
			kF_SetPixelMacro(j,i,Color,(*pInfo));
		}
	}
	return true;
}

bool kF_SetPeriodicBorder(unsigned short BorderSize,TFloatImage* pInfo)
{
	if (pInfo->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "kF_SetPeriodicBorder() Image is empty");
		return false;
	}
	else if (((unsigned int)BorderSize>pInfo->Width) || ((unsigned int)BorderSize>pInfo->Height))
	{
		k_ShowMessageSimple(IPL_ERROR, "kF_SetPeriodicBorder() Border too large");
		return false;
	}
	else
	{
		int x,y;
		unsigned int w,h;
		w=pInfo->Width;
		h=pInfo->Height;
		if (pInfo->BorderSize!=BorderSize) /* only set border if necessary */
			kF_SetBorderSize(BorderSize,pInfo);
		/* setting upper and lover horisontal border values */
		for(y=1;y<=BorderSize;y++)
		{
			for(x=0;x<(int)pInfo->Width;x++)
			{
				kF_SetPixelMacro(x,-y,kF_GetPixelMacro(x,h-y,*pInfo),*pInfo); /* setting upper border */
				kF_SetPixelMacro(x,h+y-1,kF_GetPixelMacro(x,y-1,*pInfo),*pInfo); /* setting lower border */
			}
		}
		/* setting left and right vertical border values */
		for(x=1;x<=BorderSize;x++)
		{
			for(y=0;y<(int)pInfo->Height;y++)
			{
				kF_SetPixelMacro(-x,y,kF_GetPixelMacro(w-x,y,*pInfo),*pInfo); /* setting left border */
				kF_SetPixelMacro(w+x-1,y,kF_GetPixelMacro(x-1,y,*pInfo),*pInfo); /* setting right border */
			}
		}
		/* setting the four corners */
		for(y=1;y<=BorderSize;y++)
		{
			for(x=1;x<=BorderSize;x++)
			{
				/* setting top left corner */
				kF_SetPixelMacro(-x,-y,kF_GetPixelMacro(w-x,h-y,*pInfo),*pInfo);
				/* setting top right corner */
				kF_SetPixelMacro(w+x-1,-y,kF_GetPixelMacro(-x,h+y-1,*pInfo),*pInfo);
				/* setting bottom left corner */
				kF_SetPixelMacro(-x,h+y-1,kF_GetPixelMacro(w-x,y-1,*pInfo),*pInfo);
				/* setting bottom right corner */
				kF_SetPixelMacro(w+x-1,h+y-1,kF_GetPixelMacro(x-1,y-1,*pInfo),*pInfo);
			}
		}
		return true;
	}
}

bool kF_CopyImage(TFloatImage* pDest,const TFloatImage* pSource)
{
	if (pDest==pSource)
	{
		/* cannot copy to itself! */
		k_ShowMessageSimple(
			IPL_ERROR,
			"kF_CopyImage() Source and destination image the same - doing nothing");
		return false;
	}
	/* Allocate memory if necessary */
	if ((pDest->TotalSize!=pSource->TotalSize) || (pDest->BorderSize!=pSource->BorderSize))
	{
		kF_EmptyImage(pDest);
		pDest->TotalSize = pSource->TotalSize;
		pDest->BorderSize=pSource->BorderSize;
		pDest->pPixelStream=(FLOAT32*) malloc(pDest->TotalSize);
	}
	if ((pDest->Width!=pSource->Width) || (pDest->Height!=pSource->Height))
	{
		pDest->Width=pSource->Width; /* need to set width and height for k_SetImageMatrix! */
		pDest->Height=pSource->Height;
		kF_SetImageMatrix(pDest);
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
				"%skF_CopyImage() Source image file info: %s",
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

bool kF_CopySubImage(int xmin,int ymin,int xmax,int ymax,
				  TFloatImage* pDest,const TFloatImage* pSource)
{
	TFloatImage* pTarget=pDest;
	bool CopyToSource=false;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "kF_CopySubImage() Source image is empty - doing nothing");
		return false;
	}
	/* Check if the source and destination is the same image */
	if (pDest==pSource)
	{
		pTarget=(TFloatImage*) malloc(sizeof(TFloatImage));
		kF_InitImage(pTarget);
		CopyToSource=true;
	}
	/* Not optimised regarding speed! */
	if ((xmin<xmax) && (ymin<ymax) && 
		(xmin>=kF_GetMinX(pSource)) && (ymin>=kF_GetMinY(pSource)) &&
		(xmax<=kF_GetMaxX(pSource)) && (ymax<=kF_GetMaxY(pSource)))
	{
		UINT16 xdim,ydim;
		int i;
		int xminNoOrigo=xmin+pSource->Origo.x;
		int yminNoOrigo=ymin+pSource->Origo.y;
		int ymaxNoOrigo=ymax+pSource->Origo.y;
		xdim=(UINT16)(xmax-xmin);
		ydim=(UINT16)(ymax-ymin);
		if (pTarget->TotalSize!=(kF_GetSizeScanLine(xdim)*ydim))
		{
			kF_EmptyImage(pTarget);
			/* prepare destination image */
			kF_AllocImage(xdim,ydim,pTarget);
		}
		k_CopyFileInfo(&(pTarget->FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pTarget->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pTarget->History),
				"%skF_CopySubImage(xmin=%d,ymin=%d,xmax=%d,ymax=%d) "
				"Source image file info: %s",
				ipl_HistoryIndent, xmin, ymin, xmax, ymax,
				pSource->FileInfo.PathAndFileName);
			k_IplStartHistoryMacro();
		}
#endif
		/* copy image pixels */
		for(i=yminNoOrigo; i<ymaxNoOrigo; i++)
			memcpy(pTarget->ppAllScanLines[i-yminNoOrigo],pSource->ppAllScanLines[i]+sizeof(FLOAT32)*xminNoOrigo,xdim*sizeof(FLOAT32));

		pTarget->Origin=RAM;
		/* Set origo */
		pTarget->Origo=pSource->Origo;
        if (CopyToSource==true)
        {
	        kF_CopyImage(pDest,pTarget);
			kF_EmptyImage(pTarget);
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
		k_ShowMessageSimple(IPL_ERROR, "kF_CopySubImage() Illegal clipping rectangel");
		return false;
	}
}

void kF_Rotate90(int Steps,TFloatImage* pSource)
{
	TFloatImage NewImage;
	int x,y;
	int MaxYIndex=pSource->Height-1;
	int MaxXIndex=pSource->Width-1;
	int ActSteps=(Steps % 4);
	if (ActSteps<0)
		ActSteps=4+ActSteps;

	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "kF_Rotate90() Image is empty - doing nothing");
		return;
	}
	if (ActSteps==0)
		return;

	kF_InitImage(&NewImage);
	/* prepare destination image, following lines faster than calling kI_CopyImage() */
	if (ActSteps==2)
		kF_AllocImage(pSource->Width,pSource->Height,&NewImage);
	else
		kF_AllocImage(pSource->Height,pSource->Width,&NewImage);
	k_CopyFileInfo(&(NewImage.FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
	/* copy history before adding comment */
	k_CopyText(&(NewImage.History),&(pSource->History));
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(NewImage.History),
			"%skF_Rotate90(Steps=%d)",
			ipl_HistoryIndent, Steps);
		k_IplStartHistoryMacro();
	}
#endif

	/* rotate image */
	if (ActSteps==1)
	{
		for(y=0; y<(int)pSource->Height; y++)
			for(x=0; x<(int)pSource->Width; x++)
				kF_SetPixelMacro(-y+MaxYIndex,x,kF_GetPixelMacro(x,y,(*pSource)),NewImage);
	}
	else if (ActSteps==2)
	{
		for(y=0; y<(int)pSource->Height; y++)
			for(x=0; x<(int)pSource->Width; x++)
				kF_SetPixelMacro(x,MaxYIndex-y,kF_GetPixelMacro(MaxXIndex-x,y,(*pSource)),NewImage);
	}
	else if (ActSteps==3)
	{
		for(y=0; y<(int)pSource->Height; y++)
			for(x=0; x<(int)pSource->Width; x++)
				kF_SetPixelMacro(y,MaxXIndex-x,kF_GetPixelMacro(x,y,(*pSource)),NewImage);
	}
	kF_CopyImage(pSource,&NewImage);
	kF_EmptyImage(&NewImage);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

void kF_FlipHorisontal(TFloatImage* pSource)
{
	TFloatImage Buf;
	int y;
	int Half=pSource->Height/2;
	int Ext=pSource->Height%2;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "kF_FlipHorisontal() Image is empty - doing nothing");
		return;
	}

	kF_InitImage(&Buf);
	/* flip image */
	/* prepare destination image */
	kF_CopySubImage(kF_GetMinX(pSource),kF_GetMinY(pSource),kF_GetMaxX(pSource),Half-pSource->Origo.y,&Buf,pSource);
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(pSource->History),
			"%skF_FlipHorisontal()",
			ipl_HistoryIndent);
		k_IplStartHistoryMacro();
	}
#endif
	for(y=0;y<Half;y++)
		memcpy(pSource->ppAllScanLines[y],pSource->ppAllScanLines[pSource->Height-y-1],pSource->ByteWidth);
	for(y=Half+Ext; y<(int)pSource->Height; y++)
		memcpy(pSource->ppAllScanLines[y],Buf.ppAllScanLines[Buf.Height-(y-Half-Ext)-1],pSource->ByteWidth);
	kF_EmptyImage(&Buf);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

//void kF_FlipHorisontal(TFloatImage* pSource)
//{
//	TFloatImage NewImage;
//	int x,y;
//	int MaxYIndex=pSource->Height-1;
//	if (pSource->Origin==EMPTY)
//	{
//		TString str;
//		k_InitString(&str);
//		k_AddFileAndLine(str);
//		k_ShowMessage(IPL_WARNING,&str,"kF_FlipHorisontal() Image is empty - doing nothing");
//		k_EmptyString(&str);
//		return;
//	}
//
//	kF_InitImage(&NewImage);
//	/* flip image */
//	/* prepare destination image, following lines faster than calling kF_CopyImage() */
//	kF_AllocImage(pSource->Width,pSource->Height,&NewImage);
//	k_CopyFileInfo(&(NewImage.FileInfo),&(pSource->FileInfo));
//#ifdef IPL_ADD_HISTORY
//	/* copy history before adding comment */
//	k_CopyText(&(NewImage.History),&(pSource->History));
//	/* add comment to history */
//	{
//		TString History;
//		k_InitString(&History);
//		k_SprintfString(&History,"%skF_FlipHorisontal()",ipl_HistoryIndent);
//		k_AppendTextIPL(&(NewImage.History),History.pChars);
//		k_EmptyString(&History);
//		k_IplStartHistoryMacro();
//	}
//#endif
//	for(y=0; y<(int)pSource->Height; y++)
//		for(x=0; x<(int)pSource->Width; x++)
//			kF_SetPixelMacro(x,MaxYIndex-y,kF_GetPixelMacro(x,y,(*pSource)),NewImage);
//
//	kF_CopyImage(pSource,&NewImage);
//	kF_EmptyImage(&NewImage);
//#ifdef IPL_ADD_HISTORY
//	/* end history appending (remove a leading '\t' character) */
//	k_IplStopHistoryMacro();
//#endif
//}

void kF_FlipVertical(TFloatImage* pSource)
{
	TFloatImage NewImage;
	int x,y;
	int MaxXIndex=pSource->Width-1;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "kF_FlipVertical() Image is empty - doing nothing");
		return;
	}

	kF_InitImage(&NewImage);
	/* flip image */
	/* prepare destination image, following lines faster than calling k_CopyImage() */
	kF_AllocImage(pSource->Width,pSource->Height,&NewImage);
	k_CopyFileInfo(&(NewImage.FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
	/* copy history before adding comment */
	k_CopyText(&(NewImage.History),&(pSource->History));
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(NewImage.History),
			"%skF_FlipVertical()",
			ipl_HistoryIndent);
		k_IplStartHistoryMacro();
	}
#endif
	for(y=0; y<(int)pSource->Height; y++)
		for(x=0; x<(int)pSource->Width; x++)
			kF_SetPixelMacro(MaxXIndex-x,y,kF_GetPixelMacro(x,y,(*pSource)),NewImage);

	kF_CopyImage(pSource,&NewImage);
	kF_EmptyImage(&NewImage);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

int kF_GetMinX(const TFloatImage* Img)
{
	return -Img->Origo.x;
}

int kF_GetMinY(const TFloatImage* Img)
{
	return -Img->Origo.y;
}

int kF_GetMaxX(const TFloatImage* Img)
{
	return Img->Width-Img->Origo.x;
}

int kF_GetMaxY(const TFloatImage* Img)
{
	return Img->Height-Img->Origo.y;
}

int kF_InRange(int x, int y, int Delta, const TFloatImage* Img)
{
	if ((x>=(kF_GetMinX(Img)-Delta)) && (x<(kF_GetMaxX(Img)+Delta)) &&
		(y>=(kF_GetMinY(Img)-Delta)) && (y<(kF_GetMaxY(Img)+Delta)))
		return true;
	else
		return false;
}

/****************************************/
/*        Arithmetic functions          */
/****************************************/

bool kF_ApplyBias(float Bias,TFloatImage *pImg)
{
	if (pImg->Origin!=EMPTY)
	{
		unsigned int x,y;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%skF_ApplyBias() Bias=%g",
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
		k_ShowMessageSimple(IPL_ERROR, "kF_ApplyBias: Source image empty");
		return false;
	}
}

bool kF_ScaleIntensity(float Prefactor, TFloatImage *pImg)
{
	if (pImg->Origin!=EMPTY)
	{
		unsigned int x,y;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%skF_ScaleIntensity() Prefactor=%g",
				ipl_HistoryIndent, Prefactor);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<pImg->Height;y++)
		{
			for (x=0;x<pImg->Width;x++) 
			{
				pImg->ppMatrix[y][x]*=Prefactor;
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
		k_ShowMessageSimple(IPL_ERROR, "kF_ScaleIntensity: Source image empty");
		return false;
	}
}

bool kF_Add( TFloatImage *pImg,TFloatImage *pAddend)
{
	if ((pImg->Origin==EMPTY) || (pAddend->Origin==EMPTY)
		|| (pImg->Width!=pAddend->Width) || (pImg->Height!=pAddend->Height))
	{
		k_ShowMessageSimple(IPL_ERROR, "kF_Add(): Images are empty or of different size");
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
				"%skF_Add() Adding a TIFloatmage",
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

bool kF_Subtract(TFloatImage *pImg,TFloatImage *pSubtrahend)
{
	if ((pImg->Origin==EMPTY) || (pSubtrahend->Origin==EMPTY)
		|| (pImg->Width!=pSubtrahend->Width) || (pImg->Height!=pSubtrahend->Height))
	{
		k_ShowMessageSimple(IPL_ERROR, "kF_Subtract(): Images are empty or of different size");
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
				"%skF_Subtract() subtracting a TFloatImage",
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

bool kF_Multiply( TFloatImage *pImg,TFloatImage *pFactor)
{
	if ((pImg->Origin==EMPTY) || (pFactor->Origin==EMPTY)
		|| (pImg->Width!=pFactor->Width) || (pImg->Height!=pFactor->Height))
	{
		k_ShowMessageSimple(IPL_ERROR, "kF_Multiply(): Images are empty or of different size");
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
				"%skF_Multiply() multiplying a TFloatImage",
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

bool kF_CopyDisplace(int dx,int dy, TFloatImage *pImg)
{
	if (pImg->Origin!=EMPTY)
	{
		int x,y,w,h,xd,yd;
		TFloatImage Temp;
		w=pImg->Width;
		h=pImg->Height;
		kF_InitImage(&Temp);
		kF_AllocImage(w,h,&Temp);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(Temp.History),&(pImg->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(Temp.History),
				"%skF_CopyDisplace() dx=%d dy=%d",
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
		kF_CopyImage(pImg,&Temp);
		kF_EmptyImage(&Temp);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	} 
	else
	{
		k_ShowMessageSimple(IPL_ERROR, "kF_CopyDisplace(): ComplexImage is empty");
		return false;
	}
}

bool kF_CutOrZeroPad(TFloatImage*pDest,TFloatImage *pSource)
{
	int x,y,ws,hs,wd,hd;
	ws=pSource->Width;
	hs=pSource->Height;
	wd=pDest->Width;
	hd=pDest->Height;
	if ((pSource->Origin!=EMPTY) && (pDest->Origin!=EMPTY)
		&& (ws>0) && (hs>0) && (wd>0) && (hd>0) && (pSource!=pDest))
	{
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skF_CutOrZeroPad()",
				ipl_HistoryIndent);
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
		char* msg;

		if (pSource==pDest)
			msg = "kF_CutOrZeroPad(): Cannot use same image as dest/source";
		else if ((ws<=0) || (hs<=0) || (wd<=0) || (hd<=0))
			msg = "kF_CutOrZeroPad(): One or both images have no pixels";
		else
			msg = "kF_CutOrZeroPad(): One or both images are empty";
		k_ShowMessageSimple(IPL_ERROR, msg);
		return false;
	}
}

bool kF_CopyFromByte(TFloatImage *pDest,TImage *pSource)
{
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "kF_CopyFromByte(): Image is empty");
		return false;
	}
	else
	{
		int x,y,w,h;
		w=pSource->Width;
		h=pSource->Height;
		kF_AllocImage(w,h,pDest);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skF_CopyFromByte()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<h;y++)
		{
			for (x=0;x<w;x++)
			{
				pDest->ppMatrix[y][x]=(FLOAT32)(pSource->ppMatrix[y][x]);
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool kF_CopyFromInt(TFloatImage *pDest,TIntImage *pSource)
{
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "kF_CopyFromInt(): Image is empty");
		return false;
	}
	else
	{
		int x,y,w,h;
		w=pSource->Width;
		h=pSource->Height;
		kF_AllocImage(w,h,pDest);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skF_CopyFromInt()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<h;y++)
		{
			for (x=0;x<w;x++)
			{
				pDest->ppMatrix[y][x]=(FLOAT32)(pSource->ppMatrix[y][x]);
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool kF_CopyFromComplex(COMPLEXCHOISE mode, TFloatImage *pDest,TComplexImage *pSource)
{
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "kF_CopyFromComplex(): Image is empty");
		return false;
	}
	else
	{
		int x,y,w,h;
		double buf;
		w=pSource->Width;
		h=pSource->Height;
		kF_AllocImage(w,h,pDest);
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skF_CopyFromComplex()",
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
					pDest->ppMatrix[y][x]=(FLOAT32)(pSource->ppMatrixRe[y][x]);
				}
			}
		}
		if (mode==COMPLEX_IMAGINARY)
		{
			for (y=0;y<h;y++)
			{
				for (x=0;x<w;x++)
				{
					pDest->ppMatrix[y][x]=(FLOAT32)(pSource->ppMatrixIm[y][x]);
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
					pDest->ppMatrix[y][x]=(FLOAT32)buf;
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

void kF_PrintImageInfo(const TFloatImage* pInfo)
{
	char* Name;
	k_GetOriginString(pInfo->Origin,&Name);
	printf("**************** FloatImage info ********************\n");
	printf(" Width=%ld Height=%ld ByteWidth=%d SizeImage=%ld\n",
		pInfo->Width, pInfo->Height, pInfo->ByteWidth, pInfo->TotalSize);
	printf(" BorderSize=%d Origo=(%d,%d) Origin=%s\n",
		pInfo->BorderSize,pInfo->Origo.x,pInfo->Origo.y,Name);
	k_PrintFileInfo(&pInfo->FileInfo);
	free(Name);
}

bool kF_DrawLine(T2DInt Start, T2DInt End, FLOAT32 Value, TFloatImage *pDest)
{
	TPixelGroup grp;
	if (pDest==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "kF_DrawLine(): Image is empty");
		return false;
	}
	k_InitGroup(&grp);
	k_GraphicsGenerateLine(Start,End,&grp);
	k_AddGroupToFloatImage(pDest,&grp,Value);
	return true;
}

bool kF_DrawCircle(T2DInt Center, unsigned int Radius, FLOAT32 Value, TFloatImage *pDest)
{
	TPixelGroup grp;
	if (pDest==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "kF_DrawCircle(): Image is empty");
		return false;
	}
	k_InitGroup(&grp);
	k_GraphicsGenerateCircle(Center,Radius,&grp);
	k_AddGroupToFloatImage(pDest,&grp,Value);
	return true;
}

/****************************************/
/*           C only functions           */
/****************************************/

FLOAT32 kF_GetPixel(int x, int y,const TFloatImage* pInfo)
{
	UINT16 border=pInfo->BorderSize;
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if ((x>=-border) && (x<(int) pInfo->Width+border) &&
		(y>=-border) && (y<(int) pInfo->Height+border))
	{
		return kF_GetPixelMacro(x,y,(*pInfo));
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"kF_GetPixel(%d,%d) Outside image and border",
			x, y);
		return 0;
	}
}

FLOAT32 kF_GetPixelFast(int x, int y,const TFloatImage* pInfo)
{
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	return kF_GetPixelMacro(x,y,(*pInfo));
}

bool kF_SetPixel(int x, int y, FLOAT32 Color,TFloatImage* pInfo)
{
	UINT16 border=pInfo->BorderSize;
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if((x>=-border) && (x<(int) pInfo->Width+border) &&
		(y>=-border) && (y<(int) pInfo->Height+border))
	{
		kF_SetPixelMacro(x,y,Color,(*pInfo));
		return true;
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"kF_SetPixel(%d,%d) Outside image and border",
			x, y);
		return false;
	}
}

bool kF_SetPixelFast(int x, int y, FLOAT32 Color,TFloatImage* pInfo)
{
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	kF_SetPixelMacro(x,y,Color,(*pInfo));
	return true;
}

FLOAT32 kF_GetPixelInterpolated(float x, float y, const TFloatImage* pInfo)
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
		return ((kF_GetPixelMacro(xl,yl,(*pInfo))*((float)xh-x) + kF_GetPixelMacro(xh,yl,(*pInfo))*(x-(float)xl))*((float)yh-y) +
		 (kF_GetPixelMacro(xl,yh,(*pInfo))*((float)xh-x) + kF_GetPixelMacro(xh,yh,(*pInfo))*(x-(float)xl))*(y-(float)yl));
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"kF_GetPixelInterpolated(%.2f,%.2f) Needs "
			"to access pixels outside image and border\n",
			x, y);
		return -1;
	}
}

FLOAT32 kF_GetPixelInterpolatedFast(float x, float y, const TFloatImage* pInfo)
{
	/* upper left pixel (l ~ low) */
	int xl = (x >=0 ? (int)x : (int)(x-1)); /* always round down */
	int yl = (y >=0 ? (int)y : (int)(y-1)); /* always round down */
	/* lower right pixel (h ~ high) */
	int xh = xl + 1;
	int yh = yl + 1;
	/* do bilinear interpolation */
	return ((kF_GetPixelMacro(xl,yl,(*pInfo))*((float)xh-x) + kF_GetPixelMacro(xh,yl,(*pInfo))*(x-(float)xl))*((float)yh-y) +
	 (kF_GetPixelMacro(xl,yh,(*pInfo))*((float)xh-x) + kF_GetPixelMacro(xh,yh,(*pInfo))*(x-(float)xl))*(y-(float)yl));
}

/************************************************/
/********        Private functions       ********/
/************************************************/

void kF_SetImageMatrix(TFloatImage* pInfo)
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
	UINT32Value=kF_GetSizeScanLine(realX)/sizeof(FLOAT32);
	/*printf("totalHeight=%d\n",totalHeight);*/
	if (pInfo->ppAllScanLines!=NULL)
	{
		free(pInfo->ppAllScanLines);
	}

	(pInfo->ppAllScanLines) = (FLOAT32**) malloc(sizeof(FLOAT32*)*(totalHeight));
	pInfo->ppMatrix = &((pInfo->ppAllScanLines)[border]);

	for(y=0;y<totalHeight;y++)
	{
		(pInfo->ppAllScanLines)[totalHeight-1-y]=(pInfo->pPixelStream+y*UINT32Value+border);
	}
}

UINT32 kF_GetSizeScanLine(UINT32 Width)
{
	/* returns the size of a scanline, no zero padding in a float image */
	return Width*sizeof(FLOAT32);
}

bool kF_LoadFloat(FILE* fp,TFloatImage* pInfo)
{
	unsigned int Width,Height;
	unsigned int Rows,Cols;
	UINT32 x,y;
	FLOAT32 Value;
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
				k_ShowMessageSimple(
					IPL_ERROR,
					"kF_LoadFloat() wrong header - only found a history - no data");
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
			/* read a special comment line (token) */
			char* Token=NULL;
			k_ReadToken(fp,&Token);
			TokenRead=true;
			if (Token==NULL)
			{
				k_ShowMessageSimple(IPL_ERROR, "kF_LoadFloat() wrong file format, end of file reached");
				return false;
			}
			else if (strcmp(Token,"WidthHeight")==0)
			{
				/* The following two values must be integers */
				if (fscanf(fp,"%d %d",&Width,&Height)!=2)
				{
					k_ShowMessageSimple(IPL_ERROR, "kF_LoadFloat() wrong header - "
						"found \"## WidthHeight\" but no width and height values found");
					return false;
				}
				if (WidthHeightRead==true)
				{
					/* test if rows and cols are the same as width and height */
					if ((Width!=Cols) || (Height!=Rows))
					{
						k_ShowMessageStd(
							IPL_POSITION, IPL_ERROR,
							"kF_LoadFloat() Width=Cols (%d=%d) and height=Rows (%d,%d) "
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
					k_ShowMessageSimple(IPL_ERROR, "kF_LoadFloat() wrong header - "
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
							"kF_LoadFloat() Width=Cols (%d=%d) and height=Rows (%d,%d) "
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
		k_ShowMessageSimple(IPL_ERROR, "kF_LoadFloat() wrong header - width and height not found");
		return false;
	}
	/* allocate image */
	kF_AllocImage(Width,Height,pInfo);
	/* Read data */
	for(y=0;y<Height;y++)
	{
		for(x=0;x<Width;x++)
		{
			if (k_SkipWhiteSpace(fp)!=-1)
			{
				if (fscanf(fp,"%e",&Value)==1)
				{
					kF_SetPixel(x,y,Value,pInfo);
				}
				else
				{
					k_ShowMessageSimple(IPL_ERROR, "kF_LoadFloat() Failed reading image data");
					kF_EmptyImage(pInfo);
					return false;
				}
			}
			else
			{
				k_ShowMessageSimple(IPL_ERROR, "kF_LoadFloat() Failed reading image data");
				kF_EmptyImage(pInfo);
				return false;
			}
		}
	}
	/* copy history to pInfo */
	k_CopyText(&(pInfo->History),&TempHistory);
	k_EmptyText(&TempHistory);
	return true;
}

bool kF_SaveFloat(FILE* fp,TFloatImage* pInfo)
{
	unsigned int x,y;
	char* pHistory=NULL;
	if (pInfo->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "kF_SaveFloat() No image to save");
		fclose(fp);
		return false;
	}
	k_ListToCharArrayText(&(pInfo->History),&pHistory);
	fprintf(fp,"%s\n",pHistory);
	if (pHistory!=NULL)
	{
		free(pHistory);
	}
	fprintf(fp,"## WidthHeight %ld %ld\n",pInfo->Width,pInfo->Height);
	fprintf(fp,"## RowsCols %ld %ld\n\n",pInfo->Height,pInfo->Width);
	for(y=0;y<pInfo->Height;y++)
	{
		for(x=0;x<pInfo->Width;x++)
		{
				fprintf(fp,"% e\t",kF_GetPixelMacro(x,y,(*pInfo)));
			}
			fprintf(fp,"\n");
		}
	return true;
}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
