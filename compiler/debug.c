/*
 *
 *
 */


#include "bcpl.h"
#include "parser.tab.h"


extern void	print_node ();
static void	print_symbol ();
static void	print_string ();
static void	print_number ();
static void	print_const ();
static void	print_declare ();
static void	print_func ();
static void	print_proc ();
static void	print_call ();
static void	print_expr ();
static void	print_args ();
static void	print_arglist ();
static void	print_statement ();
static void	print_return ();
static void	print_variable ();
static void	print_initialized_variable ();
static void	print_initial_list ();
static void	print_assign (struct node *nodep, int indent);
static void	print_if ();
static void	print_while ();
static void	print_switchon (struct node *nodep, int indent);
static void	print_case (struct node *nodep, int indent);
static void	print_repeatuntil (struct node *nodep, int indent);
static void	print_get (struct node *nodep, int indent);
static void	print_block (struct node *nodep, int indent);

static struct debug_entry	*get_entry ();


struct debug_entry
{
  int	type;
  char	*nicname;
  void (*f)();
};


struct debug_entry debug_table[] =
{
  { OP_DECLARE,		"declare",	print_declare	},
  { OP_MANIFEST,	"manifest",	print_node	},
  { OP_CONST,		"const",	print_const	},
  { OP_GLOBAL,		"global",	print_node	},
  { OP_STATIC,		"static",	print_node	},
  { OP_FUNCTION,	"func",		print_func      },
  { OP_PROCEDURE,	"proc",		print_proc      },
  { OP_ARGS,		"args",		print_arglist	},
  { OP_PLUSOP,		"+",		print_expr	},
  { OP_SUBOP,		"-",		print_expr	},
  { OP_MULTOP,		"*",		print_expr	},
  { OP_DIVOP,		"/",		print_expr	},
  { OP_GT,		"<",		print_expr	},
  { OP_LT,		">",		print_expr	},
  { OP_GE,		"<=",		print_expr	},
  { OP_LE,		">=",		print_expr	},
  { OP_AND,		"&",		print_expr	},
  { OP_OR,		"|",		print_expr	},
  { OP_EQUAL,		"=",		print_expr	},
  { OP_VECIS,		"!",		print_expr	},

  { OP_STATEMENT,	"-  ",		print_statement	},
  { OP_CALL,		"Call",		print_call	},
  { OP_RETURN,		"return",	print_return	},
  { OP_VALOF,		"valof",	print_node	},
  { OP_VARIABLE,	"var",		print_variable	},
  { OP_INITIALVAR,	"initvar",	print_initialized_variable },
  { OP_SET,		"assign",	print_assign },
  { OP_IF,		"if",		print_if },
  { OP_WHILE,		"whlie",	print_while },
  { OP_SWITCHON,	"switchon",	print_switchon },
  { OP_CASE,		"case",		print_case },
  { OP_CASECONT,	"case",		print_case },
  { OP_REPEATUNTIL,	"repeatuntil",	print_repeatuntil },
  { OP_GET,		"GET",		print_get },
  { OP_BLOCK,		"BLOCK",	print_block },

  { TK_SYMBOL,		"symbol",	print_symbol	},
  { TK_STRING,		"string",	print_string	},
  { TK_NUMBER,		"number",	print_number	},
  { 0,			NULL,		NULL		},
};


static struct debug_entry *
get_entry (nodep)
     struct node *nodep;
{
  int	i;

  if (nodep == NULL)
    return (NULL);

  for (i = 0; debug_table[i].type != 0; i++)
    {
      if (nodep->type == debug_table[i].type)
	{
	  return (&debug_table[i]);
	}
    }
  return (NULL);
}


static int
print_space (int n)
{
  int	i;

  for (i = 0; i < n; i++)
    {
      printf (" ");
    }
}


void
debug_print (struct node *nodep, int indent)
{
  struct debug_entry	*entp, *p;

  if (nodep == NULL)
    return;

  entp = get_entry (nodep);
  if (entp == NULL)
    return;

  (entp->f)(nodep, indent);
}


