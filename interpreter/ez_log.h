/** @brief  The Easy Logger (ez_log) library. */
#pragma once

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// NOTE The relationship between these levels is subject to change. But
//      if you do go ahead and change them, you need to change the
//      EZ_LOG_LEVEL_STRINGS too.
enum EzLogLevel {
    EZ_LOG_LEVEL_ALL = 0,
    EZ_LOG_LEVEL_VERBOSE = 1,
    EZ_LOG_LEVEL_TRACE = 2,
    EZ_LOG_LEVEL_DEBUG = 3,
    EZ_LOG_LEVEL_INFO = 4,
    EZ_LOG_LEVEL_WARN = 5,
    EZ_LOG_LEVEL_ERROR = 6,
    EZ_LOG_LEVEL_FATAL = 7,
    EZ_LOG_LEVEL_NONE = 8,
};

// NOTE These are globally configurable parameters.
//      Having these as macros means there is no performance overhead
//      or unused logging (assuming effective dead code elimination).
#define EZ_LOG_STREAM stdout
static const enum EzLogLevel EZ_LOG_COMPILER_LEVEL = EZ_LOG_LEVEL_INFO;
static enum EzLogLevel ez_log_level = EZ_LOG_LEVEL_ERROR;

static char const *const EZ_LOG_LEVEL_STRINGS[] = {"ALL",
                                                   "VERBOSE",
                                                   "TRACE",
                                                   "DEBUG",
                                                   "INFO",
                                                   "WARN",
                                                   "ERROR",
                                                   "FATAL",
                                                   "NONE"};

static inline bool
ez_log_header(FILE *const stream,
              enum EzLogLevel const compiler_threshold_level,
              enum EzLogLevel const threshold_level,
              enum EzLogLevel const log_level,
              int const errno_,
              char const *const file,
              int const line)
{
    time_t t = {0};
    if (log_level < compiler_threshold_level || log_level < threshold_level) {
        return false;
    }
    t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(stream,
            "[%s] [%d-%02d-%02d %02d:%02d:%02d] [ %s:%d ] [errno %d: %s] ",
            EZ_LOG_LEVEL_STRINGS[log_level],
            tm.tm_year + 1900,
            tm.tm_mon + 1,
            tm.tm_mday,
            tm.tm_hour,
            tm.tm_min,
            tm.tm_sec,
            file,
            line,
            // NOTE I print this even where there is no error to make
            //      the log easier to parse.
            errno_,
            strerror(errno_));
    return true;
}

// NOTE I intentionally make all of the parameters verbose instead of
//      packing some of them into a structure. This is because then we
//      are not reliant on the structure if I decide I no longer like it.
/// @note   I capture errno outside of this function because any standard
///         library call (e.g. printf) can change the value of errno.
static inline void
ez_log(FILE *const stream,
       enum EzLogLevel const compiler_threshold_level,
       enum EzLogLevel const threshold_level,
       enum EzLogLevel const log_level,
       int const errno_,
       char const *const file,
       int const line,
       char const *const format,
       ...)
{
    va_list ap;
    if (!ez_log_header(stream,
                       compiler_threshold_level,
                       threshold_level,
                       log_level,
                       errno_,
                       file,
                       line))
        return;
    va_start(ap, format);
    vfprintf(stream, format, ap);
    va_end(ap);
    fprintf(stream, "\n");
    fflush(stream);
}

#define EZ_LOG_VERBOSE(...)                                                    \
    ez_log(EZ_LOG_STREAM,                                                      \
           EZ_LOG_COMPILER_LEVEL,                                              \
           ez_log_level,                                                       \
           EZ_LOG_LEVEL_VERBOSE,                                               \
           errno,                                                              \
           __FILE__,                                                           \
           __LINE__,                                                           \
           __VA_ARGS__)

#define EZ_LOG_TRACE(...)                                                      \
    ez_log(EZ_LOG_STREAM,                                                      \
           EZ_LOG_COMPILER_LEVEL,                                              \
           ez_log_level,                                                       \
           EZ_LOG_LEVEL_TRACE,                                                 \
           errno,                                                              \
           __FILE__,                                                           \
           __LINE__,                                                           \
           __VA_ARGS__)

#define EZ_LOG_DEBUG(...)                                                      \
    ez_log(EZ_LOG_STREAM,                                                      \
           EZ_LOG_COMPILER_LEVEL,                                              \
           ez_log_level,                                                       \
           EZ_LOG_LEVEL_DEBUG,                                                 \
           errno,                                                              \
           __FILE__,                                                           \
           __LINE__,                                                           \
           __VA_ARGS__)

#define EZ_LOG_INFO(...)                                                       \
    ez_log(EZ_LOG_STREAM,                                                      \
           EZ_LOG_COMPILER_LEVEL,                                              \
           ez_log_level,                                                       \
           EZ_LOG_LEVEL_INFO,                                                  \
           errno,                                                              \
           __FILE__,                                                           \
           __LINE__,                                                           \
           __VA_ARGS__)

#define EZ_LOG_WARN(...)                                                       \
    ez_log(EZ_LOG_STREAM,                                                      \
           EZ_LOG_COMPILER_LEVEL,                                              \
           ez_log_level,                                                       \
           EZ_LOG_LEVEL_WARN,                                                  \
           errno,                                                              \
           __FILE__,                                                           \
           __LINE__,                                                           \
           __VA_ARGS__)

#define EZ_LOG_ERROR(...)                                                      \
    ez_log(EZ_LOG_STREAM,                                                      \
           EZ_LOG_COMPILER_LEVEL,                                              \
           ez_log_level,                                                       \
           EZ_LOG_LEVEL_ERROR,                                                 \
           errno,                                                              \
           __FILE__,                                                           \
           __LINE__,                                                           \
           __VA_ARGS__)

#define EZ_LOG_FATAL(...)                                                      \
    ez_log(EZ_LOG_STREAM,                                                      \
           EZ_LOG_COMPILER_LEVEL,                                              \
           ez_log_level,                                                       \
           EZ_LOG_LEVEL_FATAL,                                                 \
           errno,                                                              \
           __FILE__,                                                           \
           __LINE__,                                                           \
           __VA_ARGS__)
