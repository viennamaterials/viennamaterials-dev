/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

CONTENT:  Extended interface declarations.
          Primarily for internal use.

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  RK   Robert Klima,      Institute for Microelectronics, TU Vienna
          BR   Bernhard Reinisch, Institute for Microelectronics, TU Vienna

  who when        what
-------------------------------------------------------------------------
  BR  1997        first coding, spread in different files
  RK  1998-04-14  new from scratch
=======================================================================*/


/** @file ipdP.h
    @brief The top level include file which is to be included by end-users of the library. Including this file in your project is usually sufficient.

*/

/*********** Note: Doxygen comments are found in the .c files! ***************/


/** @ingroup Core */
/*@{*/

#ifndef IPDP_H
#define IPDP_H

#if defined(__cplusplus)
extern "C" {
#endif

/*=======================================================================
	Includes
=======================================================================*/

#include "ipd.h"

/*=======================================================================
	Some Defines
=======================================================================*/
#define ipdMAX(a,b) ((a) > (b) ? (a) : (b))

/*********************************/
/*        Error Handling         */
/*********************************/
#define ipdERR_PREFIX          "#### ipd:"
#define ipdERRORSTRLEN         2048
typedef struct ipdERRSheet_s ipdERRSheet_t;
struct ipdERRSheet_s
{
   ipdERRSheet_t *next;
   ipdERR_Type_t  errNum;
   ipdChar          errStr[ipdERRORSTRLEN];
};

#if IPD_TEST_ERROR_NUMBERS
#define IPD_ERR_ENTRY(num,str)    {num, str}
typedef struct
{
   ipdERR_Type_t  errNum;
   ipdChar         *errStr;
} ipdERR_t;
#else
#define IPD_ERR_ENTRY(num,str)     str
typedef ipdConstString ipdERR_t;
#endif /* IPD_TEST_ERROR_NUMBERS */

#if IPD_ENABLE_INDIVIDUAL_EXTENSIONS
typedef struct ipdExt_s ipdExt_t;
struct ipdExt_s
{
   ipdExt_t *next;
   char     *extension;
};
#endif /* IPD_ENABLE_INDIVIDUAL_EXTENSIONS */


/*********************************/
/*        Administration         */
/*********************************/
#define IPD_LOG_FILE                        "ipd.log"
#define IPD_LOG_ENV                         "IPD_LOGFILE"
#define IPD_DEFAULT_SEARCH_PATH             "./"
#define IPD_SEARCH_ENV                      "IPD_SEARCHPATH"
#if !IPD_ENABLE_INDIVIDUAL_EXTENSIONS
#define IPD_FILE_EXT                        "ipd"
#endif /* !IPD_ENABLE_INDIVIDUAL_EXTENSIONS */
#define IPD_TRACE_MSG                       "IPD trace: "

/* Error simplifications */
#define IPD_ipdDEBUG          0
#define ipd_RETURNonERRORb if (ipdGetErrorStatus()) return ipdFALSE
#define ipd_RETURNonERRORp if (ipdGetErrorStatus()) return NULL
#define ipd_RETURNonERRORv if (ipdGetErrorStatus()) return
#define ipd_RETURNonERROR  if (ipdGetErrorStatus()) return

/* Database              */
#define iiMAXOPERANDS    32

/* Units                 */
#if IPD_USE_UNITS
#define ipdUNITFLAGS (ipduntOUTPUT_VOLT | ipduntOUTPUT_IPDNONUMBER | ipduntOUTPUT_DIV | ipduntOUTPUT_MULT)
#endif /* IPD_USE_UNITS */

/* Mathematics           */
#define ipd_PI        3.1415926535897932384626433832795
#define ipd_E         2.71828182845904523536028747135266
#define ipd_INV_LOG10 0.4342944819032518276511289189166
#define ipd_INV_LOG2  1.4426950408889634073599246810019
#define ipd_LOG10     2.3025850929940456840179914546844
#define ipd_LOG2      0.6931471805599453094172321214582


#if IPD_TEST_ERROR_NUMBERS
#define IPD_FUNC_ENTRY(num,name,func,paranum,destr)    {num, name, func, paranum, destr}
#else
#define IPD_FUNC_ENTRY(num,name,func,paranum,destr)    {name, func, paranum, destr}
#endif /* IPD_TEST_ERROR_NUMBERS */

/* Builtin Functions     */
typedef enum 
{
   ipdFUNC_ABS,
   ipdFUNC_ACOS,
   ipdFUNC_ACOSH,
   ipdFUNC_ACOT,
   ipdFUNC_ACOTH,
   ipdFUNC_ARG,
   ipdFUNC_ASIN,
   ipdFUNC_ASINH,
   ipdFUNC_ATAN,
   ipdFUNC_ATANH,
   ipdFUNC_BOOLEAN,
   ipdFUNC_CBRT,
   ipdFUNC_COS,
   ipdFUNC_COSH,
   ipdFUNC_COT,
   ipdFUNC_COTH,
   ipdFUNC_CUB,
   ipdFUNC_EXP,
   ipdFUNC_FABS,
   ipdFUNC_FULLNAME,
   ipdFUNC_GETENV,
   ipdFUNC_IF,
   ipdFUNC_IMAGPART,
   ipdFUNC_INTEGER,
   ipdFUNC_ITERATE,
   ipdFUNC_LENGTH,
   ipdFUNC_LOG,
   ipdFUNC_LOG10,
   ipdFUNC_LOG2,
   ipdFUNC_PARSE_UNIT,
   ipdFUNC_POW,
   ipdFUNC_POW10,
   ipdFUNC_POW2,
   ipdFUNC_QUERY,
   ipdFUNC_RAND,
   ipdFUNC_REAL,
   ipdFUNC_REALPART,
   ipdFUNC_SIGMA,
   ipdFUNC_SIGN,
   ipdFUNC_SIN,
   ipdFUNC_SINH,
   ipdFUNC_SQRT,
   ipdFUNC_SQU,
   ipdFUNC_STRING,
   ipdFUNC_TAN,
   ipdFUNC_TANH,
   ipdFUNC_UNIT,
   ipdFUNC_VALUE,

   ipdBuiltinFuncNUMBER
} ipdFUNC_Type_t;


/* Debugging Macros      */
#if IPD_ipdDEBUG
#define debug(x,y)            printf((x),(y))
#define debug0(s)             printf((s))
#define debug1(s,p1)          printf((s),(p1))
#define debug2(s,p1,p2)       printf((s),(p1),(p2))
#define debug3(s,p1,p2,p3)    printf((s),(p1),(p2),(p3))
#define debug4(s,p1,p2,p3,p4) printf((s),(p1),(p2),(p3),(p4))
#else
#define debug(x,y)
#define debug0(s)
#define debug1(s,p1)
#define debug2(s,p1,p2)
#define debug3(s,p1,p2,p3)
#define debug4(s,p1,p2,p3,p4)
#endif

/*=======================================================================
        Typedefs
=======================================================================*/

typedef struct iiMBufferLowerEntry_s   iiMBufferLowerEntry_t;
typedef struct iiMBufferHigherEntry_s  iiMBufferHigherEntry_t;
typedef struct iiMemBlock_s            iiMemBlock_t;
typedef struct iiCCodedFunc_s          iiCCodedFunc_t;
typedef struct iiBuiltInFunc_s         iiBuiltInFunc_t;
typedef struct ipdSLL_s                ipdHashEntry_t;

typedef struct ipdPrivate_s            ipdPrivate_t;
typedef struct ipdBase_s               ipdBase_t;

typedef struct ipdSearchPathIterator_s ipdSearchPathIterator_t;

/*=======================================================================
	Enums
=======================================================================*/

/*********************************/
/*       Expression Flags        */
/*********************************/
enum
{
   ipdEF_NOFLAG          =  0,
   /* For each operand: If corresponding reverse bit is set,
      the "complementary" operator will be used:
      '+' -> '-'
      '*' -> '/'	       */
   ipdEF_REVERSE         =  1,
   ipdEF_CONSTANT        =  2,
   ipdEF_BUILTIN_FUNC    =  4,
   ipdEF_SIMPLIFIED      =  8  /* Is set, when tree has been simplified. */
};

/*********************************/
/*        Variable Flags         */
/*********************************/
enum
{
   ipdVF_NOFLAG          =  0,
   ipdVF_AUXILIARY       =  1,  /* Its an auxiliary variable.  */
   ipdVF_EXTERN          =  2,  /* Its an extern variable.     */
   ipdVF_APPQUERY        =  4,  /* Query by the application.   */
   ipdVF_TRACE           =  8   /* Log usage of this variable. */
};

/*********************************/
/*         Section Flags         */
/*********************************/
enum
{
   ipdSF_NOFLAG          =  0,
   ipdSF_AUXILIARY       =  1,
   ipdSF_EXTERN          =  2,
   ipdSF_EXTENDABLE      =  4,  /* Section is no longer protected from appending. */
   ipdSF_CLOSED          =  8,  /* Set when a section is closed.                  */
   ipdSF_IS_BASESECTION  = 16   /* Set when a section is closed.                  */
};

/*********************************/
/*  Section and Variable Flags   */
/*********************************/
enum
{
   ipdSV_NOFLAG          =  0,
   ipdSV_IS_TEMP_LOKAL   =  1,
   ipdSV_FORCE_APPEND    =  2   /* Append this item.              */
};

/*********************************/
/*          Base Flags           */
/*********************************/
enum
{
   ipdBF_NOFLAG          =  0,
   ipdBF_IPDNO_TRACE        =  1,
   ipdBF_PARSE_FULLNAMES =  2
};
typedef long ipdBaseFlags_t;

typedef enum
{
   iiRBC_NOFLAG          = 0,
   iiRBC_RESET           = 1,
   iiRBC_SET             = 2,
   iiRBC_TOGGLE          = 4,
   iiRBC_QUERY           = 8
} iiReverseBitCommand_t;

/*=======================================================================
	Data Structures
=======================================================================*/

/*********************************/
/* THE Global Private Structure  */
/*********************************/
struct ipdPrivate_s
{
   ipdIFL_t  *scannedFile;             /* The file which is beein parsed just now. */
   ipdLong      successfullyParsedFiles; /* Number of files read. */
   ipdvosFile   *searchPathList;          /* Array of vos paths. */
   ipdLong      searchPathCount;         /* Number of search paths. */
   ipdBase_t *database;                /* A list of all databases. */
   ipdLong      errorCount;              /* an error counter */
   ipdLong      warnCount;               /* a warning counter */
   FILE      *logFile;                 /* error, warning, debug-info,... */
   ipdBoolean   parsing;                 /* I'm parsing (reading the input deck) */
#if IPD_ENABLE_INDIVIDUAL_EXTENSIONS
   ipdExt_t  *extension;               /* Pointer to the first extension */
#endif /*IPD_ENABLE_INDIVIDUAL_EXTENSIONS */
   ipdBoolean   inheritanceInfo;         /* Print information on conditional inheritance */
};

/* iterator for searchpaths */
struct ipdSearchPathIterator_s
{
   ipdLong    index;                     /* The index of the current file. -1 .. finished */
   ipdLong    listnum;                   /* 0 .. lokal, 1 .. global */
   ipdvosFile *plist[2];                  /* path lists */
   ipdLong    pnum [2];                  /* list lenghts */
};

/*********************************/
/*    Memory Block Structure     */
/*********************************/
#define IPD_MBLOCK_LOWER_HASH_SIZE    512
#define IPD_MBLOCK_HIGHER_HASH_SIZE   1023

#define IPD_MEM_BLOCK_NUM   100

#if IPD_MEMORY_ALLOC_ipdDEBUG_INFO
/* This macro shows information on a memory pointer given, such as the location
   where it has been allocated and the size of the memory pointed to. On the
   other hand it shows if it has been already freed or not.
*/
#define IPD_MEM_INFO(mem) printf("\nIPD-memory-block%s. file: \"%s\", line %ld, size %d\n",             \
				   ((iiMBufferLowerEntry_t *)(((ipdChar *)mem) - IPD_MEM_OFFSET_LOWER))->m.next? \
				   "(freed)":"(allocated)",                                               \
				   ((iiMBufferLowerEntry_t *)(((ipdChar *)mem) - IPD_MEM_OFFSET_LOWER))->filename, \
				   ((iiMBufferLowerEntry_t *)(((ipdChar *)mem) - IPD_MEM_OFFSET_LOWER))->lineno,   \
				   ((iiMBufferLowerEntry_t *)(((ipdChar *)mem) - IPD_MEM_OFFSET_LOWER))->m.mSize)
  struct iiMBufferLowerEntry_s
  {
     const ipdChar                *filename;
     ipdLong                       lineno;
     struct
     {
	iiMBufferLowerEntry_t  *next;
	unsigned long           mSize;  /* Must be the last.  */
     } m;
  };
  struct iiMBufferHigherEntry_s
  {
     const ipdChar                *filename;
     ipdLong                       lineno;
     iiMBufferHigherEntry_t     *nextMB; /* Must be the last attribute. */
     struct  /* Must be the last.  */
     {
	iiMBufferLowerEntry_t  *next;    /* Not used when allocated. Used when freed. */
	unsigned long           mSize;
     } m;
  };
