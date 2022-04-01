#pragma once

#include <thread>
#include <queue>
#include <future>

#include <unordered_set>

class ThreadPool
{
public:
    ThreadPool(const size_t &numThreads);
    ~ThreadPool();

    template <typename Func, typename... Args>
    inline int64_t addTask(const Func &taskFunc, Args &&...args)
    {
        int64_t taskIndex = _lastId++;
        std::lock_guard<std::mutex> lock(_taskPoolMutex);
        // std::bind the same way but async::deferred can't be without all args
        _taskPool.emplace(std::async(std::launch::deferred, taskFunc, args...), taskIndex);

        _taskPoolCV.notify_one();
        return taskIndex;
    }

    void wait(const int64_t &taskId);
    void waitAll();
    bool isCalculated(const int64_t &taskId) const;

private:
    void run();

    std::queue<std::pair<std::future<void>, int64_t>> _taskPool;
    std::condition_variable _taskPoolCV;
    mutable std::mutex _taskPoolMutex;

    std::unordered_set<int64_t> _completedTasks;
    std::condition_variable _completedTasksCV;
    mutable std::mutex _completedTasksMutex;

    std::vector<std::thread> _threads;

    std::atomic<bool> _completed{false};
    std::atomic<int64_t> _lastId{0};
};