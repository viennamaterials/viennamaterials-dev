
/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

/** @file ipdparse.c
    @brief A GNU Bison parser used for parsing scanned .ipd files
*/

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse yyparse
#define yylex yylex
#define yyerror yyerror
#define yylval yylval
#define yychar yychar
#define yydebug yydebug
#define yynerrs yynerrs
#define	INTEGER	259
#define	REAL	260
#define	STRING_PART	261
#define	ID	262
#define	PATH	263
#define	INCLUDE_FILENAME	264
#define	INCLUDE	265
#define	SEMICOLON	266
#define	DELETE	267
#define	EVAL	268
#define	COLON	269
#define	ASSIGNMENT	270
#define	LOGIC_OR	271
#define	LOGIC_AND	272
#define	EQ	273
#define	NE	274
#define	LT	275
#define	LE	276
#define	GT	277
#define	GE	278
#define	IPDNOT	279
#define	COMPLEX	280
#define	UNARYPLUS	281
#define	UNARYMINUS	282
#define	KEY	283
#define	EXT	284
#define	AUX	285
#define	TRACE	286
#define	ALIAS	287

/* #line 143 "/home/markus/vproject/ipd/ipdparse.y" */


#include "ipdP.h"
#include <stdio.h>

#define	BOOLEAN	258


#if 0
#define YYERROR_VERBOSE 1
#endif

#if IPD_PARSER_ipdDEBUG_INFO
  #define ERRP(x) x
#else
  #define ERRP(x)
#endif

#if 0
int ipdAbortParsing(void);
#endif
#define IPD_ABORT_PARSING {fprintf(stderr, ipdERR_PREFIX " parsing aborted.\n"); YYABORT; }

/*#line 171 "/home/markus/vproject/ipd/ipdparse.y" */
typedef union {        /* must use the proper member:       */
                /* first set in scan.l               */
  ipdBoolean       b;    /* boolean scanned                   */
  long           i;    /* INTEGER scanned                   */
  double         d;    /* REAL scanned                      */
  char          *s;    /* ID or STRING_PART scanned    */
  ipdTreeNode_t *tn;   /* stores value, expr                */
  struct
  {
     char *s;
     int   a;          /* '+' or no '+' to force appending items */
  } p;                 /* ID or +ID */
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		233
#define	YYFLAG		-32768
#define	YYNTBASE	49

#define YYTRANSLATE(x) ((unsigned)(x) <= 287 ? yytranslate[x] : 82)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,    39,     2,     2,    26,     2,    38,    47,
    48,    27,    25,    42,    24,     2,    28,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    40,
     2,     2,    41,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    45,     2,    46,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    43,     2,    44,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    29,    30,
    31,    32,    33,    34,    35,    36,    37
};

#if YYipdDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     4,     7,    11,    12,    15,    18,    21,    24,
    27,    29,    31,    33,    35,    37,    39,    43,    47,    50,
    52,    55,    58,    60,    64,    68,    71,    73,    77,    79,
    83,    86,    88,    92,    96,    99,   100,   106,   108,   112,
   115,   117,   121,   125,   129,   133,   136,   140,   144,   146,
   148,   152,   156,   158,   161,   164,   169,   174,   179,   183,
   185,   188,   191,   195,   199,   202,   207,   212,   216,   219,
   222,   227,   232,   235,   238,   241,   243,   244,   252,   258,
   262,   266,   270,   273,   275,   279,   283,   285,   289,   293,
   297,   301,   304,   309,   314,   318,   321,   325,   330,   335,
   339,   341,   343,   345,   349,   353,   357,   361,   365,   369,
   373,   377,   381,   385,   389,   393,   397,   401,   405,   409,
   413,   417,   421,   425,   429,   433,   437,   441,   445,   449,
   452,   455,   458,   461,   464,   467,   471,   475,   478,   480,
   482,   485,   487,   489,   492,   495,   497,   500,   503,   505,
   508,   511,   513,   515,   516,   522
};

static const short yyrhs[] = {    -1,
    49,    52,     0,    49,    51,     0,    38,    76,    11,     0,
     0,    50,    51,     0,    68,    11,     0,    68,     1,     0,
    80,    11,     0,    80,     1,     0,    70,     0,    72,     0,
    75,     0,    58,     0,    11,     0,     1,     0,    10,    39,
     9,     0,    10,    40,     9,     0,    10,     1,     0,     7,
     0,    25,     7,     0,     8,     7,     0,    53,     0,    20,
    53,    22,     0,    20,    53,     1,     0,    20,     1,     0,
    65,     0,    65,    41,    76,     0,    55,     0,    55,    42,
    56,     0,    55,     1,     0,    54,     0,    54,    14,    56,
     0,    54,    14,     1,     0,    57,    11,     0,     0,    57,
    43,    59,    50,    44,     0,     7,     0,     7,    42,    60,
     0,     7,     1,     0,    76,     0,    76,    42,    61,     0,
    76,    42,     1,     0,    45,    61,    46,     0,    45,    61,
     1,     0,    45,     1,     0,     7,    15,    76,     0,     7,
    15,     1,     0,    76,     0,    63,     0,    76,    42,    64,
     0,    63,    42,    64,     0,     7,     0,     8,     7,     0,
     8,     1,     0,    65,    45,    76,    46,     0,    66,    45,
    76,    46,     0,    65,    45,    76,     1,     0,    65,    45,
     1,     0,    53,     0,    33,    53,     0,    33,     1,     0,
    67,    15,    76,     0,    67,    15,     1,     0,    67,     1,
     0,    35,    53,    15,    76,     0,    35,    53,    15,     1,
     0,    35,    53,     1,     0,    35,     1,     0,    34,    53,
     0,    34,    53,    15,    76,     0,    34,    53,    15,     1,
     0,    34,     1,     0,    76,    11,     0,    76,     1,     0,
     1,     0,     0,     7,    47,    60,    48,    15,    71,    69,
     0,     7,    47,    60,    48,     1,     0,     7,    47,     1,
     0,    12,    65,    11,     0,    12,    65,     1,     0,    12,
     1,     0,    65,     0,    65,    15,    76,     0,    65,    15,
     1,     0,    73,     0,    74,    42,    73,     0,    74,    42,
     1,     0,    36,    74,    11,     0,    36,    74,     1,     0,
    36,     1,     0,    13,    47,    76,    48,     0,    13,    47,
    76,     1,     0,    13,    47,     1,     0,    13,     1,     0,
     7,    47,    48,     0,     7,    47,    64,    48,     0,     7,
    47,    64,     1,     0,     7,    47,     1,     0,    79,     0,
    65,     0,    66,     0,    76,    25,    76,     0,    76,    25,
     1,     0,    76,    24,    76,     0,    76,    24,     1,     0,
    76,    27,    76,     0,    76,    27,     1,     0,    76,    28,
    76,     0,    76,    28,     1,     0,    76,    26,    76,     0,
    76,    26,     1,     0,    76,    18,    76,     0,    76,    18,
     1,     0,    76,    19,    76,     0,    76,    19,     1,     0,
    76,    20,    76,     0,    76,    20,     1,     0,    76,    22,
    76,     0,    76,    22,     1,     0,    76,    21,    76,     0,
    76,    21,     1,     0,    76,    23,    76,     0,    76,    23,
     1,     0,    76,    16,    76,     0,    76,    16,     1,     0,
    76,    17,    76,     0,    76,    17,     1,     0,    29,    76,
     0,    29,     1,     0,    24,    76,     0,    24,     1,     0,
    25,    76,     0,    25,     1,     0,    47,    76,    48,     0,
    47,    76,     1,     0,    47,     1,     0,    62,     0,     6,
     0,    77,     6,     0,    30,     0,     4,     0,     4,    30,
     0,    30,     4,     0,     5,     0,     5,    30,     0,    30,
     5,     0,    78,     0,    78,     7,     0,    78,    77,     0,
    77,     0,     3,     0,     0,    37,    81,     7,    15,    76,
     0,    37,     1,     0
};

#endif

#if YYipdDEBUG != 0
static const short yyrline[] = { 0,
   282,   283,   284,   285,   311,   312,   319,   336,   343,   360,
   367,   368,   369,   370,   371,   372,   386,   400,   414,   423,
   429,   435,   453,   468,   484,   491,   501,   517,   562,   567,
   582,   592,   597,   612,   625,   642,   662,   675,   689,   706,
   717,   722,   737,   748,   752,   759,   770,   785,   795,   800,
   805,   820,   849,   854,   866,   877,   883,   888,   896,   907,
   912,   917,   929,   947,   954,   961,   979,   986,   993,   999,
  1015,  1033,  1040,  1050,  1068,  1078,  1091,  1097,  1097,  1106,
  1118,  1131,  1138,  1148,  1153,  1158,  1169,  1173,  1177,  1187,
  1190,  1196,  1208,  1213,  1220,  1226,  1232,  1238,  1244,  1252,
  1259,  1264,  1269,  1274,  1286,  1293,  1305,  1312,  1324,  1331,
  1343,  1350,  1362,  1369,  1381,  1388,  1400,  1407,  1419,  1426,
  1438,  1445,  1457,  1464,  1476,  1483,  1495,  1502,  1514,  1521,
  1529,  1535,  1570,  1576,  1581,  1587,  1592,  1598,  1604,  1615,
  1619,  1630,  1638,  1643,  1651,  1659,  1667,  1675,  1689,  1693,
  1722,  1751,  1756,  1765,  1769,  1804
};
#endif


#if YYipdDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","BOOLEAN",
"INTEGER","REAL","STRING_PART","ID","PATH","INCLUDE_FILENAME","INCLUDE","SEMICOLON",
"DELETE","EVAL","COLON","ASSIGNMENT","LOGIC_OR","LOGIC_AND","EQ","NE","LT","LE",
"GT","GE","'-'","'+'","'%'","'*'","'/'","IPDNOT","COMPLEX","UNARYPLUS","UNARYMINUS",
"KEY","EXT","AUX","TRACE","ALIAS","'''","'\"'","'<'","'?'","','","'{'","'}'",
"'['","']'","'('","')'","source","input","command","include_statement","add_ID",
"sectionSpec","baseSection","sectionList","sectionDeclaration","section","@1",
"ID_list","expr_sqz","array","assoc_param_expr","parameter","path_id","arr_idx",
"key_word","stmt","function_expr","function_stmt","@2","delete_stmt","trace_item",
"trace_list","trace_stmt","expr","string","complex_number","const_value","alias",
"@3", NULL
};
#endif

