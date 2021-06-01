
#ifndef PROJECT_NET_EVENTLOOPTHREAD_H
#define PROJECT_NET_EVENTLOOPTHREAD_H

#include <functional>

#include "project/net/EventLoop.h"
#include "project/base/Thread.h"
#include "project/base/Condition.h"
#include "project/base/noncopyable.h"

namespace NetLib {

namespace net {

class EventLoopThread : NetLib::noncopyable {
public:
    typedef std::function<void (EventLoop*)> ThreadInitCallback;
    
    explicit EventLoopThread(ThreadInitCallback _cb, std::string _name = "");
    ~EventLoopThread();

    EventLoop* startLoop();

private:
    void threadFunc();

    ThreadInitCallback thread_init_cb_;
    std::string name_;
    NetLib::Thread thread_;
    EventLoop* loop_;
    bool exiting_;

    NetLib::Mutex mutex_;
    NetLib::Condition condtion_;
    
};

}   // namespace net

}   // namespace NetLib

#endif  // PROJECT_NET_EVENTLOOPTHREAD_H
