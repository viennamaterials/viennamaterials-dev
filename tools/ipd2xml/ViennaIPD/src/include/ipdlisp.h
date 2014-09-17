/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

CONTENT:  LISP interface

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  RK   Robert Klima,      Institute for Microelectronics, TU Vienna

  who when        what
-------------------------------------------------------------------------
  RK  1998-08-26  new from scratch
=======================================================================*/

/** @file ipdlisp.h
    @brief Header file including declaration of all types and functions used for interfacing ViennaIPD with LISP.
*/

/*********** Note: Doxygen comments are found in the .c files! ***************/

/** @ingroup LISP */
/*@{*/

#ifndef IPDLISP_H
#define IPDLISP_H

/*#include "vbs.h"*/
#include "../core/include/verr.h" /* [patch] switched from vbs.h to verr.h, vbs.h isn't necessary */

#if defined(__cplusplus)
extern "C" {
#endif

/*********************************/
/*         Lisp Binding          */
/*********************************/

/************* Files *************/
ipdVoid           ipdSetEditorPrivileges(void);
ipdVoid           ipdUnsetEditorPrivileges(void);
ipdBoolean        ipdInitL(ipdConstString pathList,    /* path list    */
			 ipdConstString logFileName  /* logfile name */
			 );
ipdLong           ipdCreateFileL(ipdConstString name    /* Name of the inputdeck */
			       );
ipdLong           ipdLoadFileL(ipdConstString filename     /* file name */
			     );
ipdBoolean        ipdSetFileL(ipdLong index /* index of the base */
			    );
ipdBoolean        ipdMergeFileL(ipdConstString filename  /* name of the input deck file */
			      );

/************ Freeing ************/

ipdVoid           ipdFreeCurrentIpdL(void);
ipdVoid           ipdFreeAllL(void);

/************ Parsing ************/

ipdVoid           ipdParseStringL(ipdConstString str,       /* string to parse                    */
				ipdInt         *OK        /* 1 when correct parsed else 0 [SHo] */
				);

/*********** Get info ************/

ipdBoolean        ipdGetSectionListL(ipdConstString   sectName,     /* section fullname    */
				   ipdConstString **itemNames,    /* item names          */
				   ipdInt          *nItemNames,   /* length(itemNames)   */
				   ipdConstString **expressions,  /* expression strings  */
				   ipdInt          *nExpressions, /* length(expressions) */
				   ipdConstString **sections,     /* real sections       */
				   ipdInt          *nSections,    /* length(sections)    */
				   ipdConstString **comments,     /* comments            */
				   ipdInt          *nComments,    /* length(comments)    */
				   ipdInt         **itemKind,     /* item kind           */
				   ipdInt          *nItemKind,    /* length(itemKind)    */
				   ipdString      **itemFlags,    /* flags               */
				   ipdInt          *nItemFlags    /* length(itemFlags)   */
				   );
ipdBoolean        ipdGetItemInfoL(ipdConstString  itemName,   /* section fullname        */
				ipdBoolean      local,      /* TRUE to get local items too */
				ipdConstString *expression, /* expression string       */
				ipdConstString *section,    /* real section            */
				ipdConstString *comment,    /* comment                 */
				ipdInt         *itemKind,   /* item kind               */
				ipdString      *itemFlags   /* flags                   */
				);
ipdBoolean        ipdGetItemValueL(ipdConstString  itemName, /* section fullname */
				 ipdConstString *value     /* value            */
				 );
ipdBoolean        ipdGetAncestorsL(ipdConstString   sectName,      /* section fullname      */
				 ipdConstString **ancestorNames, /* ancestor names        */
				 ipdInt          *ancestorNum    /* length(ancestorNames) */
				 );

/*********** Changing ************/

ipdBoolean        ipdChangeAncestorsL(ipdConstString  sectionFullName,  /* section fullname       */
				    ipdConstString *ancestorRelNames, /* all new ancestor names */
				    ipdInt          nAncestorRelNames /* length(itemKind)       */
				    );
ipdBoolean        ipdChangeItemCommentL(ipdConstString itemName, /* section fullname */
				      ipdConstString comment   /* value            */
				      );

/********* Delete/Rename *********/

ipdBoolean        ipdRenameItemL(ipdConstString oldFullName, /* [I] old fullname of the item  */
			       ipdConstString newName      /* [I] new name              */
			       );
ipdBoolean        ipdDeleteItemL(ipdConstString name /* item name to delete */
			       );

/******** Error handling *********/

ipdBoolean        ipdGetErrorStatusL(ipdConstString *errorMessage /* error message */
				   );
ipdVoid           ipdResetErrorsL(void);

/******** IO Functions *****************/
ipdBoolean        ipdDumpAll2FileL(ipdConstString filename,  /* [I] path and filename                          */
				 ipdBoolean     onlyValues /* [I] dump only values or expressions (formulas) */
				 );

#if defined(__cplusplus)
}
#endif

/*=======================================================================
=======================================================================*/

#endif /* IPDLISP_H */

/*@}*/
