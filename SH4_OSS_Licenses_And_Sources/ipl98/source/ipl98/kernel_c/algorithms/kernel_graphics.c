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

#include "kernel_graphics.h"
#include "../kernel_error.h"
#include "../general_functions/ipl_general_functions_c_only.h"
#include <math.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/************************************************/
/********        Public functions        ********/
/************************************************/

bool k_GraphicsGenerateLine(T2DInt Start, T2DInt End, TPixelGroup* pPixelGroup)
{
	const INT16 dx = abs(Start.x - End.x); 
	const INT16 dy = abs(Start.y - End.y); 
	const INT16 diffx = (End.x - Start.x);
	const INT16 diffy = (End.y - Start.y);
	INT16 const1, const2, p, x, y, step; 
	if (pPixelGroup==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GraphicsGenerateLine() PixelGroup is NULL");
		return false;
	}

	/* generate line points */
	x = Start.x; 
	y = Start.y; 
	k_AddPosToGroupFast(x,y,pPixelGroup);
	if (dx >= dy)
	{ 
		const1 = 2 * dy;
		const2 = 2 * (dy - dx);
		p = 2 * dy - dx;
		step = (Start.y <= End.y) ? (1) : (-1); 
		if (diffx>0) /* increment x */
		{
			while (x != End.x) 
			{ 
				if (p < 0)
					p += const1; 
				else 
				{ 
					y += step; 
					p += const2; 
				} 
				k_AddPosToGroupFast(++x,y,pPixelGroup);
			}
		}
		else /* decrement x */
		{
			while (x != End.x) 
			{ 
				if (p < 0)
					p += const1; 
				else 
				{ 
					y += step; 
					p += const2; 
				} 
				k_AddPosToGroupFast(--x,y,pPixelGroup);
			}
		}
	} 
	else /* dx < dy is true */
	{ 
		const1 = 2 * dx;
		const2 = 2 * (dx - dy);
		p = 2 * dx - dy;
		step = (Start.x <= End.x) ? (1) : (-1); 
		if (diffy>0) /* increment y */
		{
			while (y != End.y) 
			{ 
				if (p < 0)
					p += const1; 
				else 
				{ 
					x += step; 
					p += const2; 
				} 
				k_AddPosToGroupFast(x,++y,pPixelGroup);
			}
		}
		else /* decrement y */
		{
			while (y != End.y) 
			{ 
				if (p < 0)
					p += const1; 
				else 
				{ 
					x += step; 
					p += const2; 
				} 
				k_AddPosToGroupFast(x,--y,pPixelGroup);
			}
		}
	} 
	k_GroupUpdateBoundaries(pPixelGroup);
	return true;
}

bool k_GraphicsGenerateLineThick(T2DInt Start, T2DInt End, unsigned int Width, TPixelGroup* pPixelGroup)
{
	double Angle;
	int i;
	T2DInt P1,P2;
	if (pPixelGroup==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GraphicsGenerateLineThick() PixelGroup is NULL");
		return false;
	}
	if (Width==0)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GraphicsGenerateLineThick() Width is 0");
		return false;
	}
	Angle=fabs(atan2((double)(End.y-Start.y),(double)(End.x-Start.x)));
	if ((Angle<=(PI/4.0)) || (Angle>=((3.0*PI)/4.0)))
	{
		/* line is closest to horisontal - use vertical ends */
		for(i=k_Round((-(double)Width/2.0))+1; i<=(int)(Width/2); i++)
		{
			P1.x=Start.x;
			P1.y=Start.y+i;
			P2.x=End.x;
			P2.y=End.y+i;
			k_GraphicsGenerateLine(P1,P2,pPixelGroup);
		}
	}
	else
	{
		/* line is closest to vertical - use horisontal ends */
		for(i=k_Round((-(double)Width/2.0))+1; i<=(int)(Width/2); i++)
		{
			P1.x=Start.x+i;
			P1.y=Start.y;
			P2.x=End.x+i;
			P2.y=End.y;
			k_GraphicsGenerateLine(P1,P2,pPixelGroup);
		}
	}
	return true;
}

