#include "client_interface.h"
#include "nfs.h"
#include "descriptors.h"
#include <errno.h>

static CLIENT* clnt = 0;

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
	return open(pathname, O_CREAT|O_WRONLY|O_TRUNC, mode);
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
	struct FileDescriptor* fileDescriptor = getDescriptor(fd);
	if(fileDescriptor == 0) {
		return -1;
	}

	char sendingBuf[count];
	memcpy(sendingBuf, buf, count);

	struct WriteRequest request = {
		.content = {.content_val = sendingBuf, .content_len = count}, 
		.requestAttributes = {
			.offset = fileDescriptor->seekPos, .count = count, .fileAttributes = {
				.fileName = fileDescriptor->fileName, 
				.flags = fileDescriptor->flags, 
				.mode = fileDescriptor->mode
			}
		}
	};

	struct OperationStatus* response = rwrite_1(&request, clnt);
	if(response == NULL) {
		clnt_perror(clnt, "call failed");
		errno = EIO;
		return -1;
	}

	int bytesWritten = response->returnValue;
	if(bytesWritten < 0) {
		errno = response->error;
	}
	else {
		fileDescriptor->seekPos += bytesWritten;
	}

	return bytesWritten;
}

off_t lseek(int fd, off_t offset, int whence) {
	struct FileDescriptor* fileDescriptor = getDescriptor(fd);
	if(fileDescriptor == 0) {
		return -1;
	}

	struct LseekRequest request = {
		.newOffset = offset,
		.oldOffset = fileDescriptor->seekPos,
		.whence = whence,
		.fileAttributes = {
			.fileName = fileDescriptor->fileName,
			.flags = fileDescriptor->flags,
			.mode = fileDescriptor->mode
		}
	};

	struct OperationStatus* response = rlseek_1(&request, clnt);
	if(response == NULL) {
		clnt_perror(clnt, "call failed");
		errno = EIO;
		return -1;
	}
	else if(response->returnValue == -1) {
		errno = response->error;		
		return -1;
	}
	else {
		fileDescriptor->seekPos = response->returnValue;
		return fileDescriptor->seekPos;
	}
}

int close(int fd) {
	releaseDescriptor(fd);	
	return 0;
}
