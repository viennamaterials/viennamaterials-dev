/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

CONTENT:  Type conversion, operators

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  RK   Robert Klima,      Institute for Microelectronics, TU Vienna
          BR   Bernhard Reinisch, Institute for Microelectronics, TU Vienna

  who when        what
-------------------------------------------------------------------------
  BR  1997        first coding
  RK  1998-04-14  new from scratch
=======================================================================*/

/** @file ipdfunc.c
    @brief This file contains definitions of functions used for type conversions and operators within expression trees
*/


/*=======================================================================
  Includes
  =======================================================================*/

#include "ipdP.h" 
#include <stdio.h>
#include <string.h>

/*-----------------------------------------------------------------------
  Globals
  -----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------
  Type Conversion Routines
  -----------------------------------------------------------------------*/
/** @brief allocate memory for the newLen tree nodes */
static ipdTreeNode_t **iiAllocArgs(long newLen)
{
   return (ipdTreeNode_t **)ipdMBalloc(((newLen / 100) + 1) * 100 * sizeof(ipdTreeNode_t *));
} /* iiAllocArgs */

/** @brief Jumps over white spaces. */
char *iiSkipSpaces(char* str)
{
   while ((*str == ' ') || (*str == '\t') || (*str == '\n'))
      str++;
   return str;
} /* iiSkipSpaces */

/** @brief Converts a string to a real or complex number. No quantities. */
void iiString2Complex(double *re, double *im, char *str)
{
   char *tmp;

   *im = 0.0;
   /* Get real part */
   *re = strtod(tmp = str, &str);
   if (str == tmp)
   {
      ipdSetError(ipdERR_INVALID_CAST);
      return;
   }
   str = iiSkipSpaces(str);
   if (!(*str))
      return;
   if (*str != '+')
   {
      ipdSetError(ipdERR_INVALID_CAST);
      return;
   }
   str = iiSkipSpaces(str);
   if (*str == 'j')
   {  /* Leading 'j' */
      /* Get imaginary part */
      str++;
      str = iiSkipSpaces(str);
      *im = strtod(tmp = str, &str);
      if (str == tmp)
      {
	 ipdSetError(ipdERR_INVALID_CAST);
	 return;
      }
   }
   else
   {  /* 'j' at the end */
      /* Get imaginary part */
      *im = strtod(tmp = str, &str);
      if (str == tmp)
      {
	 ipdSetError(ipdERR_INVALID_CAST);
	 return;
      }
      str = iiSkipSpaces(str);
      if (*str != 'j')
      {
	 ipdSetError(ipdERR_INVALID_CAST);
	 return;
      }
   }
} /* iiString2Complex */

/** @brief Corrects behavior of printf("%.16g", double_value)

    printf will not
    print a double value without any digits after the '.' as a double
    value, eg, printf("%.16g", 1.0) results in  >1< . This function
    corrects this by adding ".0" to the given string. The string str will
    be modified!
 */
void iiCreateTrailingZero(char *str)
{
   if ((str))
   {
      while ((*str) && (*str != '.') && (*str != 'e') && (*str != 'E'))
	 str++;
      if (!(*str))
      {
	 *str++ = '.';
	 *str++ = '0';
	 *str   = '\0';
      }
   }
} /* iiCreateTrailingZero */

/** @brief Performs a type casting. 

    res will be used for storing the result,
    tn is the original value, type shows the type to convert tn to and
    force can be used to force casting. Forcing must be used, when an
    explicit casting is wanted. For implicit casting (integer to real) no
    forcing must be done.
 */
#define STRINGCASTBUFFERLEN 1000
static char stringCastBuffer[STRINGCASTBUFFERLEN];
ipdTreeNode_t *iiCastOperation(ipdTreeNode_t *res, ipdTreeNode_t *tn,
			       ipdType_t type, ipdBoolean force)
{
   /* First: Clean the destination node. */
   if (!(tn) || (!res) || !(tn->type & ipdVALUE))
      return NULL;
   iiClearValue(res);
   /* Cast to type. switch(type): */
   type = type & ipdVALUE;
   switch (type) /* Cast to type. */
   {
   case ipdINTEGER:
      if (tn->type & ipdINTEGER)
	 res->node.expr.val.integer = tn->node.expr.val.integer;
      else if ((tn->type & ipdREAL) && (!(tn->type & ipdQUANTITY) || force))
	 res->node.expr.val.integer = (long)(tn->node.expr.val.quantity.re /* + 0.5 */);
      else if (force && (tn->type & ipdBOOLEAN))
	 res->node.expr.val.integer = (tn->node.expr.val.boolean) ? 1 : 0;
      else if (force && (tn->type & ipdSTRING))
      {
	 char *str, *endptr;
	 int   base = 10;

	 if ((str = tn->node.expr.val.string))
	 {
	    if (*str == 'b')
	    {
	       str++;
	       base = 2;
	    }
	    else if (*str == '0')
	    {
	       str++;
	       if (*str == 'x')
	       {
		  str++;
		  base = 16;
	       }
	       else
		  base = 8;
	    }
	    res->node.expr.val.integer = strtol(str, &endptr, base);
	    if (str == endptr)
	    {
	       ipdSetError(ipdERR_INVALID_CAST);
	       return NULL;
	    }
	 }
	 else
	    res->node.expr.val.integer = 0;
      }
      else
      {
	 ipdSetError(ipdERR_INVALID_CAST);
	 return NULL;
      }
      res->type |= ipdINTEGER;
      break;
   case ipdREAL:
      if (tn->type & ipdINTEGER)
	 res->node.expr.val.quantity.re = (double)tn->node.expr.val.integer;
      else if ((tn->type & ipdREAL) && (!(tn->type & ipdQUANTITY) || force))
	 res->node.expr.val.quantity.re = tn->node.expr.val.quantity.re;
      else if (force && (tn->type & ipdBOOLEAN))
	 res->node.expr.val.quantity.re = (tn->node.expr.val.boolean) ? 1.0 : 0.0;
      else if (force && (tn->type & ipdSTRING))
      {
	 if ((tn->node.expr.val.string))
	 {
	    char *str;
	    res->node.expr.val.quantity.re = strtod(tn->node.expr.val.string, &str);
	    if (str == tn->node.expr.val.string)
	    {
	       ipdSetError(ipdERR_INVALID_CAST);
	       return NULL;
	    }
	 }
	 else
	    res->node.expr.val.quantity.re = 0;
      }
      else
      {
	 ipdSetError(ipdERR_INVALID_CAST);
	 return NULL;
      }
      res->type |= ipdREAL;
      break;
   case ipdCOMPLEX:
      res->node.expr.val.quantity.im = 0.0;
      if (tn->type & ipdINTEGER)
	 res->node.expr.val.quantity.re = (double)tn->node.expr.val.integer;
      else if ((tn->type & ipdREAL) && (!(tn->type & ipdQUANTITY) || force))
	 res->node.expr.val.quantity.re = tn->node.expr.val.quantity.re;
      else if ((tn->type & ipdCOMPLEX) && (!(tn->type & ipdQUANTITY) || force))
      {
	 res->node.expr.val.quantity.re = tn->node.expr.val.quantity.re;
	 res->node.expr.val.quantity.im = tn->node.expr.val.quantity.im;
      }
      else if (force && (tn->type & ipdBOOLEAN))
	 res->node.expr.val.quantity.re = (tn->node.expr.val.boolean) ? 1.0 : 0.0;
      else if (force && (tn->type & ipdSTRING))
      {
	 if ((tn->node.expr.val.string))
	 {
	    iiString2Complex(&res->node.expr.val.quantity.re, &res->node.expr.val.quantity.im,
			     tn->node.expr.val.string);
	    ipd_RETURNonERRORp;
	 }
	 else
	 {
	    ipdSetError(ipdERR_INVALID_CAST);
	    return NULL;
	 }
      }
      else
      {
	 ipdSetError(ipdERR_INVALID_CAST);
	 return NULL;
      }
      res->type |= ipdCOMPLEX;
      break;
   case ipdREALQUANTITY:
   case ipdCOMPLEXQUANTITY:
      res->node.expr.val.quantity.im = 0.0;
      if (tn->type & ipdINTEGER)
	 res->node.expr.val.quantity.re   = (double)tn->node.expr.val.integer;
      else if ((tn->type & ipdREAL) && ((tn->type & ipdQUANTITY) || force))
	 res->node.expr.val.quantity.re   = tn->node.expr.val.quantity.re;
      else if ((tn->type & ipdCOMPLEX) && ((tn->type & ipdQUANTITY) || force) &&
	       (type == ipdCOMPLEXQUANTITY))
      {
	 res->node.expr.val.quantity.re   = tn->node.expr.val.quantity.re;
	 res->node.expr.val.quantity.im   = tn->node.expr.val.quantity.im;
      }
      else if (force && (tn->type & ipdBOOLEAN))
	 res->node.expr.val.quantity.re   = (tn->node.expr.val.boolean) ? 1.0 : 0.0;
      else
      {
	 ipdSetError(ipdERR_INVALID_CAST);
	 return NULL;
      }

#if IPD_USE_UNITS
      if (tn->type & ipdQUANTITY)
	 res->node.expr.val.quantity.unit = iiCopyUnit(tn->node.expr.val.quantity.unit);
      else
	 res->node.expr.val.quantity.unit = iiUnitHashFindUnitStr((char *)"1");
      ipd_RETURNonERRORp;
#endif /* IPD_USE_UNITS */

      if (type == ipdREALQUANTITY)
	 res->type |= ipdREALQUANTITY;
      else
	 res->type |= ipdCOMPLEXQUANTITY;
      break;
   case ipdBOOLEAN:
      if (tn->type & ipdBOOLEAN)
	 res->node.expr.val.boolean = tn->node.expr.val.boolean;
      else if (force && (tn->type & ipdINTEGER))
	 res->node.expr.val.boolean = (tn->node.expr.val.integer) ? ipdTRUE : ipdFALSE;
      else if (force && (tn->type & ipdREAL) && (!(tn->type & ipdQUANTITY)))
	 res->node.expr.val.boolean = (tn->node.expr.val.quantity.re == 0.0) ? ipdFALSE : ipdTRUE;
      else if (force && (tn->type & ipdCOMPLEX) && (!(tn->type & ipdQUANTITY)))
	 res->node.expr.val.boolean = ((tn->node.expr.val.quantity.re == 0.0) &&
				       (tn->node.expr.val.quantity.im == 0.0)) ? ipdFALSE : ipdTRUE;
      else if (force && (tn->type & ipdSTRING))
      {
	 if ((tn->node.expr.val.string))
	 {
	    if (!(strncmp(tn->node.expr.val.string, "true", 4)))
	       res->node.expr.val.boolean = ipdTRUE;
	    else if (!(strncmp(tn->node.expr.val.string, "false", 5)))
	       res->node.expr.val.boolean = ipdFALSE;
	    else if (!(strncmp(tn->node.expr.val.string, "yes", 3)))
	       res->node.expr.val.boolean = ipdTRUE;
	    else if (!(strncmp(tn->node.expr.val.string, "no", 2)))
	       res->node.expr.val.boolean = ipdFALSE;
	    else
	    {
	       ipdSetError(ipdERR_INVALID_CAST);
	       return NULL;
	    }
	 }
	 else
	 {
	    ipdSetError(ipdERR_INVALID_CAST);
	    return NULL;
	 }
      }
      else
      {
	 ipdSetError(ipdERR_INVALID_CAST);
	 return NULL;
      }
      res->type |= ipdBOOLEAN;
      break;
   case ipdSTRING:
      {
	 char *str = stringCastBuffer;

	 if (tn->type & ipdSTRING)
	 {
	    if ((tn->node.expr.val.string))
	    {
	       if (!(res->node.expr.val.string = iiStrDup(tn->node.expr.val.string)))
		  return NULL;
	    }
	    else
	       res->node.expr.val.string = NULL;
	    res->type |= ipdSTRING;
	    return tn;
	 }
	 else if (tn->type & ipdINTEGER)
	    sprintf(str, "%ld", tn->node.expr.val.integer);
	 else if (tn->type & ipdREAL)
	 {
	    sprintf(str, "%.16g", tn->node.expr.val.quantity.re);
	    iiCreateTrailingZero(str);
	    str += strlen(str);
	 }
	 else if (tn->type & ipdCOMPLEX)
	 {
	    sprintf(str, "%.16g", tn->node.expr.val.quantity.re);
	    iiCreateTrailingZero(str);
	    str += strlen(str);
	    *str++ = ' ';
	    *str++ = '+';
	    *str++ = ' ';
	    *str++ = 'j';
	    *str++ = ' ';
	    sprintf(str, "%.16g", tn->node.expr.val.quantity.im);
	    iiCreateTrailingZero(str);
	    str += strlen(str);
	 }
	 else if (tn->type & ipdBOOLEAN)
	 {
	    if (tn->node.expr.val.boolean)
	       strcpy(str, "true");
	    else
	       strcpy(str, "false");
	 }
	 else
	 {
	    ipdSetError(ipdERR_INVALID_CAST);
	    return NULL;
	 }
	 if (tn->type & ipdQUANTITY)
	 {
#if IPD_USE_UNITS
	    char *unit;

	    unit = iiGetUnitString(tn->node.expr.val.quantity.unit);
	    ipd_RETURNonERRORp;
	    *str++ = ' ';
	    *str++ = '\"';
	    if ((unit))
	       strcpy(str, unit);
	    else
	       strcpy(str, "1");
	    str += strlen(str);
	    *str++ = '\"';
#endif /* IPD_USE_UNITS */
	 }

	 stringCastBuffer[STRINGCASTBUFFERLEN - 1] = '\0';
	 if (!(res->node.expr.val.string = iiStrDup(stringCastBuffer)))
	    return NULL;
	 res->type |= ipdSTRING;
      }
      break;
   default:
      break;
   }
   return res;
} /* iiCastOperation */

