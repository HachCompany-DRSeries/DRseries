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

#include "kernel_perspective.h"
#include "../general_functions/ipl_general_functions_c_only.h"
#include "../kernel_error.h"

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

/************************************************/
/********        Private functions       ********/
/************************************************/

void k_LinEq(int n, double**Coef, double*Right, double*Res);

/* moves given ccd-points according to k-value, calibrates camera matrix with these
	new ccd-points. Returns the mean error for the original 3d world-points
	transposed to 2d-coordinates and compared to the new moved ccd-points. */
double k_RadialCalibAndGetMeanError(const T3D2DPoints *pPointSets,double k,FLOAT32 CenterX,FLOAT32 CenterY);

/* finds a minimum between ax and cx with a tolerance of tol, from Numerical Recipes p. 401 */
double k_CalibGoldenSearch(double ax,double bx,double cx,double tol,double *xmin,
						  const T3D2DPoints* pPointSets, FLOAT32 CenterX,FLOAT32 CenterY);

/* return the mean error for all points in pWorld when transposed to 
	2D-coordinates and compared to the pPointSets 2d points, the latter must
	be after radial distortion correction */
double k_GetMeanError(const T3D2DPoints* pPointSets,const TCameraMatrix* pMat);

/** For a given position (x,y) with origo in center of image(!) a new inverse radial 
	corrected position is returned, that is the position in a corresponding image 
	with radial distortions. This functionality is the opposite of 
	k_GetPosRadRemoved().
	@param x The position in horisontal direction.
	@param y The position in vertical direction.
	@param k The distortion parameter calculated by k_CalibrateWithRadialDist().
	@return Inverse radial corrected 2d position.
	@see k_GetPosRadRemoved */
T2DFloat k_InverseRadialMove(double xr, double yr,double k);

/* returns the inverse radial corrected point (x,y) in image and returns it in the T2DFloat return value
	the position (xr,yr) must be relative to origin in center of image!!! */
#define CAL_SQRT2	1.4142135623731		/*	Sqrt(2)		*/
#define CAL_SQRT3	1.7320508075689		/*  Sqrt(3)		*/
#define CAL_FRAC1_3 0.33333333333333	/*  1/3			*/
#define CAL_FRAC2_3 0.66666666666667	/*  2/3			*/
#define CAL_N2POW1_3 1.2599210498949	/*	pow(2,1/3)	*/
#define CAL_N3POW1_3 1.4422495703074	/*	pow(3,1/3)	*/
#define CAL_N6POW2_3 3.3019272488946	/*	pow(6,2/3)	*/

/* faster version of k_GetPixelInterpolatedFast(), assumes 8 b/p */
float k_CalibGetPixelInterpolated8bp(float x, float y, const TImage* pInfo);

/************************************************/
/********        Public functions        ********/
/************************************************/


bool k_CalibrateCamera(const T3D2DPoints* pPointSets, TCameraMatrix* pMat)
{ 
	unsigned int nc,i,j,k;
	double sum;
	double **anm;
	double *rightm;
	double **a;
	double **a0;
	double * Res;
	double * Right;

	/* initialize camera matrix, a44 is set to 1 the rest set to 0 */
	pMat->a11=0; pMat->a12=0; pMat->a13=0; pMat->a14=0;
	pMat->a21=0; pMat->a22=0; pMat->a23=0; pMat->a24=0;
	pMat->a31=0; pMat->a32=0; pMat->a33=0; pMat->a34=0;
	pMat->a41=0; pMat->a42=0; pMat->a43=0; pMat->a44=1;

	if ((pPointSets==NULL) || (pMat==NULL))
	{
		k_ShowMessageSimple(IPL_ERROR, "k_CalibrateCamera() One of the parameters is a NULL pointer");
		return false;
	}
	nc=2*pPointSets->NumberOfSets;
	/*	do not accept a T3D2DPoints structure with entries
		where either the 2D or 3D point is not in use */
	if (pPointSets->NumberOfSets!=k_GetTotal3D2DPointSetsInUse(pPointSets))
	{
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_CalibrateCamera() The T3D2DPoints structure containes entries "
			"where either the 2D or 3D point is not used");
		return false;
	}
	/* check if there are at least 6 point sets */
	if (pPointSets->NumberOfSets<6)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_CalibrateCamera() Needs at least 6 points (only %d is given)",
			pPointSets->NumberOfSets);
		return false;
	}
	a=(double**)calloc(11,sizeof(double*));
	a0=(double**)calloc(nc,sizeof(double*));
	Res=(double*)calloc(nc,sizeof(double));
	Right=(double*)calloc(nc,sizeof(double));
	anm=(double**)calloc(nc,sizeof(double*)); /* inserted by edr after warning by VC++ 6.0 */
	rightm=(double*)calloc(nc,sizeof(double)); /* inserted by edr after warning by VC++ 6.0 */
	for (i=0;i<11;i++)
	{
		a[i]=(double*)calloc(11,sizeof(double));
	}
	for (i=0;i<nc;i++)
	{
		anm[i]=(double*)calloc(11,sizeof(double));
	}
	for (i=0;i<nc;i++)
	{
		for(j=0;j<11;j++)
		{
			anm[i][j]=0;
		}
	}
	for (i=0;i<pPointSets->NumberOfSets;i++)
	{
		anm[2*i][0]=pPointSets->pSet[i].Pnt3D.x;
		anm[2*i][1]=pPointSets->pSet[i].Pnt3D.y;
		anm[2*i][2]=pPointSets->pSet[i].Pnt3D.z;
		anm[2*i][3]=1;
		anm[2*i+1][4]=pPointSets->pSet[i].Pnt3D.x;
		anm[2*i+1][5]=pPointSets->pSet[i].Pnt3D.y;
		anm[2*i+1][6]=pPointSets->pSet[i].Pnt3D.z;
		anm[2*i+1][7]=1;
		anm[2*i][8]=-pPointSets->pSet[i].Pnt2D.x*pPointSets->pSet[i].Pnt3D.x;
		anm[2*i][9]=-pPointSets->pSet[i].Pnt2D.x*pPointSets->pSet[i].Pnt3D.y;
		anm[2*i][10]=-pPointSets->pSet[i].Pnt2D.x*pPointSets->pSet[i].Pnt3D.z;
		anm[2*i+1][8]=-pPointSets->pSet[i].Pnt2D.y*pPointSets->pSet[i].Pnt3D.x;
		anm[2*i+1][9]=-pPointSets->pSet[i].Pnt2D.y*pPointSets->pSet[i].Pnt3D.y;
		anm[2*i+1][10]=-pPointSets->pSet[i].Pnt2D.y*pPointSets->pSet[i].Pnt3D.z;
		rightm[2*i]=pPointSets->pSet[i].Pnt2D.x;
		rightm[2*i+1]=pPointSets->pSet[i].Pnt2D.y;
	}
	for (i=0;i<11;i++)
	{
		for (j=0;j<11;j++)
		{
			sum=0;
			for (k=0;k<nc;k++)
			{
				sum=sum+anm[k][i]*anm[k][j];
			}
			a[i][j]=sum;
		}
	}
	for (i=0;i<11;i++)
	{
		sum=0;
		for (k=0;k<nc;k++)
			sum=sum+anm[k][i]*rightm[k];
		Right[i]=sum;
	}
	k_LinEq(11,a,Right,Res);
	pMat->a11=(FLOAT32)Res[0];
	pMat->a12=(FLOAT32)Res[1];
	pMat->a13=(FLOAT32)Res[2];
	pMat->a14=(FLOAT32)Res[3];
	pMat->a21=(FLOAT32)Res[4];
	pMat->a22=(FLOAT32)Res[5];
	pMat->a23=(FLOAT32)Res[6];
	pMat->a24=(FLOAT32)Res[7];
	pMat->a41=(FLOAT32)Res[8];
	pMat->a42=(FLOAT32)Res[9];
	pMat->a43=(FLOAT32)Res[10];
	for (i=0;i<nc;i++)
	{
		free(anm[i]);
	}
	for (i=0;i<11;i++)
	{
		free(a[i]);
	}
	free(Res);
	free(Right);
	free(a0);
	free(a);
	free(anm);
	free(rightm);
	return true;
}

