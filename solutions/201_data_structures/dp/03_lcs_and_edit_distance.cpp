// Problem: Two-string dynamic programming: longest common subsequence, its reconstruction, and edit distance.
//   int         lcsLength(const std::string& a, const std::string& b)
//   std::string lcs(const std::string& a, const std::string& b)
//        One actual longest common subsequence (any one if several tie). The tests check that it is a subsequence
//        of both strings and has the optimal length; they do not demand one particular string.
//   int         editDistance(const std::string& a, const std::string& b)
//        Minimum number of single-character insert / delete / replace operations that turn a into b.
//
// Examples:
//   lcsLength("abcde", "ace") = 3          lcs("abcde", "ace") = "ace"
//   lcsLength("abc", "def") = 0            editDistance("kitten", "sitting") = 3
//   editDistance("", "abc") = 3            editDistance("intention", "execution") = 5
//
// Constraints: strings of length up to 3,000 each (lowercase letters in the tests, but do not rely on it).
//   For lcs() the tests stay at 2,000 x 2,000.
// Required complexity: O(|a| * |b|) time. lcsLength and editDistance should aim for O(min(|a|,|b|)) extra space;
//   lcs() may use the full table. Plain recursion without memoization is exponential and will not finish.
// Run: make run FILE=curriculum/201_data_structures/dp/03_lcs_and_edit_distance.cpp
//
// Think first:
//  1. What does dp[i][j] mean in words (prefixes of length i and j)? What are dp[0][j] and dp[i][0]?
//  2. When a[i-1] == b[j-1], is it ever worse to take the match? When they differ, which neighbours do you
//     consult, and how does that change between LCS and edit distance?
//  3. To rebuild the string you walk the table backwards. What decides each step, and why does that need the
//     table you were tempted to throw away to save memory?

#include "check.h"

#include <algorithm>
#include <string>
#include <vector>

// ===== YOUR CODE BELOW (stub) =====
int lcsLength(const std::string& a, const std::string& b) {
    const std::string& s = a.size() < b.size() ? a : b;
    const std::string& t = a.size() < b.size() ? b : a;
    std::vector<int> prev(s.size() + 1, 0), cur(s.size() + 1, 0);
    for (size_t i = 1; i <= t.size(); i++) {
        for (size_t j = 1; j <= s.size(); j++)
            cur[j] = t[i-1] == s[j-1] ? prev[j-1] + 1 : std::max(prev[j], cur[j-1]);
        std::swap(prev, cur);
    }
    return prev[s.size()];
}
std::string lcs(const std::string& a, const std::string& b) {
    size_t n = a.size(), m = b.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    for (size_t i = 1; i <= n; i++)
        for (size_t j = 1; j <= m; j++)
            dp[i][j] = a[i-1] == b[j-1] ? dp[i-1][j-1] + 1 : std::max(dp[i-1][j], dp[i][j-1]);
    std::string out;
    size_t i = n, j = m;
    while (i > 0 && j > 0) {
        if (a[i-1] == b[j-1]) { out += a[i-1]; i--; j--; }
        else if (dp[i-1][j] >= dp[i][j-1]) i--;
        else j--;
    }
    std::reverse(out.begin(), out.end());
    return out;
}
int editDistance(const std::string& a, const std::string& b) {
    std::vector<int> prev(b.size() + 1), cur(b.size() + 1);
    for (size_t j = 0; j <= b.size(); j++) prev[j] = (int)j;
    for (size_t i = 1; i <= a.size(); i++) {
        cur[0] = (int)i;
        for (size_t j = 1; j <= b.size(); j++)
            cur[j] = std::min({prev[j] + 1, cur[j-1] + 1, prev[j-1] + (a[i-1] != b[j-1])});
        std::swap(prev, cur);
    }
    return prev[b.size()];
}
// ===== TESTS (do not edit below) =====
static bool isSubsequence(const std::string& s, const std::string& t) {
    size_t i = 0;
    for (char c : t)
        if (i < s.size() && s[i] == c) i++;
    return i == s.size();
}
static void checkLcs(const std::string& a, const std::string& b, int expectedLen, int line) {
    std::string s = lcs(a, b);
    check::total++;
    if ((int)s.size() != expectedLen || !isSubsequence(s, a) || !isSubsequence(s, b))
        check::fail(__FILE__, line,
                    "lcs(...) returned \"" + s.substr(0, 40) + "\" (len " + std::to_string(s.size()) +
                        "), expected a common subsequence of length " + std::to_string(expectedLen));
}
#define CHECK_LCS(a, b, len) checkLcs((a), (b), (len), __LINE__)

int main() {
    // lcsLength
    CHECK_EQ(lcsLength("", ""), 0);
    CHECK_EQ(lcsLength("abc", ""), 0);
    CHECK_EQ(lcsLength("", "abc"), 0);
    CHECK_EQ(lcsLength("abcde", "ace"), 3);
    CHECK_EQ(lcsLength("abc", "abc"), 3);
    CHECK_EQ(lcsLength("abc", "def"), 0);
    CHECK_EQ(lcsLength("AGGTAB", "GXTXAYB"), 4);  // GTAB
    CHECK_EQ(lcsLength("aaaa", "aa"), 2);
    CHECK_EQ(lcsLength("abcd", "dcba"), 1);
    CHECK_EQ(lcsLength("bsbininm", "jmjkbkjkv"), 1);

    // lcs reconstruction
    CHECK_LCS("", "", 0);
    CHECK_LCS("abc", "def", 0);
    CHECK_LCS("abcde", "ace", 3);
    CHECK_EQ(lcs("abcde", "ace"), std::string("ace"));  // unique answer
    CHECK_LCS("AGGTAB", "GXTXAYB", 4);
    CHECK_LCS("abcd", "dcba", 1);
    CHECK_LCS("aabbcc", "abcabc", 4);
    CHECK_LCS("xyxyxy", "yxyxyx", 5);

    // editDistance
    CHECK_EQ(editDistance("", ""), 0);
    CHECK_EQ(editDistance("", "abc"), 3);
    CHECK_EQ(editDistance("abc", ""), 3);
    CHECK_EQ(editDistance("abc", "abc"), 0);
    CHECK_EQ(editDistance("kitten", "sitting"), 3);
    CHECK_EQ(editDistance("horse", "ros"), 3);
    CHECK_EQ(editDistance("intention", "execution"), 5);
    CHECK_EQ(editDistance("a", "b"), 1);
    CHECK_EQ(editDistance("sunday", "saturday"), 3);
    CHECK_EQ(editDistance("abc", "cba"), 2);

    // large: "abab..." vs "baba..." -- shifting by one is optimal
    {
        std::string a, b;
        for (int i = 0; i < 1500; i++) {
            a += "ab";
            b += "ba";
        }
        CHECK_EQ(lcsLength(a, b), 2999);
        CHECK_EQ(editDistance(a, b), 2);  // delete the first 'a', append an 'a'
        CHECK_EQ(lcsLength(a, a), 3000);
        CHECK_EQ(editDistance(a, a), 0);
        CHECK_EQ(editDistance(a, std::string(3000, 'z')), 3000);  // every position replaced
        CHECK_EQ(lcsLength(a, std::string(3000, 'z')), 0);
    }
    {
        std::string a, b;
        for (int i = 0; i < 1000; i++) {
            a += "ab";
            b += "ba";
        }
        CHECK_LCS(a, b, 1999);
        CHECK_LCS(a, a, 2000);
    }
    return check::report();
}
