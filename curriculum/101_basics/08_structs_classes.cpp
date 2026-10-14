/*
 * Problem: Structs, classes, constructors, const methods
 *   Part 1 - struct Point: a plain data type with two const-correct methods.
 *   Part 2 - class BankAccount: a class with private state, a constructor using a
 *            member-initializer list, and const accessors.
 *
 *   Point (already declared; you write the method bodies):
 *     distSq(other) const     Squared Euclidean distance to `other`, as long long.
 *     translated(dx, dy) const  A NEW Point moved by (dx, dy); the original is unchanged.
 *
 *   BankAccount(owner, initialCents = 0)
 *     - A negative initial balance is treated as 0.
 *     deposit(cents)          Add cents. Return false (no change) if cents <= 0.
 *     withdraw(cents)         Subtract cents. Return false (no change) if cents <= 0 or
 *                             cents > balance. Withdrawing exactly the balance is fine.
 *     transferTo(other, cents)  Withdraw from this and deposit into other; all-or-nothing.
 *                             Return false (no change to either) if the withdraw would
 *                             fail or if `other` is this same account.
 *     balance() const, owner() const, transactionCount() const
 *                             Read-only accessors. transactionCount counts SUCCESSFUL
 *                             deposits and withdrawals made on this account. A transfer
 *                             counts as one withdrawal on the source and one deposit on
 *                             the target.
 *
 * Examples:
 *   Point{0,0}.distSq(Point{3,4})            -> 25
 *   BankAccount a("ann", 100); a.withdraw(30) -> true, a.balance() == 70
 *   a.withdraw(1000)                         -> false, balance still 70
 *
 * Constraints / edge cases:
 *   - Mark every method that does not modify the object `const`; the tests call them
 *     through a const reference, so a missing `const` is a compile error.
 *   - Coordinates up to +-1e9: dx * dx overflows int, compute in long long.
 *   - Failed operations must not change the balance or the transaction count.
 *
 * Target complexity: O(1) for every method.
 *
 * Run: make run FILE=curriculum/101_basics/08_structs_classes.cpp
 *
 * (Stub note: delete the `(void)x;` lines once you use the parameter.)
 */
#include <iostream>
#include <string>
#include <utility>

#include "check.h"

// ===================== YOUR CODE =====================

struct Point {
    int x = 0;
    int y = 0;

    bool operator==(const Point&) const = default;  // given: compares x and y

    long long distSq(const Point& other) const {
        // TODO. Hint: long long dx = (long long)x - other.x; same for dy.
        (void)other;
        return -1;
    }

    Point translated(int dx, int dy) const {
        // TODO. Hint: return a new Point; do not modify *this (this method is const).
        (void)dx;
        (void)dy;
        return Point{-1, -1};
    }
};

// given: lets CHECK_EQ print a Point
inline std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "(" << p.x << "," << p.y << ")";
}

class BankAccount {
public:
    // TODO. Hint: BankAccount(std::string owner, long long cents = 0) : owner_(std::move(owner)), balance_(...) {}
    BankAccount(std::string owner, long long initialCents = 0) {
        (void)owner;
        (void)initialCents;
    }

    bool deposit(long long cents) {
        // TODO
        (void)cents;
        return true;
    }

    bool withdraw(long long cents) {
        // TODO
        (void)cents;
        return true;
    }

    bool transferTo(BankAccount& other, long long cents) {
        // TODO. Hint: compare addresses (&other == this) to detect self-transfer.
        (void)other;
        (void)cents;
        return true;
    }

    long long balance() const { return -1; }       // TODO: return balance_
    const std::string& owner() const { return owner_; }  // given
    int transactionCount() const { return -1; }    // TODO: return txCount_

private:
    std::string owner_;
    [[maybe_unused]] long long balance_ = 0;  // silences the stub's unused warning; remove once used
    [[maybe_unused]] int txCount_ = 0;
};

