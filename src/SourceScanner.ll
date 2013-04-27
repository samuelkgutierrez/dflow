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

[0-9]+ { SAVE_ITOKEN; return TINT; }

[0-9]+\.[0-9]* { SAVE_FTOKEN; return TFLOAT; }

"+" { SAVE_STOKEN; return TOPPLUS; }

"-" { SAVE_STOKEN; return TOPMIN; }

"*" { SAVE_STOKEN; return TOPMUL; }

"=" { SAVE_STOKEN; return TASSIGN; }

"==" { SAVE_STOKEN; return TEQ; }

"<" { SAVE_STOKEN; return TLT; }

"<=" { SAVE_STOKEN; return TLTE; }

">" { SAVE_STOKEN; return TGT; }

">=" { SAVE_STOKEN; return TGTE; }

. { std::cerr << "invalid token encountered during source scan: "
              << "\'" << std::string(yytext, yyleng) << "\'"
              << std::endl;
    yyterminate();
  }

%%
