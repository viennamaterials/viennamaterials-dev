/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

CONTENT:  error message and error handling system

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  SH   Stefan Halama,     Institute for Microelectronics, TU Vienna

  who when        what
-------------------------------------------------------------------------
  SH   1994-09-24  New from scratch to track down vos::run troubles
=======================================================================*/

static char const _rcsid_ [] = "$Id: veinit.c$";

/** @file veinit.c
    @brief Condensed global vienna error message and error handling system --- application initialization and start log
*/


#include "../../include/ipdtype.h"
#include "verrP.h"



#define veSTARTMSG "STARTING "


/** name of running program */
ipdConstString ipdveProgname;

