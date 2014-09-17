/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

CONTENT:  Datatypes

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

/** @file ipdtype.h
    @brief Provides basic types used throughout ViennaIPD

*/

/*********** Note: Doxygen comments are found in the .c files! ***************/

/** @addtogroup EndUser */
/*@{*/

#ifndef IPDTYPE_H
#define IPDTYPE_H

#ifndef _IPDmachineINCLUDED_
#include "../core/include/machine.h"
#endif

/* generic types (size may be 16, 32 or even 64 Bits on some machines) */
typedef int				ipdInt;

/* Boolean values (no enumeration to allow all operators) */
typedef int	ipdBoolean;
#define ipdFALSE	((ipdBoolean)0)
#define ipdTRUE	((ipdBoolean)1)


/* the void type */
#if ipdHasANSI
typedef void		ipdVoid;
#else /* no full ANSI compiler */
#if defined(vms) || defined(__vms)
#define ipdVoid 		void	/* has no typedef support for void type */
#else
typedef int		ipdVoid;
#endif /* defined(vms) || defined(__vms) */
#endif /* ipdHasANSI */


/* define the ipdNULL pointer */
#if ipdHasANSI && !ipdHasVoidPtrBug
#define ipdNULL		((ipdVoid*)0L)
#if !ipdHasVoidFunctionPtrBug
#define ipdNULLFUNC	ipdNULL
#else
#define ipdNULLFUNC	0L
#endif /* !ipdHasVoidFunctionPtrBug */
#else
#define ipdNULL		0L
#define ipdNULLFUNC	0L
#endif /* ipdHasANSI && !ipdHasVoidPtrBug */


/* define a function as non returning (e.g. longjmp or exit) */
#if defined(__GNUC__)
#  if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MIIPDNOR__ < 5)
#    define ipdNonReturn           __volatile__
#    define ipdNonReturnAttr	/**/
#    define ipdReturnNotReached    /**/
#    define ipdReturnNotReachedValue(val)  /**/
#  else
#    define ipdNonReturn           /**/
#    define ipdNonReturnAttr	__attribute__ ((noreturn))
#    define ipdReturnNotReached    /**/
#    define ipdReturnNotReachedValue(val)  /**/
#  endif /* __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MIIPDNOR__ < 5) */
#else
#  if defined(WINNT) && defined(DECLSPEC_IPDNORETURN)
#    define ipdNonReturn           DECLSPEC_IPDNORETURN
#    define ipdNonReturnAttr	/**/
#    define ipdReturnNotReached    /**/
#    define ipdReturnNotReachedValue(val)  /**/
#  else
#    define ipdNonReturn           /**/
#    define ipdNonReturnAttr	/**/
#    define ipdReturnNotReached    return
#    define ipdReturnNotReachedValue(val)  return(val)
#  endif /* defined(WINNT) */
#endif /* defined(__GUNC__) */


/* for pointer arithmetic (same size as an pointer) */
typedef long			      ipdPointerOffset;

/* the character types */
typedef char			      ipdChar;	/* depends on system */
typedef unsigned char		ipdByte;

/* the short integer types (16 bit) */
typedef short			ipdShort;
typedef unsigned short		ipdWord;
#define ipdvSHORT_FORMAT		"%hd"
/* the long integer types (32 bit) and long long types */
#if !ipdHas16BitInts
/* hopefully integers are 32 bit long */
typedef int			      ipdLong;
typedef unsigned int		ipdLongWord;
#  define ipdvLONGWORD_MAX		((ipdLongWord)UINT_MAX)
#else
typedef long			ipdLong;
typedef unsigned long		ipdLongWord;
#  define ipdvLONGWORD_MAX		((ipdLongWord)ULONG_MAX)
#endif /* !ipdHas16BitInts */

/* floating point formats */
typedef float			ipdFloat;
typedef double			ipdDouble;
#define ipdDOUBLE_MAX		((ipdDouble)DBL_MAX) /***TC [:type ipdDouble] maximum double value */

#define ipdLONG_SIZE		sizeof(ipdLong)

/* strings */
typedef ipdChar		   *ipdString;
typedef const ipdChar	*ipdConstString; /* for constant string parameter */

/* other types */
typedef FILE		      *ipdFile;

#ifndef IPDVBSEXTERN
   #define IPDVBSEXTERN
#endif

#endif /*IPDTYPE_H_INCLUDED */

/*@}*/

