/*******************************************************************************
 *  The Easy Integer
 *  ================
 *
 *  This module provides integers whose value is arbitrarily large (subject to
 *  the constraints on memory within the system).
 *
 *  Design Decisions
 *  ----------------
 *  1. Base-10. This is the most intuitive for most people; however, it makes
 *      bit shifts weird. It also makes terrible utilization of memory (only 10
 *      possible values per byte, which is just over 1 value per bit--so I would
 *      not be much worse off by one-hot encoding digits!). It also makes
 *      terrible natural utilization of the hardware.
 *      - In future, we may select a rather large power-of-two, such as 2^30.
 *
 ******************************************************************************/

#pragma once
#ifndef EASYINTEGER_H
#define EASYINTEGER_H

#include <stddef.h>

enum EasyIntegerSign { NEGATIVE = -1, ZERO = 0, POSITIVE = +1 };

struct EasyInteger {
    enum EasyIntegerSign sign;
    signed char *data;
    size_t length; /* Length of the data buffer -- no terminal NIL byte */
};

struct EasyInteger
EasyInteger__from_cstr(char const *const str);
struct EasyInteger
EasyInteger__copy(struct EasyInteger const *const me);
struct EasyInteger
EasyInteger__add(struct EasyInteger const *const a,
                 struct EasyInteger const *const b);
struct EasyInteger
EasyInteger__multiply(struct EasyInteger const *const a,
                      struct EasyInteger const *const b);
void
EasyInteger__print(struct EasyInteger const *const me);
void
EasyInteger__print_json(struct EasyInteger const *const me);
void
EasyInteger__destroy(struct EasyInteger *const me);

#endif /* !EASYINTEGER_H */
