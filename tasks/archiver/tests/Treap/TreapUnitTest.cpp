#include "TreapTestsSetUp.h"

TEST_CASE("OneElement") {
    Something test{123, 42};
    treap.Insert(test);
    REQUIRE(test == treap.Top());
    treap.Pop();
}

TEST_CASE("TwoElements") {
    Something test1{123, 42};
    Something test2{234, 635};

    treap.Insert(test1);
    treap.Insert(test2);
    REQUIRE(test1 == treap.Top());
    treap.Pop();
    REQUIRE(test2 == treap.Top());
    treap.Pop();

    treap.Insert(test1);
    REQUIRE(test1 == treap.Top());
    treap.Pop();
    treap.Insert(test2);
    REQUIRE(test2 == treap.Top());
    treap.Pop();

    treap.Insert(test2);
    treap.Insert(test1);
    REQUIRE(test1 == treap.Top());
    treap.Pop();
    REQUIRE(test2 == treap.Top());
    treap.Pop();

    treap.Insert(test2);
    REQUIRE(test2 == treap.Top());
    treap.Pop();
    treap.Insert(test1);
    REQUIRE(test1 == treap.Top());
    treap.Pop();
}

TEST_CASE("TwoEqualElements") {
    Something test1{123, 42};
    Something test2{123, 42};

    treap.Insert(test1);
    treap.Insert(test2);
    REQUIRE(test1 == treap.Top());
    treap.Pop();
    REQUIRE(test2 == treap.Top());
    treap.Pop();

    treap.Insert(test1);
    REQUIRE(test1 == treap.Top());
    treap.Pop();
    treap.Insert(test2);
    REQUIRE(test2 == treap.Top());
    treap.Pop();

    treap.Insert(test2);
    treap.Insert(test1);
    REQUIRE(test1 == treap.Top());
    treap.Pop();
    REQUIRE(test2 == treap.Top());
    treap.Pop();

    treap.Insert(test2);
    REQUIRE(test2 == treap.Top());
    treap.Pop();
    treap.Insert(test1);
    REQUIRE(test1 == treap.Top());
    treap.Pop();
}

TEST_CASE("SomeTest") {
    std::mt19937 Rand(std::chrono::steady_clock::now().time_since_epoch().count());
    std::priority_queue<Something, std::vector<Something>, std::greater<>> except;

    size_t amount = 100;

    while (amount--) {
        Something something{Rand(), Rand()};
        except.push(something);
        treap.Insert(something);

        if (Rand() % 10 < 2) {
            REQUIRE(except.top() == treap.Top());
            except.pop();
            treap.Pop();
        }
    }

    while (!treap.Empty()) {
        REQUIRE(except.top() == treap.Top());
        except.pop();
        treap.Pop();
    }
}