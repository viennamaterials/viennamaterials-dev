/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

static char const _rcsid_ [] = "$Id: vosconv.c$";

/** @file vosconv.c
    @brief Conversions within the virtual directory structure
*/

/***I */
#include "../../include/ipdtype.h"
#include "verr.h"
#include "vosP.h"
#include "vdir.h"
#include "vdirP.h"

ipdvosDefinition ipdsystems[ipdvosKIPDNOWN_SYS] =
 {{ "unix",                     /* all UNIX derivates and CYGNUS WindowsNT */
   {":", 1},
#if defined(__CYGWIN__) || defined(__OPENNT) || defined(apollo)
   {"//", 2},        /* UNIX has no devivce support in pathes */
#else
   {ipdNULL, 0},        /* UNIX has no devivce support in pathes */
#endif /* defined(__CYGWIN__) || defined(__OPENNT) || defined(apollo) */
   {ipdNULL, 0}, {"/", 1}, {"/", 1}, {".", 1}, {ipdNULL, 0},
   {"/", 1}, {".", 1},  {"..", 2}, {".", 1},
   ipdvosMIXED_CASE,
   ipdTRUE,  ipdFALSE, ipdNULL, ipdNULL,
   ipdFALSE, ipdFALSE, ipdTRUE,  ipdFALSE, ipdTRUE,  ipdFALSE,
   '$', ':', '-', '\0',
   {"~", 1},
   ipdNULL, ipdNULL,
   {"-vbck", 5} },
 { "vms",                       /* all VMS machines */
   {"::", 2}, {":", 1},
   {"[", 1}, {".", 1}, {"]", 1}, {".", 1}, {";", 1},
   {"000000", 6}, {"", 0}, {"-", 1}, {"$", 1}, /* not really hidden, but */
   ipdvosUPPER_CASE,
   ipdFALSE, ipdTRUE,  "DIR", "000000",
   ipdTRUE,  ipdTRUE,  ipdFALSE, ipdTRUE,  ipdFALSE, ipdTRUE,
   '\0', ',', '/', '-',		/* use - for VMS too */
   {"SYS$LOGIN:", 10},
   "EXE", "COM",
   {"$vbck", 5} },
 { "winnt",                       /* all WindowsNT machines */
   {"\\\\", 2}, {":", 1},
   {ipdNULL, 0}, {"\\", 1}, {"\\", 1}, {".", 1}, {ipdNULL, 0},
   {"\\", 1}, {".", 1}, {"..", 1}, {".", 1}, /* not really hidden, but */
   ipdvosCOMMON_CASE,              /* store case, but not relevant */
   ipdFALSE, ipdFALSE,  ipdNULL, ipdNULL,
   ipdFALSE,  ipdFALSE,  ipdTRUE, ipdFALSE,  ipdTRUE, ipdFALSE,
   '%', ';', '/', '-',		/* use - for WindowsNT too */
   {"~", 1},
   "exe", "bat",
   {"-vbck", 5} }
};

#if defined(unix) || defined(__unix)
ipdvosSys ipddefsystem = ipdvosUNIX;	/* use UNIX file/dir specification */
#endif /* defined(unix) || defined(__unix) */

static struct _Home_ {
   ipdBoolean init;
   ipdString home1;		/* getenv(HOME) */
   ipdLong len1;
   ipdString home2;		/* directory(getenv(HOME)) */
   ipdLong len2;
} Home;

/** @brief This free function takes care of the allocated memory */
ipdVoid ipdFreeVosConv()
{
	free(Home.home1);
	free(Home.home2);
}

/** @brief Init HOME paths
    @param sys Systems definitions
 */
static ipdVoid getHomes(ipdvosDefPtr sys)
{
   ipdConstString name;
   ipdChar* unused_ret_val_char_pnt; /* [patch] for capturing unused return values */   
   ipdVeStart("getHomes");

   if (! Home.init) {
      Home.init = ipdTRUE;
      name = ipdGetHomeDirectory();
      if (name != ipdNULL) {
	 ipdChar buf[IPDMAXPATHLEN+1];
	 ipdConstString dir = ipdGetDirectory();
	 ipdString hname = ipdvStrdup(name);

	 Home.home1 = hname;
	 Home.len1 = ipdvStrlen(hname);
	 if (ipdvStrcmp(Home.home1 + Home.len1 - sys->pathsepr.len,
		     sys->pathsepr.str) == 0) {
	    Home.home1[Home.len1 - sys->pathsepr.len] = '\0'; /* remove */
	    Home.len1 -= sys->pathsepr.len;
	 }
	 Home.home2 = ipdNULL;
	 if (dir != ipdNULL) {
	    ipdChar nbuf[IPDMAXPATHLEN+1];

	    unused_ret_val_char_pnt = ipdvStrcpy(buf, dir);	/* save actual directory */ /* [patch] removed warning: unused retval */
	    if (ipdChangeDirectory(name) == ipdTRUE) {
				/* switch to home directory */
	       ipdClearDirectoryCache();
	       dir = ipdGetDirectory();
	       if ((dir != ipdNULL) && (ipdvStrcmp(dir, Home.home1) != 0)) {
		  Home.home2 = ipdvStrdup(dir); /* second possiblity for home */
		  Home.len2 = ipdvStrlen(dir);
		  if (ipdvStrcmp(Home.home2 + Home.len2 - sys->pathsepr.len,
			      sys->pathsepr.str) == 0) {
		     Home.home2[Home.len2 - sys->pathsepr.len] = '\0';
		     Home.len2 -= sys->pathsepr.len;
		  }
		  /* try to normalize current directory */
		  if (ipdvStrncmp(Home.home2, buf, Home.len2) == 0) {
		     ipdString nptr = nbuf;
		     ipdvMemcpy(nptr, Home.home1, Home.len1);
		     nptr += Home.len1;
		     unused_ret_val_char_pnt = ipdvStrcpy(nptr, buf+Home.len2); /* [patch] removed warning: unused retval */
		     dir = nbuf;
		  } else
		     dir = buf;
	       } else {
		  Home.home2 = ipdNULL; /* not available or equal home1 */
		  dir = buf;
	       }
	    }
	    ipdChangeDirectory(buf); /* switch back directory */
	 } else {
	    ipdVeWarning(ipdveEXT,
		      "unable to get working directory, switching to home directory");
	    ipdChangeDirectory(hname);
	 }
      } else {
	 Home.home1 = ipdNULL;
	 Home.home2 = ipdNULL;	/* nothing available */
      }

   }
   ipdVeReturnVoid;   
}

