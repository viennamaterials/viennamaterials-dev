/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

static char const _rcsid_ [] = "$Id: verrc.c$";

/** @file verrc.c
    @brief Implementation of error handling code 
*/


#include "verrP.h"

/** @brief Used to send the error from C code to the Vienna Error System. The function should never be called directly, but the C error macros should be used instead. 
*/
ipdVoid ipdveSendError(
		  ipdConstString 	module,
#if ipdVeBackTrace
		  struct _ipdbacktrace_ *bt,
#else
		  ipdConstString	file,
#endif /* ipdVeBackTrace */
		  ipdConstString 	rcsID,
		  ipdInt	  	lineNo,
		  ipdveSeverity	severity,
		  ipdveClass	errclass,
		  ipdInt		id,
		  ipdConstString	message,
#if ipdHasStdarg
		  ...
#else
		  va_dcl
#endif /* ipdHasStdarg */
		  )
{
   ipdveError 	err;
   ipdChar _veStrBuf[IPDMAXERRMSGLEN+1];
   va_list vap;
#if ipdHasStdarg
   va_start(vap, message);
#else
   va_start(vap);
#endif /* ipdHasStdarg */

   /* no vsprintf() called, big speedup for ipdveLEVEL_ipdDEBUG */
   if ((!_ipdveDebugMessagesEnabled) && (severity == ipdveLEVEL_ipdDEBUG))
      return;

   vsprintf(_veStrBuf, message, vap); /* print message to buffer */
   va_end(vap);

   err.module = module;
#if ipdVeBackTrace
   err.bt = bt;
   err.fileName = bt->file;
   err.functionName = bt->name;
#else
   err.bt = ipdNULL;
   err.fileName = file;
   err.functionName = ipdNULL;
#endif /* ipdVeBackTrace */
   err.rcsID = rcsID;
   err.lineNo = lineNo;
   err.severity = severity;
   err.errclass = errclass;
   err.message = _veStrBuf;
   err.id = id;
   err.ErrNo = ((errclass == ipdveSYS) || (errclass == ipdveSIG)) ? errno : 0;
   errno = 0;				/* reset error number */
   err.language = ipdveLANGUAGE_C;

   ipdveDispatchError(&err);
   return;
}
