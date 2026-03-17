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

#include "kernel_feature_extraction.h"
#include "../kernel_error.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/************************************************/
/********        Private functions       ********/
/************************************************/

/* initialize all values in pMom to 0 */
void k_InitMoment(TMoment* pMom)
{
	pMom->m00=0;
	pMom->m10=0;
	pMom->m01=0;
	pMom->m20=0;
	pMom->m11=0;
	pMom->m02=0;
	pMom->m30=0;
	pMom->m21=0;
	pMom->m12=0;
	pMom->m03=0;
	pMom->m40=0;
	pMom->m31=0;
	pMom->m22=0;
	pMom->m13=0;
	pMom->m04=0;
}

/************************************************/
/********        Public functions        ********/
/************************************************/

T2DFloat k_DeriveCenterOfMassBinary(const TPixelGroup* pPixelGroup)
{
	T2DFloat P2D;
	/* Test for illegal NULL parameters */
	if ((pPixelGroup==NULL) || (pPixelGroup->NumberOfPixels==0))
	{
		k_ShowMessageSimple(IPL_ERROR,
							"k_DeriveCenterOfMassBinary() Given pixelgroup is either a NULL-"
							"pointer or it contains no positions");
		P2D.x=0;
		P2D.y=0;
		return P2D;
	}
	else
	{
		long m10=0, m01=0;
		T2DInt* p=pPixelGroup->pPos;
		T2DInt* end=&(pPixelGroup->pPos[pPixelGroup->NumberOfPixels]);
		T2DFloat CM;
		while(p!=end)
		{
			m10 += p->x;
			m01 += p->y;
			p++;
		}
		CM.x=(FLOAT32)m10/pPixelGroup->NumberOfPixels;
		CM.y=(FLOAT32)m01/pPixelGroup->NumberOfPixels;
		return CM;
	}
}

bool k_DeriveMomentsGrayTone(const TPixelGroup* pPixelGroup,double Background,
							 unsigned int MaxOrder,TMoment* pMom)
/* If MaxOrder>2 up to fourth order moments are derived. Otherwise up to second order. */
{
	unsigned int j;
	T2DInt p;
	double Gray;
	double x1,x2,x3,x4,y1,y2,y3,y4;
	
	/* Test for illegal NULL parameters */
	if ((pPixelGroup==NULL) || (pMom==NULL))
	{
		k_ShowMessageSimple(IPL_ERROR,
							"k_DeriveMomentsGrayTone() One of the parameters "
							"pPixelGroup or pSource are NULL");
		return false;
	}
	if (pPixelGroup->pColor==NULL)
	{
		k_ShowMessageSimple(IPL_ERROR, "k_DeriveMomentsGrayTone() The group contains no color info");
		return false;
	}
	k_InitMoment(pMom); /* initialize all values in pMom to 0 */
	/* Calculate moments */
	if (MaxOrder<=2)
	{
		for (j=0;j<pPixelGroup->NumberOfPixels;j++)
		{
			p=pPixelGroup->pPos[j];
			Gray=pPixelGroup->pColor[j]-Background;
			y1=p.y;
			y2=y1*y1;
			y3=y1*y2;
			y4=y2*y2;
			x1=Gray*p.x;
			x2=x1*p.x;
			x3=p.x*x2;
			x4=p.x*x3;
			pMom->m00+=Gray;
			pMom->m10+=x1;
			pMom->m01+=y1*Gray;
			pMom->m20+=x2;
			pMom->m11+=x1*y1;
			pMom->m02+=y2*Gray;
		}
	}
	else /* MaxOrder>=3 */
	{
		for (j=0;j<pPixelGroup->NumberOfPixels;j++)
		{
			p=pPixelGroup->pPos[j]; /* inserted by edr after warning by VC++ 6.0, not tested */
			Gray=pPixelGroup->pColor[j]-Background;
			y1=p.y;
			y2=y1*y1;
			y3=y1*y2;
			y4=y2*y2;
			x1=Gray*p.x;
			x2=x1*p.x;
			x3=p.x*x2;
			x4=p.x*x3;
			pMom->m00+=Gray;
			pMom->m10+=x1;
			pMom->m01+=y1*Gray;
			pMom->m20+=x2;
			pMom->m11+=x1*y1;
			pMom->m02+=y2*Gray;
			pMom->m30+=x3;
			pMom->m21+=x2*y1;
			pMom->m12+=x1*y2;
			pMom->m03+=y3*Gray;
			pMom->m40+=x4;
			pMom->m31+=x3*y1;
			pMom->m22+=y2*x2;
			pMom->m13+=x1*y3;
			pMom->m04+=y4*Gray;
		}
	}
	return true;
}

