
#ifndef PROJECT_NET_EVENTLOOPTHREADPOOL_H
#define PROJECT_NET_EVENTLOOPTHREADPOOL_H

#include <vector>
#include <memory>
#include <functional>

#include "project/net/EventLoopThread.h"
#include "project/net/EventLoop.h"

#include "project/base/noncopyable.h"

namespace NetLib {

namespace net {
class EventLoopThreadPool : NetLib::noncopyable {
public:
    typedef std::function<void (EventLoop*)> ThreadInitFunc;

    EventLoopThreadPool(EventLoop* _baseloop, ThreadInitFunc _cb, std::string& _name);
    ~EventLoopThreadPool();

    void start();

    EventLoop* getNextLoop();

    void setThreadNum(int n) { num_threads_ = n; }

    bool started() const { return started_; }
    std::string name() const { return name_; }

private:
    EventLoop* baseloop_;
    ThreadInitFunc thread_init_callback_;
    std::string name_;

    bool started_;
    int num_threads_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;

    int next_;

};

}   // namespace net

}   // namespace NetLib

#endif  // PROJECT_NET_EVENTLOOPTHREADPOOL_H
