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

#include "kernel_morphology.h"
#include "../kernel_error.h"
#include "../kernel_pixelgroup.h"
#include "../kernel_history.h"
#include <memory.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

bool k_DilateFast(TImage* pImg,unsigned short q10,unsigned short q20,unsigned short q30)
{
	
	TImage Img1; /* Buffer image */
	unsigned int j,y,Carrier;
	int i;
	unsigned short q1,q2,q3,p1,p2,p3,Result;
	unsigned char *Row1,*Row2,*Row3;
	UINT8* addr;
	if (pImg->Bits!=1)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_DilateFast() Image must be 1 b/p (current is %d b/p)",
			pImg->Bits);
		return false;
	}
	if (pImg->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_DilateFast() Source image is empty - doing nothing");
		return false;
	}
    k_InitImage(&Img1); /* always initialise the TImage structure! */
	k_CopyImage(&Img1,pImg); 
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(pImg->History),
			"%sk_DilateFast(q10=%d,q20=%d,q30=%d)",
			ipl_HistoryIndent, q10, q20, q30);
		k_IplStartHistoryMacro();
	}
#endif
	Row1=(UINT8*)calloc(Img1.ByteWidth,sizeof(UINT8));
	Row2=(UINT8*)calloc(Img1.ByteWidth,sizeof(UINT8));
	Row3=(UINT8*)calloc(Img1.ByteWidth,sizeof(UINT8));
	for (y=0;y<Img1.Height;y++)
	{
		for(i=0;i<(int)Img1.ByteWidth;i++)
		{
			addr=(Img1.pPixelStream+y*Img1.ByteWidth+i);
			*addr=(unsigned char)(~(*addr));
		}
	}
	memset(Row2,255,Img1.ByteWidth);
	memcpy(Row3,Img1.pPixelStream,Img1.ByteWidth);
	for (y=0;y<Img1.Height;y++)
	{
		Carrier=1;
		memcpy(Row1,Row2,Img1.ByteWidth);
		memcpy(Row2,Row3,Img1.ByteWidth);
		if (y==Img1.Height-1)
			memset(Row3,255,Img1.ByteWidth);
		else
			memcpy(Row3,(Img1.pPixelStream+(y+1)*Img1.ByteWidth),Img1.ByteWidth);
		for (i=Img1.ByteWidth-1;i>=0;i--)
		{
			if (i==0)
			{
				p1=(unsigned short)(Row1[i]+0xFF00);
				p2=(unsigned short)(Row2[i]+0xFF00);
				p3=(unsigned short)(Row3[i]+0xFF00);
			}
			else
			{
				p1=(unsigned short)(Row1[i]+(Row1[i-1]<<8));
				p2=(unsigned short)(Row2[i]+(Row2[i-1]<<8));
				p3=(unsigned short)(Row3[i]+((Row3[i-1])<<8));
			}
			q1=q10;
			q2=q20;
			q3=q30;
			Result=(unsigned short)Carrier;
			for (j=1;j<8;j++)
			{
				if (((q1&p1)==q1) && ((q2&p2)==q2) && ((q3&p3)==q3))
					Result+=(1<<j);
				q1<<=1;
				q2<<=1;
				q3<<=1;
			}
			if (((q1&p1)==q1) && ((q2&p2)==q2) && ((q3&p3)==q3))
				Carrier=1;
			else
				Carrier=0;
			*(pImg->pPixelStream+y*Img1.ByteWidth+i)=(unsigned char)(~Result);
		}
	}
	free(Row1);
	free(Row2);
	free(Row3);
	k_EmptyImage(&Img1);

#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
	return true;
}

bool k_ErodeFast(TImage *pImg,unsigned short q10,unsigned short q20,unsigned short q30)
{
	/* Similar to k_DilateFast() */
	TImage Img1;
	unsigned int j,y,Carrier;
	int i;
	unsigned short q1,q2,q3,p1,p2,p3,Result;
	unsigned char *Row1,*Row2,*Row3;
	if (pImg->Bits!=1)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_ErodeFast() Image must be 1 b/p (current is %d b/p)",
			pImg->Bits);
		return false;
	}
	if (pImg->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_ErodeFast() Source image is empty - doing nothing");
		return false;
	}
    k_InitImage(&Img1); /* always initialise the TImage structure! */
	k_CopyImage(&Img1,pImg); 
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(pImg->History),
			"%sk_ErodeFast(q10=%d,q20=%d,q30=%d)",
			ipl_HistoryIndent, q10, q20, q30);
		k_IplStartHistoryMacro();
	}
#endif
	Row1=(UINT8*)calloc(Img1.ByteWidth,sizeof(UINT8));
	Row2=(UINT8*)calloc(Img1.ByteWidth,sizeof(UINT8));
	Row3=(UINT8*)calloc(Img1.ByteWidth,sizeof(UINT8));
	memset(Row2,0,Img1.ByteWidth);
	memcpy(Row3,Img1.pPixelStream,Img1.ByteWidth);
	for (y=0;y<Img1.Height;y++)
	{
		Carrier=0;
		memcpy(Row1,Row2,Img1.ByteWidth);
		memcpy(Row2,Row3,Img1.ByteWidth);
		if (y==Img1.Height-1)
			memset(Row3,0,Img1.ByteWidth);
		else
			memcpy(Row3,(Img1.pPixelStream+(y+1)*Img1.ByteWidth),Img1.ByteWidth);
		for (i=Img1.ByteWidth-1;i>=0;i--)
		{
			if (i==0)
			{
				p1=Row1[i];
				p2=Row2[i];
				p3=Row3[i];
			}
			else
			{
				p1=(unsigned short)(Row1[i]+(Row1[i-1]<<8));
				p2=(unsigned short)(Row2[i]+(Row2[i-1]<<8));
				p3=(unsigned short)(Row3[i]+(Row3[i-1]<<8));
			}
			q1=q10;
			q2=q20;
			q3=q30;
			Result=(unsigned short)Carrier;
			for (j=1;j<8;j++)
			{
				if (((q1&p1)==q1)&&((q2&p2)==q2)&&((q3&p3)==q3))
					Result+=(1<<j);
				q1<<=1;
				q2<<=1;
				q3<<=1;
			}
			if(((q1&p1)==q1)&&((q2&p2)==q2)&&((q3&p3)==q3))
				Carrier=1;
			else 
				Carrier=0;
			*(pImg->pPixelStream+y*Img1.ByteWidth+i)=(UINT8)Result;
		}
	}
	free(Row1);
	free(Row2);
	free(Row3);
	k_EmptyImage(&Img1);

#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
	return true;
}


bool k_Erode(const TImage *pMask, TImage *pImg)
{ 
	int w,h,w2,h2,x,y,i,j;
	unsigned short Border,White;
	bool OK;
	TImage Img1;
	if (pImg->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_Erode() Source/target image is empty - doing nothing");
		return false;
	}
	if (pMask->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_Erode() Mask image is empty - doing nothing");
		return false;
	}
	if (pImg->Bits==8)
		White=255;
	else if (pImg->Bits==1)
		White=1;
	else
	{
		k_ShowMessageSimple(IPL_ERROR, "k_Erode() Source/target image must be 1 or 8 b/p");
		return false;
	}
	w=pMask->Width;
	h=pMask->Height;
	w2=w/2;
	h2=h/2;
	if (h2>w2) 
		Border=(unsigned short)h2;
	else
		Border=(unsigned short)w2;
    k_InitImage(&Img1); /* always initialise the TImage structure! */
	k_CopyImage(&Img1,pImg); 
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(pImg->History),
			"%sk_Erode() with dimensions (%d*%d)",
			ipl_HistoryIndent, pMask->Width, pMask->Height);
		k_IplStartHistoryMacro();
	}
