/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

CONTENT:  Error handling, dumping the input deck.

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  RK   Robert Klima,      Institute for Microelectronics, TU Vienna

  who when        what
-------------------------------------------------------------------------
  RK  1998-04-14  new from scratch
=======================================================================*/

/** @file ipderror.c
    @brief Code for error handling and dumping the input deck
*/

/*=======================================================================
	Includes
=======================================================================*/

#include "ipdP.h"
#include <stdio.h>
#include <string.h>

/*=======================================================================
   C data structures
=======================================================================*/

/****************
  Error-Handling
 ****************/
const ipdERR_t ipdErrTable[] =
{
   IPD_ERR_ENTRY(ipdERR_IPDNO_ERROR,
		 "No error occured."),
   IPD_ERR_ENTRY(ipdERR_UNKIPDNOWN,
		 "Unknown error occured!"),
   IPD_ERR_ENTRY(ipdERR_IPDNO_MEMORY,
		 "Memory could not be allocated."),
   IPD_ERR_ENTRY(ipdERR_LOGFILE,
		 "cannot open logfile: '%s'."),
   IPD_ERR_ENTRY(ipdERR_INVALID_NAME,
		 "Given name '%s' is invalid."),
   IPD_ERR_ENTRY(ipdERR_INVALID_REL_NAME,
		 "Given relative name '%s' is invalid. "
		 "Do not use '~', '.', '^'"),
   IPD_ERR_ENTRY(ipdERR_IPDNO_VARIABLE,
		 "'%s' is not a variable."),
   IPD_ERR_ENTRY(ipdERR_IPDNO_SECTION,
		 "'%s' is not a section."),
   IPD_ERR_ENTRY(ipdERR_IPDNOT_FOUND,
		 "Item '%s' not found."),
   IPD_ERR_ENTRY(ipdERR_VAR_IPDNOT_FOUND,
		 "Variable '%s' not found."),
   IPD_ERR_ENTRY(ipdERR_SECT_IPDNOT_FOUND,
		 "Section '%s' not found."),
   IPD_ERR_ENTRY(ipdERR_VAR_LOCKED,
		 "Variable '%s' is locked. Cannot perform operation."),
   IPD_ERR_ENTRY(ipdERR_ANCESTOR_IPDNOT_FOUND,
		 "base section '%s' of section '%s' not found."),
   IPD_ERR_ENTRY(ipdERR_ANCESTOR_IPDNO_SECTION,
		 "Item '%s' is not a section and thus cannot be used as a base section for section '%s'."),
   IPD_ERR_ENTRY(ipdERR_PARA_IPDNOT_FOUND,
		 "Parameter '%s' not known."),
   IPD_ERR_ENTRY(ipdERR_SEARCH_ITEM,
		 "Only variables or a sections can be looked up."),
   IPD_ERR_ENTRY(ipdERR_IPDNOT_AN_AUXILIARY,
		 "Tried to redeclare variable '%s' as 'aux'."),
   IPD_ERR_ENTRY(ipdERR_IPDNOT_EXTERN_VAR,
		 "Tried to redeclare variable '%s' as 'ext'."),
   IPD_ERR_ENTRY(ipdERR_APPLICATION_SET_VAR,
		 "Application tried to overwrite variable '%s': "
		 "Overwriting allowed only for variables of type 'ext'."),
   IPD_ERR_ENTRY(ipdERR_VAR_TYPE,
		 "The type of variable '%s' was inherited. "
		 "Locally modified variables must be of the same type (ext, key, aux)."),
   IPD_ERR_ENTRY(ipdERR_CIRCULAR_REFS,
		 "Variable '%s': Expression contains circular references."),
   IPD_ERR_ENTRY(ipdERR_TOO_FEW_PARA,
		 "Too few parameters given."),
   IPD_ERR_ENTRY(ipdERR_TOO_MANY_PARA,
		 "Too many parameters given."),
   IPD_ERR_ENTRY(ipdERR_VAR_IPDNOT_INITIALIZED,
		 "Variable '%s' has not been initialized but its value was queried."),
   IPD_ERR_ENTRY(ipdERR_INCOMPATIBLE_TYPES,
		 "Operands of operation '%s' are of incompatible data types: '%s', '%s'"),
   IPD_ERR_ENTRY(ipdERR_WRONG_PARA_TYPE,
		 "Parameter in function '%s()' of wrong data type ('%s')."),
   IPD_ERR_ENTRY(ipdERR_WRONG_TYPE,
		 "Invalid data type '%s' used. "
		 "'%s' expected."),
   IPD_ERR_ENTRY(ipdERR_WRONG_TYPE_OP,
		 "Operand for operation '%s' has wrong type: '%s'"),
   IPD_ERR_ENTRY(ipdERR_DIFFERENT_UNITS,
		 "Operands of operation '%s' must have identical units. "
		 "Units found: '%s' and '%s'."),
   IPD_ERR_ENTRY(ipdERR_IPDNO_ADDITION,
		 "Cannot add values of type '%s'."),
   IPD_ERR_ENTRY(ipdERR_IPDNO_SUBTRACTION,
		 "Cannot subtract values of type '%s'."),
   IPD_ERR_ENTRY(ipdERR_IPDNO_MULTIPLICATION,
		 "Cannot multiply values of type '%s'."),
   IPD_ERR_ENTRY(ipdERR_IPDNO_DIVISION,
		 "Cannot divide values of type '%s'."),
   IPD_ERR_ENTRY(ipdERR_DIV_BY_ZERO,
		 "Division by zero using operator '%s'."),
   IPD_ERR_ENTRY(ipdERR_INVALID_CAST,
		 "Invalid type cast."),
   IPD_ERR_ENTRY(ipdERR_ARRAY_INDEX_TYPE,
		 "Array index must be an integer."),
   IPD_ERR_ENTRY(ipdERR_EXPECT_ARRAY,
		 "Array expected."),
   IPD_ERR_ENTRY(ipdERR_DIFF_ARRAY_LEN,
		 "Cannot add arrays of different lengths."),
   IPD_ERR_ENTRY(ipdERR_INDEX2HIGH,
		 "Array index (%d) out of range (array length = %d)."),
   IPD_ERR_ENTRY(ipdERR_INDEX2LOW,
		 "Array index must not be negative."),
   IPD_ERR_ENTRY(ipdERR_IPDNO_MUL_ARRAY,
		 "No multiplication/division of arrays allowed."),
   IPD_ERR_ENTRY(ipdERR_PARAM_NUM1,
		 "Function '%s()': Wrong number of parameters. "
		 "Single parameter expected."),
   IPD_ERR_ENTRY(ipdERR_PARAM_NUMN,
		 "Function '%s()': Wrong number of parameters. "
		 "%d parameters expected."),
   IPD_ERR_ENTRY(ipdERR_NAN_RESULT,
		 "Value IPDNOT-A-NUMBER occured in function '%s()'."),
   IPD_ERR_ENTRY(ipdERR_0TO_POW0,
		 "The result of 0 ** 0 is not defined."),
   IPD_ERR_ENTRY(ipdERR_0TO_POW_COMPLEX,
		 "The result of 0 ** COMPLEX is not defined."),
   IPD_ERR_ENTRY(ipdERR_INVALID_UNIT,
		 "Invalid unit '%s'."),
   IPD_ERR_ENTRY(ipdERR_INVALID_ENV,
		 "Cannot find environment variable '%s'."),
   IPD_ERR_ENTRY(ipdERR_FUNC_IPDNOT_DEFINED,
		 "Function '%s()' is not defined."),
   IPD_ERR_ENTRY(ipdERR_FUNC_ALREADY_DEFINED,
		 "Function '%s()' already defined. "
		 "Defined functions cannot be overwritten."),
   IPD_ERR_ENTRY(ipdERR_FUNC_NUMBER,
		 "Internal error! "
		 "Using function '%s': "
		 "Wrong function index: Number stored: %d, found: %d"),
   IPD_ERR_ENTRY(ipdERR_IPDNOT_SQRT_UNIT,
		 "Cannot calculate the sqrt() of this unit: '%s'."),
   IPD_ERR_ENTRY(ipdERR_IPDNOT_CBRT_UNIT,
		 "Cannot calculate the cbrt() of this unit: '%s'."),
   IPD_ERR_ENTRY(ipdERR_ARGUMENT0,
		 "Cannot calculate arg(0). "
		 "Result not defined."),
   IPD_ERR_ENTRY(ipdERR_CANT_OPEN_FILE,
		 "Cannot open file '%s'."),
   IPD_ERR_ENTRY(ipdERR_READ_INPUTDECK,
		 "Cannot read input file '%s'."),
   IPD_ERR_ENTRY(ipdERR_CANT_EVAL_SECTION,
		 "'%s': Cannot evaluate sections."),
   IPD_ERR_ENTRY(ipdERR_QUERY_AUX_VARIABLE,
		 "'%s': Auxiliary variable queried."),
   IPD_ERR_ENTRY(ipdERR_CANT_CALCULATE_SECTION,
		 "Cannot compute the value of a section."),
   IPD_ERR_ENTRY(ipdERR_TOO_MANY_INCLUDES,
		 "Includes nested too extensively."),
   IPD_ERR_ENTRY(ipdERR_FILE_IPDNOT_FOUND,
		 "File '%s' not found."),
   IPD_ERR_ENTRY(ipdERR_INHERIT_PARENT,
		 "Section '%s' is inherited from itself."),
   IPD_ERR_ENTRY(ipdERR_PARSE,
		 "Parse error."),
   IPD_ERR_ENTRY(ipdERR_IPDNO_IPD_DATABASE_FOUND,
		 "No IPD-database with the given identifier (%d) found."),
   IPD_ERR_ENTRY(ipdERR_MISSING_SEMICOLON,
		 "Missing semicolon."),
   IPD_ERR_ENTRY(ipdERR_ILLEGAL_INCLUDE,
		 "Wrong usage of #include. "
		 "Filename must be enclosed in angle brackets or in double quotes."),
   IPD_ERR_ENTRY(ipdERR_ILLEGAL_DELETE,
		 "Wrong usage of statement 'del'. "
		 "Name expected."),
   IPD_ERR_ENTRY(ipdERR_DELETE_IPDNOT_ALLOWED,
		 "Deleting of items not allowed: '%s'"),
   IPD_ERR_ENTRY(ipdERR_ILLEGAL_TRACE,
		 "Wrong usage of statement 'trace'. "
		 "List of names expected."),
   IPD_ERR_ENTRY(ipdERR_TRACESTRING_EXPECTED,
		 "Wrong usage of statement 'trace'. "
		 "Message string expected."),
   IPD_ERR_ENTRY(ipdERR_ALIAS_EXPECTED,
		 "Wrong usage of keyword \"alias\". "
		 "Syntax: 'alias ID = expression;' whereas expression must result in the name of a section."),
   IPD_ERR_ENTRY(ipdERR_EQUAL_OP_EXPECTED,
		 "Assignment operator '=' expected."),
   IPD_ERR_ENTRY(ipdERR_EXPRESSION_EXPECTED,
		 "Expression expected."),
   IPD_ERR_ENTRY(ipdERR_SECTION_IN_EXPRESSION,
		 "Reference to section '%s' used in expression. "
		 "Cannot compute the value of a section."),
   IPD_ERR_ENTRY(ipdERR_ALIAS_OF_VARIABLE,
		 "Alias '%s' must refer to a section."),
   IPD_ERR_ENTRY(ipdERR_VARIABLE_NAME_EXPECTED,
		 "Name of the variable missing."),
   IPD_ERR_ENTRY(ipdERR_PARAMETERLIST_EXPECTED,
		 "List of parameters expected. "
		 "Maybe a ',' or a ')' missing?"),
   IPD_ERR_ENTRY(ipdERR_OPTIONS_LAST,
		 "Optional parameters have to succeed regular parameters."),
   IPD_ERR_ENTRY(ipdERR_IDLIST_EXPECTED,
		 "List of parameter names expected."),
   IPD_ERR_ENTRY(ipdERR_PATH_NAME_EXPECTED,
		 "Pathname expected."),
   IPD_ERR_ENTRY(ipdERR_CLOSING_PARENTH_EXPECTED,
		 "Closing parenthesis ')' expected."),
   IPD_ERR_ENTRY(ipdERR_OPENING_PARENTH_EXPECTED,
		 "Opening parenthesis '(' expected."),
   IPD_ERR_ENTRY(ipdERR_BINARY_OPERATOR,
		 "Binary operator '%s' needs two arguments."),
   IPD_ERR_ENTRY(ipdERR_UNARY_OPERATOR,
		 "Unary operator '%s' needs an argument."),
   IPD_ERR_ENTRY(ipdERR_SQUARE_BRACKET_EXPECTED,
		 "Closing square bracket ']' expected."),
   IPD_ERR_ENTRY(ipdERR_SECT_ANCESTORS_EXPECTED,
		 "List of base sections expected."),
   IPD_ERR_ENTRY(ipdERR_SECTION_NAME_EXPECTED,
		 "Name of section expected."),
   IPD_ERR_ENTRY(ipdERR_EVAL_EXPRESSION,
		 "Cannot evaluate expression."),
   IPD_ERR_ENTRY(ipdERR_ALIAS_VALUE,
		 "Illegal expression. An alias must refer to a section."),
   IPD_ERR_ENTRY(ipdERR_INTEGER_OVERFLOW,
		 "Integer overflow."),
   IPD_ERR_ENTRY(ipdERR_PUSH_IPDNO_SECTION,
		 "No section pushed."),
   IPD_ERR_ENTRY(ipdERR_POP_IPDNO_SECTION,
		 "No section to pop."),
   IPD_ERR_ENTRY(ipdERR_REOPEN_SECTION,
		 "'%s': Reopening section."),
   IPD_ERR_ENTRY(ipdERR_OVERWRITING_EXPRESSION,
		 "'%s': Overwriting expression."),
   IPD_ERR_ENTRY(ipdERR_APPEND_PROTECTED_SECTION,
		 "Illegal item '%s'. "
		 "Only items located in the base sections of section '%s' may be locally modified."),
   IPD_ERR_ENTRY(ipdERR_DIFFERENT_ITEM_TYPE,
		 "'%s' must be a %s."),
   IPD_ERR_ENTRY(ipdERR_IPDNOT_LOCAL,
		 "Item '%s' is not locally modified. "
		 "Maybe its inherited?"),
   IPD_ERR_ENTRY(ipdERR_IPDNO_SHADOW_INFO,
		 "Item '%s' is not inherited. "
		 "It is defined locally."),
   IPD_ERR_ENTRY(ipdERR_HEAVYSIDE_FUNC,
		 "The Heavyside function '%s' is not defined at 0.0."),
   IPD_ERR_ENTRY(ipdERR_LVALUE_IS_FULLNAME,
		 "Lvalue '%s' must not be a fullname. "),
   IPD_ERR_ENTRY(ipdERR_CANT_RENAME_INHERITED,
		 "Item '%s' cannot be renamed since it is inherited."),
   IPD_ERR_ENTRY(ipdERR_CANT_RENAME_NAME_EXISTS,
		 "Item '%s' cannot be renamed to '%s' because an item with this name already exists."),
   IPD_ERR_ENTRY(ipdERR_UNBALANCED_ARRAY_IPDNOT_MATRIX,
		 "Unbalanced array cannot be treated as a matrix."),
   IPD_ERR_ENTRY(ipdERR_UNKIPDNOWN_OPTION,
		 "Function '%s()': Unknown option '%s'."),
   IPD_ERR_ENTRY(ipdERR_GENERAL,
		 ""), /* You have to submit the format string as the
			 first parameter. */

   /***********************************************/
   IPD_ERR_ENTRY(ipdERR_TOTAL_NUM,
		 ""),
   /***********************************************/

   IPD_ERR_ENTRY(ipdWARN_INFINITE_RESULT,
		 "An infinite value occured during calculation of function '%s'."),
   IPD_ERR_ENTRY(ipdWARN_LOOSE_LAST_CUR_SECT,
		 "Internal error: Overwriting history."),
   IPD_ERR_ENTRY(ipdWARN_FORCE_APPEND_EXISTS,
		 "No '+' necessary to force item '%s' to be appended. "
		 "Item is inherited to this section."),
   IPD_ERR_ENTRY(ipdWARN_FORCE_APPEND_IPDNOT_INHERITED,
		 "No '+' necessary to force item '%s' to be appended. "
		 "Section has no base sections."),
   IPD_ERR_ENTRY(ipdWARN_APPENDED_BY_APPLICATION,
		 "Item '%s' which has not been declared 'ext' was appended to section '%s'."),
   IPD_ERR_ENTRY(ipdWARN_USELESS_STMT,
		 "Expression will be lost."),

   /***********************************************/
   IPD_ERR_ENTRY(ipdWARN_TOTAL_NUM,
		 "")
   /***********************************************/
};

