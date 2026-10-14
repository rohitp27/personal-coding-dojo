// Problem: Linked List Surgery - Reverse in K-Groups and Find the Cycle Start
// Two singly-linked-list functions. Both must relink existing nodes (never copy values
// between nodes) and use O(1) extra space.
//
// (a) reverseKGroup(head, k): reverse the list k nodes at a time and return the new head.
//     If fewer than k nodes remain at the end, leave that last group as it is. k >= 1.
//       {1,2,3,4,5}, k=2 -> {2,1,4,3,5}
//       {1,2,3,4,5}, k=3 -> {3,2,1,4,5}
//       {1,2,3,4,5}, k=1 -> unchanged;  k > length -> unchanged
//
// (b) cycleStart(head): if the list has a cycle (some node's next points back to an earlier
//     node), return the node where the cycle begins; otherwise return nullptr.
//       {3,2,0,-4} with the tail linked back to the node holding 2 -> the node holding 2
//
// Constraints: 0 <= length <= 1,000,000. Recursion that uses one stack frame per group
// or per node can crash on the large tests - the space requirement is O(1).
// Required complexity: O(n) time, O(1) extra space for both.
//
// Think first:
//   1. For (a), before reversing a group, how do you know there are at least k nodes left,
//      and which pointers must you remember so the reversed group can be reconnected to
//      the part before it and the part after it?
//   2. For (a), what does the returned head look like when k == 1, k == length, k > length,
//      and for an empty list?
//   3. For (b), after the slow and fast pointers meet inside the cycle, why does restarting
//      one pointer at the head and stepping both one node at a time make them meet exactly
//      at the cycle start?
//
// Run: make run FILE=curriculum/102_fundamentals/stacks_queues_lists/03_linked_list_ops.cpp

#include "check.h"

#include <algorithm>
#include <deque>
#include <numeric>
#include <vector>

struct ListNode {
    int val;
    ListNode* next;
};

//@REF
ListNode* reverseKGroup(ListNode* head, int k) {
    ListNode dummy{0, head};
    ListNode* prev = &dummy;
    while (true) {
        ListNode* probe = prev->next;
        int cnt = 0;
        while (probe && cnt < k) {
            probe = probe->next;
            cnt++;
        }
        if (cnt < k) break;
        ListNode* groupStart = prev->next;
        ListNode* cur = groupStart;
        ListNode* before = probe;  // node after the group
        for (int i = 0; i < k; i++) {
            ListNode* nxt = cur->next;
            cur->next = before;
            before = cur;
            cur = nxt;
        }
        prev->next = before;
        prev = groupStart;
    }
    return dummy.next;
}

ListNode* cycleStart(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            ListNode* p = head;
            while (p != slow) {
                p = p->next;
                slow = slow->next;
            }
            return p;
        }
    }
    return nullptr;
}
ListNode* reverseKGroup(ListNode* head, [[maybe_unused]] int k) {
    // TODO
    return head;
}

ListNode* cycleStart([[maybe_unused]] ListNode* head) {
    // TODO
    return nullptr;
}

// ---- test helpers (nodes live in a pool so the tests never leak) ----
static std::deque<ListNode> pool;

struct Built {
    ListNode* head;
    std::vector<ListNode*> nodes;
};

// Builds a list of the given values. If cyclePos >= 0, links the tail to nodes[cyclePos].
static Built build(const std::vector<int>& vals, int cyclePos = -1) {
    Built b{nullptr, {}};
    for (int v : vals) {
        pool.push_back({v, nullptr});
        b.nodes.push_back(&pool.back());
        if (b.nodes.size() > 1) b.nodes[b.nodes.size() - 2]->next = b.nodes.back();
    }
    if (!b.nodes.empty()) b.head = b.nodes[0];
    if (cyclePos >= 0 && !b.nodes.empty()) b.nodes.back()->next = b.nodes[cyclePos];
    return b;
}

// Reads a list; stops after a bound so a broken (cyclic) result cannot hang the test.
static std::vector<int> toVec(ListNode* h) {
    std::vector<int> out;
    for (size_t steps = 0; h && steps < 5000000; steps++, h = h->next) out.push_back(h->val);
    return out;
}

