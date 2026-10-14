// 301 advanced > graphs_advanced > 02 Topological order with cycle detection
//
// Problem:
//   n tasks (0..n-1) and dependency pairs (a, b) meaning "a must come before b".
//   Return the LEXICOGRAPHICALLY SMALLEST valid ordering of all n tasks
//   (compare element by element, smaller task id first). If the dependencies
//   contain a cycle (including a self-dependency), return an empty vector.
//
// Examples:
//   n=4, deps {(3,1),(3,0),(1,2)}  ->  {3, 0, 1, 2}
//   n=3, deps {(2,0)}              ->  {1, 2, 0}
//   n=3, deps {(0,1),(1,2),(2,0)}  ->  {}   (cycle)
//
// Constraints:
//   1 <= n <= 200000, 0 <= deps <= 400000. Duplicate pairs may appear.
//
// Required complexity: O((n + m) log n) time, O(n + m) space.
//
// Assumptions: n >= 1, so an empty result unambiguously means "cycle".
//
// Think first:
//   1. How do you detect a cycle as a by-product of the ordering, without a
//      separate DFS with colors? What does "fewer than n nodes emitted" prove?
//   2. Why does a plain FIFO queue not give the lexicographically smallest order,
//      and what greedy argument justifies the structure you use instead?
//   3. A cycle that is unreachable from the "start" nodes - do you still catch it?
//
// BEFORE CODING: write your approach, invariant and complexity in a comment
// block right here.
//
// Run: make run FILE=curriculum/301_advanced/graphs_advanced/02_topo_sort_cycle.cpp

#include "check.h"

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <queue>
#include <utility>
#include <vector>

// ===== YOUR CODE BELOW =====
#pragma GCC diagnostic ignored "-Wunused-parameter"  // stub only; delete when you implement

std::vector<int> topoSmallest(int n, const std::vector<std::pair<int, int>>& deps) {
    // TODO
    return {-1};
}

// ===== TESTS (do not edit) =====
struct Rng {
    uint64_t s;
    uint64_t next() {
        s = s * 6364136223846793005ULL + 1442695040888963407ULL;
        return s >> 33;
    }
};

// Smallest valid permutation by trying permutations in lexicographic order.
static std::vector<int> brute(int n, const std::vector<std::pair<int, int>>& deps) {
    std::vector<int> p(n);
    std::iota(p.begin(), p.end(), 0);
    do {
        std::vector<int> pos(n);
        for (int i = 0; i < n; i++) pos[p[i]] = i;
        bool ok = true;
        for (auto [a, b] : deps)
            if (pos[a] >= pos[b]) ok = false;
        if (ok) return p;
    } while (std::next_permutation(p.begin(), p.end()));
    return {};
}

using D = std::vector<std::pair<int, int>>;

int main() {
    CHECK_EQ(topoSmallest(4, D{{3, 1}, {3, 0}, {1, 2}}), std::vector<int>{3, 0, 1, 2});
    CHECK_EQ(topoSmallest(3, D{{2, 0}}), std::vector<int>{1, 2, 0});
    CHECK_EQ(topoSmallest(3, D{}), std::vector<int>{0, 1, 2});
    CHECK_EQ(topoSmallest(1, D{}), std::vector<int>{0});
    CHECK_EQ(topoSmallest(2, D{{0, 1}, {0, 1}}), std::vector<int>{0, 1});  // duplicate edges
    CHECK_EQ(topoSmallest(2, D{{1, 0}}), std::vector<int>{1, 0});

    // cycles
    CHECK_EQ(topoSmallest(3, D{{0, 1}, {1, 2}, {2, 0}}), std::vector<int>{});
    CHECK_EQ(topoSmallest(1, D{{0, 0}}), std::vector<int>{});                    // self-loop
    CHECK_EQ(topoSmallest(4, D{{0, 1}, {2, 3}, {3, 2}}), std::vector<int>{});    // cycle off to the side
    CHECK_EQ(topoSmallest(5, D{{0, 1}, {1, 2}, {3, 4}, {4, 3}, {2, 3}}), std::vector<int>{});

    // diamond; greedy smallest-available
    CHECK_EQ(topoSmallest(4, D{{0, 1}, {0, 2}, {1, 3}, {2, 3}}), std::vector<int>{0, 1, 2, 3});
    CHECK_EQ(topoSmallest(4, D{{0, 3}, {1, 3}, {2, 3}}), std::vector<int>{0, 1, 2, 3});
    CHECK_EQ(topoSmallest(5, D{{4, 0}, {3, 1}}), std::vector<int>{2, 3, 1, 4, 0});

    // random small graphs (some cyclic) against brute force over permutations
    Rng rng{777};
    for (int trial = 0; trial < 400; trial++) {
        int n = 1 + (int)(rng.next() % 7);
        int m = (int)(rng.next() % 9);
        D deps;
        for (int i = 0; i < m; i++) deps.push_back({(int)(rng.next() % n), (int)(rng.next() % n)});
        CHECK_EQ(topoSmallest(n, deps), brute(n, deps));
    }

    // large chain in reverse: forces order n-1, n-2, ..., 0
    {
        const int n = 200000;
        D deps;
        for (int i = 0; i + 1 < n; i++) deps.push_back({i + 1, i});
        std::vector<int> expected(n);
        for (int i = 0; i < n; i++) expected[i] = n - 1 - i;
        CHECK(topoSmallest(n, deps) == expected);
        deps.push_back({0, n - 1});  // closes the cycle
        CHECK(topoSmallest(n, deps).empty());
    }

    // large random DAG: result must be a valid permutation respecting every dependency
    {
        const int n = 200000;
        std::vector<int> perm(n);
        std::iota(perm.begin(), perm.end(), 0);
        for (int i = n - 1; i > 0; i--) std::swap(perm[i], perm[rng.next() % (i + 1)]);
        D deps;
        for (int i = 0; i < 400000; i++) {
            int a = (int)(rng.next() % n), b = (int)(rng.next() % n);
            if (a == b) continue;
            if (a > b) std::swap(a, b);
            deps.push_back({perm[a], perm[b]});
        }
        std::vector<int> order = topoSmallest(n, deps);
        CHECK_EQ((int)order.size(), n);
        if ((int)order.size() == n) {
            std::vector<int> pos(n, -1);
            bool perm_ok = true;
            for (int i = 0; i < n; i++) {
                if (order[i] < 0 || order[i] >= n || pos[order[i]] != -1) { perm_ok = false; break; }
                pos[order[i]] = i;
            }
            CHECK(perm_ok);
            if (perm_ok) {
                bool ok = true;
                for (auto [a, b] : deps)
                    if (pos[a] >= pos[b]) { ok = false; break; }
                CHECK(ok);
            }
        }
    }

    return check::report();
}
