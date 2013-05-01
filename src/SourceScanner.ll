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

/* source code scanner for our target language */

%option interactive noyywrap nounput noinput

/* uncomment for tons of flex output */
/* %option debug */

%{

#include <iostream>
#include <cstdlib>
#include <string>

#include "Program.hxx"
#include "SourceParser.h"

#define SAVE_STOKEN                                                            \
do {                                                                           \
    yylval.str = new std::string(yytext, yyleng);                              \
} while (0)

%}

%%

[ \t\n] { ; }

"if" { return IF; }

"then" { return THEN; }

"else" { return ELSE; }

"fi" { return FI; }

"true" { SAVE_STOKEN; return TRUE; }

"false" { SAVE_STOKEN; return FALSE; }

"skip" { return SKIP; }

[0-9]+ { SAVE_STOKEN; return INT; }

[0-9]+\.[0-9]* { SAVE_STOKEN; return FLOAT; }

[a-zA-Z][a-zA-Z]* { SAVE_STOKEN; return ID; }

"+" { SAVE_STOKEN; return OPPLUS; }

"-" { SAVE_STOKEN; return OPMIN; }

"*" { SAVE_STOKEN; return OPMUL; }

"/" { SAVE_STOKEN; return OPDIV; }

"=" { SAVE_STOKEN; return ASSIGN; }

"==" { SAVE_STOKEN; return EQ; }

"<" { SAVE_STOKEN; return LT; }

"<=" { SAVE_STOKEN; return LTE; }

">" { SAVE_STOKEN; return GT; }

">=" { SAVE_STOKEN; return GTE; }

"||" { SAVE_STOKEN; return OR; }

"&&" { SAVE_STOKEN; return AND; }

"!" { SAVE_STOKEN; return NOT; }

";" { return SEND; }

. { std::cerr << "invalid token encountered during source scan: "
              << "\'" << std::string(yytext, yyleng) << "\'"
              << std::endl;
    yyterminate();
  }

%%
