#include "position.hpp"

std::ostream &operator<<(std::ostream &out, const Position &pos)
{
    out << "(" << pos.line << "," << pos.pos << ")";
    return out;
}

void Position::operator++(int) {
    index++;
    pos++;
}

void Position::new_line() {
    pos = 1;
    line++;
}


std::ostream &operator<<(std::ostream &out, const Fragment &frag)
{
    out << frag.start << "-" << frag.end;
    return (out);
}
