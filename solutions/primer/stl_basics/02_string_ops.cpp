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
    int n = 0;
    for (char c : s) {
        char l = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (l == 'a' || l == 'e' || l == 'i' || l == 'o' || l == 'u') n++;
    }
    return n;
}

std::string extractAfter(const std::string& s, const std::string& marker) {
    size_t pos = s.find(marker);
    if (pos == std::string::npos) return "";
    return s.substr(pos + marker.size());
}

std::string joinWithComma(const std::vector<std::string>& parts) {
    std::string out;
    for (size_t i = 0; i < parts.size(); i++) {
        if (i > 0) out += ", ";
        out += parts[i];
    }
    return out;
}

std::string capitalizeWords(std::string s) {
    bool startOfWord = true;
    for (char& c : s) {
        unsigned char u = static_cast<unsigned char>(c);
        if (c == ' ') {
            startOfWord = true;
        } else {
            c = static_cast<char>(startOfWord ? std::toupper(u) : std::tolower(u));
            startOfWord = false;
        }
    }
    return s;
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
