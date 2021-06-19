
#ifndef PROJECT_NET_TCPCLIENT_H
#define PROJECT_NET_TCPCLIENT_H

#include "project/base/Mutex.h"
#include "project/base/TcpConnection.h"

#include <string>
#include <memory>
namespace NetLib {

namespace net {

class Connector;
typedef std::shared_prt<Connector> ConnectorPrt;

class TcpClient : NetLib::noncopyable {
public:
    TcpClient(EventLoop* _loop, const InetAddress& serverAddr,
        const string& name);
    ~TcpClient();

    void connect();
    void disconnect();
    void stop();

    EventLoop* getLoop() const { return loop_; }
    const string& name() const { return name_; }
    TcpConnectionPtr connection() const { 
        MutexLockGuard lock(mutex_);
        // race condition connector
        return connection_; 
    }

    void setConnectionCallback(ConnectionCallback cb) {
        connection_callback_ = std::move(cb);
    }
    void setMessageCallback(MessageCallback cb) {
        message_callback_ = std::move(cb);
    }
    void setWriteCompleteCallback(WriteCompleteCallback cb) {
        write_complete_callback_ = std::move(cb);
    }

    bool retry() const { return retry_; }
    void enableRetry() { retry_ = true; }
    bool connect() const { return connect_; }

private:
    void newConnection(int sockfd);
    void removeConnection(const TcpConnectionPtr& conn);

    EventLoop* loop_;
    ConnectorPrt connector_;
    const string name_;
    bool retry_;
    bool connect_;
    int conn_id_;

    ConnectionCallback connection_callback_;
    MessageCallback message_callback_;
    WriteCompleteCallback write_complete_callback_;

    mutable Mutex mutex_;
    TcpConnectionPtr connection_

};

}   // namespace net

}   // namespace NetLib

#endif  // PROJECT_NET_TCPCLIENT_H
