#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <string>
#include <variant>
#include <memory>
#include <functional>

#include "parser.hpp"

using return_type = DOMAIN_TAG;
using semantic_type = std::string;
using location_type = Fragment;

class Scanner : public yyFlexLexer
{
public:
    Scanner(std::istream *in, std::ostream *out) : yyFlexLexer(in, out){};
    virtual ~Scanner(){};

    using FlexLexer::yylex;

    return_type lex(std::shared_ptr<semantic_type> &lval, std::shared_ptr<location_type> &lloc);
};

class Driver
{
public:
    void common(const std::shared_ptr<semantic_type>& lval,
    const std::shared_ptr<location_type>& lloc,
    const semantic_type& yytext);

    return_type end(const std::shared_ptr<semantic_type>& lval);

public:
    std::string cur_text;

    Position cur_pos;
    bool continued = false;
};
