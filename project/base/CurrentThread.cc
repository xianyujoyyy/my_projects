
#include "project/base/CurrentThread.h"

#include <sys/syscall.h>
#include <process.h>
#include <stdio.h>

namespace NetLib {

namespace CurrentThread {

__thread int         t_cachedTid = 0;
__thread const char  t_tidString[32];
__thread int         t_tidStringLength = 6;
__thread const char* t_threadName = "unknown";

void cachedTid() {
    if (t_cachedTid == 0) {
        t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
        t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d",
            t_cachedTid);
    }
}

bool isMainThread() {
    return t_cachedTid == ::getpid();
}

}   // namespace CurrentThread

}   // namespace NetLib
