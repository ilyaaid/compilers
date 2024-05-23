#include "driver.hpp"

void Driver::common(ps::Parser::value_type* lval,
    const std::string& yytext)
{
    lval->build< std::string >( yytext );
    if (!continued)
    {
        // lloc->start = cur_pos;
        cur_text.clear();
    }
    continued = false;

    cur_text += yytext;
}