bool k_Calibrate(unsigned int np, const T3DFloat *pWorld, const T2DFloat *pCcd, TCameraMatrix* pMat)
{ 
	unsigned int nc,i,j,k;
	double sum;
	double **anm;
	double *rightm;
	double **a;
	double **a0;
	double * Res;
	double * Right;

	/* initialize camera matrix, a44 is set to 1 the rest set to 0 */
	pMat->a11=0; pMat->a12=0; pMat->a13=0; pMat->a14=0;
	pMat->a21=0; pMat->a22=0; pMat->a23=0; pMat->a24=0;
	pMat->a31=0; pMat->a32=0; pMat->a33=0; pMat->a34=0;
	pMat->a41=0; pMat->a42=0; pMat->a43=0; pMat->a44=1;

	if ((pWorld==NULL) || (pCcd==NULL) || (pMat==NULL))
	{
		k_ShowMessageSimple(IPL_ERROR, "k_Calibrate() One of the parameters is a NULL pointer");
		return false;
	}
	nc=2*np;
	if (np<6)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_Calibrate() Needs at least 6 points (only %d is given)",
			nc);
		return false;
	}
	a=(double**)calloc(11,sizeof(double*));
	a0=(double**)calloc(nc,sizeof(double*));
	Res=(double*)calloc(nc,sizeof(double));
	Right=(double*)calloc(nc,sizeof(double));
	anm=(double**)calloc(nc,sizeof(double*)); /* inserted by edr after warning by VC++ 6.0 */
	rightm=(double*)calloc(nc,sizeof(double)); /* inserted by edr after warning by VC++ 6.0 */
	for (i=0;i<11;i++)
	{
		a[i]=(double*)calloc(11,sizeof(double));
	}
	for (i=0;i<nc;i++)
	{
		anm[i]=(double*)calloc(11,sizeof(double));
	}
	for (i=0;i<nc;i++)
	{
		for(j=0;j<11;j++)
		{
			anm[i][j]=0;
		}
	}
	for (i=0;i<np;i++)
	{
		anm[2*i][0]=pWorld[i].x;
		anm[2*i][1]=pWorld[i].y;
		anm[2*i][2]=pWorld[i].z;
		anm[2*i][3]=1;
		anm[2*i+1][4]=pWorld[i].x;
		anm[2*i+1][5]=pWorld[i].y;
		anm[2*i+1][6]=pWorld[i].z;
		anm[2*i+1][7]=1;
		anm[2*i][8]=-pCcd[i].x*pWorld[i].x;
		anm[2*i][9]=-pCcd[i].x*pWorld[i].y;
		anm[2*i][10]=-pCcd[i].x*pWorld[i].z;
		anm[2*i+1][8]=-pCcd[i].y*pWorld[i].x;
		anm[2*i+1][9]=-pCcd[i].y*pWorld[i].y;
		anm[2*i+1][10]=-pCcd[i].y*pWorld[i].z;
		rightm[2*i]=pCcd[i].x;
		rightm[2*i+1]=pCcd[i].y;
	}
	for (i=0;i<11;i++)
	{
		for (j=0;j<11;j++)
		{
			sum=0;
			for (k=0;k<nc;k++)
			{
				sum=sum+anm[k][i]*anm[k][j];
			}
			a[i][j]=sum;
		}
	}
	for (i=0;i<11;i++)
	{
		sum=0;
		for (k=0;k<nc;k++)
			sum=sum+anm[k][i]*rightm[k];
		Right[i]=sum;
	}
	k_LinEq(11,a,Right,Res);
	pMat->a11=(FLOAT32)Res[0];
	pMat->a12=(FLOAT32)Res[1];
	pMat->a13=(FLOAT32)Res[2];
	pMat->a14=(FLOAT32)Res[3];
	pMat->a21=(FLOAT32)Res[4];
	pMat->a22=(FLOAT32)Res[5];
	pMat->a23=(FLOAT32)Res[6];
	pMat->a24=(FLOAT32)Res[7];
	pMat->a41=(FLOAT32)Res[8];
	pMat->a42=(FLOAT32)Res[9];
	pMat->a43=(FLOAT32)Res[10];
	for (i=0;i<nc;i++)
	{
		free(anm[i]);
	}
	for (i=0;i<11;i++)
	{
		free(a[i]);
	}
	free(Res);
	free(Right);
	free(a0);
	free(a);
	free(anm);
	free(rightm);
	return true;
}

void k_Calc3DTo2D(T3DFloat w,const TCameraMatrix* pMat, T2DFloat* pPnt)
{
	FLOAT32 den;
	den=pMat->a41*w.x+pMat->a42*w.y+pMat->a43*w.z+1;
	pPnt->x=(FLOAT32)(pMat->a11*w.x+pMat->a12*w.y+pMat->a13*w.z+pMat->a14)/den;
	pPnt->y=(FLOAT32)(pMat->a21*w.x+pMat->a22*w.y+pMat->a23*w.z+pMat->a24)/den;
}

void k_Direction(T2DFloat p,const TCameraMatrix* pMat,T3DFloat* pDir)
{
	FLOAT32 norm;
	pDir->x=-(pMat->a13-pMat->a43*p.x)*(pMat->a22-pMat->a42*p.y)+
		(pMat->a23-pMat->a43*p.y)*(pMat->a12-pMat->a42*p.x);
	pDir->y=+(pMat->a13-pMat->a43*p.x)*(pMat->a21-pMat->a41*p.y)-
		(pMat->a23-pMat->a43*p.y)*(pMat->a11-pMat->a41*p.x);
	pDir->z=(pMat->a11-pMat->a41*p.x)*(pMat->a22-pMat->a42*p.y)-
		(pMat->a21-pMat->a41*p.y)*(pMat->a12-pMat->a42*p.x);
	norm=(FLOAT32)sqrt(pDir->x*pDir->x+pDir->y*pDir->y+pDir->z*pDir->z);
	pDir->x=pDir->x/norm;
	pDir->y=pDir->y/norm;
	pDir->z=pDir->z/norm;
}

void k_PinHole(const TCameraMatrix* pMat, T3DFloat* pPinHole)
{
	int i;
	double** a=(double**)calloc(3,sizeof(double*));
	double * Res=(double*)calloc(3,sizeof(double));
	double * Right=(double*)calloc(3,sizeof(double));
	
	for (i=0;i<3;i++)
		a[i]=(double*)calloc(3,sizeof(double));

	a[0][0]=pMat->a11;
	a[0][1]=pMat->a12;
	a[0][2]=pMat->a13;
	a[1][0]=pMat->a21;
	a[1][1]=pMat->a22;
	a[1][2]=pMat->a23;
	a[2][0]=pMat->a41;
	a[2][1]=pMat->a42;
	a[2][2]=pMat->a43;
	Right[0]=-pMat->a14;
	Right[1]=-pMat->a24;
	Right[2]=-1;
	k_LinEq(3,a,Right,Res);
	pPinHole->x=(FLOAT32)Res[0];
	pPinHole->y=(FLOAT32)Res[1];
	pPinHole->z=(FLOAT32)Res[2];
	free(Res);
	free(Right);
	for (i=0;i<3;i++)
		free(a[i]);
	free(a);
}

