// Problem: Longest Consecutive Run
// Given an unsorted array of integers, return the length of the longest set of values
// that form a run of consecutive integers (x, x+1, x+2, ...). The values need not be
// adjacent in the array, and duplicates count once.
//
// Examples:
//   {100, 4, 200, 1, 3, 2}          -> 4   (1,2,3,4)
//   {0,3,7,2,5,8,4,6,0,1}           -> 9   (0..8)
//   {1, 1, 1}                       -> 1
//   {}                              -> 0
//
// Constraints: 0 <= a.size() <= 400,000; any int values, including INT_MIN / INT_MAX.
// Required complexity: O(n) expected time. Sorting is O(n log n) and does not meet the
// target; rescanning the array for x+1 repeatedly is quadratic and too slow here.
//
// Think first:
//   1. In a hash set, how can you tell, in O(1), that a value is the START of a run?
//   2. If every element extended its own run, why would the total work blow up, and
//      what single rule makes each element be walked at most once overall?
//   3. What happens to x - 1 or x + 1 when x is INT_MIN or INT_MAX?
//
// Run: make run FILE=curriculum/102_fundamentals/hashing/04_longest_consecutive.cpp

#include "check.h"

#include <algorithm>
#include <climits>
#include <numeric>
#include <random>
#include <unordered_set>
#include <vector>

//@REF
int longestConsecutive(const std::vector<int>& a) {
    std::unordered_set<long long> s(a.begin(), a.end());
    int best = 0;
    for (long long x : s) {
        if (s.count(x - 1)) continue;
        int len = 1;
        while (s.count(x + len)) len++;
        best = std::max(best, len);
    }
    return best;
}
int longestConsecutive([[maybe_unused]] const std::vector<int>& a) {
    // TODO
    return -1;
}

int main() {
    CHECK_EQ(longestConsecutive({}), 0);
    CHECK_EQ(longestConsecutive({7}), 1);
    CHECK_EQ(longestConsecutive({100, 4, 200, 1, 3, 2}), 4);
    CHECK_EQ(longestConsecutive({0, 3, 7, 2, 5, 8, 4, 6, 0, 1}), 9);
    CHECK_EQ(longestConsecutive({1, 1, 1}), 1);
    CHECK_EQ(longestConsecutive({-1, -2, -3, 0, 10}), 4);
    CHECK_EQ(longestConsecutive({5, 7, 9}), 1);
    CHECK_EQ(longestConsecutive({9, 1, 4, 7, 3, -1, 0, 5, 8, -1, 6}), 7);  // 3..9
    CHECK_EQ(longestConsecutive({INT_MAX - 1, INT_MAX, INT_MIN, INT_MIN + 1}), 2);
    CHECK_EQ(longestConsecutive({INT_MAX, INT_MIN}), 1);

    std::mt19937 rng(12345);
    {
        std::vector<int> a(400000);
        std::iota(a.begin(), a.end(), 0);
        std::shuffle(a.begin(), a.end(), rng);
        CHECK_EQ(longestConsecutive(a), 400000);
    }
    {
        std::vector<int> a;  // only even numbers: no two are consecutive
        for (int i = 0; i < 400000; i++) a.push_back(2 * i);
        std::shuffle(a.begin(), a.end(), rng);
        CHECK_EQ(longestConsecutive(a), 1);
    }
    {
        // runs [0, 100000) and [200000, 400000) with a stray 150000 in between
        std::vector<int> a;
        for (int i = 0; i < 100000; i++) a.push_back(i);
        for (int i = 200000; i < 400000; i++) a.push_back(i);
        a.push_back(150000);
        std::shuffle(a.begin(), a.end(), rng);
        CHECK_EQ(longestConsecutive(a), 200000);
    }

    return check::report();
}
