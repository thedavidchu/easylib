#pragma once

#include <stdbool.h>
#include <stddef.h>

struct EasyGenericObject;
struct EasyTableItem;

/* EasyTable */
struct EasyTable {
    struct EasyTableItem *data;
    size_t length;   /* The number of elements in the EasyTable */
    size_t capacity; /* The maximum number of elements in the EasyTable */
};

struct EasyTable
EasyTable__new_empty(void);
struct EasyTable
EasyTable__insert(struct EasyTable const *const me,
                  struct EasyGenericObject const *const key,
                  struct EasyGenericObject const *const value);
struct EasyGenericObject
EasyTable__lookup(struct EasyTable const *const me,
                  struct EasyGenericObject const *const key);
struct EasyTable
EasyTable__remove(struct EasyTable const *const me,
                  struct EasyGenericObject const *const key);
struct EasyTable
EasyTable__copy(struct EasyTable const *const me);
void
EasyTable__destroy(struct EasyTable *const me);

void
EasyTable__print_json(struct EasyTable const *const me);
void
EasyTable__print(struct EasyTable const *const me);
