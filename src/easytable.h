#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "easylib.h"

struct EasyTableItem;

/* EasyTable */
struct EasyTable {
  struct EasyTableItem *data;
  size_t length;   /* The number of elements in the EasyTable */
  size_t capacity; /* The maximum number of elements in the EasyTable */
};

struct EasyTable EasyTable__new_empty();
struct EasyTable EasyTable__insert(struct EasyTable *me,
                                   struct EasyGenericObject *key,
                                   struct EasyGenericObject *value);
struct EasyGenericObject EasyTable__lookup(struct EasyTable *me,
                                           struct EasyGenericObject *key);
bool EasyTable__remove(struct EasyTable *me, struct EasyGenericObject *key);
struct EasyTable EasyTable__copy(struct EasyTable const *const me);
void EasyTable__destroy(struct EasyTable *me);

void EasyTable__print_json(struct EasyTable *me);
void EasyTable__print(struct EasyTable *me);
