
#include "project/chat/codec.h"

#include "project/base/ThreadLocalSingleton.h"
#include "project/base/Logging.h"
#include "project/base/Mutex.h"
#include "project/net/EventLoopThreadPool.h"
#include "project/net/TcpSever.h"

#include <set>
#include <stdio.h>

using namespace NetLib;
using namespace NetLib::net;

class ChatSever : noncopyable {
public:
    ChatSever(EventLoop* loop, const InetAddress& listen_addr)
        : sever_(loop, listen_addr, "ChatSever"),
          codec_(std::bind(&ChatSever::onStringMessage, this, _1, _2, _3)) {
            sever_.setConnectionCallback(std::bind(&ChatSever::onConnection, this, _1));
            sever_.setMessageCallback(
                std::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3)
            );
        }

    void start() {
        sever_.start();
    }

    void setThreadNum(int num_threads) {
        server_.setThreadInitCallback(std::bind(&ChatServer::threadInit, this, _1));
        sever_.setThreadNum(num_threads);
    }

private:
    void onConnection(const TcpConnectionPtr& conn) {
        // log
        if (conn->connected()) {
            LocalConnections::instance().insert(conn);
        }
        else {
            LocalConnections::instance().erase(conn);
        }
    }

    void onStringMessage(const TcpConnectionPtr&,
                        const string& message,
                        Timestamp) {
        EventLoop::Functor f = std::bind(&ChatServer::distributeMessage, this, message);

        MutexLockGuard lock(mutex_);
        for (std::set<EventLoop*>::iterator it = loops_.begin();
            it != loops_.end();
            ++it) {
            (*it)->queueInLoop(f);
        }
    }

    typedef std::set<TcpConnectionPtr> ConnectionList;

    void distributeMessage(const string& message) {
        for (ConnectionList::iterator it = LocalConnections::instance().begin();
            it != LocalConnections::instance().end();
            ++it) {
            codec_.send(it->get(), message);
        }
    }

    void threadInit(EventLoop* loop) {
        LocalConnections::instance();
        MutexLockGuard lock(mutex_);
        loops_.insert(loop);
    }

    TcpSever sever_;
    LengthHeaderCodec codec_;
    typedef ThreadLocalSingleton<ConnectionList> LocalConnections;

    Mutex mutex_;
    std::set<EventLoop*> loops_;

};

int main(int argc, char* argv[]) {
    // port + n_thread
    EventLoop loop;
    uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
    InetAddress serverAddr(port);
    ChatServer server(&loop, serverAddr);
    server.setThreadNum(atoi(argv[2]));
    server.start();
    loop.loop();
}
