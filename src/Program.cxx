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
/* ////////////////////////////////////////////////////////////////////////// */
void
Identifier::buildGraph(Painter *p, void *e) const
{
    PNode n = Painter::newNode(p, this->_id, 1);
    Painter::newEdge(p, (PNode)e, n, "", 1);
    if (this->r) this->r->buildGraph(p, n);
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
void
Int::buildGraph(Painter *p, void *e) const
{
    PNode n = Painter::newNode(p, Base::int2string(this->_value), 1);
    Painter::newEdge(p, (PNode)e, n, "", 1);
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
void
Float::buildGraph(Painter *p, void *e) const
{
    PNode n = Painter::newNode(p, Base::float2string(this->_value), 1);
    Painter::newEdge(p, (PNode)e, n, "", 1);
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
void
Logical::buildGraph(Painter *p, void *e) const
{
    PNode n = Painter::newNode(p, Base::bool2string(this->_value), 1);
    Painter::newEdge(p, (PNode)e, n, "", 1);
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
AssignmentExpression::AssignmentExpression(Identifier *id,
                                           Expression *expr)
{
    this->l = id;
    this->r = expr;
}

string
AssignmentExpression::str(void) const
{
    string out;
    out = this->l->str();
    out += " = ";
    out += this->r->str();
    return out;
}

void
AssignmentExpression::buildGraph(Painter *p, void *e) const
{
    PNode opNode = Painter::newNode(p, "=", 1);
    Painter::newEdge(p, (PNode)e, opNode, "", 1);
    this->l->buildGraph(p, opNode);
    this->r->buildGraph(p, opNode);
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
ArithmeticExpression::ArithmeticExpression(Expression *l,
                                           std::string *op,
                                           Expression *r)
{
    this->l = l;
    this->_op = string(*op);
    this->r = r;
}

string
ArithmeticExpression::str(void) const
{
    string out;
    out = this->l->str();
    out += " " + this->_op + " ";
    out += this->r->str();
    return out;
}

void
ArithmeticExpression::buildGraph(Painter *p, void *e) const
{
    PNode opNode = Painter::newNode(p, this->_op, 1);
    Painter::newEdge(p, (PNode)e, opNode, "", 1);
    this->l->buildGraph(p, opNode);
    this->r->buildGraph(p, opNode);
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
LogicalExpression::LogicalExpression(Expression *l,
                                     std::string *op,
                                     Expression *r)
{
    this->l = l;
    this->_op = string(*op);
    this->r = r;
}

string
LogicalExpression::str(void) const
{
    string out;
    out = this->l->str();
    out += " " + this->_op + " ";
    out += this->r->str();
    return out;
}

void
LogicalExpression::buildGraph(Painter *p, void *e) const
{
    PNode opNode = Painter::newNode(p, this->_op, 1);
    Painter::newEdge(p, (PNode)e, opNode, "", 1);
    this->l->buildGraph(p, opNode);
    this->r->buildGraph(p, opNode);
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
Statement::Statement(Expression *expression)
{
    this->_exprStatement = false;
    this->_expr = expression;
}

string
Statement::str(void) const
{
    unsigned realPadLen = this->_exprStatement ? 0 : this->depth();
    string out = Base::pad(realPadLen) + "[" + this->_expr->str() + "] -- " +
                 Base::int2string(this->label());

    if (!this->_exprStatement) {
        out += "\n";
    }
    return out;
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
void
Block::label(int label)
{
    this->_label = label + 1;
    int start = this->_label;
    for (Statement *s : this->_statements) {
        s->label(++start);
    }
}

string
Block::str(void) const
{
    string out = "";
    for (Statement *s : this->_statements) {
        out += s->str();
    }
    return out;
}

void
Block::buildGraph(Painter *p, void *e) const
{
    for (Statement *s : this->_statements) {
        s->buildGraph(p, e);
    }
}

void
Block::depth(unsigned depth)
{
    this->_depth = depth;
    for (Statement *s : this->_statements) {
        s->depth(this->_depth + 1);
    }
}

void
Block::draw(std::string fprefix, std::string type)
{
    /* this is the top-level call, so construct the painter */
    this->painter = new Painter(fprefix, type);
    /* start the drawing process */
    PNode n = Painter::newNode(this->painter, "[[PROGRAM]]", 1);
    this->buildGraph(this->painter, n);
    /* render the thing */
    this->painter->renderAST();
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
void
Skip::buildGraph(Painter *p, void *e) const
{
    PNode n = Painter::newNode(p, "skip", 1);
    Painter::newEdge(p, (PNode)e, n, "", 1);
}


/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
IfStatement::IfStatement(Block *expr,
                         Block *ifBlock,
                         Block *elseBlock)
{
    this->_exprBlock = expr;
    this->_ifBlock = ifBlock;
    this->_elseBlock = elseBlock;
}

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
IfStatement::buildGraph(Painter *p, void *e) const
{
    PNode ifNode = Painter::newNode(p, "if", 1);
    Painter::newEdge(p, (PNode)e, ifNode, "", 1);
    PNode ifTest = Painter::newNode(p, "[[TEST]]", 1);
    Painter::newEdge(p, ifNode, ifTest, "", 1);
    this->_exprBlock->buildGraph(p, ifTest);
    PNode ifBody = Painter::newNode(p, "[[IF]]", 1);
    Painter::newEdge(p, ifNode, ifBody, "", 1);
    this->_ifBlock->buildGraph(p, ifBody);
    PNode elseBody = Painter::newNode(p, "[[ELSE]]", 1);
    Painter::newEdge(p, ifNode, elseBody, "", 1);
    this->_elseBlock->buildGraph(p, elseBody);
}

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
/* ////////////////////////////////////////////////////////////////////////// */
WhileStatement::WhileStatement(Block *expr, Block *bodyBlock)
{
    this->_exprBlock = expr;
    this->_bodyBlock = bodyBlock;
}

string
WhileStatement::str(void) const
{
    string out = Base::pad(this->depth()) + "while ";
    out += this->_exprBlock->str() + " do\n";
    out += this->_bodyBlock->str();
    out += Base::pad(this->depth()) + "od\n";
    return out;
}

void
WhileStatement::depth(unsigned depth)
{
    this->_depth = depth;
    this->_exprBlock->depth(depth);
    this->_bodyBlock->depth(depth + 1);
}

void
WhileStatement::label(int label)
{
    this->_label = label + 1;
    this->_exprBlock->label(this->label());
    this->_bodyBlock->label(this->label() + 1);
}

void
WhileStatement::buildGraph(Painter *p, void *e) const
{
    PNode whileNode = Painter::newNode(p, "while", 1);
    Painter::newEdge(p, (PNode)e, whileNode, "", 1);
    PNode test = Painter::newNode(p, "[[TEST]]", 1);
    Painter::newEdge(p, whileNode, test, "", 1);
    this->_exprBlock->buildGraph(p, test);
    PNode body = Painter::newNode(p, "[[BODY]]", 1);
    Painter::newEdge(p, whileNode, body, "", 1);
    this->_bodyBlock->buildGraph(p, body);
}
