/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

static char const _rcsid_ [] = "$Id: vosfunc.c$";

/** @file vosfunc.c
    @brief Virtual file handling
*/


/***I */
#include "../../include/ipdtype.h"
#include "verr.h"
#include "vosP.h"


/** @brief Expands all logicals of a file system independent file specification
    @param vospec Specification to expand
    @return System independent file specification
    */
ipdvosFile ipdvosExpandVospec(ipdvosFile vospec)
{
   ipdvosFile file = vospec;
   ipdString logval;
   ipdVeStart("ipdvosExpandVospec");

   if (vospec == ipdNULL)
      ipdVeReturn(ipdNULL);

   while ((ipdVosGetDevice(file) != ipdNULL) &&
	  ((logval = ipdTranslateLogical(ipdVosGetDevice(file))) != ipdNULL)) {
      /* we have a logical value, translate it */
      ipdvosFile voslog, new;
      ipdChar first;
      ipdString path;
      ipdConstString dev;
      voslog = ipdvosOs2Vospec(logval, ipdFALSE);
      path = (ipdString)ipdVosGetPath(file);
      if ((path != ipdNULL) &&
	  (*path == ipdvosTOP_CHAR)) {
	 first = *path;		/* save for later */
	 *path = ipdvosTHIS_CHAR;	/* for real concatenation */
      } else
	 first = '\0';
      dev = ipdVosGetDevice(file);
      ipdVosGetDevice(file) = ipdNULL; /* no device for specat, only path! */
      new = ipdvosSpecat(voslog, file);
      ipdVosGetDevice(file) = dev;
      if (first != '\0')
	 *path = first;		/* restore */
      ipdvosFreeVospec(voslog);	/* free temporary vals */
      if (file != vospec)
	 ipdvosFreeVospec(file);
      file = new;		/* save new value (and try again) */
   }
   if (file == vospec)
      file = ipdvosCopyVospec(file);

   ipdVeReturn(file);
}