const char *ipdTypeStrings[] =
{
#define iiTYPE_INTEGER          0
   "integer",
#define iiTYPE_REAL             1
   "real number",
#define iiTYPE_COMPLEX          2
   "complex number",
#define iiTYPE_REALQUANTITY     3
   "real quantity",
#define iiTYPE_COMPLEXQUANTITY  4
   "complex quantity",
#define iiTYPE_BOOLEAN          5
   "boolean",
#define iiTYPE_STRING           6
   "string",
#define iiTYPE_ARRAY            7
   "array",
#define iiTYPE_ARRAYINDEX       8
   "arrayindex",
#define iiTYPE_SECTION          9
   "section",
#define iiTYPE_VARIABLE        10
   "variable",
#define iiTYPE_VARIABLE_REF    11
   "reference",
#define iiTYPE_OPERATOR        12
   "operator",
#define iiTYPE_FUNCTION        13
   "function",
#define iiTYPE_PARAMETER       14
   "parameter",
#define iiTYPE_OPTION          15
   "optional parameter",
#define iiTYPE_PARAMETER_REF   16
   "parameter reference",
#define iiTYPE_ALIAS           17
   "alias",
#define iiTYPE_EVAL            18
   "eval",
#define iiTYPE_IPDNOTHING         19
   "<nothing>",
};

