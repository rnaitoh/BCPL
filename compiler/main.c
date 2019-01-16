/*
 *
 */

#include "bcpl.h"

int	nlcount;


int
main (int ac, char **av)
{
  extern struct node	*noderoot;
  extern FILE		*yyin;

  nlcount = 1;

  /* option check... */
  if (ac < 2)
    {
      fprintf (stderr, "usage: %s files...\n", av[0]);
      exit (0);
    }

  /* input file setup... */
  yyin = fopen (av[1], "r");
  if (yyin == NULL)
    {
      fprintf (stderr, "Can not open source file (%s)\n", av[1]);
      exit (0);
    }

  if (yyparse () == 1)
    {
      printf ("syntax error: compiler stop.\n");
      exit (0);
    }

  debug_print (noderoot, 0);

  make_code ();
}


yyerror ()
{
  extern char *yytext;

  fprintf (stderr, "line %d: syntax error\n", nlcount);
  fprintf (stderr, "yytext: %s\n", yytext);
}

