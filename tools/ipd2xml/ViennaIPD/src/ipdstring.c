/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

#include "ipdstring.h"

/** @file ipdstring.c
    @brief Provides definitions of several string functions not provided by the C standard library and a regular expression engine.

    Provides definitions of several string functions not provided by the C standard library and a regular expression engine.
*/

/* global #define/struct/typedef/VAR/FUNC declarations ------------------- */


/** @brief ipdvToupper() converts all lower case characters in the string ps to upper case and returns the original pointer. 
    @param ps string to upper case
    @return the original pointer
*/
ipdString ipdvToupper(ipdString ps)
{
   ipdString sp;

   sp = ps;
   while (*sp) {
      if (islower(*sp))
	 *sp = *sp + 'A' - 'a';	
      sp++;
   }
   return (ps);			
}

/** @brief ipdvTolower() converts all upper case characters in the string ps to lower case and returns the original pointer. 
    @param ps string to lower case
    @return the original pointer
*/
ipdString ipdvTolower(ipdString ps)
{
   ipdString sp;

   sp = ps;
   while (*sp) {
      if (isupper(*sp))
	 *sp = *sp + 'a' - 'A';	
      sp++;
   }
   return (ps);	
}

/** @brief Searches for the last occurrence of the string str2 within the string str1.
    @param str1 string to search in
    @param str2 string to search for in str1
    @return Pointer to the last occurrence if found, otherwise ipdNULL.
 */
ipdString ipdvStrrstr(
	ipdString str1,		/* [I] string to search in */
	ipdConstString str2)	/* [I] string to search for in {\bf str1} */
{
   ipdChar fchar = *str2;
   ipdLong len2 = ipdvStrlen(str2) - 1;

   str2++;			/* from second character on */
   while ((str1 = ipdvStrrchr(str1, fchar)) != ipdNULL) {
      if ((len2 == 0) || (ipdvStrncmp(str1+1, str2, (unsigned int) len2) == 0))
	 return (str1);		/* found it */
      str1--;			/* skip first character */
   }

   return (ipdNULL);
}



/*******************************************************

				REGEX PART	
	
********************************************************/


#if !ipdHasPosixRegexp
/*
 * this structure is used to evaluate the regular expression
 */
typedef struct _ipdvexpexec_
{
   ipdConstString reginput;	/* String-input pointer. */
   ipdConstString regbol;		/* Beginning of input, for ^ check. */
   ipdConstString regstartp[ipdvexpNSUBEXP];	/* startp array. */
   ipdConstString regendp[ipdvexpNSUBEXP];	/* endp array. */
   ipdLong reglen[ipdvexpNSUBEXP];	/* length of expression */
} ipdvexpExec;

static ipdBoolean ipdregtry(ipdvexpExec *exec,ipdrgxRegexpPtr prog, ipdConstString string);
static ipdBoolean ipdregmatch(ipdvexpExec *exec, ipdByte *prog);
static ipdLong ipdregrepeat(ipdvexpExec *exec, ipdByte *p);

#endif /* !ipdHasPosixRegexp */


/** @brief Checks the given string against the compiled regular expression prog.

    Checks the given string against the compiled regular expression prog. On succes, the
    start and end position of the match is returned in the variables pointed
    by start and end, if they are not ipdNULL.
    @param prog the compiled regular expression
    @param string string, where the regular expression should be searched
    @param start start position of found match
    @param end end position + 1 of found match
    @return ipdTRUE if the expression is found in the string, else ipdFALSE.  */