/** @brief Prints the format string given, but concates outputs. */
void ipdLogPrintf(FILE *file, const char *fstr, ...)
{
   va_list      args;

   va_start(args, fstr);
   if ((file) && (file != stderr))
   {
      vfprintf(file, fstr, args);
      fflush(file);
   }

   vfprintf(stderr, fstr, args);
   fflush(stderr);
   va_end(args);
} /* ipdLogPrintf */

/** @brief Prints the format string given, concates outputs. The file is only opened, if a string is written, otherwise not. */
void ipdStdLog(const char *fstr, ...)
{
   va_list      args;

   if (!(ipdPrivate.logFile))
      iiInitLogFile(NULL, ipdTRUE);

   va_start(args, fstr);
   if ((ipdPrivate.logFile))
   {
      vfprintf(ipdPrivate.logFile, fstr, args);
      fflush(ipdPrivate.logFile);
   }
   else  /* args undefined after first call to vfprintf!!! */
   {
      vfprintf(stderr, fstr, args);
      fflush(stderr);
   }
   va_end(args);
} /* ipdStdLog */

/** @brief Sets the flag which controls the output of information about conditional inheritance. */
void ipdSetInheritanceInfo(ipdBoolean const nInfo) 
{
   ipdPrivate.inheritanceInfo = nInfo;
}

/*-------------------------------------------------------------------------
 * Error handling
 *-------------------------------------------------------------------------*/

ipdBoolean ipdErrorGotNew        = ipdFALSE;
static ipdBoolean ipdErrorStatus = ipdFALSE;

/** @brief Reset the error status. */
void ipdResetErrorStatus(void)
{
   ipdErrorGotNew = ipdFALSE;
   ipdErrorStatus = ipdFALSE;
} /* ipdResetErrorStatus */

/** @brief Get the error status of the Input Deck Database
    @return Returns ipdTRUE in case of an error status.
 */
ipdBoolean ipdGetErrorStatus(void)
{
   return ipdErrorStatus;
} /* ipdGetErrorStatus */

/** @brief Set the error status to ipdTRUE */
void iiSetErrorStatus()
{
   ipdErrorStatus = ipdTRUE;
} /* ipdGetErrorStatus */

/** @brief Pops the next error from the stack. */
ipdBoolean ipdPopError(void)
{
   ipdResetErrorStatus();

   if ((ipdBase->errorStack))
   {
      ipdERRSheet_t *err  = ipdBase->errorStack;

      ipdBase->errorStack = ipdBase->errorStack->next;
      ipdMBfree(err);
      return ipdTRUE;
   }
   return ipdFALSE;
} /* ipdPopError */

/** @brief Reset the error stack */
void iiResetErrorStack(void)
{
   while (ipdPopError())
      ;
} /* iiResetErrorStack */

/** @brief Create a new error sheet (message) */
static ipdBoolean iiNewErrorSheet(void)
{
   ipdERRSheet_t *err;

   if ((err = ipdMBalloc(sizeof(ipdERRSheet_t))))
   {
      err->errNum         = ipdERR_IPDNO_ERROR;
      err->errStr[0]      = 0;
      err->next           = ipdBase->errorStack;
      ipdBase->errorStack = err;
      return ipdTRUE;
   }
   if (err)
      ipdMBfree(err);
   return ipdFALSE;
} /* iiNewErrorSheet */

/** @brief Get the most recent error number. */
ipdERR_Type_t ipdGetErrorNumber(void)
{
   if (ipdBase->errorStack)
   {
      ipdERRSheet_t *err = ipdBase->errorStack;
      return err->errNum;
   }
   else
      return ipdERR_IPDNO_ERROR;
} /* ipdGetErrorNumber */

