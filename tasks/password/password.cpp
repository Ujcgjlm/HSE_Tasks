#include "password.h"

bool ValidatePassword(const std::string& password) {
    if (password.size() < 8 || 14 < password.size()) {
        return false;
    }
    int cnt_lower = 0;
    int cnt_upper = 0;
    int cnt_digit = 0;
    int cnt_other = 0;
    for (int i : password) {
        if (i < 33 || 126 < i) {
            return false;
        }
        if (std::isdigit(i)) {
            cnt_digit = 1;
        } else {
            if (std::islower(i)) {
                cnt_lower = 1;
            } else {
                if (std::isupper(i)) {
                    cnt_upper = 1;
                } else {
                    cnt_other = 1;
                }
            }
        }
    }
    return cnt_upper + cnt_lower + cnt_digit + cnt_other >= 3;
}
