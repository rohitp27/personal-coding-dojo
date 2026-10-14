// Problem: Subsets and Permutations (backtracking)
// Given a list of DISTINCT integers, implement two functions.
//
// (a) subsets(nums): return all 2^n subsets (including the empty subset and the full
//     set). Inside each subset, keep the elements in their input order.
//       {1,2,3} -> {}, {1}, {2}, {3}, {1,2}, {1,3}, {2,3}, {1,2,3}   (any order of subsets)
//       {}      -> {{}}
//
// (b) permutations(nums): return all n! orderings of the elements.
//       {1,2,3} -> {1,2,3}, {1,3,2}, {2,1,3}, {2,3,1}, {3,1,2}, {3,2,1}   (any order)
//       {}      -> {{}}   (exactly one permutation: the empty one)
//
// The order of the returned list does not matter; the tests sort it before comparing.
//
// Constraints: 0 <= nums.size() <= 16 for subsets, <= 8 for permutations; elements distinct.
// Required complexity: output-sensitive, O(n * 2^n) for subsets and O(n * n!) for
// permutations. No duplicates and no missing entries.
//
// Hint: both are a choose-explore-unchoose recursion over one shared "current" vector.
// For subsets, at each index decide to include or skip the element (emit when you pass the
// end). For permutations, at each position try every element not yet used (a bool[] "used"
// array or swapping into place), emit when the current vector is full. Remember to undo
// the choice (pop_back / unmark) after the recursive call returns.
//
// Run: make run FILE=curriculum/102_fundamentals/recursion_and_sorting/02_subsets_permutations.cpp

#include "check.h"

#include <algorithm>
#include <set>
#include <vector>

using VV = std::vector<std::vector<int>>;

VV subsets([[maybe_unused]] const std::vector<int>& nums) {
    // TODO
    return {};
}

VV permutations([[maybe_unused]] const std::vector<int>& nums) {
    // TODO
    return {};
}

static VV sorted(VV v) {
    std::sort(v.begin(), v.end());
    return v;
}

int main() {
    CHECK_EQ(sorted(subsets({1, 2, 3})), (VV{{}, {1}, {1, 2}, {1, 2, 3}, {1, 3}, {2}, {2, 3}, {3}}));
    CHECK_EQ(sorted(subsets({3, 1, 2})), (VV{{}, {1}, {1, 2}, {2}, {3}, {3, 1}, {3, 1, 2}, {3, 2}}));
    CHECK_EQ(sorted(subsets({})), (VV{{}}));
    CHECK_EQ(sorted(subsets({7})), (VV{{}, {7}}));
    CHECK_EQ(sorted(subsets({-1, 5})), (VV{{}, {-1}, {-1, 5}, {5}}));

    CHECK_EQ(sorted(permutations({1, 2, 3})),
             (VV{{1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1}}));
    CHECK_EQ(sorted(permutations({})), (VV{{}}));
    CHECK_EQ(sorted(permutations({7})), (VV{{7}}));
    CHECK_EQ(sorted(permutations({2, 1})), (VV{{1, 2}, {2, 1}}));

    // size and uniqueness at larger n (catches duplicates and missing entries)
    {
        std::vector<int> a(16);
        for (int i = 0; i < 16; i++) a[i] = i * 3 - 5;
        VV s = subsets(a);
        CHECK_EQ(s.size(), size_t{65536});
        std::set<std::vector<int>> uniq(s.begin(), s.end());
        CHECK_EQ(uniq.size(), size_t{65536});
        bool inOrder = true;
        for (const auto& sub : s)
            if (!std::is_sorted(sub.begin(), sub.end())) inOrder = false;  // input is increasing
        CHECK_EQ(inOrder, true);
    }
    {
        std::vector<int> a{10, 20, 30, 40, 50, 60, 70, 80};
        VV p = permutations(a);
        CHECK_EQ(p.size(), size_t{40320});
        std::set<std::vector<int>> uniq(p.begin(), p.end());
        CHECK_EQ(uniq.size(), size_t{40320});
        bool allPerms = true;
        for (auto q : p) {
            std::sort(q.begin(), q.end());
            if (q != a) allPerms = false;
        }
        CHECK_EQ(allPerms, true);
    }

    return check::report();
}
