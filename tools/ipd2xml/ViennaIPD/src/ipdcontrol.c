/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

CONTENT:  File IO, Initializing, Iterators

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  RK   Robert Klima,      Institute for Microelectronics, TU Vienna
          BR   Bernhard Reinisch, Institute for Microelectronics, TU Vienna

  who when        what
-------------------------------------------------------------------------
  BR  1997        first coding
  RK  1998-04-14  new from scratch
=======================================================================*/

/** @file ipdcontrol.c
    @brief Function definitions for File-IO, initialization and iterators
*/

/*=======================================================================
	Includes
=======================================================================*/

#include "ipdP.h"
#include <stdio.h>
#include <string.h>

/*#include "vbs.h"*/
#include "../core/include/verr.h" /* [patch] switched from vbs.h to verr.h, vbs.h isn't necessary */
#include "../core/include/vosP.h"

/*=======================================================================
	Global variables
=======================================================================*/


ipdPrivate_t ipdPrivate =
{
   NULL,
   0,
   ipdNULL,
   0,
   NULL,
   0,
   0,
   NULL,
   ipdFALSE
#if IPD_ENABLE_INDIVIDUAL_EXTENSIONS
   ,NULL
#endif /** IPD_ENABLE_INDIVIDUAL_EXTENSIONS */
};

/* Library Base */
ipdBase_t                *ipdBase = NULL;

/** [JW] workaround for normal mem managment. we have to 
preserve memory pointers to be able to free them on the end ..*/
#if !IPD_USE_IPD_MALLOC 
ipdTreeNode_t           **global_array_backup = NULL;
ipdTreeNode_t           **global_matrix_backup = NULL;
#endif

/** Hash used for finding iterator nodes. */
static ipdHash_t          iteratorNameHash;

/*=======================================================================
	Functions Declarations
=======================================================================*/

#if defined(__cplusplus)
extern "C" {
#endif

  int           yyparse (void *);

#if defined(__cplusplus)
}
#endif

/*=======================================================================
  C code
=======================================================================*/

/** @brief Set the scanned file  */
void iiSetScannedFile(ipdIFL_t *ifn)
{
   ipdPrivate.scannedFile = ifn;
} /* iiSetScannedFile */

/** @brief Get the scanned file  */
ipdIFL_t *iiGetScannedFile(void)
{
   return ipdPrivate.scannedFile;
} /* iiGetScannedFile */

/** @brief Set the number of successfully parsed files */
void iiSetSuccessfullyParsedFiles(ipdLong i)
{
   ipdPrivate.successfullyParsedFiles = i;
} /* iiSetSuccessfullyParsedFiles */

/** @brief Get the number of successfully parsed files */
int iiGetSuccessfullyParsedFiles(void)
{
   return ipdPrivate.successfullyParsedFiles;
} /* iiGetSuccessfullyParsedFiles */

/** @brief Free the search path
    @param lokal If ipdTRUE, then the local search path is free'd
 */
static void iiFreeSearchPath(ipdBoolean lokal)
{
   ipdLong      i;
   ipdLong     *pcnt;
   ipdvosFile **plist;

   /* Find the pathlist */
   if (lokal)
   {
      plist = &ipdBase->lokalSearchPathList;
      pcnt  = &ipdBase->lokalSearchPathCount;
   }
   else
   {
      plist = &ipdPrivate.searchPathList;
      pcnt  = &ipdPrivate.searchPathCount;
   }

   if ((*plist))
   {
      for (i = 0; i < *pcnt; i++)
	 if (((*plist)[i]))
	    ipdvosFreeVospec((*plist)[i]);
      ipdMBfree(*plist);
      *plist  = NULL;
   }
   *pcnt = 0;
} /* iiFreeSearchPath */

/** @brief Sets internal search path. 
 *
 * Sets internal search path. The search path is given as a path list.
 * That's a string which consists of one ore more paths seperated by a ':'
 * or a ';'. Now iiSetSearchPaths creates an array of vos paths.
 * The ipd default path is stored in IPD_DEFAULT_SEARCH_PATH.
 */
static void iiSetSearchPaths(ipdConstString path, ipdBoolean lokal)
{
   ipdvosFile     **plist, *p, cur;
   ipdLong         num = 0;
   ipdLong        *pcnt;
   /* Unix-delimiter is used as default for internal search-path-handling */
   ipdConstString ipdSearchPathDelimiter = ":;";
   ipdLong        nPos;

   /* Find the pathlist */
   if (lokal)
   {
      plist = &ipdBase->lokalSearchPathList;
      pcnt  = &ipdBase->lokalSearchPathCount;
   }
   else
   {
      plist = &ipdPrivate.searchPathList;
      pcnt  = &ipdPrivate.searchPathCount;
   }

   /* Find number of paths in the search paths. */
   if (*IPD_DEFAULT_SEARCH_PATH)
   {
      num    += (ipdLong)iiStrocc(IPD_DEFAULT_SEARCH_PATH, ipdSearchPathDelimiter);
      num++;
   }

   if ((path) && (*path))
   {
      num += (ipdLong)iiStrocc(path, ipdSearchPathDelimiter);
      num++;
   }
   else
      path = NULL;

   *plist = NULL;
   *pcnt  = 0;

   if (num > 0)
   {
      if (!(*plist = p = (ipdvosFile *)ipdMBalloc(num * sizeof(ipdvosFile))))
      {
	 ipdSetError(ipdERR_IPDNO_MEMORY);
	 return;
      }
   /* [JW] if we are using the IPD memory management, 
   the memory has to be set to zero by ipdMBclear.
   On the contrary, using the common malloc approach
   ipdMBalloc already sets the memory zero. 
   hence, the following is not needed .. */   
   #if IPD_USE_IPD_MALLOC      
      ipdMBclear(p);
   #endif
      /* Find first token in the search paths */
      /* until there are no more tokens       */
      /* strtok(NULL,delim): Finds next token from previous string. */

      /* Add default path */
      nPos = 0;
      while ((cur = ipdvosGetPathFromPathList(IPD_DEFAULT_SEARCH_PATH,
					   &nPos)) != ipdNULL)
	 *p++ = cur;

      /* Add user path */
      if ((path))
      {
	 nPos = 0;
	 while ((cur = ipdvosGetPathFromPathList(path,
					      &nPos)) != ipdNULL)
	    *p++ = cur;
      }
      *pcnt = (ipdLong)(p - *plist);
   }
} /* iiSetSearchPaths */

/** @brief Initialize the search path iterator 
    @param it The search path iterator to be initialized
*/
static void iiItSearchPathInit(ipdSearchPathIterator_t *it)
{
   if (it)
   {
      it->index   = 0;
      it->listnum = 0;
      /* the pathlists */
      it->plist[0] = ipdPrivate.searchPathList;
      it->pnum [0] = ipdPrivate.searchPathCount;
      it->plist[1] = (ipdBase) ? ipdBase->lokalSearchPathList  : NULL;
      it->pnum [1] = (ipdBase) ? ipdBase->lokalSearchPathCount : 0;
   }
} /* iiItSearchPathInit */

/** @brief Dereference the search path iterator
    @param it The search path iterator to be dereferenced 
*/
static ipdvosFile iiItSearchPathDereference(ipdSearchPathIterator_t *it)
{
   if (it && it->index !=-1 &&
       it->plist[it->listnum])
      return it->plist[it->listnum][it->index];
   return ipdNULL;
} /* iiItSearchPathDereference */

/** @brief Increment the search path iterator
    @param it The search path iterator to be incremented
*/
static void iiItSearchPathNext(ipdSearchPathIterator_t *it)
{
   while (it->listnum < 2)
   {
      if (++it->index < it->pnum[it->listnum])
	 return;
      else
      {
	 it->index = -1;
	 it->listnum++;
      }
   }
   return;
} /* iiItSearchPathNext */

#if IPD_ENABLE_INDIVIDUAL_EXTENSIONS

/** @brief Administration of file extensions [SW] */
static void iiSetExtList(ipdConstString extList)
{
   char      *ext     = NULL;
   char      *list    = ipdvStrdup(extList);
   ipdExt_t **currExt = &ipdPrivate.extension;

   do
   {
      ext  = strtok(ipdPrivate.extension == NULL ? list : NULL, " ");
      if (ext != 0)
      {
	 *currExt = (ipdExt_t *)ipdvMalloc(sizeof(ipdExt_t));
	 if (*currExt == NULL)
	 {
	    ipdSetError(ipdERR_IPDNO_MEMORY);
	    break;
	 }

	 (*currExt)->next      = NULL;
	 (*currExt)->extension = ipdvStrdup(ext);
	 if ((*currExt)->extension == NULL)
	 {
	    ipdSetError(ipdERR_IPDNO_MEMORY);
	    break;
	 }

	 currExt = &((*currExt)->next);
      }
   } while (ext != 0);

   if (list != NULL)
      ipdvFree(list);
}

/** @brief Free the list of file extensions */
static void iiFreeExtList(void)
{
   ipdExt_t *currExt = ipdPrivate.extension;
   ipdExt_t  *tmpExt = NULL;

   while (currExt != NULL)
   {
      tmpExt = currExt->next;
      if (currExt->extension != NULL)
	 ipdvFree(currExt->extension);
      ipdvFree(currExt);
      currExt = tmpExt;
   }
   ipdPrivate.extension = NULL;
}
#endif /* IPD_ENABLE_INDIVIDUAL_EXTENSIONS */


/* Dumps the search paths to stdout. */
void ipdDumpSearchPath(void)
{
   ipdConstString            name;
   ipdSearchPathIterator_t it;
   ipdvosFile                 file = ipdNULL;

   iiItSearchPathInit(&it);

   printf("\n//Input Deck search path(s):\n");
   while ((file = iiItSearchPathDereference(&it)))
   {
      name = ipdvosConvertFile((file), ipdFALSE);
      printf("//   \"%s\"\n", name);
      ipdvFree(name);
      iiItSearchPathNext(&it);
   }
   printf("\n");
} /* ipdDumpSearchPath */

/** @brief Checks, if the given file has been already read.

 * If so, it won't be
 * stored twice in the acive file list. If not, the ipdvosFile pointer will
 * be stored in the file list.
 * This function sets the global currentComment pointer to the comment
 * string of the found file list entry.
 */
static ipdIFL_t *iiSetNewCurrentInFile(ipdvosFile file)
{
   ipdIFL_t *fileList;
   ipdChar    *name;

   name = (ipdChar *)ipdvosConvertFile(file, ipdFALSE);

   /* Find the already existing file in the list if there is one. */
   fileList = ipdBase->files;
   while ((fileList))
   {
      if (!strcmp(name, fileList->fileName))
      {
	 currentComment = NULL;
	 ipdvosFreeVospec(file);
	 ipdvFree(name);
	 return fileList;
      }
      fileList = fileList->next;
   }

   if ((fileList = (ipdIFL_t *)ipdMBalloc(sizeof(ipdIFL_t))))
   {
      fileList->next     = ipdBase->files;
      ipdBase->files     = fileList;
      fileList->file     = file;
      fileList->fileName = name;
      fileList->comment  = NULL;
      currentComment     = &fileList->comment;
   }
   else
   {
      ipdSetError(ipdERR_IPDNO_MEMORY);
      return NULL;
   }
   return fileList;
} /* iiSetNewCurrentInFile */

/** @brief Finds a file with the given vos-structure. 
 *
 * If it does not exist, this
 * routine tests, whether a file with the extension ".ipd" exists
 * and returns it.
 * @return ipdFALSE is returned when nothing could be found.
 */
#if 0 /* That's the way of mmnt: */
static ipdVoid mmfCreateVosName (ipdvosFile path, mmfPifId_t *pifId)
{
   ipdvosFile fname, pname;

   if (pifId->physical != NULL)
   {
      fname = ipdvosOs2Vospec (pifId->physical, ipdFALSE);
      pname = vosPrepName  (fname, "pbf", ipdFALSE);

      pifId->vosPhysical = vosMergeVospecs (path, pname);

      ipdvosFreeVospec (pname);
      ipdvosFreeVospec (fname);
   }
   else pifId->vosPhysical = NULL;
}
#endif
/** @brief Get the ipd file given by the name supplied as argument */
static ipdBoolean iiGetIpdFile(ipdvosFile *file)
{
   ipdvosFile      tmpFile;

#if IPD_ENABLE_INDIVIDUAL_EXTENSIONS
   ipdExt_t *ext  = ipdPrivate.extension;
   ipdBoolean found = ipdFALSE;
#endif /* IPD_ENABLE_INDIVIDUAL_EXTENSIONS */

   if (ipdvosAccessFile(*file, ipdUserReadProtection))
      return ipdTRUE;

#if IPD_ENABLE_INDIVIDUAL_EXTENSIONS
   /* This function is called if and only if the file was not found before. So
      it does not make sense to continue if the list is empty.
   */
   if (ext == NULL) 
      /* tmpFile = ipdvosUpdateVospec(*file, ipdNULL, ipdNULL, ipdNULL, ipdNULL, ipdNULL, 0); */
      return ipdFALSE;

   if ((ipdVosGetExt(*file)))
   {
      /* There is already an extension, so it should be replaced if and only if
	 it is part of the extensions stored in the list. */
      while (ext != NULL)
      {
	 if ((ext->extension != NULL) && (strcmp(ipdVosGetExt(*file), ext->extension) == 0))
	 {
	    found = ipdTRUE;
	    break;
	 }
	 ext = ext->next;
      }
      if (!found)
	 return ipdFALSE;
   }

   /* Try all extensions stored in the list. */
   ext = ipdPrivate.extension;
   do
   {
      tmpFile = ipdvosUpdateVospec(*file, ipdNULL, ipdNULL, ipdNULL, ipdNULL, ext->extension, ipdvosUPDATE_EXT);
      ext = ext->next;
   } while ((ext != NULL) && !tmpFile);
#else
   if ((ipdVosGetExt(*file)))
      return ipdFALSE;

   tmpFile = ipdvosUpdateVospec(*file,
			     ipdNULL,
			     ipdNULL,
			     ipdNULL,
			     ipdNULL,
			     IPD_FILE_EXT,	/* file extension */
			     ipdvosUPDATE_EXT);
#endif /* IPD_ENABLE_INDIVIDUAL_EXTENSIONS */

   if (!(tmpFile))
   {
      ipdSetError(ipdERR_IPDNO_MEMORY);
      return ipdFALSE;
   }
   if (ipdvosAccessFile(tmpFile, ipdUserReadProtection))
   {
      ipdvosFreeVospec(*file);
      *file = tmpFile;
      return ipdTRUE;
   }
   ipdvosFreeVospec(tmpFile);
   return ipdFALSE;
} /* iiGetIpdFile */

/** @brief Opens a file with the name filename using the internal search path list.
 *
 * Opens a file with the name 'filename' using the internal search path list.
 * If a file is found the fullname (path + filename) and a file pointer
 * will be returned.
 */
