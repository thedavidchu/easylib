#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "global.h"

static int
nothing_error(struct Object const *const me)
{
    if (me == NULL) {
        return -1;
    }
    if (me->type == NULL) {
        return -1;
    }
    if (me->type->type != OBJECT_TYPE_NOTHING) {
        return -1;
    }
    if (me->data.nothing != NULL) {
        return -1;
    }
    return 0;
}

int
nothing_ctor(struct Object *me,
             struct Global const *const global,
             union ObjectData data)
{
    if (me == NULL || global == NULL) {
        return -1;
    }
    me->global = global;
    me->type = &global->builtin_types.nothing;
    // TODO Check if data is valid.
    me->data = data;
    return 0;
}

int
nothing_dtor(struct Object *me)
{
    int err = 0;
    if ((err = nothing_error(me))) {
        return err;
    }
    *me = (struct Object){0};
    return 0;
}

int
nothing_cmp(struct Object const *me, struct Object const *other, int *result)
{
    int err = 0;
    if ((err = nothing_error(me))) {
        return err;
    }
    if (me == NULL || other == NULL || result == NULL) {
        return -1;
    }
    if (me->type->type != OBJECT_TYPE_NOTHING) {
        return -1;
    }
    if (me == other) {
        *result = 0;
        return 0;
    }
    if (other->type->type == OBJECT_TYPE_NOTHING) {
        *result = 1;
        return 0;
    }
    *result = 3;
    return 0;
}

int
nothing_fprint(struct Object const *const me,
               FILE *const fp,
               bool const newline)
{
    int err = 0;
    if ((err = nothing_error(me))) {
        return err;
    }
    fprintf(fp, "Nothing%s", newline ? "\n" : "");
    return 0;
}

int
nothing_from_cstr(struct Object *const me,
                  struct Global const *const global,
                  char const *const cstr,
                  char const **cstr_end)
{
    if (me == NULL || global == NULL || cstr == NULL || cstr_end == NULL) {
        return -1;
    }
    me->global = global;
    me->type = &global->builtin_types.nothing;
    if (strncmp(cstr, "null", sizeof("null")) == 0) {
        me->data.nothing = NULL;
        *cstr_end = &cstr[sizeof("null")];
    } else {
        *cstr_end = cstr;
        return -1;
    }
    return 0;
}
