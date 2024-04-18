#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <string>
#include <variant>
#include <memory>
#include <functional>

#include "position.hpp"
#include "token.hpp"

using return_type = DOMAIN_TAG;
using semantic_type = Token;
using location_type = Fragment;

class Scanner : public yyFlexLexer
{
public:
    Scanner(std::istream *in, std::ostream *out) : yyFlexLexer(in, out){};
    virtual ~Scanner(){};

    using FlexLexer::yylex;

    return_type lex(std::shared_ptr<Token> &lval, std::shared_ptr<location_type> &lloc);
};

class Driver
{
public:
    void common(const std::shared_ptr<Token> &lval,
                const std::shared_ptr<location_type> &lloc,
                const std::string &text);

    return_type end(const std::shared_ptr<Token> &lval);

public:
    std::string prev_text;
    std::string cur_text;

    Position prev_pos;
    Position cur_pos;
    bool continued = false;
};