#else  /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */
#define IPD_MEM_INFO(mem)
  struct iiMBufferLowerEntry_s
  {
     union  /* Must be the last.  */
     {
	iiMBufferLowerEntry_t  *next;    /* Not used when allocated. Used when freed. */
	unsigned long           mSize;
     } m;
  };
  struct iiMBufferHigherEntry_s
  {
     iiMBufferHigherEntry_t    *nextMB; /* Must be the last attribute. */
     union  /* Must be the last.  */
     {
	iiMBufferHigherEntry_t *next;   /* Not used when allocated. Used when freed. */
	unsigned long           mSize;
     } m;
  };
#endif /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */

#define IPD_MEM_ARCHITECTURE  (ipdMAX(sizeof(long),sizeof(ipdChar *)))
/* #define IPD_MEM_ARCHITECTURE  (ipdMAX(sizeof(long),sizeof(char *)))*/
#define IPD_MEM_OFFSET_LOWER  (sizeof(iiMBufferLowerEntry_s))
#define IPD_MEM_OFFSET_HIGHER (sizeof(iiMBufferHigherEntry_s))
struct iiMemBlock_s
{
   iiMemBlock_t           *next;
   iiMBufferLowerEntry_t  *first;
};

/*********************************/
/*             Units             */
/*********************************/