/** @brief Computes the common type of a operator op with two operands a and b. */
static ipdType_t iiSetCommonType(ipdType_t a, ipdType_t b, int op)
{
   ipdType_t t, x;

   t = (a | b) & ipdARRAYnVALUE;
   x = (a ^ b) & ipdARRAYnVALUE;

   if (a == ipdINVALID)
      return (b & ipdARRAYnVALUE);
   else if (b == ipdINVALID)
      return (a & ipdARRAYnVALUE);
   else if (t & ipdARRAY)
      return ipdINVALID;
   else if (t & ipdSTRING)
   {
      if ((char)op == '+')
	 return ipdSTRING;
      if (((char)op == '*') && (a == ipdSTRING) && ((b == ipdINTEGER) || (b == ipdREAL)))
	 return ipdSTRING;
      return ipdINVALID;
   }
   else if (t & ipdBOOLEAN)
   {
      if ((t == ipdBOOLEAN) && (((char)op == '&') || ((char)op == '|')))
	 return ipdBOOLEAN;
      return ipdINVALID;
   }
   else if (t & ipdCOMPLEX)
   {
      if (t & ipdQUANTITY)
      {
	 if ((char)op == '*')
	    return ipdCOMPLEXQUANTITY;
	 /* Both elements must be a quantity to add them. */
	 if (((char)op == '+') && !(x & ipdQUANTITY))
	    return ipdCOMPLEXQUANTITY;
	 return ipdINVALID;
      }
      return ipdCOMPLEX;
   }
   else if (t & ipdREAL)
   {
      if (t & ipdQUANTITY)
      {
	 if ((char)op == '*')
	    return ipdREALQUANTITY;
	 /* Both elements must be a quantity to add them. */
	 if (((char)op == '+') && !(x & ipdQUANTITY))
	    return ipdREALQUANTITY;
	 return ipdINVALID;
      }
      return ipdREAL;
   }
   else if (t & ipdINTEGER)
      return ipdINTEGER;

   return ipdINVALID;
} /* iiSetCommonType */

/** @brief Creates the expression tree. 

 func is the function operator like iiSum, iiProduct, or iiSin. Two nodes may be given: leftNode and rightNode.
*/
ipdTreeNode_t *iiCreateOperatorNode(ipdTreeNode_t *leftNode,
				    ipdTreeNode_t *rightNode,
				    ipdFunc_t      func,
				    ipdBoolean       reverse)
{
   ipdTreeNode_t *operatorNode = NULL;
   ipdTreeNode_t *lastOperand  = NULL;
   long           nOperands = 0;

   if ((rightNode))
   {
      if ((rightNode->type & ipdOPERATOR) &&
	  (rightNode->node.expr.func.op.opPtr == func) &&
	  (func != iiLogicNot)) /* For each ! allocate a single operator node */
      {
	 /* rightNode is an operator. */
	 operatorNode = rightNode;

	 rightNode     = operatorNode->node.expr.func.op.operand;
	 lastOperand   = operatorNode->node.expr.func.op.lastOperand;
	 nOperands     = operatorNode->node.expr.func.op.nOperands;
      }
      else
      {
	 lastOperand   = rightNode;
	 nOperands     = 1;
      }
   }
   if ((leftNode))
   {
      if ((leftNode->type & ipdOPERATOR) &&
	  (leftNode->node.expr.func.op.opPtr == func) &&
	  (func != iiLogicNot)) /* For each ! allocate a single operator node */
      {
	 if ((operatorNode))
	 {
	    /* An operator node has been given in primer rightNode.
	       leftNode is an operator node too. So we take leftNode as the
	       new operator node and free the rightNode one -- we need only one
	       operator node. */
	    operatorNode->node.expr.func.op.operand     = NULL;
	    operatorNode->node.expr.func.op.lastOperand = NULL;
	    iiFreeTreeNodeRec(operatorNode, ipdFALSE);
	 }
	 operatorNode = leftNode;
	 leftNode     = operatorNode->node.expr.func.op.operand;

	 if ((lastOperand))
	    operatorNode->node.expr.func.op.lastOperand->next = rightNode;
	 else
	    lastOperand = operatorNode->node.expr.func.op.lastOperand;
	 nOperands += operatorNode->node.expr.func.op.nOperands;
      }
      else
      {
	 /* leftNode is an operand. */
	 leftNode->next  = rightNode;
	 if (!(lastOperand))
	    lastOperand  = leftNode;
	 nOperands      += 1;
      }
   }
   else
   {
      leftNode  = rightNode;
      rightNode = NULL;
   }
   if (!(operatorNode))
   { /* Create a new operator node */
      if (!(operatorNode = iiCreateTreeNode(ipdOPERATOR)))
	 return NULL;
      operatorNode->node.expr.func.op.opPtr = func;
   }
   operatorNode->node.expr.func.op.operand     = leftNode;
   operatorNode->node.expr.func.op.lastOperand = lastOperand;
   operatorNode->node.expr.func.op.nOperands   = nOperands;

   /* Toggle reverse bit(s). */
   if (reverse)
   {
      ipdTreeNode_t *tn;

      if ((rightNode))
	 tn = rightNode;
      else if ((leftNode))
	 tn = leftNode;
      else
	 tn = NULL;

      /* Toggle the whole list. */
      while ((tn))
      {
	 if (func == iiSum)
	 {
	    switch (tn->type)
	    {
	    case ipdINTEGER:
	       tn->node.expr.val.integer     *= -1;
	       break;
	    case ipdCOMPLEX:
	       tn->node.expr.val.quantity.im *= -1.0;
	    case ipdREAL:
	       tn->node.expr.val.quantity.re *= -1.0;
	       break;
	    default:
	       tn->node.expr.eFlags ^= ipdEF_REVERSE;
	       break;
	    }
	 }
	 else
	 {
	    tn->node.expr.eFlags ^= ipdEF_REVERSE;
	 }
	 tn = tn->next;
      }
   }

   return operatorNode;
} /* iiCreateOperatorNode */

/** @brief Returns true, if all entries in the list are constant */
static ipdBoolean iiConstantSLL(ipdTreeNode_t *tn)
{
   while ((tn))
   {
      /* Are all childs simply values? */
      if (!iiIsConstant(tn))
      {
#if 0 /* Don't clear anything after calculation */
	 /* Clean all the entries beyond this operator node. */
	 if (tn->type & (ipdARRAY |
			 ipdOPERATOR |
			 ipdINTERNFUNCTION |
			 ipdCCODEDFUNCTION))
	    iiClearValue(tn);
#endif
	 return ipdFALSE;
      }
      tn = tn->next;
   }
   return ipdTRUE;
} /* iiConstantSLL */

/** @brief Simplifys an expression tree. (Very simple!) */
ipdTreeNode_t *iiSimplify(ipdTreeNode_t *tn)
{
   if ((tn->type & ipdOPERATOR))
   {
      if (tn->node.expr.eFlags & ipdEF_SIMPLIFIED)
	 return tn;

      /* Very simple simplify: */
      /* Test, if all childs are values.*/
      if (iiConstantSLL(tn->node.expr.func.op.operand))
      {
	 /* All childs are values. */
	 /* So only the result needs to be stored. */
	 /* Now free all childs (values). */
#if IPD_SIMPLIFY_DELETES_CONST_EXPR
	 iiFreeTreeNodeRec(tn->node.expr.func.op.operand, ipdFALSE);
	 tn->node.expr.func.op.operand = NULL;
	 /* The value has already been stored in this structure. */
	 /* We leave it unchanged, but remove ipdOPERATOR.       */
	 tn->type &= ipdARRAYnVALUE; /* It's a value now. */
#endif
	 tn->node.expr.eFlags |= ipdEF_CONSTANT;
      }
      tn->node.expr.eFlags |= ipdEF_SIMPLIFIED;
   }
   else if ((tn->type & ipdINTERNFUNCTION))
   {
      if (tn->node.expr.eFlags & ipdEF_SIMPLIFIED)
	 return tn;

      if (iiConstantSLL(tn->node.expr.func.internFunc.param))
      {  /* Function of constant values delivers a constant value: */
	 /* Free all parameters, leave only the value. */
#if IPD_SIMPLIFY_DELETES_CONST_EXPR
	 iiFreeTreeNodeRec(tn->node.expr.func.internFunc.param, ipdFALSE);
	 tn->node.expr.func.internFunc.param = NULL;
	 /* The value has already been stored in this structure. */
	 /* We leave it unchanged, but remove ipdINTERNFUNCTION. */
	 tn->type &= ipdARRAYnVALUE; /* It's a value now. */
#endif
	 tn->node.expr.eFlags |= ipdEF_CONSTANT;
      }
      tn->node.expr.eFlags |= ipdEF_SIMPLIFIED;
   }
#if 0 /* CCoded functions are not constant up to now! */
   else  if ((tn->type & ipdCCODEDFUNCTION))
   {
      if (tn->node.expr.eFlags & ipdEF_SIMPLIFIED)
	 return tn;

      if (tn->node.expr.eFlags & ipdEF_BUILTIN_FUNC)
      {
	 /* It's a built-in function. */
	 /* Test, whether parameters are constant values. */

	 if (iiConstantSLL(tn->node.expr.func.cCodedFunc.param))
	 {  /* Function of constant values delivers a constant value: */
	    /* Free all parameters, leave only the value. */
#if IPD_SIMPLIFY_DELETES_CONST_EXPR
	    iiFreeTreeNodeRec(tn->node.expr.func.cCodedFunc.param, ipdFALSE);
	    tn->node.expr.func.cCodedFunc.param = NULL;
	    /* The value has already been stored in this structure. */
	    /* We leave it unchanged, but remove ipdCCODEDFUNCTION. */
	    tn->type &= ipdARRAYnVALUE; /* It's a value now. */
#endif
	    tn->node.expr.eFlags |= ipdEF_CONSTANT;
	 }
	 tn->node.expr.eFlags |= ipdEF_SIMPLIFIED;
      }
   }
#endif

   return tn;
} /* iiSimplify */

