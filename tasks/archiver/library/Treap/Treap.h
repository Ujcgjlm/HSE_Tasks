#pragma once

#include <cstddef>
#include <random>
#include <chrono>
#include <queue>
#include <iostream>

template<class T, class Comp>
class Treap {
public:
    struct Node {
        size_t key = 0;   // Treap is binary-tree by key
        T value = 0; // Treap is heap by value
        Node *l = nullptr;
        Node *r = nullptr;

        ~Node() {
            delete l;
            delete r;
        }
    };

    Treap();

    ~Treap();

    void Insert(T a);

    void Pop();

    T Top();

    T CutTop();

    [[nodiscard]] bool Empty() const;

    [[nodiscard]] size_t Size() const;

private:
    Node *root_ = nullptr;
    size_t size_ = 0;
    std::mt19937 Rand;

    void Split_(Node *current_node, size_t key, Node *&left_tree, Node *&right_tree);

    void Merge_(Node *&current_node, Node *left_tree, Node *right_tree);

    void Insert_(Node *&current_node, Node *inserted_node);

    bool compT(const T &a, const T &b) const;
};

/// Realizations

template<class T, class Comp>
Treap<T, Comp>::Treap() : Rand(std::chrono::steady_clock::now().time_since_epoch().count()) {
}

template<class T, class Comp>
bool Treap<T, Comp>::compT(const T &a, const T &b) const {
    return Comp()(a, b);
}

template<class T, class Comp>
void Treap<T, Comp>::Insert(T a) {
    ++size_;
    Node *b = new Node{.key = Rand(), .value = a,};
    Treap::Insert_(root_, b);
}

template<class T, class Comp>
void Treap<T, Comp>::Split_(Node *current_node, size_t key, Node *&left_tree, Node *&right_tree) {
    if (current_node == nullptr) {
        left_tree = nullptr;
        right_tree = nullptr;
    } else {
        if (key < current_node->key) {
            Split_(current_node->l, key, left_tree, current_node->l);
            right_tree = current_node;
        } else {
            Split_(current_node->r, key, current_node->r, right_tree);
            left_tree = current_node;
        }
    }
}

template<class T, class Comp>
void Treap<T, Comp>::Insert_(Treap::Node *&current_node, Treap::Node *inserted_node) {
    if (current_node == nullptr) {
        current_node = inserted_node;
    } else {
        if (compT(current_node->value, inserted_node->value)) {
            Insert_((inserted_node->key < current_node->key ? current_node->l : current_node->r), inserted_node);
        } else {
            Split_(current_node, inserted_node->key, inserted_node->l, inserted_node->r);
            current_node = inserted_node;
        }
    }
}

template<class T, class Comp>
void Treap<T, Comp>::Merge_(Node *&current_node, Node *left_tree, Node *right_tree) {
    if (left_tree == nullptr || right_tree == nullptr) {
        current_node = (left_tree == nullptr ? right_tree : left_tree);
    } else {
        if (compT(left_tree->value, right_tree->value)) {
            Merge_(left_tree->l, left_tree->l, right_tree);
            current_node = left_tree;
        } else {
            Merge_(right_tree->r, left_tree, right_tree->r);
            current_node = right_tree;
        }
    }
}

template<class T, class Comp>
T Treap<T, Comp>::Top() {
    return root_->value;
}

template<class T, class Comp>
void Treap<T, Comp>::Pop() {
    --size_;
    Merge_(root_, root_->l, root_->r);
}

template<class T, class Comp>
bool Treap<T, Comp>::Empty() const {
    return size_ == 0;
}

template<class T, class Comp>
size_t Treap<T, Comp>::Size() const {
    return size_;
}

template<class T, class Comp>
T Treap<T, Comp>::CutTop() {
    T value = Top();
    Pop();
    return value;
}

template<class T, class Comp>
Treap<T, Comp>::~Treap() {
    delete root_;
}
