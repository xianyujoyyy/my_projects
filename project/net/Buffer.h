
#ifndef PROJECT_NET_BUFFER_H
#define PROJECT_NET_BUFFER_H

#include "project/base/copyable.h"
#include "project/net/Endian.h"

#include <stdint.h>
#include <cstddef>
#include <vector>
#include <string>
#include <algorithm>

namespace NetLib {

namespace net {

class Buffer : NetLib::copyable {
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;

    explicit Buffer(size_t initial_size = kInitialSize)
        : buffer_(kCheapPrepend + initial_size),
          read_index_(kCheapPrepend),
          write_index_(kCheapPrepend) {}
    
    void swap(Buffer& rhs) {
        using std::swap;
        buffer_.swap(rhs.buffer_);
        std::swap(read_index_, rhs.read_index_);
        std::swap(write_index_, rhs.write_index_);
    }

    char* peek() const {
        return beginRead();
    }

    size_t readableBytes() const {
        return write_index_ - read_index_;
    }
    size_t writableBytes() const {
        return buffer_.size() - write_index_;
    }
    size_t prependableBytes() const {
        return read_index_;
    }

    void retrieveAll() {
        read_index_ = kCheapPrepend;
        write_index_ = kCheapPrepend;
    }
    void retrieve(size_t len) {
        if (len < readableBytes()) {
            read_index_ += len;
        }
        else {
            retrieveAll();
        }
    }
    void retrieveUntil(const char* end) {
        retrieve(end - beginRead());
    }
    void retrieveInt64() {
        retrieve(sizeof(int64_t));
    }
    void retrieveInt32() {
        retrieve(sizeof(int32_t));
    }
    void retrieveInt16() {
        retrieve(sizeof(int16_t));
    }
    void retrieveInt8() {
        retrieve(sizeof(int8_t));
    }

    std::string retrieveAsString(size_t len) {
        std::string result(beginRead(), len);
        retrieve(len);
        return result;
    }
    std::string retrieveAllAsString() {
        return retrieveAsString(readableBytes());
    }

    void append(const char* data, size_t len) {
        ensureWritableBytes(len);
        std::copy(data, data + len, write_index_);
        write_index_ += len;
    }
    void append(const void* data, size_t len) {
        append(static_cast<const char*>(data), len);
    }
    void appendInt64(int64_t x) {
        int64_t be64 = sockets::hostToNetwork64(x);
        append(&be64, sizeof(int64_t));
    }
    void appendInt32(int32_t x) {
        int32_t be32 = sockets::hostToNetwork32(x);
        append(&be32, sizeof(int32_t));
    }
    void appendInt16(int16_t x) {
        int16_t be16 = sockets::hostToNetwork16(x);
        append(&be16, sizeof(int16_t));
    }
    void appendInt8(int8_t x) {
        append(&x, sizeof(int8_t));
    }

    int64_t peekInt64() {
        int64_t be64 = 0;
        ::memcpy(&be64, beginRead(), sizeof(int64_t));
        return sockets::networkToHost64(be64);
    }
    int32_t peekInt32() {
        int32_t be32 = 0;
        ::memcpy(&be32, beginRead(), sizeof(int32_t));
        return sockets::networkToHost32(be32);
    }
    int16_t peekInt16() {
        int16_t be16 = 0;
        ::memcpy(&be16, beginRead(), sizeof(int16_t));
        return sockets::networkToHost16(be16);
    }
    int8_t peekInt8() {
        int8_t be8 = 0;
        ::memcpy(&be8, beginRead(), sizeof(int8_t));
        return be8;
    }

    int64_t readInt64() {
        int64_t result = peekInt64();
        retrieveInt64();
        return result;
    }
    int32_t readInt32() {
        int32_t result = peekInt32();
        retrieveInt32();
        return result;
    }
    int16_t readInt16() {
        int16_t result = peekInt16();
        retrieveInt16();
        return result;
    }
    int8_t readInt8() {
        int8_t result = peekInt8();
        retrieveInt8();
        return result;
    }

    void prepend(const void* data, size_t len) {
        read_index_ -= len;
        const char* d = static_cast<const char*>(data);
        std::copy(d, d + len, beginRead());
    }
    void prependInt64(int64_t x) {
        int be64 = sockets::hostToNetwork64(x);
        prepend(&be64, sizeof(int64_t));
    }
    void prependInt32(int32_t x) {
        int be32 = sockets::hostToNetwork32(x);
        prepend(&be32, sizeof(int32_t));
    }
    void prependInt16(int16_t x) {
        int be16 = sockets::hostToNetwork16(x);
        prepend(&be16, sizeof(int16_t));
    }
    void prependInt8(int8_t x) {
        prepend(&x, sizeof(int8_t));
    }

    void shrink(size_t reverse) {
        Buffer other(readableBytes() + reverse);
        std::string tmp = retrieveAllAsString();
        other.append(tmp.data(), tmp.size());
        swap(other);
    }

    ssize_t readFd(int fd, int* error_num);
private:
    char* begin() {
        return &*buffer_.begin();
    }
    char* beginRead() {
        return begin() + read_index_;
    }
    char* beginWrite() {
        return begin() + write_index_;
    }

    void ensureWritableBytes(size_t len) {
        if (writableBytes() >= len) return;
        makeSpace(len);
    }
    void makeSpace(size_t len) {
        if (writableBytes() + prependableBytes() < len + kCheapPrepend) {
            buffer_.resize(write_index_ + len);
        }
        else {
            size_t readable = readableBytes();
            std::copy(begin() + read_index_,
                      begin() + write_index_,
                      begin() + kCheapPrepend);
            read_index_ = kCheapPrepend;
            write_index_ = kCheapPrepend + readable;
        }
    }

    std::vector<char> buffer_;
    size_t read_index_;
    size_t write_index_;

};

}   // namespace net

}   // namespace NetLib

#endif  // PROJECT_NET_BUFFER_H
