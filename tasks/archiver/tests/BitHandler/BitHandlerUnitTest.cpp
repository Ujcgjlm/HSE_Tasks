#include <sstream>
#include <iostream>
#include "../../library/BitHandler/BitHandler.h"
#include "../catch.hpp"

TEST_CASE("FormIntegerToNineBinary") {
    std::vector<bool> ans = {0, 1, 0, 1, 0, 0, 1, 1, 1};
    size_t number = 167;
    REQUIRE(BitHandler::FormIntegerToNineBinary(number) == ans);

    ans = {0, 1, 1, 1, 1, 0, 1, 0, 1};
    number = 245;
    REQUIRE(BitHandler::FormIntegerToNineBinary(number) == ans);

    ans = {0, 0, 0, 0, 0, 0, 0, 0, 1};
    number = 1;
    REQUIRE(BitHandler::FormIntegerToNineBinary(number) == ans);

    ans = {1, 0, 0, 0, 0, 0, 0, 0, 0};
    number = 256;
    REQUIRE(BitHandler::FormIntegerToNineBinary(number) == ans);
}

TEST_CASE("WRITE_READ") {
    std::mt19937 Rand(std::chrono::steady_clock::now().time_since_epoch().count());
    std::stringstream ss;

    BitHandler bits(&ss, &ss);
    size_t amount = 10000;
    std::vector<bool> vect;
    std::vector<std::vector<bool>> answers;

    for (size_t i = 0; i < amount; ++i) {
        size_t number = Rand() % 60 + 1;
        vect.clear();

        for (size_t j = 0; j < number; ++j) {
            size_t cur = Rand() % 2;
            if (cur) {
                vect.push_back(1);
            } else {
                vect.push_back(0);
            }
        }

        bits.Write(vect);
        answers.push_back(vect);
    }
    bits.FinishWrite();

    for (size_t i = 0; i < amount; ++i) {
        vect = bits.Read(answers[i].size());
        REQUIRE(vect == answers[i]);
    }
}

TEST_CASE("WRITE_READ_INTEGER") {
    std::mt19937 Rand(std::chrono::steady_clock::now().time_since_epoch().count());
    std::stringstream ss;

    BitHandler bits(&ss, &ss);
    size_t amount = 10000;
    std::vector<size_t> answers;

    for (size_t i = 0; i < amount; ++i) {
        size_t number = Rand() % 270 + 1;
        bits.Write(BitHandler::FormIntegerToNineBinary(number));
        answers.push_back(number);
    }
    bits.FinishWrite();

    for (size_t i = 0; i < amount; ++i) {
        size_t cur = bits.ReadInteger(9);
        REQUIRE(cur == answers[i]);
    }
}