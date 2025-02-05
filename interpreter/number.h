#pragma once

#include <assert.h>
#include <errno.h>

#include "object.h"

static int
number_error(struct Object const *const me)
{
    if (me == NULL || me->type == NULL || me->type->type != OBJECT_TYPE_NUMBER) { return -1; }
    return 0;
}

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
    int err = 0;
    if ((err = number_error(me))) { return err; }
    *me = (struct Object){0};
    return 0;
}

int
number_cmp(struct Object const *me, struct Object const *other, int *result)
{
    int err = 0;
    if ((err = number_error(me))) { return err; }
    if (me == NULL || other == NULL || result == NULL) { return -1; }
    if (me->type->type != OBJECT_TYPE_NUMBER) { return -1; }
    if (me == other) { *result = 0; return 0; }
    if (me->type != other->type) { *result = 3; return 0; }
    if (me->data.number < other->data.number) { *result = -1; return 0; }
    if (me->data.number == other->data.number) { *result = 1; return 0; }
    if (me->data.number > other->data.number) { *result = 2; return 0; }
    assert(0 && "impossible");
}

int
number_fprint(struct Object const *const me, FILE *const fp, bool const newline)
{
    int err = 0;
    if ((err = number_error(me))) { return err; }
    // TODO Handle errors in fprintf(...).
    fprintf(fp, "%f%s", me->data.number, newline ? "\n" : "");
    return 0;
}

static int
generic_number_op(struct Object const *const lhs, struct Object const *const rhs, struct Object *const result, int const op)
{
    int err = 0;
    if ((err = number_error(lhs))) { return err; }
    if ((err = number_error(rhs))) { return err; }
    double ans = 0.0;
    switch (op) {
    case 0:
        ans = lhs->data.number + rhs->data.number;
        break;
    case 1:
        ans = lhs->data.number - rhs->data.number;
        break;
    case 2:
        ans = lhs->data.number * rhs->data.number;
        break;
    case 3:
        ans = lhs->data.number * rhs->data.number;
        break;
    default:
        return -1;
    }
    if ((err = number_ctor(result, lhs->type, (union ObjectData){.number = ans})));
    return 0;
}

int
number_add(struct Object const *const me, struct Object const *const other, struct Object *const result)
{
    return generic_number_op(me, other, result, 0);
}

int
number_sub(struct Object const *const me, struct Object const *const other, struct Object *const result)
{
    return generic_number_op(me, other, result, 1);
}

int
number_mul(struct Object const *const me, struct Object const *const other, struct Object *const result)
{
    return generic_number_op(me, other, result, 2);
}

int
number_div(struct Object const *const me, struct Object const *const other, struct Object *const result)
{
    return generic_number_op(me, other, result, 3);
}
