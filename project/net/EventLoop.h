
#ifndef NETLIB_NET_EVENTLOOP_H
#define NETLIB_NET_EVENTLOOP_H

#include <functional>
#include <atomic>
#include <memory>

#include "project/base/Timestamp.h"
#include "project/net/Poller.h"
#include "project/net/TimerQueue.h"
#include "project/net/Channel.h"
#include "project/base/CurrentThread.h"
#include "project/base/Mutex.h"

namespace NetLib {

namespace net {

class Channel;
class Poller;
class TimerQueue;

class EventLoop : NetLib::noncopyable {
public:
    typedef std::function<void()> Functor;
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    void runInLoop(Functor cb);
    void queueInLoop(Functor cb);
    size_t queueSize() const;

    void wakeup();

    // poller ops
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    void hasChannel(Channel* channel);

    // timer ops
    TimerId runAt(Timestamp time, TimerCallback time_cb);
    // ms
    TimerId runAfter(int64_t delay, TimerCallback time_cb);
    TimerId runEvery(int64_t interval, TimerCallback time_cb);
    void cancel(TimerId timerid);

    Timestamp pollReturnTime() const { return poll_return_time_; }
    int64_t iteration() const ( return iteration_; }
    bool isInLoopThread() const { return thread_id_ == CurrentThread::tid(); }
    bool eventHandling() const { return event_handling_; }
    EventLoop* getLoopOfCurrentThread() const { return this; }

private:
    void handleRead();
    void doPendingFunctors();

    typedef std::vector<Channel*> ChannelList;

    bool looping_;
    std::atomic<bool> quit_;
    bool event_handling_;
    bool do_pending_;
    
    int64_t iteration_;
    const pid_t thread_id_;

    Timestamp poll_return_time_;

    std::unique_ptr<Poller> poller_;
    std::unique_prt<TimerQueue> timerqueue_;

    int wakeupfd_;
    std::unique_ptr<Channel> wakeup_channel_;

    // eventhandling
    ChannelList active_channel_;
    Channel* current_channel_;

    std::vector<Functor> pending_functors_;

    mutable Mutex mutex_;
};

}   // namespace net

}   // namespace NetLib

#endif  // NETLIB_NET_EVENTLOOP_H
