#pragma once

#include "object.h"

struct Global {
    struct BuiltinObjectTypes builtin_types;
    // TODO Hold the stack frames with variables.
};

int
init_global(struct Global *const global)
{
    int err = 0;
    init_builtin_object_types(&global->builtin_types);
    return err;
}

int
destroy_global(struct Global *const global)
{
    *global = (struct Global){0};
    return 0;
}
