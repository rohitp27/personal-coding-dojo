// Problem: Design a codec that turns a binary tree of ints into a std::string and back.
//   std::string serialize(Node* root)
//   Node*       deserialize(const std::string& s)     returns a freshly allocated tree (caller frees)
//   Round trip: deserialize(serialize(t)) must have exactly the same SHAPE and values as t (not just the same
//   in-order values).
//
// Examples:
//   serialize(nullptr) is some string that deserializes back to nullptr
//   trees (1 with left child 0), (1 with right child 0) and (10) must all serialize to DIFFERENT strings
//   two separately built but identical trees must serialize to identical strings
//
// Assumptions (chosen for this exercise, interview-style; the format itself is yours to design):
//   * values are arbitrary 32-bit ints (negative, INT_MIN, INT_MAX included); the tree may be empty
//   * up to 200,000 nodes; the tree may be a single chain 100,000 deep, so decide whether recursion is safe
//   * deserialize is only ever given strings that serialize produced (no malformed-input handling needed)
//   * the string must stay linear in size: at most 20 characters per node on average
// Required complexity: O(n) time and O(n) space for both directions (watch out for accidental O(n^2) string work).
// Run: make run FILE=curriculum/201_data_structures/trees/04_serialize_deserialize.cpp
//
// BEFORE CODING: write your own approach in a comment block right here, at the top of your solution:
//   approach (what goes in the string, how null children and delimiters are represented),
//   invariant (what the decoder knows after reading k tokens),
//   complexity (time, space, and the worst-case stack depth).

#include "check.h"

#include <climits>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

struct Node {
    int val;
    Node* left = nullptr;
    Node* right = nullptr;
    explicit Node(int v) : val(v) {}
};

// ===== YOUR CODE BELOW (stub) =====
std::string serialize(Node* root) {
    std::string out;
    std::vector<Node*> st;
    st.push_back(root);
    while (!st.empty()) {
        Node* n = st.back(); st.pop_back();
        if (!n) { out += "# "; continue; }
        out += std::to_string(n->val);
        out += ' ';
        st.push_back(n->right);
        st.push_back(n->left);
    }
    return out;
}
Node* deserialize(const std::string& s) {
    size_t pos = 0;
    auto next = [&]() -> std::string {
        size_t e = s.find(' ', pos);
        std::string tok = s.substr(pos, e - pos);
        pos = e + 1;
        return tok;
    };
    if (s.empty()) return nullptr;
    std::string tok = next();
    if (tok == "#") return nullptr;
    Node* root = new Node(std::stoi(tok));
    // stack of (node, state): 0 = needs left, 1 = needs right
    std::vector<std::pair<Node*, int>> st;
    st.push_back({root, 0});
    while (!st.empty() && pos < s.size()) {
        auto& [n, state] = st.back();
        Node* parent = n;
        int which = state;
        if (which == 0) state = 1; else st.pop_back();
        tok = next();
        if (tok == "#") continue;
        Node* child = new Node(std::stoi(tok));
        (which == 0 ? parent->left : parent->right) = child;
        st.push_back({child, 0});
    }
    return root;
}
// ===== TESTS (do not edit below) =====
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
static bool same(Node* a, Node* b) {  // iterative structural equality
    std::vector<std::pair<Node*, Node*>> st;
    st.push_back({a, b});
    while (!st.empty()) {
        auto [x, y] = st.back();
        st.pop_back();
        if (!x && !y) continue;
        if (!x || !y || x->val != y->val) return false;
        st.push_back({x->left, y->left});
        st.push_back({x->right, y->right});
    }
    return true;
}
// Random shape: attach each new node to a random free child slot.
static Node* randomTree(int n, std::mt19937& rng, bool wideValues) {
    if (n == 0) return nullptr;
    auto val = [&]() -> int {
        if (wideValues) return (int)rng();  // full 32-bit range incl. negatives
        return (int)(rng() % 21) - 10;
    };
    Node* root = new Node(val());
    std::vector<Node**> slots{&root->left, &root->right};
    for (int i = 1; i < n; i++) {
        size_t k = rng() % slots.size();
        Node** slot = slots[k];
        slots[k] = slots.back();
        slots.pop_back();
        *slot = new Node(val());
        slots.push_back(&(*slot)->left);
        slots.push_back(&(*slot)->right);
    }
    return root;
}
static Node* chain(int n, bool left, bool zigzag) {
    Node* root = new Node(0);
    Node* cur = root;
    for (int i = 1; i < n; i++) {
        Node* nx = new Node(i % 7 - 3);
        bool goLeft = zigzag ? (i % 2 == 0) : left;
        (goLeft ? cur->left : cur->right) = nx;
        cur = nx;
    }
    return root;
}
static bool roundTrips(Node* t) {
    std::string s = serialize(t);
    Node* back = deserialize(s);
    bool ok = same(t, back);
    destroy(back);
    return ok;
}
static Node* mk(int v, Node* l = nullptr, Node* r = nullptr) {
    Node* n = new Node(v);
    n->left = l;
    n->right = r;
    return n;
}

