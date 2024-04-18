#pragma once

#include "position.hpp"

#include <unordered_map>

enum DOMAIN_TAG
{
    TAG_IDENT,
    TAG_STRING,
    TAG_STAR,
    TAG_LPAREN,
    TAG_RPAREN,

    TAG_COMMENT,
    TAG_WHITESPACE,
    TAG_ERROR,
    TAG_EMPTY,

    TAG_END
};

struct Token
{
    DOMAIN_TAG tag;
    Fragment frag;
    std::string attr;

    static std::unordered_map<DOMAIN_TAG, std::string> tag2string;
};

std::ostream &operator<<(std::ostream &out, const Token &token);
