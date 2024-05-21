#pragma once

#include <iostream>

struct Position
{
    int line = 1;
    int pos = 1;
    int index = 1;

    void operator++(int);
    void new_line();
};

std::ostream &operator<<(std::ostream &out, const Position &pos);

struct Fragment
{
    Position start;
    Position end;
};

std::ostream &operator<<(std::ostream &out, const Fragment &frag);
