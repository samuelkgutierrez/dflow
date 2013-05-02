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

#include "Painter.hxx"
#include "Base.hxx"
#include "Constants.hxx"
#include "DFlowException.hxx"

#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <sstream>

#include <string.h>
#include <ctype.h>


using namespace std;

    /* graphviz configury */
static char *args[] = {
    "dot",      /* use dot */
    "-Tpdf",    /* pdf output */
    "-oabc.pdf" /* output to file abc.pdf */
};


Painter::Painter(void)
{
    Agnode_t *n, *m;
    Agedge_t *e;

    this->gvc = gvContext();
    gvParseArgs (gvc, sizeof(args)/sizeof(char*), args);

    this->graph = agopen((char *)"ast", Agdirected, 0);
    n = agnode(graph, "n", 1);
    m = agnode(graph, "m", 1);
    e = agedge(graph, n, m, 0, 1);
    agsafeset(n, "color", "red", "");
    gvLayoutJobs(gvc, graph);
    gvRenderJobs(gvc, graph);
    gvFreeLayout(gvc, graph);
    agclose(graph);
    gvFreeContext(gvc);
}
