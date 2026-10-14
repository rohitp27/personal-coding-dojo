// Problem: 0/1 knapsack and two problems that are secretly the same table.
//   int  knapsack01(const std::vector<int>& weights, const std::vector<int>& values, int capacity)
//        Each item may be taken at most once. Maximize total value with total weight <= capacity.
//   bool canPartition(const std::vector<int>& nums)
//        Can nums be split into two groups with equal sums? (An empty input splits into two empty groups: true.)
//   long long countSubsets(const std::vector<int>& nums, int target)
//        Number of subsets (by position, so equal values at different indices count separately; the empty subset
//        counts) whose sum equals target. nums[i] >= 0 and zeros are allowed. target < 0 -> 0.
//
// Examples:
//   knapsack01({1,3,4,5}, {1,4,5,7}, 7) = 9           knapsack01({2}, {3}, 10) = 3   (not 15: items are not reusable)
//   canPartition({1,5,11,5}) = true                   canPartition({1,2,3,5}) = false
//   countSubsets({1,1,1,1,1}, 3) = 10                 countSubsets({0,0,1}, 1) = 4
//
// Constraints: up to 1,000 items; weights/values/nums are non-negative ints; capacity/target up to 20,000;
//   sums fit in long long.
// Required complexity: O(items * capacity) time, O(capacity) extra space. Exponential subset search is out.
// Run: make run FILE=curriculum/201_data_structures/dp/02_knapsack_variants.cpp
//
// Think first:
//  1. With one 1D array, which direction must the capacity loop run so an item is used at most once, and what
//     would iterating the other way silently compute instead?
//  2. canPartition and countSubsets do not carry a value. What does dp[j] hold in each of them, and what are the
//     base case and the combine operation (max, OR, +)?
//  3. What goes wrong with zeros in countSubsets if your inner loop stops at j >= x with x > 0 assumed?

#include "check.h"

#include <algorithm>
#include <numeric>
#include <vector>

// ===== YOUR CODE BELOW (stub) =====
int knapsack01([[maybe_unused]] const std::vector<int>& weights, [[maybe_unused]] const std::vector<int>& values,
               [[maybe_unused]] int capacity) {
    // TODO
    return 0;
}

bool canPartition([[maybe_unused]] const std::vector<int>& nums) {
    // TODO
    return false;
}

long long countSubsets([[maybe_unused]] const std::vector<int>& nums, [[maybe_unused]] int target) {
    // TODO
    return 0;
}

// ===== TESTS (do not edit below) =====
using V = std::vector<int>;

static V iotaFrom(int lo, int hi) {  // lo..hi inclusive
    V v;
    for (int i = lo; i <= hi; i++) v.push_back(i);
    return v;
}

int main() {
    // knapsack01
    CHECK_EQ(knapsack01({}, {}, 10), 0);
    CHECK_EQ(knapsack01({1, 3, 4, 5}, {1, 4, 5, 7}, 0), 0);
    CHECK_EQ(knapsack01({1, 3, 4, 5}, {1, 4, 5, 7}, 7), 9);
    CHECK_EQ(knapsack01({2}, {3}, 10), 3);
    CHECK_EQ(knapsack01({5}, {100}, 4), 0);  // too heavy
    CHECK_EQ(knapsack01({10, 20, 30}, {60, 100, 120}, 50), 220);  // value-per-weight greedy gives 160
    CHECK_EQ(knapsack01({1, 1, 1}, {10, 20, 30}, 2), 50);
    CHECK_EQ(knapsack01({0, 3}, {5, 6}, 2), 5);  // a zero-weight item is free
    CHECK_EQ(knapsack01({4, 4, 4}, {1, 1, 1}, 8), 2);

    // canPartition
    CHECK(canPartition({}));
    CHECK(canPartition({1, 1}));
    CHECK(canPartition({1, 5, 11, 5}));
    CHECK(!canPartition({1, 2, 3, 5}));
    CHECK(!canPartition({2}));
    CHECK(!canPartition({1, 2, 4}));  // odd total
    CHECK(canPartition({0, 0}));
    CHECK(canPartition({3, 3, 3, 4, 5}));  // total 18: 4+5 | 3+3+3
    CHECK(!canPartition({100, 1, 1}));

    // countSubsets
    CHECK_EQ(countSubsets({}, 0), 1LL);
    CHECK_EQ(countSubsets({}, 5), 0LL);
    CHECK_EQ(countSubsets({1, 2, 3}, 0), 1LL);  // only the empty subset
    CHECK_EQ(countSubsets({1, 1, 1, 1, 1}, 3), 10LL);
    CHECK_EQ(countSubsets({1, 2, 3}, 6), 1LL);
    CHECK_EQ(countSubsets({1, 2, 3}, 7), 0LL);
    CHECK_EQ(countSubsets({0, 0, 1}, 1), 4LL);  // each zero can be in or out
    CHECK_EQ(countSubsets({0, 0, 0}, 0), 8LL);
    CHECK_EQ(countSubsets({2, 3, 5}, -1), 0LL);
    CHECK_EQ(countSubsets({1, 2, 3, 4, 5}, 5), 3LL);  // {5} {1,4} {2,3}

    // large
    {
        V w = iotaFrom(1, 1000);
        // weight == value: the best is exactly capacity, because every sum 0..500500 is reachable
        CHECK_EQ(knapsack01(w, w, 10000), 10000);
        // every item worth 1: take the lightest ones; 1+2+...+140 = 9870 <= 10000 < 9870 + 141
        CHECK_EQ(knapsack01(w, V(1000, 1), 10000), 140);
        // value = 2 * weight
        V doubled(1000);
        for (int i = 0; i < 1000; i++) doubled[i] = 2 * w[i];
        CHECK_EQ(knapsack01(w, doubled, 10000), 20000);
        // capacity bigger than everything: take it all
        V ones(1000, 1);
        CHECK_EQ(knapsack01(ones, w, 20000), 500500);
    }
    CHECK(canPartition(iotaFrom(1, 200)));  // total 20100, half 10050 is reachable
    {
        V a(199, 4);
        a.push_back(2);  // total 798, half 399 is odd but every element is even
        CHECK(!canPartition(a));
        V b(99, 2);
        b.push_back(1);
        b.push_back(1);  // total 200
        CHECK(canPartition(b));
        V c(99, 2);
        c.push_back(1);  // total 199, odd
        CHECK(!canPartition(c));
    }
    CHECK_EQ(countSubsets(V(20, 1), 10), 184756LL);                // C(20,10)
    CHECK_EQ(countSubsets(V(60, 1), 30), 118264581564861424LL);    // C(60,30)
    CHECK_EQ(countSubsets(V(60, 0), 0), 1LL << 60);                // every subset of 60 zeros
    return check::report();
}