bool k_CalibrateWithRadialDist(const T3D2DPoints* pPointSets,TCameraMatrix* pMat, 
							   double* k, FLOAT32 CenterX, FLOAT32 CenterY)
{
	if ((pPointSets==NULL) || (pMat==NULL) || (k==NULL))
	{
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_CalibrateWithRadialDist() One of the given parameters is a NULL pointer");
		return false;
	}
	/*	do not accept a T3D2DPoints structure with entries
		where either the 2D or 3D point is not in use */
	else if (pPointSets->NumberOfSets!=k_GetTotal3D2DPointSetsInUse(pPointSets))
	{
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_CalibrateWithRadialDist() The T3D2DPoints structure containes entries "
			"where either the 2D or 3D point is not used");
		return false;
	}
	/* check if there are at least 6 point sets */
	else if (pPointSets->NumberOfSets<6)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_CalibrateWithRadialDist() Need 6 reference marks (current is %d)",
			pPointSets->NumberOfSets);
		return false;
	}
	else
	{
		const double LOWER_VAL=0;
		const double UPPER_VAL=0.000005; /* 5e-6 since version 1.60*/
	  /*const double UPPER_VAL=0.000001;*/ /* 1e-6 */
		const unsigned int ITERATIONS=100;
		unsigned int i;
		unsigned int iterate=0;
		double LowValError,UpperValError,NewError;
		double LowVal=LOWER_VAL;
		double UpperVal=UPPER_VAL;
		double NewVal;
		T3D2DPoints NewPointSets;
		k_Init3D2DPoints(&NewPointSets);
		/* initialize system */
		LowValError=k_RadialCalibAndGetMeanError(pPointSets,LowVal,CenterX,CenterY);
		UpperValError=k_RadialCalibAndGetMeanError(pPointSets,UpperVal,CenterX,CenterY);

		/* search for a "bracketing minimum" error between 0 and UPPER_VAL,
			when found a golden search method is used for finding the minimum
			error between this bracketed interval */
		for(iterate=0;iterate<ITERATIONS;iterate++)
		{
			NewVal=(LowVal+UpperVal)/2.0;
			NewError=k_RadialCalibAndGetMeanError(pPointSets,NewVal,CenterX,CenterY);
		
			if ((NewError>LowValError) && (NewError<UpperValError))
			{
				UpperVal=NewVal;
				UpperValError=NewError;
			}
			else if ((NewError<LowValError) && (NewError>UpperValError))
			{
				LowVal=NewVal;
				LowValError=NewError;
			}
			else
			{	
				/* if we end up here, we have a bracketed minimum, 
					that is: NewError<LowValError and NewError<UpperValError */
				double Tolerance=1e-6;
				/*double test=*/
                k_CalibGoldenSearch(LowVal,NewVal,UpperVal,Tolerance,&NewVal,
						  pPointSets,CenterX,CenterY);
				break;

			}
		}
		if (iterate>=ITERATIONS)
		{
			/* a bracketed minimum was never found, warn the user */
			k_ShowMessageSimple(
				IPL_WARNING,
				"k_CalibrateWithRadialDist() Couldn't find a suitable"
				" distortion value k better than the initial 0 (no radial correction)");
		}

		*k=NewVal;
		k_Copy3D2DPoints(&NewPointSets,pPointSets);
		for(i=0; i<pPointSets->NumberOfSets;i++)
		{
			T2DFloat P2d;
			P2d=k_GetPosRadRemoved(pPointSets->pSet[i].Pnt2D.x,pPointSets->pSet[i].Pnt2D.y,*k,CenterX,CenterY);
			k_SetPoint2DIn3D2DPoints(&P2d,i,&NewPointSets);
		}
		k_CalibrateCamera(&NewPointSets,pMat);
		k_Empty3D2DPoints(&NewPointSets);
		return true;
	}
}

void k_Calc3DTo2DRad(T3DFloat w, const TCameraMatrix* pMat, double k, T2DFloat* pPnt, FLOAT32 CenterX, FLOAT32 CenterY)
{
	k_Calc3DTo2D(w,pMat,pPnt);
	/* inverse adjust for radial distortion */
	(*pPnt)=k_InverseRadialMove(pPnt->x-CenterX,pPnt->y-CenterY,k);
	pPnt->x+=(float)CenterX;
	pPnt->y+=(float)CenterY;
}

T2DInt k_GetMaxRadialDisplacement(double k, FLOAT32 CenterX, FLOAT32 CenterY)
{
	T2DInt MaxRadDispl;
	MaxRadDispl.x=k_Round(k*CenterX*(CenterX*CenterX+CenterY*CenterY));
	MaxRadDispl.y=k_Round(k*CenterY*(CenterX*CenterX+CenterY*CenterY));
	return MaxRadDispl;
}

bool k_RemoveRadialDistortion(TImage* pDest, TImage* pSource, double k, bool PreserveImageSize)
{
	TImage* pTarget=pDest;
	bool CopyToSource=false;
	bool ReturnValue=false;

	if (pSource->Origin==EMPTY)
	{
		k_ShowMessageSimple(IPL_WARNING, "k_RemoveRadialDistortion() Source image is empty - doing nothing");
		ReturnValue=false;
	}
	else if (pSource->Bits!=8)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_RemoveRadialDistortion() Image must be 8 b/p (current is %d b/p)",
			pSource->Bits);
		ReturnValue=false;
	}
	else /* Remove the radial distortion */
	{
		unsigned int x,y;
		T2DInt ImageCenter;
		T2DInt TargetCenter;
		T2DFloat Pos;
		/* variable for the optimized k_InverseRadialMove functionality */
		double x2;			/*  x^2        */
		double y2;			/*  y^2        */
		double x2y2;		/*  x^2 + y^2  */
		double MainDenominator; /* common part of denominator used for calculating new values of both x and y */
		double MainNumerator; /* common part of numerator used for calculating new values of both x and y */
		double y3;			/*  y^3        */
		double kx2y2;
		double sqrtExp;
		double x2y2Pow1_5;
		double powParam;
		const double k27=27*k;	/*	27*k	   */
		const double sqrtk=sqrt(k); /* Sqrt(k)   */
		const double kPow1_5=pow(k,1.5);
		const double sqrt3kPow1_5=CAL_SQRT3*kPow1_5;
		const double n3Pow1_3t2=-2*CAL_N3POW1_3;
		/* end of k_InverseRadialMove variables */
		ImageCenter.x=(short)/*k_Round*/(pSource->Width/2.0);
		ImageCenter.y=(short)/*k_Round*/(pSource->Height/2.0);
		/* Check if the source and destination is the same image */
		if (pDest==pSource)
		{
			pTarget=(TImage*) malloc(sizeof(TImage));
			k_InitImage(pTarget);
			CopyToSource=true;
		}
		/* calculate size of destination image and allocate memory */
		if (PreserveImageSize==true)
			k_AllocImage(pSource->Width,pSource->Height,8,pTarget);
		else
		{
			T2DInt MaxRadDispl;
			MaxRadDispl=k_GetMaxRadialDisplacement(k,(float)(pSource->Width/2.0),(float)(pSource->Height/2.0));
			k_AllocImage(pSource->Width+2*MaxRadDispl.x,pSource->Height+2*MaxRadDispl.y,8,pTarget);
		}
		TargetCenter.x=(short)/*k_Round*/(pTarget->Width/2.0);
		TargetCenter.y=(short)/*k_Round*/(pTarget->Height/2.0);
		if (PreserveImageSize==false)
		{
			/* set border so no pixel access outside image is performed */
			double by=(pSource->Height/2.0)*pow(pSource->Width/2.0,2)*k;
			double bx=(pSource->Width/2.0)*pow(pSource->Height/2.0,2)*k;
			bx>by ? k_SetBorder((UINT16)(bx+1),0,pSource) : k_SetBorder((UINT16)(by+1),0,pSource);
		}
		k_SetBorderColor(255,pSource);
		/* Copy pixels to new image with radial correction */
/*		for(x=0;x<pTarget->Width;x++)
		{
			for(y=0;y<pTarget->Height;y++)
			{
				Pos=k_InverseRadialMove(((double)x)-TargetCenter.x,((double)y)-TargetCenter.y,k);
				Pos.x+=ImageCenter.x;
				Pos.y+=ImageCenter.y;
				k_SetPixel8bp(x,y,(unsigned int)(k_CalibGetPixelInterpolated8bp(Pos.x,Pos.y,pSource)+0.5),*pTarget);
			}
		}*/
		for(x=0;x<(pTarget->Width/2.0);x++)
		{
			x2=x*x;			/*  x^2        */
			for(y=0;y<(pTarget->Height/2.0);y++)
			{
				/* calculate inverse radial move, it could be done with a call like shown below:
				   but for speed optimization, the code has been copied into this place. */
				/*Pos=k_InverseRadialMove(((double)x),((double)y),k);*/

				/* calculate sub-expressions */
				y2=y*y;			/*  y^2        */
				x2y2=x2+y2;		/*  x^2 + y^2  */

				if (y==0) /* special case */
				{
					Pos.x= (float)((2*CAL_N3POW1_3-CAL_N2POW1_3*pow(-9*sqrtk*x+CAL_SQRT3*sqrt(4+k27*x2),CAL_FRAC2_3)) /
						(CAL_N6POW2_3*sqrtk*pow(-9*sqrtk*x+CAL_SQRT3*sqrt(4+k27*x2),CAL_FRAC1_3)));
					Pos.y=0;
				}
				else
				{
					y3=pow(y,3.0);	/*  y^3        */
					kx2y2=k*x2y2;
					sqrtExp=sqrt(4+k27*x2y2);
					x2y2Pow1_5=pow(x2y2,1.5);
					powParam=9*kx2y2*kx2y2*y3+sqrt3kPow1_5*y3*x2y2Pow1_5*sqrtExp;
					MainDenominator=n3Pow1_3t2*y2*kx2y2+CAL_N2POW1_3*pow(powParam,CAL_FRAC2_3);
					MainNumerator=CAL_N6POW2_3*kx2y2*pow(powParam,CAL_FRAC1_3);
					/* Calculate the new y value */
					Pos.y=(float)((MainDenominator)/(MainNumerator));
					/* Calculate the new x value */
					/*Pos.x=(float)((x*MainDenominator)/(y*MainNumerator));*/
					Pos.x=(float)(((double)x/y)*Pos.y);
					
				}

				/* set a pixel value in each of the four quadrants in image */
				k_SetPixel8bp(TargetCenter.x+x,TargetCenter.y+y,(unsigned int)(
					k_CalibGetPixelInterpolated8bp(ImageCenter.x+Pos.x,ImageCenter.y+Pos.y,pSource)+0.5),*pTarget);
				k_SetPixel8bp(TargetCenter.x-x,TargetCenter.y+y,(unsigned int)(
					k_CalibGetPixelInterpolated8bp(ImageCenter.x-Pos.x,ImageCenter.y+Pos.y,pSource)+0.5),*pTarget);
				k_SetPixel8bp(TargetCenter.x-x,TargetCenter.y-y,(unsigned int)(
					k_CalibGetPixelInterpolated8bp(ImageCenter.x-Pos.x,ImageCenter.y-Pos.y,pSource)+0.5),*pTarget);
				k_SetPixel8bp(TargetCenter.x+x,TargetCenter.y-y,(unsigned int)(
					k_CalibGetPixelInterpolated8bp(ImageCenter.x+Pos.x,ImageCenter.y-Pos.y,pSource)+0.5),*pTarget);
			}
		}
		/* copy image to source if destination and source is the same image */
		if (CopyToSource==true)
		{
			k_CopyImage(pDest,pTarget);
			k_EmptyImage(pTarget);
			free(pTarget);
		}
		ReturnValue=true;
	}
	return ReturnValue;
}

