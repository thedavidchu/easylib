#pragma once

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>

#include "global.h"
#include "object.h"

int
number_ctor(struct Object *me,
            struct Global const *const global,
            union ObjectData data);

int
number_dtor(struct Object *me);

int
number_cmp(struct Object const *me, struct Object const *other, int *result);

int
number_fprint(struct Object const *const me,
              FILE *const fp,
              bool const newline);

int
number_from_cstr(struct Object *const me,
                 struct Global const *const global,
                 char const *const cstr,
                 char const **cstr_end);

int
number_add(struct Object const *const me,
           struct Object const *const other,
           struct Object *const result);

int
number_sub(struct Object const *const me,
           struct Object const *const other,
           struct Object *const result);

int
number_mul(struct Object const *const me,
           struct Object const *const other,
           struct Object *const result);

int
number_div(struct Object const *const me,
           struct Object const *const other,
           struct Object *const result);
