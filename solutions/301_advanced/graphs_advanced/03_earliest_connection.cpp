// 301 advanced > graphs_advanced > 03 Earliest connection time (offline connectivity)
//
// Problem:
//   An undirected network has n nodes. Link i joins nodes u and v and comes up at
//   time t (links never go down). For each query (a, b) return the earliest time T
//   such that a and b are connected using only links with t <= T.
//   Return 0 if a == b, and -1 if they are never connected.
//
// Examples:
//   n=3, links {(0,1,t=1),(1,2,t=2),(0,2,t=3)}, query (0,2)  ->  2  (via node 1)
//   n=3, links {(0,1,t=2)}, queries (1,0), (0,2)             ->  2, -1
//
// Constraints:
//   1 <= n, links, queries <= 200000; 1 <= t <= 1e9. Links arrive UNSORTED and
//   may repeat pairs or form loops. All queries are known up front (offline).
//
// Required complexity: O((m + q) log n) total (sorting links is fine).
//   Rebuilding connectivity per query, or per query binary search with a fresh
//   union-find over all links, is too slow.
//
// Assumptions: node ids are valid; an answer is a time value taken from a link.
//
// Think first:
//   1. The answer is the smallest possible "largest link time on a path" between
//      the two nodes. Which classic structure captures that?
//   2. Union-find with path compression forgets WHEN merges happened. What do you
//      keep instead, and what do you give up (and what keeps depth O(log n))?
//   3. Ties: several links with equal t. Does it affect correctness?
//
// BEFORE CODING: write your approach, invariant and complexity in a comment
// block right here.
//
// Run: make run FILE=curriculum/301_advanced/graphs_advanced/03_earliest_connection.cpp

#include "check.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <numeric>
#include <utility>
#include <vector>

// ===== YOUR CODE BELOW =====
std::vector<int> earliestConnection(int n, const std::vector<std::array<int, 3>>& links,
                                    const std::vector<std::pair<int, int>>& queries) {
    std::vector<std::array<int, 3>> ls = links;
    std::sort(ls.begin(), ls.end(), [](auto& a, auto& b) { return a[2] < b[2]; });
    const int INF = INT32_MAX;
    std::vector<int> par(n), sz(n, 1), tm(n, INF);
    std::iota(par.begin(), par.end(), 0);
    auto find = [&](int x) {
        while (par[x] != x) x = par[x];
        return x;
    };
    for (auto& l : ls) {
        int a = find(l[0]), b = find(l[1]);
        if (a == b) continue;
        if (sz[a] < sz[b]) std::swap(a, b);
        par[b] = a;
        sz[a] += sz[b];
        tm[b] = l[2];
    }
    std::vector<int> out;
    for (auto [a, b] : queries) {
        if (a == b) { out.push_back(0); continue; }
        int ans = 0;
        int u = a, v = b;
        bool ok = true;
        while (u != v) {
            if (tm[u] == INF && tm[v] == INF) { ok = false; break; }
            if (tm[u] <= tm[v]) { ans = std::max(ans, tm[u]); u = par[u]; }
            else { ans = std::max(ans, tm[v]); v = par[v]; }
        }
        out.push_back(ok ? ans : -1);
    }
    return out;
}
// ===== TESTS (do not edit) =====
struct Rng {
    uint64_t s;
    uint64_t next() {
        s = s * 6364136223846793005ULL + 1442695040888963407ULL;
        return s >> 33;
    }
};

// For each distinct time in increasing order, compute connectivity by DFS over links with t <= T.
static std::vector<int> brute(int n, const std::vector<std::array<int, 3>>& links,
                              const std::vector<std::pair<int, int>>& queries) {
    std::vector<int> times;
    for (auto& l : links) times.push_back(l[2]);
    std::sort(times.begin(), times.end());
    times.erase(std::unique(times.begin(), times.end()), times.end());
    std::vector<int> ans(queries.size(), -1);
    for (size_t qi = 0; qi < queries.size(); qi++) {
        auto [a, b] = queries[qi];
        if (a == b) { ans[qi] = 0; continue; }
        for (int T : times) {
            std::vector<char> seen(n, 0);
            std::vector<int> st{a};
            seen[a] = 1;
            while (!st.empty()) {
                int u = st.back();
                st.pop_back();
                for (auto& l : links) {
                    if (l[2] > T) continue;
                    int v = -1;
                    if (l[0] == u) v = l[1];
                    else if (l[1] == u) v = l[0];
                    if (v >= 0 && !seen[v]) { seen[v] = 1; st.push_back(v); }
                }
            }
            if (seen[b]) { ans[qi] = T; break; }
        }
    }
    return ans;
}

