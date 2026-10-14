// 301 advanced > advanced_structures > 01 Trie with prefix counts (canonical form)
//
// Problem:
//   Implement a multiset-of-words Trie over lowercase letters a-z:
//     insert(word)            add one occurrence (the empty word is allowed)
//     erase(word)             remove ONE occurrence; true if it was present
//     countEqual(word)        number of occurrences of exactly this word
//     countPrefix(prefix)     number of stored word occurrences that START with prefix
//                             (a word counts for itself; the empty prefix counts everything)
//     autocomplete(prefix, k) up to k DISTINCT stored words that start with prefix,
//                             in lexicographic order (shorter-first when one is a prefix of another)
//
// Examples:
//   insert apple, apple, app, apply, banana
//     countEqual("apple") = 2, countEqual("ap") = 0, countPrefix("app") = 4, countPrefix("") = 5
//     autocomplete("app", 2) = {"app", "apple"}
//   then erase("apple") twice -> autocomplete("app", 10) = {"app", "apply"}
//
// Constraints:
//   Up to 200000 operations; word length <= 50. Words contain only a-z.
//
// Required complexity: insert / erase / countEqual / countPrefix in O(len).
//   autocomplete in O(len(prefix) + work proportional to the nodes it actually
//   visits to produce the k words) - it must NOT walk dead branches left behind
//   by erase, and must not collect every match and sort.
//
// Assumptions: erase of a missing word returns false and changes nothing.
//
// Hint: keep two counters per node. Which two, and which operations update them?
//
// Think first:
//   1. After erase, a node may have no words below it. How do you know that
//      cheaply, and where does autocomplete need that knowledge?
//   2. Array of 26 children vs map: trade-offs in memory and speed here?
//   3. What breaks if you store only an "is end of word" bool?
//
// BEFORE CODING: write your approach, invariant and complexity in a comment
// block right here.
//
// Run: make run FILE=curriculum/301_advanced/advanced_structures/01_trie_prefix_counts.cpp

#include "check.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <string>
#include <vector>

// ===== YOUR CODE BELOW =====
class Trie {
    struct Node {
        std::array<int, 26> ch;
        int pass = 0;  // occurrences in this subtree
        int end = 0;   // occurrences ending exactly here
        Node() { ch.fill(-1); }
    };
    std::vector<Node> nodes{Node()};

    int walk(const std::string& s) const {
        int cur = 0;
        for (char c : s) {
            cur = nodes[cur].ch[c - 'a'];
            if (cur == -1) return -1;
        }
        return cur;
    }
    void collect(int u, std::string& path, int k, std::vector<std::string>& out) const {
        if ((int)out.size() >= k) return;
        if (nodes[u].end > 0) out.push_back(path);
        for (int c = 0; c < 26 && (int)out.size() < k; c++) {
            int v = nodes[u].ch[c];
            if (v == -1 || nodes[v].pass == 0) continue;
            path.push_back((char)('a' + c));
            collect(v, path, k, out);
            path.pop_back();
        }
    }

public:
    void insert(const std::string& word) {
        int cur = 0;
        nodes[0].pass++;
        for (char c : word) {
            int& nx = nodes[cur].ch[c - 'a'];
            if (nx == -1) {
                nodes.emplace_back();
                // careful: emplace_back may invalidate `nx`; recompute
                nodes[cur].ch[c - 'a'] = (int)nodes.size() - 1;
            }
            cur = nodes[cur].ch[c - 'a'];
            nodes[cur].pass++;
        }
        nodes[cur].end++;
    }
    bool erase(const std::string& word) {
        if (countEqual(word) == 0) return false;
        int cur = 0;
        nodes[0].pass--;
        for (char c : word) {
            cur = nodes[cur].ch[c - 'a'];
            nodes[cur].pass--;
        }
        nodes[cur].end--;
        return true;
    }
    int countEqual(const std::string& word) const {
        int u = walk(word);
        return u == -1 ? 0 : nodes[u].end;
    }
    int countPrefix(const std::string& prefix) const {
        int u = walk(prefix);
        return u == -1 ? 0 : nodes[u].pass;
    }
    std::vector<std::string> autocomplete(const std::string& prefix, int k) const {
        std::vector<std::string> out;
        int u = walk(prefix);
        if (u == -1 || k <= 0 || nodes[u].pass == 0) return out;
        std::string path = prefix;
        collect(u, path, k, out);
        return out;
    }
};
// ===== TESTS (do not edit) =====
struct Rng {
    uint64_t s;
    uint64_t next() {
        s = s * 6364136223846793005ULL + 1442695040888963407ULL;
        return s >> 33;
    }
};

using VS = std::vector<std::string>;

// Oracle over a sorted multiset (vector).
static int oraclePrefix(const VS& sorted, const std::string& p) {
    auto lo = std::lower_bound(sorted.begin(), sorted.end(), p);
    auto hi = std::lower_bound(sorted.begin(), sorted.end(), p + std::string(1, '\x7f'));
    return (int)(hi - lo);
}
static int oracleEqual(const VS& sorted, const std::string& w) {
    auto r = std::equal_range(sorted.begin(), sorted.end(), w);
    return (int)(r.second - r.first);
}
static VS oracleAuto(const VS& sorted, const std::string& p, int k) {
    VS out;
    for (auto it = std::lower_bound(sorted.begin(), sorted.end(), p);
         it != sorted.end() && (int)out.size() < k && it->compare(0, p.size(), p) == 0; ++it)
        if (out.empty() || out.back() != *it) out.push_back(*it);
    return out;
}