/** @brief Parses a system dependent file or directory specification and allocates an operating system independent structure.

    Parses a system dependent file or directory specification and allocates an operating system independent structure., in
    which the information is written. For conversion the VOS definition
    structure pointed to by sys is used. On success, the pointer to this
    structure is returned, else ipdNULL is returned. 

    @param ospec Operating system dependent file or directory specification
    @param directory Take specification as directory
    @param systype Type of system
    @param fileonly Parse as file only
*/
ipdvosFile ipdvosSys2Vospec(
	ipdConstString ospec,	
	ipdBoolean directory,	
	ipdvosSys systype,		
	ipdBoolean fileonly)	
{
   ipdvosDefPtr sys = &ipdsystems[systype];
   ipdLong len;			/* length of information in buffer */
   ipdString node, device, path, name, ext;
				/* parts of input */
   ipdLong version, devicelen = 0, nodelen = 0;
   ipdString scan;		/* scanning position */
   ipdString pos;
   ipdvosFile file;		/* file pointer */
   ipdBoolean havepsep;
   ipdChar sbuf[IPDMAXPATHLEN + IPDMAXNAMLEN + 10]; /* buffer for writing scanned
					       parts (sure too big) */
   ipdChar* unused_ret_val_char_pnt; /* [patch] vor capturing unused return values */
   ipdVeStart("ipdvosSys2Vospec");

   if (ospec == ipdNULL) {
      ipdVeWarning(ipdveARG,
		"ipdNULL pointer as system specific file specification, returning ipdNULL");
      ipdVeReturn(ipdNULL);		/* check this case */
   }

   unused_ret_val_char_pnt = ipdvStrcpy(sbuf, ospec);	/* generate working copy */ /* [patch] removed warning: unused retval */
   switch(sys->ncase) {
   case ipdvosUPPER_CASE:
      ipdvToupper(sbuf);		/* only upper case characters */
      break;
   case ipdvosLOWER_CASE:
      ipdvTolower(sbuf);		/* only lower case character */
      break;
   case ipdvosMIXED_CASE: case ipdvosCOMMON_CASE:
      break;
   }

   if (fileonly) {
      /* is much faster this way */
      node = ipdNULL;
      device = ipdNULL;
      path = ipdNULL;
      /* initialize suggested length */
      scan = sbuf;		/* scan pointer */
      len = 0;
   } else {
      if (directory &&
	  ((sys->pathsepl.str != ipdNULL) ?
	   (ipdvStrstr(sbuf, sys->pathsepl.str) == ipdNULL) :
	   ((sys->dirsep.str != ipdNULL) && (ipdvStrstr(sbuf, sys->dirsep.str) == ipdNULL))) &&
	  ((sys->devicesep.str == ipdNULL) || (ipdvStrstr(sbuf, sys->devicesep.str) == ipdNULL))) {
	 /* directory forced, but no path elements found */
	 if (!sys->dirext)
	    /* assume a directory */
	    ipdVeReturn(ipdvosDownDirectory(ipdNULL, sbuf, ipdFALSE));/* use utility function */
	 else if ((sys->extsep.str != ipdNULL) &&
		  (((pos = ipdvStrrstr(sbuf, sys->extsep.str)) == ipdNULL) ||
		   (pos[sys->extsep.len] == '\0') ||
		   (sys->nodirext &&
		    (ipdvStrcmp(pos+sys->extsep.len, sys->dirext) == 0)))) {
	    /* checked for directory extension (missing) */
	    if (pos != ipdNULL) *pos = '\0';/* kill empty extension */
	    ipdVeReturn(ipdvosDownDirectory(ipdNULL, sbuf, ipdFALSE));/* use utility function */
	 }
      }
      getHomes(sys);		/* get home directories */
      if ((Home.home1 != ipdNULL) &&
	  (ipdvStrncmp(sys->home.str, sbuf, sys->home.len) == 0)) {
	 if (((sys->pathsepl.str != ipdNULL) &&
	      (ipdvStrncmp(sys->pathsepl.str, sbuf+sys->home.len,
			len = sys->pathsepl.len) == 0)) ||
	     (ipdvStrncmp(sys->dirsep.str, sbuf+sys->home.len,
		       len = sys->dirsep.len) == 0) ||
	     (sbuf[sys->home.len] == (ipdChar)(len = 0))) {
	    /** OK, that a shortcut for the home directory... */
	    ipdChar nsbuf[IPDMAXPATHLEN + IPDMAXNAMLEN + 10]; /* buffer sure to big */
	    ipdString nptr = nsbuf;
	    ipdConstString r = sbuf + sys->home.len + len;

	    ipdvMemcpy(nptr, Home.home1, Home.len1);
	    nptr += Home.len1;
	    if (*r == '\0') {
	       ipdvMemcpy(nptr, sys->pathsepr.str, sys->pathsepr.len); /* no more path */
	       nptr += sys->pathsepr.len;
	    } else if (ipdvStrncmp(r, sys->pathsepr.str, sys->pathsepr.len) != 0) {
	       ipdvMemcpy(nptr, sys->dirsep.str, sys->dirsep.len); /* have some more path */
	       nptr += sys->dirsep.len;
	    }
	    unused_ret_val_char_pnt = ipdvStrcpy(nptr, r); /* [patch] removed warning: unused retval */ 
	    unused_ret_val_char_pnt = ipdvStrcpy(sbuf, nsbuf);	/* now we have the real path */ /* [patch] removed warning: unused retval */
	 } else {
	    /** this may be the homedirectory of another user */
	    ipdChar *start = sbuf+sys->home.len; /* first character of user */
	    ipdChar *ptr;
	    ipdConstString homedir;
	    if (sys->pathsepl.str != ipdNULL) {
	       ptr = ipdvStrstr(start, sys->pathsepl.str);
	       len = sys->pathsepl.len;
	    } else {
	       ptr = ipdvStrstr(start, sys->dirsep.str);
	       len = sys->dirsep.len;
	    }

	    /** OK we found a username, let's try to find a homedirectory
	     for it */
	    if (ptr != ipdNULL) {
	       ipdChar saved;
	       saved = *ptr; /* save for restore */
	       *ptr = '\0';
	       homedir = ipdGetHomeDirectoryByUserName(start); /* get dir */
	       *ptr = saved;	/* restore buffer */
	    } else
	       homedir = ipdGetHomeDirectoryByUserName(start); /* get dir */
	    if (homedir != ipdNULL) {
	       ipdLong hlen = ipdvStrlen(homedir);
	       /** we now know the directory */
	       /** OK, that's a shortcut for the home directory... */
	       ipdChar nsbuf[IPDMAXPATHLEN + IPDMAXNAMLEN + 10];
	       ipdString nptr = nsbuf;

	       ipdvMemcpy(nptr, homedir, hlen);
	       nptr += hlen;
	       if (ptr != ipdNULL) {
		  if (ipdvStrncmp(nptr-sys->pathsepr.len,
			       sys->pathsepr.str, sys->pathsepr.len) == 0)
		     nptr -= sys->pathsepr.len;	/* remove part */
		  ipdvMemcpy(nptr, sys->dirsep.str, sys->dirsep.len); /* have some more path */
		  nptr += sys->dirsep.len;
		  unused_ret_val_char_pnt = ipdvStrcpy(nptr, ptr+len); /* [patch] removed warning: unused retval */
	       } else
		  ipdvMemcpy(nptr, sys->pathsepr.str, sys->pathsepr.len); /* end of path */
	     unused_ret_val_char_pnt = ipdvStrcpy(sbuf, nsbuf); /* now we have the real path */ /* [patch] removed warning: unused retval */
	    }
	 }
      }
      if ((Home.home1 != ipdNULL) && (Home.home2 != ipdNULL) &&
	  (ipdvStrncmp(Home.home2, sbuf, Home.len2) == 0)) {
	 /* OK, that is the long form of the home directory */
	 ipdChar nsbuf[IPDMAXPATHLEN + IPDMAXNAMLEN + 10]; /* buffer sure to big */
	 ipdString nptr = nsbuf;

	 ipdvMemcpy(nptr, Home.home1, Home.len1);
	 nptr += Home.len1;
	 unused_ret_val_char_pnt = ipdvStrcpy(nptr, sbuf+Home.len2); /* [patch] removed warning: unused retval */
	 unused_ret_val_char_pnt = ipdvStrcpy(sbuf, nsbuf);	/* now we have the real path */ /* [patch] removed warning: unused retval */
      } else if (sys->envchar && (sbuf[0] == sys->envchar)) {
	 /* use environmentvariable for path */
	 ipdConstString new;
	 ipdString pos = ipdNULL;
	 ipdChar c = 0; /* [patch] removed warning: initialized the char */
	 if (sys->pathsepl.str != ipdNULL)
	    pos = ipdvStrstr(sbuf, sys->pathsepl.str);
	 if ((pos == ipdNULL) && (sys->pathsepr.str != ipdNULL))
	    pos = ipdvStrstr(sbuf, sys->pathsepr.str);
	 if (pos != ipdNULL) {
	    c = *pos; *pos = '\0';
	 }
	 new = ipdvosGetEnvironment(&sbuf[1]); /* get environment variable */
	 if (pos != ipdNULL) {
	    *pos = c;		/* reset character */
	    if (new != ipdNULL) {
	       ipdChar nsbuf[IPDMAXPATHLEN + IPDMAXNAMLEN + 10]; /* buffer sure to big */
	       unused_ret_val_char_pnt = ipdvStrcpy(nsbuf, new); /* [patch] removed warning: unused retval */
	       unused_ret_val_char_pnt = ipdvStrcat(nsbuf, pos); /* append rest value */ /* [patch] removed warning: unused retval */
	       unused_ret_val_char_pnt = ipdvStrcpy(sbuf, nsbuf); /* store new value */ /* [patch] removed warning: unused retval */
	    }
	 } else if (new != ipdNULL)
	    unused_ret_val_char_pnt = ipdvStrcpy(sbuf, new);	/* just get new name */ /* [patch] removed warning: unused retval */
	 ipdvFree(new);
      }

      /* initialize suggested length */
      scan = sbuf;		/* scan pointer */
      len = 0;
      if ((sys->nodesep.str != ipdNULL) &&
	  ((pos = ipdvStrstr(scan, sys->nodesep.str)) != ipdNULL)) {
	 node = scan;
	 scan = pos + sys->nodesep.len;
	 *pos = '\0';		/* end of string */
	 if (((sys->devicesep.str != ipdNULL) &&
	      (ipdvStrstr(node, sys->devicesep.str) != ipdNULL)) ||
	     ((sys->pathsepl.str != ipdNULL) &&
	      (ipdvStrstr(node, sys->pathsepl.str) != ipdNULL)) ||
	     (ipdvStrstr(node, sys->dirsep.str) != ipdNULL)) {
	    /* this is not a real node separator (wrong decision) */
	    scan = node;           /* start again from beginning */
	    node = ipdNULL;
	    *pos = sys->nodesep.str[0]; /* restore string */
	 } else {
	    nodelen = ipdvStrlen(node);
	    len += nodelen + 1;
	 }
      } else
	 node = ipdNULL;

      if ((sys->devicesep.str != ipdNULL) &&
	  ((pos = ipdvStrstr(scan, sys->devicesep.str)) != ipdNULL)) {
	 if (sys->deviceafter) {
	    scan = pos + sys->devicesep.len;
	    if ((((sys->pathsepl.str != ipdNULL) &&
		  ((pos = ipdvStrstr(scan, sys->pathsepl.str)) != ipdNULL)) ||
		 (pos = ipdvStrstr(scan, sys->topdir.str)) != ipdNULL) &&
		(pos != scan)) {	/* no empty device */
	       ipdChar saved = *pos;
	       device = scan;
	    scan = pos;
	       *pos = '\0';
	       devicelen = ipdvStrlen(device);
	       len += devicelen + 1;
	       *pos = saved;
	    } else {
	       devicelen = 0;
	       device = ipdNULL;
	    }
	 } else {
	    device = scan;
	    scan = pos + sys->devicesep.len;
	    *pos = '\0';		/* end of string */
	    devicelen = ipdvStrlen(device);
	    len += devicelen + 1;
	 }
      } else {
	 devicelen = 0;
	 device = ipdNULL;
      }
      if ((sys->pathsepl.str != ipdNULL) &&
	  ((pos = ipdvStrstr(scan, sys->pathsepl.str)) != ipdNULL)) {
	 scan = pos + sys->pathsepl.len;
	 path = scan;
	 *pos = '\0';		/* end of string */
	 havepsep = ipdTRUE;
      } else {
	 path = scan;
	 havepsep = ipdFALSE;
      }

      if ((sys->pathsepr.str != ipdNULL) &&
	  ((pos = ipdvStrrstr(scan, sys->pathsepr.str)) != ipdNULL)) {
	 scan = pos + sys->pathsepr.len;
	 *pos = '\0';		/* end of string */
	 len += ipdvStrlen(path) + 1;
      } else {
	 if (havepsep) {
	    ipdVeError1(ipdveARG,
		     "found left path separator in %s, but no corresponding right path separator",
		     ospec);
	 ipdVeReturn(ipdNULL);
	 }
	 path = ipdNULL;
      }
   }

   if ((sys->versionsep.str != ipdNULL) &&
       (pos = ipdvStrrstr(scan, sys->versionsep.str))) {
      *pos = '\0';		/* just ignore all versions information */
      pos += sys->versionsep.len;
      version = (*pos == '\0') ? 0 : atol(pos);
   } else
      version = 0;		/* actual */

   if (*scan) {
      name = scan;
      if ((sys->extsep.str != ipdNULL) &&
	  ((pos = ipdvStrrstr(scan, sys->extsep.str)) != ipdNULL)) {
	 *pos = '\0';		/* found name */
	 scan = pos + sys->extsep.len;
	 ext = scan;
	 if (*scan) {		/* found extension */
	    if (directory && sys->nodirext &&
		(ipdvStrcmp(ext, sys->dirext) != 0)) {
	       ipdVeError2(ipdveARG,
			"invalid extension %s for directory %s",
			ext, ospec);
	       ipdVeReturn(ipdNULL);	/* is no directory name (wrong extension) */
	    }
	    len += ipdvStrlen(ext) + 1;
	 } else if (directory && sys->nodirext) {
	    ipdVeError2(ipdveARG,
		     "missing extension %s for directory %s",
		     sys->dirext, ospec);
	    ipdVeReturn(ipdNULL);	/* there has to be an extension */
	 } else if (sys->extsepreq)
	    ext = ipdNULL;	/* empty extension ignored if seperator required */
	 if (*name == '\0')
	    name = ipdNULL;	/* name not needed, since we have extension */
      } else if (directory && sys->nodirext) {
	 ipdVeError2(ipdveARG,
		  "missing extension %s for directory %s",
		  sys->dirext, ospec);
	 ipdVeReturn(ipdNULL);	/* there has to be an extension */
      } else
	 ext = ipdNULL;
      if (name)
	 len += ipdvStrlen(name) + 1;
   } else {
      name = ipdNULL;
      ext = ipdNULL;
   }

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
   if (node) {
      file->node = pos;
      ipdvMemcpy(pos, node, nodelen);
      pos += nodelen;
      *pos++ = '\0';
   } else
      file->node = ipdNULL;

   /* enter the device specification */
   if (device) {
      file->device = pos;
      ipdvMemcpy(pos, device, devicelen);
      pos += devicelen;
      *pos++ = '\0';
   } else
      file->device = ipdNULL;

   /* enter the path specification */
   if (path) {
      file->path = pos;
      *pos = '\0';

      if ((sys->topdir.str != ipdNULL) &&
	  ((ipdvStrncmp(sys->topdir.str, path, sys->topdir.len) == 0) ||
	   ((*path == '\0') &&
	    (ipdvStrcmp(sys->topdir.str, sys->pathsepr.str) == 0)))) {
	 *pos++ = ipdvosTOP_CHAR;
	 if (*path != '\0')
	    path += sys->topdir.len;
      } else if ((sys->thisdir.str != ipdNULL) &&
	  (ipdvStrncmp(sys->thisdir.str, path, sys->thisdir.len) == 0) &&
	  (ipdvStrncmp(sys->dirsep.str, path + sys->thisdir.len,
		    sys->dirsep.len) == 0)) {
	 *pos++ = ipdvosTHIS_CHAR;
	 path += sys->thisdir.len + sys->dirsep.len;
      } else if ((sys->thisdir.str != ipdNULL) &&
	  (ipdvStrncmp(sys->thisdir.str, path, sys->thisdir.len) == 0) &&
	  (*(path + sys->thisdir.len) == '\0')) {
	 *pos++ = ipdvosTHIS_CHAR;
	 path += sys->thisdir.len;
      }

      /* check all up pathes */
      while ((sys->updir.str != ipdNULL) &&
	     (ipdvStrncmp(sys->updir.str, path, sys->updir.len) == 0)) {
	 if (ipdvStrncmp(sys->dirsep.str, path + sys->updir.len,
		      sys->dirsep.len) == 0) {
	    *pos++ = ipdvosUP_CHAR;
	    path += sys->updir.len + sys->dirsep.len;
	 } else if (*(path + sys->updir.len) == '\0') {
	    *pos++ = ipdvosUP_CHAR;
	    path += sys->updir.len;
	 } else
	    continue;
      }
      if ((*file->path == '\0') &&
	  (sys->topnospec == ipdTRUE))
	 *pos++ = ipdvosTOP_CHAR;	/* implicit TOP */
      else if ((*file->path == '\0') &&
	  (sys->thisnospec == ipdTRUE))
	 *pos++ = ipdvosTHIS_CHAR;	/* implicit THIS */

      /* check all normal pathes */
      while ((scan = ipdvStrstr(path, sys->dirsep.str)) != ipdNULL) {
	 if (scan != path) {
	    *scan = '\0';
	    if (pos[-1] != ipdvosPATHSEP_CHAR) {
	       *pos++ = ipdvosPATHSEP_CHAR;
	       if (ipdvStrcmp(path, sys->thisdir.str) == 0)
		  pos--;	/* ignore this directory here */
	       else {
		  ipdLong len = ipdvStrlen(path);
		  ipdvMemcpy(pos, path, len);	/* add path element */
		  pos += len;
	       }
	    }
	 }
	 path = scan + sys->dirsep.len;
	 if ((ipdvStrncmp(path, sys->pathsepr.str, sys->pathsepr.len) == 0) &&
	     (ipdvStrncmp(path+sys->pathsepr.len, sys->pathsepl.str, sys->pathsepl.len) == 0))
	    path += sys->pathsepr.len + sys->pathsepl.len; /* read over close/open */
      }

      if (*path) {
	 if ((sys->thisdir.str == ipdNULL) ||
	     (ipdvStrcmp(sys->thisdir.str, path) != 0)) {
	    ipdLong len = ipdvStrlen(path);
	    *pos++ = ipdvosPATHSEP_CHAR;
	    ipdvMemcpy(pos, path, len); /* add last path element */
	    pos += len;
	 }
      } else if (file->path == pos)
	 file->path = ipdNULL;	/* was no path of interest */
      *pos++ = '\0';
   } else
      file->path = ipdNULL;

   if (directory) {
      /* use name element as directory */
      if ((name != ipdNULL) ||
	  ((ext != ipdNULL) && !sys->nodirext)) {
	 ipdString npath = pos, xpath;
	 ipdString concat = ipdNULL;

	 if (file->path != ipdNULL) {
	    concat = npath - 1;
	    *concat = ipdvosPATHSEP_CHAR; /* append to path */
	 } else {
	    file->path = npath;	/* create a new path element */
	    *pos++ = ipdvosTHIS_CHAR;
	    *pos++ = ipdvosPATHSEP_CHAR; /* append to path */
	 }

	 xpath = pos;
	 if (name != ipdNULL) {
	    ipdLong len = ipdvStrlen(name);
	    ipdvMemcpy(pos, name, len);
	    pos += len;
	 }
	 if ((ext != ipdNULL) && !sys->nodirext) {
	    ipdLong len = ipdvStrlen(ext);
	    ipdvMemcpy(pos, sys->extsep.str, sys->extsep.len);
	    pos += sys->extsep.len; /* add extension separator */
	    ipdvMemcpy(pos, ext, len);
	    pos += len;
	 }
	 *pos = '\0';		/* terminate string */

	 if ((sys->thisdir.str != ipdNULL) && (sys->thisdir.len > 0) &&
	     (ipdvStrcmp(sys->thisdir.str, xpath) == 0)) {
	    pos = npath;	/* this directory (ignored here) */
	    if (concat != ipdNULL)
	       *concat = '\0';	/* cancel path appending */
	    else
	       file->path = ipdNULL; /* is no path */
	 }
	 if ((sys->updir.str != ipdNULL) &&
	     (ipdvStrcmp(sys->updir.str, xpath) == 0)) {
	    if (concat != ipdNULL) {
	       ipdString p;
	       *concat = '\0';	/* cancel path appending */
	       p = ipdvStrrchr(file->path, ipdvosPATHSEP_CHAR);
	       if (p != ipdNULL)
		  *p = '\0';	/* remove last path element (that's an up) */
	       else {
		  concat--;	/* move to last character of path */
		  if (*concat == ipdvosTHIS_CHAR)
		     *concat = ipdvosUP_CHAR; /* just replace */
		  else if (*concat == ipdvosUP_CHAR)
		     (*++concat) = ipdvosUP_CHAR; /* one more */
		  concat++;
		  pos = concat;
	       }
	    } else {
	       *npath++ = ipdvosUP_CHAR;
	       pos = npath;	/* up directory */
	    }
	 }
	 *pos++ = '\0';
      }
      file->name = ipdNULL;
      file->ext = ipdNULL;
      version = 0;		/* no versions for directories */
   } else {
      /* enter the name specification */
      if (name) {
	 ipdLong len = ipdvStrlen(name);
	 file->name = pos;
	 ipdvMemcpy(pos, name, len);
	 pos += len;
	 *pos++ = '\0';
      } else
	 file->name = ipdNULL;

      /* enter the ext specification */
      if (ext) {
	 ipdLong len = ipdvStrlen(ext);
	 file->ext = pos;
	 ipdvMemcpy(pos, ext, len);
	 pos += len;
	 *pos++ = '\0';
      } else
	 file->ext = ipdNULL;

      if ((name == ipdNULL) && (ext == ipdNULL))
	 version = 0;		/* is really a directory, so ignore any
				   version information */
   }
   file->version = version;

   /* enter the real length of specification */
   file->length = (pos - &file->vosbuf[0]);
   file->systype = systype;

   /* return the generated specification */
   ipdVeReturn(file);
}

