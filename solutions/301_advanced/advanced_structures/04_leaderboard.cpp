// 301 advanced > advanced_structures > 04 Leaderboard (hash map + order statistics)
//
// Problem:
//   Design a live leaderboard. Players have 64-bit ids and integer scores in
//   [0, maxScore]. Operations:
//     setScore(id, score)  create the player or overwrite their score
//     remove(id)           delete the player (no-op if absent)
//     size()               number of players
//     rank(id)             1 + (number of players with a STRICTLY higher score);
//                          players with equal scores share a rank; -1 if id is absent
//     topKSum(k)           sum of the k highest scores (all players if k > size, 0 if k <= 0)
//     kthScore(k)          the score of the player in position k (1-based) when
//                          sorted by score descending; -1 if k < 1 or k > size
//
// Examples (maxScore 100): set (1,50) (2,70) (3,50) (4,10)
//   rank(2) = 1, rank(1) = 2, rank(3) = 2, rank(4) = 4, rank(99) = -1
//   topKSum(2) = 120, topKSum(10) = 180, kthScore(3) = 50, kthScore(5) = -1
//   setScore(4, 90): rank(4) = 1, topKSum(2) = 160
//
// Constraints:
//   Up to 200000 players, up to 400000 operations, 1 <= maxScore <= 1'000'000.
//
// Required complexity: every operation O(log maxScore) or O(log n) (expected O(1)
//   for the id lookup). A sorted std::vector re-sorted per query, or a std::set /
//   std::map walked to find a rank (O(n)), is too slow.
//
// Assumptions: scores are always within range (no validation needed). std::set has
//   no O(log n) rank, so something else must provide the order statistics.
//
// Think first:
//   1. Which two structures are needed and which fact does each answer? What does
//      the hash map own that the ordered structure cannot?
//   2. You need "how many players have score > s" and "sum of the top k". What
//      single indexed structure over the score domain supports both, and how do
//      you answer topKSum when k cuts through a group of equal scores?
//   3. setScore on an existing id: list every piece of state you must update, in
//      order, so a crash between steps could never double-count the player.
//
// BEFORE CODING: write your approach, invariant and complexity in a comment
// block right here.
//
// Run: make run FILE=curriculum/301_advanced/advanced_structures/04_leaderboard.cpp

#include "check.h"

#include <algorithm>
#include <cstdint>
#include <unordered_map>
#include <vector>

// ===== YOUR CODE BELOW =====
class Leaderboard {
    int maxScore_;
    int m_;  // number of buckets = maxScore + 1; bucket index = maxScore - score (best first)
    std::vector<long long> cnt_, sum_;
    std::unordered_map<long long, int> score_;
    int players_ = 0;