/** @brief Gets the first node (expression node) in the tree, which is able to store a value.

    iiCalculate() must be called before.
 */
ipdTreeNode_t *iiGetExprNode(ipdTreeNode_t    *tn,
			     ipdDynamicName_t *baseSect,
			     iiGENFlags_t      flags)
{
   if ((tn))
   {
      if (tn->type & (~ipdARRAYnVALUE))
      {  /* It's a variable, operator, etc. */
         if (tn->type & (ipdOPERATOR | ipdINTERNFUNCTION | ipdCCODEDFUNCTION))
	 {
	    if (flags == iiGEN_CheckConstant)
	    {
	       ipdTreeNode_t *op = NULL;

	       if (tn->type & ipdOPERATOR)
		  op = tn->node.expr.func.op.operand;
	       else if (tn->type & ipdINTERNFUNCTION)
		  op = tn->node.expr.func.internFunc.param;
	       else
		  tn = NULL;
	       if ((tn) && !(tn->node.expr.eFlags & ipdEF_CONSTANT))
	       {
		  while ((op))
		  {
		     if (!(iiGetExprNode(op,
					 baseSect,
					 flags)))
		     {
			/* We've found a not constant item. */
			tn    = NULL;
			break;
		     }
		     op = op->next;
		  }
	       }
	    }
	 }
	 else if ((tn->type & (ipdVARIABLE | ipdVARIABLE_REF)))
	 {
	    ipdTreeNode_t    *realVar;
	    ipdDynamicName_t *oldBaseSect = baseSect;

	    if ((realVar = iiGetRealVariable(tn, &baseSect)))
	    {
	       /* Detect circular references. (Simple version) */
	       if ((ipdBase->calcVar))
	       {
		  if (realVar == ipdBase->calcVar)
		  {
		     ipdSetError(ipdERR_CIRCULAR_REFS, realVar->node.sv.fullname);
		     tn = NULL;
		  }
	       }
	       else
		  ipdBase->calcVar = realVar;

	       /* Calculate the tree of the variable. */
	       if ((tn))
	       {
		  if ((flags == iiGEN_CheckConstant) &&
		      (realVar->node.sv.svu.var.flags & ipdVF_EXTERN))
		     tn = NULL;
		  else
		  {
		     tn = iiGetExprNode(realVar->node.sv.svu.var.tree,
					baseSect,
					flags);
		  }
		  if (baseSect != oldBaseSect)
		     iiFreeDNNode(baseSect);
	       }
	    }
	    else
	       tn = NULL;
	 }
         else if (tn->type & (ipdPARAMETER | ipdPARAMETER_REF))
	 {
	    if ((tn->node.parameter.tree))
	       tn = iiGetExprNode(tn->node.expr.func.pararef.tree,
				  baseSect,
				  flags);
	 }
         else if (tn->type & ipdOPTION)
         {
	    if ((tn->node.option.tree))
	       tn = iiGetExprNode(tn->node.option.tree,
				  baseSect,
				  flags);
         }
	 else if ((tn->type & ipdINDEX))
	 {
	    if (flags == iiGEN_NoFlags)
	    {
	       ipdTreeNode_t *arrayElement;

	       if ((tn))
	       {
		  if ((arrayElement = iiFindArrayElement(tn, baseSect)))
		     tn = iiGetExprNode(arrayElement,
					baseSect,
					flags);
		  else
		     tn = NULL;
	       }
	    }
	 }
	 else if ((tn->type & ipdARRAY))
	 {
	    /* In case of arrays do nothing. */
	 }
	 /*
	   else if (tn->type & ipdEVAL) ---> do nothing.
	   Value found, return tn.
	 */
      }
   }
   return tn;
} /* iiGetExprNode */

/** @brief Calculates the value of the given expression */
ipdTreeNode_t *iiCalculate(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   if ((tn))
   {
      /* In case of arrays do nothing. */
      if (!(tn->type & ~ipdARRAYnVALUE))
      {
	 if (!(tn->type))
	 {
	    if (!(ipdGetErrorStatus()))
	       ipdSetError(ipdERR_UNKIPDNOWN);
	    tn = NULL;
	 }
      }
      else if (tn->type & ipdSECTION)
      { /* Can't calculate a sections value up to now. */
	 ipdSetError(ipdERR_CANT_EVAL_SECTION, tn->node.sv.fullname);
	 tn = NULL;
      }
      else if (tn->type & ipdSECTION_REF)
      {
	 if (!ipdAliasParsing)
	 {
	    ipdSetError(ipdERR_CANT_EVAL_SECTION, tn->node.sv.fullname);
	    tn = NULL;
	 }
      }
      else if (tn->type & ipdALIAS)
      {
      }
      else
      {  /* It's a variable, operator, etc. */
         if (tn->type & ipdOPERATOR)
         {
	    if (!(tn->node.expr.eFlags & ipdEF_CONSTANT))
	    {
	       /* Clear the value, if there is one: It will be rebuilt. */
	       iiClearValue(tn); /* Clear the value. */
	       /* Jump to operator function and calculate value */
	       if ((tn->node.expr.func.op.opPtr) &&
		   (tn = (*(tn->node.expr.func.op.opPtr))(tn, baseSect)))
		  iiSimplify(tn);
	       else
		  tn = NULL;
	    }
         }
         else if (tn->type & (ipdVARIABLE | ipdVARIABLE_REF))
         {
	    ipdTreeNode_t    *realVar, *res;
	    ipdTreeNode_t    *oldCurrVar;
	    ipdDynamicName_t *oldBaseSect = baseSect;

#if 0 /* not needed */
	    /* Trace variable. */
	    if ((tn->type & ipdVARIABLE) &&
		(tn->node.sv.svu.var.flags & ipdVF_TRACE))
	       iiTraceVar(tn, "calculate");
#endif
	    if ((tn->type & ipdVARIABLE) && (tn->node.sv.svu.var.lock))
	    {
	       /* variable locked? */
	       ipdSetError(ipdERR_VAR_LOCKED, tn->node.sv.fullname);
	       tn = NULL;
	    }
	    else if ((realVar = iiGetRealVariable(tn, &baseSect)))
	    {
	       /* Detect circular references. (Simple version) */
	       if ((ipdBase->calcVar))
	       {
		  if (realVar == ipdBase->calcVar)
		  {
		     ipdSetError(ipdERR_CIRCULAR_REFS, realVar->node.sv.fullname);
		     tn = NULL;
		  }
	       }
	       else
		  ipdBase->calcVar = realVar;

	       /* Calculate the tree of the variable. */
	       if ((tn))
	       {
		  ipdBase->currBaseSect = baseSect;
		  oldCurrVar            = ipdBase->currVar;
		  ipdBase->currVar      = realVar;

		  ipdErrorGotNew        = ipdFALSE;
		  if (!(realVar->node.sv.svu.var.tree))
		  {
		     ipdSetError(ipdERR_VAR_IPDNOT_INITIALIZED, realVar->node.sv.fullname);
		     tn = NULL;
		  }
		  else if ((res = iiCalculate(realVar->node.sv.svu.var.tree,
					      baseSect)))
		  {
		     if (tn->type & ipdVARIABLE_REF)
			iiAssignValue(tn, res);
		     else
			tn = res;
		  }
		  else
		     tn = NULL;
		  /* If an error has occured print the filename and filenumber. */
		  if ((ipdErrorGotNew))
		     ipdAddFileLine2ErrorString(realVar);

		  ipdBase->currVar = oldCurrVar;
		  if (baseSect != oldBaseSect)
		  {
		     iiFreeDNNode(baseSect);
		     ipdBase->currBaseSect = oldBaseSect;
		  }
	       }
	       /* Trace variable. */
	       if (realVar->node.sv.svu.var.flags & ipdVF_TRACE)
	       {
		  char value[200] = "calculated value: ";
		  if (tn)
		     ipdDump2String(value + strlen(value), tn, ipdFALSE, ipdTRUE);
		  else
		     strcpy(value + strlen(value), "failed");
		  iiTraceVar(realVar, value);
	       }
	    }
	    else
	    {
	       if (baseSect != oldBaseSect)
		  iiFreeDNNode(baseSect);
	       if ((tn))
		  ipdSetError(ipdERR_VAR_IPDNOT_FOUND, (tn->type & ipdVARIABLE) ?
			      tn->node.sv.fullname :
			      tn->node.expr.func.varref.name);
	       tn = NULL;
	    }
         }
         else if (tn->type & (ipdINTERNFUNCTION | ipdCCODEDFUNCTION))
         {
	    if (!(tn->node.expr.eFlags & ipdEF_CONSTANT))
	    {
	       /* Clear the value, if there is one: It will be rebuilt. */
	       iiClearValue(tn); /* Clear the value. */
	       /* Calculate the functions value. */
	       if ((tn = iiCalculateFunction(tn, baseSect)))
		  iiSimplify(tn);
	    }
         }
         else if (tn->type & ipdPARAMETER)
         {
	    tn = tn->node.parameter.tree;
         }
         else if (tn->type & ipdPARAMETER_REF)
         {
	    tn = iiCalculate(tn->node.expr.func.pararef.tree, baseSect);
         }
         else if (tn->type & ipdOPTION)
         {
	    tn = iiCalculate(tn->node.option.tree, baseSect);
         }
	 else if (tn->type & ipdINDEX)
	 {
	    ipdTreeNode_t *arrayElement;

	    if ((tn))
	    {
	       if ((arrayElement = iiFindArrayElement(tn, baseSect)))
		  tn = iiCalculate(arrayElement,
				   baseSect);
	       else
		  tn = NULL;
	    }
	 }
	 else if (tn->type & ipdARRAY)
	 {
	    ipdTreeNode_t *array;
	    ipdTreeNode_t **lTo;
	    ipdBoolean        isNotConst = ipdFALSE;

	    if (!(tn->node.expr.eFlags & ipdEF_CONSTANT))
	    {
	       if ((array = tn->node.expr.func.array))
	       {
		  if ((tn->type & ipdARRAYVAL) &&
		      (tn->node.expr.val.arrayval.len != ipdListLen(tn->node.expr.func.array)))
		  {
		     ipdMBfree(tn->node.expr.val.arrayval.tree);
		     tn->node.expr.val.arrayval.tree = NULL;
		  }
		  tn->type |= ipdARRAYVAL;
		  tn->node.expr.val.arrayval.len = ipdListLen(tn->node.expr.func.array);
		  if (!(tn->node.expr.val.arrayval.tree))
		  {
		     if (!(tn->node.expr.val.arrayval.tree = (ipdTreeNode_t **)ipdMBalloc(sizeof(ipdTreeNode_t *) *
											   tn->node.expr.val.arrayval.len)))
		     {
			ipdSetError(ipdERR_IPDNO_MEMORY);
			return NULL;
		     }
		  }
		  lTo = tn->node.expr.val.arrayval.tree;
		  while (array)
		  {
		     if ((*lTo = iiCalculate(array, baseSect)))
		     {
			if (!(isNotConst) &&
			    !(iiIsConstant(array)))
			   isNotConst = ipdTRUE;
		     }
		     else
			return NULL;
		     array = array->next;
		     lTo++;
		  }
		  if (!isNotConst)
		     tn->node.expr.eFlags |= ipdEF_CONSTANT;
	       }
	       else
	       {
		  ipdSetError(ipdERR_EXPECT_ARRAY);
		  tn = NULL;
	       }
	    }
	 }
	 /*
	   else if (tn->type & ipdEVAL) ---> do nothing.
	   Value found, return tn.
	 */
	   
      }
   }
   return tn;
} /* iiCalculate */