/** @brief Parses a system dependent file or directory specification and allocates a operating system independent structure.

    Parses a system dependent file or directory specification and allocates a operating system independent structure, in
    which the information is written. On success, the pointer to this
    structure is returned, else ipdNULL. The user of this function is
    resposible for freeing the allocated ipdvosFile structure again.

    @param ospec Operating system dependent file or directory specification
    @param directory Take specification as directory
*/

ipdvosFile ipdvosOs2Vospec(
	ipdConstString ospec,	/* [I] operating system dependent file or
				   directory specification */
	ipdBoolean directory)	/* [I] take specification as directory */
{
   ipdvosFile file;
   ipdVeStart("ipdvosOs2Vospec");

   file = ipdvosSys2Vospec(ospec, directory, ipddefsystem, ipdFALSE);
   ipdVeReturn(file);
}

/** @brief Converts a system independent file specification into an operating system specific one.

    Converts a system independent file specification into an operating system specific one, which is returned as string through ipdvStrdup. If no memory is available ipdNULL is returned. 

    @param vospec Operating system independent file or directory specification
    @param flags Output specification flags
*/
ipdConstString ipdvosVos2Ospec(
	ipdvosFile vospec,
	ipdLongWord flags)
{
   ipdConstString str;
   ipdVeStart("ipdvosVos2Ospec");

   /** check for vospec */
   if (vospec == ipdNULL) {
      ipdVeWarning(ipdveARG,
		"ipdNULL pointer as system independent file specification, returning ipdNULL");
      ipdVeReturn(ipdNULL);		/* just return NULL too */
   }
   str = ipdvosVos2Sysspec(vospec, flags, ipdVosGetSystype(vospec));
   ipdVeReturn(str);
}

