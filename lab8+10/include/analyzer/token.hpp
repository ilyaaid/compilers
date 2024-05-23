#pragma once

#include "position.hpp"

#include <unordered_map>

using DOMAIN_TAG = char;

struct Token
{
    Token() {}
    Token(DOMAIN_TAG tag, const std::string& attr, const Fragment& frag): tag(tag), attr(attr), frag(frag) {}

    DOMAIN_TAG tag;
    std::string attr;
    Fragment frag;

    bool operator ==(const Token& t) const
    {
        return tag == t.tag && attr == t.attr;
    }

    bool operator<(const Token& t) const {
        if (tag == t.tag) {
            return attr < t.attr;
        }
        return tag < t.tag;
    }
};


class TokenHash {
public:
    size_t operator()(const Token& t) const
    {
        return (std::hash<char>()(t.tag)) ^
            (std::hash<std::string>()(t.attr));
    }
};

std::ostream& operator<<(std::ostream& out, const Token& token);
