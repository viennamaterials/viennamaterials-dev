/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

static char const _rcsid_ [] = "$Id: unitconv.c$";

/** @file unitconv.c
    @brief Implementation of the unit system

    All units are converted to the SI system for internal calculations.
*/

/***I */
#include "../../include/ipdtype.h"
#include "verr.h"
#include "unit.h"
#include "unitP.h"

#include <math.h>


/* ************************************************************************* *
 *                                                                           *
 *                     The Unit Table Definitions                            *
 *                                                                           *
 *************************************************************************** */

/* ---------------------------- The Unit List ----------------------------- */

/* ATTENTION:       Adding new units
   ==========
   The unit list should be sorted alphabetically to avoid
   delays at the first unit manipulation.
   To enter a new unit, it is only necessary to enter a new entry
   in the following list. Please note that there is sort of a parsing
   problem, if two units are completely equal but for one or two characters
   in the beginning which appear as a prefix in the prefix list afterwards.
   In this case, the parsing will result in an ambiguity, which is always
   resolved by taking the longer unit without a prefix.
   IPDNO WARNING WILL BE ISSUED IN THAT CASE, so please do not enter new
   units without carefully checking the other units. */
static struct  _unitDefinitions_
{
   ipdConstString unitName;
   ipduntUnitRep   unitRep;
} unitDefinitions[] = {    /* [patch] removed warning: added braces for the unitRep member */
   /* name      	factor                10    m  s kg  A  K cd       */
{  "1",         	      { 1.0,                {  0,   0, 0, 0, 0, 0, 0 } } },
{  "A",         	      { 1.0,                {  0,   0, 0, 0, 1, 0, 0 } } },
{  "Achtel",            { 0.1250,             { -3,   3, 0, 0, 0, 0, 0 } } },
{  "Angstrom",  	      { 1.0,                {-10,   1, 0, 0, 0, 0, 0 } } },
{  "B",                 { 1.0,                {  0,   0, 0, 0, 0, 0, 0 } } },
{  "BoltzmannConstant",	{ 1.380662,           {-23,   2,-2, 1, 0,-1, 0 } } },
{  "C",         	      { 1.0,                {  0,   0, 1, 0, 1, 0, 0 } } },
{  "ElectronRestMass", 	{ 0.91093897,         {-30,   0, 0, 1, 0, 0, 0 } } },
{  "F",         	      { 1.0,                {  0,  -2, 4,-1, 2, 0, 0 } } },
{  "FIT",      		   { 2.77777777777777778,{-13,   0,-1, 0, 0, 0, 0 } } },
{  "Gauss",     	      { 1.0,                { -4,   0,-2, 1,-1, 0, 0 } } },
{  "H",         	      { 1.0,                {  0,   2,-2, 1,-2, 0, 0 } } },
{  "Hz",        	      { 1.0,                {  0,   0,-1, 0, 0, 0, 0 } } },
{  "J",         	      { 1.0,                {  0,   2,-2, 1, 0, 0, 0 } } },
{  "K",         	      { 1.0,                {  0,   0, 0, 0, 0, 1, 0 } } },
{  "Klitzing",  	      { 2.5812807,          {  4,   2,-3, 1,-2, 0, 0 } } },
{  "Kruegel",           { 0.50,               { -3,   3, 0, 0, 0, 0, 0 } } },
{  "Mho",       	      { 1.0,                {  0,  -2, 3,-1, 2, 0, 0 } } },
{  "N",         	      { 1.0,                {  0,   1,-2, 1, 0, 0, 0 } } },
{  "Ohm",       	      { 1.0,                {  0,   2,-3, 1,-2, 0, 0 } } },
{  "Pa",        	      { 1.0,                {  0,  -1,-2, 1, 0, 0, 0 } } },
{  "PlancksConstant",   { 0.662617,           {-33,   2,-1, 1, 0, 0, 0 } } },
{  "S",   	            { 1.0,                {  0,  -2, 3,-1, 2, 0, 0 } } },
{  "Schuessel",  	      { 0.40,               { -3,   3, 0, 0, 0, 0, 0 } } },
{  "Seidel",		      { 0.30,               { -3,   3, 0, 0, 0, 0, 0 } } },
{  "Siemens",   	      { 1.0,                {  0,  -2, 3,-1, 2, 0, 0 } } },
{  "T",         	      { 1.0,                {  0,   0,-2, 1,-1, 0, 0 } } },
{  "V",         	      { 1.0,                {  0,   2,-3, 1,-1, 0, 0 } } },
{  "VacuumPermeability",{ 1.25663706143591730,{ -6,   1,-2, 1,-2, 0, 0 } } },
{  "VacuumPermittivity",{ 0.88541878176203899,{-11,  -3, 4,-1, 2, 0, 0 } } },
{  "Viertel",           { 0.250,              { -3,   3, 0, 0, 0, 0, 0 } } },
{  "W",         	      { 1.0,                {  0,   2,-3, 1, 0, 0, 0 } } },
{  "Wb",        	      { 1.0,                {  0,   2,-2, 1,-1, 0, 0 } } },
{  "amu",       	      { 1.66054018,         {-27,   0, 0, 1, 0, 0, 0 } } },
{  "cal",       	      { 0.41868,            {  1,   2,-2, 1, 0, 0, 0 } } },
{  "cd",        	      { 1.0,                {  0,   0, 0, 0, 0, 0, 1 } } },
{  "day",       	      { 0.864,              {  5,   0, 1, 0, 0, 0, 0 } } },
{  "dec",       	      { 1.0,                {  0,   0, 0, 0, 0, 0, 0 } } },
{  "decades",       	   { 1.0,                {  0,   0, 0, 0, 0, 0, 0 } } },
{  "deg",       	      { 1.74532925199432958,{ -2,   0, 0, 0, 0, 0, 0 } } },
{  "dyne",         	   { 1.0,                { -5,   1,-2, 1, 0, 0, 0 } } },
{  "e",         	      { 1.6021892,          {-19,   0, 1, 0, 1, 0, 0 } } },
{  "eV",		            { 1.6021892,          {-19,   2,-2, 1, 0, 0, 0 } } },
{  "erg",       	      { 1.0,                { -7,   2,-2, 1, 0, 0, 0 } } },
{  "foot",      	      { 3.048,              { -1,   1, 0, 0, 0, 0, 0 } } },
{  "g",         	      { 1.0,                { -3,   0, 0, 1, 0, 0, 0 } } },
{  "gallonUK",      	   { 4.5461,             { -3,   3, 0, 0, 0, 0, 0 } } },
{  "gallonUS",      	   { 3.78541,            { -3,   3, 0, 0, 0, 0, 0 } } },
{  "grad",		         { 1.57079632679489661,{ -2,   0, 0, 0, 0, 0, 0 } } },
{  "hour",      	      { 0.36,               {  4,   0, 1, 0, 0, 0, 0 } } },
{  "inch",      	      { 2.54,               { -2,   1, 0, 0, 0, 0, 0 } } },
{  "k",         	      { 1.380662,           {-23,   2,-2, 1, 0,-1, 0 } } },
{  "l",         	      { 1.0,                { -3,   3, 0, 0, 0, 0, 0 } } },
{  "lumen",     	      { 1.0,                {  0,   0, 0, 0, 0, 0, 1 } } },
{  "lux",       	      { 1.0,                {  0,  -2, 0, 0, 0, 0, 1 } } },
{  "m",         	      { 1.0,                {  0,   1, 0, 0, 0, 0, 0 } } },
{  "micron",    	      { 1.0,                { -6,   1, 0, 0, 0, 0, 0 } } },
{  "min",       	      { 0.6,                {  2,   0, 1, 0, 0, 0, 0 } } },
{  "mol",       	      { 0.60221367,         { 24,   0, 0, 0, 0, 0, 0 } } },
{  "pi",	       	      { 3.14159265358979323,{  0,   0, 0, 0, 0, 0, 0 } } },
{  "pintUK",      	   { 0.568271,           { -3,   3, 0, 0, 0, 0, 0 } } },
{  "pintUS",      	   { 0.473176,           { -3,   3, 0, 0, 0, 0, 0 } } },
{  "rad",       	      { 1.0,                {  0,   0, 0, 0, 0, 0, 0 } } },
{  "s",         	      { 1.0,                {  0,   0, 1, 0, 0, 0, 0 } } },
{  "var",       	      { 1.0,                {  0,   2,-3, 1, 0, 0, 0 } } },
{  "year",      	      { 3.155692597546,     {  7,   0, 1, 0, 0, 0, 0 } } }
};

