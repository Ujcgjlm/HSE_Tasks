#include "utf8.h"
#include <cstddef>
#include <vector>
#include <string>

template <typename T>
T LeftBitsZeroing(T number, int count_of_bits) {
    return (number << (8 * sizeof(T) - count_of_bits)) >> (8 * sizeof(T) - count_of_bits);
}

template <typename T>
size_t CountBytesInUtf8Symbol(T symbol) {
    size_t count_bytes_in_utf8_symbol = 0;
    while (symbol & (1 << (8 * sizeof(T) - 1))) {
        symbol <<= 1;
        count_bytes_in_utf8_symbol++;
    }
    return (count_bytes_in_utf8_symbol == 0) ? 1 : count_bytes_in_utf8_symbol;
}

size_t CountUtf8Symbols(const std::string& str) {
    size_t count_utf8_symbols = 0;
    size_t string_flag = 0;
    while (string_flag < str.size()) {
        string_flag += CountBytesInUtf8Symbol(str[string_flag]);
        count_utf8_symbols++;
    }
    return count_utf8_symbols;
}

void EncodeUtf8(const std::vector<uint32_t>& codepoints, std::string& str) {
    size_t count_bytes;
    for (auto i : codepoints) {
        if (i < 128) {
            str += static_cast<char>(i);
            continue;
        } else if (i < 2048) {
            count_bytes = 1;
        } else if (i < 65536) {
            count_bytes = 2;
        } else {
            count_bytes = 3;
        }
        uint32_t prefix = ((((1 << 9) - 1) >> (8 - count_bytes - 1)) << (8 - count_bytes - 1));
        str += static_cast<char>(prefix + (i >> (6 * count_bytes)));
        for (size_t j = 0; j < count_bytes; ++j) {
            str +=
                static_cast<char>(128 + (LeftBitsZeroing(i, (6 * (count_bytes - j))) >> (6 * (count_bytes - j - 1))));
        }
    }
}

void DecodeUtf8(const std::string& str, std::vector<uint32_t>& codepoints) {
    size_t string_flag = 0;
    while (string_flag < str.size()) {
        uint32_t current_byte = static_cast<uint32_t>(str[string_flag]);
        size_t symbol_size = CountBytesInUtf8Symbol(str[string_flag]);
        size_t suffix_size = (8 * sizeof(char) - ((symbol_size == 1) ? 0 : symbol_size));
        uint32_t current_symbol = (LeftBitsZeroing(current_byte, suffix_size) << (6 * (symbol_size - 1)));
        for (size_t i = string_flag + 1; i < string_flag + symbol_size; ++i) {
            current_symbol +=
                (LeftBitsZeroing(static_cast<uint32_t>(str[i]), 6) << (6 * (string_flag + symbol_size - i - 1)));
        }
        string_flag += symbol_size;
        codepoints.push_back(current_symbol);
    }
}