int main() {
    using L = std::vector<std::array<int, 3>>;
    using Q = std::vector<std::pair<int, int>>;

    CHECK_EQ(earliestConnection(3, L{{0, 1, 1}, {1, 2, 2}, {0, 2, 3}}, Q{{0, 2}}), std::vector<int>{2});
    CHECK_EQ(earliestConnection(3, L{{0, 1, 2}}, Q{{1, 0}, {0, 2}}), std::vector<int>{2, -1});

    // unsorted links
    CHECK_EQ(earliestConnection(5, L{{0, 1, 5}, {1, 2, 3}, {3, 4, 1}, {2, 3, 10}},
                                Q{{0, 2}, {0, 3}, {3, 4}, {0, 0}, {0, 4}, {1, 2}, {4, 0}}),
             std::vector<int>{5, 10, 1, 0, 10, 3, 10});

    // repeated pair: the earlier link wins
    CHECK_EQ(earliestConnection(2, L{{0, 1, 9}, {0, 1, 4}}, Q{{0, 1}}), std::vector<int>{4});
    // self-loop link is irrelevant
    CHECK_EQ(earliestConnection(2, L{{0, 0, 1}}, Q{{0, 1}, {1, 1}}), std::vector<int>{-1, 0});
    // no links, no queries
    CHECK_EQ(earliestConnection(1, L{}, Q{}), std::vector<int>{});
    CHECK_EQ(earliestConnection(1, L{}, Q{{0, 0}}), std::vector<int>{0});
    // equal times
    CHECK_EQ(earliestConnection(4, L{{0, 1, 7}, {1, 2, 7}, {2, 3, 7}}, Q{{0, 3}, {3, 1}}),
             std::vector<int>{7, 7});
    // a long chain of decreasing times: the answer is the max on the path, not the last link
    CHECK_EQ(earliestConnection(4, L{{0, 1, 30}, {1, 2, 20}, {2, 3, 10}}, Q{{0, 3}, {2, 3}, {1, 3}}),
             std::vector<int>{30, 10, 20});

    // random small instances, many ties
    Rng rng{2024};
    for (int trial = 0; trial < 300; trial++) {
        int n = 1 + (int)(rng.next() % 8);
        int m = (int)(rng.next() % 11);
        L links;
        for (int i = 0; i < m; i++)
            links.push_back({(int)(rng.next() % n), (int)(rng.next() % n), 1 + (int)(rng.next() % 6)});
        Q queries;
        for (int a = 0; a < n; a++)
            for (int b = 0; b < n; b++) queries.push_back({a, b});
        CHECK_EQ(earliestConnection(n, links, queries), brute(n, links, queries));
    }

    // large: chain i--(i+1) with time i+1 (shuffled), plus redundant late links.
    // For a < b the answer is b (max time on the chain path a..b), regardless of redundant links
    // whose time is >= n.
    {
        const int n = 200000;
        L links;
        for (int i = 0; i + 1 < n; i++) links.push_back({i, i + 1, i + 1});
        for (int i = 0; i < 100000; i++)
            links.push_back({(int)(rng.next() % n), (int)(rng.next() % n), n + 1 + (int)(rng.next() % 1000000)});
        for (int i = (int)links.size() - 1; i > 0; i--) std::swap(links[i], links[rng.next() % (i + 1)]);
        Q queries;
        std::vector<int> expected;
        for (int i = 0; i < 200000; i++) {
            int a = (int)(rng.next() % n), b = (int)(rng.next() % n);
            queries.push_back({a, b});
            expected.push_back(a == b ? 0 : std::max(a, b));
        }
        CHECK(earliestConnection(n, links, queries) == expected);
    }

    return check::report();
}
