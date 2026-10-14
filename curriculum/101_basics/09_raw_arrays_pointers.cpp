/*
 * Problem: Raw arrays and pointer arithmetic
 *   A C-style array decays to a pointer to its first element. If p points into an array,
 *   p + i points i elements further, *(p + i) == p[i], and subtracting two pointers into
 *   the same array gives the element distance. A half-open range [first, last) means
 *   `last` points ONE PAST the final element and must never be dereferenced.
 *   Implement these using pointers (no std::vector, no <algorithm>).
 *
 *     sumRange(first, last)       Sum of the ints in [first, last). Empty if first == last.
 *     reverseRaw(first, last)     Reverse the ints in [first, last) in place.
 *     findMax(a, n)               Pointer to the first largest element of a[0..n), or
 *                                 nullptr if n == 0.
 *     myStrlen(s)                 Length of the NUL-terminated string s (like strlen).
 *     copyStr(dst, src)           Copy src including its terminating '\0' into dst
 *                                 (like strcpy; the caller guarantees dst is big enough).
 *     countChar(s, c)             How many times c occurs in the NUL-terminated string s.
 *     partitionEvens(a, n)        Rearrange a[0..n) so all even values come before all odd
 *                                 values (order inside each group is free). Return a pointer
 *                                 to the first odd element, or a + n if there is none.
 *                                 Two pointers moving toward each other; no extra array.
 *
 * Examples:
 *   int a[] = {1, 2, 3}; sumRange(a, a + 3)       -> 6
 *   reverseRaw(a, a + 3)                          -> a is {3, 2, 1}
 *   myStrlen("hello")                             -> 5
 *   int b[] = {1, 2, 3, 4}; partitionEvens(b, 4)  -> pointer p with p - b == 2,
 *                                                    b[0..2) even, b[2..4) odd
 *
 * Constraints / edge cases:
 *   - Empty ranges (first == last, n == 0): never dereference, never loop forever.
 *   - Never touch memory outside [first, last); the tests place sentinel values on both
 *     sides and check they are unchanged.
 *   - Negative odd numbers: -3 % 2 is -1, so test oddness with (x % 2 != 0), not (x % 2 == 1).
 *   - Ties in findMax: return the FIRST maximum.
 *
 * Target complexity: O(n) time, O(1) extra space.
 *
 * Run: make run FILE=curriculum/101_basics/09_raw_arrays_pointers.cpp
 *
 * (Stub note: delete the `(void)x;` lines once you use the parameter.)
 */
#include <algorithm>  // used by the tests only (std::sort, std::vector compare)
#include <climits>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "check.h"

// ===================== YOUR CODE =====================

long long sumRange(const int* first, const int* last) {
    // TODO. Hint: `for (const int* p = first; p != last; ++p) sum += *p;`
    (void)first;
    (void)last;
    return -1;
}

void reverseRaw(int* first, int* last) {
    // TODO. Hint: if first == last return; --last; then swap(*first, *last) while first < last.
    (void)first;
    (void)last;
}

const int* findMax(const int* a, std::size_t n) {
    // TODO. Hint: remember a pointer to the best element so far; compare *best with a[i] using <.
    (void)a;
    (void)n;
    return a;  // TODO: should be nullptr when n == 0
}

std::size_t myStrlen(const char* s) {
    // TODO. Hint: advance a second pointer until it hits '\0'; the answer is end - s.
    (void)s;
    return 999;
}

void copyStr(char* dst, const char* src) {
    // TODO. Hint: copy char by char, including the final '\0'.
    (void)dst;
    (void)src;
}

std::size_t countChar(const char* s, char c) {
    // TODO. Hint: walk until *s == '\0'.
    (void)s;
    (void)c;
    return 999;
}

int* partitionEvens(int* a, std::size_t n) {
    // TODO. Hint: lo = a, hi = a + n; move lo past evens, move hi back past odds, swap, repeat.
    (void)n;
    return a;
}

// ===================== TESTS =====================

static std::vector<int> sorted(std::vector<int> v) {
    std::sort(v.begin(), v.end());
    return v;
}

