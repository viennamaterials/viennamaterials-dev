/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

static char const _rcsid_ [] = "$Id: vdir.c$";

/** @file vdir.c
    @brief Virtual directory handling
*/

/***S directory */

/***I */
#include "../../include/ipdtype.h"
#include "vdirP.h"
#include "vdir.h"
#include "verr.h"
#include "vosP.h"

static ipdConstString dirptr = ipdNULL;
static ipdChar dirpath[IPDMAXPATHLEN+1];
static ipdLong dirlen = 0;

#if defined(unix) || defined(__unix)
ipdConstString ipdTruncateSpec(ipdConstString spec)
{
   if ((dirptr != ipdNULL) && (ipdvStrncmp(spec, dirptr, dirlen) == 0)) {
      spec += dirlen;
      if (*spec == '\0') spec = "."; /* is current directory */
   }
   return(spec);
}
#endif /* defined(unix) || defined(__unix) */

/** @brief Returns a read only pointer to a string with the current directory. */
ipdConstString ipdGetDirectory()
{
   ipdConstString retval;

   if (dirptr != ipdNULL)
      return(dirptr);		/* was cached */
#if ipdSystemV || defined(vms) || defined(__vms) || defined(osf) || defined(__osf) || defined(linux) || defined(__linux) || defined(__OPENNT)
#  if defined(ipdNeedGetcwd) && ipdNeedGetcwd && defined(unix) && defined(__unix)
   {
      dev_t rootdev, thisdev;	/* device of directory */
      ino_t rootino, thisino;	/* inode  of directory */
      ipdString pathp;		/* start of directory string in buffer */
      struct stat st;		/* information about directory */

      dirpath[IPDMAXPATHLEN] = '\0';
      pathp = &dirpath[IPDMAXPATHLEN]; /* point to last element */

      if (stat(".", &st) < 0)
	 return(ipdNULL);		/* unable to get information about current
				   directory */
      thisdev = st.st_dev;
      thisino = st.st_ino;	/* current directory information */

      if (stat("/", &st) < 0)
	 return(ipdNULL);		/* unable to get information about root
				   directory */
      rootdev = st.st_dev;
      rootino = st.st_ino;	/* root directory information */

      while (!((thisdev == rootdev) && (thisino == rootino))) {
	 /** loop over all upper directories until we find the real top
	     directory */
	 Directory dirstream;	/* opened directory */
	 DirStruct *dent;	/* entry of directory */
	 ipdConstString entry = ipdNULL;
	 dev_t dotdev;
	 ino_t dotino;		/* actual device and inode */
	 ipdBoolean mountpt;	/* directory is mount point */

	 if (chdir("..") < 0) {
	    if (pathp != &dirpath[IPDMAXPATHLEN-1]) {
	       /* try to get back to the original directory.
		  this is the only place where this is possible.  */
	       int save = errno;	/* save old error code */
	       chdir(pathp);
	       errno = save;
	    }
	    return(ipdNULL);
	 }

	 if (stat(".", &st) < 0)
	    return (ipdNULL);	/* no information about this directory */

	 /* Figure out if this directory is a mount point.  */
	 dotdev = st.st_dev;
	 dotino = st.st_ino;
	 mountpt = (dotdev != thisdev);

	 /* Search for the last directory.  */
	 dirstream = OpenDirectory(".", ipdFALSE);
	 if (dirstream == ipdNULL)
	    return(ipdNULL);

	 while ((dent = readdir(&(dirstream->dir))) != ipdNULL) {
	    /** have to use readdir here, since more information is needed */
	    entry = dent->d_name;
	    if ((entry[0] == '.') &&
		((entry[1] == 0) ||
		 ((entry[2] == 0) && (entry[1] == '.'))))
	       continue;	/*  not current and up directory */

	    if (mountpt || (dent->d_ino == thisino)) {
	       /** for a match of inode, check directory entry, for mount
		   point this must be done in any case */
	       ipdInt retcode;
#if (defined(S_IFLNK) || defined(S_ISLNK)) && !defined(__OPENNT)
	       retcode = lstat(entry, &st);
#else
	       retcode = stat(entry, &st);
#endif /* (defined(S_IFLNK) || defined(S_ISLNK)) && !defined(__OPENNT) */
	       if ((retcode == 0) &&
		   (st.st_dev == thisdev) && (st.st_ino == thisino))
		  break;	/* we found it, leave the loop */
	    }
	 }

	 if (entry == ipdNULL) {	/* this is end of directory error */
	    int save = errno;
	    CloseDirectory(dirstream);
	    errno = save;
	    return(ipdNULL);	/* no backup possible */
	 } else {
	    int len = ipdvStrlen(entry); /* new line */
	    pathp -= len;
	    /* copy path element to front of buffer */
	    ipdvMemcpy(pathp, entry, len);
	    *--pathp = '/';
	    CloseDirectory(dirstream);
	 }
	 thisdev = dotdev;
	 thisino = dotino;	/* new actual values */
      }

      if (pathp == &path[IPDMAXPATHLEN])
	 *(--pathp) = '/';	/* was top directory */
      else if (chdir(pathp) < 0)
	 return(ipdNULL);		/* cannot go back (is error) */

      retval = pathp;		/* we have done it.... */
   }
#  else
   retval = (ipdConstString)getcwd(dirpath, IPDMAXPATHLEN+1);
#  endif /* defined(ipdNeedGetcwd) && ipdNeedGetcwd */
#else
#  if defined(WINNT)
   retval = (GetCurrentDirectory(IPDMAXPATHLEN+1, dirpath) != 0) ? dirpath : ipdNULL;
#  else
   retval = (ipdConstString)getwd(dirpath);
#  endif /* defindef(WINNT) */
#endif /* defined(ipdSystemV) || defined(vms) || defined(__vms) || defined(osf) || defined(__osf) || defined(linux) || defined(__linux) || defined(__OPENNT)*/
   if (retval == ipdNULL)
      return(ipdNULL);            /* failure */
   
   dirlen = ipdvStrlen(retval);
   if (dirpath != retval)
      ipdvMemcpy(dirpath, retval, dirlen);
#if defined(unix) || defined(__unix)
   dirpath[dirlen++] = '/';	/* add trailing '/' */
   dirpath[dirlen] = '\0';
#endif /* defined(unix) || defined(__unix) */
   retval = dirptr = dirpath;	/* save for multiple calls */
   return(retval);
}


