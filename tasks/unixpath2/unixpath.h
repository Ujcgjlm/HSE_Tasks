#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <functional>

class UnixPath {
public:
    UnixPath(std::string_view initial_dir);

    void ChangeDirectory(std::string_view path);

    [[nodiscard]] std::string GetAbsolutePath() const;

    std::string GetRelativePath() const;

    std::vector<std::string> ParseTextByFunc(std::string_view substring, const std::function<bool(const char)> &) const;

    static bool IsEndOfName(const char chr) {
        return chr == '/';
    }

    void Normalize(std::vector<std::string> &dir);

private:
    std::vector<std::string> init_dir_;
    std::vector<std::string> cur_dir_;
};