/** @brief Set the most recent error number. */
void iiSetErrorNumber(ipdERR_Type_t errNum)
{
   if (ipdBase->errorStack)
   {
      ipdERRSheet_t *err = ipdBase->errorStack;
      err->errNum = errNum;
   }
} /* iiSetErrorNumber */

/** @brief Get the most recent error message. */
ipdConstString ipdGetErrorMessage(void)
{
   if (ipdBase->errorStack)
   {
      ipdERRSheet_t *err = ipdBase->errorStack;
      return err->errStr;
   }
   return "";
} /* ipdGetErrorMessage */

/** @brief Resets the error flags. */
void ipdResetError(void)
{
   iiResetErrorStack();
} /* ipdResetError */

/* Concatenates a string to the existing error string. */
void ipdErrorStrCatN(ipdConstString str, long n)
{
   if (ipdBase->errorStack)
   {
      ipdERRSheet_t *err = ipdBase->errorStack;
      if (err->errStr)
      {
	 ipdLong          i   = strlen(err->errStr);
	 unsigned long  len = ipdERRORSTRLEN - i - 1;

         /*MK*/
         /* no idee wo das herkommt :-)
	 if (n)
	    len = MIN(len, (unsigned long)n);
          */
          if (n)
          {
            if (n < len) len = n;
          }

	 strncpy(&err->errStr[i], str, len);
	 err->errStr[i + len] = 0;
      }
   }
} /* ipdErrorStrCatN */

/* Adds the filename and the line number to the existing error message. */
void ipdAddFileLine2ErrorString(ipdTreeNode_t *tn)
{
   if ((ipdBase->errorStack) && (tn))
   {
      if ((ipdErrorGotNew) && !(ipdPrivate.parsing))
      {
	 char           str[ipdERRORSTRLEN + 1];

	 ipdErrorGotNew = ipdFALSE;
	 if (tn->node.sv.file)
	 {
	    sprintf(str, "%s, line: %ld",   tn->node.sv.file->fileName, tn->node.sv.lineno);
	    ipdStdLog(   "%s, line: %ld\n", tn->node.sv.file->fileName, tn->node.sv.lineno);
	 }
	 else
	 {
	    sprintf(str, "set by application");
	    ipdStdLog(   "set by application\n");
	 }
	 ipdErrorStrCatN(str, 0);
      }
   }
} /* ipdAddFileLine2ErrorString */

/** @brief Builds an error string and sets an error number.
*
* The number of the error is set in the last error node.
* An error string is created pointed to by the string errStr in that node.
* The corresponding error strings of an error number are defined in
* ipdErrTable. Theses strings may consist a %s, which is defined
* like in the function printf. It is replaced by additional strings
* supplied as additional arguments. All additional parametes must be
* strings!
*/
void ipdSetError(ipdERR_Type_t errNum, ...)
{
#if IPD_ERROR_NOTIFICATION
   printf("\033[1;31mAn error occurred, please check the log file!\033[0m\n");
#endif

   ipdConstString sFrom, addStr;
   ipdChar        buffer[100];
   ipdChar       *sTmp;
   ipdLong        addLong;
   va_list      ap;
   ipdBoolean     parseOK  = ipdTRUE;
   ipdBoolean     is_error = ipdFALSE;

   if (errNum == ipdERR_IPDNO_ERROR)
      return;

   /* There is a new error... */
   ipdErrorGotNew = ipdTRUE;
   iiNewErrorSheet();

   /* Is it an error or a warning? */
   if ((errNum == ipdERR_TOTAL_NUM) ||
       (errNum >= ipdWARN_TOTAL_NUM))
   {
      errNum   = ipdERR_UNKIPDNOWN;
      is_error = ipdTRUE;
   }
   else if (errNum < ipdERR_TOTAL_NUM)
      is_error = ipdTRUE;

#if IPD_TEST_ERROR_NUMBERS
   /* check the consistency of the entry in the table */
   if (errNum != ipdErrTable[errNum].errNum)
   {
      ipdStdLog("\n!! Wrong error number stored: %d, given: %d\n",
		ipdErrTable[errNum].errNum,
		errNum);
      errNum   = ipdERR_UNKIPDNOWN;
      is_error = ipdTRUE;
   }
   sFrom    = ipdErrTable[errNum].errStr;
#else
   sFrom    = ipdErrTable[errNum];
#endif /* IPD_TEST_ERROR_NUMBERS */

   if (is_error)
      iiSetErrorStatus();

   va_start(ap,errNum);
   if (errNum == ipdERR_GENERAL)
      sFrom = (char *)va_arg(ap,char*);
   while (1)
   {
      if ((sTmp = strchr(sFrom, '%')))
      {
	 ipdErrorStrCatN(sFrom, (sTmp - sFrom));
	 if      (parseOK && (*(sTmp+1) == 's'))
	 {
	    if (!(addStr = (char *)va_arg(ap,char*)))
	       addStr="(Null)";
	    ipdErrorStrCatN(addStr, 0);
	 }
	 else if (parseOK && (*(sTmp+1) == 'd'))
	 {
	    addLong  = (long)va_arg(ap,long);
	    ltoa2(buffer, addLong);
	    ipdErrorStrCatN(buffer, 0);
	 }
	 else if (parseOK && (*(sTmp+1) == 'g'))
	 {
	    sprintf(buffer, "%g", (double)va_arg(ap,double));
	    ipdErrorStrCatN(buffer, 0);
	 }
	 else if (parseOK && (*(sTmp+1) == 'c'))
	 {
	    buffer[0] = (char)va_arg(ap,int);
	    buffer[1] = 0;
	    ipdErrorStrCatN(buffer, 0);
	 }
	 else
	 {
	    ipdErrorStrCatN("???", 0);
	    parseOK = ipdFALSE;
	 }
	 sFrom  = sTmp + 2;
      }
      else
	 break;
   }
   va_end(ap);
   /* Complete error message */
   ipdErrorStrCatN(sFrom, 0);
   /* Set error number       */
   iiSetErrorNumber(errNum);

   if (is_error)
   {
      ipdPrivate.errorCount++;

      ipdStdLog("\n\n" ipdERR_PREFIX " %ld. IPD ERROR\n", ipdPrivate.errorCount);
#if 0
      if (ipdPrivate.parsing)
	 ipdStdLog(ipdERR_PREFIX " error occured while reading input file\n");
#endif
#if IPD_PRINT_ERROR_NUM
      ipdStdLog(ipdERR_PREFIX " IPD-Error number %d: %s\n", errNum, ipdGetErrorMessage());
#else
      ipdStdLog(ipdERR_PREFIX " %s\n", ipdGetErrorMessage());
#endif /* IPD_PRINT_ERROR_NUM */
      ipdStdLog(ipdERR_PREFIX "\n");
   }
   else
   {
      ipdPrivate.warnCount++;

      ipdStdLog("\n\n" ipdERR_PREFIX " %ld. IPD Warning\n", ipdPrivate.warnCount);
#if IPD_PRINT_ERROR_NUM
      ipdStdLog(ipdERR_PREFIX " IPD-Warning number %d: %s\n", errNum, ipdGetErrorMessage());
#else
      ipdStdLog(ipdERR_PREFIX " %s\n", ipdGetErrorMessage());
#endif /* IPD_PRINT_ERROR_NUM */
      ipdStdLog(ipdERR_PREFIX "\n");
   }
   if (ipdPrivate.parsing)
      iiDumpScannerPosition();
   ipdStdLog("\n");
} /* ipdSetError */

