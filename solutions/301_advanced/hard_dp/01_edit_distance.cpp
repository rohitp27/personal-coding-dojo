// 301 advanced > hard_dp > 01 Edit distance (canonical form)
//
// Problem:
//   Return the minimum number of single-character operations (insert a
//   character, delete a character, replace a character) needed to turn string
//   `a` into string `b`.
//
// Examples:
//   ("kitten", "sitting")     -> 3
//   ("intention", "execution") -> 5
//   ("abc", "")               -> 3
//
// Constraints:
//   0 <= |a|, |b| <= 3000, arbitrary bytes (not just letters).
//
// Required complexity: O(|a| * |b|) time, and O(min(|a|, |b|)) extra space
//   (keeping the full table works for the tests, but state your plan for the
//   rolling version in your comment block).
//
// Assumptions: all three operations cost 1.
//
// Hint: let dp[i][j] be the answer for the first i characters of a and the first
//       j characters of b. What are the three ways the last characters can line up?
//
// Think first:
//   1. Define the state in words and give the base cases (empty prefix on either
//      side) before writing a loop.
//   2. Why is "the last characters match, so take dp[i-1][j-1]" always optimal
//      and never worse than using an edit there?
//   3. Which cells does dp[i][j] read, and what does that imply for the rolling
//      array and its iteration order?
//
// BEFORE CODING: write your approach, invariant and complexity in a comment
// block right here.
//
// Run: make run FILE=curriculum/301_advanced/hard_dp/01_edit_distance.cpp

#include "check.h"

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

// ===== YOUR CODE BELOW =====
int editDistance(const std::string& a, const std::string& b) {
    const std::string& s = a.size() >= b.size() ? a : b;  // longer: rows
    const std::string& t = a.size() >= b.size() ? b : a;  // shorter: columns
    std::vector<int> prev(t.size() + 1), cur(t.size() + 1);
    for (size_t j = 0; j <= t.size(); j++) prev[j] = (int)j;
    for (size_t i = 1; i <= s.size(); i++) {
        cur[0] = (int)i;
        for (size_t j = 1; j <= t.size(); j++) {
            if (s[i - 1] == t[j - 1]) cur[j] = prev[j - 1];
            else cur[j] = 1 + std::min({prev[j], cur[j - 1], prev[j - 1]});
        }
        std::swap(prev, cur);
    }
    return prev[t.size()];
}
// ===== TESTS (do not edit) =====
struct Rng {
    uint64_t s;
    uint64_t next() {
        s = s * 6364136223846793005ULL + 1442695040888963407ULL;
        return s >> 33;
    }
};

// Independent top-down formulation for the random comparison.
static int topDown(const std::string& a, const std::string& b, size_t i, size_t j,
                   std::vector<std::vector<int>>& memo) {
    if (i == a.size()) return (int)(b.size() - j);
    if (j == b.size()) return (int)(a.size() - i);
    int& m = memo[i][j];
    if (m >= 0) return m;
    if (a[i] == b[j]) return m = topDown(a, b, i + 1, j + 1, memo);
    return m = 1 + std::min({topDown(a, b, i + 1, j, memo),        // delete from a
                             topDown(a, b, i, j + 1, memo),        // insert into a
                             topDown(a, b, i + 1, j + 1, memo)});  // replace
}

int main() {
    CHECK_EQ(editDistance("kitten", "sitting"), 3);
    CHECK_EQ(editDistance("intention", "execution"), 5);
    CHECK_EQ(editDistance("horse", "ros"), 3);
    CHECK_EQ(editDistance("", ""), 0);
    CHECK_EQ(editDistance("abc", ""), 3);
    CHECK_EQ(editDistance("", "abc"), 3);
    CHECK_EQ(editDistance("same", "same"), 0);
    CHECK_EQ(editDistance("a", "b"), 1);
    CHECK_EQ(editDistance("abc", "yabd"), 2);
    CHECK_EQ(editDistance("sunday", "saturday"), 3);
    CHECK_EQ(editDistance("abcdef", "azced"), 3);
    CHECK_EQ(editDistance(std::string("a\0b", 3), std::string("a\0c", 3)), 1);  // arbitrary bytes
    CHECK_EQ(editDistance("ab", "ba"), 2);  // no transposition operation

    Rng rng{8};
    for (int trial = 0; trial < 500; trial++) {
        std::string a, b;
        int la = (int)(rng.next() % 8), lb = (int)(rng.next() % 8);
        for (int i = 0; i < la; i++) a += (char)('a' + rng.next() % 3);
        for (int i = 0; i < lb; i++) b += (char)('a' + rng.next() % 3);
        std::vector<std::vector<int>> memo(a.size(), std::vector<int>(b.size(), -1));
        CHECK_EQ(editDistance(a, b), topDown(a, b, 0, 0, memo));
        CHECK_EQ(editDistance(b, a), editDistance(a, b));  // symmetric
    }

    // large: a = 3000 x 'a', b = 2000 x 'a' + 500 x 'b'
    // lower bound: they share at most 2000 aligned characters, and |a| = 3000, so >= 1000 edits;
    // 500 replacements + 500 deletions achieve it.
    {
        std::string a(3000, 'a');
        std::string b = std::string(2000, 'a') + std::string(500, 'b');
        CHECK_EQ(editDistance(a, b), 1000);
        CHECK_EQ(editDistance(a, a), 0);
        CHECK_EQ(editDistance(a, std::string(3000, 'b')), 3000);
        CHECK_EQ(editDistance(a, ""), 3000);
    }

    return check::report();
}
