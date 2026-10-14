// Problem: Shortest path on a grid.
//   int shortestPath(const std::vector<std::string>& grid)
//   grid[r][c] is '.' (open) or '#' (wall). Start at the top-left cell, end at the bottom-right cell.
//   Moves: up/down/left/right, one step each, never onto a wall or off the grid.
//   Return the minimum number of moves, or -1 if the end is unreachable. An empty grid returns -1.
//   If the start or end cell is a wall, return -1. A 1x1 open grid needs 0 moves.
//
// Hint: every move costs the same, so a FIFO queue explores cells in order of distance. Mark a cell visited
//   when you PUSH it, not when you pop it, or the queue can blow up.
//
// Examples:
//   {"...", "##.", "..."}        -> 4
//   {".#", "#."}                 -> -1
//   {"....", "###.", "...."}     -> 5
//
// Constraints: up to 1,000,000 cells (e.g. 1000 x 1000, or 1 x 1,000,000).
// Required complexity: O(rows * cols) time and space.
// Run: make run FILE=curriculum/201_data_structures/graphs/01_grid_bfs_shortest_path.cpp

#include "check.h"

#include <queue>
#include <string>
#include <utility>
#include <vector>

// ===== YOUR CODE BELOW (stub) =====
int shortestPath(const std::vector<std::string>& grid) {
    if (grid.empty() || grid[0].empty()) return -1;
    int R = grid.size(), C = grid[0].size();
    if (grid[0][0] == '#' || grid[R-1][C-1] == '#') return -1;
    std::vector<int> dist((size_t)R * C, -1);
    std::queue<int> q;
    dist[0] = 0; q.push(0);
    const int dr[] = {1,-1,0,0}, dc[] = {0,0,1,-1};
    while (!q.empty()) {
        int cur = q.front(); q.pop();
        int r = cur / C, c = cur % C;
        if (r == R-1 && c == C-1) return dist[cur];
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= R || nc < 0 || nc >= C || grid[nr][nc] == '#') continue;
            int id = nr * C + nc;
            if (dist[id] != -1) continue;
            dist[id] = dist[cur] + 1; q.push(id);
        }
    }
    return -1;
}
// ===== TESTS (do not edit below) =====
using G = std::vector<std::string>;

int main() {
    CHECK_EQ(shortestPath(G{}), -1);
    CHECK_EQ(shortestPath(G{"."}), 0);
    CHECK_EQ(shortestPath(G{"#"}), -1);
    CHECK_EQ(shortestPath(G{".."}), 1);
    CHECK_EQ(shortestPath(G{".", "."}), 1);
    CHECK_EQ(shortestPath(G{"..", ".."}), 2);
    CHECK_EQ(shortestPath(G{".#", "#."}), -1);
    CHECK_EQ(shortestPath(G{"#."}), -1);  // start is a wall
    CHECK_EQ(shortestPath(G{".#"}), -1);  // end is a wall
    CHECK_EQ(shortestPath(G{"...", "##.", "..."}), 4);
    CHECK_EQ(shortestPath(G{"....", "###.", "...."}), 5);
    CHECK_EQ(shortestPath(G{".#.", ".#.", "..."}), 4);
    // forced detour: down the left side, over, up column 2, across the top, down the right side
    CHECK_EQ(shortestPath(G{".#...", ".#.#.", ".#.#.", ".#.#.", "...#."}), 16);
    // open path exists with no detour
    CHECK_EQ(shortestPath(G{"..#..", ".##.#", "...#.", "##...", "....."}), 8);
    // walls seal the end in
    CHECK_EQ(shortestPath(G{"...", ".##", ".#."}), -1);
    // rectangular, not square
    CHECK_EQ(shortestPath(G{"....."}), 4);
    CHECK_EQ(shortestPath(G{"..#..", "#.#.#", "#...."}), 6);

    // 1000 x 1000 fully open: Manhattan distance
    {
        G g(1000, std::string(1000, '.'));
        CHECK_EQ(shortestPath(g), 1998);
    }
    // 1 x 1,000,000 corridor
    {
        G g(1, std::string(1000000, '.'));
        CHECK_EQ(shortestPath(g), 999999);
    }
    // 1001 x 1000 serpentine: odd rows are walls with one gap alternating right/left, so the path must snake
    {
        const int R = 1001, C = 1000;
        G g(R, std::string(C, '.'));
        for (int r = 1; r < R; r += 2) {
            g[r] = std::string(C, '#');
            int gap = (((r - 1) / 2) % 2 == 0) ? C - 1 : 0;
            g[r][gap] = '.';
        }
        // 501 open rows crossed end to end (999 steps each) + 1000 vertical steps
        CHECK_EQ(shortestPath(g), 501 * 999 + 1000);
    }
    // 1000 x 1000 with a full wall column in the middle: end unreachable, whole half explored
    {
        G g(1000, std::string(1000, '.'));
        for (auto& row : g) row[500] = '#';
        CHECK_EQ(shortestPath(g), -1);
        g[999][500] = '.';  // open one gap in the last row
        CHECK_EQ(shortestPath(g), 1998);
    }
    return check::report();
}
