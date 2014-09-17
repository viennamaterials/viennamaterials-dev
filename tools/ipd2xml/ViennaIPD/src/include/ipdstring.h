/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

CONTENT:  String functions

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

/** @file ipdstring.h
    @brief Provides declarations of several string functions not provided by the C standard library and a regular expression engine.

*/

/*********** Note: Doxygen comments are found in the .c files! ***************/


#ifndef IPDSTRING_H
#define IPDSTRING_H

#include <string.h>
#include <math.h>

#include "ipdtype.h"

#if ipdHasPosixRegexp
#  include <regex.h>            /* system implementation */
#endif /* ipdHasPosixRegexp */


_BEGIN_CPLUSPLUS

/***S vMemory */
/***T Memory Routines */

#if ipdMalloc0ReturnsNull
#  if defined(DebugMalloc0Calls) && DebugMalloc0Calls
#  define __ipdvMallocSize(size) \
   ((size == 0) ? 1, ipdVeWarning(ipdveTDB, "try to allocate 0 bytes") : size)
#  else
#  define __ipdvMallocSize(size) ((size == 0) ? 1 : size)
#  endif /* defined(DebugMalloc0Calls) && DebugMalloc0Calls */
#else
#define __ipdvMallocSize(size)   (size)
#endif /* ipdMalloc0ReturnsNull */

#if !(defined(ipdMEMipdDEBUG) && ipdMEMipdDEBUG)
/***M3 {\bf vMalloc} allocates {\bf size} bytes of storage dynamically. On
    sucess it returns a {\bf ipdByte} Pointer to the allocated space, else
    ipdNULL. \\
    {\bf ipdvFree} frees a dynamically allocated space of memory pointed to by
    the pointer {\bf ptr}. The contents of the memory array are immediately
    lost and can't be accessed any more. \\
    {\bf ipdvRealloc} changes the size of the dynamically allocated memory
    space pointed by {\bf ptr} to the new size {\bf size}. It returns a {\bf
    ipdByte} pointer to the storage after reallocation. This pointer may
    differ from the original pointer {\bf ptr}. If there is not enough
    memory available for the new (bigger) space, ipdNULL is returned
    (Error). */
/***T Memory Allocation Routines */
/***N memory_allocation */
#if !defined(WINNT) || 1
#  define ipdvMalloc(size)	((ipdByte *) malloc(__ipdvMallocSize(size)))
#  if ipdHasANSI
#    define ipdvFree(ptr)	((ipdVoid) free((ipdVoid *)ptr))
#    define ipdvRealloc(ptr,size) ((ipdVoid *) realloc((ipdVoid *)ptr, __ipdvMallocSize(size)))
#  else
#    if defined(vms) || defined(__vms)
#      define ipdvFree       free	/* very stupid, but the only way for VAX C */
#    else
#      define ipdvFree(ptr)  free(ptr)
#    endif /* defined(vms) || defined(__vms) */
#    define ipdvRealloc(ptr,size) ((ipdByte *) realloc(ptr, __ipdvMallocSize(size)))
#endif /* ipdHasANSI */
/***M allocate a cleared area of memory */
#  define ipdvCalloc(num, size) ((ipdByte *) calloc(__ipdvMallocSize(num), __ipdvMallocSize(size)))
#else
   /* use Windows NT process heap for memory allocation */
#  define ipdvMalloc(size)	((ipdByte *) HeapAlloc(GetProcessHeap(), 0, __ipdvMallocSize(size)))
#  define ipdvFree(ptr)	((ipdVoid) HeapFree(GetProcessHeap(), 0, (ipdVoid *)ptr))
#  define ipdvRealloc(ptr,size)    \
     ((ipdVoid *) HeapReAlloc(GetProcessHeap(), 0, (ipdVoid *)ptr, __ipdvMallocSize(size)))
#  define ipdvCalloc(num, size)      \
     ((ipdByte *) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, __ipdvMallocSize(size)))
#endif /* !defined(WINNT) */
#endif /* defined(ipdMEMipdDEBUG) && ipdMEMipdDEBUG */

/***M2 {\bf ipdvAlloca} allocates {\bf size} bytes on the runtimestack. The
    storage is automatically released on exit of function. Since some system
    doesn't support this function, there is a {\bf ipdvAllocaFree} function
    for emulation trough vMalloc/ipdvFree. */
