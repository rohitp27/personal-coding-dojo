/*
 * Problem: new/delete and a class that owns heap memory (rule of three / five)
 *   Implement IntBuffer, a tiny fixed-size array of ints allocated with new[].
 *   Because the class owns a raw pointer, the compiler-generated copy would copy the
 *   POINTER (two objects, one array, double delete). You must define the special members:
 *
 *     IntBuffer(n)                 Allocate n ints, all zero. n == 0 is allowed.
 *     ~IntBuffer()                 delete[] the array.
 *     IntBuffer(const IntBuffer&)  Deep copy: a new array with the same contents.
 *     operator=(const IntBuffer&)  Deep copy assignment. Safe for self-assignment
 *                                  (b = b) and must not leak the old array.
 *     IntBuffer(IntBuffer&&)       Move: steal the pointer and size; leave the source
 *                                  empty (size 0, null data). noexcept.
 *     operator=(IntBuffer&&)       Move assignment: free ours, steal theirs, leave the
 *                                  source empty. noexcept. Safe for self-move.
 *     size() const                 Number of elements.
 *     operator[](i)                Element access (const and non-const versions); no
 *                                  bounds checking required.
 *     resize(m)                    Change the size to m, keeping min(old, m) leading
 *                                  elements and zero-filling any new ones.
 *
 * Examples:
 *   IntBuffer a(3); a[0] = 7; IntBuffer b = a; b[0] = 9;  -> a[0] is still 7
 *   IntBuffer c = std::move(a);                           -> a.size() == 0, c.size() == 3
 *
 * Constraints / edge cases:
 *   - Zero-size buffers (new int[0] is legal; delete[] of nullptr is legal).
 *   - Self-assignment and self-move-assignment.
 *   - Copies are independent (writing one does not affect the other).
 *   - No leaks: the tests replace the global operator new/delete to count live
 *     allocations and require the count to return to its starting value after the
 *     objects are gone.
 *   - Exceptions: if new[] throws during copy-assignment, the old contents should survive
 *     (allocate first, then delete the old array). Not tested, but worth doing.
 *
 * Target complexity: O(n) for construct/copy/resize; O(1) for move, size, access.
 *
 * Run: make run FILE=curriculum/101_basics/10_heap_owning_class.cpp
 *
 * (Stub note: delete the `(void)x;` lines once you use the parameter. The stub declares
 *  the five special members as `= default` so it compiles; replace each of them.)
 */
#include <cstddef>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <new>
#include <utility>

#include <algorithm>
#include <cstdlib>
#include <cctype>
#include "check.h"

// ---- Allocation counter used by the tests (given; leave it alone) ----
// Replaces the global allocation functions so tests can count live allocations.
static long g_liveAllocs = 0;

void* operator new(std::size_t size) {
    void* p = std::malloc(size == 0 ? 1 : size);
    if (!p) throw std::bad_alloc();
    ++g_liveAllocs;
    return p;
}
void operator delete(void* p) noexcept {
    if (p) --g_liveAllocs;
    std::free(p);
}
void operator delete(void* p, std::size_t) noexcept { operator delete(p); }

// ===================== YOUR CODE =====================

class IntBuffer {
public:
    explicit IntBuffer(std::size_t n) : data_(new int[n]()), size_(n) {}
    ~IntBuffer() { delete[] data_; }
    IntBuffer(const IntBuffer& o) : data_(new int[o.size_]), size_(o.size_) {
        for (std::size_t i = 0; i < size_; i++) data_[i] = o.data_[i];
    }
    IntBuffer& operator=(const IntBuffer& o) {
        if (this == &o) return *this;
        int* nd = new int[o.size_];
        for (std::size_t i = 0; i < o.size_; i++) nd[i] = o.data_[i];
        delete[] data_;
        data_ = nd;
        size_ = o.size_;
        return *this;
    }
    IntBuffer(IntBuffer&& o) noexcept : data_(o.data_), size_(o.size_) {
        o.data_ = nullptr;
        o.size_ = 0;
    }
    IntBuffer& operator=(IntBuffer&& o) noexcept {
        if (this == &o) return *this;
        delete[] data_;
        data_ = o.data_;
        size_ = o.size_;
        o.data_ = nullptr;
        o.size_ = 0;
        return *this;
    }
    std::size_t size() const { return size_; }
    int& operator[](std::size_t i) { return data_[i]; }
    const int& operator[](std::size_t i) const { return data_[i]; }
    void resize(std::size_t m) {
        int* nd = new int[m]();
        std::size_t k = size_ < m ? size_ : m;
        for (std::size_t i = 0; i < k; i++) nd[i] = data_[i];
        delete[] data_;
        data_ = nd;
        size_ = m;
    }
private:
    int* data_ = nullptr;
    std::size_t size_ = 0;
};

