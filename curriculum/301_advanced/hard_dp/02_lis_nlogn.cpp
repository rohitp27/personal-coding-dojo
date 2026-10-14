// 301 advanced > hard_dp > 02 Longest increasing subsequence in O(n log n)
//
// Problem:
//   (a) lisLength(a): length of the longest STRICTLY increasing subsequence.
//   (b) lisIndices(a): positions i1 < i2 < ... < ik of one such subsequence of
//       maximum length (any valid answer is accepted; the tests verify it).
//
// Examples:
//   {10,9,2,5,3,7,101,18} -> length 4 (e.g. 2,3,7,18)
//   {7,7,7}               -> length 1 (strict: equal values do not extend)
//   {}                    -> length 0, indices {}
//
// Constraints:
//   0 <= n <= 200000, values are arbitrary ints (negative allowed).
//
// Required complexity: O(n log n) time, O(n) space for both functions.
//   The O(n^2) DP is too slow for the large tests.
//
// Assumptions: for (b), return indices in increasing order of position.
//
// Think first:
//   1. State the invariant of your "tails" array in one sentence (what does
//      tails[k] hold, and why is it sorted?).
//   2. Strict vs non-decreasing: which binary search variant do you need, and
//      what silently changes if you pick the other one?
//   3. The tails array itself is NOT the subsequence. What extra information
//      do you record per element to reconstruct it?
//
// BEFORE CODING: write your approach, invariant and complexity in a comment
// block right here.
//
// Run: make run FILE=curriculum/301_advanced/hard_dp/02_lis_nlogn.cpp

#include "check.h"

#include <algorithm>
#include <cstdint>
#include <vector>

// ===== YOUR CODE BELOW =====
#pragma GCC diagnostic ignored "-Wunused-parameter"  // stub only; delete when you implement

int lisLength(const std::vector<int>& a) {
    // TODO
    return -1;
}

std::vector<int> lisIndices(const std::vector<int>& a) {
    // TODO
    return {};
}

// ===== TESTS (do not edit) =====
struct Rng {
    uint64_t s;
    uint64_t next() {
        s = s * 6364136223846793005ULL + 1442695040888963407ULL;
        return s >> 33;
    }
};

static int quadratic(const std::vector<int>& a) {
    std::vector<int> dp(a.size(), 1);
    int best = 0;
    for (size_t i = 0; i < a.size(); i++) {
        for (size_t j = 0; j < i; j++)
            if (a[j] < a[i]) dp[i] = std::max(dp[i], dp[j] + 1);
        best = std::max(best, dp[i]);
    }
    return best;
}

// idx must be strictly increasing positions with strictly increasing values and have `len` entries.
static bool validLis(const std::vector<int>& a, const std::vector<int>& idx, int len) {
    if ((int)idx.size() != len) return false;
    for (size_t k = 0; k < idx.size(); k++) {
        if (idx[k] < 0 || idx[k] >= (int)a.size()) return false;
        if (k > 0 && (idx[k] <= idx[k - 1] || a[idx[k]] <= a[idx[k - 1]])) return false;
    }
    return true;
}

int main() {
    using V = std::vector<int>;
    struct Case { V a; int len; };
    std::vector<Case> cases = {
        {{10, 9, 2, 5, 3, 7, 101, 18}, 4},
        {{0, 1, 0, 3, 2, 3}, 4},
        {{7, 7, 7}, 1},
        {{}, 0},
        {{5}, 1},
        {{5, 4, 3, 2, 1}, 1},
        {{1, 2, 3, 4, 5}, 5},
        {{-3, -1, -2, 0, -5, 4}, 4},
        {{INT32_MIN, INT32_MAX}, 2},
        {{3, 3, 4, 4, 5, 5}, 3},
        {{1, 3, 6, 7, 9, 4, 10, 5, 6}, 6},
    };
    for (auto& c : cases) {
        CHECK_EQ(lisLength(c.a), c.len);
        CHECK(validLis(c.a, lisIndices(c.a), c.len));
    }

    Rng rng{101};
    for (int trial = 0; trial < 400; trial++) {
        int n = (int)(rng.next() % 60);
        V a(n);
        for (auto& x : a) x = (int)(rng.next() % 21) - 10;
        int want = quadratic(a);
        CHECK_EQ(lisLength(a), want);
        CHECK(validLis(a, lisIndices(a), want));
    }

    // large cases with a known answer
    {
        const int n = 200000;
        V asc(n), equal(n, 5), blocks(n);
        for (int i = 0; i < n; i++) asc[i] = i - 100000;
        CHECK_EQ(lisLength(asc), n);
        CHECK(validLis(asc, lisIndices(asc), n));
        CHECK_EQ(lisLength(equal), 1);
        // 1000 blocks of 200; each block strictly decreasing; block ranges increase.
        // At most one element per block can be used and one per block is achievable -> 1000.
        for (int b = 0; b < 1000; b++)
            for (int j = 0; j < 200; j++) blocks[b * 200 + j] = b * 200 + (199 - j);
        CHECK_EQ(lisLength(blocks), 1000);
        CHECK(validLis(blocks, lisIndices(blocks), 1000));
        V desc(n);
        for (int i = 0; i < n; i++) desc[i] = n - i;
        CHECK_EQ(lisLength(desc), 1);
        // random: the two functions must agree and the indices must be valid
        V r(n);
        for (auto& x : r) x = (int)(rng.next() % 1000000);
        int len = lisLength(r);
        CHECK(len > 100 && len < 2000);  // sanity: ~2*sqrt(n) ~ 900 for random data
        CHECK(validLis(r, lisIndices(r), len));
    }

    return check::report();
}
