#include <stdio.h>

#include "easycommon.h"
#include "easynothing.h"

EasyNothing EasyNothing__new() { return NOTHING; }

void EasyNothing__print(EasyNothing const *const me) {
  (void)me;
  printf("null");
}

void EasyNothing__print_json(EasyNothing const *const me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  EASY_GUARD(*me != NULL, "pointer must be NULL");
  (void)me;
  printf("{\"type\": \"EasyNothing\", \"data\": \"%p\"}", *me);
}

EasyNothing EasyNothing__copy(EasyNothing const *const me) {
  EASY_GUARD(me != NULL, "ptr must not be NULL");
  EASY_GUARD(*me == NOTHING, "EasyNothing must be NOTHING");
  return *me;
}

void EasyNothing__destroy(EasyNothing *const me) {
  EASY_GUARD(me != NULL, "ptr must not be NULL");
  EASY_GUARD(*me == NOTHING, "EasyNothing must be NOTHING");
  *me = NOTHING;
}