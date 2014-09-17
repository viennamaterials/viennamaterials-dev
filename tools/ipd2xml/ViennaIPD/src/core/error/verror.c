/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

static char const _rcsid_ [] = "$Id: verror.c$";

/** @file verror.c
    @brief Main error handling file extracted from the deprecated Vienna Base System

    Main error handling file extracted from the deprecated Vienna Base System
   Note: With just a very few exceptions, the error system functions contained in
   this file (and other error system code) MAY NOT MAKE USE of the error
   system themselfes - so don't look for ipdVeStart, ipdVeReturn, etc.. - and
   don't bother if you miss them 
*/

#include "vexit.h"
#include "verr.h"
#include "verrP.h"
#include "../../include/ipdstring.h"

/* externally accessible variables (public, global variables) -------------- */

#if ipdVeBackTrace
struct _ipdbacktrace_ *_ipdbacktraceroot_ = (struct _ipdbacktrace_ *)ipdNULL;
#endif


ipdConstString ipdveClassStrings[ipdveNUM_CLASSES] =
{
  /** ipdveTDB */ "Test/Debug",
  /** ipdveSYS */ "Operating system call",
  /** ipdveSIG */ "Caught signal",
  /** ipdveALG */ "Internal algorithmic fault",
  /** ipdveEXT */ "External algorithmic fault",
  /** ipdveARG */ "Argument fault",
  /** ipdveDAT */ "(Input) data fault",
  /** ipdveCNF */ "Configuration/Installation fault",
  /** ipdveLIM */ "Limits violated",
  /** ipdveNUM */ "Numerical fault",
  /** ipdveUNI */ "Unimplemented feature",
  /** ipdveFCF */ "Function call failed",
  /** ipdveFOR */ "Foreign",
  /** ipdveUSR */ "User interaction",
};

ipdConstString ipdveSeverityStrings[ipdveNUM_LEVELS] =
{
   /** veTHIS_IS_A_TEST */	"Test Level",
   /** veipdDEBUG */	"Debug",
   /** veINFO */	"Info",
   /** veWARNING */	"Warning",
   /** veERROR */	"Error",
   /** veFATAL */	"Fatal",
};

/** this is the maximum severity of all errors occured
  (since the last call of veClearSeverityCondition() ) */
static ipdveSeverity _veSeverityCondition = ipdveLEVEL_THIS_IS_A_TEST;  /* = 0 */

/** this is the (totally unorthogonal) global filter which masks out all
debug messages  */
ipdBoolean _ipdveDebugMessagesEnabled = ipdTRUE;

/** start of the list of error handlers */
ipdveErrHanPtr _ipdveFirstHan = ipdNULL;

static int fputu(unsigned int num, FILE *stream);

/** holds a compiled regular expression to filter debug messages. If the
 value is ipdNULL, no filter is applied. The filter is applied to all
 registered error handlers. */
static ipdrgxRegexpPtr veDebugMessageFilter = ipdNULL;

/** @brief This function is only called by error transmitters.

This (secret) function is only called by error transmitters (e.g. the
veSendLispError() or ipdveSendError() functions).
It scans the list of registered
error handlers and calls them. If no error handler responds at all, the
default error handler is activated.
If this routine is called recursively, probably by an error in an error handler
(e.g. X11 System), it is processed normally with the exception of not invoking
the active error handler. If a further nested call occurs it is directly
flushed to stderr.
 */
