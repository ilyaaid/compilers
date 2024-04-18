#pragma once

#include "node.hpp"
#include "token.hpp"

#include <vector>
#include <set>

class Rule {
public:
    virtual std::vector<std::shared_ptr<Node>> parse() = 0;
    virtual std::set<DOMAIN_TAG> find_first() = 0;
};

//======================================

class TermRule : public Rule {
public:
    TermRule(DOMAIN_TAG tag): tag(tag) {}
    std::vector<std::shared_ptr<Node>> parse() override;
    std::set<DOMAIN_TAG> find_first() override;

private:
    DOMAIN_TAG tag;
};

//======================================

class NTermRule : public Rule {
public:
    NTermRule(NON_TERM nterm): nterm(nterm) {}
    std::vector<std::shared_ptr<Node>> parse() override;
    std::set<DOMAIN_TAG> find_first() override;

private:
    NON_TERM nterm;
};

//======================================

class ConcRule : public Rule {
public:
    ConcRule(const std::vector<std::shared_ptr<Rule>>& rules): rules(rules) {}
    std::vector<std::shared_ptr<Node>> parse() override;
    std::set<DOMAIN_TAG> find_first() override;

private:
    std::vector<std::shared_ptr<Rule>> rules;
};

//======================================

class AltRule : public Rule {
public:
    AltRule(const std::vector<std::shared_ptr<Rule>>& rules): rules(rules) {}
    std::vector<std::shared_ptr<Node>> parse() override;
    std::set<DOMAIN_TAG> find_first() override;

private:
    std::vector<std::shared_ptr<Rule>> rules;
};


//=====================================

class PlusRule : public Rule {
public:
    PlusRule(const std::shared_ptr<Rule>& rule): rule(rule) {}
    std::vector<std::shared_ptr<Node>> parse() override;
    std::set<DOMAIN_TAG> find_first() override;

private:
    std::shared_ptr<Rule> rule;
};
