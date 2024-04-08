#include <stdio.h>

#include "easycommon.h"
#include "easyboolean.h"

void EasyBoolean__print(enum EasyBoolean *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  EASY_GUARD(*me == FALSE || *me == TRUE, "EasyBoolean must be TRUE or FALSE");
  
  if (*me == FALSE) {
    printf("false");
  } else {
    printf("true");
  }
}

void EasyBoolean__print_json(enum EasyBoolean *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  EASY_GUARD(*me == FALSE || *me == TRUE, "EasyBoolean must be TRUE or FALSE");
  printf("{\"type\": \"EasyBoolean\", \".value\": \"%s\"}", *me == FALSE ? "false": "true");
}
