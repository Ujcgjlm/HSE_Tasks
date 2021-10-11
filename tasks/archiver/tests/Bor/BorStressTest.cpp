#include <queue>

#include "../../library/Treap/Treap.h"
#include "../../library/Bor/Bor.cpp"
#include "../catch.hpp"

class Comp {
public:
    bool operator()(Bor *a, Bor *b) const {
        return a->GetFrequency() < b->GetFrequency();
    }
};

TEST_CASE("BigAmountOfStressOnBor") {
    std::mt19937 Rand(std::chrono::steady_clock::now().time_since_epoch().count());
    Treap<Bor *, Comp> treapBor;

    size_t number = 123;
    while (number--) {
        size_t amount = 10000;

        while (amount--) {
            Bor *something = new Bor(Rand(), Rand());
            treapBor.Insert(something);
        }

        size_t mn = 0;

        while (!treapBor.Empty()) {
            REQUIRE(mn <= treapBor.Top()->GetFrequency());
            mn = treapBor.CutTop()->GetFrequency();
        }
    }
}