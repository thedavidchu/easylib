#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "common/logger.h"
#include "common/status.h"

struct SizeResult {
    bool const overflow;
    size_t const result;
};

/// @note   This is implemented non-portably with a GCC extension.
static inline struct SizeResult
safe_multiply(size_t const a, size_t const b)
{
    size_t r = 0;
    bool const overflow = __builtin_mul_overflow(a, b, &r);
    return (struct SizeResult){.overflow = overflow, .result = r};
}

struct PointerResult {
    enum EzStatus const status;
    void const *const ptr;
};

static inline struct PointerResult
ez_malloc(size_t const nmemb, size_t const size)
{
    struct SizeResult const num_bytes = safe_multiply(nmemb, size);
    if (num_bytes.overflow) {
        return (struct PointerResult){.status = EZ_STATUS_ERROR, .ptr = NULL};
    }
    int const old_errno = save_and_reset_errno();
    void const *const ptr = malloc(num_bytes.result);
    int const my_errno = errno;
    restore_errno(old_errno);
    // This works in both the my_errno = 0 and my_errno != 0 case.
    return (struct PointerResult){.status = my_errno, .ptr = ptr};
}

static inline struct PointerResult
ez_calloc(size_t const nmemb, size_t const size)
{
    int const old_errno = save_and_reset_errno();
    void const *const ptr = calloc(nmemb, size);
    int const my_errno = errno;
    restore_errno(old_errno);
    // This works in both the my_errno = 0 and my_errno != 0 case.
    return (struct PointerResult){.status = my_errno, .ptr = ptr};
}

static inline struct PointerResult
ez_realloc(void *ptr, size_t const nmemb, size_t const size)
{
    struct SizeResult const num_bytes = safe_multiply(nmemb, size);
    if (num_bytes.overflow) {
        return (struct PointerResult){.status = EZ_STATUS_ERROR, .ptr = NULL};
    }
    if (num_bytes.result == 0) {
        LOGGER_ERROR("undefined behaviour -- we will simply free the pointer");
        free(ptr);
        return (struct PointerResult){.status = OK, .ptr = NULL};
    }
    int const old_errno = save_and_reset_errno();
    void const *const my_ptr = realloc(ptr, num_bytes.result);
    int const my_errno = errno;
    restore_errno(old_errno);
    // This works in both the my_errno = 0 and my_errno != 0 case.
    return (struct PointerResult){.status = my_errno, .ptr = ptr};
}
