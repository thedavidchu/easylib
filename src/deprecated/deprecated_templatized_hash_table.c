/** Hash Table in C99 */

////////////////////////////////////////////////////////////////////////////////
/// UNRELATED TO-BE GENERIC STUFF
////////////////////////////////////////////////////////////////////////////////

typedef char* KEYTYPE;
typedef int VALUETYPE;

////////////////////////////////////////////////////////////////////////////////
/// HASH_TABLE.H
////////////////////////////////////////////////////////////////////////////////
#include <stdbool.h> // bool, true, false
#include <stddef.h> // size_t, NULL

// TODO(dchu): this should only be defined once! Oh wait, that would require #define within #define blocks... maybe? Let's see.
enum HASHTABLE_RETURN_CODES {
    ELEMENT_FOUND = 0,
    ELEMENT_NOT_FOUND = 1,
    ERROR = 2,
};

struct HASHTABLE_KEYTYPE_TO_VALUETYPE;
typedef struct HASHTABLE_KEYTYPE_TO_VALUETYPE HASHTABLE_KEYTYPE_TO_VALUETYPE;

static size_t HASH_KEYTYPE(KEYTYPE *key);
/** Compare KEYTYPE where 0 => equal; -1 => less than; +1 => greater than; and +2 => not equal (and there exists no ordering). */
static int COMPARE_KEYTYPE(KEYTYPE *key_0, KEYTYPE *key_1);

/** Is this necessary? Mostly it is to shield */
static KEYTYPE COPY_KEYTYPE(KEYTYPE *key);

////////////////////////////////////////////////////////////////////////////////

int HASHTABLE_KEYTYPE_TO_VALUETYPE__ctor(HASHTABLE_KEYTYPE_TO_VALUETYPE *me, size_t initial_size);
int HASHTABLE_KEYTYPE_TO_VALUETYPE__dtor(HASHTABLE_KEYTYPE_TO_VALUETYPE *me);

int HASHTABLE_KEYTYPE_TO_VALUETYPE__insert(HASHTABLE_KEYTYPE_TO_VALUETYPE *me, KEYTYPE key, VALUETYPE value);
int HASHTABLE_KEYTYPE_TO_VALUETYPE__search(HASHTABLE_KEYTYPE_TO_VALUETYPE *me, KEYTYPE key, VALUETYPE *value_tr);
int HASHTABLE_KEYTYPE_TO_VALUETYPE__remove(HASHTABLE_KEYTYPE_TO_VALUETYPE *me, KEYTYPE key);

////////////////////////////////////////////////////////////////////////////////
/// HASH_TABLE.C
////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Okay to name it something generic because it is private */
struct element_t {
    KEYTYPE key;
    VALUETYPE value;
    int validity;
    size_t hashcode;
};
typedef struct element_t element_t;

enum VALIDITY {
    VALID_ELEMENT = 0,
    INVALID_ELEMENT = -1,
    TOMBSTONE_ELEMENT = -2,
};

struct HASHTABLE_KEYTYPE_TO_VALUETYPE {
    element_t* elements;

    size_t length;
    size_t size;    
};

