// 301 advanced > systems_and_concurrency > 02 Fixed-size thread pool with graceful shutdown
//
// Problem:
//   Implement ThreadPool: N worker threads that run submitted tasks.
//
//     ThreadPool(n)     starts exactly n worker threads (n >= 1).
//     submit(task)      enqueue a task. Returns false (task NOT run) once shutdown
//                       has begun, otherwise true. Callable from any thread,
//                       including from inside a running task (before shutdown).
//     waitIdle()        block until the queue is empty AND no task is running
//                       (counts tasks submitted by other tasks while waiting).
//                       Never called from inside a task.
//     shutdown()        GRACEFUL: stop accepting tasks, let every already-queued
//                       task run to completion, then join all workers. Idempotent
//                       and safe to call from several threads at once; every
//                       caller returns only after the workers are joined.
//                       Never called from inside a task.
//     ~ThreadPool()     behaves like shutdown().
//     completed()       tasks that finished running (including ones that threw).
//     failed()          tasks that terminated by throwing; an exception in a task
//                       must NOT kill the worker or the process.
//
// Examples:
//   pool(4); submit 1000 increments; shutdown() -> counter == 1000, submit() now false
//   4 workers, 4 tasks that each wait until all 4 have started -> they all finish
//
// Constraints: up to 64 workers, up to ~1e5 queued tasks. Tasks are std::function<void()>.
//
// Required complexity: O(1) per submit and per task dispatch; idle workers must
//   sleep on a condition variable (no polling loops, no busy-waiting).
//
// Assumptions: tasks are independent; no priorities; FIFO dispatch order is not tested.
//
// Think first:
//   1. What can go wrong? Lost wake-up on submit, workers exiting while tasks are
//      still queued during shutdown, double-join when shutdown is called twice
//      or concurrently, a task exception terminating a worker thread.
//   2. Which mutex protects which state (queue, stop flag, running count,
//      counters), and which condition variables exist (work available, idle)?
//   3. What exact condition lets a worker exit, and why is it NOT just "stop flag set"?
//
// BEFORE CODING: write what can go wrong, which lock protects what, and your
// approach in a comment block right here.
//
// Run: make run FILE=curriculum/301_advanced/systems_and_concurrency/02_thread_pool.cpp
// Tests are deterministic in outcome (joins, counters, bounded waits). A watchdog
// aborts with a message if your code deadlocks.

#include "check.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <deque>
#include <functional>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

// ===== YOUR CODE BELOW =====
class ThreadPool {
    mutable std::mutex m_;
    std::condition_variable work_, idle_;
    std::deque<std::function<void()>> q_;
    std::vector<std::thread> workers_;
    std::once_flag joinOnce_;
    bool stop_ = false;
    size_t running_ = 0, completed_ = 0, failed_ = 0;

    void loop() {
        for (;;) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lk(m_);
                work_.wait(lk, [&] { return stop_ || !q_.empty(); });
                if (q_.empty()) return;  // stop_ is set and nothing left to drain
                task = std::move(q_.front());
                q_.pop_front();
                running_++;
            }
            bool threw = false;
            try {
                task();
            } catch (...) {
                threw = true;
            }
            {
                std::lock_guard<std::mutex> lk(m_);
                running_--;
                completed_++;
                if (threw) failed_++;
                if (q_.empty() && running_ == 0) idle_.notify_all();
            }
        }
    }

public:
    explicit ThreadPool(size_t numThreads) {
        for (size_t i = 0; i < numThreads; i++) workers_.emplace_back([this] { loop(); });
    }
    ~ThreadPool() { shutdown(); }
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    bool submit(std::function<void()> task) {
        {
            std::lock_guard<std::mutex> lk(m_);
            if (stop_) return false;
            q_.push_back(std::move(task));
        }
        work_.notify_one();
        return true;
    }
    void waitIdle() {
        std::unique_lock<std::mutex> lk(m_);
        idle_.wait(lk, [&] { return q_.empty() && running_ == 0; });
    }
    void shutdown() {
        {
            std::lock_guard<std::mutex> lk(m_);
            stop_ = true;
        }
        work_.notify_all();
        std::call_once(joinOnce_, [&] {
            for (auto& w : workers_) w.join();
        });
    }
    size_t completed() const {
        std::lock_guard<std::mutex> lk(m_);
        return completed_;
    }
    size_t failed() const {
        std::lock_guard<std::mutex> lk(m_);
        return failed_;
    }
};
// ===== TESTS (do not edit) =====
using namespace std::chrono_literals;

static void startWatchdog(int secs) {
    std::thread([secs] {
        std::this_thread::sleep_for(std::chrono::seconds(secs));
        std::cerr << "WATCHDOG: tests ran longer than " << secs << "s - deadlock or lost wake-up?\n";
        std::_Exit(3);
    }).detach();
}

