#pragma once

#include <string_view>
#include <vector>

class Line {
public:
    Line(std::string_view line);
    std::string_view GetLine() const;
    bool operator==(const Line& a) const;

    struct Hash {
        uint64_t operator()(const Line& str) const;
    };

private:
    std::string_view line_;
};

struct Relevance {
    double relevance;
    size_t line;

    friend bool operator>(const Relevance& a, const Relevance& b) {
        return a.relevance > b.relevance;
    }
};

class SearchEngine {
public:
    void BuildIndex(std::string_view text);
    std::vector<std::string_view> Search(std::string_view query, size_t results_count);

    std::vector<std::string_view> ParseTextByFunc(std::string_view substring, std::function<bool(const char)>) const;
    static bool IsEndOfLine(const char chr) {
        return chr == '\n';
    }
    static bool IsntAlpha(const char chr) {
        return !isalpha(chr);
    }

private:
    std::unordered_map<Line, double, Line::Hash> frequency_data_;
    std::vector<std::unordered_map<Line, double, Line::Hash>> line_data_;
    std::vector<std::string_view> parsed_text_;
};
