#include "../../library/Bor/Bor.cpp"
#include "../catch.hpp"

TEST_CASE("IsFrequencyValid") {
    size_t a1_frequency = 4;
    Bor *a1 = new Bor{1, a1_frequency};
    size_t a2_frequency = 234;
    Bor *a2 = new Bor{2, a2_frequency};
    size_t a3_frequency = 2144;
    Bor *a3 = new Bor{3, a3_frequency};
    size_t a4_frequency = 1341;
    Bor *a4 = new Bor{4, a4_frequency};
    Bor *a5 = new Bor(new Bor(a1, a2), new Bor(a3, a4));
    REQUIRE(a1_frequency + a2_frequency + a3_frequency + a4_frequency == a5->GetFrequency());
    delete a5;
}