#define NUM_UNIT_DEFINITIONS \
    (sizeof(unitDefinitions) / sizeof(unitDefinitions[0]))

/* static array for internal representation to string conversion */
static ipdConstString siUnitStrings[2][ipduntNUM_BASIC_UNITS+1] = {
{ "1", "m", "s", "kg", "A", "K", "cd" }, /* 'kg' system */
{ "1", "m", "s", "V",  "A", "K", "cd" }	 /* 'V' system */
};
static ipdLong siUnitConvert[2][ipduntNUM_BASIC_UNITS+1] = {
{ 0,  0, 0, 0, 0, 0, 0 },	/* 'kg' system */
{ 0, -2, 3, 0, 1, 0, 0 }	/* 'V' system */
};

/* ------------------------- The Prefix List ----------------------------- */

/* The prefixes are ordered according to their frequency;
   the most frequent one we regard to be the empty
   prefix string (no prefix at all).
*/

/* The prefix list                                                         */
static struct _unitPrefixes_
{
   ipdChar unitPrefixName[ipdLONG_SIZE];	/* same length as a ipdLong */
   ipdLong unitPrefixPowerOfTen;
} unitPrefixes[] = {
{  "",    0 },	/* empty prefix (no prefix)  */
{  "c",  -2 },  /* centi for cm              */
{  "m",  -3 },  /* milli for mm, ml, mV, ... */
{  "u",  -6 },	/* micro for um, uV, us, ... */
{  "n",  -9 },  /* nano  for nm, ns, nA, ... */
{  "k",   3 },  /* kilo  for km, kV, keV     */
{  "M",   6 },  /* Mega                      */
{  "p", -12 },  /* pico                      */
{  "d",  -1 },  /* deci                      */
{  "f", -15 },  /* femto                     */
{  "a", -18 },  /* atto                      */
{  "da",  1 },  /* deca                      */
{  "h",   2 },  /* hecto                     */
{  "G",   9 },  /* Giga                      */
{  "T",  12 },  /* Tera                      */
{  "P",  15 },  /* Peta                      */
{  "E",  18 },  /* Exa                       */
{  "z", -21 },  /* Zepto		     */
{  "Z",  21 },  /* Zetta		     */
{  "y", -24 },  /* Yocto		     */
{  "Y",  24 }   /* Yotta		     */
};


