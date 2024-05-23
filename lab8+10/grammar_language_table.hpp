#pragma once 
#include <vector>
#include <unordered_map>
#include "token.hpp" 

enum NON_TERM : int 
{ 
	// все терминалы имеют тип char, а значит значение < 256
	NON_TERM_E =256,
	NON_TERM_E_ =257,
	NON_TERM_F =258,
	NON_TERM_T =259,
	NON_TERM_T_ =260,
	ROOT,
};

const NON_TERM Axiom = NON_TERM_E;

const std::unordered_map<int, const std::unordered_map<DOMAIN_TAG, std::vector<int>>> table {
	{ NON_TERM_T_,{
		{'+', {}},
		{')', {}},
		{0, {}},
		{'*', {'*', NON_TERM_F, NON_TERM_T_}},
	}},
	{ NON_TERM_E,{
		{'n', {NON_TERM_T, NON_TERM_E_}},
		{'(', {NON_TERM_T, NON_TERM_E_}},
	}},
	{ NON_TERM_T,{
		{'n', {NON_TERM_F, NON_TERM_T_}},
		{'(', {NON_TERM_F, NON_TERM_T_}},
	}},
	{ NON_TERM_E_,{
		{')', {}},
		{0, {}},
		{'+', {'+', NON_TERM_T, NON_TERM_E_}},
	}},
	{ NON_TERM_F,{
		{'(', {'(', NON_TERM_E, ')'}},
		{'n', {'n'}},
	}},
};
