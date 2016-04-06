#include <stdlib.h>
#include "lib.h"

struct list *list_create(void) {
    return malloc(sizeof(struct list));
}

void list_append(struct list *list, void *value) {
    for (; list->next; list = list->next)
        ;
    list->next = malloc(sizeof(*list->next));
    list->next->value = value;
    list->next->next = NULL;
}

void list_destroy(struct list *list) {
    struct list *temp;
    while (list) {
        temp = list->next;
        free(list);
        list = temp;
    }
}
