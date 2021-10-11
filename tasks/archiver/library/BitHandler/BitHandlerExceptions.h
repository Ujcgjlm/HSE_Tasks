#pragma once

struct CantReadFromFile : public std::exception {
    const char *what() const throw() {
        return "Cant get new byte";
    }
};