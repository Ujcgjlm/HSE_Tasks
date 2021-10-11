#pragma once

#include <queue>
#include <tuple>
#include "../../library/Treap/Treap.h"
#include <iostream>

#define CATCH_CONFIG_MAIN

#include "../catch.hpp"

struct Something {
    int64_t value = 0;
    int64_t value2 = 0;

    bool operator<(const Something &a) const {
        return std::tie(value, value2) < std::tie(a.value, a.value2);
    }

    bool operator>(const Something &a) const {
        return std::tie(value, value2) > std::tie(a.value, a.value2);
    }

    bool operator==(const Something &a) const {
        return (value == a.value) && (value2 == a.value2);
    }

    friend std::ostream &operator<<(std::ostream &out, const Something &something) {
        out << "{" << something.value << ", " << something.value2 << "}";
        return out;
    }
};

class Comp {
public:
    bool operator()(Something a, Something b) const {
        return std::tie(a.value, a.value2) < std::tie(b.value, b.value2);
    }
};

Treap<Something, Comp> treap;