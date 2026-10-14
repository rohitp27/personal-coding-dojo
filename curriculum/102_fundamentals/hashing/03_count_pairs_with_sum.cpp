// Problem: Count Pairs With Sum
// Given an unsorted array a and a target k, return the number of index pairs (i, j)
// with i < j and a[i] + a[j] == k. Values may repeat; every distinct index pair counts.
//
// Examples:
//   a = {1,2,3,4},   k = 5 -> 2   ((1,4) and (2,3))
//   a = {1,1,1,1},   k = 2 -> 6   (every pair of the four 1s)
//   a = {1,5,1,5},   k = 6 -> 4
//   a = {-1,1,0,0},  k = 0 -> 2
//
// Constraints: 0 <= a.size() <= 300,000; values fit in int and may be negative;
//              k is a 64-bit integer. The answer can exceed 32 bits.
// Required complexity: O(n) expected time, O(n) space. Brute force over all pairs is
// too slow for the larger tests.
//
// Think first:
//   1. What is the count when a value is exactly half of k and appears m times?
//   2. Can a[i] + a[j] overflow 32-bit int? What type should the arithmetic use?
//   3. If you process left to right, what exactly does the map need to remember
//      so each pair is counted once and an element never pairs with itself?
//
// Run: make run FILE=curriculum/102_fundamentals/hashing/03_count_pairs_with_sum.cpp

#include "check.h"

#include <climits>
#include <numeric>
#include <unordered_map>
#include <vector>

long long countPairsWithSum([[maybe_unused]] const std::vector<int>& a, [[maybe_unused]] long long k) {
    // TODO
    return -1;
}

int main() {
    CHECK_EQ(countPairsWithSum({}, 0), 0LL);
    CHECK_EQ(countPairsWithSum({5}, 10), 0LL);
    CHECK_EQ(countPairsWithSum({1, 2, 3, 4}, 5), 2LL);
    CHECK_EQ(countPairsWithSum({1, 1, 1, 1}, 2), 6LL);
    CHECK_EQ(countPairsWithSum({1, 5, 1, 5}, 6), 4LL);
    CHECK_EQ(countPairsWithSum({3, 3, 3}, 6), 3LL);
    CHECK_EQ(countPairsWithSum({3, 3, 3}, 7), 0LL);
    CHECK_EQ(countPairsWithSum({-1, 1, 0, 0, 2, -2}, 0), 3LL);
    CHECK_EQ(countPairsWithSum({5}, 10), 0LL);          // a lone element must not pair with itself
    CHECK_EQ(countPairsWithSum({0, 0}, 0), 1LL);
    CHECK_EQ(countPairsWithSum({4, 6, 4, 6, 5}, 10), 4LL);

    // 32-bit overflow traps
    CHECK_EQ(countPairsWithSum({2000000000, 2000000000}, 4000000000LL), 1LL);
    CHECK_EQ(countPairsWithSum({2000000000, -2000000000}, 0), 1LL);
    CHECK_EQ(countPairsWithSum({INT_MAX, INT_MAX, INT_MIN}, -1), 2LL);  // INT_MAX + INT_MIN == -1
    CHECK_EQ(countPairsWithSum({INT_MIN, INT_MIN}, 2LL * INT_MIN), 1LL);

    // Larger inputs: all-pairs brute force is ~10^10 operations here.
    CHECK_EQ(countPairsWithSum(std::vector<int>(200000, 5), 10), 19999900000LL);  // C(200000, 2)
    {
        std::vector<int> a(200000);
        std::iota(a.begin(), a.end(), 1);  // 1..200000
        CHECK_EQ(countPairsWithSum(a, 200001), 100000LL);
        CHECK_EQ(countPairsWithSum(a, 1), 0LL);
        CHECK_EQ(countPairsWithSum(a, 399999), 1LL);  // only 199999 + 200000
    }

    return check::report();
}
