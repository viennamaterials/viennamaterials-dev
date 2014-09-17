/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

CONTENT:  Built-in functions

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  RK   Robert Klima,      Institute for Microelectronics, TU Vienna

  who when        what
-------------------------------------------------------------------------
  RK  1998-04-14  new from scratch
=======================================================================*/


/** @file ipdbuiltin.c
    @brief Implementation of 'built-in functions' which can be used in .ipd files
*/


/*=======================================================================
	Includes
=======================================================================*/

#include "ipdP.h"
#include <math.h>

#define IPD_MINLONG   (1L << (8 * sizeof(long) - 1))
#define IPD_MAXLONG   (~IPD_MINLONG)

/*-------------------------------------------------------------------------
  Function Support
-------------------------------------------------------------------------*/

/* #define ipdBuiltinFuncNUMBER 43 */
iiBuiltInFunc_t iiBuiltInFuncTable[ipdBuiltinFuncNUMBER] =
{
   IPD_FUNC_ENTRY(ipdFUNC_ABS,
		  "abs",
		  iiAbs,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_ACOS,
		  "acos",
		  iiACos,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_ACOSH,
		  "acosh",
		  iiACosh,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_ACOT,
		  "acot",
		  iiACot,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_ACOTH,
		  "acoth",
		  iiACoth,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_ARG,
		  "arg",
		  iiArg,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_ASIN,
		  "asin",
		  iiASin,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_ASINH,
		  "asinh",
		  iiASinh,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_ATAN,
		  "atan",
		  iiATan,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_ATANH,
		  "atanh",
		  iiATanh,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_BOOLEAN,
		  "boolean",
		  iiBoolean,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_CBRT,
		  "cbrt",
		  iiCbrt,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_COS,
		  "cos",
		  iiCos,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_COSH,
		  "cosh",
		  iiCosh,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_COT,
		  "cot",
		  iiCot,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_COTH,
		  "coth",
		  iiCoth,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_CUB,
		  "cub",
		  iiCub,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_EXP,
		  "exp",
		  iiExp,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_FABS,
		  "fabs",
		  iiAbs,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_FULLNAME,
		  "fullname",
		  iiFullname,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_GETENV,
		  "getenv",
		  iiGetEnv,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_IF,
		  "if",
		  iiIf,
		  3,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_IMAGPART,
		  "imagpart",
		  iiImagPart,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_INTEGER,
		  "integer",
		  iiInteger,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_ITERATE,
		  "iterate",
		  iiIterate,
		  1,
		  iiIterateDestructor),
   IPD_FUNC_ENTRY(ipdFUNC_LENGTH,
		  "length",
		  iiLength,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_LOG,
		  "log",
		  iiLog,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_LOG10,
		  "log10",
		  iiLog10,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_LOG2,
		  "log2",
		  iiLog2,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_PARSE_UNIT,
		  "parseunit",
		  iiParseUnit,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_POW,
		  "pow",
		  iiPow,
		  2,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_POW10,
		  "pow10",
		  iiPow10,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_POW2,
		  "pow2",
		  iiPow2,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_QUERY,
		  "query",
		  iiQuery,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_RAND,
		  "rand",
		  iiRand,
		  0,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_REAL,
		  "real",
		  iiReal,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_REALPART,
		  "realpart",
		  iiRealPart,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_SIGMA,
		  "sigma",
		  iiSigma,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_SIGN,
		  "sign",
		  iiSign,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_SIN,
		  "sin",
		  iiSin,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_SINH,
		  "sinh",
		  iiSinh,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_SQRT,
		  "sqrt",
		  iiSqrt,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_SQU,
		  "squ",
		  iiSqu,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_STRING,
		  "string",
		  iiString,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_TAN,
		  "tan",
		  iiTan,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_TANH,
		  "tanh",
		  iiTanh,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_UNIT,
		  "unit",
		  iiUnit,
		  1,
		  NULL),
   IPD_FUNC_ENTRY(ipdFUNC_VALUE,
		  "value",
		  iiValue,
		  1,
		  NULL),
};

/** @brief Binary search in a sorted list
 */
iiBuiltInFunc_t *iiFindBuiltinFunctionByName(const ipdChar *funcName)
{
   int res;
   int fl, fr, fm;

   fl = 0;
   fr = ipdBuiltinFuncNUMBER - 1;

   /* Binary search in a sorted list */
   while (fl <= fr)
   {
      fm = ((fl + fr) >> 1);
      if (!(res = iiStrCmp(iiBuiltInFuncTable[fm].name, funcName)))
      {
#if IPD_TEST_ERROR_NUMBERS
	 if (fm != iiBuiltInFuncTable[fm].funcNum)
	 {
	    ipdSetError(ipdERR_FUNC_NUMBER,
			funcName,
			iiBuiltInFuncTable[fm].funcNum,
			fm);
	 }
#endif /* IPD_TEST_ERROR_NUMBERS */
	 return &iiBuiltInFuncTable[fm];
      }
      else if (res < 0)
	 fl = fm + 1;
      else
	 fr = fm - 1;
   }
   return NULL;
} /* iiFindBuiltinFunctionByName */

/* Test for an error in the evaluation
    d: value to be checked for a valid result
    str: The error string
    return: ipdTRUE if error occured
*/
ipdBoolean ipdTestOnNumError (double d, const char *str)
{
   if (d != d)
   {
      ipdSetError(ipdERR_NAN_RESULT, (char *)str);
      return ipdTRUE;
   }
#if 0
   if (!finite(d))
   {
      ipdSetError(ipdWARN_INFINITE_RESULT, str);
      return ipdFALSE;
   }
#endif
   return ipdFALSE;
} /* ipdTestOnNumError */


/** @brief Check whether the number of parameters is valid */
static void iiCCodedParaError(ipdConstString funcName,
		       long         funcParameterNum)
{
   if (funcParameterNum == 1)
      ipdSetError(ipdERR_PARAM_NUM1,
		  funcName);
   else
      ipdSetError(ipdERR_PARAM_NUMN,
		  funcName,
		  funcParameterNum);
}

/*-------------------------------------------------------------------------
  Function Definitions
-------------------------------------------------------------------------*/

/**************************************************************
 * FORMULA : sin(x+jy) = {sin(x)*cosh(y)} + j{cos(x)*sinh(y)}
 */
ipdTreeNode_t *iiSin(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_SIN].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_SIN].name,
			iiBuiltInFuncTable[ipdFUNC_SIN].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_SIN].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* Integer */
   {
      tn->node.expr.val.quantity.re = sin((double)a->node.expr.val.integer);
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_SIN].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdREAL) /* real */
   {
      tn->node.expr.val.quantity.re = sin(a->node.expr.val.quantity.re);
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_SIN].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double re = a->node.expr.val.quantity.re;
      double im = a->node.expr.val.quantity.im;

      tn->node.expr.val.quantity.re = sin(re) * cosh(im);
      tn->node.expr.val.quantity.im = cos(re) * sinh(im);
      if ((ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_SIN].name)) ||
	  (ipdTestOnNumError (tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_SIN].name)))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_SIN].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiSin */

/**************************************************************
 * FORMULA : cos(x+jy) = {cos(x)*cosh(y)} - j{sin(x)*sinh(y)}
 */
ipdTreeNode_t *iiCos(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_COS].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_COS].name,
			iiBuiltInFuncTable[ipdFUNC_COS].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_COS].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      tn->node.expr.val.quantity.re = cos((double)a->node.expr.val.integer);
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_COS].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdREAL) /* real */
   {
      tn->node.expr.val.quantity.re = cos(a->node.expr.val.quantity.re);
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_COS].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double re = a->node.expr.val.quantity.re;
      double im = a->node.expr.val.quantity.im;

      tn->node.expr.val.quantity.re =   cos(re) * cosh(im);
      tn->node.expr.val.quantity.im = - sin(re) * sinh(im);
      if ((ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_COS].name)) ||
	  (ipdTestOnNumError (tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_COS].name)))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_COS].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiCos */

/**************************************************************
 * FORMULA : tan(x+jy) = [sin(2x) + j*sinh(2y)] / N
 *           N = cos(2x) + cosh(2y)
 */
ipdTreeNode_t *iiTan(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_TAN].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_TAN].name,
			iiBuiltInFuncTable[ipdFUNC_TAN].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_TAN].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      tn->node.expr.val.quantity.re = tan((double)a->node.expr.val.integer);
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_TAN].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdREAL) /* real */
   {
      tn->node.expr.val.quantity.re = tan(a->node.expr.val.quantity.re);
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_TAN].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdCOMPLEX) /* comnplex */
   {
      double re  = a->node.expr.val.quantity.re + a->node.expr.val.quantity.re;
      double im  = a->node.expr.val.quantity.im + a->node.expr.val.quantity.im;
      double den = cos(re) + cosh(im); /* denominator */

      if (den == 0.0)
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_TAN].name);
	 return NULL;
      }
      tn->node.expr.val.quantity.re = sin(re)  / den;
      tn->node.expr.val.quantity.im = sinh(im) / den;
      if ((ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_TAN].name)) ||
	  (ipdTestOnNumError (tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_TAN].name)))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_TAN].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiTan */

/**************************************************************
 * FORMULA : cot(x+jy) = [- sin(2x) + j*sinh(2y)] / N
 *           N = cos(2x) - cosh(2y)
 */
