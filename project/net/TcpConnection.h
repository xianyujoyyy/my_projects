
#ifndef PROJECT_NET_TCPCONNECTION_H
#define PROJECT_NET_TCPCONNECTION_H

#include "project/net/Callback.h"
#include "project/net/InetAddress.h"
#include "project/net/Buffer.h"
#include "project/base/noncopyable.h"

#include <string>
#include <memory>

using std::string;

namespace NetLib {

namespace net {

class EventLoop;
class Channel;
class Socket;
class Timestamp;

class TcpConnection : NetLib::noncopyable,
                      public std::enable_shared_from_this<TcpConnection> {
public:
    TcpConnection(EventLoop* loop, int sockfd, 
        const InetAddress& _localaddr, const InetAddress& _peeraddr);
    ~TcpConnection();

    EventLoop* getLoop() const { return loop_; }
    // bool connecting() const { return state_ == kConnected; }
    // bool disconnected() const { return state_ == kDisconnected; }
    void send(Buffer& buffer);
    void send(const string& s);

    bool isReading() const { return reading_; }
    void startReading();
    void stopReading();

    void shutDownWrite();

    void forceCLose();
    void forceCloseWithDelay(int ms);

    void setKeepAlive(bool on);

    void setConnectionCallback(const ConnectionCallback& cb) { 
        connection_cb_ = cb; }
    void setMessageCallback(const MessageCallback& cb) {
       message_cb_ = cb; }
    void setWriteCompleteCallback(const WriteCompleteCallback& cb) {
        write_complete_cb_ = cb; }
    void setHighWaterMarkCallback(const HighWaterMarkCallback& cb, size_t highWaterMark) {
        high_water_cb_ = cb; high_water_mark_ = highWaterMark; }

    void connectEstablished();
    void connectDestroyed();

private:
    enum State { kConnecting, kConnected, kDisconnecting, kDisconnected };

    void setState(State state) { state_ = state; }

    void startReadInLoop();
    void stopReadInLoop();
    void writeInLoop();
    void shutdownInLoop();
    void closeInLoop();

    void handleRead(Timestamp);
    void handleWrite();
    void handleClose();
    void handleError();

    EventLoop* loop_;
    State state_;
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    InetAddress localaddr_;
    InetAddress peeraddr_;
    bool reading_;
    Buffer output_buffer_;
    Buffer input_buffer_;
    // ?
    size_t high_water_mark_;

    ConnectionCallback connection_cb_;
    CloseCallback close_cb_;
    MessageCallback message_cb_;
    WriteCompleteCallback write_complete_cb_;
    HighWaterMarkCallback high_water_cb_;

};

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

}   // namespace net

}   // namespace NetLib

#endif  // PROJECT_NET_TCPCONNECTION_H
