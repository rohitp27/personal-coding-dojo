// Problem: Path questions on a plain binary tree (NOT a search tree; no parent pointers). Node values are unique.
//   Node* lca(Node* root, Node* p, Node* q)
//        lowest common ancestor of nodes p and q. Both are guaranteed to be in the tree. A node counts as an
//        ancestor of itself (lca(p, p) == p, and if p is an ancestor of q the answer is p).
//   std::vector<int> pathBetween(Node* root, int a, int b)
//        values along the unique simple path from the node holding a to the node holding b, inclusive, in walk
//        order. Both values exist. pathBetween(root, a, a) == {a}.
//   long long maxPathSum(Node* root)
//        maximum sum over all non-empty downward-or-turning paths (a path is any chain of connected nodes that
//        goes up then down at most once; it need not touch the root). Values may be negative.
//
// Examples (tree 3 / 5 1 / 6 2 0 8 / . . 7 4 under node 2):
//        3
//      /   \
//     5     1       lca(5, 1) = 3        lca(6, 4) = 5        lca(5, 4) = 5
//    / \   / \      pathBetween(7, 8) = 7 2 5 3 1 8
//   6   2 0   8     pathBetween(4, 5) = 4 2 5
//      / \
//     7   4
//   maxPathSum({-10, 9, 20, ., ., 15, 7}) = 42   (15 -> 20 -> 7)
//
// Constraints: up to ~130,000 nodes; depth up to 30,000; values fit in int, sums fit in long long.
// Required complexity: O(n) time and O(h) space per call. Not O(n * h) (e.g. no copying a root path per node).
// Run: make run FILE=curriculum/201_data_structures/trees/03_lca_and_paths.cpp
//
// Think first:
//  1. What does the recursive call return to its parent so the parent can decide "the answer is me"?
//  2. For maxPathSum, the value a node reports upward and the best path through that node are DIFFERENT
//     quantities. Why, and what should a negative subtree contribute?
//  3. How can pathBetween reuse a lowest-common-ancestor idea without a second tree walk per query?

#include "check.h"

#include <algorithm>
#include <climits>
#include <queue>
#include <vector>

struct Node {
    int val;
    Node* left = nullptr;
    Node* right = nullptr;
    explicit Node(int v) : val(v) {}
};

// ===== YOUR CODE BELOW (stub) =====
Node* lca(Node* root, Node* p, Node* q) {
    if (!root || root == p || root == q) return root;
    Node* l = lca(root->left, p, q);
    Node* r = lca(root->right, p, q);
    if (l && r) return root;
    return l ? l : r;
}
static bool rootPath(Node* n, int target, std::vector<int>& path) {
    if (!n) return false;
    path.push_back(n->val);
    if (n->val == target) return true;
    if (rootPath(n->left, target, path) || rootPath(n->right, target, path)) return true;
    path.pop_back();
    return false;
}
std::vector<int> pathBetween(Node* root, int a, int b) {
    std::vector<int> pa, pb;
    rootPath(root, a, pa);
    rootPath(root, b, pb);
    size_t i = 0;
    while (i < pa.size() && i < pb.size() && pa[i] == pb[i]) i++;
    // lca is pa[i-1]
    std::vector<int> out(pa.rbegin(), pa.rend() - (i - 1));
    for (size_t j = i; j < pb.size(); j++) out.push_back(pb[j]);
    return out;
}
static long long best_;
static long long gain(Node* n) {
    if (!n) return 0;
    long long l = std::max(0LL, gain(n->left));
    long long r = std::max(0LL, gain(n->right));
    best_ = std::max(best_, n->val + l + r);
    return n->val + std::max(l, r);
}
long long maxPathSum(Node* root) {
    if (!root) return 0;
    best_ = LLONG_MIN;
    gain(root);
    return best_;
}
// ===== TESTS (do not edit below) =====
using V = std::vector<int>;
static const int N_ = INT_MIN;  // "no child" in level-order specs

static Node* fromLevel(const V& v) {
    if (v.empty() || v[0] == N_) return nullptr;
    Node* root = new Node(v[0]);
    std::queue<Node*> q;
    q.push(root);
    size_t i = 1;
    while (!q.empty() && i < v.size()) {
        Node* cur = q.front();
        q.pop();
        if (i < v.size() && v[i] != N_) {
            cur->left = new Node(v[i]);
            q.push(cur->left);
        }
        i++;
        if (i < v.size() && v[i] != N_) {
            cur->right = new Node(v[i]);
            q.push(cur->right);
        }
        i++;
    }
    return root;
}
static void destroy(Node* root) {
    std::vector<Node*> st;
    if (root) st.push_back(root);
    while (!st.empty()) {
        Node* n = st.back();
        st.pop_back();
        if (n->left) st.push_back(n->left);
        if (n->right) st.push_back(n->right);
        delete n;
    }
}
static Node* at(Node* root, int val) {  // iterative find by value
    std::vector<Node*> st;
    if (root) st.push_back(root);
    while (!st.empty()) {
        Node* n = st.back();
        st.pop_back();
        if (n->val == val) return n;
        if (n->left) st.push_back(n->left);
        if (n->right) st.push_back(n->right);
    }
    return nullptr;
}
static int valOf(Node* n) { return n ? n->val : -999999; }
static int lcaVal(Node* root, int a, int b) { return valOf(lca(root, at(root, a), at(root, b))); }

