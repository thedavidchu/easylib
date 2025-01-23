#include "array.h"

int
nothing_ctor(struct Object *me, struct ObjectType const *const type, union ObjectData data)
{
    if (me == NULL) { return -1; }
    me->type = type;
    // TODO Check if data is valid.
    me->data = data;
    return 0;
}

int
nothing_dtor(struct Object *me)
{
    if (me == NULL) { return -1; }
    if (me->type->type != OBJECT_TYPE_BOOLEAN) { return -1; }
    *me = (struct Object){0};
    return 0;
}

int
nothing_cmp(struct Object const *me, struct Object const *other, int *result)
{
    if (me == NULL || other == NULL || result == NULL) { return -1; }
    if (me->type->type != OBJECT_TYPE_NOTHING) { return -1; }
    if (me == other) { *result = 0; return 0; }
    if (other->type->type == OBJECT_TYPE_NOTHING) { *result = 1; return 0; }
    *result = 3;
    return 0;
}
