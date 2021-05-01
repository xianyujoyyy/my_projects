
#ifndef PROJECT_BASE_CURRENTTHREAD_H
#define PROJECT_BASE_CURRENTTHREAD_H

namespace NetLib {

namespace CurrentThread {

extern __thread int         t_cachedTid;
extern __thread const char  t_tidString[32];
extern __thread int         t_tidStringLength;
extern __thread const char* t_threadName;

void cachedTid();
bool isMainThread();

inline int tid() {
    if (t_cachedTid == 0) {
        cachedTid();
    }
    return t_cachedTid;
}

inline const char* tidString() {
    return t_tidString;
}

inline int tidStringLength() {
    return t_tidStringLength;
}

inline const char* threadName() {
    return t_threadName;
}

}   // namespace CurrentThread

}   // namespace NetLib

#endif  // PROJECT_BASE_CURRENTTHREAD_H
