// Problem: Practice ordered containers (std::set / std::map) and lower_bound.
//   uniqueSorted(v)              v with duplicates removed, ascending
//   smallestAtLeast(s, x)        smallest element of s that is >= x, or -1 if none
//   countInRange(s, lo, hi)      how many elements e of s satisfy lo <= e <= hi
//   formatAges(ages)             one "name=age" string per entry, in key order
//
// Examples:
//   uniqueSorted({3,1,3,2})                  -> {1,2,3}
//   smallestAtLeast({10,20,30}, 15)          -> 20
//   countInRange({1,3,5,7,9}, 3, 7)          -> 3
//   formatAges({{"bob",30},{"al",25}})       -> {"al=25", "bob=30"}
//
// Edge cases: empty set/map, x above every element, lo > hi (count is 0).
//
// Run with: make run FILE=practice/cpp/stl_basics/04_ordered_set_map.cpp
//
// Syntax reminder:
//   std::set<int> s(v.begin(), v.end());           // sorted + de-duplicated
//   std::vector<int> out(s.begin(), s.end());
//   auto it = s.lower_bound(x);                    // first element >= x (s.end() if none)
//   auto jt = s.upper_bound(y);                    // first element >  y
//   std::distance(it, jt)                          // number of steps between iterators
//   for (const auto& [name, age] : ages) { ... }   // std::map iterates in key order
//   std::to_string(age)

#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "check.h"

std::vector<int> uniqueSorted(const std::vector<int>& v) {
    (void)v;
    // TODO
    return {-1};
}

int smallestAtLeast(const std::set<int>& s, int x) {
    (void)s; (void)x;
    // TODO
    return -2;
}

int countInRange(const std::set<int>& s, int lo, int hi) {
    (void)s; (void)lo; (void)hi;
    // TODO
    return -1;
}

std::vector<std::string> formatAges(const std::map<std::string, int>& ages) {
    (void)ages;
    // TODO
    return {"?"};
}

int main() {
    CHECK_EQ(uniqueSorted({}), std::vector<int>{});
    CHECK_EQ(uniqueSorted({5}), std::vector<int>{5});
    CHECK_EQ(uniqueSorted({3, 1, 3, 2, 1}), std::vector<int>{1, 2, 3});

    CHECK_EQ(smallestAtLeast({}, 1), -1);
    CHECK_EQ(smallestAtLeast({10, 20, 30}, 15), 20);
    CHECK_EQ(smallestAtLeast({10, 20, 30}, 20), 20);
    CHECK_EQ(smallestAtLeast({10, 20, 30}, 31), -1);
    CHECK_EQ(smallestAtLeast({10, 20, 30}, 0), 10);

    CHECK_EQ(countInRange({}, 0, 10), 0);
    CHECK_EQ(countInRange({1, 3, 5, 7, 9}, 3, 7), 3);
    CHECK_EQ(countInRange({1, 3, 5, 7, 9}, 4, 6), 1);
    CHECK_EQ(countInRange({1, 3, 5, 7, 9}, 8, 8), 0);
    CHECK_EQ(countInRange({1, 3, 5, 7, 9}, 7, 3), 0);

    CHECK_EQ(formatAges({}), std::vector<std::string>{});
    CHECK_EQ(formatAges({{"bob", 30}, {"al", 25}}), (std::vector<std::string>{"al=25", "bob=30"}));

    return check::report();
}
