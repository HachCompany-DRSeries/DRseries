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

#include "c_kernel_functions.h"
#include "../kernel_string.h"
#include "../kernel_error.h"
#include "../kernel_pixelgroup.h"
#include "../kernel_history.h"
#include "../algorithms/kernel_graphics.h"
#include <string.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/****************************************/
/*           Private functions          */
/****************************************/

void kC_SetImageMatrix(TComplexImage* pInfo);
/* returns the size of a scanline, no. zero padding in a float image */
UINT32 kC_GetSizeScanLine(UINT32 Width);
bool kC_LoadComplex(FILE* fp,TComplexImage* pInfo);
bool kC_SaveComplex(FILE* fp,TComplexImage* pInfo);

/****************************************/
/*           Public functions           */
/****************************************/

void kC_EmptyImage(TComplexImage* pInfo)
{
	k_EmptyFileInfo(&(pInfo->FileInfo));
	k_EmptyText(&(pInfo->History));
	if (pInfo->pPixelStreamRe!=NULL)
	{
		free(pInfo->pPixelStreamRe);
		pInfo->pPixelStreamRe=NULL;
	}
        if (pInfo->pPixelStreamIm!=NULL)
	{
		free(pInfo->pPixelStreamIm);
		pInfo->pPixelStreamIm=NULL;
	}
	if (pInfo->ppAllScanLinesRe!=NULL)
	{
		free(pInfo->ppAllScanLinesRe);
		pInfo->ppAllScanLinesRe=NULL;
	}
    if (pInfo->ppAllScanLinesIm!=NULL)
	{
		free(pInfo->ppAllScanLinesIm);
		pInfo->ppAllScanLinesIm=NULL;
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

void kC_InitImage(TComplexImage* pInfo)
{
	k_InitFileInfo(&(pInfo->FileInfo));
	k_InitText(&(pInfo->History));
	pInfo->pPixelStreamRe=NULL;
   	pInfo->pPixelStreamIm=NULL;
	pInfo->ppMatrixRe=NULL;
 	pInfo->ppMatrixIm=NULL;
	pInfo->ppAllScanLinesRe=NULL;
   	pInfo->ppAllScanLinesIm=NULL;
	pInfo->Origin=EMPTY;
	pInfo->Origo.x=0; /* default origo (0,0) */
	pInfo->Origo.y=0;
}

bool kC_AllocImage(UINT32 Width, UINT32 Height,TComplexImage* pInfo)
{
	if(pInfo!=NULL)
	{
		/* Allocate memory if necessary */
		if ((pInfo->Width!=Width) || (pInfo->Height!=Height) || 
			(pInfo->BorderSize!=0))
		{
			/* start with emptying the old image data (if any!) */
			kC_EmptyImage(pInfo);
			/* set values in the image info structure */
			pInfo->Width=Width;
			pInfo->Height=Height;
			pInfo->BorderSize=0; /* always 0 for a new image */
			/* calculate size and width (incl zero-padding) of image */
			pInfo->ByteWidth=(UINT16) kC_GetSizeScanLine(Width);
			pInfo->TotalSize = pInfo->ByteWidth * Height; /* no compression */
			/* Allocate memory for image */
			pInfo->pPixelStreamRe=(FLOAT32*) calloc(sizeof(FLOAT32),pInfo->TotalSize/sizeof(FLOAT32));
			pInfo->pPixelStreamIm=(FLOAT32*) calloc(sizeof(FLOAT32),pInfo->TotalSize/sizeof(FLOAT32));
			kC_SetImageMatrix(pInfo);
		}
		else
		{
			/* if new allocation not needed, we only reset a few things. */
			k_EmptyFileInfo(&(pInfo->FileInfo));
			k_EmptyText(&(pInfo->History));
			kC_FlushImage(0,0,pInfo); /* reset existing memory to 0! */
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
		k_ShowMessageSimple(IPL_ERROR, "kC_AllocImage() pInfo is a NULL pointer");
		return false;
	}
}

bool kC_AllocImageFast(UINT32 Width, UINT32 Height,TComplexImage* pInfo)
{
	if(pInfo!=NULL)
	{
		if ((pInfo->Width!=Width) || (pInfo->Height!=Height) || 
			(pInfo->BorderSize!=0))
		{
			/* start with emptying the old image data (if any!) */
			kC_EmptyImage(pInfo);
			/* set values in the image info structure */
			pInfo->Width=Width;
			pInfo->Height=Height;
			pInfo->BorderSize=0; /* always 0 for a new image */
			/* calculate size and width (incl zero-padding) of image */
			pInfo->ByteWidth=(UINT16) kC_GetSizeScanLine(Width);
			pInfo->TotalSize = pInfo->ByteWidth * Height; /* no compression */
			/* Allocate memory for image */
			pInfo->pPixelStreamRe=(FLOAT32*) malloc(pInfo->TotalSize);
			pInfo->pPixelStreamIm=(FLOAT32*) malloc(pInfo->TotalSize);
			kC_SetImageMatrix(pInfo);
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
		k_ShowMessageSimple(IPL_ERROR, "kC_AllocImageFast() pInfo is a NULL pointer");
		return false;
	}
}

bool kC_FlushImage(FLOAT32 Re, FLOAT32 Im,TComplexImage* pDest)
{
	int y,x;
	if (pDest==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "kC_FlushImage() pDest is a NULL pointer");
		return false;
	}
	else if (pDest->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "kC_FlushImage() Image is empty - doing nothing");
		return false;
	}
	else
	{
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skC_FlushImage(Re=%f Im=%f)",
				ipl_HistoryIndent, Re, Im);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<(int)pDest->Height;y++)
			for (x=0;x<(int)pDest->Width;x++)
			{
				kC_SetPixelMacroRe(x,y,Re,*pDest);
				kC_SetPixelMacroIm(x,y,Im,*pDest);
			}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool kC_Load(const char* FileName, TComplexImage* pInfo)
{
	bool returnValue;
	FILE* fp;
	char* Path=NULL,*FileNameExt=NULL,*Ext=NULL;
	if (!k_SplitFileName(FileName, &Path,&FileNameExt,&Ext))
	{
		k_ShowMessageSimple(IPL_ERROR, "kC_Load() Filename not valid");
		return false;
	}
	else if (strcmp(Ext,"")==0)
	{
		k_ShowMessageSimple(IPL_ERROR, "kC_Load() No extension found");
		returnValue=false;
	}
	else
	{
		returnValue=true;
		k_StringToUpper(Ext);
		fp=fopen(FileName,"rb");  /* open file */
		if (fp==NULL)
		{
			k_ShowMessageSimple(IPL_ERROR, "kC_Load() Could not open file");
			returnValue=false;
		}
		else
		{
			kC_EmptyImage(pInfo);
			/* read from file */
			returnValue=kC_LoadComplex(fp,pInfo);
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
					"%skC_Load(%s)",
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

bool kC_Save(const char* FileName, TComplexImage* pInfo)
{
	bool returnValue;
	FILE* fp;
	char* Path=NULL,*FileNameExt=NULL,*Ext=NULL;
	if (!k_SplitFileName(FileName, &Path,&FileNameExt,&Ext))
	{
		k_ShowMessageSimple(IPL_ERROR, "kC_Save() Filename not valid");
		return false;
	}
	else if (strcmp(Ext,"")==0)
	{
		k_ShowMessageSimple(IPL_ERROR, "kC_Save() No extension found");
		returnValue=false;
	}
	else
	{
		returnValue=true;
		k_StringToUpper(Ext);
		fp=fopen(FileName,"wb"); /* open file */
		if (fp==NULL)
		{
			k_ShowMessageSimple(IPL_ERROR, "kC_Save() Could not save file");
			returnValue=false;
		}
		else if (pInfo->Origin==EMPTY)
		{
			k_ShowMessageSimple(IPL_ERROR, "kC_Save() No image - cannot save");
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
					"%skC_Save(%s)",
					ipl_HistoryIndent, FileName);
			}
#endif
			returnValue=kC_SaveComplex(fp,pInfo);
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

bool kC_SetBorder(UINT16 Size, FLOAT32 Re, FLOAT32 Im, TComplexImage* pInfo)
{
	kC_SetBorderSize(Size,pInfo);
	kC_SetBorderColor(Re, Im,pInfo);
	return true;
}

bool kC_SetBorderSize(UINT16 Size, TComplexImage* pInfo)
{
	UINT32 realX,realY,offset;
	UINT32 UINT32Value; /* temporary variable */
	FLOAT32* pNewImageRe, *pNewImageIm; /* variables pointing to new allocated image area */
	UINT32 i;
	UINT32 SmallestByteWidth;
	
	realX=pInfo->Width+2*Size; /* width of image + border */
	realY=pInfo->Height+2*Size; /* height of image + border */
	/* calculate size and width (incl zero-padding) of image + border */
	UINT32Value=kC_GetSizeScanLine(realX);
	/* Only copy memory area for the shortest width of the destination and source image */
	SmallestByteWidth=UINT32Value<(UINT32)pInfo->ByteWidth?UINT32Value:pInfo->ByteWidth;
	pInfo->BorderSize=Size;
	pInfo->ByteWidth=(UINT16) UINT32Value;
	pInfo->TotalSize=(UINT16) UINT32Value*(realY);
	/* Allocate memory for image */
	pNewImageRe=(FLOAT32*) malloc(pInfo->TotalSize);
	pNewImageIm=(FLOAT32*) malloc(pInfo->TotalSize);
	offset=pInfo->Height-1+Size;
	UINT32Value /= sizeof(FLOAT32);
	for(i=Size;i<Size+pInfo->Height;i++)
	{
		memcpy(pNewImageRe+Size+i*UINT32Value,(pInfo->ppMatrixRe)[offset-i],SmallestByteWidth);
		memcpy(pNewImageIm+Size+i*UINT32Value,(pInfo->ppMatrixIm)[offset-i],SmallestByteWidth);
	}
	free(pInfo->pPixelStreamRe); /* release old image area */
	free(pInfo->pPixelStreamIm); /* release old image area */
	pInfo->pPixelStreamRe=pNewImageRe;
	pInfo->pPixelStreamIm=pNewImageIm;
	kC_SetImageMatrix(pInfo);
	return true;
}

bool kC_SetBorderColor(FLOAT32 Re, FLOAT32 Im, TComplexImage* pInfo)
{
	UINT16 borderSize=pInfo->BorderSize;
	int i,j;
	/* Set upper border */
	for(i=-borderSize;i<0;i++)
	{
		for(j=-borderSize; j<(int)pInfo->Width+borderSize; j++)
		{
			kC_SetPixelMacroRe(j,i,Re,(*pInfo));
			kC_SetPixelMacroIm(j,i,Im,(*pInfo));
		}
	}
	for(i=0;i<(int)pInfo->Height;i++)
	{
		for(j=0;j<borderSize;j++)
		{
			/* Set left and right border */
			kC_SetPixelMacroRe(j-borderSize,i,Re,(*pInfo));
			kC_SetPixelMacroIm(pInfo->Width+j,i,Im,(*pInfo));
		}
	}
	/* Set lower border */
	for(i=pInfo->Height;i<(int)pInfo->Height+borderSize;i++)
	{
		for(j=-borderSize; j<(int)pInfo->Width+borderSize; j++)
		{
			kC_SetPixelMacroRe(j,i,Re,(*pInfo));
			kC_SetPixelMacroIm(j,i,Im,(*pInfo));
		}
	}
	return true;
}

bool kC_SetPeriodicBorder(unsigned short BorderSize,TComplexImage* pInfo)
{
	if (pInfo->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "kC_SetPeriodicBorder() Image is empty");
		return false;
	}
	else if (((unsigned int)BorderSize>pInfo->Width) || ((unsigned int)BorderSize>pInfo->Height))
	{
		k_ShowMessageSimple(IPL_ERROR, "kC_SetPeriodicBorder() Border too large");
		return false;
	}
	else
	{
		int x,y;
		unsigned int w,h;
		w=pInfo->Width;
		h=pInfo->Height;
		if (pInfo->BorderSize!=BorderSize) /* only set border if necessary */
			kC_SetBorderSize(BorderSize,pInfo);
		/* setting upper and lover horisontal border values */
		for(y=1;y<=BorderSize;y++)
		{
			for(x=0;x<(int)pInfo->Width;x++)
			{
				kC_SetPixelMacroRe(x,-y,kC_GetPixelMacroRe(x,h-y,*pInfo),*pInfo); /* setting upper border */
				kC_SetPixelMacroRe(x,h+y-1,kC_GetPixelMacroRe(x,y-1,*pInfo),*pInfo); /* setting lower border */
				kC_SetPixelMacroIm(x,-y,kC_GetPixelMacroIm(x,h-y,*pInfo),*pInfo); /* setting upper border */
				kC_SetPixelMacroIm(x,h+y-1,kC_GetPixelMacroIm(x,y-1,*pInfo),*pInfo); /* setting lower border */
			}
		}
		/* setting left and right vertical border values */
		for(x=1;x<=BorderSize;x++)
		{
			for(y=0;y<(int)pInfo->Height;y++)
			{
				kC_SetPixelMacroRe(-x,y,kC_GetPixelMacroRe(w-x,y,*pInfo),*pInfo); /* setting left border */
				kC_SetPixelMacroRe(w+x-1,y,kC_GetPixelMacroRe(x-1,y,*pInfo),*pInfo); /* setting right border */
				kC_SetPixelMacroIm(-x,y,kC_GetPixelMacroIm(w-x,y,*pInfo),*pInfo); /* setting left border */
				kC_SetPixelMacroIm(w+x-1,y,kC_GetPixelMacroIm(x-1,y,*pInfo),*pInfo); /* setting right border */
			}
		}
		/* setting the four corners */
		for(y=1;y<=BorderSize;y++)
		{
			for(x=1;x<=BorderSize;x++)
			{
				/* setting top left corner */
				kC_SetPixelMacroRe(-x,-y,kC_GetPixelMacroRe(w-x,h-y,*pInfo),*pInfo);
				kC_SetPixelMacroIm(-x,-y,kC_GetPixelMacroIm(w-x,h-y,*pInfo),*pInfo);
				/* setting top right corner */
				kC_SetPixelMacroRe(w+x-1,-y,kC_GetPixelMacroRe(-x,h+y-1,*pInfo),*pInfo);
				kC_SetPixelMacroIm(w+x-1,-y,kC_GetPixelMacroIm(-x,h+y-1,*pInfo),*pInfo);
				/* setting bottom left corner */
				kC_SetPixelMacroRe(-x,h+y-1,kC_GetPixelMacroRe(w-x,y-1,*pInfo),*pInfo);
				kC_SetPixelMacroIm(-x,h+y-1,kC_GetPixelMacroIm(w-x,y-1,*pInfo),*pInfo);
				/* setting bottom right corner */
				kC_SetPixelMacroRe(w+x-1,h+y-1,kC_GetPixelMacroRe(x-1,y-1,*pInfo),*pInfo);
				kC_SetPixelMacroIm(w+x-1,h+y-1,kC_GetPixelMacroIm(x-1,y-1,*pInfo),*pInfo);
			}
		}
		return true;
	}
}

bool kC_CopyImage(TComplexImage* pDest,const TComplexImage* pSource)
{
	if (pDest==pSource)
	{
		/* cannot copy to itself! */
		k_ShowMessageSimple(
			IPL_ERROR,
			"kC_CopyImage() Source and destination image the same - doing nothing");
		return false;
	}
	/* Allocate memory if necessary */
	if ((pDest->TotalSize!=pSource->TotalSize) || (pDest->BorderSize!=pSource->BorderSize))
	{
		kC_EmptyImage(pDest);
		pDest->TotalSize = pSource->TotalSize;
		pDest->BorderSize=pSource->BorderSize;
		pDest->pPixelStreamRe=(FLOAT32*) malloc(pDest->TotalSize);
		pDest->pPixelStreamIm=(FLOAT32*) malloc(pDest->TotalSize);
	}
	if ((pDest->Width!=pSource->Width) || (pDest->Height!=pSource->Height))
	{
		pDest->Width=pSource->Width; /* need to set width and height for k_SetImageMatrix! */
		pDest->Height=pSource->Height;
		kC_SetImageMatrix(pDest);
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
				"%skC_CopyImage() Source image file info: %s",
				ipl_HistoryIndent, pSource->FileInfo.PathAndFileName);
			k_IplStartHistoryMacro();
		}
#endif
		/* copy file information */
		k_CopyFileInfo(&(pDest->FileInfo),&(pSource->FileInfo));
		/* copy image data */
		memcpy(pDest->pPixelStreamRe,pSource->pPixelStreamRe,pDest->TotalSize);
		memcpy(pDest->pPixelStreamIm,pSource->pPixelStreamIm,pDest->TotalSize);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
	}
	return true;
}

bool kC_CopySubImage(int xmin,int ymin,int xmax,int ymax,
					 TComplexImage* pDest,const TComplexImage* pSource)
{
	TComplexImage* pTarget=pDest;
	bool CopyToSource=false;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(
			IPL_WARNING,
			"kC_CopySubImage() Source image is empty - doing nothing");
		return false;
	}
	/* Check if the source and destination is the same image */
	if (pDest==pSource)
	{
		pTarget=(TComplexImage*) malloc(sizeof(TComplexImage));
		kC_InitImage(pTarget);
		CopyToSource=true;
	}
	/* Not optimised regarding speed! */
	if ((xmin<xmax) && (ymin<ymax) &&
		(xmin>=kC_GetMinX(pSource)) && (ymin>=kC_GetMinY(pSource)) &&
		(xmax<=kC_GetMaxX(pSource)) && (ymax<=kC_GetMaxY(pSource)))
	{
		UINT16 xdim,ydim;
		int i;
		int xminNoOrigo=xmin+pSource->Origo.x;
		int yminNoOrigo=ymin+pSource->Origo.y;
		int ymaxNoOrigo=ymax+pSource->Origo.y;
		xdim=(UINT16)(xmax-xmin);
		ydim=(UINT16)(ymax-ymin);
		if (pTarget->TotalSize!=(kC_GetSizeScanLine(xdim)*ydim))
		{
			kC_EmptyImage(pDest);
			/* prepare destination image */
			kC_AllocImage(xdim,ydim,pDest);
		}
		k_CopyFileInfo(&(pTarget->FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
		/* copy history before adding comment */
		k_CopyText(&(pTarget->History),&(pSource->History));
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pTarget->History),
				"%skC_CopySubImage(xmin=%d,ymin=%d,xmax=%d,ymax=%d)"
				" Source image file info: %s",
				ipl_HistoryIndent, xmin, ymin, xmax, ymax,
				pSource->FileInfo.PathAndFileName);
			k_IplStartHistoryMacro();
		}
#endif
		/* copy image pixels */
		for(i=yminNoOrigo; i<ymaxNoOrigo; i++)
		{
			memcpy(pTarget->ppAllScanLinesRe[i-yminNoOrigo],pSource->ppAllScanLinesRe[i]+sizeof(FLOAT32)*xminNoOrigo,xdim*sizeof(FLOAT32));
			memcpy(pTarget->ppAllScanLinesIm[i-yminNoOrigo],pSource->ppAllScanLinesIm[i]+sizeof(FLOAT32)*xminNoOrigo,xdim*sizeof(FLOAT32));
		}
		pDest->Origin=RAM;
		/* Set origo */
		pDest->Origo=pSource->Origo;
		if (CopyToSource==true)
		{
			kC_CopyImage(pDest,pTarget);
			kC_EmptyImage(pTarget);
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
		k_ShowMessageSimple(IPL_ERROR, "kC_CopySubImage() Illegal clipping rectangel");
		return false;
	}
}

void kC_Rotate90(int Steps,TComplexImage* pSource)
{
	TComplexImage NewImage;
	int x,y;
	int MaxYIndex=pSource->Height-1;
	int MaxXIndex=pSource->Width-1;
	int ActSteps=(Steps % 4);
	if (ActSteps<0)
		ActSteps=4+ActSteps;

	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "kC_Rotate90() Image is empty - doing nothing");
		return;
	}
	if (ActSteps==0)
		return;

	kC_InitImage(&NewImage);
	/* prepare destination image, following lines faster than calling kC_CopyImage() */
	if (ActSteps==2)
		kC_AllocImage(pSource->Width,pSource->Height,&NewImage);
	else
		kC_AllocImage(pSource->Height,pSource->Width,&NewImage);
	k_CopyFileInfo(&(NewImage.FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
	/* copy history before adding comment */
	k_CopyText(&(NewImage.History),&(pSource->History));
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(NewImage.History),
			"%skC_Rotate90(Steps=%d)",
			ipl_HistoryIndent, Steps);
		k_IplStartHistoryMacro();
	}
#endif

	/* rotate image */
	if (ActSteps==1)
	{
		for(y=0; y<(int)pSource->Height; y++)
		{
			for(x=0; x<(int)pSource->Width; x++)
			{
				kC_SetPixelMacroRe(-y+MaxYIndex,x,kC_GetPixelMacroRe(x,y,(*pSource)),NewImage);
				kC_SetPixelMacroIm(-y+MaxYIndex,x,kC_GetPixelMacroIm(x,y,(*pSource)),NewImage);
			}
		}
	}
	else if (ActSteps==2)
	{
		for(y=0; y<(int)pSource->Height; y++)
		{
			for(x=0; x<(int)pSource->Width; x++)
			{
				kC_SetPixelMacroRe(x,MaxYIndex-y,kC_GetPixelMacroRe(MaxXIndex-x,y,(*pSource)),NewImage);
				kC_SetPixelMacroIm(x,MaxYIndex-y,kC_GetPixelMacroIm(MaxXIndex-x,y,(*pSource)),NewImage);
			}
		}
	}
	else if (ActSteps==3)
	{
		for(y=0; y<(int)pSource->Height; y++)
		{
			for(x=0; x<(int)pSource->Width; x++)
			{
				kC_SetPixelMacroRe(y,MaxXIndex-x,kC_GetPixelMacroRe(x,y,(*pSource)),NewImage);
				kC_SetPixelMacroIm(y,MaxXIndex-x,kC_GetPixelMacroIm(x,y,(*pSource)),NewImage);
			}
		}		
	}
	kC_CopyImage(pSource,&NewImage);
	kC_EmptyImage(&NewImage);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

void kC_FlipHorisontal(TComplexImage* pSource)
{
	TComplexImage Buf;
	int y;
	int Half=pSource->Height/2;
	int Ext=pSource->Height%2;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "kC_FlipHorisontal() Image is empty - doing nothing");
		return;
	}

	kC_InitImage(&Buf);
	/* flip image */
	/* prepare destination image */
	kC_CopySubImage(kC_GetMinX(pSource),kC_GetMinY(pSource),kC_GetMaxX(pSource),Half-pSource->Origo.y,&Buf,pSource);
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(pSource->History),
			"%skC_FlipHorisontal()",
			ipl_HistoryIndent);
		k_IplStartHistoryMacro();
	}
