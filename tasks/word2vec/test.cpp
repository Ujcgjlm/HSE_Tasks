#include <catch.hpp>
#include <word2vec.h>

#include <vector>

TEST_CASE("FindClosestWords") {
    const std::vector<std::string> words = {"sweden", "queen", "norway", "king"};
    const std::vector<std::string> nothing_s = {};
    const std::vector<std::string> a = {"a"};

    const std::vector<std::vector<int>> vectors = {{4, 6, 3}, {0, 2, 2}, {4, 7, 5}, {-1, 3, 2}};
    const std::vector<std::vector<int>> odin = {{1}};

    const std::vector<std::string> expected = {"norway"};
    const std::vector<std::string> nothing_e = {};

    REQUIRE(expected == FindClosestWords(words, vectors));
    REQUIRE(nothing_e == FindClosestWords(a, odin));
}
