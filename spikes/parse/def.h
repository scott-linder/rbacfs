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
 * String linked list.
 */
struct slist {
    char *s;
    struct slist *next;
};

struct slist *slist_create(void);
void slist_append(struct slist *, char *);
void slist_destroy(struct slist *);

/*
 * Definition.
 */
struct def {
    enum def_type type;

    struct slist *roles;
    union {
        struct {
            struct slist *users;
        } user;
        struct {
            char *obj;
            struct slist *perms;
            int recursive;
        } obj;
    };
};

struct def *def_create(enum def_type type);

#endif // DEF_H
