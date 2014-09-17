/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/


/** @file vemacros.h
    @brief Various error macros that provide the external interface to ipdveSendError()
*/

#ifndef _IPDvemacrosINCLUDED_
#define _IPDvemacrosINCLUDED_
#if ipdVeBackTrace
#  define _IPDBACKTRACE_	&_ipdbackTrace_
#else
#  define _IPDBACKTRACE_	__FILE__
#endif /* ipdVeBackTrace */

/***M sends a message of {\bf Level} {\tt ipdDEBUG},
{\tt INFO}, {\tt WARNING}, {\tt ERROR}, or {\tt FATAL} with an
additional numeric identifier {\bf Id} (which can be used by error
handlers) to the Vienna Error System. The {\bf errclass} of
the error event which can be specified must be one of {\tt ipdveSYS},
{\tt ipdveALG}, {\tt ipdveEXT}, {\tt ipdveARG}, {\tt ipdveDAT}, {\tt ipdveCNF},
{\tt ipdveLIM}, {\tt ipdveNUM}, {\tt ipdveUNI}, {\tt ipdveFCF},
{\tt ipdveFOR}, or {\tt ipdveUSR}.  For a complete list see \vref{ErrorClasses}
The error message string may contain formatting sequences and
additional {\bf N} arguments ({\bf arg1...argN}) must be given (like in
{\tt printf()}). VeLevelId must be used when no additional arguments are
specified. */

#define ipdVeInfo3(errclass,msg,arg1,arg2,arg3)\
ipdveSendError(IPDVMODULE,_IPDBACKTRACE_,\
_rcsid_,__LINE__,ipdveLEVEL_INFO,errclass,-1,msg,arg1,arg2,arg3)
#define ipdVeInfoId3(errclass,id,msg,arg1,arg2,arg3)\
ipdveSendError(IPDVMODULE,_IPDBACKTRACE_,\
_rcsid_,__LINE__,ipdveLEVEL_INFO,errclass,id,msg,arg1,arg2,arg3)

#define ipdVeInfo4(errclass,msg,arg1,arg2,arg3,arg4)\
ipdveSendError(IPDVMODULE,_IPDBACKTRACE_,\
_rcsid_,__LINE__,ipdveLEVEL_INFO,errclass,-1,msg,arg1,arg2,arg3,arg4)
#define ipdVeInfoId4(errclass,id,msg,arg1,arg2,arg3,arg4)\
ipdveSendError(IPDVMODULE,_IPDBACKTRACE_,\
_rcsid_,__LINE__,ipdveLEVEL_INFO,errclass,id,msg,arg1,arg2,arg3,arg4)


#define ipdVeWarning(errclass,msg)\
ipdveSendError(IPDVMODULE,_IPDBACKTRACE_,\
_rcsid_,__LINE__,ipdveLEVEL_WARNING,errclass,-1,msg)
#define ipdVeWarningId(errclass,id,msg)\
ipdveSendError(IPDVMODULE,_IPDBACKTRACE_,\
_rcsid_,__LINE__,ipdveLEVEL_WARNING,errclass,id,msg)

#define ipdVeWarning2(errclass,msg,arg1,arg2)\
ipdveSendError(IPDVMODULE,_IPDBACKTRACE_,\
_rcsid_,__LINE__,ipdveLEVEL_WARNING,errclass,-1,msg,arg1,arg2)
#define ipdVeWarningId2(errclass,id,msg,arg1,arg2)\
ipdveSendError(IPDVMODULE,_IPDBACKTRACE_,\
_rcsid_,__LINE__,ipdveLEVEL_WARNING,errclass,id,msg,arg1,arg2)


#define ipdVeError(errclass,msg)\
ipdveSendError(IPDVMODULE,_IPDBACKTRACE_,\
_rcsid_,__LINE__,ipdveLEVEL_ERROR,errclass,-1,msg)
#define ipdVeErrorId(errclass,id,msg)\
ipdveSendError(IPDVMODULE,_IPDBACKTRACE_,\
_rcsid_,__LINE__,ipdveLEVEL_ERROR,errclass,id,msg)

#define ipdVeError1(errclass,msg,arg1)\
ipdveSendError(IPDVMODULE,_IPDBACKTRACE_,\
_rcsid_,__LINE__,ipdveLEVEL_ERROR,errclass,-1,msg,arg1)
#define ipdVeErrorId1(errclass,id,msg,arg1)\
ipdveSendError(IPDVMODULE,_IPDBACKTRACE_,\
_rcsid_,__LINE__,ipdveLEVEL_ERROR,errclass,id,msg,arg1)

#define ipdVeError2(errclass,msg,arg1,arg2)\
ipdveSendError(IPDVMODULE,_IPDBACKTRACE_,\
_rcsid_,__LINE__,ipdveLEVEL_ERROR,errclass,-1,msg,arg1,arg2)
#define ipdVeErrorId2(errclass,id,msg,arg1,arg2)\
ipdveSendError(IPDVMODULE,_IPDBACKTRACE_,\
_rcsid_,__LINE__,ipdveLEVEL_ERROR,errclass,id,msg,arg1,arg2)


/* end of automatically generated file */
#endif /* _vemacrosINCLUDED_ */

