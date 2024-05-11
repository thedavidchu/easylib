/* EasyText */
#pragma once

#include <stddef.h>

struct EasyText {
  char *data;
  size_t length; /* Not including the NIL byte at the end */
};

struct EasyText EasyText__from_cstr(char const *str);
void EasyText__print(struct EasyText *me);
void EasyText__print_json(struct EasyText *me);
struct EasyText EasyText__copy(struct EasyText const *const me);
void EasyText__destroy(struct EasyText *me);
