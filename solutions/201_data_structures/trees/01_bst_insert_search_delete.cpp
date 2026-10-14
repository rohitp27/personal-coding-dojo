// Problem: Binary search tree -- insert, search, delete.
//   Node { int val; Node* left; Node* right; }   raw pointers, no parent pointer
//   Node* insert(Node* root, int val)   returns the (possibly new) root; a value already present is ignored
//   bool  contains(Node* root, int val)
//   Node* erase(Node* root, int val)    returns the new root; absent val => tree unchanged; delete the removed node
//
// Hint: delete has three cases: 0, 1 or 2 children. With two children, copy the in-order successor
//   (smallest value in the right subtree) into the node, then erase that successor from the right subtree.
//
// Examples:
//   insert 50,30,70,20,40,60,80 -> in-order walk is 20 30 40 50 60 70 80
//   erase 20 (leaf), 30 (one child), 70 (two children), 50 (root) -> still a valid BST each time
//
// Constraints: values are ints, up to ~200,000 nodes, input order is arbitrary (tree may be unbalanced).
// Required complexity: O(h) time per operation, h = tree height. No auxiliary containers.
// Run: make run FILE=curriculum/201_data_structures/trees/01_bst_insert_search_delete.cpp

#include "check.h"

#include <algorithm>
#include <climits>
#include <initializer_list>
#include <numeric>
#include <random>
#include <vector>

struct Node {
    int val;
    Node* left = nullptr;
    Node* right = nullptr;
    explicit Node(int v) : val(v) {}
};

// ===== YOUR CODE BELOW (stub) =====
Node* insert(Node* root, int val) {
    if (!root) return new Node(val);
    if (val < root->val) root->left = insert(root->left, val);
    else if (val > root->val) root->right = insert(root->right, val);
    return root;
}
bool contains(Node* root, int val) {
    while (root) {
        if (val == root->val) return true;
        root = val < root->val ? root->left : root->right;
    }
    return false;
}
Node* erase(Node* root, int val) {
    if (!root) return nullptr;
    if (val < root->val) { root->left = erase(root->left, val); return root; }
    if (val > root->val) { root->right = erase(root->right, val); return root; }
    if (!root->left) { Node* r = root->right; delete root; return r; }
    if (!root->right) { Node* l = root->left; delete root; return l; }
    Node* s = root->right;
    while (s->left) s = s->left;
    root->val = s->val;
    root->right = erase(root->right, s->val);
    return root;
}
// ===== TESTS (do not edit below) =====
static void inorderInto(Node* n, std::vector<int>& out) {
    if (!n) return;
    inorderInto(n->left, out);
    out.push_back(n->val);
    inorderInto(n->right, out);
}
static std::vector<int> inorder(Node* n) {
    std::vector<int> out;
    inorderInto(n, out);
    return out;
}
static bool validBST(Node* n, long long lo, long long hi) {
    if (!n) return true;
    if (n->val <= lo || n->val >= hi) return false;
    return validBST(n->left, lo, n->val) && validBST(n->right, n->val, hi);
}
static void destroy(Node* n) {
    if (!n) return;
    destroy(n->left);
    destroy(n->right);
    delete n;
}
static Node* build(std::initializer_list<int> vals) {
    Node* r = nullptr;
    for (int v : vals) r = insert(r, v);
    return r;
}
using V = std::vector<int>;

int main() {
    // empty tree
    CHECK(!contains(nullptr, 1));
    CHECK(erase(nullptr, 1) == nullptr);

    // basic build + search
    Node* root = build({50, 30, 70, 20, 40, 60, 80});
    CHECK_EQ(inorder(root), (V{20, 30, 40, 50, 60, 70, 80}));
    CHECK(validBST(root, LLONG_MIN, LLONG_MAX));
    for (int v : {20, 30, 40, 50, 60, 70, 80}) CHECK(contains(root, v));
    for (int v : {0, 25, 55, 65, 100, -5}) CHECK(!contains(root, v));

    // duplicates are ignored
    root = insert(root, 50);
    root = insert(root, 20);
    CHECK_EQ(inorder(root), (V{20, 30, 40, 50, 60, 70, 80}));

    // erase leaf, one-child node, two-children node, absent value
    root = erase(root, 20);
    CHECK_EQ(inorder(root), (V{30, 40, 50, 60, 70, 80}));
    CHECK(!contains(root, 20));
    root = erase(root, 30);  // only a right child (40) now
    CHECK_EQ(inorder(root), (V{40, 50, 60, 70, 80}));
    root = erase(root, 70);  // two children (60, 80)
    CHECK_EQ(inorder(root), (V{40, 50, 60, 80}));
    root = erase(root, 999);
    CHECK_EQ(inorder(root), (V{40, 50, 60, 80}));
    CHECK(validBST(root, LLONG_MIN, LLONG_MAX));

    // erase the root repeatedly (root has two children, then one, then none)
    root = erase(root, 50);
    CHECK_EQ(inorder(root), (V{40, 60, 80}));
    CHECK(validBST(root, LLONG_MIN, LLONG_MAX));
    root = erase(root, 60);
    root = erase(root, 40);
    CHECK_EQ(inorder(root), (V{80}));
    root = erase(root, 80);
    CHECK(root == nullptr);
    destroy(root);

    // single node, re-insert after emptying
    root = insert(nullptr, 7);
    CHECK(contains(root, 7));
    root = erase(root, 7);
    CHECK(root == nullptr);
    root = insert(root, 8);
    CHECK_EQ(inorder(root), (V{8}));
    destroy(root);

    // degenerate (sorted) input: height n, still correct
    root = nullptr;
    for (int i = 1; i <= 2000; i++) root = insert(root, i);
    CHECK_EQ(inorder(root).size(), 2000u);
    for (int i = 1; i <= 1000; i++) root = erase(root, i);  // always erasing the root
    CHECK_EQ(inorder(root).size(), 1000u);
    CHECK(!contains(root, 1000));
    CHECK(contains(root, 1001));
    destroy(root);

    // negative and extreme values
    root = build({0, INT_MIN, INT_MAX, -1, 1});
    CHECK_EQ(inorder(root), (V{INT_MIN, -1, 0, 1, INT_MAX}));
    root = erase(root, INT_MIN);
    root = erase(root, INT_MAX);
    CHECK_EQ(inorder(root), (V{-1, 0, 1}));
    destroy(root);

    // large random: 200k keys, then erase all evens in random order
    {
        const int N = 200000;
        std::vector<int> keys(N);
        std::iota(keys.begin(), keys.end(), 0);
        std::mt19937 rng(12345);
        std::shuffle(keys.begin(), keys.end(), rng);
        root = nullptr;
        for (int k : keys) root = insert(root, k);
        std::vector<int> all(N);
        std::iota(all.begin(), all.end(), 0);
        CHECK_EQ(inorder(root), all);
        std::shuffle(keys.begin(), keys.end(), rng);
        std::vector<int> odds;
        for (int k : keys)
            if (k % 2 == 0) root = erase(root, k);
        for (int i = 1; i < N; i += 2) odds.push_back(i);
        CHECK_EQ(inorder(root), odds);
        CHECK(validBST(root, LLONG_MIN, LLONG_MAX));
        CHECK(contains(root, 99999));
        CHECK(!contains(root, 100000));
        destroy(root);
    }
    return check::report();
}
