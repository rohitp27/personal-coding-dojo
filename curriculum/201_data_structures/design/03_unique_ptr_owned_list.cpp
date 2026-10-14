// Problem: A singly linked list whose nodes are owned through std::unique_ptr -- RAII and move semantics.
//   template <class T> class OwnedList {
//     OwnedList();                                   empty list
//     ~OwnedList();                                  frees every node
//     OwnedList(const OwnedList&) = delete;          copying is forbidden (ownership is unique)
//     OwnedList& operator=(const OwnedList&) = delete;
//     OwnedList(OwnedList&& other) noexcept;         steals other's nodes; other becomes a valid EMPTY list
//     OwnedList& operator=(OwnedList&& other) noexcept;   frees our current nodes, then steals; safe on self-move
//     void push_front(T value);                      O(1)
//     void push_back(T value);                       O(1)  (so you must track the tail)
//     std::optional<T> pop_front();                  O(1); std::nullopt if empty; moves the value out
//     T*   front();                                  pointer to the first value, nullptr if empty
//     size_t size() const;  bool empty() const;
//     void reverse();                                O(n), in place, no allocations
//     void clear();                                  frees all nodes
//     void append(OwnedList&& other);                O(1): moves all of other's nodes to our end; other becomes empty
//   };
//   T may be MOVE-ONLY (e.g. std::unique_ptr<int>). Nothing may be copied, and no element may be leaked,
//   destroyed twice, or destroyed early.
//
// Examples:
//   push_back 1, push_back 2, push_front 0  -> pop_front gives 0, 1, 2
//   a = {1,2}; b = {3,4}; a.append(std::move(b))  -> a = {1,2,3,4}, b is empty and still usable
//
// Constraints: lists of up to 1,000,000 nodes, including destroying, clearing and move-assigning over them.
// Required complexity: all operations as listed above; destruction O(n) with O(1) stack depth.
// Run: make run FILE=curriculum/201_data_structures/design/03_unique_ptr_owned_list.cpp
//
// Think first:
//  1. The obvious node is struct { T value; std::unique_ptr<Node> next; }. What does the compiler-generated
//     destructor of a 1,000,000-node chain do to the call stack? What do you write instead?
//  2. After pop_front empties the list, or after a move, which of your members could still point at freed or
//     stolen memory (hint: the raw tail pointer)? Where is each invariant restored?
//  3. In move assignment, what is the correct order of "free my nodes", "steal theirs", "reset theirs", and why
//     does self-move break one of the naive orders?

#include "check.h"

#include <cstddef>
#include <memory>
#include <optional>
#include <utility>

// ===== YOUR CODE BELOW (stub) =====
template <class T>
class OwnedList {
public:
    OwnedList() = default;
    ~OwnedList() = default;  // TODO: careful with long chains
    OwnedList(const OwnedList&) = delete;
    OwnedList& operator=(const OwnedList&) = delete;

    OwnedList(OwnedList&&) noexcept {
        // TODO
    }
    OwnedList& operator=(OwnedList&&) noexcept {
        // TODO
        return *this;
    }

    void push_front([[maybe_unused]] T value) {
        // TODO
    }
    void push_back([[maybe_unused]] T value) {
        // TODO
    }
    std::optional<T> pop_front() {
        // TODO
        return std::nullopt;
    }
    T* front() {
        // TODO
        return nullptr;
    }
    size_t size() const { return 0; }    // TODO
    bool empty() const { return true; }  // TODO
    void reverse() {
        // TODO
    }
    void clear() {
        // TODO
    }
    void append([[maybe_unused]] OwnedList&& other) {
        // TODO
    }

private:
    // TODO: your node type and members (head, tail, size).
};

// ===== TESTS (do not edit below) =====
// Move-only element that counts how many instances are alive.
struct Tracker {
    static inline int live = 0;
    int id;
    explicit Tracker(int i) : id(i) { live++; }
    Tracker(Tracker&& o) noexcept : id(o.id) {
        o.id = -1;
        live++;
    }
    Tracker& operator=(Tracker&& o) noexcept {
        id = o.id;
        o.id = -1;
        return *this;
    }
    Tracker(const Tracker&) = delete;
    Tracker& operator=(const Tracker&) = delete;
    ~Tracker() { live--; }
};

