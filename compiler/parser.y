/*
 * Copyright (C) 1999 Ryuichi Naitoh
 *
 *
 */

/* $Id* */
%{

#include "bcpl.h"

struct node	*noderoot;

%}

%union{
  struct symbol		*symbolp;
  struct string		*stringp;
  struct character	*character;
  struct node		*nodep;
  struct number		*numberp;
};


%token TK_BEGIN
%token TK_END
%token TK_EQUAL
%token TK_COMMA
%token TK_GLOBAL
%token TK_COLON
%token TK_MULTOP
%token TK_DIVOP
%token TK_PLUSOP
%token TK_SUBOP
%token TK_RP TK_LP
%token TK_SEMICOLON
%token TK_ASSIGN
%token TK_GT			/* < */
%token TK_LT			/* > */
%token TK_GE			/* <= */
%token TK_LE			/* >= */
%token TK_ANDOP			/* & */
%token TK_OROP			/* | */
%token TK_VECIS			/* ! */

/* 予約語 : 'TK_' を削った残りが予約語と等しい */
%token TK_MANIFEST
%token TK_AND
%token TK_LET
%token TK_VALOF
%token TK_RETURN
%token TK_BE
%token TK_IF
%token TK_WHILE
%token TK_DO
%token TK_UNLESS
%token TK_TEST
%token TK_UNTIL
%token TK_STATIC
%token TK_RESULTIS
%token TK_FOR
%token TK_SWITCHON
%token TK_CASE
%token TK_CASEEND
%token TK_INTO
%token TK_REPEATUNTIL
%token TK_GET

%token <symbolp>TK_SYMBOL
%token <stringp>TK_STRING
%token <numberp>TK_NUMBER

%type <nodep>program
%type <nodep>declare.list
%type <nodep>declare.manifest
%type <nodep>manifest.list
%type <nodep>declare.global
%type <nodep>global.list
%type <nodep>declare.function
%type <nodep>argument.list argument
%type <nodep>expression
%type <nodep>declare.procedure
%type <nodep>statement.list
%type <nodep>statement
%type <nodep>call.function
%type <nodep>expression.list
%type <nodep>declare.variable.list
%type <nodep>declare.variable
%type <nodep>declare.get
%type <nodep>initialize.list
%type <nodep>if.head if.statement
%type <nodep>while.statement
%type <nodep>declare.static
%type <nodep>switchon.statement
%type <nodep>case.statement.list
%type <nodep>repeatuntil.statement


%right TK_ASSIGN
%left TK_ANDOP TK_OROP
%left TK_PLUSOP TK_SUBOP
%left TK_MULTOP TK_DIVOP TK_GT TK_LT TK_GE TK_LE TK_EQUAL TK_VECIS
%left TK_NUMBER TK_SYMBOL


%start program

%%

program
	: declare.list				{ noderoot = $1; }

declare.list
	: /* empty */				{ $$ = NULL; }
	| declare.manifest declare.list		{ $$ = make_node (OP_DECLARE, $1, NULL, $2); }
	| declare.global declare.list		{ $$ = make_node (OP_DECLARE, $1, NULL, $2); }
	| declare.function declare.list		{ $$ = make_node (OP_DECLARE, $1, NULL, $2); }
	| declare.procedure declare.list	{ $$ = make_node (OP_DECLARE, $1, NULL, $2); }
	| declare.static declare.list		{ $$ = make_node (OP_DECLARE, $1, NULL, $2); }
	| declare.get declare.list		{ $$ = make_node (OP_DECLARE, $1, NULL, $2); }
	;

declare.manifest
	: TK_MANIFEST TK_BEGIN manifest.list TK_END
						{ $$ = make_node (OP_MANIFEST, $3, NULL, NULL);
						}