T2DFloat k_GetPosRadRemoved(FLOAT32 x,FLOAT32 y, double k, FLOAT32 CenterX, FLOAT32 CenterY)
{
	T2DFloat NewPos;
	FLOAT32 u=(FLOAT32)(x-CenterX);
	FLOAT32 v=(FLOAT32)(y-CenterY);
	NewPos.x=x;
	NewPos.y=y;
	NewPos.x+=(FLOAT32)k*u*(u*u+v*v);
	NewPos.y+=(FLOAT32)k*v*(u*u+v*v);
	return NewPos;
}

T2DFloat k_GetPosInverseRad(FLOAT32 xr, FLOAT32 yr, double k, FLOAT32 CenterX, FLOAT32 CenterY)
{
	T2DFloat NewPos;
	NewPos=k_InverseRadialMove(((double)xr)-CenterX,((double)yr)-CenterY,k);
	NewPos.x+=CenterX;
	NewPos.y+=CenterY;
	return NewPos;
}

void k_PrintCameraMatrix(const TCameraMatrix* pMat, bool WithIndexes)
{
	printf("**************** TCameraMatrix info ********************\n");
	if (WithIndexes==true)
	{
		printf(" a11=% .4e a12=% .4e a13=% .4e a14=% .4e\n",pMat->a11,pMat->a12,pMat->a13,pMat->a14);
		printf(" a21=% .4e a22=% .4e a23=% .4e a24=% .4e\n",pMat->a21,pMat->a22,pMat->a23,pMat->a24);
		printf(" a31=% .4e a32=% .4e a33=% .4e a34=% .4e\n",pMat->a31,pMat->a32,pMat->a33,pMat->a34);
		printf(" a41=% .4e a42=% .4e a43=% .4e a44=% .4e\n",pMat->a41,pMat->a42,pMat->a43,pMat->a44);
	}
	else
	{
		printf(" % .4e % .4e % .4e % .4e\n",pMat->a11,pMat->a12,pMat->a13,pMat->a14);
		printf(" % .4e % .4e % .4e % .4e\n",pMat->a21,pMat->a22,pMat->a23,pMat->a24);
		printf(" % .4e % .4e % .4e % .4e\n",pMat->a31,pMat->a32,pMat->a33,pMat->a34);
		printf(" % .4e % .4e % .4e % .4e\n",pMat->a41,pMat->a42,pMat->a43,pMat->a44);
	}
}

