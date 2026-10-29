// Problem: Practice unordered_map find-or-insert.
//   countWords(words)       map from word to how many times it appears
//   mostFrequent(words)     the word with the highest count; ties go to the
//                           lexicographically smallest word; "" for empty input
//   firstRepeated(nums)     scanning left to right, the first element that has
//                           already been seen earlier; -1 if all are distinct
//
// Examples:
//   countWords({"a","b","a"})          -> a:2, b:1
//   mostFrequent({"b","a","b","a"})    -> "a"      (tie, "a" < "b")
//   firstRepeated({2,5,1,5,2})         -> 5        (5 repeats before 2 does)
//
// Edge cases: empty input, single element, all distinct, ties.
//
// Run with: make run FILE=practice/cpp/stl_basics/03_word_count.cpp
//
// Syntax reminder:
//   std::unordered_map<std::string, int> m;
//   m[w]++;                                  // operator[] inserts 0 if missing, then increments
//   auto it = m.find(w);  if (it != m.end()) { it->second; }
//   m.count(w) > 0                           // membership test
//   for (const auto& [word, n] : m) { ... }  // structured bindings, unordered iteration
//   std::unordered_set<int> seen;  seen.insert(x).second   // .second is false if already present

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "check.h"

std::unordered_map<std::string, int> countWords(const std::vector<std::string>& words) {
    (void)words;
    std::unordered_map<std::string, int> output;
    for (std::string word : words) {
        if (output.find(word) == output.end()) {
            output[word] = 0;
        }
        output[word]++;
    }
    return output;
}

std::string mostFrequent(const std::vector<std::string>& words) {
    (void)words;
    // TODO
    std::string output = "";
    int maxcount = 0;
    std::unordered_map<std::string, int> wc = countWords(words);
    for (auto it = wc.begin(); it != wc.end(); ++it){
        std::string word = it->first;
        int count = it->second;
        if (count > maxcount) {
            output = word;
            maxcount = count;
        }
    }
    return output;
}

int firstRepeated(const std::vector<int>& nums) {
    (void)nums;
    std::unordered_set<int> seen;
    for(int i : nums) {
        auto [it, inserted] = seen.insert(i);
        if (!inserted) return i;
    }
    return -1;
}

// Lookup helper for the tests (returns 0 when the key is absent).
static int get(const std::unordered_map<std::string, int>& m, const std::string& k) {
    auto it = m.find(k);
    return it == m.end() ? 0 : it->second;
}

int main() {
    auto empty = countWords({});
    CHECK_EQ(empty.size(), size_t{0});

    auto one = countWords({"x"});
    CHECK_EQ(one.size(), size_t{1});
    CHECK_EQ(get(one, "x"), 1);

    auto m = countWords({"a", "b", "a", "c", "a", "b"});
    CHECK_EQ(m.size(), size_t{3});
    CHECK_EQ(get(m, "a"), 3);
    CHECK_EQ(get(m, "b"), 2);
    CHECK_EQ(get(m, "c"), 1);
    CHECK_EQ(get(m, "zzz"), 0);

    CHECK_EQ(mostFrequent({}), "");
    CHECK_EQ(mostFrequent({"solo"}), "solo");
    CHECK_EQ(mostFrequent({"a", "b", "a"}), "a");
    CHECK_EQ(mostFrequent({"b", "a", "b", "a"}), "a");

    CHECK_EQ(firstRepeated({}), -1);
    CHECK_EQ(firstRepeated({1, 2, 3}), -1);
    CHECK_EQ(firstRepeated({2, 5, 1, 5, 2}), 5);
    CHECK_EQ(firstRepeated({7, 7}), 7);

    return check::report();
}
