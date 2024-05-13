#include <stdbool.h>
#include <stdlib.h>

#include "easy_common.h"
#include "easy_equal.h"
#include "easy_hash.h"
#include "easy_lib.h"
#include "easy_table.h"
#include "easy_table_item.h"

static bool
is_last_element(size_t i, size_t length)
{
    return i == length - 1;
}

struct EasyTable
EasyTable__new_empty(void)
{
    struct EasyTable new_item = {.data = NULL, .length = 0, .capacity = 0};
    return new_item;
}

/// @brief  This function is to remind future developers to always check
///         for a hash table with zero capacity. This means that the
///         key cannot possibly be in the table nor can it be inserted.
static bool
is_capacity_zero(struct EasyTable const *const me)
{
    EASY_GUARD(me != NULL, "inputs must not be NULL");
    return me->capacity == 0;
}

static bool
is_enough_room(struct EasyTable const *const me)
{
    EASY_GUARD(me != NULL, "inputs must not be NULL");
    // NOTE According to Wikipedia, linear probing works very well until
    //      about a load factor of 0.7-0.8. Source:
    //      https://en.wikipedia.org/wiki/Hash_table
    return 0.7 * me->capacity > me->length;
}

struct IndexStatus {
    bool found;
    size_t idx;
};

/// @brief  Get the position of the object or return the next empty/tombstone
///         position if not found.
/// @note   This will panic if it receives a full hash table that does not
///         contain the element for which we're looking.
static struct IndexStatus
get_wouldbe_position(struct EasyTable const *const me,
                     struct EasyGenericObject const *const key,
                     uint64_t const hash)
{
    EASY_GUARD(me != NULL && key != NULL, "inputs must not be NULL");
    EASY_GUARD(me->capacity != 0, "division by zero");
    size_t home = hash % me->capacity;
    for (size_t i = 0; i < me->capacity; ++i) {
        size_t const idx = (home + i) % me->capacity;
        switch (me->data[idx].valid) {
        case EASY_TABLE_INVALID:
            return (struct IndexStatus){.found = false, .idx = idx};
        case EASY_TABLE_TOMBSTONE:
            continue;
        case EASY_TABLE_VALID:
            if (EasyGenericObject__equal(key, &me->data[idx].key)) {
                return (struct IndexStatus){.found = true, .idx = idx};
            }
            continue;
        default:
            EASY_IMPOSSIBLE();
        }
    }

    // The hash table is probably full!
    return (struct IndexStatus){.found = false, .idx = me->capacity};
}

static struct EasyTable
new_empty_table_with_capacity(uint64_t const capacity)
{
    struct EasyTable new_item = {
        .data = EASY_CALLOC(capacity, sizeof(*new_item.data)),
        .length = 0,
        .capacity = capacity};
    return new_item;
}

static struct EasyTable
resize_table(struct EasyTable const *const me, uint64_t const capacity)
{
    EASY_GUARD(capacity >= me->length, "capacity must be large enough to hold");

    EasyTable__print_json(me);
    printf("\n");

    struct EasyTable new_table = new_empty_table_with_capacity(capacity);
    EasyTable__print_json(&new_table);
    printf("\n");
    for (size_t i = 0; i < me->capacity; ++i) {
        if (me->data[i].valid == EASY_TABLE_VALID) {
            struct EasyGenericObject const *const key = &me->data[i].key;
            uint64_t const hash = me->data[i].hash;
            struct EasyGenericObject const *const value = &me->data[i].value;

            struct IndexStatus const status =
                get_wouldbe_position(&new_table, key, hash);
            EASY_ASSERT(is_enough_room(&new_table), "ran out of room");
            EASY_ASSERT(!status.found, "should not have found the key");
            new_table.data[status.idx] =
                (struct EasyTableItem){.valid = EASY_TABLE_VALID,
                                       .hash = hash,
                                       .key = EasyGenericObject__copy(key),
                                       .value = EasyGenericObject__copy(value)};
            ++new_table.length;
        }
    }

    return new_table;
}

