/**
 * TODO
 *  - Global Object
 *      - Initialize objects with a pointer to the global object
 *      - This global object stores builtin types, singleton objects, etc.
 *  - Decide where to allocate memory...
 *      - Should we modify an existing object or a pointer to an object?
 *      - i.e. 'struct Object *const' OR 'struct Object **const'?
 **/
#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

struct Object;
union ObjectData {
    void *nothing;
    bool boolean;
    double number;
    char *string;
    struct Array *array;
    struct Table *table;
    // TODO Function (basically a list of commands)
    void *function;
    // TODO Custom (basically a str->object mapping)
    void *custom;
};

enum BuiltinObjectType {
    OBJECT_TYPE_NOTHING,
    OBJECT_TYPE_BOOLEAN,
    OBJECT_TYPE_NUMBER,
    OBJECT_TYPE_STRING,
    OBJECT_TYPE_ARRAY,
    OBJECT_TYPE_TABLE,
    // TODO
    OBJECT_TYPE_FUNCTION,
    // TODO
    OBJECT_TYPE_CUSTOM,
};

struct ObjectType {
    enum BuiltinObjectType type;
    /// @brief Construct an object.
    int (*ctor)(struct Object *const,
                struct ObjectType const *const,
                union ObjectData);
    /// @brief Destroy an object.
    int (*dtor)(struct Object *const);
    /// @brief Compare two objects.
    /// * -1 => lesser
    /// *  0 => identical (i.e. pointer-wise identical)
    /// * +1 => equal
    /// * +2 => greater
    /// * +3 => not-{lesser,greater} but not-equal
    /// * +4 => not-comparable
    /// TODO - Put this into a enumeration.
    int (*cmp)(struct Object const *const,
               struct Object const *const,
               int *const result);
    /// @brief Print an object
    int (*fprint)(struct Object const *const,
                  FILE *const fp,
                  bool const newline);
    int (*from_cstr)(struct Object *const me,
                     struct ObjectType const *const type,
                     char const *const text,
                     char const **end_cstr);
    // String, array, or table
    int (*len)(struct Object const *const, size_t *const result);
    int (*cap)(struct Object const *const, size_t *const result);
    int (*insert)(struct Object *const, size_t const idx, struct Object *const);
    int (*get)(struct Object *const, size_t const idx, struct Object **const);
    int (*slice)(struct Object const *const,
                 size_t const start,
                 size_t const end,
                 struct Object *const);
    int (*remove)(struct Object *const,
                  size_t const idx,
                  struct Object **const);
    // Number
    int (*add)(struct Object const *const,
               struct Object const *const,
               struct Object *const);
    int (*sub)(struct Object const *const,
               struct Object const *const,
               struct Object *const);
    int (*mul)(struct Object const *const,
               struct Object const *const,
               struct Object *const);
    int (*div)(struct Object const *const,
               struct Object const *const,
               struct Object *const);
    // Boolean
    int (*not )(struct Object const *const, bool *const result);
    int (*and)(struct Object const *const,
               struct Object const *const,
               bool *const result);
    int (* or)(struct Object const *const,
               struct Object const *const,
               bool *const result);
    int (*truthiness)(struct Object const *const, bool *const result);
    // Function (takes 1 argument, may be an array)
    int (*call)(struct Object const *const,
                struct Object *const arg,
                struct Object *const result);
};

// Quick lookup - this is a singleton object!
struct BuiltinObjectTypes {
    struct ObjectType nothing;
    struct ObjectType boolean;
    struct ObjectType number;
    struct ObjectType string;
    struct ObjectType array;
    struct ObjectType table;
    // TODO
    struct ObjectType function;
    // TODO
    struct ObjectType custom;
};

struct Object {
    struct ObjectType const *type;
    union ObjectData data;
};

/// @note   This is for the sole purpose of checking that we initialize all
/// fields.
struct ObjectType
new_object_type(
    enum BuiltinObjectType type,
    int (*ctor)(struct Object *const,
                struct ObjectType const *const,
                union ObjectData),
    int (*dtor)(struct Object *const),
    int (*cmp)(struct Object const *const,
               struct Object const *const,
               int *const result),
    int (*fprint)(struct Object const *const,
                  FILE *const fp,
                  bool const newline),
    int (*from_cstr)(struct Object *const me,
                     struct ObjectType const *const type,
                     char const *const text,
                     char const **end_cstr),
    // String, array, or table
    int (*len)(struct Object const *const, size_t *const result),
    int (*cap)(struct Object const *const, size_t *const result),
    int (*insert)(struct Object *const, size_t const idx, struct Object *const),
    int (*get)(struct Object *const, size_t const idx, struct Object **const),
    int (*slice)(struct Object const *const,
                 size_t const start,
                 size_t const end,
                 struct Object *const),
    int (*remove)(struct Object *const,
                  size_t const idx,
                  struct Object **const),
    // Number
    int (*add)(struct Object const *const,
               struct Object const *const,
               struct Object *const),
    int (*sub)(struct Object const *const,
               struct Object const *const,
               struct Object *const),
    int (*mul)(struct Object const *const,
               struct Object const *const,
               struct Object *const),
    int (*div)(struct Object const *const,
               struct Object const *const,
               struct Object *const),
    // Boolean
    int (*not )(struct Object const *const, bool *const result),
    int (*and)(struct Object const *const,
               struct Object const *const,
               bool *const result),
    int (* or)(struct Object const *const,
               struct Object const *const,
               bool *const result),
    int (*truthiness)(struct Object const *const, bool *const result),
    // Function
    int (*call)(struct Object const *const,
                struct Object *const arg,
                struct Object *const result));

// All objects
int
phony_ctor(struct Object *const,
           struct ObjectType const *const type,
           union ObjectData);
int
phony_dtor(struct Object *const);
int
phony_cmp(struct Object const *const,
          struct Object const *const,
          int *const result);
int
fprint(struct Object const *const me, FILE *const fp, bool const newline);
int
phony_from_cstr(struct Object *const me,
                struct ObjectType const *const type,
                char const *const cstr,
                char const **end_cstr);
// String, array, or table
int
phony_len(struct Object const *const, size_t *const result);
int
phony_cap(struct Object const *const, size_t *const result);
int
phony_insert(struct Object *const, size_t const idx, struct Object *const);
int
phony_get(struct Object *const, size_t const idx, struct Object **const);
int
phony_slice(struct Object const *const,
            size_t const start,
            size_t const end,
            struct Object *const);
int
phony_remove(struct Object *const, size_t const idx, struct Object **const);
// Number
int
phony_add(struct Object const *const,
          struct Object const *const,
          struct Object *const);
int
phony_sub(struct Object const *const,
          struct Object const *const,
          struct Object *const);
int
phony_mul(struct Object const *const,
          struct Object const *const,
          struct Object *const);
int
phony_div(struct Object const *const,
          struct Object const *const,
          struct Object *const);
// Boolean
int
phony_not(struct Object const *const, bool *const result);
int
phony_and(struct Object const *const,
          struct Object const *const,
          bool *const result);
int
phony_or(struct Object const *const,
         struct Object const *const,
         bool *const result);
int
phony_truthiness(struct Object const *const, bool *const result);
// Function
int
phony_call(struct Object const *const,
           struct Object *const arg,
           struct Object *const result);

void
init_builtin_object_types(struct BuiltinObjectTypes *const types);
