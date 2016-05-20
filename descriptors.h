#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

#define MAX_FILENAME_LEN 255

#include <sys/types.h>

struct FileDescriptor {
	char fileName[MAX_FILENAME_LEN];
	int flags;
	mode_t mode;
	int seekPos;
	int inUse;
};


int reserveDescriptor(const char *pathname, int flags, mode_t mode);
int releaseDescriptor(int fd);
struct FileDescriptor* getDescriptor(int fd);

#endif
