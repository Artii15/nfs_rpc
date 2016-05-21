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
	int offset; 
	unsigned count;
	struct OpenRequest fileAttributes;
};

struct OperationStatus {
	int returnValue;
	int error;
};

struct ReadResponse {
	int bytesRead;
	opaque content<100>;
	struct OperationStatus status;
};

struct WriteResponse {
	int bytesWritten;
	struct OperationStatus status;
};

program SIMPLE_NFS {
	version DEFAULT_SIGNUM {
		struct OperationStatus rOpen(struct OpenRequest) = 1;
		struct OperationStatus rCreat(struct CreatRequest) = 2;
		struct ReadResponse rRead(struct FileAccessRequest) = 3;
		struct WriteResponse rWrite(struct FileAccessRequest) = 4;
	} = 1;
} = 0x20000001;
