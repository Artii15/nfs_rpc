#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

typedef unsigned mode_t;

#define MAX_FILENAME_LEN 255

struct FileDescriptor {
	char fileName[MAX_FILENAME_LEN];
	int flags;
	mode_t mode;
	int inUse;
};


int reserveDescriptor(const char *pathname, int flags, mode_t mode);
int releaseDescriptor(int fd);
struct FileDescriptor* getDescriptor(int fd);

#endif
