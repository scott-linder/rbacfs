#include <stdlib.h>
#include "lib.h"

/*
 * list
 *      A singly linked list.
 *
 *      Each node contains a void pointer value and a next pointer. The last
 *      node contains a null next pointer.
 *
 *      Declare always as a pointer, and initialise with the LIST_INIT constant:
 *              struct list *list = LIST_INIT;
 */
struct list {
    void *value;
    struct list *next;
};

/*
 * list_append
 *      IN: A pointer to the head node (note that this is a double pointer, as
 *      the node itself is already a pointer), and the value for the new node.
 *      OUT: Nothing
 *      DESCRIPTION: Appends a new node to the end of the list with the given
 *      value.
 */
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

/*
 * list_value
 *      IN: A single list node.
 *      OUT: The value of the list node.
 *      DESCRIPTION: Get the value of the given list node.
 */
void *list_value(struct list *node) {
    return node->value;
}

/*
 * list_string
 *      IN: A single list node.
 *      OUT: The value of the list node, cast to `char *`.
 *      DESCRIPTION: Get the value of the given list node, cast to a string.
 */
char *list_string(struct list *node) {
    return (char *) list_value(node);
}

/*
 * list_next
 *      IN: A single list node.
 *      OUT: The next list node, or NULL if the given node was the end of the list.
 *      DESCRIPTION: Get the next node in the list.
 */
struct list *list_next(struct list *node) {
    return node->next;
}

/*
 * list_destroy
 *      IN: The head of a list.
 *      OUT: Nothing
 *      DESCRIPTION: Free each node of a list.
 *      NOTE: The value of each node is *not* freed, and must be freed by the
 *      user prior to calling `list_destroy`.
 */
void list_destroy(struct list *list) {
    while (list) {
        struct list *temp = list->next;
        free(list);
        list = temp;
    }
}
