# Copyright (c) 2013-2020 Samuel K. Gutierrez All rights reserved.
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


AM_YFLAGS=-d -t

BUILT_SOURCES = \
SourceParser.cc SourceParser.h \
SourceScanner.cc

CLEANFILES = \
${BUILT_SOURCES}

bin_PROGRAMS = \
dflow

PARSER_FILES = SourceParser.yy SourceParser.h SourceScanner.ll

BASE_SRC = \
Constants.hxx \
Base.hxx Base.cxx \
DFlowException.hxx DFlowException.cxx \
Painter.hxx Painter.cxx \
Program.hxx Program.cxx \
${PARSER_FILES}

dflow_CFLAGS = \
@LIBGVC_CFLAGS@

dflow_LDADD = \
@LIBGVC_LIBS@

dflow_SOURCES = \
${BASE_SRC} \
dflow.cxx
