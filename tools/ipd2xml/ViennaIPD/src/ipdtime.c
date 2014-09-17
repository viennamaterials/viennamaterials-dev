/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

#include "ipdtime.h"

/** @file ipdtime.c
    @brief Provides definitions for accessing some time information
*/


static ipdInt MonthLength[2][13] = {
{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
{ 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

#define IsLeapYear(year)	\
(((year) % 4 == 0) && (((year) % 100 != 0) || ((year) % 400) == 0))


#if defined(WINNT)
#include "vossysP.h"

/** @brief Get timing information on a WINNT machine */
ipdVoid ipdvTimeUsage(ipdvTimeUsageStruct *usage)
{
   extern ipdLongWord winntUserChildren, winntKernelChildren;
   FILETIME kernel, user, dummy;
   
   GetProcessTimes(GetCurrentProcess(), &dummy, &dummy, &kernel, &user);
   usage->utime = (ipdLongWord)convertWindowsTimeToTimeUsage(&user);
   usage->stime = (ipdLongWord)convertWindowsTimeToTimeUsage(&kernel);
   usage->cutime = winntUserChildren;
   usage->cstime = winntKernelChildren;
   return;
}
#endif /* defined(WINNT) */

/** @brief Returns the sum of the user and system time used by this process in seconds as a double value. */
ipdDouble ipdvSecTime()
{
#if defined(WINNT)
   FILETIME kernel, user, dummy;
   ipdDouble value;
   
   GetProcessTimes(GetCurrentProcess(), &dummy, &dummy, &kernel, &user);
   value = (ipdDouble)convertWindowsTimeToUnix(&kernel) +
      (ipdDouble)convertWindowsTimeToUnix(&user);
   return(value);
#else
   ipdvTimeUsageStruct val;

   ipdvTimeUsage(&val);
   return(ipdvTimeSum(val));
#endif /* defined(WINNT) */
}

/** @brief Convert the given time information in a system time stamp (type ipdvTimeType) which can be used for time calculations. 

  Convert the given time information in a system time stamp (type ipdvTimeType) which can be used for time calculations. If gmt is ipdTRUE,
  the calculated time stamp is GMT else local time. This function overwrites the global stored value by vGMTime and vLocalTime.

  @param year The year
  @param month The month
  @param day The day
  @param hour The hour
  @param min The minute
  @param sec The second 
  @param gmt Use information as GMT
 */
ipdvTimeType ipdvMkTime(
	ipdLong year,		/* [I] the year */
	ipdLong month,		/* [I] the month */
	ipdLong day,		/* [I] the day */
	ipdLong hour,		/* [I] the hour */
	ipdLong min,		/* [I] the minute */
	ipdLong sec,		/* [I] the second */
	ipdBoolean gmt)		/* [I] use information as GMT */
{
   ipdvTimeType tr = (ipdvTimeType)0;
   ipdvTimeType distance;
   ipdInt i;
#define normalize(what,low,high,next) \
   do { while (what < low) next--, what += high; \
	while (what >= high) next++, what -= high; } while(0)

   /* convert all parameters into right range */
   normalize(sec, 0, 60, min);
   normalize(min, 0, 60, hour);
   normalize(hour, 0, 24, day);
   normalize(day, 1, MonthLength[IsLeapYear(year)][month], year);
   normalize(month, 1, 13, year);
#undef normalize

   distance = ipdvTime(ipdNULL);
   for (i = 0; (i < 10) && (distance != 0); i++) {
      ipdvTimeStruct *tp;
      ipdLong v1, v2;
      ipdInt diff = 0;

      tr += distance;
      tp = gmt ? ipdvGMTime(&tr) : ipdvLocalTime(&tr);

#define tdiff(l,r,sec) \
      do { v1 += l *sec; v2 += r * sec; \
	      if (diff == 0) diff = (l < r) ? -1 : ((l > r) ? 1 : 0); \
	} while(0)

      /* calculate difference of guessed time to real time */
      v1 = v2 = 0;
      diff = 0;

      /* Okay, not all years have 365 days. */
      tdiff(year, ipdvGetTimeYear(tp), 31536000);
      /* Okay, not all months have 30 days. */
      tdiff(month, ipdvGetTimeMonth(tp), 2592000);
      tdiff(day, ipdvGetTimeDay(tp), 86400);
      tdiff(hour, ipdvGetTimeHour(tp), 3600);
      tdiff(min, ipdvGetTimeMinute(tp), 60);
      tdiff(sec, ipdvGetTimeSecond(tp), 1);

#undef tdiff

      /* We should also make sure that the sign of DISTANCE is correct -- if
	 DIFF is positive, the distance should be positive and vice versa. */

      distance = (v1 > v2) ? (v1 - v2) : (v2 - v1);
      if (diff < 0)
	 distance = -distance;

      /* We need this DIFF business because it is forseeable that the
	 distance may be zero when, in actuality, the two structures are
	 different.  This is usually the case when the dates are 366 days apart
	 and one of the years is a leap year.  */

      if ((distance == 0) && (diff != 0))
	 distance = 86400 * diff;
   }
   return ((i == 10) ? (ipdvTimeType)0 : tr);
}
