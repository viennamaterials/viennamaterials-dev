/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

CONTENT:  Lisp bindings

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  RK   Robert Klima,      Institute for Microelectronics, TU Vienna
          OB   Oskar Baumgartner, Institute for Microelectronics, TU Vienna

  who when        what
-------------------------------------------------------------------------
  RK  1998-04-14  new from scratch
  OB  2007-09-04  fixed bug that led to failure after ipdParseStringL
=======================================================================*/

/** @file ipdlisp.c
    @brief Definitions and bindings for interfacing LISP
*/

/*=======================================================================
	Includes
=======================================================================*/

#include "ipdP.h"
#include "ipdlisp.h"

/*=======================================================================
	Functions Declarations
=======================================================================*/

int           yyparse (void *);
void         *yy_scan_string(const char *);
void          yyrestart( FILE *input_file );  /* [patch] removed warning: added forward declaration */

/*=======================================================================
	Global variables
=======================================================================*/

static char buffer[2000];

#define IPD_STR_RESET_ERROR "\n==================== IPD usage error: ==================\nError status has not been reset.\n"
/*=======================================================================
  C code
=======================================================================*/

/*
    Dumps the expression of the given varable to the string given.
    Produces IPL compatible output.
    If iNode is set (iterator) iNode will be dumped. If not and if tn is set
    tn will be dumped.
    Uses vMalloc();

    iNode: an ipd iterator
    tn: a tree node
    return: the expression string
 */
ipdConstString ipdIteratorGetExpression(ipdIterator_t *iNode,
				      ipdTreeNode_t *tn)
{
   buffer[0] = '\0';

   if (!(iNode) && !(tn))
      return ipdNULL;
   if ((iNode))
   {
      if ((iNode->tn))
	 tn = iNode->tn;
      else
	 return ipdNULL;
   }
   if (tn->type & ipdSECTION)
      return ipdNULL;
   if (tn->type & ipdVARIABLE)
      tn = tn->node.sv.svu.var.tree;

   if ((tn))
      ipdDump2String(buffer, tn, ipdFALSE, ipdFALSE);

   return ipdvStrdup(buffer);
} /* ipdIteratorGetExpression */

/*  Returns the name of the parent section. Uses vMalloc();
    iNode: an ipd iterator
    tn: a tree node
    return: string identifying the parent section
 */
ipdConstString ipdIteratorGetParentSection(ipdIterator_t *iNode,
					 ipdTreeNode_t *tn)
{
   if (!(iNode) && !(tn))
      return ipdNULL;
   if ((iNode))
   {
      if ((iNode->tn))
	 tn = iNode->tn;
      else
	 return ipdNULL;
   }

   if (!(tn->type & (ipdSECTION | ipdVARIABLE)))
      return ipdNULL;

   return ipdvStrdup(tn->node.sv.parent->node.sv.name);
} /* ipdIteratorGetParentSection */

/** @brief Creates a list of all names of the sections in the btree.
    @param sanc A tree node
    @param names Pointer to a list of names
    @param num Number of sections
    @return ipdTRUE if successful
 */
static ipdBoolean iiGetAllSectionNames(ipdTreeNode_t  *sanc,
				     ipdConstString  **names,
				     ipdInt           *num)
{
   char       **p;

   if ((sanc))
   {
      *num = ipdListLen(sanc);
      if (!(p = (char **)ipdvCalloc((unsigned long)*num, sizeof(char *))))
      {
	 ipdSetError(ipdERR_IPDNO_MEMORY);
	 *num = 0;
	 return ipdFALSE;
      }
      *names = (ipdConstString *)p;
      while ((sanc))
	 {
	 if (!(*p++ = (char *)ipdvStrdup((char *)sanc->node.sectRef.name)))
	    {
	       ipdFreeNameList(*names, *num);
	       *names = ipdNULL;
	       *num   = 0;
	       return ipdFALSE;
	    }
	 sanc = sanc->next;
      }
   }
   else
   {
      *num   = 0;
      *names = ipdNULL;
   }
   return ipdTRUE;
} /* iiGetAllSectionNames */

/* Creates a list of all names of the ancestors of the given section.
   iNode: an ipd iterator
   tn: a tree node
   ancestorNames: pointer to a list of ancestor names
   ancestorNum: pointer to the returned number of ancestors
   return: ipdTRUE if successful
 */
ipdBoolean ipdIteratorGetAncestors(ipdIterator_t   *iNode,
				 ipdTreeNode_t   *tn,
				 ipdConstString   **ancestorNames,
				 ipdInt            *ancestorNum)
{
   if ((!(iNode) && !(tn)) ||
       !(ancestorNames) ||
       !(ancestorNum))
      return ipdFALSE;
   if ((iNode))
   {
      if ((iNode->tn))
	 tn = iNode->tn;
      else
	 return ipdFALSE;
   }

   if (!(tn->type & ipdSECTION))
      return ipdFALSE;

   return iiGetAllSectionNames(tn->node.sv.svu.sect.ancestor,
			     ancestorNames,
			     ancestorNum);
} /* ipdIteratorGetAncestors */