FILE *iiFileOpen(ipdConstString   filename,
		 ipdIFL_t     **newFile,
		 ipdvosFile        currentDir,
		 ipdBoolean       useSearchPaths,
		 ipdConstString   mode)
{
   FILE         *f;
   ipdConstString  name;
   ipdIFL_t     *listFile;

   if (!(filename))
      return NULL;
   if ((newFile))
      *newFile = NULL;

   if ((ipdPrivate.searchPathList))
   {
      ipdvosFile tmpFile;
      ipdvosFile locFile;

      if (!(tmpFile = ipdvosOs2Vospec(filename, ipdFALSE)))
      {
	 ipdSetError(ipdERR_IPDNO_MEMORY);
	 return NULL;
      }

      /* Test, if it is a top path filename. If so, get it. */
      if (ipdvosIsTopPath(tmpFile))
      {
	 if (iiGetIpdFile(&tmpFile))
	 {
	    if (ipdvosIsDirectory(tmpFile))
	    {
	       ipdvosFreeVospec(tmpFile);
	       ipdSetError(ipdERR_CANT_OPEN_FILE, filename);
	       return NULL;
	    }

	    name = ipdvosConvertFile(tmpFile, ipdFALSE);
	    f    = fopen(name, mode);
	    ipdvFree(name);

	    if ((f))
	    {
	       /* Search for ipdPrivate.scannedFile in the filelist. */
	       listFile = iiSetNewCurrentInFile(tmpFile);
	       if ((newFile))
		  *newFile = listFile;
	       return f;
	    }
	    ipdvosFreeVospec(tmpFile);
	    ipdSetError(ipdERR_CANT_OPEN_FILE, filename);
	    return NULL;
	 }
	 else
	 {
	    ipdvosFreeVospec(tmpFile);
	    ipdSetError(ipdERR_CANT_OPEN_FILE, filename);
	    return NULL;
	 }
      }
      /* Test, if the file exists in the local directory. */
      if (currentDir == ipdNULL)
      {
	 if (!(locFile = ipdvosCopyVospec(tmpFile)))
	 {
	    ipdvosFreeVospec(tmpFile);
	    ipdSetError(ipdERR_IPDNO_MEMORY);
	    return NULL;
	 }
      }
      else
      {
#if 0 /* Obsolet. 7.12.98 */
	 ipdvosFile tmpDir = currentDir;

	 if (!ipdvosIsDirectory(currentDir))
	 {
	    ipdConstString tmpDirName;

	    tmpDirName = ipdvosVos2Ospec(currentDir, vosOUTPUT_DIR);
	    tmpDir     = ipdvosOs2Vospec(tmpDirName, ipdTRUE);
	    ipdvFree(tmpDirName);
	 }
#endif
	 locFile = ipdvosSpecat(currentDir, tmpFile); /* make absolute path */

#if 0 /* Obsolet. 7.12.98 */
	 if (tmpDir != currentDir)
	    ipdvosFreeVospec(tmpDir);
#endif

	 if (!(locFile))
	 {
	    ipdvosFreeVospec(tmpFile);
	    ipdSetError(ipdERR_IPDNO_MEMORY);
	    return NULL;
	 }
      }

      /* Get file by its relative path. */
      if (iiGetIpdFile(&locFile))
      {
	 ipdvosFreeVospec(tmpFile);

	 if (ipdvosIsDirectory(locFile))
	 {
	    ipdvosFreeVospec(locFile);
	    ipdSetError(ipdERR_CANT_OPEN_FILE, filename);
	    return NULL;
	 }

	 name = ipdvosConvertFile(locFile, ipdFALSE);
	 f    = fopen(name, mode);
	 ipdvFree(name);

	 if ((f))
	 {
	    /* Search for ipdPrivate.scannedFile in the filelist. */
	    listFile = iiSetNewCurrentInFile(locFile);
	    if ((newFile))
	       *newFile = listFile;
	    return f;
	 }
	 ipdvosFreeVospec(locFile);
	 ipdSetError(ipdERR_CANT_OPEN_FILE, filename);
	 return NULL;
      }
      else
	 ipdvosFreeVospec(locFile);

      /* Find file within search paths. */
      if (useSearchPaths)
      {
	 ipdSearchPathIterator_t it;
	 ipdvosFile                 searchpath;

	 iiItSearchPathInit(&it);

	 while ((searchpath = iiItSearchPathDereference(&it)))
	 {
	    locFile = ipdvosSpecat(searchpath, tmpFile); /* make absolute path */

	    if (iiGetIpdFile(&locFile))
	    {
	       ipdvosFreeVospec(tmpFile);

	       if (ipdvosIsDirectory(locFile))
	       {
		  ipdvosFreeVospec(locFile);
		  ipdSetError(ipdERR_CANT_OPEN_FILE, filename);
		  return NULL;
	       }

	       name = ipdvosConvertFile(locFile, ipdFALSE);
	       f    = fopen(name, mode);
	       ipdvFree(name);

	       if ((f))
	       {
		  /* Search for ipdPrivate.scannedFile in the filelist. */
		  listFile = iiSetNewCurrentInFile(locFile);
		  if ((newFile))
		     *newFile = listFile;
		  return f;
	       }
	       ipdvosFreeVospec(locFile);
	       ipdSetError(ipdERR_CANT_OPEN_FILE, filename);
	       return NULL;
	    }
	    ipdvosFreeVospec(locFile);

	    iiItSearchPathNext(&it);
	 }
      }

      ipdvosFreeVospec(tmpFile);
      ipdSetError(ipdERR_CANT_OPEN_FILE, filename);
      return NULL;
   }
   else
   {
      if ((f = fopen(filename, mode)))
      {
	 /* Search for ipdPrivate.scannedFile in the filelist. */
	 listFile = iiSetNewCurrentInFile(ipdvosOs2Vospec(filename, ipdFALSE));
	 if ((newFile))
	    *newFile = listFile;
	 return f;
      }
      ipdSetError(ipdERR_CANT_OPEN_FILE, filename);
      return NULL;
   }
} /* iiFileOpen */

/** @brief Sets the current section. CSFullName must be a valid name! */
static ipdConstString iiSetCSFullnameValid(ipdConstString CSFullName)
{
   ipdChar *fullname;

   if (!(fullname = iiStrDup(CSFullName)))
      return NULL;
   ipdMBfree(ipdBase->CSFullName);
   ipdBase->CSFullName = fullname;

   return fullname;
} /* iiSetCSFullnameValid */

/* --------------------------------------------------------------------------
 * Read an input deck into the parser's memory and parse it. This file must
 * be written using the Input Deck Programming Language IPL.
 * To be called for more than one time if there are more files to be parsed.
 * Better: use include-directive
 */
ipdLong ipdReadInputDeck(ipdConstString filename)
{
   /* static implementieren, falls nochmals aufgerufen: einfach dazueinlesen,
      ohne ipd-Neuinitialisierung. Der User kann ja, wenn er will, mit
      ipdMBfree() alles bisher gelesene loeschen.
  */
   extern FILE *yyin /* , *yyout */ ;

   ipdVeStart("ipdReadInputDeck");

   iiSetScannedFile(ipdNULL);

   if (!(yyin = iiFileOpen(filename,
			   &ipdPrivate.scannedFile,
			   ipdNULL,
			   ipdTRUE,
			   "r")))
      ipdVeReturn(0);


   ipdResetErrorStatus();

   /****************/
   /*  Call bison  */
   /****************/
   /* Free the CSFullname. It would influence the behaviour of parsing. */
   ipdMBfree(ipdBase->CSFullName);

   ipdBase->CSFullName = NULL;

   /* Set current section. */
   iiSetCS(iiGetRoot());

   /* Reset before parsing. */
   ipdPrivate.parsing     = ipdTRUE;
   ipdParameterList       = NULL;
   /*
     ipdPrivate.scannedFile is set in iiFileOpen().
   */

   /* Reset the parser. */
   tokenpos           = 0;
   ipdLineNo          = 1;
   currentComment     = NULL;
   ipdIncludeStackPtr = 0;
   commentNewLine     = 0;

  /* Start parsing. */
   yyparse(NULL);

  /* Reset after parsing. */
   ipdPrivate.parsing     = ipdFALSE;
   ipdPrivate.scannedFile = NULL;
   ipdBase->calcVar       = NULL;

   /* Set current section. */
   iiSetCS(iiGetRoot());

   iiSetCSFullnameValid("~");

   ipdBase->calcVar       = NULL;

#if 0
   if (ipdGetErrorNumber() == ipdERR_PARSE)
   {
      fprintf (stderr, "Error while parsing inputdeck '%s'\n", filename);
      fprintf (stderr, "Maybe a closing \'}\' or final \';\' missing\n");
   }
   else
#endif
   if (ipdGetErrorStatus())
      ipdVeReturn(0) /* bad */;

   ipdVeReturn((ipdLong)iiGetSuccessfullyParsedFiles());
} /* ipdReadInputDeck */

/** @brief Initializes the input deck log file. 
 *
 * The name of the file is first
 * searched for in an environment variable defined by IPD_LOG_ENV. When
 * not successfull, a default logfile name is opened defined by
 * IPD_LOG_FILE.
 */
void iiInitLogFile(FILE *logFile, ipdBoolean force)
{
   ipdChar *logFileName;

   if ((ipdPrivate.logFile))
      fclose(ipdPrivate.logFile);
   if ((logFile))
      ipdPrivate.logFile = logFile;
   else
   {
      if (force)
      {
	 if (!(logFileName = getenv(IPD_LOG_ENV)))
	    logFileName = (ipdChar *)IPD_LOG_FILE;
	 if (!(ipdPrivate.logFile = fopen(logFileName, (ipdChar *)"w"))) /* erase old file-content */
	 {
	    ipdSetError (ipdERR_LOGFILE, logFileName);
	 }
      }
      else
	 ipdPrivate.logFile = NULL;
   }
} /* iiInitLogFile */

/*-------------------------------------------------------------------------
 * Frees the current input deck.
 */
void ipdFreeCurrentIpd(void)
{
   ipdIFL_t *fileList, *tmpFile;

   ipdVeStart("ipdFreeCurrentIpd");

   /* Reset all errors. */
   ipdResetError();

#if IPD_DUMP_WHOLE_IPD_B4_EXIT
   ipdDumpAll2File(stdout, ipdFALSE);
#endif /* IPD_DUMP_WHOLE_IPD_B4_EXIT */

   if ((ipdBase))
   {
      ipdBase_t *baseEntry, *prevBaseEntry = NULL;

   /* Free ipdBase in base list structure. */
      baseEntry = ipdPrivate.database;
      while ((baseEntry))
      {
	       if (baseEntry == ipdBase)
	          break;
	       prevBaseEntry = baseEntry;
	       baseEntry     = baseEntry->next;
      }

      if ((prevBaseEntry))
	       prevBaseEntry->next = ipdBase->next;
      else
	       ipdPrivate.database = ipdBase->next;

      /* Free the name of the currently calculated var if there is one. */
      ipdMBfree(ipdBase->currVarFullname);

      /* Free all variables and sections. */
      iiFreeTreeNodeRec(ipdBase->root_p, ipdTRUE);

      /* Free all intern functions. */
      iiFreeAllInternFunctions();

      /* Free all iterators. */
      iiFreeAllIterators();

      /* Clean the hash used by iterators if not already done. */
      iiCleanHash(&iteratorNameHash);

      /* Free the CSStack. */
      while ((ipdBase->CSStack))
         ipdPopCS();

      /* Free the CSFullName, used by stepping funcions of the iterator. */
      ipdMBfree(ipdBase->CSFullName);
      ipdBase->CSFullName = NULL;

      /* Free the hash storing all variables and sections. */
      iiFreeHash(&ipdBase->hashTable);

      /* Free all ipdvosFile structures. */
      fileList = ipdBase->files;
      while ((fileList))
      {
	    tmpFile  = fileList;
	    fileList = fileList->next;
	    ipdvosFreeVospec(tmpFile->file);
	    ipdvFree(tmpFile->fileName);
	    ipdMBfree(tmpFile->comment);
	    ipdMBfree(tmpFile);
      }

      /* Free the name of the inputdeck. */
      ipdMBfree(ipdBase->ipdName);

      /* Free error stack */
      iiResetErrorStack();

      /* Free the base structure. */
      ipdMBfree(ipdBase);
      ipdBase = ipdPrivate.database;
   }

   ipdVeReturnVoid;
} /* ipdFreeCurrentIpd */

/*-------------------------------------------------------------------------
 * Frees all input deck structures. Use this before exit.
 *
 * [JW] I've replaced the ipdFreeAll() function with the 
 * ipdFreeAllInDetail() as imho there should only exist the
 * ipdFreeAllInDetail(). It may be slower, but it frees all structures.
 * Further note, that I've deleted the old ipdFreeAll() function and
 * renamned: ipdFreeAllInDetail() --> ipdFreeAll()
 */
 
void ipdFreeAll(void)
{
   ipdVeStart("ipdFreeAll");

   while ((ipdPrivate.database))
      ipdFreeCurrentIpd();

   /* Free the hash used by iterators if not freed already. */
   iiFreeHash(&iteratorNameHash);

   /* Free all c-coded functions defined by the user. */
   iiFreeAllCCodedFunctions();

   /* Free the global unit hash. */
   iiFreeUnitHash();

   /* Delete all search paths. */
   iiFreeSearchPath(ipdFALSE);

#if IPD_ENABLE_INDIVIDUAL_EXTENSIONS
   iiFreeExtList();
#endif /* IPD_ENABLE_INDIVIDUAL_EXTENSIONS */

   /* Free the global garabe hashes. */
   iiFreeMBHash();

#if IPD_MEMORY_ALLOC_ipdDEBUG_INFO
   iiTestMalloc();
#endif /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */

   /* Close the log-file. */
   if ((ipdPrivate.logFile))
   {
      fclose(ipdPrivate.logFile);
      ipdPrivate.logFile = NULL;
   }

   /* [JW] 2. experienced seg faults when calling the free functions several 
   times within one program instance. therefore deactivated them again ..*/
	/* [JW] 1. the following free functions have been created to deal
	with unfreed strings which were allocated by some string functions. */
	//ipdFreeVosUser();
	//ipdFreeVosConv();
	//ipdFreeBuffer();

#if !IPD_USE_IPD_MALLOC
   free(global_matrix_backup);
   free(global_array_backup);
#endif  

   ipdVeReturnVoid;
} /* ipdFreeAllInDetail */



/*-------------------------------------------------------------------------
 * Create New Inputdeck
 * Init the InputDeck-internal dataBase
 */

#if IPD_ENABLE_INDIVIDUAL_EXTENSIONS
ipdBoolean ipdInitExt(ipdConstString pathList, FILE *logFile, ipdConstString extList)
{
   ipdBoolean retval = ipdInit(pathList, logFile);
   if (retval && (extList != NULL))
   {
      iiFreeExtList();
      iiSetExtList(extList);
   }
   return retval;
}
#endif /* IPD_ENABLE_INDIVIDUAL_EXTENSIONS */

ipdBoolean ipdInit(ipdConstString pathList, FILE *logFile)
{
   ipdPrivate.inheritanceInfo = ipdTRUE; /* [SW] */

   /* Allocate memblock for memory management. */
   iiSetMBHash();
   if (ipdGetErrorStatus())
      goto iiInitError;

#if IPD_USE_UNITS
   /* Inititalize the unit hash table. */
   iiInitUnitHash();
   if (ipdGetErrorStatus())
      goto iiInitError;
#endif /* IPD_USE_UNITS */

   /* reset the filecounter */
   iiSetSuccessfullyParsedFiles(0);

   iiInitLogFile(logFile, ipdFALSE);
   if (ipdGetErrorStatus())
      goto iiInitError;

  /* Get Iterator name hash.
     (Used for finding an item in a virtual section. */
   if (!(iiInitHash(&iteratorNameHash, IPD_ITERATOR_NAME_HASH_SIZE))) 
      goto iiInitError;

   iiFreeSearchPath(ipdFALSE);
   iiSetSearchPaths(pathList, ipdFALSE);

   return ipdTRUE;

 iiInitError:
   if (ipdPrivate.logFile)
      fclose(ipdPrivate.logFile);
   ipdPrivate.logFile = NULL;
#if IPD_USE_UNITS
   iiFreeUnitHash();
#endif /* IPD_USE_UNITS */
   return ipdFALSE;
} /* ipdInit */

/*-------------------------------------------------------------------------
 * Creates a new inputdeck.
 */
ipdLong ipdCreateBase(ipdConstString name,    /* Name of the inputdeck */
		    ipdLong        hashSize /* Hash size             */
		    )
{
   ipdTreeNode_t *rootTn  = NULL;
   ipdBase_t     *oldBase = ipdBase;
   ipdBase_t     *tmpBaseEntry;
   ipdLong          i = 0;

   ipdVeStart("ipdCreateBase");

   if (!(name) || !(iiMBlockHashLower) || !(iiMBlockHashHigher))
      ipdVeReturn(0);

   /* Get base structure. */
   if (!(ipdBase = (ipdBase_t *)ipdMBalloc(sizeof(ipdBase_t))))
   {
      ipdBase = oldBase;
      ipdSetError(ipdERR_IPDNO_MEMORY);
      ipdVeReturn(0);
   }
   /* [JW] if we are using the IPD memory management, 
   the memory has to be set to zero by ipdMBclear.
   On the contrary, using the common malloc approach
   ipdMBalloc already sets the memory zero. 
   hence, the following is not needed .. */      
#if IPD_USE_IPD_MALLOC    
   ipdMBclear(ipdBase);
#endif
   /* Reset all errors. */
   ipdResetError();

   /* Copy the inputdecks name. */
   ipdBase->ipdName = iiStrDup(name);
   if (ipdGetErrorStatus())
      goto ipdCreateBaseError;

   /* Init the InputDeck-internal dataBase */
   rootTn = iiCreateTreeNode(ipdSECTION);
   if (ipdGetErrorStatus())
      goto ipdCreateBaseError;
   rootTn->node.sv.fullname       = iiStrDup("~");
   rootTn->node.sv.name           = rootTn->node.sv.fullname + 1;
   rootTn->node.sv.svu.sect.flags = ipdSF_NOFLAG;
   if (ipdGetErrorStatus())
      goto ipdCreateBaseError;

   /* Initialize the input decks hash for variable and section names. */
   if (!iiInitHash(&ipdBase->hashTable, (long)hashSize))
      ipdVeReturn(0); 

   if (ipdGetErrorStatus())
      goto ipdCreateBaseError;

   iiHashStoreTreeNode(rootTn);
   if (ipdGetErrorStatus())
      goto ipdCreateBaseError;

   ipdBase->root_p = rootTn;
   rootTn = NULL; /* Because of ipdCreateBaseError:
		     Leave this in case of later changes.
		     To avoid freeing twice. */

   ipdBase->CS_p       = ipdBase->root_p; /* startUp: points to root */

   /* Now link it to the database-list */
   tmpBaseEntry        = ipdPrivate.database;
   while ((tmpBaseEntry)) /* Find out new index. */
   {
      i = ipdMAX(tmpBaseEntry->index, i);
      tmpBaseEntry = tmpBaseEntry->next;
   }
   ipdBase->index = i + 1;
   ipdBase->next  = ipdPrivate.database;
   ipdPrivate.database = ipdBase;

   ipdVeReturn(ipdBase->index);

 ipdCreateBaseError:
   if ((rootTn))
      iiFreeTreeNodeRec(rootTn, ipdTRUE);
   if ((ipdBase))
   {
      iiFreeHash(&ipdBase->hashTable);
      if ((ipdBase->ipdName))
	 ipdMBfree(ipdBase->ipdName);
      if ((ipdBase->root_p))
	 iiFreeTreeNodeRec(ipdBase->root_p, ipdTRUE);
      if ((ipdBase))
	 ipdMBfree(ipdBase);
   }
   ipdBase = oldBase;
   ipdVeReturn(0);
} /* ipdCreateBase */

