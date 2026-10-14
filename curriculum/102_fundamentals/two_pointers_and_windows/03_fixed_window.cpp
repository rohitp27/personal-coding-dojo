// Problem: Most Distinct Values in a Fixed Window
// Given an array a and a window length k, look at every contiguous window of exactly
// k elements and return the largest number of DISTINCT values found in any one window.
// If k <= 0 or k > a.size(), return 0.
//
// Examples:
//   a = {1,2,1,3,4,2,3}, k = 4 -> 4   (windows have 3, 4, 4, 3 distinct values)
//   a = {5,5,5,5},       k = 2 -> 1
//   a = {1,2,3},         k = 3 -> 3
//   a = {1,2,3},         k = 5 -> 0
//
// Constraints: 0 <= a.size() <= 1,000,000; values are arbitrary ints; 1 <= k typical.
// Required complexity: O(n) expected time, O(k) space. Recounting each window from
// scratch is O(n * k) and too slow for the larger tests.
//
// Think first:
//   1. When the window slides by one, which two elements change, and what exactly must
//      you update for each so the distinct count stays correct?
//   2. When an element leaves the window and its count drops to zero, what must happen
//      to your bookkeeping (not just the counter)?
//   3. What are the edge cases for k relative to n, and for an empty array?
//
// Run: make run FILE=curriculum/102_fundamentals/two_pointers_and_windows/03_fixed_window.cpp

#include "check.h"

#include <algorithm>
#include <climits>
#include <unordered_map>
#include <vector>

int maxDistinctInWindow([[maybe_unused]] const std::vector<int>& a, [[maybe_unused]] int k) {
    // TODO
    return -1;
}

int main() {
    CHECK_EQ(maxDistinctInWindow({1, 2, 1, 3, 4, 2, 3}, 4), 4);
    CHECK_EQ(maxDistinctInWindow({1, 2, 1, 3, 4, 2, 3}, 1), 1);
    CHECK_EQ(maxDistinctInWindow({1, 2, 1, 3, 4, 2, 3}, 7), 4);
    CHECK_EQ(maxDistinctInWindow({1, 1, 2}, 3), 2);
    CHECK_EQ(maxDistinctInWindow({5, 5, 5, 5}, 2), 1);
    CHECK_EQ(maxDistinctInWindow({1, 2, 3}, 3), 3);
    CHECK_EQ(maxDistinctInWindow({1, 2, 1, 2, 1, 2, 3}, 3), 3);
    CHECK_EQ(maxDistinctInWindow({1, 2, 1, 2, 1, 2}, 3), 2);
    CHECK_EQ(maxDistinctInWindow({INT_MIN, INT_MAX, INT_MIN, -1}, 2), 2);
    CHECK_EQ(maxDistinctInWindow({7, 7, 8, 7, 7}, 2), 2);
    // edge cases for k and n
    CHECK_EQ(maxDistinctInWindow({1, 2, 3}, 5), 0);
    CHECK_EQ(maxDistinctInWindow({1, 2, 3}, 0), 0);
    CHECK_EQ(maxDistinctInWindow({1, 2, 3}, -2), 0);
    CHECK_EQ(maxDistinctInWindow({}, 1), 0);
    CHECK_EQ(maxDistinctInWindow({}, 0), 0);
    CHECK_EQ(maxDistinctInWindow({9}, 1), 1);

    // 1,000,000 elements with a 500,000-wide window: recounting each window is ~5*10^11.
    {
        std::vector<int> a(1000000);
        for (int i = 0; i < 1000000; i++) a[i] = i % 1000;
        CHECK_EQ(maxDistinctInWindow(a, 500000), 1000);
        CHECK_EQ(maxDistinctInWindow(a, 999), 999);
    }
    {
        std::vector<int> a(1000000);
        for (int i = 0; i < 1000000; i++) a[i] = i;
        CHECK_EQ(maxDistinctInWindow(a, 250000), 250000);
    }
    CHECK_EQ(maxDistinctInWindow(std::vector<int>(1000000, 4), 500000), 1);

    return check::report();
}
