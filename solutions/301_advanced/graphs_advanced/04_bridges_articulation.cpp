// 301 advanced > graphs_advanced > 04 Bridges and articulation points
//
// Problem:
//   An undirected multigraph has n nodes (0..n-1) and an indexed edge list.
//   (a) bridges: return the indices (into `edges`) of every edge whose removal
//       increases the number of connected components, sorted ascending.
//   (b) articulationPoints: return every node whose removal (with its edges)
//       increases the number of connected components among the REMAINING nodes,
//       sorted ascending.
//
// Examples:
//   n=4, edges {(0,1),(1,2),(2,0),(2,3)}:  bridges -> {3};  articulation points -> {2}
//   n=2, edges {(0,1),(0,1)}  (parallel):  bridges -> {};   articulation points -> {}
//   n=3, edges {(0,1),(1,2)}:              bridges -> {0,1}; articulation points -> {1}
//
// Constraints:
//   1 <= n <= 20000 in the tests (the algorithm should handle 1e5+; deep
//   recursion on a path graph can overflow the stack - an iterative DFS is the
//   stretch goal). The graph may be disconnected and may contain parallel edges
//   and self-loops (a self-loop is never a bridge).
//
// Required complexity: O(n + m) time and space for each function. Removing each
//   edge/node and recounting components (O(m * (n + m))) is too slow here.
//
// Assumptions: node ids valid; an isolated node is never an articulation point.
//
// Think first:
//   1. Define discovery time and low-link precisely. What does `low[v] > tin[u]`
//      say about the subtree of v, and why is the test different for articulation
//      points (>=) than for bridges (>)?
//   2. Parallel edges: if you skip "the parent" by node id you get the wrong
//      answer. What do you skip instead?
//   3. The DFS root is special for articulation points. Why, and what exactly is
//      the root condition?
//
// BEFORE CODING: write your approach, invariant and complexity in a comment
// block right here.
//
// Run: make run FILE=curriculum/301_advanced/graphs_advanced/04_bridges_articulation.cpp

#include "check.h"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <utility>
#include <vector>

// ===== YOUR CODE BELOW =====
static void lowlink(int n, const std::vector<std::pair<int, int>>& edges, std::vector<int>& brs,
                    std::vector<int>& aps) {
    std::vector<std::vector<std::pair<int, int>>> adj(n);
    for (int i = 0; i < (int)edges.size(); i++) {
        auto [u, v] = edges[i];
        if (u == v) continue;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }
    std::vector<int> tin(n, -1), low(n, 0);
    std::vector<char> isAp(n, 0);
    int timer = 0;
    std::function<void(int, int)> dfs = [&](int u, int pe) {
        tin[u] = low[u] = timer++;
        int children = 0;
        for (auto [v, id] : adj[u]) {
            if (id == pe) continue;
            if (tin[v] != -1) {
                low[u] = std::min(low[u], tin[v]);
            } else {
                dfs(v, id);
                low[u] = std::min(low[u], low[v]);
                if (low[v] > tin[u]) brs.push_back(id);
                if (low[v] >= tin[u] && pe != -1) isAp[u] = 1;
                children++;
            }
        }
        if (pe == -1 && children > 1) isAp[u] = 1;
    };
    for (int u = 0; u < n; u++)
        if (tin[u] == -1) dfs(u, -1);
    std::sort(brs.begin(), brs.end());
    for (int u = 0; u < n; u++)
        if (isAp[u]) aps.push_back(u);
}

std::vector<int> bridges(int n, const std::vector<std::pair<int, int>>& edges) {
    std::vector<int> b, a;
    lowlink(n, edges, b, a);
    return b;
}

std::vector<int> articulationPoints(int n, const std::vector<std::pair<int, int>>& edges) {
    std::vector<int> b, a;
    lowlink(n, edges, b, a);
    return a;
}
// ===== TESTS (do not edit) =====
struct Rng {
    uint64_t s;
    uint64_t next() {
        s = s * 6364136223846793005ULL + 1442695040888963407ULL;
        return s >> 33;
    }
};

using Ed = std::vector<std::pair<int, int>>;

// Component count of the graph ignoring one edge index and/or one vertex (-1 = none).
static int comps(int n, const Ed& edges, int skipEdge, int skipVertex) {
    std::vector<int> p(n);
    std::iota(p.begin(), p.end(), 0);
    std::function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
    for (int i = 0; i < (int)edges.size(); i++) {
        if (i == skipEdge) continue;
        auto [a, b] = edges[i];
        if (a == skipVertex || b == skipVertex) continue;
        p[f(a)] = f(b);
    }
    int c = 0;
    for (int v = 0; v < n; v++)
        if (v != skipVertex && f(v) == v) c++;
    return c;
}

static std::vector<int> bruteBridges(int n, const Ed& edges) {
    std::vector<int> r;
    int base = comps(n, edges, -1, -1);
    for (int i = 0; i < (int)edges.size(); i++)
        if (comps(n, edges, i, -1) > base) r.push_back(i);
    return r;
}

