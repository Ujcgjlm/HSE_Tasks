#pragma once

struct UnknownSymbol : public std::exception {
    const char *what() const throw() {
        return "Tried to read something unknown for current Haffman dict";
    }
};