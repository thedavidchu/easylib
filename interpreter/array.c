#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "array.h"

#define FILTER(func_call) \
    do { \
        int err = (func_call); \
        if (err) { return err; } \
    } while (0)

static size_t const DEFAULT_ARRAY_CAPACITY = 8;
static double const DEFAULT_GROWTH_FACTOR = 2;

static bool
ok(struct Array const *const me)
{
    if (me == NULL) { return false; }
    if (me->array == NULL) { return me->capacity == 0; }
    if (me->capacity < me->length) { return false; }
    return true;
}


static int
resize(struct Array *const me, size_t const new_capacity)
{
    if (me == NULL) { return -1; }
    // Assert data is not corrupted.
    assert(ok(me));
    // We do not allow the user to destroy data through resizing.
    if (me->length > new_capacity) { return -1; }
    // TODO Ensure no overflow in multiplication.
    void **new_array = realloc(me->array, sizeof(*me->array) * new_capacity);
    if (new_capacity != 0 && new_array == NULL) { return -1; }
    me->array = new_array;
    me->capacity = new_capacity;
    return 0;
}

static int
grow(struct Array *const me)
{
    if (me == NULL) { return -1; }
    if (me->capacity == 0) {
        return resize(me, DEFAULT_ARRAY_CAPACITY);
    }
    // NOTE This may not be efficient, but it is effective in ensuring growth.
    return resize(me, (me->capacity + 1) * DEFAULT_GROWTH_FACTOR);
}

/// @brief  Shift elements to the right by 1 to make a hole.
static int
shift_right(struct Array *const me, size_t const hole_idx)
{
    assert(hole_idx <= me->length);
    assert(me->length + 1 <= me->capacity);
    // TODO Check for multiplication overflow.
    size_t const nbytes = sizeof(*me->array) * (me->length - hole_idx);
    // TODO Check return value.
    memmove(&me->array[hole_idx + 1], &me->array[hole_idx], nbytes);
    // NOTE This should be redundant, but it's good hygiene.
    me->array[hole_idx] = NULL;
    return 0;
}

/// @brief  Shift elements to the left by 1.
///         i.e. the element at `start_idx` will disappear.
/// @example
///         1. shift_left([0, 1, 2], 0) -> [1, 2]
///         2. shift_left([0, 1, 2], 1) -> [0, 2]
///         3. shift_left([0, 1, 2], 2) -> [0, 1]
static int
shift_left(struct Array *const me, size_t const victim_idx)
{
    assert(victim_idx < me->length);
    assert(me->length >= 1);
    // TODO Check for multiplication overflow.
    size_t const nbytes = sizeof(*me->array) * (me->length - 1 - victim_idx);
    // TODO Check return value.
    memmove(&me->array[victim_idx], &me->array[victim_idx + 1], nbytes);
    return 0;
}


int
array_ctor(struct Array *const me)
{
    if (me == NULL) { return -1; }
    *me = (struct Array){0};
    assert(ok(me));
    return grow(me);
}

int
array_dtor(struct Array *const me)
{
    if (me == NULL) { return -1; }
    assert(ok(me));
    // TODO Destroy elements in this array recursively.
    free(me->array);
    *me = (struct Array){0};
    return 0;
}

int
array_write(struct Array const *const me, FILE *const stream, bool newline)
{
    size_t i = 0;
    fprintf(stream, "(len: %zu, cap: %zu) [", me->length, me->capacity);
    for (i = 0; i < me->length; ++i) {
        fprintf(stream, "%p%s", me->array[i], i + 1 < me->length ? ", " : "");
    }
    fprintf(stream, "]%s", newline ? "\n" : "");
    return 0;
}

int
array_insert(struct Array *const me, size_t const idx, void *const item)
{
    if (me == NULL || me->array == NULL) { return -1; }
    assert(ok(me));
    // Check for out of bounds access.
    if (idx > me->length) { return -1; }
    if (me->length + 1 > me->capacity) {
        FILTER(grow(me));
    }
    FILTER(shift_right(me, idx));
    me->array[idx] = item;
    ++me->length;
    return 0;
} 

int
array_get(struct Array *const me, size_t const idx, void **const result)
{
    if (me == NULL || me->array == NULL) { return -1; }
    assert(ok(me));
    // Check for out of bounds access.
    if (idx >= me->length) { return -1; }
    // Return the result.
    if (result == NULL) { return -1; }
    *result = me->array[idx];
    return 0;
}

int
array_remove(struct Array *const me, size_t const idx, void **const victim)
{
    if (me == NULL || me->array == NULL) { return -1; }
    assert(ok(me));
    // Check for out of bounds access.
    if (idx >= me->length) { return -1; }
    // Return the victim for deletion.
    if (victim == NULL) { return -1; }
    *victim = me->array[idx];
    // TODO Shrink if necessary.
    FILTER(shift_left(me, idx));
    --me->length;
    return 0;
} 

