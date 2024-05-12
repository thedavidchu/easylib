#pragma once

#include <stdbool.h>

#include "easy_lib.h"

bool
EasyGenericObject__equal(struct EasyGenericObject const *const lhs,
                         struct EasyGenericObject const *const rhs);

bool
test_easy_equal(void);
