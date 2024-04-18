#include "scanner.hpp"

#include <iostream>

void Driver::common(const std::shared_ptr<Token> &lval,
                    const std::shared_ptr<location_type> &lloc,
                    const std::string &text)
{
    if (!continued)
    {
        lloc->start = cur_pos;
        cur_text.clear();
    }
    continued = false;

    // prev_text = cur_text;
    // prev_pos = cur_pos;

    cur_text += text;
    for (size_t i = 0; i < text.size(); ++i)
    {
        cur_pos++;
        if (text[i] == '\n')
        {
            cur_pos.new_line();
        }
    }
    lloc->end = cur_pos;

    lval->frag = *lloc;
    lval->attr = cur_text;
}

return_type Driver::end(const std::shared_ptr<Token> &lval)
{
    lval->attr.clear();
    lval->tag = DOMAIN_TAG::TAG_END;
    return lval->tag;
}
