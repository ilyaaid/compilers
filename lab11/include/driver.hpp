#pragma once

#include <memory>

#include "scanner.hpp"

class Driver
{
public:
    void common(ps::Parser::value_type* lval,
    const std::string& yytext);

public:
    std::string cur_text;
    bool continued = false;

    std::string result;

    int enclosure = 0;
};