ipdTreeNode_t *iiCot(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_COT].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_COT].name,
			iiBuiltInFuncTable[ipdFUNC_COT].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_COT].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      double re = (double)a->node.expr.val.integer;
      double den = sin(re);

      if (den == 0.0)
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_COT].name);
	 return NULL;
      }
      tn->node.expr.val.quantity.re = cos(re) / den;
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_COT].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdREAL) /* real */
   {
      double re = a->node.expr.val.quantity.re;
      double den = sin(re);

      if (den == 0.0)
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_COT].name);
	 return NULL;
      }
      tn->node.expr.val.quantity.re = cos(re) / den;
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_COT].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double re  = a->node.expr.val.quantity.re + a->node.expr.val.quantity.re;
      double im  = a->node.expr.val.quantity.im + a->node.expr.val.quantity.im;
      double den = cos(re) - cosh(im); /* denominator */

      if (den == 0.0)
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_COT].name);
	 return NULL;
      }
      tn->node.expr.val.quantity.re = - sin(re)  / den;
      tn->node.expr.val.quantity.im =   sinh(im) / den;
      if ((ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_COT].name)) ||
	  (ipdTestOnNumError (tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_COT].name)))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_COT].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiCot */

/**************************************************************
 * FORMULA : arcsin(x+jy) = {arctan(bx/ax)} - j{ln(sqrt(ax�+bx�))}
 *           ax = -y + sqrt((sqrt((1-x�+y�)�+4*x�*y�)+(1-x�+y�))/2)
 *           bx =  x + sqrt((sqrt((1-x�+y�)�+4*x�*y�)-(1-x�+y�))/2)*sign(-x*y)
 */
ipdTreeNode_t *iiASin(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_ASIN].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_ASIN].name,
			iiBuiltInFuncTable[ipdFUNC_ASIN].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ASIN].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if ((a->type & (ipdINTEGER | ipdREAL))) /* integer, real */
   {
      double x, bx;

      if (a->type & ipdINTEGER) /* integer */
	 x = (double)a->node.expr.val.integer;
      else
	 x = a->node.expr.val.quantity.re;

      if (fabs(x) <= 1.0)
      {
	 tn->node.expr.val.quantity.re = asin(x);
	 if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ASIN].name))
	    return NULL;
	 tn->type |= ipdREAL;
      }
      else
      {
	 bx = x + sqrt(x*x - 1.0);
	 tn->node.expr.val.quantity.re = (x < 0.0) ? -ipd_PI / 2.0 : ipd_PI / 2.0;
	 tn->node.expr.val.quantity.im = - log(fabs(bx));
	 if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ASIN].name) ||
	     ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_ASIN].name))
	    return NULL;
	 tn->type |= ipdCOMPLEX;
      }
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double x, y, w1, w2, rz, ax, bx;

      x = a->node.expr.val.quantity.re;
      y = a->node.expr.val.quantity.im;

      w1 = 1.0 - x*x + y*y;
      w2 = - 2.0*x*y;
      rz = sqrt(w1*w1 + w2*w2);
      ax = -y + sqrt((rz + w1)*0.5);
      bx =  x + sqrt((rz - w1)*0.5)*((-x*y < 0.0) ? -1.0 : 1.0);

      tn->node.expr.val.quantity.re =   atan2(bx,ax);
      tn->node.expr.val.quantity.im = - log(sqrt(ax*ax + bx*bx));
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ASIN].name) ||
	  ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_ASIN].name))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ASIN].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiASin */

/**************************************************************
 * FORMULA : arccos(x+jy) = {arctan(bx/ax)} - j{ln(sqrt(ax�+bx�))}
 *           ax = x + sqrt((sqrt((x�-y�-1)�+4*x�*y�)+(x�-y�-1))/2)
 *           bx = y + sqrt((sqrt((x�-y�-1)�+4*x�*y�)-(x�-y�-1))/2)*sign(x*y)
 */
ipdTreeNode_t *iiACos(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_ACOS].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_ACOS].name,
			iiBuiltInFuncTable[ipdFUNC_ACOS].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ACOS].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if ((a->type & (ipdINTEGER | ipdREAL))) /* integer, real */
   {
      double x, ax;

      if (a->type & ipdINTEGER) /* integer */
	 x = (double)a->node.expr.val.integer;
      else
	 x = a->node.expr.val.quantity.re;

      if (fabs(x) <= 1.0)
      {
	 tn->node.expr.val.quantity.re = acos(x);
	 if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ACOS].name))
	    return NULL;
	 tn->type |= ipdREAL;
      }
      else
      {
	 ax = x + sqrt(x*x - 1.0);
	 tn->node.expr.val.quantity.re = (x < 0.0) ? ipd_PI : 0.0;
	 tn->node.expr.val.quantity.im = log(sqrt(ax*ax));
	 if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ACOS].name) ||
	     ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_ACOS].name))
	    return NULL;
	 tn->type |= ipdCOMPLEX;
      }
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double x, y, w1, w2, rz, ax, bx;

      x = a->node.expr.val.quantity.re;
      y = a->node.expr.val.quantity.im;

      w1 = x*x - y*y - 1.0;
      w2 = 2.0*x*y;
      rz = sqrt(w1*w1 + w2*w2);
      ax = x + sqrt((rz + w1)*0.5);
      bx = y + sqrt((rz - w1)*0.5)*((x*y < 0.0) ? -1.0 : 1.0);

      tn->node.expr.val.quantity.re =   atan2(bx,ax);
      tn->node.expr.val.quantity.im = - log(sqrt(ax*ax + bx*bx));
      if (x*y < 0.0)
      {
	 tn->node.expr.val.quantity.re *= -1.0;
	 tn->node.expr.val.quantity.im *= -1.0;
      }
      if ((ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ACOS].name)) ||
	  (ipdTestOnNumError (tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_ACOS].name)))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ACOS].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiACos */

/**************************************************************
 * FORMULA : arctan(x+jy) = {1/2 * arctan(bx/ax)}
 *                          - j{1/2 * ln(sqrt(ax�+bx�)/((1+y)�+x�))}
 *                          bx = 2*x; ax = 1-x�-y�;
 */
ipdTreeNode_t *iiATan(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_ATAN].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_ATAN].name,
			iiBuiltInFuncTable[ipdFUNC_ATAN].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ATAN].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      tn->node.expr.val.quantity.re = atan((double)a->node.expr.val.integer);
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ATAN].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdREAL) /* real */
   {
      tn->node.expr.val.quantity.re = atan(a->node.expr.val.quantity.re);
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ATAN].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double x, y, ax, bx, w1;

      x = a->node.expr.val.quantity.re;
      y = a->node.expr.val.quantity.im;

      ax = 1.0 - x*x - y*y;
      bx = 2.0*x;
      w1 = 1.0 + y;

      w1 = w1*w1 + x*x;
      if (w1 == 0.0)
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_ATAN].name);
	 return NULL;
      }

      tn->node.expr.val.quantity.re =   atan2(bx,ax) * 0.5;
      tn->node.expr.val.quantity.im = - log(sqrt(ax*ax + bx*bx) / w1) * 0.5;
      if ((ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ATAN].name)) ||
	  (ipdTestOnNumError (tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_ATAN].name)))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ATAN].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiATan */

/**************************************************************
 * FORMULA : arctan(x+jy) = - {1/2 * arctan(bx/ax)}
 *                          j{1/2 * ln(sqrt(ax�+bx�)/((1+y�)+x�))}
 *                          bx = -2*x; ax = y�+x�-1
 */
ipdTreeNode_t *iiACot(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_ACOT].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_ACOT].name,
			iiBuiltInFuncTable[ipdFUNC_ACOT].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ACOT].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if ((a->type & (ipdINTEGER | ipdREAL))) /* integer, real */
   {
      double x;

      if (a->type & ipdINTEGER) /* integer */
	 x = (double)a->node.expr.val.integer;
      else
	 x = a->node.expr.val.quantity.re;

      if (x < 0.0)
	 tn->node.expr.val.quantity.re = -ipd_PI * 0.5 - atan(x);
      else
	 tn->node.expr.val.quantity.re = ipd_PI * 0.5 - atan(x);
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ACOT].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double x, y, ax, bx, w1;

      x = a->node.expr.val.quantity.re;
      y = a->node.expr.val.quantity.im;

      if ((x == 0.0) && (y == 0.0))
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_ACOT].name);
	 return NULL;
      }

      ax = x*x + y*y - 1.0;
      bx = - 2.0*x;
      w1 = 1.0 + y;

      w1 = w1*w1 + x*x;
      if (w1 == 0.0)
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_ACOT].name);
	 return NULL;
      }

      tn->node.expr.val.quantity.re = - atan2(bx,ax) * 0.5;
      tn->node.expr.val.quantity.im =   log(sqrt(ax*ax + bx*bx) / w1) * 0.5;

      if ((ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ACOT].name)) ||
	  (ipdTestOnNumError (tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_ACOT].name)))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ACOT].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiACot */

/**************************************************************
 * FORMULA : sinh(x+jy) = {sinh(x)*cos(y)} + j{cosh(x)*sin(y)}
 */
ipdTreeNode_t *iiSinh(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_SINH].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_SINH].name,
			iiBuiltInFuncTable[ipdFUNC_SINH].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_SINH].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      tn->node.expr.val.quantity.re = sinh((double)a->node.expr.val.integer);
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_SINH].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdREAL) /* real */
   {
      tn->node.expr.val.quantity.re = sinh(a->node.expr.val.quantity.re);
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_SINH].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double re = a->node.expr.val.quantity.re;
      double im = a->node.expr.val.quantity.im;

      tn->node.expr.val.quantity.re = sinh(re) * cos(im);
      tn->node.expr.val.quantity.im = cosh(re) * sin(im);
      if ((ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_SINH].name)) ||
	  (ipdTestOnNumError (tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_SINH].name)))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_SINH].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiSinh */

