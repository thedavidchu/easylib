#pragma once

#include <stdint.h>

#include "easy_lib.h"

enum EasyTableItemStatus {
    /* This is intentionally zero because using calloc to allocate the array
     * will create all INVALID entries. */
    EASY_TABLE_INVALID = 0,
    /* A tombstone is where an entry was formerly found but was removed in
     * a hash table with simple linear probing. This tells future searches
     * that the element they are looking for may have been pushed farther
     * away by the now-removed element (i.e. keep looking). An insert can
     * stick its element into a tombstone. */
    EASY_TABLE_TOMBSTONE,
    EASY_TABLE_VALID,
};

/* NOTE These need to come after the EasyGenericObject */
struct EasyTableItem {
    enum EasyTableItemStatus valid;
    uint64_t hash;
    struct EasyGenericObject key;
    struct EasyGenericObject value;
};
