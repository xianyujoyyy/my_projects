
#ifndef PROJECT_BASE_NONCOPYABLE_H
#define PROJECT_BASE_NONCOPYABLE_H

#include <string>

using std::string;

namespace NetLib {

class noncopyable {
public:
    void operator=(const noncopyable&) = delete;
    noncopyable(const noncopyable&) = delete;
    
protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

}   // namespace NetLib

#endif  // PROJECT_BASE_NONCOPYABLE_H