void
print_node (struct node *nodep, int indent)
{
  struct debug_entry	*entp, *p;

  if (nodep == NULL)
    return;

  entp = get_entry (nodep);
  print_space (indent);
  printf ("[%s]\n", entp->nicname);
  if (p = get_entry (nodep->head))
    {
      (p->f)(nodep->head, indent + 2);
    }

  if (p = get_entry (nodep->body))
    {
      (p->f)(nodep->body, indent + 2);
    }

  if (p = get_entry (nodep->next))
    {
      (p->f)(nodep->next, indent + 2);
    }
}


static void
print_symbol (struct symbol *symbolp, int indent)
{
  print_space (indent);
  printf ("<%s>", symbolp->value);
}


static void
print_string (struct string *strp, int indent)
{
  print_space (indent);
  printf ("<string>\"%s\"", strp->value);
}

static void
print_number (struct number *nump, int indent)
{
  print_space (indent);
  printf ("%d", nump->value);
}


static void
print_declare (struct node *nodep, int indent)
{
  struct symbol	*sp;
  struct number *np;

  sp = nodep->head;
  print_space (indent);
  printf ("[declare]\n");
  debug_print (nodep->head, indent + 10);
  if (nodep->next)
    {
      print_declare (nodep->next, indent);
    }
}

static void
print_const (struct node *nodep, int indent)
{
  struct symbol	*sp;
  struct number *np;

  sp = nodep->head;
  np = nodep->body;
  print_space (indent);
  printf ("[const]\t%s,%d\n", sp->value, np->value);
  if (nodep->next)
    {
      print_const (nodep->next, indent);
    }
}

static void
print_func (struct node *nodep, int indent)
{
  struct symbol	*sp;
  struct nodep	*argp;
  struct nodep  *exp;

  sp = nodep->head;
  argp = nodep->body;
  exp = nodep->next;

  print_space (indent);
  printf ("[function] %s\n", sp->value);
  debug_print ((void *)argp, indent + 2);
  debug_print ((void *)exp, indent + 2);
/*  printf ("\n"); ** */
}

static void
print_proc (struct node *nodep, int indent)
{
  struct symbol	*sp;
  struct nodep	*argp;
  struct nodep  *body;

  sp = nodep->head;
  argp = nodep->body;
  body = nodep->next;

  print_space (indent);
  printf ("[procedure] %s\n", sp->value);
  debug_print ((void *)argp, indent + 2);
  debug_print ((void *)body, indent + 2);
}


static void
print_call (struct node *nodep, int indent)
{
  struct symbol	*sp;
  struct node	*argp;

  sp = nodep->head;
  argp = nodep->body;

  print_space (indent);
  printf ("[call] %s(", sp->value);
  print_args ((void *)argp, indent + 2);
  printf (")");
}


static void
print_arglist (struct node *nodep, int indent)
{
  print_space (indent);
  printf ("[args] ");
  print_args (nodep, indent);
  printf ("\n");
}


static void
print_args (struct node *nodep, int indent)
{
  struct symbol	*sp;
  struct node	*argp;

  if (nodep == NULL)
    return;

  sp = nodep->head;
  argp = nodep->next;
  if (sp->type == TK_SYMBOL)
    {
      printf ("%s", sp->value);
    }
  else
    {
      debug_print ((struct node *)sp, 0);
    }

  if (argp)
    {
      printf (", ");
      print_args (argp, indent);
    }
}

static void
print_expr (struct node *nodep, int indent)
{
  struct debug_entry	*entp;
  struct node		*ex1, *ex2;

  ex1 = nodep->head;
  ex2 = nodep->body;
  entp = get_entry (nodep);

  print_space (indent);
  printf ("(%s", entp->nicname);
  if (ex1)
    {
      printf (" ");
      debug_print (ex1, 0);
    }
  if (ex2)
    {
      printf (" ");
      debug_print (ex2, 0);
    }
  printf (")");
}


static void
print_statement (struct node *nodep, int indent)
{
  struct debug_entry	*entp;
  struct node		*body, *next;

  body = nodep->head;
  next = nodep->next;
  entp = get_entry (nodep);

  print_space (indent);
  printf ("[S] ");
  if (body)
    {
      switch (body->type)
	{
	case OP_IF:
	case OP_WHILE:
	case OP_SWITCHON:
	case OP_REPEATUNTIL:
	case OP_BLOCK:
	  debug_print (body, indent);
	  break;

	default:
	  debug_print (body, 0);
	  printf ("\n");
	  break;
	}
    }
  if (next)
    {
      debug_print (next, indent);
    }
}


