#pragma once

#include <assert.h>

int unimplemented_1(struct Object *obj) {
    assert(0 && "unimplemented error");
    return 1;
}

int unimplemented_2(struct Object *obj) {
    assert(0 && "unimplemented error");
    return 1;
}

