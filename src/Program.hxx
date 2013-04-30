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
    Node *l;
    Node *r;

public:
    Node(void) { this->l = NULL; this->r = NULL; }

    ~Node(void) { ; }

    virtual std::string str(void) const = 0;
};

/* ////////////////////////////////////////////////////////////////////////// */
class Expression : public Node {
private:

public:
    Expression(void) : Node() { ; }

    ~Expression(void) { ; }

    virtual std::string str(void) const = 0;

};

class Identifier : public Expression {
private:
    std::string _id;

public:
    Identifier(void) { ; }

    ~Identifier(void) { ; }

    Identifier(std::string id) : Expression(), _id(id) { ; }

    std::string str(void) const { return this->_id; }
};

class Int : public Expression {
private:
    int _value;

public:
    Int(void) { ; }

    ~Int(void) { ; }

    Int(std::string svalue) :
        Expression(), _value(Base::string2int(svalue)) { ; }

    std::string str(void) const { return Base::int2string(this->_value); }
};

class Float : public Expression {
private:
    float _value;

public:
    Float(void) { ; }

    ~Float(void) { ; }

    Float(std::string svalue) :
        Expression(), _value(Base::string2float(svalue)) { ; }

    std::string str(void) const { return Base::float2string(this->_value); }

};

class Logical : public Expression {
private:
    bool _value;

public:
    Logical(void) { ; }

    ~Logical(void) { ; }

    Logical(const std::string &svalue) : _value(Base::string2bool(svalue)) { ; }

    std::string str(void) const { return Base::bool2string(this->_value); }

};

class AssignmentExpression : public Expression {
private:

public:
    AssignmentExpression(void) { ; }

    ~AssignmentExpression(void) { ; }

    AssignmentExpression(Identifier *id, Expression *expr);

    std::string str(void) const;
};

class ArithmeticExpression : public Expression {
private:
    std::string _op;

public:
    ArithmeticExpression(Expression *l,
                         std::string *op,
                         Expression *r);

    std::string str(void) const;

};

class LogicalExpression : public Expression {
private:
    std::string _op;

public:
    LogicalExpression(void);

    ~LogicalExpression(void);

    LogicalExpression(Expression *l,
                      std::string *op,
                      Expression *r);

    std::string str(void) const;
};

class Statement : public Node {
private:
    Expression *_expr;

public:
    Statement(void) { ; }

    ~Statement(void) { ; }

    Statement(Expression *expression);

    std::string str(void) const { return (this->_expr->str() + "\n"); }
};
typedef std::vector<Statement> Statements;

class Block : public Expression {
private:
    Statements _statements;

public:
    Block(void) { ; }

    void add(const Statement &s) { this->_statements.push_back(s); }

    std::string str(void) const;
};

#endif