#endif
	for(y=0;y<Half;y++)
	{
		memcpy(pSource->ppAllScanLinesRe[y],pSource->ppAllScanLinesRe[pSource->Height-y-1],pSource->ByteWidth);
		memcpy(pSource->ppAllScanLinesIm[y],pSource->ppAllScanLinesIm[pSource->Height-y-1],pSource->ByteWidth);
	}
	for(y=Half+Ext; y<(int)pSource->Height; y++)
	{
		memcpy(pSource->ppAllScanLinesRe[y],Buf.ppAllScanLinesRe[Buf.Height-(y-Half-Ext)-1],pSource->ByteWidth);
		memcpy(pSource->ppAllScanLinesIm[y],Buf.ppAllScanLinesIm[Buf.Height-(y-Half-Ext)-1],pSource->ByteWidth);
	}
	kC_EmptyImage(&Buf);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

void kC_FlipVertical(TComplexImage* pSource)
{
	TComplexImage NewImage;
	int x,y;
	int MaxXIndex=pSource->Width-1;
	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "kC_FlipVertical() Image is empty - doing nothing");
		return;
	}

	kC_InitImage(&NewImage);
	/* flip image  */
	/* prepare destination image, following lines faster than calling k_CopyImage() */
	kC_AllocImage(pSource->Width,pSource->Height,&NewImage);
	k_CopyFileInfo(&(NewImage.FileInfo),&(pSource->FileInfo));
