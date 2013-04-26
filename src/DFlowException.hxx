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

#ifndef DFLOW_EXCEPTION_H_INCLUDED
#define DFLOW_EXCEPTION_H_INCLUDED 

#include <string>
#include <exception>

class DFlowException : public std::exception {
private:
    std::string whatString;
    DFlowException(void);

public:
    ~DFlowException(void) throw() { }

    DFlowException(std::string fileName,
                     int lineNo,
                     const std::string &errMsg,
                     bool where = true);

    virtual const char *what(void) const throw();
};

#endif
