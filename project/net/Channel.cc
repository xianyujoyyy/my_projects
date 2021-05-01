
#include "project/net/Channel.h"
#include "project/base/Logging.h"

#include <epoll.h>

using namespace NetLib;
using namespace NetLib::net;

const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;
const int Channel::kNoneEvent = 0;

Channel::Channel(EventLoop* _loop, int _fd)
    : loop_(_loop),
      fd_(_fd),
      status_(-1),
      event_(0),
      revent_(0),
      tied_(false),
      addedToLoop_(false),
      eventHandling_(false) {}

void Channel::tie(const std::shared_ptr<void>& obj) {
    tied_ = true;
    tied_obj_ = obj;
}

void Channel::handleEvent(Timestamp revent_time) {
    if (tied_) {
        std::shared_ptr<void> guard;
        guard = tied_obj_.lock();
        if (guard) {
            _handleEvent(Timestamp revent_time);
        }
    }
    else _handleEvent(Timestamp revent_time);
}

void Channel::_handleEvent(Timestamp revent_time) {
    eventHandling_ = true;
    // TODO:log
    if (revent_ & EPOLLNVAL) {
        LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLNVAL";
    }
    if (revent_ & (EPOLLERR | EPOLLNVAL)) {
        if (close_callback_) close_callback_();
    }
    if (revent_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUB)) {
        if (read_callback_) read_callback_();
    }
    if (revent_ & EPOLLOUT) {
        if (write_callback_) write_callback_();
    }
    eventHandling_ = false;
}

void Channel::update() {
    addedToLoop_ = true;
    loop_->updateChannel();
}

void Channel::remove() {
    addedToLoop_ = false;
    loop_->removeChannel();
}