bool k_GraphicsGenerateCircle(T2DInt Center, unsigned int Radius, TPixelGroup* pPixelGroup)
{
	INT16 x = 0;
	INT16 y = Radius;
	int p = 3 - 2 * Radius;
	if (pPixelGroup==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GraphicsGenerateCircle() PixelGroup is NULL");
		return false;
	}
	while (x <= y)
	{
		k_AddPosToGroupFast((INT16)(Center.x + x), (INT16)(Center.y + y),pPixelGroup);
		k_AddPosToGroupFast((INT16)(Center.x - x), (INT16)(Center.y + y),pPixelGroup);
		k_AddPosToGroupFast((INT16)(Center.x + x), (INT16)(Center.y - y),pPixelGroup);
		k_AddPosToGroupFast((INT16)(Center.x - x), (INT16)(Center.y - y),pPixelGroup);
		k_AddPosToGroupFast((INT16)(Center.x + y), (INT16)(Center.y + x),pPixelGroup);
		k_AddPosToGroupFast((INT16)(Center.x - y), (INT16)(Center.y + x),pPixelGroup);
		k_AddPosToGroupFast((INT16)(Center.x + y), (INT16)(Center.y - x),pPixelGroup);
		k_AddPosToGroupFast((INT16)(Center.x - y), (INT16)(Center.y - x),pPixelGroup);
		if (p < 0)
			p += 4 * x++ + 6;
		else
			p += 4 * (x++ - y--) + 10;
	}
	k_GroupUpdateBoundaries(pPixelGroup);
	return true;
}

bool k_GraphicsGenerateCircleRing(T2DInt Center, unsigned int Radius1, unsigned int Radius2, TPixelGroup* pPixelGroup)
{
	int x1 = 0, x2 = 0, k=0;
	int y1 = Radius1, y2 = Radius2;
	int p1 = 3 - 2 * Radius1;
	int p2 = 3 - 2 * Radius2;
	T2DInt Pnt1,Pnt2;
	if (pPixelGroup==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GraphicsGenerateCircleRing() PixelGroup is NULL");
		return false;
	}
	if (Radius1>Radius2)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GraphicsGenerateCircleRing() Radius1>Radius2");
		return false;
	}
	while (x1 <= y1)
	{
		Pnt1.x=Center.x + x1; Pnt1.y=Center.y + y1;
		Pnt2.x=Center.x + x2; Pnt2.y=Center.y + y2;
		k_GraphicsGenerateLine(Pnt2,Pnt1,pPixelGroup);
		Pnt1.x=Center.x - x1; Pnt1.y=Center.y + y1;
		Pnt2.x=Center.x - x2; Pnt2.y=Center.y + y2;
		k_GraphicsGenerateLine(Pnt2,Pnt1,pPixelGroup);
		Pnt1.x=Center.x + x1; Pnt1.y=Center.y - y1;
		Pnt2.x=Center.x + x2; Pnt2.y=Center.y - y2;
		k_GraphicsGenerateLine(Pnt2,Pnt1,pPixelGroup);
		Pnt1.x=Center.x - x1; Pnt1.y=Center.y - y1;
		Pnt2.x=Center.x - x2; Pnt2.y=Center.y - y2;
		k_GraphicsGenerateLine(Pnt2,Pnt1,pPixelGroup);
		Pnt1.x=Center.x + y1; Pnt1.y=Center.y + x1;
		Pnt2.x=Center.x + y2; Pnt2.y=Center.y + x2;
		k_GraphicsGenerateLine(Pnt2,Pnt1,pPixelGroup);
		Pnt1.x=Center.x - y1; Pnt1.y=Center.y + x1;
		Pnt2.x=Center.x - y2; Pnt2.y=Center.y + x2;
		k_GraphicsGenerateLine(Pnt2,Pnt1,pPixelGroup);
		Pnt1.x=Center.x + y1; Pnt1.y=Center.y - x1;
		Pnt2.x=Center.x + y2; Pnt2.y=Center.y - x2;
		k_GraphicsGenerateLine(Pnt2,Pnt1,pPixelGroup);
		Pnt1.x=Center.x - y1; Pnt1.y=Center.y - x1;
		Pnt2.x=Center.x - y2; Pnt2.y=Center.y - x2;
		k_GraphicsGenerateLine(Pnt2,Pnt1,pPixelGroup);
		if (p1 < 0)
			p1 += 4 * x1++ + 6;
		else
			p1 += 4 * (x1++ - y1--) + 10;
		if (p2 < 0)
			p2 += 4 * x2++ + 6;
		else
			p2 += 4 * (x2++ - y2--) + 10;
	}
	y1=x1;
	while (x2 <= y2)
	{
		Pnt1.x=Center.x+x2; Pnt1.y=Center.y+y1+k;
		Pnt2.x=Center.x + x2; Pnt2.y=Center.y + y2;
		k_GraphicsGenerateLine(Pnt2,Pnt1,pPixelGroup);
		Pnt1.x=Center.x-x2; Pnt1.y=Center.y+y1+k;
		Pnt2.x=Center.x - x2; Pnt2.y=Center.y + y2;
		k_GraphicsGenerateLine(Pnt2,Pnt1,pPixelGroup);
		Pnt1.x=Center.x+x2; Pnt1.y=Center.y-y1-k;
		Pnt2.x=Center.x + x2; Pnt2.y=Center.y - y2;
		k_GraphicsGenerateLine(Pnt2,Pnt1,pPixelGroup);
		Pnt1.x=Center.x-x2; Pnt1.y=Center.y-y1-k;
		Pnt2.x=Center.x - x2; Pnt2.y=Center.y - y2;
		k_GraphicsGenerateLine(Pnt2,Pnt1,pPixelGroup);
		Pnt1.x=Center.x+y1+k; Pnt1.y=Center.y+x2;
		Pnt2.x=Center.x + y2; Pnt2.y=Center.y + x2;
		k_GraphicsGenerateLine(Pnt2,Pnt1,pPixelGroup);
		Pnt1.x=Center.x-y1-k; Pnt1.y=Center.y+x2;
		Pnt2.x=Center.x - y2; Pnt2.y=Center.y + x2;
		k_GraphicsGenerateLine(Pnt2,Pnt1,pPixelGroup);
		Pnt1.x=Center.x+y1+k; Pnt1.y=Center.y-x2;
		Pnt2.x=Center.x + y2; Pnt2.y=Center.y - x2;
		k_GraphicsGenerateLine(Pnt2,Pnt1,pPixelGroup);
		Pnt1.x=Center.x-y1-k; Pnt1.y=Center.y-x2;
		Pnt2.x=Center.x - y2; Pnt2.y=Center.y - x2;
		k_GraphicsGenerateLine(Pnt2,Pnt1,pPixelGroup);
		++k;
		if (p2 < 0)
			p2 += 4 * x2++ + 6;
		else
			p2 += 4 * (x2++ - y2--) + 10;
	}
	return true;
}

