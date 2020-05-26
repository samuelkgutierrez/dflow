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

#ifndef PAINTER_H_INCLUDED
#define PAINTER_H_INCLUDED

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>
#include <iostream>

#ifdef HAVE_CGRAPH_H
#include "cgraph.h"
#elif defined HAVE_GRAPHVIZ_CGRAPH_H
#include "graphviz/cgraph.h"
#else
#error "graphviz development packages are required."
#endif

#if 0 /* gvc stuff */
#include "gvc.h"
#endif

typedef Agnode_t* PNode;
typedef Agedge_t* PEdge;

class Painter {
private:
    int id;
    std::string ftype;
    std::string fprefix;
    /* graph */
    Agraph_t *graph;
#if 0 /* gvc stuff */
    /* context */
    GVC_t *gvc;
    /* config argv */
    char **config;
#endif

    Painter(void) { ; }

public:
    Painter(std::string prefix, std::string type);

    ~Painter(void);

    static PNode newNode(Painter *p, std::string label, int i);

    static PEdge newEdge(Painter *p, PNode n1, PNode n2,
                         std::string name, int j);

    void renderAST(void);

    static std::string uniqID(Painter *p);
};

#endif
