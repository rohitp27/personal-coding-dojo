// 301 advanced > systems_and_concurrency > 04 Consistent-hashing ring with virtual nodes
//
// Problem:
//   Implement HashRing, which maps string keys to nodes so that adding or removing
//   a node moves only a small share of keys. Each node is placed on a 64-bit ring
//   at `vnodes` pseudo-random positions (virtual nodes). A key belongs to the
//   first virtual node at or after hash64(key) going clockwise, wrapping around
//   past the largest position to the smallest.
//
//     HashRing(vnodes)       vnodes >= 1 virtual nodes per physical node.
//     addNode(name)          true if added, false if already present.
//     removeNode(name)       true if removed, false if absent.
//     getNode(key)           owning node, or "" if the ring is empty.
//     getNodes(key, r)       up to r DISTINCT nodes in clockwise order starting
//                            from the owner (a replica/preference list):
//                            result[0] == getNode(key); fewer than r only if the
//                            ring has fewer nodes; {} if r == 0 or ring empty.
//     nodeCount()            number of physical nodes.
//
//   The mapping must depend ONLY on the current SET of nodes: adding A,B,C in a
//   different order, or adding then removing a node, gives identical results.
//   Use the provided hash64 for every position you compute (keys and virtual
//   nodes) so results are the same on every platform.
//
// Examples:
//   ring with {n0..n4}, 200 vnodes, 100k keys -> each node owns roughly 20% (tests allow 12k..28k)
//   add n5 -> every key that moves moves TO n5 (never between old nodes), about 1/6 of keys move
//   remove n2 -> only keys that were on n2 move; every other key keeps its owner
//
// Constraints: up to ~1000 nodes, vnodes up to ~500; lookups must be fast with 100k+ keys.
//
// Required complexity: getNode O(log(nodes * vnodes)); add/remove O(vnodes * log(ring size))
//   (a rebuild of the whole ring per add/remove is acceptable only if you say so and
//   still pass the tests; the intended solution updates in place).
//
// Assumptions: node names are non-empty and distinct strings; you may assume two
//   different virtual nodes never hash to exactly the same 64-bit position (state
//   how you would handle a collision, in your comment block).
//
// Think first:
//   1. Why does one position per node give terrible balance, and how does the
//      spread of the load shrink as vnodes grows? What does that tell you about
//      choosing vnodes?
//   2. Which data structure gives "first position >= h, with wraparound" in
//      O(log n), and what is the wraparound edge case?
//   3. Why is it true that a key either keeps its owner or moves to the new node
//      when a node is added? Argue it from the ring, then check it holds in your code.
//
// BEFORE CODING: write your approach, invariant and complexity in a comment
// block right here. (If you add locking later: which lock protects the ring?)
//
// Run: make run FILE=curriculum/301_advanced/systems_and_concurrency/04_consistent_hash_ring.cpp

#include "check.h"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

// ===== YOUR CODE BELOW =====
inline uint64_t hash64(const std::string& s) {
    uint64_t h = 1469598103934665603ULL;
    for (unsigned char c : s) {
        h ^= c;
        h *= 1099511628211ULL;
    }
    h ^= h >> 30;
    h *= 0xbf58476d1ce4e5b9ULL;
    h ^= h >> 27;
    h *= 0x94d049bb133111ebULL;
    h ^= h >> 31;
    return h;
}

class HashRing {
    int vnodes_;
    std::map<uint64_t, std::string> ring_;
    std::set<std::string> nodes_;

public:
    explicit HashRing(int vnodes) : vnodes_(vnodes) {}
    bool addNode(const std::string& name) {
        if (!nodes_.insert(name).second) return false;
        for (int i = 0; i < vnodes_; i++) ring_[hash64(name + "#" + std::to_string(i))] = name;
        return true;
    }
    bool removeNode(const std::string& name) {
        if (nodes_.erase(name) == 0) return false;
        for (int i = 0; i < vnodes_; i++) {
            auto it = ring_.find(hash64(name + "#" + std::to_string(i)));
            if (it != ring_.end() && it->second == name) ring_.erase(it);
        }
        return true;
    }
    std::string getNode(const std::string& key) const {
        if (ring_.empty()) return "";
        auto it = ring_.lower_bound(hash64(key));
        if (it == ring_.end()) it = ring_.begin();
        return it->second;
    }
    std::vector<std::string> getNodes(const std::string& key, size_t r) const {
        std::vector<std::string> out;
        if (ring_.empty() || r == 0) return out;
        r = std::min(r, nodes_.size());
        auto it = ring_.lower_bound(hash64(key));
        std::set<std::string> seen;
        for (size_t steps = 0; steps < ring_.size() && out.size() < r; steps++) {
            if (it == ring_.end()) it = ring_.begin();
            if (seen.insert(it->second).second) out.push_back(it->second);
            ++it;
        }
        return out;
    }
    size_t nodeCount() const { return nodes_.size(); }
};
// ===== TESTS (do not edit) =====
static std::vector<std::string> makeKeys(int n) {
    std::vector<std::string> keys;
    keys.reserve(n);
    for (int i = 0; i < n; i++) keys.push_back("key-" + std::to_string(i));
    return keys;
}

