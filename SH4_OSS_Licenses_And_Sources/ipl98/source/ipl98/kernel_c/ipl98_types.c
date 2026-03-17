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

/*	Basic typedefinitions (global), constructors and destructors
	for structs that needs to be initialised.
	@version 0.85,  last updated 4/13/1999
	@author René Dencker Eriksen edr@mip.sdu.dk
*/

#include "../ipl98_setup.h"
#include "ipl98_types.h"
#include "kernel_string.h"
#include "kernel_error.h"
#include <string.h>

#ifdef _IPL98_USING_CPP
namespace ipl{ /* use namespace if C++ */
extern "C" { /* prevent name mangling for C functions */
#endif

void k_GetOriginString(ORIGIN Origin,char** Name)
{
	int StrSize;
	char* strOrigin[20];
	switch(Origin){
	case CAMERA:
		*strOrigin="CAMERA\0";
		break;
	case DISC:
		*strOrigin="DISC\0";
		break;
	case RAM:
		*strOrigin="RAM\0";
		break;
	case EMPTY:
		*strOrigin="EMPTY\0";
		break;
	default:
		{
			k_ShowMessageSimple(IPL_ERROR, "k_PrintOrigin() Unknown origin (should not be possible!)");
			*strOrigin="UNKNOWN?\0";
		}
	}
	StrSize=(strlen(*strOrigin)+1)*sizeof(char);
	*Name=(char*)malloc(StrSize);
	memcpy(*Name,*strOrigin,StrSize);
	/*printf("*strOrigin=%s strlen(*strOrigin)=%d *Name=%s\n",*strOrigin,strlen(*strOrigin),*Name);*/
}

void k_PrintOrigin(ORIGIN Origin)
{
	char* Name;
	k_GetOriginString(Origin,&Name);
	printf("Origin of image: %s\n",Name);
	free(Name);
}

#ifdef _IPL98_USING_CPP
} /* end of extern "C" */
} /* end namespace ipl */
#endif