#endif
	Border++;
	k_SetBorder(Border,0,&Img1);
	if (pImg->Bits==1)
	{
		for (x=k_GetMinX(&Img1);x<k_GetMaxX(&Img1);x++)
		{
			for (y=k_GetMinY(&Img1);y<k_GetMaxY(&Img1);y++)
			{ 
				OK=true;
				i=0;
				while ((i<w) && OK)
				{
					j=0;
					while ((j<h) && OK)
					{
						if (k_GetPixelFast(i,j,pMask))
							/*OK=(OK && (k_GetPixel1bp(x+i-w2,y+j-h2,Img1)>0));*/
							OK=(OK && (k_GetPixel1bp(x+i-w2,y+j-h2,Img1)));
						j++;
					}
					i++;
				}
				if (OK)
					k_SetPixel1bp(x,y,White,*pImg);
				else
					k_SetPixel1bp(x,y,0,*pImg);
			}
		}
	}
	else /* then it is 8 b/p */
	{
		for (x=k_GetMinX(&Img1);x<k_GetMaxX(&Img1);x++)
		{
			for (y=k_GetMinY(&Img1);y<k_GetMaxY(&Img1);y++)
			{ 
				OK=true;
				i=0;
				while ((i<w) && OK)
				{
					j=0;
					while ((j<h) && OK)
					{
						if (k_GetPixelFast(i,j,pMask))
							OK=(OK && (k_GetPixel8bp(x+i-w2,y+j-h2,Img1)>0));
						j++;
					}
					i++;
				}
				if (OK)
				{
					k_SetPixel8bp(x,y,White,*pImg);
				}
				else
					k_SetPixel8bp(x,y,0,*pImg);
			}
		}
	}
	k_EmptyImage(&Img1);

#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
	return true;
}

bool k_Dilate(const TImage *pMask, TImage *pImg)
{ /* Similar to Erode */
	int w,h,w2,h2,x,y,i,j;
	unsigned short Border, White;
	bool OK;
	TImage Img1;
	if (pImg->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_Dilate() Source/target image is empty - doing nothing");
		return false;
	}
	if (pMask->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_Dilate() Mask image is empty - doing nothing");
		return false;
	}
	if (pImg->Bits==8)
		White=255;
	else if (pImg->Bits==1)
		White=1;
	else
	{
		k_ShowMessageSimple(IPL_ERROR, "k_Dilate() Source/target image must be 1 or 8 b/p");
		return false;
	}

	w=pMask->Width;
	h=pMask->Height;
	w2=w/2;
	h2=h/2;
	if (h2>w2) 
		Border=(unsigned short)h2; 
	else 
		Border=(unsigned short)w2;
    k_InitImage(&Img1); /* always initialise the TImage structure! */
	k_CopyImage(&Img1,pImg); 
#ifdef IPL_ADD_HISTORY
	/* add comment to history */
	{
		k_PrintfAppendTextIPL(
			&(pImg->History),
			"%sk_Dilate() with dimensions (%d*%d)",
			ipl_HistoryIndent, pMask->Width, pMask->Height);
		k_IplStartHistoryMacro();
	}
#endif
	Border++;
	k_SetBorder(Border,0,&Img1);
	if (pImg->Bits==1)
	{
		for (x=k_GetMinX(&Img1);x<k_GetMaxX(&Img1);x++)
		{
			for (y=k_GetMinY(&Img1);y<k_GetMaxY(&Img1);y++)
			{ 
				OK=false;
				for(i=0;i<w;i++)
				{
					for(j=0;j<h;j++)
					{
						if (k_GetPixelFast(i,j,pMask))
							OK=(OK || k_GetPixel1bp(x+i-w2,y+j-h2,Img1));
					}
					if (OK)
						k_SetPixel1bp(x,y,White,*pImg);
					else
						k_SetPixel1bp(x,y,0,*pImg);
				}
			}
		}
	}
	else /* then it is 8 b/p */
	{
		for (x=k_GetMinX(&Img1);x<k_GetMaxX(&Img1);x++)
		{
			for (y=k_GetMinY(&Img1);y<k_GetMaxY(&Img1);y++)
			{ 
				OK=false;
				for(i=0;i<w;i++)
				{
					for(j=0;j<h;j++)
					{
						if (k_GetPixelFast(i,j,pMask))
							OK=(OK || k_GetPixel8bp(x+i-w2,y+j-h2,Img1));
					}
					if (OK)
					{
						k_SetPixel8bp(x,y,White,*pImg);
					}
					else
						k_SetPixel8bp(x,y,0,*pImg);
				}
			}
		}
	}
	k_EmptyImage (&Img1);

#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
	return true;
}

/******************************************************************/
/********** Private functions for the thinning algorithm **********/
/******************************************************************/

bool k_ThinningStep2cdTests(UINT8 m_Matrix22[][3]);
bool k_ThinningStep1cdTests(UINT8 m_Matrix22[][3]);
bool k_ThinningCheckTransitions(UINT8 m_Matrix22[][3]);
bool k_ThinningSearchNeighbors(int x, int y,const TImage* pImg, UINT8 m_Matrix22[][3]);