static void iiGetTypeStrCat(char         *buffer,
		     ipdConstString  str,
		     ipdBoolean     *cat)
{
   if (*cat)
      strcat(buffer, "|");
   else
      *cat = ipdTRUE;
   strcat(buffer, str);
}

/* Returns a string which discribes a given ipd type. */
#define ipdGETTYPESTRBUFFER 400
#define ipdGETTYPESTRBUFFERNUM 5
const char *ipdGetTypeStr(ipdType_t type, ipdBoolean onlyValues)
{
  static char buffer[ipdGETTYPESTRBUFFERNUM][ipdGETTYPESTRBUFFER];
  static long cnt = 0;
  ipdBoolean    cat = ipdFALSE;
  cnt++;
  if (cnt >= ipdGETTYPESTRBUFFERNUM) /* [SW] */
     cnt = 0;
  buffer[cnt][0] = 0;
  if (type & ipdSECTION)
     iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_SECTION], &cat);
  if (type & (ipdVARIABLE))
     iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_VARIABLE], &cat);
  if (type & ipdALIAS)
     iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_ALIAS], &cat);
  if (!onlyValues)
  {
     if (type & ipdVARIABLE_REF)
	iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_VARIABLE_REF], &cat);
     if (type & ipdOPERATOR)
	iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_OPERATOR], &cat);
     if (type & (ipdINTERNFUNCTION | ipdCCODEDFUNCTION))
	iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_FUNCTION], &cat);
     if (type & ipdINDEX)
	iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_ARRAYINDEX], &cat);
#if !IPD_EVAL_DELETES_EXPR
     if (type & ipdEVAL)
	iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_EVAL], &cat);
#endif
  }
  if ((onlyValues) ||
      ((!onlyValues) && (cat = ipdFALSE)))
  {
     if (type & ipdARRAY)
	iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_ARRAY], &cat);
     if (type & ipdARRAYVAL)
	iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_ARRAY], &cat);
     if (type & ipdPARAMETER)
	iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_PARAMETER], &cat);
     if (type & ipdOPTION)
	iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_OPTION], &cat);
     if ((type & ipdVALUE) == ipdREALQUANTITY)
	iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_REALQUANTITY], &cat);
     if ((type & ipdVALUE) == ipdCOMPLEXQUANTITY)
	iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_COMPLEXQUANTITY], &cat);
     if (type & ipdINTEGER)
	iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_INTEGER], &cat);
     if (type & ipdCOMPLEX)
	iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_COMPLEXQUANTITY], &cat);
     if (type & ipdREAL)
	iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_REAL], &cat);
     if (type & ipdSTRING)
	iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_STRING], &cat);
     if (type & ipdBOOLEAN)
	iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_BOOLEAN], &cat);
  }
  if (!cat)
     iiGetTypeStrCat(buffer[cnt], ipdTypeStrings[iiTYPE_IPDNOTHING], &cat);
  return buffer[cnt];
} /* ipdGetTypeStr */

/** @brief Prints the trace message. */
void iiTraceVar(ipdTreeNode_t    *tn,
		const char       *errString)
{
   if ((tn) &&
       !(ipdBase->bFlags & ipdBF_IPDNO_TRACE) &&
       (tn->type & ipdVARIABLE) &&
       (tn->node.sv.svu.var.flags & ipdVF_TRACE) &&
       !(ipdGetErrorStatus()))
   {
      ipdBase->bFlags |= ipdBF_IPDNO_TRACE;
      ipdStdLog(IPD_TRACE_MSG);
      if ((tn->node.sv.svu.var.traceTree))
      {
	 ipdTreeNode_t *ts;

	 if ((ts = ipdDoCalculation(tn->node.sv.svu.var.traceTree, "~")))
	 {
	    if (ts->type & ipdSTRING)
	       ipdStdLog(ts->node.expr.val.string);
	    else
	       ipdStdLog("<trace string error>");
	 }
	 else
	    ipdResetError();
      }
      else
	 ipdStdLog("variable \"%s\" -- %s.",
		   tn->node.sv.fullname, errString);
      ipdStdLog("\n");
      ipdBase->bFlags &= ~ipdBF_IPDNO_TRACE;
   }
} /* iiTraceVar */

/** @brief Prints the format string given, but concatenates outputs. */
static int iiPrintf(char *buffer, const char *fstr, ...)
{
   static char *pos;
   va_list      args;

   if ((buffer))
   {
      pos = buffer;
   }
   else
   {
      va_start(args, fstr);
      vsprintf(pos, fstr, args);
      pos += strlen(pos);
      va_end(args);
   }

   return 1;
} /* iiPrintf */

/*
* Dumps the given comment string. If the comment consists of more than one
* line the c-comment type is used other wise c++ style.
*/
void ipdPrintComment(void          *fOut,
		     const char    *comment,
		     int            indent,
		     ipdPrintf_f    thisPrintf)
{
   ipdBoolean  notSingleLine;
   char     *endPos;

   if ((comment) && (fOut) && (thisPrintf))
   {
      if (strchr(comment, '\n'))
	 notSingleLine = ipdTRUE;
      else
	 notSingleLine = ipdFALSE;
      if (!notSingleLine)
      {
	 thisPrintf(fOut, " // %s\n", comment);
	 return;
      }
      thisPrintf(fOut, "\n%*s/* ", indent, "");
      while (1)
      {
	 if ((endPos = strchr(comment, '\n')))
	 {
	    *endPos = '\0';
	    thisPrintf(fOut, "%s\n", comment);
	    *endPos = '\n';
	    comment = endPos + 1;
	 }
	 else
	 {
	    thisPrintf(fOut, "%s\n", comment);
	    thisPrintf(fOut, "%*s */\n", indent, "");
	    return;
	 }
	 thisPrintf(fOut, "%*s   ", indent, "");
      }
   }
} /* ipdPrintComment */

