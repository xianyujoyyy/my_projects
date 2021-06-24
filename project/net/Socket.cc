
#include "project/net/Socket.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/uio.h>

namespace NetLib {

namespace net {

namespace sockets {

const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in6* addr) {
  return reinterpret_cast<const struct sockaddr*>(addr);
}

struct sockaddr* sockets::sockaddr_cast(struct sockaddr_in6* addr) {
  return reinterpret_cast<struct sockaddr*>(addr);
}

const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in* addr) {
  return reinterpret_cast<const struct sockaddr*>(addr);
}

void setNonblockAndCloseOnExec(int sockfd) {
    int flags = ::fcntl(sockfd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int ret = ::fcntl(sockfd, F_SETFL, flags);
    // TODO:log

    flags = ::fcntl(sockfd, F_GETFD, 0);
    flags |= FD_CLOEXEC;
    ret = ::fcntl(sockfd, F_SETFD, flags);
}

int createNonblockingOrDie(sa_family_t family) {
    int sockfd = ::socket(family, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
    // TODO:log
    }
    setNonblockAndCloseOnExec(sockfd);
    if (sockfd < 0) {
    // TODO:log
    }
    return sockfd;
}

int connect(int sockfd, const struct InetAddress* servaddr) {
    return ::connect(sockfd, servaddr->getSockAddr(), static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
}

ssize_t read(int sockfd, void* buf, size_t count) {
    return ::read(sockfd, buf, count);
}
ssize_t readv(int sockfd, const struct iovec* iov, int iovcnt) {
    return ::readv(sockfd, iov, iovcnt);
}
ssize_t write(int sockfd, const void* buf, size_t count) {
    return ::write(sockfd, buf, count);
}

struct sockaddr_in6 getPeerAddr(int sockfd) {
    struct sockaddr_in6 peeraddr;
    ::bzero(&peeraddr, sizeof(peeraddr));
    socklen_t addrlen = static_cast<socklen_t>(sizeof(peeraddr));
    if (::getpeername(sockfd, sockaddr_cast(&peeraddr), &addrlen) < 0) {
        LOG_SYSERR << "sockets::getPeerAddr";
    }
    return peeraddr;
}

struct sockaddr_in6 getLocalAddr(int sockfd) {
    struct sockaddr_in6 localaddr;
    ::bzero(&localaddr, sizeof(localaddr));
    socklen_t addrlen = static_cast<socklen_t>(sizeof(localaddr));
    if (::getsockname(sockfd, sockaddr_cast(&localaddr), &addrlen) < 0) {
        LOG_SYSERR << "sockets::getLocalAddr";
    }
    return localaddr;
}

}   // namespace sockets

Socket::~Socket() {
    if (::close(sockfd_) < 0) {
        // TODO:log
    }
}

void Socket::bind(const struct InetAddress* myaddr) {
    int ret = ::bind(sockfd_, myaddr->getSockAddr(), static_cast<socklen_t>
        (sizeof(struct sockaddr_in6)));
    if (ret < 0) {
        // TODO:log
    }
}

void Socket::listen() {
    int ret = ::listen(sockfd_, SOMAXCONN);
    if (ret < 0) {
        // TODO:log
    }
}

int Socket::accept(struct InetAddress* cliaddr) {
    struct sockaddr_in6 addr;
    ::bzero(&addr, sizeof(addr));
    int connfd = ::accept(sockfd_, sockets::sockaddr_cast(&addr),
        static_cast<socklen_t>(sizeof(addr)));
    sockets::setNonblockAndCloseOnExec(connfd);
    if (connfd < 0) {
        // TODO:log
    }
    else {
        cliaddr->setSockAddrInet6(addr);
    }
    return connfd;
}

void Socket::shutdownWrite() {
    if (::shutdown(sockfd_, SHUT_WR) < 0) {
        // TODO:log
    }
}

void Socket::setTCPNoDelay(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY,
        &optval, static_cast<socklen_t>(sizeof(optval)));
}

void Socket::setReuseAddr(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
        &optval, static_cast<socklen_t>(sizeof(optval)));
}

void Socket::setReusePort(bool on) {
#ifdef SO_REUSEPORT
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT,
        &optval, static_cast<socklen_t>(sizeof(optval)));
    if (ret < 0 && on) {
        // TODO:log
    }
#else
    if (on) {
        // TODO:log
    }
#endif   
}

void Socket::setKeepAlive(bool on) {
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
        &optval, static_cast<socklen_t>(sizeof(optval)));
}

}   // namespace net

}   // namespace NetLib