/** @brief Returns ipdTRUE, if the given file filename can be accessed by the given mode. 
    @param filename File to check
    @param mode Access mode
    @return The boolean result of the check
*/
ipdBoolean ipdipdFileAccess(ipdConstString filename, ipdLongWord mode)
{
#if defined(unix) || defined(__unix)
   ipdBoolean ok;
   int md;
   ipdChar* unused_ret_val_char_pnt; /* [patch] vor capturing unused return values */   
   md = ((mode & ipdProtectionReadBit) ? R_OK : 0) |
     ((mode & ipdProtectionWriteBit) ? W_OK : 0) |
     ((mode & ipdProtectionExecuteBit) ? X_OK : 0);
   /* check the file access */
   ok = access(filename, md) == 0;
   if (ok && (mode & ipdProtectionDeleteBit)) {
      /* for delete access we need write privelidge to the directory */
      ipdChar buffer[IPDMAXPATHLEN+1];
      ipdChar *slash;
      unused_ret_val_char_pnt = ipdvStrcpy(buffer, filename); /* [patch] removed warning: unused retval */
      slash = ipdvStrrchr(buffer, '/');
      if (slash != NULL) {
	 if (slash == buffer)
	    unused_ret_val_char_pnt = ipdvStrcpy(buffer, "/"); /* top directory */ /* [patch] removed warning: unused retval */
	 else
	    *slash = '\0';	/* trunc filename */
      } else
	 unused_ret_val_char_pnt = ipdvStrcpy(buffer, ".");	/* current directory */ /* [patch] removed warning: unused retval */
      ok = access(buffer, W_OK) == 0;
   }
   return(ok);
#endif /* defined(unix) || defined(__unix) */


#if defined(WINNT)
   PACL pDacl;
   PSECURITY_DESCRIPTOR desc;
   DWORD attr;

   /*fprintf(stdout,"ipdipdFileAccess name:%s mode:%x\n",filename,mode);   [SteHo] */

   /* [MK] new implementaiton which also works for FAT */
   /* without GetNamedSecurityInfo */
   if (mode == 0 )
   {
     /* printf ("Request for file %s Mode0 _write_: ",filename); */
     if (access(filename, W_OK) == 0)
     {
      /* printf ("vTrue at 0\n"); */
      return(ipdTRUE);    /* has write permissions */
     }
     else
     {
      /* printf ("vFalse at 0\n"); */
      return(ipdFALSE);  /* no write permissions */
     }
   }
   else if (mode == 1 )
   {
     /* printf ("Request for file %s Mode1 _read_: ",filename); */
     if (access(filename, R_OK) == 0)
     {
      /* printf ("vTrue at 0\n"); */
      return(ipdTRUE);    /* has read permissions */
     }
     else
     {
      /* printf ("vFalse at 0\n"); */
      return(ipdFALSE);  /* no read permissions */
     }
   }
   printf ("Request for file %s Mode(%x) _UNKOWN_ : ",filename,mode);
   printf ("Use old implementation which might cause FAT bug\n");
   fflush(stdout);
   
   attr = GetFileAttributes((LPTSTR)filename);
   if ((attr == 0xffffffff) || ((mode & ipdProtectionWriteBit) && (attr & FILE_ATTRIBUTE_READONLY)))
      return(ipdFALSE);
   if (mode == 0)
      return(ipdTRUE);            /* file exists */

   desc = NULL;
   if (GetNamedSecurityInfo((LPSTR)filename, SE_FILE_OBJECT, DACL_SECURITY_ipdINFORMATION,
                            NULL, NULL, &pDacl, NULL, &desc) != ERROR_SUCCESS) {
      if (desc != NULL)
         LocalFree(desc);
      return(ipdFALSE);
   } else {                     /* we now have the security information */
      ULONG     i;
      ACL_SIZE_ipdINFORMATION size;
      ACCESS_MASK mask, allow;

      mask = ((mode & ipdProtectionReadBit) ? ACTRL_READMASK : 0) |
         ((mode & ipdProtectionWriteBit) ? ACTRL_WRITEMASK : 0) |
         ((mode & ipdProtectionExecuteBit) ? ACTRL_EXECUTEMASK : 0) |
         ((mode & ipdProtectionDeleteBit) ? ACTRL_DELETEMASK : 0);
      GetAclInformation(pDacl, (LPVOID)&size, sizeof(ACL_SIZE_ipdINFORMATION), AclSizeInformation);
      allow = 0;
      for (i = 0; i < size.AceCount; i++) {
         ACCESS_ALLOWED_ACE *ace;
         
         GetAce(pDacl, i, (LPVOID)&ace);
         if ((ace->Header.AceType == ACCESS_ALLOWED_ACE_TYPE) ||
             (ace->Header.AceType == ACCESS_DENIED_ACE_TYPE)) {
            ipdBoolean match =
               EqualPrefixSid((PSID)&ace->SidStart, winntUserSID) || /* User */
               EqualSid((PSID)&ace->SidStart, winntAuthentificatedSID) || /* World */
               EqualSid((PSID)&ace->SidStart, winntEveryoneSID); /* World */
            if (!match) {
               DWORD idx;       /* check groups */
               for (idx = 0; (idx < winntGroups.numGroups) && !match; idx++)
                  match = EqualSid((PSID)&ace->SidStart, winntGroups.sid[idx]);
            }
            if (match) {
               /* we found a matching ACL entry, update allowed bits */
               if (ace->Header.AceType == ACCESS_ALLOWED_ACE_TYPE) {
                  allow |= ace->Mask; /* allow access */
                  if ((allow & mask) == mask) {
                     LocalFree(desc);
                     return(ipdTRUE);
                  }
               } else if (ace->Mask & mask) { /* deny access */
                  LocalFree(desc);
                  return(ipdFALSE);
               }
            }
         }
      }
      /* free allocated memory */
      LocalFree(desc);
      return(ipdFALSE);
   }
#endif /* defined(WINNT) */
  
}

