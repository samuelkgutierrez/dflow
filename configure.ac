# Copyright (c) 2013 Samuel K. Gutierrez All rights reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

AC_PREREQ([2.60])

AC_INIT([dflow], [0.2])

AC_CONFIG_SRCDIR([src])

AC_CONFIG_MACRO_DIR([config])

# Check If AM_SILENT_RULES Rule Are Supported
# If So, Build Using AM_SILENT_RULES
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])


# note: -Wall -Werror are automake options.  they are NOT compiler flags
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AM_CONFIG_HEADER([config.h])

# checks for programs.
AC_USE_SYSTEM_EXTENSIONS
AC_LANG([C++])
AC_PROG_CXX([clang++ g++])

dnl check for C++11 support
AX_CXX_COMPILE_STDCXX_11([noext])
AS_IF([test "x$HAVE_CXX11" != "x1"],
      [AC_MSG_ERROR([** A compiler with C++11 language features is required.])])

AC_PROG_LEX
AC_PROG_YACC
AC_PROG_LIBTOOL

# checks for libraries.
dnl AC_CHECK_LIB([gvc], [gvContext], [],
dnl             [AC_MSG_ERROR([** libcgraph (DOT libraries) required.])],
dnl             [-lcgraph -lcdt])

# checks for header files.
AC_CHECK_HEADERS([\
inttypes.h limits.h stdint.h stdlib.h string.h unistd.h
])

# checks for typedefs, structures, and compiler characteristics.

# checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset strerror strtoul])

AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT

# let the user know the configuration 

cat << EOF

dflow configuration
+------------------
|
| CXX       : $CXX
| CXXFLAGS  : $CXXFLAGS
| LDFLAGS   : $LDFLAGS
| LIBS      : $LIBS
| CPPFLAGS  : $CPPFLAGS
| CPP       : $CPP

EOF
