#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

typedef unsigned mode_t;

int reserveDescriptor(const char *pathname, int flags, mode_t mode);
int returnDescriptor(int fd);

#endif
