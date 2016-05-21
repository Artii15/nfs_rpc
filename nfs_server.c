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

struct ReadResponse* rread_1_svc(struct FileAccessRequest *request, struct svc_req *rqstp)
{
	static struct ReadResponse result;
	int fd = open(request->fileAttributes.fileName, request->fileAttributes.flags, request->fileAttributes.mode);

	if(fd < 0) {
		result.content = 0;
		result.status.returnValue = fd;
		result.status.error = errno;

		return &result;
	}

	lseek(fd, request->offset, SEEK_SET);

	char buf[request->count];
	memset(buf, 0, request->count);
	result.content = buf;

	result.status.returnValue = read(fd, result.content, request->count);
	result.status.error = errno;

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