bool k_MatrixToParameters(const TCameraMatrix* pMat, TCameraParameters* pPar)
{
        double den,sqrtden;
        bool k23positive;
        double a0,a1;
        double b0,b1;
        int TrueSolutions;
        double test1,test2,test3,test4;
        double err1,err2,err3,err4;
        double ctemp;
        double c0,c1;
        double beta0,beta1,beta2,beta3;
        double n,m,nom,det;
        double c00,c01,c02,c03;
        double c10,c11,c12,c13;
        double c20,c21,c22;
        double ZeroThreshold = 1.0e-5;

        c00=pMat->a11;  c01=pMat->a12;  c02=pMat->a13;  c03=pMat->a14;
        c10=pMat->a21;  c11=pMat->a22;  c12=pMat->a23;  c13=pMat->a24;
        c20=pMat->a41;  c21=pMat->a42;  c22=pMat->a43;

        /* Focal centre (dx,dy,dz) */
        den =           c02*c11*c20 - c01*c12*c20 - c02*c10*c21 +
                        c00*c12*c21 + c01*c10*c22 - c00*c11*c22;

        pPar->dx =      (-c02*c11 + c01*c12 - c03*c12*c21 +
                        c02*c13*c21 + c03*c11*c22 - c01*c13*c22)/ den;

        pPar->dy =      (c02*c10 - c00*c12 + c03*c12*c20 - c02*c13*c20
                        - c03*c10*c22 + c00*c13*c22)/ den;

        pPar->dz =      (c00*c11 - c03*c11*c20 - c01*(c10 - c13*c20)
                        + c03*c10*c21 - c00*c13*c21)/ den;

        /* temporary results */
        den = c20*c20 + c21*c21 + c22*c22;
        sqrtden = sqrt(den);

        /* Origin (xh,yh) of image plane on the CCD chip measured in pixels */
        pPar->xh = (c00*c20 + c01*c21 + c02*c22) / den;
        pPar->yh = (c10*c20 + c11*c21 + c12*c22) / den;

        /* Focal lenght = 1/f measured in pixels */
        pPar->f = 1 / sqrt((c00*c00 + c01*c01 + c02*c02) / den - pPar->xh*pPar->xh);
		pPar->FocalLength=1/pPar->f; /* added by edr@mip.sdu.dk 21/5/2000 */

        /* Aspect ratio, defined as scale of V relative to U */
        pPar->p = pPar->f * sqrt((c10*c10 + c11*c11 + c12*c12) / den - pPar->yh*pPar->yh);

        /* External angles (a,b,c) */
        /* angle a and b */
        a0 = atan(-c21 / c22);
        a1 = a0 + PI;

        b0 = asin(-c20 / sqrtden);
        b1 = asin(c20 / sqrtden);

        test1 = -sin(b0)*pPar->dx + cos(b0)*sin(a0)*pPar->dy - cos(b0)*cos(a0)*pPar->dz;
        test2 = -sin(b1)*pPar->dx + cos(b1)*sin(a0)*pPar->dy - cos(b1)*cos(a0)*pPar->dz;
        test3 = -sin(b0)*pPar->dx + cos(b0)*sin(a1)*pPar->dy - cos(b0)*cos(a1)*pPar->dz;
        test4 = -sin(b1)*pPar->dx + cos(b1)*sin(a1)*pPar->dy - cos(b1)*cos(a1)*pPar->dz;
        err1 = fabs(test1) - 1/sqrtden;
        err2 = fabs(test2) - 1/sqrtden;
        err3 = fabs(test3) - 1/sqrtden;
        err4 = fabs(test4) - 1/sqrtden;

        TrueSolutions = 0;
        if((test1 > 0)&&(fabs(err1) < ZeroThreshold))
        {
                pPar->a = a0;
                pPar->b = b0;
                k23positive = true;
                TrueSolutions = TrueSolutions+1;
        }
        if((test2 > 0)&&(fabs(err2) < ZeroThreshold))
        {
                pPar->a = a0;
                pPar->b = b1;
                k23positive = false;
                TrueSolutions = TrueSolutions+1;
        }
        if((test3 > 0)&&(fabs(err3) < ZeroThreshold))
        {
                pPar->a = a1;
                pPar->b = b0;
                k23positive = true;
                TrueSolutions = TrueSolutions+1;
        }
        if((test4 > 0)&&(fabs(err4) < ZeroThreshold))
        {
                pPar->a = a1;
                pPar->b = b1;
                k23positive = false;
                TrueSolutions = TrueSolutions+1;
        }
        if(TrueSolutions == 0)
        {
                k_ShowMessageSimple(
					IPL_ERROR,
					"k_MatrixToParameters() No true solution found for angle a and b");
                return false;
        }
        if(TrueSolutions > 1)
        {
                k_ShowMessageSimple(
					IPL_WARNING,
					"k_MatrixToParameters() More solutions found for angle a and b");
        }

        /* angle c */
        ctemp = atan((c02*c21 - c01*c22) / (sqrtden * (c00 - pPar->xh*c20)));

        if(k23positive)
        {
                c0 = ctemp;
                c1 = ctemp + PI;
        }
        else
        {
                c0 = -ctemp;
                c1 = -ctemp - PI;
        }

        test1 =  (cos(c0)*cos(pPar->b) - pPar->xh*pPar->f*sin(pPar->b)) * pPar->dx
                +(sin(c0)*cos(pPar->a) + sin(pPar->a)*cos(c0)*sin(pPar->b)
                + pPar->xh*pPar->f*sin(pPar->a)*cos(pPar->b)) * pPar->dy
                +(sin(c0)*sin(pPar->a) - cos(pPar->a)*cos(c0)*sin(pPar->b)
                - pPar->xh*pPar->f*cos(pPar->a)*cos(pPar->b)) * pPar->dz;
        test2 =  (cos(c1)*cos(pPar->b) - pPar->xh*pPar->f*sin(pPar->b)) * pPar->dx
                +(sin(c1)*cos(pPar->a) + sin(pPar->a)*cos(c1)*sin(pPar->b)
                + pPar->xh*pPar->f*sin(pPar->a)*cos(pPar->b)) * pPar->dy
                +(sin(c1)*sin(pPar->a) - cos(pPar->a)*cos(c1)*sin(pPar->b)
                - pPar->xh*pPar->f*cos(pPar->a)*cos(pPar->b)) * pPar->dz;
        err1 = fabs(test1) - fabs(pPar->f/sqrtden * c03);
        err2 = fabs(test2) - fabs(pPar->f/sqrtden * c03);

        TrueSolutions = 0;
        if(fabs(err1) < ZeroThreshold)
        {
                pPar->c = c0;
                TrueSolutions = TrueSolutions+1;
        }
        if(fabs(err2) < ZeroThreshold)
        {
                pPar->c = c1;
                TrueSolutions = TrueSolutions+1;
        }
        if(TrueSolutions == 0)
        {
                k_ShowMessageSimple(
					IPL_ERROR,
					"k_MatrixToParameters() No true solution found for angle c");
                return false;
        }
        if(TrueSolutions > 1)
        {
                k_ShowMessageSimple(
					IPL_WARNING,
					"k_MatrixToParameters() More solutions found for angle c");
        }

        /* Skewness of V relative to U */
        nom = pPar->f*(sin(pPar->b)*pPar->dx - cos(pPar->b)*sin(pPar->a)*pPar->dy
                + cos(pPar->b)*cos(pPar->a)*pPar->dz);
        det = - c02*c11*c20 + c01*c12*c20
              + c02*c10*c21 - c00*c12*c21
              - c01*c10*c22 + c00*c11*c22;
        n = tan(pPar->c);
        m = (cos(pPar->a)*c11 + sin(pPar->a)*c12)/(pPar->p*cos(pPar->c))*nom;

        beta0 = acos(-(fabs(det)*nom*nom*nom)/(pPar->f*pPar->p));
        beta1 = -beta0;
        beta2 = 2*atan((n + sqrt(1 + n*n - m*m))/(1 + m));
        beta3 = 2*atan((n - sqrt(1 + n*n - m*m))/(1 + m));
        err1 = fabs(beta0-beta2);
        err2 = fabs(beta0-beta3);
        err3 = fabs(beta1-beta2);
        err4 = fabs(beta1-beta3);

        TrueSolutions = 0;
        if((fabs(err1) < ZeroThreshold)||(fabs(fabs(err1)-PI) < ZeroThreshold))
        {
                pPar->beta = beta0;
                TrueSolutions = TrueSolutions+1;
        }
        if((fabs(err2) < ZeroThreshold)||(fabs(fabs(err2)-PI) < ZeroThreshold))
        {
                pPar->beta = beta0;
                TrueSolutions = TrueSolutions+1;
        }
        if((fabs(err3) < ZeroThreshold)||(fabs(fabs(err3)-PI) < ZeroThreshold))
        {
                pPar->beta = beta1;
                TrueSolutions = TrueSolutions+1;
        }
        if((fabs(err4) < ZeroThreshold)||(fabs(fabs(err4)-PI) < ZeroThreshold))
        {
                pPar->beta = beta1;
                TrueSolutions = TrueSolutions+1;
        }
        if(TrueSolutions == 0)
        {
                k_ShowMessageSimple(IPL_ERROR, "k_MatrixToParameters() No true solution found for angle beta");
                return false;
        }
        if(TrueSolutions > 1)
        {
                k_ShowMessageSimple(IPL_WARNING, "k_MatrixToParameters() More solutions found for angle beta");
        }
        return true;
}


