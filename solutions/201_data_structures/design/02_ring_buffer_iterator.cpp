// Problem: A fixed-capacity ring buffer (overwrites the oldest element when full) with real STL-style iterators.
//   template <class T> class RingBuffer {
//     explicit RingBuffer(size_t capacity);           capacity >= 1
//     void   push_back(const T& v);                   O(1); when full, the oldest element is overwritten
//     size_t size() const;  size_t capacity() const;  bool empty() const;
//     T& operator[](size_t i);  (+ const)             i-th OLDEST element, 0 <= i < size()
//     T& front();  T& back();   (+ const)             oldest / newest; undefined if empty
//     void clear();                                   size becomes 0, capacity unchanged
//     iterator / const_iterator, begin() end() (+ const overloads), cbegin() cend()
//   };
//   Iteration order is oldest to newest. The iterator is a forward iterator: it must carry the five std
//   iterator typedefs (iterator_category, value_type, difference_type, pointer, reference) so std algorithms and
//   range-for work, support *, ->, prefix and postfix ++, ==, !=, be default constructible, and an iterator must
//   convert implicitly to a const_iterator. Writing through a non-const iterator modifies the buffer.
//   T need NOT be default constructible (only copy constructible and copy assignable).
//
// Examples (capacity 3):
//   push_back 1,2 -> {1,2}      push_back 3 -> {1,2,3}      push_back 4 -> {2,3,4}      push_back 5 -> {3,4,5}
//   std::accumulate(rb.begin(), rb.end(), 0) works; for (int& x : rb) x *= 2 doubles the stored values.
//
// Constraints: capacity up to 1,000,000; up to 3,000,000 push_back calls in a test.
// Required complexity: push_back O(1); iterator step O(1); begin()/end() O(1); O(capacity) space.
// Run: make run FILE=curriculum/201_data_structures/design/02_ring_buffer_iterator.cpp
//
// Think first:
//  1. When the buffer is exactly full, the physical "start" slot and the physical "one past the last" slot are
//     the same slot. How does an iterator tell begin() from end() then? What should an iterator remember
//     (physical index? logical offset from the oldest?).
//  2. One iterator class or two? How do you avoid writing the const and non-const versions twice, and how
//     does iterator -> const_iterator conversion fall out of that design?
//  3. If T has no default constructor, what can the storage not be (e.g. std::vector<T>(capacity))?
//     What happens to existing iterators when push_back overwrites an element?

#include "check.h"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <optional>
#include <ranges>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// ===== YOUR CODE BELOW (stub) =====
template <class T>
class RingBuffer {
    std::vector<T> data_;
    size_t cap_;
    size_t head_ = 0;  // physical index of the oldest element once full
    T& at(size_t i) { return data_[(head_ + i) % cap_]; }
    const T& at(size_t i) const { return data_[(head_ + i) % cap_]; }
public:
    template <bool Const>
    class Iter {
        using Owner = std::conditional_t<Const, const RingBuffer, RingBuffer>;
        Owner* o_ = nullptr;
        size_t i_ = 0;
        template <bool> friend class Iter;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = std::conditional_t<Const, const T*, T*>;
        using reference = std::conditional_t<Const, const T&, T&>;
        Iter() = default;
        Iter(Owner* o, size_t i) : o_(o), i_(i) {}
        template <bool C, class = std::enable_if_t<Const && !C>>
        Iter(const Iter<C>& other) : o_(other.o_), i_(other.i_) {}
        reference operator*() const { return o_->at(i_); }
        pointer operator->() const { return &o_->at(i_); }
        Iter& operator++() { ++i_; return *this; }
        Iter operator++(int) { Iter t = *this; ++i_; return t; }
        bool operator==(const Iter& r) const { return o_ == r.o_ && i_ == r.i_; }
        bool operator!=(const Iter& r) const { return !(*this == r); }
    };
    using iterator = Iter<false>;
    using const_iterator = Iter<true>;

    explicit RingBuffer(size_t capacity) : cap_(capacity) { data_.reserve(capacity); }
    void push_back(const T& v) {
        if (data_.size() < cap_) data_.push_back(v);
        else { data_[head_] = v; head_ = (head_ + 1) % cap_; }
    }
    size_t size() const { return data_.size(); }
    size_t capacity() const { return cap_; }
    bool empty() const { return data_.empty(); }
    T& operator[](size_t i) { return at(i); }
    const T& operator[](size_t i) const { return at(i); }
    T& front() { return at(0); }
    T& back() { return at(size() - 1); }
    const T& front() const { return at(0); }
    const T& back() const { return at(size() - 1); }
    void clear() { data_.clear(); head_ = 0; }
    iterator begin() { return iterator(this, 0); }
    iterator end() { return iterator(this, size()); }
    const_iterator begin() const { return const_iterator(this, 0); }
    const_iterator end() const { return const_iterator(this, size()); }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }
};
// ===== TESTS (do not edit below) =====
using V = std::vector<int>;

