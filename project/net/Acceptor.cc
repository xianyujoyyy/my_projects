
#include <errno.h>
#include <unistd.h>

#include "project/net/Acceptor.h"

using namespace NetLib;
using namespace NetLib::net;

Acceptor::Acceptor(EventLoop* loop, const InetAddress& sockaddr, bool reuseport)
    : loop_(loop),
      accept_socket_(socket::createNoneBlockingOrDie(sockaddr.family())),
      accept_channel_(Channel(loop, accept_socket_.fd())),
      reuseport_(reuseport),
      listening_(false),
      idlefd_(::open("dev/null", O_RDONLY | O_CLOEXEC);) {
        accept_socket_.setReuseAddress(true);
        accept_socket_.setReusePort(reuseport_);
        accept_socket_.bind(sockaddr);
        accept_channel_.setReadEventCallback(std::bind(Acceptor::handleRead(), this));
      }

Acceptor::~Acceptor() {
    listening_ = false;
    accept_channel_.disableAll();
    accept_channel_.remove();
    ::close(idlefd_);
}

void Acceptor::listen() {
    accept_channel_.enableRead();
    accept_channel_.listen();
    listening_ = true;
}

void Acceptor::handleRead() {
    InetAddress peer_address;
    int connfd = accept_socket_.accept(&peer_address);
    if (connfd >= 0) {
        if (new_connection_cb_) new_connection_cb_(connfd, peer_address);
        else {
            socket::close(connfd);
        }
    }
    else {
        // LOG_SYS
        if (errno == EMFILE) {
            ::close(idlefd_);
            idlefd_ = accept_socket_.accept(&peer_address);
            ::close(idlefd_);
            idlefd_ = ::open("dev/null", O_RDONLY | O_CLOEXEC);
        }
    }
}
