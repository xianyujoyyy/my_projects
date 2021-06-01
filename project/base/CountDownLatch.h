
#ifndef PROJECT_BASE_COUNTDOWNLATCH_H
#define PROJECT_BASE_COUNTDOWNLATCH_H

#include "project/base/Condition.h"
#include "project/base/Mutex.h"

namespace NetLib {

class CountDownLatch : noncopyable {
public:
    explicit CountDownLatch(int _count)
        : mutex_(),
          condition_(mutex_),
          count_(_count) {}
    ~CountDownLatch() {}

    void wait();

    void countDown();

    int getCount() const;

private:
    mutable Mutex mutex_;
    Condition condition_;
    int count_;

};

}   // namespace NetLib

#endif  // PROJECT_BASE_COUNTDOWNLATCH_H