#ifdef IPL_ADD_HISTORY
	/* copy history before adding comment */
	k_CopyText(&(NewImage.History),&(pSource->History));
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(NewImage.History),
			"%skC_FlipVertical()",
			ipl_HistoryIndent);
		k_IplStartHistoryMacro();
	}
#endif
	for(y=0; y<(int)pSource->Height; y++)
	{
		for(x=0; x<(int)pSource->Width; x++)
		{
			kC_SetPixelMacroRe(MaxXIndex-x,y,kC_GetPixelMacroRe(x,y,(*pSource)),NewImage);
			kC_SetPixelMacroIm(MaxXIndex-x,y,kC_GetPixelMacroIm(x,y,(*pSource)),NewImage);
		}
	}
	kC_CopyImage(pSource,&NewImage);
	kC_EmptyImage(&NewImage);
#ifdef IPL_ADD_HISTORY
	/* end history appending (remove a leading '\t' character) */
	k_IplStopHistoryMacro();
#endif
}

int kC_GetMinX(const TComplexImage* Img)
{
	return -Img->Origo.x;
}

int kC_GetMinY(const TComplexImage* Img)
{
	return -Img->Origo.y;
}

int kC_GetMaxX(const TComplexImage* Img)
{
	return Img->Width-Img->Origo.x;
}

