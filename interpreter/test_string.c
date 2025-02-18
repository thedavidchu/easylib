#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string.h"

int
test_cstr_replace(char const *const str,
                  char const *const target,
                  char const *const replace,
                  char const *const oracle)
{
    char *ans = cstr_replace(str, target, replace);
    if (!cstr_valid_equal(ans, oracle)) {
        printf("Replace: '%s' find '%s' replace with '%s' to get '%s', but "
               "expected '%s'\n",
               str,
               target,
               replace,
               ans,
               oracle);
        assert(cstr_valid_equal(ans, oracle));
        free(ans);
        return -1;
    }
    free(ans);
    return 0;
}

int
test_cstr(void)
{
    char const a[] = "Hello, World!";
    char const b[] = "Good-bye, World!";
    char const c[] = "0123456789";
    char const *d[] = {a, b, c};

    // Sanity check that my testing function is OK!
    assert(cstr_valid_equal(NULL, NULL) == false);
    assert(cstr_valid_equal(a, NULL) == false);
    assert(cstr_valid_equal(NULL, a) == false);
    assert(cstr_valid_equal(a, b) == false);
    assert(cstr_valid_equal(a, a) == true);

    char *A = cstr_dup(a);
    assert(cstr_valid_equal(A, a));
    printf("%s\n", A);

    char *B = cstr_concat(a, b);
    assert(cstr_valid_equal(B, "Hello, World!Good-bye, World!"));
    printf("%s\n", B);

    char *C = cstr_slice(c, 3, 7);
    assert(cstr_valid_equal(C, "3456"));
    printf("%s\n", C);

    char *D = cstr_join("...", 3, d);
    assert(
        cstr_valid_equal(D, "Hello, World!...Good-bye, World!...0123456789"));
    printf("%s\n", D);

    assert(cstr_find("", "") == 0);
    assert(cstr_find("", "1") == SIZE_MAX);
    assert(cstr_find(c, "") == 0);
    assert(cstr_find(c, "0") == 0);
    assert(cstr_find(c, "A") == SIZE_MAX);
    assert(cstr_find(c, "123") == 1);
    assert(cstr_find(c, "89") == 8);
    assert(cstr_find(c, "890") == SIZE_MAX);

    assert(cstr_count("", "") == 1);
    assert(cstr_count("0123456789", "") == 11);
    assert(cstr_count("", "abc") == 0);
    assert(cstr_count("abc", "abc") == 1);
    assert(cstr_count("abc abc", "abc") == 2);
    assert(cstr_count("abc abc abc", "abc") == 3);
    assert(cstr_count("abc abc abc abc", "abc") == 4);
    assert(cstr_count("abc abcabc ababc", "abc") == 4);
    assert(cstr_count("abc abcabc ababc", "abcd") == 0);

    // test_cstr_replace("", "", "", "");
    // test_cstr_replace("", "", "XYZ", "XYZ");
    test_cstr_replace(" ", "", "XYZ", "XYZ XYZ");
    test_cstr_replace("  ", "", "XYZ", "XYZ XYZ XYZ");
    test_cstr_replace("abc", "ab", "XYZ", "XYZc");
    test_cstr_replace("abc abc", "ab", "XYZ", "XYZc XYZc");
    test_cstr_replace("---abc abc---", "ab", "XYZ", "---XYZc XYZc---");
    test_cstr_replace("---abc abc---", "ab", "", "---c c---");

    free(A);
    free(B);
    free(C);
    free(D);
    return 0;
}

int
main(void)
{
    int err = 0;
    if ((err = test_cstr())) {
        perror(strerror(err));
        exit(1);
    }
    printf("OK!\n");
    return 0;
}