/***T Stackmemory Allocation Routines */
/***N stackmemory_allocation */
#if ipdHasAlloca
#define ipdvAlloca(size)       ((ipdByte *) alloca(size))
#define ipdvAllocaFree(ptr)    /**/

#if defined(WINNT) && defined(__STDC__)
/*MK #  define alloca(size)      _alloca(size) */
#endif /* defined(WINNT) && defined(__STDC__) */

#else /* emulation through vMalloc/ipdvFree */
#define ipdvAlloca(size)       ipdvMalloc(size)
#define ipdvAllocaFree(ptr)    ipdvFree(ptr)
#endif /* ipdHasAlloca */

#if !defined(WINNT)
#  define ipdvMemset(addr,val,count)  ((ipdVoid) memset((ipdVoid*)(addr), val, count))
#  define ipdvMemcpy(ptr1,ptr2,count) ((ipdVoid) memcpy((ipdVoid*)(ptr1), (ipdVoid*)(ptr2), count))
#  define ipdvMemcmp(ptr1,ptr2,count) ((ipdInt) memcmp((ipdVoid*)(ptr1), (ipdVoid*)(ptr2), count))
#else
#  define ipdvMemset(addr,val,count)  FillMemory((ipdVoid *)(addr), val, count)
#  define ipdvMemcpy(ptr1,ptr2,count) CopyMemory((ipdVoid *)(ptr1), (ipdVoid *)(ptr2), count)
#  define ipdvMemcmp(ptr1,ptr2,count) ((ipdInt) memcmp((ipdVoid*)(ptr1), (ipdVoid*)(ptr2), count))
#endif /* !defined(WINNT) */

/***M7 
    {\bf ipdvStrcpy} copies string {\bf str2} into the memory area pointed by
    {\bf str1} and returns this pointer. The destination area has to be big
    enough to hold the copied string. \\
    {\bf ipdvStrcat} concats string {\bf str2} at the end of string {\bf
    str1} and returns this pointer. The memory area pointer by {\bf str1}
    has to be large enough to hold the concated string. */
#define ipdvStrcpy(str1,str2) ((ipdString) strcpy(str1, str2))
#define ipdvStrncpy(str1,str2,n) ((ipdString) strncpy(str1, str2, n))
#define ipdvStrcat(str1,str2) ((ipdString) strcat(str1, str2))
#define ipdvStrcmp strcmp
#define ipdvStrncmp(str1,str2,n) ((ipdInt) strncmp(str1, str2, n))
/***M2 {\bf ipdvStrchr} returns a pointer to the first occurrence of character
    {\bf c} in string {\bf str}. If the character is not found
    ipdNULL is returned. \\
    {\bf ipdvStrrchr} returns a pointer to the last occurrence of character
    {\bf c} in string {\bf str}. If the character is not found
    ipdNULL is returned. */
/***T Character Search Routines */
/***N search_functions */
#define ipdvStrchr(str,c)  ((ipdString) strchr(str, c))
#define ipdvStrrchr(str,c) ((ipdString) strrchr(str, c))

#define ipdvStrlen(str)	((ipdLongWord) strlen(str))

#define ipdvStrdup(str) ((ipdString) strdup(str))

#define ipdvStrstr(str1, str2) ((ipdString) strstr(str1, str2))

/* generate a string duplicate */
/*IPDVBSEXTERN extern ipdString ipdvStrdup(ipdConstString); */
/* convert to one case */
IPDVBSEXTERN extern ipdString ipdvToupper(ipdString);
IPDVBSEXTERN extern ipdString ipdvTolower(ipdString);
/* search for string within string (both directions) */
/*IPDVBSEXTERN extern ipdString ipdvStrstr(ipdString, ipdConstString);*/
IPDVBSEXTERN extern ipdString ipdvStrrstr(ipdString, ipdConstString);


_END_CPLUSPLUS

/*******************************************************

				REGEX PART	
	
********************************************************/

  
#if !ipdHasPosixRegexp

/* configuration parameter */
#define ipdvexpNSUBEXP     20      /* number of subexpressions */

typedef struct _ipdregex_t_ {
   ipdBoolean     icase;          /* ignore case for regular expression */
   ipdBoolean     reganch;
   ipdChar        regstart;
   ipdConstString regmust;        /* must occur */
   ipdByte        program[1];     /* program to execute */
} ipdregex_t;

