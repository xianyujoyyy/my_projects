#ifndef BASE_ATOMIC_H
#define BASE_ATOMIC_H

#include "project1_websever/base/noncopyable.h"

#include <stdint.h>

namespace NetLib {

namespace detail {
template<typename T>
class AtomicIntegerT : noncopyable {
public:
    AtomicIntegerT() :
        value_(0) {}
    
    T get() {
        return __sync_val_compare_and_swap(&value_, 0, 0);
    }

    T getAndAdd(T x) {
        return __sync_fetch_and_add(&value_, x);
    }

    T addAndGet(T x) {
        return getAndAdd(x) + x;
    }

    T incrementAndGet() {
        return addAndGet(1);
    }

    T decrementAndGet() {
        return addAndGet(-1);
    }

    void add(T x) {
        getAndAdd(x);
    }

    void increment() {
        incrementAndGet();
    }

    void decrement() {
        decrementAndGet();
    }

    T getAndSet(T newValue) {
        return __sync_lock_test_and_set(&value_, newValue);
    }

private:
    volitile T value_;
};
}   // namespace detail

typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;

}   // namespace NetLib

#endif  // BASE_ATOMIC_H
