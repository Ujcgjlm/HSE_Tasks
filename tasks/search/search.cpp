#include <unordered_map>
#include <chrono>
#include <complex>
#include <queue>
#include <algorithm>
#include <map>
#include <set>
#include "search.h"

class Line {
public:
    std::string_view line_;

    Line(std::string_view line) : line_(line) {
    }

    friend bool operator==(const Line& a, const Line& b) {
        if (a.line_.size() == b.line_.size()) {
            for (auto l = a.line_.begin(), r = b.line_.begin(); l != a.line_.end(); ++l, ++r) {
                if (tolower(*l) != tolower(*r)) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

struct Hash {
    uint64_t operator()(const Line& str) const {
        uint64_t hash = 1, p = 73;
        for (const auto& i : str.line_) {
            hash += (tolower(i) - 'a' + 1) * p;
            p *= p;
        }
        return hash;
    }
};

struct Data {
    int count_in_lines = 0;
    int count_in_current_line = 0;
    double log = 0;
};

struct Relevance {
    double relevance;
    std::string_view line;

    friend bool operator>(const Relevance& a, const Relevance& b) {
        return a.relevance > b.relevance;
    }
};

void InitMap(std::string_view substring, std::unordered_map<Line, Data, Hash>& query_data) {
    for (size_t current = 0, previous = 0; current <= substring.size(); ++current) {
        if (current == substring.size() || !std::isalpha(substring[current])) {
            if (current != previous) {
                query_data[{substring.substr(previous, current - previous)}] = {};
            }
            previous = current + 1;
        }
    }
}

uint64_t CountWordsInLine(std::string_view substring, std::unordered_map<Line, Data, Hash>& query_data) {
    uint64_t words_count = 0;
    for (size_t current = 0, previous = 0; current <= substring.size(); ++current) {
        if (current == substring.size() || !std::isalpha(substring[current])) {
            if (current != previous) {
                ++words_count;
                auto map_it = query_data.find({substring.substr(previous, current - previous)});
                if (map_it != query_data.end()) {
                    ++map_it->second.count_in_current_line;
                }
            }
            previous = current + 1;
        }
    }
    return words_count;
}

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    std::unordered_map<Line, Data, Hash> query_data;
    InitMap(query, query_data);

    uint64_t line_count = 0;
    for (size_t current = 0, previous = 0; current <= text.size(); ++current) {
        if (current == text.size() || text[current] == '\n') {
            if (current != previous) {
                ++line_count;
                std::string_view line{text.substr(previous, current - previous)};
                CountWordsInLine(line, query_data);
                for (auto& [_, value] : query_data) {
                    value.count_in_lines += value.count_in_current_line ? 1 : 0;
                    value.count_in_current_line = 0;
                }
            }
            previous = current + 1;
        }
    }

    for (auto current_it = query_data.begin(); current_it != query_data.end();) {
        auto& [key, value] = *current_it;
        if (value.count_in_lines != 0) {
            value.log = std::log(static_cast<double>(line_count) / static_cast<double>(value.count_in_lines));
            ++current_it;
        } else {
            current_it = query_data.erase(current_it);
        }
    }

    std::multiset<Relevance, std::greater<Relevance>> relevance_set;
    for (size_t current = 0, previous = 0; current <= text.size(); ++current) {
        if (current == text.size() || text[current] == '\n') {
            if (current != previous) {
                std::string_view line{text.substr(previous, current - previous)};
                uint64_t words_count = CountWordsInLine(line, query_data);
                if (words_count > 0) {
                    double relevance_sum = 0;
                    for (auto& [_, value] : query_data) {
                        double relevance = value.log * value.count_in_current_line / static_cast<double>(words_count);
                        value.count_in_current_line = 0;
                        relevance_sum += relevance;
                    }
                    if (relevance_sum > 0 && results_count > 0) {
                        if (relevance_set.size() < results_count) {
                            relevance_set.insert({relevance_sum, line});
                        } else {
                            if (std::prev(relevance_set.end())->relevance < relevance_sum) {
                                relevance_set.erase(std::prev(relevance_set.end()));
                                relevance_set.insert({relevance_sum, line});
                            }
                        }
                    }
                }
            }
            previous = current + 1;
        }
    }

    std::vector<std::string_view> answer;
    for (const auto& relevance : relevance_set) {
        answer.push_back(relevance.line);
    }

    return answer;
}
