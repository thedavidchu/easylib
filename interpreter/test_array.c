#include <assert.h>
#include <stdio.h>

#include "array.h"

static size_t
zero(struct Array *me)
{
    return 0;
}

static size_t
last(struct Array *me)
{
    assert(me);
    return me->length - 1;
}

static size_t
append(struct Array *me)
{
    assert(me);
    return me->length;
}

int
test_legal_mod(char const *const test_name,
               size_t (*ins)(struct Array *me),
               size_t (*del)(struct Array *me))
{
    int err = 0;
    size_t i = 0, idx = 0, victim = 0;
    struct Array a = {0};

    printf("> %s\n", test_name);
    err = array_ctor(&a);
    assert(!err);

    printf("> \tInsert 0..10\n");
    for (i = 0; i < 10; ++i) {
        idx = ins(&a);
        err = array_insert(&a, idx, (void *)i);
        assert(!err);
        printf("> \t\tInserted %zu at %zu: ", i, idx);
        err = array_fprint(&a, stdout, true);
        assert(!err);
    }

    printf("> \tRemove 0..10\n");
    for (i = 0; i < 10; ++i) {
        idx = del(&a);
        err = array_remove(&a, idx, (struct Object **)&victim);
        assert(!err);
        printf("> \t\tRemoved %zu from %zu: ", victim, idx);
        err = array_fprint(&a, stdout, true);
        assert(!err);
    }

    err = array_dtor(&a);
    assert(!err);

    printf("> \tOK!\n");
    return 0;
}

int
test_illegal_mod(void)
{
    int err = 0;
    size_t i = 0, victim = 0;
    struct Array a = {0};

    printf("> Test Illegal Modification\n");
    err = array_ctor(&a);
    assert(!err);

    printf("> \tInsert 0..10: ");
    for (i = 0; i < 10; ++i) {
        err = array_insert(&a, a.length, (void *)i);
        assert(!err);
    }
    err = array_fprint(&a, stdout, true);
    assert(!err);

    printf("> \tFail inserting ILLEGAL position %zu + 1: ", a.length);
    err = array_insert(&a, a.length + 1, (void *)11);
    assert(err == -1);
    assert(a.length == 10 && "unexpected change in length");
    err = array_fprint(&a, stdout, true);
    assert(!err);

    printf("> \tFail removing ILLEGAL position %zu: ", a.length);
    err = array_remove(&a, a.length, (void *)&victim);
    assert(err == -1);
    assert(a.length == 10 && "unexpected change in length");
    err = array_fprint(&a, stdout, true);
    assert(!err);

    printf("> \tRemove 0..10: ");
    for (i = 0; i < 10; ++i) {
        // NOTE I do not free the victim, since they're just illegal pointers.
        err = array_remove(&a, 0, (struct Object **)&victim);
        assert(!err);
    }
    err = array_fprint(&a, stdout, true);
    assert(!err);

    err = array_dtor(&a);
    assert(!err);

    printf("> \tOK!\n");
    return 0;
}

int
test_access(void)
{
    int err = 0;
    size_t i = 0, victim = 0;
    struct Array a = {0};

    printf("> Test Access\n");
    err = array_ctor(&a);
    assert(!err);

    printf("> \tInsert 0..10: ");
    for (i = 0; i < 10; ++i) {
        err = array_insert(&a, a.length, (void *)i);
        assert(!err);
    }
    err = array_fprint(&a, stdout, true);
    assert(!err);

    printf("> \tAccess LEGAL positions 0..10\n");
    for (i = 0; i < 10; ++i) {
        err = array_get(&a, i, (struct Object **)&victim);
        assert(!err);
        assert(victim == i);
    }

    printf("> \tFail accessing ILLEGAL position %zu\n", a.length);
    err = array_get(&a, a.length, (struct Object **)&victim);
    assert(err == -1);

    printf("> \tRemove 0..10\n");
    for (i = 0; i < 10; ++i) {
        // NOTE I do not free the victim, since they're just illegal pointers.
        err = array_remove(&a, 0, (struct Object **)&victim);
        assert(!err);
    }

    err = array_dtor(&a);
    assert(!err);

    printf("> \tOK!\n");
    return 0;
}

int
main()
{
    int err = 0;
    // 1. Test inserting to front.
    // 2. Test removing from front.
    err = test_legal_mod("Test Modifying Front", zero, zero);
    assert(!err);
    // 3. Test inserting to end.
    // 4. Test removing from end.
    err = test_legal_mod("Test Modifying End", append, last);
    assert(!err);
    // 5. Test inserting out of bounds
    // 6. Test deleting out of bounds
    err = test_illegal_mod();
    assert(!err);
    // 7. Test accessing items
    // 8. Test accessing out of bounds
    err = test_access();
    assert(!err);
    printf("OK!\n");
    return 0;
}
