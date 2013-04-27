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

#define SAVE_ITOKEN                                                            \
do {                                                                           \
    yylval.intt = (int)strtol(yytext, (char **)NULL, 10);                      \
} while (0)

#define SAVE_FTOKEN                                                            \
do {                                                                           \
    yylval.floatt = strtof(yytext, (char **)NULL);                             \
} while (0)

%}

%%

[ \t\n] { ; }

[0-9]+ { SAVE_STOKEN; return TINT; }

[0-9]+\.[0-9]* { SAVE_STOKEN; return TFLOAT; }

[a-zA-Z][a-zA-Z]* { SAVE_STOKEN; return TID; }

"+" { return TOPPLUS; }

"-" { return TOPMIN; }

"*" { return TOPMUL; }

"/" { return TOPDIV; }

"=" { return TASSIGN; }

"==" { return TEQ; }

"<" { return TLT; }

"<=" { return TLTE; }

">" { return TGT; }

">=" { return TGTE; }

"||" { return TOR; }

"&&" { return TAND; }

"!" { return TNOT; }

";" { return TSEND; }

. { std::cerr << "invalid token encountered during source scan: "
              << "\'" << std::string(yytext, yyleng) << "\'"
              << std::endl;
    yyterminate();
  }

%%