/** @brief Creates a new system independent file specification from the given parts. 
    @param node Node name
    @param device Device name
    @param path Path elements
    @param name File name
    @param ext File extension
    @param systype Operating system type
    @return System independent file specification
*/
ipdvosFile ipdvosCreateDirectVospec(
	ipdConstString node,	/* [I] node name */
	ipdConstString device,	/* [I] device name */
	ipdConstString path,	/* [I] path elements */
	ipdConstString name,	/* [I] file name */
	ipdConstString ext,	/* [I] file extension */
	ipdvosSys systype)		/* [I] operating system type */
{
   ipdLong len = 0;		/* length of whole information */
   ipdvosFile file;
   ipdString pos;
   ipdVeStart("ipdvosCreateDirectVospec");

   if ((node == ipdNULL) && (device == ipdNULL) && (path == ipdNULL) &&
       (name == ipdNULL) && (ext == ipdNULL))
      path = ipdvosTHIS_CHARSTRING; /* default is current directory */

   if (node != ipdNULL)
      len += ipdvStrlen(node) + 1;	/* add length of node specification */
   if (device != ipdNULL)
      len += ipdvStrlen(device) + 1; /* add length of device specification */
   if (path != ipdNULL)
      len += ipdvStrlen(path) + 1;	/* add length of path specification */
   if (name != ipdNULL)
      len += ipdvStrlen(name) + 1;	/* add length of name specification */
   if (ext != ipdNULL)
      len += ipdvStrlen(ext) + 1;	/* add length of extension specification */

   /** now we have all start and end position found */
   /** let's check for the total needed length */
   file = (ipdvosFile)ipdvMalloc(sizeof(ipdvosFileStruct) + len + 2);
   if (file == ipdNULL) {
      ipdVeError(ipdveSYS,
	      "unable to allocate memory for system independent file specification");
      ipdVeReturn(ipdNULL);		/* no memory */
   }

   pos = &file->vosbuf[0];
   *pos++ = ipdvosFILE_MAGIC;	/* magic number */

   /* enter the node specification */
   if (node != ipdNULL) {
      ipdLong len = ipdvStrlen(node);
      file->node = pos;
      ipdvMemcpy(pos, node, len+1);
      switch(ipdsystems[ipddefsystem].ncase) {
      case ipdvosUPPER_CASE:
	 ipdvToupper(pos);		/* only upper case characters */
	 break;
      case ipdvosLOWER_CASE:
	 ipdvTolower(pos);		/* only lower case character */
	 break;
      case ipdvosMIXED_CASE: case ipdvosCOMMON_CASE:
	 break;
      }
      pos += len+1;
   } else
      file->node = ipdNULL;

   /* enter the device specification */
   if (device != ipdNULL) {
      ipdLong len = ipdvStrlen(device);
      file->device = pos;
      ipdvMemcpy(pos, device, len+1);
      switch(ipdsystems[ipddefsystem].ncase) {
      case ipdvosUPPER_CASE:
	 ipdvToupper(pos);		/* only upper case characters */
	 break;
      case ipdvosLOWER_CASE:
	 ipdvTolower(pos);		/* only lower case character */
	 break;
      case ipdvosMIXED_CASE: case ipdvosCOMMON_CASE:
	 break;
      }
      pos += len+1;
   } else
      file->device = ipdNULL;

   /* enter the path specification */
   if (path != ipdNULL) {
      ipdLong len = ipdvStrlen(path);
      file->path = pos;
      ipdvMemcpy(pos, path, len+1);
      switch(ipdsystems[ipddefsystem].ncase) {
      case ipdvosUPPER_CASE:
	 ipdvToupper(pos);		/* only upper case characters */
	 break;
      case ipdvosLOWER_CASE:
	 ipdvTolower(pos);		/* only lower case character */
	 break;
      case ipdvosMIXED_CASE: case ipdvosCOMMON_CASE:
	 break;
      }
      pos += len+1;
   } else
      file->path = ipdNULL;

   /* enter the name specification */
   if (name != ipdNULL) {
      ipdLong len = ipdvStrlen(name);
      file->name = pos;
      ipdvMemcpy(pos, name, len+1);
      switch(ipdsystems[ipddefsystem].ncase) {
      case ipdvosUPPER_CASE:
	 ipdvToupper(pos);		/* only upper case characters */
	 break;
      case ipdvosLOWER_CASE:
	 ipdvTolower(pos);		/* only lower case character */
	 break;
      case ipdvosMIXED_CASE: case ipdvosCOMMON_CASE:
	 break;
      }
      pos += len+1;
   } else
      file->name = ipdNULL;

   /* enter the ext specification */
   if (ext != ipdNULL) {
      ipdLong len = ipdvStrlen(ext);
      file->ext = pos;
      ipdvMemcpy(pos, ext, len+1);
      switch(ipdsystems[ipddefsystem].ncase) {
      case ipdvosUPPER_CASE:
	 ipdvToupper(pos);		/* only upper case characters */
	 break;
      case ipdvosLOWER_CASE:
	 ipdvTolower(pos);		/* only lower case character */
	 break;
      case ipdvosMIXED_CASE: case ipdvosCOMMON_CASE:
	 break;
      }
      pos += len+1;
   } else
      file->ext = ipdNULL;

   /* enter the version specification */
   file->version = 0;
   file->systype = systype;

   /* enter the real length of specification */
   file->length = (pos - &file->vosbuf[0]);

   /* return the generated specification */
   ipdVeReturn(file);
}

