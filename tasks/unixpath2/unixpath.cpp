#include "unixpath.h"

std::vector<std::string> UnixPath::ParseTextByFunc(std::string_view substring,
                                                   const std::function<bool(const char)> &func) const {
    std::vector<std::string> parsed_query;
    for (size_t current = 0, previous = 0; current <= substring.size(); ++current) {
        if (current == substring.size() || func(substring[current])) {
            if (current != previous) {
                parsed_query.emplace_back(substring.substr(previous, current - previous));
            }
            previous = current + 1;
        }
    }
    return parsed_query;
}

void UnixPath::Normalize(std::vector<std::string> &dir) {
    std::vector<std::string> new_dir;
    for (const std::string &cur_name : dir) {
        if (cur_name == "..") {
            if (!new_dir.empty()) {
                new_dir.pop_back();
            }
        } else {
            if (cur_name != ".") {
                new_dir.push_back(cur_name);
            }
        }
    }
    dir = new_dir;
}

UnixPath::UnixPath(std::string_view initial_dir) {
    init_dir_ = ParseTextByFunc(initial_dir, IsEndOfName);
    Normalize(init_dir_);
    cur_dir_ = init_dir_;
}

void UnixPath::ChangeDirectory(std::string_view path) {
    if (!path.empty() && IsEndOfName(path[0])) {
        cur_dir_ = ParseTextByFunc(path, IsEndOfName);
    } else {
        std::vector<std::string> parsed_test = ParseTextByFunc(path, IsEndOfName);
        cur_dir_.insert(cur_dir_.end(), parsed_test.begin(), parsed_test.end());
    }
    Normalize(cur_dir_);
}

std::string UnixPath::GetAbsolutePath() const {
    std::string dir;
    for (auto &file : cur_dir_) {
        dir += '/' + file;
    }
    return dir;
}

std::string UnixPath::GetRelativePath() const {
    std::string dir;
    size_t pos_diff = std::min(cur_dir_.size(), init_dir_.size());
    for (size_t cur = 0; cur < pos_diff; ++cur) {
        if (cur_dir_[cur] != init_dir_[cur]) {
            pos_diff = cur;
            break;
        }
    }

    size_t quit_dir_count = init_dir_.size() - pos_diff;
    if (quit_dir_count == 0) {
        dir += "./";
    } else {
        while (quit_dir_count--) {
            dir += "../";
        }
    }

    for (size_t cur = pos_diff; cur < cur_dir_.size(); ++cur) {
        dir += cur_dir_[cur] + '/';
    }

    dir.pop_back();
    return dir;
}