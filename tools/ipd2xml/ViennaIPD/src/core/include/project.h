/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

/** @file project.h
    @brief Gathers and collects information about the build-project for the current project.
*/

/* definitions: */

#define IPDNO	0
#define IPDYES	1

/* #define ipdVeBackTrace IPDYES */
#define ipdHasPosixRegexp IPDNO

#if !defined(ipdMalloc0ReturnsNull)
#define ipdMalloc0ReturnsNull IPDNO
#endif /* !defined(ipdMalloc0ReturnsNull) */
#if !defined(ipdHasPosixRegexp)
#define ipdHasPosixRegexp IPDYES
#endif /* !defined(ipdHasPosixRegexp) */
#if !defined(ipdNeedGetcwd)
#define ipdNeedGetcwd IPDNO
#endif /* !defined(ipdNeedGetcwd) */
#if !defined(ipdHasDiffTime)
#define ipdHasDiffTime IPDYES
#endif /* !defined(ipdHasDiffTime) */
#if !defined(ipdHasUtime)
#define ipdHasUtime IPDYES
#endif /* !defined(ipdHasUtime) */
#if !defined(ipdHasProcessResources)
#define ipdHasProcessResources IPDYES
#endif /* !defined(ipdHasProcessResources) */
#if !defined(ipdBSDcompat)
#define ipdBSDcompat IPDNO
#endif /* !defined(ipdBSDcompat) */
#if !defined(ipdSystemV)
#define ipdSystemV IPDNO
#endif /* !defined(ipdSystemV) */
#if !defined(ipdHasStdlib)
#define ipdHasStdlib IPDYES
#endif /* !defined(ipdHasStdlib) */
#if !defined(ipdHasStdarg)
#define ipdHasStdarg IPDYES
#endif /* !defined(ipdHasStdarg) */
#if !defined(ipdNeedAllocaInclude)
#define ipdNeedAllocaInclude IPDNO
#endif /* !defined(ipdNeedAllocaInclude) */
#if !defined(ipdHas64BitLong)
#define ipdHas64BitLong IPDNO
#endif /* !defined(ipdHas64BitLong) */
#if !defined(ipdHasLongLong)
#define ipdHasLongLong IPDNO
#endif /* !defined(ipdHasLongLong) */
#if !defined(ipdHas16BitInts)
#define ipdHas16BitInts IPDNO
#endif /* !defined(ipdHas16BitInts) */
#if !defined(ipdHasLongDouble)
#define ipdHasLongDouble IPDNO
#endif /* !defined(ipdHasLongDouble) */
#if !defined(ipdHasIPDNOconst)
#define ipdHasIPDNOconst IPDNO
#endif /* !defined(ipdHasIPDNOconst) */
#if !defined(ipdHasVoidFunctionPtrBug)
#define ipdHasVoidFunctionPtrBug IPDNO
#endif /* !defined(ipdHasVoidFunctionPtrBug) */
#if !defined(ipdHasVoidPtrBug)
#define ipdHasVoidPtrBug IPDNO
#endif /* !defined(ipdHasVoidPtrBug) */
#if !defined(ipdHasVoidArg)
#define ipdHasVoidArg IPDYES
#endif /* !defined(ipdHasVoidArg) */
#if !defined(ipdHasFunctionCastingBug)
#define ipdHasFunctionCastingBug IPDNO
#endif /* !defined(ipdHasFunctionCastingBug) */
#if !defined(ipdHasFunctionStructBug)
#define ipdHasFunctionStructBug IPDNO
#endif /* !defined(ipdHasFunctionStructBug) */
#if !defined(ipdHasFunctionParameterBug)
#define ipdHasFunctionParameterBug IPDNO
#endif /* !defined(ipdHasFunctionParameterBug) */
#if !defined(ipdHasANSI)
#define ipdHasANSI IPDYES
#endif /* !defined(ipdHasANSI) */
#if !defined(ipdMEMipdDEBUG)
#define ipdMEMipdDEBUG IPDNO
#endif /* !defined(ipdMEMipdDEBUG) */
#if !defined(ipdVeBackTrace)
#define ipdVeBackTrace IPDYES
#endif /* !defined(ipdVeBackTrace) */
#if !defined(IPDCHECK)
#define IPDCHECK IPDNO
#endif /* !defined(IPDCHECK) */
#if !defined(ipdINFORMATION)
#define ipdINFORMATION IPDNO
#endif /* !defined(ipdINFORMATION) */
#if !defined(ipdDEBUG)
#define ipdDEBUG IPDNO
#endif /* !defined(ipdDEBUG) */
#if !defined(ipdHasWaitUnion)
#define ipdHasWaitUnion IPDNO
#endif /* !defined(ipdHasWaitUnion) */
#if !defined(ipdHasSysErrlist)
#define ipdHasSysErrlist IPDYES
#endif /* !defined(ipdHasSysErrlist) */
#if !defined(ipdHasStrError)
#define ipdHasStrError IPDYES
#endif /* !defined(ipdHasStrError) */
#if !defined(ipdHasAlloca)
#define ipdHasAlloca IPDYES
#endif /* !defined(ipdHasAlloca) */



