#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string.h"

int
main()
{
    char a[] = "Hello, World!";
    char b[] = "Good-bye, World!";
    char c[] = "0123456789";

    char *d = cstr_dup(a);
    printf("%s\n", d);

    char *e = cstr_concat(a, b);
    printf("%s\n", e);

    char *f = cstr_slice(c, 3, 7);
    printf("%s\n", f);

    free(d);
    free(e);
    free(f);
    return 0;
}
