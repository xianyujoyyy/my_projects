
#include <string.h>
#include <string>

#include "project/net/EventLoopThreadPool.h"

using namespace NetLib;
using namespace NetLib::net;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* _baseloop, std::string& _name)
    : baseloop_(_baseloop),
      thread_init_callback_(std::move(_cb)),
      name_(_name),
      started_(false),
      num_threads_(0),
      next_(0) {}

// smart pointer helps
EventLoopThreadPool::~EventLoopThreadPool() { }

void EventLoopThreadPool::start() {
    started_ = true;
    for (int i = 0; i < num_threads_; ++i) {
        char buf[32] = snprintf(buf, sizeof(buf), "%s%d", name_.c_str(), i);
        std::string name = buf;
        EventLoopThread* tmp = new EventLoopThread(thread_init_callback_,
            name);
        threads_.push_back(std::unique_prt<EventLoopThread>(tmp));
        loops_.push_back(tmp->startLoop());
    }
    if (num_threads_ == 0 && thread_init_callback_) thread_init_callback_();
}

EventLoop* EventLoopThreadPool::getNextLoop() {
    EventLoop* loop = baseloop_;
    if (started) {
        ++next_;
        if (next_ == num_threads_) next_ = 0;
        loop = loops_[next_];
    }
    return loop;
}
