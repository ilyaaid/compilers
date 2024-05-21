#include "token.hpp"


std::ostream &operator<<(std::ostream &out, const Token &token)
{
    std::string str;
    try {
        str = token.tag;
    } catch(...) {
        str = "UNDEFINED";
    }
    out << str << "=" + token.attr << ":" << token.frag;
    return out;
}
