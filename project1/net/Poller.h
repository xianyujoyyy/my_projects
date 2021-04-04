
#ifndef PROJECT1_NET_POLLER_H
#define PROJECT1_NET_POLLER_H

#include <vector>
#include <map>
#include <sys/epoll.h>

#include "project1/net/EventLoop.h"
#include "project1/base/Logging.h"

class Channel;

namespace NetLib {
namespace net {

// epoll
class Poller : noncopyable {
public:
    typedef std::vector<Channel*> ChannelList;
    
    Poller(EventLoop* loop) : ownerloop_(loop), init_event_(kInitEventNum){
        epollfd_ = ::epoll_create1(EPOLL_CLOEXEC);
        if (epollfd_ < 0) {
            LOG_SYSFATAL << "Poller::Poller";
        }
    }
    ~Poller() {
        ::close(epollfd_);
    }

    Timestamp poll(int timeoutMs, ChannelList* activeChannels);
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    bool hasChannel(Channel* channel) const;

    void assertInLoopThread() const {
        ownerloop_ -> assertInLoopThread();
    }

private:
    static const char* opToString(int op);
    void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;
    void update(int operation, Channel* channel);

    EventLoop* ownerloop_;
    int epollfd_;

    typedef std::map<int, Channel*> ChannelMap;
    ChannelMap channel_map_;

    typedef std::vector<struct epoll_event> EventList;
    EventList events_;
};

}
}   // namespace NetLib

#endif  // PROJECT1_NET_POLLER_H