ipdBoolean ipdrgxExecute(
	ipdrgxRegexpPtr prog,
	ipdConstString string,
	ipdLong *start,
	ipdLong *end)
{
#if ipdHasPosixRegexp
   ipdregmatch_t regs[1];
   int err;
   
   err = regexec (prog, string, 1, regs, 0);
   
   if (start)
      *start = regs[0].rm_so; 
   if (end)
      *end   = regs[0].rm_eo; 
   return((ipdBoolean) (err == 0));
#else
   ipdBoolean retval;
   ipdString copy;
   ipdvexpExec exec;

   /* Be paranoid... */
   if ((prog == ipdNULL) || (string == ipdNULL)) {
      /** regerror("ipdNULL parameter"); string is OK, but prog ?? */
      return (ipdFALSE);
   }

   /* Check validity of program. */
   if (*(prog->program) != (ipdByte)ipdvexpMAGIC) {
      /* regerror("corrupted program"); */
      return(ipdFALSE);
   }
   if (prog->icase) {
      copy = ipdvStrdup(string);
      if (copy == ipdNULL)
         return(ipdFALSE);        /* out of memory */
      
      ipdvTolower(copy);           /* lower case is ignore case */
   } else
      copy = (ipdString)string;

   /* If there is a "must appear" string, look for it. */
   if (prog->regmust != ipdNULL) {
      if (ipdvStrstr((ipdString)copy, prog->regmust) == ipdNULL) /* not present */
	 return(ipdFALSE);
   }

   /* Mark beginning of line for ^ . */
   exec.regbol = copy;

   /* Simplest case: anchored match need be tried only once. */
   if (prog->reganch) {
      if (ipdregtry(&exec, prog, copy)) {
	 if (start != ipdNULL)
	    *start = 0;
	 if (end != ipdNULL)
	    *end = exec.regendp[0] - copy;
	 retval = ipdTRUE;
      } else
	 retval = ipdFALSE;
   } else {
      /* Messy cases:  unanchored match. */
      ipdConstString s = copy;
      retval = ipdFALSE;
      if (prog->regstart != '\0') {
	 /* We know what char it must start with. */
	 while (((s = ipdvStrchr(s, prog->regstart)) != ipdNULL) && !retval) {
	    if (ipdregtry(&exec, prog, s)) {
	       if (start != ipdNULL)
		  *start = exec.regstartp[0] - copy;
	       if (end != ipdNULL)
		  *end = exec.regendp[0] - copy;
	       retval = ipdTRUE;
	    }
	    s++;
	 }
      } else {
	 /* We don't -- general case. */
	 do {
	    if (ipdregtry(&exec, prog, s)) {
	       if (start != ipdNULL)
		  *start = exec.regstartp[0] - copy;
	       if (end != ipdNULL)
		  *end = exec.regendp[0] - copy;
	       retval = ipdTRUE;
	    }
	 } while ((*s++ != '\0') && !retval);
      }
   }
   if (prog->icase)
      ipdvFree(copy);              /* free duplicate for lower case */
   return(retval);
#endif /* ipdHasPosixRegexp */
}

#if !ipdHasPosixRegexp
/** @brief Tries to match the given compiled regular expression at the given string. 
    @param exec structure for match analysis
    @param prog the compiled regular expression 
    @param string the input string
*/
static ipdBoolean ipdregtry(
	ipdvexpExec *exec,
	ipdrgxRegexpPtr prog,
	ipdConstString string)
{
   ipdInt i;
   ipdConstString *sp;
   ipdConstString *ep;

   exec->reginput = string;
   sp = exec->regstartp;
   ep = exec->regendp;
   for (i = 0; i < ipdvexpNSUBEXP; i++) {
      *sp++ = ipdNULL;
      *ep++ = ipdNULL;
   }
   if (ipdregmatch(exec, prog->program + 1)) {
      exec->regstartp[0] = string;
      exec->regendp[0] = exec->reginput;
      return(ipdTRUE);
   } else
      return(ipdFALSE);
}


