// Problem: Build a timer queue on top of a binary min-heap you write yourself (no std::priority_queue,
//   std::set, std::map for the ordering). Tasks are identified by a string id and fire at a deadline.
//   class TimerQueue {
//     void upsert(const std::string& id, long long deadline);   add the task, or change its deadline if it exists
//     bool cancel(const std::string& id);                       remove the task; false if it was not present
//     bool contains(const std::string& id) const;
//     std::optional<std::pair<std::string, long long>> peek() const;   earliest task, without removing it
//     std::optional<std::pair<std::string, long long>> pop();          remove and return the earliest task
//     size_t size() const;  bool empty() const;
//   };
//   "Earliest" = smallest deadline; on equal deadlines, the lexicographically smaller id comes first.
//
// Examples:
//   upsert(a,50) upsert(b,10) upsert(c,10)  -> peek() = (b,10); pop() = (b,10); pop() = (c,10); pop() = (a,50)
//   upsert(a,50) upsert(a,5)                 -> one task: (a,5)
//   cancel(x) on an absent id                -> false, nothing changes
//
// Assumptions (chosen for this exercise, interview-style; challenge them in your approach comment):
//   * ids are non-empty strings; deadlines are arbitrary long long values (negative allowed)
//   * upserting an existing id with the same deadline is a no-op; any change re-orders correctly whether the
//     deadline moves earlier or later
//   * up to 100,000 live tasks and 150,000 randomized operations in one test
//   * the tests only observe behaviour through the public interface above
// Required complexity: O(log n) per upsert, cancel and pop; O(1) for peek, contains, size. O(n) space.
//   Re-sorting or scanning all tasks per operation will not finish.
// Run: make run FILE=curriculum/201_data_structures/design/04_indexed_min_heap_timer_queue.cpp
//
// BEFORE CODING: write your own approach in a comment block right here, at the top of your solution:
//   approach (heap layout, what extra index you keep and why a plain heap is not enough),
//   invariant (heap property + index consistency; what every swap must also update),
//   complexity (per operation, and the cost of removing from the middle of the heap).

#include "check.h"

#include <map>
#include <optional>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// ===== YOUR CODE BELOW (stub) =====
class TimerQueue {
public:
    using Entry = std::pair<std::string, long long>;
private:
    std::vector<Entry> h_;
    std::unordered_map<std::string, size_t> pos_;
    bool less(size_t a, size_t b) const {
        if (h_[a].second != h_[b].second) return h_[a].second < h_[b].second;
        return h_[a].first < h_[b].first;
    }
    void swp(size_t a, size_t b) {
        std::swap(h_[a], h_[b]);
        pos_[h_[a].first] = a; pos_[h_[b].first] = b;
    }
    void up(size_t i) { while (i > 0) { size_t p = (i - 1) / 2; if (!less(i, p)) break; swp(i, p); i = p; } }
    void down(size_t i) {
        for (;;) {
            size_t l = 2 * i + 1, r = l + 1, m = i;
            if (l < h_.size() && less(l, m)) m = l;
            if (r < h_.size() && less(r, m)) m = r;
            if (m == i) return;
            swp(i, m); i = m;
        }
    }
    void removeAt(size_t i) {
        size_t last = h_.size() - 1;
        if (i != last) swp(i, last);
        pos_.erase(h_[last].first);
        h_.pop_back();
        if (i < h_.size()) { up(i); down(i); }
    }
public:
    void upsert(const std::string& id, long long d) {
        auto it = pos_.find(id);
        if (it == pos_.end()) {
            h_.push_back({id, d}); pos_[id] = h_.size() - 1; up(h_.size() - 1);
        } else {
            size_t i = it->second; h_[i].second = d; up(i); down(pos_[id]);
        }
    }
    bool cancel(const std::string& id) {
        auto it = pos_.find(id);
        if (it == pos_.end()) return false;
        removeAt(it->second);
        return true;
    }
    bool contains(const std::string& id) const { return pos_.count(id) > 0; }
    std::optional<Entry> peek() const { if (h_.empty()) return std::nullopt; return h_[0]; }
    std::optional<Entry> pop() {
        if (h_.empty()) return std::nullopt;
        Entry e = h_[0]; removeAt(0); return e;
    }
    size_t size() const { return h_.size(); }
    bool empty() const { return h_.empty(); }
};
// ===== TESTS (do not edit below) =====
using Entry = TimerQueue::Entry;
using Opt = std::optional<Entry>;

