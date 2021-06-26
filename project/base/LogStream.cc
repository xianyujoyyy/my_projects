
#include "project/base/LogStream.h"

using namespace NetLib;

LogStream& LogStream::operator<<(int n) {
    string n_str = std::to_string(n);
    buffer_.append(n_str.c_str(), n_str.size());
    return *this;
}