static std::vector<int> range(int n) {
    std::vector<int> v(n);
    std::iota(v.begin(), v.end(), 1);
    return v;
}

static std::vector<int> expectedGroups(int n, int k) {
    std::vector<int> v = range(n);
    for (int i = 0; i + k <= n; i += k) std::reverse(v.begin() + i, v.begin() + i + k);
    return v;
}

int main() {
    using V = std::vector<int>;
    // --- (a) reverseKGroup ---
    CHECK_EQ(toVec(reverseKGroup(build({1, 2, 3, 4, 5}).head, 2)), (V{2, 1, 4, 3, 5}));
    CHECK_EQ(toVec(reverseKGroup(build({1, 2, 3, 4, 5}).head, 3)), (V{3, 2, 1, 4, 5}));
    CHECK_EQ(toVec(reverseKGroup(build({1, 2, 3, 4, 5, 6}).head, 3)), (V{3, 2, 1, 6, 5, 4}));
    CHECK_EQ(toVec(reverseKGroup(build({1, 2, 3, 4, 5}).head, 1)), (V{1, 2, 3, 4, 5}));
    CHECK_EQ(toVec(reverseKGroup(build({1, 2, 3, 4, 5}).head, 5)), (V{5, 4, 3, 2, 1}));
    CHECK_EQ(toVec(reverseKGroup(build({1, 2, 3, 4, 5}).head, 6)), (V{1, 2, 3, 4, 5}));
    CHECK_EQ(toVec(reverseKGroup(build({1, 2}).head, 2)), (V{2, 1}));
    CHECK_EQ(toVec(reverseKGroup(build({1}).head, 1)), (V{1}));
    CHECK_EQ(toVec(reverseKGroup(build({1}).head, 2)), (V{1}));
    CHECK(reverseKGroup(nullptr, 3) == nullptr);
    {
        // nodes must be relinked, not copied: the new head is the old k-th node
        Built b = build({10, 20, 30, 40});
        ListNode* h = reverseKGroup(b.head, 2);
        CHECK(h == b.nodes[1]);
        CHECK(h->next == b.nodes[0]);
        CHECK(b.nodes[0]->next == b.nodes[3]);
        CHECK(b.nodes[3]->next == b.nodes[2]);
        CHECK(b.nodes[2]->next == nullptr);
        CHECK_EQ(b.nodes[1]->val, 20);  // values untouched
    }
    // large: 500,000 nodes. Deep recursion (one frame per group) overflows the stack for k=2.
    for (int k : {2, 3, 500000}) {
        Built b = build(range(500000));
        CHECK_EQ(toVec(reverseKGroup(b.head, k)) == expectedGroups(500000, k), true);
    }

    // --- (b) cycleStart ---
    CHECK(cycleStart(nullptr) == nullptr);
    CHECK(cycleStart(build({1}).head) == nullptr);
    CHECK(cycleStart(build({1, 2}).head) == nullptr);
    CHECK(cycleStart(build({1, 2, 3, 4, 5}).head) == nullptr);
    {
        Built b = build({1}, 0);  // single node pointing at itself
        CHECK(cycleStart(b.head) == b.nodes[0]);
    }
    {
        Built b = build({3, 2, 0, -4}, 1);
        CHECK(cycleStart(b.head) == b.nodes[1]);
    }
    {
        Built b = build({1, 2}, 0);
        CHECK(cycleStart(b.head) == b.nodes[0]);
    }
    {
        Built b = build({1, 2}, 1);  // tail points at itself
        CHECK(cycleStart(b.head) == b.nodes[1]);
    }
    {
        Built b = build({1, 2, 3, 4, 5, 6, 7}, 6);
        CHECK(cycleStart(b.head) == b.nodes[6]);
    }
    {
        Built b = build(range(1000000), 123456);
        CHECK(cycleStart(b.head) == b.nodes[123456]);
    }
    {
        Built b = build(range(1000000), 0);
        CHECK(cycleStart(b.head) == b.nodes[0]);
    }
    {
        Built b = build(range(1000000));
        CHECK(cycleStart(b.head) == nullptr);
    }

    return check::report();
}
