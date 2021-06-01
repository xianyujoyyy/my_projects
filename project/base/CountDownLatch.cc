
#include "project/base/CountDownLatch.h"

using namespace NetLib;

void CountDownLatch::wait() {
    MutexLockGuard lock(mutex_);
    // 虚假唤醒
    while (count_ > 0) {
        condition_.wait();
    }
}

void CountDownLatch::countDown() {
    MutexLockGuard lock(mutex_);
    --count_;
    if (count_ == 0) {
        condition_.notifyAll();
    }
}

int CountDownLatch::getCount() const {
    MutexLockGuard lock(mutex_);
    return count_;
}
