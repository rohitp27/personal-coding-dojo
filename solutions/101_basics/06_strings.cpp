/*
 * Problem: Strings - palindromes, counting, frequencies
 *   Practice std::string indexing, <cctype> helpers, and std::map.
 *
 *     isPalindrome(s)     True if s reads the same forwards and backwards, considering
 *                         only letters and digits and ignoring case.
 *     countVowels(s)      Number of a, e, i, o, u (either case). 'y' is not a vowel.
 *     charFrequency(s)    Map from each character to how many times it appears
 *                         (every char counts, including spaces and punctuation).
 *     firstUniqueChar(s)  Index of the first character that appears exactly once,
 *                         or -1 if there is none.
 *     isAnagram(a, b)     True if b is a rearrangement of a (case-sensitive, every
 *                         character counts).
 *
 * Examples:
 *   isPalindrome("A man, a plan, a canal: Panama") -> true
 *   countVowels("Hello World")                     -> 3
 *   charFrequency("aab")                           -> {a: 2, b: 1}
 *   firstUniqueChar("swiss")                       -> 1      ('w')
 *   isAnagram("listen", "silent")                  -> true
 *
 * Constraints / edge cases:
 *   - Empty string: palindrome (true), 0 vowels, empty map, -1, two empties are anagrams.
 *   - A string with no alphanumerics (".,") counts as a palindrome.
 *   - Pass unsigned char to std::isalnum / std::tolower to avoid UB for negative chars.
 *   - firstUniqueChar must return an int index, not a char.
 *
 * Target complexity: O(n) time; O(1) extra space apart from the map (alphabet is bounded).
 *
 * Run: make run FILE=curriculum/101_basics/06_strings.cpp
 *
 * (Stub note: delete the `(void)x;` lines once you use the parameter.)
 */
#include <cctype>
#include <iostream>
#include <map>
#include <string>

#include <algorithm>
#include <cstdlib>
#include <cctype>
#include "check.h"

// ===================== YOUR CODE =====================

bool isPalindrome(const std::string& s) {
    int i = 0, j = (int)s.size() - 1;
    while (i < j) {
        while (i < j && !std::isalnum((unsigned char)s[i])) i++;
        while (i < j && !std::isalnum((unsigned char)s[j])) j--;
        if (std::tolower((unsigned char)s[i]) != std::tolower((unsigned char)s[j])) return false;
        i++; j--;
    }
    return true;
}
int countVowels(const std::string& s) {
    int c = 0;
    for (char ch : s) {
        int l = std::tolower((unsigned char)ch);
        if (l == 'a' || l == 'e' || l == 'i' || l == 'o' || l == 'u') c++;
    }
    return c;
}
std::map<char, int> charFrequency(const std::string& s) {
    std::map<char, int> f;
    for (char c : s) f[c]++;
    return f;
}
int firstUniqueChar(const std::string& s) {
    auto f = charFrequency(s);
    for (size_t i = 0; i < s.size(); i++) if (f[s[i]] == 1) return (int)i;
    return -1;
}
bool isAnagram(const std::string& a, const std::string& b) {
    return a.size() == b.size() && charFrequency(a) == charFrequency(b);
}

// ===================== TESTS =====================

int main() {
    // isPalindrome
    CHECK_EQ(isPalindrome("A man, a plan, a canal: Panama"), true);
    CHECK_EQ(isPalindrome(""), true);
    CHECK_EQ(isPalindrome("a"), true);
    CHECK_EQ(isPalindrome(".,"), true);
    CHECK_EQ(isPalindrome("race a car"), false);
    CHECK_EQ(isPalindrome("0P"), false);
    CHECK_EQ(isPalindrome("ab"), false);
    CHECK_EQ(isPalindrome("Aa"), true);
    CHECK_EQ(isPalindrome("abba"), true);
    CHECK_EQ(isPalindrome("abcba"), true);
    CHECK_EQ(isPalindrome("12321"), true);
    CHECK_EQ(isPalindrome("1a2"), false);
    CHECK_EQ(isPalindrome("  x  "), true);

    // countVowels
    CHECK_EQ(countVowels("Hello World"), 3);
    CHECK_EQ(countVowels(""), 0);
    CHECK_EQ(countVowels("rhythm"), 0);
    CHECK_EQ(countVowels("AEIOU"), 5);
    CHECK_EQ(countVowels("aeiou"), 5);
    CHECK_EQ(countVowels("queue"), 4);
    CHECK_EQ(countVowels("xyz 123 !?"), 0);
    CHECK_EQ(countVowels("banana"), 3);

    // charFrequency
    CHECK_EQ(charFrequency(""), std::map<char, int>{});
    CHECK_EQ(charFrequency("a"), (std::map<char, int>{{'a', 1}}));
    CHECK_EQ(charFrequency("aab"), (std::map<char, int>{{'a', 2}, {'b', 1}}));
    CHECK_EQ(charFrequency("a b a"), (std::map<char, int>{{' ', 2}, {'a', 2}, {'b', 1}}));
    CHECK_EQ(charFrequency("Aa"), (std::map<char, int>{{'A', 1}, {'a', 1}}));
    CHECK_EQ(charFrequency("zzzz"), (std::map<char, int>{{'z', 4}}));

    // firstUniqueChar
    CHECK_EQ(firstUniqueChar("swiss"), 1);
    CHECK_EQ(firstUniqueChar("abcabcd"), 6);
    CHECK_EQ(firstUniqueChar("aabb"), -1);
    CHECK_EQ(firstUniqueChar(""), -1);
    CHECK_EQ(firstUniqueChar("z"), 0);
    CHECK_EQ(firstUniqueChar("aabbc"), 4);
    CHECK_EQ(firstUniqueChar("abab c"), 4);  // the space is the unique char
    CHECK_EQ(firstUniqueChar("Aa"), 0);

    // isAnagram
    CHECK_EQ(isAnagram("listen", "silent"), true);
    CHECK_EQ(isAnagram("", ""), true);
    CHECK_EQ(isAnagram("a", "ab"), false);
    CHECK_EQ(isAnagram("rat", "car"), false);
    CHECK_EQ(isAnagram("aab", "abb"), false);
    CHECK_EQ(isAnagram("abc", "abc"), true);
    CHECK_EQ(isAnagram("Abc", "abc"), false);
    CHECK_EQ(isAnagram("a b", "b a"), true);

    return check::report();
}