/*-------------------------------------------------------------------------
 * Sets the current IPD-Database
 */
ipdBoolean ipdSetBase(ipdLong index)
{
   ipdBase_t *baseEntry;

   ipdVeStart("ipdSetBase");

   baseEntry = ipdPrivate.database;
   while ((baseEntry))
   {
      if (index == baseEntry->index)
      {
	 ipdBase = baseEntry;
	 ipdResetErrorStatus();
	 ipdVeReturn(ipdTRUE);
      }
      baseEntry = baseEntry->next;
   }
   ipdSetError(ipdERR_IPDNO_IPD_DATABASE_FOUND, (ipdLong)index);

   ipdVeReturn(ipdFALSE);
} /* ipdSetBase */

/*=======================================================================
	Find items (variables, sections), set a current section.
=======================================================================*/

/*-------------------------------------------------------------------------
 * Sets the current section by its name.
 */
ipdConstString ipdSetCSByName(ipdConstString CSFullName)
{
   ipdChar *fullname = (ipdChar *)CSFullName;

   if (fullname)
   {
#if IPD_ipdDEBUG
      printf("ipdSetCSByName old = %s (%s)\n", ipdBase->CSFullName, CSFullName);  /*this was a bug: */
#endif

      ipdResetErrorStatus();
      if (!(iiFindItemByName(&fullname, ipdSECTION, ipdFALSE)))
      {
	 ipdSetError(ipdERR_SECT_IPDNOT_FOUND, CSFullName);
	 return NULL;
      }

      ipdMBfree(ipdBase->CSFullName);
      ipdBase->CSFullName = fullname;

#if IPD_ipdDEBUG
      printf("ipdSetCSByName new : %s\n", ipdBase->CSFullName);
#endif
   }

   return fullname;
} /* ipdSetCSByName */

/*-------------------------------------------------------------------------
 * Gets the name of the current section. We can't store a current section
 * pointer because the current section may be virtual - not really
 * existing.
 */
ipdConstString ipdGetCSName(void)
{
   ipdResetErrorStatus();
   if ((ipdBase->CSFullName))
      return ipdBase->CSFullName;
   else
      return "~";
} /* ipdGetCSName */

/*-------------------------------------------------------------------------
 * Pushes the current section (its fullname) to a stack.
 */
ipdConstString ipdPushCS(void)
{
   ipdChar    *newName;
   ipdSLL_t *sll;

   ipdResetErrorStatus();
   if ((ipdBase->CSFullName))
   {
      if (!(newName = iiStrDup(ipdBase->CSFullName)))
	 return NULL;
   }
   else
      newName = NULL;
   if (!(sll = (ipdSLL_t *)ipdMBalloc(sizeof(ipdSLL_t))))
   {
      ipdMBfree(newName);
      return NULL;
   }
   sll->node        = newName;
   sll->next        = ipdBase->CSStack;
   ipdBase->CSStack = sll;

   return (ipdChar *)sll->node;
} /* ipdPushCS */

/*-------------------------------------------------------------------------
 * Pops the current section (its fullname) from a stack.
 */
ipdConstString ipdPopCS(void)
{
   ipdSLL_t *sll;

   if ((sll = ipdBase->CSStack))
   {
      ipdMBfree(ipdBase->CSFullName);
      ipdResetErrorStatus();
      ipdBase->CSFullName = (ipdChar *)sll->node;
      ipdBase->CSStack    = sll->next;
      ipdMBfree(sll);
   }
   else
   {
      ipdSetError(ipdERR_POP_IPDNO_SECTION);
      return NULL;
   }

   return ipdBase->CSFullName;
} /* ipdPopCS */

/** @brief Finds an ipd entry (variable or section) by its name.

    @return If successful, a corrected, valid fullname will be returned in itemName, which is ipd-malloced (must be freed after use!). The found item is returned by a pointer to ipdTreeNode_t. Returns 0 on error.
 */
ipdTreeNode_t *iiFindItemByName(ipdChar      **itemName,
				ipdType_t    itemType,
				ipdBoolean     doFindAuxiliaries)
{
   ipdChar         *searchForName;
   ipdTreeNode_t *tn;

   if ((itemName) && (*itemName))
   {
      /* Sets the type to a valid value. */
      if (!(itemType & (ipdSECTION | ipdVARIABLE)))
      {
	 ipdSetError(ipdERR_SEARCH_ITEM);
	 return ipdFALSE;
      }
      /* Find the item. */
      if (!(searchForName = iiCreateName(*itemName, NULL)))
	 return NULL;
      if (!(tn = iiHashFindTreeNode(searchForName)))
      {
	 iiGetInheritedItemRec(&tn,
			       searchForName,
			       NULL,
			       ipdFALSE,
			       NULL);
	 if (!(tn) || ipdGetErrorStatus())
	 {
	    ipdMBfree(searchForName);
	    return NULL;
	 }
      }
      if (!(tn->type & itemType))
      {
	 if (itemType & ipdVARIABLE)
	 {
	    ipdSetError(ipdERR_IPDNO_VARIABLE, searchForName);
	    ipdMBfree(searchForName);
	    return NULL;
	 }
	 else
	 {
	    ipdSetError(ipdERR_IPDNO_SECTION, searchForName);
	    ipdMBfree(searchForName);
	    return NULL;
	 }
      }
      if ((tn->type & ipdVARIABLE) &&
	  (tn->node.sv.svu.var.flags & ipdVF_AUXILIARY) &&
	  !(doFindAuxiliaries))
      {
	 ipdSetError(ipdERR_QUERY_AUX_VARIABLE, searchForName);
	 ipdMBfree(searchForName);
	 return NULL;
      }
      *itemName = searchForName;
      tn->inquired = ipdTRUE; /* [SW] */
      return tn;
   }
   return NULL;
} /* iiFindItemByName */

/*-------------------------------------------------------------------------
 * Test by its name, whether an item does exit or not.
 * Returns the item by a pointer to ipdTreeNode_t or 0 on error.
 */
ipdTreeNode_t *ipdExistItemByName(ipdConstString name,
				  ipdType_t    it,
				  ipdBoolean     doFindAuxiliaries)
{
   ipdChar *itemName = (ipdChar *)name;

   if ((itemName))
   {
      ipdTreeNode_t *tn;

      ipdResetErrorStatus();
      if ((tn = iiFindItemByName(&itemName, it, doFindAuxiliaries)))
      {
	 ipdMBfree(itemName);
	 return tn;
      }
   }
   return NULL;
} /* ipdExistItemByName */

/*=======================================================================
	Iterators, Interface
=======================================================================*/

/*=======================================================================
	Iterator Manipulations
=======================================================================*/

/** @brief Creates an iterator. iterator type = type !!! */
ipdIterator_t *iiCreateIterator(ipdType_t type)
{
   ipdIterator_t   *iNode;

   if (!(iNode = (ipdIterator_t *)ipdMBalloc(sizeof(ipdIterator_t))))
      return NULL;

   /* [JW] if we are using the IPD memory management, 
   the memory has to be set to zero by ipdMBclear.
   On the contrary, using the common malloc approach
   ipdMBalloc already sets the memory zero. 
   hence, the following is not needed .. */   
#if IPD_USE_IPD_MALLOC
   ipdMBclear(iNode);
#endif
   if (!(ipdIteratorSetType(iNode, type)))
   {
      ipdIteratorFree(iNode);
      return NULL;
   }
   iNode->next                = ipdBase->activeIteratorSll;
   ipdBase->activeIteratorSll = iNode;
   return iNode;
} /* iiCreateIterator */

/*-------------------------------------------------------------------------
 * Cleans an iterator for next use.
 */
void ipdIteratorClean(ipdIterator_t *iNode)
{
   /* Don't free iNode->sectName!!!
      That's why ipdIteratorClean() is used to free only the hash
      and the FIFO. */
   /* Clear the current item pointer. */
   iNode->tn     = NULL;
   /* Reset the init flag. */
   iNode->isInit = ipdFALSE;
   /* Free item name. */
   ipdMBfree(iNode->itemName);
   iNode->itemName = NULL;
   /* Now free the FIFO. */
   if (iNode->tnDll)
      while ((iiPopTNFifo(&(iNode->tnDll))));
   /* Reset the position pointer of the FIFO */
   iNode->tnDllPos = NULL;
} /* ipdIteratorClean */

/*-------------------------------------------------------------------------
 * Iterator still valid?
 */
ipdBoolean ipdIteratorIsValid(ipdIterator_t *iNode)
{
   if (iNode && iNode->tn)
      return ipdTRUE;
   else
      return ipdFALSE;
} /* ipdIteratorIsValid */

/*-------------------------------------------------------------------------
 * Frees an iterator node. Use this function for freeing!
 */
void ipdIteratorFree(ipdIterator_t *iNode)
{
   if ((iNode))
   {
      ipdIterator_t *this_i, *prev_i = NULL;

#if IPD_ipdDEBUG
      printf("skipping ipdIteratorFree\n");
      return;
#endif

      ipdIteratorClean(iNode);
      ipdMBfree(iNode->sectName);
      this_i = ipdBase->activeIteratorSll;
      while ((this_i))
      {
	 if ((this_i == iNode))
	    break;
	 prev_i = this_i;
	 this_i = this_i->next;
      }
      if ((this_i))
      {
	 if ((prev_i))
	    prev_i->next               = this_i->next;
	 else
	    ipdBase->activeIteratorSll = this_i->next;
      }
      ipdMBfree(iNode);
   }
} /* ipdIteratorFree */

/** @brief Frees all iterators. Use this function for freeing internally!  */
void iiFreeAllIterators(void)
{
   while ((ipdBase->activeIteratorSll))
      ipdIteratorFree(ipdBase->activeIteratorSll);
} /* iiFreeAllIterators */

/*-------------------------------------------------------------------------
 * Returns the name of the current section.
 */
ipdConstString ipdIteratorGetSectionName(ipdIterator_t *iNode)
{
   if ((iNode) && (iNode->sectName))
      return iNode->sectName;
   return NULL;
} /* ipdIteratorGetSectionName */

/*-------------------------------------------------------------------------
 * Returns the name of the item stored in the itherator.
 */
ipdConstString ipdIteratorGetItemFullName(ipdIterator_t *iNode)
{
   if ((iNode) && (iNode->sectName) && (iNode->tn))
   {
      ipdLong  sectNameLen;
      ipdChar *sTo;

      ipdResetErrorStatus();
      ipdMBfree(iNode->itemName);
      iNode->itemName = NULL;
      sectNameLen = strlen(iNode->sectName);
      if (sectNameLen && (iNode->itemName = (ipdChar *)ipdMBalloc((sectNameLen + strlen(iNode->tn->node.sv.name) + 2))))
      {
	 strcpy(iNode->itemName, iNode->sectName);
	 sTo = iNode->itemName + sectNameLen;
	 if (*(sTo - 1) != '~')
	    *sTo++ = '.';
	 strcpy(sTo, iNode->tn->node.sv.name);
	 return iNode->itemName;
      }
      else
	 ipdSetError(ipdERR_IPDNO_MEMORY);
   }
   return NULL;
} /* ipdIteratorGetItemFullName */

/*-------------------------------------------------------------------------
 */
ipdConstString ipdIteratorGetItemName(ipdIterator_t *iNode)
{
   if ((iNode) && (iNode->tn) &&
       (iNode->tn->type & (ipdSECTION | ipdVARIABLE)))
      return (iNode->tn->node.sv.name);
   return NULL;
} /* ipdIteratorGetItemName */

/** @brief  Returns the name of the item stored in the section tree.
 * 
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!! Just for internal use !!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * E.g.:
 * A { x=1; }
 * B : A;
 * Iterator inside section B
 * ipdIteratorGetStoredItemFullName gives '~A.x' for item 'x' in section 'B'
 */
ipdConstString ipdIteratorGetStoredItemFullName(ipdIterator_t *iNode)
{
   if ((iNode) && (iNode->tn) &&
       (iNode->tn->type & (ipdSECTION | ipdVARIABLE)))
      return (iNode->tn->node.sv.fullname);
   return NULL;
} /* ipdIteratorGetStoredItemFullName */

/*-------------------------------------------------------------------------
 * Set iterator type.
 */
ipdBoolean ipdIteratorSetType(ipdIterator_t *iNode, ipdType_t iteratorType)
{
   if ((iNode))
   {
      if (!(iteratorType & (ipdVARIABLE | ipdSECTION)))
      {
	 ipdSetError(ipdERR_SEARCH_ITEM);
	 return ipdFALSE;
      }
      iNode->type = iteratorType & (ipdVARIABLE | ipdSECTION);
      return ipdTRUE;
   }
   return ipdFALSE;
} /* ipdIteratorSetType */

/*-------------------------------------------------------------------------
 * Get the iterator type.
 */
ipdType_t ipdIteratorGetType(ipdIterator_t *iNode)
{
   if ((iNode) && (iNode->tn))
      return iNode->tn->type;
   else
      return ipdINVALID;
} /* ipdIteratorGetType */

/** @brief Finds a variable or a section by a given name.
 *
 * If the item does not
 * exist, iiIteratorFindByName() tests if an inherited item exits. A valid
 * iterator will be returned. The type of the item can be specified by
 * itemType. ipdANY finds an item of any type.
 */
static ipdBoolean iiIteratorFindByName(ipdIterator_t *iNode,
				     ipdConstString   name,
				     ipdType_t      itemType)
{
   ipdChar *itemName = (ipdChar *)name;

   if ((iNode) && (itemName))
   {
      if (!(iNode->tn = iiFindItemByName(&itemName, itemType, ipdFALSE)))
      {
	 ipdSetError(ipdERR_IPDNOT_FOUND, name);
	 return ipdFALSE;
      }
      iiRemoveNameFromPath(itemName);

      ipdMBfree(iNode->sectName);
      iNode->sectName = itemName;
      if (!iiSetCSFullnameValid(itemName))
	 return ipdFALSE;

      return ipdTRUE;
   }
   return ipdFALSE;
} /* iiIteratorFindByName */

/*-------------------------------------------------------------------------
 * Overwrites a given iterator.
 * Finds an item by its name. itemName is the relative or absolute
 * pathname of the item. A valid iterator will be returned. The type of the
 * item can be specified by searchType. ipdANY finds an item of any type.
 * iterator type = setType!!!
 */
ipdBoolean ipdIteratorSetByName(ipdIterator_t *iNode,
			      ipdConstString   itemName,
			      ipdType_t      searchType,
			      ipdType_t      setType)
{
   if ((iNode))
   {
      ipdResetErrorStatus();
      ipdIteratorClean(iNode);
      if (!iiIteratorFindByName(iNode, itemName, searchType))
      {
	 ipdSetError(ipdERR_IPDNOT_FOUND, itemName);
	 return ipdFALSE;
      }
      if (!(ipdIteratorSetType(iNode, setType)))
      {
	 ipdIteratorFree(iNode);
	 return ipdFALSE;
      }

      return ipdTRUE;
   }

   return ipdFALSE;
} /* ipdIteratorSetByName */

/*-------------------------------------------------------------------------
 * Creates a new iterator.
 * Finds an item by its name. itemName is the relative or absolute
 * pathname of the item. A valid iterator will be returned. The type of the
 * item can be specified by searchType. ipdALL finds an item of any type.
 * iterator type = setType!!!
 */
ipdBoolean ipdIteratorNewByName(ipdIterator_t **iNode_p, 
			      ipdConstString    itemName, 
			      ipdType_t       searchType,
			      ipdType_t       setType)
{
   if ((iNode_p))
   {
      ipdResetErrorStatus();
      *iNode_p = iiCreateIterator(setType);
      ipd_RETURNonERRORb;

      if (!(ipdIteratorSetByName(*iNode_p,
				 itemName,
				 searchType,
				 setType)))
      {
	 ipdIteratorFree(*iNode_p);
	 *iNode_p = NULL;
	 return ipdFALSE;
      }
      return ipdTRUE;
   }
   return ipdFALSE;
} /* ipdIteratorNewByName */

/*-------------------------------------------------------------------------
 * Creates a new iterator of the ipdTreeNode_t structure tn.
 * tn must be of searchType.
 * iterator type = setType!!! ipdALL finds an item of any type.
 */
