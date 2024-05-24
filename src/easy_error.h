#pragma once

enum EasyErrorType {
    EASY_ERROR_TYPE_OK = 0,
    EASY_ERROR_TYPE_OS,
    EASY_ERROR_TYPE_SYSTEM,
    EASY_ERROR_TYPE_MISCELLANEOUS,
};

struct EasyError {
    // My errors
    enum EasyErrorType error_type;
    char *msg;

    // Operating System errors
    int errno;
    char *strerror;
};

struct EasyError
EasyError__from_errno(int const errno);

void
EasyError__print_json(struct EasyError const *const me);

void
EasyError__destroy(struct EasyError *const me);
