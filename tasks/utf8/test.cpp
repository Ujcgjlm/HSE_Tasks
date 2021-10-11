#include <catch.hpp>
#include <utf8.h>

#include <cstddef>

struct TestData {
    std::string String;
    std::vector<uint32_t> Codepoints;
};

std::vector<TestData> TESTS = {
    {"Hello!", {72, 101, 108, 108, 111, 33}},
    {"Привет, мир!", {1055, 1088, 1080, 1074, 1077, 1090, 44, 32, 1084, 1080, 1088, 33}},
    {"德国足球甲级联赛（德语：Bundesliga）简称德甲",
     {24503, 22269, 36275, 29699, 30002, 32423, 32852, 36187, 65288, 24503, 35821, 65306, 66,   117,
      110,   100,   101,   115,   108,   105,   103,   97,    65289, 31616, 31216, 24503, 30002}}};

TEST_CASE("CountUtf8Symbols") {
    for (const auto& test : TESTS) {
        REQUIRE(CountUtf8Symbols(test.String) == test.Codepoints.size());
    }
}

TEST_CASE("EncodeUtf8") {
    for (const auto& test : TESTS) {
        std::string encodedString;
        EncodeUtf8(test.Codepoints, encodedString);
        REQUIRE(encodedString == test.String);
    }
}

TEST_CASE("DecodeUtf8") {
    for (const auto& test : TESTS) {
        std::vector<uint32_t> decodedCodepoints;
        DecodeUtf8(test.String, decodedCodepoints);
        REQUIRE(test.Codepoints == decodedCodepoints);
    }
}
