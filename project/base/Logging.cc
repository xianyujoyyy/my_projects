
#include "project/base/Logging.h"

#include "project/base/CurrentThread.h"
#include "project/base/Timestamp.h"

#include <errno.h>

namespace NetLib {

// errno

__thread char t_errnobuf[512];
__thread char t_time[64];

const char* strerror_tl(int savedErrno) {
    strerror_r(savedErrno, t_errnobuf, sizeof(t_errnobuf));
    return t_errnobuf;
}

const char* LogLevelName[Logger::NUM_LOG_LEVELS] = {
  "TRACE ",
  "DEBUG ",
  "INFO  ",
  "WARN  ",
  "ERROR ",
  "FATAL ",
};


void defaultOutput(const char* msg, int len) {
    size_t n = fwrite(msg, 1, len, stdout);
}

void defaultFlush() {
  fflush(stdout);
}

Logger::OutputFunc g_output = defaultOutput;
Logger::FlushFunc g_flush = defaultFlush;


Logger::Impl::Impl(LogLevel level, int savedErrno, 
    const SourceFile& file, int line)
    : time_(Timestamp::now()),
      stream_(),
      level_(level),
      line_(line),
      basename_(file) {
          formatTime();
          CurrentThread::tid();
          stream_ << CurrentThread::tidString();
          stream_ << LogLevelName[level];
          if (savedErrno != 0) {
            stream_ << strerror_tl(savedErrno) << " (errno=" << savedErrno << ") ";
          }
}

void Logger::Impl::formatTime() {
    int64_t microSecondsSinceEpoch = time_.microSecondSinceEpoch();
    time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / Timestamp::kMicroSecondsPerSecond);
    int microseconds = static_cast<int>(microSecondsSinceEpoch % Timestamp::kMicroSecondsPerSecond);

    struct tm tm_time;
    ::gmtime_r(&seconds, &tm_time);

    int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",
        tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
        tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
}

inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v) {
    s.append(v.data_, v.size_);
    return s;
}

void Logger::Impl::finish() {
    stream_ << " - " << basename_ << ':' << line_ << '\n';
}

Logger::Logger(SourceFile file, int line)
  : impl_(INFO, 0, file, line) {}

Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
  : impl_(level, 0, file, line) {
    impl_.stream_ << func << ' ';
}

Logger::Logger(SourceFile file, int line, LogLevel level)
  : impl_(level, 0, file, line) {}

Logger::Logger(SourceFile file, int line, bool toAbort)
  : impl_(toAbort?FATAL:ERROR, errno, file, line) {}

Logger::~Logger() {
    impl_.finish();
    const LogStream::Buffer& buf(stream().buffer());
    g_output(buf.data(), buf.length());
    if (impl_.level_ == FATAL) {
        g_flush();
        abort();
    }
}

void Logger::setLogLevel(Logger::LogLevel level) {
    g_logLevel = level;
}

}   // namespace NetLib
