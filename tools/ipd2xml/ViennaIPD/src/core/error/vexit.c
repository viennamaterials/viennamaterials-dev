/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

static char const _rcsid_ [] = "$Id: vexit.c$";

/** @file vexit.c
    @brief Handler for calling functions at exit. Mimics the behavior of destructors in C.
*/

#include "vexit.h"
#include "verrP.h"
#include "vossysP.h"


#define MAXFUNC 256

/* I *know* why I do this in that way! */
/* private array of functions to call at exit */

static int  funcCount = 0;
static ipdVoid (*exitFuncArr[MAXFUNC])(voidArg);
/* from veinit.c */
extern ipdConstString ipdveProgname;

/** @brief Call functions for cleanup at exit */
ipdVoid ipdvDoExitFunctions()
{
   ipdInt i;
   ipdVeStart("ipdvDoExitFunctions");

   for (i = funcCount - 1; i >= 0; i--) {
      ipdVoid (*func)() = exitFuncArr[i];
      exitFuncArr[i] = ipdNULLFUNC; /* clear, already called */
      if (func != ipdNULLFUNC)
	 (func)();		/* call cleanup function */
   }
   ipdVeReturnVoid;
}

/** @brief Exit the program and print some stats */
ipdNonReturn ipdVoid ipdvExit(int code)
{
   ipdInt vcode;
   ipdvTimeType tm;
   ipdString tmStr;
   ipdDouble cpu;
   ipdVeStart("ipdvExit");

   ipdvDoExitFunctions();
   vcode = ipdVosExitCode(code);
   ipdvTime(&tm);
   tmStr = ipdvCTime(&tm);
   cpu = ipdvSecTime();

   /* remove trailing linefeed if necessary */
   if(tmStr[ipdvStrlen(tmStr) - 1] == '\n')
      tmStr[ipdvStrlen(tmStr) - 1] = '\0';

#ifndef WINNT
   if (ipdveProgname != ipdNULL)
      ipdVeInfo4(ipdveTDB,
	      "%s: TERMINATING %s, CPU: %4.2lf, exit code %d",
	      tmStr, ipdveProgname, cpu, vcode);
   else
      ipdVeInfo3(ipdveTDB,
	      "%s: TERMINATING, CPU: %4.2lf, exit code %d",
	      tmStr, cpu, vcode);
#else
      ipdVeInfo2(ipdveTDB,
	      "%s: TERMINATING, exit code %d",
	      tmStr, vcode);
#endif

   fflush(stdout);
   fflush(stderr);
   exit(vcode);

   /* just for TAC/Vmake consistency */
   ipdReturnNotReached;
}


