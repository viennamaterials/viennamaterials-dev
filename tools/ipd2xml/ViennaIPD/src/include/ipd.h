/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

CONTENT:  Declarations for the interface to applications.
          For internal use too.

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  RK   Robert Klima,      Institute for Microelectronics, TU Vienna
          BR   Bernhard Reinisch, Institute for Microelectronics, TU Vienna

  who when        what
-------------------------------------------------------------------------
  BR  1997        first coding, spread in different files
  RK  1998-04-14  new from scratch
=======================================================================*/


/** @file ipd.h
    @brief This file provides the declarations for all functions and definitions for all types needed by an end-user of the library.

*/

/**
 @mainpage Source Code Documentation for ViennaIPD 1.0

 This is the source code documentation of ViennaIPD. There are three main modules:
 - \link Core Core: \endlink Code used within the core of ViennaIPD only. An end-user does not need to care about this module.
 - \link LISP  LISP: \endlink Interfaces to LISP
 - \link EndUser End-User: \endlink The functionality intended for end-users of the library is documented here. If you are an end-user without need for interfacing LISP, this module is the only one you may need.

 For a beginner's tutorial, please refer to the examples provided in the folder examples/ and to \htmlonly <a href="../../ipd-tutorial/html/index.html"> the beginner's tutorial</a>\endhtmlonly.

*/

/* Doxygen modules: */
/** @defgroup Core Core: Functionality used within the core only 
    @brief Code used within the core of ViennaIPD only. An end-user does not need to care about this module.
*/
/** @defgroup LISP LISP: Interfaces to LISP 
    @brief Interfaces to LISP
*/
/** @defgroup EndUser End-User: Functionality for end-users of the library 
    @brief The functionality intended for end-users of the library is documented here. If you are an end-user without need for interfacing LISP, this module is the only one you may need. */
/*@{*/


#ifndef IPD_H
#define IPD_H

/*=======================================================================
	Includes
=======================================================================*/
   /** An error message is printed to the terminal in case an error
       occurred. The user is urged to investigate the log file.*/
#define IPD_ERROR_NOTIFICATION             1  /*  0 or 1, Default 1 */
   /** You may compile unitless. The inputdeck will then not able
      to deal with units.*/
#define IPD_USE_UNITS                      1  /*  0 or 1, Default 1 */
   /** You may wish to tell the input deck to simplify all expressions
      when found to be constant and to delete the constant expression
      tree in it. */
#define IPD_SIMPLIFY_DELETES_CONST_EXPR    0  /*  0 or 1, Default 0 */
   /** The eval() function can be forced not to store the expression
      tree evaluated, which causes changes to the stored tree. */
#define IPD_EVAL_DELETES_EXPR              0  /*  0 or 1, Default 0 */
   /** The inputdeck comes along with its own memory management which
      sharply rises the performance. If this is unwanted this 
      can be disabled. */
#define IPD_USE_IPD_MALLOC                 0 /*  0 or 1, Default 0 */
   /** Turn this on for additional info about memory allocation and
      to perform checking on freed structures.
      Can be used only, if IPD_USE_IPD_MALLOC is set to 1. */
#define IPD_MEMORY_ALLOC_ipdDEBUG_INFO        0  /*  0 or 1, Default 0 */
   /** Turn this on to allow checking usage of error numbers. */
#define IPD_TEST_ERROR_NUMBERS             0  /*  0 or 1, Default 0 */
   /** Turn this on for debug information while parsing. */
#define IPD_PARSER_ipdDEBUG_INFO              0  /*  0 or 1, Default 0 */
   /** Print error numbers too when dumping an error. */
#define IPD_PRINT_ERROR_NUM                0  /*  0 or 1, Default 0 */
   /** Dump whole input deck before exit. */
#define IPD_DUMP_WHOLE_IPD_B4_EXIT         0  /*  0 or 1, Default 0 */
   /** Turn this on to specify an individual list of file extentions;
      Otherwise, ".ipd" is used */
#define IPD_ENABLE_INDIVIDUAL_EXTENTIONS   1  /*  0 or 1, Default 1 [SW] */

#include "../core/include/vos.h"
#if IPD_USE_UNITS
#include "../core/include/unitP.h"
#endif /* IPD_USE_UNITS */

