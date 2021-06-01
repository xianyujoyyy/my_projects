
#include "project/net/EventLoopThread.h"

using namespace NetLib;
using namespace NetLib::net;

class EventLoop;

void EventLoopThread::threadFunc() {
    EventLoop loop;
    if (thread_init_cb_) {
        thread_init_cb_();
    }

    MutexLockGuard lock(mutex_);
    loop_ = &loop;
    condition_.notify();
    loop.loop();
    
    // loop exited.
    MutexLockGuard lock(mutex_);
    loop_ = NULL;

}

EventLoopThread::EventLoopThread(ThreadInitCallback _cb, std::string _name = "")
    : thread_init_cb_(std::move(_cb)),
      name_(_name),
      thread_(std::bind(&EventLoopThread::threadFunc, this), name_),
      loop_(NULL),
      exiting_(false),
      mutex_(),
      condition_(mutex_) {}

~EventLoopThread::EventLoopThread() {
    exiting_ = true;
    if (loop_ != NULL) {
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startLoop() {
    thread_.start();
    if (loop_ == NULL) {
        MutexLockGuard lock(mutex_);
        while (loop_ == NULL) {
            condition_.wait();
        }
    }
    return loop_;
}
