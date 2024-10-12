#include <stdint.h>

#include "common/memory.h"
#include "test/test.h"

static void
test_malloc(void)
{
    struct PointerResult a = ez_malloc(1, 1);
    TEST_ASSERT_OK(a.status);
    struct PointerResult b = ez_malloc(1000, 1000);
    TEST_ASSERT_OK(b.status);

    struct PointerResult err_a = ez_malloc(SIZE_MAX, SIZE_MAX);
    // I assume SIZE_MAX * 2 overflows.
    struct PointerResult err_b = ez_malloc(SIZE_MAX, 2);
    struct PointerResult err_c = ez_malloc(2, SIZE_MAX);
    TEST_ASSERT_NOT_OK(err_a.status);
    TEST_ASSERT_NOT_OK(err_b.status);
    TEST_ASSERT_NOT_OK(err_c.status);

    ez_free(a.ptr);
    ez_free(b.ptr);
    ez_free(err_a.ptr);
    ez_free(err_b.ptr);
    ez_free(err_c.ptr);
}

static void
test_free(void)
{
    ez_free(NULL);
}

int
main(void)
{
    test_malloc();
    test_free();
    return 0;
}