int k_Thinning(TImage* pImg)
{
	if (pImg->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_Thinning() Image is empty - doing nothing");
		return -1;
	}
	else if (pImg->Bits!=1)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_WARNING,
			"k_Thinning() Image is %d b/p, must be 1 b/p",
			pImg->Bits);
		return -1;
	}
	else
	{
		bool PointsRemoved;
		UINT8 m_Matrix22[3][3];
		int Iter=0;
		int prem1,prem2;
		int iCountX, iCountY;
		T2DInt tempPoint;
		int pntinpic=0;
		TPixelGroup PointList;
		TPixelGroup RemoveList;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%sk_Thinning()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		k_InitGroup(&PointList);
		k_InitGroup(&RemoveList);
		k_SetBorder(1,1,pImg);
		PointsRemoved=false;
		Iter++;
		/* step 1 Collecting the Black point in a list */
		prem1=prem2=0;
		for(iCountY=k_GetMinY(pImg);iCountY<k_GetMaxY(pImg);iCountY++)
		{
			for(iCountX=k_GetMinX(pImg);iCountX<k_GetMaxX(pImg);iCountX++)
			{
				if (k_GetPixel1bp(iCountX,iCountY,*pImg)==0) /* if pixel is black */
				{
					tempPoint.x=iCountX;
					tempPoint.y=iCountY;
					pntinpic++;
					if (k_ThinningSearchNeighbors(iCountX,iCountY,pImg,&m_Matrix22[0]) &&
						k_ThinningCheckTransitions(&m_Matrix22[0]) &&
						k_ThinningStep1cdTests(&m_Matrix22[0]))
					{
						prem1++;
						PointsRemoved=true;
						k_Add2DPosToGroup(tempPoint,&RemoveList);
					}
					else
					{
						k_Add2DPosToGroup(tempPoint,&PointList);
					}
				}
			}
		}
		printf("Total black points: %d\n",pntinpic);
		/* Set all pixels positions in RemoveList in image to white */
		for(iCountX=0; iCountX<(int)RemoveList.NumberOfPixels; iCountX++)
		{
			k_SetPixel1bp(RemoveList.pPos[iCountX].x,RemoveList.pPos[iCountX].y,1,*pImg);
		}
		k_EmptyGroup(&RemoveList);

		/* step 2 after step 1 which inserted points in list */
		if (PointsRemoved)
		{
			for(iCountX=0; iCountX<(int)PointList.NumberOfPixels; iCountX++)
			{
				tempPoint=PointList.pPos[iCountX];
				if (k_ThinningSearchNeighbors(tempPoint.x,tempPoint.y,pImg,&m_Matrix22[0]) &&
					k_ThinningCheckTransitions(&m_Matrix22[0]) &&
					k_ThinningStep2cdTests(&m_Matrix22[0]))
				{
					prem2++;
					PointsRemoved=true;
					/*k_RemovePosFromGroupSlow(iCountX,&PointList);*/
					k_RemovePosFromGroup(iCountX,&PointList);
					k_Add2DPosToGroup(tempPoint,&RemoveList);
					iCountX--; /* Must decrease iCountX when a point has been removed */
				}
			}
		}

		/* Set all pixels positions in RemoveList in image to white */
		for(iCountX=0; iCountX<(int)RemoveList.NumberOfPixels; iCountX++)
		{
			k_SetPixel1bp(RemoveList.pPos[iCountX].x,RemoveList.pPos[iCountX].y,1,*pImg);
		}
		k_EmptyGroup(&RemoveList);
		printf("Iteration %d: Points removed: %d+%d=%d\n",Iter,prem1,prem2,prem1+prem2);
		
		/* step 1 */
		while(PointsRemoved)
		{
			prem1=prem2=0;
			Iter++;
			PointsRemoved=false;
			for(iCountX=0; iCountX<(int)PointList.NumberOfPixels; iCountX++)
			{
				tempPoint=PointList.pPos[iCountX];
				if ((k_ThinningSearchNeighbors(tempPoint.x,tempPoint.y,pImg,&m_Matrix22[0])) &&
					(k_ThinningCheckTransitions(&m_Matrix22[0])) &&
					(k_ThinningStep1cdTests(&m_Matrix22[0])))
				{
					prem1++;
					PointsRemoved=true;
					/*k_RemovePosFromGroupSlow(iCountX,&PointList);*/
					k_RemovePosFromGroup(iCountX,&PointList);
					k_Add2DPosToGroup(tempPoint,&RemoveList);
					iCountX--; /* Must decrease iCountX when a point has been removed */
				}
			}
			/* Set all pixels positions in RemoveList in image to white */
			for(iCountX=0; iCountX<(int)RemoveList.NumberOfPixels; iCountX++)
			{
				k_SetPixel1bp(RemoveList.pPos[iCountX].x,RemoveList.pPos[iCountX].y,1,*pImg);
			}
			k_EmptyGroup(&RemoveList);

			/* step 2 */
			for(iCountX=0; iCountX<(int)PointList.NumberOfPixels; iCountX++)
			{
				tempPoint=PointList.pPos[iCountX];
				if (k_ThinningSearchNeighbors(tempPoint.x,tempPoint.y,pImg,&m_Matrix22[0]) &&
					k_ThinningCheckTransitions(&m_Matrix22[0]) &&
					k_ThinningStep2cdTests(&m_Matrix22[0]))
				{
					prem2++;
					PointsRemoved=true;
					/*k_RemovePosFromGroupSlow(iCountX,&PointList);*/
					k_RemovePosFromGroup(iCountX,&PointList);
					k_Add2DPosToGroup(tempPoint,&RemoveList);
					iCountX--; /* Must decrease iCountX when a point has been removed */
				}
			}
			
			/* Set all pixels positions in RemoveList in image to white */
			for(iCountX=0; iCountX<(int)RemoveList.NumberOfPixels; iCountX++)
			{
				k_SetPixel1bp(RemoveList.pPos[iCountX].x,RemoveList.pPos[iCountX].y,1,*pImg);
			}
			k_EmptyGroup(&RemoveList);
			printf("Iteration %d: Points removed: %d+%d=%d\n",Iter,prem1,prem2,prem1+prem2);
		}

#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		k_EmptyGroup(&PointList);
		return Iter;
	}
}

/* performes the tests (c') and (d') in step 2 as explained in Gonzales and Woods page 493 */
bool k_ThinningStep2cdTests(UINT8 m_Matrix22[][3])
{
	if ((m_Matrix22[1][0]+m_Matrix22[2][1]+m_Matrix22[0][1]) &&
		(m_Matrix22[1][0]+m_Matrix22[1][2]+m_Matrix22[0][1]))
		return true;
	else
		return false;
}


/* performes the tests (c) and (d) in step 1 as explained in Gonzales and Woods page 492 */
bool k_ThinningStep1cdTests(UINT8 m_Matrix22[][3])
{
	if ((m_Matrix22[1][0]+m_Matrix22[2][1]+m_Matrix22[1][2]) &&
		(m_Matrix22[2][1]+m_Matrix22[1][2]+m_Matrix22[0][1]))
		return true;
	else
		return false;
}

/* returns true if there is exactly one transition
	in the region around the actual pixel */
bool k_ThinningCheckTransitions(UINT8 m_Matrix22[][3])
{
	int iTransitions=0;
	if ((m_Matrix22[0][0]==1) && (m_Matrix22[1][0]==0)){
		++iTransitions;}
	if ((m_Matrix22[1][0]==1) && (m_Matrix22[2][0]==0)){
		++iTransitions;}
	if ((m_Matrix22[2][0]==1) && (m_Matrix22[2][1]==0)){
		++iTransitions;}
	if ((m_Matrix22[2][1]==1) && (m_Matrix22[2][2]==0)){
		++iTransitions;}
	if ((m_Matrix22[2][2]==1) && (m_Matrix22[1][2]==0)){
		++iTransitions;}
	if ((m_Matrix22[1][2]==1) && (m_Matrix22[0][2]==0)){
		++iTransitions;}
	if ((m_Matrix22[0][2]==1) && (m_Matrix22[0][1]==0)){
		++iTransitions;}
	if ((m_Matrix22[0][1]==1) && (m_Matrix22[0][0]==0)){
		++iTransitions;}
	if (iTransitions==1)
		return true;
	else
		return false;
}


bool k_ThinningSearchNeighbors(int x, int y,const TImage* pImg, UINT8 m_Matrix22[][3])
/* As (a) in Gonzales and Woods, between 2 and 6 black neighbors */
{
	unsigned short BlackNeighbor=0;
	if ((m_Matrix22[0][0]=k_GetPixel1bp(x-1,y-1,*pImg)) == 0){++BlackNeighbor;}
	if ((m_Matrix22[1][0]=k_GetPixel1bp(x  ,y-1,*pImg)) == 0){++BlackNeighbor;}
	if ((m_Matrix22[2][0]=k_GetPixel1bp(x+1,y-1,*pImg)) == 0){++BlackNeighbor;}
	if ((m_Matrix22[0][1]=k_GetPixel1bp(x-1,y  ,*pImg)) == 0){++BlackNeighbor;}
	if ((m_Matrix22[2][1]=k_GetPixel1bp(x+1,y  ,*pImg)) == 0){++BlackNeighbor;}
	if ((m_Matrix22[0][2]=k_GetPixel1bp(x-1,y+1,*pImg)) == 0){++BlackNeighbor;}
	if ((m_Matrix22[1][2]=k_GetPixel1bp(x  ,y+1,*pImg)) == 0){++BlackNeighbor;}
	if ((m_Matrix22[2][2]=k_GetPixel1bp(x+1,y+1,*pImg)) == 0){++BlackNeighbor;}

	if ((BlackNeighbor>=2) && (BlackNeighbor<=6))
		return true;
	else
		return false;
}

