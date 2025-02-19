#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "global.h"
#include "object.h"

static inline char const *
bool_stringify(bool const x)
{
    return x ? "true" : "false";
}

int
boolean_ctor(struct Object *me,
             struct Global const *const global,
             union ObjectData data);

int
boolean_from_cstr(struct Object *const me,
                  struct Global const *const global,
                  char const *const cstr,
                  char const **cstr_end);

int
boolean_dtor(struct Object *me);

int
boolean_cmp(struct Object const *me, struct Object const *other, int *result);

int
boolean_fprint(struct Object const *const me,
               FILE *const fp,
               bool const newline);

int
boolean_not(struct Object const *const me, bool *const result);

int
boolean_and(struct Object const *const me,
            struct Object const *const other,
            bool *const result);

int
boolean_or(struct Object const *const me,
           struct Object const *const other,
           bool *const result);

int
boolean_truthiness(struct Object const *const me, bool *const result);
