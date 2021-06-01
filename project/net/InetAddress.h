
#ifndef PROJECT_NET_INETADDRESS_H
#define PROJECT_NET_INETADDRESS_H

#include "project/base/copyable.h"

// sockaddr_xx
#include <netinet/in.h>

namespace NetLib {

namespace net {

class InetAddress : NetLib::copyable {
public:
    // TODO: inet_pton, hton
    explicit InetAddress(sockaddr_in& _sockaddr)
        : sockaddr_(_sockaddr) {}
    explicit InetAddress(sockaddr_in6& _sockaddr)
        : sockaddr6_(_sockaddr) {}

    sa_family_t family() const { return sockaddr_.sin_family; }
    const struct sockaddr* getSockAddr() const { 
        return sockets::sockaddr_cast(&sockaddr6_);
    }
    void setSockAddrInet6(const struct sockaddr_in6& addr6) { 
        sockaddr6_ = addr6;
    }

private:
    union {
        struct sockaddr_in  sockaddr_;
        struct sockaddr_in6 sockaddr6_;
    };
    
};

}   // namespace net

}   // namespace NetLib

#endif  // PROJECT_NET_INETADDRESS_H
