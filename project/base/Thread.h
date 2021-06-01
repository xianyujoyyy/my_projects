
#ifndef PROJECT_BASE_THREAD_H
#define PROJECT_BASE_THREAD_H

#include "project/base/Atomic.h"
#include "project/base/CountDownLatch.h"

#include <string>
#include <functional>
#include <pthread.h>

namespace NetLib {

class Thread : noncopyable {
public:
    typedef std::function<void()> ThreadFunc;
    
    explicit Thread(ThreadFunc _func, std::string _name = "");
    ~Thread();

    void start();
    int join();

    bool started() const { return started_; }
    pid_t tid() const { return tid_; }
    std::string name() const { return name_; }

private:
    void setDefaultName();

    bool started_;
    bool joined_;
    pid_t tid_;
    pthread_t pthread_id_;
    ThreadFunc thread_func_;    
    std::string name_;
    CountDownLatch latch_;

    static AtomicInt32 num_created_;

};

}   // namespace NetLib

#endif  // PROJECT_BASE_THREAD_H
