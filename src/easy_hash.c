#include <bits/stdint-uintn.h>
#include <stddef.h>
#include <stdint.h>

#include "easy_boolean.h"
#include "easy_common.h"
#include "easy_integer.h"
#include "easy_lib.h"

#include "easy_hash.h"
#include "easy_nothing.h"
#include "easy_table_item.h"
#include "easy_text.h"

#define DJB2_INITIAL_SEED 5381ULL

/// @brief  A simple hash function for a memory buffer.
/// @note   Adapted from the DJB2 hash function.
/// @note   My goal is that if we call this hash function on the same
///         buffer, we get the same results even if we divide up the
///         buffer into smaller pieces.
/// Source: http://www.cse.yorku.ca/~oz/hash.html
static inline uint64_t
_djb2_hash(void const *const buffer, size_t const size, uint64_t const seed)
{
    uint64_t hash = seed;
    for (size_t i = 0; i < size; ++i) {
        hash =
            ((hash << 5) + hash) + ((uint8_t *)buffer)[i]; /* hash * 33 + c */
    }
    return hash;
}

/// @note   We need to be smart about this. It we do this naively, then
///         our hash value will depend on the order of the elements
///         rather than the actual elements.
static inline uint64_t
EasyTable__hash(struct EasyTable const *const me)
{
    EASY_GUARD(me != NULL, "me should not be NULL");
    uint64_t hash = 0;
    for (size_t i = 0; i < me->length; ++i) {
        if (me->data[i].valid) {
            uint64_t key_hash = EasyGenericObject__hash(&me->data[i].key);
            uint64_t val_hash = EasyGenericObject__hash(&me->data[i].value);
            // NOTE This is probably really weak for a hash function but we want
            // to make it consistent even if the key-value pairs are scrambled.
            uint64_t obj_hash =
                _djb2_hash(&key_hash, sizeof(key_hash), val_hash);
            hash += obj_hash;
        }
    }
    return hash;
}

static inline uint64_t
EasyList__hash(struct EasyList const *const me)
{
    EASY_GUARD(me != NULL, "me should not be NULL");
    uint64_t hash =
        _djb2_hash(&me->length, sizeof(me->length), DJB2_INITIAL_SEED);
    for (size_t i = 0; i < me->length; ++i) {
        uint64_t obj_hash = EasyGenericObject__hash(&me->data[i]);
        hash = _djb2_hash(&obj_hash, sizeof(obj_hash), hash);
    }
    return hash;
}

static inline uint64_t
EasyText__hash(struct EasyText const *const me)
{
    EASY_GUARD(me != NULL, "me should not be NULL");
    uint64_t hash =
        _djb2_hash(&me->length, sizeof(me->length), DJB2_INITIAL_SEED);
    hash = _djb2_hash(me->data, me->length, hash);
    return hash;
}

static inline uint64_t
EasyInteger__hash(struct EasyInteger const *const me)
{
    EASY_GUARD(me != NULL, "me should not be NULL");
    uint64_t hash =
        _djb2_hash(&me->length, sizeof(me->length), DJB2_INITIAL_SEED);
    hash = _djb2_hash(me->data, me->length, hash);
    return hash;
}

static inline uint64_t
EasyFraction__hash(struct EasyFraction const *const me)
{
    EASY_GUARD(me != NULL, "me should not be NULL");
    uint64_t hash = 0;
    EASY_NOT_IMPLEMENTED();
    return hash;
}

static inline uint64_t
EasyBoolean__hash(enum EasyBoolean const *const me)
{
    EASY_GUARD(me != NULL, "me should not be NULL");
    uint64_t hash = _djb2_hash(me, sizeof(*me), DJB2_INITIAL_SEED);
    return hash;
}

static inline uint64_t
EasyNothing__hash(EasyNothing const *const me)
{
    EASY_GUARD(me != NULL, "me should not be NULL");
    uint64_t hash = _djb2_hash(me, sizeof(*me), DJB2_INITIAL_SEED);
    return hash;
}

uint64_t
EasyGenericObject__hash(struct EasyGenericObject const *const me)
{
    EASY_GUARD(me != NULL, "me should not be NULL");
    uint64_t hash = _djb2_hash(&me->type, sizeof(me->type), DJB2_INITIAL_SEED);
    uint64_t obj_hash = 0;
    switch (me->type) {
    case EASY_TABLE_TYPE:
        obj_hash = EasyTable__hash(&me->data.table);
        hash = _djb2_hash(&obj_hash, sizeof(obj_hash), hash);
        break;
    case EASY_LIST_TYPE:
        obj_hash = EasyList__hash(&me->data.list);
        hash = _djb2_hash(&obj_hash, sizeof(obj_hash), hash);
        break;
    case EASY_TEXT_TYPE:
        obj_hash = EasyText__hash(&me->data.text);
        hash = _djb2_hash(&obj_hash, sizeof(obj_hash), hash);
        break;
    case EASY_INTEGER_TYPE:
        obj_hash = EasyInteger__hash(&me->data.integer);
        hash = _djb2_hash(&obj_hash, sizeof(obj_hash), hash);
        break;
    case EASY_FRACTION_TYPE:
        obj_hash = EasyFraction__hash(&me->data.fraction);
        hash = _djb2_hash(&obj_hash, sizeof(obj_hash), hash);
        break;
    case EASY_BOOLEAN_TYPE:
        obj_hash = EasyBoolean__hash(&me->data.boolean);
        hash = _djb2_hash(&obj_hash, sizeof(obj_hash), hash);
        break;
    case EASY_NOTHING_TYPE:
        obj_hash = EasyNothing__hash(&me->data.nothing);
        hash = _djb2_hash(&obj_hash, sizeof(obj_hash), hash);
        break;
    default:
        EASY_IMPOSSIBLE();
    }

    return hash;
}

