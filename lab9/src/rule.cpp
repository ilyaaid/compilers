#include "rule.hpp"
#include "parser.hpp"

#include <sstream>

//================== a ========

std::vector<std::shared_ptr<Node>> TermRule::parse() {
    if (tag == TAG_EMPTY) {
        return {};
    }
    Parser& parser = Parser::getInstance();
    Token t = parser.sc->cur_token;
    if (t.tag == tag) {
        std::cout << t << std::endl;
        parser.sc->nextToken();
        return { std::make_shared<LeafNode>(t) };
    }
    std::stringstream buffer;
    buffer << "Wrong tag\n\tReceived: " << t << std::endl;
    buffer << "\tExpected: " << Token::tag2string[tag] << std::endl;
    buffer << t;
    throw std::runtime_error(buffer.str());
}

std::set<DOMAIN_TAG> TermRule::find_first() {
    return { tag };
}

//================= X =======

std::vector<std::shared_ptr<Node>> NTermRule::parse() {
    auto children = Parser::rules.at(nterm)->parse();
    // TODO (убираю нетерминалы с одним ребенком и поднимаю ребенка наверх по дереву)
    if (children.size() == 1) {
        return {children[0]};
    }
    auto node = std::make_shared<NTermNode>(nterm);
    node->children = children;
    return { node };
}

std::set<DOMAIN_TAG> NTermRule::find_first() {
    return Parser::rules.at(nterm)->find_first();
}

//===============  R1R2 ==========

std::vector<std::shared_ptr<Node>> ConcRule::parse() {
    std::vector<std::shared_ptr<Node>> children;
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

std::vector<std::shared_ptr<Node>> AltRule::parse() {
    Parser& parser = Parser::getInstance();
    for (size_t i = 0; i < rules.size(); ++i) {
        auto child_first = rules[i]->find_first();
        if (child_first.find(parser.sc->cur_token.tag) != child_first.end()) {
            return rules[i]->parse();
        }
    }
    throw std::runtime_error("wrong tag in alt!");
}

std::set<DOMAIN_TAG> AltRule::find_first() {
    std::set<DOMAIN_TAG> first;
    for (size_t i = 0; i < rules.size(); ++i) {
        auto cur_first = rules[i]->find_first();
        std::set_union(first.begin(), first.end(),
            cur_first.begin(), cur_first.end(),
            std::inserter(first, first.begin()));
    }
    return first;
}

//================== R* =====

std::vector<std::shared_ptr<Node>> StarRule::parse() {
    Parser& parser = Parser::getInstance();
    auto first = rule->find_first();
    std::vector<std::shared_ptr<Node>> children;
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

std::vector<std::shared_ptr<Node>> PlusRule::parse() {
    Parser& parser = Parser::getInstance();
    auto first = rule->find_first();
    std::vector<std::shared_ptr<Node>> children;
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

std::vector<std::shared_ptr<Node>> OptRule::parse() {
    Parser& parser = Parser::getInstance();
    auto first = rule->find_first();
    std::vector<std::shared_ptr<Node>> children;
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