/**************************************************************
 * FORMULA : cosh(x+jy) = {cosh(x)*cos(y)} + j{sinh(x)*sin(y)}
 */
ipdTreeNode_t *iiCosh(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_COSH].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_COSH].name,
			iiBuiltInFuncTable[ipdFUNC_COSH].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_COSH].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      tn->node.expr.val.quantity.re = cosh((double)a->node.expr.val.integer);
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_COSH].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdREAL) /* real */
   {
      tn->node.expr.val.quantity.re = cosh(a->node.expr.val.quantity.re);
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_COSH].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double re = a->node.expr.val.quantity.re;
      double im = a->node.expr.val.quantity.im;

      tn->node.expr.val.quantity.re = cosh(re) * cos(im);
      tn->node.expr.val.quantity.im = sinh(re) * sin(im);
      if ((ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_COSH].name)) ||
	  (ipdTestOnNumError (tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_COSH].name)))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_COSH].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiCosh */

/**************************************************************
 * FORMULA : tanh(x+jy) = [sinh(2x) + j*sin(2y)] / N
 *           N = cosh(2x) + cos(2y)
 */
ipdTreeNode_t *iiTanh(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   /* no points to exclude from definition area, except inf,nan */
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_TANH].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_TANH].name,
			iiBuiltInFuncTable[ipdFUNC_TANH].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_TANH].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      tn->node.expr.val.quantity.re = tanh((double)a->node.expr.val.integer);
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_TANH].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdREAL) /* real */
   {
      tn->node.expr.val.quantity.re = tanh(a->node.expr.val.quantity.re);
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_TANH].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double re  = a->node.expr.val.quantity.re + a->node.expr.val.quantity.re;
      double im  = a->node.expr.val.quantity.im + a->node.expr.val.quantity.im;
      double den = cosh(re) + cos(im); /* denominator */

      if (den == 0.0)
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_TANH].name);
	 return NULL;
      }
      tn->node.expr.val.quantity.re = sinh(re) / den;
      tn->node.expr.val.quantity.im = sin(im)  / den;
      if ((ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_TANH].name)) ||
	  (ipdTestOnNumError (tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_TANH].name)))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_TANH].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiTanh */

/**************************************************************
 * FORMULA : coth(x+jy) = [sinh(2x) - j*sin(2y)] / N
 *           N          = cosh(2x) - cos(2y)
 */
ipdTreeNode_t *iiCoth(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_COTH].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_COTH].name,
			iiBuiltInFuncTable[ipdFUNC_COTH].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_COTH].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      double re = (double)a->node.expr.val.integer;
      double den = sinh(re);

      if (den == 0.0)
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_COTH].name);
	 return NULL;
      }
      tn->node.expr.val.quantity.re = cosh(re) / den;
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_COTH].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdREAL) /* real */
   {
      double re = a->node.expr.val.quantity.re;
      double den = sinh(re);

      if (den == 0.0)
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_COTH].name);
	 return NULL;
      }
      tn->node.expr.val.quantity.re = cosh(re) / den;
      if (ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_COTH].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double re  = a->node.expr.val.quantity.re + a->node.expr.val.quantity.re;
      double im  = a->node.expr.val.quantity.im + a->node.expr.val.quantity.im;
      double den = cosh(re) - cos(im); /* denominator */

      if (den == 0.0)
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_COTH].name);
	 return NULL;
      }
      tn->node.expr.val.quantity.re =   sinh(re) / den;
      tn->node.expr.val.quantity.im = - sin(im)  / den;
      if ((ipdTestOnNumError (tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_COTH].name)) ||
	  (ipdTestOnNumError (tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_COTH].name)))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_COTH].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiCoth */

/**************************************************************
 * FORMULA : arsinh(x+jy) = {ln(ax�+bx�)} + j{arctan(bx/ax)}
 *             ax = x + sqrt((sqrt((1+x�-y�)�+4*x�*y�)+(1+x�-y�))/2)
 *             bx = y + sqrt((sqrt((1+x�-y�)�+4*x�*y�)-(1+x�-y�))/2)*sign(x*y)
 */
ipdTreeNode_t *iiASinh(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_ASINH].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_ASINH].name,
			iiBuiltInFuncTable[ipdFUNC_ASINH].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ASINH].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      double x = (double)a->node.expr.val.integer;

      tn->node.expr.val.quantity.re = log(x + sqrt(x*x + 1.0));
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ASINH].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdREAL) /* real */
   {
      double x = a->node.expr.val.quantity.re;

      tn->node.expr.val.quantity.re = log(x + sqrt(x*x + 1.0));
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ASINH].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double x, y, w1, w2, rz, ax, bx;

      x = a->node.expr.val.quantity.re;
      y = a->node.expr.val.quantity.im;

      w1 = 1.0 + x*x - y*y;
      w2 = 2.0*x*y;
      rz = sqrt(w1*w1 + w2*w2);
      ax = x + sqrt((rz + w1)*0.5);
      bx = y + sqrt((rz - w1)*0.5)*((x*y < 0.0) ? -1.0 : 1.0);

      tn->node.expr.val.quantity.re = log(sqrt(ax*ax + bx*bx));
      tn->node.expr.val.quantity.im = atan2(bx,ax);
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ASINH].name) ||
	  ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_ASINH].name))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ASINH].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiASinh */

/**************************************************************
 * FORMULA : arcosh(x+jy) = {ln(ax�+bx�)} + j{arctan(bx/ax)}
 *             ax = x + sqrt(sqrt((x�-y�-1)�+4x�y�))*cos(1/2*arctan(2xy/(x�-y�-1)))
 *             bx = y + sqrt(sqrt((x�-y�-1)�+4x�y�))*sin(1/2*arctan(2xy/(x�-y�-1)))
 */
ipdTreeNode_t *iiACosh(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_ACOSH].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_ACOSH].name,
			iiBuiltInFuncTable[ipdFUNC_ACOSH].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ACOSH].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if ((a->type & (ipdINTEGER | ipdREAL))) /* integer, real */
   {
      double x;

      if (a->type & ipdINTEGER) /* integer */
	 x = (double)a->node.expr.val.integer;
      else
	 x = a->node.expr.val.quantity.re;

      if (x < 1.0)
      {
	 if (x > -1.0)
	 {
	    double sx = 1.0 - x*x;
	    tn->node.expr.val.quantity.re = fabs(log(sqrt(x*x + fabs(sx))));
	    tn->node.expr.val.quantity.im = atan2(sqrt(sx) * ((x < 0.0) ? -1.0 : 1.0), x);
	 }
	 else
	 {
	    tn->node.expr.val.quantity.re = fabs(log(fabs(x + sqrt(x*x - 1.0))));
	    tn->node.expr.val.quantity.im = ipd_PI;
	 }
	 if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ACOSH].name) ||
	     ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_ACOSH].name))
	    return NULL;
	 tn->type |= ipdCOMPLEX;
      }
      else
      {
	 tn->node.expr.val.quantity.re = fabs(log(x + sqrt(x*x - 1.0)));
	 if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ACOSH].name))
	    return NULL;
	 tn->type |= ipdREAL;
      }
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double x, y, w1, w2, rz, ax, bx;

      x = a->node.expr.val.quantity.re;
      y = a->node.expr.val.quantity.im;

      w1 = x*x - y*y - 1.0;
      w2 = 2.0*x*y;
      rz = sqrt(w1*w1 + w2*w2);
      ax = x + sqrt((rz + w1)*0.5);
      bx = y + sqrt((rz - w1)*0.5)*((x*y < 0.0) ? -1.0 : 1.0);

      tn->node.expr.val.quantity.re = fabs(log(sqrt(ax*ax + bx*bx)));
      tn->node.expr.val.quantity.im = atan2(bx,ax) * ((x < 0.0) ? -1.0 : 1.0);
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ACOSH].name) ||
	  ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_ACOSH].name))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ACOSH].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiACosh */

/**************************************************************
 * FORMULA : artanh(x+jy) = {1/2*ln(sqrt(ax�+bx�))} + j{1/2*arctan(bx/ax)}
 *             ax = (1-x�-y�)/((1-x)�+y�))
 *             bx = (2y)/((1-x)�+y�))
 */
ipdTreeNode_t *iiATanh(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_ATANH].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_ATANH].name,
			iiBuiltInFuncTable[ipdFUNC_ATANH].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ATANH].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if ((a->type & (ipdINTEGER | ipdREAL))) /* integer, real */
   {
      double x;

      if (a->type & ipdINTEGER) /* integer */
	 x = (double)a->node.expr.val.integer;
      else
	 x = a->node.expr.val.quantity.re;

      if (fabs(x) < 1.0)
      {
	 tn->node.expr.val.quantity.re = log((1.0 + x) / (1.0 - x)) * 0.5;
	 if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ATANH].name))
	    return NULL;
	 tn->type |= ipdREAL;
      }
      else if (fabs(x) > 1.0)
      {
	 tn->node.expr.val.quantity.re = log(fabs((1.0 + x) / (1.0 - x))) * 0.5;
	 tn->node.expr.val.quantity.im = ipd_PI * ((x < 0.0) ? 0.5 : -0.5);
	 if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ATANH].name))
	    return NULL;
	 tn->type |= ipdCOMPLEX;
      }
      else
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_ATANH].name);
	 return NULL;
      }
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double x, y, ax, bx, w1;

      x = a->node.expr.val.quantity.re;
      y = a->node.expr.val.quantity.im;

      ax = 1.0 - x*x - y*y;
      bx = 2.0*y;
      w1 = x - 1.0;

      w1 = w1*w1 + y*y;
      if (w1 == 0.0)
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_ATANH].name);
	 return NULL;
      }

      tn->node.expr.val.quantity.re = log(sqrt(ax*ax + bx*bx) / w1) * 0.5;
      tn->node.expr.val.quantity.im = atan2(bx,ax) * 0.5;
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ATANH].name) ||
	  ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_ATANH].name))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ATANH].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiATanh */

