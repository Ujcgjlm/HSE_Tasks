#pragma once

#include <cstdint>
#include <vector>

class Bor {
public:
    struct Node {
        size_t symbol = 0; // If symbol == SIZE_MAX, it means that this node hasn't got symbol
        size_t frequency = 0;
        bool is_terminal = false;
        Node *l = nullptr;
        Node *r = nullptr;

        ~Node() {
            delete l;
            delete r;
        }

        bool operator<(const Node &a) const {
            return frequency < a.frequency;
        }
    };

    struct SymbolPath {
        size_t symbol = 0;
        size_t pathSize = 0;
    };

    Bor();

    Bor(size_t symbol, size_t frequency);

    Bor(const Bor *l, const Bor *r);

    ~Bor();

    bool operator<(const Bor &a) const {
        return root_ < a.root_;
    }

    size_t GetFrequency();

    void AddPath(size_t symbol, std::vector<bool> &path);

    Node *Move(Node *from, bool where) const;

    Node *GetRoot() const;

    std::vector<SymbolPath> GetPathToEachTerminalVertex();

private:
    Node *root_ = nullptr;

    void Dfs_(Node *node, size_t size, std::vector<SymbolPath> &symbolPath);
};