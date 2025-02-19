#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "boolean.h"
#include "global.h"
#include "nothing.h"
#include "number.h"
#include "object.h"
#include "string.h"

// NOTE I picked -100 for the return value because I wanted something unique.
// #YOLO! All objects
int
phony_ctor(struct Object *const me,
           struct Global const *const global,
           union ObjectData data)
{
    (void)me;
    (void)global;
    (void)data;
    return -100;
}
int
phony_dtor(struct Object *const me)
{
    return -100;
}
int
phony_cmp(struct Object const *const me,
          struct Object const *const other,
          int *const result)
{
    return -100;
}
int
phony_fprint(struct Object const *const me, FILE *const fp, bool const newline)
{
    fprintf(fp, "<Object of type: 0x%p>%s", me->type, newline ? "\n" : "");
    return -100;
}
int
phony_from_cstr(struct Object *const me,
                struct Global const *const global,
                char const *const cstr,
                char const **end_cstr)
{
    (void)me;
    (void)global;
    (void)cstr;
    return -100;
}
// String, array, or table
int
phony_len(struct Object const *const me, size_t *const result)
{
    return -100;
}
int
phony_cap(struct Object const *const me, size_t *const result)
{
    return -100;
}
int
phony_insert(struct Object *const me,
             size_t const idx,
             struct Object *const result)
{
    return -100;
}
int
phony_get(struct Object *const me,
          size_t const idx,
          struct Object **const result)
{
    return -100;
}
int
phony_slice(struct Object const *const,
            size_t const start,
            size_t const end,
            struct Object *const)
{
    return -100;
}
int
phony_remove(struct Object *const me,
             size_t const idx,
             struct Object **const result)
{
    return -100;
}
// Number
int
phony_add(struct Object const *const me,
          struct Object const *const other,
          struct Object *const result)
{
    return -100;
}
int
phony_sub(struct Object const *const me,
          struct Object const *const other,
          struct Object *const result)
{
    return -100;
}
int
phony_mul(struct Object const *const me,
          struct Object const *const other,
          struct Object *const result)
{
    return -100;
}
int
phony_div(struct Object const *const me,
          struct Object const *const other,
          struct Object *const result)
{
    return -100;
}
// Boolean
int
phony_not(struct Object const *const me, bool *const result)
{
    return -100;
}
int
phony_and(struct Object const *const me,
          struct Object const *const other,
          bool *const result)
{
    return -100;
}
int
phony_or(struct Object const *const me,
         struct Object const *const other,
         bool *const result)
{
    return -100;
}
int
phony_truthiness(struct Object const *const me, bool *const result)
{
    return -100;
}
// Function
int
phony_call(struct Object const *const me,
           struct Object *const arg,
           struct Object *const result)
{
    return -100;
}

/// @note   This is for the sole purpose of checking that we initialize all
/// fields.
struct ObjectType
new_object_type(
    enum BuiltinObjectType type,
    int (*ctor)(struct Object *const,
                struct Global const *const type,
                union ObjectData),
    int (*dtor)(struct Object *const),
    int (*cmp)(struct Object const *const,
               struct Object const *const,
               int *const result),
    int (*fprint)(struct Object const *const,
                  FILE *const fp,
                  bool const newline),
    int (*from_cstr)(struct Object *const,
                     struct Global const *const type,
                     char const *const cstr,
                     char const **end_cstr),
    // String, array, or table
    int (*len)(struct Object const *const, size_t *const result),
    int (*cap)(struct Object const *const, size_t *const result),
    int (*insert)(struct Object *const, size_t const idx, struct Object *const),
    int (*get)(struct Object *const, size_t const idx, struct Object **const),
    int (*slice)(struct Object const *const,
                 size_t const start,
                 size_t const end,
                 struct Object *const),
    int (*remove)(struct Object *const,
                  size_t const idx,
                  struct Object **const),
    // Number
    int (*add)(struct Object const *const,
               struct Object const *const,
               struct Object *const),
    int (*sub)(struct Object const *const,
               struct Object const *const,
               struct Object *const),
    int (*mul)(struct Object const *const,
               struct Object const *const,
               struct Object *const),
    int (*div)(struct Object const *const,
               struct Object const *const,
               struct Object *const),
    // Boolean
    int (*not )(struct Object const *const, bool *const result),
    int (*and)(struct Object const *const,
               struct Object const *const,
               bool *const result),
    int (* or)(struct Object const *const,
               struct Object const *const,
               bool *const result),
    int (*truthiness)(struct Object const *const, bool *const result),
    // Function
    int (*call)(struct Object const *const,
                struct Object *const arg,
                struct Object *const result))
{
    return (struct ObjectType){
        .type = type,
        .ctor = ctor ? ctor : phony_ctor,
        .dtor = dtor ? dtor : phony_dtor,
        .cmp = cmp ? cmp : phony_cmp,
        .fprint = fprint ? fprint : phony_fprint,
        .from_cstr = from_cstr ? from_cstr : phony_from_cstr,
        .len = len ? len : phony_len,
        .cap = cap ? cap : phony_cap,
        .insert = insert ? insert : phony_insert,
        .get = get ? get : phony_get,
        .slice = slice ? slice : phony_slice,
        .remove = remove ? remove : phony_remove,
        .add = add ? add : phony_add,
        .sub = sub ? sub : phony_sub,
        .mul = mul ? mul : phony_mul,
        .div = div ? div : phony_div,
        .not = not ? not : phony_not,
        .and = and? and : phony_and,
        .or = or ? or : phony_or,
        .truthiness = truthiness ? truthiness : phony_truthiness,
        .call = call ? call : phony_call,
    };
}

void
init_builtin_object_types(struct BuiltinObjectTypes *const types)
{
    // TODO
    types->nothing = new_object_type(OBJECT_TYPE_NOTHING,
                                     nothing_ctor,
                                     nothing_dtor,
                                     nothing_cmp,
                                     nothing_fprint,
                                     nothing_from_cstr,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL);
    types->boolean = new_object_type(OBJECT_TYPE_BOOLEAN,
                                     boolean_ctor,
                                     boolean_dtor,
                                     boolean_cmp,
                                     boolean_fprint,
                                     boolean_from_cstr,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     boolean_not,
                                     boolean_and,
                                     boolean_or,
                                     boolean_truthiness,
                                     NULL);
    types->number = new_object_type(OBJECT_TYPE_NUMBER,
                                    number_ctor,
                                    number_dtor,
                                    number_cmp,
                                    number_fprint,
                                    number_from_cstr,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    number_add,
                                    number_sub,
                                    number_mul,
                                    number_div,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL);
    types->string = new_object_type(OBJECT_TYPE_STRING,
                                    string_ctor,
                                    string_dtor,
                                    string_cmp,
                                    string_fprint,
                                    string_from_cstr,
                                    string_len,
                                    NULL,
                                    NULL,
                                    NULL,
                                    string_slice,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL);
    types->array = new_object_type(OBJECT_TYPE_ARRAY,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL);
    types->table = new_object_type(OBJECT_TYPE_TABLE,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL);
    types->function = new_object_type(OBJECT_TYPE_FUNCTION,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL);
    types->custom = new_object_type(OBJECT_TYPE_CUSTOM,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL);
}