/** @brief Creates a new system independent file specification from the original vospec.

    All parts mentioned in the flags are used from the original specification if the
    corresponding flag is not set for update. 

    @param vospec File specification to update
    @param node Node name
    @param device Device name
    @param path Path elements
    @param name File name
    @param ext File extension
    @param flags Update flags
    @return System independent file specification
*/
ipdvosFile ipdvosUpdateVospec(
	ipdvosFile vospec,		/* [I] file specification to update */
	ipdConstString node,	/* [I] node name */
	ipdConstString device,	/* [I] device name */
	ipdConstString path,	/* [I] path elements */
	ipdConstString name,	/* [I] file name */
	ipdConstString ext,	/* [I] file extension */
	ipdLongWord flags)	/* [I] update flags */
{
   ipdvosFile retval;
   ipdVeStart("ipdvosUpdateVospec");

   if (vospec == ipdNULL)
      /* create a new vospec */
      ipdVeReturn(ipdvosCreateDirectVospec((flags & ipdvosUPDATE_IPDNODE) ? node : ipdNULL,
				     (flags & ipdvosUPDATE_DEVICE) ? device : ipdNULL,
				     (flags & ipdvosUPDATE_PATH) ? path : ipdNULL,
				     (flags & ipdvosUPDATE_NAME) ? name : ipdNULL,
				     (flags & ipdvosUPDATE_EXT) ? ext : ipdNULL,
				     ipddefsystem));

   if (! (flags & ipdvosUPDATE_IPDNODE))
      node = ipdVosGetNode(vospec);
   if (! (flags & ipdvosUPDATE_DEVICE))
      device = ipdVosGetDevice(vospec);
   if (! (flags & ipdvosUPDATE_PATH))
      path = ipdVosGetPath(vospec);
   if (! (flags & ipdvosUPDATE_NAME))
      name = ipdVosGetName(vospec);
   if (! (flags & ipdvosUPDATE_EXT))
      ext = ipdVosGetExt(vospec);

   if ((node == ipdNULL) && (device == ipdNULL) && (path == ipdNULL) &&
       (name == ipdNULL) && (ext == ipdNULL))
      retval = ipdNULL;		/* nothing left */
   else
      retval = ipdvosCreateDirectVospec(node, device, path, name, ext,
				     ipdVosGetSystype(vospec));
   ipdVeReturn(retval);
}

/** @brief Creates a copy of a system independent file specification vospec.

    @param vospec File specification to copy
    @return System independent file specification
 */
ipdvosFile ipdvosCopyVospec(ipdvosFile vospec)
{
   ipdConstString node, device, path, name, ext;
   ipdvosFile file;
   ipdVeStart("ipdvosCopyVospec");

   if (vospec == ipdNULL)
      ipdVeReturn(ipdNULL);

   node = ipdVosGetNode(vospec);
   device = ipdVosGetDevice(vospec);
   path = ipdVosGetPath(vospec);
   name = ipdVosGetName(vospec);
   ext = ipdVosGetExt(vospec);

   file = ipdvosCreateDirectVospec(node, device, path, name, ext,
				ipdVosGetSystype(vospec));
   if (file != ipdNULL)
      file->version = vospec->version;
   ipdVeReturn(file);
}

/** @brief Creates a copy of a system independent file specification vospec, but only things marked in the flags are copied. 
    @param vospec File specification to copy
    @param flags Copy flags
    @return System independent file specification
*/
ipdvosFile ipdvosCopyVospecParts(
	ipdvosFile vospec,
	ipdLongWord flags)
{
   ipdConstString node, device, path, name, ext;
   ipdvosFile file;
   ipdVeStart("ipdvosCopyVospecParts");

   if (vospec == ipdNULL)
      ipdVeReturn(ipdNULL);

   node = (flags & ipdvosOUTPUT_IPDNODE) ? ipdVosGetNode(vospec) : ipdNULL;
   device = (flags & ipdvosOUTPUT_DEVICE) ? ipdVosGetDevice(vospec) : ipdNULL;
   path = (flags & ipdvosOUTPUT_PATH) ? ipdVosGetPath(vospec) : ipdNULL;
   name = (flags & ipdvosOUTPUT_NAME) ? ipdVosGetName(vospec) : ipdNULL;
   ext = (flags & ipdvosOUTPUT_EXT) ? ipdVosGetExt(vospec) : ipdNULL;

   if ((node == ipdNULL) && (device == ipdNULL) && (path == ipdNULL) &&
       (name == ipdNULL) && (ext == ipdNULL))
     file = ipdNULL;		/* nothing left */
   else {
      file = ipdvosCreateDirectVospec(node, device, path, name, ext,
				   ipdVosGetSystype(vospec));
      if ((file != ipdNULL) && (flags & ipdvosOUTPUT_VERSION))
	 file->version = vospec->version;
   }
   ipdVeReturn(file);
}


