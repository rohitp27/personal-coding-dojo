// Problem: Merge Sort That Counts Inversions
// Implement sortAndCountInversions(a): sort the vector in place (non-decreasing) AND return
// the number of inversions it contained before sorting. An inversion is a pair of indices
// i < j with a[i] > a[j] (strictly greater: equal values are not inversions).
//
// Examples:
//   {2,4,1,3,5} -> returns 3  ((2,1), (4,1), (4,3)); a becomes {1,2,3,4,5}
//   {5,4,3,2,1} -> returns 10; a becomes {1,2,3,4,5}
//   {2,1,1}     -> returns 2
//   {1,1,1}     -> returns 0
//   {}          -> returns 0
//
// Constraints: 0 <= a.size() <= 200,000; arbitrary int values (including duplicates and
//              negatives). The inversion count can exceed 32 bits.
// Required complexity: O(n log n) time, O(n) extra space. Comparing every pair is ~2*10^10
// operations on the large tests.
//
// Think first:
//   1. While merging two sorted halves, at the moment you take an element from the RIGHT
//      half, what does that tell you about the elements still waiting in the left half?
//   2. With duplicates, which side must win a tie so equal values are not counted as
//      inversions (and the sort stays stable)?
//   3. What is the largest possible inversion count for n elements, and which type holds it?
//
// Run: make run FILE=curriculum/102_fundamentals/recursion_and_sorting/03_merge_sort_inversions.cpp

#include "check.h"

#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

//@REF
static long long sortRange(std::vector<int>& a, std::vector<int>& tmp, size_t lo, size_t hi) {
    if (hi - lo < 2) return 0;
    size_t mid = lo + (hi - lo) / 2;
    long long inv = sortRange(a, tmp, lo, mid) + sortRange(a, tmp, mid, hi);
    size_t i = lo, j = mid, k = lo;
    while (i < mid && j < hi) {
        if (a[i] <= a[j]) {
            tmp[k++] = a[i++];
        } else {
            inv += static_cast<long long>(mid - i);
            tmp[k++] = a[j++];
        }
    }
    while (i < mid) tmp[k++] = a[i++];
    while (j < hi) tmp[k++] = a[j++];
    std::copy(tmp.begin() + lo, tmp.begin() + hi, a.begin() + lo);
    return inv;
}

long long sortAndCountInversions(std::vector<int>& a) {
    std::vector<int> tmp(a.size());
    return sortRange(a, tmp, 0, a.size());
}
long long sortAndCountInversions([[maybe_unused]] std::vector<int>& a) {
    // TODO
    return -1;
}

static long long bruteInversions(const std::vector<int>& a) {
    long long c = 0;
    for (size_t i = 0; i < a.size(); i++)
        for (size_t j = i + 1; j < a.size(); j++)
            if (a[i] > a[j]) c++;
    return c;
}

// Calls the function and also verifies the vector ended up sorted; returns the count,
// or -777 if the vector is not sorted afterwards.
static long long run(std::vector<int> a) {
    std::vector<int> want = a;
    std::sort(want.begin(), want.end());
    long long c = sortAndCountInversions(a);
    return a == want ? c : -777;
}

int main() {
    using V = std::vector<int>;
    CHECK_EQ(run({}), 0LL);
    CHECK_EQ(run({1}), 0LL);
    CHECK_EQ(run({2, 4, 1, 3, 5}), 3LL);
    CHECK_EQ(run({5, 4, 3, 2, 1}), 10LL);
    CHECK_EQ(run({1, 2, 3, 4, 5}), 0LL);
    CHECK_EQ(run({1, 1, 1}), 0LL);
    CHECK_EQ(run({2, 1, 1}), 2LL);
    CHECK_EQ(run({3, 1, 2}), 2LL);
    CHECK_EQ(run({0, -1, -2}), 3LL);
    CHECK_EQ(run({2, 1}), 1LL);
    CHECK_EQ(run({1, 2}), 0LL);
    CHECK_EQ(run({2147483647, -2147483647 - 1}), 1LL);
    CHECK_EQ(run({1, 3, 2, 3, 1}), 4LL);  // (3,2) (3,1) (2,1) (3,1)

    // random small arrays with many duplicates against a brute-force oracle
    {
        std::mt19937 rng(7);
        int bad = 0;
        for (int round = 0; round < 200; round++) {
            int n = static_cast<int>(rng() % 60);
            V a(n);
            for (int& x : a) x = static_cast<int>(rng() % 20) - 10;
            if (run(a) != bruteInversions(a)) bad++;
        }
        V big(2000);
        for (int& x : big) x = static_cast<int>(rng() % 50);
        if (run(big) != bruteInversions(big)) bad++;
        CHECK_EQ(bad, 0);
    }

    // 200,000 elements: all-pairs counting is ~2*10^10 comparisons.
    {
        int n = 200000;
        V desc(n), asc(n), half(n);
        for (int i = 0; i < n; i++) {
            desc[i] = n - i;
            asc[i] = i;
            half[i] = (i + n / 2) % n;  // second half of the values first, then the first half
        }
        CHECK_EQ(run(desc), 19999900000LL);  // n * (n - 1) / 2
        CHECK_EQ(run(asc), 0LL);
        CHECK_EQ(run(half), 10000000000LL);  // (n/2)^2
        CHECK_EQ(run(V(n, 42)), 0LL);
        std::mt19937 rng(99);
        V r(n);
        std::iota(r.begin(), r.end(), 0);
        std::shuffle(r.begin(), r.end(), rng);
        long long c = run(r);
        CHECK(c > 0 && c <= 19999900000LL);  // sortedness is verified inside run()
    }

    return check::report();
}
