/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

static char const _rcsid_ [] = "$Id: vestring.c$";

/** @file vestring.c
    @brief Obtain the error string associated with a particular error number
*/

#include "../../include/ipdtype.h"
#include "vestring.h"


/** @brief Returns the string correlated to the system error number err.
    @param err System error number
    @return The error message
 */
ipdConstString ipdveStrerror(ipdInt err)
{
#if ipdHasStrError || ipdHasSysErrlist
   ipdConstString errstr;

#if ipdHasStrError
   errstr = strerror(err);
#else
#if ipdHasSysErrlist
   errstr = (err < ipdsys_nerr) ?
      ipdsys_errlist[err] : ipdNULL;
#endif /* ipdHasSysErrlist */
#endif /* ipdHasStrError */
   return(errstr);
#else
   return(ipdNULL);
#endif /* ipdHasStrError || ipdHasSysErrlist */
}
