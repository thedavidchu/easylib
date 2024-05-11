#pragma once

#include <stddef.h>

struct EasyGenericObject;

struct EasyList {
  struct EasyGenericObject *data;
  size_t length;
};

struct EasyList EasyList__new_empty();
struct EasyList EasyList__append(struct EasyList const *const me,
                                 struct EasyGenericObject const *const obj);
struct EasyGenericObject EasyList__lookup(struct EasyList const *const me,
                                          const size_t index);
struct EasyList EasyList__remove(struct EasyList const *const me,
                                 const size_t index);
struct EasyList EasyList__copy(struct EasyList const *const me);
void EasyList__destroy(struct EasyList *const me);

void EasyList__print(struct EasyList const *const me);
void EasyList__print_json(struct EasyList const *const me);
