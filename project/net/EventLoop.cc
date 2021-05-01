
#include <sys/eventfd.h>
#include <unistd.h>

#include "project/net/EventLoop.h"

namespace NetLib {

namespace net {

namespace detail {

__thread EventLoop* t_LoopInThisThread = 0;

const int kPollTime = 10000;

int createEventfd() {
    int eventfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    return eventfd;
}

}

EventLoop::EventLoop()
    : looping_(false),
      quit_(true),
      event_handling_(false),
      do_pending_(false),
      iteration_(0),
      thread_id_(NetLib::CurrentThread::tid()),
      poll_return_time_(0),
      poller_(new Poller(this)),
      timerqueue_(new TimerQueue(this)),
      wakeupfd_(detail::createEventfd()),
      wakeup_channel_(new Channel(this, wakeupfd_)),
      current_channel_(NULL) {
          // TODO:abort()
          if (detail::t_LoopInThisThread != NULL) {}
          else {
              detail::t_LoopInThisThread = this;
              wakeup_channel->setReadEventCallback(
                  std::bind(&EventLoop::handleRead, this));
              wakeup_channel->enableRead();
          }
      }

EventLoop::~EventLoop() {
    wakeup_channel->disableAll();
    wakeup_channel->remove();
}

void EventLoop::loop() {
    looping_ = true;
    quit_ = false;
    while (!quit_) {
        event_handling_ = true;
        ++iteration_;
        active_channel_.clear();
        poll_return_time_ = poller_->poll(detail::kPollTime,
            active_channel_);
        for (int i = 1; i < active_channel_.size(); ++i) {
            current_channel_ = active_channel_[i];
            current_channel_->handleEvent(poll_return_time_);
        }
        current_channel_ = NULL;
        event_handling_ = false;
        doPendingFunctors();
    }
    looping_ = false;
}

void EventLoop::quit() {
    quit_ = false;
    if (!isInLoopThread()) wakeup();
}

void EventLoop::runInLoop(Functor cb) {
    if (isInLoopThread()) cb();
    else queueInLoop(std::move(cb));
}

void EventLoop::queueInLoop(Functor cb) {
    MutexLockGuard MutexLock(mutex_);
    pending_functors_.push_back(std::move(cb));
    if (!isInLoopThread()) wakeup();
}

size_t EventLoop::queueSize() const {
    MutexLockGuard MutexLock(mutex_);
    return pending_functors_.size();
}

void EventLoop::updateChannel(Channel* channel) {
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel) {
    poller_->removeChannel(channel);
}

void EventLoop::hasChannel(Channel* channel) {
    poller_->hasChannel(channel);
}

TimerId EventLoop::runAt(Timestamp time, TimerCallback time_cb) {
    return timerqueue_->addTimer(std::move(time_cb), time, 0);
}

TimerId EventLoop::runAfter(int64_t delay, TimerCallback time_cb) {
    Timestamp time = Timestamp::now();
    time.addTime(0, delay);
    return timerqueue_->addTimer(std::move(time_cb), time, 0);
}

TimerId EventLoop::runEvery(int64_t interval, TimerCallback time_cb) {
    return timerqueue_->addTimer(std::move(time_cb), Timestamp::now(), interval);
}

void cancel(TimerId timerid) {
    timerqueue_->removeTimer(timerid); 
}

void EventLoop::doPendingFunctors() {
    MutexLockGuard lock(mutex_);
    do_pending_ = true;
    for (const Functor& functor : pending_functors_) {
        functor();
    }
    do_pending_ = false;
}

void EventLoop::wakeup() {
    uint64_t one = 1;
    ssize_t n = sockets::write(wakeupfd_, &one, sizeof(one));
}

void EventLoop::handleRead() {
    uint64_t one = 1;
    ssize_t n = sockets::read(wakeupfd_, &one, sizeof(one));
}

}   // namespace net

}   // namespace NetLib