#if IPD_USE_UNITS
struct ipdUnit_s
{
   ipdUnit_t         *next;
   ipdChar             *unitStr;
   ipduntUnitRep         unitRep;
   long               uLinkCount;
};
#endif /* IPD_USE_UNITS */

/*********************************/
/*      Single Linked List       */
/*********************************/
struct ipdSLL_s
{
   ipdSLL_t         *next;
   void             *node;
};

/*********************************/
/*      Double linked List       */
/*********************************/
struct ipdTreeNodeDll_s
{
   ipdTreeNodeDll_t *next;
   ipdTreeNodeDll_t *prev;
   ipdTreeNode_t    *tn;
};

/*********************************/
/*    Dynamic Name Structure     */
/*********************************/
struct ipdDynamicName_s
{
   ipdChar            *name;
   long              len;
};

/*********************************/
/*       Include File List       */
/*********************************/
struct ipdIFL_s
{
   ipdIFL_t         *next;
   ipdvosFile           file;
   ipdChar            *fileName;
   ipdChar            *comment;
};

/*********************************/
/*        Intern Function        */
/*********************************/
struct iiInternFunc_s
{
   iiInternFunc_t         *next;
   ipdChar                  *name;
   ipdTreeNode_t          *tree;
   ipdTreeNode_t          *paramList;
   int                     parameterNum;
   ipdChar                  *comment;
};

