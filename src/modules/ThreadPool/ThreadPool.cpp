#include "ThreadPool.h"

ThreadPool::ThreadPool(const size_t &numThreads)
{
    _threads.reserve(numThreads);
    for (size_t i = 0; i < numThreads; ++i)
    {
        _threads.emplace_back(&ThreadPool::run, this);
    }
}
ThreadPool::~ThreadPool(){
    _completed = true;

    for(uint32_t i = 0; i < _threads.size();++i){
        _taskPoolCV.notify_all();
        _threads[i].join();
    }
}


void ThreadPool::wait(const int64_t &taskId)
{
    std::unique_lock<std::mutex> lock(_completedTasksMutex);

    _completedTasksCV.wait(lock, [this, taskId]() -> bool
                           { return _completedTasks.find(taskId) != _completedTasks.end(); });
}

void ThreadPool::waitAll()
{
    std::unique_lock<std::mutex> lock(_taskPoolMutex);

    _completedTasksCV.wait(lock, [this]() -> bool
                           {
        std::lock_guard<std::mutex> taskLock(_completedTasksMutex);
        return _taskPool.empty()&& _lastId==_completedTasks.size(); });
}

bool ThreadPool::isCalculated(const int64_t &taskId) const
{
    std::lock_guard<std::mutex> lock(_completedTasksMutex);
    if (_completedTasks.find(taskId) != _completedTasks.end())
    {
        return true;
    }
    return false;
}

void ThreadPool::run()
{
    while (!_completed)
    {
        std::unique_lock<std::mutex> lock(_taskPoolMutex);
        _taskPoolCV.wait(lock, [this]() -> bool
                         { return !_taskPool.empty() || _completed; });

        if (!_taskPool.empty())
        {
            auto elem = std::move(std::move(_taskPool.front()));
            _taskPool.pop();
            lock.unlock();

            elem.first.get();

            std::lock_guard<std::mutex> lock(_completedTasksMutex);
            _completedTasks.insert(elem.second);

            _completedTasksCV.notify_all();
        }
    }
}