#define NUM_UNIT_PREFIXES \
    (sizeof(unitPrefixes) / sizeof(unitPrefixes[0]))

/* ------------------------------------------------------------------------- */
/*** untSortUnitParts sorts the unit items of the unit list alphabetically ***/

/** @brief This routine sorts the unit items of the unit list alphabetically.

   It uses the same sorting rules as will be afterwards used by the binary search in untParsePrefixedUnitPart.
*/
static ipdVoid untSortUnitParts()
{
   struct _unitDefinitions_ help; /* A helpful temporary storage */
   ipdInt i, j;	                /* for the sorting */
   ipdConstString cp1, cp2;	/* some helpful pointers */
   ipdVeStart("untSortUnitParts");	/* for automatic BackTracing */

   for (i = 0; i < NUM_UNIT_DEFINITIONS; i++) {
      help = unitDefinitions[i];
      for (j = i; j > 0; j--) {
	 cp1 = unitDefinitions[j-1].unitName;
	 cp2 = help.unitName;
	 while (*cp1 == *cp2) {
	    cp1++;
	    cp2++;
	 }
	 if (*cp1 > *cp2)
	    unitDefinitions[j] = unitDefinitions[j-1];
	 else
	    break;
      }
      unitDefinitions[j] = help;
   }
   ipdVeReturnVoid;
}



/* ------------------------------------------------------------------------- */
/** @brief Parses a unit string unitString and returns the factor with which the unit string starts and a pointer to the rest of the unit string.

    The factor is given as a combination of realFactor in the range of 1.0 ... 9.99.. and an exponent of 10 in expInt. foundOne is set to ipdTRUE.
    If no factor can be found, a factor of 1.0 and an exponent of 0
    are returned, but foundOne is set to ipdFALSE.

    @param actPos The unit string
    @param realFactor The real factor of the unit string
    @param expInt The integer exponent of 10
    @param foundOne Whether we found a factor or not
    @return ipdTRUE if the function operated successfully, otherwise ipdFALSE.
*/
ipdBoolean ipduntExtractUnitFactor(
	ipdConstString *actPos,	/* [IO] The unit string */
	ipdDouble *realFactor,	/* [O]  The real factor of the unit string */
	ipdLong *expInt,		/* [O]  The integer exponent of 10 */
	ipdBoolean *foundOne)	/* [O]  Whether we found a factor or not */
{
   ipdConstString unitString = *actPos;	/* For error messages */
   ipdConstString begPos, scanPos; 	/* Some help positions in the string */
   ipdLong expHelp;
   ipdDouble val;
   ipdVeStart("ipduntExtractUnitFactor"); /* for automatic BackTracing */

   /* if no real factor specified, take 1.0 */
   *realFactor = 1.0;
   /* set exponent of 10 (order of magnitude) to 0 */
   *expInt = 0;

#if 1
   /* [SW]: protect function against invalid input */
   if (unitString == NULL)
   {
      *foundOne = ipdFALSE;
      ipdVeReturn(ipdFALSE);
   }
#endif

   /* ------------------------ Skip leading blanks ---------------------- */
   while (isspace(**actPos))
      (*actPos)++;		/* ... this was quite easy */

   /* We could also have a unit string starting with the pseudounit "1",
      with an exponent - this we must find out here */

   /* look if it is a real factor */
   if ((isdigit(**actPos) || (**actPos == '.') || (**actPos == '+') || (**actPos == '-')) &&
       ((**actPos != '1') || (*(*actPos + 1) != '^' ))) {
      ipdBoolean negative = (**actPos == '-');
      /* yes, it is - so go on, baby */
      if ((**actPos == '+') || (**actPos == '-'))
	 (*actPos)++;		/* Skip the sign, if existing */
      val = 0.0;	/* Start with a factor of 0 */
      *expInt = -1;		/* That's just for the mantissa part */
      /*                           to provide for the place of the   */
      /*                           decimal point                     */

      /* Now - determine first non-zero digit and order of magnitude */
      /* of mantissa                                                 */
      while (**actPos == '0') (*actPos)++; /* Skip leading zeros */
      if (**actPos == '.') {	/* If decimal point comes before first */
      				/* non-zero digit:                     */
	 (*actPos)++;
	 while (**actPos == '0') {
	    (*actPos)++;
	    (*expInt)--;	/* count distance of decimal point and */
	 }			/* first non-zero digit in string      */
	 begPos = *actPos;	/* remember first non-zero digit */
      } else {			/* If first non-zero digit comes before */
      				/* decimal point:                       */
	 begPos = *actPos;	/* remember first non-zero digit */
	 while (isdigit(**actPos)) {
	    (*actPos)++;
	    (*expInt)++;	/* count distance of first non-zero */
	 }			/* digit in string                  */
	 if (**actPos == '.')
	    (*actPos)++;	/* omit a decimal point */
				/* if it's there        */
      }
      while (isdigit(**actPos))
	 (*actPos)++;		/* go forward to end of number */

      /* scan mantissa value between 1.0 and 9.99...  */
      scanPos = *actPos;	  /* from end backward not to loose accuracy */
      while (scanPos != begPos) {
	 scanPos--;
	 if (isdigit(*scanPos)) { /* That's for omitting the decimal point */
	    val /= 10;		/* Shift by one digit and add new digit */
	    val += *scanPos - '0';
	 }
      }
      if (((val < 0.9) || (val > 10.1)) && (val != 0.0)) {
	 /* Sorry - we can't allow that: error factor invalid */
	 ipdVeError2(ipdveALG,
		  "internal unit mantissa %d of unit string \"%s\" should be between 1.0 and 9.99...",
		  *realFactor, unitString);
	 ipdVeReturn(ipdFALSE);
      }
      /* now the mantissa is stored in realFactor * 10^expInt, */
      /* the realFactor lying between 1.0 and 9.99... */
      if (negative) val = -val;
      *realFactor = val;	/* store */

      /* scan exponential value if existing and add it to expInt */
      if (**actPos == 'E' || **actPos == 'e' ||
	  **actPos == 'd' || **actPos == 'D') {
	 (*actPos)++;
	 negative = (**actPos == '-'); /* remember negative sign */
	 if (**actPos == '-' || **actPos == '+') /* then skip sign   */
	    (*actPos)++;

	 expHelp = 0;		/* scan exponent of 10 in expHelp */
	 while (isdigit(**actPos)) {
	    expHelp *= 10;
	    expHelp += **actPos - '0';
	    (*actPos)++;
	 }
	 if (negative)
	    expHelp = -expHelp; /* take care for the sign */
	 *expInt += expHelp;	/* Add explicitely given exponent to expInt */
      }

      /* After the real number a separator must be found - or the */
      /* end of string                                            */
      if ((**actPos != '\0') && !isspace(**actPos) && !isalpha(**actPos) &&
	  (**actPos != '*') && (**actPos != '/') && (**actPos != '_')) {
	 /* error missing separator */
	 ipdVeError2(ipdveARG,
		  "missing separator in unit string \"%s\" at position >>\"%s\"",
		  unitString, *actPos);
	 ipdVeReturn(ipdFALSE);
      }
      /* remember that we have found a string element - so far */
      /* the string is already valid                           */
      *foundOne = ipdTRUE;
   }
   else
      *foundOne = ipdFALSE;

   ipdVeReturn(ipdTRUE);
}