#if defined(__cplusplus)
extern "C" {
#endif

/*=======================================================================
	Some Defines
=======================================================================*/

/*********************************/
/*        Error Handling         */
/*********************************/
/**                              */
/*        Error Handling         */
typedef enum 
{
   ipdWARN_UNKIPDNOWN_WARN = 0,
   ipdERR_IPDNO_ERROR = 0,
   ipdERR_UNKIPDNOWN,
   ipdERR_IPDNO_MEMORY,
   ipdERR_LOGFILE,
   ipdERR_INVALID_NAME,
   ipdERR_INVALID_REL_NAME,
   ipdERR_IPDNO_VARIABLE,
   ipdERR_IPDNO_SECTION,
   ipdERR_IPDNOT_FOUND,
   ipdERR_VAR_IPDNOT_FOUND,
   ipdERR_SECT_IPDNOT_FOUND,
   ipdERR_VAR_LOCKED,
   ipdERR_ANCESTOR_IPDNOT_FOUND,
   ipdERR_ANCESTOR_IPDNO_SECTION,
   ipdERR_PARA_IPDNOT_FOUND,
   ipdERR_SEARCH_ITEM,
   ipdERR_IPDNOT_AN_AUXILIARY,
   ipdERR_IPDNOT_EXTERN_VAR,
   ipdERR_APPLICATION_SET_VAR,
   ipdERR_VAR_TYPE,
   ipdERR_CIRCULAR_REFS,
   ipdERR_TOO_FEW_PARA,
   ipdERR_TOO_MANY_PARA,
   ipdERR_VAR_IPDNOT_INITIALIZED,
   ipdERR_INCOMPATIBLE_TYPES,
   ipdERR_WRONG_PARA_TYPE,
   ipdERR_WRONG_TYPE,
   ipdERR_WRONG_TYPE_OP,
   ipdERR_DIFFERENT_UNITS,
   ipdERR_IPDNO_ADDITION,
   ipdERR_IPDNO_SUBTRACTION,
   ipdERR_IPDNO_MULTIPLICATION,
   ipdERR_IPDNO_DIVISION,
   ipdERR_DIV_BY_ZERO,
   ipdERR_INVALID_CAST,
   ipdERR_ARRAY_INDEX_TYPE,
   ipdERR_EXPECT_ARRAY,
   ipdERR_DIFF_ARRAY_LEN,
   ipdERR_INDEX2HIGH,
   ipdERR_INDEX2LOW,
   ipdERR_IPDNO_MUL_ARRAY,
   ipdERR_PARAM_NUM1,
   ipdERR_PARAM_NUMN,
   ipdERR_NAN_RESULT,
   ipdERR_0TO_POW0,
   ipdERR_0TO_POW_COMPLEX,
   ipdERR_INVALID_UNIT,
   ipdERR_INVALID_ENV,
   ipdERR_FUNC_IPDNOT_DEFINED,
   ipdERR_FUNC_ALREADY_DEFINED,
   ipdERR_FUNC_NUMBER,
   ipdERR_IPDNOT_SQRT_UNIT,
   ipdERR_IPDNOT_CBRT_UNIT,
   ipdERR_ARGUMENT0,
   ipdERR_CANT_OPEN_FILE,
   ipdERR_READ_INPUTDECK,
   ipdERR_CANT_EVAL_SECTION,
   ipdERR_QUERY_AUX_VARIABLE,
   ipdERR_CANT_CALCULATE_SECTION,
   ipdERR_TOO_MANY_INCLUDES,
   ipdERR_FILE_IPDNOT_FOUND,
   ipdERR_INHERIT_PARENT,
   ipdERR_PARSE,
   ipdERR_IPDNO_IPD_DATABASE_FOUND,
   ipdERR_MISSING_SEMICOLON,
   ipdERR_ILLEGAL_INCLUDE,
   ipdERR_ILLEGAL_DELETE,
   ipdERR_DELETE_IPDNOT_ALLOWED,
   ipdERR_ILLEGAL_TRACE,
   ipdERR_TRACESTRING_EXPECTED,
   ipdERR_ALIAS_EXPECTED,
   ipdERR_EQUAL_OP_EXPECTED,
   ipdERR_EXPRESSION_EXPECTED,
   ipdERR_SECTION_IN_EXPRESSION,
   ipdERR_ALIAS_OF_VARIABLE,
   ipdERR_VARIABLE_NAME_EXPECTED,
   ipdERR_PARAMETERLIST_EXPECTED,
   ipdERR_OPTIONS_LAST,
   ipdERR_IDLIST_EXPECTED,
   ipdERR_PATH_NAME_EXPECTED,
   ipdERR_CLOSING_PARENTH_EXPECTED,
   ipdERR_OPENING_PARENTH_EXPECTED,
   ipdERR_BINARY_OPERATOR,
   ipdERR_UNARY_OPERATOR,
   ipdERR_SQUARE_BRACKET_EXPECTED,
   ipdERR_SECT_ANCESTORS_EXPECTED,
   ipdERR_SECTION_NAME_EXPECTED,
   ipdERR_EVAL_EXPRESSION,
   ipdERR_ALIAS_VALUE,
   ipdERR_INTEGER_OVERFLOW,
   ipdERR_PUSH_IPDNO_SECTION,
   ipdERR_POP_IPDNO_SECTION,
   ipdERR_REOPEN_SECTION,
   ipdERR_OVERWRITING_EXPRESSION,
   ipdERR_APPEND_PROTECTED_SECTION,
   ipdERR_DIFFERENT_ITEM_TYPE,
   ipdERR_IPDNOT_LOCAL,
   ipdERR_IPDNO_SHADOW_INFO,
   ipdERR_HEAVYSIDE_FUNC,
   ipdERR_LVALUE_IS_FULLNAME,
   ipdERR_CANT_RENAME_INHERITED,
   ipdERR_CANT_RENAME_NAME_EXISTS,
   ipdERR_UNBALANCED_ARRAY_IPDNOT_MATRIX,
   ipdERR_UNKIPDNOWN_OPTION,
   ipdERR_GENERAL,

   ipdERR_TOTAL_NUM,

   ipdWARN_INFINITE_RESULT,
   ipdWARN_LOOSE_LAST_CUR_SECT,
   ipdWARN_FORCE_APPEND_EXISTS,
   ipdWARN_FORCE_APPEND_IPDNOT_INHERITED,
   ipdWARN_APPENDED_BY_APPLICATION,
   ipdWARN_USELESS_STMT,

   ipdWARN_TOTAL_NUM
} ipdERR_Type_t;



/*********************************/
/*             Basic             */
/*      0123456789876543210      */
/*********************************/

/*=======================================================================
        Other Typedefs
=======================================================================*/

/* Data Types            */
/** A IPD tree node, holding the data */
typedef struct ipdTreeNode_s    ipdTreeNode_t;
#if IPD_USE_UNITS
/** An IPD unit, only supported if IPD is compiled with the unit system */
typedef struct ipdUnit_s        ipdUnit_t;
#endif /* IPD_USE_UNITS */
/** A single linked list */
typedef struct ipdSLL_s         ipdSLL_t;

typedef struct iiInternFunc_s   iiInternFunc_t;
typedef struct ipdTreeNodeDll_s ipdTreeNodeDll_t;
typedef struct ipdDynamicName_s ipdDynamicName_t;

/** An iterator type for C++ abstraction at C level */
typedef struct ipdIterator_s    ipdIterator_t;

/** An iterator type for C++ abstraction at C level.
* Used for iteration over values.
*/
typedef struct ipdValueIterator_s ipdValueIterator_t;

/** Type definition for a function pointer, especially used internally */
typedef ipdTreeNode_t          *(*ipdFunc_t)(ipdTreeNode_t *, ipdDynamicName_t *);
/** Type definition for a function pointer to user defined functions */
typedef ipdBoolean                (*ipdUserFunc_t)(ipdTreeNode_t *);
/** Type definition for a function pointer that is able to redirect calls to printf */
typedef int                     (*ipdPrintf_f)(void *, const char *, ...);
/** Input file list */
typedef struct ipdIFL_s         ipdIFL_t;

/*=======================================================================
	Enums
=======================================================================*/

/*********************************/
/*           Main Type           */
/*********************************/
/** 
* Type identifier for the different unit types stored within an ipd tree
*/
enum
{
   ipdINVALID            =        0,

   ipdINTEGER            =  (1<< 0),
   ipdREAL               =  (1<< 1),
   ipdCOMPLEX            =  (1<< 2),
   ipdQUANTITY           =  (1<< 3),
   ipdBOOLEAN            =  (1<< 4),
   ipdSTRING             =  (1<< 5),
   ipdARRAYVAL           =  (1<< 6),
   ipdREALQUANTITY       =  (ipdREAL    | ipdQUANTITY),
   ipdCOMPLEXQUANTITY    =  (ipdCOMPLEX | ipdQUANTITY),

   ipdARRAY              =  (1<< 7),
   ipdINDEX              =  (1<< 8),

   ipdSECTION            =  (1<< 9),
   ipdVARIABLE           =  (1<<10),
   ipdVARIABLE_REF       =  (1<<11),

   ipdOPERATOR           =  (1<<12),

   ipdINTERNFUNCTION     =  (1<<13),
   ipdCCODEDFUNCTION     =  (1<<14),

   ipdPARAMETER          =  (1<<15),
   ipdOPTION             =  (1<<16), /* optional parameter */
   ipdPARAMETER_REF      =  (1<<17),

   ipdINCLUDE            =  (1<<18),

   ipdSECTION_REF        =  (1<<19),

   ipdALIAS              =  (1<<20),

#if !IPD_EVAL_DELETES_EXPR
   ipdEVAL               =  (1<<21),
#endif

   ipdVALUE              =  (ipdINTEGER | ipdREAL | ipdCOMPLEX | ipdQUANTITY | ipdBOOLEAN |
			     ipdSTRING),
   ipdARRAYnVALUE        =  (ipdARRAYVAL | ipdVALUE),
   ipdIPDNOFUNCVAL          =  (ipdQUANTITY | ipdSTRING | ipdBOOLEAN | ipdSECTION),

   ipdANY                =  (ipdSECTION | ipdVARIABLE)
};
typedef long ipdType_t;

/**
*       Expression Flags
*/
typedef long ipdExprFlags_t;

/**
*        Variable Flags
*/
typedef long ipdVarFlags_t;

/**
*         Section Flags
*/
typedef long ipdSectFlags_t;

/**
*  Section and Variable Flags
*/
typedef long ipdSVFlags_t;

/**
*        Include Flags
*/
enum
{
   ipdIF_NOFLAG          = 0,
   ipdIF_WITH_SEARCHPATH = 1
};
typedef long ipdInclFlags_t;

/*=======================================================================
        Data Structures
=======================================================================*/

/*********************************/
/*      Tree Node Structure      */
/*********************************/

/**
*      Tree Node Structure
*/
struct ipdTreeNode_s
{
   ipdTreeNode_t *next;       /** next sibling in the parents section */

   char          *comment;    /** comment given in the ipd-File */

   ipdBoolean       inquired;   /** this tree-node has been inquired by the application */

   union
   {
      /** Expression */
      struct
      {
			 /** Values */
			 union
			 {
			    long               integer;   /** ipdINTEGER          */
			    struct
			    {
			       double          re, im;    /** ipdREAL, ipdCOMPLEX */
		#if IPD_USE_UNITS
			       ipdUnit_t      *unit;      /** ipdQUANTITY         */
		#endif /* IPD_USE_UNITS */
			    } quantity;
			    char              *string;    /** ipdSTRING           */
			    ipdBoolean           boolean;   /** ipdBOOLEAN          */
			    struct                        /** ipdARRAYVAL         */
			    {
			       ipdTreeNode_t **tree;      /** array of values */
			       long            len;       /** array length    */
			    } arrayval;
			 } val;

			 /** Functions */
			 union
			 {
			    /** ipdOPERATOR */
			    struct
			    {
			       ipdFunc_t       opPtr;       /** Operator pointer */
			       ipdTreeNode_t  *operand;     /** Operands are a SLL concatenated by next in operand */
			       ipdTreeNode_t  *lastOperand; /** Last operand in the SLL */
			       long            nOperands;   /** Number of operands */
			    } op;
			    /** ipdINTERNFUNCTION */
			    struct
			    {
			       iiInternFunc_t *func;     /** the IPL function definition */
			       ipdTreeNode_t  *param;    /** Parameters are a SLL concatenated by next in param */
			       char           *funcName; /** don't free.                 */
			    } internFunc;
			    /** ipdCCODEDFUNCTION */
			    struct
			    {
			       ipdFunc_t       func;     /** extern C-function */
			       ipdTreeNode_t  *param;    /** Parameters are a SLL concatenated by next in param */
			       void           *userData;
			       char           *funcName;
			       ipdUserFunc_t   userDestructor;
			    } cCodedFunc;
			    /** ipdARRAY */
			    /** Not in expr, to be able to generate arrays of arrays. */
			    ipdTreeNode_t *array;     /** array of trees (implemented as list) */
			    /** ipdEVAL */
		#if !IPD_EVAL_DELETES_EXPR
			    ipdTreeNode_t  *evalTree;     /** Tree of a ipdEVAL structure */
		#endif
			    /* ipdVARIABLE_REF */
			    struct
			    {
			       char          *name;       /** points to the name of fullname */
			    } varref;
			    /** ipdPARAMETER_REF */
			    struct
			    {
			       ipdTreeNode_t *tree;
			       char          *name;
			    } pararef;

			    /** ipdINDEX */
			    struct
			    {
			       ipdTreeNode_t  *tree;           /** the array   */
			       ipdTreeNode_t  *index;          /** array index */
			       ipdTreeNode_t  *next;           /** next index  */
			    } arrayIndex;
			 } func;
			 ipdExprFlags_t  eFlags;
      } expr;

      struct /* Sections, variables */
      {
			 char          *name;       /** points to the name of fullname */
			 char          *fullname;

			 ipdTreeNode_t *parent;     /** parent section */
			 ipdTreeNode_t *prev;       /** prev sibling in the parents section */

			 ipdIFL_t      *file;       /** file where the item was loaded from */
			 long           lineno;     /** line number within the file */

			 ipdSVFlags_t   flags;      /** flags common to all these following items. */

			 union /** Sections, variables : unit */
			 {
			    /** ipdVARIABLE */
			    struct
			    {
			       ipdTreeNode_t  *tree;
			       ipdVarFlags_t   flags;
			       ipdTreeNode_t  *traceTree;
			       long            lock;
			    } var;

			    /** ipdSECTION */
			    struct
			    {
			       ipdTreeNode_t  *child;       /** first subsection */
			       ipdTreeNode_t  *lastChild;   /** last subsection, may be NULL if not set */
			       ipdTreeNode_t  *history;     /** is used to store the previous current section */
			       /** The sections this node was inherited FROM: */
			       ipdTreeNode_t  *ancestor;    /** all ancestors */
			       ipdSectFlags_t  flags;
			    } sect;

			    /** ipdINCLUDE */
			    struct
			    {
			       ipdIFL_t       *file;       /** ipdvosFile pointer to the vos structur of the file */
			       ipdInclFlags_t  flags;      /** to open with/without searchpaths. */
			       char           *string;     /** The include file */
			    } icld;
			    /** ipdALIAS */
			    struct
			    {
			       ipdTreeNode_t *condTree;
			       ipdTreeNode_t *sectRef;
			    } alias;
			 } svu;
      } sv;

      /** ipdSECTION_REF */
      struct
      {
			 ipdTreeNode_t *condTree;
			 ipdBoolean       isActive;
			 char          *name;
      } sectRef;

      /** ipdPARAMETER : The parameters of an ipdINTERNFUNCTION */
      struct
      {
			 ipdTreeNode_t *tree;
			 char          *name;
      } parameter;

      /** ipdOPTION */
      struct
      {
			 ipdTreeNode_t *tree;
			 char          *name;
      } option;
   } node;

   ipdType_t   type;          /** what's stored in union{..}value */
};

/*********************************/
/*           Iterators           */
/*********************************/
/** Iteration over an IPD tree */
struct ipdIterator_s
{
   ipdIterator_t    *next;
   ipdTreeNode_t    *tn;
   char             *sectName;
   char             *itemName;
   ipdTreeNodeDll_t *tnDll;    /** FIFO */
   ipdTreeNodeDll_t *tnDllPos; /** Position in the fifo. */
   ipdBoolean          isInit;
   ipdType_t         type;     /** if not used as iterator ipdINVALID */
};

/** Iteration over values associated with a IPD tree item */
struct ipdValueIterator_s
{
   ipdTreeNode_t *tn;
   ipdLong          index; /** index, if (normally) we have an array */
};

/*=======================================================================
        Function Declarations
=======================================================================*/

/*********************************/
/*       Memory Operations       */
/*********************************/
/** @brief Fills memory of the given size (implicit) with zeros. 
    @param mem Pointer to the beginning of the memory chunk
*/
void              ipdMBclear(void *mem);

/**  @brief Frees the memory block pointed to by 's' if not NULL to the memory block buffer hash!!
    @param s Pointer to the beginning of the memory chunk
*/
void              ipdMBfree(const void *s);

#if IPD_MEMORY_ALLOC_ipdDEBUG_INFO
#define         ipdMBalloc(size) ipdMBallocDB(size,__FILE__,__LINE__)
/** @brief Malloc a string buffer. Gets some from the buffer hash or mallocs it.
    @param size Size of the requested memory in bytes
    @param filename Filename from which this function is called (automatically added via makro)
    @param lineno Line number from which this function is called (automatically added via makro)
    @return Void-pointer to the first byte of the allocated memory
 */
  void           *ipdMBallocDB(unsigned long size, ipdConstString filename, ipdLong lineno);

#else  /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */
/** @brief Malloc a string buffer. Gets some from the buffer hash or mallocs it.
    @param size Size of the requested memory in bytes
    @return Void-pointer to the first byte of the allocated memory
 */
  void           *ipdMBalloc(unsigned long size);

#endif /* IPD_MEMORY_ALLOC_ipdDEBUG_INFO */

/*********************************/
/*        Administration         */
/*********************************/

/** @brief Returns a string which discribes a given ipd type
    @param type The type
    @param onlyValues Boolean flag whether to check only value types
    @return Pointer to the first character of the string
*/
const char      *ipdGetTypeStr(ipdType_t type, ipdBoolean onlyValues);

/** @brief Test for an error in the evaluation

    @param d Value to be checked for a valid result
    @param str Error string
    @return ipdTRUE if error occured
*/
ipdBoolean         ipdTestOnNumError (double d, const char *str);

/*********************************/
/*          Calculation          */
/*********************************/
/** @brief Starts the calculation of an expression or a variable.
    @param tn A tree node item
    @param sectName Name of the section
    @return Result of the evaluation
  */
ipdTreeNode_t   *ipdDoCalculation(ipdTreeNode_t *tn, const char *sectName);

/*********************************/
/*        Logging                */
/*********************************/
/** @brief Prints a message to the logging system
*   @param file The file to print in
*   @param fstr Formatting string
*/
void             ipdLogPrintf(FILE *file, const char *fstr, ...);

/** @brief Prints a message to the standard logging system
*   @param fstr Formatting string
*/
void             ipdStdLog(const char *fstr, ...);

/** @brief Set or unselect the printing of inheritance information
*   @param nInfo Boolean flag
*/
void             ipdSetInheritanceInfo(ipdBoolean const nInfo); /* [SW] */

/*********************************/
/*        Error Handling         */
/*********************************/
/**  @brief Resets the error status */
void             ipdResetErrorStatus(void);

/**  @brief Get the current error status 
     @return Returns true if there was an error
*/
ipdBoolean         ipdGetErrorStatus(void);

/**  @brief Get the latest error from the error stack
     @return Returns true if there was an error
 */
ipdBoolean         ipdPopError(void);

/**  @brief Get the error number of the error
     @return Return the type of the error, @see ipdERR_Type_t
*/
ipdERR_Type_t    ipdGetErrorNumber(void);

/**  @brief Returns the error message of the current error
     @return The error string
*/
ipdConstString     ipdGetErrorMessage(void);

/**  @brief Discard all errors and messages that have occurred so far */
void             ipdResetError(void);

/**  @brief Concatenates a string to the existing error string.
     @param str The string where the error should be appended
     @param n Maximum number of characters to be appended
*/
void             ipdErrorStrCatN(ipdConstString str, long n);

/**  @brief Adds the filename and the line number to the existing error message.
     @param tn A tree node item
*/
void             ipdAddFileLine2ErrorString(ipdTreeNode_t *tn);

/**  @brief Throws an error identifed by errNum
     @param errNum The error number associated with the thrown error
*/
void             ipdSetError(ipdERR_Type_t errNum, ...);

/*********************************/
/*        Initialization         */
/*********************************/
/**  @brief Read an input deck into the parser's memory and parse it.
*
* Read an input deck into the parser's memory and parse it. This file must
* be written using the Input Deck Programming Language IPL.
* To be called for more than one time if there are more files to be parsed.
* Better: use include-directive
*
*    @param filename Name of the input deck file
*    @return The number of successfully parsed files
*/
ipdLong            ipdReadInputDeck(ipdConstString filename);

#define IPD_ENABLE_INDIVIDUAL_EXTENSIONS 1  /* [SW] */
#if IPD_ENABLE_INDIVIDUAL_EXTENSIONS
/**  @brief Create New Inputdeck and initialize the InputDeck-internal dataBase
     @param pathList List of paths
     @param logFile The file to write log messages to
     @param extList List of possible file extensions
     @return ipdTRUE if successful
*/
ipdBoolean         ipdInitExt(const char *pathList, FILE *logFile, const char *extList);
#endif /* IPD_ENABLE_INDIVIDUAL_EXTENSIONS */

/**  @brief Initialize the IPD
     @param pathList List of paths
     @param logFile The logfile to write in
     @return ipdTRUE if successful
*/
ipdBoolean         ipdInit(const char *pathList, FILE *logFile);

/**  @brief Creates a new inputdeck.
     @param name Name of the inputdeck
     @param hashSize Hash size
     @return Index of the newly created database
*/
ipdLong            ipdCreateBase(ipdConstString name, ipdLong hashSize);

/**  @brief Set the current ipd database as base
     @param index Index of the specific ipd database
     @return ipdTRUE if successful
*/
ipdBoolean         ipdSetBase(ipdLong index);

/**  @brief Creates a C-coded function.
     @param funcName Name of the function (No path!! Only the name).
     @param cFunc The C-function
     @param userData Pointer to user
     @param userDestructor Pointer to a destructor function
*/
void             ipdCreateExternFunction(char *funcName,
					 ipdFunc_t cFunc,
					 void *userData,
					 ipdUserFunc_t userDestructor);

/**  @brief Returns the optional parameter with the given name
     @param tn A tree node item
     @param name Name of the requested option
     @return The optional parameter identified by name
*/
ipdTreeNode_t   *ipdGetOptionByName(ipdTreeNode_t *tn,
				    const char *name);

/**  @brief Returns the parameter with the given index
     @param tn A tree node item
     @param index Index of the requested option
     @return The optional parameter identified by index
*/
ipdTreeNode_t   *ipdGetParamByIndex(ipdTreeNode_t *tn,
				    ipdLong          index);

/**  @brief Prints list of valid options for a function.
     @param funcName Name of the function
     @param validOptions A list of valid options
*/
ipdVoid            ipdPrintOptions(const char *funcName,
				 const char *validOptions[]);

/**  @brief Checks options. If a wrong option has been given in the input files an error is thrown.

     All valid options known to the current function must be given by an NULL-terminated array of strings in validOptions.
     @param funcName Name of the function
     @param param The parameter
     @param validOptions A list of options
*/
ipdBoolean         ipdCheckOptions(const char    *funcName,
				 ipdTreeNode_t *param,
				 const char    *validOptions[]);

/**  @brief Counts the number of parameters in a parameter list. Options are ignored.
     @param tn A tree node item
     @return The number of parameters
*/
ipdLong            ipdGetParamNum(ipdTreeNode_t *tn);

/*********************************/
/*        Free Inputdeck         */
/*********************************/
/**  @brief Frees the current input deck. */
void             ipdFreeCurrentIpd(void);

/**  @brief Frees all input deck structures. Use this before exit.
 *
 * [JW] I've replaced the ipdFreeAll() function with the 
 * ipdFreeAllInDetail() as imho there should only exist the
 * ipdFreeAllInDetail(). It may be slower, but it frees all structures.
 * Further note, that I've deleted the old ipdFreeAll() function and
 * renamned: ipdFreeAllInDetail() --> ipdFreeAll()
 */
void             ipdFreeAll(void);

#define ipdFreeAllInDetail() ipdFreeAll() /* [JW] preserving the interface ..*/

/*********************************/
/*        Current Section        */
/*********************************/
/**  @brief Sets the current section by its name.
     @param CSFullName Name of the section
     @return full name of the section
*/
const char      *ipdSetCSByName(const char *CSFullName);

/**  @brief Gets the name of the current section.

     We can't store a current section pointer because the current section may be virtual - not really existing.
     @return String holding the name
*/
const char      *ipdGetCSName(void);

/**  @brief Pushes the current section (its fullname) on a stack
     @return full name of the pushed section
*/
const char      *ipdPushCS(void);

/**  @brief Pops the current section (its fullname) from a stack.
     @return Name of the new current section
*/
const char      *ipdPopCS(void);

/*********************************/
/*        Finding Items          */
/*********************************/
/**  @brief Test an item by its name, whether it exists or not.
     @param name The key name
     @param it Type of the requested item
     @param doFindAuxiliaries Boolean flag, whether also auxiliary variables should be returned
     @return Returns the item by a pointer to ipdTreeNode_t or 0 on error.
*/
ipdTreeNode_t   *ipdExistItemByName(const char *name,
				    ipdType_t   it,
				    ipdBoolean    doFindAuxiliaries);


/**  @brief Finds a variable identified by a name, @see ipdExistItemByName() */
#define ipdExistVariableByName(varName) ipdExistItemByName(varName,  ipdVARIABLE, ipdFALSE)

/**  @brief Finds a section identified by a name, @see ipdExistItemByName() */
#define ipdExistSectionByName(sectName) ipdExistItemByName(sectName, ipdSECTION,  ipdFALSE)

/*********************************/
/*       Iterator Handling       */
/*********************************/
#define          ipdIteratorItem(i)  (((i)->tn))

/**  @brief Frees an iterator node. Use this function for freeing!
     @param iNode An ipd iterator
*/
void             ipdIteratorFree(ipdIterator_t *iNode);

/**  @brief Cleans an iterator for next use.
     @param iNode An ipd iterator
*/
void             ipdIteratorClean(ipdIterator_t *iNode);

/**  @brief Checks whether the iterator is still valid
     @param iNode An ipd iterator
     @return ipdTRUE if iterator is valid
*/
ipdBoolean         ipdIteratorIsValid(ipdIterator_t *iNode);

/**  @brief Returns the name of the section the iterator is pointing to.
     @param iNode An ipd iterator
     @return The section name
*/
ipdConstString     ipdIteratorGetSectionName(ipdIterator_t *iNode);

/**  @brief Returns the name of the item stored in the iterator.
     @param iNode An ipd iterator
     @return Name of the item as string
*/
ipdConstString     ipdIteratorGetItemFullName(ipdIterator_t *iNode);

/**  @brief Get the name of a section of a variable
     @param iNode An ipd iterator
     @return Name of the item as string
*/
ipdConstString     ipdIteratorGetItemName(ipdIterator_t *iNode);

/** @brief  Returns the name of the item stored in the section tree.

    Returns the name of the item stored in the section tree.
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !!!!! Just for internal use !!!!!!
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    E.g.:
    A { x=1; }
    B : A;
    Iterator inside section B
    ipdIteratorGetStoredItemFullName gives '~A.x' for item 'x' in section 'B'
    @param iNode An ipd iterator
*/
ipdConstString     ipdIteratorGetStoredItemFullName(ipdIterator_t *iNode); /* internal use!!! */

/**  @brief Sets the type of the iterator.
     @param iNode An ipd iterator
     @param iteratorType Either ipdVARIABLE or ipdSECTION
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorSetType(ipdIterator_t *iNode, ipdType_t iteratorType);

/**  @brief Get the iterator type
     @param iNode An ipd iterator
     @return Type of the iterator, ipdINVALID if iNode is malformed
*/
ipdType_t        ipdIteratorGetType(ipdIterator_t *iNode);

/**  @brief Overwrites a given iterator.

     Finds an item by its name. A valid iterator will be returned. The type of the
     item can be specified by searchType. ipdANY finds an item of any type.
     iterator type = setType!!!
     @param iNode An ipd iterator
     @param itemName The relative or absolute pathname of the item
     @param searchType Type of the item ipdANY finds an item of any type.
     @param setType Sets the type of the resulting iterator
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorSetByName(ipdIterator_t *iNode,
				      const char *itemName,
				      ipdType_t     searchType,
				      ipdType_t     setType);

/** @brief Creates a new iterator.

    Finds an item by its name. A valid iterator will be returned. 
    iterator type = setType!!!
    @param iNode_p Pointer to an ipd iterator
    @param itemName The relative or absolute pathname of the item
    @param searchType Type of the item. ipdALL finds an item of any type.
    @param setType Sets the type of the resulting iterator
    @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorNewByName(ipdIterator_t **iNode_p, 
					 const char   *itemName, 
					 ipdType_t     searchType,
					 ipdType_t     setType);

/** @brief Creates a new iterator on the ipdTreeNode_t structure tn.

    @param iNode_p Pointer to an ipd iterator
    @param tn A tree node item of type searchType
    @param searchType Type of the item (tree node). ipdALL finds an item of any type.
    @param setType Sets the type of the resulting iterator
    @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorNewByTN(ipdIterator_t **iNode_p, 
			   ipdTreeNode_t  *tn, 
			   ipdType_t       searchType,
			   ipdType_t       setType);

/**  @brief Creates a new iterator and sets the iterator to the root section.
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorNewAtRootSection(ipdIterator_t **iNode_p,
					       ipdType_t     setType);

/** @brief Overwrites a given iterator and stores the next item found. Does NOT step into sections.

    But if the item is a section, ipdIteratorDoNext() will NOT step into it.
    The iterator type remains unchanged.
    @param iNode An ipd iterator
    @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorDoNext(ipdIterator_t *iNode);

/** @brief Overwrites a given iterator and stores the next item found. WILL step into sections.
 
    But if the item is a section, ipdIteratorDoStep() WILL step into it.
    The iterator type remains unchanged.
    @param iNode An ipd iterator
    @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorDoStep(ipdIterator_t *iNode);

/** @brief Overwrites a given iterator and goes to the parent section
 
    Goes one section up (i.e. to the parent section).
    If the current section is the root section, this function does nothing.
    The iterator type remains unchanged.
    @param iNode An ipd iterator
    @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorGoUp(ipdIterator_t *iNode);

/**  @brief Overwrites a given iterator so that it points to the root section.

     The iterator type remains unchanged.
     @param iNode An ipd iterator
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorGotoRootSection(ipdIterator_t *iNode);

/**  @brief Tests if the iterator type matches the type given.
     @param iNode An ipd iterator
     @param itemType The type to which the iterator is compared.
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorTestType(ipdIterator_t *iNode, ipdType_t itemType);
		 
/** @brief Returns a iterator which can be used to analyse Inputdeck-lists.

    To analyse simple lists, just call ipdValueIteratorNext()
    The type of an element can be found by ipdValueIteratorGetType()
    If an element is a list again, (so its type is ipdARRAYVAL) call ipdValueIteratorStep(); It will step into this sub-list.
    Stepping back or stepping up is not possible.
    ipdValueIteratorIsValid() returns whether the iterator is still valid or not.
    ipdValueIteratorSubscript() jumps to the index within the list (array).
    !!!
    !!! Please evaluate tn before!
    !!!
    @param iVNode_p Pointer to the new ipd value iterator
    @param tn A tree node item
    @return A boolean indicating success of the operation
*/
ipdBoolean         ipdValueIteratorNew(ipdValueIterator_t **iVNode_p, 
				     ipdTreeNode_t       *tn);

/**  @brief Returns a iterator which can be used to analyse Inputdeck-lists.

    To analyse simple lists, just call ipdValueIteratorNext()
    The type of an element can be found by ipdValueIteratorGetType()
    If an element is a list again, (so its type is ipdARRAYVAL) call ipdValueIteratorStep(); It will step into this sub-list.
    Stepping back or stepping up is not possible.
    ipdValueIteratorIsValid() returns whether the iterator is still valid or not.
    ipdValueIteratorSubscript() jumps to the index within the list (array).
    !!!
    !!! Evaluate the iterator iNode before!
    !!!
    @param iVNode_p Pointer to the new ipd value iterator
    @param iNode An ipd iterator (must be evaluated!)
    @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorValueIteratorNew(ipdValueIterator_t **iVNode_p, 
					     ipdIterator_t       *iNode);

/**  @brief Increment the ipd value iterator. Does NOT step into sub-lists.
     @param iVNode An ipd value iterator
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdValueIteratorNext(ipdValueIterator_t *iVNode);

/**  @brief Increment the ipd value iterator. WILL step into sub-lists.
     @param iVNode An ipd value iterator
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdValueIteratorStep(ipdValueIterator_t *iVNode);

/**  @brief Check whether the ipd value iterator is still valid
     @param iVNode An ipd value iterator
     @return Boolean: ipdTRUE if iterator is valid
*/
ipdBoolean         ipdValueIteratorIsValid(ipdValueIterator_t *iVNode);

/**  @brief Jumps to the index within the value list (array)
     @param iVNode An ipd value iterator
     @param index The index within the array
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdValueIteratorSubscript(ipdValueIterator_t *iVNode,
					   ipdLong               index);

/** @brief Get the type of the value iterator
    @param iVNode An ipd value iterator
    @return The type of the element the iterator is pointing to
*/
ipdType_t        ipdValueIteratorGetType(ipdValueIterator_t *iVNode);
		 
/*********************************/
/*           Evaluation          */
/*********************************/
/**  @brief Evaluates an expression pointed to by an iterator.
     @param iNode An ipd iterator
     @return  The structure returned stores the result.
*/
ipdTreeNode_t   *ipdIteratorEval(ipdIterator_t *iNode);

/**  @brief Evaluates an expression stored on an item identified by the supplied string
     @param name Name of the item
     @param doFindAuxiliaries Should the function also consider auxiliary variables
     @return The structure returned stores the result.
*/
ipdTreeNode_t   *ipdEvalByName(const char *name,
			       ipdBoolean    doFindAuxiliaries);

/**  @brief Returns the value type (array or number) of an item
    @param tn A tree node item
     @return The value type of the item
*/
ipdType_t        ipdGetValueType(const ipdTreeNode_t *tn);

/**  @brief Returns the type of an item (not necessarily an array or number, cf. @see ipdGetValueType() )
    @param tn A tree node item
     @return The type of the tree node
*/
ipdType_t        ipdGetType(ipdTreeNode_t *tn);

/*********************************/
/*        Get Item Values        */
/*********************************/
/**  @brief Get an integer from an item
    @param tn A tree node item
     @param integer_p Pointer to the object where the integer is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetInteger(ipdTreeNode_t *tn,
			       ipdLong         *integer_p);

/**  @brief Get an integer from a tree node using an iterator?
     @param iNode An ipd iterator
     @param integer_p Pointer to the object where the integer is to be stored in
     @return A boolean indicating success of the operation
*/

ipdBoolean         ipdIteratorGetInteger(ipdIterator_t *iNode,
				       ipdLong         *integer_p);

/**  @brief Get an integer from a tree node using a value iterator?
     @param iVNode A value iterator
     @param integer_p Pointer to the object where the integer is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdValueIteratorGetInteger(ipdValueIterator_t *iVNode,
					    ipdLong              *integer_p);

/**  @brief Get an integer identified by a name
     @param name A unique identifier string of the integer
     @param integer_p Pointer to the object where the integer is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetIntegerByName(const char *name,
				     ipdLong      *integer_p);



/**  @brief Get a real number (no integer!) from a tree node
    @param tn A tree node item
     @param re_p Pointer to the object where the real number is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetReal(const ipdTreeNode_t *tn,
			    ipdDouble             *re_p);

/**  @brief Get a real number (no integer!) using an ipd iterator
     @param iNode An ipd iterator
     @param re_p Pointer to the object where the real number is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorGetReal(const ipdIterator_t *iNode,
				    ipdDouble             *re_p);

/**  @brief Get a real number (no integer!) using a value iterator
     @param iVNode A value iterator
     @param re_p Pointer to the object where the real number is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdValueIteratorGetReal(ipdValueIterator_t *iVNode,
					 ipdDouble            *re_p);

/**  @brief Get a real number (no integer!) identified by a name
     @param name A unique identifier string of the real number
     @param re_p Pointer to the object where the real number is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetRealByName(const char *name,
				  ipdDouble    *re_p);


/**  @brief Get a real number from a tree node, where integers are also considered as real numbers, @see ipdGetReal
    @param tn A tree node item
     @param re_p Pointer to the object where the real number is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetSloppyReal(ipdTreeNode_t *tn,
				  ipdDouble       *re_p);

/**  @brief Get a real number from a tree node using an ipd iterator, where integers are also considered as real numbers, @see ipdIteratorGetReal
     @param iNode An ipd iterator
     @param re_p Pointer to the object where the real number is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorGetSloppyReal(const ipdIterator_t *iNode,
					  ipdDouble             *re_p);

/**  @brief Get a real number from a tree node using a value iterator, where integers are also considered as real numbers, @see ipdValueIteratorGetReal
     @param iVNode A value iterator
     @param re_p Pointer to the object where the real number is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdValueIteratorGetSloppyReal(ipdValueIterator_t *iVNode,
					       ipdDouble            *re_p);

/**  @brief Get a real number from a tree node using a unique identifier, where integers are also considered as real numbers, @see ipdGetRealByName
     @param name The unique identifier string of the requested value
     @param re_p Pointer to the object where the real number is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetSloppyRealByName(const char *name,
					ipdDouble     *re_p);


/**  @brief Get an array of real numbers stored on a tree node, where integers are also considered as real numbers. The array must be free'd using free() !! 
    @param tn A tree node item
     @param len_p Pointer to the object where the length of the array is to be stored in
     @param rearray_p Pointer where the pointer to the real array is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetSloppyRealArray(ipdTreeNode_t  *tn,         /* [I] */
				       ipdLong          *len_p,         /* [O] */
				       ipdDouble       **rearray_p);    /* [O] */