/******************************************************************/
/******* Private functions for the skeletonizing algorithm ********/
/******************************************************************/
bool k_SkeletonCheckB1(T2DInt pnt,TImage* pImg);
bool k_SkeletonCheckB2(T2DInt pnt,TImage* pImg);
bool k_SkeletonCheckB3(T2DInt pnt,TImage* pImg);
bool k_SkeletonCheckB4(T2DInt pnt,TImage* pImg);
bool k_SkeletonCheckB5(T2DInt pnt,TImage* pImg);
bool k_SkeletonCheckB6(T2DInt pnt,TImage* pImg);
bool k_SkeletonCheckB7(T2DInt pnt,TImage* pImg);
bool k_SkeletonCheckB8(T2DInt pnt,TImage* pImg);

int k_Skeletonizing(TImage* pImg)
{
	if (pImg->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_Skeletonizing() Image is empty - doing nothing");
		return -1;
	}
	else if (pImg->Bits!=1)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_WARNING,
			"k_Skeletonizing() Image is %d b/p, must be 1 b/p",
			pImg->Bits);
		return -1;
	}
	else /* perform skeletonizing */
	{
		TPixelGroup PointList;
		TPixelGroup RemoveList;
		int iCountX, iCountY;
		T2DInt tempPoint;
		int NoPointsRemoved=0;
		int Iter=0;
		int prem;
		int pntinpic=0;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%sk_Skeletonizing()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		k_SetBorder(1,1,pImg);
		k_InitGroup(&PointList);
		k_InitGroup(&RemoveList);
		/* Collecting black points */
		for(iCountY=k_GetMinY(pImg);iCountY<k_GetMaxY(pImg);iCountY++)
		{
			for(iCountX=k_GetMinX(pImg);iCountX<k_GetMaxX(pImg);iCountX++)
			{
				tempPoint.x=iCountX;
				tempPoint.y=iCountY;
				if ((k_GetPixel1bp(iCountX,iCountY,*pImg))==0)
				{
					pntinpic++;
					k_Add2DPosToGroup(tempPoint,&PointList);
				}
			}
		}
		printf("Black points in picture=%d\n",pntinpic);

		/* All 8 patterns have to remove 0 points before leaving */
		while (true)
		{
			/*printf("Iteration %d\n",++Iter);*/
			/* Testing picture with pattern B1 */
			prem=0;
			if (PointList.NumberOfPixels==0) break;
			for(iCountX=0; iCountX<(int)PointList.NumberOfPixels; iCountX++)
			{
				tempPoint=PointList.pPos[iCountX];
				if (k_SkeletonCheckB1(tempPoint,pImg))
				{
					prem++;
					k_RemovePosFromGroup(iCountX,&PointList);
					k_Add2DPosToGroup(tempPoint,&RemoveList);
					iCountX--; /* Must decrease iCountX when a point has been removed */
				}
			}
			/* Set all pixels positions in RemoveList in image to white */
			for(iCountX=0; iCountX<(int)RemoveList.NumberOfPixels; iCountX++)
				k_SetPixel1bp(RemoveList.pPos[iCountX].x,RemoveList.pPos[iCountX].y,1,*pImg);

			k_EmptyGroup(&RemoveList);
			/*printf("  Result of B1 check: removed points=%d\n",prem);*/

			if (prem==0)
				NoPointsRemoved++;
			else
				NoPointsRemoved=0;

			if (NoPointsRemoved>=8) break;
			
			/* Testing picture with pattern B2 */
			prem=0;
			if (PointList.NumberOfPixels==0) break;
			for(iCountX=0; iCountX<(int)PointList.NumberOfPixels; iCountX++)
			{
				tempPoint=PointList.pPos[iCountX];
				if (k_SkeletonCheckB2(tempPoint,pImg))
				{
					prem++;
					k_RemovePosFromGroup(iCountX,&PointList);
					k_Add2DPosToGroup(tempPoint,&RemoveList);
					iCountX--; /* Must decrease iCountX when a point has been removed */
				}
			}
			/* Set all pixels positions in RemoveList in image to white */
			for(iCountX=0; iCountX<(int)RemoveList.NumberOfPixels; iCountX++)
				k_SetPixel1bp(RemoveList.pPos[iCountX].x,RemoveList.pPos[iCountX].y,1,*pImg);

			k_EmptyGroup(&RemoveList);
			/*printf("  Result of B2 check: removed points=%d\n",prem);*/

			if (prem==0)
				NoPointsRemoved++;
			else
				NoPointsRemoved=0;

			if (NoPointsRemoved>=8) break;
			
			/* Testing picture with pattern B3 */
			prem=0;
			if (PointList.NumberOfPixels==0) break;
			for(iCountX=0; iCountX<(int)PointList.NumberOfPixels; iCountX++)
			{
				tempPoint=PointList.pPos[iCountX];
				if (k_SkeletonCheckB3(tempPoint,pImg))
				{
					prem++;
					k_RemovePosFromGroup(iCountX,&PointList);
					k_Add2DPosToGroup(tempPoint,&RemoveList);
					iCountX--; /* Must decrease iCountX when a point has been removed */
				}
			}
			/* Set all pixels positions in RemoveList in image to white */
			for(iCountX=0; iCountX<(int)RemoveList.NumberOfPixels; iCountX++)
				k_SetPixel1bp(RemoveList.pPos[iCountX].x,RemoveList.pPos[iCountX].y,1,*pImg);

			k_EmptyGroup(&RemoveList);
			/*printf("  Result of B3 check: removed points=%d\n",prem);*/

			if (prem==0)
				NoPointsRemoved++;
			else
				NoPointsRemoved=0;

			if (NoPointsRemoved>=8) break;

			/* Testing picture with pattern B4 */
			prem=0;
			if (PointList.NumberOfPixels==0) break;
			for(iCountX=0; iCountX<(int)PointList.NumberOfPixels; iCountX++)
			{
				tempPoint=PointList.pPos[iCountX];
				if (k_SkeletonCheckB4(tempPoint,pImg))
				{
					prem++;
					k_RemovePosFromGroup(iCountX,&PointList);
					k_Add2DPosToGroup(tempPoint,&RemoveList);
					iCountX--; /* Must decrease iCountX when a point has been removed */
				}
			}
			/* Set all pixels positions in RemoveList in image to white */
			for(iCountX=0; iCountX<(int)RemoveList.NumberOfPixels; iCountX++)
				k_SetPixel1bp(RemoveList.pPos[iCountX].x,RemoveList.pPos[iCountX].y,1,*pImg);

			k_EmptyGroup(&RemoveList);
			/*printf("  Result of B4 check: removed points=%d\n",prem);*/
			if (prem==0)
				NoPointsRemoved++;
			else
				NoPointsRemoved=0;

			if (NoPointsRemoved>=8) break;

			/* Testing picture with pattern B5 */
			prem=0;
			if (PointList.NumberOfPixels==0) break;
			for(iCountX=0; iCountX<(int)PointList.NumberOfPixels; iCountX++)
			{
				tempPoint=PointList.pPos[iCountX];
				if (k_SkeletonCheckB5(tempPoint,pImg))
				{
					prem++;
					k_RemovePosFromGroup(iCountX,&PointList);
					k_Add2DPosToGroup(tempPoint,&RemoveList);
					iCountX--; /* Must decrease iCountX when a point has been removed */
				}
			}
			/* Set all pixels positions in RemoveList in image to white */
			for(iCountX=0; iCountX<(int)RemoveList.NumberOfPixels; iCountX++)
				k_SetPixel1bp(RemoveList.pPos[iCountX].x,RemoveList.pPos[iCountX].y,1,*pImg);

			k_EmptyGroup(&RemoveList);
			/*printf("  Result of B5 check: removed points=%d\n",prem);*/

			if (prem==0)
				NoPointsRemoved++;
			else
				NoPointsRemoved=0;

			if (NoPointsRemoved>=8) break;

			/* Testing picture with pattern B6 */
			prem=0;
			if (PointList.NumberOfPixels==0) break;
			for(iCountX=0; iCountX<(int)PointList.NumberOfPixels; iCountX++)
			{
				tempPoint=PointList.pPos[iCountX];
				if (k_SkeletonCheckB6(tempPoint,pImg))
				{
					prem++;
					k_RemovePosFromGroup(iCountX,&PointList);
					k_Add2DPosToGroup(tempPoint,&RemoveList);
					iCountX--; /* Must decrease iCountX when a point has been removed */
				}
			}
			/* Set all pixels positions in RemoveList in image to white */
			for(iCountX=0; iCountX<(int)RemoveList.NumberOfPixels; iCountX++)
				k_SetPixel1bp(RemoveList.pPos[iCountX].x,RemoveList.pPos[iCountX].y,1,*pImg);

			k_EmptyGroup(&RemoveList);
			/*printf("  Result of B6 check: removed points=%d\n",prem);*/

			if (prem==0)
				NoPointsRemoved++;
			else
				NoPointsRemoved=0;

			if (NoPointsRemoved>=8) break;

			/* Testing picture with pattern B7 */
			prem=0;
			if (PointList.NumberOfPixels==0) break;
			for(iCountX=0; iCountX<(int)PointList.NumberOfPixels; iCountX++)
			{
				tempPoint=PointList.pPos[iCountX];
				if (k_SkeletonCheckB7(tempPoint,pImg))
				{
					prem++;
					k_RemovePosFromGroup(iCountX,&PointList);
					k_Add2DPosToGroup(tempPoint,&RemoveList);
					iCountX--; /* Must decrease iCountX when a point has been removed */
				}
			}
			/* Set all pixels positions in RemoveList in image to white */
			for(iCountX=0; iCountX<(int)RemoveList.NumberOfPixels; iCountX++)
				k_SetPixel1bp(RemoveList.pPos[iCountX].x,RemoveList.pPos[iCountX].y,1,*pImg);

			k_EmptyGroup(&RemoveList);
			/*printf("  Result of B7 check: removed points=%d\n",prem);*/

			if (prem==0)
				NoPointsRemoved++;
			else
				NoPointsRemoved=0;

			if (NoPointsRemoved>=8) break;

			/* Testing picture with pattern B8 */
			prem=0;
			if (PointList.NumberOfPixels==0) break;
			for(iCountX=0; iCountX<(int)PointList.NumberOfPixels; iCountX++)
			{
				tempPoint=PointList.pPos[iCountX];
				if (k_SkeletonCheckB8(tempPoint,pImg))
				{
					prem++;
					k_RemovePosFromGroup(iCountX,&PointList);
					k_Add2DPosToGroup(tempPoint,&RemoveList);
					iCountX--; /* Must decrease iCountX when a point has been removed */
				}
			}
			/* Set all pixels positions in RemoveList in image to white */
			for(iCountX=0; iCountX<(int)RemoveList.NumberOfPixels; iCountX++)
				k_SetPixel1bp(RemoveList.pPos[iCountX].x,RemoveList.pPos[iCountX].y,1,*pImg);

			k_EmptyGroup(&RemoveList);
			/*printf("  Result of B8 check: removed points=%d\n",prem);*/

			if (prem==0)
				NoPointsRemoved++;
			else
				NoPointsRemoved=0;

			if (NoPointsRemoved>=8) break;

		}

#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		k_EmptyGroup(&PointList);
		return Iter; /* return number of iterations */
	}
}