/* Starts the calculation of an expression or a variable. */
ipdTreeNode_t *ipdDoCalculation(ipdTreeNode_t *tn, const char *sectName)
{
   ipdDynamicName_t *baseSect = NULL, *oldttopBaseSect;
   ipdTreeNode_t    *retVal;

   /* Clear calcVar. calcVar is used to find circular references. */
   ipdBase->calcVar = NULL;

   /* Create a structure, necessary for calculations in iiCalculate().
      Store the name of the current section in this structure. */
   /* Get empty node. */
   oldttopBaseSect = ipdBase->topBaseSect;
   ipdBase->topBaseSect = ipdBase->currBaseSect = baseSect = iiCreateDNNode();
   ipd_RETURNonERRORp;

   /* Store fullname of the parent section.    */
   /* First copy the current section fullname. */
   iiStrExtend(&(baseSect->name), &(baseSect->len),
	       sectName, ipdFALSE);
   if (ipdGetErrorStatus())
   {
      iiFreeDNNode(baseSect);
      return NULL;
   }

   /* Start calculation. */
   if (!(retVal = iiCalculate(tn, baseSect)))
      ipdDumpRec(stderr, tn, ipdTRUE, ipdFALSE, (ipdPrintf_f)fprintf);
   /* Free section name structure. */
   iiFreeDNNode(baseSect);
   ipdBase->topBaseSect = oldttopBaseSect;
   ipdBase->currBaseSect = NULL;
   ipdBase->calcVar = NULL;

   return retVal;
} /* ipdDoCalculation */

/** @brief Returns the fullname of the currently calculated variable.
    @return The current variable name is returned and stored within ipdBase.
 */
ipdConstString iiGetCurrVarFullname(void)
{
   long len = 0;

   ipdMBfree(ipdBase->currVarFullname);
   ipdBase->currVarFullname = ipdNULL;
   if ((ipdBase->currVar))
   {
      if ((ipdBase->currBaseSect))
      {
	 iiStrExtend(&(ipdBase->currVarFullname),
		     &len,
		     ipdBase->currBaseSect->name,
		     ipdFALSE);
	 iiStrExtend(&(ipdBase->currVarFullname),
		     &len,
		     ipdBase->currVar->node.sv.name,
		     ipdTRUE);
      }
      else
      {
	 iiStrExtend(&(ipdBase->currVarFullname),
		     &len,
		     ipdBase->currVar->node.sv.fullname,
		     ipdFALSE);
      }
   }
   if (ipdGetErrorStatus())
      return ipdNULL;
   else
      return (ipdConstString)ipdBase->currVarFullname;
} /* iiGetCurrVarFullname */

/** @brief Returns the fullname of the currently calculated variable.
    @return The current variable name is returned und stored within ipdBase.
 */
ipdConstString iiGetCurrVarName(void)
{
   return (ipdBase->currVar) ?
      (ipdConstString)ipdBase->currVar->node.sv.name :
      (ipdConstString)ipdNULL;
} /* iiGetCurrVarName */

/** @brief Calculates the sum of a given expression. Distinguishes normal values and arrays. */
ipdTreeNode_t *iiSum(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t  *res    = NULL;
   ipdTreeNode_t **args;
   ipdTreeNode_t **iiArgs = NULL;
   long            iiCount;
   ipdTreeNode_t  *op;

   if (!(tn))
      return NULL;

   if (!(op      = tn->node.expr.func.op.operand))
      return NULL;

   if (!(iiCount = tn->node.expr.func.op.nOperands))
   {
      ipdSetError(ipdERR_UNKIPDNOWN);
      return NULL;
   }
   if (!(iiArgs = iiAllocArgs(iiCount)))
      return NULL;

   /* Calculate all childs.  */
   args = iiArgs;
   while ((op))
   {
      if (!(*args++ = iiCalculate(op, baseSect)))
      {
	 /* Error while calculation */
	 ipdMBfree(iiArgs);
	 return NULL;
      }
      op = op->next;
   }

   /* Calculate the sum. */
   if ((*iiArgs)->type & ipdARRAYVAL)
   {
      if (!(res = iiAddArrays(tn,
			      baseSect,
			      iiArgs,
			      iiCount)))
      {
	 ipdMBfree(iiArgs);
	 iiClearValue(tn); /* Free empty array. */
	 return NULL;
      }
      tn = res;
   }
   else
   {
      if (!(tn = iiAdd(tn,
		       iiArgs,
		       iiCount)))
      {
	 ipdMBfree(iiArgs);
	 return NULL;
      }
   }

   ipdMBfree(iiArgs);

   return tn;
} /* iiSum */

/** @brief Recursively adds arrays. */
ipdTreeNode_t *iiAddArrays(ipdTreeNode_t     *tn,
			   ipdDynamicName_t  *baseSect,
			   ipdTreeNode_t    **iiArgs,
			   long               iiCount
			   )
{
   /* Check the type of the argument (!) and determine the
      result type. */
   if ((*iiArgs)->type & ipdARRAYVAL)
   {
      /* The result type must be again a list. */
      ipdTreeNode_t  *res      = NULL;
      ipdTreeNode_t **argsNew  = NULL;
      ipdTreeNode_t **argsNewP, **iiArgsP, **aTreeRes;
      long            i, arrayIndex = 0, arrayLen;

      /* Extend the given ipdTreeNode_t* tn to store a list. */
      arrayLen = (*iiArgs)->node.expr.val.arrayval.len;
      if (!(tn->type & ipdARRAYVAL))
      {
	 tn->type |= ipdARRAYVAL;
	 if (!(tn->node.expr.val.arrayval.tree = aTreeRes =
	       (ipdTreeNode_t **)ipdMBalloc(arrayLen * sizeof(ipdTreeNode_t *))))
	 {
	    iiFreeTreeNodeRec(tn, ipdFALSE);
	    ipdMBfree(argsNew);
	    return NULL;
	 }
#if 0
	 ipdMBclear(aTreeRes);
#endif
	 tn->node.expr.val.arrayval.len = arrayLen;
      }
      else
	 aTreeRes = tn->node.expr.val.arrayval.tree;

      /* Check, if all elements are arrays and of the same length. */
      iiArgsP  = iiArgs;
      i        = iiCount;
      while (--i)
      {
	 iiArgsP++;
	 if ((*iiArgsP)->type & ipdARRAYVAL)
	 {
	    if ((*iiArgs)->type & ipdARRAYVAL)
	    {
	       if (arrayLen != (*iiArgsP)->node.expr.val.arrayval.len)
	       {
		  ipdSetError(ipdERR_DIFF_ARRAY_LEN);
		  return NULL;
	       }
	    }
	    else
	    {
	       ipdSetError(ipdERR_INCOMPATIBLE_TYPES, "+/-", 
			   ipdGetTypeStr((*iiArgs)->type, ipdTRUE),
			   ipdGetTypeStr((*iiArgsP)->type, ipdTRUE));
	       return NULL;
	    }
	 }
	 else if ((*iiArgs)->type & ipdARRAYVAL)
	 {
	    ipdSetError(ipdERR_INCOMPATIBLE_TYPES, "+/-", 
			ipdGetTypeStr((*iiArgs)->type, ipdTRUE),
			ipdGetTypeStr((*iiArgsP)->type, ipdTRUE));
	    return NULL;
	 }
      }

      /* Allocate new argument list. */
      if (!(argsNew = iiAllocArgs(iiCount)))
	 return NULL;

      iiArgsP  = iiArgs;
      i        = iiCount;
      while (arrayIndex < arrayLen)
      {
	 /* Generate the new parameter list. */
	 iiArgsP  = iiArgs;
	 argsNewP = argsNew;
	 i        = iiCount;
	 while (i--)
	 {
	    *argsNewP++ = ((*iiArgsP)->node.expr.val.arrayval.tree)[arrayIndex];
	    iiArgsP++;
	 }

	 /* Allocate empty (!) result node. */
	 if (!(res = iiCreateTreeNode(ipdINVALID)))
	 {
	    ipdMBfree(argsNew);
	    return NULL;
	 }

	 /* Do the sum. */
	 if (!(*aTreeRes++ = iiAddArrays(res,
					 baseSect,
					 argsNew,
					 iiCount)))
	 {
	    iiFreeTreeNodeRec(res, ipdFALSE);
	    ipdMBfree(argsNew);
	    return NULL;
	 }
	 arrayIndex++;
      }
      ipdMBfree(argsNew);
      return tn;
   }
   else
      return iiAdd(tn,
		   iiArgs,
		   iiCount);
} /* iiAddArrays */

