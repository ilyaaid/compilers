#include "node.hpp"

// TagORNTerm::TagORNTerm(DOMAIN_TAG tag)
// {
//     this->tag = tag;
//     is_tag = true;
// }

// TagORNTerm::TagORNTerm(NON_TERM nterm)
// {
//     this->nterm = nterm;
//     is_tag = false;
// }

NTermNode::NTermNode(NON_TERM nterm): nterm(nterm) {}

//TODO
const std::unordered_map<NON_TERM, std::string> NTermNode::nterm2string = {
    {Program, "Program"},
    {FuncD, "FuncD"},
    {FuncDParams, "FuncDParams"},
    {FuncDParam, "FuncDParam"},
    {Statement, "Statement"},
    {IfStatement, "IfStatement"},
    {Expr, "Expr"},
    {Exprs, "Exprs"},
    {Var, "Var"},

    {Type, "Type"},
    {Types, "Types"},

    {NumberLit, "NumberLit"},
    {BoolLit, "BoolLit"},
    {TupleLit, "TupleLit"},
    {ListLit, "ListLit"},

    {ArithExpr, "ArithExpr"},
    {ArithT1, "ArithT1"},
    {ArithT1, "ArithT2"},
    {ArithT3, "ArithT3"},

    {LogicExpr, "LogicExpr"},
    {LogicT1, "LogicT1"},
    {LogicT2, "LogicT2"},
    {LogicT3, "LogicT3"},
};

LeafNode::LeafNode(const Token& token): token(token) {}

//===============================  DOT

void Node::printDOT(std::ostream& out) {
    out << "digraph G {" << std::endl;
    int n = 0;
    printDOT_(out, n);
    out << "}" << std::endl;
}

void NTermNode::printDOT_(std::ostream& out, int& n)
{
    out << n << " [label=\"" << nterm2string.at(nterm) << "\"]" << std::endl;
    int parent = n;
    std::vector<int> children_nums;
    for (size_t i = 0; i < children.size(); ++i)
    {
        n++;
        out << parent << "->" << n << std::endl;
        children_nums.push_back(n);
        children[i]->printDOT_(out, n);
    }

    if (children_nums.size() > 1)
    {
        out << "{ rank=same; ";
        out << children_nums[0];
        for (size_t i = 1; i < children_nums.size(); ++i)
        {
            out << " -> " << children_nums[i];
        }
        out << " [style=invis] }" << std::endl;
    }
}

void LeafNode::printDOT_(std::ostream& out, int& n)
{

    out << n << " [label=\"";
    if (token.tag2string[token.tag] == token.attr) {
        out << token.attr;
    }
    else {
        out << token.tag2string[token.tag] << ":" << token.attr;
    }
    out << "\"]" << std::endl;
}

//===============================  print

void NTermNode::print(int s)
{
    for (int i = 0; i < s; ++i)
    {
        std::cout << "------";
    }
    std::cout << nterm2string.at(nterm) << std::endl;
    for (size_t i = 0; i < children.size(); ++i)
    {
        children[i]->print(s + 1);
    }
}

void LeafNode::print(int s)
{
    for (int i = 0; i < s; ++i)
    {
        std::cout << "------";
    }
    std::cout << token << " " << std::endl;
}
