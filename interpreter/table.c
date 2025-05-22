#include <assert.h>
#include <bits/stdint-uintn.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "object.h"
#include "table.h"

static bool debug = true;

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define FILTER(func_call)                                                      \
    do {                                                                       \
        int err = (func_call);                                                 \
        if (err) {                                                             \
            return err;                                                        \
        }                                                                      \
    } while (0)

static uint64_t
hash(struct Object const *const x)
{
    uint64_t result = 0;
    if (debug) {
        result = (uint64_t)x;
    } else {
        int err = object_hash(x, &result);
        assert(!err);
    }
    return result;
}

static bool
ok(struct Table const *const me)
{
    bool const debug = true;
    if (me == NULL) {
        return false;
    }
    if (me->data == NULL && me->capacity != 0) {
        return false;
    }
    if (me->length > me->capacity) {
        return false;
    }
    // NOTE [EXPENSIVE] Check if the correct number of buckets are filled.
    if (debug) {
        size_t cnt = 0;
        for (size_t i = 0; i < me->capacity; ++i) {
            if (me->data[i].status == TABLE_NODE_VALID) {
                ++cnt;
            }
        }
        if (cnt != me->length) {
            return -1;
        }
    }
    // TODO [MAYBE?] Check for duplicates in the buckets.
    return true;
}

/// @brief  Get the index of the key or SIZE_MAX.
static size_t
get_index(struct Table const *const me, struct Object const *const key)
{
    size_t const h = hash(key);
    assert(me && me->data && me->capacity);
    for (size_t i = 0; i < me->capacity; ++i) {
        size_t idx = (h + i) % me->capacity;
        if (me->data[idx].status == TABLE_NODE_VALID &&
            me->data[idx].key == key) {
            return idx;
        }
        // This means that we will NOT find the item!
        if (me->data[idx].status == TABLE_NODE_INVALID) {
            break;
        }
    }
    return SIZE_MAX;
}

/// @brief  Get the would-be or actual index of the key or SIZE_MAX.
static size_t
get_maybe_index(struct Table const *const me, struct Object const *const key)
{
    size_t const h = hash(key);
    assert(me && me->data && me->capacity);
    for (size_t i = 0; i < me->capacity; ++i) {
        size_t idx = (h + i) % me->capacity;
        // This means we've found the key!
        // NOTE By placing this condition first, we optimize for updating an
        //      existing key. #premature-optimization!
        if (me->data[idx].status == TABLE_NODE_VALID &&
            me->data[idx].key == key) {
            return idx;
        }
        // NOTE We are assuming the status is one of the enumerated values.
        if (me->data[idx].status != TABLE_NODE_VALID) {
            return idx;
        }
    }
    return SIZE_MAX;
}

static int
grow(struct Table *const me)
{
    struct Table new_table = {0};
    if (!ok(me)) {
        return -1;
    }
    new_table.capacity = MAX(8, 2 * me->capacity);
    new_table.data = calloc(new_table.capacity, sizeof(*new_table.data));
    if (new_table.data == NULL) {
        return ENOMEM;
    }
    // Copy over data to new_table.
    for (size_t i = 0; i < me->capacity; ++i) {
        // NOTE We don't want to iterate was time searching an empty table!
        if (new_table.length == me->length) {
            break;
        }
        if (me->data[i].status == TABLE_NODE_VALID) {
            size_t idx = get_maybe_index(&new_table, me->data[i].key);
            assert(idx != SIZE_MAX);
            new_table.data[i] = (struct TableNode){
                .status = TABLE_NODE_VALID,
                .key = me->data[i].key,
                .value = me->data[i].value,
            };
            ++new_table.length;
        }
    }
    free(me->data);
    *me = new_table;
    return 0;
}

int
table_ctor(struct Table *const me)
{
    if (me == NULL) {
        return -1;
    }
    *me = (struct Table){0};
    return grow(me);
}

int
table_dtor(struct Table *const me)
{
    if (me == NULL) {
        return -1;
    }
    free(me->data);
    // TODO Destroy all objects.
    *me = (struct Table){0};
    return 0;
}

int
table_fprint(struct Table const *const me, FILE *const fp, bool const newline)
{
    size_t cnt = 0;
    if (me == NULL || me->data == NULL) {
        return -1;
    }
    fprintf(fp, "(len: %zu, cap: %zu) {", me->length, me->capacity);
    for (size_t i = 0; i < me->capacity; ++i) {
        if (me->data[i].status == TABLE_NODE_VALID) {
            ++cnt;
            if (debug) {
                fprintf(fp,
                        "%zu: %zu%s",
                        (size_t)me->data[i].key,
                        (size_t)me->data[i].value,
                        cnt < me->length ? ", " : "");
            } else {
                object_fprint(me->data[i].key, fp, false);
                fprintf(fp, ": ");
                object_fprint(me->data[i].value, fp, false);
                fprintf(fp, "%s", cnt < me->length ? ", " : "");
            }
        }
    }
    fprintf(fp, "}%s", newline ? "\n" : "");
    return 0;
}

int
table_insert(struct Table *const me,
             struct Object const *const key,
             struct Object *const value)
{
    size_t idx = 0;
    if (me == NULL || me->data == NULL) {
        return -1;
    }
    if (me->length >= (double)2 / 3 * me->capacity) {
        FILTER(grow(me));
    }
    idx = get_maybe_index(me, key);
    if (idx == SIZE_MAX) {
        return -1;
    }
    // Add to the length if this is a new object!
    if (me->data[idx].status != TABLE_NODE_VALID) {
        ++me->length;
    }
    me->data[idx] = (struct TableNode){
        .status = TABLE_NODE_VALID,
        .key = key,
        .value = value,
    };
    return 0;
}

int
table_get(struct Table *const me,
          struct Object const *const key,
          struct Object **const value)
{
    size_t idx = 0;
    if (me == NULL || me->data == NULL) {
        return -1;
    }
    if (value == NULL) {
        return -1;
    }
    idx = get_index(me, key);
    if (idx == SIZE_MAX) {
        return -1;
    }
    *value = me->data[idx].value;
    return 0;
}

int
table_remove(struct Table *const me,
             struct Object const *const key,
             struct Object **const value)
{
    size_t idx = 0;
    if (me == NULL || me->data == NULL) {
        return -1;
    }
    if (value == NULL) {
        return -1;
    }
    idx = get_index(me, key);
    if (idx == SIZE_MAX) {
        return -1;
    }
    *value = me->data[idx].value;
    me->data[idx] = (struct TableNode){.status = TABLE_NODE_TOMBSTONE,
                                       .key = 0,
                                       .value = 0};
    --me->length;
    return 0;
}
