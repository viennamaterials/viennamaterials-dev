/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

/** @file vossysP.h
    @brief Preprocessor switches for system-independent directory/file information.

    Preprocessor switches for system-independent directory/file information.
*/

#ifndef _IPDvsysPINCLUDED_
#define _IPDvsysPINCLUDED_

#ifndef _IPDvdirINCLUDED_
  #include "vdir.h"
#endif /* _vdirINCLUDED_ */

_BEGIN_CPLUSPLUS

#if defined(unix) || defined(__unix)
#  include <pwd.h>
#  include <signal.h>
#  include <sys/wait.h>
#  if ipdHasProcessResources
#    include <sys/resource.h>
#  endif /* ipdHasProcessResources */
#  if defined(_AIX)
#    include <sys/select.h>
#  endif /* defined(_AIX) */
#  if defined(sparc)
#    if ipdSystemV
#      include <sys/resource.h>	/* Solaris */
#    else
#      include <vfork.h>	/* SunOS */
#    endif /* ipdSystemV */
#  endif /* defined(sparc) */
#  if ipdSystemV
#    include <sys/utsname.h>
#  endif /* ipdSystemV */
   /** THIS IS A TEMPORARY HACK TO ALLOW REMOTE EXECUTION */
/* generate return code */
#  define ipdVosExitCode(code)	code	/* nothing to do */
#  if !defined(WEXITSTATUS)
#    if ipdHasWaitUnion
#      include <wait.h>
#    endif /* ipdHasWaitUnion */
#  endif /* defined(WEXITSTATUS) */
#endif /* defined(unix) || defined(__unix) */




_END_CPLUSPLUS

#endif /* _vsysPINCLUDED_ */
