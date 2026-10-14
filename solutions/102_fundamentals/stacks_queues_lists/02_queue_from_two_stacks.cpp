// Problem: Queue From Two Stacks
// Implement a FIFO queue of ints using only two stacks (the two vectors below, used
// strictly through push_back / back / pop_back / empty / size - i.e. as stacks).
//
// Operations:
//   push(x)  add x to the back
//   pop()    remove and return the front element (precondition: queue not empty)
//   peek()   return the front element without removing it (precondition: not empty)
//   empty()  true when there are no elements
//   size()   number of elements
//
// Examples:
//   push 1, push 2, push 3 -> peek() == 1, pop() == 1, push 4, pop() == 2, pop() == 3,
//   pop() == 4, empty() == true.
//
// Constraints: up to ~10^6 operations in total.
// Required complexity: AMORTIZED O(1) per operation, size()/empty() in O(1).
// Moving every element back and forth on each operation is O(n) per op and too slow here.
//
// Hint: let one stack ("in_") receive pushes and the other ("out_") serve pops and peeks.
// Only refill out_ from in_ when out_ is empty - and then move ALL of in_. Why does the
// order come out right, and why is the cost amortized O(1)?
//
// Run: make run FILE=curriculum/102_fundamentals/stacks_queues_lists/02_queue_from_two_stacks.cpp

#include "check.h"

#include <cstddef>
#include <queue>
#include <random>
#include <vector>

class MyQueue {
public:
    void push(int x) { in_.push_back(x); }
    int pop() {
        shift();
        int v = out_.back();
        out_.pop_back();
        return v;
    }
    int peek() {
        shift();
        return out_.back();
    }
    bool empty() const { return in_.empty() && out_.empty(); }
    size_t size() const { return in_.size() + out_.size(); }

private:
    void shift() {
        if (out_.empty())
            while (!in_.empty()) {
                out_.push_back(in_.back());
                in_.pop_back();
            }
    }
    std::vector<int> in_;   // receives pushes
    std::vector<int> out_;  // serves pops; its back() is the queue front
};

int main() {
    {
        MyQueue q;
        CHECK_EQ(q.empty(), true);
        CHECK_EQ(q.size(), size_t{0});
        q.push(1);
        q.push(2);
        q.push(3);
        CHECK_EQ(q.empty(), false);
        CHECK_EQ(q.size(), size_t{3});
        CHECK_EQ(q.peek(), 1);
        CHECK_EQ(q.pop(), 1);
        q.push(4);
        CHECK_EQ(q.size(), size_t{3});
        CHECK_EQ(q.pop(), 2);
        CHECK_EQ(q.peek(), 3);  // repeated peek must not consume
        CHECK_EQ(q.peek(), 3);
        CHECK_EQ(q.pop(), 3);
        CHECK_EQ(q.pop(), 4);
        CHECK_EQ(q.empty(), true);
        CHECK_EQ(q.size(), size_t{0});
        // reuse after draining
        q.push(9);
        CHECK_EQ(q.peek(), 9);
        q.push(8);
        CHECK_EQ(q.pop(), 9);
        CHECK_EQ(q.pop(), 8);
        CHECK_EQ(q.empty(), true);
    }
    {
        // interleaving pushes between pops must not reorder elements already in out_
        MyQueue q;
        q.push(1);
        q.push(2);
        CHECK_EQ(q.pop(), 1);
        q.push(3);
        q.push(4);
        CHECK_EQ(q.pop(), 2);
        CHECK_EQ(q.pop(), 3);
        q.push(5);
        CHECK_EQ(q.pop(), 4);
        CHECK_EQ(q.pop(), 5);
    }
    {
        // randomized comparison against std::queue
        std::mt19937 rng(2024);
        MyQueue q;
        std::queue<int> model;
        int bad = 0;
        for (int i = 0; i < 400000; i++) {
            int op = static_cast<int>(rng() % 5);
            if (op <= 2 || model.empty()) {
                q.push(i);
                model.push(i);
            } else if (op == 3) {
                if (q.peek() != model.front()) bad++;
            } else {
                int want = model.front();
                model.pop();
                if (q.pop() != want) bad++;
            }
            if (q.size() != model.size() || q.empty() != model.empty()) bad++;
        }
        CHECK_EQ(bad, 0);
    }
    {
        // 100,000 queued, then 100,000 x (pop, push): O(n)-per-op designs need ~10^10 steps
        MyQueue q;
        for (int i = 0; i < 100000; i++) q.push(i);
        int bad = 0;
        for (int i = 0; i < 100000; i++) {
            if (q.pop() != i) bad++;
            q.push(100000 + i);
        }
        for (int i = 0; i < 100000; i++)
            if (q.pop() != 100000 + i) bad++;
        CHECK_EQ(bad, 0);
        CHECK_EQ(q.empty(), true);
    }

    return check::report();
}