int main() {
    // empty
    CHECK(deserialize(serialize(nullptr)) == nullptr);

    // singles, including extreme values
    for (int v : {0, 1, -1, 10, 100, INT_MIN, INT_MAX}) {
        Node* t = mk(v);
        CHECK(roundTrips(t));
        destroy(t);
    }

    // small shapes
    Node* a = mk(1, mk(0));       // 1 with left child 0
    Node* b = mk(1, nullptr, mk(0));  // 1 with right child 0
    Node* c = mk(10);
    Node* d = mk(1, mk(0), mk(0));
    Node* e = mk(-1, mk(-2, mk(-3)), mk(4, nullptr, mk(INT_MIN)));
    for (Node* t : {a, b, c, d, e}) CHECK(roundTrips(t));
    std::set<std::string> distinct;
    for (Node* t : {a, b, c, d, e}) distinct.insert(serialize(t));
    CHECK_EQ(distinct.size(), 5u);  // ambiguity check: structure and digits must not blur together
    CHECK_EQ(serialize(a), serialize(a));
    Node* a2 = mk(1, mk(0));
    CHECK_EQ(serialize(a), serialize(a2));  // deterministic: equal trees, equal strings
    destroy(a2);
    CHECK(serialize(nullptr) != serialize(c));
    for (Node* t : {a, b, c, d, e}) destroy(t);

    // two different trees with the same in-order and same preorder values need different encodings
    {
        Node* x = mk(2, mk(1, mk(0)));          // left chain 2,1,0
        Node* y = mk(2, mk(1, nullptr, mk(0)));  // 1 has a right child
        CHECK(serialize(x) != serialize(y));
        CHECK(roundTrips(x));
        CHECK(roundTrips(y));
        destroy(x);
        destroy(y);
    }

    // many random trees, small values (many repeated digits) and full-range values
    {
        std::mt19937 rng(2024);
        int bad = 0;
        for (int iter = 0; iter < 300; iter++) {
            int n = (int)(rng() % 60);
            Node* t = randomTree(n, rng, iter % 2 == 1);
            if (!roundTrips(t)) bad++;
            destroy(t);
        }
        CHECK_EQ(bad, 0);  // number of random trees that failed the round trip
    }

    // large: deep chains (left, right, zig-zag) 100,000 deep
    for (int mode = 0; mode < 3; mode++) {
        Node* t = chain(100000, mode == 0, mode == 2);
        std::string s = serialize(t);
        CHECK(s.size() <= 20u * 100000u);
        Node* back = deserialize(s);
        CHECK(same(t, back));
        destroy(back);
        destroy(t);
    }

    // large: 200,000 random nodes, full 32-bit values
    {
        std::mt19937 rng(7);
        Node* t = randomTree(200000, rng, true);
        std::string s = serialize(t);
        CHECK(s.size() <= 20u * 200000u);
        Node* back = deserialize(s);
        CHECK(same(t, back));
        CHECK_EQ(serialize(back), s);  // re-encoding the decoded tree reproduces the string
        destroy(back);
        destroy(t);
    }
    return check::report();
}