struct EasyTable
EasyTable__insert(struct EasyTable const *const me,
                  struct EasyGenericObject const *const key,
                  struct EasyGenericObject const *const value)
{
    EASY_GUARD(me != NULL, "pointer must not be NULL");
    // me->capacity != 0 implies me->data != NULL ((not A) or B)
    EASY_GUARD(!(me->capacity != 0) || me->data != NULL, "invalid capacity");
    if (is_capacity_zero(me)) {
        struct EasyTable new_item = new_empty_table_with_capacity(1);
        new_item.data[0] =
            (struct EasyTableItem){.valid = EASY_TABLE_VALID,
                                   .hash = EasyGenericObject__hash(key),
                                   .key = EasyGenericObject__copy(key),
                                   .value = EasyGenericObject__copy(value)};
        ++new_item.length;
        return new_item;
    }
    uint64_t hash = EasyGenericObject__hash(key);
    struct IndexStatus const status = get_wouldbe_position(me, key, hash);
    if (status.found) {
        struct EasyTable new_item = EasyTable__copy(me);
        EasyGenericObject__destroy(&new_item.data[status.idx].value);
        new_item.data[status.idx].value = EasyGenericObject__copy(value);
        return new_item;
    } else if (is_enough_room(me)) {
        struct EasyTable new_item = EasyTable__copy(me);
        EASY_ASSERT(!status.found, "should not find key");
        EASY_ASSERT(status.idx != me->capacity, "should be enough room");
        new_item.data[status.idx] =
            (struct EasyTableItem){.valid = EASY_TABLE_VALID,
                                   .key = EasyGenericObject__copy(key),
                                   .hash = hash,
                                   .value = EasyGenericObject__copy(value)};
        ++new_item.length;
        return new_item;
    } else {
        struct EasyTable new_item = resize_table(me, 2 * me->capacity);
        struct IndexStatus const status =
            get_wouldbe_position(&new_item, key, hash);
        EASY_ASSERT(!status.found, "should not find key");
        new_item.data[status.idx] =
            (struct EasyTableItem){.valid = EASY_TABLE_VALID,
                                   .key = EasyGenericObject__copy(key),
                                   .hash = hash,
                                   .value = EasyGenericObject__copy(value)};
        ++new_item.length;
        return new_item;
    }
}

struct EasyGenericObject
EasyTable__lookup(struct EasyTable const *const me,
                  struct EasyGenericObject const *const key)
{
    (void)me;
    (void)key;
    EASY_NOT_IMPLEMENTED();
    return (struct EasyGenericObject){0};
}

bool
EasyTable__remove(struct EasyTable const *const me,
                  struct EasyGenericObject const *const key)
{
    (void)me;
    (void)key;
    EASY_NOT_IMPLEMENTED();
    return true;
}

struct EasyTable
EasyTable__copy(struct EasyTable const *const me)
{
    EASY_GUARD(me != NULL, "pointer must not be NULL");
    // me->capacity != 0 implies me->data != NULL ((not A) or B)
    EASY_GUARD(!(me->capacity != 0) || me->data != NULL, "invalid capacity");
    struct EasyTable new_item = {
        .data = EASY_MALLOC(me->capacity, sizeof(*me->data)),
        .length = me->length,
        .capacity = me->capacity};
    for (size_t i = 0; i < me->capacity; ++i) {
        if (me->data[i].valid == EASY_TABLE_VALID) {
            new_item.data[i] = (struct EasyTableItem){
                .valid = EASY_TABLE_VALID,
                .hash = me->data[i].hash,
                .key = EasyGenericObject__copy(&me->data[i].key),
                .value = EasyGenericObject__copy(&me->data[i].value)};
        } else {
            new_item.data[i] =
                (struct EasyTableItem){.valid = me->data[i].valid};
        }
    }
    return new_item;
}

void
EasyTable__print_json(struct EasyTable const *const me)
{
    EASY_GUARD(me != NULL, "pointer must not be NULL");
    // me->capacity != 0 implies me->data != NULL ((not A) or B)
    EASY_GUARD(!(me->capacity != 0) || me->data != NULL, "invalid capacity");
    printf("{\"type\": \"EasyTable\", \".length\": %zu, \".capacity\": %zu, "
           "\".data\": [",
           me->length,
           me->capacity);
    for (size_t i = 0; i < me->capacity; ++i) {
        if (me->data[i].valid == EASY_TABLE_VALID) {
            printf("{\".key\": ");
            EasyGenericObject__print_json(&me->data[i].key);
            printf(", \".value\": ");
            EasyGenericObject__print(&me->data[i].value);
            printf("}");
        } else {
            printf("null");
        }

        if (!is_last_element(i, me->capacity)) {
            printf(", ");
        }
    }
    printf("]}");
}

void
EasyTable__print(struct EasyTable const *const me)
{
    EASY_GUARD(me != NULL, "pointer must not be NULL");
    // me->capacity != 0 implies me->data != NULL ((not A) or B)
    EASY_GUARD(!(me->capacity != 0) || me->data != NULL, "invalid capacity");

    printf("{");
    size_t seen_elements = 0;
    for (size_t i = 0; i < me->capacity; ++i) {
        if (me->data[i].valid == EASY_TABLE_VALID) {
            EasyGenericObject__print(&me->data[i].key);
            printf(": ");
            EasyGenericObject__print(&me->data[i].value);
            ++seen_elements;
            // We know there are no more elements to be seen!
            if (seen_elements == me->length) {
                break;
            }
            printf(", ");
        }
    }
    printf("}");
}

void
EasyTable__destroy(struct EasyTable *const me)
{
    // NOTE   This excludes destroying empty tables.
    EASY_GUARD(me != NULL, "ptr must not be NULL");
    // me->capacity != 0 implies me->data != NULL ((not A) or B)
    EASY_GUARD(!(me->capacity != 0) || me->data != NULL, "invalid capacity");
    for (size_t i = 0; i < me->capacity; ++i) {
        if (me->data[i].valid == EASY_TABLE_VALID) {
            EasyGenericObject__destroy(&me->data[i].key);
            EasyGenericObject__destroy(&me->data[i].value);
        }
    }
    free(me->data);
    *me = (struct EasyTable){0};
}
