#pragma once

#include <stddef.h>

struct EasyGenericObject;

struct EasyList {
  struct EasyGenericObject *data;
  size_t length;
};

struct EasyList EasyList__new_empty();
struct EasyList EasyList__append(struct EasyList *me,
                                 struct EasyGenericObject *obj);
struct EasyList EasyList__copy(struct EasyList *me);
void EasyList__destroy(struct EasyList *me);

void EasyList__print(struct EasyList *me);
void EasyList__print_json(struct EasyList *me);