int main() {
    startWatchdog(40);

    // basic: run everything, waitIdle, counters, submit after shutdown
    {
        ThreadPool pool(4);
        std::atomic<int> counter{0};
        bool allAccepted = true;
        for (int i = 0; i < 1000; i++) allAccepted &= pool.submit([&] { counter++; });
        CHECK(allAccepted);
        pool.waitIdle();
        CHECK_EQ(counter.load(), 1000);
        CHECK_EQ(pool.completed(), (size_t)1000);
        CHECK_EQ(pool.failed(), (size_t)0);
        pool.shutdown();
        CHECK(!pool.submit([&] { counter += 1000000; }));
        pool.shutdown();  // idempotent
        CHECK_EQ(counter.load(), 1000);
    }

    // graceful: shutdown right after submitting must still run every queued task
    {
        ThreadPool pool(4);
        std::atomic<int> counter{0};
        for (int i = 0; i < 200; i++)
            pool.submit([&] {
                std::this_thread::sleep_for(1ms);
                counter++;
            });
        pool.shutdown();
        CHECK_EQ(counter.load(), 200);
    }

    // destructor drains the queue too
    {
        std::atomic<int> counter{0};
        {
            ThreadPool pool(2);
            for (int i = 0; i < 100; i++) pool.submit([&] { counter++; });
        }
        CHECK_EQ(counter.load(), 100);
    }

    // exactly N workers run concurrently: 4 tasks that rendezvous can only all finish with 4 threads
    {
        ThreadPool pool(4);
        std::atomic<int> started{0}, sawAll{0};
        for (int i = 0; i < 4; i++)
            pool.submit([&] {
                started++;
                auto deadline = std::chrono::steady_clock::now() + 5s;
                while (started.load() < 4 && std::chrono::steady_clock::now() < deadline)
                    std::this_thread::sleep_for(100us);
                if (started.load() >= 4) sawAll++;
            });
        pool.shutdown();
        CHECK_EQ(sawAll.load(), 4);
    }

    // never more than N tasks at once, and real parallelism is used
    {
        ThreadPool pool(3);
        std::atomic<int> cur{0}, maxSeen{0};
        for (int i = 0; i < 200; i++)
            pool.submit([&] {
                int c = ++cur;
                int prev = maxSeen.load();
                while (c > prev && !maxSeen.compare_exchange_weak(prev, c)) {}
                std::this_thread::sleep_for(200us);
                --cur;
            });
        pool.shutdown();
        CHECK(maxSeen.load() <= 3);
        CHECK(maxSeen.load() >= 1);
        CHECK_EQ(cur.load(), 0);
    }

    // task exceptions do not kill workers
    {
        ThreadPool pool(2);
        std::atomic<int> counter{0};
        for (int i = 0; i < 10; i++) {
            pool.submit([] { throw std::runtime_error("boom"); });
            pool.submit([&] { counter++; });
        }
        pool.shutdown();
        CHECK_EQ(counter.load(), 10);
        CHECK_EQ(pool.completed(), (size_t)20);
        CHECK_EQ(pool.failed(), (size_t)10);
    }

    // a running task submits more work; waitIdle accounts for it
    {
        ThreadPool pool(2);
        std::atomic<int> counter{0};
        pool.submit([&] {
            counter++;
            for (int i = 0; i < 5; i++) pool.submit([&] { counter++; });
        });
        pool.waitIdle();
        CHECK_EQ(counter.load(), 6);
        pool.shutdown();
    }

    // shutdown from several threads at once, then again
    {
        ThreadPool pool(3);
        std::atomic<int> counter{0};
        for (int i = 0; i < 300; i++) pool.submit([&] { counter++; });
        std::vector<std::thread> callers;
        for (int i = 0; i < 3; i++) callers.emplace_back([&] { pool.shutdown(); });
        for (auto& t : callers) t.join();
        pool.shutdown();
        CHECK_EQ(counter.load(), 300);
        CHECK(!pool.submit([] {}));
    }

    // many concurrent submitters
    {
        ThreadPool pool(4);
        std::atomic<int> counter{0}, rejected{0};
        std::vector<std::thread> submitters;
        for (int t = 0; t < 4; t++)
            submitters.emplace_back([&] {
                for (int i = 0; i < 2500; i++)
                    if (!pool.submit([&] { counter++; })) rejected++;
            });
        for (auto& t : submitters) t.join();
        pool.waitIdle();
        CHECK_EQ(rejected.load(), 0);
        CHECK_EQ(counter.load(), 10000);
        CHECK_EQ(pool.completed(), (size_t)10000);
    }

    return check::report();
}
