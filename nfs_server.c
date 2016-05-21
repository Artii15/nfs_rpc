#include "nfs.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

struct OperationStatus* ropen_1_svc(struct OpenRequest *request, struct svc_req *rqstp)
{
	printf("open %s, %d, %d\n", request->fileName, request->flags, request->mode);
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

struct ReadResponse* rread_1_svc(struct FileAccessRequest *request, struct svc_req *rqstp)
{
	static struct ReadResponse result;
	int fd = open(request->fileAttributes.fileName, request->fileAttributes.flags);

	if(fd < 0) {
		result.content.content_val = 0;
		result.content.content_len = 0;
		result.status.returnValue = fd;
		result.status.error = errno;

		return &result;
	}

	lseek(fd, request->offset, SEEK_SET);

	char buf[request->count];
	memset(buf, 0, request->count);

	result.content.content_val = buf;
	result.status.returnValue = read(fd, result.content.content_val, request->count);
	if(result.status.returnValue > 0) {
		result.content.content_len = result.status.returnValue;
	}
	else {
		result.status.error = errno;
	}

	printf("test %s\n", request->fileAttributes.fileName);

	close(fd);

	return &result;
}

struct WriteResponse* rwrite_1_svc(struct FileAccessRequest *argp, struct svc_req *rqstp)
{
	static struct WriteResponse  result;

	/*
	 * insert server code here
	 */

	return &result;
}
