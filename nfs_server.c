#include "nfs.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

struct OperationStatus* ropen_1_svc(struct OpenRequest *request, struct svc_req *rqstp)
{
	static struct OperationStatus result;
	result.returnValue = open(request->fileName, request->flags, request->mode);

	if(result.returnValue < 0) {
		result.error = errno;
	}
	else {
		result.error = 0;
		close(result.returnValue);
	}

	return &result;
}

struct OperationStatus* rcreat_1_svc(struct CreatRequest *argp, struct svc_req *rqstp)
{
	static struct OperationStatus  result;

	/*
	 * insert server code here
	 */

	return &result;
}

void cleanReadBuffer();
static short readBufferSize = 0;
static struct ReadResponse readResponse = {.content = {.content_val = 0, .content_len = 0}, .status = {.returnValue = 0, .error = 0}};
struct ReadResponse* rread_1_svc(struct FileAccessRequest *request, struct svc_req *rqstp) {
	int fd = open(request->fileAttributes.fileName, request->fileAttributes.flags);

	if(fd < 0) {
		readResponse.content.content_len = 0;
		readResponse.status.returnValue = fd;
		readResponse.status.error = errno;

		return &readResponse;
	}

	lseek(fd, request->offset, SEEK_SET);

	if(readBufferSize < request->count*sizeof(char)) {
		if(readBufferSize == 0) {
			readResponse.content.content_val = malloc(request->count*sizeof(char));
			atexit(cleanReadBuffer);
		}
		else {
			readResponse.content.content_val = realloc(readResponse.content.content_val, request->count*sizeof(char));
		}
		readBufferSize = request->count;
	}
	memset(readResponse.content.content_val, 0, readBufferSize);

	readResponse.status.returnValue = read(fd, readResponse.content.content_val, request->count);
	if(readResponse.status.returnValue > 0) {
		readResponse.content.content_len = readResponse.status.returnValue;
	}
	else {
		readResponse.status.error = errno;
		readResponse.content.content_len = 0;
	}

	close(fd);

	return &readResponse;
}

void cleanReadBuffer() {
	free(readResponse.content.content_val);
}

struct OperationStatus* rwrite_1_svc(struct FileAccessRequest *argp, struct svc_req *rqstp)
{
	static struct OperationStatus  result;

	/*
	 * insert server code here
	 */

	return &result;
}
