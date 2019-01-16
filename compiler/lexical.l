%{

#include "bcpl.h"
#include "parser.tab.h"

char string_buf[MAX_STRING_LENGTH];
char *string_buf_ptr;

#define yylex	zzlex

%}

%x STRING

%%


[\t ]+				/* ignore */

\/\/.*\n			{ nlcount++; /* (comment) */ }

MANIFEST			{ return (TK_MANIFEST); }
GLOBAL				{ return (TK_GLOBAL); }
STATIC				{ return (TK_STATIC); }
LET				{ return (TK_LET); }
BE				{ return (TK_BE); }
RETURN				{ return (TK_RETURN); }
VALOF				{ return (TK_VALOF); }
WHILE				{ return (TK_WHILE); }
IF				{ return (TK_IF); }
RESULTIS			{ return (TK_RESULTIS); }
DO				{ return (TK_DO); }
THEN				{ return (TK_DO); }
SWITCHON			{ return (TK_SWITCHON); }
CASE				{ return (TK_CASE); }
CASEEND				{ return (TK_CASEEND); }
INTO				{ return (TK_INTO); }
REPEATUNTIL			{ return (TK_REPEATUNTIL); }
GET				{ return (TK_GET); }
AND				{ return (TK_AND); }

#[0-7]+				{ yylval.numberp = make_number (&yytext[1], 8); return (TK_NUMBER); }
#X[0-9A-Fa-f]+			{ yylval.numberp = make_number (&yytext[2], 16); return (TK_NUMBER); }
[0-9]+				{ yylval.numberp = make_number (yytext, 10); return (TK_NUMBER); }

","				{ return (TK_COMMA); }
"$("				{ return (TK_BEGIN); }
"$)"				{ return (TK_END); }
"$("[a-zA-Z]+			{ return (TK_BEGIN); }
"$)"[a-zA-Z]+			{ return (TK_END); }
":="				{ return (TK_ASSIGN); }
":"				{ return (TK_COLON); }
";"				{ return (TK_SEMICOLON); }
"="				{ return (TK_EQUAL); }
"*"				{ return (TK_MULTOP); }
"/"				{ return (TK_DIVOP); }
"+"				{ return (TK_PLUSOP); }
"-"				{ return (TK_SUBOP); }
">"				{ return (TK_LT); }
">="				{ return (TK_LE); }
"<"				{ return (TK_GT); }
"<="				{ return (TK_GE); }
"&"				{ return (TK_ANDOP); }
"|"				{ return (TK_OROP); }
"!"				{ return (TK_VECIS); }
")"				{ return (TK_RP); }
"("				{ return (TK_LP); }

\"				{ string_buf_ptr = string_buf; BEGIN(STRING); }


<STRING>\"			{ /* saw closing quote - all done */
		                   BEGIN(INITIAL);
		                   *string_buf_ptr = '\0';
		                   /* return string constant token type and
		                    * value to parser
		                    */
				   yylval.stringp = make_string (string_buf);
				   return (TK_STRING);
		                }

<STRING>\n        		{
                  		   /* error - unterminated string constant */
                   		   /* generate error message */
				   fprintf (stderr, "string unterminated.\n");
				   exit (0);
                   		}

<STRING>\\[0-7]{1,3} 		{
                   		/* octal escape sequence */
                   		int result;

                   		(void) sscanf( yytext + 1, "%o", &result );

                   		if ( result > 0xff )
				  {
					  fprintf (stderr, "range over for escape\n");
	                          	 /* error, constant is out-of-bounds */
				  }

	                   	*string_buf_ptr++ = result;
                   		}

<STRING>\\[0-9]+ 		{
                 		  /* generate error - bad escape sequence; something
                  		  * like '\48' or '\0777777'
                  		  */
                   		}

<STRING>"*N"  			{ *string_buf_ptr++ = '\n'; }
<STRING>"*T"  			{ *string_buf_ptr++ = '\t'; }
<STRING>"*R"  			{ *string_buf_ptr++ = '\r'; }
<STRING>"**"  			{ *string_buf_ptr++ = '*'; }

<STRING>"*n"	  		{ *string_buf_ptr++ = yytext[1]; }

<STRING>[^\\\n\"]+        	{
                 		  char *yptr = yytext;
                 		  while ( *yptr )
                   		        *string_buf_ptr++ = *yptr++;
                   		}


[a-zA-Z\.][a-zA-Z\.0-9]*	{ yylval.symbolp = make_symbol (yytext); return TK_SYMBOL; }


\n				{ nlcount++; }


%%
