#include <assert.h>
#include <math.h>
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

static int
test_boolean_op(struct Global const *const global,
                char const op,
                double const lhs,
                double const rhs,
                double const expected,
                FILE *const fp)
{
    struct Object x = {0}, y = {0};
    bool r = false;
    global->builtin_types.boolean.ctor(&x,
                                       global,
                                       (union ObjectData){.boolean = lhs});
    global->builtin_types.boolean.ctor(&y,
                                       global,
                                       (union ObjectData){.boolean = rhs});
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
        x.type->and (&x, &y, &r);
        if (fp) {
            x.type->fprint(&x, fp, false);
            fprintf(fp, " && ");
            y.type->fprint(&y, fp, false);
            fprintf(fp, " = %s\n", bool_stringify(r));
        }
        assert((x.data.boolean && y.data.boolean) == r);
        break;
    case '|':
        x.type->or (&x, &y, &r);
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
test_number_op(struct Global const *const global,
               char const op,
               double const lhs,
               double const rhs,
               double const expected,
               FILE *const fp)
{
    struct Object x = {0}, y = {0}, r = {0};
    global->builtin_types.number.ctor(&x,
                                      global,
                                      (union ObjectData){.number = lhs});
    global->builtin_types.number.ctor(&y,
                                      global,
                                      (union ObjectData){.number = rhs});
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

static int
test_string_from_cstr(struct Global const *const global,
                      char const *const cstr,
                      int const expected_err,
                      char const *const expected)
{
    struct Object string = {0};
    char const *endptr = NULL;
    int err =
        global->builtin_types.string.from_cstr(&string, global, cstr, &endptr);
    if (err != expected_err) {
        printf("Expected %d, got %d\n", expected_err, err);
        return -1;
    }
    if (!err) {
        string.type->fprint(&string, stdout, true);
        if (strcmp(string.data.string, expected) != 0) {
            printf("Expected '%s', got '%s'\n", expected, string.data.string);
            return -1;
        }
        string.type->dtor(&string);
    }
    return 0;
}

int
main(void)
{
    int err = 0;
    char const *endptr = NULL;

    struct Global global = {0};
    init_global(&global);

    // Test Nothing
    struct Object nothing = {0};
    global.builtin_types.nothing.ctor(&nothing,
                                      &global,
                                      (union ObjectData){.nothing = NULL});
    nothing.type->fprint(&nothing, stdout, true);
    nothing.type->dtor(&nothing);

    global.builtin_types.nothing.from_cstr(&nothing, &global, "null", &endptr);
    nothing.type->fprint(&nothing, stdout, true);
    nothing.type->dtor(&nothing);

    err = global.builtin_types.nothing.from_cstr(&nothing,
                                                 &global,
                                                 "blah",
                                                 &endptr);
    assert(err);

    // Test Boolean
    struct Object boolean = {0};
    global.builtin_types.boolean.ctor(&boolean,
                                      &global,
                                      (union ObjectData){.boolean = false});
    boolean.type->fprint(&boolean, stdout, true);
    boolean.data.boolean = true;
    boolean.type->fprint(&boolean, stdout, true);
    boolean.type->dtor(&boolean);

    global.builtin_types.boolean.from_cstr(&boolean, &global, "true", &endptr);
    boolean.type->fprint(&boolean, stdout, true);
    boolean.type->dtor(&boolean);
    global.builtin_types.boolean.from_cstr(&boolean, &global, "false", &endptr);
    boolean.type->fprint(&boolean, stdout, true);
    boolean.type->dtor(&boolean);
    err = global.builtin_types.boolean.from_cstr(&boolean,
                                                 &global,
                                                 "blah",
                                                 &endptr);
    assert(err);

    test_boolean_op(&global, ' ', true, 0, true, stdout);
    test_boolean_op(&global, ' ', false, 0, false, stdout);
    test_boolean_op(&global, '!', true, 0, false, stdout);
    test_boolean_op(&global, '!', false, 0, true, stdout);
    test_boolean_op(&global, '&', true, true, true, stdout);
    test_boolean_op(&global, '&', true, false, false, stdout);
    test_boolean_op(&global, '&', false, true, false, stdout);
    test_boolean_op(&global, '&', false, false, false, stdout);
    test_boolean_op(&global, '|', true, true, true, stdout);
    test_boolean_op(&global, '|', true, false, true, stdout);
    test_boolean_op(&global, '|', false, true, true, stdout);
    test_boolean_op(&global, '|', false, false, false, stdout);

    // Test Number
    struct Object number = {0};
    global.builtin_types.boolean.ctor(&number,
                                      &global,
                                      (union ObjectData){.number = 0.0});
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

    global.builtin_types.number.from_cstr(&number, &global, "0.0", &endptr);
    number.type->fprint(&number, stdout, true);
    number.type->dtor(&number);
    global.builtin_types.number.from_cstr(&number, &global, "1.23e45", &endptr);
    number.type->fprint(&number, stdout, true);
    number.type->dtor(&number);
    global.builtin_types.number.from_cstr(&number,
                                          &global,
                                          "-12.34E-56",
                                          &endptr);
    number.type->fprint(&number, stdout, true);
    number.type->dtor(&number);
    global.builtin_types.number.from_cstr(&number, &global, "10000.0", &endptr);
    number.type->fprint(&number, stdout, true);
    number.type->dtor(&number);
    err = global.builtin_types.number.from_cstr(&number,
                                                &global,
                                                "blah",
                                                &endptr);
    assert(err);

    test_number_op(&global, '+', 1.0, 1.0, 2.0, stdout);
    test_number_op(&global, '-', 1.0, 1.0, 0.0, stdout);
    test_number_op(&global, '*', 1.0, 1.0, 1.0, stdout);
    test_number_op(&global, '/', 1.0, 1.0, 1.0, stdout);

    // Test String
    struct Object string = {0};
    struct Object string_slice = {0};
    global.builtin_types.string.ctor(
        &string,
        &global,
        (union ObjectData){.string = cstr_dup("Hello, World!")});
    string.type->fprint(&string, stdout, true);

    global.builtin_types.string.slice(&string, 3, 10, &string_slice);
    string_slice.type->fprint(&string_slice, stdout, true);

    string.type->dtor(&string);
    string_slice.type->dtor(&string_slice);

    test_string_from_cstr(&global, "\"Hello, World!\"", 0, "Hello, World!");
    test_string_from_cstr(&global, "\" \\a \\b \\\\ \"", 0, " \a \b \\ ");
    test_string_from_cstr(&global, " \\ ", -1, NULL);
    test_string_from_cstr(&global, "\" \\ ", -1, NULL);

    return 0;
}
