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

#define SAVE_TOKEN                                                             \
do {                                                                           \
    yylval.str = new std::string(yytext, yyleng);                              \
} while (0)

%}

/* range of valid term ascii characters in octal */
ASCII [\45-\176]

WS [ \t]

%%

{WS}+ { ; }

"\n" { return NEWLINE; }

"#".*"\n" { return COMMENT; }

"-->" { return ARROW; }

{ASCII}+ { SAVE_TOKEN; return TERM; }

. { std::cerr << "invalid token encountered during source scan... bye!"
              << std::endl;
    yyterminate();
  }

%%