/**  @brief Get an array of real numbers from an ipd iterator, where integers are also considered as real numbers. The array must be free'd using free() !! 
     @param iNode An ipd iterator
     @param len_p Pointer to the object where the length of the array is to be stored in
     @param rearray_p Pointer where the pointer to the real array is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorGetSloppyRealArray(const ipdIterator_t  *iNode,       /* [I] */
					       ipdLong                *len_p,       /* [O] */
					       ipdDouble             **rearray_p);  /* [O] */

/**  @brief Get an array of real numbers from a value iterator, where integers are also considered as real numbers. The array must be free'd using free() !! 
     @param iVNode A value iterator
     @param len_p Pointer to the object where the length of the array is to be stored in
     @param rearray_p Pointer where the pointer to the real array is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdValueIteratorGetSloppyRealArray(ipdValueIterator_t  *iVNode,     /* [I] */
						    ipdLong               *len_p,      /* [O] */
						    ipdDouble            **rearray_p); /* [O] */

/**  @brief Get an array of real numbers identified by a string, where integers are also considered as real numbers. The array must be free'd using free() !! 
     @param name A unique identifier string
     @param len_p Pointer to the object where the length of the array is to be stored in
     @param rearray_p Pointer where the pointer to the real array is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetSloppyRealArrayByName(ipdConstString   name,       /* [I] */
					     ipdLong         *len_p,      /* [O] */
					     ipdDouble      **rearray_p); /* [O] */



