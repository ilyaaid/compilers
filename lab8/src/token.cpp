#include "token.hpp"

std::unordered_map<DOMAIN_TAG, std::string> Token::tag2string = {
    {TAG_IDENT, "IDENT"},
    {TAG_STRING, "STRING"},
    {TAG_STAR, "*"},
    {TAG_LPAREN, "("},
    {TAG_RPAREN, ")"},
    {TAG_END, "EOF"},
};

std::ostream &operator<<(std::ostream &out, const Token &token)
{
    out << token.tag2string.at(token.tag) << ":" << token.frag;
    return out;
}
