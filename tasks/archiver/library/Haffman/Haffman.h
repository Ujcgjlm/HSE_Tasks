#pragma once

#include <string>
#include <vector>

class Haffman {
public:
    static void Encode(std::ostream &in, const std::vector<std::istream *> &out, const std::vector<std::string> &names);

    static void Decode(std::istream &in);

private:
    inline static const size_t COUNT_OF_DIFFERENT_SYMBOLS = 259;
    inline static const size_t FILENAME_END = 256;
    inline static const size_t ONE_MORE_FILE = 257;
    inline static const size_t ARCHIVE_END = 258;
};