
#ifndef PROJECT_BASE_LOGGING_H
#define PROJECT_BASE_LOGGING_H

#include "project/base/LogStream.h"
#include "project/base/Timestamp.h"

namespace NetLib {

class Logger {
public:
    enum LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };

    class SourceFile {
    public:
        explicit SourceFile(const char* filename)
            : data_(filename) {
                const char* slash = strrchr(data_, '/');
                if (slash) {
                    data_ = slash + 1;
                }
                size_ = static_cast<int>(strlen(data_));
            }
    private:
        const char* data_;
        int size_;

    };

    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);
    Logger(SourceFile file, int line, LogLevel level, const char* func);
    Logger(SourceFile file, int line, bool toAbort);
    ~Logger();

    LogStream& stream() { return impl_.stream_; }

    static LogLevel LogLevel();
    static void setLogLevel(LogLevel level);

    typedef void (*OutputFunc)(const char* msg, int len);
    typedef void (*FlushFunc)();
    static OutputFunc g_output;
    static FlushFunc g_flush;

private:
    class Impl {
    public:
        typedef Logger::LogLevel Loglevel;
        Impl(LogLevel level, int old_errno, const SourceFile& file, int line);

        void formatTime();
        void finish();

        Timestamp time_;
        LogStream stream_;
        LogLevel level_;
        int line_;
        SourceFile basename_;

    };

    Impl impl_;

};

#define LOG_TRACE if (NetLib::Logger::logLevel() <= NetLib::Logger::TRACE) \
    NetLib::Logger(__FILE__, __LINE__, NetLib::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (NetLib::Logger::logLevel() <= NetLib::Logger::DEBUG) \
    NetLib::Logger(__FILE__, __LINE__, NetLib::Logger::DEBUG, __func__).stream()
#define LOG_INFO if (NetLib::Logger::logLevel() <= NetLib::Logger::INFO) \
    NetLib::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN NetLib::Logger(__FILE__, __LINE__, NetLib::Logger::WARN).stream()
#define LOG_ERROR NetLib::Logger(__FILE__, __LINE__, NetLib::Logger::ERROR).stream()
#define LOG_FATAL NetLib::Logger(__FILE__, __LINE__, NetLib::Logger::FATAL).stream()
#define LOG_SYSERR NetLib::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL NetLib::Logger(__FILE__, __LINE__, true).stream()

const char* strerror_tl(int savedErrno);

}   // namespace NetLib

#endif  // PROJECT_BASE_LOGGING_H
