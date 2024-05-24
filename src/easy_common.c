#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "easy_common.h"

/*******************************************************************************
 *  ERROR PRINTING FUNCTIONS
 ******************************************************************************/

void
easy_print_error(char *msg, char *file, int line)
{
    fprintf(stderr, "Error [%s:%d]: %s\n", file, line, msg);
}

void
_easy_assert(int const condition,
             char const *const file,
             int const line,
             char const *const msg,
             ...)
{
    if (!condition) {
        va_list ap;
        va_start(ap, msg);
        fprintf(stderr, "Error [%s:%d]: ", file, line);
        vfprintf(stderr, msg, ap);
        fprintf(stderr, "\n");
        va_end(ap);
        exit(EXIT_FAILURE);
    }
}

/*******************************************************************************
 *  MEMORY MANAGEMENT
 ******************************************************************************/

void *
_easy_calloc(size_t nmemb, size_t size, char *file, int line)
{
    void *ptr = calloc(nmemb, size);
    if (ptr == NULL) {
        easy_print_error("out of memory", file, line);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void *
_easy_realloc(void *ptr, size_t nmemb, size_t size, char *file, int line)
{
    EASY_GUARD(nmemb > 0 && size > 0, "nmemb and size should be positive");
    EASY_GUARD(nmemb > SIZE_MAX / size, "overflow");
    const size_t new_size = nmemb * size;
    void *new_ptr = realloc(ptr, new_size);
    if (new_ptr == NULL && new_size > 0) { /* What if new_size == 0? */
        easy_print_error("out of memory", file, line);
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

void
_easy_free(void *ptr, char *file, int line)
{
    if (ptr == NULL) {
        easy_print_error("freeing null pointer", file, line);
        exit(EXIT_FAILURE);
    }
    free(ptr);
}

/*******************************************************************************
 *  MEMORY MANIPULATION
 ******************************************************************************/

void *
_easy_duplicate(void const *const src, size_t const nmemb, size_t const size)
{
    EASY_GUARD(nmemb > 0 && size > 0, "nmemb and size should be positive");
    EASY_GUARD(nmemb < SIZE_MAX / size, "overflow");
    const size_t new_size = nmemb * size;
    void *dst = EASY_CALLOC(nmemb, size);
    void *tmp_dst = memcpy(dst, src, new_size);
    EASY_ASSERT(tmp_dst == dst, "tmp_dst and dst must match!");
    return dst;
}
