#include "node.hpp"



void Term::print(std::ostream& out, int s)
{
    std::string offset(s * 5, ' ');
    out << offset << "Term(" << t.attr << ")" << std::endl;
}

void Program::print(std::ostream& out, int s)
{
    std::string offset(s * 5, ' ');
    out << offset << "Program(" << std::endl;
    for (const auto& i : items) {
        i->print(out, s + 1);
    }
    out << offset << ")" << std::endl;
}

void FuncD::print(std::ostream& out, int s)
{
    std::string offset(s * 5, ' ');
    out << offset << "FuncD(" << name << std::endl;
    params->print(out, s + 1);
    return_type->print(out, s + 1);
    expr->print(out, s + 1);
    out << offset << ")" << std::endl;
}

void FuncDParam::print(std::ostream& out, int s)
{
    std::string offset(s * 5, ' ');
    out << offset << "FuncDParam(" << name << std::endl;
    type->print(out, s + 1);
    out << offset << ")" << std::endl;
}

void FuncDParams::print(std::ostream& out, int s)
{
    std::string offset(s * 5, ' ');
    out << offset << "FuncDParams(" << std::endl;
    for (const auto& i : items) {
        i->print(out, s + 1);
    }
    out << offset << ")" << std::endl;
}


void IfStatement::print(std::ostream& out, int s)
{
    std::string offset(s * 5, ' ');
    out << offset << "IfStatement(" << std::endl;
    cond->print(out, s + 1);
    true_br->print(out, s + 1);
    false_br->print(out, s + 1);
    out << offset << ")" << std::endl;
}

void Var::print(std::ostream& out, int s)
{
    std::string offset(s * 5, ' ');
    out << offset << "Var(" << val->t.attr << ")" << std::endl;
}

void FuncCall::print(std::ostream& out, int s)
{
    std::string offset(s * 5, ' ');
    out << offset << "FuncCall(" << val->t.attr << std::endl;
    for (auto& p : params) {
        p->print(out, s + 1);
    }
    out << offset << ")" << std::endl;
}


void SimpleType::print(std::ostream& out, int s)
{
    std::string offset(s * 5, ' ');
    out << offset << "SimpleType(" << val << ")" << std::endl;
}

void ListType::print(std::ostream& out, int s)
{
    std::string offset(s * 5, ' ');
    out << offset << "ListType(" << std::endl;
    type->print(out, s + 1);
    out << offset << ")" << std::endl;
}

void TupleType::print(std::ostream& out, int s)
{
    std::string offset(s * 5, ' ');
    out << offset << "TupleType(" << std::endl;
    for (auto& i : items) {
        i->print(out, s + 1);
    }
    out << offset << ")" << std::endl;
}

void TupleLit::print(std::ostream& out, int s)
{
    std::string offset(s * 5, ' ');
    out << offset << "TupleLit(" << std::endl;
    for (auto& i : exprs) {
        i->print(out, s + 1);
    }
    out << offset << ")" << std::endl;
}

void ListLit::print(std::ostream& out, int s)
{
    std::string offset(s * 5, ' ');
    out << offset << "ListLit(" << std::endl;
    for (auto& i : exprs) {
        i->print(out, s + 1);
    }
    out << offset << ")" << std::endl;
}

void Num::print(std::ostream& out, int s)
{
    std::string offset(s * 5, ' ');
    out << offset << "Num(" << num << ")" << std::endl;
}

void BoolNum::print(std::ostream& out, int s)
{
    std::string offset(s * 5, ' ');
    out << offset << "Boolnum(" << (num ? "true" : "false") << ")" << std::endl;
}


void BinOp::print(std::ostream& out, int s)
{
    std::string offset(s * 5, ' ');
    out << offset << "BinOp(" << std::endl;
    expr1->print(out, s + 1);
    op->print(out, s + 1);
    expr2->print(out, s + 1);
    out << offset << ")" << std::endl;
}

void UnOp::print(std::ostream& out, int s)
{
    std::string offset(s * 5, ' ');
    out << offset << "UnOp(" << std::endl;
    op->print(out, s + 1);
    expr->print(out, s + 1);
    out << offset << ")" << std::endl;
}