ipdBoolean ipdIteratorNewByTN(ipdIterator_t **iNode_p, 
			   ipdTreeNode_t   *tn, 
			   ipdType_t        searchType,
			   ipdType_t        setType)
{
   if ((iNode_p))
   {
      ipdResetErrorStatus();
      if (!(tn->type & searchType))
	      return ipdFALSE;
      *iNode_p = iiCreateIterator(setType);
      ipd_RETURNonERRORb;

      (*iNode_p)->tn             = tn;

      if (!((*iNode_p)->sectName = iiStrDup(tn->node.sv.parent->node.sv.fullname)))
      {
          ipdIteratorFree(*iNode_p);
	       *iNode_p = NULL;
	       return ipdFALSE;
      }
      return ipdTRUE;
   }
   return ipdFALSE;
} /* ipdIteratorNewByTN */

/*-------------------------------------------------------------------------
 * Creates a new iterator.
 * Set the iterator to the root section.
 * iterator type = setType!!!
 */
ipdBoolean ipdIteratorNewAtRootSection(ipdIterator_t **iNode_p,
				     ipdType_t       setType)
{
   ipdResetErrorStatus();
   *iNode_p = iiCreateIterator(setType);
   ipd_RETURNonERRORb;
   if (!((*iNode_p)->sectName = (ipdChar *)ipdMBalloc(2)))
   {
      ipdIteratorFree(*iNode_p);
      ipdSetError(ipdERR_IPDNO_MEMORY);
      return ipdFALSE;
   }
   (*iNode_p)->sectName[0] = '~';
   (*iNode_p)->sectName[1] = '\0';
     
   if (!iiSetCSFullnameValid("~"))
      return ipdFALSE;

   (*iNode_p)->tn = iiGetRoot();

   return ipdTRUE;
} /* ipdIteratorNewAtRootSection */

/** @brief Overwrites a given iterator.
 * 
 * Initializes an iterator for iteration.
 * A DLL is built with all inherited sections of the name iNode->sectName
 * found.
 * iterator type = unchanged !!!
 */
static void iiInitIterator4Inheritance(ipdIterator_t *iNode)
{
   if ((iNode) && (iNode->sectName) && !(iNode->isInit))
   {
      ipdTreeNodeDll_t *sectDll = NULL, *tmpDll;

      /* Get a DLL containing all inherited sections with
	 the name iNode->sectName. */
      iiGetInheritedItemRec(NULL,
			    iNode->sectName,
			    &sectDll,
			    ipdTRUE,
			    NULL);
      ipd_RETURNonERRORv;

      /* First call:
	 Initializes static variables in iiHashStoreItem() in
	 iiAddAllItemsToStack(). */
      iiAddAllItemsToStack(&iNode->tnDll,
			   NULL,
			   &iteratorNameHash);
      /* Test if all items found are sections!
	 Add them to the iterator hash from behind to front.
	 (The behind is the first in the list.) */
      if ((sectDll))
      {
	 while ((sectDll->next))
	    sectDll = sectDll->next;
	 while ((sectDll))
	 {
	    if ((sectDll->tn))
	    {
	       if (!(sectDll->tn->type & ipdSECTION))
	       {
		  if (sectDll->tn->type & ipdVARIABLE)
		     ipdSetError(ipdERR_IPDNO_SECTION, sectDll->tn->node.sv.fullname);
		  else
		     ipdSetError(ipdERR_IPDNO_SECTION, "?itemname?");
		  return;
	       }
	       /* Add all items (variables and sections) of the section
		  stored in sectDll->tn to the nameHash. */
	       iiAddAllItemsToStack(NULL,
				    sectDll->tn,
				    &iteratorNameHash);
	       ipd_RETURNonERRORv;
	    }
	    tmpDll  = sectDll;
	    sectDll = sectDll->prev;
	    ipdMBfree(tmpDll);
	 }
      }

#if 0
      /* Get the locally modified section, if there exists one. */
      /* Find the local modified section and set it. */
      {
	 ipdTreeNode_t    *tn;

	 if ((tn = iiHashFindTreeNode(iNode->sectName)))
	 {
	    if (!(tn->type & ipdSECTION))
	    {
	       ipdSetError(ipdERR_IPDNO_SECTION, iNode->sectName);
	       return;
	    }
	    /* Add the local section to the iterator hash. */
	    iiAddAllItemsToStack(NULL,
				 tn,
				 &iteratorNameHash);
	    ipd_RETURNonERRORv;
	 }
      }
#endif

      if ((iNode->tn) &&
	  (iNode->tnDllPos = iiHashGetItem(&iteratorNameHash,
					   iNode->tn)))
      {
	 iNode->tn       = iNode->tnDllPos->tn;
      }
      else
      {
	 /* Both of these must be set. Otherwise set both to NULL. */
	 iNode->tn       = NULL;
	 iNode->tnDllPos = NULL;
      }

      /* Now clean the hash. */
      iiCleanHash(&iteratorNameHash);

      /* Everything initialized. */
      iNode->isInit = ipdTRUE;
   }
} /* iiInitIterator4Inheritance */

/** @brief Overwrites a given iterator.
 * 
 * Stores the next item found. But if the item is a section,
 * ipdIteratorDoNext() will NOT step into it.
 * iterator type = unchanged !!!
 */
ipdBoolean iiIteratorDoNextFlags(ipdIterator_t *iNode, ipdVarFlags_t vFlags)
{
   ipdTreeNode_t *tn;

   if ((iNode) && (iNode->sectName))
   {
      iiInitIterator4Inheritance(iNode);
      ipd_RETURNonERRORb;
      while (1)
      {
	 if ((iNode->tnDll))
	 {
	    if ((iNode->tnDllPos))
	       iNode->tnDllPos = iNode->tnDllPos->next;
	    else
	       iNode->tnDllPos = iNode->tnDll;
	    if ((iNode->tnDllPos))
	       tn = iNode->tnDllPos->tn;
	    else
	       goto iiIteratorDoNextFlagsStop;
	 }
	 else
	    goto iiIteratorDoNextFlagsStop;
	 if ((tn) &&
	     (tn->type & iNode->type) &&
	     ((tn->type & ipdSECTION) ||
	      ((tn->type & ipdVARIABLE) &&
	       !(tn->node.sv.svu.var.flags & vFlags))))
	 {
	    if (ipdGetErrorStatus())
	       goto iiIteratorDoNextFlagsStop;
	    iNode->tn = tn;
	    if (iiStrCmp(ipdBase->CSFullName, iNode->sectName))
	    {
	       if (!iiSetCSFullnameValid(iNode->sectName))
		  return ipdFALSE;
	    }
	    return ipdTRUE;
	 }
	 if (ipdGetErrorStatus())
	    goto iiIteratorDoNextFlagsStop;
      }
   }
   return ipdFALSE;

 iiIteratorDoNextFlagsStop:
   ipdIteratorClean(iNode);
   return ipdFALSE;
} /* iiIteratorDoNextFlags */

ipdBoolean ipdIteratorDoNext(ipdIterator_t *iNode)
{
   ipdResetErrorStatus();
   return iiIteratorDoNextFlags(iNode, ipdVF_AUXILIARY);
} /* ipdIteratorDoNext */

/** @brief Overwrites a given iterator.
 *
 * Stores the next item found. But if the item is a section,
 * ipdIteratorDoStep() WILL step into it.
 * iterator type = unchanged !!!
 */
ipdBoolean iiIteratorDoStepFlags(ipdIterator_t *iNode, ipdVarFlags_t vFlags)
{
   ipdChar *sectName = NULL;

   if ((iNode) && (iNode->sectName))
   {
      if ((iNode->tn) && (iNode->tn->type & ipdSECTION))
      {
	 /* Append the sectionname to the current section name. */
	 if (!(iNode->sectName) ||
	     !(sectName = iiStrDup(iNode->sectName)) ||
	     ((*(sectName + 1)) &&
	      !(sectName = iiStrAppendChar(sectName, '.'))) ||
	     !(sectName = iiStrAppend(sectName, iNode->tn->node.sv.name)))
	    goto iiIteratorDoStepFlagsStop;
	 ipdMBfree(iNode->sectName);
	 iNode->sectName = sectName;
	 if (!iiSetCSFullnameValid(sectName))
	    goto iiIteratorDoStepFlagsStop;
	 ipdIteratorClean(iNode);
      }
      return iiIteratorDoNextFlags(iNode, vFlags);
   }
   return ipdFALSE;

 iiIteratorDoStepFlagsStop:
   if ((sectName))
      ipdMBfree(sectName);
   ipdIteratorClean(iNode);
   return ipdFALSE;
} /* iiIteratorDoStepFlags */

ipdBoolean ipdIteratorDoStep(ipdIterator_t *iNode)
{
   ipdResetErrorStatus();
   return iiIteratorDoStepFlags(iNode, ipdVF_AUXILIARY);
} /* ipdIteratorDoStep */

/*-------------------------------------------------------------------------
 * Overwrites a given iterator.
 * Goes one section up. (Goes to the parent section.)
 * If the current section is the root section, do nothing.
 * iterator type = unchanged !!!
 */
ipdBoolean ipdIteratorGoUp(ipdIterator_t *iNode)
{
   if ((iNode) && (iNode->sectName))
   {
      iiRemoveNameFromPath(iNode->sectName);
      if (!iiSetCSFullnameValid(iNode->sectName))
	 return ipdFALSE;
      ipdIteratorClean(iNode);
      return ipdTRUE;
   }
   return ipdFALSE;
} /* ipdIteratorGoUp */

/*-------------------------------------------------------------------------
 * Overwrites a given iterator.
 * Goes to the root section.
 * iterator type = unchanged !!!
 */
ipdBoolean ipdIteratorGotoRootSection(ipdIterator_t *iNode)
{
   if ((iNode) && (iNode->sectName))
   {
      iNode->sectName[0] = '~';
      iNode->sectName[1] = '\0';
      ipdIteratorClean(iNode);
      if (!iiSetCSFullnameValid("~"))
	 return ipdFALSE;
      return ipdTRUE;
   }
   return ipdFALSE;
} /* ipdIteratorGotoRootSection */

/*=======================================================================
	QUERY
=======================================================================*/

/*-------------------------------------------------------------------------
 * Tests if the iterator type matches the type given.
 */
ipdBoolean ipdIteratorTestType(ipdIterator_t *iNode, ipdType_t itemType)
{
   if (itemType & ipdVARIABLE)
      itemType |= ipdVARIABLE;
   if ((iNode) && (iNode->tn) && (iNode->tn->type & itemType))
      return ipdTRUE;
   else
      return ipdFALSE;
} /* ipdIteratorTestType */

/** @brief Evaluates an expression tree (variable).
 *  @return The structure returned stores the result.
 */
ipdTreeNode_t *iiEvalTreeNode(ipdTreeNode_t *tn,
			      ipdConstString   sectName)
{
   if ((tn))
   {
      ipdChar *fullname = (ipdChar *)sectName;

      if (!(tn->type & ipdVARIABLE))
      { 
	 ipdSetError(ipdERR_CANT_EVAL_SECTION, tn->node.sv.fullname);
	 return NULL;
      }
      return ipdDoCalculation(tn, fullname);
   }
   return NULL;
} /* iiEvalTreeNode */

/*-------------------------------------------------------------------------
 * Evaluates an expression pointed to by an iterator.
 * The structure returned stores the result.
 */
ipdTreeNode_t *ipdIteratorEval(ipdIterator_t *iNode)
{
   ipdTreeNode_t *tn = NULL;

   if ((iNode) && (iNode->tn))
   {
      ipdResetErrorStatus();
      tn = iiEvalTreeNode(iNode->tn, iNode->sectName);
   }
   if ((ipdGetErrorStatus()))
      ipdStdLog("Error occured calculating variable \"%s\".\n",
		ipdIteratorGetItemFullName(iNode));

   return tn;
} /* ipdIteratorEval */

/*-------------------------------------------------------------------------
 */
ipdTreeNode_t *ipdEvalByName(ipdConstString name,
			     ipdBoolean     doFindAuxiliaries)
{
   ipdChar *varName = (ipdChar *)name;

   if ((varName))
   {
      ipdTreeNode_t *tn;

      ipdResetErrorStatus();
      /* Find variable with the name 'varname'.
	 iiFindItemByName returns a new (malloc'd) valid
	 fullname of the variable. */
      if (!(tn = iiFindItemByName(&varName, ipdVARIABLE, doFindAuxiliaries)))
	 return NULL;

      /* Remove the name of the variable of its fullname
	 to get the section name. */
      iiRemoveNameFromPath(varName);
      /* 'varName' now contains the name of the section! */
      /* Now calculate tn. */
      tn = iiEvalTreeNode(tn, varName);
      /* Free the name malloc'd in iiFindItemByName(). */
      if ((ipdGetErrorStatus()))
      { 
	 if (varName[1])
	    varName[strlen(varName)] = '.';
	 ipdStdLog("Error occured calculating variable \"%s\".\n",
		   varName);
      }
      ipdMBfree(varName);
      return tn;
   }
   return NULL;
} /* ipdEvalByName */

/*-------------------------------------------------------------------------
 */
ipdType_t ipdGetValueType(const ipdTreeNode_t *tn)
{
   if ((tn))
      return tn->type & ipdARRAYnVALUE;
   return ipdINVALID;
} /* ipdGetValueType */

/*-------------------------------------------------------------------------
 */
ipdType_t ipdGetType(ipdTreeNode_t *tn)
{
   if ((tn))
      return tn->type;
   return ipdINVALID;
} /* ipdGetType */

/*-------------------------------------------------------------------------
 * Checks if a given calculated result(!! not a variable or things like
 * that) is constant. So to check if a variable is constant, you have to
 * calculate its expression first an then call ipdCheckConstant().
 * This will not take auxiliary (to the simulator unchangeable) variables
 * into consideration.
 */
ipdBoolean ipdCheckConstant(ipdTreeNode_t *tn)
{
   if ((tn))
      return iiIsConstant(tn);

   return ipdFALSE; /* needs some more sophisticated test ... */
} /* ipdCheckConstant */

/*-------------------------------------------------------------------------
 * Checks if a variable with the given name is constant.
 */
ipdBoolean ipdCheckConstantByName(ipdConstString name)
{
   ipdChar            *varname = (ipdChar *)name;
   ipdTreeNode_t    *tn;
   ipdBoolean          retVal;
   ipdDynamicName_t *baseSect = NULL;

   ipdResetErrorStatus();
   if (!(tn = iiFindItemByName(&varname, ipdVARIABLE, ipdFALSE)))
   {
      ipdSetError(ipdERR_VAR_IPDNOT_FOUND, name);
      return ipdFALSE;
   }

   iiRemoveNameFromPath(varname);

   /* Clear calcVar. calcVar is used to find circular references. */
   ipdBase->calcVar = NULL;

   /* Create a structure, necessary for calculations in iiCalculate().
      Store the name of the current section in this structure. */
   /* Get empty node. */
   baseSect = iiCreateDNNode();
   if (ipdGetErrorStatus())
   {
      ipdMBfree(varname);
      return ipdFALSE;
   }
   /* Store fullname of the parent section.    */
   /* First copy the current section fullname. */
   iiStrExtend(&(baseSect->name), &(baseSect->len),
	       varname, ipdFALSE);
   ipdMBfree(varname);
   if (ipdGetErrorStatus())
   {
      iiFreeDNNode(baseSect);
      return ipdFALSE;
   }

   /* [patch] removed warning: cast from pointer to boolean 
              iiGetExprNode() sets tn=NULL in case its non-constant .. therefore
              we test if tn isn't null*/
   /*retVal = (ipdBoolean)(iiGetExprNode(tn, baseSect, iiGEN_CheckConstant));*/
   if(iiGetExprNode(tn, baseSect, iiGEN_CheckConstant) != NULL)
      retVal = ipdTRUE;
   else retVal = ipdFALSE;

   /* Free section name structure. */
   iiFreeDNNode(baseSect);

   ipdBase->currBaseSect = NULL;
   ipdBase->calcVar = NULL;

   return retVal;
} /* ipdCheckConstantByName */

/*-------------------------------------------------------------------------
 * Returns a iterator which can be used to analyse Inputdeck-lists (arrays ;-) ) .
 * To analyse simple lists, just call ipdValueIteratorNext()
 * The type of an element can be found by ipdValueIteratorGetType()
 * If an element is a list again, (so its type is ipdARRAYVAL) call
 * ipdValueIteratorStep(); It will step into this sub-list.
 * Stepping back or stepping up is not possible.
 * ipdValueIteratorIsValid() returns whether the iterator is still valid or not.
 * ipdValueIteratorSubscript() jumps to the index within the list (array).
 * !!!
 * !!! Please evaluate tn before!
 * !!! if ipdIteratorValueIteratorNew() is called evaluate the iterator!
 * !!!
 */
ipdBoolean ipdValueIteratorNew(ipdValueIterator_t **iVNode_p, 
			     ipdTreeNode_t       *tn)
{
   if (iVNode_p && tn && (*iVNode_p = ipdMBalloc(sizeof(ipdValueIterator_t))))
   {
      if (tn->type & ipdVARIABLE)
	 tn = tn->node.sv.svu.var.tree;
      else if (!(tn->type & ipdARRAYnVALUE))
	 return ipdFALSE;
      (*iVNode_p)->tn    = tn;
      (*iVNode_p)->index = 0;
      return ipdTRUE;
   }
   return ipdFALSE;
} /* ipdValueIteratorNew */