static std::vector<int> bruteAP(int n, const Ed& edges) {
    std::vector<int> r;
    int base = comps(n, edges, -1, -1);
    for (int v = 0; v < n; v++) {
        // an isolated vertex disappears with its own component, which is not an increase
        bool isolated = true;
        for (auto [a, b] : edges)
            if ((a == v || b == v) && a != b) isolated = false;
        int after = comps(n, edges, -1, v);
        if (after > base - (isolated ? 1 : 0)) r.push_back(v);
    }
    return r;
}

int main() {
    using V = std::vector<int>;

    {
        Ed e{{0, 1}, {1, 2}, {2, 0}, {2, 3}};
        CHECK_EQ(bridges(4, e), V{3});
        CHECK_EQ(articulationPoints(4, e), V{2});
    }
    {
        Ed e{{0, 1}, {0, 1}};  // parallel edges: neither is a bridge
        CHECK_EQ(bridges(2, e), V{});
        CHECK_EQ(articulationPoints(2, e), V{});
    }
    {
        Ed e{{0, 1}, {0, 1}, {1, 2}};
        CHECK_EQ(bridges(3, e), V{2});
        CHECK_EQ(articulationPoints(3, e), V{1});
    }
    {
        Ed e{{0, 1}, {1, 2}};
        CHECK_EQ(bridges(3, e), V({0, 1}));
        CHECK_EQ(articulationPoints(3, e), V{1});
    }
    {
        Ed e{{0, 0}, {0, 1}};  // self-loop
        CHECK_EQ(bridges(2, e), V{1});
        CHECK_EQ(articulationPoints(2, e), V{});
    }
    {
        Ed e{{0, 1}, {2, 3}, {3, 4}};  // disconnected
        CHECK_EQ(bridges(5, e), V({0, 1, 2}));
        CHECK_EQ(articulationPoints(5, e), V{3});
    }
    {
        Ed e{{0, 1}, {0, 2}, {0, 3}, {0, 4}};  // star
        CHECK_EQ(bridges(5, e), V({0, 1, 2, 3}));
        CHECK_EQ(articulationPoints(5, e), V{0});
    }
    {
        Ed e{{0, 1}, {1, 2}, {2, 0}, {2, 3}, {3, 4}, {4, 5}, {5, 3}};  // two triangles joined by a bridge
        CHECK_EQ(bridges(6, e), V{3});
        CHECK_EQ(articulationPoints(6, e), V({2, 3}));
    }
    CHECK_EQ(bridges(3, Ed{}), V{});
    CHECK_EQ(articulationPoints(3, Ed{}), V{});
    {
        Ed e{{0, 1}, {1, 2}, {2, 3}, {3, 0}};  // 4-cycle
        CHECK_EQ(bridges(4, e), V{});
        CHECK_EQ(articulationPoints(4, e), V{});
    }
    {
        // two cycles sharing exactly one vertex (a "bowtie"): that vertex is the cut vertex, no bridges
        Ed e{{0, 1}, {1, 2}, {2, 0}, {2, 3}, {3, 4}, {4, 2}};
        CHECK_EQ(bridges(5, e), V{});
        CHECK_EQ(articulationPoints(5, e), V{2});
    }

    // random small multigraphs against remove-and-recount
    Rng rng{99};
    for (int trial = 0; trial < 500; trial++) {
        int n = 1 + (int)(rng.next() % 8);
        int m = (int)(rng.next() % 11);
        Ed e;
        for (int i = 0; i < m; i++) e.push_back({(int)(rng.next() % n), (int)(rng.next() % n)});
        CHECK_EQ(bridges(n, e), bruteBridges(n, e));
        CHECK_EQ(articulationPoints(n, e), bruteAP(n, e));
    }

    // larger: a path (every edge a bridge, every inner node a cut vertex) hanging off a cycle
    {
        const int cyc = 5000, path = 15000;
        const int n = cyc + path;
        Ed e;
        for (int i = 0; i < cyc; i++) e.push_back({i, (i + 1) % cyc});          // edges 0..cyc-1
        e.push_back({0, cyc});                                                   // edge cyc
        for (int i = 0; i + 1 < path; i++) e.push_back({cyc + i, cyc + i + 1});  // path edges
        V expectedBridges(path);  // edge `cyc` plus path-1 path edges
        std::iota(expectedBridges.begin(), expectedBridges.end(), cyc);
        CHECK(bridges(n, e) == expectedBridges);
        V expectedAP;
        expectedAP.push_back(0);  // cycle node 0 joins the cycle to the path
        for (int i = 0; i + 1 < path; i++) expectedAP.push_back(cyc + i);  // all path nodes except the last
        CHECK(articulationPoints(n, e) == expectedAP);
    }

    return check::report();
}