int kC_GetMaxY(const TComplexImage* Img)
{
	return Img->Height-Img->Origo.y;
}

int kC_InRange(int x, int y, int Delta, const TComplexImage* Img)
{
	if ((x>=(kC_GetMinX(Img)-Delta)) && (x<(kC_GetMaxX(Img)+Delta)) &&
		(y>=(kC_GetMinY(Img)-Delta)) && (y<(kC_GetMaxY(Img)+Delta)))
		return true;
	else
		return false;
}

/****************************************/
/*        Arithmetic functions          */
/****************************************/

bool kC_ApplyBias(FLOAT32 Bias,TComplexImage *pImg)
{ 
	if (pImg->Origin!=EMPTY)
	{
		unsigned int x, y;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%skC_ApplyBias() Bias=%g",
				ipl_HistoryIndent, Bias);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<pImg->Height;y++)
		{
			for (x=0;x<pImg->Width;x++)
			{
				pImg->ppMatrixRe[y][x]+=Bias;
				pImg->ppMatrixIm[y][x]+=Bias;
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
		k_ShowMessageSimple(IPL_ERROR, "kC_ApplyBias(): Image is empty");
		return false;
	}
}

bool kC_ScaleIntensity(FLOAT32 Prefactor,TComplexImage *pImg)
{ 
	if (pImg->Origin!=EMPTY)
	{
		unsigned int x, y;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%skC_ScaleIntensity() Prefactor=%g",
				ipl_HistoryIndent, Prefactor);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<pImg->Height;y++)
		{
			for (x=0;x<pImg->Width;x++)
			{
				pImg->ppMatrixRe[y][x]*=Prefactor;
				pImg->ppMatrixIm[y][x]*=Prefactor;
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
		k_ShowMessageSimple(IPL_ERROR, "kC_ScaleIntensity(): Image is empty");
		return false;
	}
}


