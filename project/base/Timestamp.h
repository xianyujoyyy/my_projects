
#ifndef PROJECT_BASE_TIMESTAMP_H
#define PROJECT_BASE_TIMESTAMP_H

#include "project/base/copyable.h"

#include <algorithm>
#include <stdint.h>

namespace NetLib {

class Timestamp : copyable {
public:
    Timestamp()
        : microSecondsSinceEpoch_(0) {}
    explicit Timestamp(int64_t _microSecondsSinceEpoch)
        : microSecondsSinceEpoch_(_microSecondsSinceEpoch) {}
    
    int64_t microSecondSinceEpoch() const { return microSecondSinceEpoch_; }

    void swap(Timestamp& that) {
        std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
    }

    bool vaild() const { return microSecondsSinceEpoch_ > 0; }
    
    const char* toFormatString() const;

    Timestamp addTime(int seconds, int64_t microSeconds) {
        return Timestamp(microSecondsSinceEpoch_ + microSeconds 
            + seconds * kMicroSecondsPerSecond);
    }

    static Timestamp fromUnixTime(time_t unixtime) {
        return fromUnixTime(unixtime, 0);
    }
    static Timestamp fromUnixTime(time_t unixtime, int64_t micro_seconds) {
        return Timestamp(static_cast<int64_t>(unixtime) * kMicroSecondsPerSecond 
            + micro_seconds);
    }
    static Timestamp invaild() { return Timestamp(0); }
    static Timestamp now();

    static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
    friend bool operator<(const Timestamp& t1, const Timestamp& t2);
    friend bool operator==(const Timestamp& t1, const Timestamp& t2);
    
    int64_t microSecondsSinceEpoch_;
};

bool operator<(const Timestamp& t1, const Timestamp& t2) {
    return t1.microSecondsSinceEpoch_ < t2.microSecondsSinceEpoch_;
}

bool operator==(const Timestamp& t1, const Timestamp& t2) {
    return t1.microSecondsSinceEpoch_ == t2.microSecondsSinceEpoch_;
}

}   // namespace NetLib

#endif  // PROJECT_BASE_TIMESTAMP_H