static const short yyr1[] = {     0,
    49,    49,    49,    49,    50,    50,    51,    51,    51,    51,
    51,    51,    51,    51,    51,    51,    52,    52,    52,    53,
    53,    53,    54,    54,    54,    54,    55,    55,    56,    56,
    56,    57,    57,    57,    58,    59,    58,    60,    60,    60,
    61,    61,    61,    62,    62,    62,    63,    63,    64,    64,
    64,    64,    65,    65,    65,    66,    66,    66,    66,    67,
    67,    67,    68,    68,    68,    68,    68,    68,    68,    68,
    68,    68,    68,    69,    69,    69,    71,    70,    70,    70,
    72,    72,    72,    73,    73,    73,    74,    74,    74,    75,
    75,    75,    76,    76,    76,    76,    76,    76,    76,    76,
    76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
    76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
    76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
    76,    76,    76,    76,    76,    76,    76,    76,    76,    77,
    77,    78,    78,    78,    78,    78,    78,    78,    79,    79,
    79,    79,    79,    81,    80,    80
};

static const short yyr2[] = {     0,
     0,     2,     2,     3,     0,     2,     2,     2,     2,     2,
     1,     1,     1,     1,     1,     1,     3,     3,     2,     1,
     2,     2,     1,     3,     3,     2,     1,     3,     1,     3,
     2,     1,     3,     3,     2,     0,     5,     1,     3,     2,
     1,     3,     3,     3,     3,     2,     3,     3,     1,     1,
     3,     3,     1,     2,     2,     4,     4,     4,     3,     1,
     2,     2,     3,     3,     2,     4,     4,     3,     2,     2,
     4,     4,     2,     2,     2,     1,     0,     7,     5,     3,
     3,     3,     2,     1,     3,     3,     1,     3,     3,     3,
     3,     2,     4,     4,     3,     2,     3,     4,     4,     3,
     1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
     2,     2,     2,     2,     2,     3,     3,     2,     1,     1,
     2,     1,     1,     2,     2,     1,     2,     2,     1,     2,
     2,     1,     1,     0,     5,     2
};

static const short yydefact[] = {     1,
     0,     0,   153,   143,   146,   140,    53,     0,     0,     0,
     0,     0,   142,     0,     0,   139,   102,   103,     0,   152,
   149,   101,    16,    20,     0,     0,    15,     0,     0,     0,
     0,     0,     0,     0,     0,     3,     2,    23,    32,     0,
    14,     0,     0,    11,    12,    13,     0,   144,   147,     0,
    55,    54,    96,     0,   133,   132,   135,   134,   131,   130,
   145,   148,    46,     0,    41,   138,     0,     0,     0,     4,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   141,   150,   151,     0,    22,    19,     0,
     0,    83,    53,     0,    26,    20,     0,    21,    62,    61,
    73,    70,    69,     0,    92,    84,    87,     0,   156,     0,
     0,    35,    36,    65,     0,     8,     7,    10,     9,   100,
    53,    97,    50,     0,    49,    95,     0,    45,    44,     0,
   137,   136,    59,     0,     0,   127,   126,   129,   128,   115,
   114,   117,   116,   119,   118,   123,   122,   121,   120,   125,
   124,   107,   106,   105,   104,   113,   112,   109,   108,   111,
   110,    80,     0,     0,    17,    18,    82,    81,    25,    24,
     0,    68,     0,     0,    91,    90,     0,     0,    34,     0,
    33,    27,     5,    64,    63,     0,     0,    99,    98,     0,
    94,    93,    43,    42,    58,    56,    57,    40,     0,     0,
    72,    71,    67,    66,    86,    85,    89,    88,     0,    31,
     0,     0,     0,    48,    47,    52,    51,    39,    79,    77,
   155,    30,    28,    37,     6,     0,    76,    78,     0,    75,
    74,     0,     0
};

static const short yydefgoto[] = {     2,
   213,    36,    37,    38,    39,   180,   181,    40,    41,   183,
   164,    64,    16,   123,   124,    17,    18,    42,    43,   228,
    44,   226,    45,   107,   108,    46,   125,    20,    21,    22,
    47,   110
};

static const short yypact[] = {   -26,
   812,   893,-32768,    35,    94,-32768,   -15,    29,     5,   151,
   199,   213,   187,   244,   258,-32768,    76,    83,   973,   140,
   203,-32768,-32768,   104,   146,     7,-32768,   159,   119,   154,
   161,   170,   764,   181,    59,-32768,-32768,    27,   160,    -6,
-32768,    39,    24,-32768,-32768,-32768,   111,-32768,-32768,    14,
-32768,-32768,-32768,   289,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,     2,   933,-32768,    82,   303,   812,-32768,
   334,   348,   379,   393,   424,   438,   469,   483,   514,   528,
   559,   573,   604,-32768,-32768,   140,    62,-32768,-32768,   156,
   174,-32768,-32768,   112,-32768,-32768,    28,-32768,-32768,-32768,
-32768,   164,-32768,    40,-32768,   175,-32768,    22,-32768,   190,
   186,-32768,-32768,-32768,   618,-32768,-32768,-32768,-32768,-32768,
     9,-32768,   157,     1,   946,-32768,   115,-32768,-32768,   649,
-32768,-32768,-32768,   786,    69,-32768,   998,-32768,  1009,-32768,
  1009,-32768,  1009,-32768,  1018,-32768,  1018,-32768,  1018,-32768,
  1018,-32768,    91,-32768,    91,-32768,   204,-32768,-32768,-32768,
-32768,-32768,     3,   153,-32768,-32768,-32768,-32768,-32768,-32768,
   663,-32768,   694,   708,-32768,-32768,   226,   198,-32768,    15,
-32768,   189,-32768,-32768,   986,   739,   840,-32768,-32768,   840,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   208,   110,
-32768,   986,-32768,   986,-32768,   986,-32768,-32768,   812,-32768,
   228,   812,   855,-32768,   986,-32768,-32768,-32768,-32768,-32768,
   986,-32768,   986,-32768,-32768,   753,-32768,-32768,   920,-32768,
-32768,   222,-32768
};

static const short yypgoto[] = {-32768,
-32768,    26,-32768,   116,-32768,-32768,    30,-32768,-32768,-32768,
    41,   123,-32768,-32768,  -156,   -27,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,    77,-32768,-32768,    -1,   234,-32768,-32768,
-32768,-32768
};


#define	YYLAST		1046


