#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "easy_common.h"
#include "easy_lib.h"
#include "easy_nothing.h"
#include "easy_table.h"

#include "easy_equal.h"

/// @note   We need check the elements individually. We can't just compare
///         the buffers but actually need to perform lookups.
static inline bool
EasyTable__equal(struct EasyTable const *const lhs,
                 struct EasyTable const *const rhs)
{
    EASY_GUARD(lhs != NULL && rhs != NULL, "pointers should not be NULL");
    EASY_NOT_IMPLEMENTED();
    return true;
}

static inline bool
EasyList__equal(struct EasyList const *const lhs,
                struct EasyList const *const rhs)
{
    EASY_GUARD(lhs != NULL && rhs != NULL, "pointers should not be NULL");
    if (lhs->length != rhs->length) {
        return false;
    }
    const size_t length = lhs->length;
    for (size_t i = 0; i < length; ++i) {
        if (!EasyGenericObject__equal(&lhs->data[i], &rhs->data[i])) {
            return false;
        }
    }
    return true;
}

static inline bool
EasyText__equal(struct EasyText const *const lhs,
                struct EasyText const *const rhs)
{
    EASY_GUARD(lhs != NULL && rhs != NULL, "pointers should not be NULL");
    if (lhs->length != rhs->length) {
        return false;
    }
    const size_t length = lhs->length;
    return memcmp(lhs->data, rhs->data, length * sizeof(*lhs->data)) == 0;
}

static inline bool
EasyInteger__equal(struct EasyInteger const *const lhs,
                   struct EasyInteger const *const rhs)
{
    EASY_GUARD(lhs != NULL && rhs != NULL, "pointers should not be NULL");
    if (lhs->length != rhs->length) {
        return false;
    }
    const size_t length = lhs->length;
    return memcmp(lhs->data, rhs->data, length * sizeof(*lhs->data)) == 0;
}

static inline bool
EasyFraction__equal(struct EasyFraction const *const lhs,
                    struct EasyFraction const *const rhs)
{
    EASY_GUARD(lhs != NULL && rhs != NULL, "pointers should not be NULL");
    EASY_NOT_IMPLEMENTED();
    return true;
}

static inline bool
EasyBoolean__equal(enum EasyBoolean const *const lhs,
                   enum EasyBoolean const *const rhs)
{
    EASY_GUARD(lhs != NULL && rhs != NULL, "pointers should not be NULL");
    return *lhs == *rhs;
}

static inline bool
EasyNothing__equal(EasyNothing const *const lhs, EasyNothing const *const rhs)
{
    EASY_GUARD(lhs != NULL && rhs != NULL, "pointers should not be NULL");
    EASY_ASSERT(*lhs == NOTHING && *rhs == NOTHING,
                "EasyNothing should be NULL");
    // This is trivially true.
    return true;
}

bool
EasyGenericObject__equal(struct EasyGenericObject const *const lhs,
                         struct EasyGenericObject const *const rhs)
{
    EASY_GUARD(lhs != NULL && rhs != NULL, "pointers should not be NULL");

    if (lhs->type != rhs->type) {
        return false;
    }
    enum EasyGenericType type = lhs->type;

    switch (type) {
    case EASY_TABLE_TYPE:
        return EasyTable__equal(&lhs->data.table, &rhs->data.table);
    case EASY_LIST_TYPE:
        return EasyList__equal(&lhs->data.list, &rhs->data.list);
    case EASY_TEXT_TYPE:
        return EasyText__equal(&lhs->data.text, &rhs->data.text);
    case EASY_INTEGER_TYPE:
        return EasyInteger__equal(&lhs->data.integer, &rhs->data.integer);
    case EASY_FRACTION_TYPE:
        return EasyFraction__equal(&lhs->data.fraction, &rhs->data.fraction);
    case EASY_BOOLEAN_TYPE:
        return EasyBoolean__equal(&lhs->data.boolean, &rhs->data.boolean);
    case EASY_NOTHING_TYPE:
        return EasyNothing__equal(&lhs->data.nothing, &rhs->data.nothing);
    default:
        EASY_IMPOSSIBLE();
    }