// ===================== TESTS =====================

static IntBuffer filled(std::size_t n) {
    IntBuffer b(n);
    for (std::size_t i = 0; i < n; i++) b[i] = (int)i + 1;
    return b;
}

static bool equalsSeq(const IntBuffer& b, std::initializer_list<int> expected) {
    if (b.size() != expected.size()) return false;
    std::size_t i = 0;
    for (int e : expected) {
        if (b[i++] != e) return false;
    }
    return true;
}

int main() {
    // construction: zero-filled, correct size
    {
        IntBuffer b(4);
        CHECK_EQ(b.size(), std::size_t(4));
        CHECK(equalsSeq(b, {0, 0, 0, 0}));
        b[2] = 42;
        CHECK(equalsSeq(b, {0, 0, 42, 0}));
        const IntBuffer& cb = b;
        CHECK_EQ(cb[2], 42);
    }
    {
        IntBuffer z(0);
        CHECK_EQ(z.size(), std::size_t(0));
    }

    // copy constructor is deep
    {
        IntBuffer a = filled(3);
        IntBuffer b(a);
        CHECK(equalsSeq(b, {1, 2, 3}));
        b[0] = 99;
        CHECK(equalsSeq(a, {1, 2, 3}));
        CHECK(equalsSeq(b, {99, 2, 3}));
        a[1] = -5;
        CHECK(equalsSeq(b, {99, 2, 3}));
        IntBuffer empty(0);
        IntBuffer empty2(empty);
        CHECK_EQ(empty2.size(), std::size_t(0));
    }

    // copy assignment is deep, handles different sizes and self-assignment
    {
        IntBuffer a = filled(3);
        IntBuffer b = filled(5);
        b = a;
        CHECK_EQ(b.size(), std::size_t(3));
        CHECK(equalsSeq(b, {1, 2, 3}));
        b[0] = 77;
        CHECK(equalsSeq(a, {1, 2, 3}));

        IntBuffer small(1);
        small = filled(4);  // assign a bigger one
        CHECK(equalsSeq(small, {1, 2, 3, 4}));

        IntBuffer& self = a;
        a = self;  // self-assignment
        CHECK(equalsSeq(a, {1, 2, 3}));

        IntBuffer z(0);
        a = z;
        CHECK_EQ(a.size(), std::size_t(0));
    }

    // move constructor steals and empties the source
    {
        IntBuffer a = filled(3);
        IntBuffer b(std::move(a));
        CHECK(equalsSeq(b, {1, 2, 3}));
        CHECK_EQ(a.size(), std::size_t(0));
    }

    // move assignment steals, empties the source, self-move is safe
    {
        IntBuffer a = filled(3);
        IntBuffer b = filled(2);
        b = std::move(a);
        CHECK(equalsSeq(b, {1, 2, 3}));
        CHECK_EQ(a.size(), std::size_t(0));

        IntBuffer& self = b;
        b = std::move(self);
        CHECK(equalsSeq(b, {1, 2, 3}));
    }

    // resize keeps the prefix and zero-fills growth
    {
        IntBuffer a = filled(3);
        a.resize(5);
        CHECK(equalsSeq(a, {1, 2, 3, 0, 0}));
        a.resize(2);
        CHECK(equalsSeq(a, {1, 2}));
        a.resize(2);
        CHECK(equalsSeq(a, {1, 2}));
        a.resize(0);
        CHECK_EQ(a.size(), std::size_t(0));
        a.resize(3);
        CHECK(equalsSeq(a, {0, 0, 0}));
        IntBuffer e(0);
        e.resize(2);
        CHECK(equalsSeq(e, {0, 0}));
    }

    // no leaks and no double frees: live allocation count returns to the baseline
    {
        long before = g_liveAllocs;
        {
            IntBuffer a = filled(10);
            IntBuffer b(a);
            IntBuffer c(1);
            c = a;
            c = b;
            IntBuffer d(std::move(c));
            IntBuffer e(2);
            e = std::move(d);
            e.resize(50);
            e.resize(1);
            IntBuffer f(0);
            f = e;
        }
        CHECK_EQ(g_liveAllocs, before);
    }

    return check::report();
}