/** @brief Concatenates two system independent file specifications into one single vospec.

    The only concatenated part is the path specification. The
    node and device are from the first specification, the name and the
    extension from the second one. 

    @param vospec1 First specification
    @param vospec2 Second specification
    @return System independent file specification
*/
ipdvosFile ipdvosSpecat(
	ipdvosFile vospec1,
	ipdvosFile vospec2)
{
   ipdConstString node, device, path, name, ext;
   ipdConstString p1, p2;
   ipdvosFile retval, v1, v2;
   ipdChar* unused_ret_val_char_pnt; /* [patch] vor capturing unused return values */   
   ipdVeStart("ipdvosSpecat");

   if (vospec1 == ipdNULL) {
      retval = ipdvosCopyVospecParts(vospec2, ipdvosOUTPUT_PATH | ipdvosOUTPUT_NAME |
				  ipdvosOUTPUT_EXT);
      ipdVeReturn(retval);
   } else if (vospec2 == ipdNULL) {
      retval = ipdvosCopyVospecParts(vospec1, ipdvosOUTPUT_PATH | ipdvosOUTPUT_DEVICE |
				  ipdvosOUTPUT_IPDNODE);
      ipdVeReturn(retval);
   }
   if (ipdVosGetSystype(vospec1) != ipdVosGetSystype(vospec2)) {
      ipdVeError(ipdveARG,
	      "VOSPECs to concanate are valid for different operating systems, returning ipdNULL");
      ipdVeReturn(ipdNULL);
   }

   /* this is for concealed devices */
   v1 = (ipdVosGetDevice(vospec1) != ipdNULL) ? ipdvosExpandVospec(vospec1) : vospec1;
   v2 = (ipdVosGetDevice(vospec2) != ipdNULL) ? ipdvosExpandVospec(vospec2) : vospec2;

   node = ipdVosGetNode(v1);
   device = ipdVosGetDevice(v1);
   p1 = ipdVosGetPath(v1);
   p2 = ipdVosGetPath(v2);
   if (p2 == ipdNULL)
      path = p1;		/* p1 == ipdNULL is OK */
   else if ((p1 == ipdNULL) || ipdvosIsTopPath(v2))
      path = p2;		/* p2 is not ipdNULL (case above) */
   else {
      /** p1,p2 is not ipdNULL, p2 is not TOP */
      ipdString p, sbuf;
      ipdLong l1, l2;

      l1 = ipdvStrlen(p1);
      l2 = ipdvStrlen(p2);
      sbuf = (ipdString)ipdvMalloc(l1 + l2 + 3);
      if (sbuf == ipdNULL) {
	 ipdVeError(ipdveSYS,
		 "unable to allocate memory to merge directory path");
	 ipdVeReturn(ipdNULL);
      }
      path = sbuf;
      unused_ret_val_char_pnt = ipdvStrcpy(sbuf, p1); /* [patch] removed warning: unused retval */

      if (*p2 == ipdvosTHIS_CHAR) {
	 p2++;			/* read over PATH_THIS */
	 if ((*p2 == ipdvosPATHSEP_CHAR) && *(p2+1) == '\0')
	    p2++;		/* read over PATH_THIS */
      }
      while ((*p2 == ipdvosUP_CHAR) &&
	     ((p = ipdvStrrchr(path, ipdvosPATHSEP_CHAR)) != ipdNULL)) {
	 p2++;			/* remove PATH_UP */
	 *p = '\0';		/* remove last path element */
	 l1 = ipdvStrlen(path);
      }
      while (*p2 == ipdvosUP_CHAR) {
	 p2++;			/* read over PATH_CHAR */
	 sbuf[1] = '\0';	/* just to be sure */
      }
      if ((sbuf[l1-1] == ipdvosPATHSEP_CHAR) && (*p2 == ipdvosPATHSEP_CHAR))
	 p2++;			/* remove doubling PATHSEP */

      unused_ret_val_char_pnt = ipdvStrcat(sbuf, p2);	/* append second path */ /* [patch] removed warning: unused retval */
   }
   name = ipdVosGetName(v2);
   ext = ipdVosGetExt(v2);

   if ((device != ipdNULL) && (path != ipdNULL)) {
      ipdString p = (ipdString)path;
      *p = ipdvosTOP_CHAR;		/* only top path supported (if both given) */
   }

   retval = ipdvosCreateDirectVospec(node, device, path, name, ext,
				  ipdVosGetSystype(vospec1));
   if (v1 != vospec1) ipdvosFreeVospec(v1);
   if (v2 != vospec2) ipdvosFreeVospec(v2);
   if ((path != p1) && (path != p2))
      ipdvFree(path);		/* temporary concat */
   ipdVeReturn(retval);
}

