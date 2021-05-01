
#ifndef PROJECT_NET_TIMER_H
#define PROJECT_NET_TIMER_H

#include "project/base/Timestamp.h"
#include "project/base/Atomic.h"
#include "project/base/noncopyable.h"
#include "project/base/copyable.h"
#include "project/net/Callback.h"

namespace NetLib {

namespace net {

class Timer : NetLib::noncopyable {
public:
    Timer(TimerCallback _cb, Timestamp _expire, int64_t _interval) 
        : seq_(atomic_num_.incrementAndGet()),
          timer_callback_(std::move(_cb)),
          expire_(_expire),
          interval_(_interval),
          repeat_(interval_ > 0),
          seq_() {}
    ~Timer() {}

    void run() const { timer_callback_(); }

    int64_t sequence() const { return seq_; }
    Timestamp expireTime() const { return expire_; }
    bool repeat() const { return repeat_; }

    void restart(Timestamp now);

private:
    static AtomicInt64 atomic_num_;
    const int64_t seq_;
    TimerCallback timer_callback_;
    Timestamp expire_;
    const int64_t interval_;
    const bool repeat_;
};

class TimerId : NetLib::copyable {
public:
    TimerId(Timer* _timer, int64_t _seq)
        : timer_(_timer),
          seq_(_seq) {}

private:
    friend class TimerQueue;
    Timer* timer_;
    int64_t seq_;
};

}   // namespace net

}   // namespace NetLib
