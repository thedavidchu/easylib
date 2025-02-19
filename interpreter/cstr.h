#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/// @brief  Returns true if both inputs are valid strings and equal.
static inline bool
cstr_valid_equal(char const *const lhs, char const *const rhs)
{
    if (lhs == NULL || rhs == NULL) {
        return false;
    }
    return strcmp(lhs, rhs) == 0;
}

/// @brief My implementation of 'strdup', which is not guaranteed to exist.
static inline char *
cstr_dup(char const *const src)
{
    if (src == NULL) {
        return NULL;
    }
    size_t const len = strlen(src);
    char *dst = malloc(len + 1);
    return strcpy(dst, src);
}

/// @brief  Assemble a string by prepending, joining, and appending.
/// @note   This function does not accept NULL values; if you want that,
///         you can write a wrapper function.
static inline char *
cstr_assemble(char const *const start,
              char const *const end,
              char const *const join,
              size_t const length,
              char const *const *const str_array)
{
    if (start == NULL || end == NULL || join == NULL || str_array == NULL) {
        return NULL;
    }
    size_t total_length = strlen(start) + strlen(end) +
                          strlen(join) * (length == 0 ? 0 : length - 1);
    for (size_t i = 0; i < length; ++i) {
        if (str_array[i] == NULL) {
            return NULL;
        }
        total_length += strlen(str_array[i]);
    }

    char *dst = malloc(total_length + 1);
    char *tmp = dst;
    strcpy(tmp, start);
    tmp += strlen(start);
    for (size_t i = 0; i < length; ++i) {
        strcpy(tmp, str_array[i]);
        tmp += strlen(str_array[i]);
        if (i != length - 1) {
            strcpy(tmp, join);
            tmp += strlen(join);
        }
    }
    strcpy(tmp, end);
    tmp += strlen(end);
    return dst;
}

/// @brief  Join a list of C-strings.
/// @note   I cannot make all of the input strings const chars without a
///         compile-time error.
/// @note   I do not accept any NULL values (I don't treat these as
///         empty strings).
static inline char *
cstr_join(char const *const join,
          size_t const length,
          char const *const *const str_array)
{
    return cstr_assemble("", "", join, length, str_array);
}

static inline char *
cstr_concat(char const *const first, char const *const second)
{
    char const *str_array[] = {NULL};
    return cstr_assemble(first, second, "", 0, str_array);
}

static inline char *
cstr_slice(char const *const src, size_t const start, size_t const end)
{
    if (src == NULL) {
        return NULL;
    }
    size_t const len = strlen(src);
    // It is legal to ask for a slice from the very last character
    // For example, "0123"[4:] would return "".
    if (start > len) {
        return NULL;
    }
    // TODO - reverse slicing is not implemented yet!
    if (start > end) {
        return NULL;
    }
    size_t const slice_len = end - start;
    char *dst = malloc(slice_len + 1);
    return strncpy(dst, &src[start], slice_len);
}

/// @brief  Get the starting index of a the first occurrence of a substring.
/// @return Return the index or SIZE_MAX on error.
static inline size_t
cstr_find(char const *const me, char const *const pattern)
{
    if (me == NULL || pattern == NULL) {
        return SIZE_MAX;
    }
    // NOTE These are optimizations to prevent a dumb compiler from
    //      performing a scan on the input strings on each iteration of
    //      the loop.
    size_t const mylen = strlen(me);
    size_t const patlen = strlen(pattern);
    // NOTE An empty string contains an empty string but cannot possibly
    //      contain another pattern.
    if (mylen == 0) {
        return (patlen == 0) ? 0 : SIZE_MAX;
    }
    size_t i = 0, j = 0;
    for (i = 0; i < mylen; ++i) {
        for (j = 0; j < patlen; ++j) {
            if (me[i + j] != pattern[j]) {
                break;
            }
        }
        if (j == patlen) {
            return i;
        }
    }
    return SIZE_MAX;
}

/// @brief  Count the non-overlapping occurrences of a pattern.
/// @note   I haven't thought through this algorithm fully.
static inline size_t
cstr_count(char const *const me, char const *const pattern)
{
    if (me == NULL || pattern == NULL) {
        return SIZE_MAX;
    }
    // NOTE This is a special case when both strings are "". I follow
    //      Python's semantics.
    if (strcmp(pattern, "") == 0) {
        return strlen(me) == 0 ? 1 : strlen(me) + 1;
    }
    size_t cnt = 0, idx = 0;
    char const *tmp = me;
    while (true) {
        idx = cstr_find(tmp, pattern);
        if (idx == SIZE_MAX) {
            return cnt;
        }
        ++cnt;
        tmp += idx + strlen(pattern);
    }
}

/// @brief  Find and replace non-overlapping occurrences in a string.
static inline char *
cstr_replace(char const *const me,
             char const *const find,
             char const *const replace)
{
    if (me == NULL || find == NULL || replace == NULL) {
        return NULL;
    }
    size_t occurrences = cstr_count(me, find);
    ptrdiff_t diff = (strlen(replace) - strlen(find)) * occurrences;
    char *const dst = malloc(strlen(me) + diff + 1);
    char const *tmp_src = me;
    char *tmp_dst = dst;

    if (strlen(find) == 0) {
        for (size_t i = 0; i < occurrences; ++i) {
            strcpy(tmp_dst, replace);
            tmp_dst += strlen(replace);
            *tmp_dst = *tmp_src;
            tmp_dst += 1;
            tmp_src += 1;
        }
        return dst;
    }

    for (size_t i = 0; i < occurrences; ++i) {
        size_t src_len = cstr_find(tmp_src, find);
        assert(src_len != SIZE_MAX);
        // NOTE The result will not be null-terminated after strncpy
        //      unless we reach the end of the source string.
        strncpy(tmp_dst, tmp_src, src_len);
        tmp_src += src_len + strlen(find);
        tmp_dst += src_len;
        strcpy(tmp_dst, replace);
        tmp_dst += strlen(replace);
    }
    // NOTE We either will have the strlen(find) == 0 (which will always
    //      make us 'find' another occurrence) or we will be at the
    //      final occurrence.
    assert(strlen(find) == 0 || cstr_find(tmp_src, find) == SIZE_MAX);
    strcpy(tmp_dst, tmp_src);
    return dst;
}
