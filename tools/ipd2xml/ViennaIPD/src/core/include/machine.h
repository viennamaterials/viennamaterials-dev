/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

/** @file machine.h
    @brief Deduces various properties from the machine on which ViennaIPD is run.
*/

#ifndef _IPDmachineINCLUDED_
#define _IPDmachineINCLUDED_

/* ----- very important things needed for compiling */
#if defined(__unix__) && !defined(__unix)
#  define __unix
#endif /* defined(__unix__) && !defined(__unix) */

#if defined(__APPLE__) && (__APPLE__ == 1)
#  define __unix
#endif

#if defined(__CYGWIN__)
#  if !defined(__unix)
#    define __unix
#  endif /* !defined(__unix) */
#  undef WIN32
#  undef WINNT
#endif /* defined(__CYGWIN__) */


#if defined(opennt) || defined(__OPENNT)
#  if !defined(__unix)
#    define __unix
#  endif /* !defined(__unix) */
#  if !defined(__OPENNT)
#    define __OPENNT
#  endif /* !defined(__OPENNT) */
#  undef WIN32
#  undef WINNT
#endif /* defined(opennt) || defined(__OPENNT) */


#if defined(WIN32) && !defined(WINNT)
#  define WINNT
#endif /* defined(WIN32) && !defined(WINNT) */


#if defined(WINNT)
#if !defined(_WIN32_WINNT)
#  define _WIN32_WINNT 0x0400
#endif
#  include <WTYPES.H>
#  include <WINBASE.H>
#  include <WINNT.H>
/* some special pragmas for C support */
 #  pragma warning(disable: 4113)
#endif /* defined(WINNT) */


/* include project definitions */
#include "project.h"

#if defined(__cplusplus)
#  ifndef _BEGIN_CPLUSPLUS
#    define _BEGIN_CPLUSPLUS	extern "C" {
#  endif /* _BEGIN_CPLUSPLUS */
#  ifndef _END_CPLUSPLUS
#    define _END_CPLUSPLUS	}
#  endif /* _END_CPLUSPLUS */
#else
#  ifndef _BEGIN_CPLUSPLUS
#    define _BEGIN_CPLUSPLUS	/**/
#  endif /* _BEGIN_CPLUSPLUS */
#  ifndef _END_CPLUSPLUS
#    define _END_CPLUSPLUS	/**/
#  endif /* _END_CPLUSPLUS */
#endif /* defined(__cplusplus) */


/* get better alloca from GNUC */
#ifdef __GNUC__
#  undef ipdHasAlloca
/* #  define ipdHasAlloca	IPDYES */
#  define ipdHasAlloca	IPDNO /* [SteHo] */
#  ifdef alloca
#     undef alloca
#  endif
#  define alloca(size) __builtin_alloca(size)
#  ifndef ipdHasLongLong
#  define ipdHasLongLong	IPDYES
#  endif
#else
#  if ipdHasAlloca && ipdNeedAllocaInclude
#    undef ipdHasAlloca
#    define ipdHasAlloca	IPDYES
#    if defined(vms) || defined(__vms)
#      include <builtins.h>	/* need some more definitions */
#      define alloca(size) __ALLOCA(size)
#    else
#      if defined(__DECCXX)
#        undef ipdHasAlloca	/* problems with include file definition */
#        define ipdHasAlloca	IPDNO
#      else
#        include <alloca.h>	/* include definition file */
#      endif /* defined(__DECCXX) */
#    endif /* defines(vms) || defined(__vms) */
#  endif /* ipdNeedAllocaInclude */
#  if defined(WINNT)
#    include <malloc.h>
#  endif /* defined(WINNT) */
#endif /* __GNUC__ */

/* some foreign programs want's this */
#if ipdSystemV  && !ipdBSDcompat
#  ifndef SYSV
#    define SYSV	IPDYES
#  endif /* SYSV */
#endif /* ipdSystemV */

/* NeedFunctionPrototypes is used by X11 */
#ifndef NeedFunctionPrototypes
#if !ipdHasANSI || ipdHasFunctionParameterBug
#  define NeedFunctionPrototypes IPDNO
#  undef XTFUNCPROTO		/* Xt from X11R4 */
#else
#  define NeedFunctionPrototypes IPDYES
#  define XTFUNCPROTO		/* Xt from X11R4 */
#endif /* ipdHasANSIBug1 */
#endif /* NeedFunctionPrototypes */

#if ipdHasIPDNOconst
#  define const /**/
#endif /* ipdHasIPDNOconst */

/***********************************************************************/
/***********************************************************************/
/* ---- include standard system dependent include files  --- */
/* included on all known systems */
#if defined(unix) || defined(__unix)
#  if !defined(apollo) && !defined(__apollo) && !defined(__sony)
#    include <sys/param.h>
#  endif /* !defined(apollo) && !defined(__apollo) && !defined(__sony) */
#endif /* defined(unix) || defined(__unix) */
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <float.h>



#if defined(unix) || defined(__unix)
#  include <sys/types.h>
#  include <time.h>
#  include <sys/time.h>
#  include <sys/times.h>
#endif /* defined(unix) || defined(__unix) */

#if defined(WINNT)
#  include <sys/types.h>
#  include <time.h>
#endif /* defined(WINNT) */

#include <ctype.h>

#if ipdHasStdlib
#  include <stdlib.h>
#  if !defined(_WCHAR_T) && defined(_WCHAR_T_)
#    define _WCHAR_T _WCHAR_T_ /* for isc/lpi-cc and X11 */
#  endif /* !define(_WCHAR_T) && defined(_WCHAR_T_) */
#else
  _BEGIN_CPLUSPLUS
  extern char *getenv(const char *); /* definition only in stdlib.h */
  _END_CPLUSPLUS
#  if !defined(apollo) && !defined(__apollo) && !defined(vms) && !defined(__vms)
#    include <malloc.h>
#  endif /* !defined(apollo) && !defined(__apollo) && !defined(vms) && !defined(__vms) */
#endif /* ipdHasStdlib */

#include <string.h>


#if ipdHasStdarg
#  include <stdarg.h>
#else
#  include <varargs.h>
#endif /* ipdHasStdarg */

#if ipdHasVoidArg && ipdHasANSI	/* allowes {\bf void} in function prototype */
#  define voidArg	void
#else
#  define voidArg	/**/
#endif /* ipdHasVoidArg && ipdHasANSI */


/* error codes of system functions */
#include <errno.h>

#if defined(hz)
#undef hz			/* defined on some machines for HZ too */
#endif /* defined(hz) */

#endif /* _machineINCLUDED_ */