/* Frees a list of names.
   names: a list of names
   num: Number of names
 */
void ipdFreeNameList(ipdConstString *names,
		     ipdInt          num)
{
   if ((names))
   {
      while (num--)
	 if ((names[num]))
	    ipdvFree(names[num]);
      ipdvFree(names);
   }
} /* ipdFreeNameList */

/* Frees the arrays allocated in ipdLGetSection(). */
void ipdFreeSectionList(ipdInt          itemNum,
			ipdConstString *itemNames,
			ipdConstString *expressions,
			ipdConstString *sections,
			ipdConstString *comments,
			ipdInt         *itemKind,
			ipdString      *itemFlags
			)
{
   if (itemNum)
   {
      if ((itemNames))
	 ipdFreeNameList(itemNames,   itemNum);
      if ((expressions))
	 ipdFreeNameList(expressions, itemNum);
      if ((sections))
	 ipdFreeNameList(sections,    itemNum);
      if ((comments))
	 ipdFreeNameList(comments,    itemNum);
      if ((itemKind))
	 ipdvFree(itemKind);
      if ((itemFlags))
	 ipdFreeNameList((ipdConstString *)itemFlags,   itemNum);
   }
} /* ipdFreeSectionList */

/*=======================================================================
	LISP Interface Functions
=======================================================================*/

/*********************************/
/************* Files *************/
/*********************************/

/** @brief Sets the input deck editor privileges. For instance variables may be overwritten now. */
ipdVoid ipdSetEditorPrivileges()
{
   ipdEditorPrivileges = ipdTRUE;
} /* ipdSetEditorPrivileges */

/** @brief Unsets the input deck editor privileges. For instance variables cannot be overwritten now. */
ipdVoid ipdUnsetEditorPrivileges()
{
   ipdEditorPrivileges = ipdFALSE;
} /* ipdUnsetEditorPrivileges */

/** @brief Loads an input deck from a specified file.

   Does the same like ipdInit, except the log file is specified by a
   string. 
   - Creates a New Inputdeck
   - Initializes the InputDeck-internal dataBase

    @param pathList Path list
    @param logFileName Name of the log file
    @return If the file specified by logFileName can't be opened or another
   error occures, ipdFALSE will be returned.
*/
ipdBoolean ipdInitL(ipdConstString pathList, ipdConstString logFileName)
{
   FILE *file;

   ipdVeStart("ipdInitL");

   if (ipdGetErrorStatus())
   {
      ipdStdLog(IPD_STR_RESET_ERROR);
      ipdVeReturn(ipdFALSE);
   }
   if ((logFileName))
   {
      if (!(file = fopen(logFileName, "w")))
      {
	 ipdSetError(ipdERR_LOGFILE, logFileName);
	 ipdVeReturn(ipdFALSE);
      }
   }
   else
      file = NULL;

   ipdVeReturn((ipdInit(pathList, file)) ?  ipdTRUE : ipdFALSE);
} /* ipdInitL */

/** @brief Creates an empty new input deck. 
    @param name Name of the inputdeck
    @return Status integer
*/
ipdLong ipdCreateFileL(ipdConstString name)
{
   ipdVeStart("ipdCreateFileL");

   if (ipdGetErrorStatus())
   {
      ipdStdLog(IPD_STR_RESET_ERROR);
      ipdVeReturn(0);
   }

   ipdVeReturn(ipdCreateBase(name, 0));
} /* ipdCreateFileL */

/** @brief Loads an input deck from a specified file.
    @param filename The file name of the .ipd file
    @return Status integer
 */
/***TF [:lisp-name ipd::load-file] */
ipdLong ipdLoadFileL(ipdConstString filename)
{
  ipdLong index;
 
  ipdVeStart("ipdLoadFileL");

   if (ipdGetErrorStatus())
   {
      ipdStdLog(IPD_STR_RESET_ERROR);
      ipdVeReturn(0);
   }

  index = ipdCreateBase(filename, 0);

  if (!ipdReadInputDeck ((char *)filename))
  {
    ipdFreeCurrentIpd();
    if (!ipdGetErrorStatus())
       ipdSetError(ipdERR_READ_INPUTDECK, filename);
    ipdVeReturn(0);
  }

  ipdVeReturn(index);
} /* ipdLoadFileL */

/** @brief Switches to another input deck database.
    @param index Index of the database
 */
ipdBoolean ipdSetFileL(ipdLong index)
{
  ipdVeStart("ipdSetFileL");

   if (ipdGetErrorStatus())
   {
      ipdStdLog(IPD_STR_RESET_ERROR);
      ipdVeReturn(ipdFALSE);
   }

  ipdVeReturn(ipdSetBase(index));
} /* ipdSetFileL */

