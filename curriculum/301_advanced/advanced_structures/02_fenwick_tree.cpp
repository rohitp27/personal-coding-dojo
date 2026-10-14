// 301 advanced > advanced_structures > 02 Fenwick tree (BIT) and inversions
//
// Problem:
//   (a) class Fenwick over n zero-initialized 64-bit integers (0-indexed):
//         add(i, delta)        a[i] += delta
//         prefix(i)            sum of a[0..i]; prefix(-1) == 0
//         range(l, r)          sum of a[l..r] inclusive
//         lowerBound(target)   smallest index i with prefix(i) >= target, or n if none.
//                              Only called when every a[j] >= 0 and target >= 1.
//   (b) countInversions(a): number of pairs i < j with a[i] > a[j] (strict), for
//       arbitrary int values (negative, INT_MIN, INT_MAX, duplicates).
//
// Examples:
//   a = [3,0,2,5,0,1]: prefix sums 3,3,5,10,10,11
//     lowerBound(4) = 2, lowerBound(10) = 3, lowerBound(12) = 6 (= n, not found)
//   countInversions({5,1,5,1}) = 3,  countInversions({2,2,2}) = 0
//
// Constraints:
//   n <= 200000 for both parts. Totals can exceed 32 bits (inversions up to ~2e10).
//
// Required complexity: every Fenwick operation O(log n), including lowerBound
//   (no binary search wrapping prefix(), which would be O(log^2 n)).
//   countInversions O(n log n) time, O(n) space.
//
// Assumptions: indices are in range; countInversions may sort/compress values.
//
// Think first:
//   1. What does tree[i] cover, in terms of the lowest set bit of i? Derive the
//      update and query loops from that, do not memorize them.
//   2. lowerBound in O(log n): why does descending powers of two work, and which
//      property of the values makes it valid?
//   3. For inversions: what do you index the BIT by, and what must you do first
//      when values are huge or negative?
//
// BEFORE CODING: write your approach, invariant and complexity in a comment
// block right here.
//
// Run: make run FILE=curriculum/301_advanced/advanced_structures/02_fenwick_tree.cpp

#include "check.h"

#include <algorithm>
#include <climits>
#include <cstdint>
#include <vector>

// ===== YOUR CODE BELOW =====
#pragma GCC diagnostic ignored "-Wunused-parameter"  // stub only; delete when you implement

class Fenwick {
public:
    explicit Fenwick(int n) {
        // TODO
    }
    void add(int i, long long delta) {
        // TODO
    }
    long long prefix(int i) const {
        // TODO
        return -1;
    }
    long long range(int l, int r) const {
        // TODO
        return -1;
    }
    int lowerBound(long long target) const {
        // TODO
        return -2;
    }
};

long long countInversions(const std::vector<int>& a) {
    // TODO
    return -1;
}

// ===== TESTS (do not edit) =====
struct Rng {
    uint64_t s;
    uint64_t next() {
        s = s * 6364136223846793005ULL + 1442695040888963407ULL;
        return s >> 33;
    }
};