/* Dumps stored data recursively. Produces IPL compatible output. */
void ipdDumpRec(void          *fOut,
		ipdTreeNode_t *tn,
		ipdBoolean       recursive,
		ipdBoolean       onlyValues,
		ipdPrintf_f    thisPrintf)
{ 
  static int            indent = 0;
  static ipdTreeNode_t *CS;
  static char           strBuffer[100];

  if (!(tn))
  {

     CS = tn = iiGetRoot();  /* initialize */
     indent  = 0;
     if ((!(fOut)) && (thisPrintf != (ipdPrintf_f)iiPrintf))
	fOut = stdout;
     thisPrintf(fOut, "\n//***************************\n");
     thisPrintf(fOut, "// Current Input Deck    \n");
     thisPrintf(fOut, "//***************************\n\n");
     /* Print userdefined intern function list */
     if ((ipdBase->funcList))
     {
	iiInternFunc_t *fn;
	ipdTreeNode_t  *param;
	ipdBoolean    notFirstParam;

	fn = ipdBase->funcList;
	thisPrintf(fOut, "// User-defined functions:\n\n");
	while ((fn))
	{
	   thisPrintf(fOut, "%s(", (fn->name) ? fn->name : "?functionname?(");
	   notFirstParam = ipdFALSE;

	   param = fn->paramList;
	   while ((param))
		 {
		    if (notFirstParam)
		       thisPrintf(fOut, ", ");
		    else
		       notFirstParam = ipdTRUE;
		    thisPrintf(fOut, "%s",
			 (param->node.parameter.name) ?
			 param->node.parameter.name :
			       " ?parametername?");
	      param = param->next;
	   }
	   thisPrintf(fOut, ") = ");
	   if ((fn->tree))
	      ipdDumpRec(fOut, fn->tree,
			 recursive, onlyValues, thisPrintf);
	   else
	      thisPrintf(fOut, "?tree?");
	   thisPrintf(fOut, ";");
	   if ((fn->comment))
	      ipdPrintComment(fOut, fn->comment, indent, thisPrintf);
	   else
	      thisPrintf(fOut, "\n");
	   fn = fn->next;
	}
	thisPrintf(fOut, "\n");
     }
     thisPrintf(fOut, "// Section definition:\n\n");
     ipdDumpRec(fOut, tn, recursive, onlyValues, thisPrintf);
     thisPrintf(fOut, "\n//***************************\n");
     thisPrintf(fOut, "// End of current Input Deck    \n");
     thisPrintf(fOut, "//***************************\n\n");
     return;
  } 
  if (tn->type & ipdSECTION)
  {
     ipdTreeNode_t *child;

     /* Set the current section. */
     CS = tn;
     /* If the current section is the root section, don't print the {}. */
     if (tn != iiGetRoot())
     {
	/* Test if the section is append protected. */
	if (tn->node.sv.svu.sect.flags & ipdSF_EXTENDABLE)
	   thisPrintf(fOut, "%*s<", indent, "");
	else
	   thisPrintf(fOut, "%*s", indent, "");
	if (tn->node.sv.flags & ipdSV_FORCE_APPEND)
	   thisPrintf(fOut, "+");
	if ((tn->node.sv.name))
	   thisPrintf(fOut, "%s", tn->node.sv.name);
	else
	   thisPrintf(fOut, "?sectionname?");
	if (tn->node.sv.svu.sect.flags & ipdSF_EXTENDABLE)
	   thisPrintf(fOut, ">");

	/* If there are ancestors, print them. */
	if ((tn->node.sv.svu.sect.ancestor))
	{
	   ipdTreeNode_t *sanc;
	   ipdBoolean     notFirstParam = ipdFALSE;

	   thisPrintf (fOut, " : ");
	   sanc = tn->node.sv.svu.sect.ancestor;
	   while ((sanc))
	   {
	      if (notFirstParam)
		 thisPrintf(fOut, ", ");
	      else
		 notFirstParam = ipdTRUE;
	      thisPrintf(fOut, "%s", (sanc->node.sectRef.name));
	      if ((sanc->node.sectRef.condTree))
	      {
		 thisPrintf(fOut, " ? (");
		 ipdDumpRec(fOut, sanc->node.sectRef.condTree, ipdFALSE, onlyValues, thisPrintf);
		 thisPrintf(fOut, ")");
	      }
	      sanc = sanc->next;
	   }
	}
	/* Begin of section definition. */
	if (!recursive)
	{
	   thisPrintf(fOut, " {}\n");
	   return;
	}
	else if ((tn->node.sv.svu.sect.child) ||
	    !(tn->node.sv.svu.sect.ancestor))
	   thisPrintf(fOut, "\n%*s{", indent, "");
	else
	   thisPrintf(fOut, ";");
	indent += 2;
	if ((tn->comment))
	   ipdPrintComment(fOut, tn->comment, indent, thisPrintf);
	else
	   thisPrintf(fOut, "\n");
     }
     /* Print all items of this section. */
     child = tn->node.sv.svu.sect.child;
     {
	while ((child))
	{
	   ipdDumpRec(fOut, child, recursive, onlyValues, thisPrintf);
	   if ((child->type & ipdVARIABLE) &&
	       (child->node.sv.svu.var.flags & ipdVF_TRACE))
	   {
	      if ((child->node.sv.svu.var.traceTree))
	      {
		 thisPrintf(fOut, "%*strace %s = ", indent, "", child->node.sv.name);
		 ipdDumpRec(fOut, child->node.sv.svu.var.traceTree, ipdTRUE, ipdFALSE, thisPrintf);
		 thisPrintf(fOut, ";\n");
	      }
	      else
		 thisPrintf(fOut, "%*strace %s;\n", indent, "", child->node.sv.name);
	   }
	   child = child->next;
	}
     }
     if (tn != iiGetRoot())
     {
	/* End of section definition. */
	indent -= 2;
	if ((tn->node.sv.svu.sect.child) ||
	    !(tn->node.sv.svu.sect.ancestor))
	   thisPrintf(fOut, "%*s}\n", indent, "");
     }
  }
  else if (tn->type & ipdALIAS)
  {
     thisPrintf(fOut, "%*s", indent, "");
     thisPrintf(fOut, "alias %s = ", tn->node.sv.name);
     ipdDumpRec(fOut, tn->node.sv.svu.alias.condTree, ipdFALSE, ipdFALSE, thisPrintf);
     thisPrintf(fOut, ";\n");
  }
  else if (tn->type & ipdSECTION_REF)
  {
     thisPrintf(fOut, "%s", tn->node.sectRef.name);
  }
  else if (tn->type & ipdVARIABLE)
  {
     if (recursive)
     {    
	      /* Print the variable definition. */
	      /* Indent: */
	      thisPrintf(fOut, "%*s", indent, "");

	      /* Print variable type: */
	      if (tn->node.sv.svu.var.flags & ipdVF_AUXILIARY)
	      {
	         thisPrintf(fOut, "aux ");
	      }
	      else if (tn->node.sv.svu.var.flags & ipdVF_EXTERN)
	      {   
	         thisPrintf(fOut, "ext ");
	      }
	      /* The name of the variable: */
	      if (tn->node.sv.flags & ipdSV_FORCE_APPEND)
	      {
	         thisPrintf(fOut, "+");
	      }
	      if ((tn->node.sv.name))
	      {   
	         thisPrintf(fOut, "%s", tn->node.sv.name);
	      }
	      else
	      {   
	         thisPrintf(fOut, "?varname?");
	      }
	      if ((tn->node.sv.svu.var.tree) && (tn->node.sv.svu.var.tree->type != ipdINVALID))
	      {
	         thisPrintf(fOut, " = ");
	         /* Now dump the variable definition. */
	         ipdDumpRec(fOut, tn->node.sv.svu.var.tree, ipdFALSE,
		            onlyValues, thisPrintf);
	      }
	      thisPrintf(fOut, ";");
	      if ((tn->comment))
	      {
	         ipdPrintComment(fOut, tn->comment, indent, thisPrintf);
	      }
	      else
	      {
	         thisPrintf(fOut, "\n");
	      }
     }
     else
     {
	if ((tn->node.sv.fullname) && (*tn->node.sv.fullname))
	{
	   /* It's referenced. Print the name only. */
	   if (tn->node.sv.parent == CS)
	      thisPrintf(fOut, "%s", tn->node.sv.name);
	   else
	      thisPrintf(fOut, "%s", tn->node.sv.fullname);
	}
     }
  }
  else if (tn->type & ipdPARAMETER)
     thisPrintf(fOut, "%s", tn->node.parameter.name);
  else if (tn->type & ipdOPTION)
  {
     thisPrintf(fOut, "%s = ", tn->node.option.name);
     ipdDumpRec(fOut, tn->node.option.tree, ipdFALSE,
		onlyValues, thisPrintf);
  }
  else if (tn->type & ipdPARAMETER_REF)
     thisPrintf(fOut, "%s", tn->node.expr.func.pararef.name);
  else if (tn->type & ipdINCLUDE)
  {
     if (tn->node.sv.svu.icld.flags & ipdIF_WITH_SEARCHPATH)
	thisPrintf(fOut, "#include <%s>\n", tn->node.sv.fullname);
     else
	thisPrintf(fOut, "#include \"%s\"\n", tn->node.sv.fullname);
  }
  else if ((onlyValues && !(tn->type & ipdARRAYnVALUE)) ||
	   (!onlyValues && (tn->type & (~ipdARRAYnVALUE))))
  {
     if (tn->type & ipdVARIABLE_REF)
	thisPrintf(fOut, "%s", tn->node.expr.func.varref.name);
     else if (tn->type & ipdOPERATOR)
     {
	ipdTreeNode_t *op;
	ipdBoolean      notFirstParam = ipdFALSE, isNegative = ipdFALSE;
	ipdFunc_t     opPtr;
	unsigned long operandNum;

	op    = tn->node.expr.func.op.operand;
	opPtr = tn->node.expr.func.op.opPtr;
	operandNum = 0;
	while ((op))
	      {
		 if (notFirstParam)
		 {
		    if (opPtr == iiSum)
		    {
		 if (op->node.expr.eFlags & ipdEF_REVERSE)
		       {
			  thisPrintf(fOut, " - ");
			  isNegative = ipdTRUE;
		       }
		       else
		       {
			  thisPrintf(fOut, " + ");
			  isNegative = ipdFALSE;
		       }
		    }
		    else if (opPtr == iiProduct)
		    {
		 if (op->node.expr.eFlags & ipdEF_REVERSE)
			  thisPrintf(fOut, " / ");
		       else
			  thisPrintf(fOut, " * ");
		    }
		    else if (opPtr == iiModulus)
		       thisPrintf(fOut, " %% ");
		    else if (opPtr == iiEqual)
		       thisPrintf(fOut, " == ");
		    else if (opPtr == iiNotEqual)
		       thisPrintf(fOut, " != ");
		    else if (opPtr == iiLower)
		       thisPrintf(fOut, " < ");
		    else if (opPtr == iiGreater)
		       thisPrintf(fOut, " > ");
		    else if (opPtr == iiLowerEqual)
		       thisPrintf(fOut, " <= ");
		    else if (opPtr == iiGreaterEqual)
		       thisPrintf(fOut, " >= ");
		    else if (opPtr == iiLogicOr)
		       thisPrintf(fOut, " || ");
		    else if (opPtr == iiLogicAnd)
		       thisPrintf(fOut, " && ");
		    else if (opPtr == iiLogicNot)
		       thisPrintf(fOut, "!");
		    else
		       thisPrintf(fOut, "?operator?");
		 }
		 else
		 {
		    if (opPtr == iiSum)
		    {
		 if (op->node.expr.eFlags & ipdEF_REVERSE)
		       {
			  isNegative = ipdTRUE;
			  thisPrintf(fOut, "-");
		       }
		       else
			  isNegative = ipdFALSE;
		    }
		    else if (opPtr == iiProduct)
		    {
		 if (op->node.expr.eFlags & ipdEF_REVERSE)
			  thisPrintf(fOut, "1.0 / ");
		    }
		    notFirstParam = ipdTRUE;
		 }
		 if (((op->type & ipdOPERATOR) &&
		      ((op->node.expr.func.op.opPtr == iiSum) ||
		       (op->node.expr.func.op.opPtr == iiLogicOr) ||
		       (op->node.expr.func.op.opPtr == iiLogicAnd))) ||
		     ((op->type == ipdCOMPLEX) ||
		      (onlyValues && (op->type & ipdCOMPLEX))) ||
		     (isNegative) ||
		     ((op->type & ipdVARIABLE) && 
		      ((!op->node.sv.fullname) ||
		       (!*(op->node.sv.fullname)))))
		 {
		    thisPrintf(fOut, "(");
	      ipdDumpRec(fOut, op, ipdFALSE,
			       onlyValues, thisPrintf);
		    thisPrintf(fOut, ")");
		 }
		 else
	      ipdDumpRec(fOut, op, ipdFALSE,
			       onlyValues, thisPrintf);

	   operandNum++;
	   op = op->next;
	}
     }
     else if (tn->type & ipdARRAY)
     {
	ipdTreeNode_t *firstElement, *element;
	if ((firstElement = element = tn->node.expr.func.array))
	{
	   indent += 2;
	   thisPrintf(fOut, "[");
	   while (element)
	   {
	      if (element != firstElement)
		 thisPrintf(fOut, ", ");
	      ipdDumpRec(fOut, element, ipdFALSE, onlyValues, thisPrintf);
	      element = element->next;
	   }
	   thisPrintf(fOut, "]");
	   indent -= 2;
	}
	else
	   thisPrintf(fOut, "[?NULL?]\n");
     }
     else if (tn->type & ipdINTERNFUNCTION)
     {
	ipdTreeNode_t *param;
	ipdBoolean    notFirstParam = ipdFALSE;

	if ((tn->node.expr.func.internFunc.funcName))
	   thisPrintf(fOut, "%s(", tn->node.expr.func.internFunc.funcName);
	else
	   thisPrintf(fOut, "?funcname?(");
	param = tn->node.expr.func.internFunc.param;
	while ((param))
	      {
		 if (notFirstParam)
		    thisPrintf(fOut, ", ");
		 else
		    notFirstParam = ipdTRUE;
	   ipdDumpRec(fOut, param, ipdFALSE,
			    onlyValues, thisPrintf);
	   param = param->next;
	}
	thisPrintf(fOut, ")");
     }
     else if (tn->type & ipdCCODEDFUNCTION)
     {
	ipdTreeNode_t *param;
	ipdBoolean    notFirstParam = ipdFALSE;

	if ((tn->node.expr.func.cCodedFunc.funcName))
	   thisPrintf(fOut, "%s(", tn->node.expr.func.cCodedFunc.funcName);
	else
	   thisPrintf(fOut, "?funcname?(");
	param = tn->node.expr.func.cCodedFunc.param;
	while ((param))
	      {
		 if (notFirstParam)
		    thisPrintf(fOut, ", ");
		 else
		    notFirstParam = ipdTRUE;
	   ipdDumpRec(fOut, param, ipdFALSE,
			    onlyValues, thisPrintf);
	   param = param->next;
	}
	thisPrintf(fOut, ")");
     }
     else if (tn->type & ipdINDEX)
     {
	if (tn->node.expr.func.arrayIndex.tree->type == ipdVARIABLE_REF)
	   ipdDumpRec(fOut, tn->node.expr.func.arrayIndex.tree, ipdFALSE,
		      onlyValues, thisPrintf);
	else
	{
	   thisPrintf(fOut, "(");
	   ipdDumpRec(fOut, tn->node.expr.func.arrayIndex.tree, ipdFALSE,
		      onlyValues, thisPrintf);
	   thisPrintf(fOut, ")");
	}
	while ((tn))
	{
	   thisPrintf(fOut, "[");
	   ipdDumpRec(fOut, tn->node.expr.func.arrayIndex.index, ipdFALSE,
		      onlyValues, thisPrintf);
	   thisPrintf(fOut, "]");
	   tn = tn->node.expr.func.arrayIndex.next;
	}
     }
#if !IPD_EVAL_DELETES_EXPR
     else if (tn->type & ipdEVAL)
     {
	thisPrintf(fOut, "eval(");
	if ((tn->node.expr.func.evalTree))
	   ipdDumpRec(fOut, tn->node.expr.func.evalTree, ipdFALSE,
onlyValues, thisPrintf);
	thisPrintf(fOut, ")");
     }
#endif
  }
  else if (tn->type & ipdARRAYVAL)
  {
     long x;
     ipdTreeNode_t **en;

     en = tn->node.expr.val.arrayval.tree;
     if (en)
     {
	indent += 2;
	thisPrintf(fOut, "[");
	x = 0;
	while (x++ < tn->node.expr.val.arrayval.len)
	{
	   if (x > 1)
	      thisPrintf(fOut, ", ");
	   if ((*en))
	      ipdDumpRec(fOut, *en, ipdFALSE, onlyValues, thisPrintf);
	   else
	      thisPrintf(fOut, "?NULL?");
	   en++;
	}
	thisPrintf(fOut, "]");
	indent -= 2;
     }
     else
	thisPrintf(fOut, "[?NULL?]\n");
  }
  else if ((tn->type & ipdVALUE) == ipdINTEGER)
     thisPrintf(fOut, "%li",tn->node.expr.val.integer);
  else if ((tn->type & ipdVALUE) == ipdREAL)
  {
     sprintf(strBuffer, "%.16g", tn->node.expr.val.quantity.re);
     iiCreateTrailingZero(strBuffer);
     thisPrintf(fOut, "%s", strBuffer);
  }
  else if ((tn->type & ipdVALUE) == ipdCOMPLEX)
  {
     sprintf(strBuffer, "%.16g", tn->node.expr.val.quantity.re);
     iiCreateTrailingZero(strBuffer);
     thisPrintf(fOut, "%s + j ", strBuffer);
     sprintf(strBuffer, "%.16g", tn->node.expr.val.quantity.im);
     iiCreateTrailingZero(strBuffer);
     thisPrintf(fOut, "%s", strBuffer);
  }
  else if ((tn->type & ipdVALUE) == ipdREALQUANTITY)
  {
#if IPD_USE_UNITS
     char *unit;

     if ((tn->node.expr.val.quantity.unit) &&
	 (unit = iiGetUnitString(tn->node.expr.val.quantity.unit)))
     {
	sprintf(strBuffer, "%.16g", tn->node.expr.val.quantity.re);
	iiCreateTrailingZero(strBuffer);
	thisPrintf(fOut, "%s", strBuffer);
	if (strcmp(unit, "1"))
	   thisPrintf(fOut, " \"%s\"", unit);
#else
	sprintf(strBuffer, "%.16g", tn->node.expr.val.quantity.re);
	iiCreateTrailingZero(strBuffer);
	thisPrintf(fOut, "%s", strBuffer);
#endif /* IPD_USE_UNITS */
#if IPD_USE_UNITS
     }
     else
     {
	ipdSetError(ipdERR_IPDNO_MEMORY);
	return;
     }
#endif /* IPD_USE_UNITS */
  }
  else if ((tn->type & ipdVALUE) == ipdCOMPLEXQUANTITY)
  {
#if IPD_USE_UNITS
     char *unit;

     if ((tn->node.expr.val.quantity.unit) &&
	 (unit = iiGetUnitString(tn->node.expr.val.quantity.unit)))
     {
	sprintf(strBuffer, "%.16g", tn->node.expr.val.quantity.re);
	iiCreateTrailingZero(strBuffer);
	thisPrintf(fOut, "(%s + j ", strBuffer);
	sprintf(strBuffer, "%.16g", tn->node.expr.val.quantity.im);
	iiCreateTrailingZero(strBuffer);
	thisPrintf(fOut, "%s)", strBuffer);
	if (strcmp(unit, "1"))
	   thisPrintf(fOut, " * 1.0\"%s\"", unit);
#else
	sprintf(strBuffer, "%.16g", tn->node.expr.val.quantity.re);
	iiCreateTrailingZero(strBuffer);
	thisPrintf(fOut, "(%s + j ", strBuffer);
	sprintf(strBuffer, "%.16g", tn->node.expr.val.quantity.im);
	iiCreateTrailingZero(strBuffer);
	thisPrintf(fOut, "%s)", strBuffer);
#endif /* IPD_USE_UNITS */
#if IPD_USE_UNITS
     }
     else
     {
	   ipdSetError(ipdERR_IPDNO_MEMORY);
	return;
     }
#endif /* IPD_USE_UNITS */
  }
  else if ((tn->type & ipdVALUE) == ipdBOOLEAN)
     thisPrintf(fOut, "%s", tn->node.expr.val.boolean ? "true" : "false");
  else if ((tn->type & ipdVALUE) == ipdSTRING)
  {
     if ((tn->node.expr.val.string))
     {
	char *str;

	thisPrintf(fOut, "\"");

	str = tn->node.expr.val.string;
	while (*str)
	{
	   switch (*str) /* abfnrtvx01234567\\\?'\"  */
	   {
	      case '\a':
		 thisPrintf(fOut, "\\a");
		 break;
	      case '\b':
		 thisPrintf(fOut, "\\b");
		 break;
	      case '\f':
		 thisPrintf(fOut, "\\f");
		 break;
	      case '\n':
		 thisPrintf(fOut, "\\n");
		 break;
	      case '\r':
		 thisPrintf(fOut, "\\r");
		 break;
	      case '\t':
		 thisPrintf(fOut, "\\t");
		 break;
	      case '\v':
		 thisPrintf(fOut, "\\v");
		 break;
	      case '\\':
		 thisPrintf(fOut, "\\\\");
		 break;
	      case '\'':
		 thisPrintf(fOut, "\\\'");
		 break;
	      case '\"':
		 thisPrintf(fOut, "\\\"");
		 break;
	      default:
		 if (*str < 32)
		    thisPrintf(fOut, "\\x%d%d", *str >> 4, *str & 0x0F);
		 else
		    thisPrintf(fOut, "%c", *str);
	   }
	   str++;
	}

	thisPrintf(fOut, "\"");
     }
     else
	thisPrintf(fOut, "\"\"");
  }
  else
     thisPrintf(fOut, "!!! Unknown ipdType(%i) !!!\n",tn->type);

  return;
} /* ipdDumpRec */

/* Dumps all stored data to fOut. Produces IPL compatible output. */
void ipdDumpAll2File(FILE     *fOut,
		     ipdBoolean  onlyValues)
{
  ipdDumpRec(fOut, NULL, ipdTRUE, onlyValues, (ipdPrintf_f)fprintf);
} /* ipdDumpAll2File */

/* Dumps all stored data to the string given. Produces IPL compatible output. */
void ipdDumpAll2String(char     *str,
		       ipdBoolean  onlyValues)
{
   iiPrintf(str, "");
   ipdDumpRec(NULL, NULL, ipdTRUE, onlyValues, (ipdPrintf_f)iiPrintf);
} /* ipdDumpAll2String */

/* Dumps stored in tn to the string given. Produces IPL compatible output. */
void ipdDump2String(char          *str,
		    ipdTreeNode_t *tn,
		    ipdBoolean       recursive,
		    ipdBoolean       onlyValues)
{
   iiPrintf(str, "");
   ipdDumpRec(NULL, tn, recursive, onlyValues, (ipdPrintf_f)iiPrintf);
} /* ipdDump2String */
