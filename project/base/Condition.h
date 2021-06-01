
#ifndef PROJECT_BASE_CONDITION_H
#define PROJECT_BASE_CONDITION_H

#include "project/base/Mutex.h"
#include "project/base/noncopyable.h"

#include <pthread.h>

namespace NetLib {

class Condition : noncopyable {
public:
    Condition(Mutex& _mutex)
        : mutex_(_mutex) {
        pthread_cond_init(&pcond_, NULL);
    }
    ~Condition() {
        pthread_cond_destroy(&pcond_);
    }

    void wait() {
        mutex_.unassignHolder();
        pthread_cond_wait(&pcond_, mutex_.getMutexPointer());
        mutex_.assignHolder();
    }
    bool waitForSeconds(int64_t time_ms);

    void notify() {
        pthread_cond_signal(&pcond_);
    }
    void notifyAll() {
        pthread_cond_broadcast(&pcond_);
    }

private:
    Mutex& mutex_;
    pthread_cond_t pcond_;

};

}   // namespace NetLib

#endif  // PROJECT_BASE_CONDITION_H
