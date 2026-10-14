// Problem: Group Anagrams
// Group the given words so that words that are anagrams of each other (same letters
// with the same multiplicities, any order) end up in the same group.
//
// Output order (so results can be compared exactly):
//   - within a group, words appear in input order (duplicates are kept);
//   - groups are ordered by the position of their first word in the input.
//
// Examples:
//   {"eat","tea","tan","ate","nat","bat"} -> {{"eat","tea","ate"},{"tan","nat"},{"bat"}}
//   {""}        -> {{""}}
//   {}          -> {}
//
// Constraints: 0 <= words.size() <= 100,000; total characters <= 2,000,000;
//              words contain lowercase letters only (may be empty).
// Required complexity: O(total characters * log(max word length)) or better.
//
// Hint: two words are anagrams exactly when some canonical form of them is equal.
// Think about what that key could be (a sorted copy of the word works; a 26-count
// signature is faster), then use a hash map from key to group index so groups come
// out in first-appearance order.
//
// Run: make run FILE=curriculum/102_fundamentals/hashing/02_group_anagrams.cpp

#include "check.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

using Groups = std::vector<std::vector<std::string>>;

Groups groupAnagrams(const std::vector<std::string>& words) {
    Groups groups;
    std::unordered_map<std::string, size_t> idx;
    for (const auto& w : words) {
        std::string key = w;
        std::sort(key.begin(), key.end());
        auto it = idx.find(key);
        if (it == idx.end()) {
            idx[key] = groups.size();
            groups.push_back({w});
        } else {
            groups[it->second].push_back(w);
        }
    }
    return groups;
}

int main() {
    using V = std::vector<std::string>;
    CHECK_EQ(groupAnagrams({"eat", "tea", "tan", "ate", "nat", "bat"}),
             (Groups{{"eat", "tea", "ate"}, {"tan", "nat"}, {"bat"}}));
    CHECK_EQ(groupAnagrams({}), Groups{});
    CHECK_EQ(groupAnagrams({""}), (Groups{{""}}));
    CHECK_EQ(groupAnagrams({"", ""}), (Groups{{"", ""}}));
    CHECK_EQ(groupAnagrams({"a"}), (Groups{{"a"}}));
    CHECK_EQ(groupAnagrams({"ab", "ba", "abc", "cab", "bca", "b"}),
             (Groups{{"ab", "ba"}, {"abc", "cab", "bca"}, {"b"}}));
    // Same letters but different multiplicities are NOT anagrams.
    CHECK_EQ(groupAnagrams({"aab", "aba", "baa", "abb"}), (Groups{{"aab", "aba", "baa"}, {"abb"}}));
    CHECK_EQ(groupAnagrams({"aa", "a", "aaa", "a"}), (Groups{{"aa"}, {"a", "a"}, {"aaa"}}));
    CHECK_EQ(groupAnagrams({"abc", "abc", "abc"}), (Groups{{"abc", "abc", "abc"}}));
    CHECK_EQ(groupAnagrams({"listen", "silent", "enlist", "google", "gogole", "cat"}),
             (Groups{{"listen", "silent", "enlist"}, {"google", "gogole"}, {"cat"}}));

    // 1000 groups of 2 long words: w_i = a^i b^(1000-i), then every reversal.
    {
        V words;
        Groups expected(1000);
        for (int i = 0; i < 1000; i++) {
            std::string w = std::string(i, 'a') + std::string(1000 - i, 'b');
            words.push_back(w);
            expected[i].push_back(w);
        }
        for (int i = 0; i < 1000; i++) {
            std::string w = words[i];
            std::reverse(w.begin(), w.end());
            words.push_back(w);
            expected[i].push_back(w);
        }
        CHECK_EQ(groupAnagrams(words) == expected, true);
    }
    // Many short words: 100000 words, each repeated letters pattern; 10 groups.
    {
        V words;
        Groups expected(10);
        for (int i = 0; i < 100000; i++) {
            int g = i % 10;
            // group g = words made of letters {g, g+1, g+2}; rotate them by i.
            std::string base;
            base += static_cast<char>('a' + g);
            base += static_cast<char>('a' + g + 1);
            base += static_cast<char>('a' + g + 2);
            std::rotate(base.begin(), base.begin() + (i / 10) % 3, base.end());
            words.push_back(base);
            expected[g].push_back(base);
        }
        CHECK_EQ(groupAnagrams(words) == expected, true);
    }

    return check::report();
}