/*********************************/
/*       C-Coded Functions       */
/*********************************/
struct iiCCodedFunc_s
{
   iiCCodedFunc_t         *next;
   char                   *name;
   ipdFunc_t               func;
   void                   *userData;
   ipdUserFunc_t           userDestructor;
};

/*********************************/
/*      Built-In Functions       */
/*********************************/
struct iiBuiltInFunc_s
{
#if IPD_TEST_ERROR_NUMBERS
   ipdFUNC_Type_t          funcNum;
#endif /* IPD_TEST_ERROR_NUMBERS */
   ipdConstString            name;
   ipdFunc_t               func;
   int                     parameterNum;
   ipdUserFunc_t           userDestructor;
};

/* IPD_DEFAULT_HASH_SIZE:
 * Best performance when using prime-numbers
 * Very good performance when using (2^n)-1
 * the longer the faster
 */
#define IPD_DEFAULT_HASH_SIZE       65521
#define IPD_ITERATOR_NAME_HASH_SIZE    61
#define IPD_UNIT_HASH_SIZE            101

struct ipdHash_s
{
   long             hashTableSize;
   ipdHashEntry_t **hashTable;
};

typedef struct ipdHash_s ipdHash_t;

/*********************************/
/*           IPD Base            */
/*********************************/
#define ipdNAMEHASHSIZE 61
struct ipdBase_s
{
   /* The next base. */
   ipdBase_t              *next;
   /* It can be found by this index */
   ipdLong                   index;

   /* Name of this Inputdeck. */
   ipdChar                  *ipdName;

   /* The search paths */
   ipdvosFile   *lokalSearchPathList;          /* Array of vos paths. */
   ipdLong       lokalSearchPathCount;         /* Number of search paths. */

