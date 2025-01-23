#pragma once

#include "common/status.h"

struct EzNumber {
    double const data;
};

struct EzNumberResult {
    enum EzStatus const status;
    struct EzNumber result;
};

struct EzNumberResult
EzNumber_add(struct EzNumber const *const me,
             struct EzNumber const *const other);

struct EzNumberResult
EzNumber_subtract(struct EzNumber const *const me,
                  struct EzNumber const *const other);
struct EzNumberResult
EzNumber_multiply(struct EzNumber const *const me,
                  struct EzNumber const *const other);
struct EzNumberResult
EzNumber_divide(struct EzNumber const *const me,
                struct EzNumber const *const other);