bool k_DeriveMomentsBinary(const TPixelGroup* pPixelGroup,unsigned int MaxOrder,TMoment* pMom)
/* Same as k_DeriveMomentsGraytone, except that eh weight of each set pixel is one. */
{
	unsigned int j;
	T2DInt p;
	double x1,x2,x3,x4,y1,y2,y3,y4;
	/* Test for illegal NULL parameters */
	if ((pPixelGroup==NULL) || (pMom==NULL))
	{
		k_ShowMessageSimple(IPL_ERROR,
							"k_DeriveMomentsBinary() One of the parameters "
							"pPixelGroup or pMom are NULL");
		return false;
	}

	k_InitMoment(pMom); /* initialize all values in pMom to 0 */
	/* Calculate moments */
	if (MaxOrder<=2)
	{
		for (j=0;j<pPixelGroup->NumberOfPixels;j++)
		{
			p=pPixelGroup->pPos[j];
			y1=p.y;
			y2=y1*y1;
			y3=y1*y2;
			y4=y2*y2;
			x1=p.x;
			x2=x1*p.x;
			x3=p.x*x2;
			x4=p.x*x3;
			pMom->m00+=1;
			pMom->m10+=x1;
			pMom->m01+=y1;
			pMom->m20+=x2;
			pMom->m11+=x1*y1;
			pMom->m02+=y2;
		}
	}
	else /* MaxOrder>=3 */
	{
		for (j=0;j<pPixelGroup->NumberOfPixels;j++)
		{
			p=pPixelGroup->pPos[j];
			y1=p.y;
			y2=y1*y1;
			y3=y1*y2;
			y4=y2*y2;
			x1=p.x;
			x2=x1*p.x;
			x3=p.x*x2;
			x4=p.x*x3;
			pMom->m00+=1;
			pMom->m10+=x1;
			pMom->m01+=y1;
			pMom->m20+=x2;
			pMom->m11+=x1*y1;
			pMom->m02+=y2;
			pMom->m30+=x3;
			pMom->m21+=x2*y1;
			pMom->m12+=x1*y2;
			pMom->m03+=y3;
			pMom->m40+=x4;
			pMom->m31+=x3*y1;
			pMom->m22+=y2*x2;
			pMom->m13+=x1*y3;
			pMom->m04+=y4;
		}
	}
	return true;
}

