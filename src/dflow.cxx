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


#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

#include <string.h>
#include <errno.h>

#include "Constants.hxx"
#include "DFlowException.hxx"
#include "Painter.hxx"
#include "Program.hxx"

extern int parserParse(FILE *fp);
/* can safely be used AFTER call to parseAppSource */
extern Block *programRoot;

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
static void
usage(void)
{
    cout << endl << "usage:" << endl;
    cout << "dflow appsrc" << endl;
}

/* ////////////////////////////////////////////////////////////////////////// */
static void
parseAppSource(string what)
{
    FILE *fp = NULL;

    if (NULL == (fp = fopen(what.c_str(), "r"))) {
        int err = errno;
        string estr = "cannot open: " + what + ". why: " + strerror(err) + ".";
        throw DFlowException(DFLOW_WHERE, estr);
    }
    if (0 != parserParse(fp)) {
        string estr = "error encountered during source parse. cannot continue.";
        throw DFlowException(DFLOW_WHERE, estr);
    }
    if (NULL != fp) {
        fclose(fp);
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
int
main(int argc, char **argv)
{
    if (2 != argc) {
        usage();
        return EXIT_FAILURE;
    }
    try {
        /* do this before we ever touch programRoot */
        cout << "> generating AST from " + string(argv[1]) + " ... ";
        parseAppSource(string(argv[1]));
        cout << "done" << endl;
        cout << "> generating source from AST ... " << endl;
        cout << "> -- setting nesting depths ... ";
        programRoot->depth(0);
        cout << "done" << endl;
        cout << "> -- labeling ... ";
        int label = 0;
        programRoot->label(label);
        cout << "done" << endl;
        cout << "> here is the annotated source:" << endl;
        cout << programRoot->str(true);
        cout << "> drawing ASTs ... " << endl;
        programRoot->drawASTs(string(argv[1]), "eps");
        cout << "> drawing control flow graph ... " << endl;
        programRoot->drawCFG(string(argv[1]), "eps");
        cout << "> calculating reaching definitions data-flow ... " << endl;
        cout << "> -- gathering variables ... " << endl;
        programRoot->gatherVars();
        /* rm any cruft */
        programRoot->varclean();
        cout << "> -- " << programRoot->nvars() << " found ..." << endl;
        cout << "> -- "; programRoot->emitVars();
        programRoot->rdcalc();
    }
    catch (DFlowException &e) {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
