#include <stdlib.h>
#include "lib.h"

struct list {
    void *value;
    struct list *next;
};

void list_append(struct list **list_ptr, void *value) {
    if (*list_ptr) {
        struct list *l = *list_ptr;
        for (; l->next; l = l->next)
            ;
        l->next = malloc(sizeof(struct list));
        l->next->value = value;
        l->next->next = NULL;
    } else {
        *list_ptr = malloc(sizeof(struct list));
        (*list_ptr)->value = value;
        (*list_ptr)->next = NULL;
    }
}

void *list_value(struct list *node) {
    return node->value;
}

struct list *list_next(struct list *node) {
    return node->next;
}

void list_destroy(struct list *list) {
    while (list) {
        struct list *temp = list->next;
        free(list);
        list = temp;
    }
}
