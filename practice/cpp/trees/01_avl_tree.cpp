// Problem: Implement an AVL balanced binary search tree (insertion).
// Write insert below, including any helpers/rotations you need.
// Contract:
// - Return the possibly new root after inserting key. Ignore duplicate keys.
// - Strict BST ordering: left keys < node key < right keys.
// - Every node's subtree heights differ by at most 1.
// - Store correct heights: null has height 0; a leaf has height 1.
// - Preserve existing nodes; allocate only for new keys. No global state.
// - Target O(log n) time per insertion and O(log n) auxiliary stack space.
// Deletion is outside this exercise. Test cleanup owns the returned tree.
// Run: make run FILE=practice/cpp/trees/01_avl_tree.cpp

#include "check.h"
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <numeric>
#include <random>
#include <unordered_set>
#include <vector>
#include <assert.h>

struct Node {
    int key;
    int height = 1;
    Node* left = nullptr;
    Node* right = nullptr;
    explicit Node(int value) : key(value) {}
};

int max(int a, int b) {
    return (a<b)?b:a;
}

int abs(int a) {
    return (a<0)?(-1*a):a;
}

int getCurrentHeight(Node* root) {
    if (root == nullptr) {
        return 1;
    }
    int leftHeight = (root->left == nullptr)?0:root->left->height;
    int rightHeight = (root->right == nullptr)?0:root->right->height;
    return 1+max(leftHeight, rightHeight);
}

bool isBalanced(Node* root) {
    int leftHeight = (root->left == nullptr)?0:root->left->height;
    int rightHeight = (root->right == nullptr)?0:root->right->height;
    return abs(leftHeight - rightHeight) <= 1;
}

int heightFactor(Node* root) {
    int leftHeight = (root->left == nullptr)?0:root->left->height;
    int rightHeight = (root->right == nullptr)?0:root->right->height;
    return leftHeight - rightHeight;
}

bool rightHeavy(Node* root) {
    return heightFactor(root) < 0;
}

bool leftHeavy(Node* root) {
    return heightFactor(root) > 0; //left height > right height
}

Node* rotateLeft(Node* root);

Node* rotateRight(Node* root) {
    if (root == nullptr) {
        return root;
    }

    if(root->left == nullptr) {
        return root;
    }

    if (rightHeavy(root->left)) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    Node* newRoot = root->left;
    Node* newRight = root;
    Node* rightSubtreeOfNewRoot = newRoot->right;

    // assign for new root
    newRoot->left = newRoot->left; // no touch
    newRoot->right = newRight;

    // assign for the old root
    newRight->left = rightSubtreeOfNewRoot;
    newRight->right = newRight->right; // unchanged
    
    //heights
    newRight->height = getCurrentHeight(newRight);
    newRoot->height = getCurrentHeight(newRoot);
    assert(isBalanced(newRoot));

    return newRoot;
}

Node* rotateLeft(Node* root) {
    if (root == nullptr) {
        return root;
    }

    if (root->right == nullptr) {
        // nothing to rotate left
        return root;
    }

    if (leftHeavy(root->right)) {
        // double rotation. Need to rotate the newRoot
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    // get here when single rotation
    Node* newRoot = root->right;
    Node* newLeftNode = root;
    Node* leftSubtreeOfRightChild = root->right->left;

    newRoot->left = newLeftNode;
    newRoot->right = newRoot->right; // unchanged /no-op

    newLeftNode->left = newLeftNode->left; //unchanged
    newLeftNode->right = leftSubtreeOfRightChild;
    newLeftNode->height = getCurrentHeight(newLeftNode);
    newRoot->height = getCurrentHeight(newRoot);

    assert(isBalanced(newRoot));
    return newRoot;
}

Node* balanceAndReturn(Node* root) {
    if (root == nullptr || isBalanced(root)) {
        root->height = getCurrentHeight(root);
        return root;
    }
    if (heightFactor(root) > 1) {
        return rotateRight(root);
    } else if (heightFactor(root) < 1) {
        return rotateLeft(root);
    } else {
        // seems balanced?
        root->height = getCurrentHeight(root);
        return root;
    }
}

Node* insert([[maybe_unused]] Node* root, [[maybe_unused]] int key) {
    if (root == nullptr) {
        Node* node = new Node(key);
        return node;
    }
    int rootKey = root->key;
    if (rootKey == key) {
        // ignore duplicates
        return root;
    } else if (key < rootKey) {
        root->left = insert(root->left, key);
    } else {
        root->right = insert(root->right, key);
    }
    return balanceAndReturn(root);
}

// ---- checks: keep unchanged ----
struct Inspection {
    bool valid;
    int height;
};

static Inspection inspect(Node* node, long long low, long long high,
                          std::unordered_set<Node*>& seen,
                          std::vector<int>& inorder) {
    if (!node) return {true, 0};
    if (!seen.insert(node).second) return {false, 0};
    const auto left = inspect(node->left, low, node->key, seen, inorder);
    inorder.push_back(node->key);
    const auto right = inspect(node->right, node->key, high, seen, inorder);
    const int height = 1 + std::max(left.height, right.height);
    return {left.valid && right.valid && low < node->key && node->key < high
                && std::abs(left.height - right.height) <= 1
                && node->height == height,
            height};
}

static void checkSequence(const std::vector<int>& keys) {
    Node* root = nullptr;
    std::vector<int> expected;
    for (int key : keys) {
        root = insert(root, key);
        expected.push_back(key);
        std::sort(expected.begin(), expected.end());
        expected.erase(std::unique(expected.begin(), expected.end()), expected.end());
        std::unordered_set<Node*> seen;
        std::vector<int> actual;
        const auto state = inspect(root, LLONG_MIN, LLONG_MAX, seen, actual);
        CHECK(state.valid);
        CHECK_EQ(actual, expected);
        if (!state.valid || actual != expected) break;
    }
    // Collect before freeing, so even an accidental cycle is safe to clean up.
    std::unordered_set<Node*> allocated;
    std::vector<Node*> pending{root};
    while (!pending.empty()) {
        Node* node = pending.back();
        pending.pop_back();
        if (!node || !allocated.insert(node).second) continue;
        pending.push_back(node->left);
        pending.push_back(node->right);
    }
    for (Node* node : allocated) delete node;
}

int main() {
    checkSequence({});
    checkSequence({42});
    checkSequence({30, 20, 10});
    checkSequence({10, 20, 30});
    checkSequence({30, 10, 20});
    checkSequence({10, 30, 20});
    checkSequence({5, 5, 5, 3, 7, 3, 7});
    checkSequence({0, INT_MIN, INT_MAX, -1, 1});
    checkSequence({50, 25, 75, 10, 30, 60, 90, 5, 8, 7, 6});
    std::vector<int> keys(1000);
    std::iota(keys.begin(), keys.end(), -500);
    checkSequence(keys);
    std::reverse(keys.begin(), keys.end());
    checkSequence(keys);
    std::mt19937 rng(2026);
    std::shuffle(keys.begin(), keys.end(), rng);
    checkSequence(keys);
    return check::report();
}
