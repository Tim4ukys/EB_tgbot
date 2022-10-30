#pragma once

#include <string>

class LegendWords {
public:
    enum class type {
        ERR,
        GOOD,
        GOODMAN
    };

    static std::string get(type tp);
};