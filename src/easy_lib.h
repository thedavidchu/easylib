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
 *      - No segmentation faults, floating point exception errors, or integer
 *        overflow
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
 *      - In later versions, we will require implementing functions to convert
 *        to a C-style string or an EasyText object
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
 ******************************************************************************/

#pragma once
#ifndef EASYLIB_H
#define EASYLIB_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 *  GENERIC LIBRARY DATA STRUCTURES
 ******************************************************************************/
/* Generic types */
union EasyGenericData;
struct EasyGenericObject;

#include "easy_boolean.h"
#include "easy_integer.h"
#include "easy_list.h"
#include "easy_nothing.h"
#include "easy_table.h"
#include "easy_text.h"

/* EasyFraction */
struct EasyFraction {
    struct EasyInteger numerator;
    struct EasyInteger denominator;
};

/* Generic Object */
enum EasyGenericType {
    EASY_TABLE_TYPE,
    EASY_LIST_TYPE,
    EASY_TEXT_TYPE,
    EASY_INTEGER_TYPE,
    EASY_FRACTION_TYPE,
    EASY_BOOLEAN_TYPE,
    EASY_NOTHING_TYPE,
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

/*******************************************************************************
 *  GENERIC LIBRARY IMPLEMENTATION
 ******************************************************************************/

void
EasyGenericType__print_json(enum EasyGenericType const *const me);

void
EasyFraction__print_json(struct EasyFraction const *const me);

void
EasyGenericObject__print_json(struct EasyGenericObject const *const me);

void
EasyFraction__print(struct EasyFraction const *const me);

void
EasyGenericObject__print(struct EasyGenericObject const *const me);

struct EasyFraction
EasyFraction__copy(struct EasyFraction const *const me);

struct EasyGenericObject
EasyGenericObject__copy(struct EasyGenericObject const *const me);

void
EasyGenericObject__destroy(struct EasyGenericObject *const me);

#endif /* !EASYLIB_H */