/** @brief Merges an file into the CURRENT input deck. 
    @param filename Name of the input deck file
*/
/***TF [:lisp-name ipd::merge-file] */
ipdBoolean ipdMergeFileL(ipdConstString filename)
{
   ipdVeStart("ipdMergeFileL");

   if (ipdGetErrorStatus())
   {
      ipdStdLog(IPD_STR_RESET_ERROR);
      ipdVeReturn(ipdFALSE);
   }

   if (!ipdReadInputDeck ((char *)filename))
   {
      ipdFreeCurrentIpd();
      if (!ipdGetErrorStatus())
	 ipdSetError(ipdERR_READ_INPUTDECK, filename);
      ipdVeReturn(ipdFALSE);
   }

   ipdVeReturn(ipdTRUE);
} /* ipdMergeFileL */

/*********************************/
/************ Freeing ************/
/*********************************/

/** @brief Frees the CURRENT input deck. A new must be set afterwars be means of ipdSetFileL() */
/***TF [:lisp-name ipd::free-current-file] */
ipdVoid ipdFreeCurrentIpdL()
{
   ipdVeStart("ipdFreeCurrentIpdL");

   if (ipdGetErrorStatus())
   {
      ipdStdLog(IPD_STR_RESET_ERROR);
      ipdVeReturnVoid;
   }

   ipdFreeCurrentIpd();

   ipdVeReturnVoid;
} /* ipdFreeCurrentIpdL */

/** @brief Frees everything. */
ipdVoid ipdFreeAllL()
{
   ipdVeStart("ipdFreeAllL");

   if (ipdGetErrorStatus())
   {
      ipdStdLog(IPD_STR_RESET_ERROR);
      ipdVeReturnVoid;
   }

   ipdFreeAll();

   ipdVeReturnVoid;
} /* ipdFreeAllL */

/*********************************/
/************ Parsing ************/
/*********************************/

/** @brief Parses the string given.
    @param str string to parse
    @param OK 1 when parsed correctly, otherwise 0
 */
ipdVoid ipdParseStringL(ipdConstString str, ipdInt *OK)
{
   char *CSFullName;
   extern FILE *yyin /* , *yyout */ ;

   ipdVeStart("ipdParseStringL");

   *OK = 0;

   if (ipdGetErrorStatus())
   {
      ipdStdLog(IPD_STR_RESET_ERROR);
      ipdVeReturnVoid;
   }

   /* Set string to parse. */
   yyin = NULL;
   yy_scan_string((const char *)str);

   /* Remember current section. */
   CSFullName             = ipdBase->CSFullName;
   ipdBase->CSFullName    = NULL;

   /* Set current section. */
   iiSetCS(iiGetRoot());

   /* Reset before parsing. */
   ipdPrivate.parsing     = ipdTRUE;
   ipdParameterList       = NULL;
   ipdPrivate.scannedFile = NULL;

   /* Reset the parser. */
   tokenpos           = 0;
   ipdLineNo          = 1;
   currentComment     = NULL;
   ipdIncludeStackPtr = 0;
   commentNewLine     = 0;

   /* Allow fullname as lvalues */
   ipdBase->bFlags |= ipdBF_PARSE_FULLNAMES;

   /* Allow to overwrite ext variables by parsing */
   ipdApplicationSet = ipdTRUE;
   /* Start parsing. */
   yyparse(NULL);
   
   /* [OB] BUGFIX: YY_BUFFER_STATE needs reset after using yy_scan_string */
   /* otherwise it would prevent to parse new file; */
   yyrestart(NULL); 
   
   ipdApplicationSet = ipdFALSE;
   if (ipdGetErrorStatus())
      printf("!! ERROR while parsing\n");
   /* Applicaton may set/overwrite ext variables */

   /* Reset after parsing. */
   ipdBase->bFlags &= ~ipdBF_PARSE_FULLNAMES;
   ipdPrivate.parsing     = ipdFALSE;
   ipdPrivate.scannedFile = NULL;
   ipdBase->calcVar       = NULL;

   /* Restore current section. */
   iiSetCS(iiGetRoot()); /* startUp: points to root */
   ipdBase->CSFullName    = CSFullName;
   ipdBase->calcVar       = NULL;

   if (!ipdGetErrorStatus())
      *OK = 1;

   ipdVeReturnVoid;
} /* ipdParseStringL */

/*********************************/
/*********** Get info ************/
/*********************************/

/** @brief Gets information on all items within a section, where the sectionname must be given.

itemNames, expressions, sections, itemKind contain arrays of
the name of the items, the expression dumped as a string,
the name of the section it really is located in, the kind of the
item (0...variable, 1...section). Free allocated memory with ipdFreeSectionList()

    @param sectName section fullname   
    @param itemNames item names
    @param nItemNames length(itemNames)
    @param expressions expression strings
    @param nExpressions length(expressions)
    @param sections real sections 
    @param nSections length(sections) 
    @param comments comments
    @param nComments length(comments)
    @param itemKind item kind 
    @param nItemKind length(itemKind)
    @param itemFlags flags
    @param nItemFlags length(itemFlags)
    @return ipdTRUE if everything is ok. ipdFALSE --> Error!
 */
