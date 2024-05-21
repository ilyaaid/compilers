#include "node.hpp"

NTermNode::NTermNode(NON_TERM nterm): nterm(nterm) {}

std::unordered_map<NON_TERM, std::string> NTermNode::nterm2string = {
    {NON_TERM_Rules, "Rules"},
    {NON_TERM_Rule, "Rule"},
    {NON_TERM_Axiom, "Axiom"},
    {NON_TERM_NTerm, "NTerm"},
    {NON_TERM_NTermRule, "NTermRule"},
    {NON_TERM_NTermRuleTail, "NTermRuleTail"},
    {NON_TERM_ArgOR, "ArgOR"},
    {NON_TERM_SymArgOR, "SymArgOR"},
    {ROOT, "ROOT"},
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
    out << n << " [label=\"" << nterm2string[nterm] << "\"]" << std::endl;
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
    out << n << " [label=\"" << token.tag << "\"]" << std::endl;
}

//===============================  print

void NTermNode::print(int s)
{
    for (int i = 0; i < s; ++i)
    {
        std::cout << "------";
    }
    std::cout << nterm2string[nterm] << std::endl;
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
