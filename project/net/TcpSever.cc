
#include "project/net/TcpSever.h"

#include "project/net/Acceptor.h"
#include "project/net/EventLoop.h"
#include "project/net/EventLoopThread.h"
#include "project/net/EventLoopThreadPool.h"
#include "project/net/Socket.h"

using namespace NetLib;
using namespace NetLib::net;

TcpSever::TcpSever(EventLoop* loop, const InetAddress& listenaddr,
    string name, bool reuseport)
    : loop_(loop),
      listenaddr_(listenaddr),
      ip_port_(),
      name_(name),
      reuseport_(reuseport),
      acceptor_(new Acceptor(loop_, listenaddr_, reuseport_)),
      threadpool_(make_shared(EventLoopThreadPool(loop_, name_ + "threadpool"))),
      conn_id_(1) {
          acceptor_->setNewConnectionCallback(std::bind(&TcpSever::newConnection,
            this, _1, _2));
      }

TcpSever::~TcpSever() {
    for (auto & conn : connections_) {
        TcpConnectionPtr cur_conn(conn.second);
        conn.reset(); // --use_count
        cur_conn->getLoop()->queueInLoop(&TcpConnection::connectDestroyed,
            cur_conn); // !
    }
}

void TcpSever::setThreadNum(int n) {
    threadpool_->setThreadNum(n);
}

void TcpSever::setThreadInitFunction(const ThreadInitFunc& func) {
    threadpool_->setThreadInitFunction(func);
}

void TcpSever::start() {
    if (connect_.get()) return;
    connect_.add(1);
    threadpool_->start();
    loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
}

void TcpSever::newConnection(int sockfd, const InetAddress& peeraddr) {
    string conn_name = name_ + to_string(conn_id_++);
    EventLoop* io_loop = threadpool_->getNextLoop();
    // TODO
    InetAddress localaddr(sockets::getLocalAddr(sockfd));
    TcpConnectionPtr conn = make_shared(TcpConnection(io_loop, sockfd,
        localaddr, peeraddr));
    // 三个半事件
    conn->setConnectionCallback(connection_cb_);
    conn->setMessageCallback(message_cb_);
    conn->setWriteCompleteCallback(write_complete_cb_);
    conn->setCloseCallback(close_cb_);
    connections_[conn_name] = conn;
    io_loop->runInLoop(std::bind(&TcpConnection::connectEstablished, 
        conn));
}

void TcpSever::removeConnection(const TcpConnectionPtr conn) {
    conn->getLoop()->queueInLoop(std::bind(&TcpSever::removeInLoop, 
        this, conn));
}

void TcpSever::removeInLoop(const TcpConnectionPtr& conn) {
    connections_.erase(conn);
    conn->connectDestroyed();
}
