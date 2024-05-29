#pragma once

#include <memory>

#include "scanner.hpp"

#include "parser_generated.hpp"
#include "ast_tree.hpp"

class Driver
{
public:
    void common(ps::Parser::value_type* lval, ps::Parser::location_type* lloc,
    const std::string& yytext);

public:
    std::shared_ptr<Program> program;
};
