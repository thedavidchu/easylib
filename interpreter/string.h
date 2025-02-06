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

#include <string.h>
#include <stdlib.h>
#include <stddef.h>

/// @brief My implementation of 'strdup', which is not guaranteed to exist.
char *
mystrdup(char const *const src)
{
    if (src == NULL) { return NULL; }
    size_t const len = strlen(src);
    char *dst = malloc(len + 1);
    return strcpy(dst, src);
}

char *
cstr_slice(char const *const src, size_t const start, size_t const end)
{
    if (src == NULL) { return NULL; }
    size_t const len = strlen(src);
    // It is legal to ask for a slice from the very last character
    // For example, "0123"[4:] would return "".
    if (start > len) { return NULL; }
    // TODO - reverse slicing is not implemented yet!
    if (start > end) { return NULL; }
    size_t const slice_len = end - start; 
    char *dst = malloc(slice_len + 1);
    return strncpy(dst, &src[start], slice_len);
}

static int
string_error(struct Object const *const me)
{
    if (me == NULL || me->type == NULL || me->type->type != OBJECT_TYPE_STRING) { return -1; }
    if (me->data.string == NULL) { return -1; }
    return 0;
}

int
string_ctor(struct Object *const me,
            struct ObjectType const *const type,
            union ObjectData const data)
{
    if (me == NULL || type == NULL || data.string == NULL) { return -1; }
    me->type = type;
    me->data = data;
    return 0;
}

int
string_dtor(struct Object *const me)
{
    int err = 0;
    if ((err = string_error(me))) { return err; }
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
    if ((err = string_error(me))) { return err; }
    if (other == NULL || result == NULL) { return -1; }
    if (me->type->type != OBJECT_TYPE_STRING) { return -1; }
    if (me == other) { *result = 0; return 0; }
    *result = strcmp(me->data.string, other->data.string);
    return 0;
}

int
string_fprint(struct Object const *const me, FILE *const fp, bool const newline)
{
    int err = 0;
    if ((err = string_error(me))) { return err; }
    // TODO Handle errors in fprintf(...).
    fprintf(fp, "%s%s", me->data.string, newline ? "\n" : "");
    return 0;
}

int
string_len(struct Object const *const me, size_t *const len)
{
    int err = 0;
    if ((err = string_error(me))) { return err; }
    *len = strlen(me->data.string);
    return 0;
}

int
string_slice(struct Object const *const me, size_t const start, size_t const end, struct Object *const result)
{
    int err = 0;
    if ((err = string_error(me))) { return err; }
    char *slice = cstr_slice(me->data.string, start, end);
    if (slice == NULL) { return -1; }
    return string_ctor(result, me->type, (union ObjectData){.string = slice});
}

