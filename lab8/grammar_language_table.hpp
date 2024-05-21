#pragma once 
#include <vector>
#include <unordered_map>
#include "token.hpp" 

enum NON_TERM : int 
{ 
	// все терминалы имеют тип char, а значит значение < 256
	NON_TERM_ArgOR =256,
	NON_TERM_Axiom =257,
	NON_TERM_NTerm =258,
	NON_TERM_NTermRule =259,
	NON_TERM_NTermRuleTail =260,
	NON_TERM_Rule =261,
	NON_TERM_Rules =262,
	NON_TERM_SymArgOR =263,
	ROOT,
};

const NON_TERM Axiom = NON_TERM_Rules;

const std::unordered_map<int, const std::unordered_map<DOMAIN_TAG, std::vector<int>>> table {
	{ NON_TERM_ArgOR,{
		{')', {}},
		{'s', {NON_TERM_SymArgOR, NON_TERM_ArgOR}},
		{'i', {NON_TERM_SymArgOR, NON_TERM_ArgOR}},
	}},
	{ NON_TERM_NTermRuleTail,{
		{'i', {}},
		{'*', {}},
		{0, {}},
		{'(', {'(', NON_TERM_ArgOR, ')', NON_TERM_NTermRuleTail}},
	}},
	{ NON_TERM_NTermRule,{
		{'(', {'(', NON_TERM_ArgOR, ')', NON_TERM_NTermRuleTail}},
	}},
	{ NON_TERM_NTerm,{
		{'i', {'i'}},
	}},
	{ NON_TERM_SymArgOR,{
		{'i', {NON_TERM_NTerm}},
		{'s', {'s'}},
	}},
	{ NON_TERM_Rule,{
		{'i', {NON_TERM_Axiom, NON_TERM_NTerm, NON_TERM_NTermRule}},
		{'*', {NON_TERM_Axiom, NON_TERM_NTerm, NON_TERM_NTermRule}},
	}},
	{ NON_TERM_Axiom,{
		{'i', {}},
		{'*', {'*'}},
	}},
	{ NON_TERM_Rules,{
		{0, {}},
		{'i', {NON_TERM_Rule, NON_TERM_Rules}},
		{'*', {NON_TERM_Rule, NON_TERM_Rules}},
	}},
};