/** @brief Calculates the sum of the given parameter list. */
ipdTreeNode_t *iiAdd(ipdTreeNode_t     *tn,
		     ipdTreeNode_t    **iiArgs,
		     long               iiCount
		     )
{
   ipdTreeNode_t  *cur = NULL;
   ipdTreeNode_t **args;
   ipdType_t       type;
   ipdType_t       ttmp;
   long            i;
#if IPD_USE_UNITS
   ipdUnit_t      *resUnit = NULL;
#endif /* IPD_USE_UNITS */


   /* Now add all items in the list. */
   /*** But: First get the common type for calculation. ***/
   args = iiArgs;
   i    = iiCount;
   type = ipdINVALID;
   while ((i--))
   {
      ttmp = iiSetCommonType(type, (*args)->type, '+'); /* Get the common type. */
      if (ttmp == ipdINVALID)
      {
	 ipdSetError(ipdERR_INCOMPATIBLE_TYPES, "+/-", 
		     ipdGetTypeStr(type, ipdTRUE),
		     ipdGetTypeStr((*args)->type, ipdTRUE));
	 return NULL;
      }
      type = ttmp;
      args++;
   }

   /*** Set the type... ***/
   tn->type |= (type & ipdVALUE);

   /* Initialize the result value. */
   if (type & ipdINTEGER)
      tn->node.expr.val.integer       = 0;
   else if (type & (ipdREAL | ipdCOMPLEX))
   {
      tn->node.expr.val.quantity.re   = 0.0;
      tn->node.expr.val.quantity.im   = 0.0;
   }
   else if (type & ipdBOOLEAN)
      tn->node.expr.val.boolean       = ipdFALSE;
   else if (type & ipdSTRING)
      tn->node.expr.val.string        = NULL;
#if IPD_USE_UNITS
   if (type & ipdQUANTITY)
      tn->node.expr.val.quantity.unit = NULL;
#endif /* IPD_USE_UNITS */


   args = iiArgs;
   i    = iiCount;
   while ((i))
   {
      cur = *args++;
      /* Get the sign */
      if (cur->node.expr.eFlags & ipdEF_REVERSE)
      {
	 if (type & ipdINTEGER)
	    tn->node.expr.val.integer        -= cur->node.expr.val.integer;
	 else if (type & ipdREAL)
	 {
	    if (cur->type & ipdINTEGER)
	       tn->node.expr.val.quantity.re -= (double)cur->node.expr.val.integer;
	    else
	       tn->node.expr.val.quantity.re -= cur->node.expr.val.quantity.re;
	 }
	 else if (type & ipdCOMPLEX)
	 {
	    if (cur->type & ipdINTEGER)
	       tn->node.expr.val.quantity.re -= (double)cur->node.expr.val.integer;
	    else if (cur->type & ipdREAL)
	       tn->node.expr.val.quantity.re -= cur->node.expr.val.quantity.re;
	    else
	    {
	       tn->node.expr.val.quantity.re -= cur->node.expr.val.quantity.re;
	       tn->node.expr.val.quantity.im -= cur->node.expr.val.quantity.im;
	    }
	 }
	 else if (type & (ipdSTRING | ipdBOOLEAN))
	 {
	    ipdSetError(ipdERR_IPDNO_SUBTRACTION, ipdGetTypeStr(type, ipdTRUE));
	    return NULL;
	 }
	 else if (type & ipdSECTION)
	 {
	    ipdSetError(ipdERR_CANT_CALCULATE_SECTION);
	    return NULL;
	 }
	 else
	 {
	    ipdSetError(ipdERR_INCOMPATIBLE_TYPES, "+/-", 
			ipdGetTypeStr(tn->type, ipdTRUE),
			ipdGetTypeStr(cur->type, ipdTRUE));
	    return NULL;
	 }
	 if (type & ipdQUANTITY)
	 {
#if IPD_USE_UNITS
	    if (!(resUnit))
	    {
	       if ((resUnit = cur->node.expr.val.quantity.unit) &&
		   (i != iiCount))
	       {
		  char *u1;
		  
		  u1 = iiGetUnitString(resUnit);
		  ipdSetError(ipdERR_DIFFERENT_UNITS, "+/-", u1, "1");
		  return NULL;
	       }
	    }
	    else if (!(iiUnitCompareable(resUnit,
					 cur->node.expr.val.quantity.unit)))
	    {
	       char *u1, *u2;
	       
	       u1 = iiGetUnitString(resUnit);
	       u2 = iiGetUnitString(cur->node.expr.val.quantity.unit);
	       ipdSetError(ipdERR_DIFFERENT_UNITS, "+/-", u1, u2);
	       return NULL;
	    }
#endif /* IPD_USE_UNITS */
	 }
      }
      else
      {
	 if (type & ipdINTEGER)
	    tn->node.expr.val.integer        += cur->node.expr.val.integer;
	 else if (type & ipdREAL)
	 {
	    if (cur->type & ipdINTEGER)
	       tn->node.expr.val.quantity.re += (double)cur->node.expr.val.integer;
	    else
	       tn->node.expr.val.quantity.re += cur->node.expr.val.quantity.re;
	 }
	 else if (type & ipdCOMPLEX)
	 {
	    if (cur->type & ipdINTEGER)
	       tn->node.expr.val.quantity.re += (double)cur->node.expr.val.integer;
	    else if (cur->type & ipdREAL)
	       tn->node.expr.val.quantity.re += cur->node.expr.val.quantity.re;
	    else
	    {
	       tn->node.expr.val.quantity.re += cur->node.expr.val.quantity.re;
	       tn->node.expr.val.quantity.im += cur->node.expr.val.quantity.im;
	    }
	 }
	 else if (type & ipdSTRING)
	 {
	    char *resStr;

	    resStr                    = tn->node.expr.val.string;
	    tn->node.expr.val.string = NULL;
	    if (cur->type & ipdSTRING)
	       tn->node.expr.val.string = iiStrAppend(resStr,
						      cur->node.expr.val.string);
	    else
	    {
	       iiCastOperation(tn, cur, ipdSTRING, ipdFALSE);
	       resStr = iiStrAppend(resStr, tn->node.expr.val.string);
	       ipdMBfree(tn->node.expr.val.string);
	       tn->node.expr.val.string = resStr;
	    }
	    if (ipdGetErrorStatus())
	       return NULL;
	 }
	 else if (type & ipdBOOLEAN)
	 {
	    ipdSetError(ipdERR_IPDNO_ADDITION, ipdGetTypeStr(type, ipdTRUE));
	    return NULL;
	 }
	 else if (type & ipdSECTION)
	 {
	    ipdSetError(ipdERR_CANT_CALCULATE_SECTION);
	    return NULL;
	 }
	 else
	 {
	    ipdSetError(ipdERR_INCOMPATIBLE_TYPES, "+/-", 
			ipdGetTypeStr(tn->type, ipdTRUE),
			ipdGetTypeStr(cur->type, ipdTRUE));
	    return NULL;
	 }
	 if (type & ipdQUANTITY)
	 {
#if IPD_USE_UNITS
	    if (!(resUnit))
	    {
	       if ((resUnit = cur->node.expr.val.quantity.unit) &&
		   (i != iiCount))
	       {
		  char *u1;
		  
		  u1 = iiGetUnitString(resUnit);
		  ipdSetError(ipdERR_DIFFERENT_UNITS, "+/-", u1, "1");
		  return NULL;
	       }
	    }
	    else if (!(iiUnitCompareable(resUnit,
					 cur->node.expr.val.quantity.unit)))
	    {
	       char *u1, *u2;
	       
	       u1 = iiGetUnitString(resUnit);
	       u2 = iiGetUnitString(cur->node.expr.val.quantity.unit);
	       ipdSetError(ipdERR_DIFFERENT_UNITS, "+/-", u1, u2);
	       return NULL;
	    }
#endif /* IPD_USE_UNITS */
	 }
      }
      i--;
   }
   if (type & ipdQUANTITY)
   {
#if IPD_USE_UNITS
      if ((resUnit))
	 tn->node.expr.val.quantity.unit = iiCopyUnit(resUnit);
      else
	 tn->type &= ~ipdQUANTITY;
#endif /* IPD_USE_UNITS */
   }
   return tn;
} /* iiAdd */

/** @brief Calculates the product of a given expression. */
ipdTreeNode_t *iiProduct(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t **iiArgs    = NULL;
   ipdTreeNode_t  *cur       = NULL, *res, **args;
   ipdType_t       type      = ipdINVALID, ttmp;
   ipdTreeNode_t  *op;
   long            iiCount;
#if IPD_USE_UNITS
   ipdUnit_t      *resUnit = NULL;
#endif /* IPD_USE_UNITS */

   if (!(tn))
      return NULL;

   if (!(op      = tn->node.expr.func.op.operand))
      return NULL;

   if (!(iiCount = tn->node.expr.func.op.nOperands))
   {
      ipdSetError(ipdERR_UNKIPDNOWN);
      return NULL;
   }
   if (!(iiArgs = iiAllocArgs(iiCount)))
      return NULL;

   /* Now multiply items in the list. */
   /*** First get the common type for calculation. ***/
   args = iiArgs;
   while ((op))
   {
      if (!(*args++ = cur = iiCalculate(op, baseSect)))
      {
	 ipdMBfree(iiArgs);
	 return NULL;
      }

      if (cur->type & (ipdARRAY | ipdARRAYVAL))
      {
	 ipdSetError(ipdERR_IPDNO_MUL_ARRAY);
	 ipdMBfree(iiArgs);
	 return NULL;
      }
      ttmp = iiSetCommonType(type, cur->type, '*'); /* Get the common type. */
      if (ttmp == ipdINVALID)
      {
	 ipdSetError(ipdERR_INCOMPATIBLE_TYPES, "*/",
		     ipdGetTypeStr(type, ipdTRUE),
		     ipdGetTypeStr(cur->type, ipdTRUE));
	 ipdMBfree(iiArgs);
	 return NULL;
      }
      type = ttmp;
      op = op->next;
   }

   /*** Now do the product... ***/
   res = tn;

   /* Initialize the result value. */
   if (type & ipdINTEGER)
      res->node.expr.val.integer       = 1;
   else if (type & (ipdREAL | ipdCOMPLEX))
   {
      res->node.expr.val.quantity.re   = 1.0;
      res->node.expr.val.quantity.im   = 0.0;
   }
   else if (type & ipdBOOLEAN)
      res->node.expr.val.boolean       = ipdTRUE;
   else if (type & ipdSTRING)
      res->node.expr.val.string        = NULL;
#if IPD_USE_UNITS
   if (type & ipdQUANTITY)
      res->node.expr.val.quantity.unit = NULL;
#endif /* IPD_USE_UNITS */

   res->type |= (type & ipdVALUE);
   args = iiArgs;

   while ((iiCount--))
   {
      /* Get the value. */
      cur = *args++;
      /* Get the sign */
      if (cur->node.expr.eFlags & ipdEF_REVERSE)
      {  /* Divide */
	 if (type & ipdINTEGER)
	 {
	    if (cur->node.expr.val.integer == 0)
	    {
	       ipdSetError(ipdERR_DIV_BY_ZERO, "/");
	       ipdMBfree(iiArgs);
	       return NULL;
	    }
	    res->node.expr.val.integer /= cur->node.expr.val.integer;
	 }
	 else if (type & ipdREAL)
	 {
	    double re;

	    if (cur->type & ipdINTEGER)
	       re = (double)cur->node.expr.val.integer;
	    else
	       re = cur->node.expr.val.quantity.re;
	    if (re == 0.0)
	    {
	       ipdSetError(ipdERR_DIV_BY_ZERO, "/");
	       ipdMBfree(iiArgs);
	       return NULL;
	    }
	    res->node.expr.val.quantity.re  /= re;
	 }
	 else if (type & ipdCOMPLEX)
	 {
	    double re, den;

	    if (cur->type & ipdINTEGER)
	       den = (double)cur->node.expr.val.integer;
	    else if (cur->type & ipdREAL)
	       den = cur->node.expr.val.quantity.re;
	    else
	       den = cur->node.expr.val.quantity.re * cur->node.expr.val.quantity.re +
		  cur->node.expr.val.quantity.im * cur->node.expr.val.quantity.im;
	    if (den == 0.0)
	    {
	       ipdSetError(ipdERR_DIV_BY_ZERO, "/");
	       ipdMBfree(iiArgs);
	       return NULL;
	    }
	    if (cur->type & ipdCOMPLEX)
	    {
	       re  = res->node.expr.val.quantity.re;
	       res->node.expr.val.quantity.re = re * cur->node.expr.val.quantity.re +
		  res->node.expr.val.quantity.im * cur->node.expr.val.quantity.im;
	       res->node.expr.val.quantity.im = res->node.expr.val.quantity.im * cur->node.expr.val.quantity.re -
		  re * cur->node.expr.val.quantity.im;
	    }
	    res->node.expr.val.quantity.re /= den;
	    res->node.expr.val.quantity.im /= den;
	 }
	 else if (type & (ipdBOOLEAN | ipdSTRING))
	 {
	    ipdSetError(ipdERR_IPDNO_DIVISION, ipdGetTypeStr(type, ipdTRUE));
	    ipdMBfree(iiArgs);
	    return NULL;
	 }
	 else if (type & ipdSECTION)
	 {
	    ipdSetError(ipdERR_CANT_CALCULATE_SECTION);
	    ipdMBfree(iiArgs);
	    return NULL;
	 }
	 if (type & ipdQUANTITY)
	 {
#if IPD_USE_UNITS
	    iiDivideUnits(&resUnit, cur->node.expr.val.quantity.unit);
	    if (ipdGetErrorStatus())
	    {
	       ipdMBfree(iiArgs);
	       return NULL;
	    }
#endif /* IPD_USE_UNITS */
	 }
      }
      else
      {  /* Multiply */
	 if (type & ipdINTEGER)
	    res->node.expr.val.integer      *= cur->node.expr.val.integer;
	 else if (type & ipdREAL)
	 {
	    if (cur->type & ipdINTEGER)
	       res->node.expr.val.quantity.re *= (double)cur->node.expr.val.integer;
	    else
	       res->node.expr.val.quantity.re *= cur->node.expr.val.quantity.re;
	 }
	 else if (type & ipdCOMPLEX)
	 {
	    if (cur->type & ipdINTEGER)
	    {
	       res->node.expr.val.quantity.re *= (double)cur->node.expr.val.integer;
	       res->node.expr.val.quantity.im *= (double)cur->node.expr.val.integer;
	    }
	    else if (cur->type & ipdREAL)
	    {
	       res->node.expr.val.quantity.re *= cur->node.expr.val.quantity.re;
	       res->node.expr.val.quantity.im *= cur->node.expr.val.quantity.re;
	    }
	    else
	    {
	       double re, im;

	       re = res->node.expr.val.quantity.re;
	       im = res->node.expr.val.quantity.im;
	       res->node.expr.val.quantity.re = re * cur->node.expr.val.quantity.re -
		  im * cur->node.expr.val.quantity.im;
	       res->node.expr.val.quantity.im = re * cur->node.expr.val.quantity.im +
		  im * cur->node.expr.val.quantity.re;
	    }
	 }
	 else if (type & ipdSTRING)
	 {  /* "ABC" * 4 -> "ABCABCABCABC" */
	    unsigned long len;
	    long  m;
	    char *sTo, *sFrom, *str;

	    if ((res->node.expr.val.string))
	    {
	       /* Original string length. */
	       len = strlen(res->node.expr.val.string);
	       /* Get number. */
	       if (cur->type & ipdINTEGER)
		  m = cur->node.expr.val.integer;
	       else if (cur->type & ipdREAL)
		  m = (int)(cur->node.expr.val.quantity.re /* + 0.5 */);
	       else
	       {
		  ipdSetError(ipdERR_INCOMPATIBLE_TYPES, "*",
			      ipdGetTypeStr(res->type, ipdTRUE),
			      ipdGetTypeStr(cur->type, ipdTRUE));
		  ipdMBfree(iiArgs);
		  return NULL;
	       }
	       /* Start */
	       if (len *= m)
	       {
		  if (!(str = (char *)ipdMBalloc(len + 1)))
		  {
		     ipdSetError (ipdERR_IPDNO_MEMORY);
		     ipdMBfree(iiArgs);
		     return NULL;
		  }
		  sTo = str; /* Store begin of the string. */
		  /* Copy m times. */
		  while (m--)
		  {
		     sFrom = res->node.expr.val.string;
		     while ((*sTo = *sFrom++))
			sTo++;
		  }
	       }
	       else if ((str = (char *)ipdMBalloc(1)))
		  *str = 0; /* malloc "" */
	       else
	       {
		  ipdSetError (ipdERR_IPDNO_MEMORY);
		  ipdMBfree(iiArgs);
		  return NULL;
	       }
	       ipdMBfree(res->node.expr.val.string);
	       res->node.expr.val.string = str;
	    }
	    else
	    {
	       if (!(res->node.expr.val.string =
		     iiStrDup(cur->node.expr.val.string)))
	       {
		  ipdMBfree(iiArgs);
		  return NULL;
	       }
	    }
	 }
	 else if (type & ipdBOOLEAN)
	 {
	    ipdSetError(ipdERR_IPDNO_MULTIPLICATION, ipdGetTypeStr(type, ipdTRUE));
	    ipdMBfree(iiArgs);
	    return NULL;
	 }
	 else if (type & ipdSECTION)
	 {
	    ipdSetError(ipdERR_CANT_CALCULATE_SECTION);
	    ipdMBfree(iiArgs);
	    return NULL;
	 }
	 if (type & ipdQUANTITY)
	 {
#if IPD_USE_UNITS
	    iiMultiplyUnits(&resUnit, cur->node.expr.val.quantity.unit);
	    if (ipdGetErrorStatus())
	    {
	       ipdMBfree(iiArgs);
	       return NULL;
	    }
#endif /* IPD_USE_UNITS */
	 }
      }
   }
   if (res->type & ipdQUANTITY)
   {
#if IPD_USE_UNITS
      if ((resUnit))
      {
	 res->node.expr.val.quantity.unit = iiUnitHashStoreUnit(resUnit);
	 if (res->node.expr.val.quantity.unit != resUnit)
	    iiFreeUnitHashNode(resUnit);
      }
      else
	 res->type &= ~ipdQUANTITY;
#endif /* IPD_USE_UNITS */
   }
   ipdMBfree(iiArgs);
   return res;
} /* iiProduct */

