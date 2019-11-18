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

#ifndef _TIME_DATE_CPP
#define _TIME_DATE_CPP

#include "time_date.h"
#include "kernel_c/kernel_time_date.h"
#include "../ipl98/cpp/error.h"
#include <iostream>
#include <sstream>

using std::ostringstream;

CTimeDate::CTimeDate()
{
	// default constructor
	m_TimerStarted=false;
	m_TimerHoldStarted=false;
}

CTimeDate::~CTimeDate()
{
	// default destructor
}

bool CTimeDate::GetTime(std::string& str)
{
	char* Time=NULL;
	if (k_GetTime(&Time)==-1)
	{
		if (Time!=NULL)
			free(Time);
		return false;
	}
	else
	{
		str.assign(Time);
		if (Time!=NULL)
			free(Time);
		return true;
	}
}

bool CTimeDate::GetDate(std::string& str, bool AddYear)
{
	char* Date=NULL;
	if (k_GetDate(&Date,AddYear)==-1)
	{
		if (Date!=NULL)
			free(Date);
		return false;
	}
	else
	{
		str.assign(Date);
		if (Date!=NULL)
			free(Date);
		return true;
	}
}

bool CTimeDate::GetDateTimeYear(std::string& DTY)
{
	char* pDTY=NULL;
	if (k_GetDateTimeYear(&pDTY)==-1)
	{
		if (pDTY!=NULL) // not tested
			free(pDTY);
		return false;
	}
	else
	{
		DTY.assign(pDTY);
		DTY.resize(DTY.size()-1);
		if (pDTY!=NULL) // not tested
			free(pDTY);
		return true;
	}
}

std::string CTimeDate::GetDateTimeYear()
{
	std::string DTY;
	if (GetDateTimeYear(DTY)==false)
		DTY.assign("CTimeDate::GetDateTimeYear() ERROR ");
	return DTY;
}

bool CTimeDate::StartTimer()
{
	if (m_TimerStarted==true)
	{
		ostringstream ost;
		ost << "CTimeDate::StartTimer() Timer already started - doing nothing" << IPLAddFileAndLine;
		ipl::CError::ShowMessage(ipl::IPL_WARNING,ost);
		return false;
	}
	else
	{
		m_TimerStarted=true;
		m_Timer=k_StartTimer();
		return true;
	}
}

double CTimeDate::StopTimer()
{
	if (m_TimerStarted==false)
	{
		ostringstream ost;
		ost << "CTimeDate::StopTimer() Timer not started - doing nothing" << IPLAddFileAndLine;
		ipl::CError::ShowMessage(ipl::IPL_WARNING,ost);
		return -1.0;
	}
	else
	{
		m_TimerStarted=false;
		m_TimerHoldStarted=false;
		return k_GetTimer(m_Timer);
	}
}

bool CTimeDate::TimerHold()
{
	if(m_TimerHoldStarted==true)
	{
		ostringstream ost;
		ost << "CTimeDate::TimerHold() Timer already on hold - doing nothing" << IPLAddFileAndLine;
		ipl::CError::ShowMessage(ipl::IPL_WARNING,ost);
		return false;
	}
	else
	{
		m_TimerHoldStarted=true;
		m_TimerHold=k_StartTimer();
		return true;
	}
}

bool CTimeDate::TimerContinue()
{
	if(m_TimerHoldStarted==false)
	{
		ostringstream ost;
		ost << "CTimeDate::TimerContinue() Timer not on hold - doing nothing" << IPLAddFileAndLine;
		ipl::CError::ShowMessage(ipl::IPL_WARNING,ost);
		return false;
	}
	else
	{
		m_TimerHoldStarted=false;
		m_Timer += (clock()-m_TimerHold);
		return true;
	}
}

double CTimeDate::GetTimer()
{
	if (m_TimerStarted==false)
	{
		ostringstream ost;
		ost << "CTimeDate::GetTimer() Timer not started - returning -1" << IPLAddFileAndLine;
		ipl::CError::ShowMessage(ipl::IPL_WARNING,ost);
		return -1.0;
	}
	else
	{
		if (m_TimerHoldStarted==true)
		{
			return k_GetTimer(m_Timer + (clock()-m_TimerHold));
		}
		else
		{
			return k_GetTimer(m_Timer);
		}
	}
}

void CTimeDate::SecondsToHMS(unsigned long Seconds, std::string& str)
{
	char* Time=NULL;
	k_SecondsToHMS(Seconds,&Time);
	str.assign(Time);
	delete Time;
}

std::string CTimeDate::SecondsToHMS(unsigned long Seconds)
{
	char* Time=NULL;
	k_SecondsToHMS(Seconds,&Time);
	std::string str(Time);
	delete Time;
	return str;
}

#endif //_TIME_DATE_CPP