int main() {
    {
        Fenwick f(8);
        f.add(0, 5);
        f.add(3, 2);
        f.add(7, 10);
        CHECK_EQ(f.prefix(-1), 0LL);
        CHECK_EQ(f.prefix(0), 5LL);
        CHECK_EQ(f.prefix(2), 5LL);
        CHECK_EQ(f.prefix(3), 7LL);
        CHECK_EQ(f.prefix(7), 17LL);
        CHECK_EQ(f.range(1, 3), 2LL);
        CHECK_EQ(f.range(4, 6), 0LL);
        CHECK_EQ(f.range(0, 7), 17LL);
        CHECK_EQ(f.range(7, 7), 10LL);
        f.add(3, -4);  // negatives are fine for add/prefix/range
        CHECK_EQ(f.range(3, 3), -2LL);
        CHECK_EQ(f.prefix(7), 13LL);
    }
    {
        Fenwick one(1);
        one.add(0, 7);
        CHECK_EQ(one.prefix(0), 7LL);
        CHECK_EQ(one.range(0, 0), 7LL);
        CHECK_EQ(one.lowerBound(7), 0);
        CHECK_EQ(one.lowerBound(8), 1);
    }
    {
        // values [3,0,2,5,0,1]
        Fenwick f(6);
        int vals[] = {3, 0, 2, 5, 0, 1};
        for (int i = 0; i < 6; i++) f.add(i, vals[i]);
        CHECK_EQ(f.lowerBound(1), 0);
        CHECK_EQ(f.lowerBound(3), 0);
        CHECK_EQ(f.lowerBound(4), 2);
        CHECK_EQ(f.lowerBound(5), 2);
        CHECK_EQ(f.lowerBound(6), 3);
        CHECK_EQ(f.lowerBound(10), 3);
        CHECK_EQ(f.lowerBound(11), 5);
        CHECK_EQ(f.lowerBound(12), 6);
        CHECK_EQ(f.lowerBound(1000000000000LL), 6);
    }
    {
        // non-power-of-two size, brute-force comparison
        Rng rng{5};
        for (int trial = 0; trial < 100; trial++) {
            int n = 1 + (int)(rng.next() % 37);
            Fenwick f(n);
            std::vector<long long> a(n, 0);
            for (int op = 0; op < 60; op++) {
                int i = (int)(rng.next() % n);
                long long d = (long long)(rng.next() % 10);
                f.add(i, d);
                a[i] += d;
                int l = (int)(rng.next() % n), r = (int)(rng.next() % n);
                if (l > r) std::swap(l, r);
                long long want = 0;
                for (int j = l; j <= r; j++) want += a[j];
                CHECK_EQ(f.range(l, r), want);
                long long total = 0;
                for (long long v : a) total += v;
                long long target = 1 + (long long)(rng.next() % (total + 2));
                int expect = n;
                long long run = 0;
                for (int j = 0; j < n; j++) {
                    run += a[j];
                    if (run >= target) { expect = j; break; }
                }
                CHECK_EQ(f.lowerBound(target), expect);
            }
        }
    }
    {
        const int n = 200000;
        Fenwick f(n);
        for (int i = 0; i < n; i++) f.add(i, 1);
        bool ok = true;
        for (int i = 0; i < n; i += 997)
            if (f.prefix(i) != i + 1 || f.lowerBound(i + 1) != i) ok = false;
        CHECK(ok);
        CHECK_EQ(f.lowerBound(n), n - 1);
        CHECK_EQ(f.lowerBound((long long)n + 1), n);
    }

    // inversions
    CHECK_EQ(countInversions({}), 0LL);
    CHECK_EQ(countInversions({1}), 0LL);
    CHECK_EQ(countInversions({2, 1}), 1LL);
    CHECK_EQ(countInversions({3, 2, 1}), 3LL);
    CHECK_EQ(countInversions({1, 2, 3}), 0LL);
    CHECK_EQ(countInversions({2, 2, 2}), 0LL);
    CHECK_EQ(countInversions({5, 1, 5, 1}), 3LL);
    CHECK_EQ(countInversions({-1, -5, INT_MIN, INT_MAX}), 3LL);
    CHECK_EQ(countInversions({INT_MAX, INT_MIN}), 1LL);
    {
        Rng rng{77};
        for (int trial = 0; trial < 100; trial++) {
            int n = (int)(rng.next() % 60);
            std::vector<int> a(n);
            for (auto& x : a) x = (int)(rng.next() % 21) - 10;
            long long want = 0;
            for (int i = 0; i < n; i++)
                for (int j = i + 1; j < n; j++)
                    if (a[i] > a[j]) want++;
            CHECK_EQ(countInversions(a), want);
        }
        const int n = 200000;
        std::vector<int> desc(n), asc(n), blocks(n);
        for (int i = 0; i < n; i++) {
            desc[i] = n - i;
            asc[i] = i;
        }
        CHECK_EQ(countInversions(desc), (long long)n * (n - 1) / 2);
        CHECK_EQ(countInversions(asc), 0LL);
        // 1000 blocks of 200, each block ascending, blocks in descending order:
        // every cross-block pair is an inversion: C(1000,2) * 200 * 200
        for (int b = 0; b < 1000; b++)
            for (int j = 0; j < 200; j++) blocks[b * 200 + j] = (999 - b) * 200 + j;
        CHECK_EQ(countInversions(blocks), 1000LL * 999 / 2 * 200 * 200);
    }

    return check::report();
}
