#include <stdio.h>
#include "khash.h"
#include "def.h"
struct obj_list {
	char *obj;
	struct list *perms;
	int recursive;

	struct obj_list *next;
};
KHASH_MAP_INIT_STR(user_role, struct slist *)
KHASH_MAP_INIT_STR(role_res, struct slist *)
int main()
{
	struct def one = {
		.type = DEF_USER,
		.roles = NULL,
		.user.users = NULL,
		.next = NULL
	};
	struct def two = {
		.type = DEF_OBJ,
		.roles = NULL,
		.obj.obj = "/usr",
		.obj.perms = NULL,
		.obj.recursive = 1,
		.next = &one
	};

	int absent;
	khint_t k;
	khash_t(user_role) * hu = kh_init(user_role);
	khash_t(role_res) * hr = kh_init(role_res);
	for (struct def *def = &two; def; def = def->next) {
		if (def->type != DEF_USER) continue;
		switch (def->type) {
		case DEF_USER:
			for (struct slist *user = def->user.users; user; user = user->next) {
				k = kh_put(user_role, hu, user->s, &absent);
				kh_value(hu, k) = def->roles;
			}
			break;
		case DEF_OBJ:
			// TODO
			break;
		}
	}
	kh_destroy(user_role, hu);
	return 0;
}
