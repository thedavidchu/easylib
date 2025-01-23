int nothing_ctor(struct Object *obj, union ObjectData data) {
    if (obj == NULL) return -1;
    obj->type = nothing_type;
    // TODO Check if data is valid.
    obj->data = data;
    return 0;
}