bool kC_Add(TComplexImage *pAddend,TComplexImage *pImg)
{ 
	if ((pImg->Origin==EMPTY) || (pAddend->Origin==EMPTY) ||
		(pImg->Width!=pAddend->Width) || (pImg->Height!=pAddend->Height))
    {
		k_ShowMessageSimple(IPL_ERROR, "kC_Add(): Images are empty or of different size");
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
				"%skC_Add() Adding a TComplexImage",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<pImg->Height;y++)
		{
			for (x=0;x<pImg->Width;x++)
			{
				pImg->ppMatrixRe[y][x]+=pAddend->ppMatrixRe[y][x];
				pImg->ppMatrixIm[y][x]+=pAddend->ppMatrixIm[y][x];
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool kC_Subtract(TComplexImage *pSubtrahend,TComplexImage *pImg)
{ 
	if ((pImg->Origin==EMPTY) || (pSubtrahend->Origin==EMPTY)
		|| (pImg->Width!=pSubtrahend->Width) || (pImg->Height!=pSubtrahend->Height))
    {
		k_ShowMessageSimple(IPL_ERROR, "kC_Subtract(): Images are empty or of different size");
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
				"%skC_Subtract() subtracting a TComplexImage",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<pImg->Height;y++)
		{
			for (x=0;x<pImg->Width;x++)
			{
				pImg->ppMatrixRe[y][x]-=pSubtrahend->ppMatrixRe[y][x];
				pImg->ppMatrixIm[y][x]-=pSubtrahend->ppMatrixIm[y][x];
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool kC_Multiply(TComplexImage *pImg, TComplexImage *pFactor)
{ 
	if ((pImg->Origin==EMPTY) || (pFactor->Origin==EMPTY)
		|| (pImg->Width!=pFactor->Width) || (pImg->Height!=pFactor->Height))
    {
		k_ShowMessageSimple(IPL_ERROR, "kC_Multiply(): Images are empty or of different size");
		return false;
    } 
	else
    {
		unsigned int x, y;
		float buf1r,buf1i,buf2r,buf2i;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%skC_Multiply() multiplying a TComplexImage",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<pImg->Height;y++)
		{
			for (x=0;x<pImg->Width;x++)
			{
				buf1r=pImg->ppMatrixRe[y][x];
				buf1i=pImg->ppMatrixIm[y][x];
				buf2r=pFactor->ppMatrixRe[y][x];
				buf2i=pFactor->ppMatrixIm[y][x];
				pImg->ppMatrixRe[y][x]=buf1r*buf2r-buf1i*buf2i;
				pImg->ppMatrixIm[y][x]=buf1r*buf2i+buf1i*buf2r;
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool kC_CCFactorAndMultiply(TComplexImage *pImg, TComplexImage *pFactor)
{ 
	if ((pImg->Origin==EMPTY) || (pFactor->Origin==EMPTY)
		|| (pImg->Width!=pFactor->Width) || (pImg->Height!=pFactor->Height))
    {
		k_ShowMessageSimple(IPL_ERROR, "kC_CCFactorAndMultiply(): Images are empty or of different size");
		return false;
    }
	else
    {
		unsigned int x, y;
		float buf1r,buf1i,buf2r,buf2i;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%skC_CCFactorAndMultiply()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<pImg->Height;y++)
		{
			for (x=0;x<pImg->Width;x++)
			{
				buf1r=pImg->ppMatrixRe[y][x];
				buf1i=pImg->ppMatrixIm[y][x];
				buf2r=pFactor->ppMatrixRe[y][x];
				buf2i=-pFactor->ppMatrixIm[y][x];
				pImg->ppMatrixRe[y][x]=buf1r*buf2r-buf1i*buf2i;
				pImg->ppMatrixIm[y][x]=buf1r*buf2i+buf1i*buf2r;
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool kC_ComplexConjugate(TComplexImage *pImg)
{ 
	if (pImg->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "kC_ComplexConjugate(): Image is empty");
		return false;
	}
	else
	{
		unsigned int x, y;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%skC_ComplexConjugate()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<pImg->Height;y++)
		{
			for (x=0;x<pImg->Width;x++) 
			{
				pImg->ppMatrixIm[y][x]=-pImg->ppMatrixIm[y][x];
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool kC_Rearrange(TComplexImage *pImg)
{ 
	if (pImg->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_ERROR, "kC_Rerrange(): image is empty");
		return false;
	}
	else
	{
		unsigned int x,y,w2,h2;
		FLOAT32 re,im;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%skC_Rearrange()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		w2=pImg->Width/2;
		h2=pImg->Height/2;
		for (y=0;y<h2;y++)
		{
			for (x=0;x<w2;x++)
			{ 
				re=pImg->ppMatrixRe[y][x];
				im=pImg->ppMatrixIm[y][x];
				pImg->ppMatrixRe[y][x]=pImg->ppMatrixRe[y+h2][x+w2];
				pImg->ppMatrixIm[y][x]=pImg->ppMatrixIm[y+h2][x+w2];
				pImg->ppMatrixRe[y+h2][x+w2]=re;
				pImg->ppMatrixIm[y+h2][x+w2]=im;
			}
		}
		for (y=h2;y<pImg->Height;y++)
		{
			for (x=0;x<w2;x++)
			{
				re=pImg->ppMatrixRe[y][x];
				im=pImg->ppMatrixIm[y][x];
				pImg->ppMatrixRe[y][x]=pImg->ppMatrixRe[y-h2][x+w2];
				pImg->ppMatrixIm[y][x]=pImg->ppMatrixIm[y-h2][x+w2];
				pImg->ppMatrixRe[y-h2][x+w2]=re;
				pImg->ppMatrixIm[y-h2][x+w2]=im;
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool kC_CopyDisplace(int dx,int dy,TComplexImage *pImg)
{
	if (pImg->Origin!=EMPTY)
	{
		int x,y,w,h,xd,yd;
		TComplexImage Temp;
		w=pImg->Width;
		h=pImg->Height;
		kC_InitImage(&Temp);
		kC_AllocImage(w,h,&Temp);
		/* copy history before adding comment */
		k_CopyText(&(Temp.History),&(pImg->History));
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(Temp.History),
				"%skC_CopyDisplace() dx=%d dy=%d",
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
				Temp.ppMatrixRe[yd][xd]=pImg->ppMatrixRe[y][x];
				Temp.ppMatrixIm[yd][xd]=pImg->ppMatrixIm[y][x];
			}
		}
		kC_CopyImage(pImg,&Temp);
		kC_EmptyImage(&Temp);
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
	else
	{
		k_ShowMessageSimple(IPL_ERROR, "kC_CopyDisplace(): Image is empty");
		return false;
	}
}

bool kC_CutOrZeroPad(TComplexImage *pSource, TComplexImage*pDest)
{
	int x,y,ws,hs,wd,hd;
	ws=pSource->Width;
	hs=pSource->Height;
	wd=pDest->Width;
	hd=pDest->Height;
	if ((pSource->Origin!=EMPTY) && (pDest->Origin!=EMPTY) &&
		(ws>0) && (hs>0) && (wd>0) && (hd>0) && (pSource!=pDest))
	{
		/* copy history before adding comment */
		k_CopyText(&(pDest->History),&(pSource->History));
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skC_CutOrZeroPad() Source image file info: %s",
				ipl_HistoryIndent, pSource->FileInfo.PathAndFileName);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<hd;y++)
		{
			for (x=0;x<wd;x++)
			{ 
				if ((x<ws) && (y<hs))
				{
					pDest->ppMatrixRe[y][x]=pSource->ppMatrixRe[y][x];
					pDest->ppMatrixIm[y][x]=pSource->ppMatrixIm[y][x];
				}
				else
				{
					pDest->ppMatrixRe[y][x]=0;
					pDest->ppMatrixIm[y][x]=0;
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
			msg = "kC_CutOrZeroPad(): Cannot use same image as dest/source";
		else if ((ws<=0) || (hs<=0) || (wd<=0) || (hd<=0))
			msg = "kC_CutOrZeroPad(): One or both images have no pixels";
		else
			msg = "kC_CutOrZeroPad(): One or both images are empty";
		k_ShowMessageSimple(IPL_ERROR, msg);
		return false;
	}
}

bool kC_CopyFromByte(TComplexImage *pDest, TImage *pRe, TImage *pIm)
{
	int x,y,w,h;
	if ((pRe->Origin==EMPTY) || (pIm->Origin==EMPTY) || (pRe->Width!=pIm->Width) || (pRe->Height!=pIm->Height))
	{
		k_ShowMessageSimple(IPL_ERROR, "kC_CopyFromByte(): Source images are not same size or empty");
		return false;
	}
	else
	{
		w=pRe->Width;
		h=pRe->Height;
		kC_AllocImage(w,h,pDest);
#ifdef IPL_ADD_HISTORY
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skC_CopyFromByte() History from pRe",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
		/* copy history from pRe before adding comment */
		k_CopyText(&(pDest->History),&(pRe->History));
		k_IplStopHistoryMacro();
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skC_CopyFromByte() History from pIm",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
		/* copy history from pRe before adding comment */
		k_CopyText(&(pDest->History),&(pIm->History));
		k_IplStopHistoryMacro();
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skC_CopyFromByte()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<h;y++)
		{
			for (x=0;x<w;x++)
			{
				pDest->ppMatrixRe[y][x]=pRe->ppMatrix[y][x];
				pDest->ppMatrixIm[y][x]=pIm->ppMatrix[y][x];
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool kC_CopyFromInt(TComplexImage *pDest, TIntImage *pRe, TIntImage *pIm)
{
	if ((pRe->Origin==EMPTY) || (pIm->Origin==EMPTY) || (pRe->Width!=pIm->Width) || (pRe->Height!=pIm->Height))
	{
		k_ShowMessageSimple(IPL_ERROR, "kC_CopyFromInt():  Source images are not same size or empty");
		return false;
	}
	else
	{
		int x,y,w,h;
		w=pRe->Width;
		h=pRe->Height;
		kC_AllocImage(w,h,pDest);
#ifdef IPL_ADD_HISTORY
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skC_CopyFromByte() History from pRe",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
		/* copy history from pRe before adding comment */
		k_CopyText(&(pDest->History),&(pRe->History));
		k_IplStopHistoryMacro();
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skC_CopyFromByte() History from pIm",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
		/* copy history from pRe before adding comment */
		k_CopyText(&(pDest->History),&(pIm->History));
		k_IplStopHistoryMacro();
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skC_CopyFromInt()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<h;y++)
		{
			for (x=0;x<w;x++)
			{
				pDest->ppMatrixRe[y][x]=pRe->ppMatrix[y][x];
				pDest->ppMatrixIm[y][x]=pIm->ppMatrix[y][x];
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

bool kC_CopyFromFloat(TComplexImage *pDest, TFloatImage *pRe, TFloatImage *pIm)
{
	if ((pRe->Origin==EMPTY) || (pIm->Origin==EMPTY) || (pRe->Width!=pIm->Width) || (pRe->Height!=pIm->Height))
	{
		k_ShowMessageSimple(IPL_ERROR, "kC_CopyFromFloat():  Source images are not same size or empty");
		return false;
	}
	else
	{
		int x,y,w,h;
		w=pRe->Width;
		h=pRe->Height;
		kC_AllocImage(w,h,pDest);
#ifdef IPL_ADD_HISTORY
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skC_CopyFromByte() History from pRe",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
		/* copy history from pRe before adding comment */
		k_CopyText(&(pDest->History),&(pRe->History));
		k_IplStopHistoryMacro();
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skC_CopyFromByte() History from pIm",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
		/* copy history from pRe before adding comment */
		k_CopyText(&(pDest->History),&(pIm->History));
		k_IplStopHistoryMacro();
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pDest->History),
				"%skC_CopyFromFloat()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		for (y=0;y<h;y++)
		{
			for (x=0;x<w;x++)
			{
				pDest->ppMatrixRe[y][x]=pRe->ppMatrix[y][x];
				pDest->ppMatrixIm[y][x]=pIm->ppMatrix[y][x];
			}
		}
#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		return true;
	}
}

void kC_PrintImageInfo(const TComplexImage* pInfo)
{
	char* Name;
	k_GetOriginString(pInfo->Origin,&Name);
	printf("**************** ComplexImage info ********************\n");
	printf(" Width=%ld Height=%ld ByteWidth=%d SizeImage=%ld\n",
		pInfo->Width, pInfo->Height, pInfo->ByteWidth, pInfo->TotalSize);
	printf(" BorderSize=%d Origo=(%d,%d) Origin=%s\n",
		pInfo->BorderSize, pInfo->Origo.x, pInfo->Origo.y,Name);
	k_PrintFileInfo(&pInfo->FileInfo);
	free(Name);
}

bool kC_DrawLine(T2DInt Start, T2DInt End, FLOAT32 Value, TComplexImage *pDest)
{
	TPixelGroup grp;
	if (pDest==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "kC_DrawLine(): Image is empty");
		return false;
	}
	k_InitGroup(&grp);
	k_GraphicsGenerateLine(Start,End,&grp);
	k_AddGroupToComplexImage(pDest,&grp,Value);
	return true;
}

bool kC_DrawCircle(T2DInt Center, unsigned int Radius, FLOAT32 Value, TComplexImage *pDest)
{
	TPixelGroup grp;
	if (pDest==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_DrawCircle(): Image is empty");
		return false;
	}
	k_InitGroup(&grp);
	k_GraphicsGenerateCircle(Center,Radius,&grp);
	k_AddGroupToComplexImage(pDest,&grp,Value);
	return true;
}

/************************************************/
/********        C only functions        ********/
/************************************************/

FLOAT32 kC_GetPixelRe(int x, int y,const TComplexImage* pInfo)
{
	UINT16 border=pInfo->BorderSize;
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if ((x>=-border) && (x<(int) pInfo->Width+border) &&
		(y>=-border) && (y<(int) pInfo->Height+border))
	{
		return kC_GetPixelMacroRe(x,y,(*pInfo));
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"kC_GetPixel(%d,%d) Outside image and border",
			x, y);
		return 0;
	}
}

FLOAT32 kC_GetPixelIm(int x, int y,const TComplexImage* pInfo)
{
	UINT16 border=pInfo->BorderSize;
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if ((x>=-border) && (x<(int) pInfo->Width+border) &&
		(y>=-border) && (y<(int) pInfo->Height+border))
	{
		return kC_GetPixelMacroIm(x,y,(*pInfo));
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"kC_GetPixel(%d,%d) Outside image and border",
			x, y);
		return 0;
	}
}

FLOAT32 kC_GetPixelFastRe(int x, int y,const TComplexImage* pInfo)
{
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	return kC_GetPixelMacroRe(x,y,(*pInfo));
}

FLOAT32 kC_GetPixelFastIm(int x, int y,const TComplexImage* pInfo)
{
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	return kC_GetPixelMacroIm(x,y,(*pInfo));
}

bool kC_SetPixel(int x, int y, FLOAT32 Re, FLOAT32 Im,TComplexImage* pInfo)
{
	UINT16 border=pInfo->BorderSize;
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;
	
	if((x>=-border) && (x<(int) pInfo->Width+border) &&
		(y>=-border) && (y<(int) pInfo->Height+border))
	{
		kC_SetPixelMacroRe(x,y,Re,(*pInfo));
		kC_SetPixelMacroIm(x,y,Im,(*pInfo));
		return true;
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"kC_SetPixel(%d,%d) Outside image and border",
			x, y);
		return false;
	}
}

bool kC_SetPixelFast(int x, int y, FLOAT32 Re, FLOAT32 Im,TComplexImage* pInfo)
{
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;
	kC_SetPixelMacroRe(x,y,Re,(*pInfo));
	kC_SetPixelMacroIm(x,y,Im,(*pInfo));
	return true;
}

bool kC_SetPixelRe(int x, int y, FLOAT32 Re,TComplexImage *pInfo)
{
	UINT16 border=pInfo->BorderSize;
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if((x>=-border) && (x<(int) pInfo->Width+border) &&
		(y>=-border) && (y<(int) pInfo->Height+border))
	{
		kC_SetPixelMacroRe(x,y,Re,(*pInfo));
		return true;
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"kC_SetPixelRe(%d,%d) Outside image and border",
			x, y);
		return false;
	}
}

bool kC_SetPixelFastRe(int x, int y, FLOAT32 Re,TComplexImage *pInfo)
{
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	kC_SetPixelMacroRe(x,y,Re,(*pInfo));
	return true;
}

bool kC_SetPixelIm(int x, int y, FLOAT32 Im,TComplexImage *pInfo)
{
	UINT16 border=pInfo->BorderSize;
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	if((x>=-border) && (x<(int) pInfo->Width+border) &&
		(y>=-border) && (y<(int) pInfo->Height+border))
	{
		kC_SetPixelMacroIm(x,y,Im,(*pInfo));
		return true;
	}
	else
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"kC_SetPixelIm(%d,%d) Outside image and border",
			x, y);
		return false;
	}
}

bool kC_SetPixelFastIm(int x, int y, FLOAT32 Im,TComplexImage *pInfo)
{
	/* adjust origo offset */
	x += pInfo->Origo.x;
	y += pInfo->Origo.y;

	kC_SetPixelMacroIm(x,y,Im,(*pInfo));
	return true;
}

/************************************************/
/********        Private functions       ********/
/************************************************/

void kC_SetImageMatrix(TComplexImage* pInfo)
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
	UINT32Value=kC_GetSizeScanLine(realX)/sizeof(FLOAT32);
	/*printf("totalHeight=%d\n",totalHeight);*/
	if (pInfo->ppAllScanLinesRe!=NULL)
	{
		free(pInfo->ppAllScanLinesRe);
	}
	if (pInfo->ppAllScanLinesIm!=NULL)
	{
		free(pInfo->ppAllScanLinesIm);
	}
	
	(pInfo->ppAllScanLinesRe) = (FLOAT32**) malloc(sizeof(FLOAT32*)*(totalHeight));
	pInfo->ppMatrixRe = &((pInfo->ppAllScanLinesRe)[border]);
	(pInfo->ppAllScanLinesIm) = (FLOAT32**) malloc(sizeof(FLOAT32*)*(totalHeight));
	pInfo->ppMatrixIm = &((pInfo->ppAllScanLinesIm)[border]);
	
	for(y=0;y<totalHeight;y++)
	{
		(pInfo->ppAllScanLinesRe)[totalHeight-1-y]=(pInfo->pPixelStreamRe+y*UINT32Value+border);
		(pInfo->ppAllScanLinesIm)[totalHeight-1-y]=(pInfo->pPixelStreamIm+y*UINT32Value+border);
	}
}

UINT32 kC_GetSizeScanLine(UINT32 Width)
{
	/* returns the size of a scanline, no zero padding in a float image */
	return Width*sizeof(FLOAT32);
}

bool kC_LoadComplex(FILE* fp,TComplexImage* pInfo)
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
					"kC_LoadFloat() wrong header - only found a history - no data");
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
				k_ShowMessageSimple(IPL_ERROR, "kC_LoadComplex() wrong file format, end of file reached");
				return false;
			}
			else if (strcmp(Token,"WidthHeight")==0)
			{
				/* The following two values must be integers */
				if (fscanf(fp,"%d %d",&Width,&Height)!=2)
				{
					k_ShowMessageSimple(
						IPL_ERROR,
						"kC_LoadComplex() wrong header - "
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
							"kC_LoadComplex() Width=Cols (%d=%d) and height=Rows (%d,%d) "
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
					k_ShowMessageSimple(
						IPL_ERROR,
						"kC_LoadComplex() wrong header - "
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
							"kC_LoadComplex() Width=Cols (%d=%d) and height=Rows (%d,%d) "
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
		k_ShowMessageSimple(IPL_ERROR, "kC_LoadComplex() wrong header - width and height not found");
		return false;
	}
	/* allocate image */
	kC_AllocImage(Width,Height,pInfo);
	/* Read data */
	/* read the real values */
	for(y=0;y<Height;y++)
	{
		for(x=0;x<Width;x++)
		{
			if (k_SkipWhiteSpace(fp)!=-1)
			{
				if (fscanf(fp,"%e",&Value)==1)
				{
					kC_SetPixelRe(x,y,Value,pInfo);
				}
				else
				{
					k_ShowMessageSimple(IPL_ERROR, "kC_LoadComplex() Failed reading real values");
					kC_EmptyImage(pInfo);
					return false;
				}
			}
			else
			{
				k_ShowMessageSimple(IPL_ERROR, "kC_LoadComplex() Failed reading real values");
				kC_EmptyImage(pInfo);
				return false;
			}
		}
	}

	/* read the imaginar values */
	for(y=0;y<Height;y++)
	{
		for(x=0;x<Width;x++)
		{
			if (k_SkipWhiteSpace(fp)!=-1)
			{
				if (fscanf(fp,"%e",&Value)==1)
				{
					kC_SetPixelIm(x,y,Value,pInfo);
				}
				else
				{
					k_ShowMessageSimple(IPL_ERROR, "kC_LoadComplex() Failed reading imaginar values");
					kC_EmptyImage(pInfo);
					return false;
				}
			}
			else
			{
				k_ShowMessageSimple(IPL_ERROR, "kC_LoadComplex() Failed reading imaginar values");
				kC_EmptyImage(pInfo);
				return false;
			}
		}
	}
	/* copy history to pInfo */
	k_CopyText(&(pInfo->History),&TempHistory);
	k_EmptyText(&TempHistory);
	return true;
}

bool kC_SaveComplex(FILE* fp,TComplexImage* pInfo)
{
	unsigned int x,y;
	char* pHistory=NULL;
	if (pInfo->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "kC_SaveComplex() No image to save");
		fclose(fp);
		return false;
	}
	k_ListToCharArrayText(&(pInfo->History),&pHistory);
	fprintf(fp,"%s\n",pHistory);
	if (pHistory!=NULL)
	{
		free(pHistory);
	}
	fprintf(fp,"## WidthHeight %d %d\n",(int)pInfo->Width,(int)pInfo->Height);
	fprintf(fp,"## RowsCols %d %d\n\n",(int)pInfo->Height,(int)pInfo->Width);
	for(y=0;y<pInfo->Height;y++)
	{
		for(x=0;x<pInfo->Width;x++)
		{
			fprintf(fp,"% e\t",kC_GetPixelMacroRe(x,y,(*pInfo)));
		}
		fprintf(fp,"\n");
	}
	for(y=0;y<pInfo->Height;y++)
	{
		for(x=0;x<pInfo->Width;x++)
		{
			fprintf(fp,"% e\t",kC_GetPixelMacroIm(x,y,(*pInfo)));
		}
		fprintf(fp,"\n");
	}
	return true;
}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
