#include <assert.h>

#include "table.h"

int
main(void)
{
    int err = 0;
    size_t victim = 0;
    struct Table t = {0};

    printf("> Test Table\n");
    err = table_ctor(&t);
    assert(!err);

    printf("> \tEmpty table: ");
    table_fprint(&t, stdout, true);
    printf("> \tInsert keys 0..10\n");
    for (size_t i = 0; i < 10; ++i) {
        err = table_insert(&t, i, i);
        assert(!err);
        printf("> \t\tInserted %zu: %zu: ", i, i);
        err = table_fprint(&t, stdout, true);
        assert(!err);
    }

    printf("> \tGet keys 0..10\n");
    for (size_t i = 0; i < 10; ++i) {
        err = table_get(&t, i, &victim);
        assert(!err);
        assert(victim == i);
    }

    printf("> \tFail getting ILLEGAL key\n");
    err = table_get(&t, 11, &victim);
    assert(err == -1);

    printf("> \tFail removing ILLEGAL key\n");
    err = table_remove(&t, 11, &victim);
    assert(err == -1);

    printf("> \tRemove keys 0..10\n");
    for (size_t i = 0; i < 10; ++i) {
        err = table_remove(&t, i, &victim);
        assert(!err);
        assert(victim == i);
        printf("> \t\tRemoved %zu: %zu: ", i, victim);
        err = table_fprint(&t, stdout, true);
        assert(!err);
    }

    printf("> \tFail removing already-removed key\n");
    err = table_remove(&t, 0, &victim);
    assert(err == -1);

    err = table_dtor(&t);
    assert(!err);
    printf("OK!\n");

    return 0;
}
