
#ifndef PROJECT_NET_CONNECTOR_H
#define PROJECT_NET_CONNECTOR_H

#include "project/net/InetAddress.h"
#include "project/base/noncopyable.h"

#include <functional>
#include <memory>

namespace NetLib {

namespace net {

class EventLoop;
class Channel;

class Connector : NetLib::noncopyable,
                  public std::enable_shared_from_this<Connector>
{
public:
    typedef std::function<void(int sockfd)> NewConnectionCallback;

    Connector(EventLoop* _loop, InetAddress& _severaddr);
    ~Connector() {};

    void start();
    void restart();
    void stop();

    void setNewConnectionCallback(NewConnectionCallback& _cb) {
        new_connection_callback = _cb;
    }

private:
    enum States { kDisconnected, kConnecting, kConnected };
    static const int kMaxRetryDelayMs = 30*1000;
    static const int kInitRetryDelayMs = 500;

    void setState(States s) { state_ = s; }
    void startInLoop();
    void stopInLoop();
    void connect();
    void connecting(int sockfd);
    // handle Channel
    int removeAndResetChannel();
    void resetChannel();
    void handleWrite();
    void handleError();
    // handle Socket
    void retry(int sockfd);

    EventLoop* loop_;
    InetAddress severaddr_;
    bool connect_;
    std::unique_ptr<Channel*> channel_;
    int retry_delay_ms_;
    States state_;
    NewConnectionCallback new_connection_callback;

};

}   // namespace net

}   // namespace NetLib

#endif  // PROJECT_NET_CONNECTOR_H
