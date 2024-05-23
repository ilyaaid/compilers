#pragma once 

#include "analyzer/node.hpp"

class Calculator {
public:
    Calculator(const std::shared_ptr<Node>& root): root(root) {}

    int calculate();
private:
    int _calcE(const std::shared_ptr<Node>& node);
    int _calcT(const std::shared_ptr<Node>& node);
    void _calcE_(int& num, const std::shared_ptr<Node>& node);
    void _calcT_(int& num, const std::shared_ptr<Node>& node);
    int _calcF(const std::shared_ptr<Node>& node);

    std::shared_ptr<Node> root = nullptr;
};
