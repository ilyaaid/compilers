#include "scanner.hpp"

#include <iostream>

void Driver::common(const std::shared_ptr<semantic_type>& lval,
    const std::shared_ptr<location_type>& lloc,
    const semantic_type& yytext)
{
    if (!continued)
    {
        lloc->start = cur_pos;
        cur_text.clear();
    }
    continued = false;

    cur_text += yytext;
    for (size_t i = 0; i < yytext.size(); ++i)
    {
        cur_pos++;
        if (yytext[i] == '\n')
        {
            cur_pos.new_line();
        }
    }
    lloc->end = cur_pos;
    lval->assign(cur_text);
}

return_type Driver::end(const std::shared_ptr<semantic_type>& lval)
{
    return 0;
}

