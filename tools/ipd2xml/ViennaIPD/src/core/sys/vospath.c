/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

static char const _rcsid_ [] = "$Id: vospath.c$";

/** @file vospath.c
    @brief Virtual path handling
*/


/***I */
#include "../../include/ipdtype.h"
#include "verr.h"
#include "vosP.h"



/** @brief Checks, if the path starts from top. 
    @param vospec File specification to check
    @return ipdTRUE is path starts from top
*/
ipdBoolean ipdvosIsTopPath(ipdvosFile vospec)
{
   return ((vospec != ipdNULL) &&
	   (vospec->vosbuf[0] == ipdvosFILE_MAGIC) && /* VOSPEC check */
	   (vospec->path != ipdNULL) &&
	   (*(vospec->path) == ipdvosTOP_CHAR));
}

/** @brief Gets one search path from a list of pathes as OS specific string.

    For the first call, the argument pos
    should be initialized to zero, because this parameter is used to find
    further pathes. If no more pathes are available, ipdNULL is returned, else
    an allocated ipdvosFile specification.

    @param pathlist String with path list
    @param pos Position in path list
    @return System independent file specification
*/
ipdvosFile ipdvosGetPathFromPathList(
	ipdConstString pathlist,
	ipdLong *pos)
{
   ipdChar sep = ipdsystems[ipddefsystem].listsep;
   ipdConstString search, end;
   ipdChar* unused_ret_val_char_pnt; /* [patch] vor capturing unused return values */   
   ipdVeStart("ipdvosGetPathFromPathList");

   if ((pathlist == ipdNULL) || (pathlist[*pos] == '\0'))
      ipdVeReturn(ipdNULL);		/* no pathes or already lastone read */

   search = pathlist + *pos;
   end = ipdvStrchr(search, sep);
   if (end == ipdNULL) {
      ipdvosFile vospec;
      *pos += ipdvStrlen(search);
      vospec = ipdvosOs2Vospec(search, ipdTRUE);
      ipdVeReturn(vospec);
   } else {
      ipdString buf;		/* buffer for one path element */
      ipdLong len = end - search;
      ipdvosFile file;

      buf = (ipdString)ipdvAlloca(len + 1);
      unused_ret_val_char_pnt = ipdvStrncpy(buf, search, len); /* [patch] removed warning: unused retval */
      buf[len] = '\0';
      *pos += len+1;
      file = ipdvosOs2Vospec(buf, ipdTRUE);
      ipdvAllocaFree(buf);
      ipdVeReturn(file);
   }
}
