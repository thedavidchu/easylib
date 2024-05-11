#pragma once

#include <stddef.h>

/* Stand-alone types */
typedef void *EasyNothing; /* We want this to act as NULL in C */

/* EasyNothing */
static const EasyNothing NOTHING = NULL;

EasyNothing EasyNothing__new();
void EasyNothing__print(EasyNothing const *const me);
void EasyNothing__print_json(EasyNothing const *const me);
EasyNothing EasyNothing__copy(EasyNothing const *const me);
void EasyNothing__destroy(EasyNothing *const me);