/** @brief Calculates a % b. */
ipdTreeNode_t *iiModulus(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *op, *a, *b;

   if ((tn->type & ipdOPERATOR) && (tn->node.expr.func.op.nOperands >= 2))
   {
      ipdType_t      t;

      op = tn->node.expr.func.op.operand;

      if (!(a = iiCalculate(op, baseSect)))
	 return NULL;
      while ((op = op->next))
      {
	 /* Calculate the arguments value. */
	 if (!(b = iiCalculate(op, baseSect)))
	    return NULL;

	 t = (a->type | b->type) & ipdARRAYnVALUE;
	 /* Calculate the function value. */
	 if (t == ipdINTEGER)
	 {
	    tn->node.expr.val.integer = a->node.expr.val.integer % b->node.expr.val.integer;
	 }
	 else
	 {
	    ipdSetError(ipdERR_INCOMPATIBLE_TYPES, "%",
			ipdGetTypeStr(a->type, ipdTRUE),
			ipdGetTypeStr(b->type, ipdTRUE));
	    return NULL;
	 }
	 /* Set the type. */
	 tn->type |= ipdINTEGER;
	 a = tn;
      }
   }
   else
      return NULL;

   return tn;
} /* iiModulus */

/** @brief Calculates a == b. */
ipdTreeNode_t *iiEqual(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *op, *a, *b;

   if ((tn->type & ipdOPERATOR) && (tn->node.expr.func.op.nOperands >= 2))
   {
      ipdType_t      t;

      op = tn->node.expr.func.op.operand;

      if (!(a = iiCalculate(op, baseSect)))
	 return NULL;
      while ((op = op->next))
      {
	 /* Calculate the arguments value. */
	 if (!(b = iiCalculate(op, baseSect)))
	    return NULL;

	 t = (a->type | b->type) & ipdARRAYnVALUE;
	 /* Calculate the function value. */
	 if (t & ipdBOOLEAN)
	 {
	    if (t == ipdBOOLEAN)
	       tn->node.expr.val.boolean = (a->node.expr.val.boolean ==
					    b->node.expr.val.boolean);
	    else
	       goto iiEqualError;
	 }
	 else if (t & ipdSTRING)
	 {
	    if (t == ipdSTRING)
	       tn->node.expr.val.boolean = (iiStrCmp(a->node.expr.val.string,
						     b->node.expr.val.string)) ? ipdFALSE : ipdTRUE;
	    else
	       goto iiEqualError;
	 }
	 else
	 {
	    if (t & ipdQUANTITY)
	    {
	       if ((a->type & ipdQUANTITY) && (b->type & ipdQUANTITY))
	       {
#if IPD_USE_UNITS
		  if (!(iiUnitCompareable(a->node.expr.val.quantity.unit,
					  b->node.expr.val.quantity.unit)))
		  {
		     char *u1, *u2;

		     u1 = iiGetUnitString(a->node.expr.val.quantity.unit);
		     u2 = iiGetUnitString(b->node.expr.val.quantity.unit);
		     ipdSetError(ipdERR_DIFFERENT_UNITS, "==", u1, u2);
		     return NULL;
		  }
#endif /* IPD_USE_UNITS */
	       }
	       else
		  goto iiEqualError;
	    }
	    if (a->type & ipdINTEGER)
	    {
	       if (b->type & ipdINTEGER)
		  tn->node.expr.val.boolean = (a->node.expr.val.integer ==
					       b->node.expr.val.integer);
	       else if (b->type & ipdREAL)
		  tn->node.expr.val.boolean = (((double)a->node.expr.val.integer) ==
					       b->node.expr.val.quantity.re);
	       else if (b->type & ipdCOMPLEX)
		  tn->node.expr.val.boolean = ((((double)a->node.expr.val.integer) ==
						b->node.expr.val.quantity.re) &&
					       (b->node.expr.val.quantity.im == 0.0));
	       else
		  goto iiEqualError;
	    }
	    else if (a->type & ipdREAL)
	    {
	       if (b->type & ipdINTEGER)
		  tn->node.expr.val.boolean = (a->node.expr.val.quantity.re ==
					       ((double)b->node.expr.val.integer));
	       else if (b->type & ipdREAL)
		  tn->node.expr.val.boolean = (a->node.expr.val.quantity.re ==
					       b->node.expr.val.quantity.re);
	       else if (b->type & ipdCOMPLEX)
		  tn->node.expr.val.boolean = ((a->node.expr.val.quantity.re ==
						b->node.expr.val.quantity.re) &&
					       (b->node.expr.val.quantity.im == 0.0));
	       else
		  goto iiEqualError;
	    }
	    else if (a->type & ipdCOMPLEX)
	    {
	       if (b->type & ipdINTEGER)
		  tn->node.expr.val.boolean = ((a->node.expr.val.quantity.re ==
						((double)b->node.expr.val.integer)) &&
					       (a->node.expr.val.quantity.im == 0.0));
	       else if (b->type & ipdREAL)
		  tn->node.expr.val.boolean = ((a->node.expr.val.quantity.re ==
						b->node.expr.val.quantity.re) &&
					       (a->node.expr.val.quantity.im == 0.0));
	       else if (b->type & ipdCOMPLEX)
		  tn->node.expr.val.boolean = ((a->node.expr.val.quantity.re ==
						b->node.expr.val.quantity.re) &&
					       (a->node.expr.val.quantity.im == 
						b->node.expr.val.quantity.im));
	       else
		  goto iiEqualError;
	    }
	    else
	       goto iiEqualError;
	 }
	 /* Set the type. */
	 tn->type |= ipdBOOLEAN;
	 a = tn;
      }
   }
   else
      return NULL;

   return tn;

 iiEqualError:
   ipdSetError(ipdERR_INCOMPATIBLE_TYPES, "==",
	       ipdGetTypeStr(a->type, ipdTRUE),
	       ipdGetTypeStr(b->type, ipdTRUE));
   return NULL;
} /* iiEqual */

