// Problem: Find an actual shortest route through a maze where you may smash through a limited number of walls.
//   std::vector<std::pair<int,int>> shortestRoute(const std::vector<std::string>& grid, int k)
//   grid cells: 'S' start, 'T' target, '.' open, '#' wall. Moves: up/down/left/right, one step per move.
//   You may step onto at most k wall cells over the whole route (each time you enter a '#' cell counts, even
//   if you enter the same wall cell twice).
//   Return the route as (row, col) cells from S to T inclusive, with the MINIMUM number of moves. If several
//   routes tie, any one is fine. Return an empty vector if T cannot be reached.
//
// Assumptions (chosen for this exercise, interview-style; challenge them in your approach comment):
//   * exactly one 'S' and one 'T', different cells, neither is a wall
//   * 4-directional movement, every move costs 1, k >= 0 and k <= 50
//   * grid is rectangular and at most 400 x 400
//   * the tests check the route is valid (adjacent cells, in bounds, wall budget respected, endpoints right)
//     and has the optimal length; they do not compare against one specific route
// Required complexity: O(rows * cols * (k + 1)) time and space. Exhaustive search over routes will not finish.
// Run: make run FILE=curriculum/201_data_structures/graphs/04_shortest_path_with_wall_breaks.cpp
//
// BEFORE CODING: write your own approach in a comment block right here, at the top of your solution:
//   approach (what is a search state, what is an edge, why BFS and not Dijkstra or DFS),
//   invariant (what does the first time you pop a state guarantee, how do you rebuild the route),
//   complexity (number of states, memory for the route reconstruction).

#include "check.h"

#include <algorithm>
#include <queue>
#include <random>
#include <string>
#include <utility>
#include <vector>

using Cell = std::pair<int, int>;

// ===== YOUR CODE BELOW (stub) =====
std::vector<Cell> shortestRoute([[maybe_unused]] const std::vector<std::string>& grid, [[maybe_unused]] int k) {
    // TODO
    return {};
}

// ===== TESTS (do not edit below) =====
using G = std::vector<std::string>;

static Cell find(const G& g, char ch) {
    for (size_t r = 0; r < g.size(); r++)
        for (size_t c = 0; c < g[r].size(); c++)
            if (g[r][c] == ch) return {(int)r, (int)c};
    return {-1, -1};
}

// -2 = invalid route, -1 = empty route, otherwise number of moves.
static int routeMoves(const G& g, int k, const std::vector<Cell>& route) {
    if (route.empty()) return -1;
    if (route.front() != find(g, 'S') || route.back() != find(g, 'T')) return -2;
    int walls = 0;
    int R = (int)g.size(), C = (int)g[0].size();
    for (size_t i = 0; i < route.size(); i++) {
        auto [r, c] = route[i];
        if (r < 0 || r >= R || c < 0 || c >= C) return -2;
        if (g[r][c] == '#') walls++;
        if (i > 0) {
            int d = std::abs(r - route[i - 1].first) + std::abs(c - route[i - 1].second);
            if (d != 1) return -2;
        }
    }
    if (walls > k) return -2;
    return (int)route.size() - 1;
}

// Independent oracle for small grids: relax dist[r][c][w] to a fixpoint (Bellman-Ford style, no queue).
static int oracle(const G& g, int k) {
    int R = (int)g.size(), C = (int)g[0].size();
    const int INF = 1 << 28;
    std::vector<int> dist((size_t)R * C * (k + 1), INF);
    auto id = [&](int r, int c, int w) { return ((size_t)r * C + c) * (k + 1) + w; };
    Cell s = find(g, 'S'), t = find(g, 'T');
    dist[id(s.first, s.second, 0)] = 0;
    bool changed = true;
    while (changed) {
        changed = false;
        for (int r = 0; r < R; r++)
            for (int c = 0; c < C; c++)
                for (int w = 0; w <= k; w++) {
                    int d = dist[id(r, c, w)];
                    if (d >= INF) continue;
                    const int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
                    for (int q = 0; q < 4; q++) {
                        int nr = r + dr[q], nc = c + dc[q];
                        if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
                        int nw = w + (g[nr][nc] == '#' ? 1 : 0);
                        if (nw > k) continue;
                        if (dist[id(nr, nc, nw)] > d + 1) {
                            dist[id(nr, nc, nw)] = d + 1;
                            changed = true;
                        }
                    }
                }
    }
    int best = INF;
    for (int w = 0; w <= k; w++) best = std::min(best, dist[id(t.first, t.second, w)]);
    return best >= INF ? -1 : best;
}

