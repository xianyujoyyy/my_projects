
#include "project/base/Condition.h"
#include "sys/time.h"

using namespace NetLib;

bool Condition::waitForSeconds(int64_t time_ms) {
    struct timespec wait_time;
    clock_gettime(CLOCK_REALTIME, &wait_time);
    // tv_sec time_t tv_nsec long
    wait_time.tv_sec += static_cast<time_t> (time_ms / 1000);
    wait_time.tv_nsec += static_cast<long> (time_ms % 1000 * 1000 * 1000);

    return ETIMEOUT == pthread_cond_timedwait(&pcond_, mutex_.getMutexPointer(), 
        &wait_time);
}
