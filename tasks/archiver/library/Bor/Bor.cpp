#include <iostream>
#include "Bor.h"
#include "BorExceptions.h"

Bor::Bor() {
    root_ = new Node();
}


Bor::Bor(size_t symbol, size_t frequency) {
    root_ = new Node{.symbol = symbol, .frequency = frequency, .is_terminal = true};
}

Bor::Bor(const Bor *l, const Bor *r) {
    root_ = new Node{.symbol = 0, .frequency = l->root_->frequency +
                                               r->root_->frequency, .is_terminal = false, .l = l->root_, .r = r->root_};
}

Bor::~Bor() {
    delete root_;
}

std::vector<Bor::SymbolPath> Bor::GetPathToEachTerminalVertex() {
    if (root_ == nullptr) {
        throw BorRootEmpty();
    }
    std::vector<SymbolPath> wordPath;
    Dfs_(root_, {}, wordPath);
    return wordPath;
}

void Bor::Dfs_(Bor::Node *node, size_t pathSize, std::vector<SymbolPath> &symbolPath) {
    if (node->is_terminal) {
        symbolPath.push_back({node->symbol, pathSize});
        return;
    }
    Dfs_(node->l, pathSize + 1, symbolPath);
    Dfs_(node->r, pathSize + 1, symbolPath);
}

size_t Bor::GetFrequency() {
    if (root_ == nullptr) {
        throw BorRootEmpty();
    }
    return root_->frequency;
}

void Bor::AddPath(size_t symbol, std::vector<bool> &path) {
    Node *cur_node = root_;
    for (auto &&pos: path) {
        if (pos) {
            if (cur_node->r == nullptr) {
                cur_node->r = new Node();
            }
            cur_node = cur_node->r;
        } else {
            if (cur_node->l == nullptr) {
                cur_node->l = new Node();
            }
            cur_node = cur_node->l;
        }
    }
    cur_node->is_terminal = true;
    cur_node->symbol = symbol;
}

Bor::Node *Bor::GetRoot() const {
    return root_;
}

Bor::Node *Bor::Move(Bor::Node *from, bool where) const {
    if (from == nullptr) {
        return nullptr;
    }
    if (where) {
        return from->r;
    } else {
        return from->l;
    }
}

