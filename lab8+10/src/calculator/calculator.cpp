#include "calculator.hpp"

int Calculator::calculate()
{
    return _calcE(root);
}

int Calculator::_calcE(const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: E");
    }
    int num = _calcT(ntermnode->children[0]);
    _calcE_(num, ntermnode->children[1]);
    return num;
}

int Calculator::_calcT(const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: T");
    }
    int num = _calcF(ntermnode->children[0]);
    _calcT_(num, ntermnode->children[1]);
    return num;
}

void Calculator::_calcE_(int& num, const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: E'");
    }
    if (ntermnode->children.size() > 0) {
        auto leafnode = std::dynamic_pointer_cast<LeafNode>(ntermnode->children[0]);
        int numT = _calcT(ntermnode->children[1]);
        if (leafnode->token.tag == '+') {
            num += numT;
        }
        _calcE_(num, ntermnode->children[2]);
    }
}

void Calculator::_calcT_(int& num, const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: T'");
    }
    if (ntermnode->children.size() > 0) {
        auto leafnode = std::dynamic_pointer_cast<LeafNode>(ntermnode->children[0]);
        int numT = _calcF(ntermnode->children[1]);
        if (leafnode->token.tag == '*') {
            num *= numT;
        }
        _calcT_(num, ntermnode->children[2]);
    }
}

int Calculator::_calcF(const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: F");
    }

    if (ntermnode->children.size() == 1) {
        auto leafnode = std::dynamic_pointer_cast<LeafNode>(ntermnode->children[0]);
        int num = atoi(leafnode->token.attr.c_str());
        return num;
    } else {
        return _calcE(ntermnode->children[1]);
    }
    return 0;
}
