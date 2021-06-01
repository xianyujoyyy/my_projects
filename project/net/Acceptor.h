
#ifndef PROJECT_NET_ACCEPTOR_H
#define PROJECT_NET_ACCEPTOR_H

#include <functional>

#include "project/net/InetAddress.h"
#include "project/net/Socket.h"
#include "project/net/Channel.h"
#include "project/net/EventLoop.h"
#include "project/base/noncopyable.h"

namespace NetLib {

namespace net {

class Acceptor : NetLib::noncopyable {
public:
    typedef std::function<void (int sockfd, const InetAddress& sockaddr)> 
        NewConnectionCallback;
    
    Acceptor(EventLoop* loop, InetAddress& sockaddr, bool reuseport = false);
    ~Acceptor();

    void setNewConnectionCallback(NewConnectionCallback cb) {
        new_connection_cb_ = std::move(cb);
    }

    void listen();

    bool listening() const { return listening_; }

private:
    void handleRead();

    EventLoop* loop_;
    Socket accept_socket_;
    Channel accept_channel_;
    NewConnectionCallback new_connection_cb_;
    bool reuseport_;
    bool listening_;
    int idlefd_;

};

}   // namesapce net

}   // namespace NetLib

#endif  // PROJECT_NET_ACCEPTOR_H
