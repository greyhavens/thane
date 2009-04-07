/* ***** BEGIN LICENSE BLOCK *****
* Version: MPL 1.1/GPL 2.0/LGPL 2.1
*
* The contents of this file are subject to the Mozilla Public License Version
* 1.1 (the "License"); you may not use this file except in compliance with
* the License. You may obtain a copy of the License at
* http://www.mozilla.org/MPL/
*
* Software distributed under the License is distributed on an "AS IS" basis,
* WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
* for the specific language governing rights and limitations under the
* License.
*
* The Original Code is [Open Source Virtual Machine.].
*
* The Initial Developer of the Original Code is
* Adobe System Incorporated.
* Portions created by the Initial Developer are Copyright (C) 1993-2006
* the Initial Developer. All Rights Reserved.
*
* Contributor(s):
*   Adobe AS3 Team
*
* Alternatively, the contents of this file may be used under the terms of
* either the GNU General Public License Version 2 or later (the "GPL"), or
* the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
* in which case the provisions of the GPL or the LGPL are applicable instead
* of those above. If you wish to allow use of your version of this file only
* under the terms of either the GPL or the LGPL, and not to allow others to
* use your version of this file under the terms of the MPL, indicate your
* decision by deleting the provisions above and replace them with the notice
* and other provisions required by the GPL or the LGPL. If you do not delete
* the provisions above, a recipient may use your version of this file under
* the terms of any one of the MPL, the GPL or the LGPL.
*
* ***** END LICENSE BLOCK ***** */

#include "avmshell.h"

#include <Mmsystem.h>

namespace avmplus
{
	int WeekDay(double t);
	double MakeDate(double day, double time);
	double MakeDay(double year, double month, double date);
	double MakeTime(double hour, double min, double sec, double ms);
	int YearFromTime(double t);
}

/*
* Windows implementation of platform-dependent date and time code
*/
static TIME_ZONE_INFORMATION gTimeZoneInfo;
static SYSTEMTIME gGmtCache;

static double kMsecPerDay       = 86400000;
static double kMsecPerHour      = 3600000;
static double kMsecPerSecond    = 1000;
static double kMsecPerMinute    = 60000;

static void UpdateTimeZoneInfo()
{
	SYSTEMTIME gmt;
	GetSystemTime(&gmt);
	if ((gmt.wMinute != gGmtCache.wMinute) ||
		(gmt.wHour != gGmtCache.wHour) ||
		(gmt.wDay != gGmtCache.wDay) ||
		(gmt.wMonth != gGmtCache.wMonth) ||
		(gmt.wYear != gGmtCache.wYear)
		) 
	{
		// Cache is invalid.
		GetTimeZoneInformation(&gTimeZoneInfo);
		gGmtCache = gmt;
	}
}

double VMPI_getLocalTimeOffset()
{
	UpdateTimeZoneInfo();
	return -gTimeZoneInfo.Bias * 60.0 * 1000.0;
}

static double ConvertWin32DST(int year, SYSTEMTIME *st)
{
	// The StandardDate and DaylightDate members of
	// TIMEZONE_INFORMATION may be specified in two ways:
	// 1. An absolute time and date
	// 2. A month, day of week and week in month, and a time of day

	if (st->wYear != 0) {
		return avmplus::MakeDate(avmplus::MakeDay(year,
			st->wMonth - 1,
			st->wDay),
			avmplus::MakeTime(st->wHour,
			st->wMinute,
			st->wSecond,
			st->wMilliseconds));
	}

	double timeValue = avmplus::MakeDate(avmplus::MakeDay(year,
		st->wMonth-1,
		1),
		avmplus::MakeTime(st->wHour,
		st->wMinute,
		st->wSecond,
		st->wMilliseconds));

	double nextMonth;
	if (st->wMonth < 12) {
		nextMonth = avmplus::MakeDate(avmplus::MakeDay(year, st->wMonth, 1), 0);
	} else {
		nextMonth = avmplus::MakeDate(avmplus::MakeDay(year + 1, 0, 1), 0);
	}

	int dayOfWeek = avmplus::WeekDay(timeValue);
	if (dayOfWeek != st->wDayOfWeek) {
		int dayDelta = st->wDayOfWeek - dayOfWeek;
		if (dayDelta < 0) {
			dayDelta += 7;
		}
		timeValue += (double)kMsecPerDay * dayDelta;
	}

	// Advance appropriate # of weeks
	timeValue += (double)kMsecPerDay * 7.0 * (st->wDay - 1);

	// Cap it at the end of the month
	while (timeValue >= nextMonth) {
		timeValue -= (double)kMsecPerDay * 7.0;
	}

	return timeValue;
}

double VMPI_getDaylightSavingsTA(double time)
{
	// On Windows, ask the OS what the daylight saving time bias
	// is.  If it's zero, perform no adjustment.
	UpdateTimeZoneInfo();
	if (gTimeZoneInfo.DaylightBias != -60 || gTimeZoneInfo.DaylightDate.wMonth == 0) {
		return 0;
	}

	// In most of the US, Daylight Saving Time begins on the
	// first Sunday of April at 2 AM.  It ends at 2 am on
	// the last Sunday of October.

	// 1. Compute year this time represents.
	int year = avmplus::YearFromTime(time);

	// 2. Compute time that daylight saving time begins
	double timeD = ConvertWin32DST(year, &gTimeZoneInfo.DaylightDate);

	// 3. Compute time that standard time begins
	double timeS = ConvertWin32DST(year, &gTimeZoneInfo.StandardDate);

	// Subtract the daylight bias from the standard transition time
	timeS -= kMsecPerHour;

	// The times we have calculated are in local time,
	// but "time" was passed in as UTC.  Convert it to local time.
	time += VMPI_getLocalTimeOffset();

	// Does time fall in the daylight saving period?

	// Where Daylight savings time is earlier in the year than standard time
	if(timeS > timeD)
	{
		if (time >= timeD && time < timeS) 
			return kMsecPerHour;
		else 
			return 0;
	}
	// Where Daylight savings time is later in the year than standard time
	else
	{
		if (time >= timeS && time < timeD) 
			return 0;
		else 
			return kMsecPerHour;
	}

}

#define FILETIME_EPOCH_BIAS ((LONGLONG)116444736000000000)
#define FILETIME_MS_FACTOR (10000.0)

static double NormalizeFileTime(FILETIME* ft)
{
	LARGE_INTEGER li;
	li.LowPart = ft->dwLowDateTime;
	li.HighPart = ft->dwHighDateTime;

	return ((double) (li.QuadPart - FILETIME_EPOCH_BIAS)) / FILETIME_MS_FACTOR;
}

static double NormalizeSystemTime(SYSTEMTIME* st)
{
	FILETIME ft;
	SystemTimeToFileTime(st, &ft);
	return NormalizeFileTime(&ft);
}

double VMPI_getDate()
{
	SYSTEMTIME stime;
	GetSystemTime(&stime);
	return NormalizeSystemTime(&stime);
}

uint64_t VMPI_getTime()
{	
	return timeGetTime();
}


void* VMPI_alloc(size_t size)
{
	return HeapAlloc(GetProcessHeap(), 0, size);
}

void VMPI_free(void* ptr)
{
	HeapFree(GetProcessHeap(), 0, ptr);
}

void VMPI_Log(const char* message)
{
	avmshell::Platform::GetInstance()->logMessage(message);
}
