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

#include "DFlowException.hxx"
#include "Base.hxx"

#include <string>

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
DFlowException::DFlowException(string fileName,
                                   int lineNo,
                                   const string &errMsg,
                                   bool where)
{
    if (where) {
        string lineNoStr = Base::int2string(lineNo);
        whatString = "[" + fileName + " " + lineNoStr + "] " + errMsg;
    }
    else {
        whatString = errMsg;
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
const char *
DFlowException::what(void) const throw()
{
    return this->whatString.c_str();
}
