struct OpenRequest {
	char* fileName;
	int flags;
	unsigned mode;
};

struct CreatRequest {
	char* fileName;
	unsigned mode;
};

struct FileAccessRequest {
	char* fileName; 
	int offset; 
	unsigned count;
};

struct OperationStatus {
	int returnValue;
	int errno;
};

struct ReadResponse {
	int bytesRead;
	opaque content<100>;
	struct OperationStatus;
};

struct WriteResponse {
	int bytesWritten;
	struct OperationStatus;
};

program SIMPLE_NFS {
	version DEFAULT_SIGNUM {
		struct OperationStatus rOpen(struct OpenRequest) = 1;
		struct OperationStatus rCreat(struct CreatRequest) = 2;
		struct ReadResponse rRead(struct FileAccessRequest) = 3;
		struct WriteResponse rWrite(struct FileAccessRequest) = 4;
	} = 1;
} = 0x20000001;
