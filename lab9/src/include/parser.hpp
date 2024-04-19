#pragma once

#include "token.hpp"
#include "node.hpp"
#include "rule.hpp"
#include "scanner.hpp"

#include <vector>
#include <memory>
#include <unordered_map>

struct ParseNode
{
    // TagORNTerm tagnterm;
    std::shared_ptr<NTermNode> node;
};

class Parser
{

public:
    static void setParameters(const std::shared_ptr<Scanner>& sc) {
        scanner = sc;
    }

    static Parser& getInstance() {
        static Parser instance(scanner);
        return instance;
    }

private:
    Parser(const std::shared_ptr<Scanner>& sc): sc(sc) {};

public:
    std::shared_ptr<Node> parse();

    std::shared_ptr<Scanner> sc;

public:
    static std::shared_ptr<Scanner> scanner;
    static const std::unordered_map<NON_TERM, std::shared_ptr<Rule>> rules;
};