/** @brief Calculates a != b. */
ipdTreeNode_t *iiNotEqual(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *op, *a, *b;

   if ((tn->type & ipdOPERATOR) && (tn->node.expr.func.op.nOperands >= 2))
   {
      ipdType_t      t;

      op = tn->node.expr.func.op.operand;

      if (!(a = iiCalculate(op, baseSect)))
	 return NULL;
      while ((op = op->next))
      {
	 /* Calculate the arguments value. */
	 if (!(b = iiCalculate(op, baseSect)))
	    return NULL;

	 t = (a->type | b->type) & ipdARRAYnVALUE;
	 /* Calculate the function value. */
	 if (t & ipdBOOLEAN)
	 {
	    if (t == ipdBOOLEAN)
	       tn->node.expr.val.boolean = (a->node.expr.val.boolean !=
					    b->node.expr.val.boolean);
	    else
	       goto iiNotEqualError;
	 }
	 else if (t & ipdSTRING)
	 {
	    if (t == ipdSTRING)
	       tn->node.expr.val.boolean = (iiStrCmp(a->node.expr.val.string,
						     b->node.expr.val.string)) ? ipdTRUE : ipdFALSE;
	    else
	       goto iiNotEqualError;
	 }
	 else
	 {
	    if (t & ipdQUANTITY)
	    {
	       if ((a->type & ipdQUANTITY) && (b->type & ipdQUANTITY))
	       {
#if IPD_USE_UNITS
		  if (!(iiUnitCompareable(a->node.expr.val.quantity.unit,
					  b->node.expr.val.quantity.unit)))
		  {
		     char *u1, *u2;

		     u1 = iiGetUnitString(a->node.expr.val.quantity.unit);
		     u2 = iiGetUnitString(b->node.expr.val.quantity.unit);
		     ipdSetError(ipdERR_DIFFERENT_UNITS, "!=", u1, u2);
		     return NULL;
		  }
#endif /* IPD_USE_UNITS */
	       }
	       else
		  goto iiNotEqualError;
	    }
	    if (a->type & ipdINTEGER)
	    {
	       if (b->type & ipdINTEGER)
		  tn->node.expr.val.boolean = (a->node.expr.val.integer !=
					       b->node.expr.val.integer);
	       else if (b->type & ipdREAL)
		  tn->node.expr.val.boolean = (((double)a->node.expr.val.integer) !=
					       b->node.expr.val.quantity.re);
	       else if (b->type & ipdCOMPLEX)
		  tn->node.expr.val.boolean = ((((double)a->node.expr.val.integer) !=
						b->node.expr.val.quantity.re) ||
					       (b->node.expr.val.quantity.im != 0.0));
	       else
		  goto iiNotEqualError;
	    }
	    else if (a->type & ipdREAL)
	    {
	       if (b->type & ipdINTEGER)
		  tn->node.expr.val.boolean = (a->node.expr.val.quantity.re !=
					       ((double)b->node.expr.val.integer));
	       else if (b->type & ipdREAL)
		  tn->node.expr.val.boolean = (a->node.expr.val.quantity.re !=
					       b->node.expr.val.quantity.re);
	       else if (b->type & ipdCOMPLEX)
		  tn->node.expr.val.boolean = ((a->node.expr.val.quantity.re !=
						b->node.expr.val.quantity.re) ||
					       (b->node.expr.val.quantity.im != 0.0));
	       else
		  goto iiNotEqualError;
	    }
	    else if (a->type & ipdCOMPLEX)
	    {
	       if (b->type & ipdINTEGER)
		  tn->node.expr.val.boolean = ((a->node.expr.val.quantity.re !=
						((double)b->node.expr.val.integer)) ||
					       (a->node.expr.val.quantity.im != 0.0));
	       else if (b->type & ipdREAL)
		  tn->node.expr.val.boolean = ((a->node.expr.val.quantity.re !=
						b->node.expr.val.quantity.re) ||
					       (a->node.expr.val.quantity.im != 0.0));
	       else if (b->type & ipdCOMPLEX)
		  tn->node.expr.val.boolean = ((a->node.expr.val.quantity.re !=
						b->node.expr.val.quantity.re) ||
					       (a->node.expr.val.quantity.im != 
						b->node.expr.val.quantity.im));
	       else
		  goto iiNotEqualError;
	    }
	    else
	       goto iiNotEqualError;
	 }
	 /* Set the type. */
	 tn->type |= ipdBOOLEAN;
	 a = tn;
      }
   }
   else
      return NULL;

   return tn;

 iiNotEqualError:
   ipdSetError(ipdERR_INCOMPATIBLE_TYPES, "!=",
	       ipdGetTypeStr(a->type, ipdTRUE),
	       ipdGetTypeStr(b->type, ipdTRUE));
   return NULL;
} /* iiNotEqual */

/** @brief Calculates a < b. */
ipdTreeNode_t *iiLower(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *op, *a, *b;

   if ((tn->type & ipdOPERATOR) && (tn->node.expr.func.op.nOperands >= 2))
   {
      ipdType_t      t;

      op = tn->node.expr.func.op.operand;

      if (!(a = iiCalculate(op, baseSect)))
	 return NULL;
      while ((op = op->next))
      {
	 /* Calculate the arguments value. */
	 if (!(b = iiCalculate(op, baseSect)))
	    return NULL;

	 t = (a->type | b->type) & ipdARRAYnVALUE;
	 /* Calculate the function value. */
	 if (t & ipdBOOLEAN)
	    goto iiLowerError;
	 else if (t & ipdSTRING)
	 {
	    if (t == ipdSTRING)
	       tn->node.expr.val.boolean = (iiStrCmp(a->node.expr.val.string,
						     b->node.expr.val.string) < 0);
	    else
	       goto iiLowerError;
	 }
	 else
	 {
	    if (t & ipdQUANTITY)
	    {
	       if ((a->type & ipdQUANTITY) && (b->type & ipdQUANTITY))
	       {
#if IPD_USE_UNITS
		  if (!(iiUnitCompareable(a->node.expr.val.quantity.unit,
					  b->node.expr.val.quantity.unit)))
		  {
		     char *u1, *u2;

		     u1 = iiGetUnitString(a->node.expr.val.quantity.unit);
		     u2 = iiGetUnitString(b->node.expr.val.quantity.unit);
		     ipdSetError(ipdERR_DIFFERENT_UNITS, "<", u1, u2);
		     return NULL;
		  }
#endif /* IPD_USE_UNITS */
	       }
	       else
		  goto iiLowerError;
	    }
	    if (a->type & ipdINTEGER)
	    {
	       if (b->type & ipdINTEGER)
		  tn->node.expr.val.boolean = (a->node.expr.val.integer <
					       b->node.expr.val.integer);
	       else if (b->type & ipdREAL)
		  tn->node.expr.val.boolean = (((double)a->node.expr.val.integer) <
					       b->node.expr.val.quantity.re);
	       else
		  goto iiLowerError;
	    }
	    else if (a->type & ipdREAL)
	    {
	       if (b->type & ipdINTEGER)
		  tn->node.expr.val.boolean = (a->node.expr.val.quantity.re <
					       ((double)b->node.expr.val.integer));
	       else if (b->type & ipdREAL)
		  tn->node.expr.val.boolean = (a->node.expr.val.quantity.re <
					       b->node.expr.val.quantity.re);
	       else
		  goto iiLowerError;
	    }
	    else
	       goto iiLowerError;
	 }
	 /* Set the type. */
	 tn->type |= ipdBOOLEAN;
	 a = tn;
      }
   }
   else
      return NULL;

   return tn;

 iiLowerError:
   ipdSetError(ipdERR_INCOMPATIBLE_TYPES, "<",
	       ipdGetTypeStr(a->type, ipdTRUE),
	       ipdGetTypeStr(b->type, ipdTRUE));
   return NULL;
} /* iiLower */

/** @brief Calculates a > b. */
ipdTreeNode_t *iiGreater(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *op, *a, *b;

   if ((tn->type & ipdOPERATOR) && (tn->node.expr.func.op.nOperands >= 2))
   {
      ipdType_t      t;

      op = tn->node.expr.func.op.operand;

      if (!(a = iiCalculate(op, baseSect)))
	 return NULL;
      while ((op = op->next))
      {
	 /* Calculate the arguments value. */
	 if (!(b = iiCalculate(op, baseSect)))
	    return NULL;

	 t = (a->type | b->type) & ipdARRAYnVALUE;
	 /* Calculate the function value. */
	 if (t & ipdBOOLEAN)
	    goto iiGreaterError;
	 else if (t & ipdSTRING)
	 {
	    if (t == ipdSTRING)
	       tn->node.expr.val.boolean = (iiStrCmp(a->node.expr.val.string,
						     b->node.expr.val.string) > 0);
	    else
	       goto iiGreaterError;
	 }
	 else
	 {
	    if (t & ipdQUANTITY)
	    {
	       if ((a->type & ipdQUANTITY) && (b->type & ipdQUANTITY))
	       {
#if IPD_USE_UNITS
		  if (!(iiUnitCompareable(a->node.expr.val.quantity.unit,
					  b->node.expr.val.quantity.unit)))
		  {
		     char *u1, *u2;

		     u1 = iiGetUnitString(a->node.expr.val.quantity.unit);
		     u2 = iiGetUnitString(b->node.expr.val.quantity.unit);
		     ipdSetError(ipdERR_DIFFERENT_UNITS, ">", u1, u2);
		     return NULL;
		  }
#endif /* IPD_USE_UNITS */
	       }
	       else
		  goto iiGreaterError;
	    }
	    if (a->type & ipdINTEGER)
	    {
	       if (b->type & ipdINTEGER)
		  tn->node.expr.val.boolean = (a->node.expr.val.integer >
					       b->node.expr.val.integer);
	       else if (b->type & ipdREAL)
		  tn->node.expr.val.boolean = (((double)a->node.expr.val.integer) >
					       b->node.expr.val.quantity.re);
	       else
		  goto iiGreaterError;
	    }
	    else if (a->type & ipdREAL)
	    {
	       if (b->type & ipdINTEGER)
		  tn->node.expr.val.boolean = (a->node.expr.val.quantity.re >
					       ((double)b->node.expr.val.integer));
	       else if (b->type & ipdREAL)
		  tn->node.expr.val.boolean = (a->node.expr.val.quantity.re >
					       b->node.expr.val.quantity.re);
	       else
		  goto iiGreaterError;
	    }
	    else
	       goto iiGreaterError;
	 }
	 /* Set the type. */
	 tn->type |= ipdBOOLEAN;
	 a = tn;
      }
   }
   else
      return NULL;

   return tn;

 iiGreaterError:
   ipdSetError(ipdERR_INCOMPATIBLE_TYPES, ">",
	       ipdGetTypeStr(a->type, ipdTRUE),
	       ipdGetTypeStr(b->type, ipdTRUE));
   return NULL;
} /* iiGreater */

/** @brief Calculates a <= b. */
ipdTreeNode_t *iiLowerEqual(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *op, *a, *b;

   if ((tn->type & ipdOPERATOR) && (tn->node.expr.func.op.nOperands >= 2))
   {
      ipdType_t      t;

      op = tn->node.expr.func.op.operand;

      if (!(a = iiCalculate(op, baseSect)))
	 return NULL;
      while ((op = op->next))
      {
	 /* Calculate the arguments value. */
	 if (!(b = iiCalculate(op, baseSect)))
	    return NULL;

	 t = (a->type | b->type) & ipdARRAYnVALUE;
	 /* Calculate the function value. */
	 if (t & ipdBOOLEAN)
	    goto iiLowerEqualError;
	 else if (t & ipdSTRING)
	 {
	    if (t == ipdSTRING)
	       tn->node.expr.val.boolean = (iiStrCmp(a->node.expr.val.string,
						     b->node.expr.val.string) <= 0);
	    else
	       goto iiLowerEqualError;
	 }
	 else
	 {
	    if (t & ipdQUANTITY)
	    {
	       if ((a->type & ipdQUANTITY) && (b->type & ipdQUANTITY))
	       {
#if IPD_USE_UNITS
		  if (!(iiUnitCompareable(a->node.expr.val.quantity.unit,
					  b->node.expr.val.quantity.unit)))
		  {
		     char *u1, *u2;

		     u1 = iiGetUnitString(a->node.expr.val.quantity.unit);
		     u2 = iiGetUnitString(b->node.expr.val.quantity.unit);
		     ipdSetError(ipdERR_DIFFERENT_UNITS, "<=", u1, u2);
		     return NULL;
		  }
#endif /* IPD_USE_UNITS */
	       }
	       else
		  goto iiLowerEqualError;
	    }
	    if (a->type & ipdINTEGER)
	    {
	       if (b->type & ipdINTEGER)
		  tn->node.expr.val.boolean = (a->node.expr.val.integer <=
					       b->node.expr.val.integer);
	       else if (b->type & ipdREAL)
		  tn->node.expr.val.boolean = (((double)a->node.expr.val.integer) <=
					       b->node.expr.val.quantity.re);
	       else
		  goto iiLowerEqualError;
	    }
	    else if (a->type & ipdREAL)
	    {
	       if (b->type & ipdINTEGER)
		  tn->node.expr.val.boolean = (a->node.expr.val.quantity.re <=
					       ((double)b->node.expr.val.integer));
	       else if (b->type & ipdREAL)
		  tn->node.expr.val.boolean = (a->node.expr.val.quantity.re <=
					       b->node.expr.val.quantity.re);
	       else
		  goto iiLowerEqualError;
	    }
	    else
	       goto iiLowerEqualError;
	 }
	 /* Set the type. */
	 tn->type |= ipdBOOLEAN;
	 a = tn;
      }
   }
   else
      return NULL;

   return tn;

 iiLowerEqualError:
   ipdSetError(ipdERR_INCOMPATIBLE_TYPES, "<=",
	       ipdGetTypeStr(a->type, ipdTRUE),
	       ipdGetTypeStr(b->type, ipdTRUE));
   return NULL;
} /* iiLowerEqual */

