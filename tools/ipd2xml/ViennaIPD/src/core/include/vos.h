/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

/** @file vos.h
    @brief Interface to system-independent directory/file information.
*/

#ifndef	_IPDvosINCLUDED_
#define	_IPDvosINCLUDED_

#include "../../include/ipdtype.h"
#include "../../include/ipdtime.h"
#include "vdir.h"
#include "../../include/ipdstring.h"

/***TE constants with implemented system specifications */
typedef enum _ipdvossys_
{
   ipdvosUNKIPDNOWN_SYS = -1,
   ipdvosUNIX = 0,			/*** VOSPEC is valid for UNIX */
   ipdvosVMS = 1,			/*** VOSPEC is valid for VMS */
   ipdvosWINNT = 2,                /*** VOSPEC is valid for WindowsNT */
   ipdvosKIPDNOWN_SYS = 3
} ipdvosSys;


/* define a structure for holding all elements of a file/dir specification */
typedef struct _ipdvosFile_
{
   ipdConstString node;		/* host name */
   ipdConstString device;		/* device name */
   ipdConstString path;		/* path of file */
   ipdConstString name;		/* name of file */
   ipdConstString ext;		/* extension of file */
   ipdShort version;		/* version if available */
   ipdShort length;		/* length of buffer space */
   ipdvosSys systype;		/* system for which this VOSPEC is valid */
   ipdChar vosbuf[1];		/* actual buffer space */
} ipdvosFileStruct;
/* pointer to a file/dir specification */
typedef ipdvosFileStruct	*ipdvosFile;


/* access macros for reading */
#define ipdVosGetNode(ipdvosFile)	((ipdvosFile)->node)
#define ipdVosGetExt(ipdvosFile)	((ipdvosFile)->ext)
#define ipdVosGetPath(ipdvosFile)	((ipdvosFile)->path)
#define ipdVosGetName(ipdvosFile)	((ipdvosFile)->name)
#define ipdVosGetDevice(ipdvosFile)	((ipdvosFile)->device)
#define ipdVosGetSystype(ipdvosFile)	((ipdvosFile)->systype)

_BEGIN_CPLUSPLUS

/* implemented functions */
/* update some parts of a specification (new one allocated) */
IPDVBSEXTERN extern ipdvosFile ipdvosUpdateVospec(ipdvosFile vospec,
			       ipdConstString node,
			       ipdConstString device,
			       ipdConstString path,
			       ipdConstString name,
			       ipdConstString ext,
			       ipdLongWord flags); 
/* generate copy of file specification */
IPDVBSEXTERN extern ipdvosFile ipdvosCopyVospec(ipdvosFile vospec);
/* concat two specifications */
IPDVBSEXTERN extern ipdvosFile ipdvosSpecat(ipdvosFile vospec1,
			 ipdvosFile vospec2);
/* lower level directory */
IPDVBSEXTERN extern ipdvosFile ipdvosDownDirectory(ipdvosFile vospec,
				ipdConstString dirname,
				ipdBoolean hidden);			 
/* free a specification */
IPDVBSEXTERN extern ipdVoid ipdvosFreeVospec(ipdvosFile vospec);

/* conversion routines */
IPDVBSEXTERN extern ipdConstString ipdvosVos2Ospec(ipdvosFile vospec,
				 ipdLongWord flags);
IPDVBSEXTERN extern ipdvosFile ipdvosOs2Vospec(ipdConstString ospec,
			    ipdBoolean directory);

#define ipdvosOUTPUT_IPDNODE		(1 << 0)
#define ipdvosOUTPUT_DEVICE	(1 << 1)
#define ipdvosOUTPUT_PATH		(1 << 2)
#define ipdvosOUTPUT_NAME		(1 << 3)
#define ipdvosOUTPUT_EXT		(1 << 4)
#define ipdvosOUTPUT_VERSION	(1 << 5)
#define ipdvosOUTPUT_ALL	(ipdvosOUTPUT_IPDNODE | ipdvosOUTPUT_DEVICE | \
			 ipdvosOUTPUT_PATH | ipdvosOUTPUT_NAME | ipdvosOUTPUT_EXT | \
			 ipdvosOUTPUT_VERSION)
#define ipdvosOUTPUT_HOME		(1 << 8)
#define ipdvosOUTPUT_IPDNOEXTSEP	(1 << 9)
#define ipdvosOUTPUT_IPDNODIRSEP	(1 << 10)			 

#define ipdvosUPDATE_IPDNODE		ipdvosOUTPUT_IPDNODE
#define ipdvosUPDATE_DEVICE	ipdvosOUTPUT_DEVICE
#define ipdvosUPDATE_PATH		ipdvosOUTPUT_PATH
#define ipdvosUPDATE_NAME		ipdvosOUTPUT_NAME
#define ipdvosUPDATE_EXT		ipdvosOUTPUT_EXT
#define ipdvosUPDATE_ALL		ipdvosOUTPUT_ALL
#define ipdvosUPDATE_DIR		vosOUTPUT_DIR
#define ipdipdvosUPDATE_NAMEXT	ipdvosOUTPUT_NAMEXT

/* some special path elements */
#define ipdvosEND_PATH	(ipdConstString)0x0
#define ipdvosTOP_PATH	(ipdConstString)0x1
#define ipdvosTHIS_PATH	(ipdConstString)0x2
#define ipdvosUP_PATH	(ipdConstString)0x3

/* call for each path element */
/* check for thop path */
IPDVBSEXTERN extern ipdBoolean ipdvosIsTopPath(ipdvosFile vospec);

/* get pathes from a system dependent pathlist */
IPDVBSEXTERN extern ipdvosFile ipdvosGetPathFromPathList(ipdConstString pathlist,
				      ipdLong *pos);

/* [JW] this free function takes care of the allocated memory (mostly strings ..). */
IPDVBSEXTERN extern ipdVoid ipdFreeVosUser(voidArg);
IPDVBSEXTERN extern ipdVoid ipdFreeVosConv(voidArg);

/* get the current working directory */
IPDVBSEXTERN extern ipdvosFile ipdvosGetCurrentDirectory(voidArg);
/* check access of file */
IPDVBSEXTERN extern ipdBoolean ipdvosAccessFile(ipdvosFile file, ipdLongWord mode);
/* check for directory type */
IPDVBSEXTERN extern ipdBoolean ipdvosIsDirectory(ipdvosFile vospec);

IPDVBSEXTERN extern ipdConstString ipdGetHomeDirectory(voidArg);
IPDVBSEXTERN extern ipdConstString ipdGetHomeDirectoryByUserName(ipdConstString username);

IPDVBSEXTERN extern ipdChar ipdvosGetListSeparator(voidArg);
IPDVBSEXTERN extern ipdString ipdvosGetEnvironment(ipdConstString env);
_END_CPLUSPLUS

#endif	/* _vosINCLUDED_ */
