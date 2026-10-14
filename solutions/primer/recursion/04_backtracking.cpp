// Problem: Two backtracking generators.
//   generateParens(n)    every well-formed string of n pairs of parentheses (n <= 4);
//                        any order
//   permutations(s)      every arrangement of the characters of s (all characters
//                        distinct); any order
//
// Examples:
//   generateParens(2)    -> "(())", "()()"
//   permutations("ab")   -> "ab", "ba"
//
// Edge cases: n = 0 gives one empty string; permutations("") gives one empty string.
//             Counts: generateParens(3) has 5 results, permutations("abcd") has 24.
//
// Run with: make run FILE=practice/cpp/recursion/04_backtracking.cpp
//
// Syntax reminder:
//   void gen(int open, int close, int n, std::string& cur, std::vector<std::string>& out);
//   cur.push_back('(');  gen(...);  cur.pop_back();      // choose, recurse, un-choose
//   // rule: add '(' while open < n; add ')' while close < open
//   std::swap(s[i], s[j]);                               // in-place permutation by swapping
//   std::sort(out.begin(), out.end());                   // <algorithm>

#include <algorithm>
#include <string>
#include <vector>

#include "check.h"

static void gen(int open, int close, int n, std::string& cur, std::vector<std::string>& out) {
    if (static_cast<int>(cur.size()) == 2 * n) {
        out.push_back(cur);
        return;
    }
    if (open < n) {
        cur.push_back('(');
        gen(open + 1, close, n, cur, out);
        cur.pop_back();
    }
    if (close < open) {
        cur.push_back(')');
        gen(open, close + 1, n, cur, out);
        cur.pop_back();
    }
}

std::vector<std::string> generateParens(int n) {
    std::vector<std::string> out;
    std::string cur;
    gen(0, 0, n, cur, out);
    return out;
}

static void permute(std::string& s, size_t k, std::vector<std::string>& out) {
    if (k == s.size()) {
        out.push_back(s);
        return;
    }
    for (size_t i = k; i < s.size(); i++) {
        std::swap(s[k], s[i]);
        permute(s, k + 1, out);
        std::swap(s[k], s[i]);
    }
}

std::vector<std::string> permutations(std::string s) {
    std::vector<std::string> out;
    permute(s, 0, out);
    return out;
}

static std::vector<std::string> sorted(std::vector<std::string> v) {
    std::sort(v.begin(), v.end());
    return v;
}

int main() {
    CHECK_EQ(sorted(generateParens(0)), std::vector<std::string>{""});
    CHECK_EQ(sorted(generateParens(1)), std::vector<std::string>{"()"});
    CHECK_EQ(sorted(generateParens(2)), (std::vector<std::string>{"(())", "()()"}));
    CHECK_EQ(sorted(generateParens(3)),
             (std::vector<std::string>{"((()))", "(()())", "(())()", "()(())", "()()()"}));
    CHECK_EQ(generateParens(4).size(), size_t{14});

    CHECK_EQ(sorted(permutations("")), std::vector<std::string>{""});
    CHECK_EQ(sorted(permutations("a")), std::vector<std::string>{"a"});
    CHECK_EQ(sorted(permutations("ab")), (std::vector<std::string>{"ab", "ba"}));
    CHECK_EQ(sorted(permutations("abc")),
             (std::vector<std::string>{"abc", "acb", "bac", "bca", "cab", "cba"}));
    CHECK_EQ(permutations("abcd").size(), size_t{24});

    return check::report();
}
