#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "easylib.h"

uint64_t EasyGenericObject__hash(struct EasyGenericObject const *const me);

bool test_easy_hash(void);
