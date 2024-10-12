#pragma once

#include <errno.h>

#define OK EZ_STATUS_OK

enum EzStatus {
    // Generic error code.
    EZ_STATUS_ERROR = -1,
    // Everything is A-OK!
    EZ_STATUS_OK = 0,
    // Some memory allocation failed.
    EZ_STATUS_ENOMEM = ENOMEM,
    // These are defined by the C standard.
    EZ_STATUS_EDOM = EDOM,
    EZ_STATUS_ERANGE = ERANGE,
    EZ_STATUS_EILSEQ = EILSEQ,
};

static inline int
save_and_reset_errno(void)
{
    int const old_errno = errno;
    errno = 0;
    return old_errno;
}

static inline void
restore_errno(int const old_errno)
{
    errno = old_errno;
}
