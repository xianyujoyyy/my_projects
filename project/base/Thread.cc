
#include "project/base/Thread.h"

#include <stdio.h>
#include <memory>

namespace NetLib {

AtomicInt32 Thread::num_created_;

struct ThreadData {
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadFunc thread_func_;
    std::string name_;
    pid_t* tid_;
    CountDownLatch* latch_;

    ThreadData(ThreadFunc _thread_func, std::string _name,
        pid_t* _tid, CountDownLatch* _latch) :
        thread_func_(std::move(_thread_func)),
        name_(_name),
        tid_(_tid),
        latch_(_latch) {}

    void runInThread() {
        *tid_ = CurrentThread::tid();
        tid_ = NULL;
        latch_->countDown();
        latch_ = NULL;
        try {
            thread_func_();
        }
        catch (...) {
            throw;
        }
    }

};

void *startFunc(void* obj) {
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
}

void Thread::setDefaultName() {
    int num = num_created_.addAndGet();
    char buf[32];
    snprintf(buf, sizeof(buf), "Thread%d", num);
    name_ = buf;
}

Thread::Thread(ThreadFunc _func, std::string _name = "")
    : started_(true),
      joined_(false),
      tid_(0),
      pthread_id_(0),
      name_(_name),
      latch_(1) { 
          setDefaultName();
       }

Thread::~Thread() {
    if (started_ && !joined_) {
        pthread_detach(pthread_id_);
    }
}

void Thread::start() {
    ThreadData* obj = new ThreadData(thread_func_, name_,
        &tid_, &latch_);
    pthread_create(&pthread_id_, NULL, &startFunc, obj);
}

int Thread::join() {
    if (started_) {
        joined_ = true;
        started_ = false;
        return pthread_join(pthread_id_, NULL);
    }
    return 0;
}

}   // namespace NetLib
