/** Test file */

#include <stdbool.h>
#include <stdio.h>

#include "common/easy_logger.h"
#include "common/easy_test.h"
#include "easy_boolean.h"
#include "easy_equal.h"
#include "easy_error.h"
#include "easy_hash.h"
#include "easy_integer.h"
#include "easy_lib.h"
#include "easy_list.h"
#include "easy_table.h"
#include "easy_text.h"

void
print_green_ok(struct EasyLogger logger)
{
    EASY_LOGGER_INFO(logger, "\033[32m--- OK ---\n\033[0m");
}

bool
test_easy_boolean(void)
{
    enum EasyBoolean a = TRUE;
    enum EasyBoolean b = FALSE;

    EasyBoolean__print(&a);
    printf("\n");
    EasyBoolean__print(&b);
    printf("\n");

    EasyBoolean__print_json(&a);
    printf("\n");
    EasyBoolean__print_json(&b);
    printf("\n");

    return true;
}

bool
test_easy_integer(void)
{
    /* Test basic addition: a + b = c */
    struct EasyInteger a = EasyInteger__from_cstr("+1000000");
    struct EasyInteger b = EasyInteger__from_cstr("+100");
    struct EasyInteger c = EasyInteger__add(&a, &b);

    EasyInteger__print(&a);
    printf(" + ");
    EasyInteger__print(&b);
    printf(" = ");
    EasyInteger__print(&c);
    printf("\n");
    EasyInteger__print_json(&c);
    printf("\n");

    /* Test addition carrying */
    struct EasyInteger d = EasyInteger__from_cstr("+9999");
    struct EasyInteger e = EasyInteger__from_cstr("+999");
    struct EasyInteger f = EasyInteger__add(&d, &e);

    EasyInteger__print(&d);
    printf(" + ");
    EasyInteger__print(&e);
    printf(" = ");
    EasyInteger__print(&f);
    printf("\n");
    EasyInteger__print_json(&f);
    printf("\n");

    /* Test subtraction borrowing */
    struct EasyInteger g = EasyInteger__from_cstr("+1000");
    struct EasyInteger h = EasyInteger__from_cstr("-999");
    struct EasyInteger i = EasyInteger__add(&g, &h);

    EasyInteger__print(&g);
    printf(" + ");
    EasyInteger__print(&h);
    printf(" = ");
    EasyInteger__print(&i);
    printf("\n");
    EasyInteger__print_json(&i);
    printf("\n");

    /* Test simple multiplication */
    struct EasyInteger j = EasyInteger__from_cstr("9999");
    struct EasyInteger k = EasyInteger__from_cstr("99");
    struct EasyInteger l = EasyInteger__multiply(&j, &k);

    EasyInteger__print(&j);
    printf(" * ");
    EasyInteger__print(&k);
    printf(" = ");
    EasyInteger__print(&l);
    printf("\n");
    EasyInteger__print_json(&l);
    printf("\n");

    /* Destroy all integers */
    EasyInteger__destroy(&a);
    EasyInteger__destroy(&b);
    EasyInteger__destroy(&c);
    EasyInteger__destroy(&d);
    EasyInteger__destroy(&e);
    EasyInteger__destroy(&f);
    EasyInteger__destroy(&g);
    EasyInteger__destroy(&h);
    EasyInteger__destroy(&i);
    EasyInteger__destroy(&j);
    EasyInteger__destroy(&k);
    EasyInteger__destroy(&l);

    return true;
}

bool
test_easy_text(void)
{
    struct EasyText a = EasyText__from_cstr("ABC");
    EasyText__print(&a);
    printf("\n");
    EasyText__print_json(&a);
    printf("\n");

    unsigned char all_char[256] = {
        1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,
        15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,
        29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,
        43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,
        57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,
        71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,
        85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,
        99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
        113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
        127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140,
        141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154,
        155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168,
        169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182,
        183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196,
        197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
        211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224,
        225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238,
        239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
        253, 254, 255, '\0'};
    struct EasyText b = EasyText__from_cstr((char *)all_char);
    EasyText__print(&b);
    printf("\n");
    EasyText__print_json(&b);
    printf("\n");

    EasyText__destroy(&a);
    EasyText__destroy(&b);

    return true;
}

bool
test_easy_list(void)
{
    struct EasyList a = EasyList__new_empty();

    EasyList__print(&a);
    printf("\n");
    EasyList__print_json(&a);
    printf("\n");

    struct EasyGenericObject x = {
        .type = EASY_INTEGER_TYPE,
        .data = {.integer = EasyInteger__from_cstr("10")}};
    struct EasyList b = EasyList__append(&a, &x);

    EasyList__print(&b);
    printf("\n");
    EasyList__print_json(&b);
    printf("\n");

    struct EasyGenericObject y = {
        .type = EASY_INTEGER_TYPE,
        .data = {.integer = EasyInteger__from_cstr("11")}};
    struct EasyList c = EasyList__append(&b, &y);

    EasyList__print(&c);
    printf("\n");
    EasyList__print_json(&c);
    printf("\n");

    struct EasyGenericObject z = EasyList__lookup(&c, 1);

    EasyList__print(&c);
    printf("[1] = ");
    EasyGenericObject__print(&z);
    printf("\n");

    struct EasyList d = EasyList__remove(&c, 0);

    EasyList__print(&d);
    printf("\n");

    EasyList__destroy(&a);
    EasyList__destroy(&b);
    EasyList__destroy(&c);
    EasyList__destroy(&d);
    EasyGenericObject__destroy(&x);
    EasyGenericObject__destroy(&y);
    EasyGenericObject__destroy(&z);

    return true;
}