int main() {
    // sumRange
    {
        int a[] = {1, 2, 3};
        CHECK_EQ(sumRange(a, a + 3), 6LL);
        CHECK_EQ(sumRange(a, a), 0LL);
        CHECK_EQ(sumRange(a + 1, a + 3), 5LL);
        CHECK_EQ(sumRange(a + 3, a + 3), 0LL);
        int neg[] = {-5, 5, -10};
        CHECK_EQ(sumRange(neg, neg + 3), -10LL);
        int big[] = {INT_MAX, INT_MAX, INT_MAX};
        CHECK_EQ(sumRange(big, big + 3), 3LL * INT_MAX);
    }

    // reverseRaw (sentinels 99 on both sides)
    {
        int buf[] = {99, 1, 2, 3, 4, 5, 99};
        reverseRaw(buf + 1, buf + 6);
        CHECK_EQ(std::vector<int>(buf, buf + 7), std::vector<int>({99, 5, 4, 3, 2, 1, 99}));
        int even[] = {99, 1, 2, 3, 4, 99};
        reverseRaw(even + 1, even + 5);
        CHECK_EQ(std::vector<int>(even, even + 6), std::vector<int>({99, 4, 3, 2, 1, 99}));
        int one[] = {99, 7, 99};
        reverseRaw(one + 1, one + 2);
        CHECK_EQ(std::vector<int>(one, one + 3), std::vector<int>({99, 7, 99}));
        int none[] = {99, 99};
        reverseRaw(none + 1, none + 1);
        CHECK_EQ(std::vector<int>(none, none + 2), std::vector<int>({99, 99}));
        int whole[] = {1, 2};
        reverseRaw(whole, whole + 2);
        CHECK_EQ(std::vector<int>(whole, whole + 2), std::vector<int>({2, 1}));
    }

    // findMax
    {
        int a[] = {3, 9, 2, 9, 1};
        const int* p = findMax(a, 5);
        CHECK(p == a + 1);  // first maximum
        if (p == a + 1) CHECK_EQ(*p, 9);
        int one[] = {-4};
        CHECK(findMax(one, 1) == one);
        int neg[] = {-9, -3, -7};
        CHECK(findMax(neg, 3) == neg + 1);
        int lim[] = {INT_MIN, INT_MIN};
        CHECK(findMax(lim, 2) == lim);
        CHECK(findMax(a, 0) == nullptr);
        CHECK(findMax(nullptr, 0) == nullptr);
        int last[] = {1, 2, 3};
        CHECK(findMax(last, 3) == last + 2);
    }

    // myStrlen
    CHECK_EQ(myStrlen(""), std::size_t(0));
    CHECK_EQ(myStrlen("a"), std::size_t(1));
    CHECK_EQ(myStrlen("hello"), std::size_t(5));
    CHECK_EQ(myStrlen("a b"), std::size_t(3));
    {
        char s[] = {'a', 'b', '\0', 'c', '\0'};  // stops at the first NUL
        CHECK_EQ(myStrlen(s), std::size_t(2));
    }

    // copyStr (dst pre-filled with 'X', last byte always '\0' so reading it is safe)
    {
        char dst[8];
        std::fill(dst, dst + 7, 'X');
        dst[7] = '\0';
        copyStr(dst, "abc");
        CHECK_EQ(std::string(dst), "abc");
        CHECK_EQ(dst[3], '\0');   // terminator copied
        CHECK_EQ(dst[4], 'X');    // nothing written past it
        CHECK_EQ(dst[6], 'X');

        std::fill(dst, dst + 7, 'X');
        copyStr(dst, "");
        CHECK_EQ(std::string(dst), "");
        CHECK_EQ(dst[1], 'X');

        std::fill(dst, dst + 7, 'X');
        copyStr(dst, "1234567");  // exactly fills with the NUL in dst[7]
        CHECK_EQ(std::string(dst), "1234567");
    }

    // countChar
    CHECK_EQ(countChar("banana", 'a'), std::size_t(3));
    CHECK_EQ(countChar("banana", 'n'), std::size_t(2));
    CHECK_EQ(countChar("banana", 'z'), std::size_t(0));
    CHECK_EQ(countChar("", 'a'), std::size_t(0));
    CHECK_EQ(countChar("aaaa", 'a'), std::size_t(4));
    CHECK_EQ(countChar("a b c", ' '), std::size_t(2));
    CHECK_EQ(countChar("abc", '\0'), std::size_t(0));  // terminator is not part of the string

    // partitionEvens
    {
        int a[] = {99, 1, 2, 3, 4, 99};
        int* p = partitionEvens(a + 1, 4);
        CHECK_EQ(p - (a + 1), std::ptrdiff_t(2));
        CHECK_EQ(a[0], 99);
        CHECK_EQ(a[5], 99);
        CHECK_EQ(sorted(std::vector<int>(a + 1, a + 5)), std::vector<int>({1, 2, 3, 4}));
        for (int* q = a + 1; q < p; ++q) CHECK(*q % 2 == 0);
        for (int* q = p; q < a + 5; ++q) CHECK(*q % 2 != 0);
    }
    {
        int odd[] = {1, 3, 5};
        int* p = partitionEvens(odd, 3);
        CHECK(p == odd);
        int even[] = {2, 4, 6};
        int* q = partitionEvens(even, 3);
        CHECK(q == even + 3);
        int neg[] = {-3, -2, -1, 0};
        int* r = partitionEvens(neg, 4);
        CHECK_EQ(r - neg, std::ptrdiff_t(2));
        for (int* t = neg; t < r; ++t) CHECK(*t % 2 == 0);
        for (int* t = r; t < neg + 4; ++t) CHECK(*t % 2 != 0);
        CHECK_EQ(sorted(std::vector<int>(neg, neg + 4)), std::vector<int>({-3, -2, -1, 0}));
        int single[] = {7};
        CHECK(partitionEvens(single, 1) == single);
        int one_even[] = {8};
        CHECK(partitionEvens(one_even, 1) == one_even + 1);
        int dummy[] = {1};
        CHECK(partitionEvens(dummy, 0) == dummy);
        int mix[] = {5, 4, 3, 2, 1, 0, 7, 8};
        int* m = partitionEvens(mix, 8);
        CHECK_EQ(m - mix, std::ptrdiff_t(4));
        for (int* t = mix; t < m; ++t) CHECK(*t % 2 == 0);
        for (int* t = m; t < mix + 8; ++t) CHECK(*t % 2 != 0);
        CHECK_EQ(sorted(std::vector<int>(mix, mix + 8)), std::vector<int>({0, 1, 2, 3, 4, 5, 7, 8}));
    }

    return check::report();
}
