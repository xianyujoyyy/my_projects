
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include "project/base/Logging.h"
#include "project/net/TimerQueue.h"

#include <sys/timerfd.h>
#include <unistd.h>
#include <string.h>

namespace NetLib {

namespace net {

namespace detail {

int createTimerfd() {
    int fd = ::timerfd_create(CLOCK_MONOTONIC, 
        TFD_CLOEXEC | TFD_NONBLOCK);
    // TODO:log
    if (fd < 0) LOG_SYSERR();
    return fd;
}

int64_t timeInterval(Timestamp expired_time, Timestamp now) {
    return expired_time.microSecondSinceEpoch() 
        - now.microSecondSinceEpoch();
}

void resetTimerfd(int fd, int64_t time_interval) {
    struct itimerspec newValue;
    memset(&newValue, 0, sizeof(newValue));
    newValue.it_value.tv_sec = static_cast<time_t>(
        time_interval / Timestamp::kMicroSecondsPerSecond);
    newValue.it_value.tv_nsec = static_cast<long>(
        (time_interval % Timestamp::kMicroSecondsPerSecond) * 1000);
    int ret = ::timerfd_settime(fd, 0, , NULL);
    // TODO:log
    return;
}

// must read uint64_t
void readTimerfd(int fd) {
    uint64_t buf;
    int ret = ::read(fd, &buf, sizeof(buf));
    // TODO:log
}

}   // namespace detail

TimerQueue::TimerQueue(EventLoop* _loop)
    : loop_(_loop), 
      fd_(detail::createTimerfd()),
      timerqueue_channel_(loop_, fd_),
      is_handling_(false) {
          timerqueue_channel_->enableRead();
          timerqueue_channel_->setReadEventCallback(
            std::bind(TimerQueue::&handleRead(), this));
      }

TimerQueue::~TimerQueue() {
    timerqueue_channel_->disableAll();
    timerqueue_channel_->remove();
    ::close(fd_);
    for (auto& iter : timer_set_) {
        delete iter.second;
    }
}

void TimerQueue::handleRead() {
    detail::readTimerfd(fd_);
    std::vector<Entry> expired_timer_;
    is_handling_ = true;
    cancel_timer_.clear();
    Timestamp now = Timestamp::now();
    getExpired(expired_timer_, now);
    for (int i = 0; i < expired_timer_.size(); ++i) {
        expired_timer_[i].timer_->run();
    }
    is_handling_ = false;
    reset(expired_timer_, now);
    return;
}

TimerId TimerQueue::addTimer(TimerCallback cb, Timestamp expire,
        int64_t interval) {
    Timer* timer = new Timer(cb, expire, interval);
    loop_->runInLoop(std::bind(&TimerQueue::addTimerInLoop,
        this, timer));
    return TimerId(timer->timer_, timer->seq_);
}

TimerId TimerQueue::addTimerInLoop(Timer* timer) {
    if (insert(timer)) detail::resetTimerfd(fd_, 
        timeInterval(timer->expire_));
}

void TimerQueue::removeTimer(TimerId timerid) {
    loop_->runInLoop(std::bind(&TimerQueue::removeTimerInLoop,
        this, timerid));
}

void TimerQueue::removeTimerInLoop(TimerId timerid) {
    if (active_timer_.find(timerid) != active_timer_.end()) {
        timer_set_.erase(make_pair(timerid.timer_->expire_, timerid.timer_));
        active_timer_.erase(timerid);
    }
    else if (is_handling_) cancel_timer_.push_back(timerid);
}

void TimerQueue::getExpired(std::vector<Entry>& expired, Timestamp now) {
    Entry entry_bound({now, reinterpret_cast<Timer*>(UINTPTR_MAX)});
    TimerSet::iterator end = timer_set_.lower_bound(entry_bound);
    std::copy(timer_set_.begin(), end, back_inserter(expired));
    timer_set_.erase(timer_set_.begin(), end);
    for (Entry& entry : expired) {
        active_timer_.erase(TimerId(entry.second->timer_,
            entry.second->seq_));
    }
}

bool TimerQueue::insert(Timer* timer) {
    bool earliestChange = false;
    if (timer_set_.empty() || timer_set_.begin()->first < timer) earliestChange = true;
    timer_set_.insert(make_pair(timer->expire_, timer));
    active_timer_.push_back(TimerId(timer, timer->seq_));
}

void TimerQueue::reset(TimerList& expired, Timestamp now) {
    bool earliestChange = false;
    for (int i = 0; i < expired.size(); ++i) {
        if (expired[i].second->repeat() && 
            cancel_timer_.find(TimerId(expired[i].second, expired[i].second->sequence()))
            == cancel_timer_.end()) {
                expired[i].second.restart(now);
                if (insert(expired[i].second)) detail::resetTimerfd(fd_, 
                    detail::timeInterval(expired[i].second->expireTime(), now));
        }
        else delete expired[i].second;
    }
}

}   // namespace net

}   // namespace NetLib