bool k_ParametersToMatrix(const TCameraParameters* pPar, TCameraMatrix* pMat)
{
        FLOAT32 a44;
        if(pPar->f==0.0)
        {
                k_ShowMessageSimple(IPL_ERROR, "k_ParametersToMatrix() Camera matrix undefined for f=0.0");
                return false;
        }
        if((pPar->dx==0.0) && (pPar->dy==0.0) && (pPar->dz==0.0))
        {
                k_ShowMessageSimple(IPL_ERROR, "k_ParametersToMatrix() Camera matrix undefined for dx=0.0, dy=0.0 and dz=0.0");
                return false;
        }
        if((pPar->b==PI) && (pPar->dx==0.0))
        {
                k_ShowMessageSimple(IPL_ERROR, "k_ParametersToMatrix() Camera matrix undefined for b=Pi and dx=0.0o");
                return false;
        }
        /* building unnormalized transformation matrix */
        pMat->a11 =     (FLOAT32)(cos(pPar->b)*cos(pPar->c)
                        - pPar->f*pPar->xh*sin(pPar->b));
        pMat->a12 =     (FLOAT32)(pPar->f*pPar->xh*cos(pPar->b)*sin(pPar->a)
                        + cos(pPar->c)*sin(pPar->a)*sin(pPar->b)
                        + cos(pPar->a)*sin(pPar->c));
        pMat->a13 =     (FLOAT32)(-(cos(pPar->a)*(pPar->f*pPar->xh*cos(pPar->b)
                        + cos(pPar->c)*sin(pPar->b)))
                        + sin(pPar->a)*sin(pPar->c));
        pMat->a14 =     (FLOAT32)(-(cos(pPar->b)*(pPar->dx*cos(pPar->c)
                        + pPar->dy*pPar->f*pPar->xh*sin(pPar->a)))
                        + pPar->dx*pPar->f*pPar->xh*sin(pPar->b)
                        - pPar->dy*cos(pPar->c)*sin(pPar->a)*sin(pPar->b)
                        - pPar->dz*sin(pPar->a)*sin(pPar->c)
                        + cos(pPar->a)*(pPar->dz*pPar->f*pPar->xh*cos(pPar->b)
                        + pPar->dz*cos(pPar->c)*sin(pPar->b)
                        - pPar->dy*sin(pPar->c)));
        pMat->a21 =     (FLOAT32)(-(pPar->f*pPar->yh*sin(pPar->b))
                        + pPar->p*cos(pPar->b)*sin(pPar->beta - pPar->c));
        pMat->a22 =     (FLOAT32)(pPar->p*cos(pPar->a)*cos(pPar->beta - pPar->c)
                        + sin(pPar->a)*(pPar->f*pPar->yh*cos(pPar->b)
                        + pPar->p*sin(pPar->b)*sin(pPar->beta - pPar->c)));
        pMat->a23 =     (FLOAT32)(pPar->p*cos(pPar->beta - pPar->c)*sin(pPar->a)
                        - cos(pPar->a)*(pPar->f*pPar->yh*cos(pPar->b)
                        + pPar->p*sin(pPar->b)*sin(pPar->beta - pPar->c)));
        pMat->a24 =     (FLOAT32)(pPar->dz*pPar->f*pPar->yh*cos(pPar->a)*cos(pPar->b)
                        - pPar->dy*pPar->p*cos(pPar->a)*cos(pPar->beta - pPar->c)
                        - pPar->dy*pPar->f*pPar->yh*cos(pPar->b)*sin(pPar->a)
                        - pPar->dz*pPar->p*cos(pPar->beta - pPar->c)*sin(pPar->a)
                        + pPar->dx*pPar->f*pPar->yh*sin(pPar->b)
                        - pPar->p*(pPar->dx*cos(pPar->b) + (-(pPar->dz*cos(pPar->a))
                        + pPar->dy*sin(pPar->a))
                        *sin(pPar->b))*sin(pPar->beta - pPar->c));
        pMat->a41 =     (FLOAT32)(-(pPar->f*sin(pPar->b)));
        pMat->a42 =     (FLOAT32)(pPar->f*cos(pPar->b)*sin(pPar->a));
        pMat->a43 =     (FLOAT32)(-(pPar->f*cos(pPar->a)*cos(pPar->b)));

        a44 =           (FLOAT32)(pPar->f*(pPar->dz*cos(pPar->a)*cos(pPar->b)
                        - pPar->dy*cos(pPar->b)*sin(pPar->a) + pPar->dx*sin(pPar->b)));

        /* normalizing matrix */
        pMat->a11 = pMat->a11/a44;
        pMat->a12 = pMat->a12/a44;
        pMat->a13 = pMat->a13/a44;
        pMat->a14 = pMat->a14/a44;
        pMat->a21 = pMat->a21/a44;
        pMat->a22 = pMat->a22/a44;
        pMat->a23 = pMat->a23/a44;
        pMat->a24 = pMat->a24/a44;
        pMat->a41 = pMat->a41/a44;
        pMat->a42 = pMat->a42/a44;
        pMat->a43 = pMat->a43/a44;

        return true;
}

void k_PrintCameraParameterInfo(const TCameraParameters* pPar)
{
	printf("**************** TCameraParameters info ********************\n");
	printf("x coordinate of focal centre (dx) = % .4f \n",pPar->dx);
	printf("y coordinate of focal centre (dy) = % .4f \n",pPar->dy);
	printf("z coordinate of focal centre (dz) = % .4f \n",pPar->dz);
	printf("rotation of camera around the x-axis (a) = % .4f [rad] = % .4f [deg]\n",pPar->a,pPar->a*180.0/PI);
	printf("rotation of camera around the y-axis (b) = % .4f [rad] = % .4f [deg]\n",pPar->b,pPar->b*180.0/PI);
	printf("rotation of camera around the z-axis (c) = % .4f [rad] = % .4f [deg]\n",pPar->c,pPar->c*180.0/PI);
	printf("focal length = % .4f [pixels]\n",pPar->FocalLength);
	printf("x-coordinate of origin of the focal centre on the CCD chip (xh) = % .4f [pixels]\n",pPar->xh);
	printf("y-coordinate of origin of the focal centre on the CCD chip (yh) = % .4f [pixels]\n",pPar->yh);
	printf("aspect ratio, scale of V relative to U (p) = % .4f \n",pPar->p);
	printf("skewness of V relative to U (beta) =  % .4f [rad] = % .4f [deg]\n",pPar->beta,pPar->beta*180.0/PI);
}

/************************************************/
/********        Private functions       ********/
/************************************************/

void k_LinEq(int n, double**Coef, double*Right, double*Res)
{
	int* r=(int*)calloc(n, sizeof(int));
	int i,j,k,m,temp;
	double max, buf;
	for(i=0;i<n;i++)
	{
		r[i]=i;
	}
	for(k=0;k<n-1;k++)
	{
		max=0;
		for (i=k;i<n;i++)
		{
			if (fabs(Coef[r[i]][k])>max)
			{
				max=Coef[r[i]][k];
				m=i;
			}
		}
		if (m>k)
		{
			temp=r[k];
			r[k]=r[m];
			r[m]=temp;
		}
		for (i=k+1;i<n;i++)
		{
			buf=Coef[r[i]][k];
			if (buf!=0)
			{
				buf=buf/Coef[r[k]][k];
				for (j=k+1;j<n;j++){
					Coef[r[i]][j]=Coef[r[i]][j]-buf*Coef[r[k]][j];
				}
				Right[r[i]]=Right[r[i]]-buf*Right[r[k]];
			}
		}
	}
	Res[n-1]=Right[r[n-1]]/Coef[r[n-1]][n-1];
	for (i=n-2;i>=0;i--)
	{
		buf=Right[r[i]];
		for (j=i+1;j<n;j++){
			buf=buf-Coef[r[i]][j]*Res[j];
		}
		Res[i]=buf/Coef[r[i]][i];
	}
	free(r);
}

double k_RadialCalibAndGetMeanError(const T3D2DPoints *pPointSets,double k,FLOAT32 CenterX,FLOAT32 CenterY)
{
	unsigned int i;
	double ReturnValue;
	TCameraMatrix Mat;
	T3D2DPoints NewPointSets; /* used to hold the new Ccd 2D coordinates */
	k_Init3D2DPoints(&NewPointSets);
	k_Copy3D2DPoints(&NewPointSets,pPointSets);
	for(i=0; i<pPointSets->NumberOfSets;i++)
	{
		T2DFloat P2d;
		P2d=k_GetPosRadRemoved(pPointSets->pSet[i].Pnt2D.x,pPointSets->pSet[i].Pnt2D.y,k,CenterX,CenterY);
		k_SetPoint2DIn3D2DPoints(&P2d,i,&NewPointSets);
	}
	k_CalibrateCamera(&NewPointSets,&Mat);
	ReturnValue=k_GetMeanError(&NewPointSets,&Mat);
	k_Empty3D2DPoints(&NewPointSets);
	return ReturnValue;
}

