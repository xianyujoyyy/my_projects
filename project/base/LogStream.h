
#ifndef PROJECT_BASE_LOGSTREAM_H
#define PROJECT_BASE_LOGSTREAM_H

#include "project/base/noncopyable.h"
#include <string.h>

namespace NetLib {

namespace detail {

const int kSmallBuffer = 4000;

template<int SIZE>
class LogBuffer : noncopyable {
public:
    LogBuffer()
        : cur_(data_) {}
    ~LogBuffer() {}

    void append(const char* buf, size_t len) {
        if (static_cast<size_t>(avail()) > len) {
            memcpy(cur_, buf, len);
            cur_ += len;
        }
    }

    int avail() const { return static_cast<int>(end() - cur_); }

private:
    const char* end() const { return data_ + sizeof(data_); }

    char data_[SIZE];
    char* cur_;

};

}   //  namespace detail

class LogStream : noncopyable {
    // private
    typedef LogStream self;
public:
    typedef detail::LogBuffer<detail::kSmallBuffer> Buffer;

    self& operator<<(int);
    // TODO:other reload functions

    self& operator<<(const string& str) {
        buffer_.append(str.c_str(), str.size());
        return *this;
    }

    self& operator<<(const char* str) {
        buffer_.append(str, strlen(str));
        return *this;
    }

private:
    Buffer buffer_;

};

}   //  namespace NetLib

#endif  //  PROJECT_BASE_LOGSTREAM_H