static const short yytable[] = {    19,
    94,   188,   128,   198,   112,    53,   106,    89,    56,    58,
    60,     1,    65,    67,   120,   210,     3,     4,     5,     6,
   121,     8,   175,   186,   116,   -29,     9,   -60,   169,    51,
   216,    50,   176,   217,   117,    52,   113,    10,    11,   114,
   172,   -60,    12,    13,   199,    90,    91,   129,   189,   170,
   -38,    54,   127,   115,   173,    50,   211,   -29,    14,   109,
    15,   122,   162,   177,    48,  -154,   134,   135,   163,   137,
   139,   141,   143,   145,   147,   149,   151,   153,   155,   157,
   159,   161,   131,   182,    71,    72,    73,    74,    75,    76,
    77,    78,    79,    80,    81,    82,    83,    71,    72,    73,
    74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
   219,   118,   167,   185,   197,   191,    81,    82,    83,    95,
    68,   119,   168,    49,   220,    96,    25,    69,    65,   132,
    71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
    81,    82,    83,    30,    97,    84,   100,   102,   104,   106,
    87,    55,    88,     3,     4,     5,     6,     7,     8,    92,
    98,    99,   192,     9,   165,    93,     8,    96,    25,   202,
   101,   204,   206,   111,    10,    11,    96,    25,   171,    12,
    13,   105,   166,   182,   215,    30,   179,    93,     8,   174,
    61,    62,    93,     8,    30,    14,   178,    15,   187,    57,
   200,     3,     4,     5,     6,     7,     8,   221,     6,    85,
   223,     9,   209,    59,   163,     3,     4,     5,     6,     7,
     8,   233,    10,    11,   229,     9,   207,    12,    13,   212,
    82,    83,    93,     8,    93,     8,    10,    11,   225,   218,
   222,    12,    13,    14,    63,    15,     3,     4,     5,     6,
     7,     8,   194,   208,    86,     0,     9,    14,    66,    15,
     3,     4,     5,     6,     7,     8,     0,    10,    11,     0,
     9,     0,    12,    13,     0,     0,     0,     0,     0,     0,
     0,    10,    11,     0,     0,     0,    12,    13,    14,   126,
    15,     3,     4,     5,     6,     7,     8,     0,     0,     0,
     0,     9,    14,   133,    15,     3,     4,     5,     6,     7,
     8,     0,    10,    11,     0,     9,     0,    12,    13,     0,
     0,     0,     0,     0,     0,     0,    10,    11,     0,     0,
     0,    12,    13,    14,   136,    15,     3,     4,     5,     6,
     7,     8,     0,     0,     0,     0,     9,    14,   138,    15,
     3,     4,     5,     6,     7,     8,     0,    10,    11,     0,
     9,     0,    12,    13,     0,     0,     0,     0,     0,     0,
     0,    10,    11,     0,     0,     0,    12,    13,    14,   140,
    15,     3,     4,     5,     6,     7,     8,     0,     0,     0,
     0,     9,    14,   142,    15,     3,     4,     5,     6,     7,
     8,     0,    10,    11,     0,     9,     0,    12,    13,     0,
     0,     0,     0,     0,     0,     0,    10,    11,     0,     0,
     0,    12,    13,    14,   144,    15,     3,     4,     5,     6,
     7,     8,     0,     0,     0,     0,     9,    14,   146,    15,
     3,     4,     5,     6,     7,     8,     0,    10,    11,     0,
     9,     0,    12,    13,     0,     0,     0,     0,     0,     0,
     0,    10,    11,     0,     0,     0,    12,    13,    14,   148,
    15,     3,     4,     5,     6,     7,     8,     0,     0,     0,
     0,     9,    14,   150,    15,     3,     4,     5,     6,     7,
     8,     0,    10,    11,     0,     9,     0,    12,    13,     0,
     0,     0,     0,     0,     0,     0,    10,    11,     0,     0,
     0,    12,    13,    14,   152,    15,     3,     4,     5,     6,
     7,     8,     0,     0,     0,     0,     9,    14,   154,    15,
     3,     4,     5,     6,     7,     8,     0,    10,    11,     0,
     9,     0,    12,    13,     0,     0,     0,     0,     0,     0,
     0,    10,    11,     0,     0,     0,    12,    13,    14,   156,
    15,     3,     4,     5,     6,     7,     8,     0,     0,     0,
     0,     9,    14,   158,    15,     3,     4,     5,     6,     7,
     8,     0,    10,    11,     0,     9,     0,    12,    13,     0,
     0,     0,     0,     0,     0,     0,    10,    11,     0,     0,
     0,    12,    13,    14,   160,    15,     3,     4,     5,     6,
     7,     8,     0,     0,     0,     0,     9,    14,   184,    15,
     3,     4,     5,     6,     7,     8,     0,    10,    11,     0,
     9,     0,    12,    13,     0,     0,     0,     0,     0,     0,
     0,    10,    11,     0,     0,     0,    12,    13,    14,   193,
    15,     3,     4,     5,     6,     7,     8,     0,     0,     0,
     0,     9,    14,   201,    15,     3,     4,     5,     6,     7,
     8,     0,    10,    11,     0,     9,     0,    12,    13,     0,
     0,     0,     0,     0,     0,     0,    10,    11,     0,     0,
     0,    12,    13,    14,   203,    15,     3,     4,     5,     6,
     7,     8,     0,     0,     0,     0,     9,    14,   205,    15,
     3,     4,     5,     6,     7,     8,     0,    10,    11,     0,
     9,     0,    12,    13,     0,     0,     0,     0,     0,     0,
     0,    10,    11,     0,     0,     0,    12,    13,    14,   214,
    15,     3,     4,     5,     6,     7,     8,     0,     0,     0,
     0,     9,    14,   227,    15,     3,     4,     5,     6,     7,
     8,     0,    10,    11,   103,     9,     0,    12,    13,     0,
    96,    25,     0,     0,     0,     0,    10,    11,     0,     0,
     0,    12,    13,    14,     0,    15,   195,     0,    30,     0,
     0,     0,     0,     0,     0,     0,     0,    14,     0,    15,
     0,    71,    72,    73,    74,    75,    76,    77,    78,    79,
    80,    81,    82,    83,     3,     4,     5,     6,     7,     8,
     0,     0,     0,     0,     9,     0,     0,     0,     0,     0,
     0,   196,     0,     0,     0,    10,    11,     0,     0,     0,
    12,    13,     3,     4,     5,     6,   121,     8,     0,     0,
     0,     0,     9,     0,     0,    23,    14,     0,    15,     0,
     0,    24,    25,    10,    11,    27,    28,     0,    12,    13,
     0,     0,     0,     0,    29,     0,     0,     0,     0,    30,
     0,     0,     0,     0,    14,     0,    15,    31,    32,    33,
    34,    35,   232,    23,     0,     0,     0,     0,   224,    24,
    25,     0,    26,    27,    28,     0,     0,     0,     0,     0,
     0,     0,    29,     0,     0,     0,     0,    30,     0,     0,
   230,     0,     0,     0,     0,    31,    32,    33,    34,    35,
   231,     0,     0,     0,     0,    71,    72,    73,    74,    75,
    76,    77,    78,    79,    80,    81,    82,    83,    71,    72,
    73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
    83,    71,    72,    73,    74,    75,    76,    77,    78,    79,
    80,    81,    82,    83,   130,     0,     0,     0,     0,     0,
     0,     0,     0,    70,     0,     0,     0,   190,    71,    72,
    73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
    83,    71,    72,    73,    74,    75,    76,    77,    78,    79,
    80,    81,    82,    83,    72,    73,    74,    75,    76,    77,
    78,    79,    80,    81,    82,    83,    73,    74,    75,    76,
    77,    78,    79,    80,    81,    82,    83,    75,    76,    77,
    78,    79,    80,    81,    82,    83
};

static const short yycheck[] = {     1,
    28,     1,     1,     1,    11,     1,    34,     1,    10,    11,
    12,    38,    14,    15,     1,     1,     3,     4,     5,     6,
     7,     8,     1,    15,     1,    11,    13,     1,     1,     1,
   187,    47,    11,   190,    11,     7,    43,    24,    25,     1,
     1,    15,    29,    30,    42,    39,    40,    46,    48,    22,
    48,    47,    54,    15,    15,    47,    42,    43,    45,     1,
    47,    48,     1,    42,    30,     7,    68,    69,     7,    71,
    72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
    82,    83,     1,   111,    16,    17,    18,    19,    20,    21,
    22,    23,    24,    25,    26,    27,    28,    16,    17,    18,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
     1,     1,     1,   115,    46,     1,    26,    27,    28,     1,
    45,    11,    11,    30,    15,     7,     8,    45,   130,    48,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    25,    29,     6,    31,    32,    33,   177,
    47,     1,     7,     3,     4,     5,     6,     7,     8,     1,
     7,     1,    48,    13,     9,     7,     8,     7,     8,   171,
     1,   173,   174,    14,    24,    25,     7,     8,    15,    29,
    30,     1,     9,   211,   186,    25,     1,     7,     8,    15,
     4,     5,     7,     8,    25,    45,     7,    47,    42,     1,
    48,     3,     4,     5,     6,     7,     8,   209,     6,     7,
   212,    13,    15,     1,     7,     3,     4,     5,     6,     7,
     8,     0,    24,    25,   226,    13,     1,    29,    30,    41,
    27,    28,     7,     8,     7,     8,    24,    25,   213,   199,
   211,    29,    30,    45,     1,    47,     3,     4,     5,     6,
     7,     8,   130,   177,    21,    -1,    13,    45,     1,    47,
     3,     4,     5,     6,     7,     8,    -1,    24,    25,    -1,
    13,    -1,    29,    30,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    24,    25,    -1,    -1,    -1,    29,    30,    45,     1,
    47,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
    -1,    13,    45,     1,    47,     3,     4,     5,     6,     7,
     8,    -1,    24,    25,    -1,    13,    -1,    29,    30,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    -1,    -1,
    -1,    29,    30,    45,     1,    47,     3,     4,     5,     6,
     7,     8,    -1,    -1,    -1,    -1,    13,    45,     1,    47,
     3,     4,     5,     6,     7,     8,    -1,    24,    25,    -1,
    13,    -1,    29,    30,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    24,    25,    -1,    -1,    -1,    29,    30,    45,     1,
    47,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
    -1,    13,    45,     1,    47,     3,     4,     5,     6,     7,
     8,    -1,    24,    25,    -1,    13,    -1,    29,    30,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    -1,    -1,
    -1,    29,    30,    45,     1,    47,     3,     4,     5,     6,
     7,     8,    -1,    -1,    -1,    -1,    13,    45,     1,    47,
     3,     4,     5,     6,     7,     8,    -1,    24,    25,    -1,
    13,    -1,    29,    30,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    24,    25,    -1,    -1,    -1,    29,    30,    45,     1,
    47,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
    -1,    13,    45,     1,    47,     3,     4,     5,     6,     7,
     8,    -1,    24,    25,    -1,    13,    -1,    29,    30,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    -1,    -1,
    -1,    29,    30,    45,     1,    47,     3,     4,     5,     6,
     7,     8,    -1,    -1,    -1,    -1,    13,    45,     1,    47,
     3,     4,     5,     6,     7,     8,    -1,    24,    25,    -1,
    13,    -1,    29,    30,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    24,    25,    -1,    -1,    -1,    29,    30,    45,     1,
    47,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
    -1,    13,    45,     1,    47,     3,     4,     5,     6,     7,
     8,    -1,    24,    25,    -1,    13,    -1,    29,    30,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    -1,    -1,
    -1,    29,    30,    45,     1,    47,     3,     4,     5,     6,
     7,     8,    -1,    -1,    -1,    -1,    13,    45,     1,    47,
     3,     4,     5,     6,     7,     8,    -1,    24,    25,    -1,
    13,    -1,    29,    30,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    24,    25,    -1,    -1,    -1,    29,    30,    45,     1,
    47,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
    -1,    13,    45,     1,    47,     3,     4,     5,     6,     7,
     8,    -1,    24,    25,    -1,    13,    -1,    29,    30,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    -1,    -1,
    -1,    29,    30,    45,     1,    47,     3,     4,     5,     6,
     7,     8,    -1,    -1,    -1,    -1,    13,    45,     1,    47,
     3,     4,     5,     6,     7,     8,    -1,    24,    25,    -1,
    13,    -1,    29,    30,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    24,    25,    -1,    -1,    -1,    29,    30,    45,     1,
    47,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
    -1,    13,    45,     1,    47,     3,     4,     5,     6,     7,
     8,    -1,    24,    25,     1,    13,    -1,    29,    30,    -1,
     7,     8,    -1,    -1,    -1,    -1,    24,    25,    -1,    -1,
    -1,    29,    30,    45,    -1,    47,     1,    -1,    25,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    47,
    -1,    16,    17,    18,    19,    20,    21,    22,    23,    24,
    25,    26,    27,    28,     3,     4,     5,     6,     7,     8,
    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,
    -1,    46,    -1,    -1,    -1,    24,    25,    -1,    -1,    -1,
    29,    30,     3,     4,     5,     6,     7,     8,    -1,    -1,
    -1,    -1,    13,    -1,    -1,     1,    45,    -1,    47,    -1,
    -1,     7,     8,    24,    25,    11,    12,    -1,    29,    30,
    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    25,
    -1,    -1,    -1,    -1,    45,    -1,    47,    33,    34,    35,
    36,    37,     0,     1,    -1,    -1,    -1,    -1,    44,     7,
     8,    -1,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
     1,    -1,    -1,    -1,    -1,    33,    34,    35,    36,    37,
    11,    -1,    -1,    -1,    -1,    16,    17,    18,    19,    20,
    21,    22,    23,    24,    25,    26,    27,    28,    16,    17,
    18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
    28,    16,    17,    18,    19,    20,    21,    22,    23,    24,
    25,    26,    27,    28,    42,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    11,    -1,    -1,    -1,    42,    16,    17,
    18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
    28,    16,    17,    18,    19,    20,    21,    22,    23,    24,
    25,    26,    27,    28,    17,    18,    19,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    18,    19,    20,    21,
    22,    23,    24,    25,    26,    27,    28,    20,    21,    22,
    23,    24,    25,    26,    27,    28
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */ 
/*#line 3 "bison.simple"*/

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifdef IPDVMODULE /* compiled using Vienna Make Utility 2.0 */
#include "ipdstring.h"
#else
#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */
#endif /* not using the Vienna Make Utility 2.0 */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYipdDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

/*#line 200 "bison.simple"*/

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */
  int need_ipdvAllocaFree = 0;	/*  1 if yyss, yyvs and yyls are allocated using ipdvAlloca() */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYipdDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (need_ipdvAllocaFree)
	  {
	     ipdvAllocaFree(yyss); 
	     ipdvAllocaFree(yyvs); 
#ifdef YYLSP_NEEDED
	     ipdvAllocaFree(yyls);
#endif
	  }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) ipdvAlloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) ipdvAlloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) ipdvAlloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
      need_ipdvAllocaFree = 1;
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYipdDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
      {
	 if (need_ipdvAllocaFree)
	 {
	    ipdvAllocaFree(yyss); 
	    ipdvAllocaFree(yyvs); 
#ifdef YYLSP_NEEDED
	    ipdvAllocaFree(yyls);
#endif
	 }
	YYABORT;
      }
    }