#define CALIB_R 0.61803399
#define CALIB_C (1.0-CALIB_R)
#define CALIB_SHFT2(a,b,c) do { (a)=(b); (b)=(c); } while (0)
#define CALIB_SHFT3(a,b,c,d) do { (a)=(b); (b)=(c); (c)=(d); } while (0)

double k_CalibGoldenSearch(double ax,double bx,double cx,double tol,double *xmin,
						  const T3D2DPoints* pPointSets, FLOAT32 CenterX, FLOAT32 CenterY)
{
	double f1,f2,x0,x1,x2,x3;
	x0=ax;
	x3=cx;
	if (fabs(cx-bx)>fabs(bx-ax))
	{
		x1=bx;
		x2=bx+CALIB_C*(cx-bx);
	}
	else
	{
		x2=bx;
		x1=bx-CALIB_C*(bx-ax);
	}
	f1=k_RadialCalibAndGetMeanError(pPointSets,x1,CenterX,CenterY);
	f2=k_RadialCalibAndGetMeanError(pPointSets,x2,CenterX,CenterY);
	while ((fabs(x3-x0) > tol*(fabs(x1)+fabs(x2))) && (x1>1e-15)) // last check by edr - stop if k gets very small
	{
		if (f2<f1)
		{
			CALIB_SHFT3(x0,x1,x2,CALIB_R*x1+CALIB_C*x3);
			CALIB_SHFT2(f1,f2,k_RadialCalibAndGetMeanError(pPointSets,x2,CenterX,CenterY));
		}
		else
		{
			CALIB_SHFT3(x3,x2,x1,CALIB_R*x2+CALIB_C*x0);
			CALIB_SHFT2(f2,f1,k_RadialCalibAndGetMeanError(pPointSets,x1,CenterX,CenterY));
		}
	}
	if (f1<f2)
	{
		*xmin=x1;
		return f1;
	}
	else
	{
		*xmin=x2;
		return f2;
	}
}

double k_GetMeanError(const T3D2DPoints* pPointSets,const TCameraMatrix* pMat)
{
	unsigned int i;
	T2DFloat point;
	double XErrorSum=0;
	double YErrorSum=0;
	/*printf("In k_GetMeanError()\n");*/
	for(i=0; i<pPointSets->NumberOfSets;i++)
	{
		k_Calc3DTo2D(pPointSets->pSet[i].Pnt3D,pMat,&point);
		XErrorSum+=fabs(pPointSets->pSet[i].Pnt2D.x-point.x);
		YErrorSum+=fabs(pPointSets->pSet[i].Pnt2D.y-point.y);
		/*printf("  Point 0 at (%f,%f) Error: (%f,%f)\n",pPointSets->pSet[i].Pnt2D.x,pPointSets->pSet[i].Pnt2D.y,
				pPointSets->pSet[i].Pnt2D.x-point.x,pPointSets->pSet[i].Pnt2D.y-point.y);*/
	}
	return (double)((XErrorSum+YErrorSum)/(2.0*i));
}

T2DFloat k_InverseRadialMove(double xr, double yr,double k)
{

	T2DFloat NewPos; /* the new position to be returned */

	double x=fabs(xr); /* nummerical value of xr */
	double y=fabs(yr); /* nummerical value of yr */

	/* calculate sub-expressions */
	double x2=x*x;			/*  y^2        */
	double y2=y*y;			/*  y^2        */
	double x2y2=x2+y2;		/*  x^2 + y^2  */

	double MainDenominator; /* common part of denominator used for calculating new values of both x and y */
	double MainNumerator; /* common part of numerator used for calculating new values of both x and y */

	if (yr==0) /* special case */
	{
		NewPos.x= (float)((2*CAL_N3POW1_3-CAL_N2POW1_3*pow(-9*sqrt(k)*x+CAL_SQRT3*sqrt(4+27*k*x2),CAL_FRAC2_3)) /
			(CAL_N6POW2_3*sqrt(k)*pow(-9*sqrt(k)*x+CAL_SQRT3*sqrt(4+27*k*x2),CAL_FRAC1_3)));
		NewPos.y=0;
	}
	else
	{
		/* Could save 4 * operations and 1 pow() when k is constant! */
		double y3=pow(y,3);	/*  y^3        */
		double kx2y2=k*x2y2;
		double sqrtExp=sqrt(4+27*k*x2y2);
		double kPow1_5=pow(k,1.5);
		double x2y2Pow1_5=pow(x2y2,1.5);
		double powParam=9*kx2y2*kx2y2*y3+CAL_SQRT3*kPow1_5*y3*x2y2Pow1_5*sqrtExp;
		MainDenominator=-2*CAL_N3POW1_3*y2*kx2y2+CAL_N2POW1_3*pow(powParam,CAL_FRAC2_3);
		MainNumerator=CAL_N6POW2_3*kx2y2*pow(powParam,CAL_FRAC1_3);
		/* Calculate the new x value */
		NewPos.x=(float)((x*MainDenominator)/(y*MainNumerator));

		/* Calculate the new y value */
		NewPos.y=(float)((MainDenominator)/(MainNumerator));
	}
	if (x!=xr)
		NewPos.x=-NewPos.x;
	if (y!=yr)
		NewPos.y=-NewPos.y;
	return NewPos;
}

float k_CalibGetPixelInterpolated8bp(float x, float y, const TImage* pInfo)
{
	int xl = (x >=0 ? (int)x : (int)(x-1)); /* always round down */
	int yl = (y >=0 ? (int)y : (int)(y-1)); /* always round down */
	/* lower right pixel (h ~ high) */
	int xh = xl + 1;
	int yh = yl + 1;
	/* do bilinear interpolation */
	return ((k_GetPixel8bp(xl,yl,(*pInfo))*((float)xh-x) + k_GetPixel8bp(xh,yl,(*pInfo))*(x-(float)xl))*((float)yh-y) +
	 (k_GetPixel8bp(xl,yh,(*pInfo))*((float)xh-x) + k_GetPixel8bp(xh,yh,(*pInfo))*(x-(float)xl))*(y-(float)yl));
}


/************************************************/
/********         Old functions          ********/
/***** only kept for backward compatibility *****/
/************************************************/

double k_RadialCalibAndGetMeanErrorOld(T3D2DPoints *pPointSets,double k,const TImage* pImg)
{
	unsigned int i;
	double ReturnValue;
	TCameraMatrix Mat;
	T3D2DPoints NewPointSets; /* used to hold the new Ccd 2D coordinates */
	k_Init3D2DPoints(&NewPointSets);
	k_Copy3D2DPoints(&NewPointSets,pPointSets);
	for(i=0; i<pPointSets->NumberOfSets;i++)
	{
		T2DFloat P2d;
		P2d=k_GetPosRadRemovedOld(pPointSets->pSet[i].Pnt2D.x,pPointSets->pSet[i].Pnt2D.y,pImg,k);
		k_SetPoint2DIn3D2DPoints(&P2d,i,&NewPointSets);
	}
	k_CalibrateCamera(&NewPointSets,&Mat);
	ReturnValue=k_GetMeanError(&NewPointSets,&Mat);
	k_Empty3D2DPoints(&NewPointSets);
	return ReturnValue;
}

#define CALIB_R 0.61803399
#define CALIB_C (1.0-CALIB_R)
#define CALIB_SHFT2(a,b,c) do { (a)=(b); (b)=(c); } while (0)
#define CALIB_SHFT3(a,b,c,d) do { (a)=(b); (b)=(c); (c)=(d); } while (0)
double k_CalibGoldenSearchOld(double ax,double bx,double cx,double tol,double *xmin,
						  T3D2DPoints* pPointSets,const TImage* pImg)
{
	double f1,f2,x0,x1,x2,x3;
	x0=ax;
	x3=cx;
	if (fabs(cx-bx)>fabs(bx-ax))
	{
		x1=bx;
		x2=bx+CALIB_C*(cx-bx);
	}
	else
	{
		x2=bx;
		x1=bx-CALIB_C*(bx-ax);
	}
	f1=k_RadialCalibAndGetMeanErrorOld(pPointSets,x1,pImg);
	f2=k_RadialCalibAndGetMeanErrorOld(pPointSets,x2,pImg);
	while (fabs(x3-x0) > tol*(fabs(x1)+fabs(x2)))
	{
		if (f2<f1)
		{
			CALIB_SHFT3(x0,x1,x2,CALIB_R*x1+CALIB_C*x3);
			CALIB_SHFT2(f1,f2,k_RadialCalibAndGetMeanErrorOld(pPointSets,x2,pImg));
		}
		else
		{
			CALIB_SHFT3(x3,x2,x1,CALIB_R*x2+CALIB_C*x0);
			CALIB_SHFT2(f2,f1,k_RadialCalibAndGetMeanErrorOld(pPointSets,x1,pImg));
		}
	}
	if (f1<f2)
	{
		*xmin=x1;
		return f1;
	}
	else
	{
		*xmin=x2;
		return f2;
	}
}