/** @brief Main regular expression matching routine which does all the work.

    Main matching routine and does all the work.
    Conceptually the strategy is simple:  check to see whether the current
    node matches, call self recursively to see whether the rest matches,
    and then act accordingly.  In practice we make some effort to avoid
    recursion, in particular by going through "ordinary" nodes (that don't
    need to know whether the rest of the match failed) by a loop instead of
    by recursion.  
    @param exec structure for match analysis
    @param prog the compiled program part
    @return boolean indicating success of the operation
*/
static ipdBoolean ipdregmatch(
	ipdvexpExec *exec,		/* [IO] structure for match analysis */
	ipdByte *prog)		/* [I] the compiled program part */
{
   ipdByte *scan;		/* Current node. */
   ipdByte *next;		/* Next node. */

   scan = prog;
   while (scan != ipdNULL) {
      next = ipdregnext(scan);

      switch (ipdvexpOP(scan)) {
      case ipdvexpBOL:
	 if (exec->reginput != exec->regbol)
	    return(ipdFALSE);
	 break;
      case ipdvexpEOL:
	 if (*(exec->reginput) != '\0')
	    return(ipdFALSE);
	 break;
      case ipdvexpWORDA:
	 /* Must be looking at a letter, digit */
	 /* previous has to be a non letter, digit */
	 if (!isalnum(*(exec->reginput)) ||
	     ((exec->reginput > exec->regbol) && isalnum(exec->reginput[-1])))
	    return(ipdFALSE);
	 break;
      case ipdvexpWORDZ:
	 /* Must be looking at non letter, digit */
	 /* previous has to be a letter, digit */
	 if (isalnum(*(exec->reginput)) ||
	     (exec->reginput == exec->regbol) || !isalnum(exec->reginput[-1]))
	    return(ipdFALSE);
	 break;
      case ipdvexpANY:
	 if (*(exec->reginput) == '\0')
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpEXACTLY:
         {
	    ipdByte  *opnd;

	    opnd = ipdipdvexpOPERAND(scan);
	    /* Inline the first character, for speed. */
	    if (((ipdChar)*opnd) != *(exec->reginput))
	       return(ipdFALSE);
	    opnd++;		/* rest of string */
	    exec->reginput++;
	    if (*opnd != '\0') {
	       ipdLong len = ipdvStrlen((ipdString)opnd);
	       if (ipdvStrncmp((ipdString)opnd, exec->reginput, len) != 0)
		  return(ipdFALSE);
	       exec->reginput += len;
	    }
	 }
	 break;
      case ipdipdvexpANYOF:
	 if (*(exec->reginput) == '\0' ||
	     ipdvStrchr((ipdString)ipdipdvexpOPERAND(scan), *(exec->reginput)) == ipdNULL)
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdipdvexpANYBUT:
	 if (*(exec->reginput) == '\0' ||
	     ipdvStrchr((ipdString)ipdipdvexpOPERAND(scan), *(exec->reginput)) != ipdNULL)
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpRAN:
	 if (*(exec->reginput) == '\0' ||
	     ((*(exec->reginput) < *(ipdChar *)ipdipdvexpOPERAND(scan)) ||
	      (*(exec->reginput) > *(ipdChar *)(ipdipdvexpOPERAND(scan)+1))))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdipdvexpRANBUT:
	 if (*(exec->reginput) == '\0' ||
	     ((*(exec->reginput) >= *(ipdChar *)ipdipdvexpOPERAND(scan)) ||
	      (*(exec->reginput) <= *(ipdChar *)(ipdipdvexpOPERAND(scan)+1))))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpLETTER:
	 if (!isalpha(*(exec->reginput)))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpUPPER:
	 if (!isupper(*(exec->reginput)))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpLOWER:
	 if (!islower(*(exec->reginput)))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpDIGIT:
	 if (!isdigit(*(exec->reginput)))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpXDIGIT:
	 if (!isxdigit(*(exec->reginput)))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpALNUM:
	 if (!isalnum(*(exec->reginput)))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpSPACE:
	 if (!isspace(*(exec->reginput)))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpPRINT:
	 if (!isprint(*(exec->reginput)))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpPUNCT:
	 if (!ispunct(*(exec->reginput)))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpCNTRL:
	 if (!iscntrl(*(exec->reginput)))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpNLETTER:
	 if (isalpha(*(exec->reginput)) || (*exec->reginput == '\0'))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpNUPPER:
	 if (isupper(*(exec->reginput)) || (*exec->reginput == '\0'))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpNLOWER:
	 if (islower(*(exec->reginput)) || (*exec->reginput == '\0'))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpNDIGIT:
	 if (isdigit(*(exec->reginput)) || (*exec->reginput == '\0'))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpNXDIGIT:
	 if (isxdigit(*(exec->reginput)) || (*exec->reginput == '\0'))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpNALNUM:
	 if (isalnum(*(exec->reginput)) || (*exec->reginput == '\0'))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpNSPACE:
	 if (isspace(*(exec->reginput)) || (*exec->reginput == '\0'))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpNPRINT:
	 if (isprint(*(exec->reginput)) || (*exec->reginput == '\0'))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpNPUNCT:
	 if (ispunct(*(exec->reginput)) || (*exec->reginput == '\0'))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpNCNTRL:
	 if (iscntrl(*(exec->reginput)) || (*exec->reginput == '\0'))
	    return(ipdFALSE);
	 exec->reginput++;
	 break;
      case ipdvexpFLOAT:
	 {
	    ipdConstString save = exec->reginput;
	    ipdString end;
	    ipdLong len;
	    double temp;
	    ipdChar c = *(exec->reginput);
	    ipdByte nextch;

	    if (!(isdigit(c) || (c == '.') || (c == '+') || (c == '-')))
	       return(ipdFALSE);
	    temp = strtod(save, &end); 
	    if (end == ipdNULL)
	       return(ipdFALSE);
	    len = end - save;
	    exec->reginput = end;
	    /*
	     * Lookahead to avoid useless match attempts
	     * when we know what character comes next.
	     */
	    nextch = '\0';
	    if (ipdvexpOP(next) == ipdvexpEXACTLY)
	       nextch = *ipdipdvexpOPERAND(next);
	    do {
	       /* If it could work, try it. */
	       if ((nextch == (ipdByte)'\0') ||
		   (((ipdByte)*(exec->reginput)) == nextch))
		  if (ipdregmatch(exec, next))
		     return(ipdTRUE);
	       /* Couldn't or didn't -- back up. */
	       do {
		  len--;
		  c = save[len-1];
	       } while ((len > 0) && !(isdigit(c) || ((c == '.') && (len > 1))));
	       exec->reginput = save + len;
	    } while(len > 0);
	    return(ipdFALSE);
	 }
      case ipdvexpWORD:
	 {
	    ipdConstString ptr = exec->reginput;
	    /* Must be looking at a letter */
	    /* previous has to be a space or begin */
	    if (!isalpha(*ptr) ||
		((ptr > exec->regbol) && !isspace(ptr[-1])))
	       return(ipdFALSE);
	    while(isalpha(*ptr)) ptr++;
	    /* we have to look at space or at end of string */
	    if ((*ptr == '\0') || isspace(*ptr) || ispunct(*ptr)) {
	       exec->reginput = ptr;
	       return(ipdTRUE);
	    } else
	       return(ipdFALSE);
	 }

      case ipdvexpIPDNOTHING:
	 break;
      case ipdvexpBACK:
	 break;
      case ipdipdvexpOPEN+1:
      case ipdipdvexpOPEN+2:
      case ipdipdvexpOPEN+3:
      case ipdipdvexpOPEN+4:
      case ipdipdvexpOPEN+5:
      case ipdipdvexpOPEN+6:
      case ipdipdvexpOPEN+7:
      case ipdipdvexpOPEN+8:
      case ipdipdvexpOPEN+9:
         {
	    ipdInt no;

	    no = ipdvexpOP(scan) - ipdipdvexpOPEN;
	    if (exec->regstartp[no] == exec->regendp[no]) {
	       exec->regstartp[no] = exec->reginput;
	       exec->regendp[no] = exec->reginput;
	    }
	    return(ipdregmatch(exec, next));
	 }
      case ipdvexpCLOSE+1:
      case ipdvexpCLOSE+2:
      case ipdvexpCLOSE+3:
      case ipdvexpCLOSE+4:
      case ipdvexpCLOSE+5:
      case ipdvexpCLOSE+6:
      case ipdvexpCLOSE+7:
      case ipdvexpCLOSE+8:
      case ipdvexpCLOSE+9:
         {
	    ipdInt no;

	    no = ipdvexpOP(scan) - ipdvexpCLOSE;
	    if (exec->regstartp[no] == exec->regendp[no]) {
	       exec->regendp[no] = exec->reginput;
	       exec->reglen[no] = exec->reginput - exec->regstartp[no];
	    }
	    return(ipdregmatch(exec, next));
	 }
      case ipdvexpMATCH+1:
      case ipdvexpMATCH+2:
      case ipdvexpMATCH+3:
      case ipdvexpMATCH+4:
      case ipdvexpMATCH+5:
      case ipdvexpMATCH+6:
      case ipdvexpMATCH+7:
      case ipdvexpMATCH+8:
      case ipdvexpMATCH+9:
	 {
	    ipdInt no;

	    no = ipdvexpOP(scan) - ipdvexpMATCH;
	    if (exec->regstartp[no] == exec->regendp[no])
	       return(ipdFALSE);

	    if (ipdvStrncmp(exec->reginput, exec->regstartp[no],
			 exec->reglen[no]) == 0) {
	       exec->reginput += exec->reglen[no]; /* advance pointer */
	       return (ipdregmatch(exec, next));
	    } else
	       return(ipdFALSE);
	 }
      case ipdvexpBRANCH:
	 if (ipdvexpOP(next) != ipdvexpBRANCH) /* No choice. */
	    next = ipdipdvexpOPERAND(scan); /* Avoid recursion. */
	 else {
	    do {
	       ipdConstString save;

	       save = exec->reginput;
	       if (ipdregmatch(exec, ipdipdvexpOPERAND(scan)))
		  return(ipdTRUE);
	       exec->reginput = save;
	       scan = ipdregnext(scan);
	    } while (scan != ipdNULL && ipdvexpOP(scan) == ipdvexpBRANCH);
	    return(ipdFALSE);
	 }
	 break;
      case ipdvexpMULT:
	 {
	    ipdByte *opnd, *count;
	    ipdByte min, max;
	    ipdConstString save;

	    save = exec->reginput;
	    opnd = ipdipdvexpOPERAND(scan);
	    count = opnd++;	/* read over counter */
	    min = *opnd++;
	    max = *opnd++;	/* get counts */
	    if (*count <= min) {
	       ipdBoolean start;
	       ipdBoolean retval;

	       start = (*count == 0);
	       (*count)++;	/* count this try */
	       if (!ipdregmatch(exec, opnd)) {
		  (*count)--;	/* did not work */
		  exec->reginput = save; /* did not match */
		  if (*count >= min) {
		     if (start)
			*count = 0;
		     break;
		  }
	       }
	       retval = (*count >= min);
	       if (start)
		  *count = 0;
	       return (retval);
	    }
	    if ((*count == max) && (max != 0))
	       break;
	    else {
	       (*count)++;	/* one more try */
	       if (!ipdregmatch(exec, opnd)) {
		  (*count)--;	/* did not work */
		  exec->reginput = save;
		  break;
	       } else
		  return(ipdTRUE);
	    }
	 }
      case ipdvexpSTAR:
      case ipdvexpPLUS:
         {
	    ipdByte nextch;
	    ipdInt no;
	    ipdConstString save;
	    ipdInt min;

	    /*
	     * Lookahead to avoid useless match attempts
	     * when we know what character comes next.
	     */
	    nextch = '\0';
	    if (ipdvexpOP(next) == ipdvexpEXACTLY)
	       nextch = *ipdipdvexpOPERAND(next);
	    min = (ipdvexpOP(scan) == ipdvexpSTAR) ? 0 : 1;
	    save = exec->reginput;
	    no = ipdregrepeat(exec, ipdipdvexpOPERAND(scan));
	    while (no >= min) {
	       /* If it could work, try it. */
	       if ((nextch == (ipdByte)'\0') ||
		   (((ipdByte)*(exec->reginput)) == nextch))
		  if (ipdregmatch(exec, next))
		     return(ipdTRUE);
	       /* Couldn't or didn't -- back up. */
	       no--;
	       exec->reginput = save + no;
	    }
	    return(ipdFALSE);
	 }
      case ipdvexpEND:
	 return(ipdTRUE);	/* Success! */
      default:
	 /* regerror("memory corruption");*/
	 return(ipdFALSE);
      }

      scan = next;
   }

   /*
    * We get here only if there's trouble -- normally "case END" is
    * the terminating point.
    */
   /* regerror("corrupted pointers");*/
   return(ipdFALSE);
}

