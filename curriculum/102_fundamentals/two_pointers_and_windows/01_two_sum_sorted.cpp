// Problem: Two Sum in a Sorted Array
// Given an array sorted in non-decreasing order and a target, return 0-based indices
// {i, j} with i < j and a[i] + a[j] == target. If several pairs exist, any one is
// accepted. If none exists, return {-1, -1}.
//
// Examples:
//   a = {2,7,11,15},        target = 9  -> {0, 1}
//   a = {1,2,3,4,4,9,56},   target = 8  -> {3, 4}   (4 + 4, two different indices)
//   a = {1,2,3},            target = 7  -> {-1, -1}
//
// Constraints: 0 <= a.size() <= 1,000,000; values fit in int and the sum fits in
//              long long. Use no extra memory beyond O(1).
// Required complexity: O(n) time, O(1) space.
//
// Hint: keep one index at each end. If the current sum is too small, only moving the
// left index right can help; if it is too big, only moving the right index left can.
// Convince yourself why you never skip a valid pair.
//
// Run: make run FILE=curriculum/102_fundamentals/two_pointers_and_windows/01_two_sum_sorted.cpp

#include "check.h"

#include <utility>
#include <vector>

using P = std::pair<int, int>;

P twoSumSorted([[maybe_unused]] const std::vector<int>& a, [[maybe_unused]] long long target) {
    // TODO
    return {-2, -2};
}

// A returned pair is acceptable when it is in range, ordered, and sums to target.
static bool valid(const std::vector<int>& a, long long target, P p) {
    if (p.first < 0 || p.second < 0) return false;
    if (p.first >= p.second || p.second >= static_cast<int>(a.size())) return false;
    return static_cast<long long>(a[p.first]) + a[p.second] == target;
}

int main() {
    const P none{-1, -1};
    CHECK(valid({2, 7, 11, 15}, 9, twoSumSorted({2, 7, 11, 15}, 9)));
    CHECK(valid({1, 2, 3, 4, 4, 9, 56}, 8, twoSumSorted({1, 2, 3, 4, 4, 9, 56}, 8)));
    CHECK(valid({-4, -1, 0, 3, 10}, -1, twoSumSorted({-4, -1, 0, 3, 10}, -1)));
    CHECK(valid({1, 2, 3, 4, 5}, 9, twoSumSorted({1, 2, 3, 4, 5}, 9)));  // the two largest
    CHECK(valid({1, 2, 3, 4, 5}, 3, twoSumSorted({1, 2, 3, 4, 5}, 3)));  // the two smallest
    CHECK(valid({0, 0}, 0, twoSumSorted({0, 0}, 0)));
    CHECK(valid({2000000000, 2000000000}, 4000000000LL, twoSumSorted({2000000000, 2000000000}, 4000000000LL)));
    CHECK_EQ(twoSumSorted({1, 2, 3}, 7), none);
    CHECK_EQ(twoSumSorted({}, 0), none);
    CHECK_EQ(twoSumSorted({5}, 10), none);  // one element cannot pair with itself
    CHECK_EQ(twoSumSorted({1, 3, 5}, 2), none);
    CHECK_EQ(twoSumSorted({1, 1, 1}, 3), none);

    // 1,000,000 elements: all-pairs search is ~10^12 operations.
    {
        std::vector<int> a(1000000);
        for (int i = 0; i < 1000000; i++) a[i] = 2 * i;  // even numbers
        long long t = 2LL * 999998 + 2LL * 999999;       // largest two
        CHECK(valid(a, t, twoSumSorted(a, t)));
        CHECK(valid(a, 1000000, twoSumSorted(a, 1000000)));
        CHECK_EQ(twoSumSorted(a, 1000001), none);  // odd target, all values even
        CHECK_EQ(twoSumSorted(a, 5000000), none);  // above the largest possible sum
    }

    return check::report();
}