////////////////////////////////////////////////////////////////////////////////
/// TEST HASH FUNCTION
////////////////////////////////////////////////////////////////////////////////

#include "common/easy_test.h"
#include "common/easy_unused.h"

static bool
test_hash_easy_table(void)
{
    EASY_NOT_IMPLEMENTED();
    return true;
}

static bool
test_hash_easy_list(void)
{
    EASY_NOT_IMPLEMENTED();
    return true;
}

static bool
test_hash_easy_text(void)
{
    struct EasyText a = EasyText__from_cstr("Hello, World!");
    struct EasyText b = EasyText__from_cstr("Hello, World!");
    struct EasyText c = EasyText__from_cstr("Good-bye, World!");

    uint64_t hash_a = EasyText__hash(&a);
    uint64_t hash_b = EasyText__hash(&b);
    uint64_t hash_c = EasyText__hash(&c);

    EASY_TEST_ASSERT_UINTCMP(hash_a, ==, hash_b);
    EASY_TEST_ASSERT_UINTCMP(hash_a, !=, hash_c);

    EasyText__destroy(&a);
    EasyText__destroy(&b);
    EasyText__destroy(&c);
    return true;
}

static bool
test_hash_easy_integer(void)
{
    struct EasyInteger a = EasyInteger__from_cstr("100");
    struct EasyInteger b = EasyInteger__from_cstr("100");
    struct EasyInteger c = EasyInteger__from_cstr("101");

    uint64_t hash_a = EasyInteger__hash(&a);
    uint64_t hash_b = EasyInteger__hash(&b);
    uint64_t hash_c = EasyInteger__hash(&c);

    EASY_TEST_ASSERT_UINTCMP(hash_a, ==, hash_b);
    EASY_TEST_ASSERT_UINTCMP(hash_a, !=, hash_c);

    EasyInteger__destroy(&a);
    EasyInteger__destroy(&b);
    EasyInteger__destroy(&c);
    return true;
}

static bool
test_hash_easy_fraction(void)
{
    EASY_NOT_IMPLEMENTED();
    return true;
}

static bool
test_hash_easy_boolean(void)
{
    enum EasyBoolean a = TRUE;
    enum EasyBoolean b = TRUE;
    enum EasyBoolean c = FALSE;

    uint64_t hash_a = EasyBoolean__hash(&a);
    uint64_t hash_b = EasyBoolean__hash(&b);
    uint64_t hash_c = EasyBoolean__hash(&c);

    EASY_TEST_ASSERT_UINTCMP(hash_a, ==, hash_b);
    EASY_TEST_ASSERT_UINTCMP(hash_a, !=, hash_c);

    EasyBoolean__destroy(&a);
    EasyBoolean__destroy(&b);
    EasyBoolean__destroy(&c);
    return true;
}

static bool
test_hash_easy_nothing(void)
{
    EasyNothing a = EasyNothing__new();
    EasyNothing b = EasyNothing__new();

    uint64_t hash_a = EasyNothing__hash(&a);
    uint64_t hash_b = EasyNothing__hash(&b);

    EASY_TEST_ASSERT_UINTCMP(hash_a, ==, hash_b);

    EasyNothing__destroy(&a);
    EasyNothing__destroy(&b);
    return true;
}

bool
test_easy_hash(void)
{
    // Mark as potentially unused
    EASY_MAYBE_UNUSED(test_hash_easy_table);
    EASY_MAYBE_UNUSED(test_hash_easy_list);
    EASY_MAYBE_UNUSED(test_hash_easy_text);
    EASY_MAYBE_UNUSED(test_hash_easy_integer);
    EASY_MAYBE_UNUSED(test_hash_easy_fraction);
    EASY_MAYBE_UNUSED(test_hash_easy_boolean);
    EASY_MAYBE_UNUSED(test_hash_easy_nothing);

// Run tests
#if 0
  EASY_TEST_SUCCESS(test_hash_easy_table());
  EASY_TEST_SUCCESS(test_hash_easy_list());
#endif
    EASY_TEST_SUCCESS(test_hash_easy_text());
    EASY_TEST_SUCCESS(test_hash_easy_integer());
#if 0
  EASY_TEST_SUCCESS(test_hash_easy_fraction());
#endif
    EASY_TEST_SUCCESS(test_hash_easy_boolean());
    EASY_TEST_SUCCESS(test_hash_easy_nothing());
    return true;
}
