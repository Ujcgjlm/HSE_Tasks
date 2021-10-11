#include <cctype>
#include <unordered_map>
#include <functional>
#include <valarray>
#include <set>
#include <iostream>
#include "search.h"

Line::Line(std::string_view line) {
    line_ = line;
}

std::string_view Line::GetLine() const {
    return line_;
}

bool Line::operator==(const Line& a) const {
    if (a.line_.size() == this->line_.size()) {
        for (auto l = a.line_.begin(), r = this->line_.begin(); l != a.line_.end(); ++l, ++r) {
            if (tolower(*l) != tolower(*r)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

uint64_t Line::Hash::operator()(const Line& str) const {
    uint64_t hash = 1, p = 73;
    for (const auto& i : str.GetLine()) {
        hash += (tolower(i) - 'a' + 1) * p;
        p *= p;
    }
    return hash;
}

std::vector<std::string_view> SearchEngine::ParseTextByFunc(std::string_view substring,
                                                            std::function<bool(const char)> func) const {
    std::vector<std::string_view> parsed_query;
    for (size_t current = 0, previous = 0; current <= substring.size(); ++current) {
        if (current == substring.size() || func(substring[current])) {
            if (current != previous) {
                parsed_query.push_back(substring.substr(previous, current - previous));
            }
            previous = current + 1;
        }
    }
    return parsed_query;
}

void SearchEngine::BuildIndex(std::string_view text) {
    parsed_text_ = ParseTextByFunc(text, IsEndOfLine);
    frequency_data_.clear();
    line_data_.clear();

    double lines_count = 0;
    for (std::string_view line : parsed_text_) {
        std::vector<std::string_view> parsed_line = ParseTextByFunc(line, IsntAlpha);
        line_data_.push_back(std::unordered_map<Line, double, Line::Hash>());
        ++lines_count;
        double words_count = 0;
        for (const std::string_view word : parsed_line) {
            ++line_data_.back()[word];
            ++words_count;
        }
        for (auto& [word, count] : line_data_.back()) {
            ++frequency_data_[word.GetLine()];
            if (words_count != 0) {
                count = count / words_count;
            }
        }
    }

    for (auto& [key, now_count_be_log] : frequency_data_) {
        if (now_count_be_log != 0) {
            now_count_be_log = std::log(lines_count / now_count_be_log);
        }
    }
}

std::vector<std::string_view> SearchEngine::Search(std::string_view query, size_t results_count) {
    std::vector<std::string_view> parsed_query = ParseTextByFunc(query, IsntAlpha);

    std::multiset<Relevance, std::greater<Relevance>> relevance_set;
    for (size_t current_line = 0; current_line < parsed_text_.size(); ++current_line) {
        auto line_map = line_data_[current_line];
        double line_sum = 0;
        for (std::string_view word : parsed_query) {
            double tf = 0;
            double idf = 0;
            if (auto it = frequency_data_.find(word); it != frequency_data_.end()) {
                tf = it->second;
            }
            if (auto it = line_map.find(word); it != line_map.end()) {
                idf = it->second;
            }
            line_sum += tf * idf;
        }

        if (line_sum > 0 && results_count > 0) {
            if (relevance_set.size() < results_count) {
                relevance_set.insert({line_sum, current_line});
            } else {
                if (std::prev(relevance_set.end())->relevance < line_sum) {
                    relevance_set.erase(std::prev(relevance_set.end()));
                    relevance_set.insert({line_sum, current_line});
                }
            }
        }
    }

    std::vector<std::string_view> answer;
    for (const auto& relevance : relevance_set) {
        answer.push_back(parsed_text_[relevance.line]);
    }

    return answer;
}
