#include <stdio.h>
#include <assert.h>
#include "lib.h"

int main(void) {
    char *vals[] = { "foo", "bar", "baz" };
    size_t len = sizeof(vals) / sizeof(vals[0]);
    struct list *list = LIST_INIT;
    for (size_t i = 0; i < len; i++) {
        list_append(&list, vals[i]);
    }
    size_t i = 0;
    for (struct list *cur = list; cur; cur = list_next(cur)) {
        assert(list_value(cur) == vals[i++]);
    }
}
