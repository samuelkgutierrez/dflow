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

extern int parserParse(FILE *fp);
/* can safely be used AFTER call to parseCFG */
//extern CFG *contextFreeGrammar;

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
static void
echoHeader(void)
{
    ;
}

/* ////////////////////////////////////////////////////////////////////////// */
static void
usage(void)
{
    cout << endl << "usage:" << endl;
    cout << "dflow [-q] appsrc" << endl;
}

/* ////////////////////////////////////////////////////////////////////////// */
static void
parseCFG(string what)
{
    FILE *fp = NULL;

    if (NULL == (fp = fopen(what.c_str(), "r"))) {
        int err = errno;
        string estr = "cannot open: " + what + ". why: " + strerror(err) + ".";
        throw DFlowException(DFLOW_WHERE, estr);
    }
    if (0 != parserParse(fp)) {
        string estr = "error encountered during CFG parse. cannot continue.";
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
    bool verboseMode = true;
    string cfgDescription, fileToParse;

    if (3 != argc && 4 != argc) {
        usage();
        return EXIT_FAILURE;
    }
    else if (3 == argc) {
        cfgDescription = string(argv[1]);
        fileToParse = string(argv[2]);
    }
    else {
        if ("-q" != string(argv[1])) {
            usage();
            return EXIT_FAILURE;
        }
        cfgDescription = string(argv[2]);
        fileToParse = string(argv[3]);
        verboseMode = false;
    }
    try {
        echoHeader();
        /* do this before we ever touch contextFreeGrammar */
        parseCFG(cfgDescription);
        if (verboseMode) {
        }
    }
    catch (DFlowException &e) {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
