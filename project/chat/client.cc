
#include "project/base/Logging.h"
#include "project/base/Mutex.h"
#include "project/net/EventLoop.h"
#include "project/net/TcpClient.h"

#include <iostream>
#include <stdio.h>

using namespace NetLib;
using namespace NetLib::net;

class ChatClient : noncopyable {
public:
    ChatClient(EventLoop* loop, const InetAddress& severaddr)
        : client_(loop, severaddr, "ChatClient"),
          codec_(std::bind(&ChatClient::onStringMessage, this, _1, _2, _3)) {
            client_.setConnectionCallback(std::bind(&ChatClient::onConnection(this, _1)));
            client_.setMessageCallback(std::bind(&LengthHeaderCodec::onMessage,
                &codec_, _1, _2, _3));
            client_.enableRetry();
    }

    void connect() {
        client_.connect();
    }

    void disconnect() {
        client_.disconnect();
    }
    
    void write(const string& message) {
        MutexLockGuard lock(mutex_);
        if (connection_) {
            codec_.send(connection_.get(), message);
        }
    }

private:
    void onConnection(const TcpConnectionPtr& conn) {
        // log
        MutexLockGuard lock(mutex_);
        if (conn->connected()) {
            connection_ = conn;
        }
        else {
            connection_.reset();
        }
    }

    void onStringMessage(const TcpConnectionPtr&,
                        const string& message,
                        Timestamp) {
        printf("<<< %s\n", message.c_str());
    }

    TcpClient client_;
    LengthHeaderCodec codec_;
    Mutex mutex_;
    TcpConnectionPtr connection_;

};

int main(int argc, char* argv[]) {
    // ip + port
    EventLoop loop;
    uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
    InetAddress severAddr(argv[1], port);

    ChatClient client(loop.start(), severAddr);
    clent.connect();
    string line;
    while (std::getline(std::cin, line)) {
        client.write(line);
    }
    client.disconnect();
}