/** @brief Matches something simple and reports how many.

    @param exec structure for match analysis
    @param p the compiled program part
    @return number of matches
 */
static ipdLong ipdregrepeat(
	ipdvexpExec *exec,
	ipdByte *p)
{
   ipdLong count = 0;
   ipdConstString scan;
   ipdByte *opnd;

   scan = exec->reginput;
   opnd = ipdipdvexpOPERAND(p);
   switch (ipdvexpOP(p)) {
   case ipdvexpANY:
      count = ipdvStrlen((ipdString)scan);
      scan += count;
      break;
   case ipdvexpEXACTLY:
      while (((ipdChar)*opnd) == *scan)
	 count++, scan++;
      break;
   case ipdipdvexpANYOF:
      while (*scan != '\0' && ipdvStrchr((ipdString)opnd, *scan) != ipdNULL)
	 count++, scan++;
      break;
   case ipdipdvexpANYBUT:
      while (*scan != '\0' && ipdvStrchr((ipdString)opnd, *scan) == ipdNULL)
	 count++, scan++;
      break;
   case ipdvexpRAN:
      while (*scan != '\0' &&
	     ((*scan >= *(ipdChar *)opnd) && (*scan <= *(ipdChar *)(opnd+1))))
	 count++, scan++;
      break;
   case ipdipdvexpRANBUT:
      while (*scan != '\0' &&
	     ((*scan < *(ipdChar *)opnd) && (*scan > *(ipdChar *)(opnd+1))))
	 count++, scan++;
      break;
   case ipdvexpLETTER:
      while (isalpha(*scan))
	 count++, scan++;
      break;
   case ipdvexpUPPER:
      while (isupper(*scan))
	 count++, scan++;
      break;
   case ipdvexpLOWER:
      while (islower(*scan))
	 count++, scan++;
      break;
   case ipdvexpDIGIT:
      while (isdigit(*scan))
	 count++, scan++;
      break;
   case ipdvexpXDIGIT:
      while (isxdigit(*scan))
	 count++, scan++;
      break;
   case ipdvexpALNUM:
      while (isalnum(*scan))
	 count++, scan++;
      break;
   case ipdvexpSPACE:
      while (isspace(*scan))
	 count++, scan++;
      break;
   case ipdvexpPRINT:
      while (isprint(*scan))
	 count++, scan++;
      break;
   case ipdvexpPUNCT:
      while (ispunct(*scan))
	 count++, scan++;
      break;
   case ipdvexpCNTRL:
      while (iscntrl(*scan))
	 count++, scan++;
      break;
   case ipdvexpNLETTER:
      while (!isalpha(*scan) && (*scan != '\0'))
	 count++, scan++;
      break;
   case ipdvexpNUPPER:
      while (!isupper(*scan) && (*scan != '\0'))
	 count++, scan++;
      break;
   case ipdvexpNLOWER:
      while (!islower(*scan) && (*scan != '\0'))
	 count++, scan++;
      break;
   case ipdvexpNDIGIT:
      while (!isdigit(*scan) && (*scan != '\0'))
	 count++, scan++;
      break;
   case ipdvexpNXDIGIT:
      while (!isxdigit(*scan) && (*scan != '\0'))
	 count++, scan++;
      break;
   case ipdvexpNALNUM:
      while (!isalnum(*scan) && (*scan != '\0'))
	 count++, scan++;
      break;
   case ipdvexpNSPACE:
      while (!isspace(*scan) && (*scan != '\0'))
	 count++, scan++;
      break;
   case ipdvexpNPRINT:
      while (!isprint(*scan) && (*scan != '\0'))
	 count++, scan++;
      break;
   case ipdvexpNPUNCT:
      while (!ispunct(*scan) && (*scan != '\0'))
	 count++, scan++;
      break;
   case ipdvexpNCNTRL:
      while (!iscntrl(*scan) && (*scan != '\0'))
	 count++, scan++;
      break;
   default:			/* Oh dear.  Called inappropriately. */
      /* regerror("internal foulup");*/
      count = 0;		/* Best compromise. */
      break;
   }
   exec->reginput = scan;

   return(count);
}

/** @brief Returns the next pointer of a node in the compiled regular expression. 
    @param p the compiled program part
*/
ipdByte *ipdregnext(ipdByte *p)
{
   ipdWord offset;

   if (p == ipdNULL)
      return(ipdNULL);
   else {
      offset = ipdvexpNEXT(p);
      if (offset == 0)
	 return(ipdNULL);

      if (ipdvexpOP(p) == ipdvexpBACK)
	 return(p-offset);
      else
	 return(p+offset);
   }
}

#endif /* !ipdHasPosixRegexp */







