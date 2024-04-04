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

#include "easycommon.h"

/*******************************************************************************
 *  GENERIC LIBRARY DATA STRUCTURES
 ******************************************************************************/
/* Stand-alone types */
typedef void *EasyNothing; /* We want this to act as NULL in C */
struct EasyText;
struct EasyInteger;

/* Composite types */
struct EasyTable;
struct EasyList;
struct EasyFraction;

/* Generic types */
union EasyGenericData;
struct EasyGenericObject;

/* EasyTable */
struct EasyTable {
  struct EasyTableItem *data;
  size_t length;   /* The number of elements in the EasyTable */
  size_t capacity; /* The maximum number of elements in the EasyTable */
};

/* EasyList */
struct EasyList {
  struct EasyGenericObject *data;
  size_t length;
};

/* EasyBoolean */
enum EasyBoolean {
  FALSE = 0,
  TRUE = 1
}; /* We want this to behave as a normal boolean */

/* EasyNothing */
const EasyNothing NOTHING = NULL;

/* EasyText */
struct EasyText {
  char *data;
  size_t length; /* Not including the NIL byte at the end */
};

/* EasyInteger */
enum EasyIntegerSign { NEGATIVE = -1, ZERO = 0, POSITIVE = +1 };

struct EasyInteger {
  enum EasyIntegerSign sign;
  signed char *data;
  size_t length; /* Length of the data buffer -- no terminal NIL byte */
};

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

/* NOTE These need to come after the EasyGenericObject */
struct EasyTableItem {
  enum EasyBoolean valid;
  struct EasyGenericObject key;
  struct EasyGenericObject value;
};

/*******************************************************************************
 *  GENERIC LIBRARY IMPLEMENTATION
 ******************************************************************************/

/* TODO(dchu) */
void EasyGenericType__print_json(enum EasyGenericType *me) {
  switch (*me) {
  case EASY_TABLE_TYPE:
    printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_TABLE_TYPE\", "
           "\"number\": %d}",
           *me);
    break;
  case EASY_LIST_TYPE:
    printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_LIST_TYPE\", "
           "\"number\": %d}",
           *me);
    printf("EASY_LIST_TYPE");
    break;
  case EASY_TEXT_TYPE:
    printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_TEXT_TYPE\", "
           "\"number\": %d}",
           *me);
    printf("EASY_TEXT_TYPE");
    break;
  case EASY_INTEGER_TYPE:
    printf("EASY_INTEGER_TYPE");
    printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_INTEGER_TYPE\", "
           "\"number\": %d}",
           *me);
    break;
  case EASY_FRACTION_TYPE:
    printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_FRACTION_TYPE\", "
           "\"number\": %d}",
           *me);
    printf("EASY_FRACTION_TYPE");
    break;
  case EASY_BOOLEAN_TYPE:
    printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_BOOLEAN_TYPE\", "
           "\"number\": %d}",
           *me);
    printf("EASY_BOOLEAN_TYPE");
    break;
  case EASY_NOTHING_TYPE:
    printf("{\"type\": \"EasyGenericType\", \"text\": \"EASY_NOTHING_TYPE\", "
           "\"number\": %d}",
           *me);
    break;
  default:
    EASY_IMPOSSIBLE();
  }
}

void EasyTable__print_json(struct EasyTable *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  printf("{\"type\": \"EasyTable\", ...}");
}

void EasyList__print_json(struct EasyList *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  printf("{\"type\": \"EasyList\", ...}");
}

void EasyText__print_json(struct EasyText *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  printf("{\"type\": \"EasyText\", ...}");
}

void EasyInteger__print_json(struct EasyInteger *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  printf("{\"type\": \"EasyInteger\", ...}");
}

void EasyFraction__print_json(struct EasyFraction *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  printf("{\"type\": \"EasyFraction\", ...}");
}

void EasyBoolean__print_json(enum EasyBoolean *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  printf("{\"type\": \"EasyBoolean\", ...}");
}

void EasyNothing__print_json(EasyNothing *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  EASY_GUARD(*me != NULL, "pointer must be NULL");
  printf("{\"type\": \"EasyNothing\", \"data\": \"%p\"}", *me);
}

