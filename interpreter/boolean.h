#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "object.h"

static int
boolean_error(struct Object const *const me)
{
    if (me == NULL || me->type == NULL || me->type->type != OBJECT_TYPE_BOOLEAN) { return -1; }
    assert(me->data.boolean == true || me->data.boolean == false);
    return 0;
}

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
    int err = 0;
    if ((err = boolean_error(me))) { return err; }
    *me = (struct Object){0};
    return 0;
}

int
boolean_cmp(struct Object const *me, struct Object const *other, int *result)
{
    int err = 0;
    if ((err = boolean_error(me))) { return err; }
    if (me == NULL || other == NULL || result == NULL) { return -1; }
    if (me->type->type != OBJECT_TYPE_BOOLEAN) { return -1; }
    if (me == other) { *result = 0; return 0; }
    if (other->type->type == OBJECT_TYPE_BOOLEAN && me->data.boolean == other->data.boolean) { *result = 1; return 0; }
    *result = 3;
    return 0;
}

int
boolean_fprint(struct Object const *const me, FILE *const fp, bool const newline)
{
    int err = 0;
    if ((err = boolean_error(me))) { return err; }
    // TODO Handle errors in fprintf(...).
    fprintf(fp, "%s%s", me->data.boolean ? "true" : "false", newline ? "\n" : "");
    return 0;
}
