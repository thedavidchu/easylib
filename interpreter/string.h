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
#include "global.h"
#include "object.h"

int
string_ctor(struct Object *const me,
            struct Global const *const global,
            union ObjectData const data);

int
string_dtor(struct Object *const me);

int
string_cmp(struct Object const *const me,
           struct Object const *const other,
           int *const result);

int
string_fprint(struct Object const *const me,
              FILE *const fp,
              bool const newline);

int
string_from_cstr(struct Object *const me,
                 struct Global const *const global,
                 char const *const cstr,
                 char const **cstr_end);

int
string_len(struct Object const *const me, size_t *const len);

int
string_slice(struct Object const *const me,
             size_t const start,
             size_t const end,
             struct Object *const result);
