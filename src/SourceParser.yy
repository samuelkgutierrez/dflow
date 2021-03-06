/**
 * Copyright (c) 2013-2020 Samuel K. Gutierrez All rights reserved.
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

%defines "SourceParser.hh"

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

/* pointer to the top-level program block */
Block *programRoot = NULL;

/* input line number used for nice error messages */
extern int lineNo;

%}

%union {
    std::string *str;
    Block *block;
    Identifier *ident;
    Expression *expression;
    Statement *statement;
    Statements *statements;
}

%token <str> ID INT FLOAT SEND
             OPPLUS OPMIN OPMUL OPDIV
             ASSIGN EQ LT LTE GT GTE
             OR AND NOT TRUE FALSE;

%token IF THEN ELSE FI SKIP WHILE DO OD;

%type <block> program statements;
%type <expression> aexpr bexpr assignexpr expr num logical;
%type <statement> skipstat statement;
%type <str> mathbinop logicbinop;
%type <ident> ident;

%start program

%%

program : statements { programRoot = $1; }
        ;

statements : statement { $$ = new Block(); $$->add($1); }
           | statements statement { $1->add($2); }
           ;

statement  : assignexpr SEND { $$ = new Statement($1); }
           | skipstat { $$ = $1; }
           | IF expr THEN statements ELSE statements FI {
                 Block *exprBlock = new Block();
                 Statement *exprStatement = new Statement($2);
                 exprStatement->exprStatement(true);
                 exprBlock->add(exprStatement);
                 $$ = new IfStatement(exprBlock, $4, $6);
             }
           | WHILE expr DO statements OD {
                 Block *exprBlock = new Block();
                 Statement *exprStatement = new Statement($2);
                 exprStatement->exprStatement(true);
                 exprBlock->add(exprStatement);
                 $$ = new WhileStatement(exprBlock, $4);
             }
           ;

expr : aexpr { $$ = $1; }
     | bexpr { $$ = $1; }
     | ident { $$ = $1; }
     | NOT expr { $2->notit(); $$ = $2; }
     ;

skipstat : SKIP SEND { $$ = new Skip(); }
         ;

assignexpr : ident ASSIGN expr { $$ = new AssignmentExpression($1, $3); }
           ;

bexpr : logical logicbinop logical { $$ = new LogicalExpression($1, $2, $3); }
      | logical logicbinop ident { $$ = new LogicalExpression($1, $2, $3); }
      | ident logicbinop ident { $$ = new LogicalExpression($1, $2, $3); }
      | ident logicbinop logical { $$ = new LogicalExpression($1, $2, $3); }
      | ident logicbinop num { $$ = new LogicalExpression($1, $2, $3); }
      | num logicbinop ident { $$ = new LogicalExpression($1, $2, $3); }
      | num logicbinop num { $$ = new LogicalExpression($1, $2, $3); }
      | logical { $$ = $1; }
      ;

aexpr : num mathbinop num { $$ = new ArithmeticExpression($1, $2, $3); }
      | num mathbinop ident { $$ = new ArithmeticExpression($1, $2, $3); }
      | ident mathbinop ident { $$ = new ArithmeticExpression($1, $2, $3); }
      | ident mathbinop num { $$ = new ArithmeticExpression($1, $2, $3); }
      | num { $$ = $1;  }
      ;

ident : ID { $$ = new Identifier(*$1); delete $1; }
      ;

num : INT { $$ = new Int(*$1); delete $1; }
    | FLOAT { $$ = new Float(*$1); delete $1; }
    ;

logical : TRUE { $$ = new Logical(*$1); delete $1; }
        | FALSE { $$ = new Logical(*$1); delete $1; }
        ;

mathbinop : OPPLUS | OPMIN | OPMUL | OPDIV
          ;

logicbinop : EQ | LT | LTE | GT | GTE | OR | AND
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
              << "- around: line " << lineNo << std::endl;
    /* ignored */
    return 42;
}
