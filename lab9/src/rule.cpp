#include "rule.hpp"
#include "parser.hpp"

#include <sstream>

//================== a ========

NodesType TermRule::parse() {
    if (tag == TAG_EMPTY) {
        return {};
    }
    Parser& parser = Parser::getInstance();
    Token t = parser.sc->cur_token;
    if (t.tag == tag) {
        std::cout << t << std::endl;
        parser.sc->nextToken();
        return { std::make_shared<Term>(t) };
    }
    std::stringstream buffer;
    buffer << "Wrong tag\n\tReceived: " << t << std::endl;
    buffer << "\tExpected: " << Token::tag2string[tag] << std::endl;
    throw std::runtime_error(buffer.str());
}

std::set<DOMAIN_TAG> TermRule::find_first() {
    return { tag };
}

//================= X =======

NodesType NTermRule::parse() {
    auto altrules = Parser::rules.at(nterm);
    Parser& parser = Parser::getInstance();
    for (size_t i = 0; i < altrules.size(); ++i) {
        auto rule = altrules[i].rule;
        auto child_first = rule->find_first();
        if (child_first.find(parser.sc->cur_token.tag) != child_first.end()) {
            return altrules[i].callback(rule->parse());
        }
    }
    throw std::runtime_error("Wrong tag in alternative!");
}

std::set<DOMAIN_TAG> NTermRule::find_first() {
    auto altrules = Parser::rules.at(nterm);
    std::set<DOMAIN_TAG> first;
    for (size_t i = 0; i < altrules.size(); ++i) {
        auto cur_first = altrules[i].rule->find_first();
        std::set_union(first.begin(), first.end(),
            cur_first.begin(), cur_first.end(),
            std::inserter(first, first.begin()));
    }
    return first;
}

//===============  R1R2 ==========

NodesType ConcRule::parse() {
    NodesType children;
    for (size_t i = 0; i < rules.size(); ++i) {
        auto child = rules[i]->parse();
        children.insert(children.end(), child.begin(), child.end());
    }
    return children;
}

std::set<DOMAIN_TAG> ConcRule::find_first() {
    std::set<DOMAIN_TAG> first;
    if (rules.size() > 0) {
        first = rules[0]->find_first();
        for (size_t i = 1; (i < rules.size()) && first.find(TAG_EMPTY) != first.end(); ++i) {
            first.erase(TAG_EMPTY);
            auto cur_first = rules[i]->find_first();
            std::set_union(first.begin(), first.end(),
                cur_first.begin(), cur_first.end(),
                std::inserter(first, first.begin()));
        }
    }
    return first;
}

//================== R1 | R2 =======

// NodesType AltRule::parse() {
//     Parser& parser = Parser::getInstance();
//     for (size_t i = 0; i < rules.size(); ++i) {
//         auto child_first = rules[i]->find_first();
//         if (child_first.find(parser.sc->cur_token.tag) != child_first.end()) {
//             return rules[i]->parse();
//         }
//     }
//     throw std::runtime_error("wrong tag in alt!");
// }

// std::set<DOMAIN_TAG> AltRule::find_first() {
//     std::set<DOMAIN_TAG> first;
//     for (size_t i = 0; i < rules.size(); ++i) {
//         auto cur_first = rules[i]->find_first();
//         std::set_union(first.begin(), first.end(),
//             cur_first.begin(), cur_first.end(),
//             std::inserter(first, first.begin()));
//     }
//     return first;
// }

//================== R* =====

NodesType StarRule::parse() {
    Parser& parser = Parser::getInstance();
    auto first = rule->find_first();
    NodesType children;
    while (first.find(parser.sc->cur_token.tag) != first.end()) {
        auto child = rule->parse();
        children.insert(children.end(), child.begin(), child.end());
    }
    return children;
}

std::set<DOMAIN_TAG> StarRule::find_first() {
    auto child_first = rule->find_first();
    child_first.insert(TAG_EMPTY);
    return child_first;
}


//================== R+ =====

NodesType PlusRule::parse() {
    Parser& parser = Parser::getInstance();
    auto first = rule->find_first();
    NodesType children;
    do {
        auto child = rule->parse();
        children.insert(children.end(), child.begin(), child.end());
    } while (first.find(parser.sc->cur_token.tag) != first.end());
    return children;
}

std::set<DOMAIN_TAG> PlusRule::find_first() {
    return rule->find_first();
}

//================== R? =====

NodesType OptRule::parse() {
    Parser& parser = Parser::getInstance();
    auto first = rule->find_first();
    NodesType children;
    if (first.find(parser.sc->cur_token.tag) != first.end()) {
        auto child = rule->parse();
        children.insert(children.end(), child.begin(), child.end());
    }
    return children;
}

std::set<DOMAIN_TAG> OptRule::find_first() {
    auto child_first = rule->find_first();
    child_first.insert(TAG_EMPTY);
    return child_first;
}

