// Problem: Implement a fixed-capacity FIFO queue of ints as a ring buffer
// (circular array). No std::queue; use a std::vector<int> as storage and wrap
// indices with modulo.
//   RingBuffer(capacity)   capacity >= 1
//   push(x)    appends x at the back; returns false if the buffer is full
//   pop(out)   removes the front element into `out`; returns false if empty
//   size()     number of elements stored
//   empty()    true if size() == 0
//   full()     true if size() == capacity
//
// Example (capacity 3): push 1,2,3 -> push 4 fails; pop gives 1; push 4 now succeeds
// and lands in the slot that pop freed (wrap-around); pops then give 2, 3, 4.
//
// Edge cases: capacity 1, wrap-around many times, pop on empty.
//
// Run with: make run FILE=practice/cpp/stacks_queues/04_ring_buffer.cpp
//
// Syntax reminder:
//   explicit RingBuffer(size_t capacity) : buf_(capacity) {}    // member-initializer list
//   buf_[(head_ + count_) % buf_.size()] = x;                   // back slot
//   out = buf_[head_];  head_ = (head_ + 1) % buf_.size();      // pop from front
//   bool pop(int& out);                                         // out-parameter by reference
//   size_t is unsigned: compare with other size_t values

#include <cstddef>
#include <vector>

#include "check.h"

class RingBuffer {
public:
    explicit RingBuffer(size_t capacity) : buf_(capacity) {
        buffer = std::vector<int> (capacity);
        head = 0;
        tail = 0;
    }

    bool push(int x) {
        if (full()) return false;
        (void)x;
        store_at(tail++, x);
        return true;
    }

    bool pop(int& out) {
        if (empty()) return false;
        (void)out;
        out = value_at(head++);
        return true;
    }

    size_t size() const {
        // TODO
        return tail-head;
        return 999;
    }

    bool empty() const {
        return (head == tail);
    }

    bool full() const {
        return size() >= capacity;
    }

private:
    int value_at(size_t index) {
        return buffer[index % capacity];
    }

    void store_at(size_t index, int value) {
        buffer[index % capacity] = value;
    }

    std::vector<int> buffer;
    size_t head;
    size_t tail;
    [[maybe_unused]] size_t head_ = 0;
    [[maybe_unused]] size_t count_ = 0;
};

int main() {
    RingBuffer rb(3);
    int out = -1;
    CHECK(rb.empty());
    CHECK(!rb.full());
    CHECK_EQ(rb.size(), size_t{0});
    CHECK(!rb.pop(out));

    CHECK(rb.push(1));
    CHECK(rb.push(2));
    CHECK(rb.push(3));
    CHECK(rb.full());
    CHECK(!rb.push(4));
    CHECK_EQ(rb.size(), size_t{3});

    CHECK(rb.pop(out));
    CHECK_EQ(out, 1);
    CHECK(!rb.full());
    CHECK(rb.push(4));  // wraps around
    CHECK(rb.full());

    CHECK(rb.pop(out));
    CHECK_EQ(out, 2);
    CHECK(rb.pop(out));
    CHECK_EQ(out, 3);
    CHECK(rb.pop(out));
    CHECK_EQ(out, 4);
    CHECK(rb.empty());
    CHECK(!rb.pop(out));

    // Many wrap-arounds with a single slot.
    RingBuffer one(1);
    for (int i = 0; i < 5; i++) {
        CHECK(one.push(i));
        CHECK(!one.push(99));
        CHECK(one.pop(out));
        CHECK_EQ(out, i);
    }

    // Steady-state streaming through a small buffer.
    RingBuffer two(2);
    int next = 0, expect = 0;
    for (int round = 0; round < 10; round++) {
        CHECK(two.push(next++));
        CHECK(two.push(next++));
        CHECK(two.pop(out));
        CHECK_EQ(out, expect++);
        CHECK(two.pop(out));
        CHECK_EQ(out, expect++);
    }

    return check::report();
}
