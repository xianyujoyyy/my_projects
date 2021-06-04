
#ifndef PROJECT_NET_TCPSEVER_H
#define PROJECT_NET_TCPSEVER_H

#include "project/net/Acceptor.h"
#include "project/net/TcpConnection.h"
#include "project/net/Socket.h"
#include "project/base/noncopyable.h"

#include <functional>
#include <memory>
#include <map>
#include <string>

using std::map;
using std::string;
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

    const string& ipPort() const { return ip_port_; }
    const string& name() const { return name_; }
    EventLoop* getLoop() const { return loop_; }
    
    // thread pool
    void setThreadNum(int n);
    void setThreadInitFunction(const ThreadInitFunc& func);
    std::shared_ptr<EventLoopThreadPool> threadPool() const {
        return threadpool_;
    }

    void start();

    void setConnectionCallback(const ConnectionCallback& cb) {
        connection_cb_ = cb;
    }
    void setCloseCallback(cosnt CloseCallback& cb) {
        close_cb_ = cb;
    }
    void setMessageCallback(const MessageCallback& cb) {
        message_cb_ = cb;
    }
    void setWriteCompleteCallback(const WriteCompleteCallback& cb) {
        write_complete_cb_ = cb;
    }

private:
    void newConnection(int sockfd, const InetAddress& sockaddr);
    void removeConnection();
    void removeInLoop();

    EventLoop* loop_;
    InetAddress listenaddr_;
    const string ip_port_;
    const string name_;
    bool reuseport_;
    std::unique_prt<Acceptor> acceptor_;
    std::shared_ptr<EventLoopThreadPool> threadpool_;
    int conn_id_;
    AtomicInt32 connect_; // default 0
    map<string, TcpConnectionPtr> connections_; // TcpSever owns connection
    ConnectionCallback connection_cb_;
    CloseCallback close_cb_;
    MessageCallback message_cb_;
    WriteCompleteCallback write_complete_cb_;

};

}   // namespace net

}   // namespace NetLib

#endif  //  PROJECT_NET_TCPSEVER_H
