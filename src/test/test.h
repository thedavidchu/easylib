#pragma once

#include <stdlib.h>

#define TEST_ASSERT_OK(expr)                                                   \
    do {                                                                       \
        int const err = (expr);                                                \
        if (err) { /* Recall that '0' indicates OK. */                         \
            printf("%s:%d - error code %d\n", __FILE__, __LINE__, err);        \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
    } while (0)
