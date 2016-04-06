#ifndef LIB_LIST_H
#define LIB_LIST_H

#include <stdlib.h>

struct list;

#define LIST_INIT (NULL)
void list_append(struct list **list_ptr, void *value);
void *list_value(struct list *node);
struct list *list_next(struct list *node);
void list_destroy(struct list *list);
#endif
