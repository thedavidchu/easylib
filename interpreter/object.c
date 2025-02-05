#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "object.h"
#include "nothing.h"
#include "number.h"
#include "boolean.h"

// NOTE I picked -100 for the return value because I wanted something unique. #YOLO!
// All objects
int phony_ctor(struct Object *const me, struct ObjectType const *const type, union ObjectData data) { (void)me; (void)type; (void)data; return -100; }
int phony_dtor(struct Object *const me) { return -100; }
int phony_cmp(struct Object const *const me, struct Object const *const other, int *const result) { return -100; }
int phony_fprint(struct Object const *const me, FILE *const fp, bool const newline) { fprintf(fp, "<Object of type: 0x%p>%s", me->type, newline ? "\n" : ""); return -100; }
// String, array, or table
int phony_len(struct Object const *const me, size_t *const result) { return -100; }
int phony_cap(struct Object const *const me, size_t *const result) { return -100; }
int phony_insert(struct Object *const me, size_t const idx, struct Object *const result) { return -100; }
int phony_get(struct Object *const me, size_t const idx, struct Object **const result) { return -100; }
int phony_remove(struct Object *const me, size_t const idx, struct Object **const result) { return -100; }
// Number
int phony_add(struct Object const *const me, struct Object const *const other, struct Object **const result) { return -100; }
int phony_sub(struct Object const *const me, struct Object const *const other, struct Object **const result) { return -100; }
int phony_mul(struct Object const *const me, struct Object const *const other, struct Object **const result) { return -100; }
int phony_div(struct Object const *const me, struct Object const *const other, struct Object **const result) { return -100; }
// Boolean
int phony_not(struct Object const *const me, struct Object const *const other, bool *const result) { return -100; }
int phony_and(struct Object const *const me, struct Object const *const other, bool *const result) { return -100; }
int phony_or(struct Object const *const me, struct Object const *const other, bool *const result) { return -100; }
int phony_truthiness(struct Object const *const me, bool *const result) { return -100; }
// Function
int phony_call(struct Object const *const me, struct Object *const arg, struct Object **const result) { return -100; }


/// @note   This is for the sole purpose of checking that we initialize all fields.
struct ObjectType
new_object_type(
    enum BuiltinObjectType type,
    int (*ctor)(struct Object *const, struct ObjectType const *const type, union ObjectData),
    int (*dtor)(struct Object *const),
    int (*cmp)(struct Object const *const, struct Object const *const, int *const result),
    int (*fprint)(struct Object const *const, FILE *const fp, bool const newline),
    // String, array, or table
    int (*len)(struct Object const *const, size_t *const result),
    int (*cap)(struct Object const *const, size_t *const result),
    int (*insert)(struct Object *const, size_t const idx, struct Object *const),
    int (*get)(struct Object *const, size_t const idx, struct Object **const),
    int (*remove)(struct Object *const, size_t const idx, struct Object **const),
    // Number
    int (*add)(struct Object const *const, struct Object const *const, struct Object **const),
    int (*sub)(struct Object const *const, struct Object const *const, struct Object **const),
    int (*mul)(struct Object const *const, struct Object const *const, struct Object **const),
    int (*div)(struct Object const *const, struct Object const *const, struct Object **const),
    // Boolean
    int (*not)(struct Object const *const, struct Object const *const, bool *const result),
    int (*and)(struct Object const *const, struct Object const *const, bool *const result),
    int (*or)(struct Object const *const, struct Object const *const, bool *const result),
    int (*truthiness)(struct Object const *const, bool *const result),
    // Function
    int (*call)(struct Object const *const, struct Object *const arg, struct Object **const result)
)
{
    return (struct ObjectType){
        .type = type,
        .ctor = ctor ? ctor : phony_ctor,
        .dtor = dtor ? dtor : phony_dtor,
        .cmp = cmp ? cmp : phony_cmp,
        .fprint = fprint ? fprint : phony_fprint,
        .len = len ? len : phony_len,
        .cap = cap ? cap : phony_cap,
        .insert = insert ? insert : phony_insert,
        .get = get ? get : phony_get,
        .remove = remove ? remove : phony_remove,
        .add = add ? add : phony_add,
        .sub = sub ? sub : phony_sub,
        .mul = mul ? mul : phony_mul,
        .div = div ? div : phony_div,
        .not = not ? not : phony_not,
        .and = and ? and : phony_and,
        .or = or ? or : phony_or,
        .truthiness = truthiness ? truthiness : phony_truthiness,
        .call = call ? call : phony_call,
    };
}

void
init_builtin_object_types(struct BuiltinObjectTypes *const types)
{
    // TODO
    types->nothing = new_object_type(OBJECT_TYPE_NOTHING, nothing_ctor, nothing_dtor, nothing_cmp, nothing_fprint, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    types->boolean = new_object_type(OBJECT_TYPE_BOOLEAN, boolean_ctor, boolean_dtor, boolean_cmp, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    types->number = new_object_type(OBJECT_TYPE_NUMBER, number_ctor, number_dtor, number_cmp, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    types->string = new_object_type(OBJECT_TYPE_STRING, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    types->array = new_object_type(OBJECT_TYPE_ARRAY, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    types->table = new_object_type(OBJECT_TYPE_TABLE, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    types->function = new_object_type(OBJECT_TYPE_FUNCTION, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    types->custom = new_object_type(OBJECT_TYPE_CUSTOM, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

int main(void)
{
    struct BuiltinObjectTypes builtin_types = {0};
    init_builtin_object_types(&builtin_types);
    struct Object nothing = {0};
    builtin_types.nothing.ctor(&nothing, &builtin_types.nothing, (union ObjectData){.nothing = NULL});
    nothing.type->fprint(&nothing, stdout, true);
    return 0;
}