#if YYipdDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYipdDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYipdDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYipdDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
  {
     if (need_ipdvAllocaFree)
     {
	ipdvAllocaFree(yyss); 
	ipdvAllocaFree(yyvs); 
#ifdef YYLSP_NEEDED
	ipdvAllocaFree(yyls);
#endif
     }
     YYACCEPT;
  }

  /* Shift the lookahead token.  */

#if YYipdDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYipdDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 4:
/*#line 286 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ipdTreeNode_t *tn;

       /* In-Out statements */
       ipdResetErrorStatus();
       if (ipdWarnOnLoseingStmt)
	  ipdSetError(ipdWARN_USELESS_STMT);
       ipdStdLog("\nIPD-In [%ld] = ", ipdBase->InOut);
       ipdDumpRec(ipdPrivate.logFile, yyvsp[-1].tn, ipdFALSE, ipdFALSE, (ipdPrintf_f)ipdLogPrintf);
       ipdStdLog("\n");
       if ((tn = ipdDoCalculation(yyvsp[-1].tn, "~")))
       {
	  ipdStdLog("IPD-Out[%ld] = ", ipdBase->InOut);
	  ipdDumpRec(ipdPrivate.logFile, tn, ipdFALSE, ipdTRUE, (ipdPrintf_f)ipdLogPrintf);
	  ipdStdLog("\n");
       }
       ipdBase->InOut++;
       iiFreeTreeNodeRec(yyvsp[-1].tn, ipdFALSE);
    ;
    break;}
case 7:
/*#line 320 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ipdTreeNode_t *tn;

       if ((yyvsp[-1].tn) &&
	   (tn = iiSave(yyvsp[-1].tn, ipdFALSE, ipdLineNo, iiGetScannedFile())))
	  currentComment = &tn->comment;
       else
       {
	  if (!ipdGetErrorStatus())
	     ipdSetError(ipdERR_PARSE);
	  if (ipdTolerantParsing)
	     ipdResetErrorStatus();
	  else
	     IPD_ABORT_PARSING;
       }
    ;
    break;}
case 8:
/*#line 337 "/home/markus/vproject/ipd/ipdparse.y"*/
{ /* error handling */
       if ((yyvsp[-1].tn))
	  iiFreeTreeNodeRec(yyvsp[-1].tn, ipdFALSE);
       ipdSetError(ipdERR_MISSING_SEMICOLON);
       IPD_ABORT_PARSING;
    ;
    break;}
case 9:
/*#line 344 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ipdTreeNode_t *tn;

       if ((yyvsp[-1].tn) &&
	   (tn = iiSave(yyvsp[-1].tn, ipdFALSE, ipdLineNo, iiGetScannedFile())))
	  currentComment = &tn->comment;
       else
       {
	  if (!ipdGetErrorStatus())
	     ipdSetError(ipdERR_PARSE);
	  if (ipdTolerantParsing)
	     ipdResetErrorStatus();
	  else
	     IPD_ABORT_PARSING;
       }
    ;
    break;}
case 10:
/*#line 361 "/home/markus/vproject/ipd/ipdparse.y"*/
{ /* error handling */
       if ((yyvsp[-1].tn))
	  iiFreeTreeNodeRec(yyvsp[-1].tn, ipdFALSE);
       ipdSetError(ipdERR_MISSING_SEMICOLON);
       IPD_ABORT_PARSING;
    ;
    break;}
case 16:
/*#line 373 "/home/markus/vproject/ipd/ipdparse.y"*/
{
     ERRP(printf("command----------------error\n"));
     if (!ipdGetErrorStatus())
	ipdSetError(ipdERR_PARSE);
     if (ipdTolerantParsing)
	ipdResetErrorStatus();
     else
	IPD_ABORT_PARSING;
  ;
    break;}
case 17:
/*#line 387 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       long len = strlen(yyvsp[0].s) - 1;

       ERRP(printf("include_statement----------------#include \" INCLUDE_FILENAME\n"));
       if (yyvsp[0].s[len] == '\"')
	  yyvsp[0].s[len] = '\0';
       else
       {
	  ipdSetError(ipdERR_ILLEGAL_INCLUDE);
	  IPD_ABORT_PARSING;
       }
       includeFile(ipdFALSE, yyvsp[0].s);
    ;
    break;}
case 18:
/*#line 401 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       long len = strlen(yyvsp[0].s) - 1;

       ERRP(printf("include_statement----------------#include < INCLUDE_FILENAME\n"));
       if (yyvsp[0].s[len] == '>')
	  yyvsp[0].s[len] = '\0';
       else
       {
	  ipdSetError(ipdERR_ILLEGAL_INCLUDE);
	  IPD_ABORT_PARSING;
       }
       includeFile(ipdTRUE, yyvsp[0].s);
    ;
    break;}
case 19:
/*#line 415 "/home/markus/vproject/ipd/ipdparse.y"*/
{ /* error handling */
       ipdSetError(ipdERR_ILLEGAL_INCLUDE);
       IPD_ABORT_PARSING;
    ;
    break;}
case 20:
/*#line 424 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ERRP(printf("add_ID----------------ID\n"));
       yyval.p.s = yyvsp[0].s;
       yyval.p.a = 0;    /* No '+' */
    ;
    break;}
case 21:
/*#line 430 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ERRP(printf("add_ID---------------- + ID\n"));
       yyval.p.s = yyvsp[0].s;
       yyval.p.a = 1;    /* '+' */
    ;
    break;}
