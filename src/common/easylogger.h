#pragma once

#include <stdarg.h>
#include <stdio.h>

// NOTE The relationship between these levels is subject to change. But
//      if you do go ahead and change them, you need to change the
//      EASY_LOGGER_LEVEL_STRINGS too.
enum EasyLoggerLevels {
  EASY_LOGGER_LEVEL_TRACE = 0,
  EASY_LOGGER_LEVEL_DEBUG = 1,
  EASY_LOGGER_LEVEL_INFO = 2,
  EASY_LOGGER_LEVEL_WARN = 3,
  EASY_LOGGER_LEVEL_ERROR = 4,
  EASY_LOGGER_LEVEL_FATAL = 5,
};

static char const *const EASY_LOGGER_LEVEL_STRINGS[] = {
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

// NOTE This is unused but I may support logging to separate streams
//      with different loggers.
struct EasyLogger {
  FILE *stream;
  enum EasyLoggerLevels level;
};

// NOTE These are globally configurable parameters. Having these as macros
//      means there is no performance overhead for unused logging (assuming
//      effective dead code elimination) but it is harder to configure.
#define EASY_LOGGER_STREAM stderr
#define EASY_LOGGER_LEVEL EASY_LOGGER_LEVEL_TRACE

// NOTE I intentionally make all of the parameters verbose instead of
//      packing some of them into a structure. This is because then we
//      are not reliant on the structure if I decide I no longer like it.
static inline void _easy_logger(FILE *const stream,
                                enum EasyLoggerLevels const threshold_level,
                                enum EasyLoggerLevels const log_level,
                                char const *const file, int const line,
                                char const *const format, ...) {
  va_list ap;
  if (log_level < threshold_level) {
    return;
  }
  fprintf(stream, "[%s] %s:%d : ", EASY_LOGGER_LEVEL_STRINGS[log_level], file,
          line);
  va_start(ap, format);
  vfprintf(stream, format, ap);
  va_end(ap);
  fprintf(stream, "\n");
}

#define EASY_LOGGER_TRACE(logger, ...)                                         \
  _easy_logger(((logger)).stream, ((logger)).level, EASY_LOGGER_LEVEL_TRACE,   \
               __FILE__, __LINE__, __VA_ARGS__)

#define EASY_LOGGER_DEBUG(logger, ...)                                         \
  _easy_logger(((logger)).stream, ((logger)).level, EASY_LOGGER_LEVEL_DEBUG,   \
               __FILE__, __LINE__, __VA_ARGS__)

#define EASY_LOGGER_INFO(logger, ...)                                          \
  _easy_logger(((logger)).stream, ((logger)).level, EASY_LOGGER_LEVEL_INFO,    \
               __FILE__, __LINE__, __VA_ARGS__)

#define EASY_LOGGER_WARN(logger, ...)                                          \
  _easy_logger(((logger)).stream, ((logger)).level, EASY_LOGGER_LEVEL_WARN,    \
               __FILE__, __LINE__, __VA_ARGS__)

#define EASY_LOGGER_ERROR(logger, ...)                                         \
  _easy_logger(((logger)).stream, ((logger)).level, EASY_LOGGER_LEVEL_ERROR,   \
               __FILE__, __LINE__, __VA_ARGS__)

#define EASY_LOGGER_FATAL(logger, ...)                                         \
  _easy_logger(((logger)).stream, ((logger)).level, EASY_LOGGER_LEVEL_FATAL,   \
               __FILE__, __LINE__, __VA_ARGS__)