/** @brief Calculates a >= b. */
ipdTreeNode_t *iiGreaterEqual(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *op, *a, *b;

   if ((tn->type & ipdOPERATOR) && (tn->node.expr.func.op.nOperands >= 2))
   {
      ipdType_t      t;

      op = tn->node.expr.func.op.operand;

      if (!(a = iiCalculate(op, baseSect)))
	 return NULL;
      while ((op = op->next))
      {
	 /* Calculate the arguments value. */
	 if (!(b = iiCalculate(op, baseSect)))
	    return NULL;

	 t = (a->type | b->type) & ipdARRAYnVALUE;
	 /* Calculate the function value. */
	 if (t & ipdBOOLEAN)
	    goto iiGreaterEqualError;
	 else if (t & ipdSTRING)
	 {
	    if (t == ipdSTRING)
	       tn->node.expr.val.boolean = (iiStrCmp(a->node.expr.val.string,
						     b->node.expr.val.string) >= 0);
	    else
	       goto iiGreaterEqualError;
	 }
	 else
	 {
	    if (t & ipdQUANTITY)
	    {
	       if ((a->type & ipdQUANTITY) && (b->type & ipdQUANTITY))
	       {
#if IPD_USE_UNITS
		  if (!(iiUnitCompareable(a->node.expr.val.quantity.unit,
					  b->node.expr.val.quantity.unit)))
		  {
		     char *u1, *u2;

		     u1 = iiGetUnitString(a->node.expr.val.quantity.unit);
		     u2 = iiGetUnitString(b->node.expr.val.quantity.unit);
		     ipdSetError(ipdERR_DIFFERENT_UNITS, ">=", u1, u2);
		     return NULL;
		  }
#endif /* IPD_USE_UNITS */
	       }
	       else
		  goto iiGreaterEqualError;
	    }
	    if (a->type & ipdINTEGER)
	    {
	       if (b->type & ipdINTEGER)
		  tn->node.expr.val.boolean = (a->node.expr.val.integer >=
					       b->node.expr.val.integer);
	       else if (b->type & ipdREAL)
		  tn->node.expr.val.boolean = (((double)a->node.expr.val.integer) >=
					       b->node.expr.val.quantity.re);
	       else
		  goto iiGreaterEqualError;
	    }
	    else if (a->type & ipdREAL)
	    {
	       if (b->type & ipdINTEGER)
		  tn->node.expr.val.boolean = (a->node.expr.val.quantity.re >=
					       ((double)b->node.expr.val.integer));
	       else if (b->type & ipdREAL)
		  tn->node.expr.val.boolean = (a->node.expr.val.quantity.re >=
					       b->node.expr.val.quantity.re);
	       else
		  goto iiGreaterEqualError;
	    }
	    else
	       goto iiGreaterEqualError;
	 }
	 /* Set the type. */
	 tn->type |= ipdBOOLEAN;
	 a = tn;
      }
   }
   else
      return NULL;

   return tn;

 iiGreaterEqualError:
   ipdSetError(ipdERR_INCOMPATIBLE_TYPES, ">=",
	       ipdGetTypeStr(a->type, ipdTRUE),
	       ipdGetTypeStr(b->type, ipdTRUE));
   return NULL;
} /* iiGreaterEqual */

/** @brief Calculates a || b. */
ipdTreeNode_t *iiLogicOr(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *op, *a;

   if ((tn->type & ipdOPERATOR) && (tn->node.expr.func.op.nOperands >= 2))
   {
      tn->node.expr.val.boolean = ipdFALSE;
      op = tn->node.expr.func.op.operand;

      while ((op))
      {
	 /* Calculate the arguments value. */
	 if (!(a = iiCalculate(op, baseSect)))
	    return NULL;

	 /* Calculate the function value. */
	 if (a->type & ipdBOOLEAN)
	 {
	    if ((tn->node.expr.val.boolean = (tn->node.expr.val.boolean ||
					      a->node.expr.val.boolean)))
	       break;
	 }
	 else if (a->type & ipdINTEGER)
	 {
	    if ((tn->node.expr.val.boolean = (tn->node.expr.val.boolean ||
					      (ipdBoolean)a->node.expr.val.integer)))
	       break;
	 }
	 else if (a->type & ipdREAL)
	 {
	    if ((tn->node.expr.val.boolean = (tn->node.expr.val.boolean ||
					      (ipdBoolean)(a->node.expr.val.quantity.re != 0.0))))
	       break;
	 }
	 else if (a->type & ipdCOMPLEX)
	 {
	    if ((tn->node.expr.val.boolean = (tn->node.expr.val.boolean ||
					      (ipdBoolean)((a->node.expr.val.quantity.re != 0.0) ||
							 (a->node.expr.val.quantity.im != 0.0)))))
	       break;
	 }
	 else
	    goto iiLogicOrError;
	 op = op->next;
      }
      /* Set the type. */
      tn->type |= ipdBOOLEAN;
   }
   else
      return NULL;

   return tn;

 iiLogicOrError:
   ipdSetError(ipdERR_INCOMPATIBLE_TYPES, "||",
	       ipdGetTypeStr(tn->type, ipdTRUE),
	       ipdGetTypeStr(a->type, ipdTRUE));
   return NULL;
} /* iiLogicOr */

/** @brief Calculates a && b. */
ipdTreeNode_t *iiLogicAnd(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *op, *a;

   if ((tn->type & ipdOPERATOR) && (tn->node.expr.func.op.nOperands >= 2))
   {
      tn->node.expr.val.boolean = ipdTRUE;
      op = tn->node.expr.func.op.operand;

      while ((op))
      {
	 /* Calculate the arguments value. */
	 if (!(a = iiCalculate(op, baseSect)))
	    return NULL;

	 /* Calculate the function value. */
	 if (a->type & ipdBOOLEAN)
	 {
	    if (!(tn->node.expr.val.boolean = (a->node.expr.val.boolean &&
					       tn->node.expr.val.boolean)))
	       break;
	 }
	 else if (a->type & ipdINTEGER)
	 {
	    if (!(tn->node.expr.val.boolean = (tn->node.expr.val.boolean &&
					       (ipdBoolean)a->node.expr.val.integer)))
	       break;
	 }
	 else if (a->type & ipdREAL)
	 {
	    if (!(tn->node.expr.val.boolean = (tn->node.expr.val.boolean &&
					       (ipdBoolean)(a->node.expr.val.quantity.re != 0.0))))
	       break;
	 }
	 else if (a->type & ipdCOMPLEX)
	 {
	    if (!(tn->node.expr.val.boolean = (tn->node.expr.val.boolean &&
					       ((ipdBoolean)((a->node.expr.val.quantity.re != 0.0) ||
							   (a->node.expr.val.quantity.im != 0.0))))))
	       break;
	 }
	 else
	    goto iiLogicAndError;

	 op = op->next;
      }
      /* Set the type. */
      tn->type |= ipdBOOLEAN;
   }
   else
      return NULL;

   return tn;

 iiLogicAndError:
   ipdSetError(ipdERR_INCOMPATIBLE_TYPES, "&&",
	       ipdGetTypeStr(tn->type, ipdTRUE),
	       ipdGetTypeStr(a->type, ipdTRUE));
   return NULL;
} /* iiLogicAnd */

/** @brief Calculates !a. */
ipdTreeNode_t *iiLogicNot(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   if ((tn->type & ipdOPERATOR) && (tn->node.expr.func.op.nOperands == 1))
   {
      ipdTreeNode_t *a;

      /* Calculate the arguments value. */
      if (!(a = iiCalculate(tn->node.expr.func.op.operand, baseSect)))
	 return NULL;

      /* Calculate the function value. */
      if (a->type & ipdBOOLEAN)
	 tn->node.expr.val.boolean = !a->node.expr.val.boolean;
      else if (a->type & ipdINTEGER)
	 tn->node.expr.val.boolean = !(ipdBoolean)a->node.expr.val.integer;
      else if (a->type & ipdREAL)
	 tn->node.expr.val.boolean = (ipdBoolean)(a->node.expr.val.quantity.re == 0.0);
      else if (a->type & ipdCOMPLEX)
	 tn->node.expr.val.boolean = (ipdBoolean)((a->node.expr.val.quantity.re == 0.0) &&
						(a->node.expr.val.quantity.im == 0.0));
      else
      {
	 ipdSetError(ipdERR_WRONG_TYPE_OP, "!", ipdGetTypeStr(a->type, ipdTRUE));
	 return NULL;
      }
      /* Set the type. */
      tn->type |= ipdBOOLEAN;
   }
   else
      return NULL;

   return tn;
} /* iiLogicNot */

/** @brief Calulates the value of a function.

    If the parameters are constant values
    (e.g. not variables or trees again) the function call will be converted
    to its function value, e.g. sin(0.0) --> 0.0
 */
ipdTreeNode_t *iiCalculateFunction(ipdTreeNode_t    *tn,
				   ipdDynamicName_t *baseSect)
{
   if ((tn) && (tn->type & (ipdINTERNFUNCTION | ipdCCODEDFUNCTION)))
   {
      if (tn->type & ipdINTERNFUNCTION)
      {
	 ipdTreeNode_t **iiArgs = NULL;
	 ipdTreeNode_t **args;
	 ipdTreeNode_t  *func, *res;
	 ipdTreeNode_t  *val, *param;

	 if (!(tn->node.expr.func.internFunc.func))
	    return NULL;

	 if (!(iiArgs = iiAllocArgs(tn->node.expr.func.internFunc.func->parameterNum)))
	    return NULL;

	 args  = iiArgs;
	 func  = tn->node.expr.func.internFunc.func->tree;
	 param = tn->node.expr.func.internFunc.func->paramList;
	 val   = tn->node.expr.func.internFunc.param;
	 /* Calculate parameter values. */
	 /* Link the parameters to the parameters of the functions */
	 while ((param) && (val))
	 {
	    if (!(*args++ = iiCalculate(val,
					baseSect)))
	    {
	       ipdMBfree(iiArgs);
	       return NULL;
	    }
	    param = param->next;
	    val   = val->next;
	 }
	 /* Is the number of parameters correct? */
	 if ((param))
	 {
	    ipdSetError(ipdERR_TOO_FEW_PARA);
	    ipdMBfree(iiArgs);
	    return NULL;
	 }
	 if ((val))
	 {
	    ipdSetError(ipdERR_TOO_MANY_PARA);
	    ipdMBfree(iiArgs);
	    return NULL;
	 }
	 /* Link the parameters to the functions parameters */
	 args = iiArgs;
	 param = tn->node.expr.func.internFunc.func->paramList;
	 while ((param))
	 {
	    param->node.parameter.tree = *args++;
	    param = param->next;
	 }
	 /* Calculate function value and store it in the tree. So move the value */
	 if (!(res = iiCalculate(func, baseSect)))
	 {
	    if (!(ipdGetErrorStatus()))
	       ipdSetError(ipdERR_UNKIPDNOWN);
	    ipdMBfree(iiArgs);
	    return NULL;
	 }
	 iiAssignValue(tn, res);
	 ipdMBfree(iiArgs);
	 return tn;
      }
      else /* ipdCCODEDFUNCTION */
      {
	 /* We must not calculate the parameters values. This is done in
	    the C-coded function itself. */
	 /* Calculate function value. */
	 return (*(tn->node.expr.func.cCodedFunc.func))(tn, baseSect);
      }
   }
   return NULL;
} /* iiCalculateFunction */
