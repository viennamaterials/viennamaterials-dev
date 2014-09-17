/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

/** @file vosP.h
    @brief Core declarations of system-independent directory/file information.
*/

#ifndef	_IPDvospINCLUDED_
#define	_IPDvospINCLUDED_

#include "../../include/ipdtype.h"
#include "vdir.h"
#include "vos.h"

/* this is the magic character to detect IPDNON vospecs */
#define ipdvosFILE_MAGIC		'\033'

/* we use some unprintable characters in the path to define the special
   meanings of TOP, THIS, UP and the path separator */
#define ipdvosTOP_CHAR		'\01'
#define ipdvosTHIS_CHAR		'\02'
#define ipdvosUP_CHAR		'\03'
#define ipdvosPATHSEP_CHAR		'\012' /* LineFeed */
#define ipdvosTOP_CHARSTRING	"\01"
#define ipdvosTHIS_CHARSTRING	"\02"
#define ipdvosUP_CHARSTRING	"\03"

_BEGIN_CPLUSPLUS

IPDVBSEXTERN extern ipdConstString ipdvosVos2Sysspec(ipdvosFile, ipdLongWord, ipdvosSys);

IPDVBSEXTERN extern ipdvosFile ipdvosCreateDirectVospec(ipdConstString node,
				     ipdConstString device,
				     ipdConstString path,
				     ipdConstString name,
				     ipdConstString ext,
				     ipdvosSys systype);

typedef enum _ipdvoscase_
{
   ipdvosUPPER_CASE  = 0,          /* only upper case characters */
   ipdvosLOWER_CASE  = 1,          /* only lower case characters */
   ipdvosMIXED_CASE  = 2,          /* case sensitive characters */
   ipdvosCOMMON_CASE = 3           /* case insensitive characters */
} ipdvosCase;

/* internal VOS specification structure for system definitions */
typedef struct _ipdvospart_
{
   ipdConstString str;		/* string */
   ipdLongWord    len;		/* length of string */
} ipdvospart;

typedef struct _ipdvosdef_
{
   ipdConstString name;		/* name of operation system */

   ipdvospart nodesep;		/* separator afer a node [OPT] */
   ipdvospart devicesep;		/* separator after device [OPT] */
   ipdvospart pathsepl;		/* separator at begin of path [OPT] */
   ipdvospart dirsep;		/* separator between directories */
   ipdvospart pathsepr;		/* separator after end of path */
   ipdvospart extsep;		/* separator between file and extension */
   ipdvospart versionsep;		/* separator in front of version [OPT] */

   ipdvospart topdir;		/* name of top level directory */
   ipdvospart thisdir;		/* name of current directory */
   ipdvospart updir;		/* name of up directory */
   ipdvospart hidden;		/* hidden directory entry */

   ipdvosCase ncase;		/* case of names */
   ipdBoolean deviceafter;	/* device after separator  */
   ipdBoolean nodirext;		/* remove extension for directory */
   ipdConstString dirext;		/* directory extension [OPT] */
   ipdConstString topdirname;	/* directory to list top dir */
   ipdBoolean topneedssep;	/* print separator after top directory */
   ipdBoolean topnospec;		/* if no up, this or separator first -> top */
   ipdBoolean thisnospec;		/* if no up, top or separator first -> this */
   ipdBoolean extsepreq;		/* extension separator required if no ext */
   ipdBoolean completedir;	/* use directories for file completion */
   ipdBoolean sorted;		/* sorted directory listing */

   ipdChar envchar;		/* environment character */
   ipdChar listsep;		/* separator for path list */
   ipdChar optionsep;		/* character for command line options */
   ipdChar optionsep2;		/* secondary character for command line
				   options [OPT] */

   ipdvospart home;		/* shortcut of homedirectory [OPT] */
   ipdConstString exec;		/* extension of executable script [OPT] */
   ipdConstString shell;		/* extension of shell script [OPT] */

   ipdvospart backup;		/* additional string for backup files */
} ipdvosDefinition;

typedef ipdvosDefinition *ipdvosDefPtr;

IPDVBSEXTERN extern ipdvosDefinition ipdsystems[ipdvosKIPDNOWN_SYS];
IPDVBSEXTERN extern ipdvosSys        ipddefsystem;
IPDVBSEXTERN extern ipdConstString   ipdvosConvertFile(ipdvosFile, ipdBoolean);

_END_CPLUSPLUS

#endif /* _vospINCLUDED_ */
