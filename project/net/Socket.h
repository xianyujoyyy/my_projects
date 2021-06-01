
#ifndef PROJECT_NET_SOCKET_H
#define PROJECT_NET_SOCKET_H

#include "project/net/InetAddress.h"
#include "project/base/noncopyable.h"

namespace NetLib {

namespace net {

namespace sockets {

const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);
struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr);

void setNonblockAndCloseOnExec(int connfd);
int createNonblockingOrDie(sa_family_t family);

int connect(int sockfd, const struct InetAddress* servaddr);

ssize_t read(int sockfd, void* buf, size_t count);
ssize_t readv(int sockfd, const struct iovec* iov, int iovcnt);
ssize_t write(int sockfd, const void* buf, size_t count);

}   // namespace socket

class Socket : NetLib::noncopyable {
public:
    explicit Socket(int _sockfd)
        : sockfd_(_sockfd) {}
    ~Socket();

    int fd() const { return sockfd_; }

    void bind(const struct InetAddress* myaddr);
    
    void listen();

    int accept(struct InetAddress* cliaddr);

    void shutdownWrite();

    void setTCPNoDelay(bool on);

    void setReuseAddr(bool on);

    void setReusePort(bool on);

    void setKeepAlive(bool on);

private:
    const int sockfd_;

};

}   // namespace net

}   // namespace NetLib

#endif  // PROJECT_NET_SOCKET_H
