/*
 * Problem: Arrays (std::vector) - in-place edits and scans
 *   Work with std::vector<int> by index. Edit in place where the signature says
 *   non-const reference; do not allocate a second vector.
 *
 *     reverseInPlace(v)       Reverse v using two indices that walk toward each other.
 *     rotateRight(v, k)       Rotate v right by k places in place. k may be larger than
 *                             v.size() or negative (negative = rotate left).
 *     minMax(v)               Return {min, max}. Precondition: v is non-empty.
 *     moveZeroesToEnd(v)      Move all 0s to the end, keeping the order of the non-zero
 *                             elements. In place, one pass.
 *     secondLargest(v, out)   Find the second largest DISTINCT value. Return true and
 *                             set `out` if it exists; otherwise return false and leave
 *                             `out` untouched.
 *
 * Examples:
 *   reverseInPlace({1,2,3,4})     -> {4,3,2,1}
 *   rotateRight({1,2,3,4,5}, 2)   -> {4,5,1,2,3}
 *   minMax({3,-1,7})              -> {-1, 7}
 *   moveZeroesToEnd({0,1,0,3,12}) -> {1,3,12,0,0}
 *   secondLargest({5,5,4}, out)   -> true, out == 4
 *
 * Constraints / edge cases:
 *   - Empty vector and single element for the in-place functions (must not crash).
 *   - rotateRight: k % n with negative k (C++ % keeps the sign of the dividend), k == n.
 *   - Duplicates for secondLargest: {7,7,7} has no second distinct value.
 *   - Values at INT_MIN / INT_MAX (do not seed a running max with 0).
 *
 * Target complexity: O(n) time, O(1) extra space for each function.
 *   (rotateRight: three reversals do it in O(1) space.)
 *
 * Run: make run FILE=curriculum/101_basics/05_arrays.cpp
 *
 * (Stub note: delete the `(void)x;` lines once you use the parameter.)
 */
#include <algorithm>
#include <climits>
#include <iostream>
#include <utility>
#include <vector>

#include "check.h"

using Vec = std::vector<int>;

// ===================== YOUR CODE =====================

void reverseInPlace(Vec& v) {
    // TODO. Hint: i = 0, j = n - 1; swap and move inward while i < j. (Careful: n - 1 on an empty vector.)
    (void)v;
}

void rotateRight(Vec& v, int k) {
    // TODO. Hint: normalize k into [0, n), then reverse all, reverse first k, reverse the rest.
    (void)v;
    (void)k;
}

std::pair<int, int> minMax(const Vec& v) {
    // TODO. Hint: start both from v[0], not from 0.
    (void)v;
    return {0, 0};
}

void moveZeroesToEnd(Vec& v) {
    // TODO. Hint: a "write" index that only advances on non-zero values; fill the rest with 0.
    (void)v;
}

bool secondLargest(const Vec& v, int& out) {
    // TODO. Hint: track best and second; only update second for values strictly between.
    (void)v;
    (void)out;
    return false;
}

// ===================== TESTS =====================

static Vec reversed(Vec v) {
    reverseInPlace(v);
    return v;
}

static Vec rotated(Vec v, int k) {
    rotateRight(v, k);
    return v;
}

static Vec zeroesMoved(Vec v) {
    moveZeroesToEnd(v);
    return v;
}

int main() {
    // reverseInPlace
    CHECK_EQ(reversed({}), Vec{});
    CHECK_EQ(reversed({1}), Vec{1});
    CHECK_EQ(reversed({1, 2}), Vec({2, 1}));
    CHECK_EQ(reversed({1, 2, 3}), Vec({3, 2, 1}));
    CHECK_EQ(reversed({1, 2, 3, 4}), Vec({4, 3, 2, 1}));
    CHECK_EQ(reversed({INT_MIN, 0, INT_MAX}), Vec({INT_MAX, 0, INT_MIN}));

    // rotateRight
    CHECK_EQ(rotated({}, 3), Vec{});
    CHECK_EQ(rotated({9}, 5), Vec{9});
    CHECK_EQ(rotated({1, 2, 3, 4, 5}, 2), Vec({4, 5, 1, 2, 3}));
    CHECK_EQ(rotated({1, 2, 3, 4, 5}, 0), Vec({1, 2, 3, 4, 5}));
    CHECK_EQ(rotated({1, 2, 3, 4, 5}, 5), Vec({1, 2, 3, 4, 5}));
    CHECK_EQ(rotated({1, 2, 3, 4, 5}, 7), Vec({4, 5, 1, 2, 3}));
    CHECK_EQ(rotated({1, 2, 3, 4, 5}, -1), Vec({2, 3, 4, 5, 1}));
    CHECK_EQ(rotated({1, 2, 3, 4, 5}, -7), Vec({3, 4, 5, 1, 2}));
    CHECK_EQ(rotated({1, 2}, 1), Vec({2, 1}));
    CHECK_EQ(rotated({1, 2, 3}, 1000000000), Vec({3, 1, 2}));

    // minMax
    CHECK_EQ(minMax({3, -1, 7}), std::make_pair(-1, 7));
    CHECK_EQ(minMax({5}), std::make_pair(5, 5));
    CHECK_EQ(minMax({-5, -9, -2}), std::make_pair(-9, -2));
    CHECK_EQ(minMax({4, 4, 4}), std::make_pair(4, 4));
    CHECK_EQ(minMax({INT_MAX, INT_MIN}), std::make_pair(INT_MIN, INT_MAX));
    CHECK_EQ(minMax({10, 20, 30, 40}), std::make_pair(10, 40));
    CHECK_EQ(minMax({40, 30, 20, 10}), std::make_pair(10, 40));

    // moveZeroesToEnd
    CHECK_EQ(zeroesMoved({}), Vec{});
    CHECK_EQ(zeroesMoved({0}), Vec{0});
    CHECK_EQ(zeroesMoved({5}), Vec{5});
    CHECK_EQ(zeroesMoved({0, 1, 0, 3, 12}), Vec({1, 3, 12, 0, 0}));
    CHECK_EQ(zeroesMoved({0, 0, 0}), Vec({0, 0, 0}));
    CHECK_EQ(zeroesMoved({1, 2, 3}), Vec({1, 2, 3}));
    CHECK_EQ(zeroesMoved({0, 0, 7}), Vec({7, 0, 0}));
    CHECK_EQ(zeroesMoved({-1, 0, -2}), Vec({-1, -2, 0}));

    // secondLargest
    int out = -12345;
    CHECK_EQ(secondLargest({5, 5, 4}, out), true);
    CHECK_EQ(out, 4);
    CHECK_EQ(secondLargest({1, 2, 3, 4}, out), true);
    CHECK_EQ(out, 3);
    CHECK_EQ(secondLargest({4, 3, 2, 1}, out), true);
    CHECK_EQ(out, 3);
    CHECK_EQ(secondLargest({-5, -1, -3}, out), true);
    CHECK_EQ(out, -3);
    CHECK_EQ(secondLargest({INT_MIN, INT_MAX}, out), true);
    CHECK_EQ(out, INT_MIN);
    CHECK_EQ(secondLargest({9, 9, 8, 8, 7}, out), true);
    CHECK_EQ(out, 8);

    out = -12345;
    CHECK_EQ(secondLargest({}, out), false);
    CHECK_EQ(secondLargest({7}, out), false);
    CHECK_EQ(secondLargest({7, 7, 7}, out), false);
    CHECK_EQ(out, -12345);  // untouched on failure

    return check::report();
}
