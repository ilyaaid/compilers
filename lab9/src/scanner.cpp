#include "scanner.hpp"

#include <iostream>

void Scanner::nextToken() {
    std::shared_ptr<semantic_type> lval = std::make_shared<semantic_type>();
    std::shared_ptr<location_type> frag = std::make_shared<location_type>();
    DOMAIN_TAG tag = lex(lval, frag);
    cur_token = Token(tag, *lval, *frag);
}

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
    return DOMAIN_TAG::TAG_END;
}
