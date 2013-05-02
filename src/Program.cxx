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

#include "Program.hxx"
#include "Base.hxx"
#include "Painter.hxx"

#include <iostream>

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
string
Block::str(void) const
{
    string out = "";
    for (Statement *s : this->_statements) {
        out += s->str();
    }
    return out;
}

/* ////////////////////////////////////////////////////////////////////////// */
AssignmentExpression::AssignmentExpression(Identifier *id,
                                           Expression *expr)
{
    this->l = id;
    this->r = expr;
}

/* ////////////////////////////////////////////////////////////////////////// */
ArithmeticExpression::ArithmeticExpression(Expression *l,
                                           std::string *op,
                                           Expression *r)
{
    this->l = l;
    this->_op = string(*op);
    this->r = r;
}

/* ////////////////////////////////////////////////////////////////////////// */
string
AssignmentExpression::str(void) const
{
    string out;
    out = this->l->str();
    out += " = ";
    out += this->r->str();
    return out;
}

/* ////////////////////////////////////////////////////////////////////////// */
void *
Identifier::draw(Painter *p) const
{
    return Painter::newNode(p, this->_id, 1);
}

void *
Int::draw(Painter *p) const
{
    return Painter::newNode(p, Base::int2string(this->_value), 1);
}

void *
Float::draw(Painter *p) const
{
    return Painter::newNode(p, Base::float2string(this->_value), 1);
}

void *
Logical::draw(Painter *p) const
{
    return Painter::newNode(p, Base::bool2string(this->_value), 1);
}

void *
AssignmentExpression::draw(Painter *p) const
{
    PNode opNode = Painter::newNode(p, "=", 1);
    Painter::newEdge(p, opNode, (PNode)this->l->draw(p), "", 1);
    Painter::newEdge(p, opNode, (PNode)this->r->draw(p), "", 1);
    return opNode;
}

void *
ArithmeticExpression::draw(Painter *p) const
{
    PNode opNode = Painter::newNode(p, this->_op, 1);
    Painter::newEdge(p, opNode, (PNode)this->l->draw(p), "", 1);
    Painter::newEdge(p, opNode, (PNode)this->r->draw(p), "", 1);
    return opNode;
}

void *
LogicalExpression::draw(Painter *p) const
{
    PNode opNode = Painter::newNode(p, this->_op, 1);
    Painter::newEdge(p, opNode, (PNode)this->l->draw(p), "", 1);
    Painter::newEdge(p, opNode, (PNode)this->r->draw(p), "", 1);
    return opNode;
}

/* XXX */
void *
Statement::draw(Painter *p) const
{
    return this->_expr->draw(p);
}

/* XXX */
void *
Block::draw(Painter *p) const
{
    PNode blockNode = Painter::newNode(p, "block", 1);
    for (Statement *s : this->_statements) {
        Painter::newEdge(p, blockNode, (PNode)s->draw(p), " ", 1);
    }
    return blockNode;
}

void *
Skip::draw(Painter *p) const
{
    return Painter::newNode(p, "skip", 1);
}

/* XXX */
void *
IfStatement::draw(Painter *p) const
{
    return this->_exprBlock->draw(p);
}

/* ////////////////////////////////////////////////////////////////////////// */
void
Block::draw(void)
{
    this->painter = new Painter();
    this->draw(this->painter);
    /* XXX fix path */
    this->painter->drawAST("foo");
}

/* ////////////////////////////////////////////////////////////////////////// */
LogicalExpression::LogicalExpression(Expression *l,
                                     std::string *op,
                                     Expression *r)
{
    this->l = l;
    this->_op = string(*op);
    this->r = r;
}

/* ////////////////////////////////////////////////////////////////////////// */
string
LogicalExpression::str(void) const
{
    string out;
    out = this->l->str();
    out += " " + this->_op + " ";
    out += this->r->str();
    return out;
}

/* ////////////////////////////////////////////////////////////////////////// */
string
ArithmeticExpression::str(void) const
{
    string out;
    out = this->l->str();
    out += " " + this->_op + " ";
    out += this->r->str();
    return out;
}

/* ////////////////////////////////////////////////////////////////////////// */
Statement::Statement(Expression *expression)
{
    this->_expr = expression;
}

/* ////////////////////////////////////////////////////////////////////////// */
string
Statement::str(void) const
{
    unsigned realPadLen = this->_exprStatement ? 0 : this->depth();
    string out = Base::pad(realPadLen) + "[" + this->_expr->str() + "]" +
                 Base::int2string(this->label());

    if (!this->_exprStatement) {
        out += ";\n";
    }
    return out;
}

/* ////////////////////////////////////////////////////////////////////////// */
IfStatement::IfStatement(Block *expr,
                         Block *ifBlock,
                         Block *elseBlock)
{
    this->_exprBlock = expr;
    this->_ifBlock = ifBlock;
    this->_elseBlock = elseBlock;
}

/* ////////////////////////////////////////////////////////////////////////// */
string
IfStatement::str(void) const
{
    string out = Base::pad(this->depth()) + "if ";
    out += this->_exprBlock->str() + " then\n";
    out += this->_ifBlock->str();
    out += Base::pad(this->depth()) + "else\n";
    out += this->_elseBlock->str();
    out += Base::pad(this->depth()) + "fi\n";
    return out;
}

/* ////////////////////////////////////////////////////////////////////////// */
void
Block::depth(unsigned depth)
{
    this->_depth = depth;
    for (Statement *s : this->_statements) {
        s->depth(this->_depth + 1);
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
void
IfStatement::depth(unsigned depth)
{
    this->_depth = depth;
    this->_exprBlock->depth(depth);
    this->_ifBlock->depth(depth + 1);
    this->_elseBlock->depth(depth + 1);
}

void
IfStatement::label(int label)
{
    this->_label = label + 1;
    this->_exprBlock->label(this->label());
    this->_ifBlock->label(this->label() + 1);
    int next = this->_ifBlock->label() +
               (int)this->_ifBlock->nstatements() + 1;
    this->_elseBlock->label(next);
}

void
Block::label(int label)
{
    this->_label = label + 1;
    int start = this->_label;
    for (Statement *s : this->_statements) {
        s->label(++start);
    }
}