ipdBoolean ipdIteratorValueIteratorNew(ipdValueIterator_t **iVNode_p, 
				     ipdIterator_t       *iNode)
{
   if (iVNode_p && iNode && iNode->tn)
      return ipdValueIteratorNew(iVNode_p, iNode->tn);
   return ipdFALSE;
} /* ipdIteratorValueIteratorNew */

ipdBoolean ipdValueIteratorNext(ipdValueIterator_t *iVNode)
{
   if (iVNode && iVNode->tn && (iVNode->tn->type & ipdARRAYnVALUE))
   {
      if (iVNode->tn->type & ipdARRAYVAL)
      {
	 iVNode->index++;
	 if (iVNode->index >= iVNode->tn->node.expr.val.arrayval.len)
	 {
	    iVNode->tn = NULL;
	    return ipdFALSE;
	 }
	 return ipdTRUE;
      }
      iVNode->tn = NULL;
   }
   return ipdFALSE;
} /* ipdValueIteratorNext */

ipdBoolean ipdValueIteratorStep(ipdValueIterator_t *iVNode)
{
   if (iVNode && iVNode->tn && (iVNode->tn->type & ipdARRAYnVALUE))
   {
      if (iVNode->tn->type & ipdARRAYVAL)
      {
	 ipdTreeNode_t *tn = iVNode->tn->node.expr.val.arrayval.tree[iVNode->index];
	 if (tn->type & ipdARRAYVAL)
	 {
	    iVNode->tn    = tn;
	    iVNode->index = 0;
	    return ipdTRUE;
	 }
	 return ipdValueIteratorNext(iVNode);
      }
      iVNode->tn = NULL;
   }
   return ipdFALSE;
} /* ipdValueIteratorStep */

ipdBoolean ipdValueIteratorIsValid(ipdValueIterator_t *iVNode)
{
   if (iVNode && iVNode->tn)
   {
      if ((iVNode->tn->type & ipdARRAYnVALUE) &&
	  (iVNode->index >= iVNode->tn->node.expr.val.arrayval.len))
	 return ipdFALSE;
      return ipdTRUE;
   }
   return ipdFALSE;
} /* ipdValueIteratorIsValid */ 

ipdBoolean ipdValueIteratorSubscript(ipdValueIterator_t *iVNode,
				   ipdLong               index)
{
   if (iVNode && iVNode->tn && (iVNode->tn->type & ipdARRAYVAL))
   {
      if (index >= iVNode->tn->node.expr.val.arrayval.len)
      {
	 iVNode->tn = NULL;
	 return ipdFALSE;
      }
      iVNode->index = index;
      return ipdTRUE;
   }
   return ipdFALSE;
} /* ipdValueIteratorSubscript */

ipdType_t ipdValueIteratorGetType(ipdValueIterator_t *iVNode)
{
   if (iVNode && iVNode->tn)
   {
      if (iVNode->tn->type & ipdARRAYnVALUE)
      {
	 if (iVNode->index >= iVNode->tn->node.expr.val.arrayval.len)
	    return ipdINVALID;
	 else
	    return iVNode->tn->node.expr.val.arrayval.tree[iVNode->index]->type;
      }
      else
	 return iVNode->tn->type & ipdARRAYnVALUE;
   }
   return ipdINVALID;
}

/*-------------------------------------------------------------------------
 * Returns the items value (must be a integer).
 */
ipdBoolean ipdGetInteger(ipdTreeNode_t *tn,
		       ipdLong         *integer_p)
{
   if ((tn))
   {
      if ((tn->type & ipdARRAYnVALUE) == ipdINTEGER)
      {
	 if ((integer_p))
	    *integer_p = tn->node.expr.val.integer;
	 return ipdTRUE;
      }
      ipdSetError(ipdERR_WRONG_TYPE,
		  ipdGetTypeStr(tn->type & ipdARRAYnVALUE, ipdTRUE),
		  ipdGetTypeStr(ipdINTEGER, ipdFALSE));
   }
   return ipdFALSE;
} /* ipdGetInteger */
ipdBoolean ipdIteratorGetInteger(ipdIterator_t *iNode,
			       ipdLong         *integer_p)
{
   if ((iNode && iNode->tn))
      return ipdGetInteger(iNode->tn, integer_p);
   return ipdFALSE;
} /* ipdIteratorGetInteger */
ipdBoolean ipdValueIteratorGetInteger(ipdValueIterator_t *iVNode,
				    ipdLong              *integer_p)
{
   if (iVNode && iVNode->tn)
   {
      ipdTreeNode_t *tn;
      if (iVNode->tn->type & ipdARRAYVAL)
	 tn = iVNode->tn->node.expr.val.arrayval.tree[iVNode->index];
      else
	 tn = iVNode->tn;
      return ipdGetInteger(tn, integer_p);
   }
   return ipdFALSE;
} /* ipdValueIteratorGetInteger */
ipdBoolean ipdGetIntegerByName(ipdConstString  name,
			     ipdLong        *integer_p)
{
   ipdTreeNode_t *tn;

   if ((tn = ipdEvalByName(name, ipdFALSE)))
      return ipdGetInteger(tn, integer_p);

   return ipdFALSE;
} /* ipdGetIntegerByName */

/*-------------------------------------------------------------------------
 * Returns the items value (must be a real).
 */
ipdBoolean ipdGetReal(const ipdTreeNode_t *tn,
		    ipdDouble             *re_p)
{
   if ((tn))
   {
      if ((tn->type & ipdARRAYnVALUE) == ipdREAL)
      {
	 if ((re_p))
	    *re_p = tn->node.expr.val.quantity.re;
	 return ipdTRUE;
      }
      ipdSetError(ipdERR_WRONG_TYPE,
		  ipdGetTypeStr(tn->type & ipdARRAYnVALUE, ipdTRUE),
		  ipdGetTypeStr(ipdREAL, ipdFALSE));
   }
   return ipdFALSE;
} /* ipdGetReal */
ipdBoolean ipdIteratorGetReal(const ipdIterator_t *iNode,
			    ipdDouble             *re_p)
{
   if ((iNode && iNode->tn))
      return ipdGetReal(iNode->tn, re_p);
   return ipdFALSE;
} /* ipdIteratorGetReal */
ipdBoolean ipdValueIteratorGetReal(ipdValueIterator_t *iVNode,
				 ipdDouble            *re_p)
{
   if (iVNode && iVNode->tn)
   {
      ipdTreeNode_t *tn;
      if (iVNode->tn->type & ipdARRAYVAL)
	 tn = iVNode->tn->node.expr.val.arrayval.tree[iVNode->index];
      else
	 tn = iVNode->tn;
      return ipdGetReal(tn, re_p);
   }
   return ipdFALSE;
} /* ipdValueIteratorGetReal */
ipdBoolean ipdGetRealByName(ipdConstString  name,
			  ipdDouble      *re_p)
{
   ipdTreeNode_t *tn;

   if ((tn = ipdEvalByName(name, ipdFALSE)))
      return ipdGetReal(tn, re_p);

   return ipdFALSE;
} /* ipdGetRealByName */

/*-------------------------------------------------------------------------
 * Returns the items value (must be a real or integer).
 */
ipdBoolean ipdGetSloppyReal(ipdTreeNode_t *tn,
			  ipdDouble       *re_p)
{
   if ((tn))
   {
      if ((tn->type & ipdARRAYnVALUE) == ipdINTEGER)
      {
	 if ((re_p))
	    *re_p = (ipdDouble)tn->node.expr.val.integer;
	 return ipdTRUE;
      }
      if ((tn->type & ipdARRAYnVALUE) == ipdREAL)
      {
	 if ((re_p))
	    *re_p = tn->node.expr.val.quantity.re;
	 return ipdTRUE;
      }
      ipdSetError(ipdERR_WRONG_TYPE,
		  ipdGetTypeStr(tn->type & ipdARRAYnVALUE, ipdTRUE),
		  ipdGetTypeStr(ipdREAL, ipdFALSE));
   }
   return ipdFALSE;
} /* ipdGetSloppyReal */
ipdBoolean ipdIteratorGetSloppyReal(const ipdIterator_t *iNode,
				  ipdDouble             *re_p)
{
   if ((iNode && iNode->tn))
      return ipdGetSloppyReal(iNode->tn, re_p);
   return ipdFALSE;
} /* ipdIteratorGetSloppyReal */
ipdBoolean ipdValueIteratorGetSloppyReal(ipdValueIterator_t *iVNode,
				       ipdDouble            *re_p)
{
   if (iVNode && iVNode->tn)
   {
      ipdTreeNode_t *tn;
      if (iVNode->tn->type & ipdARRAYVAL)
	 tn = iVNode->tn->node.expr.val.arrayval.tree[iVNode->index];
      else
	 tn = iVNode->tn;
      return ipdGetSloppyReal(tn, re_p);
   }
   return ipdFALSE;
} /* ipdValueIteratorGetSloppyReal */
ipdBoolean ipdGetSloppyRealByName(ipdConstString  name,
				ipdDouble      *re_p)
{
   ipdTreeNode_t *tn;

   if ((tn = ipdEvalByName(name, ipdFALSE)))
      return ipdGetSloppyReal(tn, re_p);

   return ipdFALSE;
} /* ipdGetSloppyRealByName */

/*-------------------------------------------------------------------------
 * Returns an evaluated array of reals.
 * !!!!!!!!!!!!!!!
 * Free with free() !!!!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!
 */
ipdBoolean ipdGetSloppyRealArray(ipdTreeNode_t   *tn,         /* [I] */
			       ipdLong           *len_p,      /* [O] */
			       ipdDouble        **rearray_p)  /* [O] */
{
   if ((tn) && (len_p) && (rearray_p))
   {
      if (tn->type & ipdARRAYVAL)
      {
	 ipdLong           len       = tn->node.expr.val.arrayval.len, i;
	 ipdTreeNode_t **elements = tn->node.expr.val.arrayval.tree;

	 *len_p = len;
	 if (!(*rearray_p = malloc(sizeof(ipdDouble) * len)))
	 {
	    ipdSetError(ipdERR_IPDNO_MEMORY);
	    return ipdFALSE;
	 }
	 for (i = 0; i < len; i++)
	 {
	    switch (elements[i]->type & ipdVALUE)
	    {
	    case ipdREAL:
	       (*rearray_p)[i] = elements[i]->node.expr.val.quantity.re;
	       break;
	    case ipdINTEGER:
	       (*rearray_p)[i] = (ipdDouble)elements[i]->node.expr.val.integer;
	       break;
	    default:
	       ipdSetError(ipdERR_WRONG_TYPE,
			   ipdGetTypeStr(elements[i]->type & ipdARRAYnVALUE, ipdTRUE),
			   ipdGetTypeStr(ipdREAL, ipdFALSE));
	       ipdMBfree(*rearray_p);
	       *len_p     = 0;
	       *rearray_p = 0;
	       return ipdFALSE;
	    }
	 }
	 return ipdTRUE;
      }
      ipdSetError(ipdERR_WRONG_TYPE,
		  ipdGetTypeStr(ipdARRAYVAL, ipdTRUE),
		  ipdGetTypeStr(tn->type,    ipdFALSE));
   }
   return ipdFALSE;
} /* ipdGetSloppyRealArray */
ipdBoolean ipdIteratorGetSloppyRealArray(const ipdIterator_t  *iNode,      /* [I] */
				       ipdLong                *len_p,      /* [O] */
				       ipdDouble             **rearray_p)  /* [O] */
{
   if ((iNode && iNode->tn))
      return ipdGetSloppyRealArray(iNode->tn, len_p, rearray_p);
   return ipdFALSE;
} /* ipdIteratorGetSloppyRealArray */
ipdBoolean ipdValueIteratorGetSloppyRealArray(ipdValueIterator_t  *iVNode,     /* [I] */
					    ipdLong               *len_p,      /* [O] */
					    ipdDouble            **rearray_p)  /* [O] */
{
   if (iVNode && iVNode->tn)
   {
      ipdTreeNode_t *tn;
       if (iVNode->tn->type & ipdARRAYVAL)
	 tn = iVNode->tn->node.expr.val.arrayval.tree[iVNode->index];
      else
	 tn = iVNode->tn;
      return ipdGetSloppyRealArray(tn, len_p, rearray_p);
   }
   return ipdFALSE;
} /* ipdValueIteratorGetSloppyRealArray */
ipdBoolean ipdGetSloppyRealArrayByName(ipdConstString   name,       /* [I] */
				     ipdLong         *len_p,      /* [O] */
				     ipdDouble      **rearray_p)  /* [O] */
{
   ipdTreeNode_t *tn;

   if ((tn = ipdEvalByName(name, ipdFALSE)))
      return ipdGetSloppyRealArray(tn, len_p, rearray_p);

   return ipdFALSE;
} /* ipdGetSloppyRealArrayByName */

/** @brief Get a real valued matrix/tensor from a tree node item
 */
static ipdBoolean iiGetSloppyRealMatrix(ipdTreeNode_t  *tn,
				      ipdLong           dim,
				      ipdLong          *len,
				      ipdDouble        *matrix,
				      ipdUnit_t     **ipdUnit,
				      ipdLong           depth,
				      ipdLong          *pos_p)
{
   if (depth > dim)
   {
      ipdSetError(ipdERR_UNBALANCED_ARRAY_IPDNOT_MATRIX);
      return ipdFALSE;
   }
   if (depth < dim)
   {
      ipdLong i;

      if (!(tn->type & ipdARRAYVAL) ||
	  (tn->node.expr.val.arrayval.len != len[depth]))
      {
	 ipdSetError(ipdERR_UNBALANCED_ARRAY_IPDNOT_MATRIX);
	 return ipdFALSE;
      }
      for (i = 0; i < len[depth]; i++)
	 if (!iiGetSloppyRealMatrix(tn->node.expr.val.arrayval.tree[i],
				    dim,
				    len,
				    matrix,
				    ipdUnit,
				    depth + 1,
				    pos_p))
	    return ipdFALSE;
      return ipdTRUE;
   }
   if ((depth == dim) && (tn->type & ipdARRAYVAL))
   {
      ipdSetError(ipdERR_UNBALANCED_ARRAY_IPDNOT_MATRIX);
      return ipdFALSE;
   }
   if (tn->type & ipdREAL)
   {
      matrix[*pos_p] = tn->node.expr.val.quantity.re;
   }
   else if (tn->type & ipdINTEGER)
   {
      matrix[*pos_p] = tn->node.expr.val.integer;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_TYPE,
		  ipdGetTypeStr(tn->type & ipdARRAYnVALUE, ipdTRUE),
		  ipdGetTypeStr(ipdREAL, ipdFALSE));
      return ipdFALSE;
   }
#if IPD_USE_UNITS
   if (ipdUnit)
   {
      if ((!*ipdUnit) && (*pos_p == 0))
      {
	 if (tn->type & ipdQUANTITY)
	    *ipdUnit = tn->node.expr.val.quantity.unit;
	 else
	    *ipdUnit = NULL;
      }
      else
      {
	 if (!iiUnitCompareable(*ipdUnit, tn->node.expr.val.quantity.unit))
	 {
	    ipdConstString u1, u2;

	    u1 = iiGetUnitString(*ipdUnit);
	    u2 = iiGetUnitString(tn->node.expr.val.quantity.unit);
	    if (!(u1))
	       u1 = "1";
	    if (!(u2))
	       u2 = "1";
	    ipdSetError(ipdERR_DIFFERENT_UNITS, "getMatrix()", u1, u2);
	    return ipdFALSE;
	 }
      }
   }
#endif /* IPD_USE_UNITS */
   (*pos_p)++;

   return ipdTRUE;
} /* iiGetSloppyRealMatrix */
/*-------------------------------------------------------------------------
 * Returns an evaluated matrix of reals.
 * !!!!!!!!!!!!!!!
 * !!! Free *len_p and *matrix_p with free() !!!!
 * !!!!!!!!!!!!!!!
 */
