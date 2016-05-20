#include "client_interface.h"
#include "nfs.h"
#include "descriptors.h"

int open(const char *pathname, int flags, mode_t mode) {
	char fileNameBuf[MAX_FILENAME_LEN];
	strncpy(fileNameBuf, pathname, MAX_FILENAME_LEN - 1);

	struct OpenRequest openRequest;
	openRequest.fileName = fileNameBuf;
	openRequest.flags = flags;
	openRequest.mode = mode;

	return 0;
}

int creat(const char *pathname, mode_t mode) {
	return 0;
}

ssize_t read(int fd, void *buf, size_t count) {
	return 0;
}

ssize_t write(int fd, const void *buf, size_t count) {
	return 0;
}

off_t lseek(int fd, off_t offset, int whence) {
	return 0;
}

int close(int fd) {
	return 0;
}
