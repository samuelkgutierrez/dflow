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

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
string
Base::int2string(int i)
{
    char tmpBuf[256];

    fill_n(tmpBuf, sizeof(tmpBuf), '\0');
    snprintf(tmpBuf, sizeof(tmpBuf) - 1, "%d", i);
    return string(tmpBuf);
}

/* ////////////////////////////////////////////////////////////////////////// */
int
Base::string2int(const string &str)
{
    int num = 0;
    istringstream(str) >> num;
    return num;
}

/* ////////////////////////////////////////////////////////////////////////// */
float
Base::string2float(const string &str)
{
    float num = 0;
    istringstream(str) >> num;
    return num;
}
