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

#include "kernel_spectral.h"
#include "../kernel_error.h"
#include "../kernel_history.h"
#include <math.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

bool k_FFT(int isign, TComplexImage *pImg)
/*
{if 'isign'is +1 then this procedure replaces the
fourier picture z by its Fourier transform by using
fast Fourier transform. If 'isign' is -1 then the procedure
performs the reverse transformation apart from the normalization
factor }*/
{
	double PowerX,PowerY;
	if (pImg->Origin!=EMPTY)
	{
		unsigned long int i1,i2,i3,i2rev,i3rev,ibit,idim,
			ip1,ip2,ip3,ifp1,ifp2,k1,k2,n,
			w2,w21,pw2,x1,x2,x3,x3rev,y1,y2,y3,y3rev,
			ii1,ii2,ii3,nprev,nrem,ntot;
		FLOAT32 tempi,tempr,theta,wi,wpi,wpr,wr,wtemp;
		PowerX=log((double)pImg->Width)/log(2.0);
		PowerY=log((double)pImg->Height)/log(2.0);
		if (((int)(PowerX+0.00001)-PowerX<0.00001) &&
			((int)(PowerY+0.00001)-PowerY<0.00001))
		{
#ifdef IPL_ADD_HISTORY
			/* add comment to history */
			{
				k_PrintfAppendTextIPL(
					&(pImg->History),
					"%sk_FFT() isign=%d",
					ipl_HistoryIndent, isign);
				k_IplStartHistoryMacro();
			}
#endif
			w2=2*pImg->Width;
			w21=w2-1;
			pw2=(unsigned long) PowerX+1;
			ntot=pImg->Width*pImg->Height;
			nprev = 1;
			for (idim=1;idim<=2;idim++)
			{
				if (idim==1)
					n=pImg->Width; 
				else 
					n=pImg->Height;
				nrem = ntot/(n*nprev);
				ip1 = 2*nprev;
				ip2 = ip1*n;
				ip3 = ip2*nrem;
				i2rev = 1;
				for (ii2= 0;ii2<=(ip2-1)/ip1;ii2++)
				{
					i2 = 1+ii2*ip1;
					if(i2 < i2rev)
					{
						for (ii1=0;ii1<=(ip1-2)>>1;ii1++)
						{
							i1 = i2+ii1*2;
							for (ii3=0;ii3<=(ip3-i1)/ip2;ii3++)
							{
								i3 = i1+ii3*ip2;
								i3rev = i2rev+i3-i2;
								x3=((i3-1)&w21)>>1;
								y3=(i3-1)>>pw2;
								x3rev=((i3rev-1)&w21)>>1;
								y3rev=(i3rev-1)>>pw2;
								tempr=pImg->ppMatrixRe[y3][x3];
								tempi=pImg->ppMatrixIm[y3][x3];
								pImg->ppMatrixRe[y3][x3]= pImg->ppMatrixRe[y3rev][x3rev];
								pImg->ppMatrixIm[y3][x3]= pImg->ppMatrixIm[y3rev][x3rev];
								pImg->ppMatrixRe[y3rev][x3rev]= tempr;
								pImg->ppMatrixIm[y3rev][x3rev]= tempi;
							}
						}
					}
					ibit = ip2/2;
					while ((ibit >= ip1) && (i2rev > ibit))
					{
						i2rev = i2rev-ibit;
						ibit = ibit>>1;
					}
					i2rev = i2rev+ibit;
				}
				ifp1 = ip1;
				while (ifp1 < ip2)
				{
					ifp2 = 2*ifp1;
					theta = (FLOAT32)(isign*6.283185/(ifp2/ip1));
					wpr = (FLOAT32)(-2.0*sin(0.5*theta)*sin(0.5*theta));
					wpi = (FLOAT32)(sin(theta));
					wr = 1.0;
					wi = 0.0;
					for (ii3=0;ii3<=(ifp1-1)/ip1;ii3++)
					{
						i3 = 1+ii3*ip1;
						for (ii1=0;ii1<=(ip1-2)>>1;ii1++)
						{
							i1 = i3+ii1*2;
							for (ii2=0;ii2<=(ip3-i1)/ifp2;ii2++)
							{
								i2 = i1+ii2*ifp2;
								k1 = i2;
								k2 = k1+ifp1;
								x1=((k1-1)&w21)>>1;
								y1=((k1-1)>>pw2);
								x2=((k2-1)&w21)>>1;
								y2=((k2-1)>>pw2);
								tempr = wr*pImg->ppMatrixRe[y2][x2]
									-wi*pImg->ppMatrixIm[y2][x2];
								tempi = wr*pImg->ppMatrixIm[y2][x2]
									+wi*pImg->ppMatrixRe[y2][x2];
								pImg->ppMatrixRe[y2][x2]=pImg->ppMatrixRe[y1][x1]-tempr;
								pImg->ppMatrixIm[y2][x2]=pImg->ppMatrixIm[y1][x1]-tempi;
								pImg->ppMatrixRe[y1][x1]+=tempr;
								pImg->ppMatrixIm[y1][x1]+=tempi;
							}
						}
						wtemp = wr;
						wr = wr*wpr-wi*wpi+wr;
						wi = wi*wpr+wtemp*wpi+wi;
					}
					ifp1 = ifp2;
				}
				nprev = n*nprev;
			}
			if (isign==-1)
			{ 
				for (x1=0;x1<pImg->Width;x1++)
				{
					for (y1=0;y1<pImg->Height;y1++)
					{
						pImg->ppMatrixRe[y1][x1]/=ntot;
						pImg->ppMatrixIm[y1][x1]/=ntot;
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
		k_ShowMessageSimple(IPL_ERROR, "k_FFT(): Height or width of Complex image is not a power of 2");
		return false;
	}
} 
  else
  {
	  k_ShowMessageSimple(IPL_ERROR, "k_FFT(): Image is empty");
	  return false;
  }
}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
