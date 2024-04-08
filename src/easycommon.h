#pragma once

#ifndef EASYCOMMON_H
#define EASYCOMMON_H

#include <stddef.h>

/*******************************************************************************
 *  ERROR PRINTING FUNCTIONS
 ******************************************************************************/

void easy_print_error(char *msg, char *file, int line);
void _easy_assert(int condition, char *msg, char *file, int line);

/* We explicitly convert the condition to an int so that it will not cause
 * problems for the function call */
#define EASY_ASSERT(condition, msg)                                            \
  _easy_assert((condition) ? 1 : 0, msg, __FILE__, __LINE__)
/** Guard against erroneous inputs */
#define EASY_GUARD(condition, msg)                                             \
  _easy_assert((condition) ? 1 : 0, msg, __FILE__, __LINE__)
#define EASY_IMPOSSIBLE() _easy_assert(0, "impossible!", __FILE__, __LINE__)
#define EASY_NOT_IMPLEMENTED()                                                 \
  _easy_assert(0, "not implemented!", __FILE__, __LINE__)

/*******************************************************************************
 *  MEMORY MANAGEMENT
 ******************************************************************************/

/** Allocate potentially uninitialized memory.
 *  NOTE    I return memory initialized to zero, but I do not guarantee that
 *          this is the case!
 */
#define EASY_MALLOC(nmemb, size) _easy_calloc(nmemb, size, __FILE__, __LINE__)
#define EASY_CALLOC(nmemb, size) _easy_calloc(nmemb, size, __FILE__, __LINE__)
#define EASY_REALLOC(ptr, nmemb, size)                                         \
  _easy_realloc(ptr, nmemb, size, __FILE__, __LINE__)
#define EASY_FREE(ptr) _easy_free(ptr, __FILE__, __LINE__)

void *_easy_calloc(size_t nmemb, size_t size, char *file, int line);

void *_easy_realloc(void *ptr, size_t nmemb, size_t size, char *file, int line);

void _easy_free(void *ptr, char *file, int line);

/*******************************************************************************
 *  MEMORY MANIPULATION
 ******************************************************************************/

#define EASY_DUPLICATE(src, nmemb, size) _easy_duplicate(src, nmemb, size)

void *_easy_duplicate(void const *const src, size_t const nmemb,
                      size_t const size);

/** Set all values within an object to zero. */
#define EASY_SET_ZERO(ptr) memset(ptr, 0, sizeof(*ptr));
#define EASY_SET_POINTER_ARRAY_ZERO(ptr, length)                               \
  memset(ptr, 0, sizeof(*ptr) * (length))

/*******************************************************************************
 *  MISCELLANEOUS
 ******************************************************************************/

/** Get the reverse index of an array. */
#define EASY_REVERSE_INDEX(length, idx) ((length)-1 - (idx))

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#endif /* !EASYCOMMON_H */
