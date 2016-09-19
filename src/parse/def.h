/*
 * def.h - Syntax tree.
 */

#ifndef DEF_H
#define DEF_H

/*
 * Types of definitions.
 */
enum def_type {
    DEF_USER,
    DEF_OBJ
};

/*
 * Definition.
 */
struct def {
    enum def_type type;

    struct list *roles;
    union {
        struct {
            struct list *users;
        } user;
        struct {
            char *obj;
            struct list *perms;
            int recursive;
        } obj;
    };

    struct def *next;
};

struct def *def_create(enum def_type type);
void def_append(struct def *list, struct def *def);
void def_destroy(struct def *list);

#endif // DEF_H
