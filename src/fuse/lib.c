#include <unistd.h>
#include <limits.h>
#include <syslog.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <fcntl.h>
#include <dirent.h>
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
 * has_access
 *      DESCRIPTION: Primary mechanism for implementing security policy.
 *      Checks if a given path is available to the current user with the
 *      requested permissions (found in struct fuse_file_info).
 *      It searches the policy structure and implements the access controls by
 *      inspecting the open mode requested by the user and comparing it to the
 *      permissions granted to them in the RBAC definitions file.
 */
bool has_access(const char *path, struct fuse_file_info *ffi) {
    syslog(LOG_INFO, "entering has_access(\"%s\")\n", path);
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
    syslog(LOG_INFO, "mpath=%s\n", mpath);

    pw_name = get_pw_name(fuse_context->uid);
    if (!pw_name)
        goto denied;
    syslog(LOG_INFO, "pw_name=%s\n", pw_name);

    // We check for recursive perms by continuing to check the permission of
    // every path at and above the requested path. We start with the full path
    // (recursed==false) and so do not require the recursive permission on the
    // object. After that, we strip off one path component at a time and
    // require the recursive permission on each object. Finally, if we get down
    // to the root object ("/") and do not grant access, we deny access.
    for (;;) {
        struct hashmap *role_perms;
        if ((role_perms = hashmap_get(policy.obj_role_perms, mpath))) {
            struct list *roles;
            if ((roles = hashmap_get(policy.user_roles, pw_name))) {
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
    syslog(LOG_INFO, "exiting has_access with access DENIED");
    free(mpath);
    free(pw_name);
    return false;
granted:
    syslog(LOG_INFO, "exiting has_access with access GRANTED");
    free(mpath);
    free(pw_name);
    return true;
}

/*
 * nu_open
 *      DESCRIPTION: This function is a wrapper around open for the filesystem,
 *      implementing RBAC access control.
 *      RETURN: -EACCESS in the event that the permission is not granted,
 *      otherwise the return value of open for the same parameters.
 */
int nu_open(const char *path, struct fuse_file_info *ffi) {
    if (!has_access(path, ffi))
        return -EACCES;
    char fullpath[PATH_MAX + 1];
    fill_fullpath(fullpath, path);
    ffi->fh = open(fullpath, ffi->flags);
    return ffi->fh < 0 ? -errno : 0;
}

/*
 * nu_opendir
 *      DESCRIPTION: A wrapper around opendir for the filesystem, implementing
 *      RBAC access control.
 *      RETURN: -EACCESS in the event that the permission is not granted,
 *      otherwise the return value of opendir for the same parameters.
 */
int nu_opendir(const char *path, struct fuse_file_info *ffi) {
    syslog(LOG_INFO, "opendir path=%s", path);
    if (!has_access(path, ffi))
        return -EACCES;
    char fullpath[PATH_MAX + 1];
    fill_fullpath(fullpath, path);
    ffi->fh = (uintptr_t) opendir(fullpath);
    return ffi->fh == (uintptr_t) NULL ? -errno : 0;
}

/*
 * nu_readdir
 *      DESCRIPTION: Thin wrapper around readdir for the filesystem. No
 *      security decisions are made.
 */
int nu_readdir(const char *path, void *buf, fuse_fill_dir_t fill, off_t offset,
        struct fuse_file_info *ffi) {
    DIR *d = (DIR *) (uintptr_t) ffi->fh;
    struct dirent *de;
    while ((de = readdir(d)) != NULL)
        if (fill(buf, de->d_name, NULL, 0) != 0)
            return -ENOMEM;
    return 0;
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
 * nu_write
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

    struct fuse_operations fo = {
        .getattr = nu_getattr,
        .open = nu_open,
        .read = nu_read,
        .write = nu_write,
        .opendir = nu_opendir,
        .readdir = nu_readdir,
    };

    /* we store the canonical path to the shadowed root as our fuse
     * private_data, and remove it from argv so fuse_main ignores it */
    char *root = realpath(argv[--argc], NULL);
    if (!root) {
        perror("invalid root");
        exit(1);
    }
    argv[argc] = NULL;

    struct private_data private_data = {
        .root = root,
        .policy = policy,
    };

    return fuse_main(argc, argv, &fo, &private_data);
}