/* ------------------------------------------------------------------------- */
/*** untParsePrefixedUnitPart parses a unit ***/

/** @brief This routine starts parsing a unit part that consists of a (possible) prefix and a unit. 

   This routine starts parsing a unit part that consists of a (possible) prefix and a unit. 
   It returns the representation of the unit together with the prefix in unitValue and a pointer to the very first location
   after the unit part in *nextPointer.

    @param prefixedUnitPart The next part to be parsed
    @param unitValue The unit structure is returned
    @param nextPointer A pointer behind the identified item

    @return The return value is ipdTRUE if the function operated successfully,
   otherwise ipdFALSE.
*/
static ipdBoolean untParsePrefixedUnitPart(
	ipdConstString prefixedUnitPart, /* [I] The next part to be parsed */
	ipduntUnitRep *unitValue,	       /* [O] The unit structure is returned */
	ipdConstString *nextPointer)     /* [O] A pointer behind the identified item */
{
   static ipdBoolean unsorted = ipdTRUE; /* Start with unsorted units */
   ipdLong beg, end, mid;	/* for the binary search */
   ipdLong numPrefix;		/* index into prefix list */
   ipdConstString cp1, cp2;	/* some helpful pointers */
   ipdConstString unitPart;	/* pointer after end of prefix */

   /* Sort the unit list */
   if (unsorted) {
      untSortUnitParts();
      unsorted = ipdFALSE;
   }

   /* linearly search in the list of prefixes */
   for (numPrefix = 0; numPrefix < NUM_UNIT_PREFIXES; numPrefix++) {
      /* compare prefix and beginning of prefixed Unit */
      cp1 = prefixedUnitPart;
      cp2 = unitPrefixes[numPrefix].unitPrefixName;
      while (*cp2 != '\0' && *cp1 == *cp2) {
	 cp1++;
	 cp2++;			/* to next character */
      }
      /* if not full prefix contained in prefixed Unit, try next prefix */
      if (*cp2 != '\0')
	 continue;

      /* now that the prefix is fully contained, remember start of unit */
      unitPart = cp1;

      /* binary search for the matching unit definition */
      beg = 0;
      end = NUM_UNIT_DEFINITIONS;
      while (beg < end) {
	 mid = (beg + end) / 2;

	 /* compare unit Part of string to unit definition */
	 cp1 = unitPart;
	 cp2 = unitDefinitions[mid].unitName;
	 while (*cp1 == *cp2 && *cp2 != '\0') {
	    cp1++;
	    cp2++;
	 }

	 /* stop if interval size is one (with beg == mid it is ensured that
	    the beg value has also been tested) or one of strings has ended */
	 if (beg == mid || (*cp2 == '\0' && !isalpha(*cp1) && *cp1 != '1'))
	    break;
	 if (*cp1 < *cp2)
	    end = mid;
	 else
	    beg = mid;
      }

      /* if interval size has shrinked to zero, an error has occurred;
	 this should never happen */
      if (beg >= end)
	 break;

      /* if strings are equal, unit is found: return the description */
      if (*cp2 == '\0' && !isalpha(*cp1) && *cp1 != '1') {
	 /* copy unit description to return value */
	 *unitValue = unitDefinitions[mid].unitRep;

	 /* and add power of ten for the prefix */
	 unitValue->unitBasics[ipduntUNIT_EXPONENT] +=
	    unitPrefixes[numPrefix].unitPrefixPowerOfTen;

	 /* continue parsing at first position after this unit part */
	 *nextPointer = cp1;
	 return(ipdTRUE);		/* that's it - this is the normal exit */
      }

      /* now - let's look at the next prefix */
   }

   /* if loop is finished, no unit could be found */
   /* error unit part could not be parsed */
   /* The error is reported in the calling program ipduntParseUnitString */
   *nextPointer = cp1;
   return(ipdFALSE);
}