int main() {
    {
        Trie t;
        for (auto w : {"apple", "apple", "app", "apply", "banana"}) t.insert(w);
        CHECK_EQ(t.countEqual("apple"), 2);
        CHECK_EQ(t.countEqual("app"), 1);
        CHECK_EQ(t.countEqual("ap"), 0);       // a prefix of a word is not a word
        CHECK_EQ(t.countEqual("applesauce"), 0);
        CHECK_EQ(t.countPrefix("app"), 4);
        CHECK_EQ(t.countPrefix("ap"), 4);
        CHECK_EQ(t.countPrefix("appl"), 3);
        CHECK_EQ(t.countPrefix("apple"), 2);
        CHECK_EQ(t.countPrefix("b"), 1);
        CHECK_EQ(t.countPrefix(""), 5);
        CHECK_EQ(t.countPrefix("c"), 0);
        CHECK_EQ(t.countPrefix("bananas"), 0);

        CHECK_EQ(t.autocomplete("app", 10), (VS{"app", "apple", "apply"}));
        CHECK_EQ(t.autocomplete("app", 2), (VS{"app", "apple"}));
        CHECK_EQ(t.autocomplete("app", 0), VS{});
        CHECK_EQ(t.autocomplete("x", 5), VS{});
        CHECK_EQ(t.autocomplete("", 2), (VS{"app", "apple"}));
        CHECK_EQ(t.autocomplete("apple", 5), (VS{"apple"}));  // distinct words, not occurrences

        CHECK(t.erase("apple"));
        CHECK_EQ(t.countEqual("apple"), 1);
        CHECK_EQ(t.countPrefix("app"), 3);
        CHECK(t.erase("apple"));
        CHECK(!t.erase("apple"));
        CHECK(!t.erase("ap"));
        CHECK(!t.erase("zzz"));
        CHECK_EQ(t.countEqual("apple"), 0);
        CHECK_EQ(t.countPrefix("appl"), 1);
        CHECK_EQ(t.countPrefix("app"), 2);
        CHECK_EQ(t.autocomplete("app", 10), (VS{"app", "apply"}));
        CHECK_EQ(t.autocomplete("apple", 10), VS{});
        // erase must not disturb a word that shares the path
        CHECK_EQ(t.countEqual("app"), 1);
        CHECK(t.erase("app"));
        CHECK_EQ(t.countEqual("app"), 0);
        CHECK_EQ(t.countPrefix("app"), 1);
        CHECK_EQ(t.autocomplete("", 5), (VS{"apply", "banana"}));
    }
    {
        Trie t;  // empty word and empty trie
        CHECK_EQ(t.countPrefix(""), 0);
        CHECK_EQ(t.autocomplete("", 3), VS{});
        CHECK(!t.erase(""));
        t.insert("");
        t.insert("a");
        CHECK_EQ(t.countEqual(""), 1);
        CHECK_EQ(t.countPrefix(""), 2);
        CHECK_EQ(t.autocomplete("", 5), (VS{"", "a"}));
        CHECK(t.erase(""));
        CHECK_EQ(t.autocomplete("", 5), (VS{"a"}));
    }

    // large randomized, checked against a sorted-vector oracle
    {
        Rng rng{31337};
        auto word = [&]() {
            int len = 1 + (int)(rng.next() % 8);
            std::string w;
            for (int i = 0; i < len; i++) w += (char)('a' + rng.next() % 4);
            return w;
        };
        Trie t;
        VS all;
        for (int i = 0; i < 200000; i++) {
            all.push_back(word());
            t.insert(all.back());
        }
        VS sorted = all;
        std::sort(sorted.begin(), sorted.end());
        bool okCount = true, okEq = true, okAuto = true;
        for (int i = 0; i < 20000; i++) {
            std::string p;
            int len = (int)(rng.next() % 6);
            for (int j = 0; j < len; j++) p += (char)('a' + rng.next() % 4);
            if (t.countPrefix(p) != oraclePrefix(sorted, p)) okCount = false;
            std::string w = word();
            if (t.countEqual(w) != oracleEqual(sorted, w)) okEq = false;
            if (i % 10 == 0 && t.autocomplete(p, 3) != oracleAuto(sorted, p, 3)) okAuto = false;
        }
        CHECK(okCount);
        CHECK(okEq);
        CHECK(okAuto);

        // erase every other inserted occurrence, then recheck
        VS remaining;
        bool okErase = true;
        for (size_t i = 0; i < all.size(); i++) {
            if (i % 2 == 0) {
                if (!t.erase(all[i])) okErase = false;
            } else {
                remaining.push_back(all[i]);
            }
        }
        CHECK(okErase);
        std::sort(remaining.begin(), remaining.end());
        okCount = okEq = okAuto = true;
        for (int i = 0; i < 20000; i++) {
            std::string p;
            int len = (int)(rng.next() % 6);
            for (int j = 0; j < len; j++) p += (char)('a' + rng.next() % 4);
            if (t.countPrefix(p) != oraclePrefix(remaining, p)) okCount = false;
            std::string w = word();
            if (t.countEqual(w) != oracleEqual(remaining, w)) okEq = false;
            if (i % 10 == 0 && t.autocomplete(p, 3) != oracleAuto(remaining, p, 3)) okAuto = false;
        }
        CHECK(okCount);
        CHECK(okEq);
        CHECK(okAuto);
    }

    return check::report();
}
