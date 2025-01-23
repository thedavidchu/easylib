#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

enum TableNodeStatus {
    // NOTE Making the invalid option 0 is intentional so that we can calloc
    //      the array.
    TABLE_NODE_INVALID = 0,
    TABLE_NODE_VALID = 1,
    TABLE_NODE_TOMBSTONE = 2,
};

struct TableNode {
    enum TableNodeStatus status;
    size_t key;
    size_t value;
};

struct Table {
    struct TableNode *data;
    size_t length;
    size_t capacity;
};

int
table_ctor(struct Table *const me);

int
table_dtor(struct Table *const me);

/// @brief  Write the table in the order of the hashes.
int
table_write(struct Table const *const me, FILE *const stream, bool const newline);

/// @brief  Insert or update the value associated with a key.
int
table_insert(struct Table *const me, size_t const key, size_t const value);

/// @brief  Get a value associated with a key (the table maintains ownership).
int
table_get(struct Table *const me, size_t const key, size_t *const value);

int
table_remove(struct Table *const me, size_t const key, size_t *const value);

