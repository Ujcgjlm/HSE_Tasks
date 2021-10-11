#include "palindrome.h"

bool IsPalindrome(const std::string& str) {
    // [l; r)
    size_t l = 0;
    size_t r = str.size();

    while (l < r) {
        while (l < r - 1 && str[l] == ' ') {
            l++;
        }
        while (l < r - 1 && str[r - 1] == ' ') {
            r--;
        }
        if (l >= r - 1) {
            return true;
        }
        if (str[l] != str[r - 1]) {
            return false;
        }
        l++;
        r--;
    }
    return true;
}
