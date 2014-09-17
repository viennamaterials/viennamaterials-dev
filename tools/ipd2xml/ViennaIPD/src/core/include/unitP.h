/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

/** @file unitP.h
    @brief Global definitions for the unit system core
*/

#ifndef _IPDunitpINCLUDED_
#define _IPDunitpINCLUDED_

/* Structure for internal Unit representation */
#define ipduntUNIT_EXPONENT	0 /* entry for exponent of factor */
#define ipduntUNIT_KILOGRAMM	3 /* entry for SI unit 'kg' */
#define ipduntNUM_BASIC_UNITS 6	/* total number of basic units */

typedef struct
{
   ipdDouble unitFactor;
   ipdLong   unitBasics[ipduntNUM_BASIC_UNITS + 1];
} ipduntUnitRep;


_BEGIN_CPLUSPLUS
IPDVBSEXTERN extern ipdBoolean ipduntParseUnitString( 
        ipdConstString  unitString, /* [I] unit string */
        ipduntUnitRep *unitValue);	/* [O] internal representation of unit */
IPDVBSEXTERN extern ipdBoolean ipduntUnitRatio(
        ipduntUnitRep *unitRep1,   /* [I] first internal unit */
        ipduntUnitRep *unitRep2,   /* [I] second internal unit */
        ipdDouble *ratio);	/* [O] unit1 / unit2 */
IPDVBSEXTERN extern ipdBoolean ipduntUnitCompareable(
        ipduntUnitRep *unitRep1,   /* [I] first internal unit */
        ipduntUnitRep *unitRep2);  /* [I] second internal unit */
IPDVBSEXTERN extern ipdConstString ipduntCreateUnitString(
        ipduntUnitRep *unitRep,	/* [I] unit to be transformed */
        ipdLongWord flags);	/* [I] output flags */
_END_CPLUSPLUS

#endif /* _unitpINCLUDED_ */
