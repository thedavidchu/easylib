#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "easy_common.h"

#include "easy_text.h"

static void
assert_well_formed(struct EasyText const *const me)
{
    EASY_GUARD(me != NULL, "expected non-NULL me");
    EASY_ASSERT(me->data[me->length] == '\0',
                "expected '\0' terminated C-style string");
}

static inline void
print_jsonified_char(char const c)
{
    // Escape special JSON characters according to
    // https://www.json.org/json-en.html
    switch (c) {
    case '\"':
        printf("\"");
        return;
    case '\\':
        printf("\\");
        return;
    /* NOTE: I do not escape '/', because it's already valid. The parser
     *      would need to be able to parse "\/", however. */
    /* NOTE: The bell '\a' is represented in hexadecimal in JSON. */
    case '\b': /* BACKSPACE */
        printf("\\b");
        return;
    case '\f': /* FORM FEED */
        printf("\\f");
        return;
    case '\n': /* NEW LINE */
        printf("\\n");
        return;
    case '\r': /* CARRIAGE RETURN */
        printf("\\r");
        return;
    case '\t': /* HORIZONTAL TAB */
        printf("\\t");
        return;
    /* NOTE: The vertical tab '\v' is represented in hexadecimal in JSON. */
    default:
        if (isprint(c)) {
            // NOTE I could also use `putchar(c)`, but this is less "easy".
            printf("%c", c);
            return;
        } else {
            printf("\\u%.4hhx", c);
            return;
        }
    }
}

struct EasyText
EasyText__from_cstr(char const *const str)
{
    EASY_GUARD(str != NULL, "expected non-NULL C-style string");

    struct EasyText me = {0};
    me.length = strlen(str);
    me.data = EASY_DUPLICATE(str, me.length + 1, sizeof(char));
    return me;
}

void
EasyText__print(struct EasyText const *const me)
{
    assert_well_formed(me);
    printf("%s", me->data);
}

void
EasyText__print_json(struct EasyText const *const me)
{
    assert_well_formed(me);
    printf("{\"type\": \"EasyText\", \".data\": \"");

    /* Print JSON-ified characters */
    for (size_t i = 0; i < me->length; ++i) {
        print_jsonified_char(me->data[i]);
    }

    printf("\", \".length\": %zu}", me->length);
}

struct EasyText
EasyText__copy(struct EasyText const *const me)
{
    EASY_GUARD(me != NULL && me->data != NULL, "ptr must not be NULL");
    return (struct EasyText){
        .data = EASY_DUPLICATE(&me->data, me->length, sizeof(*me->data)),
        .length = me->length};
}

void
EasyText__destroy(struct EasyText *const me)
{
    EASY_GUARD(me != NULL && me->data != NULL, "input should be non-null");
    EASY_FREE(me->data);

    EASY_SET_ZERO(me);
}