bool k_GraphicsGenerateCircleRingW(T2DInt Center, unsigned int Radius, unsigned int Width, TPixelGroup* pPixelGroup)
{
	unsigned int Radius1, Radius2;
	if (Width==0)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GraphicsGenerateCircleRingW() Width is 0");
		return false;
	}
	Radius1=Radius+k_Round((-(double)Width/2.0))+1;
	Radius2=Radius+(int)(Width/2);
	return k_GraphicsGenerateCircleRing(Center, Radius1, Radius2,pPixelGroup);
}

bool k_GraphicsGenerateEllipse(T2DInt Center, unsigned int RadiusX, unsigned int RadiusY, TPixelGroup* pPixelGroup)
{
	if ((RadiusX==0) || (RadiusY==0))
	{
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_GraphicsGenerateEllipse() "
			"One of the main axises (RadiusX or RadiusY) is 0");
		return false;
	}
	else if (pPixelGroup==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GraphicsGenerateEllipse() PixelGroup is NULL");
		return false;
	}
	else
	{
		int a=RadiusX;
		int b=RadiusY;
		int x,y;
		double bsq=b*b;
		double asq=a*a;
		double ratio=bsq/asq;
		double xf,yf,dx,dy;
		y=b;
		x=0;
		dy=0;
		while (dy<=1)
		{ 
			k_AddPosToGroupFast((INT16)(x+Center.x),(INT16)(y+Center.y),pPixelGroup);
			x--;
			yf=b*sqrt(1-(x*x)/asq);
			dy=-x*ratio/yf;
			y=(int)(yf+0.5);
		}
		dx=0;
		while (dx<1)
		{
			k_AddPosToGroupFast((INT16)(x+Center.x),(INT16)(y+Center.y),pPixelGroup);
			y--;
			xf=-a*sqrt(1-(y*y)/bsq);
			dx=y/xf/ratio;
			x=(int)(xf-0.5);
		}
		dy=0;
		while (dy<=1)
		{
			k_AddPosToGroupFast((INT16)(x+Center.x),(INT16)(y+Center.y),pPixelGroup);
			x++;
			yf=-b*sqrt(1-(x*x)/asq);
			dy=-x*ratio/yf;
			y=(int)(yf-0.5);
		}
		dx=0;
		while (dx<=1)
		{
			k_AddPosToGroupFast((INT16)(x+Center.x),(INT16)(y+Center.y),pPixelGroup);
			y++;
			xf=a*sqrt(1-(y*y)/bsq);
			dx=y/xf/ratio;
			x=(int)(xf+0.5);
		}
		while (x>0)
		{
			k_AddPosToGroupFast((INT16)(x+Center.x),(INT16)(y+Center.y),pPixelGroup);
			x--;
			y=(int)(b*sqrt(1-(x*x)/asq)+0.5);
		}
	}
	k_GroupUpdateBoundaries(pPixelGroup);
	return true;
}

