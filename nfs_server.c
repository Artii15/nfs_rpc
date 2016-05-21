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

struct ReadResponse* rread_1_svc(struct FileAccessRequest *argp, struct svc_req *rqstp)
{
	static struct ReadResponse  result;

	/*
	 * insert server code here
	 */

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
