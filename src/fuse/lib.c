#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <fcntl.h>
/* recommended version, as per fuse.h */
#define FUSE_USE_VERSION 26
#include <fuse.h>
#include "../policy/lib.h"
#include "../hashmap/lib.h"
#include "../list/lib.h"

/*
 * FUSE permits storing a handle to data associated with the filesystem, which
 * in our case involves the path to the ghosted directory (`root`) and the RBAC
 * policy structures (`policy`).
 *
 * All paths within our filesystem are "redirected" to equivalent paths
 * relative to `root`, and are only permitted if the access is allowed by the
 * `policy`.
 */
struct private_data {
    char *root;
    struct policy policy;
};

/*
 * get_private_data
 *      IN: void
 *      OUT: The private data for the filesystem.
 *      DESCRIPTION: A convenience function for accessing the fuse private
 *      data, as it is stored behind a void pointer.
 */
struct private_data *get_private_data(void) {
    return (struct private_data *) fuse_get_context()->private_data;
}

/*
 * fill_fullpath
 *      IN: A buffer to fill with the full path; the original path
 *      OUT: void
 *      DESCRIPTION: Append the given path to the shadowed root, and write into `fullpath`.
 *      NOTE: Assumes `fullpath` is of at least size PATH_MAX.
 */
void fill_fullpath(char *fullpath, const char *path) {
    char *root = get_private_data()->root;
    strncpy(fullpath, root, PATH_MAX);
    strncat(fullpath, path, PATH_MAX);
}

/*
 * get_pw_name
 *      IN: uid of the user to lookup
 *      OUT: The username of the user, if any, otherwise NULL if the user does
 *      note exist or the lookup failed
 *      DESCRIPTION: Get the username of a user from their uid
 *      NOTE: Is reenterant and thread safe. The returned string is owned by
 *      the caller and must be free'd
 */
char *get_pw_name(uid_t uid) {
    struct passwd pwd;
    struct passwd *result;
    size_t bufsz = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsz == -1)
        return NULL;
    char *buf = malloc(bufsz);
    if (buf == NULL)
        return NULL;
    int r = getpwuid_r(uid, &pwd, buf, bufsz, &result);
    if (result == NULL || r != 0)
        return NULL;
    char *pw_name = strdup(pwd.pw_name);
    free(buf);
    return pw_name;
}

/*
 * nu_getattr
 *      DESCRIPTION: Thin wrapper around lstat for the filesystem. No security
 *      decisions are made.
 */
int nu_getattr(const char *path, struct stat *buf) {
    char fullpath[PATH_MAX + 1];
    fill_fullpath(fullpath, path);
    return lstat(fullpath, buf);
}

/*
 * nu_open
 *      DESCRIPTION: Primary mechanism for implementing security policy. This
 *      function is a wrapper around open for the filesystem. It searches the
 *      policy structure and implements the access controls by inspecting the
 *      open mode requested by the user and comparing it to the permissions
 *      granted to them in the RBAC definitions file.
 *      RETURN: -EACCESS in the event that the permission is not granted,
 *      otherwise the return value of open for the same parameters.
 */
int nu_open(const char *path, struct fuse_file_info *ffi) {
    struct policy policy = get_private_data()->policy;
    struct fuse_context *fuse_context = fuse_get_context();
    char *mpath = NULL;
    char *pw_name = NULL;
    bool recursed = false;

    // We will need to mutate the path to strip components off of it when
    // checking for recursive permissions.
    mpath = strdup(path);
    if (!mpath)
        goto denied;

    char fullpath[PATH_MAX + 1];
    fill_fullpath(fullpath, mpath);

    pw_name = get_pw_name(fuse_context->uid);
    if (!pw_name)
        goto denied;

    // We check for recursive perms by continuing to check the permission of
    // every path at and above the requested path. We start with the full path
    // (recursed==false) and so do not require the recursive permission on the
    // object. After that, we strip off one path component at a time and
    // require the recursive permission on each object. Finally, if we get down
    // to the root object ("/") and do not grant access, we deny access.
    for (;;) {
        struct hashmap *role_perms;
        if (role_perms = hashmap_get(policy.obj_role_perms, mpath)) {
            struct list *roles;
            if ((roles = hashmap_get(policy.user_role, pw_name))) {
                for (; roles; roles = list_next(roles)) {
                    perms *perms = hashmap_get(role_perms, list_value(roles));
                    if (!perms)
                        continue;
                    if (recursed && !(*perms & PERM_RECURSIVE))
                        continue;
                    int accmode = ffi->flags & O_ACCMODE;
                    if (accmode == O_RDONLY && *perms & PERM_READ)
                        goto granted;
                    else if (accmode == O_WRONLY && *perms & PERM_WRITE)
                        goto granted;
                    else if (*perms & PERM_READ && *perms & PERM_WRITE)
                        goto granted;
                }
            }
        }
        if (strcmp("/", mpath) == 0) {
            goto denied;
        } else {
            recursed = true;
            char *slash = strrchr(mpath, '/');
            // The last case is special because unix reuses the slash character
            // to both separate paths and to represent the root path. In the
            // latter case we just null out the character after the root path,
            // which would only fail for the empty string, which we never
            // receive.
            if (slash == mpath)
                slash++;
            *slash = '\0';
        }
    }

denied:
    free(mpath);
    free(pw_name);
    return -EACCES;

granted:
    free(mpath);
    free(pw_name);
    ffi->fh = open(fullpath, ffi->flags);
    return ffi->fh < 0 ? -errno : 0;
}

/*
 * nu_read
 *      DESCRIPTION: Thin wrapper around pread for the filesystem. No security
 *      decisions are made.
 */
int nu_read(const char *path, char *buf, size_t size, off_t offset, struct
        fuse_file_info *ffi) {
    return pread(ffi->fh, buf, size, offset);
}

/*
 * nu_read
 *      DESCRIPTION: Thin wrapper around pwrite for the filesystem. No security
 *      decisions are made.
 */
int nu_write(const char *path, const char *buf, size_t size, off_t offset,
        struct fuse_file_info *ffi) {
    return pwrite(ffi->fh, buf, size, offset);
}

/*
 * fuse_start
 *      DESCRIPTION: Entrypoint into the filesystem. Saves the path arguments
 *      to the main executable and the policy object as FUSE private data and
 *      then enters the fuse main loop.
 */
int fuse_start(int argc, char *argv[], struct policy policy) {
    if (argc < 3)
        exit(1);

    struct fuse_operations fo = {
        .getattr = nu_getattr,
        .open = nu_open,
        .read = nu_read,
        .write = nu_write,
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