manifest.list
	: /* empty */				{ $$ = NULL; }
	| TK_SYMBOL TK_EQUAL TK_NUMBER 		{ $$ = make_node (OP_CONST, $1, $3, NULL); }
	;
	| TK_SYMBOL TK_EQUAL TK_NUMBER TK_COMMA manifest.list 
						{ $$ = make_node (OP_CONST, $1, $3, $5); }
	;

declare.global
	: TK_GLOBAL TK_BEGIN global.list TK_END	{ $$ = make_node (OP_GLOBAL, $3, NULL, NULL); }

global.list
	: /* empty */				{ $$ = NULL; }
	| TK_SYMBOL TK_COLON TK_NUMBER 		{ $$ = make_node (OP_CONST, $1, $3, NULL); }
	| TK_SYMBOL TK_COLON TK_NUMBER TK_COMMA global.list
						{ $$ = make_node (OP_CONST, $1, $3, $5); }
	;
	
declare.function
	: TK_LET TK_SYMBOL argument.list TK_EQUAL expression
						{ $$ = make_node (OP_FUNCTION, $2, $3, $5); }
	;

declare.procedure
	: TK_LET TK_SYMBOL argument.list TK_BE TK_BEGIN statement.list TK_END
						{ $$ = make_node (OP_PROCEDURE, $2, $3, $6); }
	| TK_AND TK_SYMBOL argument.list TK_BE TK_BEGIN statement.list TK_END
						{ $$ = make_node (OP_PROCEDURE, $2, $3, $6); }
	;

declare.static
	: TK_STATIC TK_BEGIN global.list TK_END
						{ $$ = make_node (OP_STATIC, $3, NULL, NULL); }
	;

declare.get
	: TK_GET TK_STRING			{ $$ = make_node (OP_GET, NULL, $2, NULL); }
	;

statement.list
	: /* empty */				{ $$ = NULL; }
	| statement statement.list		{ $$ = make_node (OP_STATEMENT, $1, NULL, $2); }
	| repeatuntil.statement statement.list  { $$ = make_node (OP_STATEMENT, $1, NULL, $2); }
	;

statement
	: expression TK_SEMICOLON		{ $$ = $1; }
	| declare.variable.list	TK_SEMICOLON	{ $$ = $1; }
	| TK_RETURN TK_LP expression TK_RP TK_SEMICOLON
						{ $$ = make_node (OP_RETURN, $3, NULL, NULL); }
	| TK_RESULTIS TK_LP expression TK_RP TK_SEMICOLON
						{ $$ = make_node (OP_RETURN, $3, NULL, NULL); }
	| if.statement				{ $$ = $1; }
	| while.statement			{ $$ = $1; }
	| switchon.statement			{ $$ = $1; }
	| TK_BEGIN statement.list TK_END	{ $$ = make_node (OP_BLOCK, $2, NULL, NULL); }
	;

if.statement
	: if.head statement			{ $$ = make_node (OP_IF, $1, $2, NULL); }
	| if.head TK_BEGIN statement.list TK_END
						{ $$ = make_node (OP_IF, $1, $3, NULL); }
	;

if.head
	: TK_IF expression TK_DO		{ $$ = $2; }
	;

while.statement
	: TK_WHILE expression TK_DO statement	{ $$ = make_node (OP_WHILE, $2, $4, NULL); }
	| TK_WHILE expression TK_DO TK_BEGIN statement.list TK_END
						{ $$ = make_node (OP_WHILE, $2, $5, NULL); }
	;

switchon.statement
	: TK_SWITCHON expression TK_INTO TK_BEGIN case.statement.list TK_END { $$ = make_node (OP_SWITCHON, $2, $5, NULL); }
	;

case.statement.list
	: TK_CASE expression TK_COLON statement.list 		 			 { $$ = make_node (OP_CASECONT, $2, $4, NULL); }
	| TK_CASE expression TK_COLON statement.list TK_CASEEND 			 { $$ = make_node (OP_CASE, $2, $4, NULL); }
	| TK_CASE expression TK_COLON statement.list TK_CASEEND case.statement.list { $$ = make_node (OP_CASE, $2, $4, $6); }
	;

