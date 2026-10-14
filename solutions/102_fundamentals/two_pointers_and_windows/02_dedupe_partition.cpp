// Problem: In-Place Dedupe and Partition (read/write pointers)
// Two in-place array rewrites that both use a "write" pointer trailing a "read" pointer.
//
// (a) removeDuplicatesKeepTwo(a): a is sorted non-decreasing. Rewrite it in place so each
//     distinct value appears at most twice, keeping the original order. Return the new
//     length k; only a[0..k) is inspected afterwards (the rest is unspecified).
//     {1,1,1,2,2,3} -> returns 5, a[0..5) = {1,1,2,2,3}
//     {5,5,5,5}     -> returns 2, a[0..2) = {5,5}
//
// (b) moveZeroesToEnd(a): move every 0 to the end while keeping the relative order of the
//     non-zero elements (stable).
//     {0,1,0,3,12} -> {1,3,12,0,0}
//
// Constraints: 0 <= a.size() <= 1,000,000. No extra arrays: O(1) extra space.
//              Repeated vector::erase() is quadratic and too slow here.
// Required complexity: O(n) time, O(1) extra space for each function.
//
// Hint for (a): compare the value you are reading with the one two slots BEHIND the write
// position, not with its predecessor. Why does that rule also handle "at most one"
// and "at most k" if you change the 2?
//
// Run: make run FILE=curriculum/102_fundamentals/two_pointers_and_windows/02_dedupe_partition.cpp

#include "check.h"

#include <vector>

int removeDuplicatesKeepTwo(std::vector<int>& a) {
    size_t w = 0;
    for (int x : a) {
        if (w < 2 || x != a[w - 2]) a[w++] = x;
    }
    return static_cast<int>(w);
}

void moveZeroesToEnd(std::vector<int>& a) {
    size_t w = 0;
    for (size_t r = 0; r < a.size(); r++)
        if (a[r] != 0) a[w++] = a[r];
    while (w < a.size()) a[w++] = 0;
}

// Runs (a) and returns the first k elements, or an empty marker vector {-999} when k is wrong.
static std::vector<int> dedupe(std::vector<int> a, int expectedLen) {
    int k = removeDuplicatesKeepTwo(a);
    if (k != expectedLen) return {-999};
    return std::vector<int>(a.begin(), a.begin() + k);
}

static std::vector<int> zeroes(std::vector<int> a) {
    moveZeroesToEnd(a);
    return a;
}

int main() {
    using V = std::vector<int>;
    CHECK_EQ(dedupe({}, 0), V{});
    CHECK_EQ(dedupe({1}, 1), V{1});
    CHECK_EQ(dedupe({1, 1}, 2), (V{1, 1}));
    CHECK_EQ(dedupe({1, 1, 1}, 2), (V{1, 1}));
    CHECK_EQ(dedupe({1, 1, 1, 2, 2, 3}, 5), (V{1, 1, 2, 2, 3}));
    CHECK_EQ(dedupe({0, 0, 1, 1, 1, 1, 2, 3, 3}, 7), (V{0, 0, 1, 1, 2, 3, 3}));
    CHECK_EQ(dedupe({5, 5, 5, 5}, 2), (V{5, 5}));
    CHECK_EQ(dedupe({1, 2, 3, 4}, 4), (V{1, 2, 3, 4}));
    CHECK_EQ(dedupe({-2, -2, -2, -1, -1, 0}, 5), (V{-2, -2, -1, -1, 0}));

    CHECK_EQ(zeroes({}), V{});
    CHECK_EQ(zeroes({0}), V{0});
    CHECK_EQ(zeroes({0, 0, 0}), (V{0, 0, 0}));
    CHECK_EQ(zeroes({1, 2, 3}), (V{1, 2, 3}));
    CHECK_EQ(zeroes({0, 1, 0, 3, 12}), (V{1, 3, 12, 0, 0}));
    CHECK_EQ(zeroes({3, 0, 0, 2}), (V{3, 2, 0, 0}));
    CHECK_EQ(zeroes({-1, 0, -2}), (V{-1, -2, 0}));

    // Large: quadratic erase()-based solutions will not finish.
    {
        V a(999999);
        for (int i = 0; i < 999999; i++) a[i] = i / 3;  // every value exactly three times
        int k = removeDuplicatesKeepTwo(a);
        CHECK_EQ(k, 666666);
        int bad = 0;
        for (int i = 0; i < k && i < static_cast<int>(a.size()); i++)
            if (a[i] != i / 2) bad++;
        CHECK_EQ(bad, 0);
    }
    {
        V a(1000000);
        for (int i = 0; i < 1000000; i++) a[i] = (i % 2 == 0) ? 0 : i;
        moveZeroesToEnd(a);
        int bad = 0;
        for (int i = 0; i < 500000; i++)
            if (a[i] != 2 * i + 1) bad++;
        for (int i = 500000; i < 1000000; i++)
            if (a[i] != 0) bad++;
        CHECK_EQ(bad, 0);
    }

    return check::report();
}
