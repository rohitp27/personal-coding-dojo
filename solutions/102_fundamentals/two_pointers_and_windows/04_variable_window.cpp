// Problem: Minimum Window Containing All Required Characters
// Given strings s and t, return the shortest substring of s that contains every
// character of t, counting multiplicity (if t has two 'a's the window needs at least
// two 'a's). If several shortest windows exist, return the leftmost one. If no window
// exists, or t is empty, return "".
//
// Examples:
//   s = "ADOBECODEBANC", t = "ABC" -> "BANC"
//   s = "a",             t = "aa"  -> ""
//   s = "abxba",         t = "ab"  -> "ab"   (ties: "ab" at 0 and "ba" at 3, leftmost wins)
//   s = "bbaa",          t = "aba" -> "baa"
//
// Constraints: 0 <= s.size(), t.size() <= 1,000,000; characters are bytes (any char).
// Required complexity: O(|s| + |t|) time, O(1) extra space (fixed 256-entry tables).
// Checking all substrings is quadratic or worse and too slow for the larger tests.
//
// Think first:
//   1. What is the invariant for "the window is currently valid", and how can you
//      maintain it with ONE counter instead of re-comparing two tables on every step?
//   2. When the window is valid, why is shrinking from the left safe, and when do you
//      stop shrinking?
//   3. How do you guarantee the LEFTMOST shortest window on ties, and what must you
//      return when t is longer than s or t is empty?
//
// Run: make run FILE=curriculum/102_fundamentals/two_pointers_and_windows/04_variable_window.cpp

#include "check.h"

#include <array>
#include <string>

std::string minWindow(const std::string& s, const std::string& t) {
    if (t.empty() || t.size() > s.size()) return "";
    std::array<int, 256> need{};
    int missing = static_cast<int>(t.size());
    for (unsigned char c : t) need[c]++;
    size_t bestStart = 0, bestLen = s.size() + 1, left = 0;
    for (size_t r = 0; r < s.size(); r++) {
        unsigned char c = s[r];
        if (need[c]-- > 0) missing--;
        while (missing == 0) {
            if (r - left + 1 < bestLen) {
                bestLen = r - left + 1;
                bestStart = left;
            }
            unsigned char lc = s[left++];
            if (++need[lc] > 0) missing++;
        }
    }
    return bestLen > s.size() ? "" : s.substr(bestStart, bestLen);
}

int main() {
    CHECK_EQ(minWindow("ADOBECODEBANC", "ABC"), "BANC");
    CHECK_EQ(minWindow("a", "a"), "a");
    CHECK_EQ(minWindow("a", "aa"), "");
    CHECK_EQ(minWindow("ab", "b"), "b");
    CHECK_EQ(minWindow("abc", ""), "");
    CHECK_EQ(minWindow("", "a"), "");
    CHECK_EQ(minWindow("", ""), "");
    CHECK_EQ(minWindow("abcabc", "cab"), "abc");
    CHECK_EQ(minWindow("aab", "ab"), "ab");
    CHECK_EQ(minWindow("bbaa", "aba"), "baa");
    CHECK_EQ(minWindow("abxba", "ab"), "ab");
    CHECK_EQ(minWindow("xyz", "w"), "");
    CHECK_EQ(minWindow("aaflslflsldkalskaaa", "aaa"), "aaa");
    CHECK_EQ(minWindow("abc", "abcd"), "");
    CHECK_EQ(minWindow("a b", "b "), " b");  // spaces are ordinary characters

    // Large inputs: examining all substrings is far too slow.
    CHECK_EQ(minWindow(std::string(300000, 'a') + "bc" + std::string(300000, 'a'), "abc"), "abc");
    CHECK_EQ(minWindow(std::string(400000, 'x'), "y"), "");
    CHECK_EQ(minWindow(std::string(300000, 'a') + std::string(300000, 'b'), "ab"), "ab");
    CHECK_EQ(minWindow(std::string(300000, 'b') + std::string(300000, 'a'), "ab"), "ba");
    CHECK_EQ(minWindow(std::string(500000, 'a'), std::string(500000, 'a')), std::string(500000, 'a'));
    CHECK_EQ(minWindow(std::string(500000, 'a'), std::string(500001, 'a')), "");

    return check::report();
}
