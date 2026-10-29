// Problem: Implement a fixed-capacity stack of ints backed by a plain array.
//   push(x)    adds x on top; returns false (and changes nothing) if the stack is full
//   pop()      removes and returns the top element; throws std::out_of_range if empty
//   top()      returns the top element without removing it; throws if empty
//   empty()    true if there are no elements
//   size()     number of elements
// Capacity is the constant CAP (4).
//
// Example:
//   push(1), push(2) -> top() == 2; pop() == 2; pop() == 1; empty() == true
//
// Edge cases: pop/top on empty throw; a 5th push returns false; push after pop works.
//
// Run with: make run FILE=practice/cpp/stacks_queues/01_array_stack.cpp
//
// Syntax reminder:
//   class IntStack {
//    public:
//       bool push(int x);
//       int top() const;               // const: does not modify the object
//    private:
//       int data_[CAP];  int size_ = 0;
//   };
//   data_[size_++] = x;                // store, then increment
//   return data_[--size_];             // decrement, then read
//   throw std::out_of_range("empty");  // <stdexcept>

#include <stdexcept>

#include "check.h"

class IntStack {
public:
    static constexpr int CAP = 4;

    bool push(int x) {
        (void)x;
        if (full()){
            return false;
        } else {
            data_[size_++] = x;
            return true;
        }
    }

    int pop() {
        assertExists();
        int ret = data_[--size_];
        return ret;
    }

    int top() const {
        assertExists();
        return data_[size_-1];
    }

    bool empty() const {
        return size_ == 0;
    }

    int size() const {
        return size_;
    }

private:
    bool full() const {
        return size() == CAP;
    }

    void assertExists() const {
        if (empty()) throw std::out_of_range("Empty");
    }

    [[maybe_unused]] int data_[CAP];
    [[maybe_unused]] int size_ = 0;
};

template <typename F>
static bool throwsOutOfRange(F f) {
    try {
        f();
    } catch (const std::out_of_range&) {
        return true;
    }
    return false;
}

int main() {
    IntStack s;
    CHECK(s.empty());
    CHECK_EQ(s.size(), 0);
    CHECK(throwsOutOfRange([&] { s.pop(); }));
    CHECK(throwsOutOfRange([&] { s.top(); }));

    CHECK(s.push(1));
    CHECK(!s.empty());
    CHECK_EQ(s.top(), 1);
    CHECK(s.push(2));
    CHECK(s.push(3));
    CHECK_EQ(s.size(), 3);
    CHECK_EQ(s.top(), 3);
    CHECK_EQ(s.pop(), 3);
    CHECK_EQ(s.pop(), 2);
    CHECK_EQ(s.size(), 1);

    CHECK(s.push(10));
    CHECK(s.push(20));
    CHECK(s.push(30));
    CHECK_EQ(s.size(), IntStack::CAP);
    CHECK(!s.push(40));
    CHECK_EQ(s.size(), IntStack::CAP);
    CHECK_EQ(s.pop(), 30);
    CHECK_EQ(s.pop(), 20);
    CHECK_EQ(s.pop(), 10);
    CHECK_EQ(s.pop(), 1);
    CHECK(s.empty());

    return check::report();
}
