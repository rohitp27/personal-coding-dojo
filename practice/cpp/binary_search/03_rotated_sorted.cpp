// Problem: Search in a Rotated Sorted Array
// A sorted array of DISTINCT integers was rotated at an unknown pivot: some prefix was
// moved to the back, e.g. {0,1,2,4,5,6,7} -> {4,5,6,7,0,1,2}. (A rotation by 0 is allowed,
// so a fully sorted array is also valid input.) Return the index of target, or -1.
//
// Examples:
//   a = {4,5,6,7,0,1,2}, target = 0 -> 4
//   a = {4,5,6,7,0,1,2}, target = 3 -> -1
//   a = {3,1},           target = 1 -> 1
//   a = {1},             target = 0 -> -1
//
// Constraints: 0 <= a.size() <= 200,000; all values distinct.
// Required complexity: O(log n) per search. The test below runs 400,000 searches on a
// 200,000-element array, which a linear scan cannot finish in time.
//
// Think first:
//   1. At any mid, one of the two halves [lo, mid] or [mid, hi] is guaranteed to be
//      sorted. How do you detect which one, and what does that tell you about where
//      target can be?
//   2. Which comparisons must be <= rather than < (hint: think about a range of size 1
//      or 2, and about target == a[lo])?
//   3. What input breaks the "one half is sorted" argument, and why is it excluded here?
//
// Run: make run FILE=practice/cpp/binary_search/03_rotated_sorted.cpp

#include "check.h"

#include <vector>
#include <assert.h>

int getIndex(const std::vector<int>& a, const int index) {
    assert(!a.empty());
    return index % a.size();
}

int getValue(const std::vector<int>& a, const int index) {
    int idx = getIndex(a, index);
    return a[idx];
}

void printArray(const std::vector<int>& a) {
    for (int elem : a) {
        printf("%d -> ", elem);
    }
    printf("\n");
}

int findPivot(const std::vector<int>& a) {
    int left = 0;
    int right = a.size()-1;
    if (a[left] <= a[right]) {
        // already sorted, send 0
        return left;
    }
    while(left <= right) {
        // printf("left %d, getValLeft %d, right %d, getValRight %d\n", left, getValue(a, left), right, getValue(a, right));
        if (getValue(a, left) <= getValue(a, right)) {
            assert(getIndex(a, left) == left);
            return left;
        }
        if (left == right) {
            // this is the pivot
            return left;
        }
        // int size = right-left+1;
        int midpt = left + (right-left)/2; // lower midpt
        int midVal = a[midpt];
        // printf("Midpt %d, midval %d, a[right] %d\n", midpt, midVal, a[right]);
        // printArray(a);
        assert(midVal == getValue(a, midpt));
        if (midVal > getValue(a, right)) {
            left = midpt + 1;
        } else {
            right = midpt;
        }
    }
    return -1;
}

int searchInVector(const std::vector<int>& a, int target, int left, int right) {
    // standard binary search
    // printArray(a);
    // printf("target %d, left %d, left idx %d, right %d, right idx %d\n", target, left, getIndex(a, left), right , getIndex(a, right));
    assert(left <= right);
    // assert();
    assert(getValue(a, left) <= getValue(a, right));

    while (left <= right) {
        // printf("left %d, leftVal %d, right %d, rightVal %d\n", left, getValue(a, left), right, getValue(a, right));
        if (left == right) {
            int val = getValue(a, left);
            if (val == target) {
                return getIndex(a, left);
            } else {
                return -1;
            }
        }
        int size = right-left+1;
        int midIndex = left + size/2;
        int midVal = getValue(a, midIndex);
        // printf("Midpt %d, midVal %d\n", midIndex, midVal);
        if (midVal == target) {
            return getIndex(a, midIndex);
        } else if (midVal < target) {
            // search right
            left = midIndex+1;
        } else {
            // search left
            right = midIndex-1;
        }
    }
    return -1;
}

int searchRotated([[maybe_unused]] const std::vector<int>& a, [[maybe_unused]] int target) {
    if (a.empty()) {
        return -1;
    }
    if (a.size() == 1) {
        if (a[0] == target) {
            return 0;
        } else {
            return -1;
        }
    }
    int pivot = findPivot(a);
    if (pivot == -1) {
        printf("Bad pivot\n\n");
        printArray(a);
    }
    assert(pivot != -1);
    int startIndex = pivot;
    assert(pivot == getIndex(a, startIndex));
    int endIndex = pivot+(a.size()-1);
    int returned = searchInVector(a, target, startIndex, endIndex);
    // printf("Ans : %d\n", returned);
    return returned;
}

// Build the sorted sequence 0, 2, 4, ... (n values) rotated left by r.
static std::vector<int> rotated(int n, int r) {
    std::vector<int> v(n);
    for (int i = 0; i < n; i++) v[i] = 2 * ((i + r) % n);
    return v;
}

int main() {
    CHECK_EQ(searchRotated({4, 5, 6, 7, 0, 1, 2}, 0), 4);
    CHECK_EQ(searchRotated({4, 5, 6, 7, 0, 1, 2}, 3), -1);
    CHECK_EQ(searchRotated({4, 5, 6, 7, 0, 1, 2}, 4), 0);
    CHECK_EQ(searchRotated({4, 5, 6, 7, 0, 1, 2}, 2), 6);
    CHECK_EQ(searchRotated({4, 5, 6, 7, 0, 1, 2}, 7), 3);
    CHECK_EQ(searchRotated({1}, 0), -1);
    CHECK_EQ(searchRotated({1}, 1), 0);
    CHECK_EQ(searchRotated({}, 5), -1);
    CHECK_EQ(searchRotated({3, 1}, 1), 1);
    CHECK_EQ(searchRotated({3, 1}, 3), 0);
    CHECK_EQ(searchRotated({1, 3}, 3), 1);  // rotation by 0
    CHECK_EQ(searchRotated({1, 3}, 1), 0);
    CHECK_EQ(searchRotated({5, 1, 3}, 5), 0);
    CHECK_EQ(searchRotated({5, 1, 3}, 3), 2);
    CHECK_EQ(searchRotated({5, 1, 3}, 2), -1);
    CHECK_EQ(searchRotated({-3, -2, -1, 0, 1}, -1), 2);
    CHECK_EQ(searchRotated({0, 1, -3, -2, -1}, -2), 3);

    // exhaustive over every size up to 30, every rotation, every present and absent value
    {
        int bad = 0;
        for (int n = 0; n <= 30; n++) {
            for (int r = 0; r < (n == 0 ? 1 : n); r++) {
                std::vector<int> v = rotated(n, r);
                for (int i = 0; i < n; i++)
                    if (searchRotated(v, v[i]) != i) bad++;
                for (int x = -1; x <= 2 * n + 1; x += 2)
                    if (searchRotated(v, x) != -1) bad++;
            }
        }
        CHECK_EQ(bad, 0);
    }

    // 200,000 elements, three rotations, 400,000 searches each: O(n) per search is ~10^10.
    for (int r : {0, 77777, 199999}) {
        std::vector<int> v = rotated(200000, r);
        int bad = 0;
        for (int i = 0; i < 200000; i++) {
            if (searchRotated(v, v[i]) != i) bad++;
            if (searchRotated(v, v[i] + 1) != -1) bad++;
        }
        CHECK_EQ(bad, 0);
    }

    return check::report();
}
