
#ifndef PROJECT_NET_CHANNEL_H
#define PROJECT_NET_CHANNEL_H

#include "project/base/noncopyable.h"
#include "project/base/Timestamp.h"

#include <functional>
#include <memory>
#include <string>

namespace NetLib {

namespace net {

class EventLoop;

class Channel : noncopyable {
public:
    typedef std::function<void()> EventCallback;
    typedef std::function<void(Timestamp)> ReadEventCallback;

    Channel(EventLoop* _loop, int _fd);
    ~Channel() {}

    void handleEvent(Timestamp revent_time);
    void tie(const std::shared_ptr<void>&);

    void setReadEventCallback(ReadEventCallback cb) { read_callback_ = std::move(cb); }
    void setWriteEventCallback(EventCallback cb) { write_callback_ = std::move(cb); }
    void setCloseEventCallback(EventCallback cb) { close_callback_ = std::move(cb); }
    void setErrorEventCallback(EventCallback cb) { error_callback_ = std::move(cb); }

    int fd() const { return fd_; }

    int status() const { return status_; }
    void set_status(int status) { status_ = status; }
    int event() const { return event_; }
    void set_revent(int revent) { revent_ = revent; }

    void enableRead() { event_ |= kReadEvent; update(); }
    void enableWrite() { event_ |= kWriteEvent; update(); }
    void disableRead() { event_ &= ~kReadEvent; update(); }
    void disableWrite() { event_ &= ~kWriteEvent; update(); }
    void disableAll() { event_ = kNoneEvent; update(); }
    bool isReading() const { return event_ & kReadEvent; }
    bool isWriting() const { return event_ & kWriteEvent; }
    bool isNoneEvent() const { return event_ == kNoneEvent; }

    EventLoop* ownerLoop() const { return loop_; }

    void remove();

private:
    static const int kReadEvent;
    static const int kWriteEvent;
    static const int kNoneEvent;

    void update();
    void _handleEvent(Timestamp revent_time);

    EventLoop* loop_;
    int fd_;

    ReadEventCallback read_callback_;
    EventCallback write_callback_;
    EventCallback close_callback_;
    EventCallback error_callback_;
    
    int status_;
    int event_;
    int revent_;

    bool tied_;
    std::weak_ptr<void> tied_obj_;

    bool addedToLoop_;
    bool eventHandling_;
};

}   // namespace net

}   // namespace NetLib

#endif  // PROJECT_NET_CHANNEL_H
