#ifndef LIB_FUSE_H
#define LIB_FUSE_H
#include "../policy/lib.h"
int fuse_start(int, char *[], struct policy policy);
#endif
