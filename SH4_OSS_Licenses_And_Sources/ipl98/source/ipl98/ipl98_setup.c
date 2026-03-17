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

#include "ipl98_setup.h"

/************************************************/
/************************************************/
/********      IPL98 Setup area          ********/
/************************************************/
/************************************************/
#ifndef IPL_USER_SUPPLY_ERROR_FUNCTION
void AddIPL98ErrorHistory(const char* str)
{
	printf("%s\n",str);
}
#endif