struct NoDefault {  // copyable, but no default constructor
    int v;
    explicit NoDefault(int x) : v(x) {}
};

template <class RB>
static V contents(const RB& rb) {
    V out;
    for (auto it = rb.begin(); it != rb.end(); ++it) out.push_back(*it);
    return out;
}

// Dereference that reports -999 instead of crashing when `it` is already at end() (keeps failing runs readable).
template <class RB, class It>
static int valInt(RB& rb, It it) {
    It e = rb.end();
    return it == e ? -999 : *it;
}

int main() {
    // concepts: this is what "a real forward iterator" means to the standard library
    CHECK((std::forward_iterator<RingBuffer<int>::iterator>));
    CHECK((std::forward_iterator<RingBuffer<int>::const_iterator>));
    CHECK((std::ranges::forward_range<RingBuffer<int>>));
    CHECK((std::ranges::forward_range<const RingBuffer<int>>));
    CHECK((std::is_convertible_v<RingBuffer<int>::iterator, RingBuffer<int>::const_iterator>));
    CHECK((!std::is_convertible_v<RingBuffer<int>::const_iterator, RingBuffer<int>::iterator>));

    // empty
    {
        RingBuffer<int> rb(4);
        CHECK_EQ(rb.size(), 0u);
        CHECK_EQ(rb.capacity(), 4u);
        CHECK(rb.empty());
        CHECK(rb.begin() == rb.end());
        CHECK(rb.cbegin() == rb.cend());
        int count = 0;
        for ([[maybe_unused]] int x : rb) count++;
        CHECK_EQ(count, 0);
    }
    // partially filled, then exactly full, then overwritten
    {
        RingBuffer<int> rb(3);
        rb.push_back(1);
        rb.push_back(2);
        CHECK_EQ(contents(rb), (V{1, 2}));
        CHECK_EQ(rb.size(), 2u);
        CHECK(!rb.empty());
        CHECK_EQ(rb.front(), 1);
        CHECK_EQ(rb.back(), 2);
        rb.push_back(3);  // exactly full: begin() and end() map to the same physical slot
        CHECK_EQ(contents(rb), (V{1, 2, 3}));
        CHECK(rb.begin() != rb.end());
        CHECK_EQ(std::distance(rb.begin(), rb.end()), 3);
        rb.push_back(4);
        CHECK_EQ(contents(rb), (V{2, 3, 4}));
        CHECK_EQ(rb.front(), 2);
        CHECK_EQ(rb.back(), 4);
        CHECK_EQ(rb[1], 3);
        rb.push_back(5);
        CHECK_EQ(contents(rb), (V{3, 4, 5}));
        CHECK_EQ(rb.size(), 3u);
        CHECK_EQ(rb.capacity(), 3u);
    }
    // many wrap-arounds
    {
        RingBuffer<int> rb(7);
        for (int i = 1; i <= 100; i++) rb.push_back(i);
        CHECK_EQ(contents(rb), (V{94, 95, 96, 97, 98, 99, 100}));
        CHECK_EQ(rb[0], 94);
        CHECK_EQ(rb[6], 100);
    }
    // capacity 1
    {
        RingBuffer<int> rb(1);
        CHECK(rb.begin() == rb.end());
        rb.push_back(5);
        CHECK_EQ(contents(rb), (V{5}));
        rb.push_back(6);
        CHECK_EQ(contents(rb), (V{6}));
    }
    // iterator operations
    {
        RingBuffer<int> rb(4);
        for (int i = 10; i < 16; i++) rb.push_back(i);  // {12,13,14,15}
        auto it = rb.begin();
        CHECK_EQ(valInt(rb, it), 12);
        auto old = it++;
        CHECK_EQ(valInt(rb, old), 12);
        CHECK_EQ(valInt(rb, it), 13);
        auto copy = it;
        ++it;
        CHECK_EQ(valInt(rb, copy), 13);  // copies are independent
        CHECK(copy != it);
        ++copy;
        CHECK(copy == it);
        RingBuffer<int>::iterator def;  // default-constructed iterators exist
        (void)def;
        // walking exactly size() steps lands on end()
        auto e = rb.begin();
        for (int i = 0; i < 4; i++) ++e;
        CHECK(e == rb.end());
    }
    // writing through iterators, range-for with a reference
    {
        RingBuffer<int> rb(3);
        for (int i = 1; i <= 5; i++) rb.push_back(i);  // {3,4,5}
        for (int& x : rb) x *= 2;
        CHECK_EQ(contents(rb), (V{6, 8, 10}));
        auto b = rb.begin();
        if (b != rb.end()) *b = 0;
        CHECK_EQ(rb.front(), 0);
        rb.push_back(11);
        CHECK_EQ(contents(rb), (V{8, 10, 11}));
    }
    // const access and iterator -> const_iterator conversion
    {
        RingBuffer<int> rb(3);
        for (int i = 1; i <= 4; i++) rb.push_back(i);  // {2,3,4}
        RingBuffer<int>::const_iterator ci = rb.begin();  // implicit conversion
        CHECK_EQ(valInt(rb, ci), 2);
        const RingBuffer<int>& cr = rb;
        CHECK_EQ(contents(cr), (V{2, 3, 4}));
        CHECK_EQ(cr.front(), 2);
        CHECK_EQ(cr.back(), 4);
        CHECK_EQ(cr[2], 4);
        CHECK(cr.begin() == cr.cbegin());
        int s = 0;
        for (int x : cr) s += x;
        CHECK_EQ(s, 9);
    }
    // standard algorithms
    {
        RingBuffer<int> rb(5);
        for (int i = 1; i <= 8; i++) rb.push_back(i);  // {4,5,6,7,8}
        CHECK_EQ(std::accumulate(rb.begin(), rb.end(), 0), 30);
        CHECK(std::find(rb.begin(), rb.end(), 6) != rb.end());
        CHECK(std::find(rb.begin(), rb.end(), 3) == rb.end());
        V copy(rb.begin(), rb.end());
        CHECK_EQ(copy, (V{4, 5, 6, 7, 8}));
        CHECK(std::equal(rb.begin(), rb.end(), copy.begin()));
        CHECK_EQ(std::count_if(rb.begin(), rb.end(), [](int x) { return x % 2 == 0; }), 3);
        V out;
        std::copy(rb.begin(), rb.end(), std::back_inserter(out));
        CHECK_EQ(out, copy);
        CHECK_EQ(std::ranges::distance(rb), 5);
        auto mx = std::max_element(rb.begin(), rb.end());
        CHECK_EQ(valInt(rb, mx), 8);
        std::replace(rb.begin(), rb.end(), 8, 80);
        CHECK_EQ(rb.back(), 80);
    }
    // operator-> on a class type
    {
        RingBuffer<std::pair<int, std::string>> rb(2);
        rb.push_back({1, "one"});
        rb.push_back({2, "two"});
        rb.push_back({3, "three"});
        auto it = rb.begin();
        CHECK(it != rb.end());
        if (it != rb.end()) {
            CHECK_EQ(it->first, 2);
            CHECK_EQ(it->second, std::string("two"));
            ++it;
            CHECK(it != rb.end());
            if (it != rb.end()) CHECK_EQ(it->second, std::string("three"));
        }
    }
    // non-default-constructible element type
    {
        RingBuffer<NoDefault> rb(3);
        for (int i = 1; i <= 5; i++) rb.push_back(NoDefault(i));
        int sum = 0, count = 0;
        for (const NoDefault& n : rb) {
            sum += n.v;
            count++;
        }
        CHECK_EQ(count, 3);
        CHECK_EQ(sum, 12);  // 3 + 4 + 5
    }
    // clear and reuse
    {
        RingBuffer<int> rb(3);
        for (int i = 1; i <= 7; i++) rb.push_back(i);
        rb.clear();
        CHECK(rb.empty());
        CHECK(rb.begin() == rb.end());
        CHECK_EQ(rb.capacity(), 3u);
        rb.push_back(42);
        CHECK_EQ(contents(rb), (V{42}));
        rb.push_back(43);
        rb.push_back(44);
        rb.push_back(45);
        CHECK_EQ(contents(rb), (V{43, 44, 45}));
    }
    // large: erasing the front of a vector on every overwrite would be O(capacity) per push
    {
        const size_t cap = 1000000;
        const int pushes = 3000000;
        RingBuffer<int> rb(cap);
        for (int i = 0; i < pushes; i++) rb.push_back(i);
        CHECK_EQ(rb.size(), cap);
        CHECK_EQ(rb.front(), pushes - (int)cap);
        CHECK_EQ(rb.back(), pushes - 1);
        long long sum = std::accumulate(rb.begin(), rb.end(), 0LL);
        CHECK_EQ(sum, 2499999500000LL);  // 2,000,000 + ... + 2,999,999
        long long viaFor = 0;
        for (int x : rb) viaFor += x;
        CHECK_EQ(viaFor, sum);
        CHECK_EQ(rb[500000], 2500000);
    }
    return check::report();
}
