#pragma once

#include <stdbool.h>
#include <stdint.h>

void _easy_test_success(bool const success, char const *const func_call_string,
                        char const *const file, int const line);

#define EASY_TEST_SUCCESS(func_call)                                           \
  _easy_test_success(((func_call)), #func_call, __FILE__, __LINE__)

void _easy_test_assert_uintcmp(bool const success, char const *const op_string,
                               char const *const lhs_string,
                               char const *const rhs_string, uint64_t const lhs,
                               uint64_t const rhs, char const *const file,
                               int const line);

#define EASY_TEST_ASSERT_UINTCMP(lhs, op, rhs)                                 \
  _easy_test_assert_uintcmp(((lhs))op((rhs)), #op, #lhs, #rhs, lhs, rhs,       \
                            __FILE__, __LINE__)
