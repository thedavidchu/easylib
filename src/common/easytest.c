#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "common/easylogger.h"
#include "common/easytest.h"

void _easy_test_success(bool const success, char const *const func_call_string,
                        char const *const file, int const line) {
  if (!success) {
    _easy_logger(stderr, EASY_LOGGER_LEVEL_TRACE, EASY_LOGGER_LEVEL_ERROR, file,
                 line, "%s failed", func_call_string);
    exit(EXIT_FAILURE);
  }
  _easy_logger(stderr, EASY_LOGGER_LEVEL_TRACE, EASY_LOGGER_LEVEL_INFO, file,
               line, "%s succeeded", func_call_string);
}

void _easy_test_assert_uintcmp(bool const success, char const *const op_string,
                               char const *const lhs_string,
                               char const *const rhs_string, uint64_t const lhs,
                               uint64_t const rhs, char const *const file,
                               int const line) {
  if (!success) {
    _easy_logger(stderr, EASY_LOGGER_LEVEL_TRACE, EASY_LOGGER_LEVEL_ERROR, file,
                 line, "%s %s %s failed (%" PRIu64 " %s %" PRIu64 ")",
                 lhs_string, op_string, rhs_string, rhs, op_string, lhs);
    exit(EXIT_FAILURE);
  }
}
