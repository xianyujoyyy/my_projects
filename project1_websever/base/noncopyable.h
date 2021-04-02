#ifndef BASE_NONCOPYABLE_H
#define BASE_NONCOPYABLE_H

namespace NetLib{

class noncopyable {

public:
    // copy construction function
    noncopyable(const noncopyable&) = delete;
    // assignment construction function
    void operator=(const noncopyable&) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
    
};

}   // namespace NetLib

#endif  // BASE_NONCOPYABLE_H