/**************************************************************
 * FORMULA : arcoth(x+jy) = {1/2*ln(sqrt(ax�+bx�))} + j{1/2*arctan(bx/ax)}
 *             ax = (x�+y�-1)/((1-x)�+y�))
 *             bx = (-2y)/((1-x)�+y�))
 */

ipdTreeNode_t *iiACoth(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_ACOTH].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_ACOTH].name,
			iiBuiltInFuncTable[ipdFUNC_ACOTH].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ACOTH].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if ((a->type & (ipdINTEGER | ipdREAL))) /* integer, real */
   {
      double x;

      if (a->type & ipdINTEGER) /* integer */
	 x = (double)a->node.expr.val.integer;
      else
	 x = a->node.expr.val.quantity.re;

      if (fabs(x) > 1.0)
      {
	 tn->node.expr.val.quantity.re = log((x + 1.0) / (x - 1.0 )) * 0.5;
	 if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ACOTH].name))
	    return NULL;
	 tn->type |= ipdREAL;
      }
      else if (fabs(x) < 1.0)
      {
	 tn->node.expr.val.quantity.re = log(fabs((x + 1.0) / (x - 1.0))) * 0.5;
	 tn->node.expr.val.quantity.im = ipd_PI * ((x < 0.0) ? 0.5 : -0.5);
	 if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ACOTH].name))
	    return NULL;
	 tn->type |= ipdCOMPLEX;
      }
      else
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_ACOTH].name);
	 return NULL;
      }
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double x, y, ax, bx, w1;

      x = a->node.expr.val.quantity.re;
      y = a->node.expr.val.quantity.im;

      ax = x*x + y*y - 1.0;
      bx = - 2.0*y;
      w1 = x - 1.0;

      w1 = w1*w1 + y*y;
      if (w1 == 0.0)
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_ACOTH].name);
	 return NULL;
      }

      tn->node.expr.val.quantity.re = log(sqrt(ax*ax + bx*bx) / w1) * 0.5;
      tn->node.expr.val.quantity.im = atan2(bx,ax) * 0.5;
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ACOTH].name) ||
	  ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_ACOTH].name))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ACOTH].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiACoth */

/**************************************************************
 * FORMULA : exp(x+jy) = {exp(x)*cos(y)} +j{exp(x)*sin(y)}
*/
ipdTreeNode_t *iiExp(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_EXP].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_EXP].name,
			iiBuiltInFuncTable[ipdFUNC_EXP].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_EXP].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      tn->node.expr.val.quantity.re = exp((double)a->node.expr.val.integer);
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_EXP].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdREAL) /* real */
   {
      tn->node.expr.val.quantity.re = exp(a->node.expr.val.quantity.re);
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_EXP].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double x = exp(a->node.expr.val.quantity.re);
      double y = a->node.expr.val.quantity.im;

      tn->node.expr.val.quantity.re = x*cos(y);
      tn->node.expr.val.quantity.im = x*sin(y);
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_EXP].name) ||
	  ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_EXP].name))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_EXP].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiExp */

/**************************************************************
 * FORMULA : pow10(x+jy) = {r*cos(phi)} +j{r*sin(phi)}
 *                   r   = 10**x
 *                   phi = y*log(10);
*/
ipdTreeNode_t *iiPow10(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_POW10].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_POW10].name,
			iiBuiltInFuncTable[ipdFUNC_POW10].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_POW10].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      tn->node.expr.val.quantity.re = pow(10.0, (double)a->node.expr.val.integer);
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_POW10].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdREAL) /* real */
   {
      tn->node.expr.val.quantity.re = pow(10.0, a->node.expr.val.quantity.re);
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_POW10].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double r   = pow(10.0, a->node.expr.val.quantity.re);
      double phi = a->node.expr.val.quantity.im * ipd_LOG10;

      tn->node.expr.val.quantity.re = r*cos(phi);
      tn->node.expr.val.quantity.im = r*sin(phi);
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_POW10].name) ||
	  ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_POW10].name))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_POW10].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiPow10 */

/**************************************************************
 * FORMULA : pow2(x+jy) = {r*cos(phi)} +j{r*sin(phi)}
 *                  r   = 2**x
 *                  phi = y*log(2);
*/
ipdTreeNode_t *iiPow2(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_POW2].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_POW2].name,
			iiBuiltInFuncTable[ipdFUNC_POW2].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_POW2].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      tn->node.expr.val.quantity.re = pow(2.0, (double)a->node.expr.val.integer);
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_POW2].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdREAL) /* real */
   {
      tn->node.expr.val.quantity.re = pow(2.0, a->node.expr.val.quantity.re);
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_POW2].name))
	 return NULL;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double r   = pow(2.0, a->node.expr.val.quantity.re);
      double phi = a->node.expr.val.quantity.im * ipd_LOG2;

      tn->node.expr.val.quantity.re = r*cos(phi);
      tn->node.expr.val.quantity.im = r*sin(phi);
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_POW2].name) ||
	  ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_POW2].name))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_POW2].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiPow2 */

/**************************************************************
 * FORMULA : log(x+jy) = {log(x�+y�)/2} + j{arctan(y/x)}
 */
ipdTreeNode_t *iiLog(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_LOG].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_LOG].name,
			iiBuiltInFuncTable[ipdFUNC_LOG].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_LOG].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & (ipdINTEGER | ipdREAL)) /* integer or real */
   {
      double x;
      if (a->type & ipdINTEGER)
	 x = (double)a->node.expr.val.integer;
      else
	 x = a->node.expr.val.quantity.re;

      if (x < 0.0)
      {
	 tn->node.expr.val.quantity.re = log(-x);
	 tn->node.expr.val.quantity.im = ipd_PI;
	 tn->type |= ipdCOMPLEX;
      }
      else if (x > 0.0)
      {
	 tn->node.expr.val.quantity.re = log(x);
	 tn->type |= ipdREAL;
      }
      else
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_LOG].name);
	 return NULL;
      }
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_LOG].name))
	 return NULL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double x, y, rz;

      x = a->node.expr.val.quantity.re;
      y = a->node.expr.val.quantity.im;

      rz = x*x + y*y;
      if (rz == 0.0)
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_LOG].name);
	 return NULL;
      }
      tn->node.expr.val.quantity.re = log(rz) * 0.5;
      tn->node.expr.val.quantity.im = atan2(y,x);
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_LOG].name) ||
	  ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_LOG].name))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_LOG].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiLog */

/**************************************************************
 * FORMULA : log(x+jy) = {log(x�+y�)/2} + j{arctan(y/x)} / log(10)
 */
ipdTreeNode_t *iiLog10(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_LOG10].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_LOG10].name,
			iiBuiltInFuncTable[ipdFUNC_LOG10].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_LOG10].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & (ipdINTEGER | ipdREAL)) /* integer or real */
   {
      double x;
      if (a->type & ipdINTEGER)
	 x = (double)a->node.expr.val.integer;
      else
	 x = a->node.expr.val.quantity.re;
      if (x < 0.0)
      {
	 tn->node.expr.val.quantity.re = log(-x) * ipd_INV_LOG10;
	 tn->node.expr.val.quantity.im = ipd_PI * ipd_INV_LOG10;
	 tn->type |= ipdCOMPLEX;
      }
      else if (x > 0.0)
      {
	 tn->node.expr.val.quantity.re = log(x) * ipd_INV_LOG10;
	 tn->type |= ipdREAL;
      }
      else
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_LOG10].name);
	 return NULL;
      }
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_LOG10].name))
	 return NULL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double x, y, rz;

      x = a->node.expr.val.quantity.re;
      y = a->node.expr.val.quantity.im;

      rz = x*x + y*y;
      if (rz == 0.0)
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_LOG10].name);
	 return NULL;
      }
      tn->node.expr.val.quantity.re = log(rz) * 0.5 * ipd_INV_LOG10;
      tn->node.expr.val.quantity.im = atan2(y,x) * ipd_INV_LOG10;
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_LOG10].name) ||
	  ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_LOG10].name))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_LOG10].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiLog10 */

/**************************************************************
 * FORMULA : log(x+jy) = {log(x�+y�)/2} + j{arctan(y/x)} / log(2)
 */
ipdTreeNode_t *iiLog2(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_LOG2].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_LOG2].name,
			iiBuiltInFuncTable[ipdFUNC_LOG2].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_LOG2].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & (ipdINTEGER | ipdREAL)) /* integer or real */
   {
      double x;
      if (a->type & ipdINTEGER)
	 x = (double)a->node.expr.val.integer;
      else
	 x = a->node.expr.val.quantity.re;
      if (x < 0.0)
      {
	 tn->node.expr.val.quantity.re = log(-x) * ipd_INV_LOG2;
	 tn->node.expr.val.quantity.im = ipd_PI * ipd_INV_LOG2;
	 tn->type |= ipdCOMPLEX;
      }
      else if (x > 0.0)
      {
	 tn->node.expr.val.quantity.re = log(x) * ipd_INV_LOG2;
	 tn->type |= ipdREAL;
      }
      else
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_LOG2].name);
	 return NULL;
      }
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_LOG2].name))
	 return NULL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double x, y, rz;

      x = a->node.expr.val.quantity.re;
      y = a->node.expr.val.quantity.im;

      rz = x*x + y*y;
      if (rz == 0.0)
      {
	 ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_LOG2].name);
	 return NULL;
      }
      tn->node.expr.val.quantity.re = log(rz) * 0.5 * ipd_INV_LOG2;
      tn->node.expr.val.quantity.im = atan2(y,x) * ipd_INV_LOG2;
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_LOG2].name) ||
	  ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_LOG2].name))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_LOG2].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
}
/* iiLog2 */

