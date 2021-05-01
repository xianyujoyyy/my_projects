
#ifndef PROJECT_NET_TIMERQUEUE_H
#define PROJECT_NET_TIMERQUEUE_H

#include "project/base/noncopyable.h"

#include "project/net/EventLoop.h"
#include "project/net/Channel.h"
#include "project/net/Timer.h"

#include <vector>
#include <set>

namespace NetLib {

namespace net {

class EventLoop;
class Channel;

class TimerQueue : NetLib::noncopyable {
public:
    TimerQueue(EventLoop* _loop);
    ~TimerQueue();

    void handleRead();

    TimerId addTimer(TimerCallback _cb, Timestamp _expire,
        int64_t _interval);
    void removeTimer(TimerId timerid);

    int fd() const { return fd_; }

private:
    typedef std::pair<Timestamp, Timer*> Entry;
    typedef std::set<TimerId> TimerSet;
    typedef std::vector<TimerId> TimerList;

    TimerId addTimerInLoop(Timer* timer);
    void removeTimerInLoop(TimerId timerid);

    void getExpired(std::vector<Entry>& expired, Timestamp time);
    bool insert(Timer* timer);
    void reset(TimerList& expired, Timestamp now);

    EventLoop* loop_;
    int fd_;
    Channel* timerqueue_channel_;

    TimerSet timer_set_;
    TimerList active_timer_;
    TimerList cancel_timer_;

    bool is_handling_;

};

}   // namespace net

}   // namespace NetLib

#endif  // PROJECT_NET_TIMERQUEUE_H
