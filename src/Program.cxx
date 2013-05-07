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
vset
Node::getvs(void)
{
    vset n;
    if (this->l) {
        vset t = this->l->getvs();
        n.insert(t.begin(), t.end());
    }
    if (this->r) {
        vset t = this->r->getvs();
        n.insert(t.begin(), t.end());
    }
    return n;
}

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
AssignmentExpression::str(bool a) const
{
    string out;
    out = this->l->str(a);
    out += " = ";
    out += this->r->str(a);
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

void
AssignmentExpression::cfgPrep(Painter *p)
{
    this->_cfgnode = Painter::newNode(p, this->str(false), 1);
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
ArithmeticExpression::str(bool a) const
{
    string out;
    out = this->l->str(a);
    out += " " + this->_op + " ";
    out += this->r->str(a);
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
LogicalExpression::str(bool a) const
{
    string out;
    out = this->l->str(a);
    out += " " + this->_op + " ";
    out += this->r->str(a);
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

void
LogicalExpression::cfgPrep(Painter *p)
{
    this->_cfgnode = Painter::newNode(p, this->str(false), 1);
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
Statement::Statement(Expression *expression)
{
    this->_exprStatement = false;
    this->_expr = expression;
}

string
Statement::str(bool a) const
{
    unsigned realPadLen = this->_exprStatement ? 0 : this->depth();
    string out = "";
    if (a) {
        out += Base::pad(realPadLen) + "[";
    }
    out += this->_expr->str(a);
    if (a) {
        out += "] -- " + Base::int2string(this->label());
    }
    if (!this->_exprStatement) {
        out += "\n";
    }
    return out;
}

void
Statement::cfgStitch(Painter *p, void *in, void **out)
{
    Painter::newEdge(p, (PNode)in, (PNode)this->cfgnode(), "", 1);
    *out = this->cfgnode();
}

vset
Statement::getvs(void)
{
    return this->_expr->getvs();
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
Block::str(bool a) const
{
    string out = "";
    for (Statement *s : this->_statements) {
        out += s->str(a);
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

void
Block::drawASTs(std::string fprefix, std::string type)
{
    for (auto i = 0; i < Block::ndias; ++i) {
        string fname = fprefix + "-" + Block::diaNames[i];
        /* this is the top-level call, so construct the painter */
        Painter *painter = new Painter(fname, type);
        /* start the drawing process */
        PNode n = Painter::newNode(painter, "[[PROGRAM]]", 1);
        this->buildAST(painter, n, (Block::diaNames[i] == "dast"));
        /* render the thing -- is that even the correct term? */
        cout << "> -- writing " + fname + "." + type + " ... "; cout.flush();
        painter->renderAST();
        cout << "done" << endl;
        delete painter;
    }
}

void
Block::drawCFG(std::string fprefix, std::string type)
{
    string fname = fprefix + "-" + "cfg";
    /* this is the top-level call, so construct the painter */
    Painter *painter = new Painter(fname, type);
    /* start the drawing process */
    PNode n = Painter::newNode(painter, "[[PROGRAM]]", 1);
    PNode e = Painter::newNode(painter, "[[PROGRAM END]]", 1);
    PNode o = NULL;
    this->cfgPrep(painter);
    this->cfgStitch(painter, n, (void **)&o);
    Painter::newEdge(painter, o, e, "", 1);
    /* render the thing -- is that even the correct term? */
    cout << "> -- writing " + fname + "." + type + " ... "; cout.flush();
    painter->renderAST();
    cout << "done" << endl;
    delete painter;
}

void
Block::cfgPrep(Painter *p)
{
    for (Statement *s : this->_statements) {
        s->cfgPrep(p);
    }
}

vset
Block::getvs(void)
{
    vset n;
    for (Statement *s : this->_statements) {
        vset t = s->getvs();
        n.insert(t.begin(), t.end());
    }
    return n;
}

void
Block::cfgStitch(Painter *p, void *in, void **out)
{
    PNode ine = NULL, oute = NULL;
    bool first = true;
    for (Statement *s : this->_statements) {
        if (first) {
            s->cfgStitch(p, in, (void **)&oute);
            first = false;
            ine = oute;
        }
        else {
            s->cfgStitch(p, ine, (void **)&oute);
            ine = oute;
        }
    }
    *out = oute;
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

string
Skip::str(bool a) const
{
    std::string out = "";
    if (a) {
        out += Base::pad(this->depth()) + "[";
    }
    out += "skip";
    if (a) {
        out += "] -- " + Base::int2string(this->label()) + "\n";
    }
    return out;
}

void
Skip::cfgPrep(Painter *p)
{
    this->_cfgnode = Painter::newNode(p, this->str(false), 1);
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

string
IfStatement::str(bool a) const
{
    string out = Base::pad(this->depth()) + "if ";
    out += this->_exprBlock->str(a) + " then\n";
    out += this->_ifBlock->str(a);
    out += Base::pad(this->depth()) + "else\n";
    out += this->_elseBlock->str(a);
    out += Base::pad(this->depth()) + "fi\n";
    return out;
}

void
IfStatement::cfgPrep(Painter *p)
{
    this->_cfgnode = Painter::newNode(p, "if " +
                     this->_exprBlock->str(false), 1);
    this->_ifBlock->cfgPrep(p);
    this->_elseBlock->cfgPrep(p);
}

void
IfStatement::cfgStitch(Painter *p, void *in, void **out)
{
    Painter::newEdge(p, (PNode)in, (PNode)this->cfgnode(), "", 1);
    PNode ifOut = NULL, elseOut = NULL;
    this->_ifBlock->cfgStitch(p, (PNode)this->cfgnode(), (void **)&ifOut);
    this->_elseBlock->cfgStitch(p, (PNode)this->cfgnode(), (void **)&elseOut);
    PNode merge = Painter::newNode(p, "", 1);
    Painter::newEdge(p, ifOut, merge, "", 1);
    Painter::newEdge(p, elseOut, merge, "", 1);
    *out = merge;
}

vset
IfStatement::getvs(void)
{
    vset n, t;
    t = this->_exprBlock->getvs();
    n.insert(t.begin(), t.end());
    t = this->_ifBlock->getvs();
    n.insert(t.begin(), t.end());
    t = this->_elseBlock->getvs();
    n.insert(t.begin(), t.end());
    return n;
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
WhileStatement::str(bool a) const
{
    string out = Base::pad(this->depth()) + "while ";
    out += this->_exprBlock->str(a) + " do\n";
    out += this->_bodyBlock->str(a);
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

void
WhileStatement::cfgPrep(Painter *p)
{
    string label = "if " + this->_exprBlock->str(false);
    this->_cfgnode = Painter::newNode(p, label, 1); 
    this->_bodyBlock->cfgPrep(p);
}

void
WhileStatement::cfgStitch(Painter *p, void *in, void **out)
{
    Painter::newEdge(p, (PNode)in, (PNode)this->cfgnode(), "", 1);
    PNode wOut;
    this->_bodyBlock->cfgStitch(p, (PNode)this->cfgnode(), (void **)&wOut);
    Painter::newEdge(p, wOut, (PNode)this->cfgnode(), "", 1);
    PNode merge = Painter::newNode(p, "", 1);
    Painter::newEdge(p, (PNode)this->cfgnode(), merge, "", 1);
    *out = merge;
}

vset
WhileStatement::getvs(void)
{
    vset n, t;
    t = this->_exprBlock->getvs();
    n.insert(t.begin(), t.end());
    t = this->_bodyBlock->getvs();
    n.insert(t.begin(), t.end());
    return n;
}