/// BELOW ARE THE FUNCTIONS THAT NEED TO BE SPECIFIED FOR US (HELPER FUNCTIONS)
static size_t HASH_KEYTYPE(KEYTYPE *key) {
    char *str = *key;
    size_t hash = 5381;
    char c;
    while (c = *str++) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

static int COMPARE_KEYTYPE(KEYTYPE *key_0, KEYTYPE *key_1) {
    return strcmp(*key_0, *key_1);
}

static KEYTYPE COPY_KEYTYPE(KEYTYPE *key) {
    return *key;
}

/// TODO
/// 1. KEYTYPE ctor, dtor, print
/// 2. VALUETYPE ctor, dtor, print
/// 3. Allow custom names for functions (e.g. ht_insert, ht_dtor, etc)

////////////////////////////////////////////////////////////////////////////////

/// Assume we get all uninitialized data.
int HASHTABLE_KEYTYPE_TO_VALUETYPE__ctor(HASHTABLE_KEYTYPE_TO_VALUETYPE *me, size_t initial_size) {
    /*Initial Setup (in case of error) */
    me->length = 0;
    me->size = 0;
    size_t init_size_in_bytes = initial_size * sizeof(element_t);
    if (init_size_in_bytes < initial_size) {
        /* Multiplicative overflow condition! */
        return -1;
    }
    me->elements = (element_t *)malloc(init_size_in_bytes);
    if (!me->elements) {
        return -1;
    }
    me->size = initial_size;
    for (size_t i = 0; i < initial_size; ++i) {
        me->elements[i].validity = INVALID_ELEMENT;
    }
    return 0;
}
int HASHTABLE_KEYTYPE_TO_VALUETYPE__dtor(HASHTABLE_KEYTYPE_TO_VALUETYPE *me) {
    if (me->elements) {
        // TODO(dchu): free elements themselves
        free(me->elements);
        me->elements = NULL;
        me->length = 0;
        me->size = 0;
    }
}

int HASHTABLE_KEYTYPE_TO_VALUETYPE__insert(HASHTABLE_KEYTYPE_TO_VALUETYPE *me, KEYTYPE key, VALUETYPE value) {
    size_t hashcode = HASH_KEYTYPE(&key);
    assert((me->length < me->size || me->length == 0 && me->size == 0) && "Hash Table is Full!");
    for (size_t offset = 0; offset < me->size; ++offset) {
        element_t *element_ptr = &me->elements[(hashcode + offset) % me->size];
        switch (element_ptr->validity) {
        case INVALID_ELEMENT:
        case TOMBSTONE_ELEMENT:
            element_ptr->key = key;
            element_ptr->value = value;
            element_ptr->validity = VALID_ELEMENT;
            element_ptr->hashcode = hashcode;
            me->length = me->length + 1;
            return 0;
        case VALID_ELEMENT:
            continue;
        default:
            assert("impossible: validity must be VALID, INVALID, or TOMBSTONE");
        }
    }
}

int HASHTABLE_KEYTYPE_TO_VALUETYPE__search(HASHTABLE_KEYTYPE_TO_VALUETYPE *me, KEYTYPE key, VALUETYPE *value_ptr) {
    size_t hashcode = HASH_KEYTYPE(&key);
    for (size_t offset = 0; offset < me->size; ++offset) {
        element_t *element_ptr = &me->elements[(hashcode + offset) % me->size];
        switch (element_ptr->validity) {
        case INVALID_ELEMENT:
        case TOMBSTONE_ELEMENT:
            return ELEMENT_NOT_FOUND;
        case VALID_ELEMENT:
            if (hashcode != element_ptr->hashcode || COMPARE_KEYTYPE(&key, &element_ptr->key) != 0) {
                continue;
            }
            *value_ptr = element_ptr->value;
            return ELEMENT_FOUND;
        default:
            assert("impossible: validity must be VALID, INVALID, or TOMBSTONE");
        }
    }
}

int HASHTABLE_KEYTYPE_TO_VALUETYPE__remove(HASHTABLE_KEYTYPE_TO_VALUETYPE *me, KEYTYPE key) {
    size_t hashcode = HASH_KEYTYPE(&key);
    for (size_t offset = 0; offset < me->size; ++offset) {
        element_t *element_ptr = &me->elements[(hashcode + offset) % me->size];
        switch (element_ptr->validity) {
        case INVALID_ELEMENT:
        case TOMBSTONE_ELEMENT:
            return ELEMENT_NOT_FOUND;
        case VALID_ELEMENT:
            if (hashcode != element_ptr->hashcode || COMPARE_KEYTYPE(&key, &element_ptr->key) != 0) {
                continue;
            }
            element_ptr->validity = TOMBSTONE_ELEMENT;
            me->length = me->length - 1;
            return ELEMENT_FOUND;
        default:
            assert("impossible: validity must be VALID, INVALID, or TOMBSTONE");
        }
    }
}

void print(HASHTABLE_KEYTYPE_TO_VALUETYPE *me) {
    printf("[%zu/%zu] {", me->length, me->size);
    for (size_t i = 0; i < me->size; ++i) {
        element_t e = me->elements[i];
        switch (e.validity) {
        case VALID_ELEMENT:
            printf("'%s'/%zu: %d, ", e.key, e.hashcode, e.value);
            break;
        case INVALID_ELEMENT:
            printf("X, ");
            break;
        case TOMBSTONE_ELEMENT:
            printf("T, ");
            break;
        default:
            assert("impossible: illegal validity");
        }
    }
    printf("}\n");
}

int main() {
    HASHTABLE_KEYTYPE_TO_VALUETYPE ht;
    KEYTYPE keys[10] = {
        "zero",
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine",
    };
    HASHTABLE_KEYTYPE_TO_VALUETYPE__ctor(&ht, 8);
    print(&ht);
    HASHTABLE_KEYTYPE_TO_VALUETYPE__insert(&ht, keys[0], 0);
    print(&ht);
    HASHTABLE_KEYTYPE_TO_VALUETYPE__insert(&ht, keys[1], 1);
    print(&ht);
    HASHTABLE_KEYTYPE_TO_VALUETYPE__insert(&ht, keys[2], 2);
    print(&ht);
    HASHTABLE_KEYTYPE_TO_VALUETYPE__remove(&ht, keys[2]);
    HASHTABLE_KEYTYPE_TO_VALUETYPE__remove(&ht, keys[2]);
    print(&ht);
    HASHTABLE_KEYTYPE_TO_VALUETYPE__dtor(&ht);
}
