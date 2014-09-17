/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

/** @file verr.h
    @brief Types and declarations for the error handler
*/

#ifndef _IPDverrINCLUDED_
#define _IPDverrINCLUDED_

#include "../../include/ipdtype.h"
#include "vemacros.h"
#include "vos.h"
#include "vexit.h"

#define IPDMAXERRMSGLEN	16*1024	/* max. length of error message incl. args */

_BEGIN_CPLUSPLUS
    
/* backtrace defines */
#if ipdVeBackTrace
struct IPDVBSEXTERN _ipdbacktrace_ {
  struct _ipdbacktrace_ *prev;
  ipdConstString name;
  ipdConstString file;
};

/* may be used from any module (independent of library compiled with/without IPDCHECK) */
IPDVBSEXTERN extern struct _ipdbacktrace_ *_ipdbacktraceroot_;
_END_CPLUSPLUS

#  define ipdVeStart(function)   \
   struct _ipdbacktrace_ _ipdbackTrace_; \
   _ipdbackTrace_.prev = _ipdbacktraceroot_; \
   _ipdbacktraceroot_ = &_ipdbackTrace_; \
   _ipdbackTrace_.name = function; \
   _ipdbackTrace_.file = __FILE__
#    if STEHO_ipdDEBUG > 0
        fprintf(stderr, "File: %s , Function: %s \n",__FILE__,function); /* [SteHo] */ \
        fflush(stderr);
#    endif 
      
#  if IPDCHECK
#    define ipdVeReturn(retval)  \
     for(_ipdbacktraceroot_ != &_ipdbackTrace_ ? \
	fprintf(stderr, _errorStackInconsistency_, \
		_ipdbackTrace_.name, _ipdbackTrace_.file, __LINE__) : 0, \
       _ipdbacktraceroot_=_ipdbackTrace_.prev;;) \
       return(retval)
#    define ipdVeReturnVoid      \
     for(_ipdbacktraceroot_ != &_ipdbackTrace_ ? \
	fprintf(stderr, _errorStackInconsistency_, \
		_ipdbackTrace_.name, _ipdbackTrace_.file, __LINE__) : 0, \
       _ipdbacktraceroot_=_ipdbackTrace_.prev;;) \
       return
#  else
#    define ipdVeReturn(retval)  \
     for(_ipdbacktraceroot_=_ipdbackTrace_.prev;;) return(retval)
#    define ipdVeReturnVoid      \
     for(_ipdbacktraceroot_=_ipdbackTrace_.prev;;) return
#  endif /* IPDCHECK */
#else
#    if STEHO_ipdDEBUG > 0
#       define ipdVeStart(function) \
          ++l__count; \
          for (l__kk=0; l__kk<indentchar; ++l__kk) fprintf(stderr," "); /* [SteHo] */ \
          fprintf(stderr, "File: %s , Function: %s, (overall count = %ld) \n",__FILE__,function,l__count); /* [SteHo] */ \
          fflush(stderr); /* [SteHo] */ \
          indentchar += 2
#       define ipdVeReturn(retval) \
               for(indentchar -= 2;;) \
                  return(retval)
#       define ipdVeReturnVoid \
               for (indentchar -= 2;;) \
                   return
#    else 
#       define ipdVeStart(any) /**/
#       define ipdVeReturn(retval) return(retval)
#       define ipdVeReturnVoid	return
#    endif 
#endif /* ipdVeBackTrace */

/***TE defines the different error classes. Every reported error must be
    classified to one of these classes. */
typedef enum ipdveClass_e
{
   ipdveTDB = 0, /*** Test/Debug */
   ipdveSYS = 1, /*** Operating system call */
   ipdveSIG = 2, /*** Caught signal */
   ipdveALG = 3, /*** Internal algorithmic fault */
   ipdveEXT = 4, /*** External algorithmic fault */
   ipdveARG = 5, /*** Argument fault */
   ipdveDAT = 6, /*** (Input) data fault */
   ipdveCNF = 7, /*** Configuration/Installation fault */
   ipdveLIM = 8, /*** Limits violated */
   ipdveNUM = 9, /*** Numerical fault */
   ipdveUNI = 10, /*** Unimplemented feature */
   ipdveFCF = 11, /*** Function call failed */
   ipdveFOR = 12, /*** Foreign */
   ipdveUSR = 13, /*** User interaction */
   ipdveNUM_CLASSES
} ipdveClass;


typedef ipdInt ipdveClassMask;

#define ipdVeClassMask(ec) (0x0001 << (ec))

/***TE defines different level of severity for error reporting. Depending on
    the level of the error message different action may be done. For a fatal
    error the program will terminate always. */
typedef enum ipdveSeverity_e
{
   ipdveLEVEL_THIS_IS_A_TEST = 0,
   /*** severity level ``Test Level'' */
   ipdveLEVEL_ipdDEBUG = 1,
   /*** [:fortran-name ivedbg] severity level ``Debug'' */
   ipdveLEVEL_INFO = 2,
   /*** [:fortran-name iveinf] severity level ``Info'' */
   ipdveLEVEL_WARNING = 3,
   /*** [:fortran-name ivewrn] severity level ``Warning'' */
   ipdveLEVEL_ERROR = 4,
   /*** [:fortran-name iveerr] severity level ``Error'' */
   ipdveLEVEL_FATAL = 5,
   /*** [:fortran-name iveftl] severity level ``Fatal'' */
   ipdveNUM_LEVELS
} ipdveSeverity;

typedef struct ipdveError_s *ipdveErrorPtr;

_BEGIN_CPLUSPLUS
/* the C error transmitter */
IPDVBSEXTERN extern ipdVoid ipdveSendError(ipdConstString 	module,
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
		  ...);

_END_CPLUSPLUS

#ifndef IPDVMODULE
#define IPDVMODULE "*undefined*"
#endif

#endif /* _verrINCLUDED_ */