   /* A list containing all files (ipdvosFile) this inputdeck
      consists of. */
   ipdIFL_t               *files;

   /*  IPD-ROOT-SECTION stores the whole tree. */
   ipdTreeNode_t          *root_p;

   /*  IPD-CURRENT-SECTION  */
   ipdTreeNode_t          *CS_p;

   /*  IPD-Functionlist for intern functions (ipdINTERNFUNCTION)  */
   iiInternFunc_t         *funcList;
   
   /*  HASH-TABLE           */
   ipdHash_t               hashTable;

   /* Pointer to variable which is evaluated now. */
   /* We need this to detect circular references. */
   ipdTreeNode_t          *calcVar;         /* If we evaluate a=1+b+c,
					       this is "a" */
   ipdTreeNode_t          *currVar;         /* If we evaluate a=1+b+c,
					       this is either "a", "b" or "c" */
   ipdDynamicName_t       *topBaseSect;     /* First base section (when starting calculation). */
   ipdDynamicName_t       *currBaseSect;    /* Current base section. */
   ipdChar                  *currVarFullname; /* Fullname of currVar */

   /* Fullname of the virtual current section
      or NULL if none is set. */
   ipdChar                  *CSFullName;

   /* Current section stack. One may push and pop CSFullNames.
      This stack is totally independent from CSFullName. */
   ipdSLL_t               *CSStack;

   /* Single linked list of all opened iterators. */
   ipdIterator_t          *activeIteratorSll;

   /* Number of In-Out statements (see ipdparse.y) */
   long                    InOut;

   /* Flags */
   ipdBaseFlags_t          bFlags;

   /* Error stack */
   ipdERRSheet_t          *errorStack;
};

/*=======================================================================
	Variable Declarations
=======================================================================*/

extern ipdBase_t               *ipdBase;
extern ipdPrivate_t             ipdPrivate;

extern iiMemBlock_t           **iiMBlockHashLower;
extern iiMBufferHigherEntry_t **iiMBlockHashHigher;

extern iiBuiltInFunc_t          iiBuiltInFuncTable[];
extern iiCCodedFunc_t          *ipdCCodedFunctions;

extern ipdTreeNode_t           *ipdParameterList;

extern int                      tokenpos;
extern int                      ipdLineNo;
extern ipdChar                   **currentComment;
extern int                      ipdIncludeStackPtr;
extern int                      commentNewLine;

extern ipdBoolean                 ipdErrorGotNew;
extern ipdBoolean                 ipdEditorPrivileges;
extern ipdBoolean                 ipdApplicationSet;

extern ipdBoolean                 ipdWarnOnLoseingStmt;

extern ipdBoolean                 ipdTolerantParsing;
extern ipdBoolean                 ipdAliasParsing;

/*=======================================================================
	Functions Declarations
=======================================================================*/

/*********************************/
/*        Administration         */
/*********************************/
ipdTreeNode_t    *iiCreateTreeNode(ipdType_t type);
#if IPD_USE_UNITS
ipdUnit_t        *iiCopyUnit(ipdUnit_t *un);
#endif /* IPD_USE_UNITS */

/*********************************/
/*        Error Handling         */
/*********************************/
void              iiSetErrorStatus(void);
void              iiResetErrorStack(void);
void              iiSetErrorNumber(ipdERR_Type_t errNum);

/*********************************/
/*       Memory Operations       */
/*********************************/
#if IPD_MEMORY_ALLOC_ipdDEBUG_INFO
void              iiTestMalloc(void);
#endif /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */
void              iiFreeMBHash(void);
void              iiSetMBHash(void);

/*********************************/
/*            Scanner            */
/*********************************/
void              iiDumpScannerPosition(void);
void              includeFile(ipdBoolean withSearchPath, ipdChar *name);
void              yyerror(const char *s);
int               yylex(void);

/*********************************/
/*             Parser            */
/*********************************/
/* int           yyparse (void *); */
/* void         *yy_scan_string(const char *); */

/*********************************/
/*              Hash             */
/*********************************/
ipdBoolean          iiInitHash(ipdHash_t *hash, long hashTableSize);
ipdHashEntry_t   *iiCreateHashEntry(void);
void              iiFreeHashNode(ipdHashEntry_t *hn);
void              iiCleanHash(ipdHash_t *hash);
void              iiFreeHash(ipdHash_t *hash);
long              iiHashComputeKey(ipdConstString  string,
				   ipdHash_t    *hash);
void              iiHashRemoveNode(ipdHash_t *hash,
				   char      *name,
				   void      *node);
