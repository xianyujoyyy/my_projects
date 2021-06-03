
#include "project/net/TcpConnection.h"

#include "project/net/EventLoop.h"
#include "project/net/Channel.h"
#include "project/net/Socket.h"
#include "project/net/Buffer.h"
#include "project/base/Timestamp.h"

namespace NetLib {

namespace net {

TcpConnection::TcpConnection(EventLoop* loop, int sockfd, 
        const InetAddress& _localaddr, const InetAddress& _peeraddr)
        : loop_(loop),
          state_(kConnecting),
          socket_(new Socket(sockfd)),
          channel_(new Channel(loop, sockfd)),
          localaddr_(_localaddr),
          peeraddr_(_peeraddr),
          reading_(false),
          output_buffer_(),
          input_buffer_(), 
          high_water_mark_(64*1024*1024) {
                channel_->setReadEventCallback(std::bind(&TcpConnection::handleRead, 
                        this, _1));
                channel_->setWriteEventCallback(std::bind(&TcpConnection::handleWrite, 
                        this));
                channel_->setCloseEventCallback(std::bind(&TcpConnection::handleClose,
                        this));
                channel_->setErrorEventCallback(std::bind(&TcpConnection::handleError,
                        this));
          }

TcpConnection::~TcpConnection() {
        // log?
}

void TcpConnection::send(Buffer& buffer) {
        string data(std::move(buffer.retrieveAllAsString()));
        send(data);
}

void TcpConnection::send(const string& s) {
        loop_->runInLoop(std::bind(&TcpConnection::writeInLoop, shared_from_this(), s));
}

void TcpConnection::writeInLoop(const string& s) {
        int len = s.size(), remaining = len;
        const char* str(s.c_str());
        if (!channel_->isWriting() && output_buffer_.readableBytes() == 0) {
                int n_wrote = sockets::write(socket_.fd(), str, len);
                if (n_wrote >= 0) {
                        remaining = len - n_wrote;
                        if (remaining == 0 && write_complete_cb_) {
                                loop_->queueInLoop(std::bind(write_complete_cb_, 
                                        shared_from_this()));
                        }
                }
                else { // error handler
                }
        }
        if (remaining > 0) {
                size_t old_len = output_buffer_.readableBytes();
                if (old_len + remaining >= high_water_mark_ && high_water_cb_) {
                        loop_->queueInLoop(std::bind(high_water_cb_, shared_from_this()));
                }
                output_buffer_.append(str[len - remaining], remaining);
                if (!channel_->isWriting()) {
                        channel_->enableWrite();
                }
        }
}

void TcpConnection::handleWrite() {
        if (channel_->isWriting()) {
                int n_wrote = socket::write(socket_.fd(), output_buffer_.peek(),
                        output_buffer_.readableBytes());
                if (n_wrote > 0) {
                        output_buffer_.retrieve(n_wrote);
                        if (output_buffer_.readableBytes() == 0) {
                                channel_->disableWrite();
                                if (write_complete_cb_) {
                                        loop_->queueInLoop(std::bind(write_complete_cb_,
                                                shared_from_this()));
                                }
                                if (state_ == kDisconnecting) {
                                        shutdownInLoop();
                                }
                        }
                }
        }
}

void TcpConnection::startReading() {
        loop_->runInLoop(std::bind(&TcpConnection::startReadInLoop,
                shared_from_this());
}

void TcpConnection::startReadInLoop() {
        if (!reading_ && !channel_->isReading()) {
                channel_->enableRead();
                reading_ = true;
        }
}

void TcpConnection::handleRead(Timestamp receiveTime) {
        int saved_errno = 0;
        int n_read = readFd(socket_.fd(), &saved_errno);
        if (n_read > 0) {
                if (message_cb_) {
                        loop_->queueInLoop(std::bind(message_cb_,
                                shared_from_this(), &input_buffer_, receiveTime));
                }
        }
        else if (n_read == 0) {
                handleError();
        }
}

void TcpConnection::stopReading() {
        loop_->queueInLoop(std::bind(&TcpConnection::stopReadInLoop,
                shared_from_this()));
}

void TcpConnection::stopReadInLoop() {
        if (reading_ || channel_->isReading()) {
                channel_->disableRead();
                reading_ = false;
        }
}

void TcpConnection::shutDownWrite() {
        loop_->queueInLoop(std::bind(&TcpConnection::shutdownInLoop,
                shared_from_this()));
}

void TcpConnection::shutdownInLoop() {
        if (!channel_->isWriting()) {
                socket_.shutdownWrite();
        }
}

void TcpConnection::forceCLose() {
        loop_->queueInLoop(std::bind(&TcpConnection::closeInLoop,
                shared_from_this()));
}

void TcpConnection::forceCloseWithDelay(int ms) {
        loop_->runAfter(ms, std::bind(&TcpConnection::closeInLoop,
                shared_from_this()));
                // TODO:weak prt
}

void TcpConnection::closeInLoop() {
        if (state_ == kConnected || state_ == kDisconnecting) {
                handleClose();
        }
}

void TcpConnection::handleClose() {
        setstate(kDisconnected);
        channel_.disableAll();
        connection_cb_(shared_from_this());
        close_cb_(shared_from_this());
}

void TcpConnection::setKeepAlive(bool on) {
        socket_.setKeepAlive(on);
}

void TcpConnection::connectEstablished() {
        setstate(kConnected);
        channel_->tie(shared_from_this());
        startReading();
        connection_cb_(shared_from_this());
}

void TcpConnection::connectDestroyed() {
        if (state_ == kConnecting) {
                setstate(kDisconnected);
                channel_->disableAll();
                connection_cb_(shared_from_this());
        }
        channel_->remove();
}

// handle error

}   // namespace net

}   // namespace NetLib
