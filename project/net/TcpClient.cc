
#include "project/net/TcpClient.h"

#include "project/base/Logging.h"
#include "project/base/Connector.h"
#include "project/base/TcpConnection.h"

using namespace NetLib;
using namespace NetLib::net;

TcpClient::TcpClient(EventLoop* loop,
                    const InetAddress& sever_addr,
                    const string& name)
    : loop_(loop),
      connector_(new Connector(loop, sever_addr)),
      name_(name),
      retry_(false),
      connect_(false),
      conn_id_(1) {
        connector_->setNewConnectionCallback(std::bind(
            &TcpClient::newConnection, this, _1));
}

TcpClient::~TcpClient() {
    TcpConnectionPtr conn;
    bool unique = false;
    {
        MutexLockGuard lock(mutex_);
        unique = conn.unique();
        conn = connection_;
    }
    if (conn) {
        CloseCallback cb = std::bind(&TcpClient::removeConnection,
            this, _1);
        loop_->runInLoop(std::bind(&TcpConnection::setCloseCallback,
            conn, cb));
        if (unique) {
            conn->forceClose();
        }
    }
    else {
        connector_->stop();
    }
}

void TcpClient::connect() {
    connect_ = true;
    connector_->start();
}

void TcpClient::disconnect() {
    connect_ = false;
    {
        MutexLockGuard lock(mutex_);
        if (connection_) {
            connection_->shutdown();
        }
    }
}

void TcpClient::stop() {
    connect_ = false;
    connector_->stop();
}

void TcpClient::newConnection(int sockfd) {
    InetAddress peer_addr_(sockets::getPeerAddress(sockfd));
    char buf[32];
    // TODO:log snprintf ip,port,connid
    ++conn_id_;
    string conn_name_ = name_ + buf;

    InerAddress local_addr_(sockets::getLocalAddr(sockfd));
    TcpConnectionPtr conn(new TcpConnection(loop_,
                                            conn_name_,
                                            sockfd,
                                            local_addr_,
                                            peer_addr_));
    
    conn->setConnectionCallback(connection_callback_);
    conn->setMessageCallback(message_callback_);
    conn->setWriteCompleteCallback(write_complete_callback_);
    conn->setCloseCallback(std::bind(&TcpClient::removeConnection, this, _1));
    {
        MutexLockGuard lock(mutex_);
        connection_ = conn;
    }
    conn->connectEstablished();
}

void TcpClient::removeConnection(const TcpConnectionPtr& conn) {
    MutexLockGuard lock(mutex_);
    connection_.reset();

    loop_->queueInLoop(std::bind(&TcpConnection::connectDestroyed,
        conn));
    if (connect_ && retry_) {
        connector_.restart();
    }
}