void              iiHashRemoveTreeNode(ipdTreeNode_t *tn);
void              iiHashStoreTreeNode(ipdTreeNode_t *tn);
ipdTreeNode_t    *iiHashFindTreeNode(ipdConstString name);
ipdTreeNode_t    *iiHashFindTreeNodeType(ipdConstString name, ipdType_t type);
void              iiHashStoreItem(ipdTreeNodeDll_t **dll_p,
				  ipdTreeNode_t     *tn,
				  ipdBoolean           isLocal,
				  ipdHash_t         *hash);
ipdTreeNodeDll_t *iiHashGetItem(ipdHash_t     *hash,
				ipdTreeNode_t *tn);
void              iiAddAllItemsToStack(ipdTreeNodeDll_t **dll_p,
				       ipdTreeNode_t     *tn,
				       ipdHash_t         *hash);

/*********************************/
/*             FIFOs             */
/*********************************/
ipdTreeNodeDll_t *iiCreateTnDll(void);
void              iiFreeTnDll(ipdTreeNodeDll_t *dll);
ipdTreeNodeDll_t *iiAppendTNFifo(ipdTreeNodeDll_t **tnDll_p,
				 ipdTreeNode_t     *tn);
ipdTreeNodeDll_t *iiPushTNFifo(ipdTreeNodeDll_t **tnDll_p,
			       ipdTreeNode_t     *tn);
ipdTreeNode_t    *iiPopTNFifo(ipdTreeNodeDll_t **tnDll_p);
ipdTreeNodeDll_t *iiFindTnInFifo(const ipdTreeNodeDll_t *tnDll,
				 const ipdTreeNode_t    *tn);

/*********************************/
/*    Dynamic Name Structure     */
/*********************************/
ipdDynamicName_t *iiCreateDNNode(void);
void              iiFreeDNNode(ipdDynamicName_t *dnn);

/*********************************/
/*             Units             */
/*********************************/
#if IPD_USE_UNITS
ipdUnit_t        *iiCreateUnitNode(void);
void              iiInitUnitHash(void);
void              iiFreeUnitHashNode(ipdUnit_t *un);
void              iiFreeUnitHash(void);
void              iiSetNullUnitHash(void);
long              iiUnitHashComputeKey(ipduntUnitRep *unitRep);
long              iiUnitHashComputeCompoundCode(long a, long b);
void              iiUnitHashRemoveUnit(ipdUnit_t *unr);
ipdChar            *iiGetUnitString(ipdUnit_t *un);
ipdUnit_t        *iiUnitHashStoreUnit(ipdUnit_t *unr);
ipdUnit_t        *iiUnitHashFindUnitStr(ipdConstString unitStr);
void              iiMultiplyUnits(ipdUnit_t **resUnit, ipdUnit_t *u2);
void              iiDivideUnits(ipdUnit_t **resUnit, ipdUnit_t *u2);
ipdBoolean          iiUnitCompareable(ipdUnit_t *u1, ipdUnit_t *u2);
#endif /* IPD_USE_UNITS */

/*********************************/
/*           TreeNodes           */
/*********************************/
ipdBoolean          iiCheckName(ipdTreeNode_t *tn);
ipdTreeNode_t    *iiConnectTreeNode(ipdTreeNode_t *parent, ipdTreeNode_t *newChild);
void              iiUnlinkTreeNode(ipdTreeNode_t *tn);
void              iiClearValue(ipdTreeNode_t *tn);
ipdBoolean          iiIsConstant(ipdTreeNode_t *tn);
void              iiClearTemporaryValue(ipdTreeNode_t *tn);
void              iiCopyValue(ipdTreeNode_t *copy, ipdTreeNode_t *tree, ipdBoolean docopy);
void              iiAssignValue(ipdTreeNode_t *copy, ipdTreeNode_t *tree);
void              iiFreeTreeNodeRec(ipdTreeNode_t *tn, ipdBoolean force);

/*********************************/
/*       String Operations       */
/*********************************/
ipdChar            *ltoa2(ipdChar *str, long num);
int               iiStrCmp(ipdConstString s1, ipdConstString s2);
int               iiStrCmpNamePart(ipdConstString p1, ipdConstString p2, ipdConstString s);
int               iiStrCmpSoft(ipdChar *str1, ipdChar *str2);
long              iiStrLen(ipdConstString s1);
ipdChar            *iiStrConcate(ipdChar *s1, ipdChar *s2);
ipdChar            *iiStrAppend(ipdChar *s1, ipdChar *s2);
ipdChar            *iiStrAppendChar(ipdChar *s1, ipdLong c);
char             *iiStrDup(ipdConstString s);
ipdBoolean          iiStrExtend(ipdChar        **sTo_p,
			      long          *len_p,
			      ipdConstString   sFrom,
			      ipdBoolean       append);
