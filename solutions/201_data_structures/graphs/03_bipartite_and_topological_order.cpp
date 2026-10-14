// Problem: Two-coloring and dependency ordering. Vertices are 0..n-1; the edge list may hold duplicates.
//   bool isBipartite(int n, const Edges& edges)             undirected; the graph may be disconnected
//   std::vector<int> topoOrder(int n, const Edges& edges)   directed, edge (u, v) means u must come before v.
//        Return the LEXICOGRAPHICALLY SMALLEST valid ordering of all n vertices, or an empty vector if the graph
//        has a cycle (a self-loop is a cycle). n == 0 returns an empty vector.
//   int minLayers(int n, const Edges& edges)                directed. Group vertices into layers so every edge
//        goes from an earlier layer to a strictly later one; return the minimum number of layers, or -1 if the
//        graph has a cycle. n == 0 returns 0.
//
// Examples:
//   isBipartite: path -> true, triangle -> false, a self-loop -> false, two components where one is a triangle -> false
//   topoOrder(4, {(2,0),(3,1)})            -> {2, 0, 3, 1}
//   topoOrder(5, {(4,0),(1,2)})            -> {1, 2, 3, 4, 0}
//   topoOrder(2, {(0,1),(1,0)})            -> {}
//   minLayers(4, {(0,1),(0,2),(1,3),(2,3)}) -> 3
//
// Constraints: n up to 500,000 and ~1,000,000 edges for isBipartite; topoOrder/minLayers up to n = 200,000 and 500,000 edges.
// Required complexity: O((n + E) log n) for topoOrder, O(n + E) for the other two.
// Run: make run FILE=curriculum/201_data_structures/graphs/03_bipartite_and_topological_order.cpp
//
// Think first:
//  1. For bipartite, what is the state you propagate along an edge, and what single observation proves the
//     answer is false? What do you do about vertices not reachable from vertex 0?
//  2. For topoOrder, what data structure always hands you the smallest currently-available vertex, and what
//     does "available" mean in terms of a counter per vertex? How does the same counter detect a cycle?
//  3. minLayers: is it related to topoOrder? What does a vertex's layer equal in terms of its predecessors?

#include "check.h"

#include <algorithm>
#include <functional>
#include <numeric>
#include <queue>
#include <random>
#include <utility>
#include <vector>

using Edges = std::vector<std::pair<int, int>>;

// ===== YOUR CODE BELOW (stub) =====
bool isBipartite(int n, const Edges& edges) {
    std::vector<std::vector<int>> adj(n);
    for (auto [u, v] : edges) { if (u == v) return false; adj[u].push_back(v); adj[v].push_back(u); }
    std::vector<int> col(n, -1);
    for (int s = 0; s < n; s++) {
        if (col[s] != -1) continue;
        col[s] = 0; std::queue<int> q; q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (col[v] == -1) { col[v] = col[u] ^ 1; q.push(v); }
                else if (col[v] == col[u]) return false;
            }
        }
    }
    return true;
}
std::vector<int> topoOrder(int n, const Edges& edges) {
    std::vector<std::vector<int>> adj(n); std::vector<int> indeg(n, 0);
    for (auto [u, v] : edges) { adj[u].push_back(v); indeg[v]++; }
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
    for (int i = 0; i < n; i++) if (!indeg[i]) pq.push(i);
    std::vector<int> out;
    while (!pq.empty()) {
        int u = pq.top(); pq.pop(); out.push_back(u);
        for (int v : adj[u]) if (--indeg[v] == 0) pq.push(v);
    }
    if ((int)out.size() != n) return {};
    return out;
}
int minLayers(int n, const Edges& edges) {
    std::vector<std::vector<int>> adj(n); std::vector<int> indeg(n, 0), layer(n, 1);
    for (auto [u, v] : edges) { adj[u].push_back(v); indeg[v]++; }
    std::queue<int> q;
    for (int i = 0; i < n; i++) if (!indeg[i]) q.push(i);
    int seen = 0, best = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop(); seen++; best = std::max(best, layer[u]);
        for (int v : adj[u]) { layer[v] = std::max(layer[v], layer[u] + 1); if (--indeg[v] == 0) q.push(v); }
    }
    return seen == n ? best : -1;
}
// ===== TESTS (do not edit below) =====
using V = std::vector<int>;

static V iotaV(int n) {
    V v(n);
    std::iota(v.begin(), v.end(), 0);
    return v;
}

