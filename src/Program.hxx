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

class Painter;

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
/* nodes will be the basic building block of a program */
class Node {
protected:
    int _label;
    /* node depth */
    unsigned _depth;
    /* left child pointer */
    Node *l;
    /* right child pointer */
    Node *r;
    /* the node painter */
    Painter *painter;

public:
    Node(void) { this->l = NULL;
                 this->r = NULL;
                 this->_depth = 0;
                 this->_label = 0; }

    virtual ~Node(void) { ; }

    virtual std::string str(void) const = 0;

    virtual unsigned depth(void) const { return this->_depth; }

    virtual void depth(unsigned depth) { this->_depth = depth; }

    virtual int label(void) const { return this->_label; }

    virtual void label(int label) { this->_label = label; }

    virtual void *draw(Painter *p) const = 0;
};

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
class Expression : public Node {
private:

public:
    Expression(void) { ; }

    virtual ~Expression(void) { ; }

    virtual std::string str(void) const = 0;

    virtual void *draw(Painter *p) const = 0;
};

/* ////////////////////////////////////////////////////////////////////////// */
class Identifier : public Expression {
private:
    std::string _id;

public:
    Identifier(void) { ; }

    ~Identifier(void) { ; }

    Identifier(std::string id) : Expression(), _id(id) { ; }

    std::string str(void) const { return this->_id; }

    virtual void *draw(Painter *p) const;
};

/* ////////////////////////////////////////////////////////////////////////// */
class Int : public Expression {
private:
    int _value;

public:
    Int(void) { ; }

    ~Int(void) { ; }

    Int(std::string svalue) :
        Expression(), _value(Base::string2int(svalue)) { ; }

    std::string str(void) const { return Base::int2string(this->_value); }

    virtual void *draw(Painter *p) const;
};

/* ////////////////////////////////////////////////////////////////////////// */
class Float : public Expression {
private:
    float _value;

public:
    Float(void) { ; }

    ~Float(void) { ; }

    Float(std::string svalue) :
        Expression(), _value(Base::string2float(svalue)) { ; }

    std::string str(void) const { return Base::float2string(this->_value); }

    virtual void *draw(Painter *p) const;

};

/* ////////////////////////////////////////////////////////////////////////// */
class Logical : public Expression {
private:
    bool _value;

public:
    Logical(void) { ; }

    ~Logical(void) { ; }

    Logical(const std::string &svalue) : _value(Base::string2bool(svalue)) { ; }

    std::string str(void) const { return Base::bool2string(this->_value); }

    virtual void *draw(Painter *p) const;
};

/* ////////////////////////////////////////////////////////////////////////// */
class AssignmentExpression : public Expression {
private:

public:
    AssignmentExpression(void) { ; }

    ~AssignmentExpression(void) { ; }

    AssignmentExpression(Identifier *id, Expression *expr);

    std::string str(void) const;

    virtual void *draw(Painter *p) const;
};

/* ////////////////////////////////////////////////////////////////////////// */
class ArithmeticExpression : public Expression {
private:
    std::string _op;

public:
    ArithmeticExpression(void) { ; }

    ~ArithmeticExpression(void) { ; }

    ArithmeticExpression(Expression *l,
                         std::string *op,
                         Expression *r);

    std::string str(void) const;

    virtual void *draw(Painter *p) const { ; }

};

/* ////////////////////////////////////////////////////////////////////////// */
class LogicalExpression : public Expression {
private:
    std::string _op;

public:
    LogicalExpression(void);

    virtual ~LogicalExpression(void) { ; }

    LogicalExpression(Expression *l,
                      std::string *op,
                      Expression *r);

    std::string str(void) const;

    virtual void *draw(Painter *p) const { ; }
};

/* ////////////////////////////////////////////////////////////////////////// */
class Statement : public Node {
protected:
    bool _exprStatement;
    Expression *_expr;

public:
    Statement(void) : _exprStatement(false), _expr(NULL) { ; }

    virtual ~Statement(void) { ; }

    Statement(Expression *expression);

    virtual std::string str(void) const;

    virtual bool exprStatement(void) const { return this->_exprStatement; }

    virtual void exprStatement(bool is) { this->_exprStatement = is; }

    virtual void *draw(Painter *p) const { ; }

};
typedef std::vector<Statement> Statements;
typedef std::vector<Statement *> Statementps;

/* ////////////////////////////////////////////////////////////////////////// */
class Block : public Node {
protected:
    Statementps _statements;

public:
    Block(void) { ; }

    virtual ~Block(void) { ; }

    virtual void add(Statement *s) { this->_statements.push_back(s); }

    std::string str(void) const;

    virtual unsigned depth(void) const { return Node::depth(); }

    virtual void depth(unsigned depth);

    virtual int label(void) const { return this->_label; }

    virtual void label(int label);

    virtual void *draw(Painter *p) const { ; }

    void draw(void);

    unsigned nstatements(void) const { return this->_statements.size(); }

};
typedef std::vector<Block> Blocks;

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
class Skip : public Statement {
public:
    Skip(void) { ; }

    virtual ~Skip(void) { ; }

    std::string str(void) const {
        return Base::pad(this->depth()) + "[skip]" +
               Base::int2string(this->label()) + ";\n";
    }

    virtual void *draw(Painter *p) const { ; }
};

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
class IfStatement : public Statement {
private:
    Block *_exprBlock;
    Block *_ifBlock;
    Block *_elseBlock;

public:
    IfStatement(void) { ; }

    virtual ~IfStatement(void) { ; }

    IfStatement(Block *expr, Block *ifBlock, Block *elseBlock);

    std::string str(void) const;

    virtual unsigned depth(void) const { return this->_depth; }

    virtual void depth(unsigned depth);

    virtual int label(void) const { return this->_label; }

    virtual void label(int label);

    virtual void *draw(Painter *p) const { ; }
};

#endif