long              iiStrocc(ipdConstString string, ipdConstString ostr);
void              iiCreateTrailingZero(ipdChar *str);

/*********************************/
/*         Base Queries          */
/*********************************/
ipdTreeNode_t    *iiGetRoot(void);
ipdTreeNode_t    *iiGetCS(void);
void              iiSetCS(ipdTreeNode_t *newCS);

/*********************************/
/*        Initialization         */
/*********************************/
void             iiSetScannedFile(ipdIFL_t *ifn);
ipdIFL_t        *iiGetScannedFile(void);
void             iiSetSuccessfullyParsedFiles(ipdLong i);
int              iiGetSuccessfullyParsedFiles(void);
void             iiInitLogFile(FILE *logFile, ipdBoolean force);

/*********************************/
/*        File Operations        */
/*********************************/
FILE            *iiFileOpen(ipdConstString   filename,
			    ipdIFL_t     **newFile,
			    ipdvosFile        currentDir,
			    ipdBoolean       useSearchPaths,
			    ipdConstString   mode);

/*********************************/
/*     Input Deck Operations     */
/*********************************/
void              iiTraceVar(ipdTreeNode_t    *tn,
			     ipdConstString errString);
ipdTreeNode_t     *iiSetTraceVar(const char *fullname, ipdTreeNode_t *expr);
ipdChar             *iiCreateName(ipdConstString name, ipdConstString CSFullName);
ipdChar             *iiCreateRelName(ipdConstString name);
ipdChar             *iiCorrectName(ipdChar *name);
ipdChar             *iiComposeFullName(ipdConstString fullname, ipdConstString name);
ipdChar              iiRemoveNameFromPath(ipdChar *pathname);
ipdChar             *iiExtractNameFromFullname(ipdChar *fullname);

ipdTreeNode_t    *iiSave(ipdTreeNode_t *tn,
			 ipdBoolean       hasValidFullname,
			 long           lineno,
			 ipdIFL_t      *file);
ipdTreeNode_t    *iiPrepareEmptyVariable(ipdConstString fullname,
					 ipdConstString name);
void              iiDeleteVariable(ipdConstString name);

ipdTreeNode_t    *iiGenerateInheritedSection(ipdTreeNode_t *sect, ipdChar *sectName);
void              iiSetCSHistory(ipdTreeNode_t *sect);
ipdTreeNode_t    *iiReturnsub(void);

ipdBoolean          iiGetInheritedItemRec(ipdTreeNode_t    **result_p,
					ipdChar             *iName,
					ipdTreeNodeDll_t **tnDll_p,
					ipdBoolean           getLocalsToo,
					ipdTreeNode_t     *oldTn);
ipdTreeNode_t    *iiFindParentSection(ipdTreeNode_t *tn,
				      ipdTreeNode_t *tryParent);
ipdTreeNode_t    *iiFindParentSection(ipdTreeNode_t *tn,
				      ipdTreeNode_t *tryParent);
ipdTreeNode_t    *iiHasDescendantSection(ipdTreeNode_t *tn,
					 ipdTreeNode_t *tryDescendant);
ipdTreeNode_t    *iiGetAncestorSection(ipdTreeNode_t *tn);

ipdTreeNode_t    *iiGetBaseSect(void);
ipdTreeNode_t    *iiGetRealVariable(ipdTreeNode_t     *tn,
				    ipdDynamicName_t **baseSect_p);
ipdTreeNode_t    *iiFindNearestItem(ipdChar  *fullname,
				    ipdChar **pos_p);

ipdTreeNode_t    *iiIncreaseLinkCount(ipdTreeNode_t *tn);
ipdTreeNode_t    *iiRecallByName(ipdChar *fullname);

