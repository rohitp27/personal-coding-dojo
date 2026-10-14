# 201 Data Structures: drill-downs

Four topics, four problems each. Within a topic every problem is the same core idea made harder, so do them in order.
This level is about thinking, not recall:

- Problem 1 of each topic has a light hint.
- Problems 2 and 3 give the contract plus a "Think first" list. Answer those questions (on paper or in a comment) before typing.
- Problem 4 of each topic is under-specified on purpose, like an interview. It lists the assumptions chosen for the tests.
  Write your own approach (approach, invariant, complexity) in a comment block at the top of your solution BEFORE coding.

Every file is a compiling stub with `// TODO` bodies. Run one with `make run FILE=<path>` or a whole topic with
`make test DIR=curriculum/201_data_structures/<topic>`. Replace everything between the
`YOUR CODE BELOW` and `TESTS` marker comments; do not edit the tests. Larger tests are sized so a solution with the
required complexity finishes in about a second or two, and a slower one does not.

## trees

| File | Twist vs previous | Target complexity |
|------|-------------------|-------------------|
| `trees/01_bst_insert_search_delete.cpp` | Canonical BST: insert, search, three-case delete | O(h) per operation |
| `trees/02_iterative_traversals.cpp` | No recursion: in/pre/post-order with an explicit stack, 1,000,000-deep chains | O(n) time, O(h) space |
| `trees/03_lca_and_paths.cpp` | Plain binary tree (no ordering): lowest common ancestor, path between two values, max path sum | O(n) per call |
| `trees/04_serialize_deserialize.cpp` | Design your own codec; ambiguity, determinism, deep chains (under-specified) | O(n) encode and decode |

## graphs

| File | Twist vs previous | Target complexity |
|------|-------------------|-------------------|
| `graphs/01_grid_bfs_shortest_path.cpp` | Canonical BFS shortest path on a grid | O(rows * cols) |
| `graphs/02_components_and_cycles.cpp` | Edge-list graphs, duplicates and self-loops, undirected vs directed cycles, 1,000,000-long paths | O(n + E) |
| `graphs/03_bipartite_and_topological_order.cpp` | Colouring and dependency order: bipartite check, lexicographically smallest topological order, minimum layers | O(n + E), O((n + E) log n) for the ordered topo sort |
| `graphs/04_shortest_path_with_wall_breaks.cpp` | Shortest path with a budget (state = cell + walls used) and route reconstruction (under-specified) | O(rows * cols * (k + 1)) |

## dp

| File | Twist vs previous | Target complexity |
|------|-------------------|-------------------|
| `dp/01_one_dimensional_dp.cpp` | Canonical 1D DP: non-adjacent maximum, fewest coins, number of coin combinations | O(n); O(amount * coins) |
| `dp/02_knapsack_variants.cpp` | Second dimension (capacity), 0/1 choice; partition and subset counting as the same table | O(items * capacity) time, O(capacity) space |
| `dp/03_lcs_and_edit_distance.cpp` | Two strings, 2D table, rebuilding an actual answer, edit distance transitions | O(n * m) time |
| `dp/04_round_trip_pickup_state_design.cpp` | The state is the whole problem: two simultaneous walkers on a grid (under-specified) | O(n^3) |

## design

| File | Twist vs previous | Target complexity |
|------|-------------------|-------------------|
| `design/01_lru_cache.cpp` | Canonical LRU cache: hash map plus recency list | O(1) per get and put |
| `design/02_ring_buffer_iterator.cpp` | Templated container with a hand-written forward iterator (const and non-const), usable with STL algorithms | O(1) push and iterator step |
| `design/03_unique_ptr_owned_list.cpp` | Ownership: `unique_ptr` nodes, move-only elements, correct move operations, no stack overflow on destruction | O(1) per operation, O(n) destroy with O(1) stack |
| `design/04_indexed_min_heap_timer_queue.cpp` | Heap from scratch plus a position index so any item can be updated or cancelled (under-specified) | O(log n) update, cancel, pop |
