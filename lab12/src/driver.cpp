#include "driver.hpp"

void Driver::common(ps::Parser::value_type* lval, ps::Parser::location_type* lloc,
    const std::string& yytext)
{
    lloc->step();
    lloc->columns(yytext.size());
}