ipdBoolean ipdGetSloppyRealMatrix(ipdTreeNode_t  *tn,        /* [I] */
				ipdLong          *dim_p,     /* [O] */
				ipdLong         **len_p,     /* [O] */
				ipdDouble       **matrix_p,  /* [O] */
				ipdConstString   *unit)      /* [O] */
{
   if ((tn) && (dim_p) && (len_p) && (matrix_p))
   {
      ipdTreeNode_t *tmp;
      ipdLong          i, mlen;

      *dim_p    = 0;
      *len_p    = NULL;
      *matrix_p = NULL;

      /* determine dimension of the matrix */
      tmp = tn;
      while ((tmp) && (tmp->type & ipdARRAYVAL))
      {
        (*dim_p)++;
        tmp = tmp->node.expr.val.arrayval.tree[0];
      }
      /* determine size of the matrix */
      if ((*len_p = malloc(sizeof(ipdLong) * *dim_p)))
      {
	 tmp    = tn;
	 mlen   = 1;
	 for (i = 0; i < *dim_p; i++)
	 {
	    (*len_p)[i] = tmp->node.expr.val.arrayval.len;
	    mlen = mlen * (*len_p)[i];
	    tmp  = tmp->node.expr.val.arrayval.tree[0];
	 }
	 /*allocate matrix */
	 if ((*matrix_p = malloc(sizeof(ipdDouble) * mlen)))
	 {
	    if (tn->type & ipdARRAYVAL)
	    {
	       ipdLong      pos = 0;
	       ipdUnit_t *un  = NULL;
	       if (iiGetSloppyRealMatrix(tn, *dim_p, *len_p, *matrix_p, &un, 0, &pos))
	       {
		  if (unit)
		     *unit = iiGetUnitString(un);
		  return ipdTRUE;
	       }
	    }
	    else
	       ipdSetError(ipdERR_WRONG_TYPE,
			   ipdGetTypeStr(ipdARRAYVAL, ipdTRUE),
			   ipdGetTypeStr(tn->type,    ipdFALSE));
	 }
	 else
	    ipdSetError(ipdERR_IPDNO_MEMORY);
      }
      else
	 ipdSetError(ipdERR_IPDNO_MEMORY);

      if (*len_p)
      {
	 free(*len_p);
	 *len_p    = NULL;
      }
      if (*matrix_p)
      {
	 free(*matrix_p);
	 *matrix_p = NULL;
      }
      *dim_p    = 0;
   }
   return ipdFALSE;
} /* ipdGetSloppyRealMatrix */
ipdBoolean ipdIteratorGetSloppyRealMatrix(const ipdIterator_t  *iNode,     /* [I] */
					ipdLong                *dim_p,     /* [O] */
					ipdLong               **len_p,     /* [O] */
					ipdDouble             **matrix_p,  /* [O] */
					ipdConstString         *unit)      /* [O] */
{
   if ((iNode && iNode->tn))
      return ipdGetSloppyRealMatrix(iNode->tn, dim_p, len_p, matrix_p, unit);
   return ipdFALSE;
} /* ipdIteratorGetSloppyRealMatrix */
ipdBoolean ipdValueIteratorGetSloppyRealMatrix(const ipdValueIterator_t  *iVNode,    /* [I] */
					    ipdLong                     *dim_p,     /* [O] */
					    ipdLong                    **len_p,     /* [O] */
					    ipdDouble                  **matrix_p,  /* [O] */
					    ipdConstString              *unit)      /* [O] */
{
   if (iVNode && iVNode->tn)
   {
      ipdTreeNode_t *tn;
       if (iVNode->tn->type & ipdARRAYVAL)
	 tn = iVNode->tn->node.expr.val.arrayval.tree[iVNode->index];
      else
	 tn = iVNode->tn;
      return ipdGetSloppyRealMatrix(tn, dim_p, len_p, matrix_p, unit);
   }
   return ipdFALSE;
} /* ipdValueIteratorGetSloppyRealMatrix */
ipdBoolean ipdGetSloppyRealMatrixByName(ipdConstString   name,      /* [I] */
				      ipdLong         *dim_p,     /* [O] */
				      ipdLong        **len_p,     /* [O] */
				      ipdDouble      **matrix_p,  /* [O] */
				      ipdConstString  *unit)     /* [O] */
{
   ipdTreeNode_t *tn;

   if ((tn = ipdEvalByName(name, ipdFALSE)))
      return ipdGetSloppyRealMatrix(tn, dim_p, len_p, matrix_p, unit);

   return ipdFALSE;
} /* ipdGetSloppyRealMatrixByName */

/*-------------------------------------------------------------------------
 * Returns the items value (must be a real quantity).
 */
ipdBoolean ipdGetRealQuantity(ipdTreeNode_t  *tn,
			    ipdDouble        *re_p,
			    ipdChar         **unit_p)
{
   if ((tn))
   {
      if ((unit_p))
	 *unit_p = NULL;

      if ((tn->type & ipdARRAYnVALUE) == ipdREALQUANTITY)
      {
	 if ((re_p))
	    *re_p   = tn->node.expr.val.quantity.re;
#if IPD_USE_UNITS
	 if ((unit_p))
	    *unit_p = iiGetUnitString(tn->node.expr.val.quantity.unit);
#endif /* IPD_USE_UNITS */

	 return ipdTRUE;
      }
      ipdSetError(ipdERR_WRONG_TYPE,
		  ipdGetTypeStr(tn->type & ipdARRAYnVALUE, ipdTRUE),
		  ipdGetTypeStr(ipdREALQUANTITY, ipdFALSE));
   }
   return ipdFALSE;
} /* ipdGetRealQuantity */
ipdBoolean ipdIteratorGetRealQuantity(const ipdIterator_t  *iNode,
				    ipdDouble              *re_p,
				    ipdChar               **unit_p)
{
   if ((iNode && iNode->tn))
      return ipdGetRealQuantity(iNode->tn, re_p, unit_p);
   return ipdFALSE;
} /* ipdIteratorGetRealQuantity */
ipdBoolean ipdValueIteratorGetRealQuantity(const ipdValueIterator_t  *iVNode,
					 ipdDouble                   *re_p,
					 ipdChar                    **unit_p)
{
   if (iVNode && iVNode->tn)
   {
      ipdTreeNode_t *tn;
       if (iVNode->tn->type & ipdARRAYVAL)
	 tn = iVNode->tn->node.expr.val.arrayval.tree[iVNode->index];
      else
	 tn = iVNode->tn;
      return ipdGetRealQuantity(tn, re_p, unit_p);
   }
   return ipdFALSE;
} /* ipdValueIteratorGetRealQuantity */
ipdBoolean ipdGetRealQuantityByName(ipdConstString   name,
				  ipdDouble       *re_p,
				  ipdChar        **unit_p)
{
   ipdTreeNode_t *tn;

   if ((tn = ipdEvalByName(name, ipdFALSE)))
      return ipdGetRealQuantity(tn, re_p, unit_p);
   return ipdFALSE;
} /* ipdGetRealQuantityByName */

/*-------------------------------------------------------------------------
 * Returns the items value (must be a real quantity).
 */
ipdBoolean ipdGetSloppyRealQuantity(ipdTreeNode_t  *tn,
				  ipdDouble        *re_p,
				  ipdChar         **unit_p)
{
   if ((tn))
   {
      if ((unit_p))
	 *unit_p = NULL;

      if ((tn->type & ipdARRAYnVALUE) == ipdINTEGER)
      {
	 if ((re_p))
	    *re_p   = (ipdDouble)tn->node.expr.val.integer;
	 return ipdTRUE;
      }
      if ((tn->type & ipdARRAYnVALUE) == ipdREAL)
      {
	 if ((re_p))
	    *re_p   = tn->node.expr.val.quantity.re;
	 return ipdTRUE;
      }
      if ((tn->type & ipdARRAYnVALUE) == ipdREALQUANTITY)
      {
	 if ((re_p))
	    *re_p   = tn->node.expr.val.quantity.re;
#if IPD_USE_UNITS
	 if ((unit_p))
	    *unit_p = iiGetUnitString(tn->node.expr.val.quantity.unit);
#endif /* IPD_USE_UNITS */
	 return ipdTRUE;
      }
      ipdSetError(ipdERR_WRONG_TYPE,
		  ipdGetTypeStr(tn->type & ipdARRAYnVALUE, ipdTRUE),
		  ipdGetTypeStr(ipdREALQUANTITY, ipdFALSE));
   }
   return ipdFALSE;
} /* ipdGetSloppyRealQuantity */
ipdBoolean ipdIteratorGetSloppyRealQuantity(const ipdIterator_t  *iNode,
					  ipdDouble              *re_p,
					  ipdChar               **unit_p)
{
   if ((iNode && iNode->tn))
      return ipdGetSloppyRealQuantity(iNode->tn, re_p, unit_p);
   return ipdFALSE;
} /* ipdIteratorGetSloppyRealQuantity */
ipdBoolean ipdValueIteratorGetSloppyRealQuantity(const ipdValueIterator_t  *iVNode,
					       ipdDouble                   *re_p,
					       ipdChar                    **unit_p)
{
   if (iVNode && iVNode->tn)
   {
      ipdTreeNode_t *tn;
       if (iVNode->tn->type & ipdARRAYVAL)
	 tn = iVNode->tn->node.expr.val.arrayval.tree[iVNode->index];
      else
	 tn = iVNode->tn;
      return ipdGetSloppyRealQuantity(tn, re_p, unit_p);
   }
   return ipdFALSE;
} /* ipdValueIteratorGetSloppyRealQuantity */
ipdBoolean ipdGetSloppyRealQuantityByName(ipdConstString   name,
					ipdDouble       *re_p,
					ipdChar        **unit_p)
{
   ipdTreeNode_t *tn;

   if ((tn = ipdEvalByName(name, ipdFALSE)))
      return ipdGetSloppyRealQuantity(tn, re_p, unit_p);

   return ipdFALSE;
} /* ipdGetSloppyRealQuantityByName */

/*-------------------------------------------------------------------------
 * Returns the items value (must be a complex number).
 */
ipdBoolean ipdGetComplex(ipdTreeNode_t *tn,
		       ipdDouble       *re_p,
		       ipdDouble       *im_p)
{
   if ((tn))
   {
      if ((tn->type & ipdARRAYnVALUE) == ipdREAL)
      {
	 if ((re_p))
	    *re_p = tn->node.expr.val.quantity.re;
	 if ((im_p))
	    *im_p = 0.0;
	 return ipdTRUE;
      }
      if ((tn->type & ipdARRAYnVALUE) == ipdCOMPLEX)
      {
	 if ((re_p))
	    *re_p = tn->node.expr.val.quantity.re;
	 if ((im_p))
	    *im_p = tn->node.expr.val.quantity.im;
	 return ipdTRUE;
      }
      ipdSetError(ipdERR_WRONG_TYPE,
		  ipdGetTypeStr(tn->type & ipdARRAYnVALUE, ipdTRUE),
		  ipdGetTypeStr(ipdCOMPLEX, ipdFALSE));
   }
   return ipdFALSE;
} /* ipdGetComplex */
ipdBoolean ipdIteratorGetComplex(const ipdIterator_t *iNode,
			       ipdDouble             *re_p,
			       ipdDouble             *im_p)
{
   if ((iNode && iNode->tn))
      return ipdGetComplex(iNode->tn, re_p, im_p);
   return ipdFALSE;
} /* ipdIteratorGetComplex */
ipdBoolean ipdValueIteratorGetComplex(const ipdValueIterator_t *iVNode,
				    ipdDouble                  *re_p,
				    ipdDouble                  *im_p)
{
   if (iVNode && iVNode->tn)
   {
      ipdTreeNode_t *tn;
       if (iVNode->tn->type & ipdARRAYVAL)
	 tn = iVNode->tn->node.expr.val.arrayval.tree[iVNode->index];
      else
	 tn = iVNode->tn;
      return ipdGetComplex(tn, re_p, im_p);
   }
   return ipdFALSE;
} /* ipdValueIteratorGetComplex */
ipdBoolean ipdGetComplexByName(ipdConstString  name,
			     ipdDouble      *re_p,
			     ipdDouble      *im_p)
{
   ipdTreeNode_t *tn;

   if ((tn = ipdEvalByName(name, ipdFALSE)))
      return ipdGetComplex(tn, re_p, im_p);

   return ipdFALSE;
} /* ipdGetComplexByName */

/*-------------------------------------------------------------------------
 * Returns the items value (must be a complex quantity).
 */
ipdBoolean ipdGetComplexQuantity(ipdTreeNode_t  *tn,
			       ipdDouble        *re_p,
			       ipdDouble        *im_p,
			       ipdChar         **unit_p)
{
   if ((tn))
   {
      if ((unit_p))
	 *unit_p = NULL;

      if (tn->type & ipdQUANTITY)
      {
	 if ((re_p))
	    *re_p   = tn->node.expr.val.quantity.re;
	 if ((im_p))
	 {
	    if (tn->type & ipdCOMPLEX)
	       *im_p = tn->node.expr.val.quantity.im;
	    else
	       *im_p = 0.0;
	 }
#if IPD_USE_UNITS
	 if ((unit_p))
	    *unit_p = iiGetUnitString(tn->node.expr.val.quantity.unit);
#endif /* IPD_USE_UNITS */
	 return ipdTRUE;
      }
      ipdSetError(ipdERR_WRONG_TYPE,
		  ipdGetTypeStr(tn->type & ipdARRAYnVALUE, ipdTRUE),
		  ipdGetTypeStr(ipdCOMPLEXQUANTITY, ipdFALSE));
   }
   return ipdFALSE;
} /* ipdGetComplexQuantity */
ipdBoolean ipdIteratorGetComplexQuantity(const ipdIterator_t  *iNode,
				       ipdDouble              *re_p,
				       ipdDouble              *im_p,
				       ipdChar               **unit_p)
{
   if ((iNode && iNode->tn))
      return ipdGetComplexQuantity(iNode->tn, re_p, im_p, unit_p);
   return ipdFALSE;
} /* ipdIteratorGetComplexQuantity */
ipdBoolean ipdValueIteratorGetComplexQuantity(const ipdValueIterator_t *iVNode,
					    ipdDouble                  *re_p,
					    ipdDouble                  *im_p,
					    ipdChar                   **unit_p)
{
   if (iVNode && iVNode->tn)
   {
      ipdTreeNode_t *tn;
       if (iVNode->tn->type & ipdARRAYVAL)
	 tn = iVNode->tn->node.expr.val.arrayval.tree[iVNode->index];
      else
	 tn = iVNode->tn;
      return ipdGetComplexQuantity(tn, re_p, im_p, unit_p);
   }
   return ipdFALSE;
} /* ipdValueIteratorGetComplexQuantity */
ipdBoolean ipdGetComplexQuantityByName(ipdConstString   name,
				     ipdDouble       *re_p,
				     ipdDouble       *im_p,
				     ipdChar        **unit_p)
{
   ipdTreeNode_t *tn;

   if ((tn = ipdEvalByName(name, ipdFALSE)))
      return ipdGetComplexQuantity(tn, re_p, im_p, unit_p);

   return ipdFALSE;
} /* ipdGetComplexQuantityByName */

/*-------------------------------------------------------------------------
 * Returns the items value (must be a string).
 */
ipdBoolean ipdGetString(ipdTreeNode_t  *tn,
		      ipdChar         **string_p)
{
   if ((tn))
   {
      if ((tn->type & ipdARRAYnVALUE) == ipdSTRING)
      {
	 if ((string_p))
	 {
	    if ((*string_p = tn->node.expr.val.string) &&
		(*string_p[0] == '\0'))
	       *string_p = NULL; /* return NULL for empty strings */
	 }
	 return ipdTRUE;
      }
      ipdSetError(ipdERR_WRONG_TYPE,
		  ipdGetTypeStr(tn->type & ipdARRAYnVALUE, ipdTRUE),
		  ipdGetTypeStr(ipdSTRING, ipdFALSE));
   }
   return ipdFALSE;
} /* ipdGetString */
ipdBoolean ipdIteratorGetString(const ipdIterator_t  *iNode,
			      ipdChar               **string_p)
{
   if ((iNode && iNode->tn))
      return ipdGetString(iNode->tn, string_p);
   return ipdFALSE;
} /* ipdIteratorGetString */
ipdBoolean ipdValueIteratorGetString(const ipdValueIterator_t  *iVNode,
				   ipdChar                    **string_p)
{
   if (iVNode && iVNode->tn)
   {
      ipdTreeNode_t *tn;
       if (iVNode->tn->type & ipdARRAYVAL)
	 tn = iVNode->tn->node.expr.val.arrayval.tree[iVNode->index];
      else
	 tn = iVNode->tn;
      return ipdGetString(tn, string_p);
   }
   return ipdFALSE;
} /* ipdValueIteratorGetString */
ipdBoolean ipdGetStringByName(ipdConstString   name,
			    ipdChar        **string_p)
{
   ipdTreeNode_t *tn;

   if ((tn = ipdEvalByName(name, ipdFALSE)))
      return ipdGetString(tn, string_p);

   return ipdFALSE;
} /* ipdGetStringByName */

/*-------------------------------------------------------------------------
 * same as ipdGetString but returns a pointer to an empty string instead of a 
 * NULL Pointer in case the ipd string is empty.
 */
ipdBoolean ipdGetStringSafe(ipdTreeNode_t *tn,
			  ipdConstString  *string_p)
{
   ipdChar*	cc_ptr;
   ipdBoolean	retVal = ipdGetString(tn, &cc_ptr);

   if (cc_ptr == NULL)
   {
      *string_p = "";
   }
   else
   {
      *string_p = cc_ptr;
   }

   return retVal;
} /* ipdGetStringSafe */
ipdBoolean ipdIteratorGetStringSafe(const ipdIterator_t *iNode,
				  ipdConstString        *string_p)
{
   if ((iNode && iNode->tn))
      return ipdGetStringSafe(iNode->tn, string_p);
   return ipdFALSE;
} /* ipdIteratorGetString */
ipdBoolean ipdValueIteratorGetStringSafe(const ipdValueIterator_t *iVNode,
				       ipdConstString             *string_p)

