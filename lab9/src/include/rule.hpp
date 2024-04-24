#pragma once

#include "node.hpp"
#include "token.hpp"

#include <vector>
#include <set>
#include <functional>

class Rule;

struct Alternative {
    std::shared_ptr<Rule> rule;
    std::function<NodesType(const NodesType&)> callback;
};


class Rule {
public:
    virtual ~Rule() {}
    virtual NodesType parse() = 0;
    virtual std::set<DOMAIN_TAG> find_first() = 0;
};

//======================================

class TermRule : public Rule {
public:
    TermRule(DOMAIN_TAG tag): tag(tag) {}
    NodesType parse() override;
    std::set<DOMAIN_TAG> find_first() override;

private:
    DOMAIN_TAG tag;
};

//======================================

class NTermRule : public Rule {
public:
    NTermRule(NON_TERM::TYPE nterm): nterm(nterm) {}
    NodesType parse() override;
    std::set<DOMAIN_TAG> find_first() override;

private:
    NON_TERM::TYPE nterm;
};

//======================================

class ConcRule : public Rule {
public:
    ConcRule(const std::vector<std::shared_ptr<Rule>>& rules): rules(rules) {}
    NodesType parse() override;
    std::set<DOMAIN_TAG> find_first() override;

private:
    std::vector<std::shared_ptr<Rule>> rules;
};

//======================================

// class AltRule : public Rule {
// public:
//     AltRule(const std::vector<std::shared_ptr<Rule>>& rules): rules(rules) {}
//     NodesType parse() override;
//     std::set<DOMAIN_TAG> find_first() override;

// private:
//     std::vector<std::shared_ptr<Rule>> rules;
// };

//=====================================

class StarRule : public Rule {
public:
    StarRule(const std::shared_ptr<Rule>& rule): rule(rule) {}
    NodesType parse() override;
    std::set<DOMAIN_TAG> find_first() override;

private:
    std::shared_ptr<Rule> rule;
};


//=====================================

class PlusRule : public Rule {
public:
    PlusRule(const std::shared_ptr<Rule>& rule): rule(rule) {}
    NodesType parse() override;
    std::set<DOMAIN_TAG> find_first() override;

private:
    std::shared_ptr<Rule> rule;
};

//=====================================

class OptRule : public Rule {
public:
    OptRule(const std::shared_ptr<Rule>& rule): rule(rule) {}
    NodesType parse() override;
    std::set<DOMAIN_TAG> find_first() override;

private:
    std::shared_ptr<Rule> rule;
};
