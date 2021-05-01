
#include "project/net/Poller.h"

#include <errno.h>

using namespace NetLib;
using namespace NetLib::net;

Timestamp Poller::poll(int timeout_ms, ChannelList* active_channel) {
    int num_events = ::epoll_wait(epfd_, &*events_.begin(), events_.size(), 
        timeout_ms);
    fillActiveChannel(active_channel, num_events);
    if (num_events == events_.size()) events_.resize(num_events * 2);
    Timestamp now(Timestamp::now());
    return now;
}

void Poller::updateChannel(Channel* channel) {
    if (channel->status() == kDeleted || channel->status() == kNew) {
        channel->set_status(kAdded);
        channelmap_[channel->fd()] = channel;
        update(channel, EPOLL_CTL_ADD);
    } 
    else {
        if (channel->isNoneEvent()) {
            channel->set_status(kDeleted);
            channelmap_.erase(channel->fd());
            update(channel, EPOLL_CTL_DEL);
        }
        else update(channel, EPOLL_CTL_MOD);
    }
}

void Poller::removeChannel(Channel* channel) {
    channelmap_.erase(channel->fd());
    if (channel->status() == kAdded) {
        channel->set_status(kNew);
        update(channel, EPOLL_CTL_DEL);
    }
}

bool Poller::hasChannel(Channel* channel) {
    auto iter = channelmap_.find(channel->fd());
    if (iter != channelmap_.end()) return true;
    return false;
}

void Poller::update(Channel* channel, int op) {
    struct epoll_event event_;
    event_.events = channel->events();
    event_.data.prt = channel;
    ::epoll_ctl(epfd_, op, channel->fd(), event_);
}

void Poller::fillActiveChannel(ChannelList* active_channel, 
    int events_num) const {
    for (int i = 0; i < events_.size(); ++i) {
        Channel* channel = static_cast<Channel*>(events_[i].data.prt);
        channel->set_revent(events_[i].events);
        active_channel->push_back(channel);
    }
}
