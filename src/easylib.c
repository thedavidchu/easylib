#include "easylib.h"
#include "easyboolean.h"
#include "easycommon.h"
#include "easyinteger.h"


void EasyGenericType__print_json(enum EasyGenericType *me) {
  switch (*me) {
  case EASY_TABLE_TYPE:
    printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_TABLE_TYPE\", "
           "\"number\": %d}",
           *me);
    break;
  case EASY_LIST_TYPE:
    printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_LIST_TYPE\", "
           "\"number\": %d}",
           *me);
    printf("EASY_LIST_TYPE");
    break;
  case EASY_TEXT_TYPE:
    printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_TEXT_TYPE\", "
           "\"number\": %d}",
           *me);
    printf("EASY_TEXT_TYPE");
    break;
  case EASY_INTEGER_TYPE:
    printf("EASY_INTEGER_TYPE");
    printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_INTEGER_TYPE\", "
           "\"number\": %d}",
           *me);
    break;
  case EASY_FRACTION_TYPE:
    printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_FRACTION_TYPE\", "
           "\"number\": %d}",
           *me);
    printf("EASY_FRACTION_TYPE");
    break;
  case EASY_BOOLEAN_TYPE:
    printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_BOOLEAN_TYPE\", "
           "\"number\": %d}",
           *me);
    printf("EASY_BOOLEAN_TYPE");
    break;
  case EASY_NOTHING_TYPE:
    printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_NOTHING_TYPE\", "
           "\"number\": %d}",
           *me);
    break;
  default:
    EASY_IMPOSSIBLE();
  }
}

void EasyTable__print_json(struct EasyTable *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  printf("{\"type\": \"EasyTable\", ...}");
}

void EasyFraction__print_json(struct EasyFraction *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  printf("{\"type\": \"EasyFraction\", ...}");
}

void EasyNothing__print_json(EasyNothing *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  EASY_GUARD(*me != NULL, "pointer must be NULL");
  printf("{\"type\": \"EasyNothing\", \"data\": \"%p\"}", *me);
}

void EasyGenericObject__print_json(struct EasyGenericObject *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  printf("{\"type\": \"EasyGenericObject\", \".type\": ");
  EasyGenericType__print_json(&me->type);
  printf(", \".data\": ");
  switch (me->type) {
  case EASY_TABLE_TYPE:
    EasyTable__print_json(&me->data.table);
    break;
  case EASY_LIST_TYPE:
    EasyList__print_json(&me->data.list);
    break;
  case EASY_TEXT_TYPE:
    EasyText__print_json(&me->data.text);
    break;
  case EASY_INTEGER_TYPE:
    EasyInteger__print_json(&me->data.integer);
    break;
  case EASY_FRACTION_TYPE:
    EasyFraction__print_json(&me->data.fraction);
    break;
  case EASY_BOOLEAN_TYPE:
    EasyBoolean__print_json(&me->data.boolean);
    break;
  case EASY_NOTHING_TYPE:
    EasyNothing__print_json(&me->data.nothing);
    break;
  default:
    EASY_IMPOSSIBLE();
  }
  printf("}");
}

void EasyGenericObject__print(struct EasyGenericObject *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  switch (me->type) {
  case EASY_TABLE_TYPE:
    // EasyTable__print(&me->data.table);
    break;
  case EASY_LIST_TYPE:
    // EasyList__print(&me->data.list);
    break;
  case EASY_TEXT_TYPE:
    EasyText__print(&me->data.text);
    break;
  case EASY_INTEGER_TYPE:
    EasyInteger__print(&me->data.integer);
    break;
  case EASY_FRACTION_TYPE:
    // EasyFraction__print(&me->data.fraction);
    // break;
  case EASY_BOOLEAN_TYPE:
    EasyBoolean__print(&me->data.boolean);
    break;
  case EASY_NOTHING_TYPE:
    // EasyNothing__print(&me->data.nothing);
    break;
  default:
    EASY_IMPOSSIBLE();
  }
}

struct EasyTable EasyTable__copy(struct EasyTable *me) {
  (void)me;
  return (struct EasyTable){0};
}

struct EasyText EasyText__copy(struct EasyText *me) {
  EASY_GUARD(me != NULL && me->data != NULL, "ptr must not be NULL");
  return (struct EasyText){
      .data = EASY_DUPLICATE(&me->data, me->length, sizeof(*me->data)),
      .length = me->length};
}

