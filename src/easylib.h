/*******************************************************************************
 *  The Easy Library
 *  ================
 *
 *  Purpose
 *  -------
 *  The Easy Library is meant to be easily understood. This goal is to make the
 *  code easily accessible to C Programmers of all levels. Often times, people
 *  care more about functionality for a Minimal Viable Product.
 *
 *  > Premature optimization is the root of all evil! (Donald Knuth)
 *
 *  Design Decisions
 *  ----------------
 *  In the name of easiness, I have made the following design decisions:
 *
 *  1. Needless to say, correctness trumps all
 *      - No segmentation faults, floating point exception errors, or integer overflow
 *      - OK to exit with error (see #5)
 *  2. Simplicity over performance
 *  3. Data types are constant unless explicitly marked as "Mutable"
 *  4. Data is not shared unless explicity marked as "Shared"
 *  5. Simple error handling means printing a debug message and exiting
 *
 *  Common Functions
 *  ----------------
 *  Every object type must implement these functionality:
 *
 *  1. Recursively Copy
 *  2. Recursively Destroy
 *  3. Recursively Pretty Print (c.f. Python's str(...) function)
 *      - In later versions, we will require implementing functions to convert to a C-style string or an EasyText object
 *  4. Recursively Debug Print (c.f. Python's repr(...) function)
 *  5. Recursively Hash
 *  6. Recursively Compare If Equal
 *
 *  Naming Conventions
 *  ------------------
 *  Everything is prefixed with "easy" in some form.
 *
 *  Since I am a Pythonista at heart, we follow Pythonic naming conventions:
 *  - Functions names, struct/union members: lower_snake_case
 *  - Macros, enums members, global constants: UPPER_SNAKE_CASE
 *  - Struct/enum/union names: PascalCase
 *
*******************************************************************************/

#pragma once
#ifndef __EASYLIB_H__
#define __EASYLIB_H__

#include <ctype.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 *  COMMON FUNCTIONS
*******************************************************************************/
void
easy_print_error(char *msg, char *file, int line)
{
    fprintf(stderr, "Error [%s:%d]: %s\n", file, line, msg);
}

/* We explicitly convert the condition to an int so that it will not cause problems for the function call */
#define EASY_ASSERT(condition, msg) easy_assert((condition) ? 1 : 0, msg, __FILE__, __LINE__)
#define EASY_GUARD(condition, msg) easy_assert((condition) ? 1 : 0, msg, __FILE__, __LINE__)
#define EASY_IMPOSSIBLE() easy_assert(0, "impossible!", __FILE__, __LINE__)
#define EASY_NOT_IMPLEMENTED() easy_assert(0, "impossible!", __FILE__, __LINE__)

void
easy_assert(int condition, char *msg, char *file, int line)
{
    if (!condition) {
        easy_print_error(msg, file, line);
        exit(EXIT_FAILURE);
    }
}

#define EASY_ALLOC(nmemb, size)	easy_alloc(nmemb, size, __FILE__, __LINE__)
#define EASY_REALLOC(ptr, nmemb, size)	easy_realloc(ptr, nmemb, size, __FILE__, __LINE__)
#define EASY_FREE(ptr)		easy_free(ptr, __FILE__, __LINE__)

