#include <iostream>
#include <algorithm>
#include "unixpath.h"

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::string answer_path{current_working_dir};
    size_t current_pos = 0;
    if (path.front() == '/') {
        answer_path = '/';
        while (path.at(current_pos) == '/') {
            current_pos++;
        }
    }
    while (current_pos < path.size()) {
        size_t founded_pos = path.find('/', current_pos);
        size_t next_pos = (founded_pos == std::string::npos) ? path.size() : founded_pos;
        std::string_view current_word{path.substr(current_pos, next_pos - current_pos)};
        current_pos = next_pos + 1;
        if (current_word == "..") {
            while (answer_path.back() != '/' && answer_path.size() > 1) {
                answer_path.pop_back();
            }
            if (answer_path.size() > 1) {
                answer_path.pop_back();
            }
        } else {
            if (!current_word.empty() && current_word != ".") {
                if (answer_path.size() > 1) {
                    answer_path += '/';
                }
                answer_path += current_word;
            }
        }
    }
    return answer_path;
}
