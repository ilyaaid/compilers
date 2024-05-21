#include "table_generator.hpp"
#include <algorithm>

void TableGenerator::generate(std::ostream& out)
{
    std::vector<Rule> rules;
    _readRules(root, rules);
    _checkRules(rules);

    _findFirst(rules);
    _findFollow(rules);

    _findPredictTable(rules);

    _genetateFile(out);
}

void TableGenerator::_genetateFile(std::ostream& out)
{
    out << "#pragma once \n#include <vector>\n#include <unordered_map>\n#include \"token.hpp\" \n\n";
    out << "enum NON_TERM : int \n{ \n\t// все терминалы имеют тип char, а значит значение < 256\n";
    int nterm_value = 256;
    for (const auto& left_nterm : leftNTerms) {
        out << "\tNON_TERM_" << left_nterm.attr << " =" << nterm_value << "," << std::endl;
        nterm_value++;
    }
    out << "\tROOT,\n};\n\n";

    out << "const NON_TERM Axiom = NON_TERM_" << axiom.attr << ";\n\n";

    out << "const std::unordered_map<int, const std::unordered_map<DOMAIN_TAG, std::vector<int>>> table {\n";
    for (const auto& row : predict_table) {
        out << "\t{ NON_TERM_" << row.first.attr << ",{\n";
        for (const auto& term : row.second) {
            out << "\t\t{";
            if (term.first.tag == 0) {
                out << 0;
            }
            else {
                out << "\'" << term.first.attr << "\'";
            }
            out << ", {";
            for (size_t i = 0; i < term.second.size(); ++i) {
                if (term.second[i].tag == 'i') {
                    out << "NON_TERM_" << term.second[i].attr;
                }
                else {
                    out << "\'" << term.second[i].attr << "\'";
                }
                if (i < term.second.size() - 1) {
                    out << ", ";
                }
            }
            out << "}},\n";
        }

        out << "\t}},\n";
    }

    out << "};\n";
}

void TableGenerator::_findFirst(const std::vector<Rule>& rules)
{
    bool is_change = true;
    while (is_change) {
        is_change = false;
        for (const auto& rule : rules) {
            for (size_t j = 0; j < rule.next.size(); ++j) {
                auto first_for_next = _findFirstForRule(rule.next[j]);
                size_t old_size = first[rule.nterm].size();
                first[rule.nterm].insert(first_for_next.begin(), first_for_next.end());
                if (first[rule.nterm].size() > old_size) {
                    is_change = true;
                }
            }
        }
    }
}

std::set<Token> TableGenerator::_findFirstForRule(const std::vector<Token>& tokens)
{
    std::set<Token> first_for_rule;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].tag == 's') {
            first_for_rule.insert(tokens[i]);
            return first_for_rule;
        }
        else {
            first_for_rule.insert(first[tokens[i]].begin(), first[tokens[i]].end());
            if (first[tokens[i]].find(EPSILON) != first[tokens[i]].end()) {
                first_for_rule.erase(EPSILON);
            }
            else {
                return first_for_rule;
            }
        }
    }
    first_for_rule.insert(EPSILON);
    return first_for_rule;
}

void TableGenerator::_findFollow(const std::vector<Rule>& rules)
{
    follow[axiom].insert(END);
    bool is_change = true;
    while (is_change) {
        is_change = false;
        for (const auto& rule : rules) {
            for (size_t j = 0; j < rule.next.size(); ++j) {
                std::vector<Token> right_part = rule.next[j];
                for (size_t i = 0; i < right_part.size(); ++i) {
                    if (right_part[i].tag == 'i') {
                        size_t old_size = follow[right_part[i]].size();

                        std::vector<Token> suffix(right_part.begin() + i + 1, right_part.end());
                        auto first_for_suffix = _findFirstForRule(suffix);
                        follow[right_part[i]].insert(first_for_suffix.begin(), first_for_suffix.end());
                        follow[right_part[i]].erase(EPSILON);

                        if (first_for_suffix.find(EPSILON) != first_for_suffix.end()) {
                            follow[right_part[i]].insert(
                                follow[rule.nterm].begin(), follow[rule.nterm].end());
                        }

                        if (old_size < follow[right_part[i]].size()) {
                            is_change = true;
                        }
                    }
                }
            }
        }
    }
}

