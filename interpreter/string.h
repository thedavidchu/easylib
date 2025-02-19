/**
 *  String Operations
 *  -----------------
 *  - Slice
 *
 *  Note
 *  ----
 *  - This conflicts with the <string.h> library.
 **/

#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "cstr.h"
#include "object.h"

static int
string_error(struct Object const *const me)
{
    if (me == NULL || me->type == NULL ||
        me->type->type != OBJECT_TYPE_STRING) {
        return -1;
    }
    if (me->data.string == NULL) {
        return -1;
    }
    return 0;
}

int
string_ctor(struct Object *const me,
            struct ObjectType const *const type,
            union ObjectData const data)
{
    if (me == NULL || type == NULL || data.string == NULL) {
        return -1;
    }
    me->type = type;
    me->data = data;
    return 0;
}

int
string_dtor(struct Object *const me)
{
    int err = 0;
    if ((err = string_error(me))) {
        return err;
    }
    free(me->data.string);
    *me = (struct Object){0};
    return 0;
}

int
string_cmp(struct Object const *const me,
           struct Object const *const other,
           int *const result)
{
    int err = 0;
    if ((err = string_error(me))) {
        return err;
    }
    if (other == NULL || result == NULL) {
        return -1;
    }
    if (me->type->type != OBJECT_TYPE_STRING) {
        return -1;
    }
    if (me == other) {
        *result = 0;
        return 0;
    }
    *result = strcmp(me->data.string, other->data.string);
    return 0;
}

int
string_fprint(struct Object const *const me, FILE *const fp, bool const newline)
{
    int err = 0;
    if ((err = string_error(me))) {
        return err;
    }
    // TODO Handle errors in fprintf(...).
    fprintf(fp, "%s%s", me->data.string, newline ? "\n" : "");
    return 0;
}

static int
count_parsed_cstr_length(char const *const cstr,
                         size_t *length,
                         char const **cstr_end)
{
    if (cstr == NULL || cstr_end == NULL) {
        return -1;
    }
    if (length == NULL || *cstr != '"') {
        goto cleanup_error;
    }
    char const *tmp = cstr + 1;
    size_t len = 0;

    while (true) {
        switch (*tmp) {
        case '"':
            // Finish up!
            *length = len;
            *cstr_end = tmp + 1;
            return 0;
        case '\\':
            ++tmp;
            switch (*tmp) {
            case '"':
                break;
            case '\'':
                break;
            case '\\':
                break;
            case '/':
                break;
            case 'a':
                break;
            case 'b':
                break;
            case 'f':
                break;
            case 'n':
                break;
            case 'r':
                break;
            case 't':
                break;
            case 'v':
                break;
            default:
                goto cleanup_error;
            }
            ++len;
            ++tmp;
            break;
        case '\0':
            goto cleanup_error;
        default:
            ++len;
            ++tmp;
            break;
        }
    }
cleanup_error:
    *cstr_end = cstr;
    return -1;
}

static void
copy_parsed_cstr(char *const dst, char const *const src)
{
    assert(dst != NULL && src != NULL);
    assert(*src == '"');
    char const *tmp_src = src + 1;
    char *tmp_dst = dst;
    while (true) {
        switch (*tmp_src) {
        case '"':
            *tmp_dst = '\0';
            return;
        case '\\':
            ++tmp_src;
            switch (*tmp_src) {
            case '"':
                *tmp_dst = '"';
                break;
            case '\'':
                *tmp_dst = '\'';
                break;
            case '\\':
                *tmp_dst = '\\';
                break;
            case '/':
                *tmp_dst = '/';
                break;
            case 'a':
                *tmp_dst = '\a';
                break;
            case 'b':
                *tmp_dst = '\b';
                break;
            case 'f':
                *tmp_dst = '\f';
                break;
            case 'n':
                *tmp_dst = '\n';
                break;
            case 'r':
                *tmp_dst = '\r';
                break;
            case 't':
                *tmp_dst = '\t';
                break;
            case 'v':
                *tmp_dst = '\v';
                break;
            default:
                assert(0 && "bad escaped character");
            }
            ++tmp_dst;
            ++tmp_src;
        default:
            assert(*tmp_src != '\0');
            *tmp_dst = *tmp_src;
            ++tmp_dst;
            ++tmp_src;
        }
    }
}

int
string_from_cstr(struct Object *const me,
                 struct ObjectType const *const type,
                 char const *const cstr,
                 char const **cstr_end)
{
    if (me == NULL || type == NULL || cstr == NULL || cstr_end == NULL) {
        return -1;
    }
    me->type = type;
    size_t length = 0;
    int err = count_parsed_cstr_length(cstr, &length, cstr_end);
    if (err) {
        return -1;
    }
    me->data.string = malloc(length + 1);
    copy_parsed_cstr(me->data.string, cstr);
    return 0;
}

int
string_len(struct Object const *const me, size_t *const len)
{
    int err = 0;
    if ((err = string_error(me))) {
        return err;
    }
    *len = strlen(me->data.string);
    return 0;
}

int
string_slice(struct Object const *const me,
             size_t const start,
             size_t const end,
             struct Object *const result)
{
    int err = 0;
    if ((err = string_error(me))) {
        return err;
    }
    char *slice = cstr_slice(me->data.string, start, end);
    if (slice == NULL) {
        return -1;
    }
    return string_ctor(result, me->type, (union ObjectData){.string = slice});
}