static void expect(const G& g, int k, int moves, int line) {
    int got = routeMoves(g, k, shortestRoute(g, k));
    check::total++;
    if (got != moves)
        check::fail(__FILE__, line,
                    "route moves (-2 = invalid route, -1 = none) expected " + std::to_string(moves) + " got " +
                        std::to_string(got));
}
#define EXPECT(...) expect(__VA_ARGS__, __LINE__)

int main() {
    EXPECT(G{"S.T"}, 0, 2);
    EXPECT(G{"ST"}, 0, 1);
    EXPECT(G{"S#T"}, 0, -1);
    EXPECT(G{"S#T"}, 1, 2);
    EXPECT(G{"S#T", "..."}, 0, 4);  // around the wall
    EXPECT(G{"S#T", "..."}, 1, 2);  // straight through it
    EXPECT(G{"S##", "###", "##T"}, 2, -1);
    EXPECT(G{"S##", "###", "##T"}, 3, 4);
    EXPECT(G{"S.", "#T"}, 0, 2);
    EXPECT(G{"T#S"}, 1, 2);  // target left of start
    // detour under the wall column vs smashing through it
    EXPECT(G{"S.#..", "..#.T", "....."}, 0, 7);
    EXPECT(G{"S.#..", "..#.T", "....."}, 1, 5);
    EXPECT(G{"S.#.T"}, 0, -1);
    EXPECT(G{"S.#.T"}, 1, 4);
    // a wall budget is per entered wall cell: two walls in a row need k = 2
    EXPECT(G{"S##T"}, 1, -1);
    EXPECT(G{"S##T"}, 2, 3);

    // random small grids against the oracle
    {
        std::mt19937 rng(31337);
        int bad = 0;
        for (int iter = 0; iter < 400; iter++) {
            int R = 1 + (int)(rng() % 6), C = 2 + (int)(rng() % 5);
            G g(R, std::string(C, '.'));
            int density = 20 + (int)(rng() % 40);
            for (auto& row : g)
                for (auto& ch : row)
                    if ((int)(rng() % 100) < density) ch = '#';
            int sr = (int)(rng() % R), sc = (int)(rng() % C);
            int tr, tc;
            do {
                tr = (int)(rng() % R);
                tc = (int)(rng() % C);
            } while (tr == sr && tc == sc);
            g[sr][sc] = 'S';
            g[tr][tc] = 'T';
            int k = (int)(rng() % 4);
            if (routeMoves(g, k, shortestRoute(g, k)) != oracle(g, k)) bad++;
        }
        CHECK_EQ(bad, 0);  // number of random grids where the route was invalid or not the shortest
    }

    // large: 300 x 300, one full wall column in the middle
    {
        G g(300, std::string(300, '.'));
        for (auto& row : g) row[150] = '#';
        g[0][0] = 'S';
        g[299][299] = 'T';
        EXPECT(g, 0, -1);
        EXPECT(g, 1, 598);
        EXPECT(g, 7, 598);
        // two wall columns need two breaks
        for (auto& row : g) row[100] = '#';
        EXPECT(g, 1, -1);
        EXPECT(g, 2, 598);
        // three wall columns need three breaks
        for (auto& row : g) row[220] = '#';
        EXPECT(g, 2, -1);
        EXPECT(g, 3, 598);
    }
    // large: serpentine, odd rows are walls with a single gap alternating right/left.
    // k = 0 must snake along every open row; one break per wall row (50 of them) goes straight.
    {
        auto serpentine = [](int R, int C) {
            G g(R, std::string(C, '.'));
            for (int r = 1; r < R; r += 2) {
                g[r] = std::string(C, '#');
                g[r][(((r - 1) / 2) % 2 == 0) ? C - 1 : 0] = '.';
            }
            g[0][0] = 'S';
            g[R - 1][C - 1] = 'T';
            return g;
        };
        G big = serpentine(301, 300);  // 151 open rows * 299 steps + 300 vertical steps
        EXPECT(big, 0, 151 * 299 + 300);
        G mid = serpentine(101, 100);  // 51 open rows * 99 steps + 100 vertical steps
        EXPECT(mid, 0, 51 * 99 + 100);
        EXPECT(mid, 50, 100 + 99);  // Manhattan distance
        EXPECT(mid, 51, 100 + 99);
    }
    return check::report();
}