case 22:
/*#line 436 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ERRP(printf("add_ID----------------PATH ID\n"));

       yyval.p.s = iiStrAppend(yyvsp[-1].s,yyvsp[0].s);
       ipdMBfree(yyvsp[0].s);
       yyval.p.a = 0;
       if (!(ipdBase->bFlags & ipdBF_PARSE_FULLNAMES))
       {
	  ipdSetError(ipdERR_LVALUE_IS_FULLNAME, yyval.p.s);
	  ipdMBfree(yyval.p.s);
	  yyval.p.s = NULL;
       }
    ;
    break;}
case 23:
/*#line 454 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ERRP(printf("sectionSpec----------------add_ID\n"));
       if ((yyvsp[0].p.s) && (yyval.tn = iiCreateTreeNode(ipdSECTION)))
       {
	  yyval.tn->node.sv.fullname       = yyvsp[0].p.s;
	  if (yyvsp[0].p.a == 1)
	     yyval.tn->node.sv.flags      |= ipdSV_FORCE_APPEND;
       }
       else
       {
	  ipdMBfree(yyvsp[0].p.s);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 24:
/*#line 469 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ERRP(printf("sectionSpec---------------- < add_ID >\n"));
       if ((yyvsp[-1].p.s) && (yyval.tn = iiCreateTreeNode(ipdSECTION)))
       {
	  yyval.tn->node.sv.fullname        = yyvsp[-1].p.s;
	  if (yyvsp[-1].p.a == 1)
	     yyval.tn->node.sv.flags       |= ipdSV_FORCE_APPEND;
	  yyval.tn->node.sv.svu.sect.flags |= ipdSF_EXTENDABLE;
       }
       else
       {
	  ipdMBfree(yyvsp[-1].p.s);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 25:
/*#line 485 "/home/markus/vproject/ipd/ipdparse.y"*/
{ /* error handling */
       ERRP(printf("sectionSpec---------------- < add_ID error\n"));
       ipdMBfree(yyvsp[-1].p.s);
       ipdSetError(ipdERR_SECTION_NAME_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 26:
/*#line 492 "/home/markus/vproject/ipd/ipdparse.y"*/
{ /* error handling */
       ERRP(printf("sectionSpec---------------- < error\n"));
       ipdSetError(ipdERR_SECTION_NAME_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 27:
/*#line 502 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ERRP(printf("baseSection----------------path_id\n"));
       yyval.tn = NULL;
       if ((yyvsp[0].s) && (yyval.tn = iiCreateTreeNode(ipdSECTION_REF)))
       {
	  yyval.tn->node.sectRef.name     = yyvsp[0].s;
	  yyval.tn->node.sectRef.isActive = ipdTRUE;
       }
       else
       {
	  ipdMBfree(yyvsp[0].s);
	  iiFreeTreeNodeRec(yyval.tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 28:
/*#line 518 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ipdTreeNode_t *tn;
       ipdBoolean      isErr = ipdFALSE;

       ERRP(printf("baseSection----------------path_id ? expr\n"));
       yyval.tn = NULL;
       if ((yyvsp[-2].s) && (yyvsp[0].tn) && (yyval.tn = iiCreateTreeNode(ipdSECTION_REF)))
       {
	  yyval.tn->node.sectRef.name     = yyvsp[-2].s;
	  yyval.tn->node.sectRef.isActive = ipdTRUE;
	  yyval.tn->node.sectRef.condTree = yyvsp[0].tn;
	  yyvsp[-2].s = NULL;
	  yyvsp[0].tn = NULL;
	  if ((tn = ipdDoCalculation(yyval.tn->node.sectRef.condTree,iiGetCS()->node.sv.fullname)))
	  {
	     if (tn->type & ipdBOOLEAN)
	     {
		yyval.tn->node.sectRef.isActive = tn->node.expr.val.boolean;
	     }
	     else if (tn->type & ipdINTEGER)
	     {
		yyval.tn->node.sectRef.isActive = ((tn->node.expr.val.integer) ? ipdTRUE : ipdFALSE);
	     }
	     else
		isErr = ipdTRUE;
	  }
	  else
	     isErr = ipdTRUE;
       }
       else
	  isErr = ipdTRUE;

       if (isErr)
       {
	  ipdMBfree(yyvsp[-2].s);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyval.tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 29:
/*#line 563 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ERRP(printf("sectionList----------------baseSection\n"));
       yyval.tn = yyvsp[0].tn;
    ;
    break;}
case 30:
/*#line 568 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ERRP(printf("sectionList----------------baseSection , sectionList\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
       {
	  yyvsp[-2].tn->next = yyvsp[0].tn;
	  yyval.tn = yyvsp[-2].tn;
       }
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 31:
/*#line 583 "/home/markus/vproject/ipd/ipdparse.y"*/
{ /* error handling */
       ERRP(printf("sectionList----------------baseSection error\n"));
       iiFreeTreeNodeRec(yyvsp[-1].tn, ipdFALSE);
       yyval.tn = NULL;
    ;
    break;}
case 32:
/*#line 593 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ERRP(printf("sectionDeclaration----------------sectionSpec\n"));
       yyval.tn = yyvsp[0].tn;
    ;
    break;}
case 33:
/*#line 598 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ERRP(printf("sectionDeclaration----------------sectionSpec : sectionList\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
       {
	  yyvsp[-2].tn->node.sv.svu.sect.ancestor = yyvsp[0].tn;
	  yyval.tn = yyvsp[-2].tn;
       }
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 34:
/*#line 613 "/home/markus/vproject/ipd/ipdparse.y"*/
{ /* error handling */
       ERRP(printf("sectionDeclaration----------------sectionSpec : error\n"));
       iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
       ipdSetError(ipdERR_SECT_ANCESTORS_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 35:
/*#line 626 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ipdTreeNode_t *tn;

       ERRP(printf("section----------------sectionDeclaration ;\n"));
       if ((yyvsp[-1].tn) && (tn = iiSave(yyvsp[-1].tn, ipdFALSE, ipdLineNo, iiGetScannedFile()))) /* Save section */
	  currentComment = &(tn->comment);
       else
       {
	  if (!ipdGetErrorStatus())
	     ipdSetError(ipdERR_PARSE);
	  if (ipdTolerantParsing)
	     ipdResetErrorStatus();
	  else
	     IPD_ABORT_PARSING;
       }
    ;
    break;}
case 36:
/*#line 643 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ipdTreeNode_t *tn;

       ERRP(printf("section----------------sectionDeclaration {\n"));
       if ((yyvsp[-1].tn) && (tn = iiSave(yyvsp[-1].tn, ipdFALSE, ipdLineNo, iiGetScannedFile()))) /* Save section */
       {
	  currentComment = &(tn->comment);
	  iiSetCSHistory(tn);
       }
       else
       {
	  if (!ipdGetErrorStatus())
	     ipdSetError(ipdERR_PARSE);
	  if (ipdTolerantParsing)
	     ipdResetErrorStatus();
	  else
	     IPD_ABORT_PARSING;
       }
    ;
    break;}
case 37:
/*#line 663 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ipdTreeNode_t *tn;

       ERRP(printf("section----------------path_id { input }\n"));
       currentComment = NULL;
       if ((tn = iiReturnsub()))
	  tn->node.sv.svu.sect.flags |= ipdSF_CLOSED;
    ;
    break;}
case 38:
/*#line 676 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ERRP(printf("ID_list----------------ID(=\"%s\")\n", yyvsp[0].s));
       if ((yyvsp[0].s) &&
	   (yyval.tn = iiCreateTreeNode(ipdPARAMETER)))
       {
	  yyval.tn->node.parameter.name = yyvsp[0].s;
       }
       else
       {
	  ipdMBfree(yyvsp[0].s);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 39:
/*#line 690 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ERRP(printf("ID_list----------------ID_list , ID(=\"%s\")\n", yyvsp[0].tn));
       if ((yyvsp[-2].s) && (yyvsp[0].tn) &&
	   (yyval.tn = iiCreateTreeNode(ipdPARAMETER)))
       {
	  yyval.tn->node.parameter.name = yyvsp[-2].s;

	  yyval.tn->next = yyvsp[0].tn;
       }
       else
       {
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  ipdMBfree(yyvsp[-2].s);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 40:
/*#line 707 "/home/markus/vproject/ipd/ipdparse.y"*/
{ /* error handling */
	ERRP(printf("ID_list----------------ID_list , error\n"));
	ipdMBfree(yyvsp[-1].s);
	yyval.tn = NULL;
	ipdSetError(ipdERR_IDLIST_EXPECTED);
     ;
    break;}
case 41:
/*#line 718 "/home/markus/vproject/ipd/ipdparse.y"*/
{
       ERRP(printf("expr_sqz----------------expr\n"));
       yyval.tn = yyvsp[0].tn
    ;
    break;}
case 42:
/*#line 723 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr_sqz----------------expr , expr_sqz\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
       {
	  yyvsp[-2].tn->next = yyvsp[0].tn;
	  yyval.tn = yyvsp[-2].tn;
       }
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 43:
/*#line 738 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */ 
       ERRP(printf("expr_sqz----------------expr , error\n"));
       iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
       ipdSetError(ipdERR_EXPRESSION_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 44:
/*#line 749 "/home/markus/vproject/ipd/ipdparse.y" */
{
       yyval.tn = iiCreateArray(yyvsp[-1].tn);
    ;
    break;}
case 45:
/*#line 753 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */ 
       ERRP(printf("array---------------- [ expr_sqz error\n"));
       iiFreeTreeNodeRec(yyvsp[-1].tn, ipdFALSE);
       ipdSetError(ipdERR_EXPECT_ARRAY);
       yyval.tn = NULL;
    ;
    break;}
case 46:
/*#line 760 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */ 
       ERRP(printf("array---------------- [ error\n"));
       ipdSetError(ipdERR_EXPECT_ARRAY);
       yyval.tn = NULL;
    ;
    break;}
case 47:
/*#line 771 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("assoc_param_expr----------------ID = expr\n"));
       if ((yyvsp[-2].s) && (yyvsp[0].tn) && (yyval.tn = iiCreateTreeNode(ipdOPTION)))
       {
	  yyval.tn->node.option.tree  = yyvsp[0].tn;
	  yyval.tn->node.option.name  = yyvsp[-2].s;
       }
       else
       {
	  ipdMBfree(yyvsp[-2].s);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 48:
/*#line 786 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */ 
       ERRP(printf("assoc_param_expr----------------ID = error\n"));
       ipdSetError(ipdERR_EXPRESSION_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 49:
/*#line 796 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("parameter----------------expr\n"));
       yyval.tn = yyvsp[0].tn;
    ;
    break;}
case 50:
/*#line 801 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("parameter----------------assoc_param_expr\n"));
       yyval.tn = yyvsp[0].tn;
    ;
    break;}
case 51:
/*#line 806 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("parameter----------------param_expr , ext_param_expr\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
       { 
	  yyvsp[-2].tn->next = yyvsp[0].tn;
	  yyval.tn = yyvsp[-2].tn;
       }
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 52:
/*#line 821 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("parameter----------------assoc_param_expr , parameter\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
       {
	  if (yyvsp[0].tn->type == ipdOPTION)
	  { 
	     yyvsp[-2].tn->next = yyvsp[0].tn;
	     yyval.tn = yyvsp[-2].tn;
	  }
	  else
	  {
	     ipdSetError(ipdERR_OPTIONS_LAST);
	     iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	     iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	     yyval.tn = NULL;
	  }
       }
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 53:
/*#line 850 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("path_id----------------ID(=\"%s\")\n", yyvsp[0].s));
       yyval.s = yyvsp[0].s;
    ;
    break;}
case 54:
/*#line 855 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("path_id----------------PATH(=\"%s\") ID(=\"%s\")\n", yyvsp[-1].s, yyvsp[0].s));
       if ((yyvsp[-1].s) && (yyvsp[0].s))
	  yyval.s = iiStrAppend(yyvsp[-1].s,yyvsp[0].s);
       else
       {
	  ipdMBfree(yyvsp[-1].s); /* PATH */
	  yyval.s = NULL;
       }
       ipdMBfree(yyvsp[0].s); /* ID */
    ;
    break;}
case 55:
/*#line 867 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("path_id----------------PATH(=\"%s\") error\n", yyvsp[-1].s));
       ipdMBfree(yyvsp[-1].s);
       ipdSetError(ipdERR_PATH_NAME_EXPECTED);
       yyval.s = NULL;
    ;
    break;}
case 56:
/*#line 878 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* recall first time */
       ERRP(printf("arr_idx----------------path_id [ expr ]\n"));
       if ((yyval.tn = iiRecallByName(yyvsp[-3].s)))
	  yyval.tn = iiCreateArrayIndex(yyval.tn, yyvsp[-1].tn);
    ;
    break;}
case 57:
/*#line 884 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("arr_idx----------------arr_idx [ expr ]\n"));
       yyval.tn = iiCreateArrayIndex(yyvsp[-3].tn, yyvsp[-1].tn);
    ;
    break;}
case 58:
/*#line 889 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("arr_idx----------------path_id [ expr error\n"));
       ipdMBfree(yyvsp[-3].s);
       iiFreeTreeNodeRec(yyvsp[-1].tn, ipdFALSE);
       ipdSetError(ipdERR_SQUARE_BRACKET_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 59:
/*#line 897 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("arr_idx----------------path_id [ error\n"));
       ipdMBfree(yyvsp[-2].s);
       ipdSetError(ipdERR_EXPRESSION_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 60:
/*#line 908 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("key_word----------------ID\n"));
       yyval.p = yyvsp[0].p;
    ;
    break;}
case 61:
/*#line 913 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("key_word----------------key add_ID\n"));
       yyval.p = yyvsp[0].p;
    ;
    break;}
case 62:
/*#line 918 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("key_word----------------key error\n"));
       ipdSetError(ipdERR_VARIABLE_NAME_EXPECTED);
       yyval.p.s = NULL;
    ;
    break;}
case 63:
/*#line 930 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("stmt----------------key_word = expr\n"));
       if ((yyvsp[-2].p.s) && (yyvsp[0].tn) &&
	   (yyval.tn = iiCreateTreeNode(ipdVARIABLE)))
       {
	  yyval.tn->node.sv.fullname       = yyvsp[-2].p.s;
	  yyval.tn->node.sv.svu.var.tree   = yyvsp[0].tn;
	  if (yyvsp[-2].p.a == 1)
	     yyval.tn->node.sv.flags      |= ipdSV_FORCE_APPEND;
       }
       else
       {
	  ipdMBfree(yyvsp[-2].p.s);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 64:
/*#line 948 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("stmt----------------key_word = error\n"));
       ipdMBfree(yyvsp[-2].p.s);
       ipdSetError(ipdERR_EXPRESSION_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 65:
/*#line 955 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("stmt----------------key_word error\n"));
       ipdMBfree(yyvsp[-1].p.s);
       ipdSetError(ipdERR_EQUAL_OP_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 66:
/*#line 962 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("stmt----------------aux add_ID = expr\n"));
       if ((yyval.tn = iiCreateTreeNode(ipdVARIABLE)))
       {
	  yyval.tn->node.sv.fullname       = yyvsp[-2].p.s;
	  yyval.tn->node.sv.svu.var.tree   = yyvsp[0].tn;
	  yyval.tn->node.sv.svu.var.flags |= ipdVF_AUXILIARY;
	  if (yyvsp[-2].p.a == 1)
	     yyval.tn->node.sv.flags      |= ipdSV_FORCE_APPEND;
       }
       else
       {
	  ipdMBfree(yyvsp[-2].p.s);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 67:
/*#line 980 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("stmt----------------aux add_ID = error\n"));
       ipdMBfree(yyvsp[-2].p.s);
       ipdSetError(ipdERR_EXPRESSION_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 68:
/*#line 987 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("stmt----------------aux add_ID error\n"));
       ipdMBfree(yyvsp[-1].p.s);
       ipdSetError(ipdERR_EQUAL_OP_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 69:
/*#line 994 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("stmt----------------aux error\n"));
       ipdSetError(ipdERR_VARIABLE_NAME_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 70:
/*#line 1000 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("stmt----------------ext add_ID\n"));
       if ((yyval.tn                        = iiCreateTreeNode(ipdVARIABLE)))
       {
	  yyval.tn->node.sv.fullname       = yyvsp[0].p.s;
	  yyval.tn->node.sv.svu.var.flags |= ipdVF_EXTERN;
	  if (yyvsp[0].p.a == 1)
	     yyval.tn->node.sv.flags      |= ipdSV_FORCE_APPEND;
       }
       else
       {
	  ipdMBfree(yyvsp[0].p.s);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 71:
/*#line 1016 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("stmt----------------ext add_ID = expr\n"));
       if ((yyval.tn = iiCreateTreeNode(ipdVARIABLE)))
       {
	  yyval.tn->node.sv.fullname       = yyvsp[-2].p.s;
	  yyval.tn->node.sv.svu.var.tree   = yyvsp[0].tn;
	  yyval.tn->node.sv.svu.var.flags |= ipdVF_EXTERN;
	  if (yyvsp[-2].p.a == 1)
	     yyval.tn->node.sv.flags      |= ipdSV_FORCE_APPEND;
       }
       else
       {
	  ipdMBfree(yyvsp[-2].p.s);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 72:
/*#line 1034 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("stmt----------------ext add_ID = error\n"));
       ipdMBfree(yyvsp[-2].p.s);
       ipdSetError(ipdERR_EXPRESSION_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 73:
/*#line 1041 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("stmt----------------ext error\n"));
       ipdSetError(ipdERR_VARIABLE_NAME_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 74:
/*#line 1051 "/home/markus/vproject/ipd/ipdparse.y" */
{
       iiInternFunc_t *fn;

       ERRP(printf("function_expr----------------expr ;\n"));
       ipdParameterList = NULL;
       if ((fn = iiCreateInternFunction(yyvsp[-7].s, yyvsp[-5].tn, yyvsp[-1].tn)))
         currentComment = &fn->comment;
       else
       {
         if (!ipdGetErrorStatus())
	         ipdSetError(ipdERR_PARSE);
	      if (ipdTolerantParsing)
	         ipdResetErrorStatus();
	      else
	         IPD_ABORT_PARSING;
       }
    ;
    break;}
case 75:
/*#line 1069 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("function_expr----------------expr error\n"));
       ipdSetError(ipdERR_MISSING_SEMICOLON);
       ipdParameterList = NULL;
       iiFreeTreeNodeRec(yyvsp[-1].tn, ipdFALSE);
       iiFreeTreeNodeRec(yyvsp[-5].tn, ipdFALSE);
       ipdMBfree(yyvsp[-7].s);
       IPD_ABORT_PARSING;
    ;
    break;}
case 76:
/*#line 1079 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("function_expr----------------error;\n"));
       ipdSetError(ipdERR_EXPRESSION_EXPECTED);
       ipdParameterList = NULL;
       iiFreeTreeNodeRec(yyvsp[-4].tn, ipdFALSE);
       ipdMBfree(yyvsp[-6].s);
       IPD_ABORT_PARSING;
    ;
    break;}
case 77:
/*#line 1092 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("function_stmt----------------ID(=\"%s\") ( ID_list ) = \n", yyvsp[-4].s));
       ipdParameterList = yyvsp[-2].tn;
    ;
    break;}
case 79:
/*#line 1098 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("function_stmt----------------ID(=\"%s\") ( ID_list ) error\n", yyvsp[-4].s));
       ipdParameterList = NULL;
       ipdMBfree(yyvsp[-4].s);
       iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
       ipdSetError(ipdERR_EQUAL_OP_EXPECTED);
       IPD_ABORT_PARSING;
    ;
    break;}
case 80:
/*#line 1107 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("function_stmt----------------ID(=\"%s\") ( error\n", yyvsp[-2].s));
       ipdParameterList = NULL;
       ipdMBfree(yyvsp[-2].s);
       ipdSetError(ipdERR_IDLIST_EXPECTED);
       IPD_ABORT_PARSING;
    ;
    break;}
case 81:
/*#line 1119 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("del----------------del path_id ;\n"));
       iiDeleteVariable(yyvsp[-1].s);
       ipdMBfree(yyvsp[-1].s);
       if (ipdGetErrorStatus())
       {
	  if (ipdTolerantParsing)
	     ipdResetErrorStatus();
	  else
	     IPD_ABORT_PARSING;
       }
    ;
    break;}
case 82:
/*#line 1132 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("del----------------del path_id error\n"));
       ipdMBfree(yyvsp[-1].s);
       ipdSetError(ipdERR_MISSING_SEMICOLON);
       IPD_ABORT_PARSING;
    ;
    break;}
case 83:
/*#line 1139 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("del----------------del error\n"));
       ipdSetError(ipdERR_ILLEGAL_DELETE);
       IPD_ABORT_PARSING;
    ;
    break;}
case 84:
/*#line 1149 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("trace_item----------------path_id\n"));
       iiSetTraceVar(yyvsp[0].s, NULL);
    ;
    break;}
case 85:
/*#line 1154 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("trace_item----------------path_id = expr\n"));
       iiSetTraceVar(yyvsp[-2].s, yyvsp[0].tn);
    ;
    break;}
case 86:
/*#line 1159 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("trace_item----------------path_id = error\n"));
       ipdMBfree(yyvsp[-2].s);
       ipdSetError(ipdERR_TRACESTRING_EXPECTED);
       IPD_ABORT_PARSING;
    ;
    break;}
case 87:
/*#line 1170 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("trace_list----------------trace_item\n"));
    ;
    break;}
case 88:
/*#line 1174 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("trace_list----------------trace_list, trace_item\n"));
    ;
    break;}
case 89:
/*#line 1178 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("trace_list----------------trace_list, error\n"));
       ipdSetError(ipdERR_ILLEGAL_TRACE);
       IPD_ABORT_PARSING;
    ;
    break;}
case 90:
/*#line 1188 "/home/markus/vproject/ipd/ipdparse.y" */
{
    ;
    break;}
case 91:
/*#line 1191 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("trace----------------trace trace_list error\n"));
       ipdSetError(ipdERR_MISSING_SEMICOLON);
       IPD_ABORT_PARSING;
    ;
    break;}
case 92:
/*#line 1197 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("trace----------------trace error\n"));
       ipdSetError(ipdERR_ILLEGAL_TRACE);
       IPD_ABORT_PARSING;
    ;
    break;}
case 93:
/*#line 1209 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------eval ( expr )\n"));
       yyval.tn = iiEval(yyvsp[-1].tn);
    ;
    break;}
case 94:
/*#line 1214 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------eval ( expr error\n"));
       iiFreeTreeNodeRec(yyvsp[-1].tn, ipdFALSE);
       ipdSetError(ipdERR_CLOSING_PARENTH_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 95:
/*#line 1221 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------eval ( error\n"));
       ipdSetError(ipdERR_EXPRESSION_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 96:
/*#line 1227 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------eval error\n"));
       ipdSetError(ipdERR_OPENING_PARENTH_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 97:
/*#line 1233 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------ID(=\"%s\") ( )\n", yyvsp[-2].s));
       yyval.tn = iiGetFunction(yyvsp[-2].s, NULL);
       ipdMBfree(yyvsp[-2].s);
    ;
    break;}
case 98:
/*#line 1239 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------ID(=\"%s\") ( parameter )\n", yyvsp[-3].s));
       yyval.tn = iiGetFunction(yyvsp[-3].s, yyvsp[-1].tn);
       ipdMBfree(yyvsp[-3].s);
    ;
    break;}
case 99:
/*#line 1245 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------ID(=\"%s\") ( parameter error\n", yyvsp[-3].s));
       ipdMBfree(yyvsp[-3].s);
       iiFreeTreeNodeRec(yyvsp[-1].tn, ipdFALSE);
       ipdSetError(ipdERR_PARAMETERLIST_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 100:
/*#line 1253 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------ID(=\"%s\") ( error\n", yyvsp[-2].s));
       ipdMBfree(yyvsp[-2].s);
       ipdSetError(ipdERR_PARAMETERLIST_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 101:
/*#line 1260 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------const_value\n"));
       yyval.tn = yyvsp[0].tn;
    ;
    break;}
case 102:
/*#line 1265 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------path_id\n"));
       yyval.tn = iiRecallByName(yyvsp[0].s);
    ;
    break;}
case 103:
/*#line 1270 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------arr_idx\n"));
       yyval.tn = yyvsp[0].tn;
    ;
    break;}
case 104:
/*#line 1275 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------expr + expr\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
	  yyval.tn = iiCreateOperatorNode(yyvsp[-2].tn,   yyvsp[0].tn,   iiSum,          ipdFALSE);
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 105:
/*#line 1287 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------expr + error\n"));
       iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
       ipdSetError(ipdERR_BINARY_OPERATOR, "+");
       yyval.tn = NULL;
    ;
    break;}
case 106:
/*#line 1294 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------expr - expr\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
	  yyval.tn = iiCreateOperatorNode(yyvsp[-2].tn,   yyvsp[0].tn,   iiSum,          ipdTRUE); 
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 107:
/*#line 1306 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------expr - error\n"));
       iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
       ipdSetError(ipdERR_BINARY_OPERATOR, "-");
       yyval.tn = NULL;
    ;
    break;}
case 108:
/*#line 1313 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------expr * expr\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
	  yyval.tn = iiCreateOperatorNode(yyvsp[-2].tn,   yyvsp[0].tn,   iiProduct,      ipdFALSE);
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 109:
/*#line 1325 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------expr * error\n"));
       iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
       ipdSetError(ipdERR_BINARY_OPERATOR, "*");
       yyval.tn = NULL;
    ;
    break;}
case 110:
/*#line 1332 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------expr / expr\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
	  yyval.tn = iiCreateOperatorNode(yyvsp[-2].tn,   yyvsp[0].tn,   iiProduct,      ipdTRUE); 
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 111:
/*#line 1344 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------expr / error\n"));
       iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
       ipdSetError(ipdERR_BINARY_OPERATOR, "/");
       yyval.tn = NULL;
    ;
    break;}
case 112:
/*#line 1351 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------expr %% expr\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
	  yyval.tn = iiCreateOperatorNode(yyvsp[-2].tn,   yyvsp[0].tn,   iiModulus,       ipdFALSE); 
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 113:
/*#line 1363 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------expr %% error\n"));
       iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
       ipdSetError(ipdERR_BINARY_OPERATOR, "%%");
       yyval.tn = NULL;
    ;
    break;}
case 114:
/*#line 1370 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------expr EQ expr\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
	  yyval.tn = iiCreateOperatorNode(yyvsp[-2].tn,   yyvsp[0].tn,   iiEqual,        ipdFALSE);
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 115:
/*#line 1382 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------expr EQ error\n"));
       iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
       ipdSetError(ipdERR_BINARY_OPERATOR, "==");
       yyval.tn = NULL;
    ;
    break;}
case 116:
/*#line 1389 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------expr NE expr\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
	  yyval.tn = iiCreateOperatorNode(yyvsp[-2].tn,   yyvsp[0].tn,   iiNotEqual,     ipdFALSE);
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 117:
/*#line 1401 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------expr NE error\n"));
       iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
       ipdSetError(ipdERR_BINARY_OPERATOR, "!=");
       yyval.tn = NULL;
    ;
    break;}
case 118:
/*#line 1408 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------expr LT expr\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
	  yyval.tn = iiCreateOperatorNode(yyvsp[-2].tn,   yyvsp[0].tn,   iiLower,        ipdFALSE);
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 119:
/*#line 1420 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------expr LT error\n"));
       iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
       ipdSetError(ipdERR_BINARY_OPERATOR, "<");
       yyval.tn = NULL;
    ;
    break;}
case 120:
/*#line 1427 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------expr GT expr\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
	  yyval.tn = iiCreateOperatorNode(yyvsp[-2].tn,   yyvsp[0].tn,   iiGreater,      ipdFALSE);
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 121:
/*#line 1439 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------expr GT error\n"));
       iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
       ipdSetError(ipdERR_BINARY_OPERATOR, ">");
       yyval.tn = NULL;
    ;
    break;}
case 122:
/*#line 1446 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------expr LE expr\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
	  yyval.tn = iiCreateOperatorNode(yyvsp[-2].tn,   yyvsp[0].tn,   iiLowerEqual,   ipdFALSE);
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 123:
/*#line 1458 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------expr LE error\n"));
       iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
       ipdSetError(ipdERR_BINARY_OPERATOR, "<=");
       yyval.tn = NULL;
    ;
    break;}
case 124:
/*#line 1465 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------expr GE expr\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
	  yyval.tn = iiCreateOperatorNode(yyvsp[-2].tn,   yyvsp[0].tn,   iiGreaterEqual, ipdFALSE);
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 125:
/*#line 1477 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------expr GE error\n"));
       iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
       ipdSetError(ipdERR_BINARY_OPERATOR, ">=");
       yyval.tn = NULL;
    ;
    break;}
case 126:
/*#line 1484 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------expr LOGIC_OR expr\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
	  yyval.tn = iiCreateOperatorNode(yyvsp[-2].tn,   yyvsp[0].tn,   iiLogicOr,      ipdFALSE);
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 127:
/*#line 1496 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------expr LOGIC_OR error\n"));
       iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
       ipdSetError(ipdERR_BINARY_OPERATOR, "||");
       yyval.tn = NULL;
    ;
    break;}
case 128:
/*#line 1503 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------expr LOGIC_AND expr\n"));
       if ((yyvsp[-2].tn) && (yyvsp[0].tn))
	  yyval.tn = iiCreateOperatorNode(yyvsp[-2].tn,   yyvsp[0].tn,   iiLogicAnd,     ipdFALSE);
       else
       {
	  iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 129:
/*#line 1515 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------expr LOGIC_AND error\n"));
       iiFreeTreeNodeRec(yyvsp[-2].tn, ipdFALSE);
       ipdSetError(ipdERR_BINARY_OPERATOR, "&&");
       yyval.tn = NULL;
    ;
    break;}
case 130:
/*#line 1522 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------IPDNOT expr\n"));
       if ((yyvsp[0].tn))
	  yyval.tn = iiCreateOperatorNode(yyvsp[0].tn,   NULL, iiLogicNot,     ipdFALSE);
       else
	  yyval.tn = NULL;
    ;
    break;}
case 131:
/*#line 1530 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr----------------IPDNOT error\n"));
       ipdSetError(ipdERR_UNARY_OPERATOR, "!");
       yyval.tn = NULL;
    ;
    break;}
case 132:
/*#line 1542 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr---------------- - expr\n"));
       if ((yyvsp[0].tn))
       {
	  if ((yyvsp[0].tn->type == ipdINTEGER))
	  {
	     yyvsp[0].tn->node.expr.val.integer     = -yyvsp[0].tn->node.expr.val.integer;
	     yyval.tn = yyvsp[0].tn;
	  }
	  else if ((yyvsp[0].tn->type == ipdREAL)    || (yyvsp[0].tn->type == ipdREALQUANTITY))
	  {
	     yyvsp[0].tn->node.expr.val.quantity.re = -yyvsp[0].tn->node.expr.val.quantity.re;
	     yyval.tn = yyvsp[0].tn;
	  }
	  else if ((yyvsp[0].tn->type == ipdCOMPLEX) || (yyvsp[0].tn->type == ipdCOMPLEXQUANTITY))
	  {
	     yyvsp[0].tn->node.expr.val.quantity.re = -yyvsp[0].tn->node.expr.val.quantity.re;
	     yyvsp[0].tn->node.expr.val.quantity.im = -yyvsp[0].tn->node.expr.val.quantity.im;
	     yyval.tn = yyvsp[0].tn;
	  }
	  else
	  {
	     yyval.tn = iiCreateOperatorNode(NULL, yyvsp[0].tn,   iiSum,          ipdTRUE);
	  }
       }
       else
	  yyval.tn = NULL;
    ;
    break;}
case 133:
/*#line 1571 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr---------------- - error\n"));
       ipdSetError(ipdERR_UNARY_OPERATOR, "-");
       yyval.tn = NULL;
    ;
    break;}
case 134:
/*#line 1577 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr---------------- + expr\n"));
       yyval.tn = yyvsp[0].tn;
    ;
    break;}
case 135:
/*#line 1582 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr---------------- + error\n"));
       ipdSetError(ipdERR_UNARY_OPERATOR, "+");
       yyval.tn = NULL;
    ;
    break;}
case 136:
/*#line 1588 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr---------------- ( expr )\n"));
       yyval.tn = yyvsp[-1].tn;
    ;
    break;}
case 137:
/*#line 1593 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr---------------- ( expr error\n"));
       ipdSetError(ipdERR_CLOSING_PARENTH_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 138:
/*#line 1599 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("expr---------------- ( error\n"));
       ipdSetError(ipdERR_EXPRESSION_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
case 139:
/*#line 1605 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ERRP(printf("expr----------------array\n"));
       yyval.tn = yyvsp[0].tn;
    ;
    break;}
case 140:
/*#line 1616 "/home/markus/vproject/ipd/ipdparse.y" */
{
       yyval.s = yyvsp[0].s;
    ;
    break;}
case 141:
/*#line 1620 "/home/markus/vproject/ipd/ipdparse.y" */
{
       yyval.s = iiStrAppend(yyvsp[-1].s, yyvsp[0].s);
       ipdMBfree(yyvsp[0].s);
    ;
    break;}
case 142:
/*#line 1631 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* type: complex number */
       if ((yyval.tn = iiCreateTreeNode(ipdCOMPLEX)))
       {
	  yyval.tn->node.expr.val.quantity.re = 0.0;
	  yyval.tn->node.expr.val.quantity.im = 1.0;
       }
    ;
    break;}
