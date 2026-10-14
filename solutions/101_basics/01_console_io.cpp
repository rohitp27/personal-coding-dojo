/*
 * Problem: Console I/O with streams
 *   Real programs read from std::cin and write to std::cout. To keep tests
 *   non-interactive, every function here takes a std::istream& / std::ostream&
 *   instead. In the tests those are std::istringstream / std::ostringstream.
 *
 *   Implement:
 *     sumAll(in)            Read whitespace-separated integers until the stream is
 *                           exhausted; return their sum.
 *     fizzBuzz(out, n)      For i = 1..n write one line per number: "Fizz" if i % 3 == 0,
 *                           "Buzz" if i % 5 == 0, "FizzBuzz" if both, else the number.
 *                           Every line ends with '\n'.
 *     formatLine(label, v)  Return "<label>: <v>" with v printed in fixed notation
 *                           with exactly 2 decimals.
 *     longestLine(in)       Read the input line by line and return the longest line
 *                           (without its newline). Ties: the first one wins.
 *
 * Examples:
 *   sumAll("1 2\n3\n")        -> 6
 *   fizzBuzz(out, 5)          -> "1\n2\nFizz\n4\nBuzz\n"
 *   formatLine("pi", 3.14159) -> "pi: 3.14"
 *   longestLine("ab\nabcd\nxy\n") -> "abcd"
 *
 * Constraints / edge cases:
 *   - Empty input: sumAll -> 0, longestLine -> "".
 *   - Negative numbers in sumAll; the sum may exceed 32 bits (use long long).
 *   - fizzBuzz with n <= 0 writes nothing.
 *   - A blank line is a valid (empty) line for longestLine.
 *
 * Target complexity: O(size of input) time, O(longest line) extra space.
 *
 * Run: make run FILE=curriculum/101_basics/01_console_io.cpp
 *
 * (Stub note: the `(void)x;` lines only silence "unused parameter" warnings while
 *  the TODOs are unfinished. Delete them once you use the parameter.)
 */
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <algorithm>
#include <cstdlib>
#include <cctype>
#include "check.h"

// ===================== YOUR CODE =====================

long long sumAll(std::istream& in) {
    long long x, s = 0;
    while (in >> x) s += x;
    return s;
}

void fizzBuzz(std::ostream& out, int n) {
    for (int i = 1; i <= n; i++) {
        if (i % 15 == 0) out << "FizzBuzz\n";
        else if (i % 3 == 0) out << "Fizz\n";
        else if (i % 5 == 0) out << "Buzz\n";
        else out << i << "\n";
    }
}

std::string formatLine(const std::string& label, double value) {
    std::ostringstream os;
    os << label << ": " << std::fixed << std::setprecision(2) << value;
    return os.str();
}

std::string longestLine(std::istream& in) {
    std::string line, best;
    while (std::getline(in, line)) if (line.size() > best.size()) best = line;
    return best;
}

// ===================== TESTS =====================

static long long sumOf(const std::string& s) {
    std::istringstream in(s);
    return sumAll(in);
}

static std::string fizz(int n) {
    std::ostringstream out;
    fizzBuzz(out, n);
    return out.str();
}

static std::string longest(const std::string& s) {
    std::istringstream in(s);
    return longestLine(in);
}

int main() {
    // sumAll
    CHECK_EQ(sumOf("1 2\n3\n"), 6LL);
    CHECK_EQ(sumOf(""), 0LL);
    CHECK_EQ(sumOf("   \n  \n"), 0LL);
    CHECK_EQ(sumOf("42"), 42LL);
    CHECK_EQ(sumOf("-5 5 10"), 10LL);
    CHECK_EQ(sumOf("-1 -2 -3"), -6LL);
    CHECK_EQ(sumOf("2000000000 2000000000 2000000000"), 6000000000LL);
    CHECK_EQ(sumOf("1\n\n\n2\t3"), 6LL);

    // fizzBuzz
    CHECK_EQ(fizz(5), "1\n2\nFizz\n4\nBuzz\n");
    CHECK_EQ(fizz(1), "1\n");
    CHECK_EQ(fizz(0), "");
    CHECK_EQ(fizz(-3), "");
    CHECK_EQ(fizz(15), "1\n2\nFizz\n4\nBuzz\nFizz\n7\n8\nFizz\nBuzz\n11\nFizz\n13\n14\nFizzBuzz\n");
    CHECK_EQ(fizz(3), "1\n2\nFizz\n");

    // formatLine
    CHECK_EQ(formatLine("pi", 3.14159), "pi: 3.14");
    CHECK_EQ(formatLine("x", 2), "x: 2.00");
    CHECK_EQ(formatLine("", 0), ": 0.00");
    CHECK_EQ(formatLine("neg", -1.5), "neg: -1.50");
    CHECK_EQ(formatLine("big", 1234567.891), "big: 1234567.89");
    CHECK_EQ(formatLine("tiny", 0.001), "tiny: 0.00");

    // longestLine
    CHECK_EQ(longest("ab\nabcd\nxy\n"), "abcd");
    CHECK_EQ(longest(""), "");
    CHECK_EQ(longest("only"), "only");
    CHECK_EQ(longest("aa\nbb\ncc\n"), "aa");  // tie -> first
    CHECK_EQ(longest("\n\n"), "");
    CHECK_EQ(longest("hello world\nhi\n"), "hello world");  // spaces count
    CHECK_EQ(longest("a\nbb\nccc"), "ccc");                 // no trailing newline

    return check::report();
}
