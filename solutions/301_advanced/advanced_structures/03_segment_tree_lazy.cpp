// 301 advanced > advanced_structures > 03 Segment tree with lazy range add
//
// Problem:
//   Build a structure over an initial array of 64-bit integers supporting
//     pointSet(i, v)       a[i] = v
//     rangeAdd(l, r, d)    a[j] += d for every j in [l, r]   (inclusive)
//     rangeSum(l, r)       sum of a[l..r]
//     rangeMin(l, r)       minimum of a[l..r]
//   Both queries must reflect every earlier update, in any interleaving.
//
// Examples:
//   a = {5,3,8,1,9,2}: rangeSum(0,5) = 28, rangeMin(0,5) = 1, rangeSum(1,3) = 12
//   pointSet(3,10) -> rangeMin(0,5) = 2;  rangeAdd(0,2,-4) -> a = {1,-1,4,10,9,2}
//   rangeAdd(0,5,100) then pointSet(2,7) -> a = {101,99,7,110,109,102}, rangeSum(0,5) = 528
//
// Constraints:
//   1 <= n <= 200000, up to 200000 operations; |values| stays within +-1e15
//   (sums fit in long long). Indices are valid and l <= r.
//
// Required complexity: O(n) build, O(log n) per operation. Applying rangeAdd as a
//   loop of point updates is O(n) and too slow for the large test.
//
// Assumptions: the interface is fixed, the internal layout is up to you.
//
// Think first:
//   1. What exactly does a pending lazy tag mean: "applied to this node but not
//      yet to its children", or the opposite? Pick one and keep it invariant.
//   2. A node stores both sum and min. How does one pending +d change each of
//      them, and what does it depend on (the node's length)?
//   3. pointSet descends through nodes that may carry pending tags. What goes
//      wrong if you skip pushing down?
//
// BEFORE CODING: write your approach, invariant and complexity in a comment
// block right here.
//
// Run: make run FILE=curriculum/301_advanced/advanced_structures/03_segment_tree_lazy.cpp

#include "check.h"

#include <algorithm>
#include <climits>
#include <cstdint>
#include <vector>

// ===== YOUR CODE BELOW =====
class SegTree {
    int n_;
    std::vector<long long> sum_, mn_, lz_;

    void build(int o, int l, int r, const std::vector<long long>& a) {
        if (l == r) {
            sum_[o] = mn_[o] = a[l];
            return;
        }
        int m = (l + r) / 2;
        build(2 * o, l, m, a);
        build(2 * o + 1, m + 1, r, a);
        pull(o);
    }
    void pull(int o) {
        sum_[o] = sum_[2 * o] + sum_[2 * o + 1];
        mn_[o] = std::min(mn_[2 * o], mn_[2 * o + 1]);
    }
    void apply(int o, int len, long long d) {
        sum_[o] += d * len;
        mn_[o] += d;
        lz_[o] += d;
    }
    void push(int o, int l, int r) {
        if (lz_[o] != 0) {
            int m = (l + r) / 2;
            apply(2 * o, m - l + 1, lz_[o]);
            apply(2 * o + 1, r - m, lz_[o]);
            lz_[o] = 0;
        }
    }
    void add(int o, int l, int r, int ql, int qr, long long d) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply(o, r - l + 1, d);
            return;
        }
        push(o, l, r);
        int m = (l + r) / 2;
        add(2 * o, l, m, ql, qr, d);
        add(2 * o + 1, m + 1, r, ql, qr, d);
        pull(o);
    }
    void set(int o, int l, int r, int i, long long v) {
        if (l == r) {
            sum_[o] = mn_[o] = v;
            lz_[o] = 0;
            return;
        }
        push(o, l, r);
        int m = (l + r) / 2;
        if (i <= m) set(2 * o, l, m, i, v);
        else set(2 * o + 1, m + 1, r, i, v);
        pull(o);
    }
    long long qsum(int o, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return sum_[o];
        push(o, l, r);
        int m = (l + r) / 2;
        return qsum(2 * o, l, m, ql, qr) + qsum(2 * o + 1, m + 1, r, ql, qr);
    }
    long long qmin(int o, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return LLONG_MAX;
        if (ql <= l && r <= qr) return mn_[o];
        push(o, l, r);
        int m = (l + r) / 2;
        return std::min(qmin(2 * o, l, m, ql, qr), qmin(2 * o + 1, m + 1, r, ql, qr));
    }

