#pragma once

struct BorRootEmpty : public std::exception {
    const char *what() const throw() {
        return "Try get root info from nullptr";
    }
};