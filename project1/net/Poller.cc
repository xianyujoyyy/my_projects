
#include <errno.h>

#include "project1/net/Poller.h"
#include "project1/net/Channel.h"

using namespace NetLib;
using namespace NetLib::net;

class Timestamp;

#define kNew    -1
#define kAdded   1
#define kDeleted 2

Timestamp Poller::poll(int timeoutMs, ChannelList* activeChannels) {
    LOG_TRACE << "fd total count " << channel_map_.size();
    int n_events = ::epoll_wait(int epollfd_, &*events_.begin(), 
                    static_cast<int>events_.size(), timeoutMs);
    int savedErrno = errno;
    Timestamp now(Timestamp::now());

    if (n_events > 0) {
        LOG_TRACE << n_events << "events happened.";
        fillActiveChannels(n_events, activeChannels);
        if (static_cast<size_t>(numEvents) == events_.size()) {
            events_.resize(events_.size()*2);
        }
    }
    else if (n_events == 0) {
        LOG_TRACE << "nothing happened.";
    }
    else {
        if (savedErrno != EINTER) {
            errno = savedErrno;
            LOGSYSERR << "POLLER::poll()";
        }
    }
    return now;
}

void Poller::updateChannel(Channel* channel) {
    Poller::assertInLoopThread();
    const int index = channel -> index();
    LOG_TRACE << "fd = " << channel->fd()
        << " events = " << channel->events() << " index = " << index;
    if (index == kNew || index == kDeleted) {
        int fd = channel -> fd();
        channel_map_[fd] = channel;
        channel -> set_index(kAdded);
        update(EPOLL_CTL_ADD, channel);
    }
    else {
        if (channel->isNoneEvent()) {
            channel->set_index(kDeleted);
            update(EPOLL_CTL_DEL, channel);
        }
        else {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

void Poller::removeChannel(Channel* channel) {
    Poller::assertInLoopThread();
    LOG_TRACE << "fd = " << channel->fd()
        << " events = " << channel->events() << " index = " << index;
    const int fd = channel->fd();
    if (channel_map_[fd] == channel) {
        channel_map_[fd].erase(fd);
    }
    if (channel->index() == kAdded) {
        update(EPOLL_CTL_DEL, channel);
    }
    channel->set_index(kNew);
}

bool Poller::hasChannel(Channel* channel) const {
    Poller::assertInLoopThread();
    auto iter = channel_map_.find(channel->fd());
    return iter != channel_map_.end() && (iter->second == channel);
}

const char* Poller::opToString(int op) {
    switch(op) {
        case EPOLL_CTL_ADD:
            return "ADD";
        case EPOLL_CTL_DEL:
            return "DEL";
        case EPOLL_CTL_MOD:
            return "MOD";
        default:
        return "Unknown Operation";
    }
}

void Poller::fillActiveChannels(int numEvents, 
                ChannelList* activeChannels) const {
    for (int i = 0; i < numEvents; ++i) {
        Channel* channel = static_cast<Channel*>(events_[i].data.prt);
        channel->set_revents(events_[i].events);
        activeChannels.push_back(channel);
    }
}

void Epoll::update(int operation, Channel* channel) {
    const int fd = channel->fd();
    struct epoll_event ev;
    ev.event = channel->events();
    ev.data.prt = channel;
    int ret = ::epoll_ctl(epollfd_, operation, fd, ev);
    if (ret < 0) {
        if (operation == EPOLL_CTL_DEL) {
            LOG_SYSERR << "epoll_ctl op =" << operationToString(operation) << " fd =" << fd;
        }
        else {
            LOG_SYSFATAL << "epoll_ctl op =" << operationToString(operation) << " fd =" << fd;
        }
    }
    else {
        LOGTRACE << "epoll_ctl op =" << operationToString(operation) << " fd =" << fd;
    }
}
