#include <math.h>
#include <stddef.h>

#include "common/status.h"
#include "type/ez_number.h"

struct EzNumberResult
EzNumber_add(struct EzNumber const *const me,
             struct EzNumber const *const other)
{
    if (me == NULL || other == NULL) {
        return (struct EzNumberResult){.status = EZ_STATUS_ERROR,
                                       .result = {NAN}};
    }
    double const r = me->data + other->data;
    return (struct EzNumberResult){.status = OK, .result = {r}};
}

struct EzNumberResult
EzNumber_subtract(struct EzNumber const *const me,
                  struct EzNumber const *const other)
{
    if (me == NULL || other == NULL) {
        return (struct EzNumberResult){.status = EZ_STATUS_ERROR,
                                       .result = {0}};
    }
    double const r = me->data - other->data;
    return (struct EzNumberResult){.status = OK, .result = {r}};
}

struct EzNumberResult
EzNumber_multiply(struct EzNumber const *const me,
                  struct EzNumber const *const other)
{
    if (me == NULL || other == NULL) {
        return (struct EzNumberResult){.status = EZ_STATUS_ERROR,
                                       .result = {NAN}};
    }
    double const r = me->data * other->data;
    return (struct EzNumberResult){.status = OK, .result = {r}};
}

struct EzNumberResult
EzNumber_divide(struct EzNumber const *const me,
                struct EzNumber const *const other)
{
    if (me == NULL || other == NULL) {
        return (struct EzNumberResult){.status = EZ_STATUS_ERROR,
                                       .result = {NAN}};
    }
    if (other->data == 0.0) {
        return (struct EzNumberResult){.status = EZ_STATUS_ERROR,
                                       .result = {NAN}};
    }
    double const r = me->data / other->data;
    return (struct EzNumberResult){.status = OK, .result = {r}};
}
