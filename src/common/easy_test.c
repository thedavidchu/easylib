#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "common/easy_logger.h"
#include "common/easy_test.h"

#include "colours.h"

void
_easy_test_success(bool const success,
                   char const *const func_call_string,
                   char const *const file,
                   int const line)
{
    if (!success) {
        _easy_logger(stderr,
                     EASY_LOGGER_LEVEL_TRACE,
                     EASY_LOGGER_LEVEL_ERROR,
                     file,
                     line,
                     RED "%s failed" RESET,
                     func_call_string);
        exit(EXIT_FAILURE);
    }
    _easy_logger(stderr,
                 EASY_LOGGER_LEVEL_TRACE,
                 EASY_LOGGER_LEVEL_INFO,
                 file,
                 line,
                 GREEN "%s succeeded" RESET,
                 func_call_string);
}

void
_easy_test_assert_uintcmp(bool const success,
                          char const *const op_string,
                          char const *const lhs_string,
                          char const *const rhs_string,
                          uint64_t const lhs,
                          uint64_t const rhs,
                          char const *const file,
                          int const line)
{
    if (!success) {
        _easy_logger(stderr,
                     EASY_LOGGER_LEVEL_TRACE,
                     EASY_LOGGER_LEVEL_ERROR,
                     file,
                     line,
                     RED "%s %s %s failed, (%" PRIu64 " %s %" PRIu64 ")" RESET,
                     lhs_string,
                     op_string,
                     rhs_string,
                     rhs,
                     op_string,
                     lhs);
        exit(EXIT_FAILURE);
    }
}
