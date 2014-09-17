/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

static char const _rcsid_ [] = "$Id: vosdir.c$";

/** @file vosdir.c
    @brief More virtual directory handling
*/

/***I */
#include "../../include/ipdtype.h"
#include "verr.h"
#include "vdir.h"
#include "vos.h"
#include "vosP.h"



/** @brief Convert a file or directory to a file specification. This is important for OS where this directories are handled differently. 
    @param file Selected file
    @param dir Convert directory to file
    @return File specification
*/
ipdConstString ipdvosConvertFile(
	ipdvosFile file,
	ipdBoolean dir)
{
   ipdConstString fstr, logval;
   ipdvosFile cfile = ipdNULL;	/* for checks */
   ipdvosSys systype;
   ipdVeStart("ipdvosConvertFile");

   if (file == ipdNULL) {
      ipdVeWarning(ipdveARG,
		"ipdNULL vospec argument, returning ipdNULL");
      ipdVeReturn(ipdNULL);
   }

   systype = ipdVosGetSystype(file);
   if ((ipdvosIsTopPath(file) == ipdFALSE) &&
       (ipdVosGetDevice(file) == ipdNULL) && (ipdVosGetNode(file) == ipdNULL)) {
      ipdvosFile curdir = ipdvosGetCurrentDirectory();
      if (curdir != ipdNULL) {
	 cfile = ipdvosSpecat(curdir, file);
	 ipdvosFreeVospec(curdir);	/* free allocated one */
	 if (cfile != ipdNULL)
	    file = cfile;	/* now it is absolute */
      }
      /* for error cases try relative one */
   }

   while ((ipdVosGetDevice(file) != ipdNULL) &&
	  ((logval = ipdTranslateLogical(ipdVosGetDevice(file))) != ipdNULL)) {
      /* we have a logical value, translate it */
      ipdvosFile voslog, new;
      ipdChar first;
      ipdString path;
      voslog = ipdvosOs2Vospec(logval, ipdFALSE);
      if (voslog == ipdNULL) {
	 ipdVeWarning2(ipdveFCF,
		    "error converting logical value %s (value=%s) using it directly",
		    ipdVosGetDevice(file), logval);
	 break;
      } else {
	 path = (ipdString)ipdVosGetPath(file);
	 if ((path != ipdNULL) &&
	     (*path == ipdvosTOP_CHAR)) {
	    first = *path;	/* save for later */
	    *path = ipdvosTHIS_CHAR; /* for real concatenation */
	 } else
	    first = '\0';
	 new = ipdvosSpecat(voslog, file);
	 if (first != '\0')
	    *path = first;	/* restore */
	 ipdvosFreeVospec(voslog);
	 ipdvosFreeVospec(cfile);	/* free temporary vals */
	 file = cfile = new;	/* save new value (and try again) */
      }
   }

   if (dir && (ipdVosGetName(file) == ipdNULL) && (ipdVosGetExt(file) == ipdNULL)) {
      /* we have a directory, convert to right name */
      ipdChar *pos;

      if (ipdVosGetPath(file) == ipdNULL) {
	 ipdvosFreeVospec(cfile);	/* free temporary copy */
   	 ipdVeReturn (ipdNULL);	/* no file and path information ???? */
      }

      pos = ipdvStrrchr(ipdVosGetPath(file), ipdvosPATHSEP_CHAR);
      if (pos != ipdNULL) {
	 if (cfile == ipdNULL) {
	    cfile = ipdvosCopyVospec(file); /* we need a temporary copy */
	    pos = ipdvStrrchr(ipdVosGetPath(cfile), ipdvosPATHSEP_CHAR);
	 }
	 *pos++ = '\0';
	 cfile->name = pos;	/* last part is directory name */
	 if (ipdVosGetPath(cfile)[0] == '\0')
	    cfile->path = ipdNULL;	/* path is empty now */
	 if (ipdsystems[systype].nodirext == ipdTRUE)
	    cfile->ext = ipdsystems[systype].dirext;
      } else {
	 /* top directory */
	 if (cfile == ipdNULL)
	    cfile = ipdvosCreateDirectVospec(ipdVosGetNode(file),
					  ipdVosGetDevice(file),
					  ipdVosGetPath(file),
					  ipdsystems[systype].topdirname,
					  ipdsystems[systype].dirext,
					  systype);
	 else {
	    /* just update */
	    cfile->name = ipdsystems[systype].topdirname;
	    cfile->ext = ipdsystems[systype].dirext;
	 }
      }
      file = cfile;
   }

   /* we now have all of the directory done */
   fstr = ipdvosVos2Ospec(file, ipdvosOUTPUT_ALL);
   if (cfile != ipdNULL)
      ipdvosFreeVospec(cfile);	/* free temporary copy */

   ipdVeReturn(fstr);
}

/** @brief Returns the current directory in system independent (VOS) format.

    The returned ipdvosFile structure (vospec) is allocated automatically and has to be freed by the application after use.
*/
/***R ipdvosFreeVospec */
ipdvosFile ipdvosGetCurrentDirectory()
{
   ipdConstString dir;
   ipdvosFile vdir;
   ipdVeStart("ipdvosGetCurrentDirectory");

   dir = ipdGetDirectory();
   if (dir == ipdNULL) {
      ipdVeError(ipdveSYS,
	      "unable to get working directory, switching to home directory");
      dir = ipdGetHomeDirectory();
      ipdChangeDirectory(dir);
   }
   vdir = ipdvosOs2Vospec(dir, ipdTRUE); /* convert to system independent */
   ipdVeReturn (vdir);
}


/** @brief Checks if the given vospec is a directory in the file system. 
    @param vospec File specification to check
    @return Returns ipdTRUE on success
*/
ipdBoolean ipdvosIsDirectory(ipdvosFile vospec)
{
   ipdvDirCode isdir;
   ipdConstString file;
   ipdVeStart("ipdvosIsDirectory");

   if ((vospec == ipdNULL) ||
       (ipdVosGetName(vospec) != ipdNULL) ||
       (ipdVosGetExt(vospec) != ipdNULL))
      ipdVeReturn(ipdFALSE);		/* has a filename or extension */

   file = ipdvosConvertFile(vospec, ipdTRUE);
   if (file == ipdNULL)
      ipdVeReturn(ipdFALSE);
   isdir = ipdvIsDirectory(file);
   ipdvFree(file);
   ipdVeReturn(isdir == ipdIS_DIRECTORY);
}

/** @brief Checks wheter the given file specification is read/write or executable by the current process.
    @param file Selected file
    @param mode Selected mode
    @return ipdTRUE if the given file specification is
   read/write or executable by the current process. 
*/
ipdBoolean ipdvosAccessFile(
	ipdvosFile file,
	ipdLongWord mode)
{
   ipdConstString fstr;
   ipdBoolean retval;
   ipdVeStart("ipdvosAccessFile");

   fstr = ipdvosConvertFile(file, ipdTRUE);
   if (fstr == ipdNULL)
      ipdVeReturn(ipdFALSE);		/* cannot convert specification */
   retval = ipdipdFileAccess(fstr, mode);
   ipdvFree(fstr);
   ipdVeReturn(retval);
}