bool k_SkeletonCheckB1(T2DInt pnt,TImage* pImg)
{
	int x=pnt.x;
	int y=pnt.y;
	if ((k_GetPixel1bp(x-1,y-1,*pImg)==1) && (k_GetPixel1bp(x,y-1,*pImg)==1) &&
	   (k_GetPixel1bp(x+1,y-1,*pImg)==1) && (k_GetPixel1bp(x-1,y+1,*pImg)==0) &&
	   (k_GetPixel1bp(x,y+1,*pImg)==0) && (k_GetPixel1bp(x+1,y+1,*pImg)==0))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool k_SkeletonCheckB2(T2DInt pnt,TImage* pImg)
{
	int x=pnt.x;
	int y=pnt.y;
	if ((k_GetPixel1bp(x-1,y,*pImg)==0) && (k_GetPixel1bp(x-1,y+1,*pImg)==0) &&
	   (k_GetPixel1bp(x,y+1,*pImg)==0) && (k_GetPixel1bp(x,y-1,*pImg)==1) &&
	   (k_GetPixel1bp(x+1,y-1,*pImg)==1) && (k_GetPixel1bp(x+1,y,*pImg)==1))
	{
		return true;
	}
	else{
		return false;
	}
}

bool k_SkeletonCheckB3(T2DInt pnt,TImage* pImg)
{
	int x=pnt.x;
	int y=pnt.y;
	if ((k_GetPixel1bp(x+1,y-1,*pImg)==1) && (k_GetPixel1bp(x+1,y,*pImg)==1) &&
	   (k_GetPixel1bp(x+1,y+1,*pImg)==1) && (k_GetPixel1bp(x-1,y-1,*pImg)==0) &&
	   (k_GetPixel1bp(x-1,y,*pImg)==0) && (k_GetPixel1bp(x-1,y+1,*pImg)==0))
	{
		return true;
	}
	else{
		return false;
	}
}

bool k_SkeletonCheckB4(T2DInt pnt,TImage* pImg)
{
	int x=pnt.x;
	int y=pnt.y;
	if ((k_GetPixel1bp(x+1,y,*pImg)==1) && (k_GetPixel1bp(x,y+1,*pImg)==1) &&
	   (k_GetPixel1bp(x+1,y+1,*pImg)==1) && (k_GetPixel1bp(x-1,y-1,*pImg)==0) &&
	   (k_GetPixel1bp(x,y-1,*pImg)==0) && (k_GetPixel1bp(x-1,y,*pImg)==0))
	{
		return true;
	}
	else{
		return false;
	}
}

bool k_SkeletonCheckB5(T2DInt pnt,TImage* pImg)
{
	int x=pnt.x;
	int y=pnt.y;
	if ((k_GetPixel1bp(x-1,y+1,*pImg)==1) && (k_GetPixel1bp(x,y+1,*pImg)==1) &&
	   (k_GetPixel1bp(x+1,y+1,*pImg)==1) && (k_GetPixel1bp(x-1,y-1,*pImg)==0) &&
	   (k_GetPixel1bp(x,y-1,*pImg)==0) && (k_GetPixel1bp(x+1,y-1,*pImg)==0))
	{
		return true;
	}
	else{
		return false;
	}
}

bool k_SkeletonCheckB6(T2DInt pnt,TImage* pImg)
{
	int x=pnt.x;
	int y=pnt.y;
	if ((k_GetPixel1bp(x-1,y,*pImg)==1) && (k_GetPixel1bp(x-1,y+1,*pImg)==1) &&
	   (k_GetPixel1bp(x,y+1,*pImg)==1) && (k_GetPixel1bp(x,y-1,*pImg)==0) &&
	   (k_GetPixel1bp(x+1,y-1,*pImg)==0) && (k_GetPixel1bp(x+1,y,*pImg)==0))
	{
		return true;
	}
	else{
		return false;
	}
}

bool k_SkeletonCheckB7(T2DInt pnt,TImage* pImg)
{
	int x=pnt.x;
	int y=pnt.y;
	if ((k_GetPixel1bp(x-1,y-1,*pImg)==1) && (k_GetPixel1bp(x-1,y,*pImg)==1) &&
	   (k_GetPixel1bp(x-1,y+1,*pImg)==1) && (k_GetPixel1bp(x+1,y-1,*pImg)==0) &&
	   (k_GetPixel1bp(x+1,y,*pImg)==0) && (k_GetPixel1bp(x+1,y+1,*pImg)==0))
	{
		return true;
	}
	else{
		return false;
	}
}

bool k_SkeletonCheckB8(T2DInt pnt,TImage* pImg)
{
	int x=pnt.x;
	int y=pnt.y;
	if ((k_GetPixel1bp(x-1,y-1,*pImg)==1) && (k_GetPixel1bp(x,y-1,*pImg)==1) &&
	   (k_GetPixel1bp(x-1,y,*pImg)==1) && (k_GetPixel1bp(x+1,y,*pImg)==0) &&
	   (k_GetPixel1bp(x,y+1,*pImg)==0) && (k_GetPixel1bp(x+1,y+1,*pImg)==0))
	{
		return true;
	}
	else{
		return false;
	}
}

/******************************************************************/
/******* Private functions for the skeletonizing algorithm ********/
/******************************************************************/
bool k_ZhouPrevNeighboursZhou(T2DInt pnt,TImage* pImg,UINT8 m_Matrix22[][3]);
bool k_ZhouCurrNeighboursZhou(T2DInt pnt,TImage* pFlagMap,
									  bool m_FlagMtrx[][3],UINT8 m_Matrix22[][3]);
bool k_ZhouCheckTransZhou(bool m_FlagMtrx[][3],UINT8 m_Matrix22[][3],bool PreserveCorners);
bool k_ZhouCond3Zhou(bool m_FlagMtrx[][3],UINT8 m_Matrix[][3]);
bool k_ZhouCornerZhou(UINT8 m_Matrix[][3]);


int k_SkeletonZhou(TImage* pImg, bool PreserveCorners)
{
	if (pImg->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_SkeletonZhou() Image is empty - doing nothing");
		return -1;
	}
	else if (pImg->Bits!=1)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_WARNING,
			"k_SkeletonZhou() Image is %d b/p, must be 1 b/p",
			pImg->Bits);
		return -1;
	}
	else /* perform skeletonizing with the Zhou algorithm */
	{
		int iCountX, iCountY,prem1;

		T2DInt tempPoint;
		TImage FlagMap;
		UINT8 m_Matrix22[3][3]; /* Used in original image */
		bool m_FlagMtrx[3][3]; /* Used in Flag map */
		TPixelGroup PointList;
		TPixelGroup RemoveList;
		int pntinpic=0;
		int Iter=0;
		bool PointsRemoved=true;
#ifdef IPL_ADD_HISTORY
		/* add comment to history */
		{
			k_PrintfAppendTextIPL(
				&(pImg->History),
				"%sk_SkeletonZhou()",
				ipl_HistoryIndent);
			k_IplStartHistoryMacro();
		}
#endif
		k_InitImage(&FlagMap);
		k_AllocImage(pImg->Width,pImg->Height,1,&FlagMap);
		k_FlushImage(1,&FlagMap);
		k_SetBorder(1,1,&FlagMap);
		k_SetBorder(1,1,pImg);
		k_InitGroup(&PointList);
		k_InitGroup(&RemoveList);
		/* Collecting black points */
		for(iCountY=k_GetMinY(pImg);iCountY<k_GetMaxY(pImg);iCountY++)
		{
			for(iCountX=k_GetMinX(pImg);iCountX<k_GetMaxX(pImg);iCountX++)
			{
				tempPoint.x=iCountX;
				tempPoint.y=iCountY;
				if ((k_GetPixel1bp(iCountX,iCountY,*pImg))==0)
				{
					pntinpic++;
					k_Add2DPosToGroup(tempPoint,&PointList);
				}
			}
		}
		printf("Black points in picture=%d\n",pntinpic);


		while(PointsRemoved)
		{
			prem1=0;
			Iter++;
			printf("Iteration %d\n",Iter);
			PointsRemoved=false;
			for(iCountX=0; iCountX<(int)PointList.NumberOfPixels; iCountX++)
			{
				tempPoint=PointList.pPos[iCountX];
				/*TRACE("\tWorking point: (%d,%d)\n",tempPoint.x,tempPoint.y);*/
				if ((k_ZhouPrevNeighboursZhou(tempPoint,pImg,&m_Matrix22[0])) &&
				   (k_ZhouCurrNeighboursZhou(tempPoint,&FlagMap,&m_FlagMtrx[0],&m_Matrix22[0]) &&
				   (k_ZhouCheckTransZhou(&m_FlagMtrx[0],&m_Matrix22[0],PreserveCorners) || 
					k_ZhouCond3Zhou(&m_FlagMtrx[0],&m_Matrix22[0]))))
				{
					/*TRACE0("\t  Point removed!\n");*/
					prem1++;
					PointsRemoved=true;
					k_RemovePosFromGroup(iCountX,&PointList);
					k_Add2DPosToGroup(tempPoint,&RemoveList);
					iCountX--; /* Must decrease iCountX when a point has been removed */
					/*ppFlagMap[tempPoint.x][tempPoint.y]=0;*/ /* Flag point */
					k_SetPixel1bp(tempPoint.x,tempPoint.y,0,FlagMap);
				}
			}

			/* Set all pixels positions in RemoveList in image to white */
			for(iCountX=0; iCountX<(int)RemoveList.NumberOfPixels; iCountX++)
				k_SetPixel1bp(RemoveList.pPos[iCountX].x,RemoveList.pPos[iCountX].y,1,*pImg);

			k_EmptyGroup(&RemoveList);

			printf("  Iteration %d - Points removed=%d\n",Iter,prem1);
		}

		k_EmptyImage(&FlagMap);

#ifdef IPL_ADD_HISTORY
		/* end history appending (remove a leading '\t' character) */
		k_IplStopHistoryMacro();
#endif
		k_EmptyGroup(&PointList);
		return Iter;
	}
}

