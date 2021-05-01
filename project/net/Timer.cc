
#include "project/net/Timer.h"

using namespace NetLib;
using namespace NetLib::net;

void Timer::restart(Timestamp now) {
    if (repeat_) {
        expire_ = Timestamp(now.addTime(0, interval_));
    }
    else expire_ = Timestamp::invaild();
}