#define ipdFLAG_NUMBER 6
/***TF [:lisp-name ipd::get-section-list] */
ipdBoolean ipdGetSectionListL(ipdConstString   sectName,     /* [I]  section fullname        */
			    ipdConstString **itemNames,    /* [OA] item names              */
			    ipdInt          *nItemNames,   /* [O :der length(itemNames)]   */
			    ipdConstString **expressions,  /* [OA] expression strings      */
			    ipdInt          *nExpressions, /* [O :der length(expressions)] */
			    ipdConstString **sections,     /* [OA] real sections           */
			    ipdInt          *nSections,    /* [O :der length(sections)]    */
			    ipdConstString **comments,     /* [OA] comments                */
			    ipdInt          *nComments,    /* [O :der length(comments)]    */
			    ipdInt         **itemKind,     /* [OA] item kind               */
			    ipdInt          *nItemKind,    /* [O :der length(itemKind)]    */
			    ipdString      **itemFlags,    /* [OA] flags                   */
			    ipdInt          *nItemFlags    /* [O :der length(itemFlags)]   */
			    )
{
   ipdIterator_t   *iNode = NULL;
   ipdBoolean         ok = ipdTRUE;
   long             index = 0;

   ipdVeStart("ipdGetSectionListL");

   if (!(sectName)    ||
       !(itemNames)   || !(nItemNames)   ||
       !(expressions) || !(nExpressions) ||
       !(sections)    || !(nSections)    ||
       !(comments)    || !(nComments)    ||
       !(itemKind)    || !(nItemKind)    ||
       !(itemFlags)   || !(nItemFlags)
       )
   {
      ipdStdLog("\nInvalid parameters to \"ipd::get-section-list\"\n");
      ipdVeReturn(ipdFALSE);
   }

   *itemNames    = ipdNULL;
   *nItemNames   = 0;
   *expressions  = ipdNULL;
   *nExpressions = 0;
   *sections     = ipdNULL;
   *nSections    = 0;
   *comments     = ipdNULL;
   *nComments    = 0;
   *itemKind     = ipdNULL;
   *nItemKind    = 0;
   *itemFlags    = ipdNULL;
   *nItemFlags   = 0;

   if (ipdGetErrorStatus())
   {
      ipdStdLog(IPD_STR_RESET_ERROR);
      ipdVeReturn(ipdFALSE);
   }

   if (!ipdIteratorNewByName(&iNode, sectName, ipdSECTION, ipdANY))
      ipdVeReturn(ipdFALSE);

   if (!(ok = iiIteratorDoStepFlags(iNode, ipdVF_NOFLAG)))
   {
      ipdIteratorFree(iNode);
      if (ipdGetErrorStatus())
	 ipdVeReturn(ipdFALSE);
      ipdVeReturn(ipdTRUE);
   }

   *nItemNames = *nExpressions = *nSections =
      *nComments = *nItemKind = *nItemFlags = ipdListLen(iNode->tnDll);

   if (!(*itemNames   = (ipdConstString *)ipdvCalloc((unsigned long)*nItemNames,   sizeof(ipdConstString))))
      goto ipdLGetSectionError;
   if (!(*expressions = (ipdConstString *)ipdvCalloc((unsigned long)*nExpressions, sizeof(ipdConstString))))
      goto ipdLGetSectionError;
   if (!(*sections    = (ipdConstString *)ipdvCalloc((unsigned long)*nSections,    sizeof(ipdConstString))))
      goto ipdLGetSectionError;
   if (!(*comments    = (ipdConstString *)ipdvCalloc((unsigned long)*nComments,    sizeof(ipdConstString))))
      goto ipdLGetSectionError;
   if (!(*itemKind    = (ipdInt *)        ipdvCalloc((unsigned long)*nItemKind,    sizeof(ipdInt))))
      goto ipdLGetSectionError;
   if (!(*itemFlags   = (ipdString *)     ipdvCalloc((unsigned long)*nItemFlags,   sizeof(ipdString))))
      goto ipdLGetSectionError;
   while (ok)
   {
      if ((iNode->tn))
      {
	 /* printf("%s\n", iNode->tn->node.sv.fullname); */
	 /* Copy item name. */
	 (*itemNames)[index] = ipdvStrdup(iNode->tn->node.sv.name);
	 (*itemFlags)[index] = (ipdString)ipdvMalloc(ipdFLAG_NUMBER);
	 if (iNode->tn->type & ipdVARIABLE)
	 {
	    /* Copy expression. */
	    (*expressions)[index]  = ipdIteratorGetExpression(iNode, NULL);
	    /* Set item kind. */
	    (*itemKind)[index]        = 0;
	    /* Copy item flags. */
	    if (iNode->tn->node.sv.svu.var.flags & ipdVF_AUXILIARY)
	       (*itemFlags)[index][0] = 'a';
	    else if (iNode->tn->node.sv.svu.var.flags & ipdVF_EXTERN)
	       (*itemFlags)[index][0] = 'e';
	    else
	       (*itemFlags)[index][0] = ' ';
	    (*itemFlags)[index][1] = ' ';
	    if (iNode->tn->node.sv.svu.var.flags & ipdVF_TRACE)
	       (*itemFlags)[index][2] = 't';
	    else
	       (*itemFlags)[index][2] = ' ';
	    (*itemFlags)[index][3]    = '\0';
	 }
	 else
	 {
	    /* Copy expression. */
	    (*expressions)[index]   = ipdNULL;
	    /* Set item kind. */
	    (*itemKind)[index]      = 1;
	    /* Copy item flags. */
	    if (iNode->tn->node.sv.svu.sect.flags & ipdSF_EXTENDABLE)
	       (*itemFlags)[index][0] = 'x';
	    else
	       (*itemFlags)[index][0] = ' ';
	    (*itemFlags)[index][1]    = ' ';
	    if (iNode->tn->node.sv.svu.sect.flags & ipdSF_IS_BASESECTION)
	       (*itemFlags)[index][2] = 'b';
	    else
	       (*itemFlags)[index][2] = ' ';
	    (*itemFlags)[index][3] = '\0';
	 }
	 /* Copy current section name. */
	 (*sections)[index]    = ipdvStrdup(iNode->tn->node.sv.parent->node.sv.fullname);
	 /* Copy comment. */
	 if ((iNode->tn->comment))
	    (*comments)[index] = ipdvStrdup(iNode->tn->comment);
	 else
	    (*comments)[index] = ipdNULL;

	 if (ipdGetErrorStatus())
	    goto ipdLGetSectionError;
      }
      else
      {
	 if (!(ipdGetErrorStatus()))
	    ipdSetError(ipdERR_UNKIPDNOWN);
	 goto ipdLGetSectionError;
      }
      ok = iiIteratorDoNextFlags(iNode, ipdVF_NOFLAG);
      if (ipdGetErrorStatus())
	 goto ipdLGetSectionError;
      index++;
   }
   ipdIteratorFree(iNode);
   ipdVeReturn(ipdTRUE);

 ipdLGetSectionError:
   ipdFreeSectionList(*nItemNames,
		      *itemNames,
		      *expressions,
		      *sections,
		      *comments,
		      *itemKind,
		      *itemFlags
		      );

   if ((iNode))
      ipdIteratorFree(iNode);

   *itemNames    = ipdNULL;
   *nItemNames   = 0;
   *expressions  = ipdNULL;
   *nExpressions = 0;
   *sections     = ipdNULL;
   *nSections    = 0;
   *comments     = ipdNULL;
   *nComments    = 0;
   *itemKind     = ipdNULL;
   *nItemKind    = 0;
   *itemFlags    = ipdNULL;
   *nItemFlags   = 0;

   ipdVeReturn(ipdFALSE);
} /* ipdGetSectionListL */

