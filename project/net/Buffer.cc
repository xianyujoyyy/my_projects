
#include "project/net/Buffer.h"

#include <errno.h>
#include <sys/uio.h>

using namespace NetLib::net;

ssize_t Buffer::readFd(int fd, int* error_num) {
    char extrabuf[65536];
    struct iovec vec[2];
    size_t writable = writableBytes();
    vec[0].iov_base = beginWrite();
    vec[0].iov_len = writable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);
    const int iovcnt = (writableBytes() > sizeof(extrabuf)) ? 1 : 2;
    ssize_t n = ::readv(fd, vec, iovcnt);
    if (n < 0) {
        *error_num = errno;
    }
    else if (static_cast<size_t>(n) <= writableBytes()) {
        write_index_ += n;
    }
    else {
        write_index_ = buffer_.size();
        append(extrabuf, n - writable);
    }
    return n;
}
