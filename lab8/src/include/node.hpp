#pragma once


#include "token.hpp"
#include "table.hpp"

#include <vector>
#include <memory>
#include <unordered_map>


struct Node
{
    virtual void print(int s) = 0;

    void printDOT(std::ostream &out);

    virtual void printDOT_(std::ostream &out_filename, int &n) = 0;
};

struct NTermNode : public Node
{
    NTermNode(NON_TERM nterm);

    void print(int s) override;
    // void printDOT(std::ostream &out) override;

    void printDOT_(std::ostream &out_filename, int &n) override;

public:
    NON_TERM nterm;
    std::vector<std::shared_ptr<Node>> children;

    static std::unordered_map<NON_TERM, std::string> nterm2string;
};

struct LeafNode : public Node
{
    LeafNode(const Token &token);

    void print(int s) override;
    // void printDOT(std::ostream &out) override;

    void printDOT_(std::ostream &out_filename, int &n) override;

public:
    Token token;
};