/**************************************************************
 * FORMULA : (x+jy)**(x1+jy1) = {exp(ax)*cos(bx)} + j{exp(ax)*sin(bx)}
 *                ax = x2*ln(sqrt(x1�+y1�)) - y2*arctan(y1/x1)
 *                bx = y2*ln(sqrt(x1�+y1�)) + x2*arctan(y1/x1)
 */
ipdTreeNode_t *iiPow(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a, *b;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_POW].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_POW].name,
			iiBuiltInFuncTable[ipdFUNC_POW].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;
   if (!(b = iiCalculate(tn->node.expr.func.cCodedFunc.param->next, baseSect)))
      return NULL;

   if (a->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_POW].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }
   if (b->type & ipdIPDNOFUNCVAL)
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_POW].name, ipdGetTypeStr(b->type, ipdTRUE));
      return NULL;
   }

   if (!((a->type & (ipdINTEGER | ipdREAL | ipdCOMPLEX)) &&
	 (b->type & (ipdINTEGER | ipdREAL | ipdCOMPLEX))))
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_POW].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (((a->type | b->type ) & ipdCOMPLEX) || /* one of them complex ? -> all complex */
       ((a->type & ipdREAL)    && (a->node.expr.val.quantity.re < 0.0)) ||
       ((a->type & ipdINTEGER) && (a->node.expr.val.integer < 0)))
   {
      double x1, y1, x2, y2, z, r, phi;

      if (a->type & ipdREAL)
      {
	 x1  = a->node.expr.val.quantity.re;
	 y1  = 0.0;
      }
      else if (a->type & ipdINTEGER)
      {
	 x1  = (double)a->node.expr.val.integer;
	 y1  = 0.0;
      }
      else /* : if (a->type & ipdCOMPLEX) */
      {
	 x1  = a->node.expr.val.quantity.re;
	 y1  = a->node.expr.val.quantity.im;
      }

      if (b->type & ipdREAL)
      {
	 x2  = b->node.expr.val.quantity.re;
	 y2  = 0.0;
      }
      else if (b->type & ipdINTEGER)
      {
	 x2  = (double)b->node.expr.val.integer;
	 y2  = 0.0;
      }
      else /* : if (b->type & ipdCOMPLEX) */
      {
	 x2  = b->node.expr.val.quantity.re;
	 y2  = b->node.expr.val.quantity.im;
      }

      z   = x1*x1 + y1*y1;

      if ((x2 == 0.0) && (y2 == 0.0)) /* pow((x1+jy1),0) */
      {
	 if (z == 0.0) /* pow(0,0) */
	 {
	    ipdSetError(ipdERR_0TO_POW0);
	    return NULL;
	 }
	 else
	 {
	    tn->node.expr.val.integer = 1;
	    tn->type |= ipdINTEGER;
	 }
      }
      else if (z == 0.0) /* pow(0,(x2+jy2)) */
      {
	 if (y2 == 0.0)
	 {
	    if (x2 < 0.0)
	    {    /* infinite result */
	       ipdSetError(ipdERR_DIV_BY_ZERO, iiBuiltInFuncTable[ipdFUNC_POW].name);
	       return NULL;
	    }
	    else if (x1 > 0.0)
	    {
	       tn->node.expr.val.integer = 0;
	       tn->type |= ipdINTEGER;
	    }
	 }
	 else
	 {                        /* if ((x1 < 0) && (y1 < 0)) */
	    ipdSetError(ipdERR_0TO_POW_COMPLEX);
	    return NULL;
	 }
      }
      else
      {
	 phi = atan2(y1, x1);
	 z = log(z) * 0.5;
	 r = exp(x2 * z - y2 * phi);
	 phi = x2 * phi + y2 * z;

	 tn->node.expr.val.quantity.re = r * cos(phi);
	 tn->node.expr.val.quantity.im = r * sin(phi);
	 if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_POW].name) ||
	     ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_POW].name))
	    return NULL;
	 if (tn->node.expr.val.quantity.im == 0.0)
	    tn->type |= ipdREAL;
	 else
	    tn->type |= ipdCOMPLEX;
      }
   }
   else
   {
      double x1, x2;

      if (a->type & ipdINTEGER) /* a is an integer */
	 x1 = (double)a->node.expr.val.integer;
      else                      /* a is a real. */
	 x1 = a->node.expr.val.quantity.re;

      if (b->type & ipdINTEGER) /* b is an integer */
	 x2 = (double)b->node.expr.val.integer;
      else                      /* b is a real */
	 x2 = b->node.expr.val.quantity.re;

      if ((x1 == 0.0) && (x2 == 0.0))
      {
	 ipdSetError(ipdERR_0TO_POW0);
	 return NULL;
      }
      tn->node.expr.val.quantity.re = pow(x1, x2);
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_POW].name))
	 return NULL;
      tn->type |= ipdREAL;
   }

   return tn;
} /* iiPow */

/**************************************************************
 * Parses the given Unit string and returns a quantity.
 */
ipdTreeNode_t *iiParseUnit(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_PARSE_UNIT].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_PARSE_UNIT].name,
			iiBuiltInFuncTable[ipdFUNC_PARSE_UNIT].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (!(a->type & ipdSTRING))
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_PARSE_UNIT].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (!(tn->node.expr.val.quantity.unit = iiUnitHashFindUnitStr(a->node.expr.val.string)))
   {
      ipdSetError(ipdERR_INVALID_UNIT, a->node.expr.val.string);
      return NULL;
   }
   tn->node.expr.val.quantity.re = 1.0;
   tn->type |= ipdREALQUANTITY;

   return tn;
} /* iiParseUnit */

/**************************************************************
 * FORMULA : sqr(x+jy) = {x�-y�} + j{2*x*y}
 */
ipdTreeNode_t *iiSqu(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_SQU].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_SQU].name,
			iiBuiltInFuncTable[ipdFUNC_SQU].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & (ipdSTRING | ipdBOOLEAN | ipdSECTION))
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_SQU].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if ((a->type & (ipdINTEGER | ipdREAL))) /* integer, real */
   {
      double x;

      if (a->type & ipdINTEGER) /* integer */
	 x = (double)a->node.expr.val.integer;
      else
	 x = a->node.expr.val.quantity.re;

      tn->node.expr.val.quantity.re = x * x;
      tn->type |= ipdREAL;
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_SQU].name))
	 return NULL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double x, y;

      x = a->node.expr.val.quantity.re;
      y = a->node.expr.val.quantity.im;

      tn->node.expr.val.quantity.re = x*x - y*y;
      tn->node.expr.val.quantity.im = 2.0*x*y;
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_SQU].name) ||
	  ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_SQU].name))
	 return NULL;
      tn->type |= ipdCOMPLEX;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_SQU].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   if (a->type & ipdQUANTITY) /* ipdQUANTITY */
   {
#if IPD_USE_UNITS
      ipdUnit_t *resUnit = NULL;

      iiMultiplyUnits(&resUnit, a->node.expr.val.quantity.unit);
      ipd_RETURNonERRORp;
      iiMultiplyUnits(&resUnit, a->node.expr.val.quantity.unit);
      ipd_RETURNonERRORp;
      if ((resUnit))
      {
	 tn->node.expr.val.quantity.unit = iiUnitHashStoreUnit(resUnit);
	 ipd_RETURNonERRORp;
	 if (tn->node.expr.val.quantity.unit != resUnit)
	    iiFreeUnitHashNode(resUnit);
#endif /* IPD_USE_UNITS */
	 tn->type |= ipdQUANTITY;
#if IPD_USE_UNITS
      }
#endif /* IPD_USE_UNITS */
   }

   return tn;
} /* iiSqu */

/**************************************************************
 * FORMULA : sqrt(x+jy) = ax + jbx
 *                        ax = sqrt((sqrt(x�+y�)+x)/2)
 *                        bx = sqrt((sqrt(x�+y�)-x)/2)*sign(y)
 */
ipdTreeNode_t *iiSqrt(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_SQRT].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_SQRT].name,
			iiBuiltInFuncTable[ipdFUNC_SQRT].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & (ipdSTRING | ipdBOOLEAN | ipdSECTION))
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_SQRT].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & (ipdINTEGER | ipdREAL)) /* integer or real */
   {
      double x;

      if (a->type & ipdINTEGER)
	 x = (double)a->node.expr.val.integer;
      else
	 x = a->node.expr.val.quantity.re;
      if (x < 0.0)
      {
	 tn->node.expr.val.quantity.re = 0.0;
	 tn->node.expr.val.quantity.im = sqrt(-x);
	 tn->type |= ipdCOMPLEX;
      }
      else
      {
	 tn->node.expr.val.quantity.re = sqrt(x);
	 tn->type |= ipdREAL;
      }
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double x, y, rz;

      x = a->node.expr.val.quantity.re;
      y = a->node.expr.val.quantity.im;
      rz = sqrt(x*x + y*y);
      tn->node.expr.val.quantity.re = sqrt((rz + x) * 0.5);
      tn->node.expr.val.quantity.im = sqrt((rz - x) * 0.5) * ((y < 0) ? -1.0 : 1.0);
      tn->type |= ipdCOMPLEX;
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_SQRT].name) ||
	  ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_SQRT].name))
	 return NULL;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_SQRT].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   if (a->type & ipdQUANTITY)
   {
#if IPD_USE_UNITS
      ipdUnit_t *resUnit, *srcUnit;
      int        i;

      if ((srcUnit = a->node.expr.val.quantity.unit))
      {
	 resUnit = iiCreateUnitNode();
	 ipd_RETURNonERRORp;
	 for (i = 1; i <= ipduntNUM_BASIC_UNITS; i++)
	    if (srcUnit->unitRep.unitBasics[i] & 1) /* Is odd? */
	    {
	       char *u1;

	       u1 = iiGetUnitString(srcUnit);
	       ipdSetError(ipdERR_IPDNOT_SQRT_UNIT, u1);
	       iiFreeUnitHashNode(resUnit);
	       return NULL;
	    }
	    else
	       resUnit->unitRep.unitBasics[i] = srcUnit->unitRep.unitBasics[i] / 2;
	 tn->node.expr.val.quantity.unit = iiUnitHashStoreUnit(resUnit);
	 ipd_RETURNonERRORp;
	 if (tn->node.expr.val.quantity.unit != resUnit)
	    iiFreeUnitHashNode(resUnit);
	 tn->type |= ipdQUANTITY;
      }
#endif /* IPD_USE_UNITS */
   }

   return tn;
} /* iiSqrt */

