/*
 *
 */


#include "bcpl.h"
#include "parser.tab.h"

extern char	*yytext;

struct symbol *
make_symbol (char *sym)
{
  struct symbol	*p;

  p = (struct symbol *)calloc (sizeof (struct symbol), 1);
  if (p == NULL)
    {
      return (NULL);
    }

  p->type = TK_SYMBOL;
  p->hash = NULL;
  p->length = strlen (sym);
  p->value = strdup (sym);
  return (p);
}


struct number *
make_number (char *s, int base)
{
  struct number	*p;
  int		n;

  if (base == 10)
    n = atoi (s);
  else if (base == 8)
    sscanf (s, "%o", &n);
  else if (base == 16)
    sscanf (s, "%x", &n);

  p = (struct number *)calloc (sizeof (struct number), 1);
  if (p == NULL)
    {
      return (NULL);
    }
  p->type = TK_NUMBER;
  p->value = n;
  return (p);
}

struct string *
make_string (char *s)
{
  struct string	*p;

  p = (struct string *)calloc (sizeof (struct string), 1);
  if (p == NULL)
    {
      return (NULL);
    }

  p->type = TK_STRING;
  p->length = strlen (s);
  p->value = strdup (s);
  return (p);
}


struct node *
make_node (int type, void *a, void *b, void *c)
{
  struct node	*p;

  p = (struct node *)calloc (sizeof (struct node), 1);
  if (p == NULL)
    {
      return (NULL);
    }

  p->type = type;
  p->head = a;
  p->body = b;
  p->next = c;
  return (p);
}


static char	*tag_stack[MAX_TAG];
int		tag_stack_top = 0;

int
yylex ()
{
  int	token;
  static char	tag[MAX_STRING_LENGTH];
  char		tmptag[MAX_STRING_LENGTH];

  printf ("yylex\n");
start:  
  if (tag[0] != '\0')
    {
      if (pop_tag (tmptag) == -1)
	{
	  printf ("pop_tag error\n");
	  return (-1);
	}

      if (strcmp (tag, tmptag) == 0)
	{
	  tag[0] = '\0';
	}
      printf ("return TK_END\n");
      return (TK_END);
    }

  token = zzlex ();
  if (token == TK_BEGIN)
    {
      if (yytext[2] != '\0')
	{
	  printf ("PUSH: tag: \"$(%s\"\n", &yytext[2]); 
	  push_tag (&yytext[2]);
	}
    }
  else if (token == TK_END)
    {
      if (yytext[2] != '\0')
	{
	  if (check_tag (&yytext[2]) < 0)
	    {
	      /* error -- not found tag. */
	    }
	  else
	    {
	      strcpy (tag, &yytext[2]);
	      printf ("tag is found. \"%s\"\n", tag);
	      goto start;
	    }
	}
    }
  return (token);
}


int
check_tag (char *tag)
{
  int	i;

  for (i = 0; i < tag_stack_top; i++)
    {
      if (strcmp (tag, tag_stack[i]) == 0)
	{
	  return (i);
	}
    }
  return (-1);
}

int
push_tag (char *tag)	
{
  tag_stack[tag_stack_top] = strdup (tag);
  tag_stack_top++;
  return (tag_stack_top);
}


int
pop_tag (char tag[])
{
  if (tag_stack_top == 0)
    {
      return (-1);
    }

  strcpy (tag, tag_stack[tag_stack_top - 1]);
  free (tag_stack[tag_stack_top - 1]);
  tag_stack_top--;
  return (tag_stack_top);
}
