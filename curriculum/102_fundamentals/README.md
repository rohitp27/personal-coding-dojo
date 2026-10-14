# 102 Fundamentals: Drill-Down

Five topics, four problems each. Within a topic, problem 1 is the canonical form and each
next problem is a harder variant of the same idea. Problem 1 (and usually 2) carries a hint.
Problems 3 and 4 give only the contract plus a short "Think first:" list: answer those
questions (on paper) before you write code. Larger tests in 3 and 4 are sized so that the
obvious brute force runs out of time.

Each file is a stub: it compiles, and its checks fail until you implement the `// TODO`
bodies. Run one with `make run FILE=<path>`; run a whole topic with
`make test DIR=curriculum/102_fundamentals/<topic>`.

## 1. hashing

| File | Twist vs the previous one | Target complexity |
|------|---------------------------|-------------------|
| `hashing/01_first_unique_char.cpp` | Baseline: count occurrences, then pick the first with count 1 | O(n) time, O(1) space |
| `hashing/02_group_anagrams.cpp` | Counts become a canonical key; hash map from key to group, with exact output ordering | O(total chars * log L) or better |
| `hashing/03_count_pairs_with_sum.cpp` | Look up a complement while scanning; duplicates and 64-bit overflow | O(n) expected |
| `hashing/04_longest_consecutive.cpp` | No complement: use a set to find run starts so each element is walked once; INT edge values | O(n) expected |

## 2. two_pointers_and_windows

| File | Twist vs the previous one | Target complexity |
|------|---------------------------|-------------------|
| `two_pointers_and_windows/01_two_sum_sorted.cpp` | Baseline: opposite-end pointers on a sorted array | O(n) time, O(1) space |
| `two_pointers_and_windows/02_dedupe_partition.cpp` | Same-direction read/write pointers: in-place filtering (keep-at-most-two, stable zero move) | O(n) time, O(1) space |
| `two_pointers_and_windows/03_fixed_window.cpp` | A window that slides with a hash-map count of distinct values | O(n) expected, O(k) space |
| `two_pointers_and_windows/04_variable_window.cpp` | Window grows and shrinks to hold an invariant; shortest valid window, leftmost on ties | O(\|s\| + \|t\|) time, O(1) space |

## 3. binary_search

| File | Twist vs the previous one | Target complexity |
|------|---------------------------|-------------------|
| `binary_search/01_binary_search_classic.cpp` | Baseline: find an exact value in a strictly sorted array | O(log n) per search |
| `binary_search/02_first_last_occurrence.cpp` | Duplicates: boundaries instead of any hit (lower and upper bound) | O(log n) per query |
| `binary_search/03_rotated_sorted.cpp` | Array is not globally sorted: decide which half is sorted at each step | O(log n) per search |
| `binary_search/04_search_on_answer.cpp` | No array to search: binary search a numeric answer space with a feasibility check | O(n log(sum of weights)) |

## 4. stacks_queues_lists

| File | Twist vs the previous one | Target complexity |
|------|---------------------------|-------------------|
| `stacks_queues_lists/01_balanced_brackets.cpp` | Baseline: stack of open brackets matched against closers | O(n) time, O(n) space |
| `stacks_queues_lists/02_queue_from_two_stacks.cpp` | Build one structure from another; amortized cost analysis | amortized O(1) per op |
| `stacks_queues_lists/03_linked_list_ops.cpp` | Pointer manipulation: reverse in groups of k, and find a cycle's start (two functions) | O(n) time, O(1) space |
| `stacks_queues_lists/04_largest_rectangle.cpp` | Monotonic stack: pop-time reasoning to get nearest-smaller bounds | O(n) time, O(n) space |

## 5. recursion_and_sorting

| File | Twist vs the previous one | Target complexity |
|------|---------------------------|-------------------|
| `recursion_and_sorting/01_memo_climb_stairs.cpp` | Baseline: top-down recursion with a memo table | O(n) time and space |
| `recursion_and_sorting/02_subsets_permutations.cpp` | No overlap to memoize: backtracking with choose, explore, unchoose (two functions) | O(n * 2^n) and O(n * n!) |
| `recursion_and_sorting/03_merge_sort_inversions.cpp` | Divide and conquer that also counts across the split while merging | O(n log n) time, O(n) space |
| `recursion_and_sorting/04_quickselect.cpp` | Partition-based selection: recurse into one side only; pivot choice and duplicates matter | expected O(n) time |
