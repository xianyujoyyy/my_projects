
#ifndef BASE_THREADPOOL_H
#define BASE_THREADPOOL_H

#include "project1_websever/base/Condition.h"
#include "project1_websever/base/Mutex.h"
#include "project1_websever/base/Thread.h"
#include "project1_websever/base/Types.h"

#include <deque>
#include <vector>

namespace NetLib {

class ThreadPool : noncopyable {
public:
    typedef std::function<void ()> Task;

    explicit ThreadPool(const string& nameArg = string("ThreadPool"));
    ~ThreadPool();

    void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
    void setThreadInitCallback(const Task& cb) {
        threadInitCallback_ = cb;
    }

    void start(int numThreads);
    void stop();

    const string& name() const { return name_; }

    size_t queueSize() const;

    void run(Task f);

private:
    bool isFull() const;
    void runInThread();
    Task take();

    mutable MutexLock mutex_;
    Condition notEmpty_;
    Condition notFull_;
    string name_;
    Task threadInitCallback_;
    std::vector<std::unique_prt<NetLib::Thread>> threads_;
    std::deque<Task> queue_;
    size_t setMaxQueueSize_;
    bool running_;
};

}   // namespace NetLib

#endif  // BASE_THREADPOOL_H