    void upd(int score, long long dc) {
        int idx = maxScore_ - score;
        for (int x = idx + 1; x <= m_; x += x & -x) {
            cnt_[x] += dc;
            sum_[x] += dc * score;
        }
    }
    // number of players in buckets [0, idx)
    long long cntBefore(int idx) const {
        long long s = 0;
        for (int x = idx; x > 0; x -= x & -x) s += cnt_[x];
        return s;
    }

public:
    explicit Leaderboard(int maxScore)
        : maxScore_(maxScore), m_(maxScore + 1), cnt_(maxScore + 2, 0), sum_(maxScore + 2, 0) {}
    void setScore(long long id, int score) {
        auto it = score_.find(id);
        if (it != score_.end()) {
            upd(it->second, -1);
            it->second = score;
        } else {
            score_[id] = score;
            players_++;
        }
        upd(score, +1);
    }
    void remove(long long id) {
        auto it = score_.find(id);
        if (it == score_.end()) return;
        upd(it->second, -1);
        score_.erase(it);
        players_--;
    }
    int size() const { return players_; }
    int rank(long long id) const {
        auto it = score_.find(id);
        if (it == score_.end()) return -1;
        return (int)cntBefore(maxScore_ - it->second) + 1;
    }
    // descend: largest pos with cumulative count <= k; returns pos, accumulated count and sum
    long long topKSum(int k) const {
        if (k <= 0) return 0;
        if (k >= players_) k = players_;
        int pos = 0;
        long long c = 0, s = 0;
        int pw = 1;
        while (pw * 2 <= m_) pw *= 2;
        for (; pw > 0; pw >>= 1) {
            if (pos + pw <= m_ && c + cnt_[pos + pw] <= k) {
                pos += pw;
                c += cnt_[pos];
                s += sum_[pos];
            }
        }
        if (c < k) {  // partial bucket at index pos (0-based)
            int score = maxScore_ - pos;
            s += (k - c) * score;
        }
        return s;
    }
    int kthScore(int k) const {
        if (k < 1 || k > players_) return -1;
        int pos = 0;
        long long c = 0;
        int pw = 1;
        while (pw * 2 <= m_) pw *= 2;
        for (; pw > 0; pw >>= 1) {
            if (pos + pw <= m_ && c + cnt_[pos + pw] < k) {
                pos += pw;
                c += cnt_[pos];
            }
        }
        return maxScore_ - pos;
    }
};
// ===== TESTS (do not edit) =====
struct Rng {
    uint64_t s;
    uint64_t next() {
        s = s * 6364136223846793005ULL + 1442695040888963407ULL;
        return s >> 33;
    }
};