{
   if (iVNode && iVNode->tn)
   {
      ipdTreeNode_t *tn;
       if (iVNode->tn->type & ipdARRAYVAL)
	 tn = iVNode->tn->node.expr.val.arrayval.tree[iVNode->index];
      else
	 tn = iVNode->tn;
      return ipdGetStringSafe(tn, string_p);
   }
   return ipdFALSE;
} /* ipdValueIteratorGetStringSafe */
ipdBoolean ipdGetStringByNameSafe(ipdConstString  name,
				ipdConstString *string_p)
{
   ipdTreeNode_t *tn;

   if ((tn = ipdEvalByName(name, ipdFALSE)))
      return ipdGetStringSafe(tn, string_p);

   return ipdFALSE;
} /* ipdGetStringByNameSafe */


/*-------------------------------------------------------------------------
 * Returns the items value (must be a boolean).
 */
ipdBoolean ipdGetBoolean(ipdTreeNode_t *tn,
		       ipdBoolean      *boolean_p)
{
   if ((tn))
   {
      if ((tn->type & ipdARRAYnVALUE) == ipdBOOLEAN)
      {
	 if ((boolean_p))
	    *boolean_p = tn->node.expr.val.boolean;
	 return ipdTRUE;
      }
      ipdSetError(ipdERR_WRONG_TYPE,
		  ipdGetTypeStr(tn->type & ipdARRAYnVALUE, ipdTRUE),
		  ipdGetTypeStr(ipdBOOLEAN, ipdFALSE));
   }
   return ipdFALSE;
} /* ipdGetBoolean */
ipdBoolean ipdIteratorGetBoolean(const ipdIterator_t *iNode,
			       ipdBoolean            *boolean_p)
{
   if ((iNode && iNode->tn))
      return ipdGetBoolean(iNode->tn, boolean_p);
   return ipdFALSE;
} /* ipdIteratorGetBoolean */
ipdBoolean ipdValueIteratorGetBoolean(const ipdValueIterator_t *iVNode,
				   ipdBoolean                 *boolean_p)
{
   if (iVNode && iVNode->tn)
   {
      ipdTreeNode_t *tn;
       if (iVNode->tn->type & ipdARRAYVAL)
	 tn = iVNode->tn->node.expr.val.arrayval.tree[iVNode->index];
      else
	 tn = iVNode->tn;
      return ipdGetBoolean(tn, boolean_p);
   }
   return ipdFALSE;
} /* ipdValueIteratorGetBoolean */
ipdBoolean ipdGetBooleanByName(ipdConstString name,
			     ipdBoolean   *boolean_p)
{
   ipdTreeNode_t *tn;

   if ((tn = ipdEvalByName(name, ipdFALSE)))
      return ipdGetBoolean(tn, boolean_p);

   return ipdFALSE;
} /* ipdGetBooleanByName */

/*=======================================================================
	SET DATA
=======================================================================*/

/** @brief Prepares a empty ipdTreeNode_t structure for storing a new variable and stores it.
 *
 * To do this the variable name and the sections to store it
 * must be given. This can be done seperately or by a complete fullname.
 * If name == NULL, fullname must be a vaild fullname of the variable.
 * If name is a valid (!) name, fullname must be the section name where
 * the variable must be stored.
 */
ipdTreeNode_t *iiPrepareEmptyVariable(ipdConstString fullname,
				      ipdConstString name)
{
   ipdTreeNode_t *tn = NULL;

   if ((fullname))
   {
      ipdResetErrorStatus();
      tn = iiCreateTreeNode(ipdVARIABLE);  /* memory allocation */
      ipd_RETURNonERRORp;
      if ((name))
	     tn->node.sv.fullname     = iiComposeFullName(fullname, name);
      else
	     tn->node.sv.fullname     = iiCreateName(fullname, NULL);
        tn->node.sv.svu.var.flags  |= ipdVF_EXTERN;
        tn->node.sv.svu.var.tree    = iiCreateTreeNode(ipdINVALID);
      if (ipdGetErrorStatus())
      {
			 iiFreeTreeNodeRec(tn, ipdFALSE);
			 tn = NULL;
      }
      else
      {
			 ipdApplicationSet = ipdTRUE;
			 if ((tn = iiSave(tn, ipdTRUE, 0, NULL)))
			 {
			    /* variable locked? */
			    if (tn->node.sv.svu.var.lock)
			    {
			       ipdSetError(ipdERR_VAR_LOCKED, name);
			       tn = NULL;
			    }
			 }
			 ipdApplicationSet = ipdFALSE;
      }
   }
   return tn;
} /* iiPrepareEmptyVariable */

/*-------------------------------------------------------------------------
 */
static ipdBoolean iiCheckIteratorExtVariable(ipdIterator_t *iNode)
{
   if (!(iNode->tn->type & ipdVARIABLE))
   {
      ipdSetError(ipdERR_CANT_EVAL_SECTION, iNode->tn->node.sv.fullname);
      return ipdFALSE;
   }
   if (!ipdEditorPrivileges &&
       !(iNode->tn->node.sv.svu.var.flags & ipdVF_EXTERN))
   {
      ipdSetError(ipdERR_APPLICATION_SET_VAR, iNode->tn->node.sv.fullname);
      return ipdFALSE;
   }
   return ipdTRUE;
}
/*-------------------------------------------------------------------------
 */
static ipdBoolean iiSetInteger(ipdTreeNode_t *tn,
			     const ipdLong    integer)
{
   tn->node.sv.svu.var.tree->type                  = ipdINTEGER;
   tn->node.sv.svu.var.tree->node.expr.val.integer = integer;
   return ipdTRUE;
} /* iiSetInteger */

ipdBoolean ipdSetInteger(ipdIterator_t *iNode,
		       const ipdLong    integer)
{
   ipdTreeNode_t *tn;

   if ((iNode) && (iNode->tn) &&
       (iiCheckIteratorExtVariable(iNode)) &&
       (tn = iiPrepareEmptyVariable(iNode->sectName,
				    iNode->tn->node.sv.name)))
   {
      return iiSetInteger(tn, integer);
   }
   return ipdFALSE;
} /* ipdSetInteger */

ipdBoolean ipdSetIntegerByName(ipdConstString fullname,
			     const ipdLong  integer)
{
   ipdTreeNode_t *tn;

   if ((fullname) && (tn = iiPrepareEmptyVariable(fullname, NULL)))
   {
      return iiSetInteger(tn, integer);
   }
   return ipdFALSE;
} /* ipdSetIntegerByName */

/*-------------------------------------------------------------------------
 */

static ipdBoolean iiSetReal(ipdTreeNode_t *tn,
			  const ipdDouble  re)
{
   tn->node.sv.svu.var.tree->type                      = ipdREAL;
   tn->node.sv.svu.var.tree->node.expr.val.quantity.re = re;
   return ipdTRUE;
} /* iiSetReal */

ipdBoolean ipdSetReal(ipdIterator_t *iNode,
		    const ipdDouble  re)
{
   ipdTreeNode_t *tn;

   if ((iNode) && (iNode->tn) &&
       (iiCheckIteratorExtVariable(iNode)) &&
       (tn = iiPrepareEmptyVariable(iNode->sectName,
				    iNode->tn->node.sv.name)))
   {
      return iiSetReal(tn, re);
   }
   return ipdFALSE;
} /* ipdSetReal */


ipdBoolean ipdSetRealByName(ipdConstString  fullname,
			  const ipdDouble re)
{
   ipdTreeNode_t *tn;

   if ((fullname) && (tn = iiPrepareEmptyVariable(fullname, NULL)))
   {
      return iiSetReal(tn, re);
   }
   return ipdFALSE;
} /* ipdSetRealByName */

/*-------------------------------------------------------------------------
 */
static ipdBoolean iiSetRealQuantity(ipdTreeNode_t *tn,
				  const ipdDouble  re,
				  ipdConstString   unit)
{
   tn->node.sv.svu.var.tree->type                       = ipdREAL;
   tn->node.sv.svu.var.tree->node.expr.val.quantity.re  = re;
#if IPD_USE_UNITS
   if ((unit))
   {
      if (!(tn->node.sv.svu.var.tree->node.expr.val.quantity.unit =
	    iiUnitHashFindUnitStr(unit)))
      {
	 iiFreeTreeNodeRec(tn, ipdFALSE);
	 return ipdFALSE;
      }
      tn->node.sv.svu.var.tree->type                      |= ipdQUANTITY;
      tn->node.sv.svu.var.tree->node.expr.val.quantity.re *=
	 tn->node.sv.svu.var.tree->node.expr.val.quantity.unit->unitRep.unitFactor;
   }
#endif /* IPD_USE_UNITS */
   return ipdTRUE;
} /* iiSetRealQuantity */

ipdBoolean ipdSetRealQuantity(ipdIterator_t *iNode,
			    const ipdDouble  re,
			    ipdConstString   unit)
{
   ipdTreeNode_t *tn;

   if ((iNode) && (iNode->tn) &&
       (iiCheckIteratorExtVariable(iNode)) &&
       (tn = iiPrepareEmptyVariable(iNode->sectName,
				    iNode->tn->node.sv.name)))
   {
      return iiSetRealQuantity(tn, re, unit);
   }
   return ipdFALSE;
} /* ipdSetRealQuantity */

ipdBoolean ipdSetRealQuantityByName(ipdConstString fullname,
				  ipdDouble      re,
				  ipdConstString unit)
{
   ipdTreeNode_t *tn;

   if ((fullname) && (tn = iiPrepareEmptyVariable(fullname, NULL)))
   {
      return iiSetRealQuantity(tn, re, unit);
   }
   return ipdFALSE;
} /* ipdSetRealQuantityByName */

/*-------------------------------------------------------------------------
 */
static ipdBoolean iiSetComplex(ipdTreeNode_t *tn,
			     const ipdDouble  re,
			     const ipdDouble  im)
{
   tn->node.sv.svu.var.tree->type                      = ipdCOMPLEX;
   tn->node.sv.svu.var.tree->node.expr.val.quantity.re = re;
   tn->node.sv.svu.var.tree->node.expr.val.quantity.im = im;
   return ipdTRUE;
} /* iiSetComplex */

ipdBoolean ipdSetComplex(ipdIterator_t *iNode,
		       const ipdDouble  re,
		       const ipdDouble  im)
{
   ipdTreeNode_t *tn;

   if ((iNode) && (iNode->tn) &&
       (iiCheckIteratorExtVariable(iNode)) &&
       (tn = iiPrepareEmptyVariable(iNode->sectName,
				    iNode->tn->node.sv.name)))
   {
      return iiSetComplex(tn, re, im);
   }
   return ipdFALSE;
} /* ipdSetComplex */

ipdBoolean ipdSetComplexByName(ipdConstString  fullname,
			     const ipdDouble re,
			     const ipdDouble im)
{
   ipdTreeNode_t *tn;

   if ((fullname) && (tn = iiPrepareEmptyVariable(fullname, NULL)))
   {
      return iiSetComplex(tn, re, im);
   }
   return ipdFALSE;
} /* ipdSetComplexByName */

/*-------------------------------------------------------------------------
 */
static ipdBoolean iiSetComplexQuantity(ipdTreeNode_t *tn,
				     const ipdDouble  re,
				     const ipdDouble  im,
				     ipdConstString   unit)
{
   tn->node.sv.svu.var.tree->type                      = ipdCOMPLEX;
   tn->node.sv.svu.var.tree->node.expr.val.quantity.re = re;
   tn->node.sv.svu.var.tree->node.expr.val.quantity.im = im;
#if IPD_USE_UNITS
   if ((unit))
   {
      if (!(tn->node.sv.svu.var.tree->node.expr.val.quantity.unit =
	    iiUnitHashFindUnitStr(unit)))
      {
	 iiFreeTreeNodeRec(tn, ipdFALSE);
	 return ipdFALSE;
      }
      tn->node.sv.svu.var.tree->type                      |= ipdQUANTITY;
      tn->node.sv.svu.var.tree->node.expr.val.quantity.re *=
	 tn->node.sv.svu.var.tree->node.expr.val.quantity.unit->unitRep.unitFactor;
      tn->node.sv.svu.var.tree->node.expr.val.quantity.im *=
	 tn->node.sv.svu.var.tree->node.expr.val.quantity.unit->unitRep.unitFactor;
   }
#endif /* IPD_USE_UNITS */
   return ipdTRUE;
} /* iiSetComplexQuantity */

ipdBoolean ipdSetComplexQuantity(ipdIterator_t *iNode,
			       const ipdDouble  re,
			       const ipdDouble  im,
			       ipdConstString   unit)
{
   ipdTreeNode_t *tn;

   if ((iNode) && (iNode->tn) &&
       (iiCheckIteratorExtVariable(iNode)) &&
       (tn = iiPrepareEmptyVariable(iNode->sectName,
				    iNode->tn->node.sv.name)))
   {
      return iiSetComplexQuantity(tn, re, im, unit);
   }
   return ipdFALSE;
} /* ipdSetComplexQuantity */

ipdBoolean ipdSetComplexQuantityByName(ipdConstString  fullname,
				     const ipdDouble re,
				     const ipdDouble im,
				     ipdConstString  unit)
{
   ipdTreeNode_t *tn;

   if ((fullname) && (tn = iiPrepareEmptyVariable(fullname, NULL)))
   {
      return iiSetComplexQuantity(tn, re, im, unit);
   }
   return ipdFALSE;
} /* ipdSetComplexQuantityByName */

/*-------------------------------------------------------------------------
 */
static ipdBoolean iiSetString(ipdTreeNode_t *tn,
			    ipdConstString   string)
{
   if (!(string))
      string = "";
   tn->node.sv.svu.var.tree->type                 = ipdSTRING;
   tn->node.sv.svu.var.tree->node.expr.val.string = (string) ?
      iiStrDup(string) : (ipdChar *)NULL;
   ipd_RETURNonERRORb;
   return ipdTRUE;
} /* iiSetString */

ipdBoolean ipdSetString(ipdIterator_t *iNode,
		      ipdConstString   string)
{
   ipdTreeNode_t *tn;

   if ((iNode) && (iNode->tn) &&
       (iiCheckIteratorExtVariable(iNode)) &&
       (tn = iiPrepareEmptyVariable(iNode->sectName,
				    iNode->tn->node.sv.name)))
   {
      return iiSetString(tn, string);
   }
   return ipdFALSE;
} /* ipdSetString */

ipdBoolean ipdSetStringByName(ipdConstString fullname,
			    ipdConstString string)
{
   ipdTreeNode_t *tn;

   if ((fullname) && (tn = iiPrepareEmptyVariable(fullname, NULL)))
   {
      return iiSetString(tn, string);
   }
   return ipdFALSE;
} /* ipdSetStringByName */

/*-------------------------------------------------------------------------
 */
static ipdBoolean iiSetBoolean(ipdTreeNode_t  *tn,
			     const ipdBoolean  boolean)
{
   tn->node.sv.svu.var.tree->type                  = ipdBOOLEAN;
   tn->node.sv.svu.var.tree->node.expr.val.boolean = boolean;
   return ipdTRUE;
} /* iiSetBoolean */

ipdBoolean ipdSetBoolean(ipdIterator_t  *iNode,
		       const ipdBoolean  boolean)
{
   ipdTreeNode_t *tn;

   if ((iNode) && (iNode->tn) &&
       (iiCheckIteratorExtVariable(iNode)) &&
       (tn = iiPrepareEmptyVariable(iNode->sectName,
				    iNode->tn->node.sv.name)))
   {
      return iiSetBoolean(tn, boolean);
   }
   return ipdFALSE;
} /* ipdSetBoolean */

ipdBoolean ipdSetBooleanByName(ipdConstString    fullname,
			     const ipdBoolean  boolean)
{
   ipdTreeNode_t *tn;

   if ((fullname) && (tn = iiPrepareEmptyVariable(fullname, NULL)))
   {
      return iiSetBoolean(tn, boolean);
   }
   return ipdFALSE;
} /* ipdSetBooleanByName */

/** @brief Creates a multidimensional list which is comparable to and looks similar to a matrix.
 *
 * itemType must one of ipdINTEGER, ipdREAL, ipdREALQUANTITY.
 * Corresponding to this type the dmatrix, lmatrix, and/or unit have to be
 * completed respectively.
 * Example: dim = 1 means something like this: [ 1, 2, 3, 4, 5 ]
 *          dim = 2 means something like this: [[ 1, 2, 3 ], [ 4, 5, 6 ]]
 *          dim = 3 means something like this: [[[ 1, 2 ], [ 3, 4 ]], [[ 5, 6 ], [ 7, 8 ]]]
 *          ...
 * len = [z, y, x] for dim = 3
 */