// ===================== TESTS =====================

int main() {
    // Point::distSq
    {
        const Point origin{0, 0};
        CHECK_EQ(origin.distSq(Point{3, 4}), 25LL);
        CHECK_EQ(origin.distSq(origin), 0LL);
        CHECK_EQ((Point{1, 1}.distSq(Point{-2, -3})), 25LL);
        CHECK_EQ((Point{-1000000000, 0}.distSq(Point{1000000000, 0})), 4000000000000000000LL);
        CHECK_EQ((Point{-1000000000, -1000000000}.distSq(Point{1000000000, 1000000000})), 8000000000000000000LL);
        CHECK_EQ((Point{5, 5}.distSq(Point{5, 9})), 16LL);
    }

    // Point::translated
    {
        const Point p{1, 2};
        Point q = p.translated(3, -5);
        CHECK_EQ(q, Point({4, -3}));
        CHECK_EQ(p, Point({1, 2}));  // original unchanged
        CHECK_EQ(p.translated(0, 0), p);
        CHECK_EQ(Point{}.translated(7, 7), Point({7, 7}));
    }

    // BankAccount basics (accessors called through a const reference)
    {
        BankAccount a("ann", 100);
        const BankAccount& ca = a;
        CHECK_EQ(ca.owner(), "ann");
        CHECK_EQ(ca.balance(), 100LL);
        CHECK_EQ(ca.transactionCount(), 0);

        CHECK_EQ(a.withdraw(30), true);
        CHECK_EQ(ca.balance(), 70LL);
        CHECK_EQ(a.withdraw(1000), false);
        CHECK_EQ(ca.balance(), 70LL);
        CHECK_EQ(a.withdraw(70), true);  // exactly the balance
        CHECK_EQ(ca.balance(), 0LL);
        CHECK_EQ(a.withdraw(1), false);
        CHECK_EQ(a.withdraw(0), false);
        CHECK_EQ(a.withdraw(-5), false);
        CHECK_EQ(a.deposit(0), false);
        CHECK_EQ(a.deposit(-5), false);
        CHECK_EQ(a.deposit(250), true);
        CHECK_EQ(ca.balance(), 250LL);
        CHECK_EQ(ca.transactionCount(), 3);  // withdraw 30, withdraw 70, deposit 250
    }

    // defaults and clamping in the constructor
    {
        BankAccount b("bob");
        CHECK_EQ(b.balance(), 0LL);
        CHECK_EQ(b.owner(), "bob");
        BankAccount c("cy", -50);
        CHECK_EQ(c.balance(), 0LL);
        BankAccount d("", 5000000000LL);
        CHECK_EQ(d.balance(), 5000000000LL);
        CHECK_EQ(d.owner(), "");
    }

    // transferTo
    {
        BankAccount a("a", 100), b("b", 10);
        CHECK_EQ(a.transferTo(b, 40), true);
        CHECK_EQ(a.balance(), 60LL);
        CHECK_EQ(b.balance(), 50LL);
        CHECK_EQ(a.transactionCount(), 1);
        CHECK_EQ(b.transactionCount(), 1);

        CHECK_EQ(a.transferTo(b, 61), false);  // insufficient
        CHECK_EQ(a.balance(), 60LL);
        CHECK_EQ(b.balance(), 50LL);
        CHECK_EQ(a.transferTo(b, 0), false);
        CHECK_EQ(a.transferTo(b, -1), false);
        CHECK_EQ(a.transferTo(a, 10), false);  // self-transfer rejected
        CHECK_EQ(a.balance(), 60LL);
        CHECK_EQ(a.transactionCount(), 1);
        CHECK_EQ(b.transactionCount(), 1);

        CHECK_EQ(a.transferTo(b, 60), true);  // drain
        CHECK_EQ(a.balance(), 0LL);
        CHECK_EQ(b.balance(), 110LL);
    }

    return check::report();
}