/**  @brief Get a matrix/tensor with real valued entries from a tree node, where integers are also considered as real numbers. Free *len_p and *matrix_p with free() !!!!
     @param tn A tree node item
     @param dim_p Pointer to the object where the number of dimensions of the tensor is to be stored in
     @param len_p Pointer to the object where the length of each dimension is to be stored in
     @param matrix_p Pointer to the memory where matrix entries are to be stored in
     @param unit Pointer to the memory where the unit of the object is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetSloppyRealMatrix(ipdTreeNode_t  *tn,        /* [I] */
					ipdLong          *dim_p,     /* [O] */
					ipdLong         **len_p,     /* [O] */
					ipdDouble       **matrix_p,  /* [O] */
					ipdConstString   *unit);     /* [O] */

/**  @brief Get a matrix/tensor with real valued entries from an ipd iterator, where integers are also considered as real numbers. Free *len_p and *matrix_p with free() !!!!
     @param iNode An ipd iterator
     @param dim_p Pointer to the object where the number of dimensions of the tensor is to be stored in
     @param len_p Pointer to the object where the length of each dimension is to be stored in
     @param matrix_p Pointer to the memory where matrix entries are to be stored in
     @param unit Pointer to the memory where the unit of the object is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorGetSloppyRealMatrix(const ipdIterator_t  *iNode,     /* [I] */
						ipdLong                *dim_p,     /* [O] */
						ipdLong               **len_p,     /* [O] */
						ipdDouble             **matrix_p,  /* [O] */
						ipdConstString         *unit);     /* [O] */