/** @brief Get the infos like ipdGetSectionListL() but only for a known item.
    @param itemName section fullname   
    @param local set to ipdTRUE if you also want to get local items
    @param expression expression string
    @param section real section
    @param comment comment
    @param itemKind item kind 
    @param itemFlags flags
    @return ipdTRUE if everything is ok. ipdFALSE --> Error!
 */
ipdBoolean ipdGetItemInfoL(ipdConstString  itemName,   /* [I]  section fullname        */
			 ipdBoolean      local,      /* [I]  TRUE to get local items too */
			 ipdConstString *expression, /* [OA] expression string       */
			 ipdConstString *section,    /* [OA] real section            */
			 ipdConstString *comment,    /* [OA] comment                 */
			 ipdInt         *itemKind,   /* [O]  item kind               */
			 ipdString      *itemFlags   /* [OA] flags                   */
			 )
{
   char          *varName = (char *)itemName;
   ipdTreeNode_t *tn;

   ipdVeStart("ipdGetItemInfoL");

   if (!(itemName)   ||
       !(expression) ||
       !(section)    ||
       !(comment)    ||
       !(itemKind)   ||
       !(itemFlags)
       )
      ipdVeReturn(ipdFALSE);

   if (ipdGetErrorStatus())
   {
      ipdStdLog(IPD_STR_RESET_ERROR);
      ipdVeReturn(ipdFALSE);
   }

   /* Find variable with the name 'varname'.
      iiFindItemByName returns a new (malloc'd) valid
      fullname of the variable. */
   if (local)
   {
      if (!(tn = iiFindItemByName(&varName, ipdANY, ipdTRUE)))
      {
	 if (!ipdGetErrorStatus())
	    ipdSetError(ipdERR_VAR_IPDNOT_FOUND, itemName);
	 ipdVeReturn(ipdFALSE);
      }
   }
   else
   {
      if (!(varName = iiCreateName(itemName, NULL)))
	 ipdVeReturn(ipdFALSE);
      tn = NULL;
      iiGetInheritedItemRec(&tn, /* Find the variable behind */
			    varName,
			    NULL,
			    ipdFALSE,
			    NULL);
      if (!(tn))
      {
	 if (!ipdGetErrorStatus())
	    ipdSetError(ipdERR_IPDNO_SHADOW_INFO, itemName);
	 ipdMBfree(varName);
	 ipdVeReturn(ipdFALSE);
      }
   }

   /* Copy the sections name. */
   if (!(*section = ipdvStrdup(tn->node.sv.parent->node.sv.fullname)))
   {
      ipdMBfree(varName);
      ipdVeReturn(ipdFALSE);
   }

   /* Free the name malloc'd in iiFindItemByName(). */
   ipdMBfree(varName);

   if (!(*itemFlags = (ipdString)ipdvMalloc(ipdFLAG_NUMBER)))
      goto ipdLGetItemInfoError;

   if (tn->type & ipdVARIABLE)
   {
      /* Copy expression. */
      *expression        = ipdIteratorGetExpression(NULL, tn);
      /* Set item kind. */
      *itemKind          = 0;
      /* Copy item flags. */
      if (tn->node.sv.svu.var.flags & ipdVF_AUXILIARY)
	 (*itemFlags)[0] = 'a';
      else if (tn->node.sv.svu.var.flags & ipdVF_EXTERN)
	 (*itemFlags)[0] = 'e';
      else
	 (*itemFlags)[0] = ' ';
      (*itemFlags)[1]    = ' ';
      if (tn->node.sv.svu.var.flags & ipdVF_TRACE)
	 (*itemFlags)[2] = 't';
      else
	 (*itemFlags)[2] = ' ';
      (*itemFlags)[3]    = '\0';
   }
   else
   {
      /* Copy expression. */
      *expression        = ipdNULL;
      /* Set item kind. */
      *itemKind          = 1;
      /* Copy item flags. */
      if (tn->node.sv.svu.sect.flags & ipdSF_EXTENDABLE)
	 (*itemFlags)[0] = 'x';
      else
	 (*itemFlags)[0] = ' ';
      (*itemFlags)[1]    = ' ';
      if (tn->node.sv.svu.sect.flags & ipdSF_IS_BASESECTION)
	 (*itemFlags)[2] = 'b';
      else
	 (*itemFlags)[2] = ' ';
      (*itemFlags)[3]    = '\0';
   }
   /* Copy comment. */
   if ((tn->comment))
      *comment           = ipdvStrdup(tn->comment);
   else
      *comment           = ipdNULL;

   if (ipdGetErrorStatus())
      goto ipdLGetItemInfoError;

   ipdVeReturn(ipdTRUE);

 ipdLGetItemInfoError:
   if ((expression))
      ipdvFree(expression);
   if ((section))
      ipdvFree(section);
   if ((comment))
      ipdvFree(comment);
   if ((itemFlags))
      ipdvFree(itemFlags);

   *expression = ipdNULL;
   *section    = ipdNULL;
   *comment    = ipdNULL;
   *itemFlags  = ipdNULL;

   ipdVeReturn(ipdFALSE);
} /* ipdGetItemInfoL */