/** @brief Add the given directory to the path element of the file specification. 
    @param vospec File specification
    @param dirname Name of subdirectory
    @param hidden Is it a hidden directory?
    @return System independent file specification
*/
ipdvosFile ipdvosDownDirectory(
	ipdvosFile vospec,
	ipdConstString dirname,
	ipdBoolean hidden)
{
   ipdConstString node, device, path;
   ipdvosFile file;
   ipdString npath;
   ipdString buf;
   ipdLong plen;
   ipdvosSys systype;
   ipdChar* unused_ret_val_char_pnt; /* [patch] vor capturing unused return values */   
   ipdVeStart("ipdvosDownDirectory");

   if (dirname == ipdNULL) {
      ipdvosFile copy;
      copy = ipdvosCopyVospec(vospec); /* just a copy */
      ipdVeReturn(copy);
   }
   if (vospec == ipdNULL) {
      node = device = ipdNULL;
      path = ipdvosTHIS_CHARSTRING, plen = 1;
      systype = ipddefsystem;
   } else {
      node = ipdVosGetNode(vospec);
      device = ipdVosGetDevice(vospec);
      path = ipdVosGetPath(vospec);
      if (path == ipdNULL)
	 path = ipdvosTHIS_CHARSTRING, plen = 1;
      else
	 plen = ipdvStrlen(path);
      systype = ipdVosGetSystype(vospec);
   }
   buf = (ipdString)ipdvAlloca(plen+ipdvStrlen(dirname)+ipdsystems[systype].hidden.len+3);
   ipdvMemcpy(buf, path, plen);	/* we need local copy */
   npath = buf+plen;
   *npath++ = ipdvosPATHSEP_CHAR;	/* internal representation */
   if (hidden) {
      ipdvMemcpy(npath, ipdsystems[systype].hidden.str,
	      ipdsystems[systype].hidden.len);
      npath+=ipdsystems[systype].hidden.len;
   }
   unused_ret_val_char_pnt = ipdvStrcpy(npath, dirname);	/* add dirname */ /* [patch] removed warning: unused retval */
   file = ipdvosCreateDirectVospec(node, device, buf, ipdNULL, ipdNULL, systype);
   ipdvAllocaFree(buf);
   ipdVeReturn(file);
}

/** @brief Returns the separator for lists in the environment */
ipdChar ipdvosGetListSeparator()
{
   return(ipdsystems[ipddefsystem].listsep);
}

