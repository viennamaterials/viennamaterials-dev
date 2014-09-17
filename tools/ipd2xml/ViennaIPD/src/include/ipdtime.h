/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

CONTENT:  Time related functions

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

#ifndef IPDTIME_H
#define IPDTIME_H

/** @file ipdtime.h
    @brief Provides declarations for accessing some time information

*/

/*********** Note: Doxygen comments are found in the .c files! ***************/

#include "ipdtype.h"

/***S vTime */

/* define all needed types */
typedef time_t		ipdvTimeType;
typedef struct tm	ipdvTimeStruct;

/***M5 {\bf vTime} returns the time since 1st Jan. 1970 in the variable
    {\bf tval} of type {\bf ipdvTimeType}. If a Null pointer is used as
    parameter, no value is stored. \\
    {\bf vCTime} returns a pointer to an internal buffer with the
    converted time {\bf tval}, which is given as a pointer, which can be
    used as read only string. \\
    {\bf vGMTime} returns a pointer to a read only struct with the time
    information. The time is converted into GMT0.
    {\bf vLocaltime} returns a pointer to a read only struct with the
    local corrected time (TIMEZONE). */
/***E Time Access Functions */
#define ipdvTime(tval)		(ipdvTimeType)(time(tval))
#define ipdvCTime(tval)		(ipdString)(ctime(tval))
#define ipdvGMTime(tval)		(ipdvTimeStruct *)(gmtime(tval))
#define ipdvLocalTime(tval)	(ipdvTimeStruct *)(localtime(tval))

_BEGIN_CPLUSPLUS
/* converts a time information into a ipdvTimeType */
IPDVBSEXTERN extern ipdvTimeType ipdvMkTime(ipdLong year, ipdLong month, ipdLong day,
			 ipdLong hour, ipdLong min, ipdLong sec, ipdBoolean gmt);

/* get time in seconds (plus microseconds) */
IPDVBSEXTERN extern ipdDouble ipdvSecTime(voidArg);

/***M8 {\bf vTime<type>} allows access to the different fields in the {\bf
    ipdvTimeStruct} field. */
/***E Fields of Time Structure */
#define ipdvGetTimeYear(tp)	(ipdInt)((tp -> tm_year) + 1900)
#define ipdvGetTimeMonth(tp)	(ipdInt)((tp -> tm_mon) + 1)
#define ipdvGetTimeDay(tp)		(ipdInt)(tp -> tm_mday)
#define ipdvGetTimeHour(tp)	(ipdInt)(tp -> tm_hour)
#define ipdvGetTimeMinute(tp)	(ipdInt)(tp -> tm_min)
#define ipdvGetTimeSecond(tp)	(ipdInt)(tp -> tm_sec)

/***M {\bf ipdvTimeUsage} returns the used system/user time by the current
    process and it's children in the struct {\bf ipdvTimeUsageStruct} in
    units of {\sl HZ}. */
#if defined(vms) || defined(__vms)
#  define ipdvTimeUsage(val)	(ipdVoid)(times(val))
typedef struct tbuffer      	ipdvTimeUsageStruct;
#endif /* defined(vms) || defined(__vms) */
#if defined(unix) || defined(__unix)
#  define ipdvTimeUsage(val)	(ipdVoid)(times(val))
typedef struct tms      	ipdvTimeUsageStruct;
#endif /* defined(unix) || defined(__unix) */



#if !defined(HZ)
#  if defined(CLK_TCK)
#    define HZ CLK_TCK
#  else
#    define HZ 60		/* default value, may be wrong */
#  endif /* defined(CLK_TCK) */
#endif /* !defined(HZ) */


#if defined(unix) || defined(__unix)
#define ipdvTimeSum(usage) \
   ((ipdDouble)(usage.tms_utime + usage.tms_stime) / (ipdDouble)HZ)
#endif /* defined(unix) || defined(__unix) */

_END_CPLUSPLUS

#endif /* _vtimeINCLUDED_ */
