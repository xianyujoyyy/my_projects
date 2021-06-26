// 注意头文件自洽
#ifndef PROJECT_BASE_MUTEX_H
#define PROJECT_BASE_MUTEX_H

#include "project/base/CurrentThread.h"
#include "project/base/noncopyable.h"

#include <pthread.h>
#include <assert.h>

namespace NetLib {

class Mutex : noncopyable {
public:
    explicit Mutex(int _holder = 0)
        : mutex_(), 
          holder_(_holder) {
        pthread_mutex_init(&mutex_, NULL);
    }

    ~Mutex() {
        assert(holder_ == 0);
        pthread_mutex_destroy(&mutex_);
    }

    bool isLockedByThisThread() const {
        return holder_ == CurrentThread::tid();
    }

    void assignHolder() {
        holder_ = CurrentThread::tid();
    }

    void unassignHolder() {
        holder_ = 0;
    }

    void lock() {
        pthread_mutex_lock(&mutex_);
        assignHolder();
    }

    void unlock() {
        unassignHolder();
        pthread_mutex_unlock(&mutex_);
    }
    
    pthread_mutex_t* getMutexPointer() {
        return &mutex_;
    }

private:
    friend class Condition;

    pthread_mutex_t mutex_;
    pid_t           holder_;
};

class MutexLockGuard : noncopyable {
public:
    explicit MutexLockGuard(Mutex& _mutexlock)
        : mutexlock_(_mutexlock) {
            mutexlock_.lock();
        }
    ~MutexLockGuard() {
        mutexlock_.unlock();
    }
private:
    Mutex& mutexlock_;
};

}   // namespace NetLib

#endif  // PROJECT_BASE_MUTEX_H
