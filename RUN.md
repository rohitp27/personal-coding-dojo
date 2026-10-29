# Running things

All commands run from the repo root.

| Goal | Command |
|---|---|
| Run one C++ practice file | `make run FILE=practice/cpp/stacks_queues_lists/03_linked_list_ops.cpp` |
| Run one curriculum file | `make run FILE=curriculum/101_basics/01_console_io.cpp` |
| Run one Java file | `make run FILE=practice/java/<topic>/<Name>.java` (asserts are enabled with `-ea`) |
| Run every file in a dir, PASS/FAIL summary | `make test DIR=curriculum/102_fundamentals` |
| Run one topic only | `make test DIR=curriculum/102_fundamentals/binary_search` |
| Check your current practice topic | `make test DIR=practice/cpp/stacks_queues_lists` |
| Clean build outputs | `make clean` |

- C++ flags: `g++ -std=c++20 -Wall -Wextra -g -pthread -I common/cpp`.
- On macOS with Apple Clang, the Makefile also includes the SDK's C++ headers when present. No separate GCC installation is needed. Override the compiler with `make run CXX=clang++ FILE=<path>` if desired.
- A file PASSes when it compiles and exits 0. Stubs fail until you solve them, so a fresh dir is all FAIL.
- Test helpers live in `common/cpp/check.h`: `CHECK`, `CHECK_EQ(actual, expected)`, `CHECK_NEAR`, and `return check::report();` at the end of `main()`. A failure prints file:line plus expected and actual values, and the run keeps going.
- Each file has 20s to run under `make test` (a hung solution counts as FAIL).

# Layout

- `practice/` — your working exercises (cpp/java), grouped by topic. `practice/cpp/PRIMER.md` orders the C++ refresher exercises. Some topics are copies of the 102 curriculum exercises; these are separate files, so edits do not sync between them.
- `curriculum/` — graded problems: 101 basics, then 102 / 201 / 301 as drill-downs (topic folders, 4 problems each, each harder than the last). Each level has a README with the ordered list.
- `solutions/` — reference answers (spoilers). Curriculum answers are under `solutions/<level>/`; primer answers are under `solutions/primer/`.
- `common/cpp/check.h` — test helper.
- `build/` — generated executables; removed by `make clean`.

# Practice workflow

1. Stay in the repo root: `cd /Users/rohitpandey/Software/personal-coding-dojo`.
2. Pick one working copy. If you are editing `practice/cpp/`, keep running that exact path; running its curriculum counterpart tests a different file.
3. Read the problem, examples, constraints and "Think first" questions. Write your approach, invariant and time/space complexity before coding.
4. Implement the exercise functions, keeping the supplied checks. Run the file after each small change; use `make test DIR=<topic-path>` once the individual files work.
5. Ask for a hint or trace a tiny example when stuck. Compare with `solutions/` after your attempt, then close the answer and reproduce the idea yourself.
6. Work through a topic in numeric order. Use 101 or the primer to fill syntax gaps, then 102 for core patterns, 201 for trees/graphs/DP/design, and 301 for advanced topics relevant to your interviews.

For the current linked-list exercise, first practice ordinary reversal in
`practice/cpp/linked_lists/reverse_linked_list.cpp`, then return to reversing in
k-groups. Treat cycle detection as a separate task within the same exercise.