/** @brief Frees the memory associated with the given file specification. ipdNULL may be given as argument, in which case nothing will be freed. 
    @param vospec Specification to free
*/
ipdVoid ipdvosFreeVospec(
	ipdvosFile vospec)		/* [I] specification to free */
{
   if ((vospec != ipdNULL) &&
       (vospec->vosbuf[0] == ipdvosFILE_MAGIC))
      ipdvFree(vospec);		/* free only if check is OK */
}

/** @brief Converts a system independent file specification into an operating system specific one, which is returned as string through ipdvStrdup. 
    @param vospec Operating system independent file or directory specification
    @param flags Output specification flags
    @param systype Type of system
    @return If no memory is available, ipdNULL is returned. */
ipdConstString ipdvosVos2Sysspec(
	ipdvosFile vospec,
	ipdLongWord flags,
	ipdvosSys systype)
{
   ipdvosDefPtr sys = &ipdsystems[systype];
   ipdChar sbuf[IPDMAXPATHLEN + IPDMAXNAMLEN + 10]; /* buffer for writing scanned parts (sure too big) */
   ipdChar *ptr = sbuf;
   ipdChar* unused_ret_val_char_pnt; /* [patch] vor capturing unused return values */   
   ipdVeStart("ipdvosVos2Sysspec");

   /** check for vospec */
   if (vospec == ipdNULL) {
      ipdVeWarning(ipdveARG,
		"ipdNULL pointer as system independent file specification, returning ipdNULL");
      ipdVeReturn(ipdNULL);		/* just return NULL too */
   } else if (vospec->vosbuf[0] != ipdvosFILE_MAGIC) {
      ipdVeError(ipdveARG,
	      "parameter is no system independent file specification");
      ipdVeReturn(ipdNULL);
   }

   sbuf[0] = '\0';		/* init buffer */
   if (flags == 0) flags = ipdvosOUTPUT_ALL; /* used as default */

   /* output the node specification */
   if ((flags & ipdvosOUTPUT_IPDNODE) &&
       (ipdVosGetNode(vospec) != ipdNULL) &&
       (sys->nodesep.str != ipdNULL)) {
      ipdConstString node = ipdVosGetNode(vospec);
      ipdLong len = ipdvStrlen(node);
      ipdvMemcpy(ptr, node, len);
      ptr += len;
      ipdvMemcpy(ptr, sys->nodesep.str, sys->nodesep.len);
      ptr += sys->nodesep.len;
   }

   /* output the device specification */
   if ((flags & ipdvosOUTPUT_DEVICE) &&
       (ipdVosGetDevice(vospec) != ipdNULL) &&
       (sys->devicesep.str != ipdNULL)) {
      ipdConstString dev = ipdVosGetDevice(vospec);
      ipdLong len = ipdvStrlen(dev);
      if (sys->deviceafter) {
	 ipdvMemcpy(ptr, sys->devicesep.str, sys->devicesep.len);
	 ptr += sys->devicesep.len;
      }
      ipdvMemcpy(ptr, dev, len);
      ptr += len;
      if (!sys->deviceafter) {
	 ipdvMemcpy(ptr, sys->devicesep.str, sys->devicesep.len);
	 ptr += sys->devicesep.len;
      }
   }

   /* output the path specification */
   if ((flags & ipdvosOUTPUT_PATH) &&
       (ipdVosGetPath(vospec) != ipdNULL)) {
      ipdBoolean first = ipdTRUE;
      ipdBoolean dirpart = ipdFALSE;
      ipdConstString path = ipdVosGetPath(vospec);

      if (sys->pathsepl.str != ipdNULL) {
	 ipdvMemcpy(ptr, sys->pathsepl.str, sys->pathsepl.len);
	 ptr += sys->pathsepl.len;
      }
      do {
	 switch(*path) {
	 case ipdvosTOP_CHAR:	/* output top level directory */
	    if ((sys->topnospec == ipdTRUE) &&
		(path[1] == ipdvosPATHSEP_CHAR) &&
		(path[2] != '\0')) {
	       path++;		/* ignore top (not special) */
	    } else {
	       ipdvMemcpy(ptr, sys->topdir.str, sys->topdir.len);
	       ptr += sys->topdir.len;
	       if (sys->topneedssep) {
		  ipdvMemcpy(ptr, sys->dirsep.str, sys->dirsep.len);
		  ptr += sys->dirsep.len;
	       }
	       path++;		/* top was parsed */
	    }
	    break;

	 case ipdvosTHIS_CHAR:	/* output current directory */
	    if (sys->thisnospec == ipdFALSE) {
	       ipdvMemcpy(ptr, sys->thisdir.str, sys->thisdir.len);
	       ptr += sys->thisdir.len;
	       ipdvMemcpy(ptr, sys->dirsep.str, sys->dirsep.len);
	       ptr += sys->dirsep.len;
	    }
	    path++;
	    break;

	 case ipdvosUP_CHAR:	/* output upper directory */
	    ipdvMemcpy(ptr, sys->updir.str, sys->updir.len);
	    ptr += sys->updir.len;
	    ipdvMemcpy(ptr, sys->dirsep.str, sys->dirsep.len);
	    ptr += sys->dirsep.len;
	    path++;
	    break;

	 case ipdvosPATHSEP_CHAR:	/* end of one directory name */
	    if (first)
	       first = ipdFALSE;	/* first one is not printed */
	    else if (sys->dirsep.len == 1)
	       *ptr++ = sys->dirsep.str[0]; /* faster */
	    else {
	       ipdvMemcpy(ptr, sys->dirsep.str, sys->dirsep.len);
	       ptr += sys->dirsep.len;
	    }
	    path++;
	    break;

	 default:		/* just add character of directory name */
	    dirpart = ipdTRUE;
	    *ptr++ = *path++;
	    break;
	 }
      } while (*path != '\0');
      *ptr = '\0';
      if (dirpart == ipdFALSE) {
	 ipdLong dp = (ptr - sbuf) - sys->dirsep.len;
	 if ((dp >= 0) && (ipdvStrcmp(&sbuf[dp], sys->dirsep.str) == 0))
	    ptr -= sys->dirsep.len; /* clear additional directory separator */
	 if (ptr == sbuf) {
	    path = ipdVosGetPath(vospec);
	    if ((path[0] == ipdvosTOP_CHAR) &&
		(sys->topnospec == ipdTRUE)) {
	       ipdvMemcpy(ptr, sys->topdir.str, sys->topdir.len); /* top directory */
	       ptr += sys->topdir.len;
	    } else if ((path[0] == ipdvosTHIS_CHAR) &&
		     (sys->thisnospec == ipdTRUE)) {
	       ipdvMemcpy(ptr, sys->thisdir.str, sys->thisdir.len);
	       ptr += sys->thisdir.len;
	    }
	 }
      }
      if (((flags & ipdvosOUTPUT_EXT) && (ipdVosGetExt(vospec) != ipdNULL)) ||
          ((flags & ipdvosOUTPUT_NAME) && (ipdVosGetName(vospec) != ipdNULL)) ||
	  !(flags & ipdvosOUTPUT_IPDNODIRSEP) ||
          (sys->pathsepl.str != ipdNULL)) {
         ipdvMemcpy(ptr, sys->pathsepr.str, sys->pathsepr.len);
         ptr += sys->pathsepr.len;
      }
   }

   /* output the name specification */
   if ((flags & ipdvosOUTPUT_NAME) &&
       (ipdVosGetName(vospec) != ipdNULL)) {
      ipdConstString name = ipdVosGetName(vospec);
      ipdLong len = ipdvStrlen(name);
      ipdvMemcpy(ptr, name, len);
      ptr += len;
      if ((!(flags & ipdvosOUTPUT_EXT) || (ipdVosGetExt(vospec) == ipdNULL)) &&
	  (sys->extsepreq == ipdTRUE) && !(flags & ipdvosOUTPUT_IPDNOEXTSEP)) {
	 ipdvMemcpy(ptr, sys->extsep.str, sys->extsep.len);
	 ptr += sys->extsep.len;
      }
   }

   /* output the ext specification */
   if ((flags & ipdvosOUTPUT_EXT) &&
       (ipdVosGetExt(vospec) != ipdNULL)) {
      ipdConstString ext = ipdVosGetExt(vospec);
      ipdLong len = ipdvStrlen(ext);
      ipdvMemcpy(ptr, sys->extsep.str, sys->extsep.len);
      ptr += sys->extsep.len;
      ipdvMemcpy(ptr, ext, len);
      ptr += len;
   }

   /* output the version if available */
   if ((sys->versionsep.str != ipdNULL) &&
       (vospec->version != 0) &&
       (flags & ipdvosOUTPUT_NAME) && (flags & ipdvosOUTPUT_EXT) &&
       (flags & ipdvosOUTPUT_VERSION)) {
      ipdChar buffer[20];
      ipdLong len;

      ipdvMemcpy(ptr, sys->versionsep.str, sys->versionsep.len);
      ptr += sys->versionsep.len;
      sprintf(buffer, ipdvSHORT_FORMAT, vospec->version);
      len = ipdvStrlen(buffer);
      ipdvMemcpy(ptr, buffer, len);
      ptr += len;
   }
   *ptr = '\0';

   if ((sbuf == ptr) && (flags & ipdvosOUTPUT_PATH)) {
      /** all empty ? */
      if (sys->pathsepl.str != ipdNULL)
	 unused_ret_val_char_pnt = ipdvStrcpy(sbuf, sys->pathsepl.str); /* [patch] removed warning: unused retval */
      if (sys->thisdir.str != ipdNULL)
	 unused_ret_val_char_pnt = ipdvStrcat(sbuf, sys->thisdir.str); /* [patch] removed warning: unused retval */
      if (sys->pathsepr.str != ipdNULL)
	 unused_ret_val_char_pnt = ipdvStrcat(sbuf, sys->pathsepr.str); /* [patch] removed warning: unused retval */
      /* now we have the string for the current directory */
   }

   switch(sys->ncase) {
   case ipdvosUPPER_CASE:
      ipdvToupper(sbuf);		/* only upper case characters */
      break;
   case ipdvosLOWER_CASE:
      ipdvTolower(sbuf);		/* only lower case character */
      break;
   case ipdvosMIXED_CASE: case ipdvosCOMMON_CASE:
      break;
   }

   if ((flags & ipdvosOUTPUT_HOME) &&
       (sys->home.str != ipdNULL)) {
      ipdLong len;
      getHomes(sys);		/* get home directories */
      if (((Home.home1 != ipdNULL) &&
	   (ipdvStrncmp(Home.home1, sbuf, len = Home.len1) == 0)) ||
	  ((Home.home2 != ipdNULL) &&
	   (ipdvStrncmp(Home.home2, sbuf, len = Home.len2) == 0))) {
	 /* we change to short cut of home directory */
	 ipdChar nsbuf[IPDMAXPATHLEN + IPDMAXNAMLEN + 10]; /* buffer sure to big */
	 ipdString nptr = nsbuf;
	 ipdvMemcpy(nptr, sys->home.str, sys->home.len);
	 nptr += sys->home.len;
	 if (sys->pathsepl.str != ipdNULL) {
	    ipdvMemcpy(nptr, sys->pathsepl.str, sys->pathsepl.len);
	    nptr += sys->pathsepl.len; /* add path separator */
	 } else {
	    ipdvMemcpy(nptr, sys->dirsep.str, sys->dirsep.len);
	    nptr += sys->dirsep.len; /* add directory separator */
	 }
	 if (ipdvStrncmp(sbuf+len, sys->dirsep.str, sys->dirsep.len) == 0)
	    len += sys->dirsep.len; /* remove and preceeding directory
				       separator */
	 unused_ret_val_char_pnt = ipdvStrcpy(nptr, sbuf+len); /* add rest of path */ /* [patch] removed warning: unused retval */
	 ipdVeReturn(ipdvStrdup(nsbuf));
      } else
	 ipdVeReturn(ipdvStrdup(sbuf));
   } else
      ipdVeReturn(ipdvStrdup(sbuf));
}

