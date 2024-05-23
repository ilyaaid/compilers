#pragma once

#include <vector>
#include <set>
#include <string>

#include "analyzer/node.hpp"

struct Rule {
    bool axiom;
    Token nterm;
    std::vector<std::vector<Token>> next;
};

class TableGenerator {
public:
    TableGenerator(const std::shared_ptr<Node>& root): root(root) {}

    void generate(std::ostream& out);

private:
    void _genetateFile(std::ostream& out);

    void _findFirst(const std::vector<Rule>& rules);
    std::set<Token> _findFirstForRule(const std::vector<Token>& tokens);
    void _findFollow(const std::vector<Rule>& rules);
    void _findPredictTable(const std::vector<Rule>& rules);

    void _checkRules(const std::vector<Rule>& rules);

    void _readRules(const std::shared_ptr<Node>& node, std::vector<Rule>& rules);
    Rule _readRule(const std::shared_ptr<Node>& node);
    bool _readAxiom(const std::shared_ptr<Node>& node);
    Token _readNTerm(const std::shared_ptr<Node>& node);
    std::vector<std::vector<Token>> _readNTermRule(const std::shared_ptr<Node>& node);
    std::vector<Token> _readArgOR(const std::shared_ptr<Node>& node, std::vector<Token>& tokens);
    void _readNTermRuleTail(const std::shared_ptr<Node>& node, std::vector<std::vector<Token>>& argsOR);
    Token _readSymArgOR(const std::shared_ptr<Node>& node);

    std::set<Token> leftNTerms;
    std::set<Token> rightNTerms;

    const Token EPSILON = Token('e', "EPSILON", {});
    const Token END = Token(0, "", {});
    std::unordered_map<Token, std::set<Token>, TokenHash> first;
    std::unordered_map<Token, std::set<Token>, TokenHash> follow;

    std::unordered_map<Token, 
    std::unordered_map<Token, std::vector<Token>, 
    TokenHash>, 
    TokenHash> predict_table;

    Token axiom;

    std::shared_ptr<Node> root = nullptr;
};