#undef ipdFLAG_NUMBER

/** @brief Gets the value of a variable.
    @param itemName Full name of the section
    @param value The value of the variable
    @return ipdTRUE if everything is ok. ipdFALSE --> Error!
 */
ipdBoolean ipdGetItemValueL(ipdConstString  itemName, /* [I]  section fullname */
			  ipdConstString *value     /* [OA] value            */
			  )
{
   ipdTreeNode_t *tn;

   ipdVeStart("ipdGetItemValueL");

   if (ipdGetErrorStatus())
   {
      ipdStdLog(IPD_STR_RESET_ERROR);
      *value = NULL;
      ipdVeReturn(ipdFALSE);
   }

   if ((tn = ipdEvalByName(itemName, ipdTRUE)))
   {
      ipdDump2String(buffer, tn, ipdFALSE, ipdTRUE);
      if (!(*value = ipdvStrdup(buffer)))
	 ipdVeReturn(ipdFALSE);
   }
   else
      *value = ipdNULL;

   ipdVeReturn(ipdTRUE);
} /* ipdGetItemValueL */

/** @brief Get all ancestors of the section with the name sectName.
    @param sectName Full name of the section
    @param ancestorNames Pointer to memory where ancestor names should be stored
    @param ancestorNum Pointer to an integer where the number of ancestors should be stored
    @return ipdTRUE if everything is ok. ipdFALSE --> Error!
 */
ipdBoolean ipdGetAncestorsL(ipdConstString   sectName,      /* [I]  section fullname          */
			  ipdConstString **ancestorNames, /* [OA] ancestor names            */
			  ipdInt          *ancestorNum    /* [O :der length(ancestorNames)] */
			  )
{
   char          *realName = (char *)sectName;
   ipdTreeNode_t *tn;
   ipdBoolean       retVal;

   ipdVeStart("ipdGetAncestorsL");

   if (ipdGetErrorStatus())
   {
      ipdStdLog(IPD_STR_RESET_ERROR);
      ipdVeReturn(ipdFALSE);
   }

   *ancestorNum = 0;
   if (!(tn = iiFindItemByName(&realName, ipdSECTION, ipdFALSE)))
   {
      ipdSetError(ipdERR_SECT_IPDNOT_FOUND, sectName);
      ipdVeReturn(ipdFALSE);
   }
   retVal = (ipdBoolean)ipdIteratorGetAncestors(ipdNULL,
					      tn,
					      ancestorNames,
					      ancestorNum);
   ipdMBfree(realName);

   ipdVeReturn(retVal);
} /* ipdGetAncestorsL */

