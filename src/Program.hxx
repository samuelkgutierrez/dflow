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

#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED 

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>
#include <vector>

class Identifier {
private:
    std::string _id;

public:
    Identifier(void) { ; }

    ~Identifier(void) { ; }

    Identifier(std::string id) : _id(id) { ; }

    std::string id(void) const { return this->_id; }
};

class Expression {
private:

public:
    Expression(void) { ; }

    ~Expression(void) { ; }

    //virtual bool evaluate(void);
};

class AssignmentExpression : public Expression {
private:

public:
    AssignmentExpression(void) { ; }

    ~AssignmentExpression(void) { ; }

    AssignmentExpression(Identifier id, Expression expr);
};

class ArithmeticExpression : public Expression {
private:

public:
};

class BooleanExpression : public Expression {
private:

public:
};

class Statement {
private:

public:
    Statement(void) { ; }

    ~Statement(void) { ; }

    Statement(Expression expression) { ; }
};
typedef std::vector<Statement> Statements;

class Program {
private:
    Statements _statements;

public:
    Program(void) { ; }

    ~Program(void) { ; }

    Program(const Statements &statements) : _statements(statements) { ; }
};

#endif
