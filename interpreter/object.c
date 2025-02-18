#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "object.h"
#include "nothing.h"
#include "number.h"
#include "boolean.h"
#include "string.h"

// NOTE I picked -100 for the return value because I wanted something unique. #YOLO!
// All objects
int phony_ctor(struct Object *const me, struct ObjectType const *const type, union ObjectData data) { (void)me; (void)type; (void)data; return -100; }
int phony_dtor(struct Object *const me) { return -100; }
int phony_cmp(struct Object const *const me, struct Object const *const other, int *const result) { return -100; }
int phony_fprint(struct Object const *const me, FILE *const fp, bool const newline) { fprintf(fp, "<Object of type: 0x%p>%s", me->type, newline ? "\n" : ""); return -100; }
int phony_from_cstr(struct Object const *const me, struct ObjectType const *const type, char const *const cstr) { (void)me; (void)type; (void)cstr; return -100; }
// String, array, or table
int phony_len(struct Object const *const me, size_t *const result) { return -100; }
int phony_cap(struct Object const *const me, size_t *const result) { return -100; }
int phony_insert(struct Object *const me, size_t const idx, struct Object *const result) { return -100; }
int phony_get(struct Object *const me, size_t const idx, struct Object **const result) { return -100; }
int phony_slice(struct Object const *const, size_t const start, size_t const end, struct Object *const) { return -100; }
int phony_remove(struct Object *const me, size_t const idx, struct Object **const result) { return -100; }
// Number
int phony_add(struct Object const *const me, struct Object const *const other, struct Object *const result) { return -100; }
int phony_sub(struct Object const *const me, struct Object const *const other, struct Object *const result) { return -100; }
int phony_mul(struct Object const *const me, struct Object const *const other, struct Object *const result) { return -100; }
int phony_div(struct Object const *const me, struct Object const *const other, struct Object *const result) { return -100; }
// Boolean
int phony_not(struct Object const *const me, bool *const result) { return -100; }
int phony_and(struct Object const *const me, struct Object const *const other, bool *const result) { return -100; }
int phony_or(struct Object const *const me, struct Object const *const other, bool *const result) { return -100; }
int phony_truthiness(struct Object const *const me, bool *const result) { return -100; }
// Function
int phony_call(struct Object const *const me, struct Object *const arg, struct Object *const result) { return -100; }


