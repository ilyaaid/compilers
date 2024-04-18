#pragma once

#include "token.hpp"

#include <vector>
#include <memory>
#include <unordered_map>

enum NON_TERM : int
{
    Program,
    FuncD,
};

struct TagORNTerm
{
    DOMAIN_TAG tag;
    NON_TERM nterm;
    bool is_tag;

    TagORNTerm(DOMAIN_TAG tag);
    TagORNTerm(NON_TERM nterm);
};

struct Node
{
    virtual void print(int s) = 0;

    void printDOT(std::ostream& out);

    virtual void printDOT_(std::ostream& out_filename, int& n) = 0;
};

struct NTermNode : public Node
{
    NTermNode(NON_TERM nterm);

    void print(int s) override;
    // void printDOT(std::ostream &out) override;

    void printDOT_(std::ostream& out_filename, int& n) override;

public:
    NON_TERM nterm;
    std::vector<std::shared_ptr<Node>> children;

    static const std::unordered_map<NON_TERM, std::string> nterm2string;
};

struct LeafNode : public Node
{
    LeafNode(const Token& token);

    void print(int s) override;
    // void printDOT(std::ostream &out) override;

    void printDOT_(std::ostream& out_filename, int& n) override;

public:
    Token token;
};
