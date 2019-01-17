/*
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>


#define MAX_STRING_LENGTH	255
#define MAX_TAG			10

struct symbol
{
  int		type;
  struct symbol	*hash;
  int		length;
  char		*value;
};


struct string
{
  int		type;
  int		length;
  char		*value;
};


struct character
{
  int		type;
  int		value;
};


struct number
{
  int		type;
  int		value;
};


struct node
{
  int		type;
  void		*head;
  void		*body;
  void		*next;
};


#define OP_MANIFEST		1
#define OP_CONST		2
#define OP_DECLARE		3
#define OP_GLOBAL		4
#define OP_EXPR			5
#define OP_PLUSOP		6
#define OP_SUBOP		7
#define OP_FUNCTION		8
#define OP_ARGS			9
#define OP_STATEMENT		10
#define OP_PROCEDURE		11
#define OP_CALL			12
#define OP_VALOF		13
#define OP_RETURN		14
#define OP_VARIABLE		15
#define OP_CONST_LIST		16
#define OP_INITIALVAR		17
#define OP_IF			18
#define OP_WHILE		19
#define OP_SET			20	/* ÂåÆþ */
#define OP_STATIC		21
#define OP_MULTOP		22
#define OP_DIVOP		23
#define OP_LT			24
#define OP_GT			25
#define OP_OR			26
#define OP_AND			27
#define OP_EQUAL		28
#define OP_VECIS		29	/* ! */
#define OP_SWITCHON		30
#define OP_CASE			31
#define OP_GE			32
#define OP_LE			33
#define OP_REPEATUNTIL		34
#define OP_CASECONT		35
#define OP_GET			36
#define OP_BLOCK		37
#define OP_UNLESS		38


extern struct symbol	*make_symbol ();
extern struct string	*make_string ();
extern struct node	*make_node ();
extern struct number	*make_number ();

extern void		debug_print ();

extern int		nlcount;

extern int	        yyparse();
extern int		yyerror();
extern int	        yylex();
extern int		zzlex();
