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

#ifndef _TIME_DATE_H
#define _TIME_DATE_H

#include <string>
#include <time.h>

/** @ingroup utilities
@{ */

/** CTimeDate Timer functionality and time and date functions,
	last updated 11/10/2005.
	Get actual time and date (24 hours). Use the timer to calculate precise
	time elapsed (down to 1/100 seconds)

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CTimeDate time/time_date.h
	@version 0.99,  by edr@mip.sdu.dk
	@author René Dencker Eriksen */

class CTimeDate
{
	private: // attributes
		bool m_TimerStarted; // defaults to false
		clock_t m_Timer; // keeps track of timer
		bool m_TimerHoldStarted;
		clock_t m_TimerHold; // keep track of the time elapsed in a hold period
	public: // methods
		/// default constructor
		CTimeDate();
		/// default destructor
		~CTimeDate();
		/** Returns the actual time in the parameter "str" in
			the format: HH:MM:SS. Deletes previous allocated memory for Time (if any)
			and allocates new memory.
			@return false if failed, otherwise true */
		bool GetTime(std::string& str);
		/** Returns the actual date in the parameter "str" in
			the format: 'Weekday Month Day' or 'Weekday Month Day Year'
			depending on the parameter AddYear.
			@param AddYear If true, the returned date format will include year
				at the end, see general description for this method. Defaults
				to false.
			@return false if failed, otherwise true */
		static bool GetDate(std::string& str, bool AddYear=false);
		/** Returns the full date, time and year in the parameter "DTY" in
			the format: Weekday Month Day HH:MM:SS Year. Deletes previous
			allocated memory for Time (if any) and allocates new memory.
			@return false if failed, otherwise true */
		static bool GetDateTimeYear(std::string& DTY);
		/** Same as GetDateTimeYear(std::string& DTY), except the result
			is returned directly as a string. If it fails, the return value
			is "CTimeDate::GetDateTimeYear() ERROR ". */
		static std::string GetDateTimeYear();

		/** Starts the timer If timer already started, nothing is done.
			@return false if the timer was already started */
		bool StartTimer();
		/** Stops the timer. Only possible if the timer is started with a
			call to StartTimer(). Otherwise does nothing.
			@return Timer value in seconds, -1 if timer was not started */
		double StopTimer();
		/** Sometimes it may be usefull to hold the timer 
			(for example if the program waits for user input) without
			receiving a result. Release it again with a call to TimerContinue()
			@return false if timer was already on hold, then doing nothing */
		bool TimerHold();
		/** Makes the timer run again after a call to StopTimer()
			@return false if timer was not on hold, then doing nothing */
		bool TimerContinue();
		/** Returns the timer value but don't stop it!
			@return Timer value in seconds */
		double GetTimer();
		/** Converts seconds to the H--H:MM:SS format as in GetTime(), but with arbitrary
			number of ciffers to represent the hours. Result placed in "str".
			@param Seconds The value to be converted
			@param str Result is placed in here */
		void SecondsToHMS(unsigned long Seconds, std::string& str);
		/** Same as method SecondsToHMS(unsigned long Seconds, string& str), except the 
			result is returned directly as a string. A bit slower but can be convenient.
			@param Seconds The value to be converted
			@return string containing the time as H--H:MM:SS  */
		std::string SecondsToHMS(unsigned long Seconds);
};

/*@}*/ /* end group "utilities" */

#endif /* _TIME_DATE_H */
