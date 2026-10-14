// Tiny test helper. Unlike assert(), a failed check prints file:line, expected and
// actual, keeps going, and the process exits non-zero via `return check::report();`.
//
//   CHECK(cond)              boolean check
//   CHECK_EQ(actual, expect) equality; prints both values (vectors, pairs, strings ok)
//   CHECK_NEAR(a, b, eps)    floating-point comparison
#pragma once

#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace check {

inline int failures = 0;
inline int total = 0;

template <typename T>
std::string show(const T& v);

template <typename A, typename B>
std::string show(const std::pair<A, B>& p) {
    return "(" + show(p.first) + ", " + show(p.second) + ")";
}

template <typename T>
std::string show(const std::vector<T>& v) {
    std::string s = "[";
    for (size_t i = 0; i < v.size(); i++) {
        if (i) s += ", ";
        s += show(v[i]);
    }
    return s + "]";
}

template <typename K, typename V>
std::string show(const std::map<K, V>& m) {
    std::string s = "{";
    bool first = true;
    for (const auto& [k, v] : m) {
        if (!first) s += ", ";
        first = false;
        s += show(k) + ": " + show(v);
    }
    return s + "}";
}

inline std::string show(const std::string& v) { return "\"" + v + "\""; }
inline std::string show(const char* v) { return std::string("\"") + v + "\""; }
inline std::string show(bool v) { return v ? "true" : "false"; }

template <typename T>
std::string show(const T& v) {
    std::ostringstream os;
    os << v;
    return os.str();
}

inline void fail(const char* file, int line, const std::string& msg) {
    failures++;
    std::cerr << "  FAILED " << file << ":" << line << "  " << msg << "\n";
}

template <typename A, typename B>
void eq(const A& actual, const B& expected, const char* expr, const char* file, int line) {
    total++;
    if (!(actual == expected)) {
        fail(file, line, std::string(expr) + "\n      expected: " + show(expected) +
                             "\n      actual:   " + show(actual));
    }
}

inline void near(double a, double b, double eps, const char* expr, const char* file, int line) {
    total++;
    if (!(std::fabs(a - b) <= eps)) {
        fail(file, line, std::string(expr) + "  (" + std::to_string(a) + " vs " + std::to_string(b) + ")");
    }
}

inline int report() {
    if (failures == 0) {
        std::cout << "all " << total << " checks passed\n";
        return 0;
    }
    std::cout << failures << " of " << total << " checks FAILED\n";
    return 1;
}

}  // namespace check

#define CHECK(cond)                                                          \
    do {                                                                     \
        check::total++;                                                      \
        if (!(cond)) check::fail(__FILE__, __LINE__, "CHECK(" #cond ")");    \
    } while (0)
// `expected` is variadic so braced lists work: CHECK_EQ(f(x), std::vector<int>{1, 2})
#define CHECK_EQ(actual, ...) check::eq((actual), (__VA_ARGS__), #actual " == " #__VA_ARGS__, __FILE__, __LINE__)
#define CHECK_NEAR(a, b, eps) check::near((a), (b), (eps), #a " ~= " #b, __FILE__, __LINE__)
