
#ifndef PROJECT_BASE_ATOMIC_H
#define PROJECT_BASE_ATOMIC_H

#include "project/base/noncopyable.h"

#include <stdint.h>

namespace NetLib {

namespace detail {

template<typename T>
class AtomicIntergerT : noncopyable {
public:
    AtomicIntergerT(int _value = 0)
        : value_(_value) {}
    
    T get() {
        return __sync_val_compare_and_swap(&value_, 0, 0);
    }

    T addAndGet() {
        return __sync_fetch_and_add(&value_, x) + x;
    }

    T incrementAndGet() {
        return addAndGet(1);
    }

    void add(T x) {
        getAndAdd(x);
    }

private:
    volatile T value_;
};

}   // namespace detail

typedef detail::AtomicIntergerT<int32_t> AtomicInt32;
typedef detail::AtomicIntergerT<int64_t> AtomicInt64;

}   // namespace NetLib

#endif  // PROJECT_BASE_ATOMIC_H