/** @brief Clear the current working directory cached pointer */
ipdVoid ipdClearDirectoryCache()
{
   dirptr = ipdNULL;
}

/** @brief Changes the default directory to the given directory path which has to exist.
    @param path A directory path
    @return Returns ipdTRUE on sucess. 
*/
ipdBoolean ipdChangeDirectory(
	ipdConstString path)	/* [I] new default directory */
{
#if defined(vms) || defined(__vms)
   if (chdir((char *)path) == 0) {
      ipdConstString ptr = ipdvStrchr(path, ':'); /* with device */
      if ((ptr != ipdNULL) && (ptr[1] == '[') && (ptr[2] != '.')) {
	 dirlen = ipdvStrlen(path);
	 ipdvMemcpy(dirpath, path, dirlen+1); /* cache new directory */
      } else
	 dirptr = ipdNULL;
      return(ipdTRUE);
   }
#endif /* defined(vms) || defined(__vms) */
#if defined(unix) || defined(__unix)
   if (chdir((char *)path) == 0) {
      if (*path == '/') {
	 dirlen = ipdvStrlen(path);
	 ipdvMemcpy(dirpath, path, dirlen+1); /* cache new directory */
	 if (dirpath[dirlen-1] != '/') {
	    dirpath[dirlen++] = '/'; /* add trailing '/' */
	    dirpath[dirlen] = '\0';
	 }
	 dirptr = dirpath;
      } else
	 dirptr = ipdNULL;	/* clear cache */
      return(ipdTRUE);
   }
#endif /* defined(unix) || defined(__unix) */
#if defined(WINNT)
   if (SetCurrentDirectory(path) != 0) {
      dirptr = ipdNULL;
      return(ipdTRUE);
   }
#endif /* defined(WINNT) */
   return(ipdFALSE);
}

