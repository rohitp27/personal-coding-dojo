// Problem: First and Last Occurrence
// Given an array sorted in non-decreasing order (duplicates allowed) and a target, return
// {first, last}: the index of the first and the index of the last occurrence of target.
// If target is absent, return {-1, -1}.
//
// Examples:
//   a = {5,7,7,8,8,10}, target = 8 -> {3, 4}
//   a = {5,7,7,8,8,10}, target = 6 -> {-1, -1}
//   a = {2,2,2,2},      target = 2 -> {0, 3}
//   a = {},             target = 0 -> {-1, -1}
//
// Constraints: 0 <= a.size() <= 1,000,000. Many duplicates are allowed.
// Required complexity: O(log n) per query (two binary searches, not a binary search
// followed by a linear walk). The "all equal" test punishes walking outward.
//
// Hint: do not stop when you find the target. Write a single helper that returns the
// first index whose value is >= x (a "lower bound"); then the first occurrence is
// lowerBound(target) and the last is lowerBound(target + 1) - 1 (watch out for overflow at
// INT_MAX, or write a second helper for "first index > x").
//
// Run: make run FILE=practice/cpp/binary_search/02_first_last_occurrence.cpp

#include "check.h"

#include <climits>
#include <utility>
#include <vector>
#include <assert.h>

using P = std::pair<int, int>;

int binarySearch(const std::vector<int>& a, const int target, int start, int end){
    int left = start;
    int right = end;
    while(left <= right){
        assert(left >= 0 && (size_t) left <= a.size());
        assert(right >= 0 && (size_t) right <= a.size());
        if (left == right) {
            if (a[left] == target) {
                return left;
            } else {
                return -1;
            }
        }
        int size = right-left+1;
        int midpt = left + (size/2);
        if (a[midpt] == target) {
            return midpt;
        } else if (a[midpt] < target) {
            left = midpt+1;
        } else {
            right = midpt-1;
        }
    }
    return -1;
}

int max(int a, int b) {
    return (a<b)?b:a;
}

int min(int a, int b) {
    return (b<a)?b:a;
}

bool withinRange(const std::vector<int>& a, const int index){
    return index >= 0 && (size_t) index < a.size();
}

int findRightRange(const std::vector<int>& a, const int target, int start, int end) {
    int rightMost = start-1;
    assert(withinRange(a, rightMost));
    assert(a[rightMost] == target);
    int left = start;
    int right = end;
    while (left <= right) {
        if (left == right) {
            assert(withinRange(a, left));
            if (a[right] == target) {
                rightMost = max(rightMost, right);
            } else {
                break;
            }
        }
        int size = right-left+1;
        int midpt = left+size/2;
        assert(withinRange(a, midpt));
        int midval = a[midpt];
        if (midval == target) {
            // found it.
            rightMost = max(rightMost, midpt);
            // search the right of it
            left = midpt+1;
        } else if (midval < target) {
            left = midpt+1;
        } else {
            right = midpt-1;
        }
    }
    return rightMost;
}

int findLeftRange(const std::vector<int>& a, const int target, int start, int end) {
    int leftMost = end+1;
    assert(withinRange(a, leftMost));
    assert(a[leftMost] == target);
    int left = start;
    int right = end;

    while (left <= right) {
        if (left == right) {
            assert(withinRange(a, left));
            if (a[left] == target) {
                leftMost = min(leftMost, left);
            } 
            break;
        }
        int size = right-left+1;
        int midpt = left + size/2;
        assert(withinRange(a, midpt));
        int midval = a[midpt];
        if (midval == target) {
            leftMost = min(leftMost, midpt);
            right = midpt-1;
        } else if (midval < target) {
            left = midpt+1;
        } else {
            right = midpt-1;
        }
    }
    return leftMost;
}

P equalRange([[maybe_unused]] const std::vector<int>& a, [[maybe_unused]] int target) {
    int currentIndex = binarySearch(a, target, 0, a.size()-1);
    if (currentIndex == -1) {
        return {-1, -1};
    } 
    // means it exists, and can exist both left and right
    int leftIndex = findLeftRange(a, target, 0, currentIndex-1);
    int rightIndex = findRightRange(a, target, currentIndex+1, a.size()-1);
    return {leftIndex, rightIndex};
}

int main() {
    const P none{-1, -1};
    CHECK_EQ(equalRange({5, 7, 7, 8, 8, 10}, 8), (P{3, 4}));
    CHECK_EQ(equalRange({5, 7, 7, 8, 8, 10}, 7), (P{1, 2}));
    CHECK_EQ(equalRange({5, 7, 7, 8, 8, 10}, 5), (P{0, 0}));
    CHECK_EQ(equalRange({5, 7, 7, 8, 8, 10}, 10), (P{5, 5}));
    CHECK_EQ(equalRange({5, 7, 7, 8, 8, 10}, 6), none);
    CHECK_EQ(equalRange({5, 7, 7, 8, 8, 10}, 4), none);
    CHECK_EQ(equalRange({5, 7, 7, 8, 8, 10}, 11), none);
    CHECK_EQ(equalRange({}, 0), none);
    CHECK_EQ(equalRange({1}, 1), (P{0, 0}));
    CHECK_EQ(equalRange({1}, 2), none);
    CHECK_EQ(equalRange({2, 2, 2, 2}, 2), (P{0, 3}));
    CHECK_EQ(equalRange({2, 2, 2, 2}, 1), none);
    CHECK_EQ(equalRange({2, 2, 2, 2}, 3), none);
    CHECK_EQ(equalRange({1, 2, 2, 3}, 2), (P{1, 2}));
    CHECK_EQ(equalRange({-3, -3, -1}, -3), (P{0, 1}));
    CHECK_EQ(equalRange({0, INT_MAX, INT_MAX}, INT_MAX), (P{1, 2}));  // target + 1 would overflow
    CHECK_EQ(equalRange({INT_MIN, INT_MIN, 0}, INT_MIN), (P{0, 1}));

    // 1,000,000 elements, each of the 1000 values repeated 1000 times.
    {
        std::vector<int> a(1000000);
        for (int i = 0; i < 1000000; i++) a[i] = i / 1000;
        int bad = 0;
        for (int v = 0; v < 1000; v++)
            if (equalRange(a, v) != P{v * 1000, v * 1000 + 999}) bad++;
        if (equalRange(a, -1) != none) bad++;
        if (equalRange(a, 1000) != none) bad++;
        CHECK_EQ(bad, 0);
    }
    // All equal, many queries: walking outward from a hit is ~10^11 steps.
    {
        std::vector<int> a(1000000, 7);
        int bad = 0;
        for (int q = 0; q < 100000; q++)
            if (equalRange(a, 7) != P{0, 999999}) bad++;
        CHECK_EQ(bad, 0);
    }

    return check::report();
}
