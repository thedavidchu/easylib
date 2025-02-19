#pragma once

#include <stdbool.h>

#include "global.h"

int
nothing_ctor(struct Object *me,
             struct Global const *const global,
             union ObjectData data);

int
nothing_dtor(struct Object *me);

int
nothing_cmp(struct Object const *me, struct Object const *other, int *result);

int
nothing_fprint(struct Object const *const me,
               FILE *const fp,
               bool const newline);

int
nothing_from_cstr(struct Object *const me,
                  struct Global const *const global,
                  char const *const cstr,
                  char const **cstr_end);
