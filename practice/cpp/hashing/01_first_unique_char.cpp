// Problem: First Unique Character
// Given a string s of lowercase letters 'a'-'z', return the index of the first
// character that occurs exactly once in s, or -1 if no such character exists.
//
// Examples:
//   "swiss"   -> 1   ('w' is the first character with a count of 1)
//   "abcabcd" -> 6
//   "aabb"    -> -1
//   ""        -> -1
//
// Constraints: 0 <= s.size() <= 1,000,000.
// Required complexity: O(n) time, O(1) extra space (the alphabet is fixed at 26).
//
// Hint: one pass to count how often each letter occurs, then a second pass over
// the string (not over the counts) to find the first letter whose count is 1.
//
// Run: make run FILE=curriculum/102_fundamentals/hashing/01_first_unique_char.cpp

#include "check.h"

#include <array>
#include <string>

//@REF
int firstUniqueChar(const std::string& s) {
    std::array<int, 26> cnt{};
    for (char c : s) cnt[c - 'a']++;
    for (size_t i = 0; i < s.size(); i++)
        if (cnt[s[i] - 'a'] == 1) return static_cast<int>(i);
    return -1;
}
int firstUniqueChar([[maybe_unused]] const std::string& s) {
    // TODO
    return -2;
}

int main() {
    CHECK_EQ(firstUniqueChar(""), -1);
    CHECK_EQ(firstUniqueChar("z"), 0);
    CHECK_EQ(firstUniqueChar("aabb"), -1);
    CHECK_EQ(firstUniqueChar("swiss"), 1);
    CHECK_EQ(firstUniqueChar("abcabcd"), 6);
    CHECK_EQ(firstUniqueChar("aabbc"), 4);
    CHECK_EQ(firstUniqueChar("abab"), -1);
    CHECK_EQ(firstUniqueChar("zyx"), 0);
    CHECK_EQ(firstUniqueChar("aaabccc"), 3);

    // Larger inputs: a quadratic "count each char by rescanning" approach is too slow.
    CHECK_EQ(firstUniqueChar(std::string(1000000, 'x')), -1);
    CHECK_EQ(firstUniqueChar(std::string(400000, 'a') + "b" + std::string(400000, 'a')), 400000);
    CHECK_EQ(firstUniqueChar(std::string(300000, 'a') + std::string(300000, 'b') + "c"), 600000);

    return check::report();
}