public:
    explicit SegTree(const std::vector<long long>& a)
        : n_((int)a.size()), sum_(4 * a.size(), 0), mn_(4 * a.size(), 0), lz_(4 * a.size(), 0) {
        build(1, 0, n_ - 1, a);
    }
    void pointSet(int i, long long v) { set(1, 0, n_ - 1, i, v); }
    void rangeAdd(int l, int r, long long d) { add(1, 0, n_ - 1, l, r, d); }
    long long rangeSum(int l, int r) { return qsum(1, 0, n_ - 1, l, r); }
    long long rangeMin(int l, int r) { return qmin(1, 0, n_ - 1, l, r); }
};
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
        SegTree t({5, 3, 8, 1, 9, 2});
        CHECK_EQ(t.rangeSum(0, 5), 28LL);
        CHECK_EQ(t.rangeMin(0, 5), 1LL);
        CHECK_EQ(t.rangeSum(1, 3), 12LL);
        CHECK_EQ(t.rangeMin(0, 2), 3LL);
        CHECK_EQ(t.rangeMin(4, 5), 2LL);
        CHECK_EQ(t.rangeSum(2, 2), 8LL);
        CHECK_EQ(t.rangeMin(2, 2), 8LL);

        t.pointSet(3, 10);
        CHECK_EQ(t.rangeMin(0, 5), 2LL);
        CHECK_EQ(t.rangeSum(0, 5), 37LL);

        t.rangeAdd(0, 2, -4);  // {1,-1,4,10,9,2}
        CHECK_EQ(t.rangeSum(0, 5), 25LL);
        CHECK_EQ(t.rangeMin(0, 5), -1LL);
        CHECK_EQ(t.rangeMin(2, 5), 2LL);
        CHECK_EQ(t.rangeSum(0, 1), 0LL);

        t.rangeAdd(0, 5, 100);  // {101,99,104,110,109,102}
        t.pointSet(2, 7);       // {101,99,7,110,109,102}  (pointSet must see through the pending add)
        CHECK_EQ(t.rangeSum(0, 5), 528LL);
        CHECK_EQ(t.rangeMin(0, 5), 7LL);
        CHECK_EQ(t.rangeSum(0, 1), 200LL);
        CHECK_EQ(t.rangeMin(3, 5), 102LL);
    }
    {
        SegTree one({42});
        CHECK_EQ(one.rangeSum(0, 0), 42LL);
        one.rangeAdd(0, 0, -50);
        CHECK_EQ(one.rangeMin(0, 0), -8LL);
        one.pointSet(0, 1);
        CHECK_EQ(one.rangeSum(0, 0), 1LL);
    }

    // random operations vs brute force; several sizes incl. non-powers of two
    for (int n : {1, 2, 3, 7, 16, 33, 100}) {
        Rng rng{(uint64_t)n * 7919};
        std::vector<long long> a(n);
        for (auto& x : a) x = (long long)(rng.next() % 41) - 20;
        SegTree t(a);
        bool ok = true;
        for (int op = 0; op < 3000; op++) {
            int l = (int)(rng.next() % n), r = (int)(rng.next() % n);
            if (l > r) std::swap(l, r);
            int kind = (int)(rng.next() % 4);
            if (kind == 0) {
                long long d = (long long)(rng.next() % 21) - 10;
                t.rangeAdd(l, r, d);
                for (int j = l; j <= r; j++) a[j] += d;
            } else if (kind == 1) {
                long long v = (long long)(rng.next() % 41) - 20;
                t.pointSet(l, v);
                a[l] = v;
            } else if (kind == 2) {
                long long s = 0;
                for (int j = l; j <= r; j++) s += a[j];
                if (t.rangeSum(l, r) != s) ok = false;
            } else {
                long long m = LLONG_MAX;
                for (int j = l; j <= r; j++) m = std::min(m, a[j]);
                if (t.rangeMin(l, r) != m) ok = false;
            }
        }
        CHECK(ok);
    }

    // large: n updates rangeAdd(i, n-1, 1) turn zeros into a[j] = j + 1
    {
        const int n = 200000;
        SegTree t(std::vector<long long>(n, 0));
        for (int i = 0; i < n; i++) t.rangeAdd(i, n - 1, 1);
        CHECK_EQ(t.rangeSum(0, n - 1), (long long)n * (n + 1) / 2);
        CHECK_EQ(t.rangeMin(0, n - 1), 1LL);
        bool ok = true;
        for (int k = 0; k < n; k += 1013)
            if (t.rangeMin(k, n - 1) != k + 1 || t.rangeSum(k, k) != k + 1) ok = false;
        CHECK(ok);
        t.rangeAdd(0, n - 1, 1000000000000LL);
        CHECK_EQ(t.rangeMin(0, n - 1), 1000000000001LL);
        CHECK_EQ(t.rangeSum(n - 1, n - 1), 1000000000000LL + n);
        for (int i = 0; i < n; i += 2) t.pointSet(i, 0);
        CHECK_EQ(t.rangeMin(0, n - 1), 0LL);
        CHECK_EQ(t.rangeSum(0, 1), 1000000000002LL);
    }

    return check::report();
}