void *
easy_alloc(size_t nmemb, size_t size, char *file, int line)
{
    void *ptr = calloc(num, size);
    if (ptr == NULL) {
        easy_print_error("out of memory", file, line);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void *
easy_realloc(void *ptr, size_t nmemb, size_t size, char *file, int line)
{
    ASSERT_GUARD(nmemb > 0 && size > 0, "nmemb and size should be positive");
    ASSERT_GUARD(nmemb > SIZE_MAX / size, "overflow");
    const size_t new_size = nmemb * size;
    void *new_ptr = realloc(ptr, new_size);
    if (new_ptr == NULL && new_size > 0) {	/* What if new_size == 0? */
        easy_print_error("out of memory", file, line);
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

void
easy_free(void *ptr, char *file, int line)
{
    if (ptr == NULL) {
        easy_print_error("freeing null pointer", file, line);
        exit(EXIT_FAILURE);
    }
    free(ptr);
}

/*******************************************************************************
 *  GENERIC LIBRARY DATA STRUCTURES
*******************************************************************************/
/* Stand-alone types */
enum EasyBoolean;	/* We want this to evalutate to True/False as expected */
typedef void * EasyNothing;	/* We want this to act as NULL in C */
struct EasyText;
struct EasyInteger;

/* Composite types */
struct EasyTable;
struct EasyList;
struct EasyFraction;

/* Generic types */
enum EasyGenericType;
union EasyGenericData;
struct EasyGenericObject;


/* EasyTable */
struct EasyTable {
    struct EasyTableItem *data;
    size_t length;    /* The number of elements in the EasyTable */
    size_t capacity;  /* The maximum number of elements in the EasyTable */
};

/* EasyList */
struct EasyList {
    struct EasyGenericObject *data;
    size_t length;
};

/* EasyBoolean */
enum EasyBoolean { FALSE = 0, TRUE = 1 };

/* EasyNothing */
const EasyNothing NOTHING = NULL;

/* EasyText */
struct EasyText {
    char *data;
    size_t length;	/* Not including the NIL byte at the end */
};

/* EasyInteger */
enum EasyIntegerSign { NEGATIVE = -1, ZERO = 0, POSITIVE = +1 };

struct EasyInteger {
    enum EasyIntegerSign sign;
    unsigned char *data;
    size_t length;	/* Length of the data buffer -- no terminal NIL byte */
};

/* EasyFraction */
struct EasyFraction {
    struct EasyInteger numerator;
    struct EasyInteger denominator;
};

/* Generic Object */
enum EasyGenericType {
    EASY_TABLE_TYPE, EASY_LIST_TYPE, EASY_TEXT_TYPE,
    EASY_INTEGER_TYPE, EASY_FRACTION_TYPE,
    EASY_BOOLEAN_TYPE, EASY_NOTHING_TYPE,
};

union EasyGenericData {
    struct EasyTable table;
    struct EasyList list;
    struct EasyText text;
    struct EasyInteger integer;
    struct EasyFraction fraction;
    enum EasyBoolean boolean;
    EasyNothing nothing;
};

struct EasyGenericObject {
    enum EasyGenericType type;
    union EasyGenericData data;
};

/* NOTE These need to come after the EasyGenericObject */
struct EasyTableItem {
    enum EasyBoolean valid;
    struct EasyGenericObject key;
    struct EasyGenericObject value;
};

/*******************************************************************************
 *  GENERIC LIBRARY IMPLEMENTATION
*******************************************************************************/

/* TODO(dchu) */
void
EasyGenericType__print_json(enum EasyGenericType *me)
{
    switch (*me) {
    case EASY_TABLE_TYPE:
        printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_TABLE_TYPE\", \"number\": %d}", *me);
        break;
    case EASY_LIST_TYPE:
        printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_LIST_TYPE\", \"number\": %d}", *me);
        printf("EASY_LIST_TYPE");
        break;
    case EASY_TEXT_TYPE:
        printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_TEXT_TYPE\", \"number\": %d}", *me);
        printf("EASY_TEXT_TYPE");
        break;
    case EASY_INTEGER_TYPE:
        printf("EASY_INTEGER_TYPE");
        printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_INTEGER_TYPE\", \"number\": %d}", *me);
        break;
    case EASY_FRACTION_TYPE:
        printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_FRACTION_TYPE\", \"number\": %d}", *me);
        printf("EASY_FRACTION_TYPE");
        break;
    case EASY_BOOLEAN_TYPE:
        printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_BOOLEAN_TYPE\", \"number\": %d}", *me);
        printf("EASY_BOOLEAN_TYPE");
        break;
    case EASY_NOTHING_TYPE:
        printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_NOTHING_TYPE\", \"number\": %d}", *me);
        break;
    default:
        EASY_IMPOSSIBLE();
    }
}

void
EasyTable__print_json(struct EasyTable *me)
{
    EASY_GUARD(me != NULL, "pointer must not be NULL");
    printf("{\"type\": \"EasyTable\", ...}");
}

void
EasyList__print_json(struct EasyList *me)
{
    EASY_GUARD(me != NULL, "pointer must not be NULL");
    printf("{\"type\": \"EasyList\", ...}");
}

void
EasyText__print_json(struct EasyText *me)
{
    EASY_GUARD(me != NULL, "pointer must not be NULL");
    printf("{\"type\": \"EasyText\", ...}");
}

