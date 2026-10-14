// 301 advanced > hard_dp > 04 Bitmask DP: cheapest closed walk visiting every node
//
// Problem:
//   A directed graph has n nodes (0..n-1) and weighted edges {from, to, w}.
//   Start at node 0, visit EVERY node at least once, and return to node 0.
//   Nodes and edges may be reused. Return the minimum total weight of such a walk,
//   or -1 if no such walk exists.
//
// Examples:
//   n=3, edges {0->1 w1, 1->2 w2, 2->0 w3}                        -> 6
//   n=3, edges {0->1 w1, 1->0 w1, 0->2 w1, 2->0 w1}               -> 4  (0,1,0,2,0 revisits node 0)
//   n=2, edges {0->1 w1}                                          -> -1 (cannot come back)
//   n=1                                                           -> 0
//
// Constraints:
//   1 <= n <= 18, 1 <= w <= 1000, up to n*n edges (parallel edges allowed, keep the cheapest).
//   Weights are NOT guaranteed to satisfy the triangle inequality.
//
// Required complexity: O(n^3 + 2^n * n^2) time, O(2^n * n) space. Trying all
//   permutations (n!) is too slow at n = 18.
//
// Assumptions: self-loops may appear and are never useful.
//
// Think first:
//   1. Why can't you run the usual "visit each node exactly once" DP directly on
//      the input edges, and what preprocessing makes it valid?
//   2. State the DP: what do (mask, last) mean, what is the base state, and what
//      is the final combination step that closes the walk?
//   3. Unreachable pairs: how do you represent "no path" so it never wins a min()
//      and never overflows when added?
//
// BEFORE CODING: write your approach, invariant and complexity in a comment
// block right here.
//
// Run: make run FILE=curriculum/301_advanced/hard_dp/04_bitmask_closed_walk.cpp

#include "check.h"

#include <algorithm>
#include <array>
#include <climits>
#include <cstdint>
#include <numeric>
#include <vector>

// ===== YOUR CODE BELOW =====
long long minClosedWalk(int n, const std::vector<std::array<int, 3>>& edges) {
    if (n == 1) return 0;
    const long long INF = LLONG_MAX / 4;
    std::vector<std::vector<long long>> d(n, std::vector<long long>(n, INF));
    for (int i = 0; i < n; i++) d[i][i] = 0;
    for (auto& e : edges) d[e[0]][e[1]] = std::min<long long>(d[e[0]][e[1]], e[2]);
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (d[i][k] + d[k][j] < d[i][j]) d[i][j] = d[i][k] + d[k][j];
    int full = (1 << n) - 1;
    std::vector<long long> dp((size_t)(full + 1) * n, INF);
    dp[(size_t)1 * n + 0] = 0;
    for (int mask = 1; mask <= full; mask++) {
        if (!(mask & 1)) continue;
        for (int last = 0; last < n; last++) {
            long long cur = dp[(size_t)mask * n + last];
            if (cur >= INF) continue;
            for (int nxt = 0; nxt < n; nxt++) {
                if (mask >> nxt & 1) continue;
                if (d[last][nxt] >= INF) continue;
                size_t idx = (size_t)(mask | 1 << nxt) * n + nxt;
                dp[idx] = std::min(dp[idx], cur + d[last][nxt]);
            }
        }
    }
    long long best = INF;
    for (int last = 0; last < n; last++) {
        long long c = dp[(size_t)full * n + last];
        if (c < INF && d[last][0] < INF) best = std::min(best, c + d[last][0]);
    }
    return best >= INF ? -1 : best;
}
// ===== TESTS (do not edit) =====
struct Rng {
    uint64_t s;
    uint64_t next() {
        s = s * 6364136223846793005ULL + 1442695040888963407ULL;
        return s >> 33;
    }
};

using E = std::vector<std::array<int, 3>>;

// Floyd-Warshall, then try every visiting order of nodes 1..n-1 (tiny n only).
static long long brute(int n, const E& edges) {
    const long long INF = LLONG_MAX / 4;
    std::vector<std::vector<long long>> d(n, std::vector<long long>(n, INF));
    for (int i = 0; i < n; i++) d[i][i] = 0;
    for (auto& e : edges) d[e[0]][e[1]] = std::min<long long>(d[e[0]][e[1]], e[2]);
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);
    if (n == 1) return 0;
    std::vector<int> p(n - 1);
    std::iota(p.begin(), p.end(), 1);
    long long best = INF;
    do {
        long long c = 0;
        int cur = 0;
        bool ok = true;
        for (int v : p) {
            if (d[cur][v] >= INF) ok = false;
            c += ok ? d[cur][v] : 0;
            cur = v;
        }
        if (d[cur][0] >= INF) ok = false;
        c += ok ? d[cur][0] : 0;
        if (ok) best = std::min(best, c);
    } while (std::next_permutation(p.begin(), p.end()));
    return best >= INF ? -1 : best;
}

int main() {
    CHECK_EQ(minClosedWalk(3, E{{0, 1, 1}, {1, 2, 2}, {2, 0, 3}}), 6LL);
    CHECK_EQ(minClosedWalk(3, E{{0, 1, 1}, {1, 0, 1}, {0, 2, 1}, {2, 0, 1}}), 4LL);
    CHECK_EQ(minClosedWalk(2, E{{0, 1, 1}}), -1LL);
    CHECK_EQ(minClosedWalk(2, E{{1, 0, 1}}), -1LL);
    CHECK_EQ(minClosedWalk(1, E{}), 0LL);
    CHECK_EQ(minClosedWalk(2, E{{0, 1, 5}, {1, 0, 7}}), 12LL);
    CHECK_EQ(minClosedWalk(2, E{{0, 1, 5}, {0, 1, 3}, {1, 0, 7}, {0, 0, 1}}), 10LL);  // cheapest parallel edge
    // not metric: direct 0->2 costs 100 but 0->1->2 costs 2
    CHECK_EQ(minClosedWalk(3, E{{0, 1, 1}, {1, 2, 1}, {2, 0, 1}, {0, 2, 100}}), 3LL);
    // node 2 is unreachable from 0
    CHECK_EQ(minClosedWalk(3, E{{0, 1, 1}, {1, 0, 1}, {2, 0, 1}}), -1LL);
    // node 2 reachable but cannot return to 0
    CHECK_EQ(minClosedWalk(3, E{{0, 1, 1}, {1, 0, 1}, {1, 2, 1}}), -1LL);

    Rng rng{909};
    for (int trial = 0; trial < 300; trial++) {
        int n = 1 + (int)(rng.next() % 7);
        int m = (int)(rng.next() % (2 * n * n + 1));
        E edges;
        for (int i = 0; i < m; i++)
            edges.push_back({(int)(rng.next() % n), (int)(rng.next() % n), 1 + (int)(rng.next() % 20)});
        CHECK_EQ(minClosedWalk(n, edges), brute(n, edges));
    }

    // n = 18: ring of weight-1 edges plus heavier chords. Any closed walk through 18 nodes uses
    // >= 18 edges of weight >= 1, and the ring achieves exactly 18.
    {
        const int n = 18;
        E edges;
        for (int i = 0; i < n; i++) edges.push_back({i, (i + 1) % n, 1});
        for (int i = 0; i < 100; i++) {
            int u = (int)(rng.next() % n), v = (int)(rng.next() % n);
            edges.push_back({u, v, 2 + (int)(rng.next() % 99)});
        }
        CHECK_EQ(minClosedWalk(n, edges), 18LL);
    }

    return check::report();
}
