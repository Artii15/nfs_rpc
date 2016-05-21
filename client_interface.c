#include "client_interface.h"
#include "nfs.h"
#include "descriptors.h"
#include <errno.h>

static CLIENT* clnt = 0;

/*
	struct OperationStatus  *result_2;
	struct CreatRequest  rcreat_1_arg;
	struct ReadResponse  *result_3;
	struct FileAccessRequest  rread_1_arg;
	struct WriteResponse  *result_4;
	struct FileAccessRequest  rwrite_1_arg;

	result_2 = rcreat_1(&rcreat_1_arg, clnt);
	if (result_2 == (struct OperationStatus *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_3 = rread_1(&rread_1_arg, clnt);
	if (result_3 == (struct ReadResponse *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_4 = rwrite_1(&rwrite_1_arg, clnt);
	if (result_4 == (struct WriteResponse *) NULL) {
		clnt_perror (clnt, "call failed");
	}
*/

void clientInit(char* hostname) {
	clnt = clnt_create(hostname, SIMPLE_NFS, DEFAULT_SIGNUM, "udp");
	if(clnt == 0) {
		clnt_pcreateerror(hostname);
		exit(1);
	}
}

void clientFinish() {
	clnt_destroy(clnt);
}

int open(const char *pathname, int flags, mode_t mode) {
	if(strnlen(pathname, MAX_FILENAME_LEN) >= MAX_FILENAME_LEN) {
		errno = ENAMETOOLONG;	
		return -1;
	}
	char fileNameBuf[MAX_FILENAME_LEN];
	strcpy(fileNameBuf, pathname);

	struct OpenRequest openRequest = {.fileName = fileNameBuf, .flags = flags, .mode = mode};

	struct OperationStatus* remoteOpenStatus = ropen_1(&openRequest, clnt);
	if (remoteOpenStatus == (struct OperationStatus *) NULL) {
		clnt_perror(clnt, "call failed");
		errno = EACCES;

		return -1;
	}
	else if(remoteOpenStatus->returnValue < 0) {
		errno = remoteOpenStatus->error;
		return remoteOpenStatus->returnValue;
	}
	else {
		return reserveDescriptor(pathname, flags, mode);
	}
}

int creat(const char *pathname, mode_t mode) {
	return 0;
}

ssize_t read(int fd, void *buf, size_t count) {
	struct FileDescriptor* fileDescriptor = getDescriptor(fd);
	if(fileDescriptor == 0) {
		return -1;
	}

	struct FileAccessRequest request = {.offset = fileDescriptor->seekPos, .count = count, 
			.fileAttributes = {.fileName = fileDescriptor->fileName, .flags = fileDescriptor->flags, .mode = fileDescriptor->mode}};

	struct ReadResponse* response = rread_1(&request, clnt);

	if(response == NULL) {
		clnt_perror(clnt, "call failed");
		errno = EIO;
		return -1;
	}

	int bytesRead = response->status.returnValue;
	if(bytesRead < 0) {
		errno = response->status.error;
	}
	else {
		fileDescriptor->seekPos += bytesRead;
		memcpy(buf, response->content.content_val, response->content.content_len);
	}

	return bytesRead;
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