bool
test_easy_table(void)
{
    struct EasyTable a = EasyTable__new_empty();

    EasyTable__print(&a);
    printf("\n");
    EasyTable__print_json(&a);
    printf("\n");

    struct EasyGenericObject i = {
        .type = EASY_INTEGER_TYPE,
        .data = {.integer = EasyInteger__from_cstr("10")}};
    struct EasyGenericObject j = {
        .type = EASY_INTEGER_TYPE,
        .data = {.integer = EasyInteger__from_cstr("11")}};
    struct EasyGenericObject k = {
        .type = EASY_INTEGER_TYPE,
        .data = {.integer = EasyInteger__from_cstr("12")}};
    struct EasyGenericObject l = {
        .type = EASY_INTEGER_TYPE,
        .data = {.integer = EasyInteger__from_cstr("0")}};

    struct EasyTable b = EasyTable__insert(&a, &i, &j);
    struct EasyTable c = EasyTable__insert(&b, &j, &k);
    struct EasyTable d = EasyTable__insert(&c, &k, &l);

    EasyTable__print(&b);
    printf("\n");
    EasyTable__print(&c);
    printf("\n");
    EasyTable__print(&d);
    printf("\n");

    // Test lookup
    struct EasyGenericObject m = EasyTable__lookup(&d, &i);
    struct EasyGenericObject n = EasyTable__lookup(&d, &j);
    struct EasyGenericObject o = EasyTable__lookup(&d, &k);
    struct EasyGenericObject p = EasyTable__lookup(&d, &l);

    EasyGenericObject__print(&m);
    printf(" == ");
    EasyGenericObject__print(&j);
    printf("\n");

    EasyGenericObject__print(&n);
    printf(" == ");
    EasyGenericObject__print(&k);
    printf("\n");

    EasyGenericObject__print(&o);
    printf(" == ");
    EasyGenericObject__print(&l);
    printf("\n");

    EasyGenericObject__print(&p);
    printf(" == null\n");

    // Test remove
    struct EasyTable e = EasyTable__remove(&d, &i);
    struct EasyTable f = EasyTable__remove(&e, &j);
    struct EasyTable g = EasyTable__remove(&f, &k);
    struct EasyTable h = EasyTable__remove(&g, &l);

    EasyTable__print(&e);
    printf(" == {11: 12, 12: 0, }\n");
    EasyTable__print(&f);
    printf(" == {12: 0, }\n");
    EasyTable__print(&g);
    printf(" == {}\n");
    EasyTable__print(&h);
    printf(" == {}\n");

    // Cleanup
    EasyTable__destroy(&a);
    EasyTable__destroy(&b);
    EasyTable__destroy(&c);
    EasyTable__destroy(&d);
    EasyTable__destroy(&e);
    EasyTable__destroy(&f);
    EasyTable__destroy(&g);
    EasyTable__destroy(&h);

    EasyGenericObject__destroy(&i);
    EasyGenericObject__destroy(&j);
    EasyGenericObject__destroy(&k);
    EasyGenericObject__destroy(&l);
    EasyGenericObject__destroy(&m);
    EasyGenericObject__destroy(&n);
    EasyGenericObject__destroy(&o);
    EasyGenericObject__destroy(&p);
    return true;
}

bool
test_easy_error(void)
{
    struct EasyError a = EasyError__from_errno(0);
    struct EasyError b = EasyError__from_errno(1);
    struct EasyError c = EasyError__from_errno(2);
    struct EasyError d = EasyError__from_errno(3);
    struct EasyError e = EasyError__from_errno(4);
    struct EasyError f = EasyError__from_errno(5);

    EasyError__print_json(&a);
    printf("\n");
    EasyError__print_json(&b);
    printf("\n");
    EasyError__print_json(&c);
    printf("\n");
    EasyError__print_json(&d);
    printf("\n");
    EasyError__print_json(&e);
    printf("\n");
    EasyError__print_json(&f);
    printf("\n");

    EasyError__destroy(&a);
    EasyError__destroy(&b);
    EasyError__destroy(&c);
    EasyError__destroy(&d);
    EasyError__destroy(&e);
    EasyError__destroy(&f);

    return true;
}

int
main(void)
{
    // Test types
    EASY_TEST_SUCCESS(test_easy_boolean());
    EASY_TEST_SUCCESS(test_easy_integer());
    EASY_TEST_SUCCESS(test_easy_text());
    EASY_TEST_SUCCESS(test_easy_list());
    EASY_TEST_SUCCESS(test_easy_table());

    // Test Sort-of-Types
    EASY_TEST_SUCCESS(test_easy_error());

    // Test functions
    EASY_TEST_SUCCESS(test_easy_hash());
    EASY_TEST_SUCCESS(test_easy_equal());
    return 0;
}
