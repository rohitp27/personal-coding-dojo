# C++ Primer

Twelve gentle problems to rebuild syntax and STL fluency. Each file is a stub: it compiles,
and its checks fail until you fill in the `// TODO` bodies. Run one with
`make run FILE=<path>`, or a whole topic with `make test DIR=practice/cpp/<topic>`.

## Order

### stl_basics
1. `stl_basics/01_vector_edit.cpp` - range-for, erase-remove, find + insert, sort with a comparator.
2. `stl_basics/02_string_ops.cpp` - substr/find/npos, appending, char classification and case conversion.
3. `stl_basics/03_word_count.cpp` - unordered_map find-or-insert via `operator[]`, structured bindings, unordered_set.
4. `stl_basics/04_ordered_set_map.cpp` - set/map ordered iteration, lower_bound/upper_bound, std::distance.

### recursion
5. `recursion/01_factorial_digits.cpp` - base case + shrinking argument; `%`/`/` digit tricks; substr.
6. `recursion/02_fibonacci_memo.cpp` - memo table passed by reference, bottom-up vector DP.
7. `recursion/03_subsets.cpp` - include/exclude recursion; choose, recurse, un-choose.
8. `recursion/04_backtracking.cpp` - balanced parentheses and permutations by backtracking.

### stacks_queues
9. `stacks_queues/01_array_stack.cpp` - class skeleton, array member, const methods, throwing exceptions.
10. `stacks_queues/02_queue_tricks.cpp` - std::queue and std::stack API (front/pop/push), rotating a queue.
11. `stacks_queues/03_balanced_brackets.cpp` - std::stack for matching; a std::string used as a stack.
12. `stacks_queues/04_ring_buffer.cpp` - circular indexing with modulo, member-initializer list, out-parameters.

## Syntax cheat-sheet

### Headers
```cpp
#include <vector>          // std::vector
#include <string>          // std::string, std::to_string
#include <map>             // std::map (ordered by key)
#include <set>             // std::set (ordered, unique)
#include <unordered_map>   // std::unordered_map (hash)
#include <unordered_set>   // std::unordered_set
#include <queue>           // std::queue, std::priority_queue
#include <stack>           // std::stack
#include <algorithm>       // sort, find, remove, reverse, min, max
#include <functional>      // std::greater, std::function
#include <iterator>        // std::distance
#include <cctype>          // std::isalpha, std::toupper, ...
#include <stdexcept>       // std::out_of_range, std::runtime_error
#include <iostream>        // std::cout
```

### Declaring things
```cpp
std::vector<int> v;                       // empty
std::vector<int> w = {1, 2, 3};           // initializer list
std::vector<int> z(5, 0);                 // five zeros
std::vector<std::vector<int>> grid(3, std::vector<int>(4, 0));   // 3x4 of zeros

std::string s = "hello";
std::map<std::string, int> m;             // sorted by key
std::unordered_map<std::string, int> h;   // hash map
std::set<int> st = {3, 1, 2};
```

### Common operations
```cpp
v.push_back(4);   v.pop_back();   v.size();   v.empty();   v.back();   v[0];   v.at(0);   // at() throws if out of range
v.insert(v.begin() + 1, 9);                       // insert BEFORE the iterator
v.erase(v.begin());                               // erase one element by iterator
v.erase(std::remove(v.begin(), v.end(), 7), v.end());   // erase-remove: delete all 7s
std::sort(v.begin(), v.end());
std::sort(v.begin(), v.end(), std::greater<int>());
std::reverse(v.begin(), v.end());
auto it = std::find(v.begin(), v.end(), 3);       // it == v.end() if absent

s.substr(2);  s.substr(2, 3);  s.find("lo");      // find returns std::string::npos if absent
s += "x";  s.push_back('y');  s.back();  s.pop_back();
std::to_string(42);

h["a"]++;                                         // operator[] inserts a default (0) if missing
h.count("a");  h.find("a");  h.erase("a");
auto f = m.lower_bound("b");                      // first key >= "b" (map/set only)
auto u = st.upper_bound(2);                       // first element > 2
```

### Range-for and structured bindings
```cpp
for (int x : v) { ... }                           // copy of each element
for (const auto& x : v) { ... }                   // no copy, read-only
for (auto& x : v) { x *= 2; }                     // modify in place
for (const auto& [key, value] : m) { ... }        // map entries (C++17)
for (size_t i = 0; i < v.size(); i++) { ... }     // size() is unsigned
```

### Lambdas
```cpp
auto cmp = [](int a, int b) { return a > b; };
std::sort(v.begin(), v.end(), cmp);

int base = 10;
auto addBase = [base](int x) { return x + base; };   // capture by value
auto bump = [&base]() { base++; };                   // capture by reference
auto all = [&](int x) { return x + base; };          // capture everything by reference
```

### References, pointers, const
```cpp
void f(std::vector<int> v);          // copy
void g(std::vector<int>& v);         // reference: caller's object can be modified
void h(const std::vector<int>& v);   // const reference: no copy, no modification (default for read-only params)
int  n(const std::string& s) const;  // trailing const on a member function: does not modify *this

int x = 5;
int& r = x;          // r is another name for x
int* p = &x;         // pointer to x
*p = 6;              // dereference
const int c = 3;     // cannot be reassigned
```

### struct / class skeleton
```cpp
struct Point {
    int x = 0;
    int y = 0;
};

class Counter {
public:
    explicit Counter(int start) : count_(start) {}   // member-initializer list
    void inc() { count_++; }
    int value() const { return count_; }
private:
    int count_;
};                                                   // note the semicolon

Counter c(10);
c.inc();
```

### Exceptions
```cpp
throw std::out_of_range("message");
try { risky(); } catch (const std::out_of_range& e) { /* e.what() */ }
```

### Test helper used in this repo
```cpp
#include "check.h"
CHECK(cond);
CHECK_EQ(actual, expected);          // braced lists work: CHECK_EQ(f(), std::vector<int>{1, 2})
CHECK_NEAR(a, b, 1e-9);
int main() { /* checks */ return check::report(); }
```