static ipdBoolean iiSetIntRealQuanMatrix(ipdTreeNode_t   *tn,
				       const ipdLong      dim,
				       const ipdLong     *len,
				       const ipdType_t  itemType,
				       const ipdLong     *lmatrix,
				       const ipdDouble   *dmatrix,
				       ipdConstString     unit)
{
   ipdTreeNode_t **array;
   ipdTreeNode_t **matrix;
   ipdBoolean        ok = ipdTRUE;
   ipdLong           i, j;
   ipdLong           arraylen, matrixlen;
   ipdLong           pos, npos, tlen, nlen, nrep, irep;
   ipdUnit_t      *ipdunit = NULL;

   if ((itemType != ipdINTEGER) &&
       (itemType != ipdREAL) &&
       (itemType != ipdREALQUANTITY))
   {
      ipdSetError(ipdERR_WRONG_TYPE,
		  ipdGetTypeStr(itemType, ipdTRUE),
		  ipdGetTypeStr(ipdINTEGER|ipdREAL|ipdREALQUANTITY, ipdFALSE));
      return ipdFALSE;
   }
   if (((itemType == ipdINTEGER)      && (lmatrix != NULL)) ||
       ((itemType == ipdREAL)         && (dmatrix != NULL)) ||
       ((itemType == ipdREALQUANTITY) && (dmatrix != NULL) && (unit != NULL)))
   {
#if IPD_USE_UNITS
      if ((unit))
      {
	      if (!(ipdunit = iiUnitHashFindUnitStr(unit)))
	      return ipdFALSE;
      }
#endif /* IPD_USE_UNITS */

      /* number of auxiliary node elements */
      arraylen = 0;
      for (i = dim - 2; i >= 0; i--)
      {
	      arraylen = arraylen * len[i];
	      arraylen = arraylen + len[i];
      }
      /* The auxiliary node elements  */
      if (arraylen > 0)
      {
	      if ((array = ipdMBalloc(arraylen * sizeof(ipdTreeNode_t *))))
	      {
	         for (i = 0; i < arraylen; i++)
	            if (!(array[i] = iiCreateTreeNode(ipdARRAY)))
		         ok = ipdFALSE;
	      }
	      else
	         ok = ipdFALSE;
      }
      else
	      array = NULL;

      /* number of matrix elements */
      matrixlen = 1;
      for (i = 0; i < dim; i++)
			 matrixlen = matrixlen * len[i];
      /* The matrix elements */
      if ((matrix = ipdMBalloc(matrixlen * sizeof(ipdTreeNode_t *))))
      {  
			 /* create them */
			 for (i = 0; i < matrixlen; i++)
			 {
			    if ((matrix[i] = iiCreateTreeNode(itemType)))
			    {
			       if (itemType == ipdINTEGER)
				  matrix[i]->node.expr.val.integer     = lmatrix[i];
			       else if ((itemType == ipdREAL) || (itemType == ipdREALQUANTITY))
				  matrix[i]->node.expr.val.quantity.re = dmatrix[i];
		#if IPD_USE_UNITS
			       if ((unit) && (itemType == ipdREALQUANTITY))
			       {
				  matrix[i]->node.expr.val.quantity.unit  = iiCopyUnit(ipdunit);
				  matrix[i]->node.expr.val.quantity.re   *= ipdunit->unitRep.unitFactor;
			       }
		#endif /* IPD_USE_UNITS */
			    }
			    else
			       ok = ipdFALSE;
			 }
			 /* Since the unit has been instanciated by a call to iiUnitHashFindUnitStr()
			    and then copied matrixlen times, it is therefore used (matrixlen + 1) times.
			    So we have to remove it one time. */
			 iiUnitHashRemoveUnit(ipdunit);
			 /* link the matrix elements */
			 tlen = 0;
			 for (i = 0; i < matrixlen; i++)
			 {
			    if (tlen == 0)
			       tlen = len[dim - 1];
			    tlen--;
			    if (tlen)
			       matrix[i]->next = matrix[i + 1];
			 }
      }
      else
	 ok = ipdFALSE;

      if (!ok) /* Error? */
      {
			 if (array)
			 {
			    for (i = 0; i < arraylen; i++)
			       ipdMBfree(array[i]);
			    ipdMBfree(array);
			 }
			 if (matrix)
			 {
			    for (i = 0; i < matrixlen; i++)
			       ipdMBfree(matrix[i]);
			    ipdMBfree(matrix);
			 }
			 return ipdFALSE;
      }

      /* link the auxiliary node elements */
      pos  = 0;
      npos = 0;
      nrep = 1;
      for (j = 0; j < dim - 1; j++)
      {
			 tlen = len[j];
			 nlen = len[j + 1];
			 if (npos == 0)
			    npos = npos + tlen;
			 irep = tlen * nrep;
			 nrep = nrep * tlen;
			 for (i = 0; i < irep; i++)
			 {
			    if (j < dim - 2)
			    {
			       array[pos]->node.expr.func.array = array[npos];
			    }
			    else
			    {
			       array[pos]->node.expr.func.array = matrix[i * len[dim - 1]];
			    }
			    if (tlen == 0)
			       tlen = len[j];
			    tlen--;
			    if (tlen)
			    {
			       array[pos]->next = array[pos + 1];
			    }
			    pos  = pos  + 1;
			    if (j < dim - 2)
			       npos = npos + nlen;
			 }
      }

      tn->node.sv.svu.var.tree->type = ipdARRAY;
      if (dim > 1)
	     tn->node.sv.svu.var.tree->node.expr.func.array = array[0];
      else
	     tn->node.sv.svu.var.tree->node.expr.func.array = matrix[0];
      /* ipdDumpRec(stdout, tn, ipdTRUE, ipdFALSE, fprintf); */
      
   #if !IPD_USE_IPD_MALLOC /*[JW] for normal mem managment we have to backup a memory pointer*/
      global_array_backup = array;
      global_matrix_backup = matrix;
   #endif          
      return ipdTRUE;
   }
   return ipdFALSE;
} /* iiSetIntRealQuanMatrix */

ipdBoolean ipdSetIntegerMatrix(ipdIterator_t *iNode,
			     const ipdLong    dim,
			     const ipdLong   *len,
			     const ipdLong   *lmatrix)
{
   ipdTreeNode_t *tn;

   if ((iNode) && (iNode->tn) &&
       (iiCheckIteratorExtVariable(iNode)) &&
       (tn = iiPrepareEmptyVariable(iNode->sectName,
				    iNode->tn->node.sv.name)))
   {
      return iiSetIntRealQuanMatrix(tn, dim, len, ipdINTEGER, lmatrix, NULL, NULL);
   }
   return ipdFALSE;
} /* ipdSetIntegerMatrix */

ipdBoolean ipdSetIntegerMatrixByName(ipdConstString   fullname,
				const ipdLong    dim,
				const ipdLong   *len,
				const ipdLong   *lmatrix)
{
   ipdTreeNode_t *tn;

   if ((fullname) && (tn = iiPrepareEmptyVariable(fullname, NULL)))
   {
      return iiSetIntRealQuanMatrix(tn, dim, len, ipdINTEGER, lmatrix, NULL, NULL);
   }
   return ipdFALSE;
} /* ipdSetIntegerMatrixByName */

ipdBoolean ipdSetRealMatrix(ipdIterator_t *iNode,
			  const ipdLong    dim,
			  const ipdLong   *len,
			  const ipdDouble *dmatrix)
{
   ipdTreeNode_t *tn;

   if ((iNode) && (iNode->tn) &&
       (iiCheckIteratorExtVariable(iNode)) &&
       (tn = iiPrepareEmptyVariable(iNode->sectName,
				    iNode->tn->node.sv.name)))
   {
      return iiSetIntRealQuanMatrix(tn, dim, len, ipdREAL, NULL, dmatrix, NULL);
   }
   return ipdFALSE;
} /* ipdSetRealMatrix */

ipdBoolean ipdSetRealMatrixByName(ipdConstString   fullname,
				const ipdLong    dim,
				const ipdLong   *len,
				const ipdDouble *dmatrix)
{
   ipdTreeNode_t *tn;

   if ((fullname) && (tn = iiPrepareEmptyVariable(fullname, NULL)))
   {
      return iiSetIntRealQuanMatrix(tn, dim, len, ipdREAL, NULL, dmatrix, NULL);
   }
   return ipdFALSE;
} /* ipdSetRealMatrixByName */

ipdBoolean ipdSetRealQuantityMatrix(ipdIterator_t *iNode,
				  const ipdLong    dim,
				  const ipdLong   *len,
				  const ipdDouble *dmatrix,
				  ipdConstString   unit)
{
   ipdTreeNode_t *tn;

   if ((iNode) && (iNode->tn) &&
       (iiCheckIteratorExtVariable(iNode)) &&
       (tn = iiPrepareEmptyVariable(iNode->sectName,
				    iNode->tn->node.sv.name)))
   {
      return iiSetIntRealQuanMatrix(tn, dim, len, ipdREALQUANTITY, NULL, dmatrix, unit);
   }
   return ipdFALSE;
} /* ipdSetRealQuantityMatrix */

ipdBoolean ipdSetRealQuantityMatrixByName(ipdConstString   fullname,
					const ipdLong    dim,
					const ipdLong   *len,
					const ipdDouble *dmatrix,
					ipdConstString   unit)
{
   ipdTreeNode_t *tn;

   if ((fullname) && (tn = iiPrepareEmptyVariable(fullname, NULL)))
   {
      return iiSetIntRealQuanMatrix(tn, dim, len, ipdREALQUANTITY, NULL, dmatrix, unit);
   }

   return ipdFALSE;
} /* ipdSetRealQuantityMatrixByName */

#if 0 /*obsolet */
static ipdBoolean iiSetRealMatrix2D(ipdTreeNode_t *tn,
				  const ipdLong    lenx,
				  const ipdLong    leny,
				  const ipdDouble *dmatrix)
{
   ipdTreeNode_t *array [leny];
   ipdTreeNode_t *matrix[leny][lenx];
   ipdBoolean       ok = ipdTRUE;
   ipdLong          y, x;

   /* The matrix elements */
   for (y = 0; y < leny; y++)
      for (x = 0; x < lenx; x++)
	 if (!(matrix[y][x] = iiCreateTreeNode(ipdREAL)))
	 {
	    matrix[y][x]->node.expr.val.quantity.re = dmatrix[leny * y + x];
	    ok = ipdFALSE;
	 }
   /* The rows */
   for (y = 0; y < leny; y++)
      if (!(array[y] = iiCreateTreeNode(ipdARRAY)))
	 ok = ipdFALSE;
   /* The matrix */
   if (!ok) /* Error? */
   {
      for (y = 0; y < leny; y++)
      {
	 for (x = 0; x < lenx; x++)
	    ipdMBfree(matrix[y][x]);
	 ipdMBfree(array[y]);
      }
      return ipdFALSE;
   }
   /* link the elements to form leny lists */
   for (y = 0; y < leny; y++)
   {
      for (x = 0; x < lenx - 1; x++)
	 matrix[y][x]->next = matrix[y][x + 1];
      array[y]->node.expr.func.array = matrix[y][0];
   }
   /* link the rows to form a list (the matrix) */
   for (y = 0; y < leny - 1; y++)
      array[y]->next = array[y + 1];
   tn->node.sv.svu.var.tree->type                 = ipdARRAY;
   tn->node.sv.svu.var.tree->node.expr.func.array = array[0];

   return ipdTRUE;
} /* iiSetRealMatrix2D */
#endif

/*=======================================================================
	Locking Variables
=======================================================================*/

/* A lock is a counter which is increased each time a LOCK happens.
   Of course this counter will be decreased each time an UNLOCK
   is performed.
   Only variable can be locked.
   A lock is recommended when reading values from the inner structures
   of a variable, e.g. for reading an array by using handles. */

ipdBoolean ipdLockVariable(ipdConstString name)
{
   if ((name))
   {
      ipdTreeNode_t *tn;

      ipdResetErrorStatus();
      if ((tn = ipdExistItemByName(name, ipdVARIABLE, ipdFALSE)))
      {
	 tn->node.sv.svu.var.lock++;
	 return ipdTRUE;
      }
   }
   return ipdFALSE;
}

ipdBoolean ipdUnLockVariable(ipdConstString name)
{
   if ((name))
   {
      ipdTreeNode_t *tn;

      ipdResetErrorStatus();
      if ((tn = ipdExistItemByName(name, ipdVARIABLE, ipdFALSE)))
      {
	 if (tn->node.sv.svu.var.lock)
	    tn->node.sv.svu.var.lock--;
	 return ipdTRUE;
      }
   }
   return ipdFALSE;
}

/*=======================================================================
	DELETE
=======================================================================*/

/** @brief Deletes an item */
static ipdBoolean iiDeleteItem(ipdTreeNode_t *tn)
{
   if ((tn))
   {
      if (!ipdEditorPrivileges)
      {
	 ipdSetError(ipdERR_DELETE_IPDNOT_ALLOWED, tn->node.sv.fullname);
	 return ipdFALSE;
      }
      if (tn->type & ipdVARIABLE)
      {
	 /* Trace variable. */
	 iiTraceVar(tn, "delete");

	 /* variable locked? */
	 if (tn->node.sv.svu.var.lock)
	 {
	    ipdSetError(ipdERR_VAR_LOCKED, tn->node.sv.fullname);
	    return ipdFALSE;
	 }
      }
      else if (!(tn->type & ipdSECTION))
	 return ipdFALSE;

      iiFreeTreeNodeRec(tn, ipdFALSE);
      if (!ipdGetErrorStatus())
	 return ipdTRUE;
   }

   return ipdFALSE;
} /* iiDeleteItem */

/*-------------------------------------------------------------------------
 * Deletes an existing item.
 */
ipdBoolean ipdIteratorDelete(ipdIterator_t *iNode)
{
   if ((iNode) && (iNode->tn))
   {
      ipdResetErrorStatus();
      if (!(iiDeleteItem(iNode->tn)))
      {
	 if (!ipdGetErrorStatus())
	    ipdSetError(ipdERR_IPDNOT_FOUND,
			((iNode) && (iNode->tn)) ?
			iNode->tn->node.sv.fullname : "?itemname?");
	 return ipdFALSE;
      }
      return ipdTRUE;
   }
   return ipdFALSE;
} /* ipdIteratorDelete */

/*-------------------------------------------------------------------------
 * Deletes the item with the given name.
 */
ipdBoolean ipdDeleteByName(ipdConstString name /* item name to delete */
			 )
{
   ipdTreeNode_t *tn;
   ipdChar         *searchForName;

   ipdVeStart("ipdDeleteByName");

   if ((name))
   {
      ipdResetErrorStatus();
      searchForName = iiCreateName(name, NULL);
      if (ipdGetErrorStatus())
	 ipdVeReturn(ipdFALSE);
      tn = iiHashFindTreeNode(searchForName);
      ipdMBfree(searchForName);

      if (!(iiDeleteItem(tn)))
      {
	 if (!ipdGetErrorStatus())
	    ipdSetError(ipdERR_IPDNOT_FOUND, name);
	 ipdVeReturn(ipdFALSE);
      }
      ipdVeReturn(ipdTRUE);
   }
   ipdVeReturn(ipdFALSE);
} /* ipdDeleteByName */

/*=======================================================================
	EXTENSIONS FOR FILE-SPECIFIC TREE-IPDNODES
=======================================================================*/

ipdTreeNodeDll_t *ipdGetDllOfTreeNodesInFile(ipdConstString filename)
{
  /*ipdTreeNodeDll_t *temp_dll = NULL;*/ /* [patch] removed unused variable*/
  ipdTreeNodeDll_t *prev_dll = NULL;
  ipdTreeNodeDll_t *head_dll = NULL;
  ipdTreeNodeDll_t *tail_dll = NULL;
  ipdTreeNodeDll_t *i_dll = NULL;
  ipdTreeNodeDll_t *j_dll = NULL;
  
  ipdTreeNode_t    *tn = iiGetRoot();
  
  ipdIFL_t *searchfile = ipdBase->files;
  
  while (searchfile && (strcmp(filename, searchfile->fileName) != 0 ))
  {
    searchfile = searchfile->next;
  }

  if(searchfile == NULL)
  {
    printf("ERROR: FILE IPDNOT FOUND: %s\n", filename);
    return NULL;
  }
  
  while (tn)
  {
    if (tn->node.sv.file == searchfile /*|| !tn->node.sv.file*/)
    {
      if (!head_dll)
      {
        head_dll = iiCreateTnDll();
        tail_dll = head_dll;
        prev_dll = head_dll;
      }
      else
      {
        tail_dll = iiCreateTnDll();
        prev_dll->next = tail_dll;
        tail_dll->prev = prev_dll;
        prev_dll = tail_dll;
      }
      if (tail_dll)
      {
        tail_dll->tn = tn;
      }
    }
    if ((tn->type & ipdSECTION) && tn->node.sv.svu.sect.child)
    {
      tn = tn->node.sv.svu.sect.child;
      continue;
    }
    while (tn->next == NULL && tn != iiGetRoot())
    {
      tn = tn->node.sv.parent;  
    }  
    tn = tn->next;    
  }

  /*SORT IT*/
  for (j_dll = tail_dll; j_dll != head_dll; j_dll = j_dll->prev )
  {
    for (i_dll = head_dll; i_dll != j_dll ; i_dll = i_dll->next )
    {
      if (i_dll->tn->node.sv.lineno > i_dll->next->tn->node.sv.lineno )
      {
        tn = i_dll->tn;       
        i_dll->tn = i_dll->next->tn;        
        i_dll->next->tn = tn;       
      }
    }
  }
  
  return head_dll;
}