/*
 * The "internal use only" fields in regexp.h are present to pass info from
 * compile to execute that permits the execute phase to run lots faster on
 * simple cases.  They are:
 *
 * regstart	char that must begin a match; '\0' if none obvious
 * reganch	is the match anchored (at beginning-of-line only)?
 * regmust	string (pointer into program) that match must include, or ipdNULL
 * regmlen	length of regmust string
 *
 * Regstart and reganch permit very fast decisions on suitable starting points
 * for a match, cutting down the work a lot.  Regmust permits fast rejection
 * of lines that cannot possibly match.  The regmust tests are costly enough
 * that regcomp() supplies a regmust only if the r.e. contains something
 * potentially expensive (at present, the only such thing detected is * or +
 * at the start of the r.e., which can involve a lot of backup).  Regmlen is
 * supplied because the test in regexec() needs it and regcomp() is computing
 * it anyway.
 */

/*
 * Structure for regexp "program".  This is essentially a linear encoding
 * of a nondeterministic finite-state machine (aka syntax charts or
 * "railroad normal form" in parsing technology).  Each node is an opcode
 * plus a "next" pointer, possibly plus an operand.  "Next" pointers of
 * all nodes except BRANCH implement concatenation; a "next" pointer with
 * a BRANCH on both ends of it is connecting two alternatives.  (Here we
 * have one of the subtle syntax dependencies:  an individual BRANCH (as
 * opposed to a collection of them) is never concatenated with anything
 * because of operator precedence.)  The operand of some types of node is
 * a literal string; for others, it is a node leading into a sub-FSM.  In
 * particular, the operand of a BRANCH node is the first node of the branch.
 * (NB this is *not* a tree structure:  the tail of the branch connects
 * to the thing following the set of BRANCHes.)  The opcodes are:
 */

/* definition	number	opnd?	meaning */
#define	ipdvexpEND	     (ipdByte)0	/* no	End of program. */
#define	ipdvexpBOL	     (ipdByte)1	/* no	Match "" at beginning of line. */
#define	ipdvexpEOL	     (ipdByte)2	/* no	Match "" at end of line. */
#define	ipdvexpANY	     (ipdByte)3	/* no	Match any one character. */
#define ipdipdvexpANYOF    (ipdByte)4	/* str	Match any character in this string. */
#define	ipdipdvexpANYBUT   (ipdByte)5	/* str	Match any character not in this string. */
#define	ipdvexpBRANCH   (ipdByte)6	/* node	Match this alternative, or the next... */
#define	ipdvexpBACK     (ipdByte)7	/* no	Match "", "next" ptr points backward. */
#define	ipdvexpEXACTLY  (ipdByte)8	/* str	Match this string. */
#define	ipdvexpIPDNOTHING  (ipdByte)9	/* no	Match empty string. */
#define	ipdvexpSTAR    (ipdByte)10	/* node	Match this (simple) thing 0 or more times. */
#define	ipdvexpPLUS    (ipdByte)11	/* node	Match this (simple) thing 1 or more times. */
#define ipdvexpMULT    (ipdByte)12	/* min max Match expression from min to max times. */
#define	ipdvexpWORDA   (ipdByte)13	/* no	Match "" at wordchar, where prev is nonword */
#define	ipdvexpWORDZ   (ipdByte)14	/* no	Match "" at nonwordchar */
#define ipdvexpRAN     (ipdByte)15	/* str	Match any character in this between next two chars. */
#define	ipdipdvexpRANBUT  (ipdByte)16	/* str	Match any character not beetween next two chars. */
#define ipdvexpLETTER  (ipdByte)20	/* no	any letter character */
#define ipdvexpUPPER   (ipdByte)21	/* no	any uppercasse character */
#define ipdvexpLOWER   (ipdByte)22	/* no	any uppercasse character */
#define ipdvexpDIGIT   (ipdByte)23   /* no	any digit character */
#define ipdvexpXDIGIT  (ipdByte)24	/* no	any hexadecimal digit character */
#define ipdvexpALNUM   (ipdByte)25	/* no	any alphanumeric character */
#define ipdvexpSPACE   (ipdByte)26	/* no	any space character */
#define ipdvexpPRINT   (ipdByte)27	/* no	any printable character */
#define ipdvexpPUNCT   (ipdByte)28	/* no	any punctation character */
#define ipdvexpCNTRL   (ipdByte)29	/* no	any control character */
#define ipdvexpINV     (ipdByte)10	/* inverse all 20-29 */
#define ipdvexpNLETTER (ipdByte)30	/* no	any but no letter character */
#define ipdvexpNUPPER  (ipdByte)31	/* no	any but no uppercasse character */
#define ipdvexpNLOWER  (ipdByte)32	/* no	any but no uppercasse character */
#define ipdvexpNDIGIT  (ipdByte)33   /* no	any but no digit character */
#define ipdvexpNXDIGIT (ipdByte)34	/* no	any but no hexadecimal digit character */
#define ipdvexpNALNUM  (ipdByte)35	/* no	any but no alphanumeric character */
#define ipdvexpNSPACE  (ipdByte)36	/* no	any but no space character */
#define ipdvexpNPRINT  (ipdByte)37	/* no	any but no printable character */
#define ipdvexpNPUNCT  (ipdByte)38	/* no	any but no punctation character */
#define ipdvexpNCNTRL  (ipdByte)39	/* no	any but no control character */
#define ipdvexpFLOAT   (ipdByte)40   /* no   float constant */
#define ipdvexpWORD    (ipdByte)41	/* no   word constant (letters) */
#define ipdvexpSYMBOL  (ipdByte)42   /* no   symbol constant */
#define ipdvexpPIFNAME (ipdByte)43	/* no	PIF symbol name */
#define	ipdipdvexpOPEN    (ipdByte)50	/* no	Mark this point in input as start of #n. */
        			/*	OPEN+1 is number 1, etc. */