int main() {
    // isBipartite
    CHECK(isBipartite(0, {}));
    CHECK(isBipartite(1, {}));
    CHECK(isBipartite(3, {}));
    CHECK(isBipartite(2, {{0, 1}}));
    CHECK(isBipartite(4, {{0, 1}, {1, 2}, {2, 3}}));
    CHECK(!isBipartite(3, {{0, 1}, {1, 2}, {2, 0}}));
    CHECK(isBipartite(4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}}));
    CHECK(!isBipartite(5, {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}}));
    CHECK(!isBipartite(1, {{0, 0}}));                                  // self-loop
    CHECK(isBipartite(2, {{0, 1}, {0, 1}, {1, 0}}));                   // duplicates are harmless
    CHECK(!isBipartite(7, {{0, 1}, {2, 3}, {3, 4}, {4, 2}, {5, 6}}));  // odd cycle hides in a later component
    CHECK(isBipartite(6, {{0, 3}, {1, 4}, {2, 5}}));                   // several components
    CHECK(isBipartite(6, {{0, 1}, {0, 2}, {0, 3}, {4, 1}, {4, 2}, {4, 3}}));  // K(2,3)
    CHECK(!isBipartite(4, {{0, 1}, {0, 2}, {0, 3}, {1, 2}}));                 // triangle with a pendant

    // topoOrder
    CHECK_EQ(topoOrder(0, {}), V{});
    CHECK_EQ(topoOrder(3, {}), (V{0, 1, 2}));
    CHECK_EQ(topoOrder(4, {{2, 0}, {3, 1}}), (V{2, 0, 3, 1}));
    CHECK_EQ(topoOrder(5, {{4, 0}, {1, 2}}), (V{1, 2, 3, 4, 0}));
    CHECK_EQ(topoOrder(3, {{1, 0}, {2, 1}}), (V{2, 1, 0}));
    CHECK_EQ(topoOrder(4, {{0, 1}, {0, 2}, {1, 3}, {2, 3}}), (V{0, 1, 2, 3}));
    CHECK_EQ(topoOrder(2, {{1, 0}, {1, 0}}), (V{1, 0}));  // duplicate edges: in-degree counts both
    CHECK_EQ(topoOrder(2, {{0, 1}, {1, 0}}), V{});
    CHECK_EQ(topoOrder(1, {{0, 0}}), V{});
    CHECK_EQ(topoOrder(4, {{0, 1}, {1, 2}, {2, 1}, {3, 0}}), V{});  // cycle not touching the first source
    // only 2 is a source; after it, 0 and 3 are both available and the smaller goes first
    CHECK_EQ(topoOrder(4, {{2, 0}, {3, 1}, {2, 3}}), (V{2, 0, 3, 1}));

    // minLayers
    CHECK_EQ(minLayers(0, {}), 0);
    CHECK_EQ(minLayers(3, {}), 1);
    CHECK_EQ(minLayers(3, {{0, 1}, {1, 2}}), 3);
    CHECK_EQ(minLayers(4, {{0, 1}, {0, 2}, {1, 3}, {2, 3}}), 3);
    CHECK_EQ(minLayers(3, {{0, 1}, {1, 0}}), -1);
    CHECK_EQ(minLayers(1, {{0, 0}}), -1);
    CHECK_EQ(minLayers(4, {{0, 2}, {1, 2}, {2, 3}}), 3);
    CHECK_EQ(minLayers(6, {{0, 1}, {2, 3}, {3, 4}, {4, 5}}), 4);  // longest chain decides
    CHECK_EQ(minLayers(5, {{0, 4}, {0, 1}, {1, 2}, {2, 4}}), 4);  // long path beats the shortcut 0 -> 4

    // large
    const int n = 200000;
    {
        // no edges: identity order; a scan-for-smallest implementation is O(n^2)
        CHECK_EQ(topoOrder(n, {}), iotaV(n));
        CHECK_EQ(minLayers(n, {}), 1);

        // random forward edges only (u < v): the smallest topological order is the identity
        std::mt19937 rng(99);
        Edges edges;
        for (int i = 0; i < 300000; i++) {
            int a = (int)(rng() % n), b = (int)(rng() % n);
            if (a == b) continue;
            edges.push_back({std::min(a, b), std::max(a, b)});
        }
        CHECK_EQ(topoOrder(n, edges), iotaV(n));
        // plus the chain 0 -> 1 -> ... -> n-1: the DAG has exactly one layer per vertex
        for (int i = 0; i + 1 < n; i++) edges.push_back({i, i + 1});
        CHECK_EQ(minLayers(n, edges), n);
        CHECK_EQ(topoOrder(n, edges), iotaV(n));
        // close the chain into a cycle
        edges.push_back({n - 1, 0});
        CHECK_EQ(topoOrder(n, edges), V{});
        CHECK_EQ(minLayers(n, edges), -1);

        // reversed chain n-1 -> n-2 -> ... -> 0 forces the reverse order
        Edges rev;
        for (int i = n - 1; i > 0; i--) rev.push_back({i, i - 1});
        V expect(n);
        for (int i = 0; i < n; i++) expect[i] = n - 1 - i;
        CHECK_EQ(topoOrder(n, rev), expect);
    }
    {
        // bipartite on big graphs
        const int m = 500000;
        Edges path;
        for (int i = 0; i + 1 < m; i++) path.push_back({i, i + 1});
        CHECK(isBipartite(m, path));
        Edges evenCycle = path;
        evenCycle.push_back({m - 1, 0});  // cycle of length 500,000 (even)
        CHECK(isBipartite(m, evenCycle));
        Edges oddCycle;
        for (int i = 0; i + 1 < m - 1; i++) oddCycle.push_back({i, i + 1});
        oddCycle.push_back({m - 2, 0});  // cycle of length 499,999 (odd); vertex m-1 isolated
        CHECK(!isBipartite(m, oddCycle));

        // 700 x 700 grid graph is bipartite; one diagonal edge makes a triangle
        const int S = 700;
        Edges grid;
        for (int r = 0; r < S; r++)
            for (int c = 0; c < S; c++) {
                if (c + 1 < S) grid.push_back({r * S + c, r * S + c + 1});
                if (r + 1 < S) grid.push_back({r * S + c, (r + 1) * S + c});
            }
        CHECK(isBipartite(S * S, grid));
        grid.push_back({500 * S + 500, 501 * S + 501});
        CHECK(!isBipartite(S * S, grid));
    }
    return check::report();
}