/*********************************/
/*********** Changing ************/
/*********************************/

/** @brief Sets new ancestors of a specified section.
    @warning !!!That's a ugly dirty hack. Please beware and stay away!!!

    @param sectionFullName Full name of the section
    @param ancestorRelNames All new ancestor names
    @param nAncestorRelNames Length of the array ancestorRelNames
    @return ipdTRUE if everything is ok. ipdFALSE --> Error!
*/
ipdBoolean ipdChangeAncestorsL(ipdConstString  sectionFullName,  /* [I]  section fullname       */ 
			     ipdConstString *ancestorRelNames, /* [IA] all new ancestor names */ 
			     ipdInt          nAncestorRelNames /* [I  :der length(ancestorRelNames)]  */ 
			     ) 
{
   ipdTreeNode_t *tn;
   ipdTreeNode_t *sanc = NULL, *first = NULL;
   ipdInt           i;

   ipdVeStart("ipdChangeAncestorsL");

   if (ipdGetErrorStatus())
   {
      ipdStdLog(IPD_STR_RESET_ERROR);
      ipdVeReturn(ipdFALSE);
   }

   /* Find the section with the name 'sectionFullName'. */
   if (!(tn = iiHashFindTreeNodeType(sectionFullName, ipdSECTION)))
   {
      ipdSetError(ipdERR_SECT_IPDNOT_FOUND, sectionFullName);
      ipdVeReturn(ipdFALSE);
   }

   /* Store all new ancestor names. */
   for (i = nAncestorRelNames; i > 0; i--)
   {
      if (!(sanc = iiCreateTreeNode(ipdSECTION_REF)) ||
	  !(sanc->node.sectRef.name = iiStrDup(ancestorRelNames[i - 1])))
      {
	 if (!ipdGetErrorStatus())
	    ipdSetError(ipdERR_UNKIPDNOWN);
	 iiFreeTreeNodeRec(sanc, ipdFALSE);
	 ipdVeReturn(ipdFALSE);
      }
      sanc->node.sectRef.isActive=ipdTRUE; /*[CK] This sets the Ancestor really active. Bugfix without thinking...*/
      sanc->next = first;
      first = sanc;
   }

   /* Free all old ancestor names. */
   iiFreeTreeNodeRec(tn->node.sv.svu.sect.ancestor, ipdFALSE);

   /* Now store new ancestors.     */
   tn->node.sv.svu.sect.ancestor = first;

   ipdVeReturn(ipdTRUE);
} /* ipdChangeAncestorsL */

/** @brief Changes the comment of an item.
    @param itemName Full name of the item
    @param comment The new comment
    @return ipdTRUE if everything is ok. ipdFALSE --> Error!
 */
ipdBoolean ipdChangeItemCommentL(ipdConstString itemName, /* [I]  section fullname */
			       ipdConstString comment   /* [I] value            */
			       )
{
   char *varName, *newComment;
   ipdTreeNode_t *tn;

   ipdVeStart("ipdChangeItemCommentL");

   if (ipdGetErrorStatus())
   {
      ipdStdLog(IPD_STR_RESET_ERROR);
      ipdVeReturn(ipdFALSE);
   }

   if (!(varName = iiCreateName(itemName, NULL)))
      ipdVeReturn(ipdFALSE);
   if (!(tn = iiHashFindTreeNode(varName)))
   {
      ipdSetError(ipdERR_IPDNOT_LOCAL, itemName);
      ipdMBfree(varName);
      ipdVeReturn(ipdFALSE);
   }
   ipdMBfree(varName);

   if (!(newComment = iiStrDup(comment)))
      ipdVeReturn(ipdFALSE);

   /* Trace variable. */
   iiTraceVar(tn, "change comment");

   ipdMBfree(tn->comment);
   tn->comment = newComment;

   ipdVeReturn(ipdTRUE);
} /* ipdChangeItemCommentL */

/*********************************/
/********* Delete/Rename *********/
/*********************************/

/** @brief Renames an item.
    @param oldFullName The old full name of the item
    @param newName The new name
    @return ipdTRUE if everything is ok. ipdFALSE --> Error!
 */