/// @note   This is for the sole purpose of checking that we initialize all fields.
struct ObjectType
new_object_type(
    enum BuiltinObjectType type,
    int (*ctor)(struct Object *const, struct ObjectType const *const type, union ObjectData),
    int (*dtor)(struct Object *const),
    int (*cmp)(struct Object const *const, struct Object const *const, int *const result),
    int (*fprint)(struct Object const *const, FILE *const fp, bool const newline),
    int (*from_cstr)(struct Object const *const, struct ObjectType const *const type, char const *const cstr),
    // String, array, or table
    int (*len)(struct Object const *const, size_t *const result),
    int (*cap)(struct Object const *const, size_t *const result),
    int (*insert)(struct Object *const, size_t const idx, struct Object *const),
    int (*get)(struct Object *const, size_t const idx, struct Object **const),
    int (*slice)(struct Object const *const, size_t const start, size_t const end, struct Object *const),
    int (*remove)(struct Object *const, size_t const idx, struct Object **const),
    // Number
    int (*add)(struct Object const *const, struct Object const *const, struct Object *const),
    int (*sub)(struct Object const *const, struct Object const *const, struct Object *const),
    int (*mul)(struct Object const *const, struct Object const *const, struct Object *const),
    int (*div)(struct Object const *const, struct Object const *const, struct Object *const),
    // Boolean
    int (*not)(struct Object const *const, bool *const result),
    int (*and)(struct Object const *const, struct Object const *const, bool *const result),
    int (*or)(struct Object const *const, struct Object const *const, bool *const result),
    int (*truthiness)(struct Object const *const, bool *const result),
    // Function
    int (*call)(struct Object const *const, struct Object *const arg, struct Object *const result)
)
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
    types->nothing = new_object_type(OBJECT_TYPE_NOTHING, nothing_ctor, nothing_dtor, nothing_cmp, nothing_fprint, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    types->boolean = new_object_type(OBJECT_TYPE_BOOLEAN, boolean_ctor, boolean_dtor, boolean_cmp, boolean_fprint, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, boolean_not, boolean_and, boolean_or, boolean_truthiness, NULL);
    types->number = new_object_type(OBJECT_TYPE_NUMBER, number_ctor, number_dtor, number_cmp, number_fprint, NULL, NULL, NULL, NULL, NULL, NULL, NULL, number_add, number_sub, number_mul, number_div, NULL, NULL, NULL, NULL, NULL);
    types->string = new_object_type(OBJECT_TYPE_STRING, string_ctor, string_dtor, string_cmp, string_fprint, NULL, string_len, NULL, NULL, NULL, string_slice, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    types->array = new_object_type(OBJECT_TYPE_ARRAY, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    types->table = new_object_type(OBJECT_TYPE_TABLE, NULL,NULL,  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    types->function = new_object_type(OBJECT_TYPE_FUNCTION, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    types->custom = new_object_type(OBJECT_TYPE_CUSTOM, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

static int
test_boolean_op(struct BuiltinObjectTypes const *const builtin_types,
               char const op,
               double const lhs,
               double const rhs,
               double const expected,
               FILE *const fp)
{
    struct Object x = {0}, y = {0};
    bool r = false;
    builtin_types->boolean.ctor(&x, &builtin_types->boolean, (union ObjectData){.boolean = lhs});
    builtin_types->boolean.ctor(&y, &builtin_types->boolean, (union ObjectData){.boolean = rhs});
    switch (op) {
    case ' ':
        x.type->truthiness(&x, &r);
        if (fp) {
            x.type->fprint(&x, fp, false);
            fprintf(fp, " = %s\n", bool_stringify(r));
        }
        assert(x.data.boolean == r);
        break;
    case '!':
        x.type->not(&x, &r);
        if (fp) {
            fprintf(fp, "!");
            x.type->fprint(&x, fp, false);
            fprintf(fp, " = %s\n", bool_stringify(r));
        }
        assert(!x.data.boolean == r);
        break;
    case '&':
        x.type->and(&x, &y, &r);
        if (fp) {
            x.type->fprint(&x, fp, false);
            fprintf(fp, " && ");
            y.type->fprint(&y, fp, false);
            fprintf(fp, " = %s\n", bool_stringify(r));
        }
        assert((x.data.boolean && y.data.boolean) == r);
        break;
    case '|':
        x.type->or(&x, &y, &r);
        if (fp) {
            x.type->fprint(&x, fp, false);
            fprintf(fp, " || ");
            y.type->fprint(&y, fp, false);
            fprintf(fp, " = %s\n", bool_stringify(r));
        }
        assert((x.data.boolean || y.data.boolean) == r);
        break;
    default:
        return -1;
    }
    x.type->dtor(&x);
    y.type->dtor(&y);
    return 0;
}

static int
test_number_op(struct BuiltinObjectTypes const *const builtin_types,
               char const op,
               double const lhs,
               double const rhs,
               double const expected,
               FILE *const fp)
{
    struct Object x = {0}, y = {0}, r = {0};
    builtin_types->number.ctor(&x, &builtin_types->number, (union ObjectData){.number = lhs});
    builtin_types->number.ctor(&y, &builtin_types->number, (union ObjectData){.number = rhs});
    switch (op) {
    case '+':
        x.type->add(&x, &y, &r);
        assert(x.data.number + y.data.number == r.data.number);
        break;
    case '-':
        x.type->sub(&x, &y, &r);
        assert(x.data.number - y.data.number == r.data.number);
        break;
    case '*':
        x.type->mul(&x, &y, &r);
        assert(x.data.number * y.data.number == r.data.number);
        break;
    case '/':
        x.type->div(&x, &y, &r);
        assert(x.data.number / y.data.number == r.data.number);
        break;
    default:
        return -1;
    }
    if (fp) {
        x.type->fprint(&x, fp, false);
        fprintf(fp, " %c ", op);
        y.type->fprint(&y, fp, false);
        fprintf(fp, " = ");
        r.type->fprint(&r, fp, true);
    }
    x.type->dtor(&x);
    y.type->dtor(&y);
    r.type->dtor(&r);
    return 0;
}

int main(void)
{
    // TODO Make global state object.
    struct BuiltinObjectTypes builtin_types = {0};
    init_builtin_object_types(&builtin_types);

    // Test Nothing
    struct Object nothing = {0};
    builtin_types.nothing.ctor(&nothing, &builtin_types.nothing, (union ObjectData){.nothing = NULL});
    nothing.type->fprint(&nothing, stdout, true);
    nothing.type->dtor(&nothing);

    // Test Boolean
    struct Object boolean = {0};
    builtin_types.boolean.ctor(&boolean, &builtin_types.boolean, (union ObjectData){.boolean = false});
    boolean.type->fprint(&boolean, stdout, true);
    boolean.data.boolean = true;
    boolean.type->fprint(&boolean, stdout, true);
    boolean.type->dtor(&boolean);

    test_boolean_op(&builtin_types, ' ', true, 0, true, stdout);
    test_boolean_op(&builtin_types, ' ', false, 0, false, stdout);
    test_boolean_op(&builtin_types, '!', true, 0, false, stdout);
    test_boolean_op(&builtin_types, '!', false, 0, true, stdout);
    test_boolean_op(&builtin_types, '&', true, true, true, stdout);
    test_boolean_op(&builtin_types, '&', true, false, false, stdout);
    test_boolean_op(&builtin_types, '&', false, true, false, stdout);
    test_boolean_op(&builtin_types, '&', false, false, false, stdout);
    test_boolean_op(&builtin_types, '|', true, true, true, stdout);
    test_boolean_op(&builtin_types, '|', true, false, true, stdout);
    test_boolean_op(&builtin_types, '|', false, true, true, stdout);
    test_boolean_op(&builtin_types, '|', false, false, false, stdout);

    // Test Number
    struct Object number = {0};
    builtin_types.boolean.ctor(&number, &builtin_types.number, (union ObjectData){.number = 0.0});
    number.type->fprint(&number, stdout, true);
    number.data.number = 1.0;
    number.type->fprint(&number, stdout, true);
    number.data.number = 3.14;
    number.type->fprint(&number, stdout, true);
    number.data.number = INFINITY;
    number.type->fprint(&number, stdout, true);
    number.data.number = NAN;
    number.type->fprint(&number, stdout, true);
    number.type->dtor(&number);

    test_number_op(&builtin_types, '+', 1.0, 1.0, 2.0, stdout);
    test_number_op(&builtin_types, '-', 1.0, 1.0, 0.0, stdout);
    test_number_op(&builtin_types, '*', 1.0, 1.0, 1.0, stdout);
    test_number_op(&builtin_types, '/', 1.0, 1.0, 1.0, stdout);

    // Test String
    struct Object string = {0};
    struct Object string_slice = {0};
    builtin_types.string.ctor(&string, &builtin_types.string, (union ObjectData){.string = cstr_dup("Hello, World!")});
    string.type->fprint(&string, stdout, true);
    
    builtin_types.string.slice(&string, 3, 10, &string_slice);
    string_slice.type->fprint(&string_slice, stdout, true);

    string.type->dtor(&string);
    string_slice.type->dtor(&string_slice);


    return 0;
}