bool k_DeriveCentralMoments(const TMoment* pMom, TMoment* pCentrMom)
/* Calculates central moments from simple moments */
{
	double r10,r01;
	if ((pMom==NULL) || (pCentrMom==NULL))
	{
		k_ShowMessageSimple(IPL_ERROR,
							"k_DeriveCentralMoments() One of the "
							"parameters are a NULL pointer");
		return false;
	}
    r10=pMom->m10/pMom->m00;
	r01=pMom->m01/pMom->m00;
	pCentrMom->m00=pMom->m00;
	pCentrMom->m10=0;
	pCentrMom->m01=0;
	pCentrMom->m02=(pMom->m02-pMom->m01*pMom->m01/pMom->m00);
	pCentrMom->m11=(pMom->m11-pMom->m10*r01);
	pCentrMom->m20=(pMom->m20-pMom->m10*pMom->m10/pMom->m00);
	pCentrMom->m30=(pMom->m30-3*pMom->m20*r10+2*pMom->m10*r10*r10);
	pCentrMom->m12=(pMom->m12-pMom->m02*r10-2*pMom->m11*r01+2*r01*r01*pMom->m10);
	pCentrMom->m21=(pMom->m21-pMom->m20*r01-2*pMom->m11*r10+2*r10*r10*pMom->m01);
	pCentrMom->m03=(pMom->m03-3*pMom->m02*r01+2*pMom->m01*r01*r01);
	pCentrMom->m40=(pMom->m40-4*pMom->m30*r10+6*pMom->m20*r10*r10-
		3*r10*r10*r10*r10*pMom->m00);
	pCentrMom->m31=(pMom->m31-3*r10*pMom->m21+3*r10*r10*pMom->m11-
		r01*pMom->m30+3*r01*r10*pMom->m20-3*r01*r10*r10*pMom->m10);
	pCentrMom->m22=(pMom->m22-2*pMom->m21*r01+pMom->m20*r01*r01-
		2*r10*pMom->m12+4*pMom->m10*pMom->m11*r01/pMom->m00+
		r10*r10*pMom->m02-3*r10*r10*pMom->m01*pMom->m01/pMom->m00);
	pCentrMom->m13=(pMom->m13-3*pMom->m12*r01+3*pMom->m11*r01*r01-
		pMom->m03*r10+3*pMom->m02*r10*r01-3*pMom->m01*r10*r01*r01);
	pCentrMom->m04=(pMom->m04-4*pMom->m03*r01+6*pMom->m02*r01*r01-
		3*r01*r01*r01*r01*pMom->m00);
	return true;
}

bool k_DeriveCentralNormMoments(const TMoment* pMom, TMoment* pCentrNormMom)
/* Calculates central normalised moments from simple moments */
{
	double r10,r01,f1,f2,f3,f4;
	if ((pMom==NULL) || (pCentrNormMom==NULL))
	{
		k_ShowMessageSimple(IPL_ERROR,
							"k_DeriveCentralNormMoments() "
							"One of the parameters is a NULL pointer");
		return false;
	}
	f1=sqrt(pMom->m00);
	f2=pMom->m00;
	f3=f1*f2;
	f4=f2*f2;
	r10=pMom->m10/pMom->m00;
	r01=pMom->m01/pMom->m00;
	f1=sqrt(pMom->m00);
	f2=pMom->m00;
	f3=f1*f2;
	f4=f2*f2;
	pCentrNormMom->m00=pMom->m00;
	pCentrNormMom->m10=0;
	pCentrNormMom->m01=0;
	pCentrNormMom->m02=(pMom->m02-pMom->m01*pMom->m01/pMom->m00)/pMom->m00/f2;
	pCentrNormMom->m11=(pMom->m11-pMom->m10*r01)/pMom->m00/f2;
	pCentrNormMom->m20=(pMom->m20-pMom->m10*pMom->m10/pMom->m00)/pMom->m00/f2;
	pCentrNormMom->m30=(pMom->m30-3*pMom->m20*r10+2*pMom->m10*r10*r10)/pMom->m00/f3;
	pCentrNormMom->m12=(pMom->m12-pMom->m02*r10-2*pMom->m11*r01+2*r01*r01*pMom->m10)/pMom->m00/f3;
	pCentrNormMom->m21=(pMom->m21-pMom->m20*r01-2*pMom->m11*r10+2*r10*r10*pMom->m01)/pMom->m00/f3;
	pCentrNormMom->m03=(pMom->m03-3*pMom->m02*r01+2*pMom->m01*r01*r01)/pMom->m00/f3;
	pCentrNormMom->m40=(pMom->m40-4*pMom->m30*r10+6*pMom->m20*r10*r10-
		3*r10*r10*r10*r10*pMom->m00)/pMom->m00/f4;
	pCentrNormMom->m31=(pMom->m31-3*r10*pMom->m21+3*r10*r10*pMom->m11-
		r01*pMom->m30+3*r01*r10*pMom->m20-3*r01*r10*r10*pMom->m10)/pMom->m00/f4;
	pCentrNormMom->m22=(pMom->m22-2*pMom->m21*r01+pMom->m20*r01*r01-
		2*r10*pMom->m12+4*pMom->m10*pMom->m11*r01/pMom->m00+
		r10*r10*pMom->m02-3*r10*r10*pMom->m01*pMom->m01/pMom->m00)/pMom->m00/f4;
	pCentrNormMom->m13=(pMom->m13-3*pMom->m12*r01+3*pMom->m11*r01*r01-
		pMom->m03*r10+3*pMom->m02*r10*r01-3*pMom->m01*r10*r01*r01)/pMom->m00/f4;
	pCentrNormMom->m04=(pMom->m04-4*pMom->m03*r01+6*pMom->m02*r01*r01-
		3*r01*r01*r01*r01*pMom->m00)/pMom->m00/f4;
	return true;
}