void TableGenerator::_findPredictTable(const std::vector<Rule>& rules)
{
    for (const auto& rule : rules) {
        for (size_t i = 0; i < rule.next.size(); ++i) {
            auto first_for_rule = _findFirstForRule(rule.next[i]);
            if (first_for_rule.find(EPSILON) != first_for_rule.end()) {
                first_for_rule.insert(follow[rule.nterm].begin(), follow[rule.nterm].end());
            }
            first_for_rule.erase(EPSILON);

            for (const auto& term : first_for_rule) {
                if (predict_table[rule.nterm].find(term) != predict_table[rule.nterm].end()) {
                    throw std::runtime_error("Grammar is not LL(1)");
                }
                predict_table[rule.nterm][term] = rule.next[i];
            }
        }
    }
}

void TableGenerator::_checkRules(const std::vector<Rule>& rules)
{
    // Количество аксиом
    int cntAxioms = 0;
    for (size_t i = 0; i < rules.size(); ++i) {
        cntAxioms += rules[i].axiom;
        if (rules[i].axiom) {
            axiom = rules[i].nterm;
        }
    }
    if (cntAxioms != 1) {
        throw std::runtime_error("wrong number of axioms");
    }

    // наличие нетерминального символа, не присутствующего в левой части ни одного правила
    std::set<Token> unionNTerms = leftNTerms;
    unionNTerms.insert(rightNTerms.begin(), rightNTerms.end());
    if (leftNTerms.size() < unionNTerms.size()) {
        throw std::runtime_error("there is non terminal without a rule");
    }
}

void TableGenerator::_readRules(const std::shared_ptr<Node>& node, std::vector<Rule>& rules)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: Rules");
    }
    if (ntermnode->children.size() > 0) {
        rules.push_back(_readRule(ntermnode->children[0]));
        _readRules(ntermnode->children[1], rules);
    }
}

Rule TableGenerator::_readRule(const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: Rule");
    }

    Token nterm = _readNTerm(ntermnode->children[1]);
    leftNTerms.insert(nterm);
    Rule rule{
        _readAxiom(ntermnode->children[0]),
        nterm,
        _readNTermRule(ntermnode->children[2])
    };
    return rule;
}

bool TableGenerator::_readAxiom(const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: Axiom");
    }
    return ntermnode->children.size() == 1;
}

Token TableGenerator::_readNTerm(const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: NTerm");
    }

    auto ident = std::dynamic_pointer_cast<LeafNode>(ntermnode->children[0]);
    if (!ident) {
        throw std::runtime_error("expected: LeafNode");
    }
    return ident->token;
}

std::vector<std::vector<Token>> TableGenerator::_readNTermRule(const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: NTermRule");
    }
    std::vector<std::vector<Token>> argsOR;
    std::vector<Token> tokens;
    _readArgOR(ntermnode->children[1], tokens);
    argsOR.push_back(tokens);
    _readNTermRuleTail(ntermnode->children[3], argsOR);
    return argsOR;
}

std::vector<Token> TableGenerator::_readArgOR(const std::shared_ptr<Node>& node, std::vector<Token>& tokens)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: ArgOR");
    }
    if (ntermnode->children.size() > 0) {
        tokens.push_back(_readSymArgOR(ntermnode->children[0]));
        _readArgOR(ntermnode->children[1], tokens);
    }
    return tokens;
}



void TableGenerator::_readNTermRuleTail(const std::shared_ptr<Node>& node,
    std::vector<std::vector<Token>>& argsOR)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: NTermRuleTail");
    }
    if (ntermnode->children.size() > 0) {
        std::vector<Token> tokens;
        _readArgOR(ntermnode->children[1], tokens);
        argsOR.push_back(tokens);
        _readNTermRuleTail(ntermnode->children[3], argsOR);
    }
}

Token TableGenerator::_readSymArgOR(const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: SymArgOR");
    }
    auto child = std::dynamic_pointer_cast<NTermNode>(ntermnode->children[0]);
    if (child) {
        Token token = _readNTerm(child);
        rightNTerms.insert(token);
        return token;
    }
    auto child2 = std::dynamic_pointer_cast<LeafNode>(ntermnode->children[0]);
    return child2->token;
}