/**************************************************************
 * FORMULA : cub(x+jy) = {x*(x�-3y�)} + j{y*(3x�-y�)}
 */
ipdTreeNode_t *iiCub(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_CUB].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_CUB].name,
			iiBuiltInFuncTable[ipdFUNC_CUB].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & (ipdSTRING | ipdBOOLEAN | ipdSECTION))
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_CUB].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* real */
   {
      double x;

      x = (double)a->node.expr.val.integer;
      x = x*x*x;
      if ((x >= (double)(IPD_MINLONG)) && (x <= (double)(IPD_MAXLONG)))
      {
	 tn->node.expr.val.integer = (long)x;
	 tn->type |= ipdINTEGER;
      }
      else
      {
	 tn->node.expr.val.quantity.re = x;
	 tn->type |= ipdREAL;
	 if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_CUB].name))
	    return NULL;
      }
   }
   if (a->type & ipdREAL) /* real */
   {
      double x;

      x = a->node.expr.val.quantity.re;
      tn->node.expr.val.quantity.re = x*x*x;
      tn->type |= ipdREAL;
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_CUB].name))
	 return NULL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double x, y, x2, y2;

      x = a->node.expr.val.quantity.re;
      x2 = x*x;
      y = a->node.expr.val.quantity.im;
      y2 = y*y;

      tn->node.expr.val.quantity.re = x*(x2 - 3.0*y2);
      tn->node.expr.val.quantity.im = y*(3.0*x2 - y2);

      tn->type |= ipdCOMPLEX;
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_CUB].name) ||
	  ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_CUB].name))
	 return NULL;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_CUB].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   if (a->type & ipdQUANTITY) /* ipdQUANTITY */
   {
#if IPD_USE_UNITS
      ipdUnit_t *resUnit = NULL;

      iiMultiplyUnits(&resUnit, a->node.expr.val.quantity.unit);
      ipd_RETURNonERRORp;
      iiMultiplyUnits(&resUnit, a->node.expr.val.quantity.unit);
      ipd_RETURNonERRORp;
      iiMultiplyUnits(&resUnit, a->node.expr.val.quantity.unit);
      ipd_RETURNonERRORp;
      if ((resUnit))
      {
	 tn->node.expr.val.quantity.unit = iiUnitHashStoreUnit(resUnit);
	 ipd_RETURNonERRORp;
	 if (tn->node.expr.val.quantity.unit != resUnit)
	    iiFreeUnitHashNode(resUnit);
#endif /* IPD_USE_UNITS */
	 tn->type |= ipdQUANTITY;
#if IPD_USE_UNITS
      }
#endif /* IPD_USE_UNITS */
   }

   return tn;
} /* iiCub */

/**************************************************************
 * FORMULA : cur(x+jy) = {ax * cos(bx)} + j{ax * sin(bx)}
 *              ax = cbrt(sqrt(x�+y�))
 *              bx = 1/3 * arctan(y/x)
 */
ipdTreeNode_t *iiCbrt(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_CBRT].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_CBRT].name,
			iiBuiltInFuncTable[ipdFUNC_CBRT].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & (ipdSTRING | ipdBOOLEAN | ipdSECTION))
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_CBRT].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      tn->node.expr.val.quantity.re = cbrt((double)a->node.expr.val.integer);
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdREAL) /* real */
   {
      tn->node.expr.val.quantity.re = cbrt(a->node.expr.val.quantity.re);
      tn->type |= ipdREAL;
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_CBRT].name))
	 return NULL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double x, y, phi, r;

      x = a->node.expr.val.quantity.re;
      y = a->node.expr.val.quantity.im;

      phi = atan2(y, x) / 3.0;
      r = pow(x*x + y*y, 1.0 / 6.0);  /* cbrt(abs(x + jy)) */

      tn->node.expr.val.quantity.re = r*cos(phi);
      tn->node.expr.val.quantity.im = r*sin(phi);

      tn->type |= ipdCOMPLEX;
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_CBRT].name) ||
	  ipdTestOnNumError(tn->node.expr.val.quantity.im, iiBuiltInFuncTable[ipdFUNC_CBRT].name))
	 return NULL;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_CBRT].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   if (a->type & ipdQUANTITY)
   {
#if IPD_USE_UNITS
      ipdUnit_t *resUnit, *srcUnit;
      int        i;

      if ((srcUnit = a->node.expr.val.quantity.unit))
      {
	 resUnit = iiCreateUnitNode();
	 ipd_RETURNonERRORp;
	 for (i = 1; i <= ipduntNUM_BASIC_UNITS; i++)
	    if (srcUnit->unitRep.unitBasics[i] % 3) /* Cbrt possible? */
	    {
	       char *u1;

	       u1 = iiGetUnitString(srcUnit);
	       ipdSetError(ipdERR_IPDNOT_CBRT_UNIT, u1);
	       iiFreeUnitHashNode(resUnit);
	       return NULL;
	    }
	    else
	       resUnit->unitRep.unitBasics[i] = srcUnit->unitRep.unitBasics[i] / 3;
	 tn->node.expr.val.quantity.unit = iiUnitHashStoreUnit(resUnit);
	 ipd_RETURNonERRORp;
	 if (tn->node.expr.val.quantity.unit != resUnit)
	    iiFreeUnitHashNode(resUnit);
	 tn->type |= ipdQUANTITY;
      }
#endif /* IPD_USE_UNITS */
   }

   return tn;
} /* iiCbrt */

/**************************************************************
 * FORMULA : abs(x+jy) = sqrt(ax�+bx�)
 */
ipdTreeNode_t *iiAbs(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_ABS].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_ABS].name,
			iiBuiltInFuncTable[ipdFUNC_ABS].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & (ipdSTRING | ipdBOOLEAN | ipdSECTION))
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ABS].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      if (a->node.expr.val.integer < 0)
	 tn->node.expr.val.integer = - a->node.expr.val.integer;
      else
	 tn->node.expr.val.integer =   a->node.expr.val.integer;
      tn->type |= ipdINTEGER;
   }
   else if (a->type & ipdREAL) /* real */
   {
      tn->node.expr.val.quantity.re = fabs(a->node.expr.val.quantity.re);
      tn->type |= ipdREAL;
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ABS].name))
	 return NULL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      double x, y;

      x = a->node.expr.val.quantity.re;
      y = a->node.expr.val.quantity.im;

      tn->node.expr.val.quantity.re = sqrt(x*x + y*y);
      tn->type |= ipdREAL;
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ABS].name))
	 return NULL;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ABS].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   if (a->type & ipdQUANTITY) /* quantity */
   {
#if IPD_USE_UNITS
      tn->node.expr.val.quantity.unit = iiCopyUnit(a->node.expr.val.quantity.unit);
#endif /* IPD_USE_UNITS */
      tn->type |= ipdQUANTITY;
   }

   return tn;
} /* iiAbs */

/**************************************************************
 * FORMULA : arg(x+jy) = arctan(y/x)
 */
ipdTreeNode_t *iiArg(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_ARG].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_ARG].name,
			iiBuiltInFuncTable[ipdFUNC_ARG].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & (ipdSTRING | ipdBOOLEAN | ipdSECTION))
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ARG].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if ((a->type & (ipdINTEGER | ipdREAL))) /* integer, real */
   {
      double x;

      if (a->type & ipdINTEGER) /* integer */
	 x = (double)a->node.expr.val.integer;
      else
	 x = a->node.expr.val.quantity.re;

      if (x == 0.0)
      {
	 ipdSetError(ipdERR_ARGUMENT0);
	 return NULL;
      }
      tn->node.expr.val.quantity.re = (x < 0.0) ? ipd_PI : 0.0;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      if ((a->node.expr.val.quantity.re == 0.0) &&
	  (a->node.expr.val.quantity.im == 0.0))
      {
	 ipdSetError(ipdERR_ARGUMENT0);
	 return NULL;
      }
      tn->node.expr.val.quantity.re = atan2(a->node.expr.val.quantity.im,
					    a->node.expr.val.quantity.re);
      tn->type |= ipdREAL;
      if (ipdTestOnNumError(tn->node.expr.val.quantity.re, iiBuiltInFuncTable[ipdFUNC_ARG].name))
	 return NULL;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ARG].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiArg */

/**************************************************************
 * FORMULA : realpart(x+jy) = x
 */
