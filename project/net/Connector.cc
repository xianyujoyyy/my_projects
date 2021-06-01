
#include "project/net/Connector.h"

#include "project/net/Socket.h"
#include "project/net/Channel.h"

#include <sys/socket.h>
#include <errno>

using namespace NetLib;
using namespace NetLib::net;

const int Connector::kMaxRetryDelayMs;
const int Connector::kInitRetryDelayMs;

Connector::Connector(EventLoop* _loop, InetAddress& _severaddr)
    : loop_(_loop),
      severaddr_(_severaddr),
      connect_(false),
      retry_delay_ms_(kInitRetryDelayMs) {}

void Connector::start() {
    connect_ = true;
    loop_->runInLoop(std::bind(&Connector::startInLoop, this));
}

void Connector::startInLoop() {
    if (connect_) {
        connect();
    }
}

void Connector::stop() {
    connect_ = false;
    loop_->queueInLoop(std::bind(&Connector::stopInLoop, this));
}

void Connector::stopInLoop() {
    if (state_ == kConnecting) setState(kDisconnected);
    int sockfd = removeAndResetChannel();
    retry(sockfd);
}

void Connector::connect() {
    int sockfd = sockets::createNonblockingOrDie(severaddr_.family());
    int err = sockets::connect(sockfd, &severaddr_);
    // TODO:complex error handle
    if (err == 0) {
        connecting(sockfd);
    }
    //... retry
}

void Connector::connecting(int sockfd) {
    setState(kConnecting);
    channel_ = new Channel(loop_, sockfd);
    channel_->setWriteEventCallback(std::bind(&Connector::handleWrite, this));
    channel_->setErrorEventCallback(std::bind(&Connector::handleError, this));
    channel_->enableRead();
}

void Connector::restart() {
    setState(kDisconnected);
    int sockfd = removeAndResetChannel();
    if (connect_) retry(sockfd);
}

int Connector::removeAndResetChannel() {
    channel_->disableAll();
    channel_->remove();
    int sockfd = channel_->fd();
    // after poller
    loop_->queueInLoop(std::bind(&Connector::resetChannel, this));
    return sockfd;
}

void Connector::resetChannel() {
    channel_.reset();
}

void Connector::handleWrite() {
    // error type
    // handle
    // succeed
    int sockfd = removeAndResetChannel();
    new_connection_callback(sockfd);
}

void Connector::handleError() {
    // 
}

void Connector::retry(int sockfd) {
    setState(kDisconnected);
    ::close(sockfd);
    if (connect_) {
        retry_delay_ms_ = min(2 * retry_delay_ms_, kMaxRetryDelayMs);
        loop_->runAfter(retry_delay_ms_, std::bind(&Connector::connect, shared_from_this()));
    }
}
