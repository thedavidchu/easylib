#pragma once

#include <stdbool.h>

#include "object.h"

int
boolean_ctor(struct Object *me, struct ObjectType const *const type, union ObjectData data)
{
    if (me == NULL) { return -1; }
    me->type = type;
    // TODO Check if data is valid.
    me->data = data;
    return 0;
}

int
boolean_dtor(struct Object *me)
{
    if (me == NULL) { return -1; }
    if (me->type->type != OBJECT_TYPE_BOOLEAN) { return -1; }
    *me = (struct Object){0};
    return 0;
}

int
boolean_cmp(struct Object const *me, struct Object const *other, int *result)
{
    if (me == NULL || other == NULL || result == NULL) { return -1; }
    if (me->type->type != OBJECT_TYPE_BOOLEAN) { return -1; }
    if (me == other) { *result = 0; return 0; }
    if (other->type->type == OBJECT_TYPE_BOOLEAN && me->data.boolean == other->data.boolean) { *result = 1; return 0; }
    *result = 3;
    return 0;
}