bool k_DeriveHuInvariants(const TMoment* pCentrMom,double *pHu)
/* Calculates Hu type moment invarians from central moments
	or central normalized moments. */
{
	double a62,a64,b62,b64;
	pHu[0]=pCentrMom->m00;
	pHu[1]=pCentrMom->m20+pCentrMom->m02;
	pHu[2]=k_Sqr(pCentrMom->m20-pCentrMom->m02)+4*k_Sqr(pCentrMom->m11);
	pHu[3]=(pCentrMom->m20-pCentrMom->m02)*(k_Sqr(pCentrMom->m30+pCentrMom->m12)-k_Sqr(pCentrMom->m21+pCentrMom->m03))
		+4*pCentrMom->m11*(pCentrMom->m30+pCentrMom->m12)*(pCentrMom->m03+pCentrMom->m21);
	pHu[4]=k_Sqr(pCentrMom->m30-3*pCentrMom->m12)+k_Sqr(3*pCentrMom->m21-pCentrMom->m03);
	pHu[5]=k_Sqr(pCentrMom->m30+pCentrMom->m12)+k_Sqr(pCentrMom->m21+pCentrMom->m03);
	pHu[6]=(pCentrMom->m30-3*pCentrMom->m12)*(pCentrMom->m30+pCentrMom->m12)*(k_Sqr(pCentrMom->m30+pCentrMom->m12)-
		3*k_Sqr(pCentrMom->m21+pCentrMom->m03))+
		(pCentrMom->m03-3*pCentrMom->m21)*(pCentrMom->m03+pCentrMom->m21)*(k_Sqr(pCentrMom->m03+pCentrMom->m21)-
		3*k_Sqr(pCentrMom->m12+pCentrMom->m30));
	pHu[7]=(3*pCentrMom->m21-pCentrMom->m03)*(pCentrMom->m30+pCentrMom->m12)*(k_Sqr(pCentrMom->m30+pCentrMom->m12)-
		3*k_Sqr(pCentrMom->m21+pCentrMom->m03))-
		(3*pCentrMom->m12-pCentrMom->m30)*(pCentrMom->m03+pCentrMom->m21)*(k_Sqr(pCentrMom->m03+pCentrMom->m21)-
		3*k_Sqr(pCentrMom->m12+pCentrMom->m30));
	pHu[8]=pCentrMom->m40+2*pCentrMom->m22+pCentrMom->m04;
	a62=pCentrMom->m40-pCentrMom->m04; a64=pCentrMom->m40-6*pCentrMom->m22+pCentrMom->m04;
	b62=2*(pCentrMom->m31+pCentrMom->m13); b64=4*(pCentrMom->m31-pCentrMom->m13);
	pHu[9]= k_Sqr(a62)+k_Sqr(b62);
	pHu[10]=k_Sqr(a64)+k_Sqr(b64);
	pHu[11]=a64*(k_Sqr(a62)-k_Sqr(b62))+2*a62*b62*b64;
	pHu[12]=b64*(k_Sqr(a62)-k_Sqr(b62))-2*a62*b62*a64;
	return true;
}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
