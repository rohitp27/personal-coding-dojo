// Problem: Kth Largest Element
// Return the kth-largest value in nums, counting duplicates separately.
// Example: {3, 2, 1, 5, 6, 4}, k=2 -> 5
// Example: {5, 5, 4}, k=2 -> 5
// Do not modify nums. Throw std::out_of_range if k < 1 or k > nums.size().
// Use std::priority_queue with at most k retained values; do not sort the input.
// Target: O(n log k) time for k >= 2 (O(n) for k=1), O(k) extra space.
// Before coding: write what the heap contains after each processed element.
//
// Syntax reference:
//   std::priority_queue<int, std::vector<int>, std::greater<int>> heap;
//   heap.push(value); heap.top(); heap.pop(); heap.size(); heap.empty();
//   top() reads the minimum; pop() removes it and returns nothing.
//   Never call top() or pop() on an empty priority_queue.
// Run: make run FILE=practice/cpp/heaps/02_kth_largest.cpp

#include "check.h"
#include <functional>
#include <queue>
#include <stdexcept>
#include <vector>

int kthLargest([[maybe_unused]] const std::vector<int>& nums,
               [[maybe_unused]] int k) {
    
    if (k < 1 || (size_t) k > nums.size()) {
        throw std::out_of_range("out of range");
    }
    std::priority_queue<int, std::vector<int>, std::greater<int>> heap;
    for(int elem : nums) {
        if (heap.size() < (size_t) k) { //includes empty
            heap.push(elem);
        } else if (heap.size() == (size_t) k && heap.top() < elem) { // full and can replace
            heap.pop();
            heap.push(elem);
        }
        // either full or can't replace
    }
    return heap.top();
}

// ---- checks: keep these unchanged ----
static void checkInvalid(const std::vector<int>& nums, int k) {
    bool threw = false;
    try { (void)kthLargest(nums, k); }
    catch (const std::out_of_range&) { threw = true; }
    CHECK(threw);
}

int main() {
    CHECK_EQ(kthLargest({3, 2, 1, 5, 6, 4}, 2), 5);
    CHECK_EQ(kthLargest({5, 5, 4}, 2), 5);
    CHECK_EQ(kthLargest({3, 2, 3, 1, 2, 4, 5, 5, 6}, 4), 4);
    CHECK_EQ(kthLargest({7}, 1), 7);
    CHECK_EQ(kthLargest({2, 9, 1, 4}, 1), 9);
    CHECK_EQ(kthLargest({2, 9, 1, 4}, 4), 1);
    CHECK_EQ(kthLargest({-8, -2, -5, -1}, 2), -2);
    CHECK_EQ(kthLargest({0, -1, 0, 2}, 3), 0);
    CHECK_EQ(kthLargest({4, 4, 4, 4}, 3), 4);
    checkInvalid({}, 1);
    checkInvalid({1, 2}, 0);
    checkInvalid({1, 2}, -1);
    checkInvalid({1, 2}, 3);
    const std::vector<int> input{9, 1, 7, 2};
    CHECK_EQ(kthLargest(input, 2), 7);
    CHECK_EQ(input, (std::vector<int>{9, 1, 7, 2}));
    std::vector<int> ascending;
    for (int i = 0; i < 10000; ++i) ascending.push_back(i);
    CHECK_EQ(kthLargest(ascending, 100), 9900);
    return check::report();
}
