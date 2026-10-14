// Problem: Implement a singly linked list.
//   - Node struct: holds an int value and a pointer to the next Node.
//   - LinkedList class:
//       - pushFront(int val): insert a new node at the head.
//       - pushBack(int val): insert a new node at the tail.
//       - toVector() const: return a vector<int> of the values, head to tail (for testing).
//       - Destructor: frees every node (no memory leaks).
//
// This is your first hand-rolled use of `new` to build a structure of linked objects,
// and a destructor whose job is to walk the list and `delete` each node.
//
// Run with: make run FILE=practice/cpp/linked_lists/singly_linked_list.cpp

#include <cassert>
#include <iostream>
#include <vector>

struct Node {
    int value;
    Node* next;

    // TODO: constructor(int value) that sets value and next = nullptr
    Node(int value) {
        this->value = value;
        this->next = nullptr;
    }
};

class LinkedList {
public:
    LinkedList() : head_(nullptr), tail_(nullptr) {}

    // TODO: ~LinkedList() destructor — walk the list, delete each Node
    ~LinkedList() {
        Node* nextNode = head_;
        while (nextNode != nullptr) {
            Node* current = nextNode;
            nextNode = current->next;
            delete current;
        }
    }

    // TODO: void pushFront(int val)
    void pushFront(int val) {
        if ( isEmpty() ) {
            init(val);
            return;
        }
        Node* node = makeNode(val);
        node->next = head_;
        head_ = node;
    }

    // TODO: void pushBack(int val)
    void pushBack(int val) {
        if ( isEmpty() ) {
            init(val);
            return;
        }
        Node* node = makeNode(val);
        tail_->next = node;
        tail_ = node;
    }

    std::vector<int> toVector() const {
        std::vector<int> result;
        // TODO: walk from head_ to nullptr, appending each value
        Node* next = head_;
        while (next != nullptr) {
            result.push_back(next->value);
            next = next->next;
        }
        return result;
    }

private:
    Node* head_;
    Node* tail_;

    bool isEmpty() {
        return head_ == nullptr && tail_ == nullptr;
    }

    Node* makeNode(int val) {
        return new Node(val);
    }

    void init(int val) {
        Node* node = makeNode(val);
        this->head_ = node;
        this->tail_ = node;
    }
};

int main() {
    LinkedList list;
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    list.pushFront(0);

    std::vector<int> expected = {0, 1, 2, 3};
    assert(list.toVector() == expected);

    std::cout << "all tests passed\n";
    return 0;
}
