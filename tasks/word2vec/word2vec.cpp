#include "word2vec.h"

#include <vector>
#include <iostream>

std::vector<std::string> FindClosestWords(const std::vector<std::string>& words,
                                          const std::vector<std::vector<int>>& vectors) {
    std::vector<std::string> closest_words;
    int64_t current_max = INT64_MIN;
    for (auto i = vectors.begin() + (vectors.empty() ? 0 : 1); i != vectors.end(); ++i) {
        const auto& current_vector = *i;
        auto a = current_vector.begin();
        auto b = vectors.at(0).begin();
        int64_t result = 0;
        while (a != current_vector.end()) {
            result += static_cast<int64_t>(*a) * *b;
            ++a;
            ++b;
        }
        if (result > current_max) {
            closest_words.clear();
            current_max = result;
        }
        if (result == current_max) {
            closest_words.push_back(words.at(i - vectors.begin()));
        }
    }
    return closest_words;
}
