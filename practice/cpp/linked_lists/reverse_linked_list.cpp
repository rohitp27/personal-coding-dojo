// Problem: Given the head of a singly linked list, reverse it in place and return
// the new head. Do this iteratively, using only pointers (no recursion, no extra
// container) — track previous/current/next as you walk the list.
//
// Classic whiteboard question — the goal is to get comfortable re-pointing `next`
// pointers without losing track of the rest of the list.
//
// Run with: make run FILE=practice/cpp/linked_lists/reverse_linked_list.cpp

#include <cassert>
#include <iostream>
#include <vector>

struct Node {
    int value;
    Node* next;
    Node(int v) : value(v), next(nullptr) {}
};

Node* reverseList(Node* head) {
    // TODO: implement iteratively with prev/curr/next pointers
    Node* prev = nullptr;
    Node* curr = head;

    while (curr != nullptr){
        Node* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

// Helpers for building/reading lists in tests — no need to modify these.
Node* buildList(const std::vector<int>& values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : values) {
        Node* node = new Node(v);
        if (!head) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

std::vector<int> toVector(Node* head) {
    std::vector<int> result;
    for (Node* curr = head; curr != nullptr; curr = curr->next) {
        result.push_back(curr->value);
    }
    return result;
}

void freeList(Node* head) {
    while (head != nullptr) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    Node* head = buildList({1, 2, 3, 4, 5});
    Node* reversed = reverseList(head);

    std::vector<int> expected = {5, 4, 3, 2, 1};
    assert(toVector(reversed) == expected);

    freeList(reversed);

    std::cout << "all tests passed\n";
    return 0;
}