case 143:
/*#line 1639 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* type: long    i;     */
       if ((yyval.tn = iiCreateTreeNode(ipdINTEGER)))
	  yyval.tn->node.expr.val.integer = yyvsp[0].i;
    ;
    break;}
case 144:
/*#line 1644 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* type: complex number */
       if ((yyval.tn = iiCreateTreeNode(ipdCOMPLEX)))
       {
	  yyval.tn->node.expr.val.quantity.re = 0.0;
	  yyval.tn->node.expr.val.quantity.im = (double)yyvsp[-1].i;
       }
    ;
    break;}
case 145:
/*#line 1652 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* type: complex number */
       if ((yyval.tn = iiCreateTreeNode(ipdCOMPLEX)))
       {
	  yyval.tn->node.expr.val.quantity.re = 0.0;
	  yyval.tn->node.expr.val.quantity.im = (double)yyvsp[0].i;
       }
    ;
    break;}
case 146:
/*#line 1660 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* type: double d; */
       if ((yyval.tn = iiCreateTreeNode(ipdREAL))) /* No imaginary part */
       {
	  yyval.tn->node.expr.val.quantity.re     = yyvsp[0].d;
	  yyval.tn->node.expr.val.quantity.im     = 0.0;
       }
    ;
    break;}
case 147:
/*#line 1668 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* type: complex number */
       if ((yyval.tn = iiCreateTreeNode(ipdCOMPLEX)))
       {
	  yyval.tn->node.expr.val.quantity.re = 0.0;
	  yyval.tn->node.expr.val.quantity.im = yyvsp[-1].d;
       }
    ;
    break;}
