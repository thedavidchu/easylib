/* EasyBoolean */
#pragma once

enum EasyBoolean {
  FALSE = 0,
  TRUE = 1
}; /* We want this to behave as a normal boolean */

enum EasyBoolean EasyBoolean__copy(enum EasyBoolean const *const me);

void EasyBoolean__print(enum EasyBoolean *me);
void EasyBoolean__print_json(enum EasyBoolean *me);

void EasyBoolean__destroy(enum EasyBoolean *me);