/**  @brief Get a matrix/tensor with real valued entries from a value iterator, where integers are also considered as real numbers. Free *len_p and *matrix_p with free() !!!!
     @param iVNode A value iterator
     @param dim_p Pointer to the object where the number of dimensions of the tensor is to be stored in
     @param len_p Pointer to the object where the length of each dimension is to be stored in
     @param matrix_p Pointer to the memory where matrix entries are to be stored in
     @param unit Pointer to the memory where the unit of the object is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdValueIteratorGetSloppyRealMatrix(const ipdValueIterator_t  *iVNode,    /* [I] */
						     ipdLong                     *dim_p,     /* [O] */
						     ipdLong                    **len_p,     /* [O] */
						     ipdDouble                  **matrix_p,  /* [O] */
						     ipdConstString              *unit);     /* [O] */

/**  @brief Get a matrix/tensor with real valued entries identified by a string, where integers are also considered as real numbers. Free *len_p and *matrix_p with free() !!!!
     @param name A unique identifier string
     @param dim_p Pointer to the object where the number of dimensions of the tensor is to be stored in
     @param len_p Pointer to the object where the length of each dimension is to be stored in
     @param matrix_p Pointer to the memory where matrix entries are to be stored in
     @param unit Pointer to the memory where the unit of the object is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetSloppyRealMatrixByName(ipdConstString   name,      /* [I] */
					      ipdLong         *dim_p,     /* [O] */
					      ipdLong        **len_p,     /* [O] */
					      ipdDouble      **matrix_p,  /* [O] */
					      ipdConstString  *unit);     /* [O] */