static std::vector<std::string> owners(const HashRing& ring, const std::vector<std::string>& keys) {
    std::vector<std::string> out;
    out.reserve(keys.size());
    for (auto& k : keys) out.push_back(ring.getNode(k));
    return out;
}

int main() {
    using VS = std::vector<std::string>;

    // empty ring and membership bookkeeping
    {
        HashRing ring(50);
        CHECK_EQ(ring.nodeCount(), (size_t)0);
        CHECK_EQ(ring.getNode("x"), std::string(""));
        CHECK_EQ(ring.getNodes("x", 3), VS{});
        CHECK(ring.addNode("A"));
        CHECK(!ring.addNode("A"));
        CHECK_EQ(ring.nodeCount(), (size_t)1);
        bool all = true;
        for (auto& k : makeKeys(200)) all &= ring.getNode(k) == "A";
        CHECK(all);  // one node owns everything, including keys that wrap around
        CHECK_EQ(ring.getNodes("x", 5), VS{"A"});
        CHECK(!ring.removeNode("B"));
        CHECK(ring.removeNode("A"));
        CHECK(!ring.removeNode("A"));
        CHECK_EQ(ring.nodeCount(), (size_t)0);
        CHECK_EQ(ring.getNode("x"), std::string(""));
    }

    const auto keys = makeKeys(100000);
    const VS names = {"n0", "n1", "n2", "n3", "n4"};

    HashRing ring(200);
    for (auto& n : names) CHECK(ring.addNode(n));
    CHECK_EQ(ring.nodeCount(), (size_t)5);
    const VS before = owners(ring, keys);

    // balance: ideal is 20000 per node
    {
        std::map<std::string, int> count;
        for (auto& o : before) count[o]++;
        CHECK_EQ(count.size(), (size_t)5);
        for (auto& n : names) {
            CHECK(count[n] >= 12000);
            CHECK(count[n] <= 28000);
        }
    }

    // deterministic: same answer when asked again
    CHECK(owners(ring, keys) == before);

    // add a node: keys only ever move TO the new node, and about 1/6 of them
    {
        HashRing r2(200);
        for (auto& n : names) r2.addNode(n);
        CHECK(r2.addNode("n5"));
        VS after = owners(r2, keys);
        int moved = 0;
        bool onlyToNew = true;
        for (size_t i = 0; i < keys.size(); i++) {
            if (after[i] != before[i]) {
                moved++;
                if (after[i] != "n5") onlyToNew = false;
            }
        }
        CHECK(onlyToNew);
        CHECK(moved >= 8000);   // ideal ~16667
        CHECK(moved <= 25000);
        // add then remove restores the exact original mapping
        CHECK(r2.removeNode("n5"));
        CHECK(owners(r2, keys) == before);
    }

    // remove a node: only that node's keys move, and none stay on it
    {
        HashRing r3(200);
        for (auto& n : names) r3.addNode(n);
        CHECK(r3.removeNode("n2"));
        CHECK_EQ(r3.nodeCount(), (size_t)4);
        VS after = owners(r3, keys);
        bool othersStay = true, movedAway = true;
        for (size_t i = 0; i < keys.size(); i++) {
            if (before[i] != "n2") {
                if (after[i] != before[i]) othersStay = false;
            } else if (after[i] == "n2" || after[i].empty()) {
                movedAway = false;
            }
        }
        CHECK(othersStay);
        CHECK(movedAway);
    }

    // mapping depends only on the set of nodes, not on insertion order
    {
        HashRing rev(200);
        for (auto it = names.rbegin(); it != names.rend(); ++it) rev.addNode(*it);
        CHECK(owners(rev, keys) == before);
    }

    // preference lists
    {
        bool firstIsOwner = true, distinct = true, sized = true;
        for (size_t i = 0; i < keys.size(); i += 7) {
            VS p = ring.getNodes(keys[i], 3);
            if (p.size() != 3) sized = false;
            if (p.empty() || p[0] != before[i]) firstIsOwner = false;
            std::set<std::string> s(p.begin(), p.end());
            if (s.size() != p.size()) distinct = false;
        }
        CHECK(sized);
        CHECK(firstIsOwner);
        CHECK(distinct);
        CHECK_EQ(ring.getNodes("some-key", 0), VS{});
        VS all = ring.getNodes("some-key", 50);  // more than the node count
        CHECK_EQ(all.size(), (size_t)5);
        CHECK_EQ(std::set<std::string>(all.begin(), all.end()).size(), (size_t)5);

        // removing a node that is NOT in a key's list leaves that list unchanged
        HashRing r4(200);
        for (auto& n : names) r4.addNode(n);
        r4.removeNode("n4");
        bool stable = true;
        for (size_t i = 0; i < keys.size(); i += 11) {
            VS old = ring.getNodes(keys[i], 3);
            if (std::find(old.begin(), old.end(), "n4") != old.end()) continue;
            if (r4.getNodes(keys[i], 3) != old) stable = false;
        }
        CHECK(stable);
    }

    return check::report();
}
