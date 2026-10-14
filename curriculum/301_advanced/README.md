# 301 advanced - drill-down

Four topics, four problems each. Within a topic the problems get harder in order:
problem 1 is the canonical form (with a one-line hint), problems 2-4 give only the contract.

Every file starts with a header (problem, examples, constraints, required complexity,
assumptions, "Think first" questions). **Before writing code, write your approach,
invariant and complexity in a comment block** (for the concurrency files: what can go
wrong, and which lock protects what). Where something is ambiguous, state your own
assumption in that block; the header lists the assumptions the tests rely on.

Each file contains a stub that compiles but fails its checks, and the tests below the
`TESTS (do not edit)` marker. Replace the stub between the markers. The stubs carry a
`#pragma ... -Wunused-parameter` line only to keep stub builds quiet; delete it once you implement.

Run one: `make run FILE=curriculum/301_advanced/<topic>/<file>.cpp`
Run a topic: `make test DIR=curriculum/301_advanced/<topic>`

## graphs_advanced

| File | Twist vs previous | Target complexity |
|---|---|---|
| `01_dijkstra.cpp` | canonical single-source shortest paths, 64-bit distances, unreachable = -1 | O((n+m) log n) |
| `02_topo_sort_cycle.cpp` | different algorithm: lexicographically smallest topological order, cycle detection as a by-product | O((n+m) log n) |
| `03_earliest_connection.cpp` | offline connectivity over time: earliest moment two nodes become connected (union-find that remembers merge times) | O((m+q) log n) |
| `04_bridges_articulation.cpp` | low-link DFS: bridges AND articulation points on a multigraph (parallel edges, self-loops, disconnected) | O(n+m) |

## advanced_structures

| File | Twist vs previous | Target complexity |
|---|---|---|
| `01_trie_prefix_counts.cpp` | canonical trie as a multiset: counts, erase, prefix count, lexicographic autocomplete | O(len) per op; autocomplete proportional to nodes visited |
| `02_fenwick_tree.cpp` | different structure: BIT with prefix/range sums, O(log n) `lowerBound` by descent, plus inversion counting with compression | O(log n) per op, O(n log n) inversions |
| `03_segment_tree_lazy.cpp` | range add with lazy propagation answering both range sum and range min, plus point set | O(log n) per op |
| `04_leaderboard.cpp` | design problem combining a hash map with an order-statistics structure: rank, top-k sum, k-th score under updates | O(log maxScore) per op |

## hard_dp

| File | Twist vs previous | Target complexity |
|---|---|---|
| `01_edit_distance.cpp` | canonical two-string DP, rolling-array space | O(nm) time, O(min(n,m)) space |
| `02_lis_nlogn.cpp` | one-sequence DP sped up with binary search over "tails", plus reconstructing the subsequence | O(n log n) |
| `03_interval_merge_cost.cpp` | interval DP with an extra "merged into m piles" dimension, K-way merges, impossibility check | about O(n^3) |
| `04_bitmask_closed_walk.cpp` | bitmask DP over visited sets after an all-pairs shortest-path preprocessing (revisits allowed, non-metric weights) | O(n^3 + 2^n n^2), n <= 18 |

## systems_and_concurrency

| File | Twist vs previous | Target complexity |
|---|---|---|
| `01_bounded_blocking_queue.cpp` | canonical mutex + two condition variables; close() semantics, timed pop | O(1) per op, no lost wake-ups |
| `02_thread_pool.cpp` | builds on the queue idea: workers, graceful shutdown, idempotent/concurrent shutdown, exceptions, waitIdle | O(1) per submit/dispatch, no polling |
| `03_token_bucket.cpp` | time as an injected dependency: lazy refill, fractional tokens, backwards clock, waitTimeMs | O(1) per call |
| `04_consistent_hash_ring.cpp` | distribution rather than locking: virtual nodes, balance and minimal-movement properties, replica lists | getNode O(log(nodes * vnodes)) |

### Running the concurrency problems

- The tests are written to have a deterministic outcome: they use `join`, counters, and
  bounded waits (5 seconds) rather than timing assertions that a correct solution could fail.
  The only sleeps are short pauses used to assert that something is still blocked, which a
  correct blocking implementation can never violate.
- If your code deadlocks or loses a wake-up, a bounded wait prints `TIMEOUT waiting for: ...`
  and exits with code 2, or a 40-second watchdog prints `WATCHDOG ...` and exits with code 3.
  Neither means the test is broken; look for a missing notify or a wait without a predicate.
- Because the build uses `-pthread`, you can also try ThreadSanitizer by hand, for example
  `g++ -std=c++20 -g -pthread -fsanitize=thread -I common/cpp <file> && ./a.out`
  (availability depends on your toolchain).
- Run each concurrent solution many times before trusting it, for example
  `for i in $(seq 20); do ./build/<name> || break; done` after one `make run`.