ipdVoid ipdveDispatchError(ipdveErrorPtr err)	/* [I] description of actual error */
{
   /** holds the message level of the currently processed error */
   static ipdveSeverity  processingLevel1 = ipdveLEVEL_THIS_IS_A_TEST;
   /** saves the message level of an error already being process on call to
     ipdveDispatchError. */
   static ipdveSeverity  processingLevel2 = ipdveLEVEL_THIS_IS_A_TEST;
   static ipdInt        serial = 0;

          ipdveErrHanPtr scan;
   /** saves the currently active error handler during nested error
     processing */
   static ipdveErrHanPtr activeScan;
   /** saves the error handler already active on entry to this function */
          ipdveErrHanPtr prevScan = activeScan;

   if ((ipdVeSeverity(err) == ipdveLEVEL_ipdDEBUG) &&
       ((!_ipdveDebugMessagesEnabled) ||
        ((veDebugMessageFilter != ipdNULL) && (ipdVeFunctionName(err) != ipdNULL) &&
	 !ipdrgxExecute(veDebugMessageFilter,ipdVeFunctionName(err), ipdNULL, ipdNULL))))
      return;			/* ignore this debug messages */

   /** set the global severity condition */
   if (ipdVeSeverity(err) > _veSeverityCondition)
      _veSeverityCondition = ipdVeSeverity(err);

   /** emergence print and return if we are invoked from an error handler a
      second time */
   if (processingLevel2 > ipdveLEVEL_THIS_IS_A_TEST) {
      /** avoid using printf due to static buffer there! */
      fputs("\n###", stderr);
      fputu(serial++, stderr);
      fputc(' ', stderr);
      fputs(ipdVeSeverityString(ipdVeSeverity(err)), stderr);
      fputs(" (", stderr);
      fputs(ipdVeClassString(ipdveClass(err)), stderr);
      fputs("): \n   ", stderr);
      fputs(ipdVeMessage(err), stderr);
      fputc('\n', stderr);
      if (ipdVeErrno(err) > 0) {
	 ipdConstString errstr;

	 errstr = ipdveStrerror(ipdVeErrno(err));
	 if (errstr != ipdNULL) {
	    fputs("   errno: ", stderr);
	    fputs(errstr, stderr);
	    fputc('\n', stderr);
	 }
      }
      if (ipdVeModule(err) != ipdNULL) {
	 fputs("   module: ", stderr);
	 fputs(ipdVeModule(err), stderr);
	 fputs("\n   ", stderr);
      }
      if (ipdVeFunctionName(err) != ipdNULL) {
	 fputs(ipdVeFileName(err), stderr);
	 fputs("\n   ", stderr);
	 fputs(ipdVeFunctionName(err), stderr);
      } else {
	 fputs(ipdVeFileName(err), stderr);
	 fputs(", line ", stderr);
	 fputu(ipdVeLineNo(err), stderr);
	 fputs("\n   ", stderr);
      }
      fputs(ipdVeRcsID(err), stderr);
      fputc('\n', stderr);

      return;
   }

   processingLevel2 = processingLevel1;
   processingLevel1 = ipdVeSeverity(err);

   /** count the error messages */
   ipdVeSerial(err) = serial;
   serial++;

   for (scan = _ipdveFirstHan; scan != (ipdveErrHanPtr)ipdNULL; scan = scan->next) {
      if (scan == prevScan)
	 continue;

      if (scan->grab) {
	 if ((scan->classMask & ipdVeClassMask(ipdveClass(err))) &&
	     (scan->activationLevel <= ipdVeSeverity(err))) {
	    if (prevScan == ipdNULL)
	       activeScan = scan;
	    (scan->handler)(scan, scan->handlerData, err);
	    if (ipdVeSeverity(err) >= ipdveLEVEL_FATAL)
	       ipdvExit(-1);
	    else {
	       processingLevel1 = processingLevel2;
	       processingLevel2 = ipdveLEVEL_THIS_IS_A_TEST;
	       activeScan = prevScan;
	       return;
	    }
	 }
      }
   }
   for (scan = _ipdveFirstHan; scan != (ipdveErrHanPtr)ipdNULL; scan = scan->next) {
      if (scan == prevScan)
	 continue;

      if (scan->active) {
	 if ((scan->classMask & ipdVeClassMask(ipdveClass(err))) &&
	     (scan->activationLevel <= ipdVeSeverity(err))) {
	    if (prevScan == ipdNULL)
	       activeScan = scan;
	    if ((scan->handler)(scan, scan->handlerData, err) == ipdveTRAPPED) {
	       if (ipdVeSeverity(err) >= ipdveLEVEL_FATAL)
		  ipdvExit(-1);
	       else {
		  processingLevel1 = processingLevel2;
		  processingLevel2 = ipdveLEVEL_THIS_IS_A_TEST;
		  activeScan = prevScan;
		  return;
	       }
	    }
	 }
      }
   }
   /** exit on fatal severity */
   if (ipdVeSeverity(err) >= ipdveLEVEL_FATAL)
      ipdvExit(-1);

   processingLevel1 = processingLevel2;
   processingLevel2 = ipdveLEVEL_THIS_IS_A_TEST;
   activeScan = prevScan;
   return;
}

/* ------------------- private functions ----------------------- */

/** @brief Prints an unsigned integer number to stream. It uses only fputc, but not fprintf. 

    @param num Number to be printed as text
    @param stream File to print num to
    @return Number of characters printed or EOF if an error occurred. 
*/
static int fputu(unsigned int num, FILE *stream)
{
   int i = 0, n;
   char s[16];

   do {
      s[i++] = num % 10 + '0';
   } while ((num /= 10) > 0);
   n = i;

   /* now print in reverse order */
   do {
      if (fputc(s[--i], stream) == EOF)
	 return(EOF);
   } while (i);
   return n;
}

