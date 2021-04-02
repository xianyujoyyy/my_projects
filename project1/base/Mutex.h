
#ifndef PROJECT1_BASE_MUTEX_H
#define PROJECT1_BASE_MUTEX_H

#include "project1/base/noncopyable.h"

#include <assert.h>
#include <pthread.h>

namespace NetLib {

class MutexLock : noncopyable {
public:
    MutexLock() : 
            mutex_(), 
            holder_(0) {
        pthread_mutex_init(&mutex_, NULL);
    }
    ~MutexLock() {
        assert(holder_ == 0);
        pthread_mutex_destroy(&mutex_);
    }

    bool isLockedByThisThread() {
        return holder_ == CurrentThread::t_cachedtid;
    }
    void assertLockedByThisThread() {
        assert(isLockedByThisThread());
    }

    void assignHolder() {
        if (CurrentThread::t_cachedtid == 0) {
            CurrentThread::t_cachedtid = tid();
        }
        holder_ = CurrentThread::t_cachedtid;
    }
    void unassignHolder() {
        holder_ = 0;
    }

    void lock() {
        if (holder_ == CurrentThread::t_cachedtid)
            pthread_mutex_lock(&mutex_);
    }
    void unlock() {
        if (holder_ == CurrentThread::t_cachedtid)
            pthread_mutex_unlock(&mutex_);
    }

    pthread_mutex_t* getPthreadMutex() {
        return &mutex_;
    }
private:
    friend Condition;

    pthread_mutex_t mutex_; 
    pid_t holder_;
};

class MutexLockGuard : noncopyable {
public:
    explicit MutexLockGuard(MutexLock& _mutexlock) : 
        mutexlock_(_mutexlock) {
        mutexlock_.lock();
        mutexlock_.assignHolder();
        }
    ~MutexLockGuard() {
        mutexlock_.unassignHolder();
        mutexlock_.unlock();
    }
private:
    MutexLock& mutexlock_;
};

}   // namespace NetLib

#endif  // PROJECT1_BASE_MUTEX_H