int main() {
    Node* t = fromLevel({3, 5, 1, 6, 2, 0, 8, N_, N_, 7, 4});
    CHECK_EQ(lcaVal(t, 5, 1), 3);
    CHECK_EQ(lcaVal(t, 6, 4), 5);
    CHECK_EQ(lcaVal(t, 5, 4), 5);  // one is an ancestor of the other
    CHECK_EQ(lcaVal(t, 4, 5), 5);  // argument order does not matter
    CHECK_EQ(lcaVal(t, 7, 8), 3);
    CHECK_EQ(lcaVal(t, 7, 4), 2);
    CHECK_EQ(lcaVal(t, 6, 6), 6);  // same node
    CHECK_EQ(lcaVal(t, 3, 8), 3);  // root is one of the nodes
    CHECK_EQ(lcaVal(t, 0, 8), 1);

    CHECK_EQ(pathBetween(t, 7, 8), (V{7, 2, 5, 3, 1, 8}));
    CHECK_EQ(pathBetween(t, 6, 4), (V{6, 5, 2, 4}));
    CHECK_EQ(pathBetween(t, 4, 5), (V{4, 2, 5}));  // a is below b
    CHECK_EQ(pathBetween(t, 5, 4), (V{5, 2, 4}));  // a is above b
    CHECK_EQ(pathBetween(t, 3, 3), (V{3}));
    CHECK_EQ(pathBetween(t, 3, 7), (V{3, 5, 2, 7}));
    CHECK_EQ(pathBetween(t, 0, 7), (V{0, 1, 3, 5, 2, 7}));
    destroy(t);

    // single node and degenerate chain
    t = fromLevel({42});
    CHECK_EQ(lcaVal(t, 42, 42), 42);
    CHECK_EQ(pathBetween(t, 42, 42), (V{42}));
    destroy(t);
    t = fromLevel({1, 2, N_, 3, N_, 4});
    CHECK_EQ(lcaVal(t, 4, 3), 3);
    CHECK_EQ(pathBetween(t, 4, 1), (V{4, 3, 2, 1}));
    CHECK_EQ(pathBetween(t, 1, 4), (V{1, 2, 3, 4}));
    destroy(t);

    // maxPathSum
    CHECK_EQ(maxPathSum(nullptr), 0LL);
    t = fromLevel({1, 2, 3});
    CHECK_EQ(maxPathSum(t), 6LL);
    destroy(t);
    t = fromLevel({-10, 9, 20, N_, N_, 15, 7});
    CHECK_EQ(maxPathSum(t), 42LL);
    destroy(t);
    t = fromLevel({-3});
    CHECK_EQ(maxPathSum(t), -3LL);  // must contain at least one node
    destroy(t);
    t = fromLevel({-2, -1, -3});
    CHECK_EQ(maxPathSum(t), -1LL);
    destroy(t);
    t = fromLevel({2, -1});
    CHECK_EQ(maxPathSum(t), 2LL);  // do not add a negative child
    destroy(t);
    t = fromLevel({5, 4, 8, 11, N_, 13, 4, 7, 2, N_, N_, N_, 1});
    CHECK_EQ(maxPathSum(t), 48LL);  // 7 -> 11 -> 4 -> 5 -> 8 -> 13
    destroy(t);
    t = fromLevel({-1, 10, N_, -20, 30});  // best path lives entirely below the root
    CHECK_EQ(maxPathSum(t), 40LL);
    destroy(t);
    t = fromLevel({INT_MAX, INT_MAX, INT_MAX});
    CHECK_EQ(maxPathSum(t), 3LL * INT_MAX);
    destroy(t);

    // large: perfect tree with 17 levels of all ones (131071 nodes): longest path = 16 + 1 + 16 nodes
    {
        V spec(131071, 1);
        t = fromLevel(spec);
        CHECK_EQ(maxPathSum(t), 33LL);
        destroy(t);
    }

    // large: caterpillar. Spine nodes 0..n-1 chained by left children; spine node i has a right leaf n+i.
    {
        const int n = 30000;
        std::vector<Node*> spine(n), leaf(n);
        for (int i = 0; i < n; i++) spine[i] = new Node(i);
        for (int i = 0; i < n; i++) leaf[i] = new Node(n + i);
        for (int i = 0; i + 1 < n; i++) spine[i]->left = spine[i + 1];
        for (int i = 0; i < n; i++) spine[i]->right = leaf[i];
        Node* root = spine[0];
        CHECK_EQ(valOf(lca(root, leaf[100], leaf[n - 1])), 100);
        CHECK_EQ(valOf(lca(root, leaf[n - 1], spine[n - 1])), n - 1);
        CHECK_EQ(valOf(lca(root, leaf[0], leaf[n - 1])), 0);
        CHECK_EQ(valOf(lca(root, spine[n / 2], spine[n - 1])), n / 2);
        V expected{n};  // leaf of spine 0
        for (int i = 0; i < n; i++) expected.push_back(i);
        expected.push_back(2 * n - 1);
        CHECK_EQ(pathBetween(root, n, 2 * n - 1), expected);
        // best path: leaf of spine 1 -> spine 1 .. spine n-1 -> leaf of spine n-1 (skips spine 0, whose value is 0,
        // to start at the larger leaf n+1 instead of n)
        long long spineSum = (long long)(n - 1) * n / 2;
        CHECK_EQ(maxPathSum(root), spineSum + (n + 1) + (2 * n - 1));
        destroy(root);
    }
    return check::report();
}
