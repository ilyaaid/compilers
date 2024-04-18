#pragma once

#include "position.hpp"

#include <unordered_map>

enum DOMAIN_TAG
{
    TAG_NUMBER,
    TAG_BOOL_NUMBER,
    TAG_IDENT,

    TAG_IF,
    TAG_THEN,
    TAG_ELSE,

    TAG_OR,
    TAG_AND,
    TAG_NOT,
    
    TAG_TYPE,

    TAG_LPAREN,
    TAG_RPAREN,
    TAG_LQPAREN,
    TAG_RQPAREN,
    TAG_COLON,
    TAG_SEMICOLON,
    TAG_COMMA,
    TAG_EQUAL,

    TAG_MULT,
    TAG_DIV,
    TAG_PLUS,
    TAG_MINUS,

    // TAG_COMMENT,
    // TAG_WHITESPACE,
    // TAG_ERROR,
    TAG_EMPTY,

    TAG_END
};

struct Token
{
    Token() {}
    Token(DOMAIN_TAG tag, const std::string& attr, const Fragment& frag): tag(tag), attr(attr), frag(frag) {}

    DOMAIN_TAG tag;
    std::string attr;
    Fragment frag;

    static std::unordered_map<DOMAIN_TAG, std::string> tag2string;
};

std::ostream &operator<<(std::ostream &out, const Token &token);
