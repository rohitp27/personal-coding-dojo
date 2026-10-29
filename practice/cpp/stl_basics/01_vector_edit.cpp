// Problem: Four small vector edits.
//   sum(v)                          total of all elements (0 for empty)
//   removeValue(v, x)               copy of v with every x removed, order kept
//   insertAfterFirst(v, target, val) insert val right after the first target;
//                                   if target is absent, append val at the end
//   sortedDesc(v)                   copy of v sorted from largest to smallest
//
// Examples:
//   removeValue({1,2,1,3}, 1)            -> {2,3}
//   insertAfterFirst({5,6,5}, 5, 9)      -> {5,9,6,5}
//   sortedDesc({3,1,2})                  -> {3,2,1}
//
// Edge cases: empty vector, single element, value not present, duplicates.
//
// Run with: make run FILE=practice/cpp/stl_basics/01_vector_edit.cpp
//
// Syntax reminder:
//   for (int x : v) total += x;                       // range-for (copy); use const int& to avoid copies
//   v.erase(std::remove(v.begin(), v.end(), x), v.end());   // erase-remove idiom, <algorithm>
//   auto it = std::find(v.begin(), v.end(), t);       // it == v.end() if not found
//   v.insert(it, val);                                // inserts BEFORE it; returns iterator to new element
//   v.push_back(val);
//   std::sort(v.begin(), v.end(), std::greater<int>());   // or a lambda: [](int a, int b) { return a > b; }

#include <algorithm>
#include <functional>
#include <vector>

#include "check.h"

int sum(const std::vector<int>& v) {
    (void)v;
    // TODO
    int sum = 0;
    if (v.empty()) {
        return sum;
    }
    for (int elem : v) {
        sum += elem;
    }
    return sum;
}

std::vector<int> removeValue(std::vector<int> v, int x) {
    (void)v; (void)x;
    // TODO
    std::vector<int> output;
    for (int elem : v ) {
        if (elem != x ) {
            output.push_back(elem);
        }
    }
    return output;
}

std::vector<int> insertAfterFirst(std::vector<int> v, int target, int val) {
    (void)v; (void)target; (void)val;
    // TODO
    bool found = false;
    std::vector<int> output;
    for(int elem : v) {
        output.push_back(elem);
        if (!found && elem == target){
            output.push_back(val);
            found = true;
        }
    }
    if (!found){
        output.push_back(val);
        found = true;
    }
    return output;
}

std::vector<int> sortedDesc(std::vector<int> v) {
    (void)v;
    // TODO
    std::vector<int> output(v);
    std::sort(output.begin(), output.end(), [](const int& a, const int& b) -> bool {return a>b;});
    return output;
}

int main() {
    CHECK_EQ(sum({}), 0);
    CHECK_EQ(sum({4}), 4);
    CHECK_EQ(sum({1, 2, 3, -1}), 5);

    CHECK_EQ(removeValue({}, 1), std::vector<int>{});
    CHECK_EQ(removeValue({1, 2, 1, 3}, 1), std::vector<int>{2, 3});
    CHECK_EQ(removeValue({1, 1}, 1), std::vector<int>{});
    CHECK_EQ(removeValue({1, 2}, 7), std::vector<int>{1, 2});

    CHECK_EQ(insertAfterFirst({}, 5, 9), std::vector<int>{9});
    CHECK_EQ(insertAfterFirst({5, 6, 5}, 5, 9), std::vector<int>{5, 9, 6, 5});
    CHECK_EQ(insertAfterFirst({1, 2, 3}, 3, 9), std::vector<int>{1, 2, 3, 9});
    CHECK_EQ(insertAfterFirst({1, 2, 3}, 7, 9), std::vector<int>{1, 2, 3, 9});

    CHECK_EQ(sortedDesc({}), std::vector<int>{});
    CHECK_EQ(sortedDesc({7}), std::vector<int>{7});
    CHECK_EQ(sortedDesc({3, 1, 2, 3}), std::vector<int>{3, 3, 2, 1});

    return check::report();
}
