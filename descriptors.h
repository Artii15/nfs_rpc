#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

typedef unsigned mode_t;

int open(const char *pathname, int flags, mode_t mode);

#endif
