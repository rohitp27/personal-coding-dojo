/*
 * Problem: References vs pointers, out-parameters
 *   A reference (T&) is an alias that must refer to something. A pointer (T*) holds an
 *   address, can be null, and can be reseated. Implement the helpers below.
 *
 *     swapInts(a, b)               Swap two ints through references.
 *     swapThrough(pa, pb)          Swap the ints the pointers point to. If either pointer
 *                                  is null, do nothing.
 *     divmod(a, b, q, r)           Out-parameters: q = a / b, r = a % b (C++ truncation).
 *                                  Return false and leave q and r untouched if b == 0 or
 *                                  the division would overflow (INT_MIN / -1).
 *     maxRef(a, b)                 Return a const reference to the larger argument
 *                                  (to a itself on a tie). No copy: the caller can compare
 *                                  addresses.
 *     findFirst(v, target)         Return a pointer to the first element equal to target
 *                                  inside v, or nullptr if absent. Writing through the
 *                                  pointer changes v.
 *     clampAll(v, lo, hi)          Clamp every element of v into [lo, hi] (lo <= hi), in
 *                                  place.
 *     minMaxOut(v, mn, mx)         Out-parameters: smallest and largest element. Return
 *                                  false (leaving mn and mx untouched) if v is empty.
 *
 * Examples:
 *   int a = 1, b = 2; swapInts(a, b);            -> a == 2, b == 1
 *   divmod(-7, 2, q, r)                          -> true, q == -3, r == -1
 *   findFirst(v, 5) with v = {3, 5, 5}           -> pointer to v[1]
 *
 * Constraints / edge cases:
 *   - Swapping a variable with itself (swapInts(x, x)) must leave it unchanged.
 *   - Null pointers must never be dereferenced.
 *   - Do NOT return a reference or pointer to a local variable (dangling).
 *
 * Target complexity: O(1) for swaps/divmod/maxRef; O(n) for the vector helpers.
 *
 * Run: make run FILE=curriculum/101_basics/07_references_pointers.cpp
 *
 * (Stub note: delete the `(void)x;` lines once you use the parameter.)
 */
#include <climits>
#include <iostream>
#include <vector>

#include <algorithm>
#include <cstdlib>
#include <cctype>
#include "check.h"

// ===================== YOUR CODE =====================

void swapInts(int& a, int& b) { int t = a; a = b; b = t; }
void swapThrough(int* pa, int* pb) {
    if (!pa || !pb) return;
    int t = *pa; *pa = *pb; *pb = t;
}
bool divmod(int a, int b, int& q, int& r) {
    if (b == 0 || (a == INT_MIN && b == -1)) return false;
    q = a / b; r = a % b;
    return true;
}
const int& maxRef(const int& a, const int& b) { return (b > a) ? b : a; }
int* findFirst(std::vector<int>& v, int target) {
    for (size_t i = 0; i < v.size(); i++) if (v[i] == target) return &v[i];
    return nullptr;
}
void clampAll(std::vector<int>& v, int lo, int hi) {
    for (int& x : v) { if (x < lo) x = lo; else if (x > hi) x = hi; }
}
bool minMaxOut(const std::vector<int>& v, int& mn, int& mx) {
    if (v.empty()) return false;
    int a = v[0], b = v[0];
    for (int x : v) { if (x < a) a = x; if (x > b) b = x; }
    mn = a; mx = b;
    return true;
}

// ===================== TESTS =====================

