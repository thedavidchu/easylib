#include <stdbool.h>
#include <stdio.h>

#include "easycommon.h"
#include "easylib.h"
#include "easylist.h"

static bool is_last_element(size_t i, size_t length) { return i == length - 1; }

struct EasyList EasyList__new_empty() {
  return (struct EasyList){.data = EASY_MALLOC(0, 0), .length = 0};
}

struct EasyList EasyList__append(struct EasyList *me,
                                 struct EasyGenericObject *obj) {
  struct EasyList new_item = {
      .data = EASY_CALLOC(me->length + 1, sizeof(*me->data)),
      .length = me->length + 1};
  /* Recursively copy data */
  for (size_t i = 0; i < me->length; ++i) {
    new_item.data[i] = EasyGenericObject__copy(&me->data[i]);
  }
  new_item.data[me->length] = *obj;
  return new_item;
}

struct EasyList EasyList__copy(struct EasyList *me) {
  struct EasyList new_item = {
      .data = EASY_CALLOC(me->length, sizeof(*me->data)),
      .length = me->length};
  /* Recursively copy data */
  for (size_t i = 0; i < me->length; ++i) {
    new_item.data[i] = EasyGenericObject__copy(&me->data[i]);
  }
  return new_item;
}

void EasyList__destroy(struct EasyList *me) {
    EASY_GUARD(me != NULL && me->data != NULL, "ptr must not be NULL");
    for (size_t i = 0; i < me->length; ++i) {
        EasyGenericObject__destroy(&me->data[i]);
    }
    EASY_FREE(me->data);
    *me = (struct EasyList){0};
}

void EasyList__print(struct EasyList *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  EASY_ASSERT(me->data != NULL, "me->data should not be NULL");
  printf("[");
  for (size_t i = 0; i < me->length; ++i) {
    EasyGenericObject__print(&me->data[i]);
    if (!is_last_element(i, me->length)) {
      printf(", ");
    }
  }
  printf("]");
}

void EasyList__print_json(struct EasyList *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  EASY_ASSERT(me->data != NULL, "me->data should not be NULL");
  printf("{\"type\": \"EasyList\", \".length\": %zu, \".data\": [", me->length);

  for (size_t i = 0; i < me->length; ++i) {
    EasyGenericObject__print_json(&me->data[i]);
    if (!is_last_element(i, me->length)) {
      printf(", ");
    }
  }
  printf("]}");
}