static int popInt(OwnedList<int>& l) {
    auto v = l.pop_front();
    return v ? *v : -999;
}
static int popId(OwnedList<Tracker>& l) {
    auto v = l.pop_front();
    return v ? v->id : -999;
}

int main() {
    // empty list
    {
        OwnedList<int> l;
        CHECK(l.empty());
        CHECK_EQ(l.size(), 0u);
        CHECK(l.front() == nullptr);
        CHECK(!l.pop_front().has_value());
        l.reverse();  // no-ops on empty
        l.clear();
        CHECK(l.empty());
    }
    // push_front / pop_front is a stack
    {
        OwnedList<int> l;
        for (int i = 1; i <= 3; i++) l.push_front(i);
        CHECK_EQ(l.size(), 3u);
        CHECK(l.front() != nullptr);
        if (l.front()) CHECK_EQ(*l.front(), 3);
        CHECK_EQ(popInt(l), 3);
        CHECK_EQ(popInt(l), 2);
        CHECK_EQ(popInt(l), 1);
        CHECK_EQ(popInt(l), -999);
        CHECK(l.empty());
    }
    // push_back / push_front mix
    {
        OwnedList<int> l;
        l.push_back(1);
        l.push_back(2);
        l.push_front(0);
        CHECK_EQ(l.size(), 3u);
        CHECK_EQ(popInt(l), 0);
        CHECK_EQ(popInt(l), 1);
        CHECK_EQ(popInt(l), 2);
    }
    // the tail must be reset when the list drains (dangling-tail bug)
    {
        OwnedList<int> l;
        l.push_back(1);
        CHECK_EQ(popInt(l), 1);
        l.push_back(2);
        CHECK_EQ(l.size(), 1u);
        CHECK_EQ(popInt(l), 2);
        l.push_back(3);
        l.push_back(4);
        CHECK_EQ(popInt(l), 3);
        CHECK_EQ(popInt(l), 4);
        l.push_front(5);
        l.push_back(6);  // tail must be the node holding 5
        CHECK_EQ(popInt(l), 5);
        CHECK_EQ(popInt(l), 6);
    }
    // front() gives mutable access
    {
        OwnedList<int> l;
        l.push_back(10);
        if (l.front()) *l.front() = 11;
        CHECK_EQ(popInt(l), 11);
    }
    // move-only payloads
    {
        OwnedList<std::unique_ptr<int>> l;
        l.push_back(std::make_unique<int>(5));
        l.push_front(std::make_unique<int>(4));
        auto a = l.pop_front();
        auto b = l.pop_front();
        CHECK(a.has_value() && b.has_value());
        if (a && b) {
            CHECK_EQ(**a, 4);
            CHECK_EQ(**b, 5);
        }
        CHECK(!l.pop_front().has_value());
    }
    // lifetime accounting: every element destroyed exactly once, at the right time
    {
        CHECK_EQ(Tracker::live, 0);
        {
            OwnedList<Tracker> l;
            for (int i = 0; i < 100; i++) l.push_back(Tracker(i));
            CHECK_EQ(Tracker::live, 100);
            for (int i = 0; i < 10; i++) l.pop_front();
            CHECK_EQ(Tracker::live, 90);
            CHECK_EQ(l.size(), 90u);
            l.clear();
            CHECK_EQ(Tracker::live, 0);
            CHECK(l.empty());
            l.push_front(Tracker(1));
            l.push_front(Tracker(2));
            CHECK_EQ(Tracker::live, 2);
        }  // destructor frees the rest
        CHECK_EQ(Tracker::live, 0);
    }
    // move construction
    {
        OwnedList<Tracker> a;
        for (int i = 0; i < 3; i++) a.push_back(Tracker(i));
        OwnedList<Tracker> b(std::move(a));
        CHECK_EQ(Tracker::live, 3);  // moving a list moves nodes, not elements
        CHECK(a.empty());
        CHECK_EQ(a.size(), 0u);
        CHECK_EQ(b.size(), 3u);
        a.push_back(Tracker(9));  // the moved-from list is valid and reusable
        CHECK_EQ(popId(a), 9);
        b.push_back(Tracker(3));  // the new owner's tail is correct
        CHECK_EQ(b.size(), 4u);
        for (int i = 0; i < 4; i++) CHECK_EQ(popId(b), i);
        CHECK(b.empty());
    }
    CHECK_EQ(Tracker::live, 0);
    // move assignment frees the old contents; self-move is harmless
    {
        OwnedList<Tracker> b, c;
        for (int i = 0; i < 3; i++) b.push_back(Tracker(i));
        for (int i = 10; i < 12; i++) c.push_back(Tracker(i));
        CHECK_EQ(Tracker::live, 5);
        c = std::move(b);
        CHECK_EQ(Tracker::live, 3);  // c's two old elements are gone
        CHECK(b.empty());
        CHECK_EQ(c.size(), 3u);
        OwnedList<Tracker>& alias = c;
        c = std::move(alias);  // self-move: contents must survive
        CHECK_EQ(c.size(), 3u);
        CHECK_EQ(Tracker::live, 3);
        CHECK_EQ(popId(c), 0);
        b.push_back(Tracker(7));  // moved-from list works
        CHECK_EQ(popId(b), 7);
    }
    CHECK_EQ(Tracker::live, 0);
    // append (splice) in O(1)
    {
        OwnedList<Tracker> a, b, empty;
        a.push_back(Tracker(1));
        a.push_back(Tracker(2));
        b.push_back(Tracker(3));
        b.push_back(Tracker(4));
        a.append(std::move(b));
        CHECK_EQ(Tracker::live, 4);
        CHECK(b.empty());
        CHECK_EQ(a.size(), 4u);
        a.append(std::move(empty));  // appending an empty list changes nothing
        CHECK_EQ(a.size(), 4u);
        a.push_back(Tracker(5));  // tail follows the appended nodes
        b.append(std::move(a));   // appending onto an empty list takes everything
        CHECK(a.empty());
        CHECK_EQ(b.size(), 5u);
        for (int i = 1; i <= 5; i++) CHECK_EQ(popId(b), i);
        // the appended-from list is reusable
        a.push_back(Tracker(8));
        CHECK_EQ(popId(a), 8);
    }
    CHECK_EQ(Tracker::live, 0);
    // reverse
    {
        OwnedList<int> l;
        for (int i = 1; i <= 4; i++) l.push_back(i);
        l.reverse();
        CHECK_EQ(l.size(), 4u);
        l.push_back(0);  // the old head is the new tail
        CHECK_EQ(popInt(l), 4);
        CHECK_EQ(popInt(l), 3);
        CHECK_EQ(popInt(l), 2);
        CHECK_EQ(popInt(l), 1);
        CHECK_EQ(popInt(l), 0);
        OwnedList<int> one;
        one.push_back(7);
        one.reverse();
        one.push_back(8);
        CHECK_EQ(popInt(one), 7);
        CHECK_EQ(popInt(one), 8);
    }
    // large: a recursive node destructor overflows the stack on 1,000,000 nodes
    {
        const int n = 1000000;
        {
            OwnedList<int> l;
            for (int i = 0; i < n; i++) l.push_back(i);
            CHECK_EQ(l.size(), (size_t)n);
        }  // destructor
        OwnedList<int> l;
        for (int i = 0; i < n; i++) l.push_front(i);
        l.reverse();
        CHECK(l.front() != nullptr);
        if (l.front()) CHECK_EQ(*l.front(), 0);
        long long sum = 0;
        for (int i = 0; i < 1000; i++) sum += popInt(l);
        CHECK_EQ(sum, 499500LL);  // 0 + 1 + ... + 999
        l.clear();                 // clear() must be safe on a long chain too
        CHECK(l.empty());

        OwnedList<int> a, b;
        for (int i = 0; i < n; i++) a.push_back(i);
        for (int i = 0; i < n; i++) b.push_back(i);
        a = std::move(b);  // old 1,000,000-node chain freed here
        CHECK_EQ(a.size(), (size_t)n);
        OwnedList<int> big;
        big.append(std::move(a));
        big.append(std::move(a));  // appending a now-empty list
        CHECK_EQ(big.size(), (size_t)n);
    }
    return check::report();
}
