// Problem: Round-trip pickup on a grid. The hard part is choosing the DP state.
//   int maxPickup(const std::vector<std::vector<int>>& grid)
//   grid is n x n. Cell values: -1 = blocked, 0 = empty, v > 0 = v coins. A courier walks from the top-left cell
//   to the bottom-right cell moving only right or down, then walks back to the top-left moving only left or up.
//   Every cell the courier visits must be non-blocked. Coins are picked up the first time a cell is visited
//   (the cell becomes 0 afterwards, so a cell on both legs is counted once).
//   Return the maximum coins collectable; if no valid round trip exists return 0.
//
// Examples:
//   {{0,1,-1},{1,0,-1},{1,1,1}}            -> 5
//   {{1,1,-1},{1,-1,1},{-1,1,1}}           -> 0   (no path to the corner)
//   {{5}}                                  -> 5   (start == end, count it once)
//   n x n all ones, n >= 2                 -> 4n - 4
//
// Assumptions (chosen for this exercise, interview-style; challenge them in your approach comment):
//   * n >= 1, 1 <= n <= 50, values are -1 or 0..100
//   * if the start or the end cell is blocked the answer is 0
//   * the return leg is the same as a second down/right trip viewed in reverse, but the two legs may overlap
//   * the tests compare only the numeric answer
// Required complexity: O(n^3) time and at most O(n^3) space (O(n^2) is possible). Enumerating path pairs, or
//   running the best single trip twice, will not be accepted: the second is incorrect and the first is
//   exponential.
// Run: make run FILE=curriculum/201_data_structures/dp/04_round_trip_pickup_state_design.cpp
//
// BEFORE CODING: write your own approach in a comment block right here, at the top of your solution:
//   approach (what is the state, what is the transition, what makes the two legs comparable step by step),
//   invariant (what does dp[state] promise, and how are shared cells counted once),
//   complexity (number of states, transitions per state).

#include "check.h"

#include <algorithm>
#include <functional>
#include <random>
#include <vector>

using Grid = std::vector<std::vector<int>>;

// ===== YOUR CODE BELOW (stub) =====
int maxPickup([[maybe_unused]] const Grid& grid) {
    // TODO
    return 0;
}

// ===== TESTS (do not edit below) =====
// Independent oracle for tiny grids: enumerate every monotone path, then every pair, count the union.
static int oracle(const Grid& g) {
    int n = (int)g.size();
    if (g[0][0] == -1 || g[n - 1][n - 1] == -1) return 0;
    std::vector<std::vector<std::pair<int, int>>> paths;
    std::vector<std::pair<int, int>> cur;
    std::function<void(int, int)> go = [&](int r, int c) {
        if (r >= n || c >= n || g[r][c] == -1) return;
        cur.push_back({r, c});
        if (r == n - 1 && c == n - 1)
            paths.push_back(cur);
        else {
            go(r + 1, c);
            go(r, c + 1);
        }
        cur.pop_back();
    };
    go(0, 0);
    int best = 0;
    for (auto& p : paths)
        for (auto& q : paths) {
            std::vector<std::vector<char>> seen(n, std::vector<char>(n, 0));
            int sum = 0;
            for (auto* path : {&p, &q})
                for (auto [r, c] : *path)
                    if (!seen[r][c]) {
                        seen[r][c] = 1;
                        sum += g[r][c];
                    }
            best = std::max(best, sum);
        }
    return best;
}

int main() {
    CHECK_EQ(maxPickup({{5}}), 5);
    CHECK_EQ(maxPickup({{0}}), 0);
    CHECK_EQ(maxPickup({{-1}}), 0);
    CHECK_EQ(maxPickup({{0, 1}, {1, 0}}), 2);
    CHECK_EQ(maxPickup({{1, 1}, {1, 1}}), 4);
    CHECK_EQ(maxPickup({{0, 1, -1}, {1, 0, -1}, {1, 1, 1}}), 5);
    CHECK_EQ(maxPickup({{1, 1, -1}, {1, -1, 1}, {-1, 1, 1}}), 0);
    CHECK_EQ(maxPickup({{-1, 1}, {1, 1}}), 0);  // start blocked
    CHECK_EQ(maxPickup({{1, 1}, {1, -1}}), 0);  // end blocked
    CHECK_EQ(maxPickup({{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}), 8);
    CHECK_EQ(maxPickup({{1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 1}}), 7);  // one path holds it all
    CHECK_EQ(maxPickup({{0, 0, 9}, {0, 0, 0}, {9, 0, 0}}), 18);  // opposite corners: one leg per corner

    // random tiny grids against the oracle (this also validates the hand-made expectations above)
    {
        std::mt19937 rng(5);
        int bad = 0;
        for (int iter = 0; iter < 400; iter++) {
            int n = 1 + (int)(rng() % 5);
            Grid g(n, std::vector<int>(n));
            int blockPct = (int)(rng() % 30);
            for (auto& row : g)
                for (auto& x : row) x = (int)(rng() % 100) < blockPct ? -1 : (int)(rng() % 4);
            if (maxPickup(g) != oracle(g)) bad++;
        }
        CHECK_EQ(bad, 0);  // number of random grids where the answer differs from brute force
    }

    // n = 50 where brute force is hopeless
    {
        const int n = 50;
        CHECK_EQ(maxPickup(Grid(n, std::vector<int>(n, 1))), 4 * n - 4);
        CHECK_EQ(maxPickup(Grid(n, std::vector<int>(n, 0))), 0);
        // only the main diagonal holds coins: a staircase path collects all of it
        Grid diag(n, std::vector<int>(n, 0));
        for (int i = 0; i < n; i++) diag[i][i] = 1;
        CHECK_EQ(maxPickup(diag), n);
        // anti-diagonal wall with a single door at (24, 25): both legs squeeze through it
        Grid door(n, std::vector<int>(n, 1));
        for (int i = 0; i < n; i++) door[i][n - 1 - i] = -1;
        door[24][25] = 1;
        // each half is a 25x26 / 26x25 rectangle: two paths from corner to corner cover 2 * 50 - 2 = 98 cells,
        // and the door cell is shared by both halves: 98 + 98 - 1
        CHECK_EQ(maxPickup(door), 195);
        // full of 100s with a fully blocked anti-diagonal: unreachable
        Grid wall(n, std::vector<int>(n, 100));
        for (int i = 0; i < n; i++) wall[i][n - 1 - i] = -1;
        CHECK_EQ(maxPickup(wall), 0);
    }
    return check::report();
}
