#pragma once

#include <string>
#include <list>
#include <algorithm>

std::string operator "" _tgstr(const char* text, const size_t sz) {
    const std::list<char> forb_symbols{ '[', ']', '_', '*', '`', '(', ')', '~' };

    std::string str{text, sz};
    for (const auto symb : forb_symbols) {
        for (size_t off = 0, f = str.find(symb, off); 
            f != std::string::npos;
            f = str.find(symb, off)) 
        {
            char buff[3]{ '\\', symb, '\0' };
            str.replace(f, 1, buff);
            off = f + 2;
        }
    }
    return str;
}

std::string fix_tgstr(std::string str) {
    const std::list<char> forb_symbols{ '[', ']', '_', '*', '`', '(', ')', '~' };

    for (const auto symb : forb_symbols) {
        for (size_t off = 0, f = str.find(symb, off); 
            f != std::string::npos;
            f = str.find(symb, off)) 
        {
            char buff[3]{ '\\', symb, '\0' };
            str.replace(f, 1, buff);
            off = f + 2;
        }
    }
    return str;
}