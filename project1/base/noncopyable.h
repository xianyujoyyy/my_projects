
#ifndef PROJECT1_BASE_NONCOPYABLE_H
#define PROJECT1_BASE_NONCOPYABLE_H

namespace NetLib {

class noncopyable {
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;
protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

};  // namespace NetLib

#endif  // PROJECT1_BASE_NONCOPYABLE_H