bool k_GraphicsGenerateCross(T2DInt Center, unsigned int Size, TPixelGroup* pPixelGroup)
{
	int Half1 = (int)(Size/2);
	int Half2 = (int)((Size/2.0)+0.5)-1;
	T2DInt Start, End;
	if (pPixelGroup==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GraphicsGenerateCross() PixelGroup is NULL");
		return false;
	}
	Start.x = Center.x - Half1;
	Start.y = Center.y - Half1;
	End.x   = Center.x + Half2;
	End.y   = Center.y + Half2;
	k_GraphicsGenerateLine(Start,End,pPixelGroup);
	Start.x = Center.x + Half1;
	Start.y = Center.y - Half1;
	End.x   = Center.x - Half2;
	End.y   = Center.y + Half2;
	k_GraphicsGenerateLine(Start,End,pPixelGroup);
	return true;
}

bool k_GraphicsGeneratePlus(T2DInt Center, unsigned int Size, TPixelGroup* pPixelGroup)
{
	int Half1 = (int)(Size/2);
	int Half2 = (int)((Size/2.0)+0.5)-1;
	T2DInt Start, End;
	if (pPixelGroup==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GraphicsGenerateCross() PixelGroup is NULL");
		return false;
	}
	Start.x = Center.x - Half1;
	Start.y = Center.y;
	End.x   = Center.x + Half2;
	End.y   = Center.y;
	k_GraphicsGenerateLine(Start,End,pPixelGroup);
	Start.x = Center.x;
	Start.y = Center.y - Half1;
	End.x   = Center.x;
	End.y   = Center.y + Half2;
	k_GraphicsGenerateLine(Start,End,pPixelGroup);
	return true;
}

bool k_GraphicsGenerateRectangle(T2DInt TopLeft, T2DInt BottumRight, TPixelGroup* pPixelGroup)
{
	T2DInt Start, End;
	if (pPixelGroup==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GraphicsGenerateRectangle() PixelGroup is NULL");
		return false;
	}
	else if ((TopLeft.x>=BottumRight.x) || (TopLeft.y>=BottumRight.y))
	{
		k_ShowMessageSimple(IPL_ERROR, "k_GraphicsGenerateRectangle() TopLeft and BottumRight not correct positioned");
		return false;
	}
	/* upper horisontal line */
	Start = TopLeft;
	End.x = BottumRight.x;
	End.y = TopLeft.y;
	k_GraphicsGenerateLine(Start,End,pPixelGroup);
	/* right vertical line */
	Start.x = BottumRight.x;
	Start.y = TopLeft.y + 1;
	End = BottumRight;
	k_GraphicsGenerateLine(Start,End,pPixelGroup);
	/* bottum horisontal line */
	Start.x = BottumRight.x - 1;
	Start.y = BottumRight.y;
	End.x = TopLeft.x;
	End.y = BottumRight.y;
	k_GraphicsGenerateLine(Start,End,pPixelGroup);
	/* right vertical line */
	Start.x = TopLeft.x;
	Start.y = BottumRight.y - 1;
	End.x = TopLeft.x;
	End.y = TopLeft.y+1;
	/* special cases */
	if (Start.y>=End.y) 
		k_GraphicsGenerateLine(Start,End,pPixelGroup);
	return true;
}



/************************************************/
/********        Private functions       ********/
/************************************************/



#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