struct EasyFraction EasyFraction__copy(struct EasyFraction *me) {
  EASY_GUARD(me != NULL, "ptr must not be NULL");
  return (struct EasyFraction){.numerator = EasyInteger__copy(&me->numerator),
                               .denominator =
                                   EasyInteger__copy(&me->denominator)};
}

enum EasyBoolean EasyBoolean__copy(enum EasyBoolean *me) {
  EASY_GUARD(me != NULL && (*me == TRUE || *me == FALSE), "ptr must not be NULL");
  return *me;
}

EasyNothing EasyNothing__copy(EasyNothing *me) {
  EASY_GUARD(me != NULL, "ptr must not be NULL");
  EASY_GUARD(*me == NOTHING, "EasyNothing must be NOTHING");
  return *me;
}

struct EasyGenericObject EasyGenericObject__copy(struct EasyGenericObject *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  switch (me->type) {
  case EASY_TABLE_TYPE:
    return (struct EasyGenericObject){
        .type = EASY_TABLE_TYPE,
        .data = {.table = EasyTable__copy(&me->data.table)}};
  case EASY_LIST_TYPE:
    return (struct EasyGenericObject){
        .type = EASY_LIST_TYPE,
        .data = {.list = EasyList__copy(&me->data.list)}};
  case EASY_TEXT_TYPE:
    return (struct EasyGenericObject){
        .type = EASY_TEXT_TYPE,
        .data = {.text = EasyText__copy(&me->data.text)}};
  case EASY_INTEGER_TYPE:
    return (struct EasyGenericObject){
        .type = EASY_INTEGER_TYPE,
        .data = {.integer = EasyInteger__copy(&me->data.integer)}};
  case EASY_FRACTION_TYPE:
    return (struct EasyGenericObject){
        .type = EASY_FRACTION_TYPE,
        .data = {.fraction = EasyFraction__copy(&me->data.fraction)}};
  case EASY_BOOLEAN_TYPE:
    return (struct EasyGenericObject){
        .type = EASY_BOOLEAN_TYPE,
        .data = {.boolean = EasyBoolean__copy(&me->data.boolean)}};
  case EASY_NOTHING_TYPE:
    return (struct EasyGenericObject){
        .type = EASY_NOTHING_TYPE,
        .data = {.nothing = EasyNothing__copy(&me->data.nothing)}};
  default:
    EASY_IMPOSSIBLE();
  }
  /* Impossible... but compiler doesn't realize! */
  return (struct EasyGenericObject){0};
}

void EasyTable__destroy(struct EasyTable *me) {
    /* TODO */
    (void)me;
    EASY_ASSERT(0, "TODO");
    return;
}

void EasyFraction__destroy(struct EasyFraction *me) {
    EasyInteger__destroy(&me->numerator);
    EasyInteger__destroy(&me->denominator);
    *me = (struct EasyFraction){0};
}

void EasyBoolean__destroy(enum EasyBoolean *me) {
  EASY_GUARD(me != NULL && (*me == TRUE || *me == FALSE), "ptr must not be NULL");
  *me = FALSE;
}

void EasyNothing__destroy(EasyNothing *me) {
  EASY_GUARD(me != NULL, "ptr must not be NULL");
  EASY_GUARD(*me == NOTHING, "EasyNothing must be NOTHING");
  *me = NOTHING;
}

void EasyGenericObject__destroy(struct EasyGenericObject *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  switch (me->type) {
  case EASY_TABLE_TYPE:
    EasyTable__destroy(&me->data.table);
    break;
  case EASY_LIST_TYPE:
    EasyList__destroy(&me->data.list);
    break;
  case EASY_TEXT_TYPE:
    EasyText__destroy(&me->data.text);
    break;
  case EASY_INTEGER_TYPE:
    EasyInteger__destroy(&me->data.integer);
    break;
  case EASY_FRACTION_TYPE:
    EasyFraction__destroy(&me->data.fraction);
    break;
  case EASY_BOOLEAN_TYPE:
    EasyBoolean__destroy(&me->data.boolean);
    break;
  case EASY_NOTHING_TYPE:
    EasyNothing__destroy(&me->data.nothing);
    break;
  default:
    EASY_IMPOSSIBLE();
  }
}