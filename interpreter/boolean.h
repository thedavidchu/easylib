#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "object.h"

static inline char const *
bool_stringify(bool const x)
{
    return x ? "true" : "false";
}

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
boolean_from_cstr(struct Object *const me, struct ObjectType const *const type, char const *const cstr, char const **cstr_end)
{
    if (me == NULL || type == NULL || cstr == NULL) { return -1; }
    me->type = type;
    if (strncmp(cstr, "true", sizeof("true")) == 0) {
        me->data.boolean = true;
        *cstr_end = &cstr[sizeof("true")];
    } else if (strncmp(cstr, "false", sizeof("false")) == 0) {
        me->data.boolean = false;
        *cstr_end = &cstr[sizeof("false")];
    } else {
        *cstr_end = cstr;
        return -1;
    }
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
    fprintf(fp, "%s%s", bool_stringify(me->data.boolean), newline ? "\n" : "");
    return 0;
}

static int
generic_boolean_op(struct Object const *const lhs, struct Object const *const rhs, bool *const result, int const op)
{
    int err = 0;
    if ((err = boolean_error(lhs))) { return err; }
    if ((err = boolean_error(rhs))) { return err; }
    bool ans = false;
    switch (op) {
    case 0:
        ans = lhs->data.boolean && rhs->data.boolean;
        break;
    case 1:
        ans = lhs->data.boolean || rhs->data.boolean;
        break;
    default:
        return -1;
    }
    *result = ans;
    return 0;
}

int
boolean_not(struct Object const *const me,  bool *const result)
{
    int err = 0;
    if ((err = boolean_error(me))) { return err; }
    *result = !me->data.boolean;
    return 0;
}

int
boolean_and(struct Object const *const me, struct Object const *const other, bool *const result)
{
    return generic_boolean_op(me, other, result, 0);
}

int
boolean_or(struct Object const *const me, struct Object const *const other, bool *const result)
{
    return generic_boolean_op(me, other, result, 1);
}

int
boolean_truthiness(struct Object const *const me,  bool *const result)
{
    int err = 0;
    if ((err = boolean_error(me))) { return err; }
    *result = me->data.boolean;
    return 0;
}
