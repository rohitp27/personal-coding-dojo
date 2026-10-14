// 301 advanced > hard_dp > 03 Interval DP: merging adjacent piles, K at a time
//
// Problem:
//   n piles sit in a row. One move merges exactly K CONSECUTIVE piles into a
//   single pile and costs the total size of those K piles. Return the minimum
//   total cost to end with a single pile, or -1 if that is impossible.
//
// Examples:
//   piles {3,2,4,1}, K=2        -> 20  ((3+2) + (4+1) + (5+5))
//   piles {3,5,1,2,6}, K=3      -> 25  (merge 5,1,2 -> 8, then 3+8+6 = 17; 8 + 17)
//   piles {3,2,4,1}, K=3        -> -1  (every move removes K-1 = 2 piles; 4 -> 1 needs 3)
//   piles {7}, K=anything       -> 0
//
// Constraints:
//   1 <= n <= 300, 2 <= K <= 8, 1 <= pile <= 1000.
//
// Required complexity: about O(n^3) time (up to O(n^3 * K / (K - 1)) is fine) and
//   O(n^2 * K) or better space. Trying merge orders exhaustively is exponential.
//
// Assumptions: K >= 2; the result fits in long long.
//
// Think first:
//   1. When is the answer -1, decided by arithmetic alone before any DP?
//   2. Define the state on an interval [i, j] with an extra dimension: "merged
//      into m piles". Why do you need m, and why can you step the split point
//      by K-1 instead of by 1?
//   3. Where does the cost of a merge get charged - at each level, or once per
//      final merge of an interval? How do prefix sums come in?
//
// BEFORE CODING: write your approach, invariant and complexity in a comment
// block right here.
//
// Run: make run FILE=curriculum/301_advanced/hard_dp/03_interval_merge_cost.cpp

#include "check.h"

#include <algorithm>
#include <climits>
#include <cstdint>
#include <map>
#include <vector>

// ===== YOUR CODE BELOW =====
#pragma GCC diagnostic ignored "-Wunused-parameter"  // stub only; delete when you implement

long long minMergeCost(const std::vector<int>& piles, int K) {
    // TODO
    return -2;
}

// ===== TESTS (do not edit) =====
struct Rng {
    uint64_t s;
    uint64_t next() {
        s = s * 6364136223846793005ULL + 1442695040888963407ULL;
        return s >> 33;
    }
};

// Exhaustive search over every merge order, memoized on the whole row (tiny n only).
static long long brute(const std::vector<int>& row, int K, std::map<std::vector<int>, long long>& memo) {
    if (row.size() == 1) return 0;
    auto it = memo.find(row);
    if (it != memo.end()) return it->second;
    long long best = -1;
    for (size_t s = 0; s + K <= row.size(); s++) {
        std::vector<int> next(row.begin(), row.begin() + s);
        int sum = 0;
        for (int t = 0; t < K; t++) sum += row[s + t];
        next.push_back(sum);
        next.insert(next.end(), row.begin() + s + K, row.end());
        long long rest = brute(next, K, memo);
        if (rest >= 0 && (best < 0 || rest + sum < best)) best = rest + sum;
    }
    return memo[row] = best;
}

int main() {
    using V = std::vector<int>;
    CHECK_EQ(minMergeCost(V{3, 2, 4, 1}, 2), 20LL);
    CHECK_EQ(minMergeCost(V{3, 5, 1, 2, 6}, 3), 25LL);
    CHECK_EQ(minMergeCost(V{3, 2, 4, 1}, 3), -1LL);
    CHECK_EQ(minMergeCost(V{7}, 2), 0LL);
    CHECK_EQ(minMergeCost(V{7}, 5), 0LL);
    CHECK_EQ(minMergeCost(V{4, 5}, 2), 9LL);
    CHECK_EQ(minMergeCost(V{4, 5}, 3), -1LL);
    CHECK_EQ(minMergeCost(V{1, 2, 3}, 3), 6LL);
    CHECK_EQ(minMergeCost(V{1, 2, 3, 4}, 4), 10LL);
    CHECK_EQ(minMergeCost(V{1, 1, 1, 1}, 2), 8LL);  // balanced tree: every pile at depth 2

    Rng rng{555};
    for (int trial = 0; trial < 400; trial++) {
        int n = 1 + (int)(rng.next() % 8);
        int K = 2 + (int)(rng.next() % 3);
        V piles(n);
        for (auto& p : piles) p = 1 + (int)(rng.next() % 9);
        std::map<std::vector<int>, long long> memo;
        CHECK_EQ(minMergeCost(piles, K), brute(piles, K, memo));
    }

    // large, with closed-form answers: n equal piles of size 1 and K = 2 is the cost of a
    // complete binary tree with n leaves, i.e. the sum of leaf depths.
    //   n = 256: 256 leaves at depth 8                       = 2048
    //   n = 300: 212 leaves at depth 8 + 88 at depth 9       = 2488
    CHECK_EQ(minMergeCost(V(256, 1), 2), 2048LL);
    CHECK_EQ(minMergeCost(V(300, 1), 2), 2488LL);
    CHECK_EQ(minMergeCost(V(300, 1), 4), -1LL);  // 299 % 3 != 0
    // 256 piles, K = 4: (256-1) % 3 == 0; four levels of 4-way merges -> 4 levels * 256 = 1024
    CHECK_EQ(minMergeCost(V(256, 1), 4), 1024LL);
    // 299 piles, K = 3: a full ternary tree with 299 leaves; 215 leaves at depth 5 and
    // 84 at depth 6 (3a + b = 729, a + b = 299) -> 215*5 + 84*6 = 1579
    CHECK_EQ(minMergeCost(V(299, 1), 3), 1579LL);

    return check::report();
}
