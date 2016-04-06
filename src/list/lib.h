#ifndef LIB_LIST_H
#define LIB_LIST_H
struct list {
    void *value;
    struct list *next;
};

struct list *list_create(void);
void list_append(struct list *list, void *value);
void list_destroy(struct list *list);
#endif
