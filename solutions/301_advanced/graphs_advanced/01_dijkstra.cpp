// 301 advanced > graphs_advanced > 01 Dijkstra (canonical form)
//
// Problem:
//   A directed graph has n nodes (0..n-1) and non-negative integer edge weights.
//   Return the shortest distance from `src` to every node, or -1 for nodes that
//   cannot be reached.
//
// Examples:
//   n=3, edges {0->1 w4, 0->2 w1, 2->1 w2}, src=0   ->  {0, 3, 1}
//   n=2, edges {1->0 w5}, src=0                      ->  {0, -1}
//
// Constraints:
//   1 <= n <= 200000, 0 <= m <= 600000, 0 <= w <= 2'000'000'000 (fits in int).
//   Path lengths can exceed 32 bits. Parallel edges and self-loops are allowed.
//
// Required complexity: O((n + m) log n) time, O(n + m) space.
//
// Assumptions: edges are given as {from, to, weight}; src is a valid node.
//
// Hint: a priority queue holds (distance, node); what do you do with entries
//       that are popped after a better distance was already finalized?
//
// Think first:
//   1. Why does Dijkstra break with a negative edge? State the exact invariant
//      that fails.
//   2. Zero-weight edges: does your loop still terminate and stay correct?
//   3. Lazy deletion vs decrease-key: what does each cost in C++?
//
// BEFORE CODING: write your approach, loop invariant and complexity in a comment
// block right here.
//
// Run: make run FILE=curriculum/301_advanced/graphs_advanced/01_dijkstra.cpp

#include "check.h"

#include <array>
#include <climits>
#include <cstdint>
#include <queue>
#include <utility>
#include <vector>

// ===== YOUR CODE BELOW =====
std::vector<long long> dijkstra(int n, const std::vector<std::array<int, 3>>& edges, int src) {
    std::vector<std::vector<std::pair<int, int>>> adj(n);
    for (auto& e : edges) adj[e[0]].push_back({e[1], e[2]});
    const long long INF = LLONG_MAX;
    std::vector<long long> d(n, INF);
    using P = std::pair<long long, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    d[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [du, u] = pq.top();
        pq.pop();
        if (du > d[u]) continue;
        for (auto [v, w] : adj[u])
            if (du + w < d[v]) {
                d[v] = du + w;
                pq.push({d[v], v});
            }
    }
    for (auto& x : d)
        if (x == INF) x = -1;
    return d;
}
// ===== TESTS (do not edit) =====
struct Rng {
    uint64_t s;
    uint64_t next() {
        s = s * 6364136223846793005ULL + 1442695040888963407ULL;
        return s >> 33;
    }
};

static std::vector<long long> bellmanFord(int n, const std::vector<std::array<int, 3>>& edges, int src) {
    const long long INF = LLONG_MAX / 4;
    std::vector<long long> d(n, INF);
    d[src] = 0;
    for (int it = 0; it < n; it++)
        for (auto& e : edges)
            if (d[e[0]] < INF && d[e[0]] + e[2] < d[e[1]]) d[e[1]] = d[e[0]] + e[2];
    for (auto& x : d)
        if (x >= INF) x = -1;
    return d;
}

int main() {
    using E = std::vector<std::array<int, 3>>;

    CHECK_EQ(dijkstra(3, E{{0, 1, 4}, {0, 2, 1}, {2, 1, 2}}, 0), std::vector<long long>{0, 3, 1});
    CHECK_EQ(dijkstra(2, E{{1, 0, 5}}, 0), std::vector<long long>{0, -1});
    CHECK_EQ(dijkstra(1, E{}, 0), std::vector<long long>{0});

    // a node unreachable, others reachable through a longer-looking route
    CHECK_EQ(dijkstra(5, E{{0, 1, 4}, {0, 2, 1}, {2, 1, 2}, {1, 3, 1}, {2, 3, 5}}, 0),
             std::vector<long long>{0, 3, 1, 4, -1});

    // zero weights, parallel edges, self-loop
    CHECK_EQ(dijkstra(3, E{{0, 1, 0}, {1, 2, 0}}, 0), std::vector<long long>{0, 0, 0});
    CHECK_EQ(dijkstra(2, E{{0, 1, 5}, {0, 1, 2}, {0, 0, 3}}, 0), std::vector<long long>{0, 2});

    // source is not node 0; edges are directed
    CHECK_EQ(dijkstra(3, E{{0, 1, 1}, {1, 2, 1}}, 2), std::vector<long long>{-1, -1, 0});
    CHECK_EQ(dijkstra(3, E{{0, 1, 1}, {1, 2, 1}}, 1), std::vector<long long>{-1, 0, 1});

    // overflow of 32-bit: two 2e9 edges
    CHECK_EQ(dijkstra(3, E{{0, 1, 2000000000}, {1, 2, 2000000000}}, 0),
             std::vector<long long>{0, 2000000000LL, 4000000000LL});

    // random graphs against Bellman-Ford
    Rng rng{12345};
    for (int trial = 0; trial < 300; trial++) {
        int n = 1 + (int)(rng.next() % 8);
        int m = (int)(rng.next() % 20);
        E edges;
        for (int i = 0; i < m; i++)
            edges.push_back({(int)(rng.next() % n), (int)(rng.next() % n), (int)(rng.next() % 11)});
        int src = (int)(rng.next() % n);
        CHECK_EQ(dijkstra(n, edges, src), bellmanFord(n, edges, src));
    }

    // large: dist[i] == i by construction (see comment). Brute force Bellman-Ford is far too slow.
    {
        const int n = 200000;
        E edges;
        for (int i = 0; i + 1 < n; i++) edges.push_back({i, i + 1, 1});
        for (int i = 0; i < 400000; i++) {
            int u = (int)(rng.next() % n), v = (int)(rng.next() % n);
            int w = v > u ? (v - u) + (int)(rng.next() % 1000) : (int)(rng.next() % 5);
            edges.push_back({u, v, w});
        }
        std::vector<long long> expected(n);
        for (int i = 0; i < n; i++) expected[i] = i;
        // forward edges cost >= distance advanced, backward edges cost >= 0 and go back,
        // so no path to i is shorter than i; the chain achieves exactly i.
        CHECK(dijkstra(n, edges, 0) == expected);
    }

    return check::report();
}
