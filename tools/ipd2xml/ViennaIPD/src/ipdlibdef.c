/*=======================================================================
Copyright 1992 -- 2009, Institute for Microelectronics, TU Vienna.

                                 ViennaIPD
                             -----------------

LICENSE:  see ViennaIPD/LICENSE

AUTHORS:  Members of the Institute for Microelectronics, TU Vienna

=======================================================================*/

/** @file ipdlibdef.c
    @brief This file contains functions usually found in the libs
*/

extern unsigned linenum[];
extern int include_stack_ptr;

#if defined(__cplusplus)
extern "C" {
#endif
int yywrap(void)
{
  return 1;
}
#if defined(__cplusplus)
}
#endif

#if 0
int yyerror (const char *msg)
{
  /*fprintf (stderr, "Line %d: %s \n%s\n", linenum, msg, linebuffer);*/
  printf ("Line %d: '%s'\n", linenum[include_stack_ptr], msg);
  /*  fprintf (stderr, "%*s\n", tokenpos, "^");*/
 return (-1);
}
#endif
