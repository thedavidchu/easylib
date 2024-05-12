#include <stdbool.h>
#include <stdlib.h>

#include "easy_common.h"
#include "easy_lib.h"

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

struct EasyTable
EasyTable__insert(struct EasyTable const *const me,
                  struct EasyGenericObject const *const key,
                  struct EasyGenericObject const *const value)
{
    (void)me;
    (void)key;
    (void)value;
    EASY_NOT_IMPLEMENTED();
    return (struct EasyTable){0};
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
    struct EasyTable new_item = {
        .data = EASY_MALLOC(me->capacity, sizeof(*me->data)),
        .length = me->length,
        .capacity = me->capacity};
    for (size_t i = 0; i < me->capacity; ++i) {
        if (me->data[i].valid) {
            new_item.data[i] = (struct EasyTableItem){
                .valid = true,
                .key = EasyGenericObject__copy(&me->data[i].key),
                .value = EasyGenericObject__copy(&me->data[i].value)};
        } else {
            new_item.data[i] = (struct EasyTableItem){.valid = false};
        }
    }
    return new_item;
}

void
EasyTable__print_json(struct EasyTable const *const me)
{
    EASY_GUARD(me != NULL, "pointer must not be NULL");
    printf("{\"type\": \"EasyTable\", \".length\": %zu, \".capacity\": %zu, "
           "\".data\": [",
           me->length,
           me->capacity);
    for (size_t i = 0; i < me->capacity; ++i) {
        if (me->data[i].valid) {
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
    EASY_GUARD(me->data == NULL && me->capacity != 0, "invalid capacity");

    printf("{");
    size_t seen_elements = 0;
    for (size_t i = 0; i < me->capacity; ++i) {
        if (me->data[i].valid) {
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
    EASY_GUARD(me->data == NULL && me->capacity != 0, "invalid capacity");
    for (size_t i = 0; i < me->capacity; ++i) {
        if (me->data[i].valid) {
            EasyGenericObject__destroy(&me->data[i].key);
            EasyGenericObject__destroy(&me->data[i].value);
        }
    }
    free(me->data);
    *me = (struct EasyTable){0};
}
