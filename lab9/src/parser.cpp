#include "parser.hpp"
#include "node.hpp"
#include "token.hpp"
#include "rule.hpp"

#include <tuple>
#include <stack>
#include <experimental/memory>

std::shared_ptr<Scanner> Parser::scanner = nullptr;

//TODO

#define TERM_RULE(param) std::make_shared<TermRule>(param)
#define NTERM_RULE(param) std::make_shared<NTermRule>(param)
#define CONC_RULE(...) std::make_shared<ConcRule>(std::vector<std::shared_ptr<Rule>>{__VA_ARGS__})
#define ALT_RULE(...) std::make_shared<AltRule>(std::vector<std::shared_ptr<Rule>>{__VA_ARGS__})
#define PLUS_RULE(param) std::make_shared<PlusRule>(param)

const std::unordered_map<NON_TERM, std::shared_ptr<Rule>> Parser::rules{
    {Program, PLUS_RULE(NTERM_RULE(FuncD))},
    {FuncD, ALT_RULE( CONC_RULE(TERM_RULE(TAG_IDENT), TERM_RULE(TAG_AND)), CONC_RULE(TERM_RULE(TAG_OR), TERM_RULE(TAG_AND)))},
};

/*
Program = FuncD+
FuncD = IDENT 'and' | 'or' 'and'
*/

std::shared_ptr<Node> Parser::parse() {
    // std::set<DOMAIN_TAG>first =  Parser::rules.at(Program)->find_first();
    return NTERM_RULE(Program)->parse()[0];
}

