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

#include "Painter.hxx"

#define ARGC 3
#define MAX_LEN 4096

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
Painter::Painter(string prefix, string type)
{
    string ftype = "-T" + type;
    string fname = "-o" + prefix + "." + type;

    /* fake an argv with our settings */
    this->config = (char **)calloc(ARGC + 1, sizeof(char *));
    this->config[ARGC] = (char *)NULL;
    for (unsigned i = 0; i < ARGC; ++i) {
        this->config[i] = (char *)calloc(MAX_LEN, sizeof(char));
    }
    strncpy(this->config[0], (char *)"dot", MAX_LEN - 1);
    strncpy(this->config[1], (char *)ftype.c_str(), MAX_LEN - 1);
    strncpy(this->config[2], (char *)fname.c_str(), MAX_LEN - 1);

    /* set up a graphviz context */
    //this->gvc = gvContext();
    /* set output and layout engine */
    //gvParseArgs(this->gvc, ARGC, this->config);
    /* prep graph so nodes and edges can be added later */
    this->graph = agopen((char *)"ast", Agdirected, 0);
    /* used to generate uniq ids */
    this->id = 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
Painter::~Painter(void)
{
    //gvFreeLayout(gvc, graph);
    agclose(graph);
    //gvFreeContext(gvc);

    if (this->config) {
        for (unsigned i = 0; i < ARGC; ++i) {
            if (this->config[i]) free(this->config[i]);
        }
        free(this->config);
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
void
Painter::renderAST(void)
{
#if 0
    gvLayoutJobs(gvc, graph);
    gvRenderJobs(gvc, graph);
#endif 

    FILE *test = fopen("/tmp/test.TEST", "w+");
    if (NULL == test) cout << "BOOOOOOOOOO" << endl;
    agwrite(this->graph, test);
}

/* ////////////////////////////////////////////////////////////////////////// */
PNode
Painter::newNode(Painter *p, string label, int i)
{
    PNode newNode =  agnode(p->graph, (char *)Painter::uniqID(p).c_str(), i);
    agsafeset(newNode, (char *)"label", (char *)label.c_str(), (char *)"");
    return newNode;
}

/* ////////////////////////////////////////////////////////////////////////// */
PEdge
Painter::newEdge(Painter *p, PNode n1, PNode n2, string name, int j)
{
    return agedge(p->graph, n1, n2, (char *)name.c_str(), j);
}

/* ////////////////////////////////////////////////////////////////////////// */
string
Painter::uniqID(Painter *p)
{
    return "__0xPrograMx0__" + Base::int2string(p->id++);
}