/**  @brief Get a real quantity (i.e. real number plus unit) from a tree node item
     @param tn A tree node item
     @param re_p Pointer to the object where the real number is to be stored in
     @param unit_p Pointer to the memory where the unit string of the object is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetRealQuantity(ipdTreeNode_t  *tn,
				    double         *re_p,
				    char          **unit_p);

/**  @brief Get a real quantity (i.e. real number plus unit) using an ipd iterator
     @param iNode An ipd iterator
     @param re_p Pointer to the object where the real number is to be stored in
     @param unit_p Pointer to the memory where the unit string of the object is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorGetRealQuantity(const ipdIterator_t  *iNode,
					    ipdDouble              *re_p,
					    ipdChar               **unit_p);

/**  @brief Get a real quantity (i.e. real number plus unit) using a value iterator
     @param iVNode A value iterator
     @param re_p Pointer to the object where the real number is to be stored in
     @param unit_p Pointer to the memory where the unit string of the object is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdValueIteratorGetRealQuantity(const ipdValueIterator_t  *iVNode,
						 ipdDouble                   *re_p,
						 ipdChar                    **unit_p);

/**  @brief Get a real quantity (i.e. real number plus unit) identified by a string
     @param name A unique identifier string
     @param re_p Pointer to the object where the real number is to be stored in
     @param unit_p Pointer to the memory where the unit string of the object is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetRealQuantityByName(const char  *name,
					  double      *re_p,
					  char       **unit_p);


/**  @brief Get a real quantity (i.e. real number plus unit) from a tree node item, where integer quantities are also considered as real quantities.
     @param tn A tree node item
     @param re_p Pointer to the object where the real number is to be stored in
     @param unit_p Pointer to the memory where the unit string of the object is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetSloppyRealQuantity(ipdTreeNode_t  *tn,
					  double         *re_p,
					  char          **unit_p);

/**  @brief Get a real quantity (i.e. real number plus unit) using an ipd iterator, where integer quantities are also considered as real quantities.
     @param iNode An ipd iterator
     @param re_p Pointer to the object where the real number is to be stored in
     @param unit_p Pointer to the memory where the unit string of the object is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorGetSloppyRealQuantity(const ipdIterator_t  *iNode,
						  ipdDouble              *re_p,
						  ipdChar               **unit_p);

/**  @brief Get a real quantity (i.e. real number plus unit) using a value iterator, where integer quantities are also considered as real quantities.
     @param iVNode A value iterator
     @param re_p Pointer to the object where the real number is to be stored in
     @param unit_p Pointer to the memory where the unit string of the object is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdValueIteratorGetSloppyRealQuantity(const ipdValueIterator_t  *iVNode,
						       ipdDouble                   *re_p,
						       ipdChar                    **unit_p);

/**  @brief Get a real quantity (i.e. real number plus unit) identified by a string, where integer quantities are also considered as real quantities.
     @param name A unique identifier string
     @param re_p Pointer to the object where the real number is to be stored in
     @param unit_p Pointer to the memory where the unit string of the object is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetSloppyRealQuantityByName(const char  *name,
						double      *re_p,
						char       **unit_p);



/**  @brief Get a complex number from a tree node item
     @param tn A tree node item
     @param re_p Pointer to the object where the real part is to be stored in
     @param im_p Pointer to the object where the imaginary part is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetComplex(ipdTreeNode_t *tn,
			       double        *re_p,
			       double        *im_p);

/**  @brief Get a complex number from an ipd iterator
     @param iNode An ipd iterator
     @param re_p Pointer to the object where the real part is to be stored in
     @param im_p Pointer to the object where the imaginary part is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorGetComplex(const ipdIterator_t *iNode,
				       ipdDouble             *re_p,
				       ipdDouble             *im_p);

/**  @brief Get a complex number from a value iterator
     @param iVNode A value iterator
     @param re_p Pointer to the object where the real part is to be stored in
     @param im_p Pointer to the object where the imaginary part is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdValueIteratorGetComplex(const ipdValueIterator_t *iVNode,
					    ipdDouble                  *re_p,
					    ipdDouble                  *im_p);

/**  @brief Get a complex number identified by a string
     @param name A unique identifier string
     @param re_p Pointer to the object where the real part is to be stored in
     @param im_p Pointer to the object where the imaginary part is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetComplexByName(const char *name,
				     double     *re_p,
				     double     *im_p);



/**  @brief Get a complex quantity (i.e. complex number plus unit) from a tree node item
     @param tn A tree node item
     @param re_p Pointer to the object where the real part is to be stored in
     @param im_p Pointer to the object where the imaginary part is to be stored in
     @param unit_p Pointer to the memory where the unit of the object is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetComplexQuantity(ipdTreeNode_t  *tn,
				       double         *re_p,
				       double         *im_p,
				       char          **unit_p);

/**  @brief Get a complex quantity (i.e. complex number plus unit) using an ipd iterator
     @param iNode An ipd iterator
     @param re_p Pointer to the object where the real number is to be stored in
     @param im_p Pointer to the object where the imaginary part is to be stored in
     @param unit_p Pointer to the memory where the unit of the object is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorGetComplexQuantity(const ipdIterator_t  *iNode,
					       ipdDouble              *re_p,
					       ipdDouble              *im_p,
					       ipdChar               **unit_p);

/**  @brief Get a complex quantity (i.e. complex number plus unit) using a value iterator
     @param iVNode A value iterator
     @param re_p Pointer to the object where the real number is to be stored in
     @param im_p Pointer to the object where the imaginary part is to be stored in
     @param unit_p Pointer to the memory where the unit of the object is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdValueIteratorGetComplexQuantity(const ipdValueIterator_t *iVNode,
						    ipdDouble                  *re_p,
						    ipdDouble                  *im_p,
						    ipdChar                   **unit_p);

/**  @brief Get a complex quantity (i.e. complex number plus unit) identified by a string
     @param name A unique identifier string
     @param re_p Pointer to the object where the real number is to be stored in
     @param im_p Pointer to the object where the imaginary part is to be stored in
     @param unit_p Pointer to the memory where the unit of the object is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetComplexQuantityByName(const char  *name,
					     double      *re_p,
					     double      *im_p,
					     char       **unit_p);



/**  @brief Get a character string from a tree node
     @param tn A tree node item
     @param string_p Pointer to the memory where the string is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetString(ipdTreeNode_t  *tn,
			      char          **string_p);

/**  @brief Get a character string using an ipd iterator
     @param iNode An ipd iterator
     @param string_p Pointer to the memory where the string is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorGetString(const ipdIterator_t  *iNode,
				      ipdChar               **string_p);

/**  @brief Get a character string using a value iterator
     @param iVNode A value iterator
     @param string_p Pointer to the memory where the string is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdValueIteratorGetString(const ipdValueIterator_t  *iVNode,
					   ipdChar                    **string_p);

/**  @brief Get a character string identified by a string
     @param name A unique identifier string
     @param string_p Pointer to the memory where the string is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetStringByName(const char  *name,
				    char       **string_p);



/**  @brief Get a character string from a tree node. Same as ipdGetString() but returns a pointer to an empty string instead of a NULL pointer in case the ipd string is empty.
     @see ipdGetString
     @param tn A tree node item
     @param string_p Pointer to the memory where the string is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetStringSafe(ipdTreeNode_t *tn,
				  ipdConstString  *string_p);

/**  @brief Get a character string using an ipd iterator. Same as ipdIteratorGetString() but returns a pointer to an empty string instead of a NULL pointer in case the ipd string is empty.
     @see ipdIteratorGetString
     @param iNode An ipd iterator
     @param string_p Pointer to the memory where the string is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorGetStringSafe(const ipdIterator_t *iNode,
					  ipdConstString        *string_p);

/**  @brief Get a character string using a value iterator. Same as ipdValueIteratorGetString() but returns a pointer to an empty string instead of a NULL pointer in case the ipd string is empty.
     @see ipdValueIteratorGetString
     @param iVNode A value iterator
     @param string_p PAointer to the memory where the string is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdValueIteratorGetStringSafe(const ipdValueIterator_t *iVNode,
					       ipdConstString             *string_p);

/**  @brief Get a character string identified by a string. Same as ipdGetStringByName() but returns a pointer to an empty string instead of a NULL pointer in case the ipd string is empty.
     @see ipdGetStringByName
     @param name A unique identifier string
     @param string_p Pointer to the memory where the string is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetStringByNameSafe(ipdConstString  name,
					ipdConstString *string_p);



/**  @brief Get a boolean from a tree node
     @param tn A tree node item
     @param boolean_p Pointer to the object where the boolean is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetBoolean(ipdTreeNode_t *tn,
			       ipdBoolean      *boolean_p);

/**  @brief Get a boolean using an ipd iterator
     @param iNode An ipd iterator
     @param boolean_p Pointer to the object where the boolean is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorGetBoolean(const ipdIterator_t *iNode,
				       ipdBoolean            *boolean_p);

/**  @brief Get a boolean using a value iterator
     @param iVNode An ipd iterator
     @param boolean_p Pointer to the object where the boolean is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdValueIteratorGetBoolean(const ipdValueIterator_t *iVNode,
					    ipdBoolean                 *boolean_p);

/**  @brief Get a boolean identified by a string
     @param name A unique identifier string
     @param boolean_p Pointer to the object where the boolean is to be stored in
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdGetBooleanByName(const char *name,
				     ipdBoolean   *boolean_p);

/*********************************/
/*        Set Item Values        */
/*********************************/