ipdTreeNode_t *iiRealPart(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_REALPART].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_REALPART].name,
			iiBuiltInFuncTable[ipdFUNC_REALPART].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & (ipdSTRING | ipdBOOLEAN | ipdSECTION))
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_REALPART].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      tn->node.expr.val.integer = a->node.expr.val.integer;
      tn->type |= ipdINTEGER;
   }
   else if (a->type & ipdREAL) /* real */
   {
      tn->node.expr.val.quantity.re = a->node.expr.val.quantity.re;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      tn->node.expr.val.quantity.re = a->node.expr.val.quantity.re;
      tn->type |= ipdREAL;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_REALPART].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   if (a->type & ipdQUANTITY) /* quantity */
   {
#if IPD_USE_UNITS
      tn->node.expr.val.quantity.unit = iiCopyUnit(a->node.expr.val.quantity.unit);
#endif /* IPD_USE_UNITS */
      tn->type |= ipdQUANTITY;
   }

   return tn;
} /* iiRealPart */

/**************************************************************
 * FORMULA : imagpart(x+jy) = y
 */
ipdTreeNode_t *iiImagPart(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_IMAGPART].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_IMAGPART].name,
			iiBuiltInFuncTable[ipdFUNC_IMAGPART].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & (ipdSTRING | ipdBOOLEAN | ipdSECTION))
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_IMAGPART].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      tn->node.expr.val.integer = 0;
      tn->type |= ipdINTEGER;
   }
   else if (a->type & ipdREAL) /* real */
   {
      tn->node.expr.val.quantity.re = 0.0;
      tn->type |= ipdREAL;
   }
   else if (a->type & ipdCOMPLEX) /* complex */
   {
      tn->node.expr.val.quantity.re = a->node.expr.val.quantity.im;
      tn->type |= ipdREAL;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_IMAGPART].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   if (a->type & ipdQUANTITY) /* quantity */
   {
#if IPD_USE_UNITS
      tn->node.expr.val.quantity.unit = iiCopyUnit(a->node.expr.val.quantity.unit);
#endif /* IPD_USE_UNITS */
      tn->type |= ipdQUANTITY;
   }

   return tn;
} /* iiImagPart */

/**************************************************************
 * FORMULA : sign(x) = {
 *                      -1, if x < 0
 *                      0, if x == 0
 *                      1, if x > 0
 *                     }
 */
ipdTreeNode_t *iiSign(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_SIGN].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_SIGN].name,
			iiBuiltInFuncTable[ipdFUNC_SIGN].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & (ipdSTRING | ipdBOOLEAN | ipdSECTION))
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_SIGN].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      if (a->node.expr.val.integer < 0)
	 tn->node.expr.val.integer = -1;
      else if (a->node.expr.val.integer == 0)
	 tn->node.expr.val.integer = 0;
      else
	 tn->node.expr.val.integer = 1;
      tn->type |= ipdINTEGER;
   }
   else if (a->type & ipdREAL) /* real */
   {
      if (a->node.expr.val.quantity.re < 0.0)
	 tn->node.expr.val.integer = -1;
      else if (a->node.expr.val.quantity.re == 0.0)
	 tn->node.expr.val.integer = 0;
      else
	 tn->node.expr.val.integer = 1;
      tn->type |= ipdINTEGER;
   }
   else if ((a->type & ipdCOMPLEX) && (a->node.expr.val.quantity.im == 0.0)) /* complex */
   {
      if (a->node.expr.val.quantity.re < 0.0)
	 tn->node.expr.val.integer = -1;
      else if (a->node.expr.val.quantity.re == 0.0)
	 tn->node.expr.val.integer = 0;
      else
	 tn->node.expr.val.integer = 1;
      tn->type |= ipdINTEGER;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_SIGN].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiSign */


/**************************************************************
 * FORMULA : sigma(x) = {1 , x > 0
 *                       0 , else}
 */
ipdTreeNode_t *iiSigma(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_SIGMA].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_SIGMA].name,
			iiBuiltInFuncTable[ipdFUNC_SIGMA].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & (ipdSTRING | ipdBOOLEAN | ipdSECTION))
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_SIGMA].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdINTEGER) /* integer */
   {
      if (a->node.expr.val.integer < 0)
	 tn->node.expr.val.integer = 0;
      else if (a->node.expr.val.integer == 0)
      {
	 tn->node.expr.val.integer = 1;
	 ipdSetError(ipdERR_HEAVYSIDE_FUNC, iiBuiltInFuncTable[ipdFUNC_SIGMA].name);
      }
      else
	 tn->node.expr.val.integer = 1;
      tn->type |= ipdINTEGER;
   }
   else if (a->type & ipdREAL) /* real */
   {
      if (a->node.expr.val.quantity.re < 0.0)
	 tn->node.expr.val.integer = 0;
      else if (a->node.expr.val.quantity.re == 0.0)
      {
	 tn->node.expr.val.integer = 1;
	 ipdSetError(ipdERR_HEAVYSIDE_FUNC, iiBuiltInFuncTable[ipdFUNC_SIGMA].name);
      }
      else
	 tn->node.expr.val.integer = 1;
      tn->type |= ipdINTEGER;
   }
   else if ((a->type & ipdCOMPLEX) && (a->node.expr.val.quantity.im == 0.0)) /* complex */
   {
      if (a->node.expr.val.quantity.re < 0.0)
	 tn->node.expr.val.integer = 0;
      else if (a->node.expr.val.quantity.re == 0.0)
      {
	 tn->node.expr.val.integer = 1;
	 ipdSetError(ipdERR_HEAVYSIDE_FUNC, iiBuiltInFuncTable[ipdFUNC_SIGMA].name);
      }
      else
	 tn->node.expr.val.integer = 1;
      tn->type |= ipdINTEGER;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_SIGMA].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiSigma */

/** @brief Returns the fullname of a variable. */
ipdTreeNode_t *iiFullname(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_FULLNAME].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_FULLNAME].name,
			iiBuiltInFuncTable[ipdFUNC_FULLNAME].parameterNum);
      return NULL;
   }
   if (!(baseSect))
   {
      if (!(ipdGetErrorStatus()))
	 ipdSetError(ipdERR_UNKIPDNOWN);
      return NULL;
   }

   /* Get the variable node. */
   a = tn->node.expr.func.cCodedFunc.param;
   iiClearValue(tn);
   /* We need not calculate the parameters values.
      We only need the fullname. */
   if ((a) &&(a->type & ipdVARIABLE_REF))
   {
      char *name = NULL;
      long  len  = 0;

      if (!(a->node.expr.func.varref.name) || !(*a->node.expr.func.varref.name))
      {
	 ipdSetError(ipdERR_VAR_IPDNOT_FOUND, "??");
	 return NULL;
      }
      /* Create the name from the baseSection name and the relative name
	 of the variable. */
      iiStrExtend(&name, &len, baseSect->name, ipdFALSE);
      iiStrExtend(&name, &len, a->node.expr.func.varref.name, ipdTRUE);
      if (ipdGetErrorStatus())
      {
	 ipdMBfree(name);
	 return NULL;
      }

      /* Correct the name.*/
      iiCorrectName(name);

      tn->node.expr.val.string = name;
      tn->type |= ipdSTRING;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_FULLNAME].name, "Variable reference expected.");
      return NULL;
   }

   return tn;
} /* iiFullname */

/** @brief Evaluates a variable which is specified by its name. */
ipdTreeNode_t *iiQuery(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t    *a;
   ipdDynamicName_t *dnn;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_QUERY].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_QUERY].name,
			iiBuiltInFuncTable[ipdFUNC_QUERY].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (!(a->type & ipdSTRING))
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_LENGTH].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   if (!(dnn = iiCreateDNNode()) ||
       !iiStrExtend(&(dnn->name),
		    &(dnn->len),
		    baseSect->name,
		    ipdFALSE) ||
       !iiStrExtend(&(dnn->name),
		    &(dnn->len),
		    a->node.expr.val.string,
		    ipdTRUE))
      return NULL;
   iiCorrectName(dnn->name);
   a = ipdEvalByName(dnn->name, ipdTRUE);
   iiFreeDNNode(dnn);
   if (!(a))
      return NULL;
   iiClearValue(tn);
   iiAssignValue(tn, a);

   return tn;
} /* iiQuery */

/** @brief Returns a random number in the range between 0.0 and 1.0

    Interval: [ 0.0 , 1.0 ].
    The seed of the random number generator is set by the time of first invocation of the call.
 */
ipdTreeNode_t *iiRand(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   static long seed = 0;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_RAND].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_RAND].name,
			iiBuiltInFuncTable[ipdFUNC_RAND].parameterNum);
      return NULL;
   }
   if (!(baseSect))
   {
      if (!(ipdGetErrorStatus()))
	 ipdSetError(ipdERR_UNKIPDNOWN);
      return NULL;
   }

   if (!seed)
   {
      seed = (long)time(0);
      srand((unsigned int)seed);
   }
   iiClearValue(tn);
   tn->type |= ipdREAL;
   tn->node.expr.val.quantity.re = ((double)rand())/RAND_MAX;

   return tn;
} /* iiRand */

/** @brief Returns the length of the array which must be given as argument. */
ipdTreeNode_t *iiLength(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_LENGTH].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_LENGTH].name,
			iiBuiltInFuncTable[ipdFUNC_LENGTH].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (!(a->type & ipdARRAYVAL))
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_LENGTH].name, ipdGetTypeStr(a->type, ipdTRUE));
      ipdSetError(ipdERR_EXPECT_ARRAY);
      return NULL;
   }

   iiClearValue(tn);
   tn->node.expr.val.integer = a->node.expr.val.arrayval.len;
   tn->type |= ipdINTEGER;

   return tn;
} /* iiLength */

