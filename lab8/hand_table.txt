#pragma once
#include <vector>
#include <unordered_map>

#include "token.hpp"

enum NON_TERM : int
{
  // все терминалы имеют тип char, а значит значение < 256
  NON_TERM_Rules = 256,
  NON_TERM_Rule = 257,
  NON_TERM_Axiom = 258,
  NON_TERM_NTerm = 259,
  NON_TERM_NTermRule = 260,
  NON_TERM_NTermRuleTail = 261,
  NON_TERM_ArgOR = 262,
  NON_TERM_SymArgOR = 263,

  ROOT,
};

const NON_TERM Axiom = NON_TERM_Rules;


const std::vector<std::vector<int>> rules{
      {NON_TERM_Rule, NON_TERM_Rules},
      {},
      {NON_TERM_Axiom, NON_TERM_NTerm, NON_TERM_NTermRule},
      {'*'},
      {},
      {'i'},
      {'(', NON_TERM_ArgOR, ')', NON_TERM_NTermRuleTail},
      {'(', NON_TERM_ArgOR, ')', NON_TERM_NTermRuleTail},
      {},
      {NON_TERM_SymArgOR, NON_TERM_ArgOR},
      {},
      {'s'},
      {NON_TERM_NTerm},
};

const std::unordered_map<int, const std::unordered_map<DOMAIN_TAG, std::vector<int>>> table{
    {NON_TERM_Rules,
     {{'i', rules[0]},
      {'*', rules[0]},
      {0, rules[1]}}},
    {NON_TERM_Rule,
     {{'i', rules[2]},
      {'*', rules[2]}}},
    {NON_TERM_Axiom,
     {{'*', rules[3]},
      {'i', rules[4]}}},
    {NON_TERM_NTerm,
     {{'i', rules[5]}}},
    {NON_TERM_NTermRule,
     {{'(', rules[6]}},
     },
    {NON_TERM_NTermRuleTail,
     {{'i', rules[8]},
      {'*', rules[8]},
      {'(', rules[7]},
      {0, rules[8]}}},
    {NON_TERM_ArgOR,
     {{'i', rules[9]},
      {'s', rules[9]},
      {')', rules[10]}}},
    {NON_TERM_SymArgOR,
     {{'i', rules[12]},
      {'s', rules[11]}}}
};