case 148:
/*#line 1676 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* type: complex number */
       if ((yyval.tn = iiCreateTreeNode(ipdCOMPLEX)))
       {
	  yyval.tn->node.expr.val.quantity.re = 0.0;
	  yyval.tn->node.expr.val.quantity.im = yyvsp[0].d;
       }
    ;
    break;}
case 149:
/*#line 1690 "/home/markus/vproject/ipd/ipdparse.y" */
{
       yyval.tn = yyvsp[0].tn;
    ;
    break;}
case 150:
/*#line 1694 "/home/markus/vproject/ipd/ipdparse.y" */
{
       if ((yyval.tn = yyvsp[-1].tn))
       {
	  if (yyval.tn->type & ipdINTEGER)
	  {
	     double x;

	     x = (double)yyval.tn->node.expr.val.integer;
	     yyval.tn->node.expr.val.quantity.re = x;
	     yyval.tn->node.expr.val.quantity.im = 0.0;
	     yyval.tn->type = ipdREAL;
	  }
#if IPD_USE_UNITS
	  if ((yyval.tn->node.expr.val.quantity.unit = iiUnitHashFindUnitStr(yyvsp[0].s)))
	  {
	     yyval.tn->type |= ipdQUANTITY;
	     yyval.tn->node.expr.val.quantity.re *= ((double)yyval.tn->node.expr.val.quantity.unit->unitRep.unitFactor);
	     yyval.tn->node.expr.val.quantity.im *= ((double)yyval.tn->node.expr.val.quantity.unit->unitRep.unitFactor);
	  }
	  else
	  {
	     iiFreeTreeNodeRec(yyval.tn, ipdFALSE);
	     yyval.tn = NULL;
	  }
#endif /* IPD_USE_UNITS */
       }
       ipdMBfree(yyvsp[0].s);
    ;
    break;}