/* ------------------------------------------------------------------------- */
/** @brief Parses a unit string unitString and returns the representation of the unit string in internal representation in the variable ipduntUnitRep.

    This means the result is stored in half-integer coding with all integers contained in the unit
    representation structure have only a meaning of half of their numerical
    value. This is done to provide for units with square roots like
    nV/sqrt(Hz) for the noise distribution.

    @param unitString unit string
    @param unitValue internal represenation of the unit
    @return The return value is ipdTRUE if
    the function operated successfully, otherwise ipdFALSE. */
ipdBoolean ipduntParseUnitString(
	ipdConstString  unitString, /* [I] unit string */
	ipduntUnitRep *unitValue)	  /* [O] internal representation of unit
				     string */
{
   ipdInt i;
   ipdBoolean foundOne, foundFraction, negative;
   ipdConstString actPos;
   ipdDouble realFactor, realHelp;
   ipdLong expInt, expHelp;
   ipduntUnitRep helpUnit;
   ipdBoolean rstat;
   ipdVeStart("ipduntParseUnitString"); /* for automatic BackTracing */

   /* remember you haven't found any valid elements of the unit string
      up to now */
   foundOne = ipdFALSE;
   /* remember you haven't found a fraction symbol ('/') up to now */
   foundFraction = ipdFALSE;
   /* start at the beginning of the unit string */
   actPos = unitString;

   /* now let's go in medias res: do the parsing */
   /* -------------- *** first Part: real factor *** -------------------- */
   rstat = ipduntExtractUnitFactor(&actPos, &realFactor, &expInt, &foundOne);
   if (!rstat)
      ipdVeReturn(rstat);

   /* --------- *** second Part: units with prefixes *** ---------------- */
   /* initialize unitValue */
   unitValue->unitFactor = realFactor;
   unitValue->unitBasics[ipduntUNIT_EXPONENT] = expInt * 2;
				/* due to half-integer coding */
   for ( i = 1; i <= ipduntNUM_BASIC_UNITS; i++ )
      unitValue->unitBasics[i] = 0; /* reset all SI units */

   for (;;) {		/* loop for prefixed unit parts */
      while (isspace(*actPos))
	 actPos++;		/* Skip leading blanks */
      if (*actPos == '\0') {	/* On end of string return */
	 /* normal return */
	 if (foundOne)
	    ipdVeReturn(ipdTRUE);
	 /* error empty unit string */
	 ipdVeError(ipdveARG,
		 "unit string is empty");
	 ipdVeReturn(ipdFALSE);
      }
      if ((*actPos == '*') || (*actPos == '_')) { /* A multiplicative separator */
	 if (!foundOne)	{	/* It may not be placed at the  */
	 			/* beginning of the string */
	    /* error no multiplication separator at beginning allowed */
	    ipdVeError2(ipdveARG,
		     "found unit multiplicator on bad position >>\"%s\" in unit string \"%s\"",
		     actPos, unitString);
	    ipdVeReturn(ipdFALSE);
	 }
	 actPos++;
      } else if (*actPos == '/') { /* If no multiplicative separator, a */
      				   /* division is allowed               */
	 if (foundFraction) {	  /* but not if a division has already */
	 			  /* been recognized                   */
	    /* error double fraction symbol */
	    ipdVeError2(ipdveARG,
		     "double fraction sign found on position >>\"%s\" in unit string \"%s\"",
		     actPos, unitString);
	    ipdVeReturn(ipdFALSE);
	 }
	 actPos++;
	 foundFraction = 1;
      }
      while (isspace(*actPos))
	 actPos++;		/* Skip rest of the separator */

      /* read prefixed unit */
      if (untParsePrefixedUnitPart(actPos, &helpUnit, &actPos) == ipdFALSE) {
	 /* Report the error which has been detected in
	    untParsePrefixedUnitString */
	 ipdVeError2(ipdveFCF,
		  "bad item found in unit string \"%s\" at position >>\"%s\"",
		  unitString, actPos);
	 ipdVeReturn(ipdFALSE);
      }

      /* determine exponent */
      negative = foundFraction;	/* exponent after '/' gets negative sign */
      expInt = 2;		/* default exponent is 1 (half-integer!) */
      if (*actPos == '^') {	/* Somebody wants an other one */
	 actPos++;
	 if (*actPos == '-')
	    negative = !negative; /* change sign */
	 if (*actPos == '-' || *actPos == '+')
	    actPos++;		/* skip sign */
	 expInt = 0;		/* this shouldn't stay for long */
	 if (!isdigit(*actPos)) {
	    /* error no exponent */
	    ipdVeError2(ipdveARG,
		     "no exponent in unit string \"%s\" at position >>\"%s\"",
		     unitString, actPos);
	    ipdVeReturn(ipdFALSE);
	 }
	 while (isdigit(*actPos)) { /* scan the exponent: integer */
	    expInt *= 10;
	    expInt += *actPos - '0';
	    actPos++;
	 }
	 expInt *= 2;		/* now make it half-integer */
	 if (*actPos == '.') {	/* If exponent is no integer, round it */
	 			/* to a half-integer */
	    actPos++;
	    if (isdigit(*actPos)) {
	       if (*actPos >= '7') /* next full integer */
		  expInt += 2;
	       else if (*actPos >= '3') /* next half integer */
		  expInt += 1;
	    }
	    while (isdigit(*actPos)) actPos++; /* ignore rest of decimals */
	 }
      }
      expHelp = expInt;		/* remember the absolute value */
      if (negative)		/* and give the exponent its sign */
	 expInt = -expInt;

      /* calculate the power of the real factor */
      realHelp = helpUnit.unitFactor;
      realFactor = 1.0;		/* If full integer, start with 1.0 */
      if (expHelp & 1)		/* else with the square root of the */
	 realFactor = sqrt(realHelp); /* unit factor                 */
      expHelp /= 2;		/* now make the half integer a full one */
      while (expHelp) {		/* binary compose the real factor */
	 if (expHelp & 1)
	    realFactor *= realHelp;
	 realHelp *= realHelp;
	 expHelp /= 2;
      }
      if (negative)		/* at last give the real factor */
	 realFactor = 1.0 / realFactor;	/* the proper 'sign'    */

      /* multiply parsed unit to return Value */
      unitValue->unitFactor *= realFactor; /* as composed before */
      for (i = 0; i <= ipduntNUM_BASIC_UNITS; i++ )
				/* give the basic units their */
	 unitValue->unitBasics[i] += /* appropriate exponents      */
	    helpUnit.unitBasics[i] * expInt;


      /* After the prefixed unit a separator must be found - or the */
      /* end of string                                              */
      if ((*actPos != '\0') && !isspace(*actPos) && !isalpha(*actPos) &&
	  (*actPos != '*') && (*actPos != '/') && (*actPos != '_')) {
	 /* error missing separator */
	 ipdVeError2(ipdveARG,
		  "missing separator in unit string \"%s\" at position >>\"%s\"",
		  unitString, actPos);
	 ipdVeReturn(ipdFALSE);
      }
      /* remember that we have found a string element - so far */
      /* the string is already valid                           */
      foundOne = ipdTRUE;

      /* and now continue with the next prefixed unit part */
   }
   /* no return since no regular exit of while-loop */
}


