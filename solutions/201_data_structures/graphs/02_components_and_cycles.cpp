// Problem: Connected components and cycle detection on graphs given as vertex count + edge list
//   (vertices are 0..n-1; the edge list may contain duplicates and self-loops).
//   int  countComponents(int n, const std::vector<std::pair<int,int>>& edges)     undirected
//   bool hasCycleUndirected(int n, const std::vector<std::pair<int,int>>& edges)  undirected
//   bool hasCycleDirected(int n, const std::vector<std::pair<int,int>>& edges)    edge (u, v) means u -> v
//
// Rules:
//   * Isolated vertices are components of size 1; n == 0 has 0 components.
//   * Undirected: a self-loop (v, v) IS a cycle; two parallel edges between the same pair ARE a cycle
//     (a walk u-v-u over the SAME edge is not).
//   * Directed: a self-loop is a cycle; a diamond (0->1, 0->2, 1->3, 2->3) is NOT a cycle.
//
// Examples:
//   n=5, edges {(0,1),(1,2),(3,4)}                     -> 2 components
//   undirected {(0,1),(1,2),(2,0)} -> true      {(0,1),(1,2)} -> false      {(0,1),(0,1)} -> true
//   directed   {(0,1),(1,0)} -> true            diamond -> false
//
// Constraints: n and the edge count up to 1,000,000; the graph can be one path 1,000,000 vertices long.
// Required complexity: O(n + E) time and space (inverse-Ackermann factors are fine).
// Run: make run FILE=curriculum/201_data_structures/graphs/02_components_and_cycles.cpp
//
// Think first:
//  1. In an undirected DFS, how do you tell "I came from my parent" from "I found a real back edge",
//     especially when parallel edges exist? Is there an approach that needs no DFS for the undirected cases?
//  2. A directed cycle needs more than a visited bit. What are the states a vertex can be in, and which
//     edge into which state means "cycle"?
//  3. A 1,000,000-long path breaks recursive DFS. What is your plan for the call stack?

#include "check.h"

#include <numeric>
#include <utility>
#include <vector>

using Edges = std::vector<std::pair<int, int>>;

// ===== YOUR CODE BELOW (stub) =====
struct DSU {
    std::vector<int> p;
    explicit DSU(int n) : p(n) { std::iota(p.begin(), p.end(), 0); }
    int find(int x) { while (p[x] != x) { p[x] = p[p[x]]; x = p[x]; } return x; }
    bool unite(int a, int b) { a = find(a); b = find(b); if (a == b) return false; p[a] = b; return true; }
};
int countComponents(int n, const Edges& edges) {
    DSU d(n); int comps = n;
    for (auto [u, v] : edges) if (d.unite(u, v)) comps--;
    return comps;
}
bool hasCycleUndirected(int n, const Edges& edges) {
    DSU d(n);
    for (auto [u, v] : edges) if (!d.unite(u, v)) return true;
    return false;
}
bool hasCycleDirected(int n, const Edges& edges) {
    std::vector<std::vector<int>> adj(n);
    for (auto [u, v] : edges) adj[u].push_back(v);
    std::vector<int> color(n, 0), it(n, 0);
    for (int s = 0; s < n; s++) {
        if (color[s]) continue;
        std::vector<int> st{s}; color[s] = 1;
        while (!st.empty()) {
            int u = st.back();
            if (it[u] < (int)adj[u].size()) {
                int v = adj[u][it[u]++];
                if (color[v] == 1) return true;
                if (color[v] == 0) { color[v] = 1; st.push_back(v); }
            } else { color[u] = 2; st.pop_back(); }
        }
    }
    return false;
}
// ===== TESTS (do not edit below) =====
int main() {
    // components
    CHECK_EQ(countComponents(0, {}), 0);
    CHECK_EQ(countComponents(1, {}), 1);
    CHECK_EQ(countComponents(5, {}), 5);
    CHECK_EQ(countComponents(5, {{0, 1}, {1, 2}, {3, 4}}), 2);
    CHECK_EQ(countComponents(5, {{0, 1}, {1, 2}, {2, 3}, {3, 4}}), 1);
    CHECK_EQ(countComponents(4, {{0, 1}, {0, 1}, {2, 2}}), 3);  // duplicate and self-loop add nothing
    CHECK_EQ(countComponents(6, {{5, 0}, {4, 1}, {3, 2}}), 3);

    // undirected cycles
    CHECK(!hasCycleUndirected(0, {}));
    CHECK(!hasCycleUndirected(3, {}));
    CHECK(!hasCycleUndirected(3, {{0, 1}, {1, 2}}));
    CHECK(hasCycleUndirected(3, {{0, 1}, {1, 2}, {2, 0}}));
    CHECK(hasCycleUndirected(2, {{0, 1}, {0, 1}}));  // parallel edges
    CHECK(hasCycleUndirected(2, {{0, 1}, {1, 0}}));  // parallel edges, reversed
    CHECK(hasCycleUndirected(3, {{2, 2}}));          // self-loop
    CHECK(!hasCycleUndirected(7, {{0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}, {2, 6}}));  // a tree
    CHECK(hasCycleUndirected(7, {{0, 1}, {1, 2}, {3, 4}, {4, 5}, {5, 3}}));  // cycle in the second component
    CHECK(hasCycleUndirected(4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}}));          // 4-cycle

    // directed cycles
    CHECK(!hasCycleDirected(0, {}));
    CHECK(!hasCycleDirected(3, {}));
    CHECK(!hasCycleDirected(3, {{0, 1}, {1, 2}}));
    CHECK(hasCycleDirected(2, {{0, 1}, {1, 0}}));
    CHECK(hasCycleDirected(1, {{0, 0}}));
    CHECK(!hasCycleDirected(4, {{0, 1}, {0, 2}, {1, 3}, {2, 3}}));  // diamond: reaches 3 twice, no cycle
    CHECK(hasCycleDirected(3, {{0, 1}, {1, 2}, {2, 0}}));
    CHECK(!hasCycleDirected(3, {{0, 1}, {1, 2}, {0, 2}}));  // forward edge only
    CHECK(hasCycleDirected(6, {{0, 1}, {2, 3}, {3, 4}, {4, 2}}));  // cycle away from vertex 0
    CHECK(!hasCycleDirected(3, {{1, 0}, {1, 0}}));                 // duplicate directed edge is not a cycle

    // large: one path of 1,000,000 vertices (recursion depth 1e6 would crash)
    {
        const int n = 1000000;
        Edges path;
        path.reserve(n);
        for (int i = 0; i + 1 < n; i++) path.push_back({i, i + 1});
        CHECK_EQ(countComponents(n, path), 1);
        CHECK(!hasCycleUndirected(n, path));
        CHECK(!hasCycleDirected(n, path));
        Edges closed = path;
        closed.push_back({n - 1, 0});
        CHECK(hasCycleUndirected(n, closed));
        CHECK(hasCycleDirected(n, closed));
        Edges reversedClose = path;
        reversedClose.push_back({0, n - 1});  // forward chord: undirected cycle, directed DAG
        CHECK(hasCycleUndirected(n, reversedClose));
        CHECK(!hasCycleDirected(n, reversedClose));
        CHECK_EQ(countComponents(n, {}), n);
        // every other vertex joined to the next: n/2 components
        Edges pairs;
        for (int i = 0; i + 1 < n; i += 2) pairs.push_back({i, i + 1});
        CHECK_EQ(countComponents(n, pairs), n / 2);
        CHECK(!hasCycleUndirected(n, pairs));
    }
    return check::report();
}
