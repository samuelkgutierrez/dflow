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
Identifier::buildAST(Painter *p, void *e, bool a) const
{
    string label = this->_id; 
    if (a) label += " " + Base::int2string(this->label());
    PNode n = Painter::newNode(p, label, 1);
    Painter::newEdge(p, (PNode)e, n, "", 1);
    if (this->r) this->r->buildAST(p, n, a);
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
void
Int::buildAST(Painter *p, void *e, bool a) const
{
    string label = Base::int2string(this->_value);
    if (a) label += " " + Base::int2string(this->label());
    PNode n = Painter::newNode(p, label, 1);
    Painter::newEdge(p, (PNode)e, n, "", 1);
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
void
Float::buildAST(Painter *p, void *e, bool a) const
{
    string label = Base::float2string(this->_value);
    if (a) label += " " + Base::int2string(this->label());
    PNode n = Painter::newNode(p, label, 1);
    Painter::newEdge(p, (PNode)e, n, "", 1);
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
void
Logical::buildAST(Painter *p, void *e, bool a) const
{
    string label = Base::bool2string(this->_value);
    if (a) label += " " + Base::int2string(this->label());
    PNode n = Painter::newNode(p, label, 1);
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
AssignmentExpression::buildAST(Painter *p, void *e, bool a) const
{
    string label  = "=";
    if (a) label += " " + Base::int2string(this->label());
    PNode opNode = Painter::newNode(p, label, 1);
    Painter::newEdge(p, (PNode)e, opNode, "", 1);
    this->l->buildAST(p, opNode, a);
    this->r->buildAST(p, opNode, a);
}

void *
AssignmentExpression::buildCFG(Painter *p)
{
    string label  = this->str();
    return Painter::newNode(p, label, 1);
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
ArithmeticExpression::buildAST(Painter *p, void *e, bool a) const
{
    string label = this->_op;
    if (a) label += " " + Base::int2string(this->label());
    PNode opNode = Painter::newNode(p, label, 1);
    Painter::newEdge(p, (PNode)e, opNode, "", 1);
    this->l->buildAST(p, opNode, a);
    this->r->buildAST(p, opNode, a);
}

void *
ArithmeticExpression::buildCFG(Painter *p)
{
    cout << "MATH EXPR" << endl;
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
LogicalExpression::buildAST(Painter *p, void *e, bool a) const
{
    string label = this->_op;
    if (a) label += " " + Base::int2string(this->label());
    PNode opNode = Painter::newNode(p, label, 1);
    Painter::newEdge(p, (PNode)e, opNode, "", 1);
    this->l->buildAST(p, opNode, a);
    this->r->buildAST(p, opNode, a);
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
const int Block::ndias = 2;
const string Block::diaNames[Block::ndias] = {"ast", "dast"};

void
Block::label(int &label)
{
    bool first = true;
    this->_label = label;
    Statement *lastp = NULL;
    for (Statement *s : this->_statements) {
        if (first) {
            s->entry(true);
            first = false;
        }
        s->label(label);
        lastp = s;
    }
    lastp->exit(true);
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
Block::buildAST(Painter *p, void *e, bool a) const
{
    for (Statement *s : this->_statements) {
        s->buildAST(p, e, a);
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

void *
Block::buildCFG(Painter *p)
{
    bool first = true;
    PNode firstNode, prev;
    for (Statement *s : this->_statements) {
        if (first) {
            firstNode = (PNode)s->buildCFG(p);
            first = false;
            prev = firstNode;
        }
        else {
            PNode cur = (PNode)s->buildCFG(p);
            Painter::newEdge(p, prev, cur, "", 1);
            prev = cur;
        }
    }
    return firstNode;
}

void
Block::drawASTs(std::string fprefix, std::string type)
{
    for (auto i = 0; i < Block::ndias; ++i) {
        string fname = fprefix + "-" + Block::diaNames[i];
        /* this is the top-level call, so construct the painter */
        this->painter = new Painter(fname, type);
        /* start the drawing process */
        PNode n = Painter::newNode(this->painter, "[[PROGRAM]]", 1);
        this->buildAST(this->painter, n, (Block::diaNames[i] == "dast"));
        /* render the thing -- is that even the correct term? */
        cout << "> -- writing " + fname + "." + type + " ... "; cout.flush();
        this->painter->renderAST();
        cout << "done" << endl;
        delete this->painter;
    }
}

void
Block::drawCFG(std::string fprefix, std::string type)
{
    string fname = fprefix + "-" + "cfg";
    /* this is the top-level call, so construct the painter */
    this->painter = new Painter(fname, type);
    /* start the drawing process */
    PNode n = Painter::newNode(this->painter, "[[PROGRAM]]", 1);
    PNode t = (PNode)this->buildCFG(this->painter);
    Painter::newEdge(this->painter, n, t, "", 1);
    /* render the thing -- is that even the correct term? */
    cout << "> -- writing " + fname + "." + type + " ... "; cout.flush();
    this->painter->renderAST();
    cout << "done" << endl;
    delete this->painter;
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
void
Skip::buildAST(Painter *p, void *e, bool a) const
{
    string label = "skip";
    if (a) label += " " + Base::int2string(this->label());
    PNode n = Painter::newNode(p, label, 1);
    Painter::newEdge(p, (PNode)e, n, "", 1);
}

void *
Skip::buildCFG(Painter *p)
{
    string label = "skip";
    return Painter::newNode(p, label, 1);
}


/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
IfStatement::IfStatement(Block *expr,
                         Block *ifBlock,
                         Block *elseBlock)
{
    this->_meta = "if";
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
IfStatement::label(int &label)
{
    this->_label = ++label;
    this->_exprBlock->label(label);
    this->_ifBlock->label(label);
    this->_elseBlock->label(label);
}

void
IfStatement::buildAST(Painter *p, void *e, bool a) const
{
    string label = "if";
    if (a) label += " " + Base::int2string(this->label());
    PNode ifNode = Painter::newNode(p, label, 1);
    Painter::newEdge(p, (PNode)e, ifNode, "", 1);
    PNode ifTest = Painter::newNode(p, "[[TEST]]", 1);
    Painter::newEdge(p, ifNode, ifTest, "", 1);
    this->_exprBlock->buildAST(p, ifTest, a);
    PNode ifBody = Painter::newNode(p, "[[IF]]", 1);
    Painter::newEdge(p, ifNode, ifBody, "", 1);
    this->_ifBlock->buildAST(p, ifBody, a);
    PNode elseBody = Painter::newNode(p, "[[ELSE]]", 1);
    Painter::newEdge(p, ifNode, elseBody, "", 1);
    this->_elseBlock->buildAST(p, elseBody, a);
}

void *
IfStatement::buildCFG(Painter *p)
{
    string label = "if " + this->_exprBlock->str();
    PNode ifNode = Painter::newNode(p, label, 1);
    /* if body */
    for (Statement *s : this->_ifBlock->_statements) {
        this->ifNodes.push_back(s->buildCFG(p));
    }
    PNode fIf = (PNode)*ifNodes.begin();
    PNode lIf = (PNode)*ifNodes.rbegin();
    Painter::newEdge(p, ifNode, fIf, "", 1);
    PNode prev = ifNode;
    for (auto cur : ifNodes) {
        Painter::newEdge(p, prev, (PNode)cur, "", 1);
        prev = (PNode)cur;
    }
    /* else body */
    for (Statement *s : this->_elseBlock->_statements) {
        this->elseNodes.push_back((PNode)s->buildCFG(p));
    }
    prev = ifNode;
    for (auto cur : elseNodes) {
        Painter::newEdge(p, prev, (PNode)cur, "", 1);
        prev = (PNode)cur;
    }
    PNode lElse = (PNode)*this->elseNodes.rbegin();
    PNode endNode = Painter::newNode(p, "", 1);
    Painter::newEdge(p, lIf, endNode, "", 1);
    Painter::newEdge(p, lElse, endNode, "", 1);

    return ifNode;
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
    this->_meta = "while";
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
WhileStatement::label(int &label)
{
    this->_label = ++label;
    this->_exprBlock->label(label);
    this->_bodyBlock->label(label);
}

void
WhileStatement::buildAST(Painter *p, void *e, bool a) const
{
    string label = "while";
    if (a) label += " " + Base::int2string(this->label());
    PNode whileNode = Painter::newNode(p, label, 1);
    Painter::newEdge(p, (PNode)e, whileNode, "", 1);
    PNode test = Painter::newNode(p, "[[TEST]]", 1);
    Painter::newEdge(p, whileNode, test, "", 1);
    this->_exprBlock->buildAST(p, test, a);
    PNode body = Painter::newNode(p, "[[BODY]]", 1);
    Painter::newEdge(p, whileNode, body, "", 1);
    this->_bodyBlock->buildAST(p, body, a);
}

void *
WhileStatement::buildCFG(Painter *p)
{
    string label = "while " + this->_exprBlock->str();
    PNode whileNode = Painter::newNode(p, label, 1);
    /* body */
    vector<PNode> nodes;
    for (Statement *s : this->_bodyBlock->_statements) {
        nodes.push_back((PNode)s->buildCFG(p));
    }
    PNode prev = whileNode;
    for (auto cur : nodes) {
        Painter::newEdge(p, prev, cur, "", 1);
        prev = cur;
    }
    PNode exitNode = *nodes.rbegin();
    Painter::newEdge(p, exitNode, whileNode, "", 1);

    return whileNode;
}
