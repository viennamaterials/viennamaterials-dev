/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

/** @file vestring.h
    @brief Obtain the error string associated with a particular error number
*/

#ifndef IPDvestring_INCLUDED
#define IPDvestring_INCLUDED 

#include <errno.h>
/** need additional external variables */
#if ipdHasSysErrlist && !ipdHasStrError
   extern char *ipdsys_errlist[];
   extern int ipdsys_nerr;
#else
#  include <string.h>
#endif /* ipdHasSysErrlist && !ipdHasStrError */

_BEGIN_CPLUSPLUS
IPDVBSEXTERN extern ipdConstString ipdveStrerror(ipdInt err);
_END_CPLUSPLUS

#endif /* vestring_INCLUDED */
