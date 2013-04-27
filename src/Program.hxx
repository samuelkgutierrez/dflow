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

#include "Base.hxx"

#include <string>
#include <vector>

/* ////////////////////////////////////////////////////////////////////////// */
/* nodes will be the basic building block of a program */
class Node {
public:
    Node(void) { ; }

    ~Node(void) { ; }
};

/* ////////////////////////////////////////////////////////////////////////// */
class Identifier : public Node {
private:
    std::string _id;

public:
    Identifier(void) { ; }

    ~Identifier(void) { ; }

    Identifier(std::string id) : _id(id) { ; }

    std::string id(void) const { return this->_id; }
};

/* ////////////////////////////////////////////////////////////////////////// */
class Expression : public Node {
private:

public:
    Expression(void) { ; }

    ~Expression(void) { ; }
};

class Int : public Expression {
private:
    int _value;

public:
    Int(void) { ; }

    ~Int(void) { ; }

    Int(std::string svalue) : _value(Base::string2int(svalue)) { ; }

};

class Float : public Expression {
private:
    float _value;

public:
    Float(void) { ; }

    ~Float(void) { ; }

    Float(std::string svalue) : _value(Base::string2int(svalue)) { ; }

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

class Statement : public Node {
private:

public:
    Statement(void) { ; }

    ~Statement(void) { ; }

    Statement(Expression expression) { ; }
};
typedef std::vector<Statement> Statements;

class Block : public Expression {
private:
    Statements _statements;

public:
    Block(void) { ; }

    void add(const Statement &s) { this->_statements.push_back(s); }
};

#endif
