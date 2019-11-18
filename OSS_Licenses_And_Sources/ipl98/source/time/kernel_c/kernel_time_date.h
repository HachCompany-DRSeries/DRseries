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

#ifndef _KERNELTIMEDATE_H
#define _KERNELTIMEDATE_H
#include <time.h>

#if __cplusplus == 1
extern "C" { /* prevent name mangling for C functions */
#endif


/** @ingroup ansic ANSI-C Kernel code
    @{ */

/** @defgroup timedate ANSI-C TimeDate Timer and time/date functionality,
	last updated 11/10/2005.
	Get actual time and date (24 hours). Use the timer to calculate precise
	time elapsed (down to 1/1000 seconds on NT and 1/100 on Windows 95)
	These functions are part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	@version 0.98
	@author René Dencker Eriksen edr@mip.sdu.dk
    @{ */

/** Returns the actual time in the parameter "Time" in
	the format: HH:MM:SS. Deletes previous allocated memory for Time (if any)
	and allocates new memory.
	@return -1 if failed, otherwise 1. If failed the content of Time is left unchanged. */
int k_GetTime(char** Time);

/** Returns the actual date in the parameter "Date" in
	the format: 'Weekday Month Day' or 'Weekday Month Day Year'
	depending on the parameter AddYear. Deletes previous allocated
	memory for Date (if any) and allocates new memory.
	@param AddYear If true, the returned date format will include year
		at the end, see general description for this function.
	@return -1 if failed, otherwise 1 */
int k_GetDate(char** Date, bool AddYear);

/** Returns the full date, time and year in the parameter "DTY" in
	the format: Weekday Month Day HH:MM:SS Year. Deletes previous
	allocated memory for Time (if any) and allocates new memory.
	@return -1 if failed, otherwise 1 */
int k_GetDateTimeYear(char** DTY);

/** Starts the timer, the return value is the machine clock value. This
	return value must be used in a call to GetTimer() as parameter */
clock_t k_StartTimer();

/** Returns the time in seconds elapsed between a call to StartTimer() and this function.
	The parameter "StartValue" must be the value returned by StartTimer(). On a PC the
	precision is 1/1000 of a second on NT and 1/100 of a second on Windows 95 (as far as I know) */
double k_GetTimer(clock_t StartValue);

/** Converts seconds to the H--H:MM:SS format as in GetTime(), but with arbitrary
	number of ciffers to represent the hours. Result placed in "Time".
	Deletes previous allocated memory for Time (if any) and allocates new memory.
	@param Seconds The value to be converted
	@param Time Result is placed in here, memory is allocated */
void k_SecondsToHMS(unsigned long Seconds, char** Time);

/*@}*/ /* end group "timedate" */

/*@}*/ /* end group "ansic" */

#if __cplusplus == 1
} /* end of extern "C" */
#endif

#endif /* _KERNELTIMEDATE_H */
