/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

/** @file vexit.h
    @brief Handler for calling functions at exit. Mimics the behavior of destructors in C.
*/

#ifndef _IPDvexitINCLUDED_
#define _IPDvexitINCLUDED_

#include "../../include/ipdtype.h"

_BEGIN_CPLUSPLUS
IPDVBSEXTERN extern ipdNonReturn ipdVoid ipdvExit(int code) ipdNonReturnAttr;
_END_CPLUSPLUS
#endif /* _vexitINCLUDED_ */
