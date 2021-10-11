#include "TreapTestsSetUp.h"

TEST_CASE("BigStress") {
    std::mt19937 Rand(std::chrono::steady_clock::now().time_since_epoch().count());
    std::priority_queue<Something, std::vector<Something>, std::greater<>> except;

    size_t amount = 20000000;

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

TEST_CASE("BigAmountOfStress") {
    std::mt19937 Rand(std::chrono::steady_clock::now().time_since_epoch().count());
    std::priority_queue<Something, std::vector<Something>, std::greater<>> except;

    size_t number = 123;
    while (number--) {
        size_t amount = 10000;

        while (amount--) {
            Something something{Rand(), Rand()};
            except.push(something);
            treap.Insert(something);

            if (Rand() % 10 < 2) {
                REQUIRE(except.top() == treap.CutTop());
                except.pop();
            }
        }

        while (!treap.Empty()) {
            REQUIRE(except.top() == treap.CutTop());
            except.pop();
        }
    }
}