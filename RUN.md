# Running things

All commands run from the repo root.

| Goal | Command |
|---|---|
| Run one C++ file | `make run FILE=curriculum/101_basics/01_<name>.cpp` |
| Run one Java file | `make run FILE=practice/java/<topic>/<Name>.java` (asserts are enabled with `-ea`) |
| Run every file in a dir, PASS/FAIL summary | `make test DIR=curriculum/102_fundamentals` |
| Run one topic only | `make test DIR=curriculum/102_fundamentals/binary_search` |
| Clean build outputs | `make clean` |

- C++ flags: `g++ -std=c++20 -Wall -Wextra -g -pthread -I common/cpp`.
- A file PASSes when it compiles and exits 0. Stubs fail until you solve them, so a fresh dir is all FAIL.
- Test helpers live in `common/cpp/check.h`: `CHECK`, `CHECK_EQ(actual, expected)`, `CHECK_NEAR`, and `return check::report();` at the end of `main()`. A failure prints file:line plus expected and actual values, and the run keeps going.
- Each file has 20s to run under `make test` (a hung solution counts as FAIL).

# Layout

- `practice/` — your own warm-ups (cpp/java), flat by topic.
- `curriculum/` — graded problems: 101 basics, then 102 / 201 / 301 as drill-downs (topic folders, 4 problems each, each harder than the last). Each level has a README with the ordered list.
- `common/cpp/check.h` — test helper.
