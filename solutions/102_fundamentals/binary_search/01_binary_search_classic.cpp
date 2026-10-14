// Problem: Classic Binary Search
// Given an array sorted in strictly increasing order (no duplicates) and a target,
// return the index of target, or -1 if it is not present.
//
// Examples:
//   a = {-1,0,3,5,9,12}, target = 9 -> 4
//   a = {-1,0,3,5,9,12}, target = 2 -> -1
//   a = {},              target = 1 -> -1
//
// Constraints: 0 <= a.size() <= 1,000,000.
// Required complexity: O(log n) per search. A linear scan is too slow for the
// "many searches" test at the bottom.
//
// Hint: keep a half-open or closed range [lo, hi] that always contains target if it is
// present, and say the loop invariant out loud before writing code. Compute the middle as
// lo + (hi - lo) / 2, and make sure every branch shrinks the range so the loop terminates.
//
// Run: make run FILE=curriculum/102_fundamentals/binary_search/01_binary_search_classic.cpp

#include "check.h"

#include <vector>

int binarySearch(const std::vector<int>& a, int target) {
    int lo = 0, hi = static_cast<int>(a.size()) - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] == target) return mid;
        if (a[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

int main() {
    std::vector<int> a{-1, 0, 3, 5, 9, 12};
    CHECK_EQ(binarySearch(a, 9), 4);
    CHECK_EQ(binarySearch(a, 2), -1);
    CHECK_EQ(binarySearch(a, -1), 0);  // first element
    CHECK_EQ(binarySearch(a, 12), 5);  // last element
    CHECK_EQ(binarySearch(a, -5), -1);  // below the range
    CHECK_EQ(binarySearch(a, 100), -1);  // above the range
    CHECK_EQ(binarySearch({}, 1), -1);
    CHECK_EQ(binarySearch({4}, 4), 0);
    CHECK_EQ(binarySearch({4}, 3), -1);
    CHECK_EQ(binarySearch({4}, 5), -1);
    CHECK_EQ(binarySearch({1, 2}, 1), 0);
    CHECK_EQ(binarySearch({1, 2}, 2), 1);
    CHECK_EQ(binarySearch({1, 2}, 3), -1);
    CHECK_EQ(binarySearch({-2147483647 - 1, 2147483647}, 2147483647), 1);

    // every element of every small size is found at its own index; gaps are not found
    {
        int bad = 0;
        for (int n = 0; n <= 40; n++) {
            std::vector<int> v(n);
            for (int i = 0; i < n; i++) v[i] = 2 * i;
            for (int i = 0; i < n; i++)
                if (binarySearch(v, 2 * i) != i) bad++;
            for (int x = -1; x <= 2 * n; x += 2)
                if (binarySearch(v, x) != -1) bad++;
        }
        CHECK_EQ(bad, 0);
    }

    // 200,000 searches in a 200,000-element array: a linear scan is ~10^10 steps.
    {
        std::vector<int> v(200000);
        for (int i = 0; i < 200000; i++) v[i] = 2 * i;
        int bad = 0;
        for (int i = 0; i < 200000; i++) {
            if (binarySearch(v, 2 * i) != i) bad++;
            if (binarySearch(v, 2 * i + 1) != -1) bad++;
        }
        CHECK_EQ(bad, 0);
    }

    return check::report();
}
