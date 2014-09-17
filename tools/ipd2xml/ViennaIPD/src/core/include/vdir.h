/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/


/** @file vdir.h
    @brief Interface to filesystem-independent directory information
*/

#ifndef	_IPDvdirINCLUDED_
#define	_IPDvdirINCLUDED_

#if defined(unix) || defined(__unix)
#  include <fcntl.h>		/* constants for file access */
#  include <sys/stat.h>		/* modes for file creation */

#  if defined(_AIX)
#    include <sys/statfs.h>	/* for file system information */
#  else
#  if (defined(__sparc) && ipdSystemV) || defined(__sgi) || defined(__OPENNT)
#    include <sys/statvfs.h>	/* for file system information */
#  else
#    include <sys/mount.h>	/* for file system information */
#    if defined(hpux) || defined(__hpux) || defined(__sparc) || defined(__sony) || defined(__linux) || defined(__CYGWIN__)
#      include <sys/vfs.h>	/* for file system information */
#    endif /* defined(hpux) || defined(__hpux) || defined(sparc) || defined(__sony) || defined(__linux) || defined(__CYGWIN__) */
#  endif /* (defined(__sparc) && ipdSystemV) || defined(__sgi) || defined(__OPENNT) */
#  endif /* defined(_AIX) */

#  include <unistd.h>
#  include <utime.h>

   _BEGIN_CPLUSPLUS
   extern ipdConstString ipdTruncateSpec(ipdConstString spec);
#  define ipdTranslateLogical(logical)		ipdNULL
#  define ipdTranslateLogicalList(logical,num)	ipdNULL
   _END_CPLUSPLUS
#endif /* defined(unix) || defined(__unix) */




#include "../../include/ipdtype.h"
#include "../../include/ipdtime.h"



_BEGIN_CPLUSPLUS

#if !defined(_IPD_S_IFMT) && defined(S_IFMT)
#  define _IPD_S_IFMT  S_IFMT       /* type of file */
#  endif /* !defined(_IPD_S_IFMT) && defined(IPD_S_IFMT) */

typedef enum _ipddircode_ {
   ipdIS_FILE, ipdIS_DIRECTORY, ipdIS_SPECIAL, ipdIS_IPDNONEXIST
} ipdvDirCode;

/* fast check for directory */
extern ipdvDirCode ipdvIsDirectory(ipdConstString path);
/*** reads out some informations about the file {\bf file} into the buffer
    {\bf buf} which is of the type {\bf ipdStatStruct}. To
    access the fields use the corresponding macros. Returns ipdTRUE on
    success. */
#if !defined(WINNT)
typedef struct stat ipdStatStruct;
#else
typedef struct _stat ipdStatStruct;
#endif /* !defined(WINNT) */

extern ipdBoolean       ipdvStatFile(ipdConstString filename, ipdStatStruct *buf);
extern ipdConstString   ipdGetDirectory(voidArg);
extern ipdBoolean       ipdChangeDirectory(ipdConstString);
extern ipdVoid          ipdClearDirectoryCache(voidArg);

#define ipdStatFileType(stat)  ((stat).st_mode & _IPD_S_IFMT)

#define ipdProtectionReadBit	(1 << 0)
#define ipdProtectionWriteBit	(1 << 1)
#define ipdProtectionExecuteBit	(1 << 2)
#define ipdProtectionDeleteBit	(1 << 3)

#define ipdUserProtectionShift	0

#define ipdUserReadProtection	(ipdProtectionReadBit << ipdUserProtectionShift)

#define ipdProtectionMask	((ipdLongWord)0xff)
#define ipdUserProtectionMask	(ipdProtectionMask << ipdUserProtectionShift)

/*** check for file access allowed */
IPDVBSEXTERN extern ipdBoolean ipdipdFileAccess(ipdConstString, ipdLongWord);

_END_CPLUSPLUS

#endif	/* _vdirINCLUDED_ */
