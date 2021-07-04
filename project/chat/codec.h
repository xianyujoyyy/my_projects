
#include "project/base/Logging.h"
#include "project/net/Buffer.h"
#include "project/net/Endian.h"
#include "project/net/TcpConnection.h"

// length + content
class LengthHeaderCodec : NetLib::noncopyable {
public:
    typedef std::function<void (const NetLib::net::TcpConnectionPtr&,
                                const string& message,
                                NetLib::Timestamp)> StringMessageCallback;

    LengthHeaderCodec(const StringMessageCallback& cb)
        : string_message_callback_(cb) {}
    
    void onMessage(const NetLib::net::TcpConnectionPtr& conn,
                NetLib::net::Buffer* buf,
                NetLib::Timestamp receiveTime) {
        while (buf->readableBytes() >= kHeaderLen) {
            string data = buf->peek();
            const void* tmp = data.c_str();
            int32_t be32 = *static_cast<const int32_t*>(tmp);
            const int32_t len = NetLib::net::sockets::networkToHost32(be32);
            if (len > 65536 || len < 0) {
                // error
            }
            else if (buf->readableBytes() >= len + kHeaderLen) {
                buf->retrieve(kHeaderLen);
                string message(buf->peek(), len);
                string_message_callback_(conn, message, receiveTime);
                buf->retrieve(len);
            }
        }
    }

    void send(NetLib::net::TcpConnectionPtr& conn,
            const string& message) {
        NetLib::net::Buffer buf;
        buf.append(message.data(), message.size());
        int32_t len = static_cast<int32_t>(message.size());
        int32_t be32 = NetLib::net::sockets::hostToNetwork32(len);
        buf.prepend(&be32, sizeof(be32));
        conn->send(&buf);
    }

private:
    StringMessageCallback string_message_callback_;
    const static size_t kHeaderLen = sizeof(int32_t);

};
