/** Pointer array library.
 * */
#pragma once

#include "object.h"
#include <stdbool.h>
#include <stddef.h>

struct Array {
    struct Object **array;
    size_t length;
    size_t capacity;
};

int
array_ctor(struct Array *const me);

int
array_dtor(struct Array *const me);

int
array_fprint(struct Array const *const me, FILE *const fp, bool newline);

/// @brief  Insert an item into the array and give ownership to the array.
int
array_insert(struct Array *const me,
             size_t const idx,
             struct Object *const item);

/// @brief  Get a reference to the item at idx (the array still owns it).
int
array_get(struct Array *const me,
          size_t const idx,
          struct Object **const result);

/// @brief  Remove the victim at `idx` and return.
/// @note   The caller is responsible for freeing the resources of the victim.
int
array_remove(struct Array *const me,
             size_t const idx,
             struct Object **const victim);