repeatuntil.statement
	: TK_BEGIN statement.list TK_END TK_REPEATUNTIL expression { $$ = make_node (OP_REPEATUNTIL, $2, $5, NULL); }
	;

declare.variable.list
	: TK_LET declare.variable		{ $$ = $2; }
	| TK_LET declare.variable TK_EQUAL initialize.list
						{ $$ = make_node (OP_INITIALVAR, $2, $4, NULL); }
	;

declare.variable
	: TK_SYMBOL TK_COMMA declare.variable	{ $$ = make_node (OP_VARIABLE, $1, NULL, $3); }
	| TK_SYMBOL 				{ $$ = make_node (OP_VARIABLE, $1, NULL, NULL); }
	;

initialize.list
	: TK_NUMBER TK_COMMA initialize.list	{ $$ = make_node (OP_CONST_LIST, $1, NULL, $3); }
	| TK_NUMBER				{ $$ = make_node (OP_CONST_LIST, $1, NULL, NULL); }
	;


call.function
	: TK_SYMBOL TK_LP  TK_RP		{ $$ = make_node (OP_CALL, $1, NULL, NULL); }
	| TK_SYMBOL TK_LP expression.list TK_RP	{ $$ = make_node (OP_CALL, $1, $3, NULL); }
	;

argument.list
	: TK_LP argument TK_RP			{ $$ = $2; }
	| TK_LP  TK_RP				{ $$ = NULL; }
	;

argument
	: TK_SYMBOL TK_COMMA argument		{ $$ = make_node (OP_ARGS, $1, NULL, $3); }
	| TK_SYMBOL 				{ $$ = make_node (OP_ARGS, $1, NULL, NULL); }
	;

expression.list
	: expression TK_COMMA expression.list	{ $$ = make_node (OP_ARGS, $1, NULL, $3); }
	;
	| expression 				{ $$ = make_node (OP_ARGS, $1, NULL, NULL); }
	;

expression
	: TK_NUMBER				{ $$ = (struct node *)$1; }
	| TK_SYMBOL				{ $$ = (struct node *)$1; }
	| TK_VALOF TK_BEGIN statement.list TK_END
						{ $$ = make_node (OP_VALOF, $3, NULL, NULL); }
	| TK_STRING				{ $$ = (struct node *)$1; }
	| call.function				{ $$ = $1; }
	| TK_LP expression TK_RP		{ $$ = $2; }
	| expression TK_PLUSOP expression	{ $$ = make_node (OP_PLUSOP, $1, $3, NULL); }
	| expression TK_SUBOP expression	{ $$ = make_node (OP_SUBOP, $1, $3, NULL); }
 	| expression TK_MULTOP expression	{ $$ = make_node (OP_MULTOP, $1, $3, NULL); }
	| expression TK_DIVOP expression	{ $$ = make_node (OP_DIVOP, $1, $3, NULL); }
	| expression TK_GT expression           { $$ = make_node (OP_GT, $1, $3, NULL); }
	| expression TK_LT expression           { $$ = make_node (OP_LT, $1, $3, NULL); }
	| expression TK_GE expression           { $$ = make_node (OP_GE, $1, $3, NULL); }
	| expression TK_LE expression           { $$ = make_node (OP_LE, $1, $3, NULL); }
	| expression TK_ANDOP expression	{ $$ = make_node (OP_AND, $1, $3, NULL); }
	| expression TK_OROP expression		{ $$ = make_node (OP_OR, $1, $3, NULL); }
	| expression TK_EQUAL expression	{ $$ = make_node (OP_EQUAL, $1, $3, NULL); }
	| expression TK_ASSIGN expression	{ $$ = make_node (OP_SET, $1, $3, NULL); }
	| expression TK_VECIS expression	{ $$ = make_node (OP_VECIS, $1, $3, NULL); }
	;	

%%