/** @brief  Returns the value of the environment variable given by its name. */
ipdTreeNode_t *iiGetEnv(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_GETENV].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_GETENV].name,
			iiBuiltInFuncTable[ipdFUNC_GETENV].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   iiClearValue(tn);
   if (a->type & ipdSTRING)
   {
      if ((a->node.expr.val.string))
      {
	 char *envStr;

	 if (!(envStr = getenv(a->node.expr.val.string)))
	 {
	    ipdSetError(ipdERR_INVALID_ENV, a->node.expr.val.string);
	    return NULL;
	 }
	 if (!(tn->node.expr.val.string = iiStrDup(envStr)))
	    return NULL;
      }
      else
      {
	 ipdSetError(ipdERR_INVALID_ENV);
	 return NULL;
      }
      tn->type |= ipdSTRING;
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_GETENV].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   return tn;
} /* iiGetEnv */

/** @brief Returns the value of the environment variable given by its name. */
ipdTreeNode_t *iiIterate(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;
   ipdIterator_t *iNode;
   ipdBoolean       isThereItem;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_ITERATE].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_ITERATE].name,
			iiBuiltInFuncTable[ipdFUNC_ITERATE].parameterNum);
      return NULL;
   }

   if (!(ipdPushCS()))
       return NULL;

   iiClearValue(tn);
   if ((tn->node.expr.func.cCodedFunc.userData))
   {
      iNode = *(ipdIterator_t **)tn->node.expr.func.cCodedFunc.userData;
      isThereItem = iiIteratorDoNextFlags(iNode, ipdVF_AUXILIARY);
   }
   else
   {
      /* Calculate the arguments value. */
      if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      {
	 ipdPopCS();
	 return NULL;
      }
      if (a->type & ipdSTRING)
      {
	 if (!(a->node.expr.val.string))
	 {
	    ipdSetError(ipdERR_INVALID_NAME, "");
	    ipdPopCS();
	    return NULL;
	 }
	 if (!(tn->node.expr.func.cCodedFunc.userData = (void *)ipdMBalloc(sizeof(ipdIterator_t *))))
	 {
	    ipdPopCS();
	    return NULL;
	 }
	 if (!(ipdIteratorNewByName((ipdIterator_t **)tn->node.expr.func.cCodedFunc.userData,
				    a->node.expr.val.string,
				    ipdSECTION,
				    ipdANY)))
	 {
	    ipdSetError(ipdERR_IPDNOT_FOUND, a->node.expr.val.string);
	    ipdMBfree(tn->node.expr.func.cCodedFunc.userData);
	    tn->node.expr.func.cCodedFunc.userData = NULL;
	    ipdPopCS();
	    return NULL;
	 }
	 iNode = *(ipdIterator_t **)tn->node.expr.func.cCodedFunc.userData;
	 if (!(isThereItem = iiIteratorDoStepFlags(iNode, ipdVF_AUXILIARY)))
	 {
	    if (!(ipdGetErrorStatus()))
	       ipdSetError(ipdERR_UNKIPDNOWN);
	    ipdPopCS();
	    return NULL;
	 }
      }
      else
      {
	 ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_ITERATE].name, ipdGetTypeStr(a->type, ipdTRUE));
	 ipdPopCS();
	 return NULL;
      }
   }

   if (isThereItem)
   {
      tn->type |= ipdSTRING;
      if (!(tn->node.expr.val.string = iiStrDup(iNode->tn->node.sv.name)))
      {
	 ipdPopCS();
	 return NULL;
      }
   }
   else if (!ipdGetErrorStatus())
   {
      tn->type |= ipdSTRING;
      tn->node.expr.val.string = NULL;
   }
   else
   {
      ipdPopCS();
      return NULL;
   }

   ipdPopCS();
   return tn;
} /* iiIterate */

/** @brief Destructor for an iterator */
ipdBoolean iiIterateDestructor(ipdTreeNode_t *tn)
{
   if ((tn) && (tn->node.expr.func.cCodedFunc.userData))
   {
      ipdIteratorFree(*(ipdIterator_t **)tn->node.expr.func.cCodedFunc.userData);
      ipdMBfree(tn->node.expr.func.cCodedFunc.userData);
      tn->node.expr.func.cCodedFunc.userData = NULL;
   }
   return ipdTRUE;
}

/** @brief If-statement. */
ipdTreeNode_t *iiIf(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;
   ipdBoolean       condition;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_IF].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_IF].name,
			iiBuiltInFuncTable[ipdFUNC_IF].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdBOOLEAN)
   {
      condition = a->node.expr.val.boolean;
   }
   else if (a->type & ipdINTEGER)
   {
      condition = (ipdBoolean)a->node.expr.val.integer;
   }
   else if (a->type & ipdREAL)
   {
      condition = (ipdBoolean)(a->node.expr.val.quantity.re != 0.0);
   }
   else if (a->type & ipdCOMPLEX)
   {
      condition = (ipdBoolean)((a->node.expr.val.quantity.re != 0.0) ||
			     (a->node.expr.val.quantity.im != 0.0));
   }
   else
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_IF].name, ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   if (!(a = ((condition) ? tn->node.expr.func.cCodedFunc.param->next :
	      tn->node.expr.func.cCodedFunc.param->next->next)))
      return NULL;

   if (a->type & ipdSECTION_REF)
      return a;
   if ((a = iiCalculate(a, baseSect)))
   {
      iiClearValue(tn);
      iiAssignValue(tn, a);
      ipd_RETURNonERRORp;
      return tn;
   }

   return NULL;
} /* iiIf */

/** @brief  Returns the value of a given expression. Used to remove units. */
ipdTreeNode_t *iiValue(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_VALUE].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_VALUE].name,
			iiBuiltInFuncTable[ipdFUNC_VALUE].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;
   if (!(a->type & ipdVALUE))
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_VALUE].name,
		  ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   iiAssignValue(tn, a);
   if (tn->type & ipdQUANTITY)
   {
      iiUnitHashRemoveUnit(tn->node.expr.val.quantity.unit);
      tn->node.expr.val.quantity.unit = NULL;
      tn->type ^= ipdQUANTITY;
   }
   return tn;
} /* iiValue */

/** @brief Returns the unit of a given expression as a string. If no unit is detected "1" will be returned. */
ipdTreeNode_t *iiUnit(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;
   char          *unitStr;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_UNIT].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_UNIT].name,
			iiBuiltInFuncTable[ipdFUNC_UNIT].parameterNum);
      return NULL;
   }

   /* Get the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;
   if (a->type & (ipdSTRING | ipdBOOLEAN | ipdSECTION))
   {
      ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_UNIT].name,
		  ipdGetTypeStr(a->type, ipdTRUE));
      return NULL;
   }

   iiClearValue(tn);
   if (a->type & ipdQUANTITY)
   {
      if ((unitStr = iiGetUnitString(a->node.expr.val.quantity.unit)))
	 unitStr = iiStrDup(unitStr);
   }
   else
      unitStr = iiStrDup("1");

   if (!(unitStr))
      return NULL;

   tn->node.expr.val.string = unitStr;
   tn->type |= ipdSTRING;

   return tn;
} /* iiUnit */

/** @brief Performs a cast to an integer. */
ipdTreeNode_t *iiInteger(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_INTEGER].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_INTEGER].name,
			iiBuiltInFuncTable[ipdFUNC_INTEGER].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdVALUE)
   {
      if (!(iiCastOperation(tn, a, ipdINTEGER, ipdTRUE)))
	 return NULL;
      return tn;
   }

   ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_INTEGER].name, ipdGetTypeStr(a->type, ipdTRUE));
   return NULL;
} /* iiInteger */

/** @brief Performs a cast to a real number. */
ipdTreeNode_t *iiReal(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_REAL].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_REAL].name,
			iiBuiltInFuncTable[ipdFUNC_REAL].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdVALUE)
   {
      if (!(iiCastOperation(tn, a, ipdREAL, ipdTRUE)))
	 return NULL;
      return tn;
   }

   ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_REAL].name, ipdGetTypeStr(a->type, ipdTRUE));
   return NULL;
} /* iiReal */

/** @brief Performs a cast to a string. */
ipdTreeNode_t *iiString(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_STRING].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_STRING].name,
			iiBuiltInFuncTable[ipdFUNC_STRING].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdVALUE)
   {
      if (!(iiCastOperation(tn, a, ipdSTRING, ipdTRUE)))
	 return NULL;
      return tn;
   }

   ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_STRING].name, ipdGetTypeStr(a->type, ipdTRUE));
   return NULL;
} /* iiString */

/** @brief Performs a cast to a boolean. */
ipdTreeNode_t *iiBoolean(ipdTreeNode_t *tn, ipdDynamicName_t *baseSect)
{
   ipdTreeNode_t *a;

   if (ipdListLen(tn->node.expr.func.cCodedFunc.param) != iiBuiltInFuncTable[ipdFUNC_BOOLEAN].parameterNum)
   {
      iiCCodedParaError(iiBuiltInFuncTable[ipdFUNC_BOOLEAN].name,
			iiBuiltInFuncTable[ipdFUNC_BOOLEAN].parameterNum);
      return NULL;
   }

   /* Calculate the arguments value. */
   if (!(a = iiCalculate(tn->node.expr.func.cCodedFunc.param, baseSect)))
      return NULL;

   if (a->type & ipdVALUE)
   {
      if (!(iiCastOperation(tn, a, ipdBOOLEAN, ipdTRUE)))
	 return NULL;
      return tn;
   }

   ipdSetError(ipdERR_WRONG_PARA_TYPE, iiBuiltInFuncTable[ipdFUNC_BOOLEAN].name, ipdGetTypeStr(a->type, ipdTRUE));
   return NULL;
} /* iiBoolean */
