#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <string>
#include <variant>
#include <memory>
#include <functional>

#include "parser_generated.hpp"


class Scanner : public yyFlexLexer
{
public:
    Scanner(std::istream *in, std::ostream *out) : yyFlexLexer(in, out){};
    virtual ~Scanner(){};

    using FlexLexer::yylex;

    ps::Parser::token_type lex(ps::Parser::value_type* lval, ps::Parser::location_type* lloc);
};