ipdTreeNode_t    *iiEval(ipdTreeNode_t *tn);
ipdTreeNode_t    *iiCreateArray(ipdTreeNode_t *tn);
ipdTreeNode_t    *iiCreateArrayIndex(ipdTreeNode_t *tn, ipdTreeNode_t *index);
ipdTreeNode_t    *iiFindArrayElement(ipdTreeNode_t    *arr,
				     ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiFindItemByName(ipdChar      **itemName,
				   ipdType_t    itemType,
				   ipdBoolean     doFindAuxiliaries);
ipdBoolean          iiIteratorDoNextFlags(ipdIterator_t *iNode, ipdVarFlags_t vFlags);
ipdBoolean          iiIteratorDoStepFlags(ipdIterator_t *iNode, ipdVarFlags_t vFlags);

/*********************************/
/*       Iterator Handling       */
/*********************************/
ipdIterator_t    *iiCreateIterator(ipdType_t type);
void              iiFreeAllIterators(void);

/*********************************/
/*     Function Manipulation     */
/*********************************/
void              iiFreeAllInternFunctions(void);
void              iiFreeAllCCodedFunctions(void);
ipdTreeNode_t    *iiGetFunction(const ipdChar *funcName, ipdTreeNode_t *parameter);
iiInternFunc_t   *iiCreateInternFunction(ipdChar         *funcName,
					 ipdTreeNode_t *parameterList,
					 ipdTreeNode_t *tree);

/*********************************/
/*       Type Conversions        */
/*********************************/
ipdChar             *iiSkipSpaces(ipdChar* str);
void              iiString2Complex(double *re, double *im, ipdChar *str);
ipdTreeNode_t    *iiCastOperation(ipdTreeNode_t *res, ipdTreeNode_t *tn,
				  ipdType_t type, ipdBoolean force);
unsigned long     iiOperandReverseBit(unsigned long         *num,
				      unsigned long          index,
				      iiReverseBitCommand_t  rCommand);
     
     
void ipdFreeBuffer();     
     
     
void iiPrintInternFunctions(ipdTreeNode_t *tn);     //[JW]
     
/*********************************/
/*          Calculation          */
/*********************************/
typedef enum
{
   iiGEN_NoFlags        = 0,
   iiGEN_CheckConstant
} iiGENFlags_t;
ipdTreeNode_t    *iiGetExprNode(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect,
				iiGENFlags_t flags);

ipdTreeNode_t    *iiCalculateFunction(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiCalculate(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiEvalTreeNode(ipdTreeNode_t *tn,
				 ipdConstString sectName);
ipdConstString      iiGetCurrVarFullname(void);
ipdConstString      iiGetCurrVarName(void);

/*********************************/
/*           Evaluation          */
/*********************************/
ipdTreeNode_t    *iiCreateOperatorNode(ipdTreeNode_t *leftNode,
				       ipdTreeNode_t *rightNode,
				       ipdFunc_t      func,
				       ipdBoolean       reverse);
ipdTreeNode_t    *iiSimplify(ipdTreeNode_t *tn);

/*********************************/
/*           Operators           */
/*********************************/
ipdTreeNode_t    *iiAdd(ipdTreeNode_t     *tn,
			ipdTreeNode_t    **iiArgs,
			long               iiCount
			);
ipdTreeNode_t    *iiAddArrays(ipdTreeNode_t     *tn,
			      ipdDynamicName_t  *baseSect,
			      ipdTreeNode_t    **iiArgs,
			      long               iiCount
			      );
ipdTreeNode_t    *iiSum(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiProduct(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiModulus(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiEqual(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiNotEqual(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiLower(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiGreater(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiLowerEqual(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiGreaterEqual(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiLogicOr(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiLogicAnd(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiLogicNot(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);

/*********************************/
/*       BuiltIn Functions       */
/*********************************/
iiBuiltInFunc_t  *iiFindBuiltinFunctionByName(const ipdChar *funcName);
ipdTreeNode_t    *iiSin(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiCos(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiTan(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiCot(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiASin(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiACos(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiATan(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiACot(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiSinh(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiCosh(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiTanh(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiCoth(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiASinh(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiACosh(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiATanh(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiACoth(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiExp(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiPow10(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiPow2(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiLog(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiLog10(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiLog2(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiPow(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiParseUnit(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiSqu(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiSqrt(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiCub(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiCbrt(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiAbs(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiArg(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiRealPart(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiImagPart(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiSign(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiSigma(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiFullname(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiQuery(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiRand(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiLength(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiGetEnv(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiIterate(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdBoolean          iiIterateDestructor(ipdTreeNode_t *tn);
ipdTreeNode_t    *iiIf(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiValue(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiUnit(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiInteger(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiReal(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiString(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);
ipdTreeNode_t    *iiBoolean(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect);

/*=======================================================================
=======================================================================*/

#if defined(__cplusplus)
}
#endif

#endif /* IPDP_H */

/*@}*/

