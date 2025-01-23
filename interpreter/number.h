#pragma once

#include <assert.h>

#include "object.h"

int
number_ctor(struct Object *me, struct ObjectType const *const type, union ObjectData data)
{
    if (me == NULL) { return -1; }
    me->type = type;
    // TODO Check if data is valid.
    me->data = data;
    return 0;
}

int
number_dtor(struct Object *me)
{
    if (me == NULL) { return -1; }
    if (me->type->type != OBJECT_TYPE_NUMBER) { return -1; }
    *me = (struct Object){0};
    return 0;
}

int
number_cmp(struct Object const *me, struct Object const *other, int *result)
{
    if (me == NULL || other == NULL || result == NULL) { return -1; }
    if (me->type->type != OBJECT_TYPE_NUMBER) { return -1; }
    if (me == other) { *result = 0; return 0; }
    if (me->type != other->type) { *result = 3; return 0; }
    if (me->data.number < other->data.number) { *result = -1; return 0; }
    if (me->data.number == other->data.number) { *result = 1; return 0; }
    if (me->data.number > other->data.number) { *result = 2; return 0; }
    assert(0 && "impossible");
}
