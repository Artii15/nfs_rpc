#ifndef CLIENT_INTERFACE_H
#define CLIENT_INTERFACE_H

#include "descriptors.h"

void clientInit(char* hostname);
void clientFinish();
int open(const char *pathname, int flags, mode_t mode);
int creat(const char *pathname, mode_t mode);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
off_t lseek(int fd, off_t offset, int whence);
int close(int fd);

#endif