/** @brief ipdvStatFile sets the file time of a file filename to the values times
    @param filename File to get information from
    @param buf File information
    @return Returns ipdTRUE on success.
*/
ipdBoolean ipdvStatFile(ipdConstString filename, ipdStatStruct *buf)
{

#if defined(unix) || defined(__unix)
   return((ipdBoolean)(stat((ipdString)ipdTruncateSpec(filename), buf) == 0));
#endif /* defined(unix) || defined(__unix) */

#if defined(WINNT_REAL) /* MK   [SteHo] : Should only be called in unix-based OSs */
   return(ipdTRUE);
#endif /* defined(WINNT) */

#if defined(WINNT)


   DWORD prot;
   ipdLongWord uid, gid;
   HANDLE find;
   WIN32_FIND_DATA findbuf;
         
   ipdvMemset(buf, '\0', sizeof(ipdStatStruct)); /* clear memory first */
   if (!vGetFileProtection(filename, (ipdLongWord *)(&prot), &uid, &gid))
      return(ipdFALSE);
   find = FindFirstFile(filename, &findbuf);
   if (find == INVALID_HANDLE_VALUE)
      return(ipdFALSE);
   FindClose(find);
   
   StatFileOwner(*buf) = uid;
   StatFileGroup(*buf) = gid;
   StatFileSize(*buf) = findbuf.nFileSizeLow;
   /* [SteHo] */
   /*StatFileAccessTime(*buf) = convertWindowsTimeToUnix(&findbuf.ftLastAccessTime); */
   /*StatFileModifyTime(*buf) = convertWindowsTimeToUnix(&findbuf.ftLastWriteTime); */
   /*StatFileStatusTime(*buf) = convertWindowsTimeToUnix(&findbuf.ftCreationTime); */
   switch(ipdvIsDirectory(filename)) {
   case ipdIS_FILE:      buf->st_mode = _S_IFREG; break;
   case ipdIS_DIRECTORY: buf->st_mode = _S_IFDIR; break;
   case ipdIS_SPECIAL:   buf->st_mode = _S_IFCHR; break;
   case ipdIS_IPDNONEXIST:  buf->st_mode = 0; break;
   }
   return(ipdTRUE);
#endif /* defined(WINNT) */
   
   
}

/** @brief Checks if the given file specification points to a directory 
    @param path Directory path to check
    @return A return code indicating the type of the directory path
*/
ipdvDirCode ipdvIsDirectory(ipdConstString path)
{
   ipdvDirCode code;
#if defined(unix) || defined(__unix)
   struct stat statinfo;
   if (!ipdvStatFile(path, &statinfo))
      return(ipdIS_IPDNONEXIST);
   if (S_ISDIR(ipdStatFileType(statinfo)))
       code = ipdIS_DIRECTORY;
   else if (S_ISCHR(ipdStatFileType(statinfo)) ||
	    S_ISBLK(ipdStatFileType(statinfo)))
      code = ipdIS_SPECIAL;
   else
      code = ipdIS_FILE;
#endif /* defined(unix) || defined(__unix) */
#if defined(WINNT)
   DWORD attr = GetFileAttributes(path);
   if (attr == 0xffffffff)
     code = ipdIS_IPDNONEXIST;
   else if (attr & FILE_ATTRIBUTE_DIRECTORY)
      code = ipdIS_DIRECTORY;
   else if (attr & FILE_ATTRIBUTE_SYSTEM)
      code = ipdIS_SPECIAL;
   else
      code = ipdIS_FILE;
#endif /* defined(WINNT) */
   return(code);
}


