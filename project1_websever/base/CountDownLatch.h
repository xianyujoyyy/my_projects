#ifndef BASE_COUNTDOWNLATCH_H
#define BASE_COUNTDOWNLATCH_H

#include "project1_websever/base/Condition.h"
#include "project1_websever/base/Mutex.h"

namespace NetLib {

class CountDownLatch : noncopyable {
public:
    explicit CountDownLatch(int count);

    void wait();

    void countDown();

    int getCount() const;

private:
    mutable MutexLock mutex_;
    Condition condition_;
    int count_;
};

}   // namespace NetLib

#endif  //  BASE_COUNTDOWNLATCH_H
