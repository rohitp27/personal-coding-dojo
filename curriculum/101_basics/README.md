# 101 Basics

Ten gentle C++ warm-ups, in strictly increasing difficulty. Each file has a problem
statement, a stub with `// TODO` bodies and small hints, and tests in `main()`.
The stubs compile cleanly and the checks fail until you implement them.

Run one: `make run FILE=curriculum/101_basics/01_console_io.cpp`
Run all: `make test DIR=curriculum/101_basics`

| File | Concept | What it drills |
|------|---------|----------------|
| `01_console_io.cpp` | Streams | `std::istream` / `std::ostream` parameters (tests use `istringstream` / `ostringstream`), `>>` until EOF, `getline`, `std::fixed` + `setprecision` |
| `02_integer_pitfalls.cpp` | Integer arithmetic | Truncating division, negative `%`, widening to `long long`, overflow-safe midpoint / ceil-div / add check, factorial limits |
| `03_loops.cpp` | Loops | Digit sum, digit reversal with overflow guard, primality up to `INT_MAX`, prime counting, Collatz steps |
| `04_functions_gcd.cpp` | Functions | Euclid gcd, lcm without overflow, fraction reduction, folding gcd over a vector, fast modular exponentiation |
| `05_arrays.cpp` | `std::vector` by index | In-place reverse, rotate by k (negative / oversized k), min/max, stable zero-move, second largest distinct with an out-parameter |
| `06_strings.cpp` | Strings | Alphanumeric-only palindrome, vowel count, `std::map` character frequency, first unique char, anagram check |
| `07_references_pointers.cpp` | References vs pointers | Swap via `&` and `*`, null-safe pointers, out-parameters, returning a reference (no copy), returning a pointer into a container |
| `08_structs_classes.cpp` | Structs and classes | Constructors with member-initializer lists, `const` methods, private state, all-or-nothing operations, self-operation guards |
| `09_raw_arrays_pointers.cpp` | Raw arrays and pointer arithmetic | Half-open `[first, last)` ranges, `p + i`, pointer subtraction, C-string walking, two-pointer partition, staying in bounds |
| `10_heap_owning_class.cpp` | `new` / `delete`, rule of five | A class owning `new[]` memory: destructor, deep copy, copy/move assignment, self-assignment safety, resize; tests count live allocations to catch leaks |
