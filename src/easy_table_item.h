#pragma once

#include <stdbool.h>

#include "easy_lib.h"

/* NOTE These need to come after the EasyGenericObject */
struct EasyTableItem {
    bool valid;
    struct EasyGenericObject key;
    struct EasyGenericObject value;
};
