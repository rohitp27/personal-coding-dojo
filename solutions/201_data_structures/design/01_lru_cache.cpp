// Problem: LRU (least recently used) cache with O(1) get and put.
//   class LRUCache {
//     explicit LRUCache(int capacity);     capacity >= 1
//     int  get(int key);                   value for key, or -1 if absent; a hit makes key the most recently used
//     void put(int key, int value);        insert or overwrite; either way key becomes the most recently used;
//                                          if the insert would exceed capacity, evict the least recently used key
//   };
//   Values stored are always >= 0, so -1 is an unambiguous "miss". Keys may be any int (negatives included).
//
// Hint: you need O(1) lookup by key AND O(1) "move to front / drop from back". A hash map from key to a list
//   iterator, plus a std::list ordered by recency, does both (list::splice does not invalidate iterators).
//
// Examples (capacity 2):
//   put(1,1) put(2,2) get(1)=1 put(3,3)   -> evicts key 2
//   get(2)=-1 put(4,4)                    -> evicts key 1
//   get(1)=-1 get(3)=3 get(4)=4
//
// Constraints: capacity up to 100,000; up to 1,000,000 operations in a test.
// Required complexity: O(1) average per get and per put; O(capacity) space.
// Run: make run FILE=curriculum/201_data_structures/design/01_lru_cache.cpp

#include "check.h"

#include <list>
#include <random>
#include <unordered_map>
#include <utility>
#include <vector>

// ===== YOUR CODE BELOW (stub) =====
class LRUCache {
    int cap_;
    std::list<std::pair<int,int>> order_;  // front = most recent
    std::unordered_map<int, std::list<std::pair<int,int>>::iterator> idx_;
public:
    explicit LRUCache(int capacity) : cap_(capacity) { idx_.reserve(capacity * 2); }
    int get(int key) {
        auto it = idx_.find(key);
        if (it == idx_.end()) return -1;
        order_.splice(order_.begin(), order_, it->second);
        return it->second->second;
    }
    void put(int key, int value) {
        auto it = idx_.find(key);
        if (it != idx_.end()) {
            it->second->second = value;
            order_.splice(order_.begin(), order_, it->second);
            return;
        }
        if ((int)order_.size() == cap_) { idx_.erase(order_.back().first); order_.pop_back(); }
        order_.emplace_front(key, value);
        idx_[key] = order_.begin();
    }
};
// ===== TESTS (do not edit below) =====
// Slow but obviously-correct model: front of the vector = most recently used.
class Model {
public:
    explicit Model(int cap) : cap_(cap) {}
    int get(int key) {
        for (size_t i = 0; i < items_.size(); i++)
            if (items_[i].first == key) {
                auto kv = items_[i];
                items_.erase(items_.begin() + (long)i);
                items_.insert(items_.begin(), kv);
                return kv.second;
            }
        return -1;
    }
    void put(int key, int value) {
        for (size_t i = 0; i < items_.size(); i++)
            if (items_[i].first == key) {
                items_.erase(items_.begin() + (long)i);
                break;
            }
        items_.insert(items_.begin(), {key, value});
        if ((int)items_.size() > cap_) items_.pop_back();
    }

private:
    int cap_;
    std::vector<std::pair<int, int>> items_;
};