int main() {
    // swapInts
    {
        int a = 1, b = 2;
        swapInts(a, b);
        CHECK_EQ(a, 2);
        CHECK_EQ(b, 1);
        int x = 5;
        swapInts(x, x);
        CHECK_EQ(x, 5);
        int lo = INT_MIN, hi = INT_MAX;
        swapInts(lo, hi);
        CHECK_EQ(lo, INT_MAX);
        CHECK_EQ(hi, INT_MIN);
    }

    // swapThrough
    {
        int a = 10, b = 20;
        swapThrough(&a, &b);
        CHECK_EQ(a, 20);
        CHECK_EQ(b, 10);
        swapThrough(&a, nullptr);
        swapThrough(nullptr, &b);
        swapThrough(nullptr, nullptr);
        CHECK_EQ(a, 20);
        CHECK_EQ(b, 10);
        swapThrough(&a, &a);
        CHECK_EQ(a, 20);
        int arr[2] = {1, 2};
        swapThrough(&arr[0], &arr[1]);
        CHECK_EQ(arr[0], 2);
        CHECK_EQ(arr[1], 1);
    }

    // divmod
    {
        int q = -99, r = -99;
        CHECK_EQ(divmod(7, 2, q, r), true);
        CHECK_EQ(q, 3);
        CHECK_EQ(r, 1);
        CHECK_EQ(divmod(-7, 2, q, r), true);
        CHECK_EQ(q, -3);
        CHECK_EQ(r, -1);
        CHECK_EQ(divmod(7, -2, q, r), true);
        CHECK_EQ(q, -3);
        CHECK_EQ(r, 1);
        CHECK_EQ(divmod(0, 5, q, r), true);
        CHECK_EQ(q, 0);
        CHECK_EQ(r, 0);
        CHECK_EQ(divmod(INT_MIN, 1, q, r), true);
        CHECK_EQ(q, INT_MIN);
        CHECK_EQ(r, 0);
        q = 111;
        r = 222;
        CHECK_EQ(divmod(5, 0, q, r), false);
        CHECK_EQ(divmod(INT_MIN, -1, q, r), false);
        CHECK_EQ(q, 111);  // untouched on failure
        CHECK_EQ(r, 222);
    }

    // maxRef
    {
        int a = 3, b = 7;
        CHECK(&maxRef(a, b) == &b);
        CHECK(&maxRef(b, a) == &b);
        int c = 4, d = 4;
        CHECK(&maxRef(c, d) == &c);  // tie -> first argument
        int neg1 = -5, neg2 = -9;
        CHECK_EQ(maxRef(neg1, neg2), -5);
        CHECK_EQ(maxRef(INT_MIN, INT_MAX), INT_MAX);
    }

    // findFirst
    {
        std::vector<int> v = {3, 5, 5, 8};
        int* p = findFirst(v, 5);
        CHECK(p == &v[1]);
        if (p == &v[1]) {
            *p = 50;
            CHECK_EQ(v, std::vector<int>({3, 50, 5, 8}));
        }
        CHECK(findFirst(v, 99) == nullptr);
        std::vector<int> empty;
        CHECK(findFirst(empty, 1) == nullptr);
        CHECK(findFirst(v, 3) == &v[0]);
        CHECK(findFirst(v, 8) == &v[3]);
    }

    // clampAll
    {
        std::vector<int> v = {-10, 0, 5, 10, 100};
        clampAll(v, 0, 10);
        CHECK_EQ(v, std::vector<int>({0, 0, 5, 10, 10}));
        std::vector<int> empty;
        clampAll(empty, 0, 1);
        CHECK_EQ(empty, std::vector<int>{});
        std::vector<int> w = {1, 2, 3};
        clampAll(w, 2, 2);
        CHECK_EQ(w, std::vector<int>({2, 2, 2}));
        std::vector<int> x = {INT_MIN, INT_MAX};
        clampAll(x, -1, 1);
        CHECK_EQ(x, std::vector<int>({-1, 1}));
    }

    // minMaxOut
    {
        int mn = 111, mx = 222;
        CHECK_EQ(minMaxOut({4, -2, 9, 0}, mn, mx), true);
        CHECK_EQ(mn, -2);
        CHECK_EQ(mx, 9);
        CHECK_EQ(minMaxOut({5}, mn, mx), true);
        CHECK_EQ(mn, 5);
        CHECK_EQ(mx, 5);
        mn = 111;
        mx = 222;
        CHECK_EQ(minMaxOut({}, mn, mx), false);
        CHECK_EQ(mn, 111);
        CHECK_EQ(mx, 222);
        CHECK_EQ(minMaxOut({INT_MAX, INT_MIN}, mn, mx), true);
        CHECK_EQ(mn, INT_MIN);
        CHECK_EQ(mx, INT_MAX);
    }

    return check::report();
}