case 151:
/*#line 1723 "/home/markus/vproject/ipd/ipdparse.y" */
{
       if ((yyval.tn = yyvsp[-1].tn))
       {
	  if (yyval.tn->type & ipdINTEGER)
	  {
	     double x;

	     x = (double)yyval.tn->node.expr.val.integer;
	     yyval.tn->node.expr.val.quantity.re = x;
	     yyval.tn->node.expr.val.quantity.im = 0.0;
	     yyval.tn->type = ipdREAL;
	  }
#if IPD_USE_UNITS
	  if ((yyval.tn->node.expr.val.quantity.unit = iiUnitHashFindUnitStr(yyvsp[0].s)))
	  {
	     yyval.tn->type |= ipdQUANTITY;
	     yyval.tn->node.expr.val.quantity.re *= ((double)yyval.tn->node.expr.val.quantity.unit->unitRep.unitFactor);
	     yyval.tn->node.expr.val.quantity.im *= ((double)yyval.tn->node.expr.val.quantity.unit->unitRep.unitFactor);
	  }
	  else
	  {
	     iiFreeTreeNodeRec(yyval.tn, ipdFALSE);
	     yyval.tn = NULL;
	  }
#endif /* IPD_USE_UNITS */
       }
       ipdMBfree(yyvsp[0].s);
    ;
    break;}
case 152:
/*#line 1752 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* type: char *s; */
       if ((yyval.tn = iiCreateTreeNode(ipdSTRING)))
	  yyval.tn->node.expr.val.string   = yyvsp[0].s;
    ;
    break;}
case 153:
/*#line 1757 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* type: bool b; */
       if ((yyval.tn = iiCreateTreeNode(ipdBOOLEAN)))
	  yyval.tn->node.expr.val.boolean  = yyvsp[0].b;
    ;
    break;}
case 154:
/*#line 1766 "/home/markus/vproject/ipd/ipdparse.y" */
{
       ipdAliasParsing = ipdTRUE;
    ;
    break;}
case 155:
/*#line 1770 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ipdTreeNode_t *tn;
       ipdBoolean      isErr = ipdFALSE;

       ERRP(printf("alias---------------- ( error\n"));

       yyval.tn = NULL;
       if ((yyvsp[-2].s) && (yyvsp[0].tn) && (yyval.tn = iiCreateTreeNode(ipdALIAS)))
       {
	  if ((tn = ipdDoCalculation(yyvsp[0].tn,iiGetCS()->node.sv.fullname)))
	  {
	     if (tn->type & ipdSECTION_REF)
	     {
		yyval.tn->node.sv.fullname           = yyvsp[-2].s;
		yyval.tn->node.sv.svu.alias.condTree = yyvsp[0].tn;
		yyval.tn->node.sv.svu.alias.sectRef  = tn;
	     }
	     else
	     {
		ipdSetError(ipdERR_ALIAS_VALUE);
		isErr = ipdTRUE;
	     }
	  }
	  else
	     isErr = ipdTRUE;
       }
       ipdAliasParsing = ipdFALSE;
       if (isErr)
       {
	  iiFreeTreeNodeRec(yyvsp[0].tn, ipdFALSE);
	  iiFreeTreeNodeRec(yyval.tn, ipdFALSE);
	  yyval.tn = NULL;
       }
    ;
    break;}
case 156:
/*#line 1805 "/home/markus/vproject/ipd/ipdparse.y" */
{ /* error handling */
       ERRP(printf("alias---------------- alias error\n"));
       ipdSetError(ipdERR_ALIAS_EXPECTED);
       yyval.tn = NULL;
    ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
/*#line 532 "bison.simple" */

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYipdDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  unsigned int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if ((unsigned int) yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if ((unsigned int)yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
      {
	 if (need_ipdvAllocaFree)
	 {
	    ipdvAllocaFree(yyss); 
	    ipdvAllocaFree(yyvs); 
#ifdef YYLSP_NEEDED
	    ipdvAllocaFree(yyls);
#endif
	 }
	YYABORT;
      }

#if YYipdDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) 
  {
     if (need_ipdvAllocaFree)
     {
	ipdvAllocaFree(yyss); 
	ipdvAllocaFree(yyvs); 
#ifdef YYLSP_NEEDED
	ipdvAllocaFree(yyls);
#endif
     }
     YYABORT;
  }
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYipdDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
  {
     if (need_ipdvAllocaFree)
     {
	ipdvAllocaFree(yyss); 
	ipdvAllocaFree(yyvs); 
#ifdef YYLSP_NEEDED
	ipdvAllocaFree(yyls);
#endif
     }
     YYACCEPT;
  }

#if YYipdDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
/*#line 1811 "/home/markus/vproject/ipd/ipdparse.y" */



/*=========================================================================
     Additional C code
=========================================================================*/

int doParse(void);
int doParse(void)
 {
/*  yydebug = 1; */
  return yyparse();
 } /* doParse */

#if 0
int ipdAbortParsing(void)
{
   fprintf(stderr, ipdERR_PREFIX " parsing aborted.\n");
   if (ipdTolerantParsing)
      return 0;
   else
      YYABORT;
} /* ipdAbortParsing */
#endif
