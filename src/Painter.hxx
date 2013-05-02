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

#ifndef PAINTER_H_INCLUDED
#define PAINTER_H_INCLUDED 

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>
#include <iostream>

#include "cgraph.h"
#include "gvc.h"

class Painter {
private:
    /* context */
    GVC_t *gvc;
    /* graph */
    Agraph_t *graph;

public:
    Painter(void);

    ~Painter(void);

    void drawAST(std::string fileName);
};

#endif
