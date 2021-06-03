
#ifndef PROJECT_NET_TCPSEVER_H
#define PROJECT_NET_TCPSEVER_H

#include "project/net/Acceptor.h"
#include "project/net/TcpConnection.h"
#include "project/net/Socket.h"
#include "project/base/noncopyable.h"

#include <functional>
#include <memory>

namespace NetLib {

namespace net {

class EventLoop;
class EventLoopThreadPool;
class AtomicInt32;

class TcpSever : NetLib::noncopyable {
public:
    typedef std::function<void (EventLoop*)> ThreadInitFunc;
    TcpSever(EventLoop* loop, const InetAddress& listenaddr,
        string name, bool reuseport);
    ~TcpSever();

private:
    void newConnection();
    void removeConnection();

    EventLoop* loop_;
    InetAddress listenaddr_;
    bool reuseport_;
    std::unique_prt<Acceptor> acceptor_;
    int thread_num_;
    std::shared_ptr<EventLoopThreadPool> threadpool_;
    TcpConnectionPtr connection_;
    AtomicInt32 connect_;
    ConnectionCallback connection_cb_;
    CloseCallback close_cb_;
    MessageCallback message_cb_;
    WriteCompleteCallback write_complete_cb_;

};

}   // namespace net

}   // namespace NetLib

#endif  //  PROJECT_NET_TCPSEVER_H