static void
print_variable (struct node *nodep, int indent)
{
  struct debug_entry	*entp;
  struct symbol		*body;
  struct node		*next;

  body = nodep->head;
  next = nodep->next;
  entp = get_entry (nodep);

  print_space (indent);
  printf ("[Var] ");
  if (body)
    {
      if (body->type == TK_SYMBOL)
	{
	  printf ("%s", body->value);
	}
      else
	{
	  debug_print ((struct node *)body, 0);
	}
    }
  if (next)
    {
      printf (", ");
      print_args (next, 0);
    }
}


static void
print_assign (struct node *nodep, int indent)
{
  struct node		*body, *head;

  head = nodep->head;
  body = nodep->body;
  print_space (indent);
  printf ("[assign] ");
  if (head)
    {
      debug_print (head, 0);
    }

  printf (" := ");

  if (body)
    {
      debug_print (body, 0);
    }
}


static void
print_return (struct node *nodep, int indent)
{
  struct node		*body;

  body = nodep->head;
  print_space (indent);
  printf ("[return] ");
  if (body)
    {
      debug_print (body, 0);
    }
}


static void
print_initialized_variable (struct node *nodep, int indent)
{
  struct node		*body, *next;

  body = nodep->head;
  next = nodep->body;

  print_space (indent);
  if (body)
    {
      print_variable (body, 0);
    }

  if (next)
    {
      printf (" = ");
      print_initial_list (next);
    }
}


static void
print_initial_list (struct node *nodep)
{
  struct number	*nump;

  if (nodep == NULL)
    return;

  nump = nodep->head;
  if (nump->type == TK_NUMBER)
    {
      printf ("%d", nump->value);
    }
  else
    {
      print_node ((struct node *)nump, 0);
    }

  if (nodep->next)
    {
      printf (", ");
      print_initial_list (nodep->next);
    }
}

static void
print_if (struct node *nodep, int indent)
{
  struct node	*head, *body;
  
  printf ("[IF]  ( ");
  head = nodep->head;
  body = nodep->body;
  debug_print ((struct node *)head, 0);
  printf (" ) \n");
  debug_print ((struct node *)body, indent + 4);
}


static void
print_while (struct node *nodep, int indent)
{
  struct node	*head, *body;
  
  printf ("[WHILE]  ( ");
  head = nodep->head;
  body = nodep->body;
  debug_print ((struct node *)head, 0);
  printf (" )\n");
  debug_print ((struct node *)body, indent + 4);
}


static void
print_switchon (struct node *nodep, int indent)
{
  struct node	*expr, *caselist;
  
  printf ("[SWITCHON] ( ");
  expr = nodep->head;
  caselist = nodep->body;

  debug_print (expr, 0);
  printf (" )\n");
  debug_print (caselist, indent + 4);
}


static void
print_case (struct node *nodep, int indent)
{
  print_space (indent);
  printf ("[CASE]  ( ");
  debug_print (nodep->head, 0);
  printf (" )\n");
  debug_print (nodep->body, indent + 8);
  if (nodep->next)
    {
      print_case (nodep->next, indent);
    }
}

static void
print_repeatuntil (struct node *nodep, int indent)
{
  struct node	*head, *body;
  
  printf ("[REEATUNTIL]  ( ");
  head = nodep->head;
  body = nodep->body;
  debug_print ((struct node *)body, 0);
  printf (" )\n");
  debug_print ((struct node *)head, indent + 4);
}

static void
print_get (struct node *nodep, int indent)
{
  print_space (indent);
  printf ("[GET] ");
  debug_print ((struct node *)nodep->body, 0);
  printf ("\n\n");
}

static void
print_block (struct node *nodep, int indent)
{
  printf ("[BLOCK]\n");
  debug_print ((struct node *)nodep->head, indent + 4);
  printf ("\n");
}