ipdBoolean ipdRenameItemL(ipdConstString oldFullName, /* [I] old fullname of the item  */
			ipdConstString newName      /* [I] new name              */
			)
{
   ipdTreeNode_t *tn, *tmp;
   char          *newFullName;

   ipdVeStart("ipdRenameItemL");

   if (ipdGetErrorStatus())
   {
      ipdStdLog(IPD_STR_RESET_ERROR);
      ipdVeReturn(ipdFALSE);
   }

   if (ipdvStrchr(newName, '~') || ipdvStrchr(newName, '.') || ipdvStrchr(newName, '^'))
   {
      ipdSetError(ipdERR_INVALID_REL_NAME, newName);
      ipdVeReturn(ipdFALSE);
   }

   /* If variable 'oldFullName' is inherited do not rename it.
      This might cause an internal error. */
   iiGetInheritedItemRec(&tmp, /* Find the variable behind */
			 (char *)oldFullName,
			 NULL,
			 ipdFALSE,
			 NULL);
   if ((tmp))
   {
      if (!ipdGetErrorStatus())
	 ipdSetError(ipdERR_CANT_RENAME_INHERITED, oldFullName);
      ipdVeReturn(ipdFALSE);
   }
   /* Find the item with the name 'oldFullName'. */
   if (!(tn = iiHashFindTreeNode(oldFullName)))
   {
      ipdSetError(ipdERR_IPDNOT_FOUND, oldFullName);
      ipdVeReturn(ipdFALSE);
   }

   /* Create a valid new name. */
   if (!(newFullName = iiCreateName(newName, tn->node.sv.parent->node.sv.fullname)))
      ipdVeReturn(ipdFALSE);

   /* Test, if there exists a variable 'newFullName'... */
   iiGetInheritedItemRec(&tmp, /* Find the variable behind */
			 newFullName,
			 NULL,
			 ipdTRUE,
			 NULL);
   if ((tmp))
   {
      if (!ipdGetErrorStatus())
	 ipdSetError(ipdERR_CANT_RENAME_NAME_EXISTS, oldFullName, newFullName);
      ipdMBfree(newFullName);
      ipdVeReturn(ipdFALSE);
   }

   /* Trace variable. */
   iiTraceVar(tn, "rename");

   /* Remove item from the hash. */
   iiHashRemoveTreeNode(tn);

   /* Free old name. */
   ipdMBfree(tn->node.sv.fullname);

   /* Link the new name. */
   tn->node.sv.fullname = newFullName;
   tn->node.sv.name     = iiExtractNameFromFullname(newFullName);

   /* Back into the hash. */
   iiHashStoreTreeNode(tn);
   if (ipdGetErrorStatus())
      ipdVeReturn(ipdFALSE);

   ipdVeReturn(ipdTRUE);
} /* ipdRenameItemL */

/** @brief Deletes an item.
    @param name Name of the item to delete
    @return ipdTRUE if everything is ok. ipdFALSE --> Error!
 */
/***TF [:lisp-name ipd::delete-item] */
ipdBoolean ipdDeleteItemL(ipdConstString name)
{
   ipdVeStart("ipdDeleteItemL");

   if (ipdGetErrorStatus())
   {
      ipdStdLog(IPD_STR_RESET_ERROR);
      ipdVeReturn(ipdFALSE);
   }

   ipdVeReturn(ipdDeleteByName(name));
} /* ipdDeleteItemL */

/*********************************/
/******** Error handling *********/
/*********************************/

/** @brief The error message is returned in errorMessage.
    @param errorMessage The error message
    @return Returns ipdTRUE if an error has occured, ipdFALSE if an warning has occured.
 */
ipdBoolean ipdGetErrorStatusL(ipdConstString *errorMessage)
{
   ipdVeStart("ipdGetErrorStatusL");

   if ((errorMessage))
   {
      if (ipdGetErrorStatus())
      {
	 *errorMessage = ipdvStrdup(ipdGetErrorMessage());
	 ipdVeReturn(ipdTRUE);
      }
      *errorMessage = ipdNULL;
   }
   ipdVeReturn(ipdFALSE);
} /* ipdGetErrorStatusL */

/** @brief Resets all errors. Necessary after each ipd-call
 */
/***TF [:lisp-name ipd::reset-errors] */
ipdVoid ipdResetErrorsL()
{
   ipdVeStart("ipdResetErrorsL");

   ipdResetError();
   ipdVeReturnVoid;
} /* ipdResetErrorsL */


/******** IO Functions *****************/

/***TF [:lisp-name ipd::dump-all-2-file] */
/** @brief Dump everything to a file
    @param filename Path and filename
    @param onlyValues Dump only values or expressions (formulas)
    @return ipdTRUE if everything is ok. ipdFALSE --> Error!
*/
ipdBoolean ipdDumpAll2FileL(ipdConstString filename,  /* [I] path and filename                          */
			  ipdBoolean     onlyValues /* [I] dump only values or expressions (formulas) */
			  )
{
   FILE *file;

   if (!(file = fopen(filename, "w")))
   {
      ipdSetError(ipdERR_CANT_OPEN_FILE, filename);
      return ipdFALSE;
   }
   ipdDumpAll2File(file, onlyValues);
   fclose(file);
   return ipdTRUE;
} /* ipdDumpAll2FileL */