bool k_CalibrateWithRadialDistOld(T3D2DPoints* pPointSets,TCameraMatrix* pMat, 
							   double* k,const TImage* pImg)
{
	if ((pPointSets==NULL) || (pMat==NULL) || (k==NULL) || (pImg==NULL))
	{
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_CalibrateWithRadialDist() One of the given parameters is a NULL pointer");
		return false;
	}
	/*	do not accept a T3D2DPoints structure with entries
		where either the 2D or 3D point is not in use */
	else if (pPointSets->NumberOfSets!=k_GetTotal3D2DPointSetsInUse(pPointSets))
	{
		k_ShowMessageSimple(
			IPL_ERROR,
			"k_CalibrateWithRadialDist() The T3D2DPoints structure containes entries "
			"where either the 2D or 3D point is not used");
		return false;
	}
	/* check if there are at least 6 point sets */
	else if (pPointSets->NumberOfSets<6)
	{
		k_ShowMessageStd(
			IPL_POSITION, IPL_ERROR,
			"k_CalibrateWithRadialDist() Need 6 refernce marks (current is %d)",
			pPointSets->NumberOfSets);
		return false;
	}
	else
	{
		const double LOWER_VAL=0;
		const double UPPER_VAL=0.000005; /* 5e-6 since version 1.60*/
		/*const double UPPER_VAL=0.000001;*/ /* 1e-6 */
		const unsigned int ITERATIONS=100;
		unsigned int i;
		unsigned int iterate=0;
		double LowValError,UpperValError,NewError;
		double LowVal=LOWER_VAL;
		double UpperVal=UPPER_VAL;
		double NewVal;
		T3D2DPoints NewPointSets;
		k_Init3D2DPoints(&NewPointSets);
		/* initialize system */
		LowValError=k_RadialCalibAndGetMeanErrorOld(pPointSets,LowVal,pImg);
		UpperValError=k_RadialCalibAndGetMeanErrorOld(pPointSets,UpperVal,pImg);

		/* search for a "bracketing minimum" error between 0 and UPPER_VAL,
			when found a golden search method is used for finding the minimum
			error between this bracketed interval */
		for(iterate=0;iterate<ITERATIONS;iterate++)
		{
			NewVal=(LowVal+UpperVal)/2.0;
			NewError=k_RadialCalibAndGetMeanErrorOld(pPointSets,NewVal,pImg);
		
			if ((NewError>LowValError) && (NewError<UpperValError))
			{
				UpperVal=NewVal;
				UpperValError=NewError;
			}
			else if ((NewError<LowValError) && (NewError>UpperValError))
			{
				LowVal=NewVal;
				LowValError=NewError;
			}
			else
			{	
				/* if we end up here, we have a bracketed minimum, 
					that is: NewError<LowValError and NewError<UpperValError */
				double Tolerance=1e-8;
				/*double test=*/
                                k_CalibGoldenSearchOld(LowVal,NewVal,UpperVal,Tolerance,&NewVal,
						  pPointSets,pImg);
				break;

			}
		}
		if (iterate>=ITERATIONS)
		{
			/* a bracketed minimum was never found, warn the user */
			k_ShowMessageSimple(
				IPL_WARNING,
				"k_CalibrateWithRadialDist() Couldn't find a suitable"
				" distortion value k better than the initial 0 (no radial correction)");
		}

		*k=NewVal;
		k_Copy3D2DPoints(&NewPointSets,pPointSets);
		for(i=0; i<pPointSets->NumberOfSets;i++)
		{
			T2DFloat P2d;
			P2d=k_GetPosRadRemovedOld(pPointSets->pSet[i].Pnt2D.x,pPointSets->pSet[i].Pnt2D.y,pImg,*k);
			k_SetPoint2DIn3D2DPoints(&P2d,i,&NewPointSets);
		}
		k_CalibrateCamera(&NewPointSets,pMat);
		k_Empty3D2DPoints(&NewPointSets);
		return true;
	}
}

void k_Calc3DTo2DRadOld(T3DFloat w, const TCameraMatrix* pMat, double k, T2DFloat* pPnt, const TImage* pImg)
{
	k_Calc3DTo2D(w,pMat,pPnt);
	/* inverse adjust for radial distortion */
	(*pPnt)=k_InverseRadialMove(pPnt->x-(pImg->Width/2.0),pPnt->y-(pImg->Height/2.0),k);
	pPnt->x+=(float)(pImg->Width/2.0);
	pPnt->y+=(float)(pImg->Height/2.0);
}

T2DInt k_GetMaxRadialDisplacementOld(const TImage* pImg, double k)
{
	T2DInt ImageCenter;
	float u,v;
	T2DInt MaxRadDispl;
	ImageCenter.x=(short)/*k_Round*/(pImg->Width/2.0);
	ImageCenter.y=(short)/*k_Round*/(pImg->Height/2.0);
	u=(float)(pImg->Width-ImageCenter.x);
	v=(float)(pImg->Height-ImageCenter.y);
	MaxRadDispl.x=k_Round(k*u*(u*u+v*v));
	MaxRadDispl.y=k_Round(k*v*(u*u+v*v));
	return MaxRadDispl;
}

T2DFloat k_GetPosRadRemovedOld(FLOAT32 x,FLOAT32 y,const TImage* pImg, double k)
{
	T2DFloat NewPos;
	FLOAT32 u=(FLOAT32)(x-pImg->Width/2.0);
	FLOAT32 v=(FLOAT32)(y-pImg->Height/2.0);
	NewPos.x=x;
	NewPos.y=y;
	NewPos.x+=(FLOAT32)k*u*(u*u+v*v);
	NewPos.y+=(FLOAT32)k*v*(u*u+v*v);
	return NewPos;
}

T2DFloat k_GetPosRadRemovedCOld(FLOAT32 x,FLOAT32 y, FLOAT32 CenterX, FLOAT32 CenterY, double k)
{
	T2DFloat NewPos;
	FLOAT32 u=(FLOAT32)(x-CenterX);
	FLOAT32 v=(FLOAT32)(y-CenterY);
	NewPos.x=x;
	NewPos.y=y;
	NewPos.x+=(FLOAT32)k*u*(u*u+v*v);
	NewPos.y+=(FLOAT32)k*v*(u*u+v*v);
	return NewPos;
}

T2DFloat k_GetPosInverseRadOld(FLOAT32 xr, FLOAT32 yr, const TImage* pImg, double k)
{
	T2DFloat NewPos;
	FLOAT32 CenterX=(FLOAT32)(pImg->Width/2.0);
	FLOAT32 CenterY=(FLOAT32)(pImg->Height/2.0);
	NewPos=k_InverseRadialMove(((double)xr)-CenterX,((double)yr)-CenterY,k);
	NewPos.x+=CenterX;
	NewPos.y+=CenterY;
	return NewPos;
}

T2DFloat k_GetPosInverseRadCOld(FLOAT32 xr, FLOAT32 yr, FLOAT32 CenterX, FLOAT32 CenterY, double k)
{
	T2DFloat NewPos;
	NewPos=k_InverseRadialMove(((double)xr)-CenterX,((double)yr)-CenterY,k);
	NewPos.x+=CenterX;
	NewPos.y+=CenterY;
	return NewPos;
}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
