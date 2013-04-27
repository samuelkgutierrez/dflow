/**
 * Copyright (c) 2013 Samuel K. Gutierrez All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

%error-verbose

%{

#include "Program.hxx"

#include <cstdlib>
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

int yylex(void);
extern "C" int yyerror(const char *s);
extern "C" FILE *yyin;

/* pointer to the newly created program grammar instance */
//Program *program = NULL;

/* input line number used for nice error messages */
static int lineNo = 1;

%}

%union {
    int intt;
    float floatt;
    std::string *str;
    Identifier *ident;
    Expression *expression;
    Statement *statement;
    Statements *statements;
    Program *program;
}

%token <str> TID;

%token <intt> TINT;

%token <floatt> TFLOAT;

%token TOPPLUS TOPMIN TOPMUL TOPDIV
       TASSIGN TEQ TLT TLTE TGT TGTE
       TOR TAND TNOT TSEND;

%type <program> program;
%type <ident> ident;
%type <expression> expr;
%type <statement> statement;
%type <statements> statements;

%start program

%%

program : statements { $$ = new Program(*$1); }
;

statements : statement { $$ = new Statements(); }
           | statements statement { ; }
;

statement : expr { $$ = new Statement(*$1); }
;

expr : ident TASSIGN expr { $$ = new AssignmentExpression(*$1, *$3); }
     | expr arithmeticbinop expr
     | numeric
     | ident
;

ident : TID { $$ = new Identifier(*$1); delete $1; } 
;

numeric : TINT
        | TFLOAT
;

arithmeticbinop : TOPPLUS
                | TOPMIN
                | TOPMUL
                | TOPDIV
;

%%

/* ////////////////////////////////////////////////////////////////////////// */
/* wrapper for yyparse */
int
parserParse(FILE *fp = NULL)
{
    /* set YYDEBUG to anything for more parser debug output */
    yydebug = !!getenv("YYDEBUG");
    /* set yyin */
    yyin = fp;
    /* fp closed by caller */
    return yyparse();
}

/* ////////////////////////////////////////////////////////////////////////// */
int
yyerror(const char *s)
{
    std::cout << "parse error:" << std::endl
              << "- what: " << s << std::endl
              << "- where: line " << lineNo << std::endl;
    /* ignored */
    return 42;
}

