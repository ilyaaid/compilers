#include "token.hpp"

std::unordered_map<DOMAIN_TAG, std::string> Token::tag2string = {
    {TAG_IF, "if"},
    {TAG_THEN, "then"},
    {TAG_ELSE, "else"},

    {TAG_OR, "or"},
    {TAG_AND, "and"},
    {TAG_NOT, "not"},

    {TAG_TYPE, "TYPE"},

    {TAG_NUMBER, "NUMBER"},
    {TAG_BOOL_NUMBER, "BOOL"},
    {TAG_IDENT, "IDENT"},

    {TAG_LPAREN, "("},
    {TAG_RPAREN, ")"},
    {TAG_LQPAREN, "["},
    {TAG_RQPAREN, "]"},
    {TAG_COLON, ":"},
    {TAG_SEMICOLON, ";"},
    {TAG_COMMA, ","},
    {TAG_EQUAL, "="},


    {TAG_MULT, "*"},
    {TAG_DIV, "/"},
    {TAG_PLUS, "+"},
    {TAG_MINUS, "-"},
    

    {TAG_END, "EOF"},
};

std::ostream &operator<<(std::ostream &out, const Token &token)
{
    std::string str;
    try {
        str = token.tag2string.at(token.tag);
    } catch(...) {
        str = "UNDEFINED";
    }
    out << str << "=" + token.attr << ":" << token.frag;
    return out;
}
