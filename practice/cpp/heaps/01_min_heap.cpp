// Problem: Build a Min-Heap
// Store integers in a vector representing a complete binary tree.
// Implement push(value), top(), and pop() without std::priority_queue or
// the standard heap algorithms. Duplicates and negative values are allowed.
// top() returns the minimum without removing it; pop() removes the minimum.
// Both top() and pop() must throw std::out_of_range on an empty heap.
// Target: top/empty/size O(1); push amortized O(log n), pop O(log n).
// Vector storage: O(n). Use iterative repairs with O(1) auxiliary space.
//
// Index reference (zero-based):
//   parent: (i - 1) / 2, only when i > 0
//   left:   2*i + 1, only when < size
//   right:  2*i + 2, only when < size
//
// Start with push and top, then implement pop.
// Write your heap invariant before coding.
// Run: make run FILE=practice/cpp/heaps/01_min_heap.cpp

#include "check.h"
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>

class MinHeap {
public:
    bool empty() const { return data.empty(); }
    size_t size() const { return data.size(); }

    void push( int value) {
        data.push_back(value);
        shiftUp(data.size()-1);
    }

    int top() const {
        if (empty()) {
            throw std::out_of_range("Out of range");
        }
        return data.front();
    }

    void pop() {
        if (empty()) {
            throw std::out_of_range("Out of range");
        }
        if (data.size() != 1) {
            data[0] = data.back();
        }
        data.pop_back();
        if (!empty()) shiftDown(0);
        return;
    }

private:
    int max(int a, int b) {
        return (a<b)?b:a;
    }

    void swapValues(int i, int j) {
        int temp = data[i];
        data[i] = data[j];
        data[j] = temp;
        return;
    }

    void shiftDown(size_t i) {
        size_t current = i;
        while(current != data.size()-1) {
            size_t left = 2*current + 1;
            size_t right = 2*current + 2;

            if (right > data.size()-1) {
                if (left > data.size()-1) {
                    return;
                } else if (data[current] > data[left]) {
                    swapValues(current, left);
                    current = left;
                    continue;
                } else {
                    // no swap needed
                    return;
                }
            } else {
                //both are valid here
                int rightVal = data[right];
                int leftVal = data[left];
                int datai = data[current];
                if (datai > rightVal && datai > leftVal) {
                    if (leftVal < rightVal) {
                        // swap with left
                        swapValues(current, left);
                        current = left;
                        continue;
                    } else {
                        // swap with right
                        swapValues(current, right);
                        current = right;
                        continue;
                    }
                } else if (datai > rightVal) {
                    // swap with right
                    swapValues(current, right);
                    current = right;
                    continue;
                } else if (datai > leftVal) {
                    // swap with left
                    swapValues(current, left);
                    current = left;
                    continue;
                } else {
                    return;
                }
            }
        }
    }

    void shiftDown2(size_t i) {
        if (i == data.size()-1) {
            //last element, cant shift down further
            return;
        }
        size_t left = 2*i + 1;
        size_t right = 2*i + 2;
        
        if (right >= data.size()) { //nothing on the right
            // check left
            if (left >= data.size()) { //nothing on the right
                return;
            } else if (data[i] > data[left]) {
                // swap
                swapValues(i, left);
                shiftDown(left);
                return;
            } else {
                return;
            }
        } else {
            //both are valid here
            int rightVal = data[right];
            int leftVal = data[left];
            int datai = data[i];
            if (datai > rightVal && datai > leftVal) {
                if (leftVal < rightVal) {
                    // swap with left
                    swapValues(i, left);
                    shiftDown(left);
                } else {
                    // swap with right
                    swapValues(i, right);
                    shiftDown(right);
                }
            } else if (datai > rightVal) {
                // swap with right
                swapValues(i, right);
                shiftDown(right);
            } else if (datai > leftVal) {
                // swap with left
                swapValues(i, left);
                shiftDown(left);
            }
            return;
        }
    }

    void shiftUp(size_t i) {
        size_t current = i;
        while(current != 0) {
            size_t parent;
            if (current%2 == 1) { //left child
                parent = (current-1)/2;
            } else {
                parent = (current-2)/2; 
            }
            if (data[parent] > data[current]) {
                swapValues(current, parent);
                current = parent;
            } else {
                break;
            }
        }
    }

    std::vector<int> data;
};

// ---- checks: keep these unchanged ----
// Bounds on draining keep an unfinished pop() from hanging the tests.
static std::vector<int> drain(MinHeap& heap, size_t limit) {
    std::vector<int> out;
    while (!heap.empty() && out.size() < limit) {
        out.push_back(heap.top());
        heap.pop();
    }
    return out;
}

static void checkEmptyErrors(MinHeap& heap) {
    bool topThrew = false;
    try { (void)heap.top(); }
    catch (const std::out_of_range&) { topThrew = true; }
    CHECK(topThrew);
    bool popThrew = false;
    try { heap.pop(); }
    catch (const std::out_of_range&) { popThrew = true; }
    CHECK(popThrew);
    CHECK(heap.empty());
}

int main() {
    using V = std::vector<int>;
    MinHeap heap;
    CHECK(heap.empty());
    CHECK_EQ(heap.size(), size_t{0});
    checkEmptyErrors(heap);

    heap.push(7);
    CHECK_EQ(heap.size(), size_t{1});
    CHECK(!heap.empty());
    if (!heap.empty()) {
        CHECK_EQ(heap.top(), 7);
        CHECK_EQ(heap.top(), 7); // reading does not remove
        CHECK_EQ(heap.size(), size_t{1});
        heap.pop();
    }
    CHECK(heap.empty());

    for (int x : {2, 5, 3, 9, 7, 1}) heap.push(x);
    CHECK_EQ(heap.size(), size_t{6});
    CHECK_EQ(drain(heap, 7), (V{1, 2, 3, 5, 7, 9}));
    CHECK(heap.empty());
    checkEmptyErrors(heap);

    MinHeap mixed;
    for (int x : {4, -2, 4, 0, -2}) mixed.push(x);
    CHECK_EQ(drain(mixed, 6), (V{-2, -2, 0, 4, 4}));
    CHECK(mixed.empty());

    MinHeap interleaved;
    for (int x : {8, 3, 5}) interleaved.push(x);
    if (!interleaved.empty()) {
        CHECK_EQ(interleaved.top(), 3);
        interleaved.pop();
    }
    interleaved.push(1);
    interleaved.push(6);
    CHECK_EQ(drain(interleaved, 5), (V{1, 5, 6, 8}));
    CHECK(interleaved.empty());

    for (bool descending : {false, true}) {
        MinHeap ordered;
        for (int i = 0; i < 1000; ++i)
            ordered.push(descending ? 999 - i : i);
        V expected;
        for (int i = 0; i < 1000; ++i) expected.push_back(i);
        CHECK_EQ(drain(ordered, 1001), expected);
        CHECK(ordered.empty());
    }
    return check::report();
}