/**  @brief Store an integer value on a node given by an iterator
     @param iNode An ipd iterator
     @param integer The integer value which should be stored
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetInteger(ipdIterator_t *iNode, const ipdLong integer);

/**  @brief Store an integer value on a node identified by a string
     @param fullname A unique identifier string - the full name of the variable
     @param integer The integer value which should be stored
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetIntegerByName(ipdConstString fullname, const ipdLong integer);

/**  @brief Store a real value on a node given by an iterator
     @param iNode An ipd iterator
     @param re The real value which should be stored
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetReal(ipdIterator_t *iNode, const ipdDouble re);

/**  @brief Store a real value on a node identified by a string
     @param fullname A unique identifier string - the full name of the variable
     @param re The real value which should be stored
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetRealByName(ipdConstString fullname, const ipdDouble re);

/**  @brief Store a real quantity (i.e. real number plus unit) on a node given by an iterator
     @param iNode An ipd iterator
     @param re The real value which should be stored
     @param unit A string holding the unit of the object
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetRealQuantity(ipdIterator_t *iNode, const ipdDouble re, ipdConstString unit);

/**  @brief Store a real quantity (i.e. real number plus unit) on a node identified by a string
     @param fullname A unique identifier string - the full name of the variable
     @param re The real value which should be stored
     @param unit A string holding the unit of the object
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetRealQuantityByName(ipdConstString fullname, ipdDouble re, ipdConstString unit);

/**  @brief Store a complex value on a node given by an iterator
     @param iNode An ipd iterator
     @param re The real part which should be stored
     @param im The imaginary part which should be stored
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetComplex(ipdIterator_t *iNode, const ipdDouble re, const ipdDouble im);

/**  @brief Store a complex value on a node identified by a string
     @param fullname A unique identifier string - the full name of the variable
     @param re The real part which should be stored
     @param im The imaginary part which should be stored
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetComplexByName(ipdConstString fullname, const ipdDouble re, const ipdDouble im);

/**  @brief Store a complex quantity (i.e. complex number plus unit) on a node given by an iterator
     @param iNode An ipd iterator
     @param re The real part which should be stored
     @param im The imaginary part which should be stored
     @param unit A string holding the unit of the object
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetComplexQuantity(ipdIterator_t *iNode, const ipdDouble re, const ipdDouble im, ipdConstString unit);

/**  @brief Store a complex quantity (i.e. complex number plus unit) on a node identified by a string
     @param fullname A unique identifier string - the full name of the variable
     @param re The real part which should be stored
     @param im The imaginary part which should be stored
     @param unit A string holding the unit of the object
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetComplexQuantityByName(ipdConstString fullname, const ipdDouble re,
					     const ipdDouble im, ipdConstString unit);

/**  @brief Store a character string on a node given by an iterator
     @param iNode An ipd iterator
     @param string The character string which should be stored
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetString(ipdIterator_t *iNode, ipdConstString string);

/**  @brief Store a character string on a node identified by a string
     @param fullname A unique identifier string - the full name of the variable
     @param string The character string which should be stored
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetStringByName(ipdConstString fullname, ipdConstString string);

/**  @brief Store a boolean on a node given by an iterator
     @param iNode An ipd iterator
     @param boolean The boolean which should be stored
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetBoolean(ipdIterator_t *iNode, const ipdBoolean boolean);

/**  @brief Store a boolean on a node identified by a string
     @param fullname A unique identifier string - the full name of the variable
     @param boolean The boolean which should be stored
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetBooleanByName(ipdConstString fullname, const ipdBoolean boolean);



/** @brief Creates a multidimensional list of integers which is comparable to and looks similar to a matrix on a tree node item.

    Example: dim = 1 means something like this: [ 1, 2, 3, 4, 5 ]
             dim = 2 means something like this: [[ 1, 2, 3 ], [ 4, 5, 6 ]]
             dim = 3 means something like this: [[[ 1, 2 ], [ 3, 4 ]], [[ 5, 6 ], [ 7, 8 ]]]
              ...
    len = [z, y, x] for dim = 3
    @param iNode An ipd iterator
    @param dim Number of dimensions of the matrix/tensor
    @param len Pointer to an array of size dim with the number of entries in each dimension
    @param lmatrix Pointer to an array holding the matrix/tensor entries
    @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetIntegerMatrix(ipdIterator_t *iNode,
				     const ipdLong    dim,
				     const ipdLong   *len,
				     const ipdLong   *lmatrix);

/**  @brief Creates a multidimensional list of integers which is comparable to and looks similar to a matrix on a tree node item given by the full name.

    Example: dim = 1 means something like this: [ 1, 2, 3, 4, 5 ]
             dim = 2 means something like this: [[ 1, 2, 3 ], [ 4, 5, 6 ]]
             dim = 3 means something like this: [[[ 1, 2 ], [ 3, 4 ]], [[ 5, 6 ], [ 7, 8 ]]]
              ...
    len = [z, y, x] for dim = 3
     @param fullname A unique identifier string
     @param dim Number of dimensions of the matrix/tensor
     @param len Pointer to an array of size dim with the number of entries in each dimension
     @param lmatrix Pointer to an array holding the matrix/tensor entries
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetIntegerMatrixByName(ipdConstString   fullname,
					   const ipdLong    dim,
					   const ipdLong   *len,
					   const ipdLong   *lmatrix);

/**  @brief Creates a multidimensional list of real numbers which is comparable to and looks similar to a matrix on a tree node item.

    Example: dim = 1 means something like this: [ 1, 2, 3, 4, 5 ]
             dim = 2 means something like this: [[ 1, 2, 3 ], [ 4, 5, 6 ]]
             dim = 3 means something like this: [[[ 1, 2 ], [ 3, 4 ]], [[ 5, 6 ], [ 7, 8 ]]]
              ...
    len = [z, y, x] for dim = 3
     @param iNode An ipd iterator
     @param dim Number of dimensions of the matrix/tensor
     @param len Pointer to an array of size dim with the number of entries in each dimension
     @param dmatrix Pointer to an array holding the matrix/tensor entries
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetRealMatrix(ipdIterator_t *iNode,
				  const ipdLong    dim,
				  const ipdLong   *len,
				  const ipdDouble *dmatrix);

/**  @brief Creates a multidimensional list of real numbers which is comparable to and looks similar to a matrix on a tree node item given by the full name.

    Example: dim = 1 means something like this: [ 1, 2, 3, 4, 5 ]
             dim = 2 means something like this: [[ 1, 2, 3 ], [ 4, 5, 6 ]]
             dim = 3 means something like this: [[[ 1, 2 ], [ 3, 4 ]], [[ 5, 6 ], [ 7, 8 ]]]
              ...
    len = [z, y, x] for dim = 3
     @param fullname A unique identifier string
     @param dim Number of dimensions of the matrix/tensor
     @param len Pointer to an array of size dim with the number of entries in each dimension
     @param dmatrix Pointer to an array holding the matrix/tensor entries
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetRealMatrixByName(ipdConstString    fullname,
					const ipdLong     dim,
					const ipdLong    *len,
					const ipdDouble  *dmatrix);

/**  @brief Creates a multidimensional list of real quantities (i.e. real number plus unit) which is comparable to and looks similar to a matrix on a tree node item given by the full name.

    Example: dim = 1 means something like this: [ 1, 2, 3, 4, 5 ]
             dim = 2 means something like this: [[ 1, 2, 3 ], [ 4, 5, 6 ]]
             dim = 3 means something like this: [[[ 1, 2 ], [ 3, 4 ]], [[ 5, 6 ], [ 7, 8 ]]]
              ...
    len = [z, y, x] for dim = 3
     @param iNode An ipd iterator
     @param dim Number of dimensions of the matrix/tensor
     @param len Pointer to an array of size dim with the number of entries in each dimension
     @param dmatrix Pointer to an array holding the matrix/tensor entries
     @param unit A string holding the unit of the object (uniformly for all entries)
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdSetRealQuantityMatrix(ipdIterator_t *iNode,
					  const ipdLong    dim,
					  const ipdLong   *len,
					  const ipdDouble *dmatrix,
					  ipdConstString   unit);

/**  @brief Creates a multidimensional list of real quantities (i.e. real number plus unit) which is comparable to and looks similar to a matrix on a tree node item given by the full name.

    Example: dim = 1 means something like this: [ 1, 2, 3, 4, 5 ]
             dim = 2 means something like this: [[ 1, 2, 3 ], [ 4, 5, 6 ]]
             dim = 3 means something like this: [[[ 1, 2 ], [ 3, 4 ]], [[ 5, 6 ], [ 7, 8 ]]]
              ...
    len = [z, y, x] for dim = 3
     @param fullname A unique identifier string
     @param dim Number of dimensions of the matrix/tensor
     @param len Pointer to an array of size dim with the number of entries in each dimension
     @param dmatrix Pointer to an array holding the matrix/tensor entries
     @param unit A string holding the unit of the object (uniformly for all entries)
     @return A boolean indicating success of the operation 
*/
ipdBoolean         ipdSetRealQuantityMatrixByName(ipdConstString   fullname,
						const ipdLong    dim,
						const ipdLong   *len,
						const ipdDouble *dmatrix,
						ipdConstString   unit);

