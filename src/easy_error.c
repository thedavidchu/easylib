#include <stdio.h>
#include <string.h>

#include "easy_common.h"
#include "easy_error.h"

struct EasyError
EasyError__from_errno(int const errno_)
{
    return (struct EasyError){.error_type = EASY_ERROR_TYPE_OS,
                              .msg = NULL,
                              .errno = errno_,
                              .strerror = strerror(errno_)};
}

void
EasyError__print_json(struct EasyError const *const me)
{
    EASY_GUARD(me != NULL, "pointer must not be NULL");
    printf("{\"type\": \"EasyError\", \".error_type\": %d, \".msg\": %s, "
           "\".errno\": %d, \".strerror\": %s}",
           me->error_type,
           me->msg ? me->msg : "null",
           me->errno,
           me->strerror ? me->strerror : "null");
}

void
EasyError__destroy(struct EasyError *const me)
{
    // We assume that all the strings are statically allocated. I am hesitant
    // to use dynamic allocation in the error handling code in case the
    // memory allocator is struggling.
    *me = (struct EasyError){0};
}