void EasyGenericObject__print_json(struct EasyGenericObject *me) {
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
struct EasyInteger EasyInteger__from_cstr(char const *str) {
  struct EasyInteger me = {0};
  char const *digit_str = NULL;
  size_t num_char = strlen(str);
  size_t num_digits = 0;

  switch (str[0]) {
  case '0':
    EASY_ASSERT(num_char == 1,
                "the only valid string beginning with a '0' is \"0\"");
    me.sign = ZERO;
    me.data = EASY_ALLOC(1, sizeof(char));
    me.data[0] = 0;
    me.length = 1;
    return me;
  case '-':
    me.sign = NEGATIVE;
    me.data = EASY_ALLOC(num_char - 1, sizeof(char));
    me.length = num_char - 1;
    digit_str = &str[1];
    num_digits = num_char - 1;
    break;
  case '+':
    me.sign = POSITIVE;
    me.data = EASY_ALLOC(num_char - 1, sizeof(char));
    me.length = num_char - 1;
    digit_str = &str[1];
    num_digits = num_char - 1;
    break;
  default:
    EASY_GUARD(isdigit(str[0]), "the string must begin with \"[+-0-9]\"");
    me.sign = POSITIVE;
    me.data = EASY_ALLOC(num_char, sizeof(char));
    me.length = num_char;
    digit_str = &str[0];
    num_digits = num_char;
    break;
  }

  for (size_t i = 0; i < num_digits; ++i) {
    /* Flip digits */
    EASY_GUARD(isdigit(digit_str[i]), "the string must begin with \"[0-9]\"");
    me.data[num_digits - 1 - i] = digit_str[i] - '0';
  }
  return me;
}

struct EasyInteger EasyInteger__copy(struct EasyInteger *me) {
  EASY_GUARD(me != NULL, "inputs must be non-null");
  struct EasyInteger copy = *me;
  /* Deep copy data so that the owner is clear */
  copy.data = EASY_DUPLICATE(me->data, copy.length, sizeof(*copy.data));
  return copy;
}

#define NUM_INTEGER_DIGITS 10

/** Helper function to find the larger absolute value */
int compare_absolute_integer(struct EasyInteger *a, struct EasyInteger *b) {
  EASY_GUARD(a != NULL && a->data != NULL, "inputs must be non-null");
  EASY_GUARD(b != NULL && b->data != NULL, "inputs must be non-null");
  if (a->length > b->length) {
    return 1;
  } else if (b->length > a->length) {
    return -1;
  }

  /* Starting from the most significant digit, work backwards until you find a
   * difference */
  for (size_t i = 0; i < a->length; ++i) {
    uint8_t a_ = a->data[EASY_REVERSE_INDEX(a->length, i)];
    uint8_t b_ = b->data[EASY_REVERSE_INDEX(b->length, i)];

    if (a_ > b_) {
      return 1;
    } else if (b_ > a_) {
      return -1;
    }
  }

  /* Return NULL if they are equal */
  return 0;
}

struct EasyInteger EasyInteger__add(struct EasyInteger *a,
                                    struct EasyInteger *b) {
  EASY_GUARD(a != NULL && a->data != NULL, "inputs must be non-null");
  EASY_GUARD(b != NULL && b->data != NULL, "inputs must be non-null");
  struct EasyInteger me = {0};
  if (a->sign == ZERO && b->sign == ZERO) {
    /* This case could be subsumed by the next two cases */
    return EasyInteger__from_cstr("0");
  } else if (a->sign == ZERO) {
    return EasyInteger__copy(b);
  } else if (b->sign == ZERO) {
    return EasyInteger__copy(a);
  } else if (a->sign == b->sign) {
    size_t max_ans_length = MAX(a->length, b->length) + 1;

    me.sign = a->sign; /* Sign matches both a and b */
    me.data = EASY_ALLOC(max_ans_length, sizeof(*me.data));
    me.length = 0; /* This will be iteratively lengthened */
    signed char carry = 0;
    /* Importantly, max_ans_length > MAX(a->length, b->length)! */
    for (size_t i = 0; i < max_ans_length; ++i) {
      signed char a_ = i < a->length ? a->data[i] : 0;
      signed char b_ = i < b->length ? b->data[i] : 0;

      if (a_ + b_ + carry >= NUM_INTEGER_DIGITS) {
        me.data[i] = a_ + b_ + carry - NUM_INTEGER_DIGITS;
        carry = 1;
      } else {
        me.data[i] = a_ + b_ + carry;
        carry = 0;
      }

      if (me.data[i] > 0) {
        me.length = i + 1;
      }
    }
    return me;
  } else {
    size_t max_ans_length = MAX(a->length, b->length);
    me.data = EASY_ALLOC(max_ans_length, sizeof(*me.data));
    me.length = 0; /* This will be iteratively lengthened */

    struct EasyInteger *larger_int = NULL, *smaller_int = NULL;
    switch (compare_absolute_integer(a, b)) {
    case 0:
      return EasyInteger__from_cstr("0");
    case -1:
      larger_int = b;
      smaller_int = a;
      break;
    case +1:
      larger_int = a;
      smaller_int = b;
      break;
    default:
      EASY_IMPOSSIBLE();
    }

    me.sign = larger_int->sign;
    me.data = EASY_ALLOC(max_ans_length, sizeof(*me.data));

    signed char borrow = 0;

    /* Importantly, max_ans_length == MAX(a->length, b->length)! */
    for (size_t i = 0; i < max_ans_length; ++i) {
      signed char big_ = i < larger_int->length ? larger_int->data[i] : 0;
      signed char small_ = i < smaller_int->length ? smaller_int->data[i] : 0;

      if (big_ - small_ - borrow < 0) {
        me.data[i] = big_ - small_ - borrow + NUM_INTEGER_DIGITS;
        borrow = 1;
      } else {
        me.data[i] = big_ - small_ - borrow;
        borrow = 0;
      }

      if (me.data[i] > 0) {
        me.length = i + 1;
      }
      EASY_ASSERT(0 <= me.data[i] && me.data[i] < NUM_INTEGER_DIGITS,
                  "digits must be in range!");
    }
    return me;
  }
}

struct EasyInteger EasyInteger__multiply(struct EasyInteger *a, struct EasyInteger *b) {
  EASY_GUARD(a != NULL && a->data != NULL, "inputs must be non-null");
  EASY_GUARD(b != NULL && b->data != NULL, "inputs must be non-null");

  struct EasyInteger me = {0};

  if (a->sign == ZERO || b->sign == ZERO) {
    return EasyInteger__from_cstr("0");
  } else if (a->sign == b->sign) {
    me.sign = POSITIVE;
  } else {
    me.sign = NEGATIVE;
  }

  const size_t max_ans_length = a->length + b->length;
  /* Relies on the fact that it zeroes the array */
  me.data = EASY_ALLOC(max_ans_length, sizeof(*me.data));
  EASY_SET_POINTER_ARRAY_ZERO(me.data, max_ans_length);
  for (size_t i = 0; i < a->length; ++i) {
    for (size_t j = 0; j < b->length; ++j) {
      signed char product = a->data[i] * b->data[j] + me.data[i + j];
      if (product >= NUM_INTEGER_DIGITS) {
        me.data[i + j] = product % NUM_INTEGER_DIGITS;
        me.data[i + j + 1] += product / NUM_INTEGER_DIGITS;
      } else {
        me.data[i + j] = product;
      }
    }
  }

  /* Set length */
  for (size_t i = 0; i < max_ans_length; ++i) {
    size_t idx = EASY_REVERSE_INDEX(max_ans_length, i);
    signed char digit = me.data[idx];
    if (digit != 0) {
      EASY_ASSERT(0 <= digit && digit < NUM_INTEGER_DIGITS, "must be in range");
      me.length = idx + 1;
      break;
    }
  }

  /* Assert no overflows */
  for (size_t i = 0; i < max_ans_length; ++i) {
    size_t idx = EASY_REVERSE_INDEX(max_ans_length, i);
    signed char digit = me.data[idx];
    EASY_ASSERT(0 <= digit && digit < NUM_INTEGER_DIGITS, "must be in range");
  }

  return me;
}

void EasyInteger__print(struct EasyInteger *me) {
  EASY_GUARD(me != NULL && me->data != NULL, "input should be non-null");
  if (me->sign == NEGATIVE) {
    printf("-");
  }
  for (size_t i = 0; i < me->length; ++i) {
    printf("%c", '0' + me->data[me->length - 1 - i]);
  }
}

void EasyInteger__destroy(struct EasyInteger *me) {
  EASY_GUARD(me != NULL && me->data != NULL, "input should be non-null");
  EASY_FREE(me->data);

  EASY_SET_ZERO(me);
}

#endif /* !EASYLIB_H */