int main() {
    {
        LRUCache c(2);
        c.put(1, 1);
        c.put(2, 2);
        CHECK_EQ(c.get(1), 1);
        c.put(3, 3);  // evicts 2
        CHECK_EQ(c.get(2), -1);
        c.put(4, 4);  // evicts 1
        CHECK_EQ(c.get(1), -1);
        CHECK_EQ(c.get(3), 3);
        CHECK_EQ(c.get(4), 4);
    }
    {  // capacity 1
        LRUCache c(1);
        CHECK_EQ(c.get(7), -1);
        c.put(1, 10);
        CHECK_EQ(c.get(1), 10);
        c.put(2, 20);
        CHECK_EQ(c.get(1), -1);
        CHECK_EQ(c.get(2), 20);
        c.put(2, 21);  // overwrite does not evict
        CHECK_EQ(c.get(2), 21);
    }
    {  // overwriting refreshes recency and does not grow the cache
        LRUCache c(2);
        c.put(1, 1);
        c.put(2, 2);
        c.put(1, 10);  // 1 is now newest
        c.put(3, 3);   // evicts 2, not 1
        CHECK_EQ(c.get(2), -1);
        CHECK_EQ(c.get(1), 10);
        CHECK_EQ(c.get(3), 3);
    }
    {  // get refreshes recency
        LRUCache c(3);
        c.put(1, 1);
        c.put(2, 2);
        c.put(3, 3);
        CHECK_EQ(c.get(1), 1);  // order newest..oldest: 1 3 2
        c.put(4, 4);            // evicts 2, order: 4 1 3
        CHECK_EQ(c.get(2), -1);
        CHECK_EQ(c.get(3), 3);  // order is now 3 4 1 (newest first)
        c.put(5, 5);            // evicts 1
        CHECK_EQ(c.get(1), -1);
        CHECK_EQ(c.get(4), 4);
        CHECK_EQ(c.get(5), 5);
    }
    {  // a miss must not change anything; value 0 and negative keys are real entries
        LRUCache c(2);
        c.put(0, 0);
        c.put(-5, 7);
        CHECK_EQ(c.get(99), -1);
        CHECK_EQ(c.get(0), 0);
        c.put(1, 1);  // evicts -5 (0 was just read)
        CHECK_EQ(c.get(-5), -1);
        CHECK_EQ(c.get(0), 0);
    }
    {  // random operations against the model, several capacities and key ranges
        std::mt19937 rng(77);
        for (int cap : {1, 2, 3, 5, 17}) {
            LRUCache c(cap);
            Model m(cap);
            int mismatches = 0;
            for (int i = 0; i < 20000; i++) {
                int key = (int)(rng() % (unsigned)(cap * 2 + 1)) - cap;
                if (rng() % 2) {
                    int v = (int)(rng() % 1000);
                    c.put(key, v);
                    m.put(key, v);
                } else if (c.get(key) != m.get(key)) {
                    mismatches++;
                }
            }
            CHECK_EQ(mismatches, 0);  // number of get() results that differ from the model, capacity = cap
        }
    }
    {  // medium random, still checked against the model
        std::mt19937 rng(8);
        LRUCache c(500);
        Model m(500);
        int mismatches = 0;
        for (int i = 0; i < 60000; i++) {
            int key = (int)(rng() % 800);
            if (rng() % 3 == 0) {
                c.put(key, i);
                m.put(key, i);
            } else if (c.get(key) != m.get(key)) {
                mismatches++;
            }
        }
        CHECK_EQ(mismatches, 0);
    }
    {  // large: an O(capacity) scan per operation would do ~1e11 steps here
        const int cap = 100000, n = 1000000;
        LRUCache c(cap);
        for (int i = 0; i < n; i++) c.put(i, i);
        CHECK_EQ(c.get(n - cap - 1), -1);
        long long sum = 0;
        for (int i = n - cap; i < n; i++) sum += c.get(i);
        long long expected = 0;
        for (int i = n - cap; i < n; i++) expected += i;
        CHECK_EQ(sum, expected);
    }
    {  // classic LRU worst case: cycling over capacity + 1 keys never hits
        const int cap = 1000;
        LRUCache c(cap);
        int hits = 0;
        for (int i = 0; i < 300000; i++) {
            int key = i % (cap + 1);
            if (c.get(key) != -1)
                hits++;
            else
                c.put(key, key);
        }
        CHECK_EQ(hits, 0);
        // cycling over exactly capacity keys hits every time after the first lap
        LRUCache d(cap);
        hits = 0;
        for (int i = 0; i < 300000; i++) {
            int key = i % cap;
            if (d.get(key) != -1)
                hits++;
            else
                d.put(key, key);
        }
        CHECK_EQ(hits, 300000 - cap);
    }
    return check::report();
}