/*********************************/
/*            Specials           */
/*********************************/

/**  @brief Delete the existing item the iterator is currently pointing to
     @param iNode An ipd iterator pointing to the node which is to be deleted
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdIteratorDelete(ipdIterator_t *iNode);

/**  @brief Deletes the item with the given name.
     @param name A unique identifier string for the item which is to be deleted
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdDeleteByName(ipdConstString name);

/**  @brief Checks if a given calculated result is constant.

     Checks if a given calculated result (not a variable or things like that!!) is constant. So to check if a variable is constant, you have to
     calculate its expression first an then call ipdCheckConstant().
     This will not take auxiliary (to the simulator unchangeable) variables into consideration.
     @param tn A tree node item
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdCheckConstant(ipdTreeNode_t *tn);

/**  @brief Checks if a variable with the given name is constant.
     @param name A unique identifier string
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdCheckConstantByName(ipdConstString name);

/**  @brief Locks a variable.

   A lock is a counter which is increased each time a LOCK happens.
   Of course this counter will be decreased each time an UNLOCK
   is performed. Only variables can be locked.
   A lock is recommended when reading values from the inner structures
   of a variable, e.g. for reading an array by using handles.
     @param name Unique identifier string of the variable
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdLockVariable  (ipdConstString name);

/**  @brief Unlock a variable.

   A lock is a counter which is increased each time a LOCK happens.
   Of course this counter will be decreased each time an UNLOCK
   is performed. Only variables can be locked.
   A lock is recommended when reading values from the inner structures
   of a variable, e.g. for reading an array by using handles.
     @param name Unique identifier string of the variable
     @return A boolean indicating success of the operation
*/
ipdBoolean         ipdUnLockVariable(ipdConstString name);

/** @brief Get the length of a linked list.

    The list is expected to be of type ipdSLL_t
    @param list An ipd list
    @return The length of the provided list
*/
long             ipdListLen(void *list);

/**  @brief Check whether a particular variable is declared as external ('ext')
     @param tn A tree node item
     @return TRUE if the tree node is external, else FALSE
*/
ipdBoolean         ipdIsExternal(ipdTreeNode_t *tn);

/*********************************/
/*        Dump Input Deck        */
/*********************************/
/**  @brief Dumps the search paths to stdout. */
void             ipdDumpSearchPath(void);

/**  @brief Dumps the given comment string. If the comment consists of more than one line the C-comment type is used, otherwise C++ style.
     @param fOut The output file (stream)
     @param comment The comment string
     @param indent Number of spaces used for indenting
     @param thisPrintf Which printf(...) function to use
*/
void             ipdPrintComment(void          *fOut,
				 const char    *comment,
				 int            indent,
				 ipdPrintf_f    thisPrintf);

/**  @brief Dumps stored data recursively. Produces IPL compatible output.
     @param fOut The output file (stream)
     @param tn A tree node item
     @param recursive If this flag is ipdTRUE, the full (sub-)tree with root tn is printed
     @param onlyValues If this flag is ipdTRUE, only nodes which are values are printed
     @param thisPrintf Which printf(...) function to use
*/
void             ipdDumpRec(void          *fOut,
			    ipdTreeNode_t *tn,
			    ipdBoolean       recursive,
			    ipdBoolean       onlyValues,
			    ipdPrintf_f    thisPrintf);

/**  @brief Dumps all stored data to fOut. Produces IPL compatible output.
     @param fOut The output file (stream)
     @param onlyValues If this flag is ipdTRUE, only nodes which are values are printed
*/
void             ipdDumpAll2File(FILE     *fOut,
				 ipdBoolean  onlyValues);

/**  @brief Dumps all stored data to the string given. Produces IPL compatible output.
     @param str The string which should be dumped to
     @param onlyValues If this flag is ipdTRUE, only nodes which are values are printed
*/
void             ipdDumpAll2String(char     *str,
				   ipdBoolean  onlyValues);

/**  @brief Dumps stored in tn to the string given. Produces IPL compatible output.
     @param str The string which should be dumped to
     @param tn A tree node item
     @param recursive If this flag is ipdTRUE, the full (sub-)tree with root tn is printed
     @param onlyValues If this flag is ipdTRUE, only nodes which are values are printed
*/
void             ipdDump2String(char          *str,
				ipdTreeNode_t *tn,
				ipdBoolean       recursive,
				ipdBoolean       onlyValues);
/*********************************/
/*          Extensions           */
/*********************************/

/** @brief Dumps the expression of the given varable to a static buffer string.

    Produces IPL compatible output.
    If iNode is set (iterator) iNode will be dumped. If not and if tn is set
    tn will be dumped.
    Uses vMalloc();

    @param iNode An ipd iterator
    @param tn A tree node item
    @return The expression string
 */
ipdConstString     ipdIteratorGetExpression(ipdIterator_t *iNode,
					  ipdTreeNode_t *tn);

/** @brief  Returns the name of the parent section. Uses vMalloc();
    @param iNode An ipd iterator
    @param tn A tree node item
    @return String identifying the parent section
 */
ipdConstString     ipdIteratorGetParentSection(ipdIterator_t *iNode,
					     ipdTreeNode_t *tn);

/** @brief Creates a list of all names of the ancestors of the given section.
    @param iNode An ipd iterator
    @param tn A tree node item
    @param ancestorNames Pointer to a list of ancestor names
    @param ancestorNum Pointer to the returned number of ancestors
    @return ipdTRUE if successful
 */
ipdBoolean         ipdIteratorGetAncestors(ipdIterator_t   *iNode,
					 ipdTreeNode_t   *tn,
					 ipdConstString   **ancestorNames,
					 ipdInt            *ancestorNum);

/**  @brief Free an array of strings (names)
     @param names Pointer to an array of names
     @param num Number of elements in the array
*/
void             ipdFreeNameList(ipdConstString *names,
				 ipdInt          num);

/**  @brief Frees the arrays allocated in ipdLGetSection().
     @param itemNum Number of items in the list
     @param itemNames Array of item names which should be free'd
     @param expressions Array of expression strings which should be free'd
     @param sections Array of sections which should be free'd
     @param comments Array of comments which should be free'd
     @param itemKind Array of item types which should be free'd
     @param itemFlags Array of item flags which should be free'd
*/
void             ipdFreeSectionList(ipdInt          itemNum,
				    ipdConstString *itemNames,
				    ipdConstString *expressions,
				    ipdConstString *sections,
				    ipdConstString *comments,
				    ipdInt         *itemKind,
				    ipdString      *itemFlags
				    );

/*=======================================================================
	EXTENSIONS FOR FILE-SPECIFIC TREE-IPDNODES
=======================================================================*/

/**  @brief Get the double linked list of tree nodes in a file (extension for file-specific tree ipd-nodes)
     @param filename Name of the file
     @return Pointer to the tree root
*/
ipdTreeNodeDll_t *ipdGetDllOfTreeNodesInFile(ipdConstString filename);


/*=======================================================================
=======================================================================*/

# if 0
#   define free(x)  { printf("Calling free at %s %d\n", __FILE__, __LINE__); free(x); }
# endif

#if defined(__cplusplus)
}
#endif

#endif /* IPD_H */

/*@}*/