/* ------------------------------------------------------------------------- */
/** @brief Computes the ratio of two units unitRep1 and unitRep2 in internal representation.

    @param unitRep1 First internal unit
    @param unitRep2 Second internal unit
    @param ratio Result of unit1 / unit2
    @return If the two units are not comparable, ipdFALSE is returned, otherwise ipdTRUE for success. 
*/

ipdBoolean ipduntUnitRatio(
	ipduntUnitRep *unitRep1,	/* first internal unit */
	ipduntUnitRep *unitRep2,	/* second internal unit */
	ipdDouble *ratio)		/* unit1 / unit2 */
{
   ipdLong unitInd;		/* The index for the basic units */
   ipdLong expBits;		/* The (shifted) exponent of 10 */
   ipdBoolean negative;		/* The boolean for the sign of the exponent */
   ipdDouble pow10;		/* 10 raised to a power of 2 */
   ipdDouble limit;		/* Number limit to do some calculations */
   ipdDouble factor;		/* The unit ratio being composed */

   ipdVeStart("ipduntUnitRatio");	/* for automatic BackTracing */
   /* *** test for physical dimension mismatch *** */
   for (unitInd = 1; unitInd <= ipduntNUM_BASIC_UNITS; unitInd++) {
      if (unitRep1->unitBasics[unitInd] != unitRep2->unitBasics[unitInd]) {
	 ipdConstString unit1, unit2;
	 unit1 = ipduntCreateUnitString(unitRep1,
				     ipduntOUTPUT_VOLT | ipduntOUTPUT_IPDNONUMBER);
	 unit2 = ipduntCreateUnitString(unitRep2,
				     ipduntOUTPUT_VOLT | ipduntOUTPUT_IPDNONUMBER);
	 ipdVeError2(ipdveARG,
		  "units %s and %s are physically not comparable",
		  unit1,
		  unit2);
	 ipdvFree(unit1);
	 ipdvFree(unit2);		/* free the automatic allocated memory */
	 ipdVeReturn(ipdFALSE);
      }
   }
   /* *** exponent of 10 separated in sign and absolute value *** */
   expBits =			/* exponent of 10 */
      unitRep1->unitBasics[ipduntUNIT_EXPONENT] -
	 unitRep2->unitBasics[ipduntUNIT_EXPONENT];
   negative = ipdFALSE;		/* sign of exponent of 10 */
   if (expBits < 0) {
      negative = ipdTRUE;
      expBits = -expBits;	/* absolute value of exponent of 10 */
   }

   /* *** start with ratio of unit factors *** */
   if (negative)		/* ratio of real factor */
      factor = unitRep2->unitFactor / unitRep1->unitFactor;
   else
      factor = unitRep1->unitFactor / unitRep2->unitFactor;

   /* *** multiply by 10 ^ exponent; exponent is half-integer coded *** */

   /* --- first care for square root of 10 --- */
   if (expBits & 1)		/* remember the half-integer coding */
      factor *= sqrt(10.0);	/* last bit means 10 ^ 0.5 */
   expBits /= 2;		/* make half integer full integer */

   /* --- then binary 'add' all the power-of-10-factors --- */
   pow10 = 10.0;		/* start exponent of 10 is 1 */
   limit = ipdDOUBLE_MAX / pow10;
   for (;;) {
      if (expBits & 1) {	/* odd number leads to multiplication ... */
	 if (factor >= limit) {	/* real overflow test */
	    ipdVeError(ipdveCNF,
		    "unit conversion factor exceeds machine limit");
	    ipdVeReturn(ipdFALSE);
	 }
	 factor *= pow10;	/* ... with power of 10 for next bit */
      }
      expBits /= 2;		/* binary decrease expBits */
      if (expBits == 0) 	/* ready - don't do any more multiplications */
	 break;
      if (pow10 >= limit) {	/* real overflow test */
	 ipdVeError(ipdveCNF,
		 "unit conversion factor exceeds machine limit");
	 ipdVeReturn(ipdFALSE);
      }
      pow10 *= pow10;		/* binary increase (= double) exponent of 10 */
   }

   /* *** and give the power the right exponential sign *** */
   if (negative)
      *ratio = 1.0 / factor;
   else
      *ratio = factor;
   ipdVeReturn(ipdTRUE);
}


