#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <fcntl.h>
/* recommended version, as per fuse.h */
#define FUSE_USE_VERSION 26
#include <fuse.h>
#include "../parse/lib.h"
#include "../policy/lib.h"
#include "../hashmap/lib.h"
#include "../list/lib.h"

struct private_data {
    char *root;
    struct policy policy;
};

struct private_data *get_private_data(void) {
    return (struct private_data *) fuse_get_context()->private_data;
}

/*
 * Append the given path to the shadowed root, and write into fullpath.
 *
 * Assumes fullpath is of at least size PATH_MAX.
 */
void fill_fullpath(char *fullpath, const char *path) {
    char *root = get_private_data()->root;
    strncpy(fullpath, root, PATH_MAX);
    strncat(fullpath, path, PATH_MAX);
}

int nu_getattr(const char *path, struct stat *buf) {
    char fullpath[PATH_MAX + 1];
    fill_fullpath(fullpath, path);
    return lstat(fullpath, buf);
}

int nu_open(const char *path, struct fuse_file_info *ffi) {
    struct policy policy = get_private_data()->policy;
    struct fuse_context *fuse_context = fuse_get_context();
    char fullpath[PATH_MAX + 1];
    fill_fullpath(fullpath, path);

    // XXX: implement recursive perms
    struct hashmap *role_perms;
    if (!(role_perms = hashmap_get(policy.obj_role_perms, path))) {
        return -EACCES;
    }
    // XXX: getpwuid is *not* thread safe
    struct passwd *passwd = getpwuid(fuse_context->uid);
    struct list *roles;
    if ((roles = hashmap_get(policy.user_role, passwd->pw_name))) {
        for (; roles; roles = list_next(roles)) {
            perms *perms = hashmap_get(role_perms, list_value(roles));
            if (!perms)
                continue;
            // XXX: check ffi->flags and confirm correct perms
            if (*perms & PERM_READ)
                goto granted;
        }
    }
    return -EACCES;

granted:
    ffi->fh = open(fullpath, ffi->flags);
    return ffi->fh < 0 ? -errno : 0;
}

int nu_read(const char *path, char *buf, size_t size, off_t offset, struct
        fuse_file_info *ffi) {
    return pread(ffi->fh, buf, size, offset);
}

int fuse_start(int argc, char *argv[], struct policy policy) {
    if (argc != 3)
        exit(1);

    struct fuse_operations fo = {
        .getattr = nu_getattr,
        .open = nu_open,
        .read = nu_read
    };

    /* we store the canonical path to the shadowed root as our fuse
     * private_data, and remove it from argv so fuse_main ignores it */
    char *root = realpath(argv[--argc], NULL);
    argv[argc] = NULL;

    struct private_data private_data = {
        .root = root,
        .policy = policy,
    };

    fuse_main(argc, argv, &fo, &private_data);
}
