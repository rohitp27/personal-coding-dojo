// Problem: Three small std::stack exercises.
//   isBalanced(s)               true if every (, [, { in s is closed by the matching
//                               bracket in the right order; other characters are ignored
//   maxDepth(s)                 deepest nesting of '(' in a balanced string made of only
//                               '(' and ')'; 0 for ""
//   removeAdjacentDuplicates(s) repeatedly delete two equal neighbouring characters until
//                               none remain
//
// Examples:
//   isBalanced("a(b[c]{d})")          -> true
//   isBalanced("([)]")                -> false
//   maxDepth("(()(()))")              -> 3
//   removeAdjacentDuplicates("abbaca")-> "ca"     (abbaca -> aaca -> ca)
//
// Edge cases: empty string, lone closer ")", lone opener "(", leftover openers.
//
// Run with: make run FILE=practice/cpp/stacks_queues/03_balanced_brackets.cpp
//
// Syntax reminder:
//   std::stack<char> st;  st.push(c);  st.top();  st.pop();  st.empty();
//   // check st.empty() BEFORE top()/pop(); both are undefined behaviour on an empty stack
//   switch (c) { case ')': ... break; }     // or: if (c == ')' && ...)
//   std::string out;  out.push_back(c);  out.back();  out.pop_back();   // a string works as a stack

#include <stack>
#include <unordered_set>
#include <string>

#include "check.h"

 
std::unordered_set<char> opening = {'(', '[', '{'};
std::unordered_set<char> closing = {']', ')', '}'};

bool isOpening(char a) {
    return opening.find(a) != opening.end();
}

bool isClosing(char a) {
    return closing.find(a) != closing.end();
}

bool isMatchingClosing(char a, char b) {
    return (a=='(' && b == ')') || (a=='[' && b == ']') || (a=='{' && b == '}');
}

bool isBalanced(const std::string& s) {
    (void)s;
    std::stack<char> st;
    for(char c : s) {
        if (opening.find(c) != opening.end()){ //is opening brace
            st.push(c);
        } else if (closing.find(c) != closing.end() ) {
            if (st.empty()) return false;
            else if (isMatchingClosing( st.top(), c)) {
                // this matches, pop
                st.pop();
            } else {
                return false;
            }
        } else {
            // pass
        }
    }
    return st.empty();;
}

int max(int a, int b) {
    return (b<a)?a:b;
}

int maxDepth(const std::string& s) {
    (void)s;
    std::stack<char> stk;
    int maxDepth = 0;
    for (char current : s) {
        if ( isOpening(current) ) {
            // add to the stack
            stk.push(current);
            // update the maxDepth 
            // assumption : the brackets are already balanced
            maxDepth = max(maxDepth, stk.size());
        } else if ( isClosing(current) ) {
            // std::assert(!stk.empty());
            // std::assert(isMatchingClosing(stk.top(), current) ); // assumption, brackets are always matching
            stk.pop();
        } else {
        //    std::assert(false);
        }
    }
    return maxDepth;
}

char NULLCHAR = '\0';

int traverseAndDelete(std::string& s) {
    int deletedPairs = 0;
    for (size_t i = 0; i + 1 < s.length(); i++) {
        char current = s.at(i);
        char next = s.at(i+1);
        if (current == NULLCHAR || next == NULLCHAR) continue; //already deleted or next is deleted
        if (current == next) {
            s[i] = NULLCHAR;
            s[i+1] = NULLCHAR;
            deletedPairs++;
        }
    }
    return deletedPairs;
}

std::string traverseAndExtract(std::string& s) {
    std::string output = "";
    for(char c : s) {
        if (c == NULLCHAR) continue;
        output.push_back(c);
    }
    return output;
}

std::string removeAdjacentDuplicates(const std::string& s) {
    (void)s;
    std::string hold(s);
    std::string output;
    while( true ) {
        int deleted = traverseAndDelete(hold);
        if (deleted == 0) {
            break;
        }
        std::string temp = traverseAndExtract(hold);
        hold = temp;
    }
    return hold;
}

int main() {
    CHECK(isBalanced(""));
    CHECK(isBalanced("()"));
    CHECK(isBalanced("([]{})"));
    CHECK(isBalanced("a(b[c]{d})"));
    CHECK(!isBalanced("(]"));
    CHECK(!isBalanced("(("));
    CHECK(!isBalanced(")"));
    CHECK(!isBalanced("([)]"));
    CHECK(!isBalanced("(()"));

    CHECK_EQ(maxDepth(""), 0);
    CHECK_EQ(maxDepth("()"), 1);
    CHECK_EQ(maxDepth("()()"), 1);
    CHECK_EQ(maxDepth("(()(()))"), 3);

    CHECK_EQ(removeAdjacentDuplicates(""), "");
    CHECK_EQ(removeAdjacentDuplicates("a"), "a");
    CHECK_EQ(removeAdjacentDuplicates("aa"), "");
    CHECK_EQ(removeAdjacentDuplicates("abbaca"), "ca");
    CHECK_EQ(removeAdjacentDuplicates("abba"), "");

    return check::report();
}
