#pragma once

#include "token.hpp"
#include "node.hpp"

#include <vector>
#include <memory>
#include <unordered_map>

struct ParseNode
{
  TagORNTerm tagnterm;
  std::shared_ptr<NTermNode> node;
};

class Parser
{
public:
  Parser(const std::vector<Token> &tokens);

  std::shared_ptr<Node> parse();

private:
  std::vector<Token> tokens;
  const std::vector<std::vector<TagORNTerm>> rules{
      {Rule, Rules},
      {},
      {Axiom, NTerm, NTermRule},
      {TAG_STAR},
      {},
      {TAG_IDENT},
      {TAG_LPAREN, ArgOR, TAG_RPAREN, NTermRuleTail},
      {TAG_LPAREN, ArgOR, TAG_RPAREN},
      {},
      {SymArgOR, ArgOR},
      {},
      {TAG_STRING},
      {NTerm},
  };
  std::unordered_map<NON_TERM, std::unordered_map<DOMAIN_TAG, std::vector<TagORNTerm>>> table{
      {Rules,
       {{TAG_IDENT, rules[0]},
        {TAG_STAR, rules[0]},
        {TAG_END, rules[1]}}},
      {Rule,
       {{TAG_IDENT, rules[2]},
        {TAG_STAR, rules[2]}}},
      {Axiom,
       {{TAG_IDENT, rules[4]},
        {TAG_STAR, rules[3]}}},
      {NTerm,
       {{TAG_IDENT, rules[5]}}},
      {NTermRule,
       {{TAG_LPAREN, rules[6]}}},
      {NTermRuleTail,
       {{TAG_IDENT, rules[8]},
        {TAG_STAR, rules[8]},
        {TAG_LPAREN, rules[7]},
        {TAG_END, rules[8]}}},
      {ArgOR,
       {{TAG_IDENT, rules[9]},
        {TAG_STRING, rules[9]},
        {TAG_RPAREN, rules[10]}}},
      {SymArgOR,
       {{TAG_IDENT, rules[12]},
        {TAG_STRING, rules[11]}}}};
};
