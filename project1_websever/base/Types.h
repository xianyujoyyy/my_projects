#ifndef BASE_TYPES_H
#define BASE_TYPES_H

#include <stdint.h>
#include <string.h>
#include <string>

#ifndef NDEBUG
#include <assert.h>
#endif

namespace NetLib {

using std::string;

inline void memZero(void* p, size_t n) {
    memset(p, 0, n);    // string.h
}

// upcasting
template<typename To, typename From>
inline To implicit_cast(From const &f) {
    return f;
}

// downcasting
template<typename To, typename From>
inline To down_cast(From* f) {
    if (false) {
        implicit_cast<From*, To>(0);
    }

#if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
    assert(f == NULL || dynamic_cast<To>(f) != NULL);
#endif
    return static_cast<To>(f);
}
}   // namespace NetLib

#endif  // BASE_TYPES_H