int main() {
    {
        Leaderboard lb(100);
        CHECK_EQ(lb.size(), 0);
        CHECK_EQ(lb.rank(1), -1);
        CHECK_EQ(lb.topKSum(3), 0LL);
        CHECK_EQ(lb.kthScore(1), -1);

        lb.setScore(1, 50);
        lb.setScore(2, 70);
        lb.setScore(3, 50);
        lb.setScore(4, 10);
        CHECK_EQ(lb.size(), 4);
        CHECK_EQ(lb.rank(2), 1);
        CHECK_EQ(lb.rank(1), 2);
        CHECK_EQ(lb.rank(3), 2);
        CHECK_EQ(lb.rank(4), 4);
        CHECK_EQ(lb.rank(99), -1);
        CHECK_EQ(lb.topKSum(0), 0LL);
        CHECK_EQ(lb.topKSum(-3), 0LL);
        CHECK_EQ(lb.topKSum(1), 70LL);
        CHECK_EQ(lb.topKSum(2), 120LL);
        CHECK_EQ(lb.topKSum(3), 170LL);
        CHECK_EQ(lb.topKSum(4), 180LL);
        CHECK_EQ(lb.topKSum(10), 180LL);
        CHECK_EQ(lb.kthScore(1), 70);
        CHECK_EQ(lb.kthScore(2), 50);
        CHECK_EQ(lb.kthScore(3), 50);
        CHECK_EQ(lb.kthScore(4), 10);
        CHECK_EQ(lb.kthScore(5), -1);
        CHECK_EQ(lb.kthScore(0), -1);

        lb.setScore(4, 90);  // overwrite moves the player, never duplicates
        CHECK_EQ(lb.size(), 4);
        CHECK_EQ(lb.rank(4), 1);
        CHECK_EQ(lb.rank(2), 2);
        CHECK_EQ(lb.rank(1), 3);
        CHECK_EQ(lb.topKSum(2), 160LL);
        CHECK_EQ(lb.kthScore(4), 50);

        lb.remove(2);
        lb.remove(2);  // second remove is a no-op
        lb.remove(12345);
        CHECK_EQ(lb.size(), 3);
        CHECK_EQ(lb.rank(2), -1);
        CHECK_EQ(lb.rank(1), 2);
        CHECK_EQ(lb.topKSum(3), 190LL);

        lb.setScore(1, 50);  // same score again: no change
        CHECK_EQ(lb.size(), 3);
        CHECK_EQ(lb.topKSum(3), 190LL);

        lb.setScore(7, 0);  // boundaries
        lb.setScore(8, 100);
        CHECK_EQ(lb.rank(8), 1);
        CHECK_EQ(lb.rank(7), 5);
        CHECK_EQ(lb.kthScore(5), 0);
        CHECK_EQ(lb.topKSum(5), 290LL);
        lb.setScore(-5, 100);  // ids are 64-bit and can be negative or huge
        lb.setScore(1LL << 40, 100);
        CHECK_EQ(lb.rank(-5), 1);
        CHECK_EQ(lb.rank(8), 1);
        CHECK_EQ(lb.rank(1LL << 40), 1);
        CHECK_EQ(lb.rank(4), 4);
    }

    // random operations vs brute force
    {
        Rng rng{4242};
        Leaderboard lb(20);
        std::unordered_map<long long, int> ref;
        bool ok = true;
        for (int op = 0; op < 20000; op++) {
            long long id = (long long)(rng.next() % 30);
            int kind = (int)(rng.next() % 6);
            if (kind <= 1) {
                int s = (int)(rng.next() % 21);
                lb.setScore(id, s);
                ref[id] = s;
            } else if (kind == 2) {
                lb.remove(id);
                ref.erase(id);
            } else if (kind == 3) {
                int want = -1;
                auto it = ref.find(id);
                if (it != ref.end()) {
                    want = 1;
                    for (auto& [pid, s] : ref)
                        if (s > it->second) want++;
                }
                if (lb.rank(id) != want) ok = false;
            } else {
                std::vector<int> scores;
                for (auto& [pid, s] : ref) scores.push_back(s);
                std::sort(scores.rbegin(), scores.rend());
                int k = (int)(rng.next() % 35) - 2;
                if (kind == 4) {
                    long long want = 0;
                    for (int i = 0; i < k && i < (int)scores.size(); i++) want += scores[i];
                    if (lb.topKSum(k) != want) ok = false;
                } else {
                    int want = (k >= 1 && k <= (int)scores.size()) ? scores[k - 1] : -1;
                    if (lb.kthScore(k) != want) ok = false;
                }
            }
            if (lb.size() != (int)ref.size()) ok = false;
        }
        CHECK(ok);
    }

    // large: ids 0..N-1, score = id % 1000 -> exactly 200 players per score value 0..999
    {
        const int N = 200000, G = 200;
        Leaderboard lb(1000);
        for (int id = 0; id < N; id++) lb.setScore(id, id % 1000);
        CHECK_EQ(lb.size(), N);
        bool ok = true;
        for (int id = 0; id < N; id += 331)
            if (lb.rank(id) != 1 + G * (999 - id % 1000)) ok = false;
        CHECK(ok);
        auto top = [&](long long k) {
            long long sum = 0, rem = std::min<long long>(k, N);
            for (int s = 999; s >= 0 && rem > 0; s--) {
                long long take = std::min<long long>(rem, G);
                sum += take * s;
                rem -= take;
            }
            return sum;
        };
        ok = true;
        for (int k : {0, 1, 199, 200, 201, 99999, 200000, 300000})
            if (lb.topKSum(k) != top(k)) ok = false;
        CHECK(ok);
        CHECK_EQ(lb.kthScore(1), 999);
        CHECK_EQ(lb.kthScore(200), 999);
        CHECK_EQ(lb.kthScore(201), 998);
        CHECK_EQ(lb.kthScore(N), 0);
        CHECK_EQ(lb.kthScore(N + 1), -1);

        for (int id = 0; id < N; id++)
            if (id % 1000 < 500) lb.remove(id);
        CHECK_EQ(lb.size(), N / 2);
        CHECK_EQ(lb.kthScore(N / 2), 500);
        CHECK_EQ(lb.kthScore(N / 2 + 1), -1);
        CHECK_EQ(lb.rank(999), 1);
        CHECK_EQ(lb.rank(500), 1 + G * 499);
        CHECK_EQ(lb.rank(3), -1);
    }

    return check::report();
}
