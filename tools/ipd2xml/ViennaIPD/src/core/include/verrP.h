/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

CONTENT:  Private header file for the global error message
          and error handling system

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  SH   Stefan Halama,     Institute for Microelectronics, TU Vienna

  who when        what
-------------------------------------------------------------------------
  SH   1992-08-27  New from scratch
  SH   1992-11-05  included errno and errno processing by standard handlers
=======================================================================*/

/** @file verrP.h
    @brief Private header file for the global vienna error message and error handling system
*/


#ifndef _IPDverrpINCLUDED_
#define _IPDverrpINCLUDED_

#include "verr.h"
#include "vestring.h"

#define ipdVeModule(e) 		((e)->module)
#define ipdVeFileName(e) 		((e)->fileName)
#define ipdVeRcsID(e) 		((e)->rcsID)
#define ipdVeLineNo(e) 		((e)->lineNo)
#define ipdVeFunctionName(e) 	((e)->functionName)
#define ipdVeSeverity(e) 		((e)->severity)
#define ipdveClass(e) 		((e)->errclass)
#define ipdVeMessage(e) 		((e)->message)
#define ipdVeSerial(e) 		((e)->serial)
#define ipdVeId(e) 		((e)->id)
#define ipdVeBackTraceStart(e)	((e)->bt)
#define ipdVeErrno(e)		((e)->ErrNo)
#define ipdVeLanguage(e)		((e)->language)

#define ipdVeSeverityString(s) ipdveSeverityStrings[s]
#define ipdVeClassString(c) ipdveClassStrings[c]

/* this string is used to identify unspecified string values */
#define ipdveUNDEFINED	"*undefined*"

IPDVBSEXTERN extern ipdConstString ipdveSeverityStrings[];
IPDVBSEXTERN extern ipdConstString ipdveClassStrings[];
IPDVBSEXTERN extern ipdBoolean _ipdveDebugMessagesEnabled;

/***TE defines the return value of an error handler. Each called error
    handler should return one of these values. */
typedef enum veErrHanStatus_e
{
   ipdveIGIPDNORED 	= 0, /*** error ignored by error handler and may be passed
			  to other ones */
   ipdvePROCESSED	= 1, /*** error processed by error handler and may be passed
			  to other ones */
   ipdveTRAPPED	= 2  /*** error trapped by error handler and will not passed
			  to other ones */
} ipdveErrHanStatus;

/***E defines the language in which the error occurred */
typedef enum veLanguage_e
{
   ipdveLANGUAGE_UNKIPDNOWN,
   ipdveLANGUAGE_C,
   ipdveLANGUAGE_LISP,
   ipdveLANGUAGE_FORTRAN
} ipdveLanguage;


/* this is the C structure which represents an vienna error */
struct ipdveError_s
{
   ipdConstString	module;		/* name of the module */
   struct _ipdbacktrace_ *bt;	/* The start of the backtrace list */
   ipdConstString	fileName;	/* name of the source file __FILE__ */
   ipdConstString	rcsID;		/* rcs ID of respective file */
   ipdInt		lineNo;		/* line number in that file */
   ipdConstString	functionName;	/* function name (if given) */
   ipdveSeverity	severity;	/* severity level */
   ipdveClass	errclass;	/* error class (SEMANTICS!) */
   ipdConstString	message;	/* sprintf-ed message string */
   ipdInt		serial;		/* serial number of error */
   ipdInt		id;		/* optional error id */
   int		ErrNo;		/* The Ansi-C/OS errno */
   ipdveLanguage   language;	/* error occured in language */
};
typedef struct ipdveError_s ipdveError;


_BEGIN_CPLUSPLUS
/* this is the C structure which stores an error handler */
typedef struct ipdveErrHan_s
{
   ipdConstString		name;
   ipdveErrHanStatus	(*handler)(
#if !ipdHasFunctionStructBug
				   struct ipdveErrHan_s *han, ipdVoid *hData, ipdveErrorPtr err
#endif /* !ipdHasFunctionStructBug */
				   );
   ipdBoolean		active;
   ipdBoolean		grab;
   ipdveClassMask		classMask;
   ipdveSeverity		activationLevel;
   ipdveSeverity		brevityLevel;
   ipdVoid		*handlerData;
   struct ipdveErrHan_s	*next;
   struct ipdveErrHan_s	*previous;
} ipdveErrHan;

typedef struct ipdveErrHan_s 	*ipdveErrHanPtr;


/* this function is secret - only to be used by error transmitters */
IPDVBSEXTERN extern ipdVoid ipdveDispatchError(ipdveErrorPtr err);
_END_CPLUSPLUS

#endif /* _verrpINCLUDED_ */
