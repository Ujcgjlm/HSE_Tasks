#include "multiplication.h"

#include <stdexcept>
#include <iostream>
#include <bitset>

int64_t Multiply(int a, int b) {
    std::string s = "П";
    char k = s[0];
    std::bitset<8> bt = (k >> 5);
    std::cout << bt.to_string();
    return static_cast<int64_t>(a) * static_cast<int64_t>(b);
}
