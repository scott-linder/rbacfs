#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
/* recommended version, as per fuse.h */
#define FUSE_USE_VERSION 26
#include <fuse.h>
#include "../parse/lib.h"

/*
 * Append the given path to the shadowed root, and write into fullpath.
 *
 * Assumes fullpath is of at least size PATH_MAX.
 */
void fill_fullpath(char *fullpath, const char *path) {
    char *root = (char *) fuse_get_context()->private_data;
    strncpy(fullpath, root, PATH_MAX);
    strncat(fullpath, path, PATH_MAX);
}

int nu_getattr(const char *path, struct stat *buf) {
    char fullpath[PATH_MAX + 1];
    fill_fullpath(fullpath, path);
    return lstat(fullpath, buf);
}

int nu_open(const char *path, struct fuse_file_info *ffi) {
    char fullpath[PATH_MAX + 1];
    fill_fullpath(fullpath, path);
    ffi->fh = open(fullpath, ffi->flags);
    return ffi->fh < 0 ? -errno : 0;
}

int nu_read(const char *path, char *buf, size_t size, off_t offset, struct
        fuse_file_info *ffi) {
    return pread(ffi->fh, buf, size, offset);
}

int fuse_start(int argc, char *argv[], struct def *def) {
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

    fuse_main(argc, argv, &fo, root);
}
