/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

/** @file unit.h
    @brief Global definitions for the unit system
*/

#ifndef _IPDunitINCLUDED_
#define _IPDunitINCLUDED_

#include "../../include/ipdtype.h"

#include <math.h>

#define ipduntMAX_UNITSLENGTH   100

#define ipduntOUTPUT_VOLT		(0 << 0) /* use Volt instead of kg */
#define ipduntOUTPUT_KG		(1 << 0) /* use kg */
#define ipduntOUTPUT_IPDNONUMBER	(1 << 1) /* no leading number */
#define ipduntOUTPUT_DIV		(1 << 2) /* output unit with / */
#define ipduntOUTPUT_MULT		(1 << 3) /* output unit with * */


#endif /* _unitINCLUDED_ */