void
EasyInteger__print_json(struct EasyInteger *me)
{
    EASY_GUARD(me != NULL, "pointer must not be NULL");
    printf("{\"type\": \"EasyInteger\", ...}");
}

void
EasyFraction__print_json(struct EasyFraction *me)
{
    EASY_GUARD(me != NULL, "pointer must not be NULL");
    printf("{\"type\": \"EasyFraction\", ...}");
}

void
EasyBoolean__print_json(enum EasyBoolean *me)
{
    EASY_GUARD(me != NULL, "pointer must not be NULL");
    printf("{\"type\": \"EasyBoolean\", ...}");
}

void
EasyNothing__print_json(EasyNothing *me)
{
    EASY_GUARD(me != NULL, "pointer must not be NULL");
    EASY_GUARD(*me != NULL, "pointer must be NULL");
    printf("{\"type\": \"EasyNothing\", \"data\": \"%p\"}", *me);
}

void
EasyGenericObject__print_json(struct EasyGenericObject *me)
{
    EASY_GUARD(me != NULL, "pointer must not be NULL");
    printf("{\"type\": \"EasyGenericObject\", \".type\": ");
    EasyGenericType__print_json(&me->type);
    printf(", \".data\": ");
    switch (me->type) {
    case EASY_TABLE_TYPE:
        EasyTable__print_json(&me->data.table);
        break;
    case EASY_LIST_TYPE:
        EasyList__print_json(&me->data.list);
        break;
    case EASY_TEXT_TYPE:
        EasyText__print_json(&me->data.text);
        break;
    case EASY_INTEGER_TYPE:
        EasyInteger__print_json(&me->data.integer);
        break;
    case EASY_FRACTION_TYPE:
        EasyFraction__print_json(&me->data.fraction);
        break;
    case EASY_BOOLEAN_TYPE:
        EasyBoolean__print_json(&me->data.boolean);
        break;
    case EASY_NOTHING_TYPE:
        EasyNothing__print_json(&me->data.nothing);
        break;
    default:
        EASY_IMPOSSIBLE();
    }
    printf("}");
}

/* EasyInteger */
/** Convert a C-style string to an EasyInteger. */
struct EasyInteger
EasyInteger__from_cstr(char const *str)
{
    struct EasyInteger me = {0};
    size_t num_char = strlen(str);
    size_t start_i = 0;
    size_t num_digits = 0;

    switch (str[0]) {
    case '0':
        EASY_ASSERT(num_char == 1, "the only valid string beginning with a '0' is \"0\"");
        me.sign = ZERO;
        me.data = EASY_ALLOC(1, sizeof(char));
        me.length = 1;
        return;
    case '-':
        me.sign = NEGATIVE;
        me.data = EASY_ALLOC(num_char - 1, sizeof(char));
        me.length = num_char - 1;
        start_i = 1;
        num_digits = num_char - 1;
        break;
    case '+':
        me.sign = POSITIVE;
        me.data = EASY_ALLOC(num_char - 1, sizeof(char));
        me.length = num_char - 1;
        start_i = 1;
        num_digits = num_char - 1;
        break;
    default:
        EASY_ASSERT(isdigit(str[0]), "the string must begin with \"[+-0-9]\"")
        me.sign = POSITIVE;
        me.data = EASY_ALLOC(num_char, sizeof(char));
        me.length = num_char;
        start_i = 0;
        num_digits = num_char;
        break;
    }

    for (size_t i = start_i; i < num_char; ++i) {
        /* Flip digits */
        me.data[num_digits - i] = str[i] - '0';
    }
    return me;
}

struct EasyInteger
EasyInteger__add(struct EasyInteger *a, struct EasyInteger *b)
{
    EASY_GUARD(a != NULL && b != NULL, "inputs must be non-null");
    if (a->sign == ZERO) {
        // TODO
        EASY_NOT_IMPLEMENTED();
    } else if (b->sign == ZERO) {
        // TODO
        EASY_NOT_IMPLEMENTED();
    } else if (a->sign == b->sign) {
        // TODO
        EASY_NOT_IMPLEMENTED();
    } else {
        // TODO
        EASY_NOT_IMPLEMENTED();
    }
}

#endif /* !__EASYLIB_H__ */