bool k_ZhouPrevNeighboursZhou(T2DInt pnt,TImage* pImg,UINT8 m_Matrix22[][3])
/* As PN(P) in Zhou article (PN=Previous Neighbourhood)
   returns true if pixel is a boundary pixel */
{
	int x=pnt.x;
	int y=pnt.y;
	int iBlackNeighbor=0;
	if ((m_Matrix22[0][0]=k_GetPixel1bp(x-1,y-1,*pImg)) == 0){++iBlackNeighbor;}
	if ((m_Matrix22[1][0]=k_GetPixel1bp(x  ,y-1,*pImg)) == 0){++iBlackNeighbor;}
	if ((m_Matrix22[2][0]=k_GetPixel1bp(x+1,y-1,*pImg)) == 0){++iBlackNeighbor;}
	if ((m_Matrix22[0][1]=k_GetPixel1bp(x-1,y  ,*pImg)) == 0){++iBlackNeighbor;}
	if ((m_Matrix22[2][1]=k_GetPixel1bp(x+1,y  ,*pImg)) == 0){++iBlackNeighbor;}
	if ((m_Matrix22[0][2]=k_GetPixel1bp(x-1,y+1,*pImg)) == 0){++iBlackNeighbor;}
	if ((m_Matrix22[1][2]=k_GetPixel1bp(x  ,y+1,*pImg)) == 0){++iBlackNeighbor;}
	if ((m_Matrix22[2][2]=k_GetPixel1bp(x+1,y+1,*pImg)) == 0){++iBlackNeighbor;}
	/*TRACE("\t  iPrevNeighbor=%d\n",iBlackNeighbor);*/
	if (iBlackNeighbor==8)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool k_ZhouCurrNeighboursZhou(T2DInt pnt,TImage* pFlagMap,
									  bool m_FlagMtrx[][3],UINT8 m_Matrix22[][3])
/* As CN(P) in Zhou article (CN=Current Neighbourhood) */
{
	int x=pnt.x;
	int y=pnt.y;
	int iBlackNeighbor=0;
	if (((m_FlagMtrx[0][0]=k_GetPixel1bp(x-1,y-1,*pFlagMap))==1) && (m_Matrix22[0][0]==0))
		{++iBlackNeighbor;}
	if (((m_FlagMtrx[1][0]=k_GetPixel1bp(x,y-1,*pFlagMap))==1) && (m_Matrix22[1][0]==0))
		{++iBlackNeighbor;}
	if (((m_FlagMtrx[2][0]=k_GetPixel1bp(x+1,y-1,*pFlagMap))==1) && (m_Matrix22[2][0]==0))
		{++iBlackNeighbor;}
	if (((m_FlagMtrx[0][1]=k_GetPixel1bp(x-1,y,*pFlagMap))==1) && (m_Matrix22[0][1]==0))
		{++iBlackNeighbor;}
	if (((m_FlagMtrx[2][1]=k_GetPixel1bp(x+1,y,*pFlagMap))==1) && (m_Matrix22[2][1]==0))
		{++iBlackNeighbor;}
	if (((m_FlagMtrx[0][2]=k_GetPixel1bp(x-1,y+1,*pFlagMap))==1) && (m_Matrix22[0][2]==0))
		{++iBlackNeighbor;}
	if (((m_FlagMtrx[1][2]=k_GetPixel1bp(x,y+1,*pFlagMap))==1) && (m_Matrix22[1][2]==0))
		{++iBlackNeighbor;}
	if (((m_FlagMtrx[2][2]=k_GetPixel1bp(x+1,y+1,*pFlagMap))==1) && (m_Matrix22[2][2]==0))
		{++iBlackNeighbor;}
	/*TRACE0("\tIn CurrentNeighbourhood\n"); */
	/*TRACE("\t  iCurrNeighbor=%d\n",iBlackNeighbor);*/
	if ((iBlackNeighbor>1) && (iBlackNeighbor<6))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool k_ZhouCheckTransZhou(bool m_FlagMtrx[][3],UINT8 m_Matrix22[][3],bool PreserveCorners)
/* As Trans(P0) in Zhou article (Trans= 0->1 Transitions) */
{
	int iTransitions=0;
	if (((m_Matrix22[0][0]==1) || (m_FlagMtrx[0][0]==0)) &&
		((m_Matrix22[1][0]==0) && (m_FlagMtrx[1][0]==1))){
		++iTransitions;}
	if (((m_Matrix22[1][0]==1) || (m_FlagMtrx[1][0]==0)) &&
		((m_Matrix22[2][0]==0) && (m_FlagMtrx[2][0]==1))){
		++iTransitions;}
	if (((m_Matrix22[2][0]==1) || (m_FlagMtrx[2][0]==0)) &&
		((m_Matrix22[2][1]==0) && (m_FlagMtrx[2][1]==1))){
		++iTransitions;}
	if (((m_Matrix22[2][1]==1) || (m_FlagMtrx[2][1]==0)) &&
		((m_Matrix22[2][2]==0) && (m_FlagMtrx[2][2]==1))){
		++iTransitions;}
	if (((m_Matrix22[2][2]==1) || (m_FlagMtrx[2][2]==0)) &&
		((m_Matrix22[1][2]==0) && (m_FlagMtrx[1][2]==1))){
		++iTransitions;}
	if (((m_Matrix22[1][2]==1) || (m_FlagMtrx[1][2]==0)) &&
		((m_Matrix22[0][2]==0) && (m_FlagMtrx[0][2]==1))){
		++iTransitions;}
	if (((m_Matrix22[0][2]==1) || (m_FlagMtrx[0][2]==0)) &&
		((m_Matrix22[0][1]==0) && (m_FlagMtrx[0][1]==1))){
		++iTransitions;}
	if (((m_Matrix22[0][1]==1) || (m_FlagMtrx[0][1]==0)) &&
		((m_Matrix22[0][0]==0) && (m_FlagMtrx[0][0]==1))){
		++iTransitions;}
	/*TRACE("\t  iTransitions=%d\n",iTransitions);*/
	if (iTransitions==1)
	{
		if (PreserveCorners)
		{
			if (k_ZhouCornerZhou(&m_Matrix22[0]))
			{
				/*TRACE0("\tpoint recognized as corner\n");*/
				return false;
			}
			else return true;
		}
		else return true;
	}
	else
	{
		return false;
	}
}

bool k_ZhouCond3Zhou(bool m_FlagMtrx[][3],UINT8 m_Matrix[][3])
{
	/* Checks for condition 3 as in article (see fig. 4)
	   check template a. */
	if ((m_Matrix[1][0]==0) && (m_Matrix[2][1]==0) &&
		(m_Matrix[2][0]==1) && (m_Matrix[0][1]==1) &&
		(m_Matrix[0][2]==1) && (m_Matrix[1][2]==1) &&
		(m_Matrix[2][2]==1) &&
		(m_FlagMtrx[1][0]==1) && (m_FlagMtrx[2][1]==1) &&
		(m_FlagMtrx[2][0]==0) && (m_FlagMtrx[0][1]==0) && 
		(m_FlagMtrx[0][2]==0) && (m_FlagMtrx[1][2]==0) &&
		(m_FlagMtrx[2][2]==0)){
		/*TRACE0("\t  Matched template a\n");*/
		return true;
	}
	/* check template b */
	if ((m_Matrix[1][0]==0) && (m_Matrix[2][1]==0) &&
		(m_Matrix[2][0]==1) && (m_Matrix[0][1]==1) &&
		(m_Matrix[0][2]==1) && (m_Matrix[1][2]==1) &&
		(m_Matrix[0][0]==1) &&
		(m_FlagMtrx[1][0]==1) && (m_FlagMtrx[2][1]==1) &&
		(m_FlagMtrx[2][0]==0) && (m_FlagMtrx[0][1]==0) && 
		(m_FlagMtrx[0][2]==0) && (m_FlagMtrx[1][2]==0) &&
		(m_FlagMtrx[0][0]==0)){
		/*TRACE0("\t  Matched template b\n");*/
		return true;
	}
	/* check template c */
	if ((m_Matrix[2][1]==0) && (m_Matrix[1][2]==0) &&
		(m_Matrix[0][0]==1) && (m_Matrix[1][0]==1) &&
		(m_Matrix[0][1]==1) && (m_Matrix[0][2]==1) &&
		(m_Matrix[2][2]==1) &&
		(m_FlagMtrx[2][1]==1) && (m_FlagMtrx[1][2]==1) &&
		(m_FlagMtrx[0][0]==0) && (m_FlagMtrx[1][0]==0) && 
		(m_FlagMtrx[0][1]==0) && (m_FlagMtrx[0][2]==0) &&
		(m_FlagMtrx[2][2]==0)){
		/*TRACE0("\t  Matched template c\n");*/
		return true;
	}
	/* check template d */
	if ((m_Matrix[2][1]==0) && (m_Matrix[1][2]==0) &&
		(m_Matrix[0][0]==1) && (m_Matrix[1][0]==1) &&
		(m_Matrix[0][1]==1) && (m_Matrix[2][0]==1) &&
		(m_Matrix[2][2]==1) &&
		(m_FlagMtrx[2][1]==1) && (m_FlagMtrx[1][2]==1) &&
		(m_FlagMtrx[0][0]==0) && (m_FlagMtrx[1][0]==0) && 
		(m_FlagMtrx[0][1]==0) && (m_FlagMtrx[2][0]==0) &&
		(m_FlagMtrx[2][2]==0)){
		/*TRACE0("\t  Matched template d\n");*/
		return true;
	}
	/* check template e */
	if ((m_Matrix[0][1]==0) && (m_Matrix[1][2]==0) &&
		(m_Matrix[0][0]==1) && (m_Matrix[1][0]==1) &&
		(m_Matrix[2][0]==1) && (m_Matrix[2][1]==1) &&
		(m_Matrix[0][2]==1) &&
		(m_FlagMtrx[0][1]==1) && (m_FlagMtrx[1][2]==1) &&
		(m_FlagMtrx[0][0]==0) && (m_FlagMtrx[1][0]==0) && 
		(m_FlagMtrx[2][0]==0) && (m_FlagMtrx[2][1]==0) &&
		(m_FlagMtrx[0][2]==0)){
		/*TRACE0("\t  Matched template e\n");*/
		return true;
	}
	/* check template f */
	if ((m_Matrix[0][1]==0) && (m_Matrix[1][2]==0) &&
		(m_Matrix[2][2]==1) && (m_Matrix[1][0]==1) &&
		(m_Matrix[2][0]==1) && (m_Matrix[2][1]==1) &&
		(m_Matrix[0][2]==1) &&
		(m_FlagMtrx[0][1]==1) && (m_FlagMtrx[1][2]==1) &&
		(m_FlagMtrx[2][2]==0) && (m_FlagMtrx[1][0]==0) && 
		(m_FlagMtrx[2][0]==0) && (m_FlagMtrx[2][1]==0) &&
		(m_FlagMtrx[0][2]==0)){
		/*TRACE0("\t  Matched template f\n");*/
		return true;
	}
	/* check template g */
	if ((m_Matrix[1][0]==0) && (m_Matrix[0][1]==0) &&
		(m_Matrix[0][0]==1) && (m_Matrix[2][0]==1) &&
		(m_Matrix[2][1]==1) && (m_Matrix[1][2]==1) &&
		(m_Matrix[2][2]==1) &&
		(m_FlagMtrx[1][0]==1) && (m_FlagMtrx[0][1]==1) &&
		(m_FlagMtrx[0][0]==0) && (m_FlagMtrx[2][0]==0) && 
		(m_FlagMtrx[2][1]==0) && (m_FlagMtrx[1][2]==0) &&
		(m_FlagMtrx[2][2]==0)){
		/*TRACE0("\t  Matched template g\n");*/
		return true;
	}
	/* check template h */
	if ((m_Matrix[1][0]==0) && (m_Matrix[0][1]==0) &&
		(m_Matrix[0][0]==1) && (m_Matrix[0][2]==1) &&
		(m_Matrix[2][1]==1) && (m_Matrix[1][2]==1) &&
		(m_Matrix[2][2]==1) &&
		(m_FlagMtrx[1][0]==1) && (m_FlagMtrx[0][1]==1) &&
		(m_FlagMtrx[0][0]==0) && (m_FlagMtrx[0][2]==0) && 
		(m_FlagMtrx[2][1]==0) && (m_FlagMtrx[1][2]==0) &&
		(m_FlagMtrx[2][2]==0)){
		/*TRACE0("\t  Matched template h\n");*/
		return true;
	}
	/*TRACE0("\t  No template matched\n");*/
	return false;
}

bool k_ZhouCornerZhou(UINT8 m_Matrix[][3])
{
	/* checks if there are more than 4 white pixels in a row surrounding P
	   avoids removing cornerpixels */
	short sWhitePixs=0;
	if (m_Matrix[0][0]==1){
		sWhitePixs++;
	}
	else sWhitePixs=0;
	if (m_Matrix[1][0]==1){
		sWhitePixs++;
	}
	else sWhitePixs=0;
	if (m_Matrix[2][0]==1){
		sWhitePixs++;
	}
	else sWhitePixs=0;
	if (m_Matrix[2][1]==1){
		sWhitePixs++;
	}
	else sWhitePixs=0;
	if (m_Matrix[2][2]==1){
		sWhitePixs++;
		if (sWhitePixs==5){
			return true;
		}
	}
	else sWhitePixs=0;
	if (m_Matrix[1][2]==1){
		sWhitePixs++;
		if (sWhitePixs==5){
			return true;
		}
	}
	else sWhitePixs=0;
	if (m_Matrix[0][2]==1){
		sWhitePixs++;
		if (sWhitePixs==5){
			return true;
		}
	}
	else sWhitePixs=0;
	if (m_Matrix[0][1]==1){
		sWhitePixs++;
		if (sWhitePixs==5){
			return true;
		}
	}
	else sWhitePixs=0;


	if (m_Matrix[0][0]==1){
		sWhitePixs++;
		if (sWhitePixs==5){
			return true;
		}
	}
	else return false;
	if (m_Matrix[1][0]==1){
		sWhitePixs++;
		if (sWhitePixs==5){
			return true;
		}
	}
	else return false;
	if (m_Matrix[2][0]==1){
		sWhitePixs++;
		if (sWhitePixs==5){
			return true;
		}
	}
	else return false;
	if (m_Matrix[2][1]==1){
		sWhitePixs++;
		if (sWhitePixs==5){
			return true;
		}
	}
	return false;
}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