// Compare to the model through a single printable value.
static std::string show(const Opt& o) { return o ? o->first + "@" + std::to_string(o->second) : "none"; }

int main() {
    // empty
    {
        TimerQueue q;
        CHECK(q.empty());
        CHECK_EQ(q.size(), 0u);
        CHECK_EQ(show(q.peek()), std::string("none"));
        CHECK_EQ(show(q.pop()), std::string("none"));
        CHECK(!q.cancel("x"));
        CHECK(!q.contains("x"));
    }
    // ordering and tie-breaking
    {
        TimerQueue q;
        q.upsert("a", 50);
        q.upsert("b", 10);
        q.upsert("c", 10);
        CHECK_EQ(q.size(), 3u);
        CHECK_EQ(show(q.peek()), std::string("b@10"));
        CHECK_EQ(q.size(), 3u);  // peek does not remove
        CHECK_EQ(show(q.pop()), std::string("b@10"));
        CHECK_EQ(show(q.pop()), std::string("c@10"));
        CHECK_EQ(show(q.pop()), std::string("a@50"));
        CHECK(q.empty());
        CHECK_EQ(show(q.pop()), std::string("none"));
    }
    // update moves a task earlier or later; same value is a no-op; size does not grow
    {
        TimerQueue q;
        q.upsert("a", 50);
        q.upsert("b", 60);
        q.upsert("c", 70);
        q.upsert("c", 5);  // c jumps to the front
        CHECK_EQ(q.size(), 3u);
        CHECK_EQ(show(q.peek()), std::string("c@5"));
        q.upsert("c", 100);  // and back to the end
        CHECK_EQ(show(q.peek()), std::string("a@50"));
        q.upsert("a", 50);  // no-op
        q.upsert("a", 65);  // a moves behind b
        CHECK_EQ(q.size(), 3u);
        CHECK_EQ(show(q.pop()), std::string("b@60"));
        CHECK_EQ(show(q.pop()), std::string("a@65"));
        CHECK_EQ(show(q.pop()), std::string("c@100"));
    }
    // tie-break survives an update
    {
        TimerQueue q;
        q.upsert("m", 10);
        q.upsert("a", 20);
        q.upsert("a", 10);  // now tied with m; "a" < "m"
        CHECK_EQ(show(q.pop()), std::string("a@10"));
        CHECK_EQ(show(q.pop()), std::string("m@10"));
    }
    // cancel: root, middle, last, absent, then re-add
    {
        TimerQueue q;
        for (int i = 0; i < 10; i++) q.upsert("t" + std::to_string(i), 100 - i * 10);  // t9@10 is the earliest
        CHECK(q.contains("t5"));
        CHECK(q.cancel("t9"));  // root
        CHECK(!q.contains("t9"));
        CHECK(!q.cancel("t9"));
        CHECK(q.cancel("t5"));  // middle
        CHECK(q.cancel("t0"));  // the latest
        CHECK_EQ(q.size(), 7u);
        CHECK_EQ(show(q.peek()), std::string("t8@20"));
        q.upsert("t9", 1);  // re-adding a cancelled id works
        std::vector<std::string> order;
        while (auto e = q.pop()) order.push_back(show(e));
        std::vector<std::string> expected = {"t9@1",  "t8@20", "t7@30", "t6@40", "t4@60", "t3@70", "t2@80", "t1@90"};
        CHECK_EQ(order, expected);
    }
    // negative and extreme deadlines
    {
        TimerQueue q;
        q.upsert("neg", -5);
        q.upsert("big", 9000000000000000000LL);
        q.upsert("min", -9000000000000000000LL);
        CHECK_EQ(show(q.pop()), std::string("min@-9000000000000000000"));
        CHECK_EQ(show(q.pop()), std::string("neg@-5"));
        CHECK_EQ(show(q.pop()), std::string("big@9000000000000000000"));
    }
    // cancel everything, then reuse
    {
        TimerQueue q;
        for (int i = 0; i < 50; i++) q.upsert("k" + std::to_string(i), (i * 37) % 11);
        for (int i = 0; i < 50; i++) CHECK(q.cancel("k" + std::to_string(i)));
        CHECK(q.empty());
        CHECK_EQ(show(q.peek()), std::string("none"));
        q.upsert("again", 3);
        CHECK_EQ(show(q.peek()), std::string("again@3"));
    }

    // large randomized run against a model built from std::map + std::set; compare the observable state each step
    {
        TimerQueue q;
        std::map<std::string, long long> cur;
        std::set<std::pair<long long, std::string>> byDeadline;
        std::mt19937_64 rng(2025);
        int mismatches = 0;
        auto modelPeek = [&]() -> Opt {
            if (byDeadline.empty()) return std::nullopt;
            return Entry{byDeadline.begin()->second, byDeadline.begin()->first};
        };
        const int idSpace = 40000;
        for (int step = 0; step < 150000; step++) {
            std::string id = "id" + std::to_string(rng() % idSpace);
            unsigned op = (unsigned)(rng() % 10);
            if (op < 6) {  // upsert (deadlines in a small range so ties are common)
                long long d = (long long)(rng() % 5000) - 1000;
                auto it = cur.find(id);
                if (it != cur.end()) byDeadline.erase({it->second, id});
                cur[id] = d;
                byDeadline.insert({d, id});
                q.upsert(id, d);
            } else if (op < 8) {  // cancel
                bool had = cur.count(id) > 0;
                if (had) {
                    byDeadline.erase({cur[id], id});
                    cur.erase(id);
                }
                if (q.cancel(id) != had) mismatches++;
            } else if (op == 8) {  // pop
                Opt expected = modelPeek();
                if (expected) {
                    byDeadline.erase(byDeadline.begin());
                    cur.erase(expected->first);
                }
                if (show(q.pop()) != show(expected)) mismatches++;
            } else {  // contains
                if (q.contains(id) != (cur.count(id) > 0)) mismatches++;
            }
            if (show(q.peek()) != show(modelPeek()) || q.size() != cur.size()) mismatches++;
        }
        CHECK_EQ(mismatches, 0);  // number of steps where peek/size/pop/cancel/contains disagreed with the model
        // drain what is left: must come out in model order
        int drainMismatches = 0;
        while (!byDeadline.empty()) {
            Opt expected = modelPeek();
            byDeadline.erase(byDeadline.begin());
            if (show(q.pop()) != show(expected)) drainMismatches++;
        }
        CHECK_EQ(drainMismatches, 0);
        CHECK(q.empty());
    }
    // large: 100,000 inserts with descending deadlines, then pop all (worst case for a naive scan)
    {
        TimerQueue q;
        const int n = 100000;
        for (int i = 0; i < n; i++) q.upsert("job" + std::to_string(i), (long long)(n - i));
        CHECK_EQ(q.size(), (size_t)n);
        long long prev = -1;
        bool sorted = true;
        for (int i = 0; i < n; i++) {
            auto e = q.pop();
            if (!e || e->second <= prev) sorted = false;
            if (e) prev = e->second;
        }
        CHECK(sorted);
        CHECK(q.empty());
    }
    return check::report();
}
