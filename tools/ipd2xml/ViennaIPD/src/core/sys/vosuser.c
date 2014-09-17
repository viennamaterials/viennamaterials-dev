/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

static char const _rcsid_ [] = "$Id: vosuser.c$";

/** @file vosuser.c
    @brief Virtual home directory management

*/

#include "../../include/ipdtype.h"
#include "verr.h"
#include "vosP.h"
#include <pwd.h>

/* internal variables */
static ipdConstString homedir = ipdNULL;

/** @brief This free function takes care of the allocated strings which have been created by InitInformation. */
ipdVoid ipdFreeVosUser()
{
	free((ipdString)homedir);
}

/** @brief Returns the user home directory */
ipdConstString ipdGetHomeDirectory()
{
   ipdVeStart("ipdGetHomeDirectory");
   if(homedir == ipdNULL)
   {
      homedir = ipdvosGetEnvironment("HOME");
      if(homedir == ipdNULL)
      {
         /* [JW] use the getpwuid function only if absolutly necessary,
            as it seems that the function is leaking memory. */      
         struct passwd *pw;
         pw = getpwuid(getuid());	
         if (pw != ipdNULL) 
         {
            homedir = ipdvStrdup(pw->pw_dir);
         }
      }
   }
   ipdVeReturn(homedir);
}

/** @brief Returns the home directory of usernm
    @param usernm The username 
    @return the home directory as string
*/
ipdConstString ipdGetHomeDirectoryByUserName(ipdConstString usernm)
{
   ipdConstString homdir = ipdNULL;
   ipdVeStart("ipdGetHomeDirectoryByUserName");
   { 
      struct passwd *pw;
      pw = getpwnam(usernm);	/* get information by name */
      if (pw != ipdNULL)
         homdir = pw->pw_dir;	/* get home directory */
   }
   ipdVeReturn(homdir);
}

/** @brief Returns an allocated environment string 
    @param env The environment name
    @return ipdNULL is not allocated
*/
ipdString ipdvosGetEnvironment(ipdConstString env)
{
   ipdConstString value;
   ipdString val;
   ipdInt len = ipdvStrlen(env);
   ipdVeStart("ipdvosGetEnvironment");

   if (env[len-1] == ipdsystems[ipddefsystem].envchar) {
      ipdString cenv = (ipdString)env;
      cenv[len-1] = '\0';        /* remove environment character */
      value = getenv(cenv);
      cenv[len-1] = ipdsystems[ipddefsystem].envchar; /* restore */
   } else
      value = getenv(env);
   if (value == ipdNULL)
      ipdVeReturn(ipdNULL);		/* no value found */

   val = ipdvStrdup(value);
   /* check for LOGICAL search list */
   value = ipdTranslateLogicalList(env, 0);
   if ((value != ipdNULL) && (ipdvStrcmp(value, val) == 0)) {
      ipdLong nlen = ipdvStrlen(val);
      ipdChar ls = ipdvosGetListSeparator();
      ipdLong num;
      num = 1;

      /* try evaluation of LOGICAL in search list */
      value = ipdTranslateLogicalList(env, num++);
      while(value != ipdNULL) {
	 ipdLong llen = ipdvStrlen(value);
	 val = (ipdString)ipdvRealloc(val, nlen+llen+2);
	 val[nlen] = ls;	/* add list seperator */
	 ipdvMemcpy(val+nlen+1, value, llen+1); /* append next list value */
	 nlen += llen+1;	/* new string length */
	 /* try next LOGICAL in search list */
	 value = ipdTranslateLogicalList(env, num++);
      }
   }
   ipdVeReturn(val);
}

