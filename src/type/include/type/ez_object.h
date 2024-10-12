#pragma once

#include "common/status.h"

enum EzObjectType {
    EZ_OBJECT_TABLE,
    EZ_OBJECT_NUMBER,
    EZ_OBJECT_ARRAY,
    EZ_OBJECT_TEXT,
    EZ_OBJECT_CUSTOM,
};

struct EzObject {
    enum EzObjectType type;
};

enum EzStatus
EzObject__x(struct EzObject *const me);
