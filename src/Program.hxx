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
protected:
    std::string _str;

public:
    Node(void) { ; }

    ~Node(void) { ; }

    virtual std::string str(void) const = 0;
};

/* ////////////////////////////////////////////////////////////////////////// */
class Expression : public Node {
private:

public:
    Expression(void) { ; }

    ~Expression(void) { ; }

    virtual std::string str(void) const { return this->_str; }

};

class Identifier : public Expression {
private:
    std::string _id;

public:
    Identifier(void) { ; }

    ~Identifier(void) { ; }

    Identifier(std::string id) : _id(id) { ; }

    std::string id(void) const { return this->_id; }

    std::string str(void) const { return this->id(); }
};

class Int : public Expression {
private:
    int _value;

public:
    Int(void) { ; }

    ~Int(void) { ; }

    Int(std::string svalue) : _value(Base::string2int(svalue)) { ; }

    std::string str(void) const { return Base::int2string(this->_value); }

};

class Float : public Expression {
private:
    float _value;

public:
    Float(void) { ; }

    ~Float(void) { ; }

    Float(std::string svalue) : _value(Base::string2int(svalue)) { ; }

    std::string str(void) const { return "FLOAT"; }

};

class AssignmentExpression : public Expression {
private:
    Identifier _id;
    Expression _expr;

public:
    AssignmentExpression(void) { ; }

    ~AssignmentExpression(void) { ; }

    AssignmentExpression(const Identifier &id, const Expression &expr);

    std::string str(void) const { return this->_id.str() + this->_expr.str(); }
};

class ArithmeticExpression : public Expression {
private:
    Expression _lhs;
    std::string _op;
    Expression _rhs;

public:
    ArithmeticExpression(const Expression &l,
                         const std::string &op,
                         const Expression &r) : _lhs(l), _op(op), _rhs(r) { ; }

    std::string str(void) const { return this->_lhs.str() + this->_op + this->_rhs.str(); }

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

    std::string str(void) const { return "Statement" ; } 
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