#define	ipdvexpCLOSE   (ipdByte)(ipdipdvexpOPEN+ipdvexpNSUBEXP)
        			/* no	Analogous to OPEN. */
#define ipdvexpMATCH   (ipdByte)(ipdvexpCLOSE+ipdvexpNSUBEXP)
                		/* no   match same string as bewen OPEN and CLOSE */


#define ipdvexpMAGIC	0231	/* magic start character */

/*
 * Opcode notes:
 *
 * BRANCH	The set of branches constituting a single choice are hooked
 *		together with their "next" pointers, since precedence prevents
 *		anything being concatenated to any individual branch.  The
 *		"next" pointer of the last BRANCH in a choice points to the
 *		thing following the whole choice.  This is also where the
 *		final "next" pointer of each individual branch points; each
 *		branch starts with the operand node of a BRANCH node.
 *
 * BACK		Normal "next" pointers all implicitly point forward; BACK
 *		exists to make loop structures possible.
 *
 * STAR,PLUS	'?', and complex '*' and '+', are implemented as circular
 *		BRANCH structures using BACK.  Simple cases (one character
 *		per match) are implemented with STAR and PLUS for speed
 *		and to minimize recursive plunges.
 *
 * OPEN,CLOSE	...are numbered at compile time.
 */

/*
 * A node is one char of opcode followed by two chars of "next" pointer.
 * "Next" pointers are stored as two 8-bit pieces, high order first.  The
 * value is a positive offset from the opcode of the node containing it.
 * An operand, if any, simply follows the node.  (Note that much of the
 * code generation knows about this implicit relationship.)
 *
 * Using two bytes for the "next" pointer is vast overkill for most things,
 * but allows patterns to get big without disasters.
 */
#define	ipdvexpOP(p)	(*(p))
#define	ipdvexpNEXT(p)	(((*((p)+1)) << 8) + (*((p)+2)))
#define	ipdipdvexpOPERAND(p)	((p) + 3)


extern ipdByte *ipdregnext(ipdByte *p);

#endif /* !ipdHasPosixRegexp */






typedef ipdregex_t        ipdrgxRegexpType;
typedef ipdrgxRegexpType *ipdrgxRegexpPtr;

_BEGIN_CPLUSPLUS

IPDVBSEXTERN extern ipdBoolean     ipdrgxExecute(ipdrgxRegexpPtr, ipdConstString, ipdLong *, ipdLong *);

_END_CPLUSPLUS







#endif /* _vstdcINCLUDED_ */