/* ------------------------------------------------------------------------- */
/** @brief Checks if the two units unitRep1 and unitRep2 in internal representation are compareable. 

    @param unitRep1 First internal unit
    @param unitRep2 Second internal unit
    @return If the two units are not comparable, ipdFALSE is returned, else ipdTRUE.
*/
ipdBoolean ipduntUnitCompareable(
	ipduntUnitRep *unitRep1,	/* first internal unit */
	ipduntUnitRep *unitRep2)	/* second internal unit */
{
   ipdLong unitInd;		/* The index for the basic units */

   ipdVeStart("ipduntUnitCompareable"); /* for automatic BackTracing */
   /* *** test for physical dimension mismatch *** */
   for (unitInd = 1; unitInd <= ipduntNUM_BASIC_UNITS; unitInd++) {
      if (unitRep1->unitBasics[unitInd] != unitRep2->unitBasics[unitInd])
	 ipdVeReturn(ipdFALSE);
   }
   ipdVeReturn(ipdTRUE);
}

/* ------------------------------------------------------------------------- */
/** @brief Creates a new unit

    @param unitstr Unit string to generate part
    @param name Name of the unit
    @param exponent Exponent factor of the unit
    @return New end of string
*/
static ipdString untCreateOneUnit(
	ipdString unitstr,	/* [IO] unit string to generate part */
	ipdConstString name,	/* [I] name of the unit */
	ipdLong exponent)		/* [I] exponent factor of unit */
{
   while (*name)
      *unitstr++ = *name++;	/* copy unit name */

   if (exponent != 2) {
      *unitstr++ = '^';		/* non linear unit */
      sprintf(unitstr, "%d", exponent / 2);
      while (*unitstr)
	 unitstr++;		/* go over number */
      if (exponent & 1)
	 *unitstr++= '.', *unitstr++ = '5'; /* sqrt of unit */
   }
   return (unitstr);		/* return new end of string */
}

/* ------------------------------------------------------------------------- */
/*** ipduntCreateUnitString transforms a unit representation to a string ***/

/** @brief Transforms a unit representation as it is used throughout the unit library for handling units to a unit string.

    @param unitRep Unit to be transformed
    @param flags Output flags
    @return A pointer to the unit string, which is allocated by ipdvStrdup inside the routine. If no space is left for the allocation or
    an other error occurred, ipdNULL is returned. */
