
#ifndef PROJECT_NET_POLLER_H
#define PROJECT_NET_POLLER_H

#include "project/base/Timestamp.h"
#include "project/base/noncopyable.h"
#include "project/net/EventLoop.h"

#include <sys/epoll.h>
#include <unistd.h>
#include <vector>
#include <map>

namespace NetLib {

namespace net {

class Channel;

class Poller : NetLib::noncopyable {
public:
    static const int kInitEvent = 16;

    Poller(EventLoop* _loop) :
        events_(kInitEvent),
        loop_(_loop),
        epfd_(::epoll_create1(EPOLL_CLOEXEC);)
        {}
    ~Poller() {
        ::close(fd_);
    }
    
    typedef std::vector<Channel*> ChannelList;
    Timestamp poll(int timeout_ms, ChannelList* active_channel);
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    bool hasChannel(Channel* channel);

private:
    typedef std::map<int, Channel*> ChannelMap;
    typedef std::vector<struct epoll_event> EventList;
    void update(Channel* channel, int op);
    void fillActiveChannel(ChannelList* active_channel, int events_num) const;
    ChannelMap channelmap_;
    EventList events_;
    EventLoop* loop_;
    int epfd_;

    static const int kAdded = 1;
    static const int kDeleted = -1;
    static const int kNew = 0;

};

}   // namespace net

}   // namespace NetLib

#endif  // PROJECT_NET_POLLER_H
