// Problem: In-order, pre-order and post-order traversal of a binary tree WITHOUT recursion.
//   std::vector<int> inorder(Node* root)
//   std::vector<int> preorder(Node* root)
//   std::vector<int> postorder(Node* root)
//   Use an explicit stack (std::vector or std::stack). No recursive calls, no parent pointers, do not modify the tree.
//
// Examples:
//        1
//       / \
//      2   3        inorder   = 4 2 5 1 3 6
//     / \   \       preorder  = 1 2 4 5 3 6
//    4   5   6      postorder = 4 5 2 6 3 1
//
// Constraints: up to 1,000,000 nodes; the tree can be a single chain 1,000,000 deep.
// Required complexity: O(n) time, O(h) extra space (the stack), each node pushed a constant number of times.
// Run: make run FILE=curriculum/201_data_structures/trees/02_iterative_traversals.cpp
//
// Think first:
//  1. For inorder, what exactly is on the stack at the moment you visit a node?
//  2. Post-order must visit a node after BOTH children. How do you know the right subtree is already done
//     when you pop back to a node (without a visited flag per node)?
//  3. Which of the three is easiest to derive from another one, and is that derivation still O(n)?

#include "check.h"

#include <climits>
#include <queue>
#include <stack>
#include <vector>

struct Node {
    int val;
    Node* left = nullptr;
    Node* right = nullptr;
    explicit Node(int v) : val(v) {}
};

// ===== YOUR CODE BELOW (stub) =====
std::vector<int> inorder(Node* root) {
    std::vector<int> out; std::vector<Node*> st; Node* cur = root;
    while (cur || !st.empty()) {
        while (cur) { st.push_back(cur); cur = cur->left; }
        cur = st.back(); st.pop_back();
        out.push_back(cur->val);
        cur = cur->right;
    }
    return out;
}
std::vector<int> preorder(Node* root) {
    std::vector<int> out; std::vector<Node*> st;
    if (root) st.push_back(root);
    while (!st.empty()) {
        Node* n = st.back(); st.pop_back();
        out.push_back(n->val);
        if (n->right) st.push_back(n->right);
        if (n->left) st.push_back(n->left);
    }
    return out;
}
std::vector<int> postorder(Node* root) {
    std::vector<int> out; std::vector<Node*> st; Node* cur = root; Node* last = nullptr;
    while (cur || !st.empty()) {
        while (cur) { st.push_back(cur); cur = cur->left; }
        Node* top = st.back();
        if (top->right && top->right != last) { cur = top->right; }
        else { out.push_back(top->val); last = top; st.pop_back(); }
    }
    return out;
}
// ===== TESTS (do not edit below) =====
using V = std::vector<int>;
static const int N_ = INT_MIN;  // "no child" in level-order specs

// Level-order spec: children of each real node are consumed left-to-right, N_ marks a missing child.
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
static void destroy(Node* root) {  // iterative so deep chains are fine
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
static V rangeV(int lo, int hi) {  // lo..hi inclusive, ascending or descending
    V v;
    if (lo <= hi)
        for (int i = lo; i <= hi; i++) v.push_back(i);
    else
        for (int i = lo; i >= hi; i--) v.push_back(i);
    return v;
}

int main() {
    // empty and single
    CHECK_EQ(inorder(nullptr), V{});
    CHECK_EQ(preorder(nullptr), V{});
    CHECK_EQ(postorder(nullptr), V{});
    Node* one = new Node(9);
    CHECK_EQ(inorder(one), V{9});
    CHECK_EQ(preorder(one), V{9});
    CHECK_EQ(postorder(one), V{9});
    destroy(one);

    // example tree
    Node* t = fromLevel({1, 2, 3, 4, 5, N_, 6});
    CHECK_EQ(inorder(t), (V{4, 2, 5, 1, 3, 6}));
    CHECK_EQ(preorder(t), (V{1, 2, 4, 5, 3, 6}));
    CHECK_EQ(postorder(t), (V{4, 5, 2, 6, 3, 1}));
    destroy(t);

    // only-left and only-right small chains
    t = fromLevel({1, 2, N_, 3});
    CHECK_EQ(inorder(t), (V{3, 2, 1}));
    CHECK_EQ(preorder(t), (V{1, 2, 3}));
    CHECK_EQ(postorder(t), (V{3, 2, 1}));
    destroy(t);
    t = fromLevel({1, N_, 2, N_, 3});
    CHECK_EQ(inorder(t), (V{1, 2, 3}));
    CHECK_EQ(preorder(t), (V{1, 2, 3}));
    CHECK_EQ(postorder(t), (V{3, 2, 1}));
    destroy(t);

    // zig-zag: right child's left child's right child ...
    t = fromLevel({1, N_, 2, 3, N_, N_, 4});
    CHECK_EQ(inorder(t), (V{1, 3, 4, 2}));
    CHECK_EQ(preorder(t), (V{1, 2, 3, 4}));
    CHECK_EQ(postorder(t), (V{4, 3, 2, 1}));
    destroy(t);

    // negative values, left subtree only has a right child (post-order backtrack case)
    t = fromLevel({-5, -10, 7, N_, -3, 6, 8});
    CHECK_EQ(inorder(t), (V{-10, -3, -5, 6, 7, 8}));
    CHECK_EQ(preorder(t), (V{-5, -10, -3, 7, 6, 8}));
    CHECK_EQ(postorder(t), (V{-3, -10, 6, 8, 7, -5}));
    destroy(t);

    // perfect tree of 15 nodes labelled in level order
    t = fromLevel(rangeV(1, 15));
    CHECK_EQ(inorder(t), (V{8, 4, 9, 2, 10, 5, 11, 1, 12, 6, 13, 3, 14, 7, 15}));
    CHECK_EQ(preorder(t), (V{1, 2, 4, 8, 9, 5, 10, 11, 3, 6, 12, 13, 7, 14, 15}));
    CHECK_EQ(postorder(t), (V{8, 9, 4, 10, 11, 5, 2, 12, 13, 6, 14, 15, 7, 3, 1}));
    destroy(t);

    // 1,000,000-deep chains: recursion would overflow the stack
    {
        const int n = 1000000;
        Node* leftChain = new Node(0);
        Node* right = new Node(0);
        Node* l = leftChain;
        Node* r = right;
        for (int i = 1; i < n; i++) {
            l->left = new Node(i);
            l = l->left;
            r->right = new Node(i);
            r = r->right;
        }
        CHECK_EQ(inorder(leftChain), rangeV(n - 1, 0));
        CHECK_EQ(preorder(leftChain), rangeV(0, n - 1));
        CHECK_EQ(postorder(leftChain), rangeV(n - 1, 0));
        CHECK_EQ(inorder(right), rangeV(0, n - 1));
        CHECK_EQ(preorder(right), rangeV(0, n - 1));
        CHECK_EQ(postorder(right), rangeV(n - 1, 0));
        destroy(leftChain);
        destroy(right);
    }
    return check::report();
}
