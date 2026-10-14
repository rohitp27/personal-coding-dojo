// Problem: Four string helpers.
//   countVowels(s)              number of a/e/i/o/u (either case) in s
//   extractAfter(s, marker)     the part of s after the first occurrence of marker;
//                               "" if marker does not occur
//   joinWithComma(parts)        parts joined by ", " (no trailing separator)
//   capitalizeWords(s)          first letter of each space-separated word upper-case,
//                               the rest lower-case; spaces are preserved as-is
//
// Examples:
//   countVowels("Hello")                  -> 2
//   extractAfter("key=value", "=")        -> "value"
//   joinWithComma({"a","b","c"})          -> "a, b, c"
//   capitalizeWords("hELLO wORLD")        -> "Hello World"
//
// Edge cases: empty string, marker at the very end, marker missing, one part only.
//
// Run with: make run FILE=practice/cpp/stl_basics/02_string_ops.cpp
//
// Syntax reminder:
//   size_t pos = s.find(marker);            // std::string::npos if not found
//   s.substr(pos + marker.size());          // from index to the end
//   s.substr(start, len);
//   out += "text"; out.append(other); out.push_back('c');
//   std::tolower(c), std::toupper(c), std::isalpha(c)   // <cctype>; cast arg: static_cast<unsigned char>(c)
//   for (char c : s) { ... }                // or index loop: s[i], s.size()

#include <cctype>
#include <string>
#include <vector>

#include "check.h"

int countVowels(const std::string& s) {
    (void)s;
    // TODO
    return -1;
}

std::string extractAfter(const std::string& s, const std::string& marker) {
    (void)s; (void)marker;
    // TODO
    return "?";
}

std::string joinWithComma(const std::vector<std::string>& parts) {
    (void)parts;
    // TODO
    return "?";
}

std::string capitalizeWords(std::string s) {
    (void)s;
    // TODO
    return "?";
}

int main() {
    CHECK_EQ(countVowels(""), 0);
    CHECK_EQ(countVowels("Hello"), 2);
    CHECK_EQ(countVowels("AEIOUaeiou"), 10);
    CHECK_EQ(countVowels("rhythm"), 0);

    CHECK_EQ(extractAfter("key=value", "="), "value");
    CHECK_EQ(extractAfter("key=", "="), "");
    CHECK_EQ(extractAfter("novalue", "="), "");
    CHECK_EQ(extractAfter("a::b::c", "::"), "b::c");
    CHECK_EQ(extractAfter("", "x"), "");

    CHECK_EQ(joinWithComma({}), "");
    CHECK_EQ(joinWithComma({"solo"}), "solo");
    CHECK_EQ(joinWithComma({"a", "b", "c"}), "a, b, c");

    CHECK_EQ(capitalizeWords(""), "");
    CHECK_EQ(capitalizeWords("x"), "X");
    CHECK_EQ(capitalizeWords("hELLO wORLD"), "Hello World");
    CHECK_EQ(capitalizeWords("  two  spaces"), "  Two  Spaces");

    return check::report();
}
