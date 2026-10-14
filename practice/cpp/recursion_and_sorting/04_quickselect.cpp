// Problem: K-th Smallest Element (quickselect)
// Given an unsorted vector (taken by value, so you may rearrange it freely) and k
// (1-based), return the k-th smallest element, counting duplicates: in {2,2,3}, the 1st and
// 2nd smallest are both 2 and the 3rd is 3. Precondition: 1 <= k <= a.size().
//
// Examples:
//   a = {3,2,1,5,6,4},         k = 2 -> 2
//   a = {3,2,3,1,2,4,5,5,6},   k = 4 -> 3    (sorted: 1,2,2,3,3,4,5,5,6)
//   a = {7},                   k = 1 -> 7
//   a = {2,2,2,2},             k = 3 -> 2
//
// Constraints: 1 <= a.size() <= 1,000,000; arbitrary int values, many duplicates allowed.
//              Inputs include already-sorted, reverse-sorted and all-equal arrays.
// Required complexity: expected O(n) time, O(1) extra space beyond the copy. Fully sorting
// is O(n log n). A partition scheme with a fixed pivot (first or last element) degrades to
// O(n^2) on the sorted and all-equal tests and will not finish.
//
// Think first:
//   1. After one partition around a pivot, how do you decide which side the k-th smallest is
//      on, and why do you only need to recurse (or loop) into ONE side?
//   2. What pivot rule avoids the O(n^2) behaviour on already-sorted input? And what must the
//      partition do with elements EQUAL to the pivot so an all-equal array does not degrade?
//   3. Can you write it as a loop instead of recursion so stack depth is not a concern?
//
// Run: make run FILE=curriculum/102_fundamentals/recursion_and_sorting/04_quickselect.cpp

#include "check.h"

#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

//@REF
int kthSmallest(std::vector<int> a, int k) {
    static std::mt19937 rng(20240601);
    int lo = 0, hi = static_cast<int>(a.size()) - 1;
    int target = k - 1;
    while (true) {
        if (lo == hi) return a[lo];
        int pivot = a[lo + static_cast<int>(rng() % static_cast<unsigned>(hi - lo + 1))];
        // three-way partition: [lo,lt) < pivot, [lt,gt] == pivot, (gt,hi] > pivot
        int lt = lo, i = lo, gt = hi;
        while (i <= gt) {
            if (a[i] < pivot) std::swap(a[lt++], a[i++]);
            else if (a[i] > pivot) std::swap(a[i], a[gt--]);
            else i++;
        }
        if (target < lt) hi = lt - 1;
        else if (target > gt) lo = gt + 1;
        else return pivot;
    }
}
int kthSmallest([[maybe_unused]] std::vector<int> a, [[maybe_unused]] int k) {
    // TODO
    return -1;
}

int main() {
    using V = std::vector<int>;
    CHECK_EQ(kthSmallest({3, 2, 1, 5, 6, 4}, 2), 2);
    CHECK_EQ(kthSmallest({3, 2, 3, 1, 2, 4, 5, 5, 6}, 4), 3);
    CHECK_EQ(kthSmallest({7}, 1), 7);
    CHECK_EQ(kthSmallest({2, 2, 2, 2}, 3), 2);
    CHECK_EQ(kthSmallest({-5, 10, -3}, 1), -5);
    CHECK_EQ(kthSmallest({-5, 10, -3}, 2), -3);
    CHECK_EQ(kthSmallest({-5, 10, -3}, 3), 10);
    CHECK_EQ(kthSmallest({2, 2, 3}, 2), 2);
    CHECK_EQ(kthSmallest({2, 2, 3}, 3), 3);
    CHECK_EQ(kthSmallest({1, 2}, 2), 2);
    CHECK_EQ(kthSmallest({2, 1}, 1), 1);
    CHECK_EQ(kthSmallest({2147483647, -2147483647 - 1, 0}, 3), 2147483647);

    // every k on small random arrays with many duplicates, against std::sort
    {
        std::mt19937 rng(5);
        int bad = 0;
        for (int round = 0; round < 300; round++) {
            int n = 1 + static_cast<int>(rng() % 40);
            V a(n);
            for (int& x : a) x = static_cast<int>(rng() % 12) - 6;
            V s = a;
            std::sort(s.begin(), s.end());
            for (int k = 1; k <= n; k++)
                if (kthSmallest(a, k) != s[k - 1]) bad++;
        }
        CHECK_EQ(bad, 0);
    }

    // adversarial large inputs (a fixed first/last pivot is ~2*10^10 steps on these)
    {
        int n = 200000;
        V asc(n), desc(n), saw(n), pipe(n);
        for (int i = 0; i < n; i++) {
            asc[i] = i;
            desc[i] = n - 1 - i;
            saw[i] = i % 100;
            pipe[i] = i < n / 2 ? 2 * i : 2 * (n - 1 - i) + 1;  // rises then falls
        }
        int bad = 0;
        for (int k : {1, 2, 1000, 100000, 100001, 199999, 200000}) {
            if (kthSmallest(asc, k) != k - 1) bad++;
            if (kthSmallest(desc, k) != k - 1) bad++;
            if (kthSmallest(saw, k) != (k - 1) / 2000) bad++;  // each value appears 2000 times
        }
        CHECK_EQ(bad, 0);
        V ps = pipe;
        std::sort(ps.begin(), ps.end());
        for (int k : {1, 777, 100000, 200000})
            if (kthSmallest(pipe, k) != ps[k - 1]) bad++;
        CHECK_EQ(bad, 0);
        CHECK_EQ(kthSmallest(V(n, 5), 100000), 5);
        CHECK_EQ(kthSmallest(V(n, 5), 1), 5);
        CHECK_EQ(kthSmallest(V(n, 5), n), 5);
    }
    {
        std::mt19937 rng(321);
        V a(1000000);
        std::iota(a.begin(), a.end(), 0);
        std::shuffle(a.begin(), a.end(), rng);
        CHECK_EQ(kthSmallest(a, 500000), 499999);
        CHECK_EQ(kthSmallest(a, 1), 0);
        CHECK_EQ(kthSmallest(a, 1000000), 999999);
    }

    return check::report();
}
