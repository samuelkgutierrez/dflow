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
void
Identifier::draw(Painter *p, void *e) const
{
    PNode n = Painter::newNode(p, this->_id, 1);
    Painter::newEdge(p, (PNode)e, n, "", 1);
    if (this->r) this->r->draw(p, n);
}

void
Int::draw(Painter *p, void *e) const
{
    PNode n = Painter::newNode(p, Base::int2string(this->_value), 1);
    Painter::newEdge(p, (PNode)e, n, "", 1);
}

void
Float::draw(Painter *p, void *e) const
{
    PNode n = Painter::newNode(p, Base::float2string(this->_value), 1);
    Painter::newEdge(p, (PNode)e, n, "", 1);
}

void
Logical::draw(Painter *p, void *e) const
{
    PNode n = Painter::newNode(p, Base::bool2string(this->_value), 1);
    Painter::newEdge(p, (PNode)e, n, "", 1);
}

void
AssignmentExpression::draw(Painter *p, void *e) const
{
    PNode opNode = Painter::newNode(p, "=", 1);
    Painter::newEdge(p, (PNode)e, opNode, "", 1);
    this->l->draw(p, opNode);
    this->r->draw(p, opNode);
}

void
ArithmeticExpression::draw(Painter *p, void *e) const
{
    PNode opNode = Painter::newNode(p, this->_op, 1);
    Painter::newEdge(p, (PNode)e, opNode, "", 1);
    this->l->draw(p, opNode);
    this->r->draw(p, opNode);
}

void
LogicalExpression::draw(Painter *p, void *e) const
{
    PNode opNode = Painter::newNode(p, this->_op, 1);
    Painter::newEdge(p, (PNode)e, opNode, "", 1);
    this->l->draw(p, opNode);
    this->r->draw(p, opNode);
}

void
Statement::draw(Painter *p, void *e) const
{
    this->_expr->draw(p, e);
}

void
Block::draw(Painter *p, void *e) const
{
    for (Statement *s : this->_statements) {
        s->draw(p, e);
    }
}

void
Skip::draw(Painter *p, void *e) const
{
    PNode n = Painter::newNode(p, "skip", 1);
    Painter::newEdge(p, (PNode)e, n, "", 1);
}

void
IfStatement::draw(Painter *p, void *e) const
{
    PNode ifNode = Painter::newNode(p, "if", 1);
    Painter::newEdge(p, (PNode)e, ifNode, "", 1);
    PNode ifTest = Painter::newNode(p, "[[TEST]]", 1);
    Painter::newEdge(p, ifNode, ifTest, "", 1);
    this->_exprBlock->draw(p, ifTest);
    PNode ifBody = Painter::newNode(p, "[[IF]]", 1);
    Painter::newEdge(p, ifNode, ifBody, "", 1);
    this->_ifBlock->draw(p, ifBody);
    PNode elseBody = Painter::newNode(p, "[[ELSE]]", 1);
    Painter::newEdge(p, ifNode, elseBody, "", 1);
    this->_elseBlock->draw(p, elseBody);
}

/* ////////////////////////////////////////////////////////////////////////// */
void
Block::draw(void)
{
    this->painter = new Painter();

    PNode n = Painter::newNode(this->painter, "[[PROGRAM]]", 1);
    this->draw(this->painter, n);
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
    this->_exprStatement = false;
    this->_expr = expression;
}

/* ////////////////////////////////////////////////////////////////////////// */
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
WhileStatement::draw(Painter *p, void *e) const
{
    PNode whileNode = Painter::newNode(p, "while", 1);
    Painter::newEdge(p, (PNode)e, whileNode, "", 1);
    PNode test = Painter::newNode(p, "[[TEST]]", 1);
    Painter::newEdge(p, whileNode, test, "", 1);
    this->_exprBlock->draw(p, test);
    PNode body = Painter::newNode(p, "[[BODY]]", 1);
    Painter::newEdge(p, whileNode, body, "", 1);
    this->_bodyBlock->draw(p, body);
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