ipdConstString ipduntCreateUnitString(
	ipduntUnitRep *unitRep,    /* [I] unit to be transformed */
	ipdLongWord flags)	/* [I] output flags */
{
   ipdChar buffer[ipduntMAX_UNITSLENGTH+1];
   ipdDouble unitFactor;
   ipdString cp1, cp2;
   ipdConstString returnString;
   ipdBoolean negative = ipdFALSE;
   ipdLong exponent = 0;		/* The exponent of the number in the string
				   or of a unit in the string */
   ipdLong unitExponent;		/* The exponent of 10 in the unit rep. */
   ipdLong voltsExponent;		/* The exponent of the volts in the string */
   ipdBoolean kg;			/* output in 'kg', not 'V' */
   ipdBoolean first;
   ipdInt i, ind;
   ipdVeStart("ipduntCreateUnitString"); /* for automatic BackTracing */

   unitFactor   = unitRep->unitFactor;
   unitExponent = unitRep->unitBasics[ipduntUNIT_EXPONENT];
   if ((flags & ipduntOUTPUT_KG) &&
       (unitRep->unitBasics[ipduntUNIT_KILOGRAMM] != 0)) {
      /* we have to do some adaption due to g to kg conversion */
      unitExponent -= unitRep->unitBasics[ipduntUNIT_KILOGRAMM] * 3;
      kg = ipdTRUE;
   } else
      kg = ipdFALSE;

   cp1 = &(buffer[0]);
   if ((flags & ipduntOUTPUT_IPDNONUMBER) != ipduntOUTPUT_IPDNONUMBER) {
      if (unitExponent & 1) {
	 unitFactor *= sqrt(10.0);
	 if (unitFactor >= 10.0) {
	    unitFactor = unitRep->unitFactor / sqrt(10.0);
	    unitExponent++;
	 } else
	    unitExponent--;
      }
      /* this prints a leading number to the unit */
      sprintf(cp1,"%.40g",unitFactor);
      while (*cp1 != 'e' && *cp1 != 'E' && *cp1 != '\0') cp1++;
      if (*cp1 != '\0') {
	 cp2 = cp1 + 1;
	 if (*cp2 == '-') {
	    negative = ipdTRUE;
	    cp2++;
	 }
	 if (*cp2 == '+') cp2++;
	 while (isdigit(*cp2)) {
	    exponent *= 10;
	    exponent += *cp2 - '0';
	    cp2++;
	 }
	 if (negative)
	    exponent = -exponent;
      }
      exponent += unitExponent / 2;
      if (exponent != 0) {
	 sprintf(cp1,"e%d",exponent);
	 while (*cp1) cp1++;
      }
      *cp1++ = ' ';
   }

   /* Now create the system of basic units */

   /* For we deal mostly with electrotechnics, and for g is
      not a SI unit and kg needs a prefix, we may choose V instead */
   voltsExponent = kg ? 0 : unitRep->unitBasics[ipduntUNIT_KILOGRAMM];
   ind = (kg ? 0 : 1);
   if (flags & ipduntOUTPUT_DIV) {
      first = ipdTRUE;
      for (i = 1; i < ipduntNUM_BASIC_UNITS; i++) {
	 exponent = unitRep->unitBasics[i] +
	    siUnitConvert[ind][i] * voltsExponent;
	 if (exponent > 0) {
	    if (first)
	       first = ipdFALSE;
	    else
	       *cp1++ = (flags & ipduntOUTPUT_MULT) ? '*' : ' ';
	    cp1 = untCreateOneUnit(cp1, siUnitStrings[ind][i], exponent);
	 }
      }
      if (first && ((flags & ipduntOUTPUT_IPDNONUMBER) == ipduntOUTPUT_IPDNONUMBER))
	 *cp1++ = '1';		/* need a one */
      first = ipdTRUE;
      for (i = 1; i < ipduntNUM_BASIC_UNITS; i++) {
	 exponent = unitRep->unitBasics[i] +
	    siUnitConvert[ind][i] * voltsExponent;
	 if (exponent < 0) {
	    if (first)
	       *cp1++ = '/', first = ipdFALSE;
	    else
	       *cp1++ = (flags & ipduntOUTPUT_MULT) ? '*' : ' ';
	    cp1 = untCreateOneUnit(cp1, siUnitStrings[ind][i], -exponent);
	 }
      }
   } else {
      first = ipdTRUE;
      for (i = 1; i < ipduntNUM_BASIC_UNITS; i++) {
	 exponent = unitRep->unitBasics[i] +
	    siUnitConvert[ind][i] * voltsExponent;
	 if (exponent != 0) {
	    if (first)
	       first = ipdFALSE;
	    else
	       *cp1++ = (flags & ipduntOUTPUT_MULT) ? '*' : ' ';
	    cp1 = untCreateOneUnit(cp1, siUnitStrings[ind][i], exponent);
	 }
      }
      if (first && ((flags & ipduntOUTPUT_IPDNONUMBER) == ipduntOUTPUT_IPDNONUMBER))
	 *cp1++ = '1';		/* need a one */
   }
   *cp1 = '\0';

   /* Allocate the return value String */
   returnString = ipdvStrdup(buffer);
   if (returnString == ipdNULL)
      ipdVeError(ipdveSYS,
	      "cannot allocate return string");
   ipdVeReturn (returnString);
}

