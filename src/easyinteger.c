/* EasyInteger */

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "easycommon.h"

#include "easyinteger.h"

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
    me.data = EASY_CALLOC(1, sizeof(char));
    me.data[0] = 0;
    me.length = 1;
    return me;
  case '-':
    me.sign = NEGATIVE;
    me.data = EASY_CALLOC(num_char - 1, sizeof(char));
    me.length = num_char - 1;
    digit_str = &str[1];
    num_digits = num_char - 1;
    break;
  case '+':
    me.sign = POSITIVE;
    me.data = EASY_CALLOC(num_char - 1, sizeof(char));
    me.length = num_char - 1;
    digit_str = &str[1];
    num_digits = num_char - 1;
    break;
  default:
    EASY_GUARD(isdigit(str[0]), "the string must begin with \"[+-0-9]\"");
    me.sign = POSITIVE;
    me.data = EASY_CALLOC(num_char, sizeof(char));
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
static int compare_absolute_integer(struct EasyInteger *a,
                                    struct EasyInteger *b) {
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
    me.data = EASY_CALLOC(max_ans_length, sizeof(*me.data));
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
    me.data = EASY_CALLOC(max_ans_length, sizeof(*me.data));
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
    me.data = EASY_CALLOC(max_ans_length, sizeof(*me.data));

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

struct EasyInteger EasyInteger__multiply(struct EasyInteger *a,
                                         struct EasyInteger *b) {
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
  me.data = EASY_CALLOC(max_ans_length, sizeof(*me.data));
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

void EasyInteger__print_json(struct EasyInteger *me) {
  EASY_GUARD(me != NULL, "pointer must not be NULL");
  printf("{\"type\": \"EasyInteger\", \".sign\": %d, \".data\": ", me->sign);
  /* NOTE This isn't guaranteed to be printed in such an easily readable
   *      form! In future, I may just print the raw array. */
  for (size_t i = 0; i < me->length; ++i) {
    printf("%d", me->data[EASY_REVERSE_INDEX(me->length, i)]);
  }
  printf(", \".length\": %zu}", me->length);
}

void EasyInteger__destroy(struct EasyInteger *me) {
  EASY_GUARD(me != NULL && me->data != NULL, "input should be non-null");
  EASY_FREE(me->data);

  EASY_SET_ZERO(me);
}