    EASY_IMPOSSIBLE();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// TEST EQUAL FUNCTION
////////////////////////////////////////////////////////////////////////////////

#include "common/easy_test.h"
#include "common/easy_unused.h"

static bool
test_equal_easy_table(void)
{
    EASY_NOT_IMPLEMENTED();
    return true;
}

static bool
test_equal_easy_list(void)
{
    EASY_NOT_IMPLEMENTED();
    return true;
}

static bool
test_equal_easy_text(void)
{
    struct EasyText a = EasyText__from_cstr("Hello, World!");
    struct EasyText b = EasyText__from_cstr("Hello, World!");
    struct EasyText c = EasyText__from_cstr("Good-bye, World!");

    uint64_t a_eq_b = EasyText__equal(&a, &b);
    uint64_t a_neq_c = EasyText__equal(&a, &c);

    EASY_TEST_ASSERT_TRUE(a_eq_b);
    EASY_TEST_ASSERT_TRUE(!a_neq_c);

    EasyText__destroy(&a);
    EasyText__destroy(&b);
    EasyText__destroy(&c);
    return true;
}

static bool
test_equal_easy_integer(void)
{
    struct EasyInteger a = EasyInteger__from_cstr("100");
    struct EasyInteger b = EasyInteger__from_cstr("100");
    struct EasyInteger c = EasyInteger__from_cstr("101");

    uint64_t a_eq_b = EasyInteger__equal(&a, &b);
    uint64_t a_neq_c = EasyInteger__equal(&a, &c);

    EASY_TEST_ASSERT_TRUE(a_eq_b);
    EASY_TEST_ASSERT_TRUE(!a_neq_c);

    EasyInteger__destroy(&a);
    EasyInteger__destroy(&b);
    EasyInteger__destroy(&c);
    return true;
}

static bool
test_equal_easy_fraction(void)
{
    EASY_NOT_IMPLEMENTED();
    return true;
}

static bool
test_equal_easy_boolean(void)
{
    enum EasyBoolean a = TRUE;
    enum EasyBoolean b = TRUE;
    enum EasyBoolean c = FALSE;

    uint64_t a_eq_b = EasyBoolean__equal(&a, &b);
    uint64_t a_neq_c = EasyBoolean__equal(&a, &c);

    EASY_TEST_ASSERT_TRUE(a_eq_b);
    EASY_TEST_ASSERT_TRUE(!a_neq_c);

    EasyBoolean__destroy(&a);
    EasyBoolean__destroy(&b);
    EasyBoolean__destroy(&c);
    return true;
}

static bool
test_equal_easy_nothing(void)
{
    EasyNothing a = EasyNothing__new();
    EasyNothing b = EasyNothing__new();

    uint64_t a_eq_b = EasyNothing__equal(&a, &b);

    EASY_TEST_ASSERT_TRUE(a_eq_b);

    EasyNothing__destroy(&a);
    EasyNothing__destroy(&b);
    return true;
}

bool
test_easy_equal(void)
{
    // Mark as potentially unused
    EASY_MAYBE_UNUSED(test_equal_easy_table);
    EASY_MAYBE_UNUSED(test_equal_easy_list);
    EASY_MAYBE_UNUSED(test_equal_easy_text);
    EASY_MAYBE_UNUSED(test_equal_easy_integer);
    EASY_MAYBE_UNUSED(test_equal_easy_fraction);
    EASY_MAYBE_UNUSED(test_equal_easy_boolean);
    EASY_MAYBE_UNUSED(test_equal_easy_nothing);

// Run tests
#if 0
  EASY_TEST_SUCCESS(test_equal_easy_table());
  EASY_TEST_SUCCESS(test_equal_easy_list());
#endif
    EASY_TEST_SUCCESS(test_equal_easy_text());
    EASY_TEST_SUCCESS(test_equal_easy_integer());
#if 0
  EASY_TEST_SUCCESS(test_equal_easy_fraction());
#endif
    EASY_TEST_SUCCESS(test_equal_easy_boolean());
    EASY_TEST_SUCCESS(test_equal_easy_nothing());
    return true;
}
