#pragma once

#include <assert.h>

enum BuiltinObjectType {
    OBJECT_TYPE_NOTHING,
    OBJECT_TYPE_BOOLEAN,
    OBJECT_TYPE_NUMBER,
    OBJECT_TYPE_STRING,
    OBJECT_TYPE_ARRAY,
    OBJECT_TYPE_TABLE,
    OBJECT_TYPE_CUSTOM,
};

struct ObjectType {
    enum BuiltinObjectType type;
    /// @brief Construct an object.
    int (*ctor)(struct Object *, enum BuiltinObjectType, union ObjectData);
    /// @brief Destroy an object.
    int (*dtor)(struct Object *);
    /// @brief Compare two objects.
    /// * -1 => lesser
    /// *  0 => equal
    /// * +1 => greater
    /// * +2 => non-comparable but not-equal
    int (*cmp)(struct Object *, struct Object *);
    // String, array, or table
    int (*len)(struct Object *);
    int (*cap)(struct Object *);
};

union ObjectData {
    void *nothing;
    bool *boolean;
    double *number;
    char *string;
    struct Array *array;
    struct Table *table;
};

struct Object {
    struct ObjectType *type;
    union ObjectData data;
};



